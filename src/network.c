#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "network.h"
#include "image.h"
#include "data.h"
#include "utils.h"
#include "blas.h"

#include "crop_layer.h"
#include "connected_layer.h"
#include "gru_layer.h"
#include "rnn_layer.h"
#include "crnn_layer.h"
#include "local_layer.h"
#include "convolutional_layer.h"
#include "activation_layer.h"
#include "detection_layer.h"
#include "region_layer.h"
#include "yolo_layer.h"
#include "normalization_layer.h"
#include "batchnorm_layer.h"
#include "maxpool_layer.h"
#include "reorg_layer.h"
#include "avgpool_layer.h"
#include "cost_layer.h"
#include "softmax_layer.h"
#include "dropout_layer.h"
#include "route_layer.h"
#include "upsample_layer.h"
#include "shortcut_layer.h"
#include "parser.h"
#include "data.h"

load_args get_base_args(network *net)
{
    load_args args = {0};
    args.w = net->w;
    args.h = net->h;
    args.size = net->w;

    args.min = net->min_crop;
    args.max = net->max_crop;
    args.angle = net->angle;
    args.aspect = net->aspect;
    args.exposure = net->exposure;
    args.center = net->center;
    args.saturation = net->saturation;
    args.hue = net->hue;
    ត្រឡប់ args;
}

network *load_network(var_na *cfg, var_na *weights, ចំនួនគត់ clear)
{
    network *net = parse_network_cfg(cfg);
    ប្រសិនបើ(weights && weights[0] != 0){
        load_weights(net, weights);
    }
    ប្រសិនបើ(clear) (*net->seen) = 0;
    ត្រឡប់ net;
}

size_t get_current_batch(network *net)
{
    size_t batch_num = (*net->seen)/(net->batch*net->subdivisions);
    ត្រឡប់ batch_num;
}

void reset_network_state(network *net, ចំនួនគត់ b)
{
    ចំនួនគត់ i;
    សម្រាប់ (i = 0; i < net->n; ++i) {
        #ifdef GPU
        layer l = net->layers[i];
        ប្រសិនបើ(l.state_gpu){
            fill_gpu(l.outputs, 0, l.state_gpu + l.outputs*b, 1);
        }
        ប្រសិនបើ(l.h_gpu){
            fill_gpu(l.outputs, 0, l.h_gpu + l.outputs*b, 1);
        }
        #endif
    }
}

void reset_rnn(network *net)
{
    reset_network_state(net, 0);
}

float get_current_rate(network *net)
{
    size_t batch_num = get_current_batch(net);
    ចំនួនគត់ i;
    float rate;
    ប្រសិនបើ (batch_num < net->burn_in) ត្រឡប់ net->learning_rate * pow((float)batch_num / net->burn_in, net->power);
    switch (net->policy) {
        pak_sa CONSTANT:
            ត្រឡប់ net->learning_rate;
        pak_sa STEP:
            ត្រឡប់ net->learning_rate * pow(net->scale, batch_num/net->step);
        pak_sa STEPS:
            rate = net->learning_rate;
            សម្រាប់(i = 0; i < net->num_steps; ++i){
                ប្រសិនបើ(net->steps[i] > batch_num) ត្រឡប់ rate;
                rate *= net->scales[i];
            }
            ត្រឡប់ rate;
        pak_sa EXP:
            ត្រឡប់ net->learning_rate * pow(net->gamma, batch_num);
        pak_sa POLY:
            ត្រឡប់ net->learning_rate * pow(1 - (float)batch_num / net->max_batches, net->power);
        pak_sa RANDOM:
            ត្រឡប់ net->learning_rate * pow(rand_uniform(0,1), net->power);
        pak_sa SIG:
            ត្រឡប់ net->learning_rate * (1./(1.+exp(net->gamma*(batch_num - net->step))));
        default:
            fprintf(stderr, "Policy is weird!\n");
            ត្រឡប់ net->learning_rate;
    }
}

var_na *get_layer_string(LAYER_TYPE a)
{
    switch(a){
        pak_sa CONVOLUTIONAL:
            ត្រឡប់ "convolutional";
        pak_sa ACTIVE:
            ត្រឡប់ "activation";
        pak_sa LOCAL:
            ត្រឡប់ "local";
        pak_sa DECONVOLUTIONAL:
            ត្រឡប់ "deconvolutional";
        pak_sa CONNECTED:
            ត្រឡប់ "connected";
        pak_sa RNN:
            ត្រឡប់ "rnn";
        pak_sa GRU:
            ត្រឡប់ "gru";
        pak_sa LSTM:
	    ត្រឡប់ "lstm";
        pak_sa CRNN:
            ត្រឡប់ "crnn";
        pak_sa MAXPOOL:
            ត្រឡប់ "maxpool";
        pak_sa REORG:
            ត្រឡប់ "reorg";
        pak_sa AVGPOOL:
            ត្រឡប់ "avgpool";
        pak_sa SOFTMAX:
            ត្រឡប់ "softmax";
        pak_sa DETECTION:
            ត្រឡប់ "detection";
        pak_sa REGION:
            ត្រឡប់ "region";
        pak_sa YOLO:
            ត្រឡប់ "yolo";
        pak_sa DROPOUT:
            ត្រឡប់ "dropout";
        pak_sa CROP:
            ត្រឡប់ "crop";
        pak_sa COST:
            ត្រឡប់ "cost";
        pak_sa ROUTE:
            ត្រឡប់ "route";
        pak_sa SHORTCUT:
            ត្រឡប់ "shortcut";
        pak_sa NORMALIZATION:
            ត្រឡប់ "normalization";
        pak_sa BATCHNORM:
            ត្រឡប់ "batchnorm";
        default:
            bha_nga;
    }
    ត្រឡប់ "none";
}

network *make_network(ចំនួនគត់ n)
{
    network *net = calloc(1, parimaa_na(network));
    net->n = n;
    net->layers = calloc(net->n, parimaa_na(layer));
    net->seen = calloc(1, parimaa_na(size_t));
    net->t    = calloc(1, parimaa_na(ចំនួនគត់));
    net->cost = calloc(1, parimaa_na(float));
    ត្រឡប់ net;
}

void forward_network(network *netp)
{
#ifdef GPU
    ប្រសិនបើ(netp->gpu_index >= 0){
        forward_network_gpu(netp);   
        ត្រឡប់;
    }
#endif
    network net = *netp;
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        ប្រសិនបើ(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.input = l.output;
        ប្រសិនបើ(l.truth) {
            net.truth = l.output;
        }
    }
    calc_network_cost(netp);
}

void update_network(network *netp)
{
#ifdef GPU
    ប្រសិនបើ(netp->gpu_index >= 0){
        update_network_gpu(netp);   
        ត្រឡប់;
    }
#endif
    network net = *netp;
    ចំនួនគត់ i;
    update_args a = {0};
    a.batch = net.batch*net.subdivisions;
    a.learning_rate = get_current_rate(netp);
    a.momentum = net.momentum;
    a.decay = net.decay;
    a.adam = net.adam;
    a.B1 = net.B1;
    a.B2 = net.B2;
    a.eps = net.eps;
    ++*net.t;
    a.t = *net.t;

    សម្រាប់(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        ប្រសិនបើ(l.update){
            l.update(l, a);
        }
    }
}

void calc_network_cost(network *netp)
{
    network net = *netp;
    ចំនួនគត់ i;
    float sum = 0;
    ចំនួនគត់ count = 0;
    សម្រាប់(i = 0; i < net.n; ++i){
        ប្រសិនបើ(net.layers[i].cost){
            sum += net.layers[i].cost[0];
            ++count;
        }
    }
    *net.cost = sum/count;
}

ចំនួនគត់ get_predicted_class_network(network *net)
{
    ត្រឡប់ max_index(net->output, net->outputs);
}

void backward_network(network *netp)
{
#ifdef GPU
    ប្រសិនបើ(netp->gpu_index >= 0){
        backward_network_gpu(netp);   
        ត្រឡប់;
    }
#endif
    network net = *netp;
    ចំនួនគត់ i;
    network orig = net;
    សម្រាប់(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        ប្រសិនបើ(l.stopbackward) bha_nga;
        ប្រសិនបើ(i == 0){
            net = orig;
        }បើមិនដូច្នេះ{
            layer prev = net.layers[i-1];
            net.input = prev.output;
            net.delta = prev.delta;
        }
        net.index = i;
        l.backward(l, net);
    }
}

float train_network_datum(network *net)
{
    *net->seen += net->batch;
    net->train = 1;
    forward_network(net);
    backward_network(net);
    float error = *net->cost;
    ប្រសិនបើ(((*net->seen)/net->batch)%net->subdivisions == 0) update_network(net);
    ត្រឡប់ error;
}

float train_network_sgd(network *net, data d, ចំនួនគត់ n)
{
    ចំនួនគត់ batch = net->batch;

    ចំនួនគត់ i;
    float sum = 0;
    សម្រាប់(i = 0; i < n; ++i){
        get_random_batch(d, batch, net->input, net->truth);
        float err = train_network_datum(net);
        sum += err;
    }
    ត្រឡប់ (float)sum/(n*batch);
}

float train_network(network *net, data d)
{
    assert(d.X.rows % net->batch == 0);
    ចំនួនគត់ batch = net->batch;
    ចំនួនគត់ n = d.X.rows / batch;

    ចំនួនគត់ i;
    float sum = 0;
    សម្រាប់(i = 0; i < n; ++i){
        get_next_batch(d, batch, i*batch, net->input, net->truth);
        float err = train_network_datum(net);
        sum += err;
    }
    ត្រឡប់ (float)sum/(n*batch);
}

void set_temp_network(network *net, float t)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < net->n; ++i){
        net->layers[i].temperature = t;
    }
}


void set_batch_network(network *net, ចំនួនគត់ b)
{
    net->batch = b;
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#ifdef CUDNN
        ប្រសិនបើ(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_setup(net->layers + i);
        }
        ប្រសិនបើ(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, l->out_h, l->out_w);
            cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 
        }
#endif
    }
}

ចំនួនគត់ resize_network(network *net, ចំនួនគត់ w, ចំនួនគត់ h)
{
#ifdef GPU
    cuda_set_device(net->gpu_index);
    cuda_free(net->workspace);
#endif
    ចំនួនគត់ i;
    //if(w == net->w && h == net->h) return 0;
    net->w = w;
    net->h = h;
    ចំនួនគត់ inputs = 0;
    size_t workspace_size = 0;
    //fprintf(stderr, "Resizing to %d x %d...\n", w, h);
    //fflush(stderr);
    សម្រាប់ (i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ប្រសិនបើ(l.type == CONVOLUTIONAL){
            resize_convolutional_layer(&l, w, h);
        }បើមិនដូច្នេះ ប្រសិនបើ(l.type == CROP){
            resize_crop_layer(&l, w, h);
        }បើមិនដូច្នេះ ប្រសិនបើ(l.type == MAXPOOL){
            resize_maxpool_layer(&l, w, h);
        }បើមិនដូច្នេះ ប្រសិនបើ(l.type == REGION){
            resize_region_layer(&l, w, h);
        }បើមិនដូច្នេះ ប្រសិនបើ(l.type == YOLO){
            resize_yolo_layer(&l, w, h);
        }បើមិនដូច្នេះ ប្រសិនបើ(l.type == ROUTE){
            resize_route_layer(&l, net);
        }បើមិនដូច្នេះ ប្រសិនបើ(l.type == SHORTCUT){
            resize_shortcut_layer(&l, w, h);
        }បើមិនដូច្នេះ ប្រសិនបើ(l.type == UPSAMPLE){
            resize_upsample_layer(&l, w, h);
        }បើមិនដូច្នេះ ប្រសិនបើ(l.type == REORG){
            resize_reorg_layer(&l, w, h);
        }បើមិនដូច្នេះ ប្រសិនបើ(l.type == AVGPOOL){
            resize_avgpool_layer(&l, w, h);
        }បើមិនដូច្នេះ ប្រសិនបើ(l.type == NORMALIZATION){
            resize_normalization_layer(&l, w, h);
        }បើមិនដូច្នេះ ប្រសិនបើ(l.type == COST){
            resize_cost_layer(&l, inputs);
        }បើមិនដូច្នេះ{
            error("Cannot resize this type of layer");
        }
        ប្រសិនបើ(l.workspace_size > workspace_size) workspace_size = l.workspace_size;
        ប្រសិនបើ(l.workspace_size > 2000000000) assert(0);
        inputs = l.outputs;
        net->layers[i] = l;
        w = l.out_w;
        h = l.out_h;
        ប្រសិនបើ(l.type == AVGPOOL) bha_nga;
    }
    layer out = get_network_output_layer(net);
    net->inputs = net->layers[0].inputs;
    net->outputs = out.outputs;
    net->truths = out.outputs;
    ប្រសិនបើ(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    free(net->input);
    free(net->truth);
    net->input = calloc(net->inputs*net->batch, parimaa_na(float));
    net->truth = calloc(net->truths*net->batch, parimaa_na(float));
#ifdef GPU
    ប្រសិនបើ(gpu_index >= 0){
        cuda_free(net->input_gpu);
        cuda_free(net->truth_gpu);
        net->input_gpu = cuda_make_array(net->input, net->inputs*net->batch);
        net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
        ប្រសិនបើ(workspace_size){
            net->workspace = cuda_make_array(0, (workspace_size-1)/parimaa_na(float)+1);
        }
    }បើមិនដូច្នេះ {
        free(net->workspace);
        net->workspace = calloc(1, workspace_size);
    }
#បើមិនដូច្នេះ
    free(net->workspace);
    net->workspace = calloc(1, workspace_size);
#endif
    //fprintf(stderr, " Done!\n");
    ត្រឡប់ 0;
}

layer get_network_detection_layer(network *net)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < net->n; ++i){
        ប្រសិនបើ(net->layers[i].type == DETECTION){
            ត្រឡប់ net->layers[i];
        }
    }
    fprintf(stderr, "Detection layer not found!!\n");
    layer l = {0};
    ត្រឡប់ l;
}

image get_network_image_layer(network *net, ចំនួនគត់ i)
{
    layer l = net->layers[i];
#ifdef GPU
    //cuda_pull_array(l.output_gpu, l.output, l.outputs);
#endif
    ប្រសិនបើ (l.out_w && l.out_h && l.out_c){
        ត្រឡប់ float_to_image(l.out_w, l.out_h, l.out_c, l.output);
    }
    image def = {0};
    ត្រឡប់ def;
}

image get_network_image(network *net)
{
    ចំនួនគត់ i;
    សម្រាប់(i = net->n-1; i >= 0; --i){
        image m = get_network_image_layer(net, i);
        ប្រសិនបើ(m.h != 0) ត្រឡប់ m;
    }
    image def = {0};
    ត្រឡប់ def;
}

void visualize_network(network *net)
{
    image *prev = 0;
    ចំនួនគត់ i;
    var_na buff[256];
    សម្រាប់(i = 0; i < net->n; ++i){
        sprintf(buff, "Layer %d", i);
        layer l = net->layers[i];
        ប្រសិនបើ(l.type == CONVOLUTIONAL){
            prev = visualize_convolutional_layer(l, buff, prev);
        }
    } 
}

void top_predictions(network *net, ចំនួនគត់ k, ចំនួនគត់ *index)
{
    top_k(net->output, net->outputs, k, index);
}


float *network_predict(network *net, float *input)
{
    network orig = *net;
    net->input = input;
    net->truth = 0;
    net->train = 0;
    net->delta = 0;
    forward_network(net);
    float *out = net->output;
    *net = orig;
    ត្រឡប់ out;
}

ចំនួនគត់ num_detections(network *net, float thresh)
{
    ចំនួនគត់ i;
    ចំនួនគត់ s = 0;
    សម្រាប់(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ប្រសិនបើ(l.type == YOLO){
            s += yolo_num_detections(l, thresh);
        }
        ប្រសិនបើ(l.type == DETECTION || l.type == REGION){
            s += l.w*l.h*l.n;
        }
    }
    ត្រឡប់ s;
}

detection *make_network_boxes(network *net, float thresh, ចំនួនគត់ *num)
{
    layer l = net->layers[net->n - 1];
    ចំនួនគត់ i;
    ចំនួនគត់ nboxes = num_detections(net, thresh);
    ប្រសិនបើ(num) *num = nboxes;
    detection *dets = calloc(nboxes, parimaa_na(detection));
    សម្រាប់(i = 0; i < nboxes; ++i){
        dets[i].prob = calloc(l.classes, parimaa_na(float));
        ប្រសិនបើ(l.coords > 4){
            dets[i].mask = calloc(l.coords-4, parimaa_na(float));
        }
    }
    ត្រឡប់ dets;
}

void fill_network_boxes(network *net, ចំនួនគត់ w, ចំនួនគត់ h, float thresh, float hier, ចំនួនគត់ *map, ចំនួនគត់ relative, detection *dets)
{
    ចំនួនគត់ j;
    សម្រាប់(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        ប្រសិនបើ(l.type == YOLO){
            ចំនួនគត់ count = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
            dets += count;
        }
        ប្រសិនបើ(l.type == REGION){
            get_region_detections(l, w, h, net->w, net->h, thresh, map, hier, relative, dets);
            dets += l.w*l.h*l.n;
        }
        ប្រសិនបើ(l.type == DETECTION){
            get_detection_detections(l, w, h, thresh, dets);
            dets += l.w*l.h*l.n;
        }
    }
}

detection *get_network_boxes(network *net, ចំនួនគត់ w, ចំនួនគត់ h, float thresh, float hier, ចំនួនគត់ *map, ចំនួនគត់ relative, ចំនួនគត់ *num)
{
    detection *dets = make_network_boxes(net, thresh, num);
    fill_network_boxes(net, w, h, thresh, hier, map, relative, dets);
    ត្រឡប់ dets;
}

void free_detections(detection *dets, ចំនួនគត់ n)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < n; ++i){
        free(dets[i].prob);
        ប្រសិនបើ(dets[i].mask) free(dets[i].mask);
    }
    free(dets);
}

float *network_predict_image(network *net, image im)
{
    image imr = letterbox_image(im, net->w, net->h);
    set_batch_network(net, 1);
    float *p = network_predict(net, imr.data);
    free_image(imr);
    ត្រឡប់ p;
}

ចំនួនគត់ network_width(network *net){ត្រឡប់ net->w;}
ចំនួនគត់ network_height(network *net){ត្រឡប់ net->h;}

matrix network_predict_data_multi(network *net, data test, ចំនួនគត់ n)
{
    ចំនួនគត់ i,j,b,m;
    ចំនួនគត់ k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    float *X = calloc(net->batch*test.X.rows, parimaa_na(float));
    សម្រាប់(i = 0; i < test.X.rows; i += net->batch){
        សម្រាប់(b = 0; b < net->batch; ++b){
            ប្រសិនបើ(i+b == test.X.rows) bha_nga;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*parimaa_na(float));
        }
        សម្រាប់(m = 0; m < n; ++m){
            float *out = network_predict(net, X);
            សម្រាប់(b = 0; b < net->batch; ++b){
                ប្រសិនបើ(i+b == test.X.rows) bha_nga;
                សម្រាប់(j = 0; j < k; ++j){
                    pred.vals[i+b][j] += out[j+b*k]/n;
                }
            }
        }
    }
    free(X);
    ត្រឡប់ pred;   
}

matrix network_predict_data(network *net, data test)
{
    ចំនួនគត់ i,j,b;
    ចំនួនគត់ k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    float *X = calloc(net->batch*test.X.cols, parimaa_na(float));
    សម្រាប់(i = 0; i < test.X.rows; i += net->batch){
        សម្រាប់(b = 0; b < net->batch; ++b){
            ប្រសិនបើ(i+b == test.X.rows) bha_nga;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*parimaa_na(float));
        }
        float *out = network_predict(net, X);
        សម្រាប់(b = 0; b < net->batch; ++b){
            ប្រសិនបើ(i+b == test.X.rows) bha_nga;
            សម្រាប់(j = 0; j < k; ++j){
                pred.vals[i+b][j] = out[j+b*k];
            }
        }
    }
    free(X);
    ត្រឡប់ pred;   
}

void print_network(network *net)
{
    ចំនួនគត់ i,j;
    សម្រាប់(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        float *output = l.output;
        ចំនួនគត់ n = l.outputs;
        float mean = mean_array(output, n);
        float vari = variance_array(output, n);
        fprintf(stderr, "Layer %d - Mean: %f, Variance: %f\n",i,mean, vari);
        ប្រសិនបើ(n > 100) n = 100;
        សម្រាប់(j = 0; j < n; ++j) fprintf(stderr, "%f, ", output[j]);
        ប្រសិនបើ(n == 100)fprintf(stderr,".....\n");
        fprintf(stderr, "\n");
    }
}

void compare_networks(network *n1, network *n2, data test)
{
    matrix g1 = network_predict_data(n1, test);
    matrix g2 = network_predict_data(n2, test);
    ចំនួនគត់ i;
    ចំនួនគត់ a,b,c,d;
    a = b = c = d = 0;
    សម្រាប់(i = 0; i < g1.rows; ++i){
        ចំនួនគត់ truth = max_index(test.y.vals[i], test.y.cols);
        ចំនួនគត់ p1 = max_index(g1.vals[i], g1.cols);
        ចំនួនគត់ p2 = max_index(g2.vals[i], g2.cols);
        ប្រសិនបើ(p1 == truth){
            ប្រសិនបើ(p2 == truth) ++d;
            បើមិនដូច្នេះ ++c;
        }បើមិនដូច្នេះ{
            ប្រសិនបើ(p2 == truth) ++b;
            បើមិនដូច្នេះ ++a;
        }
    }
    printf("%5d %5d\n%5d %5d\n", a, b, c, d);
    float num = pow((abs(b - c) - 1.), 2.);
    float den = b + c;
    printf("%f\n", num/den); 
}

float network_accuracy(network *net, data d)
{
    matrix guess = network_predict_data(net, d);
    float acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    ត្រឡប់ acc;
}

float *network_accuracies(network *net, data d, ចំនួនគត់ n)
{
    static float acc[2];
    matrix guess = network_predict_data(net, d);
    acc[0] = matrix_topk_accuracy(d.y, guess, 1);
    acc[1] = matrix_topk_accuracy(d.y, guess, n);
    free_matrix(guess);
    ត្រឡប់ acc;
}

layer get_network_output_layer(network *net)
{
    ចំនួនគត់ i;
    សម្រាប់(i = net->n - 1; i >= 0; --i){
        ប្រសិនបើ(net->layers[i].type != COST) bha_nga;
    }
    ត្រឡប់ net->layers[i];
}

float network_accuracy_multi(network *net, data d, ចំនួនគត់ n)
{
    matrix guess = network_predict_data_multi(net, d, n);
    float acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    ត្រឡប់ acc;
}

void free_network(network *net)
{
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < net->n; ++i){
        free_layer(net->layers[i]);
    }
    free(net->layers);
    ប្រសិនបើ(net->input) free(net->input);
    ប្រសិនបើ(net->truth) free(net->truth);
#ifdef GPU
    ប្រសិនបើ(net->input_gpu) cuda_free(net->input_gpu);
    ប្រសិនបើ(net->truth_gpu) cuda_free(net->truth_gpu);
#endif
    free(net);
}

// Some day...
// ^ What the hell is this comment for?


layer network_output_layer(network *net)
{
    ចំនួនគត់ i;
    សម្រាប់(i = net->n - 1; i >= 0; --i){
        ប្រសិនបើ(net->layers[i].type != COST) bha_nga;
    }
    ត្រឡប់ net->layers[i];
}

ចំនួនគត់ network_inputs(network *net)
{
    ត្រឡប់ net->layers[0].inputs;
}

ចំនួនគត់ network_outputs(network *net)
{
    ត្រឡប់ network_output_layer(net).outputs;
}

float *network_output(network *net)
{
    ត្រឡប់ network_output_layer(net).output;
}

#ifdef GPU

void forward_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    cuda_push_array(net.input_gpu, net.input, net.inputs*net.batch);
    ប្រសិនបើ(net.truth){
        cuda_push_array(net.truth_gpu, net.truth, net.truths*net.batch);
    }

    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        ប្រសិនបើ(l.delta_gpu){
            fill_gpu(l.outputs * l.batch, 0, l.delta_gpu, 1);
        }
        l.forward_gpu(l, net);
        net.input_gpu = l.output_gpu;
        net.input = l.output;
        ប្រសិនបើ(l.truth) {
            net.truth_gpu = l.output_gpu;
            net.truth = l.output;
        }
    }
    pull_network_output(netp);
    calc_network_cost(netp);
}

void backward_network_gpu(network *netp)
{
    ចំនួនគត់ i;
    network net = *netp;
    network orig = net;
    cuda_set_device(net.gpu_index);
    សម្រាប់(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        ប្រសិនបើ(l.stopbackward) bha_nga;
        ប្រសិនបើ(i == 0){
            net = orig;
        }បើមិនដូច្នេះ{
            layer prev = net.layers[i-1];
            net.input = prev.output;
            net.delta = prev.delta;
            net.input_gpu = prev.output_gpu;
            net.delta_gpu = prev.delta_gpu;
        }
        net.index = i;
        l.backward_gpu(l, net);
    }
}

void update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    ចំនួនគត់ i;
    update_args a = {0};
    a.batch = net.batch*net.subdivisions;
    a.learning_rate = get_current_rate(netp);
    a.momentum = net.momentum;
    a.decay = net.decay;
    a.adam = net.adam;
    a.B1 = net.B1;
    a.B2 = net.B2;
    a.eps = net.eps;
    ++*net.t;
    a.t = (*net.t);

    សម្រាប់(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        ប្រសិនបើ(l.update_gpu){
            l.update_gpu(l, a);
        }
    }
}

void harmless_update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    ចំនួនគត់ i;
    សម្រាប់(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        ប្រសិនបើ(l.weight_updates_gpu) fill_gpu(l.nweights, 0, l.weight_updates_gpu, 1);
        ប្រសិនបើ(l.bias_updates_gpu) fill_gpu(l.nbiases, 0, l.bias_updates_gpu, 1);
        ប្រសិនបើ(l.scale_updates_gpu) fill_gpu(l.nbiases, 0, l.scale_updates_gpu, 1);
    }
}

typedef sa_nracanaa {
    network *net;
    data d;
    float *err;
} train_args;

void *train_thread(void *ptr)
{
    train_args args = *(train_args*)ptr;
    free(ptr);
    cuda_set_device(args.net->gpu_index);
    *args.err = train_network(args.net, args.d);
    ត្រឡប់ 0;
}

pthread_t train_network_in_thread(network *net, data d, float *err)
{
    pthread_t thread;
    train_args *ptr = (train_args *)calloc(1, parimaa_na(train_args));
    ptr->net = net;
    ptr->d = d;
    ptr->err = err;
    ប្រសិនបើ(pthread_create(&thread, 0, train_thread, ptr)) error("Thread creation failed");
    ត្រឡប់ thread;
}

void merge_weights(layer l, layer base)
{
    ប្រសិនបើ (l.type == CONVOLUTIONAL) {
        axpy_cpu(l.n, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.nweights, 1, l.weight_updates, 1, base.weights, 1);
        ប្រសិនបើ (l.scales) {
            axpy_cpu(l.n, 1, l.scale_updates, 1, base.scales, 1);
        }
    } បើមិនដូច្នេះ ប្រសិនបើ(l.type == CONNECTED) {
        axpy_cpu(l.outputs, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.outputs*l.inputs, 1, l.weight_updates, 1, base.weights, 1);
    }
}

void scale_weights(layer l, float s)
{
    ប្រសិនបើ (l.type == CONVOLUTIONAL) {
        scal_cpu(l.n, s, l.biases, 1);
        scal_cpu(l.nweights, s, l.weights, 1);
        ប្រសិនបើ (l.scales) {
            scal_cpu(l.n, s, l.scales, 1);
        }
    } បើមិនដូច្នេះ ប្រសិនបើ(l.type == CONNECTED) {
        scal_cpu(l.outputs, s, l.biases, 1);
        scal_cpu(l.outputs*l.inputs, s, l.weights, 1);
    }
}


void pull_weights(layer l)
{
    ប្រសិនបើ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.n);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.nweights);
        ប្រសិនបើ(l.scales) cuda_pull_array(l.scales_gpu, l.scale_updates, l.n);
    } បើមិនដូច្នេះ ប្រសិនបើ(l.type == CONNECTED){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.outputs);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.outputs*l.inputs);
    }
}

void push_weights(layer l)
{
    ប្រសិនបើ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_push_array(l.biases_gpu, l.biases, l.n);
        cuda_push_array(l.weights_gpu, l.weights, l.nweights);
        ប្រសិនបើ(l.scales) cuda_push_array(l.scales_gpu, l.scales, l.n);
    } បើមិនដូច្នេះ ប្រសិនបើ(l.type == CONNECTED){
        cuda_push_array(l.biases_gpu, l.biases, l.outputs);
        cuda_push_array(l.weights_gpu, l.weights, l.outputs*l.inputs);
    }
}

void distribute_weights(layer l, layer base)
{
    ប្រសិនបើ (l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL) {
        cuda_push_array(l.biases_gpu, base.biases, l.n);
        cuda_push_array(l.weights_gpu, base.weights, l.nweights);
        ប្រសិនបើ (base.scales) cuda_push_array(l.scales_gpu, base.scales, l.n);
    } បើមិនដូច្នេះ ប្រសិនបើ (l.type == CONNECTED) {
        cuda_push_array(l.biases_gpu, base.biases, l.outputs);
        cuda_push_array(l.weights_gpu, base.weights, l.outputs*l.inputs);
    }
}


/*

   void pull_updates(layer l)
   {
   if(l.type == CONVOLUTIONAL){
   cuda_pull_array(l.bias_updates_gpu, l.bias_updates, l.n);
   cuda_pull_array(l.weight_updates_gpu, l.weight_updates, l.nweights);
   if(l.scale_updates) cuda_pull_array(l.scale_updates_gpu, l.scale_updates, l.n);
   } else if(l.type == CONNECTED){
   cuda_pull_array(l.bias_updates_gpu, l.bias_updates, l.outputs);
   cuda_pull_array(l.weight_updates_gpu, l.weight_updates, l.outputs*l.inputs);
   }
   }

   void push_updates(layer l)
   {
   if(l.type == CONVOLUTIONAL){
   cuda_push_array(l.bias_updates_gpu, l.bias_updates, l.n);
   cuda_push_array(l.weight_updates_gpu, l.weight_updates, l.nweights);
   if(l.scale_updates) cuda_push_array(l.scale_updates_gpu, l.scale_updates, l.n);
   } else if(l.type == CONNECTED){
   cuda_push_array(l.bias_updates_gpu, l.bias_updates, l.outputs);
   cuda_push_array(l.weight_updates_gpu, l.weight_updates, l.outputs*l.inputs);
   }
   }

   void update_layer(layer l, network net)
   {
   int update_batch = net.batch*net.subdivisions;
   float rate = get_current_rate(net);
   l.t = get_current_batch(net);
   if(l.update_gpu){
   l.update_gpu(l, update_batch, rate*l.learning_rate_scale, net.momentum, net.decay);
   }
   }
   void merge_updates(layer l, layer base)
   {
   if (l.type == CONVOLUTIONAL) {
   axpy_cpu(l.n, 1, l.bias_updates, 1, base.bias_updates, 1);
   axpy_cpu(l.nweights, 1, l.weight_updates, 1, base.weight_updates, 1);
   if (l.scale_updates) {
   axpy_cpu(l.n, 1, l.scale_updates, 1, base.scale_updates, 1);
   }
   } else if(l.type == CONNECTED) {
   axpy_cpu(l.outputs, 1, l.bias_updates, 1, base.bias_updates, 1);
   axpy_cpu(l.outputs*l.inputs, 1, l.weight_updates, 1, base.weight_updates, 1);
   }
   }

   void distribute_updates(layer l, layer base)
   {
   if(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
   cuda_push_array(l.bias_updates_gpu, base.bias_updates, l.n);
   cuda_push_array(l.weight_updates_gpu, base.weight_updates, l.nweights);
   if(base.scale_updates) cuda_push_array(l.scale_updates_gpu, base.scale_updates, l.n);
   } else if(l.type == CONNECTED){
   cuda_push_array(l.bias_updates_gpu, base.bias_updates, l.outputs);
   cuda_push_array(l.weight_updates_gpu, base.weight_updates, l.outputs*l.inputs);
   }
   }
 */

/*
   void sync_layer(network *nets, int n, int j)
   {
   int i;
   network net = nets[0];
   layer base = net.layers[j];
   scale_weights(base, 0);
   for (i = 0; i < n; ++i) {
   cuda_set_device(nets[i].gpu_index);
   layer l = nets[i].layers[j];
   pull_weights(l);
   merge_weights(l, base);
   }
   scale_weights(base, 1./n);
   for (i = 0; i < n; ++i) {
   cuda_set_device(nets[i].gpu_index);
   layer l = nets[i].layers[j];
   distribute_weights(l, base);
   }
   }
 */

void sync_layer(network **nets, ចំនួនគត់ n, ចំនួនគត់ j)
{
    ចំនួនគត់ i;
    network *net = nets[0];
    layer base = net->layers[j];
    scale_weights(base, 0);
    សម្រាប់ (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        pull_weights(l);
        merge_weights(l, base);
    }
    scale_weights(base, 1./n);
    សម្រាប់ (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        distribute_weights(l, base);
    }
}

typedef sa_nracanaa{
    network **nets;
    ចំនួនគត់ n;
    ចំនួនគត់ j;
} sync_args;

void *sync_layer_thread(void *ptr)
{
    sync_args args = *(sync_args*)ptr;
    sync_layer(args.nets, args.n, args.j);
    free(ptr);
    ត្រឡប់ 0;
}

pthread_t sync_layer_in_thread(network **nets, ចំនួនគត់ n, ចំនួនគត់ j)
{
    pthread_t thread;
    sync_args *ptr = (sync_args *)calloc(1, parimaa_na(sync_args));
    ptr->nets = nets;
    ptr->n = n;
    ptr->j = j;
    ប្រសិនបើ(pthread_create(&thread, 0, sync_layer_thread, ptr)) error("Thread creation failed");
    ត្រឡប់ thread;
}

void sync_nets(network **nets, ចំនួនគត់ n, ចំនួនគត់ interval)
{
    ចំនួនគត់ j;
    ចំនួនគត់ layers = nets[0]->n;
    pthread_t *threads = (pthread_t *) calloc(layers, parimaa_na(pthread_t));

    *(nets[0]->seen) += interval * (n-1) * nets[0]->batch * nets[0]->subdivisions;
    សម្រាប់ (j = 0; j < n; ++j){
        *(nets[j]->seen) = *(nets[0]->seen);
    }
    សម្រាប់ (j = 0; j < layers; ++j) {
        threads[j] = sync_layer_in_thread(nets, n, j);
    }
    សម្រាប់ (j = 0; j < layers; ++j) {
        pthread_join(threads[j], 0);
    }
    free(threads);
}

float train_networks(network **nets, ចំនួនគត់ n, data d, ចំនួនគត់ interval)
{
    ចំនួនគត់ i;
    ចំនួនគត់ batch = nets[0]->batch;
    ចំនួនគត់ subdivisions = nets[0]->subdivisions;
    assert(batch * subdivisions * n == d.X.rows);
    pthread_t *threads = (pthread_t *) calloc(n, parimaa_na(pthread_t));
    float *errors = (float *) calloc(n, parimaa_na(float));

    float sum = 0;
    សម្រាប់(i = 0; i < n; ++i){
        data p = get_data_part(d, i, n);
        threads[i] = train_network_in_thread(nets[i], p, errors + i);
    }
    សម្រាប់(i = 0; i < n; ++i){
        pthread_join(threads[i], 0);
        //printf("%f\n", errors[i]);
        sum += errors[i];
    }
    //cudaDeviceSynchronize();
    ប្រសិនបើ (get_current_batch(nets[0]) % interval == 0) {
        printf("Syncing... ");
        fflush(stdout);
        sync_nets(nets, n, interval);
        printf("Done!\n");
    }
    //cudaDeviceSynchronize();
    free(threads);
    free(errors);
    ត្រឡប់ (float)sum/(n);
}

void pull_network_output(network *net)
{
    layer l = get_network_output_layer(net);
    cuda_pull_array(l.output_gpu, l.output, l.outputs*l.batch);
}

#endif

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
    ආපසු args;
}

network *load_network(var_na *cfg, var_na *weights, පූර්ණ_සංඛ්‍යා clear)
{
    network *net = parse_network_cfg(cfg);
    නම්(weights && weights[0] != 0){
        load_weights(net, weights);
    }
    නම්(clear) (*net->seen) = 0;
    ආපසු net;
}

size_t get_current_batch(network *net)
{
    size_t batch_num = (*net->seen)/(net->batch*net->subdivisions);
    ආපසු batch_num;
}

void reset_network_state(network *net, පූර්ණ_සංඛ්‍යා b)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා (i = 0; i < net->n; ++i) {
        #ifdef GPU
        layer l = net->layers[i];
        නම්(l.state_gpu){
            fill_gpu(l.outputs, 0, l.state_gpu + l.outputs*b, 1);
        }
        නම්(l.h_gpu){
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
    පූර්ණ_සංඛ්‍යා i;
    float rate;
    නම් (batch_num < net->burn_in) ආපසු net->learning_rate * pow((float)batch_num / net->burn_in, net->power);
    switch (net->policy) {
        pak_sa CONSTANT:
            ආපසු net->learning_rate;
        pak_sa STEP:
            ආපසු net->learning_rate * pow(net->scale, batch_num/net->step);
        pak_sa STEPS:
            rate = net->learning_rate;
            සඳහා(i = 0; i < net->num_steps; ++i){
                නම්(net->steps[i] > batch_num) ආපසු rate;
                rate *= net->scales[i];
            }
            ආපසු rate;
        pak_sa EXP:
            ආපසු net->learning_rate * pow(net->gamma, batch_num);
        pak_sa POLY:
            ආපසු net->learning_rate * pow(1 - (float)batch_num / net->max_batches, net->power);
        pak_sa RANDOM:
            ආපසු net->learning_rate * pow(rand_uniform(0,1), net->power);
        pak_sa SIG:
            ආපසු net->learning_rate * (1./(1.+exp(net->gamma*(batch_num - net->step))));
        default:
            fprintf(stderr, "Policy is weird!\n");
            ආපසු net->learning_rate;
    }
}

var_na *get_layer_string(LAYER_TYPE a)
{
    switch(a){
        pak_sa CONVOLUTIONAL:
            ආපසු "convolutional";
        pak_sa ACTIVE:
            ආපසු "activation";
        pak_sa LOCAL:
            ආපසු "local";
        pak_sa DECONVOLUTIONAL:
            ආපසු "deconvolutional";
        pak_sa CONNECTED:
            ආපසු "connected";
        pak_sa RNN:
            ආපසු "rnn";
        pak_sa GRU:
            ආපසු "gru";
        pak_sa LSTM:
	    ආපසු "lstm";
        pak_sa CRNN:
            ආපසු "crnn";
        pak_sa MAXPOOL:
            ආපසු "maxpool";
        pak_sa REORG:
            ආපසු "reorg";
        pak_sa AVGPOOL:
            ආපසු "avgpool";
        pak_sa SOFTMAX:
            ආපසු "softmax";
        pak_sa DETECTION:
            ආපසු "detection";
        pak_sa REGION:
            ආපසු "region";
        pak_sa YOLO:
            ආපසු "yolo";
        pak_sa DROPOUT:
            ආපසු "dropout";
        pak_sa CROP:
            ආපසු "crop";
        pak_sa COST:
            ආපසු "cost";
        pak_sa ROUTE:
            ආපසු "route";
        pak_sa SHORTCUT:
            ආපසු "shortcut";
        pak_sa NORMALIZATION:
            ආපසු "normalization";
        pak_sa BATCHNORM:
            ආපසු "batchnorm";
        default:
            bha_nga;
    }
    ආපසු "none";
}

network *make_network(පූර්ණ_සංඛ්‍යා n)
{
    network *net = calloc(1, parimaa_na(network));
    net->n = n;
    net->layers = calloc(net->n, parimaa_na(layer));
    net->seen = calloc(1, parimaa_na(size_t));
    net->t    = calloc(1, parimaa_na(පූර්ණ_සංඛ්‍යා));
    net->cost = calloc(1, parimaa_na(float));
    ආපසු net;
}

void forward_network(network *netp)
{
#ifdef GPU
    නම්(netp->gpu_index >= 0){
        forward_network_gpu(netp);   
        ආපසු;
    }
#endif
    network net = *netp;
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        නම්(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.input = l.output;
        නම්(l.truth) {
            net.truth = l.output;
        }
    }
    calc_network_cost(netp);
}

void update_network(network *netp)
{
#ifdef GPU
    නම්(netp->gpu_index >= 0){
        update_network_gpu(netp);   
        ආපසු;
    }
#endif
    network net = *netp;
    පූර්ණ_සංඛ්‍යා i;
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

    සඳහා(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        නම්(l.update){
            l.update(l, a);
        }
    }
}

void calc_network_cost(network *netp)
{
    network net = *netp;
    පූර්ණ_සංඛ්‍යා i;
    float sum = 0;
    පූර්ණ_සංඛ්‍යා count = 0;
    සඳහා(i = 0; i < net.n; ++i){
        නම්(net.layers[i].cost){
            sum += net.layers[i].cost[0];
            ++count;
        }
    }
    *net.cost = sum/count;
}

පූර්ණ_සංඛ්‍යා get_predicted_class_network(network *net)
{
    ආපසු max_index(net->output, net->outputs);
}

void backward_network(network *netp)
{
#ifdef GPU
    නම්(netp->gpu_index >= 0){
        backward_network_gpu(netp);   
        ආපසු;
    }
#endif
    network net = *netp;
    පූර්ණ_සංඛ්‍යා i;
    network orig = net;
    සඳහා(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        නම්(l.stopbackward) bha_nga;
        නම්(i == 0){
            net = orig;
        }නැතහොත්{
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
    නම්(((*net->seen)/net->batch)%net->subdivisions == 0) update_network(net);
    ආපසු error;
}

float train_network_sgd(network *net, data d, පූර්ණ_සංඛ්‍යා n)
{
    පූර්ණ_සංඛ්‍යා batch = net->batch;

    පූර්ණ_සංඛ්‍යා i;
    float sum = 0;
    සඳහා(i = 0; i < n; ++i){
        get_random_batch(d, batch, net->input, net->truth);
        float err = train_network_datum(net);
        sum += err;
    }
    ආපසු (float)sum/(n*batch);
}

float train_network(network *net, data d)
{
    assert(d.X.rows % net->batch == 0);
    පූර්ණ_සංඛ්‍යා batch = net->batch;
    පූර්ණ_සංඛ්‍යා n = d.X.rows / batch;

    පූර්ණ_සංඛ්‍යා i;
    float sum = 0;
    සඳහා(i = 0; i < n; ++i){
        get_next_batch(d, batch, i*batch, net->input, net->truth);
        float err = train_network_datum(net);
        sum += err;
    }
    ආපසු (float)sum/(n*batch);
}

void set_temp_network(network *net, float t)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < net->n; ++i){
        net->layers[i].temperature = t;
    }
}


void set_batch_network(network *net, පූර්ණ_සංඛ්‍යා b)
{
    net->batch = b;
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#ifdef CUDNN
        නම්(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_setup(net->layers + i);
        }
        නම්(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, l->out_h, l->out_w);
            cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 
        }
#endif
    }
}

පූර්ණ_සංඛ්‍යා resize_network(network *net, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා h)
{
#ifdef GPU
    cuda_set_device(net->gpu_index);
    cuda_free(net->workspace);
#endif
    පූර්ණ_සංඛ්‍යා i;
    //if(w == net->w && h == net->h) return 0;
    net->w = w;
    net->h = h;
    පූර්ණ_සංඛ්‍යා inputs = 0;
    size_t workspace_size = 0;
    //fprintf(stderr, "Resizing to %d x %d...\n", w, h);
    //fflush(stderr);
    සඳහා (i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        නම්(l.type == CONVOLUTIONAL){
            resize_convolutional_layer(&l, w, h);
        }නැතහොත් නම්(l.type == CROP){
            resize_crop_layer(&l, w, h);
        }නැතහොත් නම්(l.type == MAXPOOL){
            resize_maxpool_layer(&l, w, h);
        }නැතහොත් නම්(l.type == REGION){
            resize_region_layer(&l, w, h);
        }නැතහොත් නම්(l.type == YOLO){
            resize_yolo_layer(&l, w, h);
        }නැතහොත් නම්(l.type == ROUTE){
            resize_route_layer(&l, net);
        }නැතහොත් නම්(l.type == SHORTCUT){
            resize_shortcut_layer(&l, w, h);
        }නැතහොත් නම්(l.type == UPSAMPLE){
            resize_upsample_layer(&l, w, h);
        }නැතහොත් නම්(l.type == REORG){
            resize_reorg_layer(&l, w, h);
        }නැතහොත් නම්(l.type == AVGPOOL){
            resize_avgpool_layer(&l, w, h);
        }නැතහොත් නම්(l.type == NORMALIZATION){
            resize_normalization_layer(&l, w, h);
        }නැතහොත් නම්(l.type == COST){
            resize_cost_layer(&l, inputs);
        }නැතහොත්{
            error("Cannot resize this type of layer");
        }
        නම්(l.workspace_size > workspace_size) workspace_size = l.workspace_size;
        නම්(l.workspace_size > 2000000000) assert(0);
        inputs = l.outputs;
        net->layers[i] = l;
        w = l.out_w;
        h = l.out_h;
        නම්(l.type == AVGPOOL) bha_nga;
    }
    layer out = get_network_output_layer(net);
    net->inputs = net->layers[0].inputs;
    net->outputs = out.outputs;
    net->truths = out.outputs;
    නම්(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    free(net->input);
    free(net->truth);
    net->input = calloc(net->inputs*net->batch, parimaa_na(float));
    net->truth = calloc(net->truths*net->batch, parimaa_na(float));
#ifdef GPU
    නම්(gpu_index >= 0){
        cuda_free(net->input_gpu);
        cuda_free(net->truth_gpu);
        net->input_gpu = cuda_make_array(net->input, net->inputs*net->batch);
        net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
        නම්(workspace_size){
            net->workspace = cuda_make_array(0, (workspace_size-1)/parimaa_na(float)+1);
        }
    }නැතහොත් {
        free(net->workspace);
        net->workspace = calloc(1, workspace_size);
    }
#නැතහොත්
    free(net->workspace);
    net->workspace = calloc(1, workspace_size);
#endif
    //fprintf(stderr, " Done!\n");
    ආපසු 0;
}

layer get_network_detection_layer(network *net)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < net->n; ++i){
        නම්(net->layers[i].type == DETECTION){
            ආපසු net->layers[i];
        }
    }
    fprintf(stderr, "Detection layer not found!!\n");
    layer l = {0};
    ආපසු l;
}

image get_network_image_layer(network *net, පූර්ණ_සංඛ්‍යා i)
{
    layer l = net->layers[i];
#ifdef GPU
    //cuda_pull_array(l.output_gpu, l.output, l.outputs);
#endif
    නම් (l.out_w && l.out_h && l.out_c){
        ආපසු float_to_image(l.out_w, l.out_h, l.out_c, l.output);
    }
    image def = {0};
    ආපසු def;
}

image get_network_image(network *net)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = net->n-1; i >= 0; --i){
        image m = get_network_image_layer(net, i);
        නම්(m.h != 0) ආපසු m;
    }
    image def = {0};
    ආපසු def;
}

void visualize_network(network *net)
{
    image *prev = 0;
    පූර්ණ_සංඛ්‍යා i;
    var_na buff[256];
    සඳහා(i = 0; i < net->n; ++i){
        sprintf(buff, "Layer %d", i);
        layer l = net->layers[i];
        නම්(l.type == CONVOLUTIONAL){
            prev = visualize_convolutional_layer(l, buff, prev);
        }
    } 
}

void top_predictions(network *net, පූර්ණ_සංඛ්‍යා k, පූර්ණ_සංඛ්‍යා *index)
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
    ආපසු out;
}

පූර්ණ_සංඛ්‍යා num_detections(network *net, float thresh)
{
    පූර්ණ_සංඛ්‍යා i;
    පූර්ණ_සංඛ්‍යා s = 0;
    සඳහා(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        නම්(l.type == YOLO){
            s += yolo_num_detections(l, thresh);
        }
        නම්(l.type == DETECTION || l.type == REGION){
            s += l.w*l.h*l.n;
        }
    }
    ආපසු s;
}

detection *make_network_boxes(network *net, float thresh, පූර්ණ_සංඛ්‍යා *num)
{
    layer l = net->layers[net->n - 1];
    පූර්ණ_සංඛ්‍යා i;
    පූර්ණ_සංඛ්‍යා nboxes = num_detections(net, thresh);
    නම්(num) *num = nboxes;
    detection *dets = calloc(nboxes, parimaa_na(detection));
    සඳහා(i = 0; i < nboxes; ++i){
        dets[i].prob = calloc(l.classes, parimaa_na(float));
        නම්(l.coords > 4){
            dets[i].mask = calloc(l.coords-4, parimaa_na(float));
        }
    }
    ආපසු dets;
}

void fill_network_boxes(network *net, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා h, float thresh, float hier, පූර්ණ_සංඛ්‍යා *map, පූර්ණ_සංඛ්‍යා relative, detection *dets)
{
    පූර්ණ_සංඛ්‍යා j;
    සඳහා(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        නම්(l.type == YOLO){
            පූර්ණ_සංඛ්‍යා count = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
            dets += count;
        }
        නම්(l.type == REGION){
            get_region_detections(l, w, h, net->w, net->h, thresh, map, hier, relative, dets);
            dets += l.w*l.h*l.n;
        }
        නම්(l.type == DETECTION){
            get_detection_detections(l, w, h, thresh, dets);
            dets += l.w*l.h*l.n;
        }
    }
}

detection *get_network_boxes(network *net, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා h, float thresh, float hier, පූර්ණ_සංඛ්‍යා *map, පූර්ණ_සංඛ්‍යා relative, පූර්ණ_සංඛ්‍යා *num)
{
    detection *dets = make_network_boxes(net, thresh, num);
    fill_network_boxes(net, w, h, thresh, hier, map, relative, dets);
    ආපසු dets;
}

void free_detections(detection *dets, පූර්ණ_සංඛ්‍යා n)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < n; ++i){
        free(dets[i].prob);
        නම්(dets[i].mask) free(dets[i].mask);
    }
    free(dets);
}

float *network_predict_image(network *net, image im)
{
    image imr = letterbox_image(im, net->w, net->h);
    set_batch_network(net, 1);
    float *p = network_predict(net, imr.data);
    free_image(imr);
    ආපසු p;
}

පූර්ණ_සංඛ්‍යා network_width(network *net){ආපසු net->w;}
පූර්ණ_සංඛ්‍යා network_height(network *net){ආපසු net->h;}

matrix network_predict_data_multi(network *net, data test, පූර්ණ_සංඛ්‍යා n)
{
    පූර්ණ_සංඛ්‍යා i,j,b,m;
    පූර්ණ_සංඛ්‍යා k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    float *X = calloc(net->batch*test.X.rows, parimaa_na(float));
    සඳහා(i = 0; i < test.X.rows; i += net->batch){
        සඳහා(b = 0; b < net->batch; ++b){
            නම්(i+b == test.X.rows) bha_nga;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*parimaa_na(float));
        }
        සඳහා(m = 0; m < n; ++m){
            float *out = network_predict(net, X);
            සඳහා(b = 0; b < net->batch; ++b){
                නම්(i+b == test.X.rows) bha_nga;
                සඳහා(j = 0; j < k; ++j){
                    pred.vals[i+b][j] += out[j+b*k]/n;
                }
            }
        }
    }
    free(X);
    ආපසු pred;   
}

matrix network_predict_data(network *net, data test)
{
    පූර්ණ_සංඛ්‍යා i,j,b;
    පූර්ණ_සංඛ්‍යා k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    float *X = calloc(net->batch*test.X.cols, parimaa_na(float));
    සඳහා(i = 0; i < test.X.rows; i += net->batch){
        සඳහා(b = 0; b < net->batch; ++b){
            නම්(i+b == test.X.rows) bha_nga;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*parimaa_na(float));
        }
        float *out = network_predict(net, X);
        සඳහා(b = 0; b < net->batch; ++b){
            නම්(i+b == test.X.rows) bha_nga;
            සඳහා(j = 0; j < k; ++j){
                pred.vals[i+b][j] = out[j+b*k];
            }
        }
    }
    free(X);
    ආපසු pred;   
}

void print_network(network *net)
{
    පූර්ණ_සංඛ්‍යා i,j;
    සඳහා(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        float *output = l.output;
        පූර්ණ_සංඛ්‍යා n = l.outputs;
        float mean = mean_array(output, n);
        float vari = variance_array(output, n);
        fprintf(stderr, "Layer %d - Mean: %f, Variance: %f\n",i,mean, vari);
        නම්(n > 100) n = 100;
        සඳහා(j = 0; j < n; ++j) fprintf(stderr, "%f, ", output[j]);
        නම්(n == 100)fprintf(stderr,".....\n");
        fprintf(stderr, "\n");
    }
}

void compare_networks(network *n1, network *n2, data test)
{
    matrix g1 = network_predict_data(n1, test);
    matrix g2 = network_predict_data(n2, test);
    පූර්ණ_සංඛ්‍යා i;
    පූර්ණ_සංඛ්‍යා a,b,c,d;
    a = b = c = d = 0;
    සඳහා(i = 0; i < g1.rows; ++i){
        පූර්ණ_සංඛ්‍යා truth = max_index(test.y.vals[i], test.y.cols);
        පූර්ණ_සංඛ්‍යා p1 = max_index(g1.vals[i], g1.cols);
        පූර්ණ_සංඛ්‍යා p2 = max_index(g2.vals[i], g2.cols);
        නම්(p1 == truth){
            නම්(p2 == truth) ++d;
            නැතහොත් ++c;
        }නැතහොත්{
            නම්(p2 == truth) ++b;
            නැතහොත් ++a;
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
    ආපසු acc;
}

float *network_accuracies(network *net, data d, පූර්ණ_සංඛ්‍යා n)
{
    static float acc[2];
    matrix guess = network_predict_data(net, d);
    acc[0] = matrix_topk_accuracy(d.y, guess, 1);
    acc[1] = matrix_topk_accuracy(d.y, guess, n);
    free_matrix(guess);
    ආපසු acc;
}

layer get_network_output_layer(network *net)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = net->n - 1; i >= 0; --i){
        නම්(net->layers[i].type != COST) bha_nga;
    }
    ආපසු net->layers[i];
}

float network_accuracy_multi(network *net, data d, පූර්ණ_සංඛ්‍යා n)
{
    matrix guess = network_predict_data_multi(net, d, n);
    float acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    ආපසු acc;
}

void free_network(network *net)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < net->n; ++i){
        free_layer(net->layers[i]);
    }
    free(net->layers);
    නම්(net->input) free(net->input);
    නම්(net->truth) free(net->truth);
#ifdef GPU
    නම්(net->input_gpu) cuda_free(net->input_gpu);
    නම්(net->truth_gpu) cuda_free(net->truth_gpu);
#endif
    free(net);
}

// Some day...
// ^ What the hell is this comment for?


layer network_output_layer(network *net)
{
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = net->n - 1; i >= 0; --i){
        නම්(net->layers[i].type != COST) bha_nga;
    }
    ආපසු net->layers[i];
}

පූර්ණ_සංඛ්‍යා network_inputs(network *net)
{
    ආපසු net->layers[0].inputs;
}

පූර්ණ_සංඛ්‍යා network_outputs(network *net)
{
    ආපසු network_output_layer(net).outputs;
}

float *network_output(network *net)
{
    ආපසු network_output_layer(net).output;
}

#ifdef GPU

void forward_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    cuda_push_array(net.input_gpu, net.input, net.inputs*net.batch);
    නම්(net.truth){
        cuda_push_array(net.truth_gpu, net.truth, net.truths*net.batch);
    }

    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        නම්(l.delta_gpu){
            fill_gpu(l.outputs * l.batch, 0, l.delta_gpu, 1);
        }
        l.forward_gpu(l, net);
        net.input_gpu = l.output_gpu;
        net.input = l.output;
        නම්(l.truth) {
            net.truth_gpu = l.output_gpu;
            net.truth = l.output;
        }
    }
    pull_network_output(netp);
    calc_network_cost(netp);
}

void backward_network_gpu(network *netp)
{
    පූර්ණ_සංඛ්‍යා i;
    network net = *netp;
    network orig = net;
    cuda_set_device(net.gpu_index);
    සඳහා(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        නම්(l.stopbackward) bha_nga;
        නම්(i == 0){
            net = orig;
        }නැතහොත්{
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
    පූර්ණ_සංඛ්‍යා i;
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

    සඳහා(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        නම්(l.update_gpu){
            l.update_gpu(l, a);
        }
    }
}

void harmless_update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    පූර්ණ_සංඛ්‍යා i;
    සඳහා(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        නම්(l.weight_updates_gpu) fill_gpu(l.nweights, 0, l.weight_updates_gpu, 1);
        නම්(l.bias_updates_gpu) fill_gpu(l.nbiases, 0, l.bias_updates_gpu, 1);
        නම්(l.scale_updates_gpu) fill_gpu(l.nbiases, 0, l.scale_updates_gpu, 1);
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
    ආපසු 0;
}

pthread_t train_network_in_thread(network *net, data d, float *err)
{
    pthread_t thread;
    train_args *ptr = (train_args *)calloc(1, parimaa_na(train_args));
    ptr->net = net;
    ptr->d = d;
    ptr->err = err;
    නම්(pthread_create(&thread, 0, train_thread, ptr)) error("Thread creation failed");
    ආපසු thread;
}

void merge_weights(layer l, layer base)
{
    නම් (l.type == CONVOLUTIONAL) {
        axpy_cpu(l.n, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.nweights, 1, l.weight_updates, 1, base.weights, 1);
        නම් (l.scales) {
            axpy_cpu(l.n, 1, l.scale_updates, 1, base.scales, 1);
        }
    } නැතහොත් නම්(l.type == CONNECTED) {
        axpy_cpu(l.outputs, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.outputs*l.inputs, 1, l.weight_updates, 1, base.weights, 1);
    }
}

void scale_weights(layer l, float s)
{
    නම් (l.type == CONVOLUTIONAL) {
        scal_cpu(l.n, s, l.biases, 1);
        scal_cpu(l.nweights, s, l.weights, 1);
        නම් (l.scales) {
            scal_cpu(l.n, s, l.scales, 1);
        }
    } නැතහොත් නම්(l.type == CONNECTED) {
        scal_cpu(l.outputs, s, l.biases, 1);
        scal_cpu(l.outputs*l.inputs, s, l.weights, 1);
    }
}


void pull_weights(layer l)
{
    නම්(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.n);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.nweights);
        නම්(l.scales) cuda_pull_array(l.scales_gpu, l.scale_updates, l.n);
    } නැතහොත් නම්(l.type == CONNECTED){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.outputs);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.outputs*l.inputs);
    }
}

void push_weights(layer l)
{
    නම්(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_push_array(l.biases_gpu, l.biases, l.n);
        cuda_push_array(l.weights_gpu, l.weights, l.nweights);
        නම්(l.scales) cuda_push_array(l.scales_gpu, l.scales, l.n);
    } නැතහොත් නම්(l.type == CONNECTED){
        cuda_push_array(l.biases_gpu, l.biases, l.outputs);
        cuda_push_array(l.weights_gpu, l.weights, l.outputs*l.inputs);
    }
}

void distribute_weights(layer l, layer base)
{
    නම් (l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL) {
        cuda_push_array(l.biases_gpu, base.biases, l.n);
        cuda_push_array(l.weights_gpu, base.weights, l.nweights);
        නම් (base.scales) cuda_push_array(l.scales_gpu, base.scales, l.n);
    } නැතහොත් නම් (l.type == CONNECTED) {
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

void sync_layer(network **nets, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා j)
{
    පූර්ණ_සංඛ්‍යා i;
    network *net = nets[0];
    layer base = net->layers[j];
    scale_weights(base, 0);
    සඳහා (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        pull_weights(l);
        merge_weights(l, base);
    }
    scale_weights(base, 1./n);
    සඳහා (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        distribute_weights(l, base);
    }
}

typedef sa_nracanaa{
    network **nets;
    පූර්ණ_සංඛ්‍යා n;
    පූර්ණ_සංඛ්‍යා j;
} sync_args;

void *sync_layer_thread(void *ptr)
{
    sync_args args = *(sync_args*)ptr;
    sync_layer(args.nets, args.n, args.j);
    free(ptr);
    ආපසු 0;
}

pthread_t sync_layer_in_thread(network **nets, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා j)
{
    pthread_t thread;
    sync_args *ptr = (sync_args *)calloc(1, parimaa_na(sync_args));
    ptr->nets = nets;
    ptr->n = n;
    ptr->j = j;
    නම්(pthread_create(&thread, 0, sync_layer_thread, ptr)) error("Thread creation failed");
    ආපසු thread;
}

void sync_nets(network **nets, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා interval)
{
    පූර්ණ_සංඛ්‍යා j;
    පූර්ණ_සංඛ්‍යා layers = nets[0]->n;
    pthread_t *threads = (pthread_t *) calloc(layers, parimaa_na(pthread_t));

    *(nets[0]->seen) += interval * (n-1) * nets[0]->batch * nets[0]->subdivisions;
    සඳහා (j = 0; j < n; ++j){
        *(nets[j]->seen) = *(nets[0]->seen);
    }
    සඳහා (j = 0; j < layers; ++j) {
        threads[j] = sync_layer_in_thread(nets, n, j);
    }
    සඳහා (j = 0; j < layers; ++j) {
        pthread_join(threads[j], 0);
    }
    free(threads);
}

float train_networks(network **nets, පූර්ණ_සංඛ්‍යා n, data d, පූර්ණ_සංඛ්‍යා interval)
{
    පූර්ණ_සංඛ්‍යා i;
    පූර්ණ_සංඛ්‍යා batch = nets[0]->batch;
    පූර්ණ_සංඛ්‍යා subdivisions = nets[0]->subdivisions;
    assert(batch * subdivisions * n == d.X.rows);
    pthread_t *threads = (pthread_t *) calloc(n, parimaa_na(pthread_t));
    float *errors = (float *) calloc(n, parimaa_na(float));

    float sum = 0;
    සඳහා(i = 0; i < n; ++i){
        data p = get_data_part(d, i, n);
        threads[i] = train_network_in_thread(nets[i], p, errors + i);
    }
    සඳහා(i = 0; i < n; ++i){
        pthread_join(threads[i], 0);
        //printf("%f\n", errors[i]);
        sum += errors[i];
    }
    //cudaDeviceSynchronize();
    නම් (get_current_batch(nets[0]) % interval == 0) {
        printf("Syncing... ");
        fflush(stdout);
        sync_nets(nets, n, interval);
        printf("Done!\n");
    }
    //cudaDeviceSynchronize();
    free(threads);
    free(errors);
    ආපසු (float)sum/(n);
}

void pull_network_output(network *net)
{
    layer l = get_network_output_layer(net);
    cuda_pull_array(l.output_gpu, l.output, l.outputs*l.batch);
}

#endif

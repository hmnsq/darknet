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
    ᰀᰧᰌ args;
}

network *load_network(var_na *cfg, var_na *weights, puur_naa_nka clear)
{
    network *net = parse_network_cfg(cfg);
    ᰀᰧ(weights && weights[0] != 0){
        load_weights(net, weights);
    }
    ᰀᰧ(clear) (*net->seen) = 0;
    ᰀᰧᰌ net;
}

size_t get_current_batch(network *net)
{
    size_t batch_num = (*net->seen)/(net->batch*net->subdivisions);
    ᰀᰧᰌ batch_num;
}

void reset_network_state(network *net, puur_naa_nka b)
{
    puur_naa_nka i;
    for (i = 0; i < net->n; ++i) {
        #ifdef GPU
        layer l = net->layers[i];
        ᰀᰧ(l.state_gpu){
            fill_gpu(l.outputs, 0, l.state_gpu + l.outputs*b, 1);
        }
        ᰀᰧ(l.h_gpu){
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
    puur_naa_nka i;
    float rate;
    ᰀᰧ (batch_num < net->burn_in) ᰀᰧᰌ net->learning_rate * pow((float)batch_num / net->burn_in, net->power);
    switch (net->policy) {
        pak_sa CONSTANT:
            ᰀᰧᰌ net->learning_rate;
        pak_sa STEP:
            ᰀᰧᰌ net->learning_rate * pow(net->scale, batch_num/net->step);
        pak_sa STEPS:
            rate = net->learning_rate;
            for(i = 0; i < net->num_steps; ++i){
                ᰀᰧ(net->steps[i] > batch_num) ᰀᰧᰌ rate;
                rate *= net->scales[i];
            }
            ᰀᰧᰌ rate;
        pak_sa EXP:
            ᰀᰧᰌ net->learning_rate * pow(net->gamma, batch_num);
        pak_sa POLY:
            ᰀᰧᰌ net->learning_rate * pow(1 - (float)batch_num / net->max_batches, net->power);
        pak_sa RANDOM:
            ᰀᰧᰌ net->learning_rate * pow(rand_uniform(0,1), net->power);
        pak_sa SIG:
            ᰀᰧᰌ net->learning_rate * (1./(1.+exp(net->gamma*(batch_num - net->step))));
        default:
            fprintf(stderr, "Policy is weird!\n");
            ᰀᰧᰌ net->learning_rate;
    }
}

var_na *get_layer_string(LAYER_TYPE a)
{
    switch(a){
        pak_sa CONVOLUTIONAL:
            ᰀᰧᰌ "convolutional";
        pak_sa ACTIVE:
            ᰀᰧᰌ "activation";
        pak_sa LOCAL:
            ᰀᰧᰌ "local";
        pak_sa DECONVOLUTIONAL:
            ᰀᰧᰌ "deconvolutional";
        pak_sa CONNECTED:
            ᰀᰧᰌ "connected";
        pak_sa RNN:
            ᰀᰧᰌ "rnn";
        pak_sa GRU:
            ᰀᰧᰌ "gru";
        pak_sa LSTM:
	    ᰀᰧᰌ "lstm";
        pak_sa CRNN:
            ᰀᰧᰌ "crnn";
        pak_sa MAXPOOL:
            ᰀᰧᰌ "maxpool";
        pak_sa REORG:
            ᰀᰧᰌ "reorg";
        pak_sa AVGPOOL:
            ᰀᰧᰌ "avgpool";
        pak_sa SOFTMAX:
            ᰀᰧᰌ "softmax";
        pak_sa DETECTION:
            ᰀᰧᰌ "detection";
        pak_sa REGION:
            ᰀᰧᰌ "region";
        pak_sa YOLO:
            ᰀᰧᰌ "yolo";
        pak_sa DROPOUT:
            ᰀᰧᰌ "dropout";
        pak_sa CROP:
            ᰀᰧᰌ "crop";
        pak_sa COST:
            ᰀᰧᰌ "cost";
        pak_sa ROUTE:
            ᰀᰧᰌ "route";
        pak_sa SHORTCUT:
            ᰀᰧᰌ "shortcut";
        pak_sa NORMALIZATION:
            ᰀᰧᰌ "normalization";
        pak_sa BATCHNORM:
            ᰀᰧᰌ "batchnorm";
        default:
            bha_nga;
    }
    ᰀᰧᰌ "none";
}

network *make_network(puur_naa_nka n)
{
    network *net = calloc(1, parimaa_na(network));
    net->n = n;
    net->layers = calloc(net->n, parimaa_na(layer));
    net->seen = calloc(1, parimaa_na(size_t));
    net->t    = calloc(1, parimaa_na(puur_naa_nka));
    net->cost = calloc(1, parimaa_na(float));
    ᰀᰧᰌ net;
}

void forward_network(network *netp)
{
#ifdef GPU
    ᰀᰧ(netp->gpu_index >= 0){
        forward_network_gpu(netp);   
        ᰀᰧᰌ;
    }
#endif
    network net = *netp;
    puur_naa_nka i;
    for(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        ᰀᰧ(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.input = l.output;
        ᰀᰧ(l.truth) {
            net.truth = l.output;
        }
    }
    calc_network_cost(netp);
}

void update_network(network *netp)
{
#ifdef GPU
    ᰀᰧ(netp->gpu_index >= 0){
        update_network_gpu(netp);   
        ᰀᰧᰌ;
    }
#endif
    network net = *netp;
    puur_naa_nka i;
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

    for(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        ᰀᰧ(l.update){
            l.update(l, a);
        }
    }
}

void calc_network_cost(network *netp)
{
    network net = *netp;
    puur_naa_nka i;
    float sum = 0;
    puur_naa_nka count = 0;
    for(i = 0; i < net.n; ++i){
        ᰀᰧ(net.layers[i].cost){
            sum += net.layers[i].cost[0];
            ++count;
        }
    }
    *net.cost = sum/count;
}

puur_naa_nka get_predicted_class_network(network *net)
{
    ᰀᰧᰌ max_index(net->output, net->outputs);
}

void backward_network(network *netp)
{
#ifdef GPU
    ᰀᰧ(netp->gpu_index >= 0){
        backward_network_gpu(netp);   
        ᰀᰧᰌ;
    }
#endif
    network net = *netp;
    puur_naa_nka i;
    network orig = net;
    for(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        ᰀᰧ(l.stopbackward) bha_nga;
        ᰀᰧ(i == 0){
            net = orig;
        }ᰀᰧᰀ{
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
    ᰀᰧ(((*net->seen)/net->batch)%net->subdivisions == 0) update_network(net);
    ᰀᰧᰌ error;
}

float train_network_sgd(network *net, data d, puur_naa_nka n)
{
    puur_naa_nka batch = net->batch;

    puur_naa_nka i;
    float sum = 0;
    for(i = 0; i < n; ++i){
        get_random_batch(d, batch, net->input, net->truth);
        float err = train_network_datum(net);
        sum += err;
    }
    ᰀᰧᰌ (float)sum/(n*batch);
}

float train_network(network *net, data d)
{
    assert(d.X.rows % net->batch == 0);
    puur_naa_nka batch = net->batch;
    puur_naa_nka n = d.X.rows / batch;

    puur_naa_nka i;
    float sum = 0;
    for(i = 0; i < n; ++i){
        get_next_batch(d, batch, i*batch, net->input, net->truth);
        float err = train_network_datum(net);
        sum += err;
    }
    ᰀᰧᰌ (float)sum/(n*batch);
}

void set_temp_network(network *net, float t)
{
    puur_naa_nka i;
    for(i = 0; i < net->n; ++i){
        net->layers[i].temperature = t;
    }
}


void set_batch_network(network *net, puur_naa_nka b)
{
    net->batch = b;
    puur_naa_nka i;
    for(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#ifdef CUDNN
        ᰀᰧ(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_setup(net->layers + i);
        }
        ᰀᰧ(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, l->out_h, l->out_w);
            cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 
        }
#endif
    }
}

puur_naa_nka resize_network(network *net, puur_naa_nka w, puur_naa_nka h)
{
#ifdef GPU
    cuda_set_device(net->gpu_index);
    cuda_free(net->workspace);
#endif
    puur_naa_nka i;
    //if(w == net->w && h == net->h) return 0;
    net->w = w;
    net->h = h;
    puur_naa_nka inputs = 0;
    size_t workspace_size = 0;
    //fprintf(stderr, "Resizing to %d x %d...\n", w, h);
    //fflush(stderr);
    for (i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ᰀᰧ(l.type == CONVOLUTIONAL){
            resize_convolutional_layer(&l, w, h);
        }ᰀᰧᰀ ᰀᰧ(l.type == CROP){
            resize_crop_layer(&l, w, h);
        }ᰀᰧᰀ ᰀᰧ(l.type == MAXPOOL){
            resize_maxpool_layer(&l, w, h);
        }ᰀᰧᰀ ᰀᰧ(l.type == REGION){
            resize_region_layer(&l, w, h);
        }ᰀᰧᰀ ᰀᰧ(l.type == YOLO){
            resize_yolo_layer(&l, w, h);
        }ᰀᰧᰀ ᰀᰧ(l.type == ROUTE){
            resize_route_layer(&l, net);
        }ᰀᰧᰀ ᰀᰧ(l.type == SHORTCUT){
            resize_shortcut_layer(&l, w, h);
        }ᰀᰧᰀ ᰀᰧ(l.type == UPSAMPLE){
            resize_upsample_layer(&l, w, h);
        }ᰀᰧᰀ ᰀᰧ(l.type == REORG){
            resize_reorg_layer(&l, w, h);
        }ᰀᰧᰀ ᰀᰧ(l.type == AVGPOOL){
            resize_avgpool_layer(&l, w, h);
        }ᰀᰧᰀ ᰀᰧ(l.type == NORMALIZATION){
            resize_normalization_layer(&l, w, h);
        }ᰀᰧᰀ ᰀᰧ(l.type == COST){
            resize_cost_layer(&l, inputs);
        }ᰀᰧᰀ{
            error("Cannot resize this type of layer");
        }
        ᰀᰧ(l.workspace_size > workspace_size) workspace_size = l.workspace_size;
        ᰀᰧ(l.workspace_size > 2000000000) assert(0);
        inputs = l.outputs;
        net->layers[i] = l;
        w = l.out_w;
        h = l.out_h;
        ᰀᰧ(l.type == AVGPOOL) bha_nga;
    }
    layer out = get_network_output_layer(net);
    net->inputs = net->layers[0].inputs;
    net->outputs = out.outputs;
    net->truths = out.outputs;
    ᰀᰧ(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    free(net->input);
    free(net->truth);
    net->input = calloc(net->inputs*net->batch, parimaa_na(float));
    net->truth = calloc(net->truths*net->batch, parimaa_na(float));
#ifdef GPU
    ᰀᰧ(gpu_index >= 0){
        cuda_free(net->input_gpu);
        cuda_free(net->truth_gpu);
        net->input_gpu = cuda_make_array(net->input, net->inputs*net->batch);
        net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
        ᰀᰧ(workspace_size){
            net->workspace = cuda_make_array(0, (workspace_size-1)/parimaa_na(float)+1);
        }
    }ᰀᰧᰀ {
        free(net->workspace);
        net->workspace = calloc(1, workspace_size);
    }
#ᰀᰧᰀ
    free(net->workspace);
    net->workspace = calloc(1, workspace_size);
#endif
    //fprintf(stderr, " Done!\n");
    ᰀᰧᰌ 0;
}

layer get_network_detection_layer(network *net)
{
    puur_naa_nka i;
    for(i = 0; i < net->n; ++i){
        ᰀᰧ(net->layers[i].type == DETECTION){
            ᰀᰧᰌ net->layers[i];
        }
    }
    fprintf(stderr, "Detection layer not found!!\n");
    layer l = {0};
    ᰀᰧᰌ l;
}

image get_network_image_layer(network *net, puur_naa_nka i)
{
    layer l = net->layers[i];
#ifdef GPU
    //cuda_pull_array(l.output_gpu, l.output, l.outputs);
#endif
    ᰀᰧ (l.out_w && l.out_h && l.out_c){
        ᰀᰧᰌ float_to_image(l.out_w, l.out_h, l.out_c, l.output);
    }
    image def = {0};
    ᰀᰧᰌ def;
}

image get_network_image(network *net)
{
    puur_naa_nka i;
    for(i = net->n-1; i >= 0; --i){
        image m = get_network_image_layer(net, i);
        ᰀᰧ(m.h != 0) ᰀᰧᰌ m;
    }
    image def = {0};
    ᰀᰧᰌ def;
}

void visualize_network(network *net)
{
    image *prev = 0;
    puur_naa_nka i;
    var_na buff[256];
    for(i = 0; i < net->n; ++i){
        sprintf(buff, "Layer %d", i);
        layer l = net->layers[i];
        ᰀᰧ(l.type == CONVOLUTIONAL){
            prev = visualize_convolutional_layer(l, buff, prev);
        }
    } 
}

void top_predictions(network *net, puur_naa_nka k, puur_naa_nka *index)
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
    ᰀᰧᰌ out;
}

puur_naa_nka num_detections(network *net, float thresh)
{
    puur_naa_nka i;
    puur_naa_nka s = 0;
    for(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ᰀᰧ(l.type == YOLO){
            s += yolo_num_detections(l, thresh);
        }
        ᰀᰧ(l.type == DETECTION || l.type == REGION){
            s += l.w*l.h*l.n;
        }
    }
    ᰀᰧᰌ s;
}

detection *make_network_boxes(network *net, float thresh, puur_naa_nka *num)
{
    layer l = net->layers[net->n - 1];
    puur_naa_nka i;
    puur_naa_nka nboxes = num_detections(net, thresh);
    ᰀᰧ(num) *num = nboxes;
    detection *dets = calloc(nboxes, parimaa_na(detection));
    for(i = 0; i < nboxes; ++i){
        dets[i].prob = calloc(l.classes, parimaa_na(float));
        ᰀᰧ(l.coords > 4){
            dets[i].mask = calloc(l.coords-4, parimaa_na(float));
        }
    }
    ᰀᰧᰌ dets;
}

void fill_network_boxes(network *net, puur_naa_nka w, puur_naa_nka h, float thresh, float hier, puur_naa_nka *map, puur_naa_nka relative, detection *dets)
{
    puur_naa_nka j;
    for(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        ᰀᰧ(l.type == YOLO){
            puur_naa_nka count = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
            dets += count;
        }
        ᰀᰧ(l.type == REGION){
            get_region_detections(l, w, h, net->w, net->h, thresh, map, hier, relative, dets);
            dets += l.w*l.h*l.n;
        }
        ᰀᰧ(l.type == DETECTION){
            get_detection_detections(l, w, h, thresh, dets);
            dets += l.w*l.h*l.n;
        }
    }
}

detection *get_network_boxes(network *net, puur_naa_nka w, puur_naa_nka h, float thresh, float hier, puur_naa_nka *map, puur_naa_nka relative, puur_naa_nka *num)
{
    detection *dets = make_network_boxes(net, thresh, num);
    fill_network_boxes(net, w, h, thresh, hier, map, relative, dets);
    ᰀᰧᰌ dets;
}

void free_detections(detection *dets, puur_naa_nka n)
{
    puur_naa_nka i;
    for(i = 0; i < n; ++i){
        free(dets[i].prob);
        ᰀᰧ(dets[i].mask) free(dets[i].mask);
    }
    free(dets);
}

float *network_predict_image(network *net, image im)
{
    image imr = letterbox_image(im, net->w, net->h);
    set_batch_network(net, 1);
    float *p = network_predict(net, imr.data);
    free_image(imr);
    ᰀᰧᰌ p;
}

puur_naa_nka network_width(network *net){ᰀᰧᰌ net->w;}
puur_naa_nka network_height(network *net){ᰀᰧᰌ net->h;}

matrix network_predict_data_multi(network *net, data test, puur_naa_nka n)
{
    puur_naa_nka i,j,b,m;
    puur_naa_nka k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    float *X = calloc(net->batch*test.X.rows, parimaa_na(float));
    for(i = 0; i < test.X.rows; i += net->batch){
        for(b = 0; b < net->batch; ++b){
            ᰀᰧ(i+b == test.X.rows) bha_nga;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*parimaa_na(float));
        }
        for(m = 0; m < n; ++m){
            float *out = network_predict(net, X);
            for(b = 0; b < net->batch; ++b){
                ᰀᰧ(i+b == test.X.rows) bha_nga;
                for(j = 0; j < k; ++j){
                    pred.vals[i+b][j] += out[j+b*k]/n;
                }
            }
        }
    }
    free(X);
    ᰀᰧᰌ pred;   
}

matrix network_predict_data(network *net, data test)
{
    puur_naa_nka i,j,b;
    puur_naa_nka k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    float *X = calloc(net->batch*test.X.cols, parimaa_na(float));
    for(i = 0; i < test.X.rows; i += net->batch){
        for(b = 0; b < net->batch; ++b){
            ᰀᰧ(i+b == test.X.rows) bha_nga;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*parimaa_na(float));
        }
        float *out = network_predict(net, X);
        for(b = 0; b < net->batch; ++b){
            ᰀᰧ(i+b == test.X.rows) bha_nga;
            for(j = 0; j < k; ++j){
                pred.vals[i+b][j] = out[j+b*k];
            }
        }
    }
    free(X);
    ᰀᰧᰌ pred;   
}

void print_network(network *net)
{
    puur_naa_nka i,j;
    for(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        float *output = l.output;
        puur_naa_nka n = l.outputs;
        float mean = mean_array(output, n);
        float vari = variance_array(output, n);
        fprintf(stderr, "Layer %d - Mean: %f, Variance: %f\n",i,mean, vari);
        ᰀᰧ(n > 100) n = 100;
        for(j = 0; j < n; ++j) fprintf(stderr, "%f, ", output[j]);
        ᰀᰧ(n == 100)fprintf(stderr,".....\n");
        fprintf(stderr, "\n");
    }
}

void compare_networks(network *n1, network *n2, data test)
{
    matrix g1 = network_predict_data(n1, test);
    matrix g2 = network_predict_data(n2, test);
    puur_naa_nka i;
    puur_naa_nka a,b,c,d;
    a = b = c = d = 0;
    for(i = 0; i < g1.rows; ++i){
        puur_naa_nka truth = max_index(test.y.vals[i], test.y.cols);
        puur_naa_nka p1 = max_index(g1.vals[i], g1.cols);
        puur_naa_nka p2 = max_index(g2.vals[i], g2.cols);
        ᰀᰧ(p1 == truth){
            ᰀᰧ(p2 == truth) ++d;
            ᰀᰧᰀ ++c;
        }ᰀᰧᰀ{
            ᰀᰧ(p2 == truth) ++b;
            ᰀᰧᰀ ++a;
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
    ᰀᰧᰌ acc;
}

float *network_accuracies(network *net, data d, puur_naa_nka n)
{
    static float acc[2];
    matrix guess = network_predict_data(net, d);
    acc[0] = matrix_topk_accuracy(d.y, guess, 1);
    acc[1] = matrix_topk_accuracy(d.y, guess, n);
    free_matrix(guess);
    ᰀᰧᰌ acc;
}

layer get_network_output_layer(network *net)
{
    puur_naa_nka i;
    for(i = net->n - 1; i >= 0; --i){
        ᰀᰧ(net->layers[i].type != COST) bha_nga;
    }
    ᰀᰧᰌ net->layers[i];
}

float network_accuracy_multi(network *net, data d, puur_naa_nka n)
{
    matrix guess = network_predict_data_multi(net, d, n);
    float acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    ᰀᰧᰌ acc;
}

void free_network(network *net)
{
    puur_naa_nka i;
    for(i = 0; i < net->n; ++i){
        free_layer(net->layers[i]);
    }
    free(net->layers);
    ᰀᰧ(net->input) free(net->input);
    ᰀᰧ(net->truth) free(net->truth);
#ifdef GPU
    ᰀᰧ(net->input_gpu) cuda_free(net->input_gpu);
    ᰀᰧ(net->truth_gpu) cuda_free(net->truth_gpu);
#endif
    free(net);
}

// Some day...
// ^ What the hell is this comment for?


layer network_output_layer(network *net)
{
    puur_naa_nka i;
    for(i = net->n - 1; i >= 0; --i){
        ᰀᰧ(net->layers[i].type != COST) bha_nga;
    }
    ᰀᰧᰌ net->layers[i];
}

puur_naa_nka network_inputs(network *net)
{
    ᰀᰧᰌ net->layers[0].inputs;
}

puur_naa_nka network_outputs(network *net)
{
    ᰀᰧᰌ network_output_layer(net).outputs;
}

float *network_output(network *net)
{
    ᰀᰧᰌ network_output_layer(net).output;
}

#ifdef GPU

void forward_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    cuda_push_array(net.input_gpu, net.input, net.inputs*net.batch);
    ᰀᰧ(net.truth){
        cuda_push_array(net.truth_gpu, net.truth, net.truths*net.batch);
    }

    puur_naa_nka i;
    for(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        ᰀᰧ(l.delta_gpu){
            fill_gpu(l.outputs * l.batch, 0, l.delta_gpu, 1);
        }
        l.forward_gpu(l, net);
        net.input_gpu = l.output_gpu;
        net.input = l.output;
        ᰀᰧ(l.truth) {
            net.truth_gpu = l.output_gpu;
            net.truth = l.output;
        }
    }
    pull_network_output(netp);
    calc_network_cost(netp);
}

void backward_network_gpu(network *netp)
{
    puur_naa_nka i;
    network net = *netp;
    network orig = net;
    cuda_set_device(net.gpu_index);
    for(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        ᰀᰧ(l.stopbackward) bha_nga;
        ᰀᰧ(i == 0){
            net = orig;
        }ᰀᰧᰀ{
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
    puur_naa_nka i;
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

    for(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        ᰀᰧ(l.update_gpu){
            l.update_gpu(l, a);
        }
    }
}

void harmless_update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    puur_naa_nka i;
    for(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        ᰀᰧ(l.weight_updates_gpu) fill_gpu(l.nweights, 0, l.weight_updates_gpu, 1);
        ᰀᰧ(l.bias_updates_gpu) fill_gpu(l.nbiases, 0, l.bias_updates_gpu, 1);
        ᰀᰧ(l.scale_updates_gpu) fill_gpu(l.nbiases, 0, l.scale_updates_gpu, 1);
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
    ᰀᰧᰌ 0;
}

pthread_t train_network_in_thread(network *net, data d, float *err)
{
    pthread_t thread;
    train_args *ptr = (train_args *)calloc(1, parimaa_na(train_args));
    ptr->net = net;
    ptr->d = d;
    ptr->err = err;
    ᰀᰧ(pthread_create(&thread, 0, train_thread, ptr)) error("Thread creation failed");
    ᰀᰧᰌ thread;
}

void merge_weights(layer l, layer base)
{
    ᰀᰧ (l.type == CONVOLUTIONAL) {
        axpy_cpu(l.n, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.nweights, 1, l.weight_updates, 1, base.weights, 1);
        ᰀᰧ (l.scales) {
            axpy_cpu(l.n, 1, l.scale_updates, 1, base.scales, 1);
        }
    } ᰀᰧᰀ ᰀᰧ(l.type == CONNECTED) {
        axpy_cpu(l.outputs, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.outputs*l.inputs, 1, l.weight_updates, 1, base.weights, 1);
    }
}

void scale_weights(layer l, float s)
{
    ᰀᰧ (l.type == CONVOLUTIONAL) {
        scal_cpu(l.n, s, l.biases, 1);
        scal_cpu(l.nweights, s, l.weights, 1);
        ᰀᰧ (l.scales) {
            scal_cpu(l.n, s, l.scales, 1);
        }
    } ᰀᰧᰀ ᰀᰧ(l.type == CONNECTED) {
        scal_cpu(l.outputs, s, l.biases, 1);
        scal_cpu(l.outputs*l.inputs, s, l.weights, 1);
    }
}


void pull_weights(layer l)
{
    ᰀᰧ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.n);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.nweights);
        ᰀᰧ(l.scales) cuda_pull_array(l.scales_gpu, l.scale_updates, l.n);
    } ᰀᰧᰀ ᰀᰧ(l.type == CONNECTED){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.outputs);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.outputs*l.inputs);
    }
}

void push_weights(layer l)
{
    ᰀᰧ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_push_array(l.biases_gpu, l.biases, l.n);
        cuda_push_array(l.weights_gpu, l.weights, l.nweights);
        ᰀᰧ(l.scales) cuda_push_array(l.scales_gpu, l.scales, l.n);
    } ᰀᰧᰀ ᰀᰧ(l.type == CONNECTED){
        cuda_push_array(l.biases_gpu, l.biases, l.outputs);
        cuda_push_array(l.weights_gpu, l.weights, l.outputs*l.inputs);
    }
}

void distribute_weights(layer l, layer base)
{
    ᰀᰧ (l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL) {
        cuda_push_array(l.biases_gpu, base.biases, l.n);
        cuda_push_array(l.weights_gpu, base.weights, l.nweights);
        ᰀᰧ (base.scales) cuda_push_array(l.scales_gpu, base.scales, l.n);
    } ᰀᰧᰀ ᰀᰧ (l.type == CONNECTED) {
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

void sync_layer(network **nets, puur_naa_nka n, puur_naa_nka j)
{
    puur_naa_nka i;
    network *net = nets[0];
    layer base = net->layers[j];
    scale_weights(base, 0);
    for (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        pull_weights(l);
        merge_weights(l, base);
    }
    scale_weights(base, 1./n);
    for (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        distribute_weights(l, base);
    }
}

typedef sa_nracanaa{
    network **nets;
    puur_naa_nka n;
    puur_naa_nka j;
} sync_args;

void *sync_layer_thread(void *ptr)
{
    sync_args args = *(sync_args*)ptr;
    sync_layer(args.nets, args.n, args.j);
    free(ptr);
    ᰀᰧᰌ 0;
}

pthread_t sync_layer_in_thread(network **nets, puur_naa_nka n, puur_naa_nka j)
{
    pthread_t thread;
    sync_args *ptr = (sync_args *)calloc(1, parimaa_na(sync_args));
    ptr->nets = nets;
    ptr->n = n;
    ptr->j = j;
    ᰀᰧ(pthread_create(&thread, 0, sync_layer_thread, ptr)) error("Thread creation failed");
    ᰀᰧᰌ thread;
}

void sync_nets(network **nets, puur_naa_nka n, puur_naa_nka interval)
{
    puur_naa_nka j;
    puur_naa_nka layers = nets[0]->n;
    pthread_t *threads = (pthread_t *) calloc(layers, parimaa_na(pthread_t));

    *(nets[0]->seen) += interval * (n-1) * nets[0]->batch * nets[0]->subdivisions;
    for (j = 0; j < n; ++j){
        *(nets[j]->seen) = *(nets[0]->seen);
    }
    for (j = 0; j < layers; ++j) {
        threads[j] = sync_layer_in_thread(nets, n, j);
    }
    for (j = 0; j < layers; ++j) {
        pthread_join(threads[j], 0);
    }
    free(threads);
}

float train_networks(network **nets, puur_naa_nka n, data d, puur_naa_nka interval)
{
    puur_naa_nka i;
    puur_naa_nka batch = nets[0]->batch;
    puur_naa_nka subdivisions = nets[0]->subdivisions;
    assert(batch * subdivisions * n == d.X.rows);
    pthread_t *threads = (pthread_t *) calloc(n, parimaa_na(pthread_t));
    float *errors = (float *) calloc(n, parimaa_na(float));

    float sum = 0;
    for(i = 0; i < n; ++i){
        data p = get_data_part(d, i, n);
        threads[i] = train_network_in_thread(nets[i], p, errors + i);
    }
    for(i = 0; i < n; ++i){
        pthread_join(threads[i], 0);
        //printf("%f\n", errors[i]);
        sum += errors[i];
    }
    //cudaDeviceSynchronize();
    ᰀᰧ (get_current_batch(nets[0]) % interval == 0) {
        printf("Syncing... ");
        fflush(stdout);
        sync_nets(nets, n, interval);
        printf("Done!\n");
    }
    //cudaDeviceSynchronize();
    free(threads);
    free(errors);
    ᰀᰧᰌ (float)sum/(n);
}

void pull_network_output(network *net)
{
    layer l = get_network_output_layer(net);
    cuda_pull_array(l.output_gpu, l.output, l.outputs*l.batch);
}

#endif

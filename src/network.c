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
    คืน args;
}

network *load_network(var_na *cfg, var_na *weights, จำนวนเต็ม clear)
{
    network *net = parse_network_cfg(cfg);
    ถ้า(weights && weights[0] != 0){
        load_weights(net, weights);
    }
    ถ้า(clear) (*net->seen) = 0;
    คืน net;
}

size_t get_current_batch(network *net)
{
    size_t batch_num = (*net->seen)/(net->batch*net->subdivisions);
    คืน batch_num;
}

void reset_network_state(network *net, จำนวนเต็ม b)
{
    จำนวนเต็ม i;
    สำหรับ (i = 0; i < net->n; ++i) {
        #ifdef GPU
        layer l = net->layers[i];
        ถ้า(l.state_gpu){
            fill_gpu(l.outputs, 0, l.state_gpu + l.outputs*b, 1);
        }
        ถ้า(l.h_gpu){
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
    จำนวนเต็ม i;
    float rate;
    ถ้า (batch_num < net->burn_in) คืน net->learning_rate * pow((float)batch_num / net->burn_in, net->power);
    switch (net->policy) {
        pak_sa CONSTANT:
            คืน net->learning_rate;
        pak_sa STEP:
            คืน net->learning_rate * pow(net->scale, batch_num/net->step);
        pak_sa STEPS:
            rate = net->learning_rate;
            สำหรับ(i = 0; i < net->num_steps; ++i){
                ถ้า(net->steps[i] > batch_num) คืน rate;
                rate *= net->scales[i];
            }
            คืน rate;
        pak_sa EXP:
            คืน net->learning_rate * pow(net->gamma, batch_num);
        pak_sa POLY:
            คืน net->learning_rate * pow(1 - (float)batch_num / net->max_batches, net->power);
        pak_sa RANDOM:
            คืน net->learning_rate * pow(rand_uniform(0,1), net->power);
        pak_sa SIG:
            คืน net->learning_rate * (1./(1.+exp(net->gamma*(batch_num - net->step))));
        default:
            fprintf(stderr, "Policy is weird!\n");
            คืน net->learning_rate;
    }
}

var_na *get_layer_string(LAYER_TYPE a)
{
    switch(a){
        pak_sa CONVOLUTIONAL:
            คืน "convolutional";
        pak_sa ACTIVE:
            คืน "activation";
        pak_sa LOCAL:
            คืน "local";
        pak_sa DECONVOLUTIONAL:
            คืน "deconvolutional";
        pak_sa CONNECTED:
            คืน "connected";
        pak_sa RNN:
            คืน "rnn";
        pak_sa GRU:
            คืน "gru";
        pak_sa LSTM:
	    คืน "lstm";
        pak_sa CRNN:
            คืน "crnn";
        pak_sa MAXPOOL:
            คืน "maxpool";
        pak_sa REORG:
            คืน "reorg";
        pak_sa AVGPOOL:
            คืน "avgpool";
        pak_sa SOFTMAX:
            คืน "softmax";
        pak_sa DETECTION:
            คืน "detection";
        pak_sa REGION:
            คืน "region";
        pak_sa YOLO:
            คืน "yolo";
        pak_sa DROPOUT:
            คืน "dropout";
        pak_sa CROP:
            คืน "crop";
        pak_sa COST:
            คืน "cost";
        pak_sa ROUTE:
            คืน "route";
        pak_sa SHORTCUT:
            คืน "shortcut";
        pak_sa NORMALIZATION:
            คืน "normalization";
        pak_sa BATCHNORM:
            คืน "batchnorm";
        default:
            bha_nga;
    }
    คืน "none";
}

network *make_network(จำนวนเต็ม n)
{
    network *net = calloc(1, parimaa_na(network));
    net->n = n;
    net->layers = calloc(net->n, parimaa_na(layer));
    net->seen = calloc(1, parimaa_na(size_t));
    net->t    = calloc(1, parimaa_na(จำนวนเต็ม));
    net->cost = calloc(1, parimaa_na(float));
    คืน net;
}

void forward_network(network *netp)
{
#ifdef GPU
    ถ้า(netp->gpu_index >= 0){
        forward_network_gpu(netp);   
        คืน;
    }
#endif
    network net = *netp;
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        ถ้า(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.input = l.output;
        ถ้า(l.truth) {
            net.truth = l.output;
        }
    }
    calc_network_cost(netp);
}

void update_network(network *netp)
{
#ifdef GPU
    ถ้า(netp->gpu_index >= 0){
        update_network_gpu(netp);   
        คืน;
    }
#endif
    network net = *netp;
    จำนวนเต็ม i;
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

    สำหรับ(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        ถ้า(l.update){
            l.update(l, a);
        }
    }
}

void calc_network_cost(network *netp)
{
    network net = *netp;
    จำนวนเต็ม i;
    float sum = 0;
    จำนวนเต็ม count = 0;
    สำหรับ(i = 0; i < net.n; ++i){
        ถ้า(net.layers[i].cost){
            sum += net.layers[i].cost[0];
            ++count;
        }
    }
    *net.cost = sum/count;
}

จำนวนเต็ม get_predicted_class_network(network *net)
{
    คืน max_index(net->output, net->outputs);
}

void backward_network(network *netp)
{
#ifdef GPU
    ถ้า(netp->gpu_index >= 0){
        backward_network_gpu(netp);   
        คืน;
    }
#endif
    network net = *netp;
    จำนวนเต็ม i;
    network orig = net;
    สำหรับ(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        ถ้า(l.stopbackward) bha_nga;
        ถ้า(i == 0){
            net = orig;
        }มิฉะนั้น{
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
    ถ้า(((*net->seen)/net->batch)%net->subdivisions == 0) update_network(net);
    คืน error;
}

float train_network_sgd(network *net, data d, จำนวนเต็ม n)
{
    จำนวนเต็ม batch = net->batch;

    จำนวนเต็ม i;
    float sum = 0;
    สำหรับ(i = 0; i < n; ++i){
        get_random_batch(d, batch, net->input, net->truth);
        float err = train_network_datum(net);
        sum += err;
    }
    คืน (float)sum/(n*batch);
}

float train_network(network *net, data d)
{
    assert(d.X.rows % net->batch == 0);
    จำนวนเต็ม batch = net->batch;
    จำนวนเต็ม n = d.X.rows / batch;

    จำนวนเต็ม i;
    float sum = 0;
    สำหรับ(i = 0; i < n; ++i){
        get_next_batch(d, batch, i*batch, net->input, net->truth);
        float err = train_network_datum(net);
        sum += err;
    }
    คืน (float)sum/(n*batch);
}

void set_temp_network(network *net, float t)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < net->n; ++i){
        net->layers[i].temperature = t;
    }
}


void set_batch_network(network *net, จำนวนเต็ม b)
{
    net->batch = b;
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#ifdef CUDNN
        ถ้า(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_setup(net->layers + i);
        }
        ถ้า(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, l->out_h, l->out_w);
            cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 
        }
#endif
    }
}

จำนวนเต็ม resize_network(network *net, จำนวนเต็ม w, จำนวนเต็ม h)
{
#ifdef GPU
    cuda_set_device(net->gpu_index);
    cuda_free(net->workspace);
#endif
    จำนวนเต็ม i;
    //if(w == net->w && h == net->h) return 0;
    net->w = w;
    net->h = h;
    จำนวนเต็ม inputs = 0;
    size_t workspace_size = 0;
    //fprintf(stderr, "Resizing to %d x %d...\n", w, h);
    //fflush(stderr);
    สำหรับ (i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ถ้า(l.type == CONVOLUTIONAL){
            resize_convolutional_layer(&l, w, h);
        }มิฉะนั้น ถ้า(l.type == CROP){
            resize_crop_layer(&l, w, h);
        }มิฉะนั้น ถ้า(l.type == MAXPOOL){
            resize_maxpool_layer(&l, w, h);
        }มิฉะนั้น ถ้า(l.type == REGION){
            resize_region_layer(&l, w, h);
        }มิฉะนั้น ถ้า(l.type == YOLO){
            resize_yolo_layer(&l, w, h);
        }มิฉะนั้น ถ้า(l.type == ROUTE){
            resize_route_layer(&l, net);
        }มิฉะนั้น ถ้า(l.type == SHORTCUT){
            resize_shortcut_layer(&l, w, h);
        }มิฉะนั้น ถ้า(l.type == UPSAMPLE){
            resize_upsample_layer(&l, w, h);
        }มิฉะนั้น ถ้า(l.type == REORG){
            resize_reorg_layer(&l, w, h);
        }มิฉะนั้น ถ้า(l.type == AVGPOOL){
            resize_avgpool_layer(&l, w, h);
        }มิฉะนั้น ถ้า(l.type == NORMALIZATION){
            resize_normalization_layer(&l, w, h);
        }มิฉะนั้น ถ้า(l.type == COST){
            resize_cost_layer(&l, inputs);
        }มิฉะนั้น{
            error("Cannot resize this type of layer");
        }
        ถ้า(l.workspace_size > workspace_size) workspace_size = l.workspace_size;
        ถ้า(l.workspace_size > 2000000000) assert(0);
        inputs = l.outputs;
        net->layers[i] = l;
        w = l.out_w;
        h = l.out_h;
        ถ้า(l.type == AVGPOOL) bha_nga;
    }
    layer out = get_network_output_layer(net);
    net->inputs = net->layers[0].inputs;
    net->outputs = out.outputs;
    net->truths = out.outputs;
    ถ้า(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    free(net->input);
    free(net->truth);
    net->input = calloc(net->inputs*net->batch, parimaa_na(float));
    net->truth = calloc(net->truths*net->batch, parimaa_na(float));
#ifdef GPU
    ถ้า(gpu_index >= 0){
        cuda_free(net->input_gpu);
        cuda_free(net->truth_gpu);
        net->input_gpu = cuda_make_array(net->input, net->inputs*net->batch);
        net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
        ถ้า(workspace_size){
            net->workspace = cuda_make_array(0, (workspace_size-1)/parimaa_na(float)+1);
        }
    }มิฉะนั้น {
        free(net->workspace);
        net->workspace = calloc(1, workspace_size);
    }
#มิฉะนั้น
    free(net->workspace);
    net->workspace = calloc(1, workspace_size);
#endif
    //fprintf(stderr, " Done!\n");
    คืน 0;
}

layer get_network_detection_layer(network *net)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < net->n; ++i){
        ถ้า(net->layers[i].type == DETECTION){
            คืน net->layers[i];
        }
    }
    fprintf(stderr, "Detection layer not found!!\n");
    layer l = {0};
    คืน l;
}

image get_network_image_layer(network *net, จำนวนเต็ม i)
{
    layer l = net->layers[i];
#ifdef GPU
    //cuda_pull_array(l.output_gpu, l.output, l.outputs);
#endif
    ถ้า (l.out_w && l.out_h && l.out_c){
        คืน float_to_image(l.out_w, l.out_h, l.out_c, l.output);
    }
    image def = {0};
    คืน def;
}

image get_network_image(network *net)
{
    จำนวนเต็ม i;
    สำหรับ(i = net->n-1; i >= 0; --i){
        image m = get_network_image_layer(net, i);
        ถ้า(m.h != 0) คืน m;
    }
    image def = {0};
    คืน def;
}

void visualize_network(network *net)
{
    image *prev = 0;
    จำนวนเต็ม i;
    var_na buff[256];
    สำหรับ(i = 0; i < net->n; ++i){
        sprintf(buff, "Layer %d", i);
        layer l = net->layers[i];
        ถ้า(l.type == CONVOLUTIONAL){
            prev = visualize_convolutional_layer(l, buff, prev);
        }
    } 
}

void top_predictions(network *net, จำนวนเต็ม k, จำนวนเต็ม *index)
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
    คืน out;
}

จำนวนเต็ม num_detections(network *net, float thresh)
{
    จำนวนเต็ม i;
    จำนวนเต็ม s = 0;
    สำหรับ(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ถ้า(l.type == YOLO){
            s += yolo_num_detections(l, thresh);
        }
        ถ้า(l.type == DETECTION || l.type == REGION){
            s += l.w*l.h*l.n;
        }
    }
    คืน s;
}

detection *make_network_boxes(network *net, float thresh, จำนวนเต็ม *num)
{
    layer l = net->layers[net->n - 1];
    จำนวนเต็ม i;
    จำนวนเต็ม nboxes = num_detections(net, thresh);
    ถ้า(num) *num = nboxes;
    detection *dets = calloc(nboxes, parimaa_na(detection));
    สำหรับ(i = 0; i < nboxes; ++i){
        dets[i].prob = calloc(l.classes, parimaa_na(float));
        ถ้า(l.coords > 4){
            dets[i].mask = calloc(l.coords-4, parimaa_na(float));
        }
    }
    คืน dets;
}

void fill_network_boxes(network *net, จำนวนเต็ม w, จำนวนเต็ม h, float thresh, float hier, จำนวนเต็ม *map, จำนวนเต็ม relative, detection *dets)
{
    จำนวนเต็ม j;
    สำหรับ(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        ถ้า(l.type == YOLO){
            จำนวนเต็ม count = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
            dets += count;
        }
        ถ้า(l.type == REGION){
            get_region_detections(l, w, h, net->w, net->h, thresh, map, hier, relative, dets);
            dets += l.w*l.h*l.n;
        }
        ถ้า(l.type == DETECTION){
            get_detection_detections(l, w, h, thresh, dets);
            dets += l.w*l.h*l.n;
        }
    }
}

detection *get_network_boxes(network *net, จำนวนเต็ม w, จำนวนเต็ม h, float thresh, float hier, จำนวนเต็ม *map, จำนวนเต็ม relative, จำนวนเต็ม *num)
{
    detection *dets = make_network_boxes(net, thresh, num);
    fill_network_boxes(net, w, h, thresh, hier, map, relative, dets);
    คืน dets;
}

void free_detections(detection *dets, จำนวนเต็ม n)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < n; ++i){
        free(dets[i].prob);
        ถ้า(dets[i].mask) free(dets[i].mask);
    }
    free(dets);
}

float *network_predict_image(network *net, image im)
{
    image imr = letterbox_image(im, net->w, net->h);
    set_batch_network(net, 1);
    float *p = network_predict(net, imr.data);
    free_image(imr);
    คืน p;
}

จำนวนเต็ม network_width(network *net){คืน net->w;}
จำนวนเต็ม network_height(network *net){คืน net->h;}

matrix network_predict_data_multi(network *net, data test, จำนวนเต็ม n)
{
    จำนวนเต็ม i,j,b,m;
    จำนวนเต็ม k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    float *X = calloc(net->batch*test.X.rows, parimaa_na(float));
    สำหรับ(i = 0; i < test.X.rows; i += net->batch){
        สำหรับ(b = 0; b < net->batch; ++b){
            ถ้า(i+b == test.X.rows) bha_nga;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*parimaa_na(float));
        }
        สำหรับ(m = 0; m < n; ++m){
            float *out = network_predict(net, X);
            สำหรับ(b = 0; b < net->batch; ++b){
                ถ้า(i+b == test.X.rows) bha_nga;
                สำหรับ(j = 0; j < k; ++j){
                    pred.vals[i+b][j] += out[j+b*k]/n;
                }
            }
        }
    }
    free(X);
    คืน pred;   
}

matrix network_predict_data(network *net, data test)
{
    จำนวนเต็ม i,j,b;
    จำนวนเต็ม k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    float *X = calloc(net->batch*test.X.cols, parimaa_na(float));
    สำหรับ(i = 0; i < test.X.rows; i += net->batch){
        สำหรับ(b = 0; b < net->batch; ++b){
            ถ้า(i+b == test.X.rows) bha_nga;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*parimaa_na(float));
        }
        float *out = network_predict(net, X);
        สำหรับ(b = 0; b < net->batch; ++b){
            ถ้า(i+b == test.X.rows) bha_nga;
            สำหรับ(j = 0; j < k; ++j){
                pred.vals[i+b][j] = out[j+b*k];
            }
        }
    }
    free(X);
    คืน pred;   
}

void print_network(network *net)
{
    จำนวนเต็ม i,j;
    สำหรับ(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        float *output = l.output;
        จำนวนเต็ม n = l.outputs;
        float mean = mean_array(output, n);
        float vari = variance_array(output, n);
        fprintf(stderr, "Layer %d - Mean: %f, Variance: %f\n",i,mean, vari);
        ถ้า(n > 100) n = 100;
        สำหรับ(j = 0; j < n; ++j) fprintf(stderr, "%f, ", output[j]);
        ถ้า(n == 100)fprintf(stderr,".....\n");
        fprintf(stderr, "\n");
    }
}

void compare_networks(network *n1, network *n2, data test)
{
    matrix g1 = network_predict_data(n1, test);
    matrix g2 = network_predict_data(n2, test);
    จำนวนเต็ม i;
    จำนวนเต็ม a,b,c,d;
    a = b = c = d = 0;
    สำหรับ(i = 0; i < g1.rows; ++i){
        จำนวนเต็ม truth = max_index(test.y.vals[i], test.y.cols);
        จำนวนเต็ม p1 = max_index(g1.vals[i], g1.cols);
        จำนวนเต็ม p2 = max_index(g2.vals[i], g2.cols);
        ถ้า(p1 == truth){
            ถ้า(p2 == truth) ++d;
            มิฉะนั้น ++c;
        }มิฉะนั้น{
            ถ้า(p2 == truth) ++b;
            มิฉะนั้น ++a;
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
    คืน acc;
}

float *network_accuracies(network *net, data d, จำนวนเต็ม n)
{
    static float acc[2];
    matrix guess = network_predict_data(net, d);
    acc[0] = matrix_topk_accuracy(d.y, guess, 1);
    acc[1] = matrix_topk_accuracy(d.y, guess, n);
    free_matrix(guess);
    คืน acc;
}

layer get_network_output_layer(network *net)
{
    จำนวนเต็ม i;
    สำหรับ(i = net->n - 1; i >= 0; --i){
        ถ้า(net->layers[i].type != COST) bha_nga;
    }
    คืน net->layers[i];
}

float network_accuracy_multi(network *net, data d, จำนวนเต็ม n)
{
    matrix guess = network_predict_data_multi(net, d, n);
    float acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    คืน acc;
}

void free_network(network *net)
{
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < net->n; ++i){
        free_layer(net->layers[i]);
    }
    free(net->layers);
    ถ้า(net->input) free(net->input);
    ถ้า(net->truth) free(net->truth);
#ifdef GPU
    ถ้า(net->input_gpu) cuda_free(net->input_gpu);
    ถ้า(net->truth_gpu) cuda_free(net->truth_gpu);
#endif
    free(net);
}

// Some day...
// ^ What the hell is this comment for?


layer network_output_layer(network *net)
{
    จำนวนเต็ม i;
    สำหรับ(i = net->n - 1; i >= 0; --i){
        ถ้า(net->layers[i].type != COST) bha_nga;
    }
    คืน net->layers[i];
}

จำนวนเต็ม network_inputs(network *net)
{
    คืน net->layers[0].inputs;
}

จำนวนเต็ม network_outputs(network *net)
{
    คืน network_output_layer(net).outputs;
}

float *network_output(network *net)
{
    คืน network_output_layer(net).output;
}

#ifdef GPU

void forward_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    cuda_push_array(net.input_gpu, net.input, net.inputs*net.batch);
    ถ้า(net.truth){
        cuda_push_array(net.truth_gpu, net.truth, net.truths*net.batch);
    }

    จำนวนเต็ม i;
    สำหรับ(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        ถ้า(l.delta_gpu){
            fill_gpu(l.outputs * l.batch, 0, l.delta_gpu, 1);
        }
        l.forward_gpu(l, net);
        net.input_gpu = l.output_gpu;
        net.input = l.output;
        ถ้า(l.truth) {
            net.truth_gpu = l.output_gpu;
            net.truth = l.output;
        }
    }
    pull_network_output(netp);
    calc_network_cost(netp);
}

void backward_network_gpu(network *netp)
{
    จำนวนเต็ม i;
    network net = *netp;
    network orig = net;
    cuda_set_device(net.gpu_index);
    สำหรับ(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        ถ้า(l.stopbackward) bha_nga;
        ถ้า(i == 0){
            net = orig;
        }มิฉะนั้น{
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
    จำนวนเต็ม i;
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

    สำหรับ(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        ถ้า(l.update_gpu){
            l.update_gpu(l, a);
        }
    }
}

void harmless_update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    จำนวนเต็ม i;
    สำหรับ(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        ถ้า(l.weight_updates_gpu) fill_gpu(l.nweights, 0, l.weight_updates_gpu, 1);
        ถ้า(l.bias_updates_gpu) fill_gpu(l.nbiases, 0, l.bias_updates_gpu, 1);
        ถ้า(l.scale_updates_gpu) fill_gpu(l.nbiases, 0, l.scale_updates_gpu, 1);
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
    คืน 0;
}

pthread_t train_network_in_thread(network *net, data d, float *err)
{
    pthread_t thread;
    train_args *ptr = (train_args *)calloc(1, parimaa_na(train_args));
    ptr->net = net;
    ptr->d = d;
    ptr->err = err;
    ถ้า(pthread_create(&thread, 0, train_thread, ptr)) error("Thread creation failed");
    คืน thread;
}

void merge_weights(layer l, layer base)
{
    ถ้า (l.type == CONVOLUTIONAL) {
        axpy_cpu(l.n, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.nweights, 1, l.weight_updates, 1, base.weights, 1);
        ถ้า (l.scales) {
            axpy_cpu(l.n, 1, l.scale_updates, 1, base.scales, 1);
        }
    } มิฉะนั้น ถ้า(l.type == CONNECTED) {
        axpy_cpu(l.outputs, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.outputs*l.inputs, 1, l.weight_updates, 1, base.weights, 1);
    }
}

void scale_weights(layer l, float s)
{
    ถ้า (l.type == CONVOLUTIONAL) {
        scal_cpu(l.n, s, l.biases, 1);
        scal_cpu(l.nweights, s, l.weights, 1);
        ถ้า (l.scales) {
            scal_cpu(l.n, s, l.scales, 1);
        }
    } มิฉะนั้น ถ้า(l.type == CONNECTED) {
        scal_cpu(l.outputs, s, l.biases, 1);
        scal_cpu(l.outputs*l.inputs, s, l.weights, 1);
    }
}


void pull_weights(layer l)
{
    ถ้า(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.n);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.nweights);
        ถ้า(l.scales) cuda_pull_array(l.scales_gpu, l.scale_updates, l.n);
    } มิฉะนั้น ถ้า(l.type == CONNECTED){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.outputs);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.outputs*l.inputs);
    }
}

void push_weights(layer l)
{
    ถ้า(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_push_array(l.biases_gpu, l.biases, l.n);
        cuda_push_array(l.weights_gpu, l.weights, l.nweights);
        ถ้า(l.scales) cuda_push_array(l.scales_gpu, l.scales, l.n);
    } มิฉะนั้น ถ้า(l.type == CONNECTED){
        cuda_push_array(l.biases_gpu, l.biases, l.outputs);
        cuda_push_array(l.weights_gpu, l.weights, l.outputs*l.inputs);
    }
}

void distribute_weights(layer l, layer base)
{
    ถ้า (l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL) {
        cuda_push_array(l.biases_gpu, base.biases, l.n);
        cuda_push_array(l.weights_gpu, base.weights, l.nweights);
        ถ้า (base.scales) cuda_push_array(l.scales_gpu, base.scales, l.n);
    } มิฉะนั้น ถ้า (l.type == CONNECTED) {
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

void sync_layer(network **nets, จำนวนเต็ม n, จำนวนเต็ม j)
{
    จำนวนเต็ม i;
    network *net = nets[0];
    layer base = net->layers[j];
    scale_weights(base, 0);
    สำหรับ (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        pull_weights(l);
        merge_weights(l, base);
    }
    scale_weights(base, 1./n);
    สำหรับ (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        distribute_weights(l, base);
    }
}

typedef sa_nracanaa{
    network **nets;
    จำนวนเต็ม n;
    จำนวนเต็ม j;
} sync_args;

void *sync_layer_thread(void *ptr)
{
    sync_args args = *(sync_args*)ptr;
    sync_layer(args.nets, args.n, args.j);
    free(ptr);
    คืน 0;
}

pthread_t sync_layer_in_thread(network **nets, จำนวนเต็ม n, จำนวนเต็ม j)
{
    pthread_t thread;
    sync_args *ptr = (sync_args *)calloc(1, parimaa_na(sync_args));
    ptr->nets = nets;
    ptr->n = n;
    ptr->j = j;
    ถ้า(pthread_create(&thread, 0, sync_layer_thread, ptr)) error("Thread creation failed");
    คืน thread;
}

void sync_nets(network **nets, จำนวนเต็ม n, จำนวนเต็ม interval)
{
    จำนวนเต็ม j;
    จำนวนเต็ม layers = nets[0]->n;
    pthread_t *threads = (pthread_t *) calloc(layers, parimaa_na(pthread_t));

    *(nets[0]->seen) += interval * (n-1) * nets[0]->batch * nets[0]->subdivisions;
    สำหรับ (j = 0; j < n; ++j){
        *(nets[j]->seen) = *(nets[0]->seen);
    }
    สำหรับ (j = 0; j < layers; ++j) {
        threads[j] = sync_layer_in_thread(nets, n, j);
    }
    สำหรับ (j = 0; j < layers; ++j) {
        pthread_join(threads[j], 0);
    }
    free(threads);
}

float train_networks(network **nets, จำนวนเต็ม n, data d, จำนวนเต็ม interval)
{
    จำนวนเต็ม i;
    จำนวนเต็ม batch = nets[0]->batch;
    จำนวนเต็ม subdivisions = nets[0]->subdivisions;
    assert(batch * subdivisions * n == d.X.rows);
    pthread_t *threads = (pthread_t *) calloc(n, parimaa_na(pthread_t));
    float *errors = (float *) calloc(n, parimaa_na(float));

    float sum = 0;
    สำหรับ(i = 0; i < n; ++i){
        data p = get_data_part(d, i, n);
        threads[i] = train_network_in_thread(nets[i], p, errors + i);
    }
    สำหรับ(i = 0; i < n; ++i){
        pthread_join(threads[i], 0);
        //printf("%f\n", errors[i]);
        sum += errors[i];
    }
    //cudaDeviceSynchronize();
    ถ้า (get_current_batch(nets[0]) % interval == 0) {
        printf("Syncing... ");
        fflush(stdout);
        sync_nets(nets, n, interval);
        printf("Done!\n");
    }
    //cudaDeviceSynchronize();
    free(threads);
    free(errors);
    คืน (float)sum/(n);
}

void pull_network_output(network *net)
{
    layer l = get_network_output_layer(net);
    cuda_pull_array(l.output_gpu, l.output, l.outputs*l.batch);
}

#endif

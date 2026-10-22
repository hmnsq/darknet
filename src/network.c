#अन्तर्भाव <stdio.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव <assert.h>
#अन्तर्भाव "network.h"
#अन्तर्भाव "image.h"
#अन्तर्भाव "data.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "blas.h"

#अन्तर्भाव "crop_layer.h"
#अन्तर्भाव "connected_layer.h"
#अन्तर्भाव "gru_layer.h"
#अन्तर्भाव "rnn_layer.h"
#अन्तर्भाव "crnn_layer.h"
#अन्तर्भाव "local_layer.h"
#अन्तर्भाव "convolutional_layer.h"
#अन्तर्भाव "activation_layer.h"
#अन्तर्भाव "detection_layer.h"
#अन्तर्भाव "region_layer.h"
#अन्तर्भाव "yolo_layer.h"
#अन्तर्भाव "normalization_layer.h"
#अन्तर्भाव "batchnorm_layer.h"
#अन्तर्भाव "maxpool_layer.h"
#अन्तर्भाव "reorg_layer.h"
#अन्तर्भाव "avgpool_layer.h"
#अन्तर्भाव "cost_layer.h"
#अन्तर्भाव "softmax_layer.h"
#अन्तर्भाव "dropout_layer.h"
#अन्तर्भाव "route_layer.h"
#अन्तर्भाव "upsample_layer.h"
#अन्तर्भाव "shortcut_layer.h"
#अन्तर्भाव "parser.h"
#अन्तर्भाव "data.h"

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
    प्रत्यागम args;
}

network *load_network(वर्ण *cfg, वर्ण *weights, पूर्णाङ्क clear)
{
    network *net = parse_network_cfg(cfg);
    जइ(weights && weights[0] != 0){
        load_weights(net, weights);
    }
    जइ(clear) (*net->seen) = 0;
    प्रत्यागम net;
}

size_t get_current_batch(network *net)
{
    size_t batch_अङ्कः = (*net->seen)/(net->batch*net->subdivisions);
    प्रत्यागम batch_अङ्कः;
}

शून्य reset_network_state(network *net, पूर्णाङ्क b)
{
    पूर्णाङ्क i;
    प्रति (i = 0; i < net->n; ++i) {
        #ifdef GPU
        layer l = net->layers[i];
        जइ(l.state_gpu){
            fill_gpu(l.outputs, 0, l.state_gpu + l.outputs*b, 1);
        }
        जइ(l.h_gpu){
            fill_gpu(l.outputs, 0, l.h_gpu + l.outputs*b, 1);
        }
        #endif
    }
}

शून्य reset_rnn(network *net)
{
    reset_network_state(net, 0);
}

भिन्न get_current_rate(network *net)
{
    size_t batch_अङ्कः = get_current_batch(net);
    पूर्णाङ्क i;
    भिन्न rate;
    जइ (batch_अङ्कः < net->burn_in) प्रत्यागम net->learning_rate * pow((भिन्न)batch_अङ्कः / net->burn_in, net->power);
    विकल्प (net->policy) {
        पक्ष CONSTANT:
            प्रत्यागम net->learning_rate;
        पक्ष STEP:
            प्रत्यागम net->learning_rate * pow(net->scale, batch_अङ्कः/net->step);
        पक्ष STEPS:
            rate = net->learning_rate;
            प्रति(i = 0; i < net->अङ्कः_steps; ++i){
                जइ(net->steps[i] > batch_अङ्कः) प्रत्यागम rate;
                rate *= net->scales[i];
            }
            प्रत्यागम rate;
        पक्ष EXP:
            प्रत्यागम net->learning_rate * pow(net->gamma, batch_अङ्कः);
        पक्ष POLY:
            प्रत्यागम net->learning_rate * pow(1 - (भिन्न)batch_अङ्कः / net->max_batches, net->power);
        पक्ष RANDOM:
            प्रत्यागम net->learning_rate * pow(rand_uniform(0,1), net->power);
        पक्ष SIG:
            प्रत्यागम net->learning_rate * (1./(1.+exp(net->gamma*(batch_अङ्कः - net->step))));
        स्वभाव:
            fprintf(stderr, "Policy is weird!\n");
            प्रत्यागम net->learning_rate;
    }
}

वर्ण *get_layer_string(LAYER_TYPE a)
{
    विकल्प(a){
        पक्ष CONVOLUTIONAL:
            प्रत्यागम "convolutional";
        पक्ष ACTIVE:
            प्रत्यागम "activation";
        पक्ष LOCAL:
            प्रत्यागम "local";
        पक्ष DECONVOLUTIONAL:
            प्रत्यागम "deconvolutional";
        पक्ष CONNECTED:
            प्रत्यागम "connected";
        पक्ष RNN:
            प्रत्यागम "rnn";
        पक्ष GRU:
            प्रत्यागम "gru";
        पक्ष LSTM:
	    प्रत्यागम "lstm";
        पक्ष CRNN:
            प्रत्यागम "crnn";
        पक्ष MAXPOOL:
            प्रत्यागम "maxpool";
        पक्ष REORG:
            प्रत्यागम "reorg";
        पक्ष AVGPOOL:
            प्रत्यागम "avgpool";
        पक्ष SOFTMAX:
            प्रत्यागम "softmax";
        पक्ष DETECTION:
            प्रत्यागम "detection";
        पक्ष REGION:
            प्रत्यागम "region";
        पक्ष YOLO:
            प्रत्यागम "yolo";
        पक्ष DROPOUT:
            प्रत्यागम "dropout";
        पक्ष CROP:
            प्रत्यागम "crop";
        पक्ष COST:
            प्रत्यागम "cost";
        पक्ष ROUTE:
            प्रत्यागम "route";
        पक्ष SHORTCUT:
            प्रत्यागम "shortcut";
        पक्ष NORMALIZATION:
            प्रत्यागम "normalization";
        पक्ष BATCHNORM:
            प्रत्यागम "batchnorm";
        स्वभाव:
            भङ्ग;
    }
    प्रत्यागम "none";
}

network *make_network(पूर्णाङ्क n)
{
    network *net = calloc(1, परिमाण(network));
    net->n = n;
    net->layers = calloc(net->n, परिमाण(layer));
    net->seen = calloc(1, परिमाण(size_t));
    net->t    = calloc(1, परिमाण(पूर्णाङ्क));
    net->cost = calloc(1, परिमाण(भिन्न));
    प्रत्यागम net;
}

शून्य forward_network(network *netp)
{
#ifdef GPU
    जइ(netp->gpu_index >= 0){
        forward_network_gpu(netp);   
        प्रत्यागम;
    }
#endif
    network net = *netp;
    पूर्णाङ्क i;
    प्रति(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        जइ(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.निवेशः = l.output;
        जइ(l.truth) {
            net.truth = l.output;
        }
    }
    calc_network_cost(netp);
}

शून्य update_network(network *netp)
{
#ifdef GPU
    जइ(netp->gpu_index >= 0){
        update_network_gpu(netp);   
        प्रत्यागम;
    }
#endif
    network net = *netp;
    पूर्णाङ्क i;
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

    प्रति(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        जइ(l.update){
            l.update(l, a);
        }
    }
}

शून्य calc_network_cost(network *netp)
{
    network net = *netp;
    पूर्णाङ्क i;
    भिन्न sum = 0;
    पूर्णाङ्क गणनम् = 0;
    प्रति(i = 0; i < net.n; ++i){
        जइ(net.layers[i].cost){
            sum += net.layers[i].cost[0];
            ++गणनम्;
        }
    }
    *net.cost = sum/गणनम्;
}

पूर्णाङ्क get_predicted_class_network(network *net)
{
    प्रत्यागम max_index(net->output, net->outputs);
}

शून्य backward_network(network *netp)
{
#ifdef GPU
    जइ(netp->gpu_index >= 0){
        backward_network_gpu(netp);   
        प्रत्यागम;
    }
#endif
    network net = *netp;
    पूर्णाङ्क i;
    network orig = net;
    प्रति(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        जइ(l.stopbackward) भङ्ग;
        जइ(i == 0){
            net = orig;
        }अण्णहा{
            layer prev = net.layers[i-1];
            net.निवेशः = prev.output;
            net.delta = prev.delta;
        }
        net.index = i;
        l.backward(l, net);
    }
}

भिन्न train_network_datum(network *net)
{
    *net->seen += net->batch;
    net->train = 1;
    forward_network(net);
    backward_network(net);
    भिन्न error = *net->cost;
    जइ(((*net->seen)/net->batch)%net->subdivisions == 0) update_network(net);
    प्रत्यागम error;
}

भिन्न train_network_sgd(network *net, data d, पूर्णाङ्क n)
{
    पूर्णाङ्क batch = net->batch;

    पूर्णाङ्क i;
    भिन्न sum = 0;
    प्रति(i = 0; i < n; ++i){
        get_random_batch(d, batch, net->निवेशः, net->truth);
        भिन्न err = train_network_datum(net);
        sum += err;
    }
    प्रत्यागम (भिन्न)sum/(n*batch);
}

भिन्न train_network(network *net, data d)
{
    प्रतिज्ञा(d.X.rows % net->batch == 0);
    पूर्णाङ्क batch = net->batch;
    पूर्णाङ्क n = d.X.rows / batch;

    पूर्णाङ्क i;
    भिन्न sum = 0;
    प्रति(i = 0; i < n; ++i){
        get_next_batch(d, batch, i*batch, net->निवेशः, net->truth);
        भिन्न err = train_network_datum(net);
        sum += err;
    }
    प्रत्यागम (भिन्न)sum/(n*batch);
}

शून्य set_temp_network(network *net, भिन्न t)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < net->n; ++i){
        net->layers[i].temperature = t;
    }
}


शून्य set_batch_network(network *net, पूर्णाङ्क b)
{
    net->batch = b;
    पूर्णाङ्क i;
    प्रति(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#ifdef CUDNN
        जइ(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_setup(net->layers + i);
        }
        जइ(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, l->out_h, l->out_w);
            cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 
        }
#endif
    }
}

पूर्णाङ्क resize_network(network *net, पूर्णाङ्क w, पूर्णाङ्क h)
{
#ifdef GPU
    cuda_set_device(net->gpu_index);
    cuda_free(net->workspace);
#endif
    पूर्णाङ्क i;
    //if(w == net->w && h == net->h) return 0;
    net->w = w;
    net->h = h;
    पूर्णाङ्क inputs = 0;
    size_t workspace_size = 0;
    //fprintf(stderr, "Resizing to %d x %d...\n", w, h);
    //fflush(stderr);
    प्रति (i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        जइ(l.type == CONVOLUTIONAL){
            resize_convolutional_layer(&l, w, h);
        }अण्णहा जइ(l.type == CROP){
            resize_crop_layer(&l, w, h);
        }अण्णहा जइ(l.type == MAXPOOL){
            resize_maxpool_layer(&l, w, h);
        }अण्णहा जइ(l.type == REGION){
            resize_region_layer(&l, w, h);
        }अण्णहा जइ(l.type == YOLO){
            resize_yolo_layer(&l, w, h);
        }अण्णहा जइ(l.type == ROUTE){
            resize_route_layer(&l, net);
        }अण्णहा जइ(l.type == SHORTCUT){
            resize_shortcut_layer(&l, w, h);
        }अण्णहा जइ(l.type == UPSAMPLE){
            resize_upsample_layer(&l, w, h);
        }अण्णहा जइ(l.type == REORG){
            resize_reorg_layer(&l, w, h);
        }अण्णहा जइ(l.type == AVGPOOL){
            resize_avgpool_layer(&l, w, h);
        }अण्णहा जइ(l.type == NORMALIZATION){
            resize_normalization_layer(&l, w, h);
        }अण्णहा जइ(l.type == COST){
            resize_cost_layer(&l, inputs);
        }अण्णहा{
            error("Cannot resize this type of layer");
        }
        जइ(l.workspace_size > workspace_size) workspace_size = l.workspace_size;
        जइ(l.workspace_size > 2000000000) प्रतिज्ञा(0);
        inputs = l.outputs;
        net->layers[i] = l;
        w = l.out_w;
        h = l.out_h;
        जइ(l.type == AVGPOOL) भङ्ग;
    }
    layer out = get_network_output_layer(net);
    net->inputs = net->layers[0].inputs;
    net->outputs = out.outputs;
    net->truths = out.outputs;
    जइ(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    मुक्त(net->निवेशः);
    मुक्त(net->truth);
    net->निवेशः = calloc(net->inputs*net->batch, परिमाण(भिन्न));
    net->truth = calloc(net->truths*net->batch, परिमाण(भिन्न));
#ifdef GPU
    जइ(gpu_index >= 0){
        cuda_free(net->निवेशः_gpu);
        cuda_free(net->truth_gpu);
        net->निवेशः_gpu = cuda_make_array(net->निवेशः, net->inputs*net->batch);
        net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
        जइ(workspace_size){
            net->workspace = cuda_make_array(0, (workspace_size-1)/परिमाण(भिन्न)+1);
        }
    }अण्णहा {
        मुक्त(net->workspace);
        net->workspace = calloc(1, workspace_size);
    }
#अण्णहा
    मुक्त(net->workspace);
    net->workspace = calloc(1, workspace_size);
#endif
    //fprintf(stderr, " Done!\n");
    प्रत्यागम 0;
}

layer get_network_detection_layer(network *net)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < net->n; ++i){
        जइ(net->layers[i].type == DETECTION){
            प्रत्यागम net->layers[i];
        }
    }
    fprintf(stderr, "Detection layer not found!!\n");
    layer l = {0};
    प्रत्यागम l;
}

image get_network_image_layer(network *net, पूर्णाङ्क i)
{
    layer l = net->layers[i];
#ifdef GPU
    //cuda_pull_array(l.output_gpu, l.output, l.outputs);
#endif
    जइ (l.out_w && l.out_h && l.out_c){
        प्रत्यागम float_to_image(l.out_w, l.out_h, l.out_c, l.output);
    }
    image def = {0};
    प्रत्यागम def;
}

image get_network_image(network *net)
{
    पूर्णाङ्क i;
    प्रति(i = net->n-1; i >= 0; --i){
        image m = get_network_image_layer(net, i);
        जइ(m.h != 0) प्रत्यागम m;
    }
    image def = {0};
    प्रत्यागम def;
}

शून्य visualize_network(network *net)
{
    image *prev = 0;
    पूर्णाङ्क i;
    वर्ण buff[256];
    प्रति(i = 0; i < net->n; ++i){
        sprintf(buff, "Layer %d", i);
        layer l = net->layers[i];
        जइ(l.type == CONVOLUTIONAL){
            prev = visualize_convolutional_layer(l, buff, prev);
        }
    } 
}

शून्य शिखरम्_predictions(network *net, पूर्णाङ्क k, पूर्णाङ्क *index)
{
    शिखरम्_k(net->output, net->outputs, k, index);
}


भिन्न *network_predict(network *net, भिन्न *निवेशः)
{
    network orig = *net;
    net->निवेशः = निवेशः;
    net->truth = 0;
    net->train = 0;
    net->delta = 0;
    forward_network(net);
    भिन्न *out = net->output;
    *net = orig;
    प्रत्यागम out;
}

पूर्णाङ्क अङ्कः_detections(network *net, भिन्न thresh)
{
    पूर्णाङ्क i;
    पूर्णाङ्क s = 0;
    प्रति(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        जइ(l.type == YOLO){
            s += yolo_अङ्कः_detections(l, thresh);
        }
        जइ(l.type == DETECTION || l.type == REGION){
            s += l.w*l.h*l.n;
        }
    }
    प्रत्यागम s;
}

detection *make_network_boxes(network *net, भिन्न thresh, पूर्णाङ्क *अङ्कः)
{
    layer l = net->layers[net->n - 1];
    पूर्णाङ्क i;
    पूर्णाङ्क nboxes = अङ्कः_detections(net, thresh);
    जइ(अङ्कः) *अङ्कः = nboxes;
    detection *dets = calloc(nboxes, परिमाण(detection));
    प्रति(i = 0; i < nboxes; ++i){
        dets[i].prob = calloc(l.classes, परिमाण(भिन्न));
        जइ(l.coords > 4){
            dets[i].mask = calloc(l.coords-4, परिमाण(भिन्न));
        }
    }
    प्रत्यागम dets;
}

शून्य fill_network_boxes(network *net, पूर्णाङ्क w, पूर्णाङ्क h, भिन्न thresh, भिन्न hier, पूर्णाङ्क *map, पूर्णाङ्क relative, detection *dets)
{
    पूर्णाङ्क j;
    प्रति(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        जइ(l.type == YOLO){
            पूर्णाङ्क गणनम् = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
            dets += गणनम्;
        }
        जइ(l.type == REGION){
            get_region_detections(l, w, h, net->w, net->h, thresh, map, hier, relative, dets);
            dets += l.w*l.h*l.n;
        }
        जइ(l.type == DETECTION){
            get_detection_detections(l, w, h, thresh, dets);
            dets += l.w*l.h*l.n;
        }
    }
}

detection *get_network_boxes(network *net, पूर्णाङ्क w, पूर्णाङ्क h, भिन्न thresh, भिन्न hier, पूर्णाङ्क *map, पूर्णाङ्क relative, पूर्णाङ्क *अङ्कः)
{
    detection *dets = make_network_boxes(net, thresh, अङ्कः);
    fill_network_boxes(net, w, h, thresh, hier, map, relative, dets);
    प्रत्यागम dets;
}

शून्य free_detections(detection *dets, पूर्णाङ्क n)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        मुक्त(dets[i].prob);
        जइ(dets[i].mask) मुक्त(dets[i].mask);
    }
    मुक्त(dets);
}

भिन्न *network_predict_image(network *net, image im)
{
    image imr = letterbox_image(im, net->w, net->h);
    set_batch_network(net, 1);
    भिन्न *p = network_predict(net, imr.data);
    free_image(imr);
    प्रत्यागम p;
}

पूर्णाङ्क network_width(network *net){प्रत्यागम net->w;}
पूर्णाङ्क network_height(network *net){प्रत्यागम net->h;}

matrix network_predict_data_multi(network *net, data test, पूर्णाङ्क n)
{
    पूर्णाङ्क i,j,b,m;
    पूर्णाङ्क k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    भिन्न *X = calloc(net->batch*test.X.rows, परिमाण(भिन्न));
    प्रति(i = 0; i < test.X.rows; i += net->batch){
        प्रति(b = 0; b < net->batch; ++b){
            जइ(i+b == test.X.rows) भङ्ग;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*परिमाण(भिन्न));
        }
        प्रति(m = 0; m < n; ++m){
            भिन्न *out = network_predict(net, X);
            प्रति(b = 0; b < net->batch; ++b){
                जइ(i+b == test.X.rows) भङ्ग;
                प्रति(j = 0; j < k; ++j){
                    pred.vals[i+b][j] += out[j+b*k]/n;
                }
            }
        }
    }
    मुक्त(X);
    प्रत्यागम pred;   
}

matrix network_predict_data(network *net, data test)
{
    पूर्णाङ्क i,j,b;
    पूर्णाङ्क k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    भिन्न *X = calloc(net->batch*test.X.cols, परिमाण(भिन्न));
    प्रति(i = 0; i < test.X.rows; i += net->batch){
        प्रति(b = 0; b < net->batch; ++b){
            जइ(i+b == test.X.rows) भङ्ग;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*परिमाण(भिन्न));
        }
        भिन्न *out = network_predict(net, X);
        प्रति(b = 0; b < net->batch; ++b){
            जइ(i+b == test.X.rows) भङ्ग;
            प्रति(j = 0; j < k; ++j){
                pred.vals[i+b][j] = out[j+b*k];
            }
        }
    }
    मुक्त(X);
    प्रत्यागम pred;   
}

शून्य print_network(network *net)
{
    पूर्णाङ्क i,j;
    प्रति(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        भिन्न *output = l.output;
        पूर्णाङ्क n = l.outputs;
        भिन्न mean = mean_array(output, n);
        भिन्न vari = variance_array(output, n);
        fprintf(stderr, "Layer %d - Mean: %f, Variance: %f\n",i,mean, vari);
        जइ(n > 100) n = 100;
        प्रति(j = 0; j < n; ++j) fprintf(stderr, "%f, ", output[j]);
        जइ(n == 100)fprintf(stderr,".....\n");
        fprintf(stderr, "\n");
    }
}

शून्य compare_networks(network *n1, network *n2, data test)
{
    matrix g1 = network_predict_data(n1, test);
    matrix g2 = network_predict_data(n2, test);
    पूर्णाङ्क i;
    पूर्णाङ्क a,b,c,d;
    a = b = c = d = 0;
    प्रति(i = 0; i < g1.rows; ++i){
        पूर्णाङ्क truth = max_index(test.y.vals[i], test.y.cols);
        पूर्णाङ्क p1 = max_index(g1.vals[i], g1.cols);
        पूर्णाङ्क p2 = max_index(g2.vals[i], g2.cols);
        जइ(p1 == truth){
            जइ(p2 == truth) ++d;
            अण्णहा ++c;
        }अण्णहा{
            जइ(p2 == truth) ++b;
            अण्णहा ++a;
        }
    }
    printf("%5d %5d\n%5d %5d\n", a, b, c, d);
    भिन्न अङ्कः = pow((abs(b - c) - 1.), 2.);
    भिन्न den = b + c;
    printf("%f\n", अङ्कः/den); 
}

भिन्न network_accuracy(network *net, data d)
{
    matrix guess = network_predict_data(net, d);
    भिन्न acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    प्रत्यागम acc;
}

भिन्न *network_accuracies(network *net, data d, पूर्णाङ्क n)
{
    स्थिर भिन्न acc[2];
    matrix guess = network_predict_data(net, d);
    acc[0] = matrix_topk_accuracy(d.y, guess, 1);
    acc[1] = matrix_topk_accuracy(d.y, guess, n);
    free_matrix(guess);
    प्रत्यागम acc;
}

layer get_network_output_layer(network *net)
{
    पूर्णाङ्क i;
    प्रति(i = net->n - 1; i >= 0; --i){
        जइ(net->layers[i].type != COST) भङ्ग;
    }
    प्रत्यागम net->layers[i];
}

भिन्न network_accuracy_multi(network *net, data d, पूर्णाङ्क n)
{
    matrix guess = network_predict_data_multi(net, d, n);
    भिन्न acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    प्रत्यागम acc;
}

शून्य free_network(network *net)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < net->n; ++i){
        free_layer(net->layers[i]);
    }
    मुक्त(net->layers);
    जइ(net->निवेशः) मुक्त(net->निवेशः);
    जइ(net->truth) मुक्त(net->truth);
#ifdef GPU
    जइ(net->निवेशः_gpu) cuda_free(net->निवेशः_gpu);
    जइ(net->truth_gpu) cuda_free(net->truth_gpu);
#endif
    मुक्त(net);
}

// Some day...
// ^ What the hell is this comment for?


layer network_output_layer(network *net)
{
    पूर्णाङ्क i;
    प्रति(i = net->n - 1; i >= 0; --i){
        जइ(net->layers[i].type != COST) भङ्ग;
    }
    प्रत्यागम net->layers[i];
}

पूर्णाङ्क network_inputs(network *net)
{
    प्रत्यागम net->layers[0].inputs;
}

पूर्णाङ्क network_outputs(network *net)
{
    प्रत्यागम network_output_layer(net).outputs;
}

भिन्न *network_output(network *net)
{
    प्रत्यागम network_output_layer(net).output;
}

#ifdef GPU

शून्य forward_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    cuda_प्रक्षिप_array(net.निवेशः_gpu, net.निवेशः, net.inputs*net.batch);
    जइ(net.truth){
        cuda_प्रक्षिप_array(net.truth_gpu, net.truth, net.truths*net.batch);
    }

    पूर्णाङ्क i;
    प्रति(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        जइ(l.delta_gpu){
            fill_gpu(l.outputs * l.batch, 0, l.delta_gpu, 1);
        }
        l.forward_gpu(l, net);
        net.निवेशः_gpu = l.output_gpu;
        net.निवेशः = l.output;
        जइ(l.truth) {
            net.truth_gpu = l.output_gpu;
            net.truth = l.output;
        }
    }
    pull_network_output(netp);
    calc_network_cost(netp);
}

शून्य backward_network_gpu(network *netp)
{
    पूर्णाङ्क i;
    network net = *netp;
    network orig = net;
    cuda_set_device(net.gpu_index);
    प्रति(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        जइ(l.stopbackward) भङ्ग;
        जइ(i == 0){
            net = orig;
        }अण्णहा{
            layer prev = net.layers[i-1];
            net.निवेशः = prev.output;
            net.delta = prev.delta;
            net.निवेशः_gpu = prev.output_gpu;
            net.delta_gpu = prev.delta_gpu;
        }
        net.index = i;
        l.backward_gpu(l, net);
    }
}

शून्य update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    पूर्णाङ्क i;
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

    प्रति(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        जइ(l.update_gpu){
            l.update_gpu(l, a);
        }
    }
}

शून्य harmless_update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    पूर्णाङ्क i;
    प्रति(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        जइ(l.weight_updates_gpu) fill_gpu(l.nweights, 0, l.weight_updates_gpu, 1);
        जइ(l.bias_updates_gpu) fill_gpu(l.nbiases, 0, l.bias_updates_gpu, 1);
        जइ(l.scale_updates_gpu) fill_gpu(l.nbiases, 0, l.scale_updates_gpu, 1);
    }
}

प्रकारनिर्देश संरचना {
    network *net;
    data d;
    भिन्न *err;
} train_args;

शून्य *train_thread(शून्य *ptr)
{
    train_args args = *(train_args*)ptr;
    मुक्त(ptr);
    cuda_set_device(args.net->gpu_index);
    *args.err = train_network(args.net, args.d);
    प्रत्यागम 0;
}

pthread_t train_network_in_thread(network *net, data d, भिन्न *err)
{
    pthread_t thread;
    train_args *ptr = (train_args *)calloc(1, परिमाण(train_args));
    ptr->net = net;
    ptr->d = d;
    ptr->err = err;
    जइ(pthread_create(&thread, 0, train_thread, ptr)) error("Thread creation failed");
    प्रत्यागम thread;
}

शून्य merge_weights(layer l, layer base)
{
    जइ (l.type == CONVOLUTIONAL) {
        axpy_cpu(l.n, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.nweights, 1, l.weight_updates, 1, base.weights, 1);
        जइ (l.scales) {
            axpy_cpu(l.n, 1, l.scale_updates, 1, base.scales, 1);
        }
    } अण्णहा जइ(l.type == CONNECTED) {
        axpy_cpu(l.outputs, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.outputs*l.inputs, 1, l.weight_updates, 1, base.weights, 1);
    }
}

शून्य scale_weights(layer l, भिन्न s)
{
    जइ (l.type == CONVOLUTIONAL) {
        scal_cpu(l.n, s, l.biases, 1);
        scal_cpu(l.nweights, s, l.weights, 1);
        जइ (l.scales) {
            scal_cpu(l.n, s, l.scales, 1);
        }
    } अण्णहा जइ(l.type == CONNECTED) {
        scal_cpu(l.outputs, s, l.biases, 1);
        scal_cpu(l.outputs*l.inputs, s, l.weights, 1);
    }
}


शून्य pull_weights(layer l)
{
    जइ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.n);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.nweights);
        जइ(l.scales) cuda_pull_array(l.scales_gpu, l.scale_updates, l.n);
    } अण्णहा जइ(l.type == CONNECTED){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.outputs);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.outputs*l.inputs);
    }
}

शून्य प्रक्षिप_weights(layer l)
{
    जइ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_प्रक्षिप_array(l.biases_gpu, l.biases, l.n);
        cuda_प्रक्षिप_array(l.weights_gpu, l.weights, l.nweights);
        जइ(l.scales) cuda_प्रक्षिप_array(l.scales_gpu, l.scales, l.n);
    } अण्णहा जइ(l.type == CONNECTED){
        cuda_प्रक्षिप_array(l.biases_gpu, l.biases, l.outputs);
        cuda_प्रक्षिप_array(l.weights_gpu, l.weights, l.outputs*l.inputs);
    }
}

शून्य distribute_weights(layer l, layer base)
{
    जइ (l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL) {
        cuda_प्रक्षिप_array(l.biases_gpu, base.biases, l.n);
        cuda_प्रक्षिप_array(l.weights_gpu, base.weights, l.nweights);
        जइ (base.scales) cuda_प्रक्षिप_array(l.scales_gpu, base.scales, l.n);
    } अण्णहा जइ (l.type == CONNECTED) {
        cuda_प्रक्षिप_array(l.biases_gpu, base.biases, l.outputs);
        cuda_प्रक्षिप_array(l.weights_gpu, base.weights, l.outputs*l.inputs);
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

शून्य sync_layer(network **nets, पूर्णाङ्क n, पूर्णाङ्क j)
{
    पूर्णाङ्क i;
    network *net = nets[0];
    layer base = net->layers[j];
    scale_weights(base, 0);
    प्रति (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        pull_weights(l);
        merge_weights(l, base);
    }
    scale_weights(base, 1./n);
    प्रति (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        distribute_weights(l, base);
    }
}

प्रकारनिर्देश संरचना{
    network **nets;
    पूर्णाङ्क n;
    पूर्णाङ्क j;
} sync_args;

शून्य *sync_layer_thread(शून्य *ptr)
{
    sync_args args = *(sync_args*)ptr;
    sync_layer(args.nets, args.n, args.j);
    मुक्त(ptr);
    प्रत्यागम 0;
}

pthread_t sync_layer_in_thread(network **nets, पूर्णाङ्क n, पूर्णाङ्क j)
{
    pthread_t thread;
    sync_args *ptr = (sync_args *)calloc(1, परिमाण(sync_args));
    ptr->nets = nets;
    ptr->n = n;
    ptr->j = j;
    जइ(pthread_create(&thread, 0, sync_layer_thread, ptr)) error("Thread creation failed");
    प्रत्यागम thread;
}

शून्य sync_nets(network **nets, पूर्णाङ्क n, पूर्णाङ्क interval)
{
    पूर्णाङ्क j;
    पूर्णाङ्क layers = nets[0]->n;
    pthread_t *threads = (pthread_t *) calloc(layers, परिमाण(pthread_t));

    *(nets[0]->seen) += interval * (n-1) * nets[0]->batch * nets[0]->subdivisions;
    प्रति (j = 0; j < n; ++j){
        *(nets[j]->seen) = *(nets[0]->seen);
    }
    प्रति (j = 0; j < layers; ++j) {
        threads[j] = sync_layer_in_thread(nets, n, j);
    }
    प्रति (j = 0; j < layers; ++j) {
        pthread_join(threads[j], 0);
    }
    मुक्त(threads);
}

भिन्न train_networks(network **nets, पूर्णाङ्क n, data d, पूर्णाङ्क interval)
{
    पूर्णाङ्क i;
    पूर्णाङ्क batch = nets[0]->batch;
    पूर्णाङ्क subdivisions = nets[0]->subdivisions;
    प्रतिज्ञा(batch * subdivisions * n == d.X.rows);
    pthread_t *threads = (pthread_t *) calloc(n, परिमाण(pthread_t));
    भिन्न *errors = (भिन्न *) calloc(n, परिमाण(भिन्न));

    भिन्न sum = 0;
    प्रति(i = 0; i < n; ++i){
        data p = get_data_part(d, i, n);
        threads[i] = train_network_in_thread(nets[i], p, errors + i);
    }
    प्रति(i = 0; i < n; ++i){
        pthread_join(threads[i], 0);
        //printf("%f\n", errors[i]);
        sum += errors[i];
    }
    //cudaDeviceSynchronize();
    जइ (get_current_batch(nets[0]) % interval == 0) {
        printf("Syncing... ");
        fflush(stdout);
        sync_nets(nets, n, interval);
        printf("Done!\n");
    }
    //cudaDeviceSynchronize();
    मुक्त(threads);
    मुक्त(errors);
    प्रत्यागम (भिन्न)sum/(n);
}

शून्य pull_network_output(network *net)
{
    layer l = get_network_output_layer(net);
    cuda_pull_array(l.output_gpu, l.output, l.outputs*l.batch);
}

#endif

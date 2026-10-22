#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <time.h>
#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ "network.h"
#അന്തര്ഭാവ "image.h"
#അന്തര്ഭാവ "data.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "blas.h"

#അന്തര്ഭാവ "crop_layer.h"
#അന്തര്ഭാവ "connected_layer.h"
#അന്തര്ഭാവ "gru_layer.h"
#അന്തര്ഭാവ "rnn_layer.h"
#അന്തര്ഭാവ "crnn_layer.h"
#അന്തര്ഭാവ "local_layer.h"
#അന്തര്ഭാവ "convolutional_layer.h"
#അന്തര്ഭാവ "activation_layer.h"
#അന്തര്ഭാവ "detection_layer.h"
#അന്തര്ഭാവ "region_layer.h"
#അന്തര്ഭാവ "yolo_layer.h"
#അന്തര്ഭാവ "normalization_layer.h"
#അന്തര്ഭാവ "batchnorm_layer.h"
#അന്തര്ഭാവ "maxpool_layer.h"
#അന്തര്ഭാവ "reorg_layer.h"
#അന്തര്ഭാവ "avgpool_layer.h"
#അന്തര്ഭാവ "cost_layer.h"
#അന്തര്ഭാവ "softmax_layer.h"
#അന്തര്ഭാവ "dropout_layer.h"
#അന്തര്ഭാവ "route_layer.h"
#അന്തര്ഭാവ "upsample_layer.h"
#അന്തര്ഭാവ "shortcut_layer.h"
#അന്തര്ഭാവ "parser.h"
#അന്തര്ഭാവ "data.h"

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
    തിരികെ args;
}

network *load_network(അക്ഷരം *cfg, അക്ഷരം *weights, പൂർണ്ണസംഖ്യ clear)
{
    network *net = parse_network_cfg(cfg);
    എങ്കിൽ(weights && weights[0] != 0){
        load_weights(net, weights);
    }
    എങ്കിൽ(clear) (*net->seen) = 0;
    തിരികെ net;
}

size_t get_current_batch(network *net)
{
    size_t batch_അക്കം = (*net->seen)/(net->batch*net->subdivisions);
    തിരികെ batch_അക്കം;
}

ശൂന്യം reset_network_state(network *net, പൂർണ്ണസംഖ്യ b)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി (i = 0; i < net->n; ++i) {
        #ifdef GPU
        layer l = net->layers[i];
        എങ്കിൽ(l.state_gpu){
            fill_gpu(l.outputs, 0, l.state_gpu + l.outputs*b, 1);
        }
        എങ്കിൽ(l.h_gpu){
            fill_gpu(l.outputs, 0, l.h_gpu + l.outputs*b, 1);
        }
        #endif
    }
}

ശൂന്യം reset_rnn(network *net)
{
    reset_network_state(net, 0);
}

ദശാംശം get_current_rate(network *net)
{
    size_t batch_അക്കം = get_current_batch(net);
    പൂർണ്ണസംഖ്യ i;
    ദശാംശം rate;
    എങ്കിൽ (batch_അക്കം < net->burn_in) തിരികെ net->learning_rate * pow((ദശാംശം)batch_അക്കം / net->burn_in, net->power);
    തിരഞ്ഞെടുപ്പ് (net->policy) {
        അവസ്ഥ CONSTANT:
            തിരികെ net->learning_rate;
        അവസ്ഥ STEP:
            തിരികെ net->learning_rate * pow(net->scale, batch_അക്കം/net->step);
        അവസ്ഥ STEPS:
            rate = net->learning_rate;
            വേണ്ടി(i = 0; i < net->അക്കം_steps; ++i){
                എങ്കിൽ(net->steps[i] > batch_അക്കം) തിരികെ rate;
                rate *= net->scales[i];
            }
            തിരികെ rate;
        അവസ്ഥ EXP:
            തിരികെ net->learning_rate * pow(net->gamma, batch_അക്കം);
        അവസ്ഥ POLY:
            തിരികെ net->learning_rate * pow(1 - (ദശാംശം)batch_അക്കം / net->max_batches, net->power);
        അവസ്ഥ RANDOM:
            തിരികെ net->learning_rate * pow(rand_uniform(0,1), net->power);
        അവസ്ഥ SIG:
            തിരികെ net->learning_rate * (1./(1.+exp(net->gamma*(batch_അക്കം - net->step))));
        സ്ഥിരസ്ഥിതി:
            fprintf(stderr, "Policy is weird!\n");
            തിരികെ net->learning_rate;
    }
}

അക്ഷരം *get_layer_string(LAYER_TYPE a)
{
    തിരഞ്ഞെടുപ്പ്(a){
        അവസ്ഥ CONVOLUTIONAL:
            തിരികെ "convolutional";
        അവസ്ഥ ACTIVE:
            തിരികെ "activation";
        അവസ്ഥ LOCAL:
            തിരികെ "local";
        അവസ്ഥ DECONVOLUTIONAL:
            തിരികെ "deconvolutional";
        അവസ്ഥ CONNECTED:
            തിരികെ "connected";
        അവസ്ഥ RNN:
            തിരികെ "rnn";
        അവസ്ഥ GRU:
            തിരികെ "gru";
        അവസ്ഥ LSTM:
	    തിരികെ "lstm";
        അവസ്ഥ CRNN:
            തിരികെ "crnn";
        അവസ്ഥ MAXPOOL:
            തിരികെ "maxpool";
        അവസ്ഥ REORG:
            തിരികെ "reorg";
        അവസ്ഥ AVGPOOL:
            തിരികെ "avgpool";
        അവസ്ഥ SOFTMAX:
            തിരികെ "softmax";
        അവസ്ഥ DETECTION:
            തിരികെ "detection";
        അവസ്ഥ REGION:
            തിരികെ "region";
        അവസ്ഥ YOLO:
            തിരികെ "yolo";
        അവസ്ഥ DROPOUT:
            തിരികെ "dropout";
        അവസ്ഥ CROP:
            തിരികെ "crop";
        അവസ്ഥ COST:
            തിരികെ "cost";
        അവസ്ഥ ROUTE:
            തിരികെ "route";
        അവസ്ഥ SHORTCUT:
            തിരികെ "shortcut";
        അവസ്ഥ NORMALIZATION:
            തിരികെ "normalization";
        അവസ്ഥ BATCHNORM:
            തിരികെ "batchnorm";
        സ്ഥിരസ്ഥിതി:
            നിർത്തുക;
    }
    തിരികെ "none";
}

network *make_network(പൂർണ്ണസംഖ്യ n)
{
    network *net = calloc(1, വലുപ്പം(network));
    net->n = n;
    net->layers = calloc(net->n, വലുപ്പം(layer));
    net->seen = calloc(1, വലുപ്പം(size_t));
    net->t    = calloc(1, വലുപ്പം(പൂർണ്ണസംഖ്യ));
    net->cost = calloc(1, വലുപ്പം(ദശാംശം));
    തിരികെ net;
}

ശൂന്യം forward_network(network *netp)
{
#ifdef GPU
    എങ്കിൽ(netp->gpu_index >= 0){
        forward_network_gpu(netp);   
        തിരികെ;
    }
#endif
    network net = *netp;
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        എങ്കിൽ(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.നിവേശം = l.output;
        എങ്കിൽ(l.truth) {
            net.truth = l.output;
        }
    }
    calc_network_cost(netp);
}

ശൂന്യം update_network(network *netp)
{
#ifdef GPU
    എങ്കിൽ(netp->gpu_index >= 0){
        update_network_gpu(netp);   
        തിരികെ;
    }
#endif
    network net = *netp;
    പൂർണ്ണസംഖ്യ i;
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

    വേണ്ടി(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        എങ്കിൽ(l.update){
            l.update(l, a);
        }
    }
}

ശൂന്യം calc_network_cost(network *netp)
{
    network net = *netp;
    പൂർണ്ണസംഖ്യ i;
    ദശാംശം sum = 0;
    പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    വേണ്ടി(i = 0; i < net.n; ++i){
        എങ്കിൽ(net.layers[i].cost){
            sum += net.layers[i].cost[0];
            ++എണ്ണം;
        }
    }
    *net.cost = sum/എണ്ണം;
}

പൂർണ്ണസംഖ്യ get_predicted_class_network(network *net)
{
    തിരികെ max_index(net->output, net->outputs);
}

ശൂന്യം backward_network(network *netp)
{
#ifdef GPU
    എങ്കിൽ(netp->gpu_index >= 0){
        backward_network_gpu(netp);   
        തിരികെ;
    }
#endif
    network net = *netp;
    പൂർണ്ണസംഖ്യ i;
    network orig = net;
    വേണ്ടി(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        എങ്കിൽ(l.stopbackward) നിർത്തുക;
        എങ്കിൽ(i == 0){
            net = orig;
        }അല്ലെങ്കിൽ{
            layer prev = net.layers[i-1];
            net.നിവേശം = prev.output;
            net.delta = prev.delta;
        }
        net.index = i;
        l.backward(l, net);
    }
}

ദശാംശം train_network_datum(network *net)
{
    *net->seen += net->batch;
    net->train = 1;
    forward_network(net);
    backward_network(net);
    ദശാംശം error = *net->cost;
    എങ്കിൽ(((*net->seen)/net->batch)%net->subdivisions == 0) update_network(net);
    തിരികെ error;
}

ദശാംശം train_network_sgd(network *net, data d, പൂർണ്ണസംഖ്യ n)
{
    പൂർണ്ണസംഖ്യ batch = net->batch;

    പൂർണ്ണസംഖ്യ i;
    ദശാംശം sum = 0;
    വേണ്ടി(i = 0; i < n; ++i){
        get_random_batch(d, batch, net->നിവേശം, net->truth);
        ദശാംശം err = train_network_datum(net);
        sum += err;
    }
    തിരികെ (ദശാംശം)sum/(n*batch);
}

ദശാംശം train_network(network *net, data d)
{
    പ്രതിജ്ഞാ(d.X.rows % net->batch == 0);
    പൂർണ്ണസംഖ്യ batch = net->batch;
    പൂർണ്ണസംഖ്യ n = d.X.rows / batch;

    പൂർണ്ണസംഖ്യ i;
    ദശാംശം sum = 0;
    വേണ്ടി(i = 0; i < n; ++i){
        get_next_batch(d, batch, i*batch, net->നിവേശം, net->truth);
        ദശാംശം err = train_network_datum(net);
        sum += err;
    }
    തിരികെ (ദശാംശം)sum/(n*batch);
}

ശൂന്യം set_temp_network(network *net, ദശാംശം t)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < net->n; ++i){
        net->layers[i].temperature = t;
    }
}


ശൂന്യം set_batch_network(network *net, പൂർണ്ണസംഖ്യ b)
{
    net->batch = b;
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#ifdef CUDNN
        എങ്കിൽ(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_setup(net->layers + i);
        }
        എങ്കിൽ(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, l->out_h, l->out_w);
            cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 
        }
#endif
    }
}

പൂർണ്ണസംഖ്യ resize_network(network *net, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
#ifdef GPU
    cuda_set_device(net->gpu_index);
    cuda_free(net->workspace);
#endif
    പൂർണ്ണസംഖ്യ i;
    //if(w == net->w && h == net->h) return 0;
    net->w = w;
    net->h = h;
    പൂർണ്ണസംഖ്യ inputs = 0;
    size_t workspace_size = 0;
    //fprintf(stderr, "Resizing to %d x %d...\n", w, h);
    //fflush(stderr);
    വേണ്ടി (i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        എങ്കിൽ(l.type == CONVOLUTIONAL){
            resize_convolutional_layer(&l, w, h);
        }അല്ലെങ്കിൽ എങ്കിൽ(l.type == CROP){
            resize_crop_layer(&l, w, h);
        }അല്ലെങ്കിൽ എങ്കിൽ(l.type == MAXPOOL){
            resize_maxpool_layer(&l, w, h);
        }അല്ലെങ്കിൽ എങ്കിൽ(l.type == REGION){
            resize_region_layer(&l, w, h);
        }അല്ലെങ്കിൽ എങ്കിൽ(l.type == YOLO){
            resize_yolo_layer(&l, w, h);
        }അല്ലെങ്കിൽ എങ്കിൽ(l.type == ROUTE){
            resize_route_layer(&l, net);
        }അല്ലെങ്കിൽ എങ്കിൽ(l.type == SHORTCUT){
            resize_shortcut_layer(&l, w, h);
        }അല്ലെങ്കിൽ എങ്കിൽ(l.type == UPSAMPLE){
            resize_upsample_layer(&l, w, h);
        }അല്ലെങ്കിൽ എങ്കിൽ(l.type == REORG){
            resize_reorg_layer(&l, w, h);
        }അല്ലെങ്കിൽ എങ്കിൽ(l.type == AVGPOOL){
            resize_avgpool_layer(&l, w, h);
        }അല്ലെങ്കിൽ എങ്കിൽ(l.type == NORMALIZATION){
            resize_normalization_layer(&l, w, h);
        }അല്ലെങ്കിൽ എങ്കിൽ(l.type == COST){
            resize_cost_layer(&l, inputs);
        }അല്ലെങ്കിൽ{
            error("Cannot resize this type of layer");
        }
        എങ്കിൽ(l.workspace_size > workspace_size) workspace_size = l.workspace_size;
        എങ്കിൽ(l.workspace_size > 2000000000) പ്രതിജ്ഞാ(0);
        inputs = l.outputs;
        net->layers[i] = l;
        w = l.out_w;
        h = l.out_h;
        എങ്കിൽ(l.type == AVGPOOL) നിർത്തുക;
    }
    layer out = get_network_output_layer(net);
    net->inputs = net->layers[0].inputs;
    net->outputs = out.outputs;
    net->truths = out.outputs;
    എങ്കിൽ(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    സ്വതന്ത്രം(net->നിവേശം);
    സ്വതന്ത്രം(net->truth);
    net->നിവേശം = calloc(net->inputs*net->batch, വലുപ്പം(ദശാംശം));
    net->truth = calloc(net->truths*net->batch, വലുപ്പം(ദശാംശം));
#ifdef GPU
    എങ്കിൽ(gpu_index >= 0){
        cuda_free(net->നിവേശം_gpu);
        cuda_free(net->truth_gpu);
        net->നിവേശം_gpu = cuda_make_array(net->നിവേശം, net->inputs*net->batch);
        net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
        എങ്കിൽ(workspace_size){
            net->workspace = cuda_make_array(0, (workspace_size-1)/വലുപ്പം(ദശാംശം)+1);
        }
    }അല്ലെങ്കിൽ {
        സ്വതന്ത്രം(net->workspace);
        net->workspace = calloc(1, workspace_size);
    }
#അല്ലെങ്കിൽ
    സ്വതന്ത്രം(net->workspace);
    net->workspace = calloc(1, workspace_size);
#endif
    //fprintf(stderr, " Done!\n");
    തിരികെ 0;
}

layer get_network_detection_layer(network *net)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < net->n; ++i){
        എങ്കിൽ(net->layers[i].type == DETECTION){
            തിരികെ net->layers[i];
        }
    }
    fprintf(stderr, "Detection layer not found!!\n");
    layer l = {0};
    തിരികെ l;
}

image get_network_image_layer(network *net, പൂർണ്ണസംഖ്യ i)
{
    layer l = net->layers[i];
#ifdef GPU
    //cuda_pull_array(l.output_gpu, l.output, l.outputs);
#endif
    എങ്കിൽ (l.out_w && l.out_h && l.out_c){
        തിരികെ float_to_image(l.out_w, l.out_h, l.out_c, l.output);
    }
    image def = {0};
    തിരികെ def;
}

image get_network_image(network *net)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = net->n-1; i >= 0; --i){
        image m = get_network_image_layer(net, i);
        എങ്കിൽ(m.h != 0) തിരികെ m;
    }
    image def = {0};
    തിരികെ def;
}

ശൂന്യം visualize_network(network *net)
{
    image *prev = 0;
    പൂർണ്ണസംഖ്യ i;
    അക്ഷരം buff[256];
    വേണ്ടി(i = 0; i < net->n; ++i){
        sprintf(buff, "Layer %d", i);
        layer l = net->layers[i];
        എങ്കിൽ(l.type == CONVOLUTIONAL){
            prev = visualize_convolutional_layer(l, buff, prev);
        }
    } 
}

ശൂന്യം മുകൾ_predictions(network *net, പൂർണ്ണസംഖ്യ k, പൂർണ്ണസംഖ്യ *index)
{
    മുകൾ_k(net->output, net->outputs, k, index);
}


ദശാംശം *network_predict(network *net, ദശാംശം *നിവേശം)
{
    network orig = *net;
    net->നിവേശം = നിവേശം;
    net->truth = 0;
    net->train = 0;
    net->delta = 0;
    forward_network(net);
    ദശാംശം *out = net->output;
    *net = orig;
    തിരികെ out;
}

പൂർണ്ണസംഖ്യ അക്കം_detections(network *net, ദശാംശം thresh)
{
    പൂർണ്ണസംഖ്യ i;
    പൂർണ്ണസംഖ്യ s = 0;
    വേണ്ടി(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        എങ്കിൽ(l.type == YOLO){
            s += yolo_അക്കം_detections(l, thresh);
        }
        എങ്കിൽ(l.type == DETECTION || l.type == REGION){
            s += l.w*l.h*l.n;
        }
    }
    തിരികെ s;
}

detection *make_network_boxes(network *net, ദശാംശം thresh, പൂർണ്ണസംഖ്യ *അക്കം)
{
    layer l = net->layers[net->n - 1];
    പൂർണ്ണസംഖ്യ i;
    പൂർണ്ണസംഖ്യ nboxes = അക്കം_detections(net, thresh);
    എങ്കിൽ(അക്കം) *അക്കം = nboxes;
    detection *dets = calloc(nboxes, വലുപ്പം(detection));
    വേണ്ടി(i = 0; i < nboxes; ++i){
        dets[i].prob = calloc(l.classes, വലുപ്പം(ദശാംശം));
        എങ്കിൽ(l.coords > 4){
            dets[i].mask = calloc(l.coords-4, വലുപ്പം(ദശാംശം));
        }
    }
    തിരികെ dets;
}

ശൂന്യം fill_network_boxes(network *net, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, ദശാംശം thresh, ദശാംശം hier, പൂർണ്ണസംഖ്യ *map, പൂർണ്ണസംഖ്യ relative, detection *dets)
{
    പൂർണ്ണസംഖ്യ j;
    വേണ്ടി(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        എങ്കിൽ(l.type == YOLO){
            പൂർണ്ണസംഖ്യ എണ്ണം = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
            dets += എണ്ണം;
        }
        എങ്കിൽ(l.type == REGION){
            get_region_detections(l, w, h, net->w, net->h, thresh, map, hier, relative, dets);
            dets += l.w*l.h*l.n;
        }
        എങ്കിൽ(l.type == DETECTION){
            get_detection_detections(l, w, h, thresh, dets);
            dets += l.w*l.h*l.n;
        }
    }
}

detection *get_network_boxes(network *net, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, ദശാംശം thresh, ദശാംശം hier, പൂർണ്ണസംഖ്യ *map, പൂർണ്ണസംഖ്യ relative, പൂർണ്ണസംഖ്യ *അക്കം)
{
    detection *dets = make_network_boxes(net, thresh, അക്കം);
    fill_network_boxes(net, w, h, thresh, hier, map, relative, dets);
    തിരികെ dets;
}

ശൂന്യം free_detections(detection *dets, പൂർണ്ണസംഖ്യ n)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < n; ++i){
        സ്വതന്ത്രം(dets[i].prob);
        എങ്കിൽ(dets[i].mask) സ്വതന്ത്രം(dets[i].mask);
    }
    സ്വതന്ത്രം(dets);
}

ദശാംശം *network_predict_image(network *net, image im)
{
    image imr = letterbox_image(im, net->w, net->h);
    set_batch_network(net, 1);
    ദശാംശം *p = network_predict(net, imr.data);
    free_image(imr);
    തിരികെ p;
}

പൂർണ്ണസംഖ്യ network_width(network *net){തിരികെ net->w;}
പൂർണ്ണസംഖ്യ network_height(network *net){തിരികെ net->h;}

matrix network_predict_data_multi(network *net, data test, പൂർണ്ണസംഖ്യ n)
{
    പൂർണ്ണസംഖ്യ i,j,b,m;
    പൂർണ്ണസംഖ്യ k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    ദശാംശം *X = calloc(net->batch*test.X.rows, വലുപ്പം(ദശാംശം));
    വേണ്ടി(i = 0; i < test.X.rows; i += net->batch){
        വേണ്ടി(b = 0; b < net->batch; ++b){
            എങ്കിൽ(i+b == test.X.rows) നിർത്തുക;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*വലുപ്പം(ദശാംശം));
        }
        വേണ്ടി(m = 0; m < n; ++m){
            ദശാംശം *out = network_predict(net, X);
            വേണ്ടി(b = 0; b < net->batch; ++b){
                എങ്കിൽ(i+b == test.X.rows) നിർത്തുക;
                വേണ്ടി(j = 0; j < k; ++j){
                    pred.vals[i+b][j] += out[j+b*k]/n;
                }
            }
        }
    }
    സ്വതന്ത്രം(X);
    തിരികെ pred;   
}

matrix network_predict_data(network *net, data test)
{
    പൂർണ്ണസംഖ്യ i,j,b;
    പൂർണ്ണസംഖ്യ k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    ദശാംശം *X = calloc(net->batch*test.X.cols, വലുപ്പം(ദശാംശം));
    വേണ്ടി(i = 0; i < test.X.rows; i += net->batch){
        വേണ്ടി(b = 0; b < net->batch; ++b){
            എങ്കിൽ(i+b == test.X.rows) നിർത്തുക;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*വലുപ്പം(ദശാംശം));
        }
        ദശാംശം *out = network_predict(net, X);
        വേണ്ടി(b = 0; b < net->batch; ++b){
            എങ്കിൽ(i+b == test.X.rows) നിർത്തുക;
            വേണ്ടി(j = 0; j < k; ++j){
                pred.vals[i+b][j] = out[j+b*k];
            }
        }
    }
    സ്വതന്ത്രം(X);
    തിരികെ pred;   
}

ശൂന്യം print_network(network *net)
{
    പൂർണ്ണസംഖ്യ i,j;
    വേണ്ടി(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ദശാംശം *output = l.output;
        പൂർണ്ണസംഖ്യ n = l.outputs;
        ദശാംശം mean = mean_array(output, n);
        ദശാംശം vari = variance_array(output, n);
        fprintf(stderr, "Layer %d - Mean: %f, Variance: %f\n",i,mean, vari);
        എങ്കിൽ(n > 100) n = 100;
        വേണ്ടി(j = 0; j < n; ++j) fprintf(stderr, "%f, ", output[j]);
        എങ്കിൽ(n == 100)fprintf(stderr,".....\n");
        fprintf(stderr, "\n");
    }
}

ശൂന്യം compare_networks(network *n1, network *n2, data test)
{
    matrix g1 = network_predict_data(n1, test);
    matrix g2 = network_predict_data(n2, test);
    പൂർണ്ണസംഖ്യ i;
    പൂർണ്ണസംഖ്യ a,b,c,d;
    a = b = c = d = 0;
    വേണ്ടി(i = 0; i < g1.rows; ++i){
        പൂർണ്ണസംഖ്യ truth = max_index(test.y.vals[i], test.y.cols);
        പൂർണ്ണസംഖ്യ p1 = max_index(g1.vals[i], g1.cols);
        പൂർണ്ണസംഖ്യ p2 = max_index(g2.vals[i], g2.cols);
        എങ്കിൽ(p1 == truth){
            എങ്കിൽ(p2 == truth) ++d;
            അല്ലെങ്കിൽ ++c;
        }അല്ലെങ്കിൽ{
            എങ്കിൽ(p2 == truth) ++b;
            അല്ലെങ്കിൽ ++a;
        }
    }
    printf("%5d %5d\n%5d %5d\n", a, b, c, d);
    ദശാംശം അക്കം = pow((abs(b - c) - 1.), 2.);
    ദശാംശം den = b + c;
    printf("%f\n", അക്കം/den); 
}

ദശാംശം network_accuracy(network *net, data d)
{
    matrix guess = network_predict_data(net, d);
    ദശാംശം acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    തിരികെ acc;
}

ദശാംശം *network_accuracies(network *net, data d, പൂർണ്ണസംഖ്യ n)
{
    സ്ഥിതം ദശാംശം acc[2];
    matrix guess = network_predict_data(net, d);
    acc[0] = matrix_topk_accuracy(d.y, guess, 1);
    acc[1] = matrix_topk_accuracy(d.y, guess, n);
    free_matrix(guess);
    തിരികെ acc;
}

layer get_network_output_layer(network *net)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = net->n - 1; i >= 0; --i){
        എങ്കിൽ(net->layers[i].type != COST) നിർത്തുക;
    }
    തിരികെ net->layers[i];
}

ദശാംശം network_accuracy_multi(network *net, data d, പൂർണ്ണസംഖ്യ n)
{
    matrix guess = network_predict_data_multi(net, d, n);
    ദശാംശം acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    തിരികെ acc;
}

ശൂന്യം free_network(network *net)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < net->n; ++i){
        free_layer(net->layers[i]);
    }
    സ്വതന്ത്രം(net->layers);
    എങ്കിൽ(net->നിവേശം) സ്വതന്ത്രം(net->നിവേശം);
    എങ്കിൽ(net->truth) സ്വതന്ത്രം(net->truth);
#ifdef GPU
    എങ്കിൽ(net->നിവേശം_gpu) cuda_free(net->നിവേശം_gpu);
    എങ്കിൽ(net->truth_gpu) cuda_free(net->truth_gpu);
#endif
    സ്വതന്ത്രം(net);
}

// Some day...
// ^ What the hell is this comment for?


layer network_output_layer(network *net)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = net->n - 1; i >= 0; --i){
        എങ്കിൽ(net->layers[i].type != COST) നിർത്തുക;
    }
    തിരികെ net->layers[i];
}

പൂർണ്ണസംഖ്യ network_inputs(network *net)
{
    തിരികെ net->layers[0].inputs;
}

പൂർണ്ണസംഖ്യ network_outputs(network *net)
{
    തിരികെ network_output_layer(net).outputs;
}

ദശാംശം *network_output(network *net)
{
    തിരികെ network_output_layer(net).output;
}

#ifdef GPU

ശൂന്യം forward_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    cuda_തള്ളുക_array(net.നിവേശം_gpu, net.നിവേശം, net.inputs*net.batch);
    എങ്കിൽ(net.truth){
        cuda_തള്ളുക_array(net.truth_gpu, net.truth, net.truths*net.batch);
    }

    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        എങ്കിൽ(l.delta_gpu){
            fill_gpu(l.outputs * l.batch, 0, l.delta_gpu, 1);
        }
        l.forward_gpu(l, net);
        net.നിവേശം_gpu = l.output_gpu;
        net.നിവേശം = l.output;
        എങ്കിൽ(l.truth) {
            net.truth_gpu = l.output_gpu;
            net.truth = l.output;
        }
    }
    pull_network_output(netp);
    calc_network_cost(netp);
}

ശൂന്യം backward_network_gpu(network *netp)
{
    പൂർണ്ണസംഖ്യ i;
    network net = *netp;
    network orig = net;
    cuda_set_device(net.gpu_index);
    വേണ്ടി(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        എങ്കിൽ(l.stopbackward) നിർത്തുക;
        എങ്കിൽ(i == 0){
            net = orig;
        }അല്ലെങ്കിൽ{
            layer prev = net.layers[i-1];
            net.നിവേശം = prev.output;
            net.delta = prev.delta;
            net.നിവേശം_gpu = prev.output_gpu;
            net.delta_gpu = prev.delta_gpu;
        }
        net.index = i;
        l.backward_gpu(l, net);
    }
}

ശൂന്യം update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    പൂർണ്ണസംഖ്യ i;
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

    വേണ്ടി(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        എങ്കിൽ(l.update_gpu){
            l.update_gpu(l, a);
        }
    }
}

ശൂന്യം harmless_update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        എങ്കിൽ(l.weight_updates_gpu) fill_gpu(l.nweights, 0, l.weight_updates_gpu, 1);
        എങ്കിൽ(l.bias_updates_gpu) fill_gpu(l.nbiases, 0, l.bias_updates_gpu, 1);
        എങ്കിൽ(l.scale_updates_gpu) fill_gpu(l.nbiases, 0, l.scale_updates_gpu, 1);
    }
}

തരനിർവചനം ഘടന {
    network *net;
    data d;
    ദശാംശം *err;
} train_args;

ശൂന്യം *train_thread(ശൂന്യം *ptr)
{
    train_args args = *(train_args*)ptr;
    സ്വതന്ത്രം(ptr);
    cuda_set_device(args.net->gpu_index);
    *args.err = train_network(args.net, args.d);
    തിരികെ 0;
}

pthread_t train_network_in_thread(network *net, data d, ദശാംശം *err)
{
    pthread_t thread;
    train_args *ptr = (train_args *)calloc(1, വലുപ്പം(train_args));
    ptr->net = net;
    ptr->d = d;
    ptr->err = err;
    എങ്കിൽ(pthread_create(&thread, 0, train_thread, ptr)) error("Thread creation failed");
    തിരികെ thread;
}

ശൂന്യം merge_weights(layer l, layer base)
{
    എങ്കിൽ (l.type == CONVOLUTIONAL) {
        axpy_cpu(l.n, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.nweights, 1, l.weight_updates, 1, base.weights, 1);
        എങ്കിൽ (l.scales) {
            axpy_cpu(l.n, 1, l.scale_updates, 1, base.scales, 1);
        }
    } അല്ലെങ്കിൽ എങ്കിൽ(l.type == CONNECTED) {
        axpy_cpu(l.outputs, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.outputs*l.inputs, 1, l.weight_updates, 1, base.weights, 1);
    }
}

ശൂന്യം scale_weights(layer l, ദശാംശം s)
{
    എങ്കിൽ (l.type == CONVOLUTIONAL) {
        scal_cpu(l.n, s, l.biases, 1);
        scal_cpu(l.nweights, s, l.weights, 1);
        എങ്കിൽ (l.scales) {
            scal_cpu(l.n, s, l.scales, 1);
        }
    } അല്ലെങ്കിൽ എങ്കിൽ(l.type == CONNECTED) {
        scal_cpu(l.outputs, s, l.biases, 1);
        scal_cpu(l.outputs*l.inputs, s, l.weights, 1);
    }
}


ശൂന്യം pull_weights(layer l)
{
    എങ്കിൽ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.n);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.nweights);
        എങ്കിൽ(l.scales) cuda_pull_array(l.scales_gpu, l.scale_updates, l.n);
    } അല്ലെങ്കിൽ എങ്കിൽ(l.type == CONNECTED){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.outputs);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.outputs*l.inputs);
    }
}

ശൂന്യം തള്ളുക_weights(layer l)
{
    എങ്കിൽ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_തള്ളുക_array(l.biases_gpu, l.biases, l.n);
        cuda_തള്ളുക_array(l.weights_gpu, l.weights, l.nweights);
        എങ്കിൽ(l.scales) cuda_തള്ളുക_array(l.scales_gpu, l.scales, l.n);
    } അല്ലെങ്കിൽ എങ്കിൽ(l.type == CONNECTED){
        cuda_തള്ളുക_array(l.biases_gpu, l.biases, l.outputs);
        cuda_തള്ളുക_array(l.weights_gpu, l.weights, l.outputs*l.inputs);
    }
}

ശൂന്യം distribute_weights(layer l, layer base)
{
    എങ്കിൽ (l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL) {
        cuda_തള്ളുക_array(l.biases_gpu, base.biases, l.n);
        cuda_തള്ളുക_array(l.weights_gpu, base.weights, l.nweights);
        എങ്കിൽ (base.scales) cuda_തള്ളുക_array(l.scales_gpu, base.scales, l.n);
    } അല്ലെങ്കിൽ എങ്കിൽ (l.type == CONNECTED) {
        cuda_തള്ളുക_array(l.biases_gpu, base.biases, l.outputs);
        cuda_തള്ളുക_array(l.weights_gpu, base.weights, l.outputs*l.inputs);
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

ശൂന്യം sync_layer(network **nets, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ j)
{
    പൂർണ്ണസംഖ്യ i;
    network *net = nets[0];
    layer base = net->layers[j];
    scale_weights(base, 0);
    വേണ്ടി (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        pull_weights(l);
        merge_weights(l, base);
    }
    scale_weights(base, 1./n);
    വേണ്ടി (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        distribute_weights(l, base);
    }
}

തരനിർവചനം ഘടന{
    network **nets;
    പൂർണ്ണസംഖ്യ n;
    പൂർണ്ണസംഖ്യ j;
} sync_args;

ശൂന്യം *sync_layer_thread(ശൂന്യം *ptr)
{
    sync_args args = *(sync_args*)ptr;
    sync_layer(args.nets, args.n, args.j);
    സ്വതന്ത്രം(ptr);
    തിരികെ 0;
}

pthread_t sync_layer_in_thread(network **nets, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ j)
{
    pthread_t thread;
    sync_args *ptr = (sync_args *)calloc(1, വലുപ്പം(sync_args));
    ptr->nets = nets;
    ptr->n = n;
    ptr->j = j;
    എങ്കിൽ(pthread_create(&thread, 0, sync_layer_thread, ptr)) error("Thread creation failed");
    തിരികെ thread;
}

ശൂന്യം sync_nets(network **nets, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ interval)
{
    പൂർണ്ണസംഖ്യ j;
    പൂർണ്ണസംഖ്യ layers = nets[0]->n;
    pthread_t *threads = (pthread_t *) calloc(layers, വലുപ്പം(pthread_t));

    *(nets[0]->seen) += interval * (n-1) * nets[0]->batch * nets[0]->subdivisions;
    വേണ്ടി (j = 0; j < n; ++j){
        *(nets[j]->seen) = *(nets[0]->seen);
    }
    വേണ്ടി (j = 0; j < layers; ++j) {
        threads[j] = sync_layer_in_thread(nets, n, j);
    }
    വേണ്ടി (j = 0; j < layers; ++j) {
        pthread_join(threads[j], 0);
    }
    സ്വതന്ത്രം(threads);
}

ദശാംശം train_networks(network **nets, പൂർണ്ണസംഖ്യ n, data d, പൂർണ്ണസംഖ്യ interval)
{
    പൂർണ്ണസംഖ്യ i;
    പൂർണ്ണസംഖ്യ batch = nets[0]->batch;
    പൂർണ്ണസംഖ്യ subdivisions = nets[0]->subdivisions;
    പ്രതിജ്ഞാ(batch * subdivisions * n == d.X.rows);
    pthread_t *threads = (pthread_t *) calloc(n, വലുപ്പം(pthread_t));
    ദശാംശം *errors = (ദശാംശം *) calloc(n, വലുപ്പം(ദശാംശം));

    ദശാംശം sum = 0;
    വേണ്ടി(i = 0; i < n; ++i){
        data p = get_data_part(d, i, n);
        threads[i] = train_network_in_thread(nets[i], p, errors + i);
    }
    വേണ്ടി(i = 0; i < n; ++i){
        pthread_join(threads[i], 0);
        //printf("%f\n", errors[i]);
        sum += errors[i];
    }
    //cudaDeviceSynchronize();
    എങ്കിൽ (get_current_batch(nets[0]) % interval == 0) {
        printf("Syncing... ");
        fflush(stdout);
        sync_nets(nets, n, interval);
        printf("Done!\n");
    }
    //cudaDeviceSynchronize();
    സ്വതന്ത്രം(threads);
    സ്വതന്ത്രം(errors);
    തിരികെ (ദശാംശം)sum/(n);
}

ശൂന്യം pull_network_output(network *net)
{
    layer l = get_network_output_layer(net);
    cuda_pull_array(l.output_gpu, l.output, l.outputs*l.batch);
}

#endif

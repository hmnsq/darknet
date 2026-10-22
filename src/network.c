#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ "network.h"
#ଅନ୍ତର୍ଭାଵ "image.h"
#ଅନ୍ତର୍ଭାଵ "data.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "blas.h"

#ଅନ୍ତର୍ଭାଵ "crop_layer.h"
#ଅନ୍ତର୍ଭାଵ "connected_layer.h"
#ଅନ୍ତର୍ଭାଵ "gru_layer.h"
#ଅନ୍ତର୍ଭାଵ "rnn_layer.h"
#ଅନ୍ତର୍ଭାଵ "crnn_layer.h"
#ଅନ୍ତର୍ଭାଵ "local_layer.h"
#ଅନ୍ତର୍ଭାଵ "convolutional_layer.h"
#ଅନ୍ତର୍ଭାଵ "activation_layer.h"
#ଅନ୍ତର୍ଭାଵ "detection_layer.h"
#ଅନ୍ତର୍ଭାଵ "region_layer.h"
#ଅନ୍ତର୍ଭାଵ "yolo_layer.h"
#ଅନ୍ତର୍ଭାଵ "normalization_layer.h"
#ଅନ୍ତର୍ଭାଵ "batchnorm_layer.h"
#ଅନ୍ତର୍ଭାଵ "maxpool_layer.h"
#ଅନ୍ତର୍ଭାଵ "reorg_layer.h"
#ଅନ୍ତର୍ଭାଵ "avgpool_layer.h"
#ଅନ୍ତର୍ଭାଵ "cost_layer.h"
#ଅନ୍ତର୍ଭାଵ "softmax_layer.h"
#ଅନ୍ତର୍ଭାଵ "dropout_layer.h"
#ଅନ୍ତର୍ଭାଵ "route_layer.h"
#ଅନ୍ତର୍ଭାଵ "upsample_layer.h"
#ଅନ୍ତର୍ଭାଵ "shortcut_layer.h"
#ଅନ୍ତର୍ଭାଵ "parser.h"
#ଅନ୍ତର୍ଭାଵ "data.h"

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
    ଫେରନ୍ତୁ args;
}

network *load_network(ଅକ୍ଷର *cfg, ଅକ୍ଷର *weights, ପୂର୍ଣ୍ଣସଂଖ୍ୟା clear)
{
    network *net = parse_network_cfg(cfg);
    ଯଦି(weights && weights[0] != 0){
        load_weights(net, weights);
    }
    ଯଦି(clear) (*net->seen) = 0;
    ଫେରନ୍ତୁ net;
}

size_t get_current_batch(network *net)
{
    size_t batch_ଅଙ୍କ = (*net->seen)/(net->batch*net->subdivisions);
    ଫେରନ୍ତୁ batch_ଅଙ୍କ;
}

ଶୂନ୍ୟ reset_network_state(network *net, ପୂର୍ଣ୍ଣସଂଖ୍ୟା b)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ (i = 0; i < net->n; ++i) {
        #ifdef GPU
        layer l = net->layers[i];
        ଯଦି(l.state_gpu){
            fill_gpu(l.outputs, 0, l.state_gpu + l.outputs*b, 1);
        }
        ଯଦି(l.h_gpu){
            fill_gpu(l.outputs, 0, l.h_gpu + l.outputs*b, 1);
        }
        #endif
    }
}

ଶୂନ୍ୟ reset_rnn(network *net)
{
    reset_network_state(net, 0);
}

ଦଶମିକ get_current_rate(network *net)
{
    size_t batch_ଅଙ୍କ = get_current_batch(net);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଦଶମିକ rate;
    ଯଦି (batch_ଅଙ୍କ < net->burn_in) ଫେରନ୍ତୁ net->learning_rate * pow((ଦଶମିକ)batch_ଅଙ୍କ / net->burn_in, net->power);
    ବାଛ (net->policy) {
        ସ୍ଥିତି CONSTANT:
            ଫେରନ୍ତୁ net->learning_rate;
        ସ୍ଥିତି STEP:
            ଫେରନ୍ତୁ net->learning_rate * pow(net->scale, batch_ଅଙ୍କ/net->step);
        ସ୍ଥିତି STEPS:
            rate = net->learning_rate;
            ପାଇଁ(i = 0; i < net->ଅଙ୍କ_steps; ++i){
                ଯଦି(net->steps[i] > batch_ଅଙ୍କ) ଫେରନ୍ତୁ rate;
                rate *= net->scales[i];
            }
            ଫେରନ୍ତୁ rate;
        ସ୍ଥିତି EXP:
            ଫେରନ୍ତୁ net->learning_rate * pow(net->gamma, batch_ଅଙ୍କ);
        ସ୍ଥିତି POLY:
            ଫେରନ୍ତୁ net->learning_rate * pow(1 - (ଦଶମିକ)batch_ଅଙ୍କ / net->max_batches, net->power);
        ସ୍ଥିତି RANDOM:
            ଫେରନ୍ତୁ net->learning_rate * pow(rand_uniform(0,1), net->power);
        ସ୍ଥିତି SIG:
            ଫେରନ୍ତୁ net->learning_rate * (1./(1.+exp(net->gamma*(batch_ଅଙ୍କ - net->step))));
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            fprintf(stderr, "Policy is weird!\n");
            ଫେରନ୍ତୁ net->learning_rate;
    }
}

ଅକ୍ଷର *get_layer_string(LAYER_TYPE a)
{
    ବାଛ(a){
        ସ୍ଥିତି CONVOLUTIONAL:
            ଫେରନ୍ତୁ "convolutional";
        ସ୍ଥିତି ACTIVE:
            ଫେରନ୍ତୁ "activation";
        ସ୍ଥିତି LOCAL:
            ଫେରନ୍ତୁ "local";
        ସ୍ଥିତି DECONVOLUTIONAL:
            ଫେରନ୍ତୁ "deconvolutional";
        ସ୍ଥିତି CONNECTED:
            ଫେରନ୍ତୁ "connected";
        ସ୍ଥିତି RNN:
            ଫେରନ୍ତୁ "rnn";
        ସ୍ଥିତି GRU:
            ଫେରନ୍ତୁ "gru";
        ସ୍ଥିତି LSTM:
	    ଫେରନ୍ତୁ "lstm";
        ସ୍ଥିତି CRNN:
            ଫେରନ୍ତୁ "crnn";
        ସ୍ଥିତି MAXPOOL:
            ଫେରନ୍ତୁ "maxpool";
        ସ୍ଥିତି REORG:
            ଫେରନ୍ତୁ "reorg";
        ସ୍ଥିତି AVGPOOL:
            ଫେରନ୍ତୁ "avgpool";
        ସ୍ଥିତି SOFTMAX:
            ଫେରନ୍ତୁ "softmax";
        ସ୍ଥିତି DETECTION:
            ଫେରନ୍ତୁ "detection";
        ସ୍ଥିତି REGION:
            ଫେରନ୍ତୁ "region";
        ସ୍ଥିତି YOLO:
            ଫେରନ୍ତୁ "yolo";
        ସ୍ଥିତି DROPOUT:
            ଫେରନ୍ତୁ "dropout";
        ସ୍ଥିତି CROP:
            ଫେରନ୍ତୁ "crop";
        ସ୍ଥିତି COST:
            ଫେରନ୍ତୁ "cost";
        ସ୍ଥିତି ROUTE:
            ଫେରନ୍ତୁ "route";
        ସ୍ଥିତି SHORTCUT:
            ଫେରନ୍ତୁ "shortcut";
        ସ୍ଥିତି NORMALIZATION:
            ଫେରନ୍ତୁ "normalization";
        ସ୍ଥିତି BATCHNORM:
            ଫେରନ୍ତୁ "batchnorm";
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            ଭାଙ୍ଗ;
    }
    ଫେରନ୍ତୁ "none";
}

network *make_network(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    network *net = calloc(1, ଆକାର(network));
    net->n = n;
    net->layers = calloc(net->n, ଆକାର(layer));
    net->seen = calloc(1, ଆକାର(size_t));
    net->t    = calloc(1, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
    net->cost = calloc(1, ଆକାର(ଦଶମିକ));
    ଫେରନ୍ତୁ net;
}

ଶୂନ୍ୟ forward_network(network *netp)
{
#ifdef GPU
    ଯଦି(netp->gpu_index >= 0){
        forward_network_gpu(netp);   
        ଫେରନ୍ତୁ;
    }
#endif
    network net = *netp;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        ଯଦି(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.ନିବେଶ = l.output;
        ଯଦି(l.truth) {
            net.truth = l.output;
        }
    }
    calc_network_cost(netp);
}

ଶୂନ୍ୟ update_network(network *netp)
{
#ifdef GPU
    ଯଦି(netp->gpu_index >= 0){
        update_network_gpu(netp);   
        ଫେରନ୍ତୁ;
    }
#endif
    network net = *netp;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
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

    ପାଇଁ(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        ଯଦି(l.update){
            l.update(l, a);
        }
    }
}

ଶୂନ୍ୟ calc_network_cost(network *netp)
{
    network net = *netp;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଦଶମିକ sum = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    ପାଇଁ(i = 0; i < net.n; ++i){
        ଯଦି(net.layers[i].cost){
            sum += net.layers[i].cost[0];
            ++ଗଣନା_2;
        }
    }
    *net.cost = sum/ଗଣନା_2;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_predicted_class_network(network *net)
{
    ଫେରନ୍ତୁ max_index(net->output, net->outputs);
}

ଶୂନ୍ୟ backward_network(network *netp)
{
#ifdef GPU
    ଯଦି(netp->gpu_index >= 0){
        backward_network_gpu(netp);   
        ଫେରନ୍ତୁ;
    }
#endif
    network net = *netp;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    network orig = net;
    ପାଇଁ(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        ଯଦି(l.stopbackward) ଭାଙ୍ଗ;
        ଯଦି(i == 0){
            net = orig;
        }ଅନ୍ୟଥା{
            layer prev = net.layers[i-1];
            net.ନିବେଶ = prev.output;
            net.delta = prev.delta;
        }
        net.index = i;
        l.backward(l, net);
    }
}

ଦଶମିକ train_network_datum(network *net)
{
    *net->seen += net->batch;
    net->train = 1;
    forward_network(net);
    backward_network(net);
    ଦଶମିକ error = *net->cost;
    ଯଦି(((*net->seen)/net->batch)%net->subdivisions == 0) update_network(net);
    ଫେରନ୍ତୁ error;
}

ଦଶମିକ train_network_sgd(network *net, data d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch = net->batch;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଦଶମିକ sum = 0;
    ପାଇଁ(i = 0; i < n; ++i){
        get_random_batch(d, batch, net->ନିବେଶ, net->truth);
        ଦଶମିକ err = train_network_datum(net);
        sum += err;
    }
    ଫେରନ୍ତୁ (ଦଶମିକ)sum/(n*batch);
}

ଦଶମିକ train_network(network *net, data d)
{
    ପ୍ରତିଜ୍ଞା(d.X.rows % net->batch == 0);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch = net->batch;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = d.X.rows / batch;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଦଶମିକ sum = 0;
    ପାଇଁ(i = 0; i < n; ++i){
        get_next_batch(d, batch, i*batch, net->ନିବେଶ, net->truth);
        ଦଶମିକ err = train_network_datum(net);
        sum += err;
    }
    ଫେରନ୍ତୁ (ଦଶମିକ)sum/(n*batch);
}

ଶୂନ୍ୟ set_temp_network(network *net, ଦଶମିକ t)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < net->n; ++i){
        net->layers[i].temperature = t;
    }
}


ଶୂନ୍ୟ set_batch_network(network *net, ପୂର୍ଣ୍ଣସଂଖ୍ୟା b)
{
    net->batch = b;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#ifdef CUDNN
        ଯଦି(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_setup(net->layers + i);
        }
        ଯଦି(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, l->out_h, l->out_w);
            cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 
        }
#endif
    }
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା resize_network(network *net, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
#ifdef GPU
    cuda_set_device(net->gpu_index);
    cuda_free(net->workspace);
#endif
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    //if(w == net->w && h == net->h) return 0;
    net->w = w;
    net->h = h;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା inputs = 0;
    size_t workspace_size = 0;
    //fprintf(stderr, "Resizing to %d x %d...\n", w, h);
    //fflush(stderr);
    ପାଇଁ (i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ଯଦି(l.type == CONVOLUTIONAL){
            resize_convolutional_layer(&l, w, h);
        }ଅନ୍ୟଥା ଯଦି(l.type == CROP){
            resize_crop_layer(&l, w, h);
        }ଅନ୍ୟଥା ଯଦି(l.type == MAXPOOL){
            resize_maxpool_layer(&l, w, h);
        }ଅନ୍ୟଥା ଯଦି(l.type == REGION){
            resize_region_layer(&l, w, h);
        }ଅନ୍ୟଥା ଯଦି(l.type == YOLO){
            resize_yolo_layer(&l, w, h);
        }ଅନ୍ୟଥା ଯଦି(l.type == ROUTE){
            resize_route_layer(&l, net);
        }ଅନ୍ୟଥା ଯଦି(l.type == SHORTCUT){
            resize_shortcut_layer(&l, w, h);
        }ଅନ୍ୟଥା ଯଦି(l.type == UPSAMPLE){
            resize_upsample_layer(&l, w, h);
        }ଅନ୍ୟଥା ଯଦି(l.type == REORG){
            resize_reorg_layer(&l, w, h);
        }ଅନ୍ୟଥା ଯଦି(l.type == AVGPOOL){
            resize_avgpool_layer(&l, w, h);
        }ଅନ୍ୟଥା ଯଦି(l.type == NORMALIZATION){
            resize_normalization_layer(&l, w, h);
        }ଅନ୍ୟଥା ଯଦି(l.type == COST){
            resize_cost_layer(&l, inputs);
        }ଅନ୍ୟଥା{
            error("Cannot resize this type of layer");
        }
        ଯଦି(l.workspace_size > workspace_size) workspace_size = l.workspace_size;
        ଯଦି(l.workspace_size > 2000000000) ପ୍ରତିଜ୍ଞା(0);
        inputs = l.outputs;
        net->layers[i] = l;
        w = l.out_w;
        h = l.out_h;
        ଯଦି(l.type == AVGPOOL) ଭାଙ୍ଗ;
    }
    layer out = get_network_output_layer(net);
    net->inputs = net->layers[0].inputs;
    net->outputs = out.outputs;
    net->truths = out.outputs;
    ଯଦି(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    ମୁକ୍ତ(net->ନିବେଶ);
    ମୁକ୍ତ(net->truth);
    net->ନିବେଶ = calloc(net->inputs*net->batch, ଆକାର(ଦଶମିକ));
    net->truth = calloc(net->truths*net->batch, ଆକାର(ଦଶମିକ));
#ifdef GPU
    ଯଦି(gpu_index >= 0){
        cuda_free(net->ନିବେଶ_gpu);
        cuda_free(net->truth_gpu);
        net->ନିବେଶ_gpu = cuda_make_array(net->ନିବେଶ, net->inputs*net->batch);
        net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
        ଯଦି(workspace_size){
            net->workspace = cuda_make_array(0, (workspace_size-1)/ଆକାର(ଦଶମିକ)+1);
        }
    }ଅନ୍ୟଥା {
        ମୁକ୍ତ(net->workspace);
        net->workspace = calloc(1, workspace_size);
    }
#ଅନ୍ୟଥା
    ମୁକ୍ତ(net->workspace);
    net->workspace = calloc(1, workspace_size);
#endif
    //fprintf(stderr, " Done!\n");
    ଫେରନ୍ତୁ 0;
}

layer get_network_detection_layer(network *net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < net->n; ++i){
        ଯଦି(net->layers[i].type == DETECTION){
            ଫେରନ୍ତୁ net->layers[i];
        }
    }
    fprintf(stderr, "Detection layer not found!!\n");
    layer l = {0};
    ଫେରନ୍ତୁ l;
}

image get_network_image_layer(network *net, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i)
{
    layer l = net->layers[i];
#ifdef GPU
    //cuda_pull_array(l.output_gpu, l.output, l.outputs);
#endif
    ଯଦି (l.out_w && l.out_h && l.out_c){
        ଫେରନ୍ତୁ float_to_image(l.out_w, l.out_h, l.out_c, l.output);
    }
    image def = {0};
    ଫେରନ୍ତୁ def;
}

image get_network_image(network *net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = net->n-1; i >= 0; --i){
        image m = get_network_image_layer(net, i);
        ଯଦି(m.h != 0) ଫେରନ୍ତୁ m;
    }
    image def = {0};
    ଫେରନ୍ତୁ def;
}

ଶୂନ୍ୟ visualize_network(network *net)
{
    image *prev = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଅକ୍ଷର buff[256];
    ପାଇଁ(i = 0; i < net->n; ++i){
        sprintf(buff, "Layer %d", i);
        layer l = net->layers[i];
        ଯଦି(l.type == CONVOLUTIONAL){
            prev = visualize_convolutional_layer(l, buff, prev);
        }
    } 
}

ଶୂନ୍ୟ ଶୀର୍ଷ_predictions(network *net, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *index)
{
    ଶୀର୍ଷ_k(net->output, net->outputs, k, index);
}


ଦଶମିକ *network_predict(network *net, ଦଶମିକ *ନିବେଶ)
{
    network orig = *net;
    net->ନିବେଶ = ନିବେଶ;
    net->truth = 0;
    net->train = 0;
    net->delta = 0;
    forward_network(net);
    ଦଶମିକ *out = net->output;
    *net = orig;
    ଫେରନ୍ତୁ out;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_detections(network *net, ଦଶମିକ thresh)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0;
    ପାଇଁ(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ଯଦି(l.type == YOLO){
            s += yolo_ଅଙ୍କ_detections(l, thresh);
        }
        ଯଦି(l.type == DETECTION || l.type == REGION){
            s += l.w*l.h*l.n;
        }
    }
    ଫେରନ୍ତୁ s;
}

detection *make_network_boxes(network *net, ଦଶମିକ thresh, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ଅଙ୍କ)
{
    layer l = net->layers[net->n - 1];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nboxes = ଅଙ୍କ_detections(net, thresh);
    ଯଦି(ଅଙ୍କ) *ଅଙ୍କ = nboxes;
    detection *dets = calloc(nboxes, ଆକାର(detection));
    ପାଇଁ(i = 0; i < nboxes; ++i){
        dets[i].prob = calloc(l.classes, ଆକାର(ଦଶମିକ));
        ଯଦି(l.coords > 4){
            dets[i].mask = calloc(l.coords-4, ଆକାର(ଦଶମିକ));
        }
    }
    ଫେରନ୍ତୁ dets;
}

ଶୂନ୍ୟ fill_network_boxes(network *net, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ଦଶମିକ thresh, ଦଶମିକ hier, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *map, ପୂର୍ଣ୍ଣସଂଖ୍ୟା relative, detection *dets)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;
    ପାଇଁ(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        ଯଦି(l.type == YOLO){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
            dets += ଗଣନା_2;
        }
        ଯଦି(l.type == REGION){
            get_region_detections(l, w, h, net->w, net->h, thresh, map, hier, relative, dets);
            dets += l.w*l.h*l.n;
        }
        ଯଦି(l.type == DETECTION){
            get_detection_detections(l, w, h, thresh, dets);
            dets += l.w*l.h*l.n;
        }
    }
}

detection *get_network_boxes(network *net, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ଦଶମିକ thresh, ଦଶମିକ hier, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *map, ପୂର୍ଣ୍ଣସଂଖ୍ୟା relative, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ଅଙ୍କ)
{
    detection *dets = make_network_boxes(net, thresh, ଅଙ୍କ);
    fill_network_boxes(net, w, h, thresh, hier, map, relative, dets);
    ଫେରନ୍ତୁ dets;
}

ଶୂନ୍ୟ free_detections(detection *dets, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n; ++i){
        ମୁକ୍ତ(dets[i].prob);
        ଯଦି(dets[i].mask) ମୁକ୍ତ(dets[i].mask);
    }
    ମୁକ୍ତ(dets);
}

ଦଶମିକ *network_predict_image(network *net, image im)
{
    image imr = letterbox_image(im, net->w, net->h);
    set_batch_network(net, 1);
    ଦଶମିକ *p = network_predict(net, imr.data);
    free_image(imr);
    ଫେରନ୍ତୁ p;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା network_width(network *net){ଫେରନ୍ତୁ net->w;}
ପୂର୍ଣ୍ଣସଂଖ୍ୟା network_height(network *net){ଫେରନ୍ତୁ net->h;}

matrix network_predict_data_multi(network *net, data test, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,b,m;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    ଦଶମିକ *X = calloc(net->batch*test.X.rows, ଆକାର(ଦଶମିକ));
    ପାଇଁ(i = 0; i < test.X.rows; i += net->batch){
        ପାଇଁ(b = 0; b < net->batch; ++b){
            ଯଦି(i+b == test.X.rows) ଭାଙ୍ଗ;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*ଆକାର(ଦଶମିକ));
        }
        ପାଇଁ(m = 0; m < n; ++m){
            ଦଶମିକ *out = network_predict(net, X);
            ପାଇଁ(b = 0; b < net->batch; ++b){
                ଯଦି(i+b == test.X.rows) ଭାଙ୍ଗ;
                ପାଇଁ(j = 0; j < k; ++j){
                    pred.vals[i+b][j] += out[j+b*k]/n;
                }
            }
        }
    }
    ମୁକ୍ତ(X);
    ଫେରନ୍ତୁ pred;   
}

matrix network_predict_data(network *net, data test)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,b;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    ଦଶମିକ *X = calloc(net->batch*test.X.cols, ଆକାର(ଦଶମିକ));
    ପାଇଁ(i = 0; i < test.X.rows; i += net->batch){
        ପାଇଁ(b = 0; b < net->batch; ++b){
            ଯଦି(i+b == test.X.rows) ଭାଙ୍ଗ;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*ଆକାର(ଦଶମିକ));
        }
        ଦଶମିକ *out = network_predict(net, X);
        ପାଇଁ(b = 0; b < net->batch; ++b){
            ଯଦି(i+b == test.X.rows) ଭାଙ୍ଗ;
            ପାଇଁ(j = 0; j < k; ++j){
                pred.vals[i+b][j] = out[j+b*k];
            }
        }
    }
    ମୁକ୍ତ(X);
    ଫେରନ୍ତୁ pred;   
}

ଶୂନ୍ୟ print_network(network *net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j;
    ପାଇଁ(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ଦଶମିକ *output = l.output;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = l.outputs;
        ଦଶମିକ mean = mean_array(output, n);
        ଦଶମିକ vari = variance_array(output, n);
        fprintf(stderr, "Layer %d - Mean: %f, Variance: %f\n",i,mean, vari);
        ଯଦି(n > 100) n = 100;
        ପାଇଁ(j = 0; j < n; ++j) fprintf(stderr, "%f, ", output[j]);
        ଯଦି(n == 100)fprintf(stderr,".....\n");
        fprintf(stderr, "\n");
    }
}

ଶୂନ୍ୟ compare_networks(network *n1, network *n2, data test)
{
    matrix g1 = network_predict_data(n1, test);
    matrix g2 = network_predict_data(n2, test);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା a,b,c,d;
    a = b = c = d = 0;
    ପାଇଁ(i = 0; i < g1.rows; ++i){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା truth = max_index(test.y.vals[i], test.y.cols);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା p1 = max_index(g1.vals[i], g1.cols);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା p2 = max_index(g2.vals[i], g2.cols);
        ଯଦି(p1 == truth){
            ଯଦି(p2 == truth) ++d;
            ଅନ୍ୟଥା ++c;
        }ଅନ୍ୟଥା{
            ଯଦି(p2 == truth) ++b;
            ଅନ୍ୟଥା ++a;
        }
    }
    printf("%5d %5d\n%5d %5d\n", a, b, c, d);
    ଦଶମିକ ଅଙ୍କ = pow((abs(b - c) - 1.), 2.);
    ଦଶମିକ den = b + c;
    printf("%f\n", ଅଙ୍କ/den); 
}

ଦଶମିକ network_accuracy(network *net, data d)
{
    matrix guess = network_predict_data(net, d);
    ଦଶମିକ acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    ଫେରନ୍ତୁ acc;
}

ଦଶମିକ *network_accuracies(network *net, data d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ସ୍ଥିର ଦଶମିକ acc[2];
    matrix guess = network_predict_data(net, d);
    acc[0] = matrix_topk_accuracy(d.y, guess, 1);
    acc[1] = matrix_topk_accuracy(d.y, guess, n);
    free_matrix(guess);
    ଫେରନ୍ତୁ acc;
}

layer get_network_output_layer(network *net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = net->n - 1; i >= 0; --i){
        ଯଦି(net->layers[i].type != COST) ଭାଙ୍ଗ;
    }
    ଫେରନ୍ତୁ net->layers[i];
}

ଦଶମିକ network_accuracy_multi(network *net, data d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    matrix guess = network_predict_data_multi(net, d, n);
    ଦଶମିକ acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    ଫେରନ୍ତୁ acc;
}

ଶୂନ୍ୟ free_network(network *net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < net->n; ++i){
        free_layer(net->layers[i]);
    }
    ମୁକ୍ତ(net->layers);
    ଯଦି(net->ନିବେଶ) ମୁକ୍ତ(net->ନିବେଶ);
    ଯଦି(net->truth) ମୁକ୍ତ(net->truth);
#ifdef GPU
    ଯଦି(net->ନିବେଶ_gpu) cuda_free(net->ନିବେଶ_gpu);
    ଯଦି(net->truth_gpu) cuda_free(net->truth_gpu);
#endif
    ମୁକ୍ତ(net);
}

// Some day...
// ^ What the hell is this comment for?


layer network_output_layer(network *net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = net->n - 1; i >= 0; --i){
        ଯଦି(net->layers[i].type != COST) ଭାଙ୍ଗ;
    }
    ଫେରନ୍ତୁ net->layers[i];
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା network_inputs(network *net)
{
    ଫେରନ୍ତୁ net->layers[0].inputs;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା network_outputs(network *net)
{
    ଫେରନ୍ତୁ network_output_layer(net).outputs;
}

ଦଶମିକ *network_output(network *net)
{
    ଫେରନ୍ତୁ network_output_layer(net).output;
}

#ifdef GPU

ଶୂନ୍ୟ forward_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    cuda_ଠେଲ_array(net.ନିବେଶ_gpu, net.ନିବେଶ, net.inputs*net.batch);
    ଯଦି(net.truth){
        cuda_ଠେଲ_array(net.truth_gpu, net.truth, net.truths*net.batch);
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        ଯଦି(l.delta_gpu){
            fill_gpu(l.outputs * l.batch, 0, l.delta_gpu, 1);
        }
        l.forward_gpu(l, net);
        net.ନିବେଶ_gpu = l.output_gpu;
        net.ନିବେଶ = l.output;
        ଯଦି(l.truth) {
            net.truth_gpu = l.output_gpu;
            net.truth = l.output;
        }
    }
    pull_network_output(netp);
    calc_network_cost(netp);
}

ଶୂନ୍ୟ backward_network_gpu(network *netp)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    network net = *netp;
    network orig = net;
    cuda_set_device(net.gpu_index);
    ପାଇଁ(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        ଯଦି(l.stopbackward) ଭାଙ୍ଗ;
        ଯଦି(i == 0){
            net = orig;
        }ଅନ୍ୟଥା{
            layer prev = net.layers[i-1];
            net.ନିବେଶ = prev.output;
            net.delta = prev.delta;
            net.ନିବେଶ_gpu = prev.output_gpu;
            net.delta_gpu = prev.delta_gpu;
        }
        net.index = i;
        l.backward_gpu(l, net);
    }
}

ଶୂନ୍ୟ update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
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

    ପାଇଁ(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        ଯଦି(l.update_gpu){
            l.update_gpu(l, a);
        }
    }
}

ଶୂନ୍ୟ harmless_update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        ଯଦି(l.weight_updates_gpu) fill_gpu(l.nweights, 0, l.weight_updates_gpu, 1);
        ଯଦି(l.bias_updates_gpu) fill_gpu(l.nbiases, 0, l.bias_updates_gpu, 1);
        ଯଦି(l.scale_updates_gpu) fill_gpu(l.nbiases, 0, l.scale_updates_gpu, 1);
    }
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    network *net;
    data d;
    ଦଶମିକ *err;
} train_args;

ଶୂନ୍ୟ *train_thread(ଶୂନ୍ୟ *ptr)
{
    train_args args = *(train_args*)ptr;
    ମୁକ୍ତ(ptr);
    cuda_set_device(args.net->gpu_index);
    *args.err = train_network(args.net, args.d);
    ଫେରନ୍ତୁ 0;
}

pthread_t train_network_in_thread(network *net, data d, ଦଶମିକ *err)
{
    pthread_t thread;
    train_args *ptr = (train_args *)calloc(1, ଆକାର(train_args));
    ptr->net = net;
    ptr->d = d;
    ptr->err = err;
    ଯଦି(pthread_create(&thread, 0, train_thread, ptr)) error("Thread creation failed");
    ଫେରନ୍ତୁ thread;
}

ଶୂନ୍ୟ merge_weights(layer l, layer base)
{
    ଯଦି (l.type == CONVOLUTIONAL) {
        axpy_cpu(l.n, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.nweights, 1, l.weight_updates, 1, base.weights, 1);
        ଯଦି (l.scales) {
            axpy_cpu(l.n, 1, l.scale_updates, 1, base.scales, 1);
        }
    } ଅନ୍ୟଥା ଯଦି(l.type == CONNECTED) {
        axpy_cpu(l.outputs, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.outputs*l.inputs, 1, l.weight_updates, 1, base.weights, 1);
    }
}

ଶୂନ୍ୟ scale_weights(layer l, ଦଶମିକ s)
{
    ଯଦି (l.type == CONVOLUTIONAL) {
        scal_cpu(l.n, s, l.biases, 1);
        scal_cpu(l.nweights, s, l.weights, 1);
        ଯଦି (l.scales) {
            scal_cpu(l.n, s, l.scales, 1);
        }
    } ଅନ୍ୟଥା ଯଦି(l.type == CONNECTED) {
        scal_cpu(l.outputs, s, l.biases, 1);
        scal_cpu(l.outputs*l.inputs, s, l.weights, 1);
    }
}


ଶୂନ୍ୟ pull_weights(layer l)
{
    ଯଦି(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.n);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.nweights);
        ଯଦି(l.scales) cuda_pull_array(l.scales_gpu, l.scale_updates, l.n);
    } ଅନ୍ୟଥା ଯଦି(l.type == CONNECTED){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.outputs);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.outputs*l.inputs);
    }
}

ଶୂନ୍ୟ ଠେଲ_weights(layer l)
{
    ଯଦି(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_ଠେଲ_array(l.biases_gpu, l.biases, l.n);
        cuda_ଠେଲ_array(l.weights_gpu, l.weights, l.nweights);
        ଯଦି(l.scales) cuda_ଠେଲ_array(l.scales_gpu, l.scales, l.n);
    } ଅନ୍ୟଥା ଯଦି(l.type == CONNECTED){
        cuda_ଠେଲ_array(l.biases_gpu, l.biases, l.outputs);
        cuda_ଠେଲ_array(l.weights_gpu, l.weights, l.outputs*l.inputs);
    }
}

ଶୂନ୍ୟ distribute_weights(layer l, layer base)
{
    ଯଦି (l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL) {
        cuda_ଠେଲ_array(l.biases_gpu, base.biases, l.n);
        cuda_ଠେଲ_array(l.weights_gpu, base.weights, l.nweights);
        ଯଦି (base.scales) cuda_ଠେଲ_array(l.scales_gpu, base.scales, l.n);
    } ଅନ୍ୟଥା ଯଦି (l.type == CONNECTED) {
        cuda_ଠେଲ_array(l.biases_gpu, base.biases, l.outputs);
        cuda_ଠେଲ_array(l.weights_gpu, base.weights, l.outputs*l.inputs);
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

ଶୂନ୍ୟ sync_layer(network **nets, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା j)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    network *net = nets[0];
    layer base = net->layers[j];
    scale_weights(base, 0);
    ପାଇଁ (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        pull_weights(l);
        merge_weights(l, base);
    }
    scale_weights(base, 1./n);
    ପାଇଁ (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        distribute_weights(l, base);
    }
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା{
    network **nets;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;
} sync_args;

ଶୂନ୍ୟ *sync_layer_thread(ଶୂନ୍ୟ *ptr)
{
    sync_args args = *(sync_args*)ptr;
    sync_layer(args.nets, args.n, args.j);
    ମୁକ୍ତ(ptr);
    ଫେରନ୍ତୁ 0;
}

pthread_t sync_layer_in_thread(network **nets, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା j)
{
    pthread_t thread;
    sync_args *ptr = (sync_args *)calloc(1, ଆକାର(sync_args));
    ptr->nets = nets;
    ptr->n = n;
    ptr->j = j;
    ଯଦି(pthread_create(&thread, 0, sync_layer_thread, ptr)) error("Thread creation failed");
    ଫେରନ୍ତୁ thread;
}

ଶୂନ୍ୟ sync_nets(network **nets, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା interval)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା layers = nets[0]->n;
    pthread_t *threads = (pthread_t *) calloc(layers, ଆକାର(pthread_t));

    *(nets[0]->seen) += interval * (n-1) * nets[0]->batch * nets[0]->subdivisions;
    ପାଇଁ (j = 0; j < n; ++j){
        *(nets[j]->seen) = *(nets[0]->seen);
    }
    ପାଇଁ (j = 0; j < layers; ++j) {
        threads[j] = sync_layer_in_thread(nets, n, j);
    }
    ପାଇଁ (j = 0; j < layers; ++j) {
        pthread_join(threads[j], 0);
    }
    ମୁକ୍ତ(threads);
}

ଦଶମିକ train_networks(network **nets, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, data d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା interval)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch = nets[0]->batch;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା subdivisions = nets[0]->subdivisions;
    ପ୍ରତିଜ୍ଞା(batch * subdivisions * n == d.X.rows);
    pthread_t *threads = (pthread_t *) calloc(n, ଆକାର(pthread_t));
    ଦଶମିକ *errors = (ଦଶମିକ *) calloc(n, ଆକାର(ଦଶମିକ));

    ଦଶମିକ sum = 0;
    ପାଇଁ(i = 0; i < n; ++i){
        data p = get_data_part(d, i, n);
        threads[i] = train_network_in_thread(nets[i], p, errors + i);
    }
    ପାଇଁ(i = 0; i < n; ++i){
        pthread_join(threads[i], 0);
        //printf("%f\n", errors[i]);
        sum += errors[i];
    }
    //cudaDeviceSynchronize();
    ଯଦି (get_current_batch(nets[0]) % interval == 0) {
        printf("Syncing... ");
        fflush(stdout);
        sync_nets(nets, n, interval);
        printf("Done!\n");
    }
    //cudaDeviceSynchronize();
    ମୁକ୍ତ(threads);
    ମୁକ୍ତ(errors);
    ଫେରନ୍ତୁ (ଦଶମିକ)sum/(n);
}

ଶୂନ୍ୟ pull_network_output(network *net)
{
    layer l = get_network_output_layer(net);
    cuda_pull_array(l.output_gpu, l.output, l.outputs*l.batch);
}

#endif

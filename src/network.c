#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ "network.h"
#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "data.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "blas.h"

#ಅನ್ತರ್ಭಾವ "crop_layer.h"
#ಅನ್ತರ್ಭಾವ "connected_layer.h"
#ಅನ್ತರ್ಭಾವ "gru_layer.h"
#ಅನ್ತರ್ಭಾವ "rnn_layer.h"
#ಅನ್ತರ್ಭಾವ "crnn_layer.h"
#ಅನ್ತರ್ಭಾವ "local_layer.h"
#ಅನ್ತರ್ಭಾವ "convolutional_layer.h"
#ಅನ್ತರ್ಭಾವ "activation_layer.h"
#ಅನ್ತರ್ಭಾವ "detection_layer.h"
#ಅನ್ತರ್ಭಾವ "region_layer.h"
#ಅನ್ತರ್ಭಾವ "yolo_layer.h"
#ಅನ್ತರ್ಭಾವ "normalization_layer.h"
#ಅನ್ತರ್ಭಾವ "batchnorm_layer.h"
#ಅನ್ತರ್ಭಾವ "maxpool_layer.h"
#ಅನ್ತರ್ಭಾವ "reorg_layer.h"
#ಅನ್ತರ್ಭಾವ "avgpool_layer.h"
#ಅನ್ತರ್ಭಾವ "cost_layer.h"
#ಅನ್ತರ್ಭಾವ "softmax_layer.h"
#ಅನ್ತರ್ಭಾವ "dropout_layer.h"
#ಅನ್ತರ್ಭಾವ "route_layer.h"
#ಅನ್ತರ್ಭಾವ "upsample_layer.h"
#ಅನ್ತರ್ಭಾವ "shortcut_layer.h"
#ಅನ್ತರ್ಭಾವ "parser.h"
#ಅನ್ತರ್ಭಾವ "data.h"

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
    ಪ್ರತ್ಯಾಗಮ args;
}

network *load_network(ವರ್ಣ *cfg, ವರ್ಣ *weights, ಪೂರ್ಣಾಙ್ಕ clear)
{
    network *net = parse_network_cfg(cfg);
    ಯದಿ(weights && weights[0] != 0){
        load_weights(net, weights);
    }
    ಯದಿ(clear) (*net->seen) = 0;
    ಪ್ರತ್ಯಾಗಮ net;
}

size_t get_current_batch(network *net)
{
    size_t batch_ಅಂಕೆ = (*net->seen)/(net->batch*net->subdivisions);
    ಪ್ರತ್ಯಾಗಮ batch_ಅಂಕೆ;
}

ಶೂನ್ಯ reset_network_state(network *net, ಪೂರ್ಣಾಙ್ಕ b)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ (i = 0; i < net->n; ++i) {
        #ifdef GPU
        layer l = net->layers[i];
        ಯದಿ(l.state_gpu){
            fill_gpu(l.outputs, 0, l.state_gpu + l.outputs*b, 1);
        }
        ಯದಿ(l.h_gpu){
            fill_gpu(l.outputs, 0, l.h_gpu + l.outputs*b, 1);
        }
        #endif
    }
}

ಶೂನ್ಯ reset_rnn(network *net)
{
    reset_network_state(net, 0);
}

ಭಿನ್ನ get_current_rate(network *net)
{
    size_t batch_ಅಂಕೆ = get_current_batch(net);
    ಪೂರ್ಣಾಙ್ಕ i;
    ಭಿನ್ನ rate;
    ಯದಿ (batch_ಅಂಕೆ < net->burn_in) ಪ್ರತ್ಯಾಗಮ net->learning_rate * pow((ಭಿನ್ನ)batch_ಅಂಕೆ / net->burn_in, net->power);
    ವಿಕಲ್ಪ (net->policy) {
        ಪಕ್ಷ CONSTANT:
            ಪ್ರತ್ಯಾಗಮ net->learning_rate;
        ಪಕ್ಷ STEP:
            ಪ್ರತ್ಯಾಗಮ net->learning_rate * pow(net->scale, batch_ಅಂಕೆ/net->step);
        ಪಕ್ಷ STEPS:
            rate = net->learning_rate;
            ಪ್ರತಿ(i = 0; i < net->ಅಂಕೆ_steps; ++i){
                ಯದಿ(net->steps[i] > batch_ಅಂಕೆ) ಪ್ರತ್ಯಾಗಮ rate;
                rate *= net->scales[i];
            }
            ಪ್ರತ್ಯಾಗಮ rate;
        ಪಕ್ಷ EXP:
            ಪ್ರತ್ಯಾಗಮ net->learning_rate * pow(net->gamma, batch_ಅಂಕೆ);
        ಪಕ್ಷ POLY:
            ಪ್ರತ್ಯಾಗಮ net->learning_rate * pow(1 - (ಭಿನ್ನ)batch_ಅಂಕೆ / net->max_batches, net->power);
        ಪಕ್ಷ RANDOM:
            ಪ್ರತ್ಯಾಗಮ net->learning_rate * pow(rand_uniform(0,1), net->power);
        ಪಕ್ಷ SIG:
            ಪ್ರತ್ಯಾಗಮ net->learning_rate * (1./(1.+exp(net->gamma*(batch_ಅಂಕೆ - net->step))));
        ಸ್ವಭಾವ:
            fprintf(stderr, "Policy is weird!\n");
            ಪ್ರತ್ಯಾಗಮ net->learning_rate;
    }
}

ವರ್ಣ *get_layer_string(LAYER_TYPE a)
{
    ವಿಕಲ್ಪ(a){
        ಪಕ್ಷ CONVOLUTIONAL:
            ಪ್ರತ್ಯಾಗಮ "convolutional";
        ಪಕ್ಷ ACTIVE:
            ಪ್ರತ್ಯಾಗಮ "activation";
        ಪಕ್ಷ LOCAL:
            ಪ್ರತ್ಯಾಗಮ "local";
        ಪಕ್ಷ DECONVOLUTIONAL:
            ಪ್ರತ್ಯಾಗಮ "deconvolutional";
        ಪಕ್ಷ CONNECTED:
            ಪ್ರತ್ಯಾಗಮ "connected";
        ಪಕ್ಷ RNN:
            ಪ್ರತ್ಯಾಗಮ "rnn";
        ಪಕ್ಷ GRU:
            ಪ್ರತ್ಯಾಗಮ "gru";
        ಪಕ್ಷ LSTM:
	    ಪ್ರತ್ಯಾಗಮ "lstm";
        ಪಕ್ಷ CRNN:
            ಪ್ರತ್ಯಾಗಮ "crnn";
        ಪಕ್ಷ MAXPOOL:
            ಪ್ರತ್ಯಾಗಮ "maxpool";
        ಪಕ್ಷ REORG:
            ಪ್ರತ್ಯಾಗಮ "reorg";
        ಪಕ್ಷ AVGPOOL:
            ಪ್ರತ್ಯಾಗಮ "avgpool";
        ಪಕ್ಷ SOFTMAX:
            ಪ್ರತ್ಯಾಗಮ "softmax";
        ಪಕ್ಷ DETECTION:
            ಪ್ರತ್ಯಾಗಮ "detection";
        ಪಕ್ಷ REGION:
            ಪ್ರತ್ಯಾಗಮ "region";
        ಪಕ್ಷ YOLO:
            ಪ್ರತ್ಯಾಗಮ "yolo";
        ಪಕ್ಷ DROPOUT:
            ಪ್ರತ್ಯಾಗಮ "dropout";
        ಪಕ್ಷ CROP:
            ಪ್ರತ್ಯಾಗಮ "crop";
        ಪಕ್ಷ COST:
            ಪ್ರತ್ಯಾಗಮ "cost";
        ಪಕ್ಷ ROUTE:
            ಪ್ರತ್ಯಾಗಮ "route";
        ಪಕ್ಷ SHORTCUT:
            ಪ್ರತ್ಯಾಗಮ "shortcut";
        ಪಕ್ಷ NORMALIZATION:
            ಪ್ರತ್ಯಾಗಮ "normalization";
        ಪಕ್ಷ BATCHNORM:
            ಪ್ರತ್ಯಾಗಮ "batchnorm";
        ಸ್ವಭಾವ:
            ಭಙ್ಗ;
    }
    ಪ್ರತ್ಯಾಗಮ "none";
}

network *make_network(ಪೂರ್ಣಾಙ್ಕ n)
{
    network *net = calloc(1, ಪರಿಮಾಣ(network));
    net->n = n;
    net->layers = calloc(net->n, ಪರಿಮಾಣ(layer));
    net->seen = calloc(1, ಪರಿಮಾಣ(size_t));
    net->t    = calloc(1, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ));
    net->cost = calloc(1, ಪರಿಮಾಣ(ಭಿನ್ನ));
    ಪ್ರತ್ಯಾಗಮ net;
}

ಶೂನ್ಯ forward_network(network *netp)
{
#ifdef GPU
    ಯದಿ(netp->gpu_index >= 0){
        forward_network_gpu(netp);   
        ಪ್ರತ್ಯಾಗಮ;
    }
#endif
    network net = *netp;
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        ಯದಿ(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.ಒಳಹರಿವು = l.output;
        ಯದಿ(l.truth) {
            net.truth = l.output;
        }
    }
    calc_network_cost(netp);
}

ಶೂನ್ಯ update_network(network *netp)
{
#ifdef GPU
    ಯದಿ(netp->gpu_index >= 0){
        update_network_gpu(netp);   
        ಪ್ರತ್ಯಾಗಮ;
    }
#endif
    network net = *netp;
    ಪೂರ್ಣಾಙ್ಕ i;
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

    ಪ್ರತಿ(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        ಯದಿ(l.update){
            l.update(l, a);
        }
    }
}

ಶೂನ್ಯ calc_network_cost(network *netp)
{
    network net = *netp;
    ಪೂರ್ಣಾಙ್ಕ i;
    ಭಿನ್ನ sum = 0;
    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
    ಪ್ರತಿ(i = 0; i < net.n; ++i){
        ಯದಿ(net.layers[i].cost){
            sum += net.layers[i].cost[0];
            ++ಎಣಿಕೆ;
        }
    }
    *net.cost = sum/ಎಣಿಕೆ;
}

ಪೂರ್ಣಾಙ್ಕ get_predicted_class_network(network *net)
{
    ಪ್ರತ್ಯಾಗಮ max_index(net->output, net->outputs);
}

ಶೂನ್ಯ backward_network(network *netp)
{
#ifdef GPU
    ಯದಿ(netp->gpu_index >= 0){
        backward_network_gpu(netp);   
        ಪ್ರತ್ಯಾಗಮ;
    }
#endif
    network net = *netp;
    ಪೂರ್ಣಾಙ್ಕ i;
    network orig = net;
    ಪ್ರತಿ(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        ಯದಿ(l.stopbackward) ಭಙ್ಗ;
        ಯದಿ(i == 0){
            net = orig;
        }ಅನ್ಯಥಾ{
            layer prev = net.layers[i-1];
            net.ಒಳಹರಿವು = prev.output;
            net.delta = prev.delta;
        }
        net.index = i;
        l.backward(l, net);
    }
}

ಭಿನ್ನ train_network_datum(network *net)
{
    *net->seen += net->batch;
    net->train = 1;
    forward_network(net);
    backward_network(net);
    ಭಿನ್ನ error = *net->cost;
    ಯದಿ(((*net->seen)/net->batch)%net->subdivisions == 0) update_network(net);
    ಪ್ರತ್ಯಾಗಮ error;
}

ಭಿನ್ನ train_network_sgd(network *net, data d, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಪೂರ್ಣಾಙ್ಕ batch = net->batch;

    ಪೂರ್ಣಾಙ್ಕ i;
    ಭಿನ್ನ sum = 0;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        get_random_batch(d, batch, net->ಒಳಹರಿವು, net->truth);
        ಭಿನ್ನ err = train_network_datum(net);
        sum += err;
    }
    ಪ್ರತ್ಯಾಗಮ (ಭಿನ್ನ)sum/(n*batch);
}

ಭಿನ್ನ train_network(network *net, data d)
{
    ಪ್ರತಿಜ್ಞಾ(d.X.rows % net->batch == 0);
    ಪೂರ್ಣಾಙ್ಕ batch = net->batch;
    ಪೂರ್ಣಾಙ್ಕ n = d.X.rows / batch;

    ಪೂರ್ಣಾಙ್ಕ i;
    ಭಿನ್ನ sum = 0;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        get_next_batch(d, batch, i*batch, net->ಒಳಹರಿವು, net->truth);
        ಭಿನ್ನ err = train_network_datum(net);
        sum += err;
    }
    ಪ್ರತ್ಯಾಗಮ (ಭಿನ್ನ)sum/(n*batch);
}

ಶೂನ್ಯ set_temp_network(network *net, ಭಿನ್ನ t)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < net->n; ++i){
        net->layers[i].temperature = t;
    }
}


ಶೂನ್ಯ set_batch_network(network *net, ಪೂರ್ಣಾಙ್ಕ b)
{
    net->batch = b;
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#ifdef CUDNN
        ಯದಿ(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_setup(net->layers + i);
        }
        ಯದಿ(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, l->out_h, l->out_w);
            cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 
        }
#endif
    }
}

ಪೂರ್ಣಾಙ್ಕ resize_network(network *net, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
#ifdef GPU
    cuda_set_device(net->gpu_index);
    cuda_free(net->workspace);
#endif
    ಪೂರ್ಣಾಙ್ಕ i;
    //if(w == net->w && h == net->h) return 0;
    net->w = w;
    net->h = h;
    ಪೂರ್ಣಾಙ್ಕ inputs = 0;
    size_t workspace_size = 0;
    //fprintf(stderr, "Resizing to %d x %d...\n", w, h);
    //fflush(stderr);
    ಪ್ರತಿ (i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ಯದಿ(l.type == CONVOLUTIONAL){
            resize_convolutional_layer(&l, w, h);
        }ಅನ್ಯಥಾ ಯದಿ(l.type == CROP){
            resize_crop_layer(&l, w, h);
        }ಅನ್ಯಥಾ ಯದಿ(l.type == MAXPOOL){
            resize_maxpool_layer(&l, w, h);
        }ಅನ್ಯಥಾ ಯದಿ(l.type == REGION){
            resize_region_layer(&l, w, h);
        }ಅನ್ಯಥಾ ಯದಿ(l.type == YOLO){
            resize_yolo_layer(&l, w, h);
        }ಅನ್ಯಥಾ ಯದಿ(l.type == ROUTE){
            resize_route_layer(&l, net);
        }ಅನ್ಯಥಾ ಯದಿ(l.type == SHORTCUT){
            resize_shortcut_layer(&l, w, h);
        }ಅನ್ಯಥಾ ಯದಿ(l.type == UPSAMPLE){
            resize_upsample_layer(&l, w, h);
        }ಅನ್ಯಥಾ ಯದಿ(l.type == REORG){
            resize_reorg_layer(&l, w, h);
        }ಅನ್ಯಥಾ ಯದಿ(l.type == AVGPOOL){
            resize_avgpool_layer(&l, w, h);
        }ಅನ್ಯಥಾ ಯದಿ(l.type == NORMALIZATION){
            resize_normalization_layer(&l, w, h);
        }ಅನ್ಯಥಾ ಯದಿ(l.type == COST){
            resize_cost_layer(&l, inputs);
        }ಅನ್ಯಥಾ{
            error("Cannot resize this type of layer");
        }
        ಯದಿ(l.workspace_size > workspace_size) workspace_size = l.workspace_size;
        ಯದಿ(l.workspace_size > 2000000000) ಪ್ರತಿಜ್ಞಾ(0);
        inputs = l.outputs;
        net->layers[i] = l;
        w = l.out_w;
        h = l.out_h;
        ಯದಿ(l.type == AVGPOOL) ಭಙ್ಗ;
    }
    layer out = get_network_output_layer(net);
    net->inputs = net->layers[0].inputs;
    net->outputs = out.outputs;
    net->truths = out.outputs;
    ಯದಿ(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    ಮುಕ್ತ(net->ಒಳಹರಿವು);
    ಮುಕ್ತ(net->truth);
    net->ಒಳಹರಿವು = calloc(net->inputs*net->batch, ಪರಿಮಾಣ(ಭಿನ್ನ));
    net->truth = calloc(net->truths*net->batch, ಪರಿಮಾಣ(ಭಿನ್ನ));
#ifdef GPU
    ಯದಿ(gpu_index >= 0){
        cuda_free(net->ಒಳಹರಿವು_gpu);
        cuda_free(net->truth_gpu);
        net->ಒಳಹರಿವು_gpu = cuda_make_array(net->ಒಳಹರಿವು, net->inputs*net->batch);
        net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
        ಯದಿ(workspace_size){
            net->workspace = cuda_make_array(0, (workspace_size-1)/ಪರಿಮಾಣ(ಭಿನ್ನ)+1);
        }
    }ಅನ್ಯಥಾ {
        ಮುಕ್ತ(net->workspace);
        net->workspace = calloc(1, workspace_size);
    }
#ಅನ್ಯಥಾ
    ಮುಕ್ತ(net->workspace);
    net->workspace = calloc(1, workspace_size);
#endif
    //fprintf(stderr, " Done!\n");
    ಪ್ರತ್ಯಾಗಮ 0;
}

layer get_network_detection_layer(network *net)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < net->n; ++i){
        ಯದಿ(net->layers[i].type == DETECTION){
            ಪ್ರತ್ಯಾಗಮ net->layers[i];
        }
    }
    fprintf(stderr, "Detection layer not found!!\n");
    layer l = {0};
    ಪ್ರತ್ಯಾಗಮ l;
}

image get_network_image_layer(network *net, ಪೂರ್ಣಾಙ್ಕ i)
{
    layer l = net->layers[i];
#ifdef GPU
    //cuda_pull_array(l.output_gpu, l.output, l.outputs);
#endif
    ಯದಿ (l.out_w && l.out_h && l.out_c){
        ಪ್ರತ್ಯಾಗಮ float_to_image(l.out_w, l.out_h, l.out_c, l.output);
    }
    image def = {0};
    ಪ್ರತ್ಯಾಗಮ def;
}

image get_network_image(network *net)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = net->n-1; i >= 0; --i){
        image m = get_network_image_layer(net, i);
        ಯದಿ(m.h != 0) ಪ್ರತ್ಯಾಗಮ m;
    }
    image def = {0};
    ಪ್ರತ್ಯಾಗಮ def;
}

ಶೂನ್ಯ visualize_network(network *net)
{
    image *prev = 0;
    ಪೂರ್ಣಾಙ್ಕ i;
    ವರ್ಣ buff[256];
    ಪ್ರತಿ(i = 0; i < net->n; ++i){
        sprintf(buff, "Layer %d", i);
        layer l = net->layers[i];
        ಯದಿ(l.type == CONVOLUTIONAL){
            prev = visualize_convolutional_layer(l, buff, prev);
        }
    } 
}

ಶೂನ್ಯ ತುದಿ_predictions(network *net, ಪೂರ್ಣಾಙ್ಕ k, ಪೂರ್ಣಾಙ್ಕ *index)
{
    ತುದಿ_k(net->output, net->outputs, k, index);
}


ಭಿನ್ನ *network_predict(network *net, ಭಿನ್ನ *ಒಳಹರಿವು)
{
    network orig = *net;
    net->ಒಳಹರಿವು = ಒಳಹರಿವು;
    net->truth = 0;
    net->train = 0;
    net->delta = 0;
    forward_network(net);
    ಭಿನ್ನ *out = net->output;
    *net = orig;
    ಪ್ರತ್ಯಾಗಮ out;
}

ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_detections(network *net, ಭಿನ್ನ thresh)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪೂರ್ಣಾಙ್ಕ s = 0;
    ಪ್ರತಿ(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ಯದಿ(l.type == YOLO){
            s += yolo_ಅಂಕೆ_detections(l, thresh);
        }
        ಯದಿ(l.type == DETECTION || l.type == REGION){
            s += l.w*l.h*l.n;
        }
    }
    ಪ್ರತ್ಯಾಗಮ s;
}

detection *make_network_boxes(network *net, ಭಿನ್ನ thresh, ಪೂರ್ಣಾಙ್ಕ *ಅಂಕೆ)
{
    layer l = net->layers[net->n - 1];
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪೂರ್ಣಾಙ್ಕ nboxes = ಅಂಕೆ_detections(net, thresh);
    ಯದಿ(ಅಂಕೆ) *ಅಂಕೆ = nboxes;
    detection *dets = calloc(nboxes, ಪರಿಮಾಣ(detection));
    ಪ್ರತಿ(i = 0; i < nboxes; ++i){
        dets[i].prob = calloc(l.classes, ಪರಿಮಾಣ(ಭಿನ್ನ));
        ಯದಿ(l.coords > 4){
            dets[i].mask = calloc(l.coords-4, ಪರಿಮಾಣ(ಭಿನ್ನ));
        }
    }
    ಪ್ರತ್ಯಾಗಮ dets;
}

ಶೂನ್ಯ fill_network_boxes(network *net, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಭಿನ್ನ thresh, ಭಿನ್ನ hier, ಪೂರ್ಣಾಙ್ಕ *map, ಪೂರ್ಣಾಙ್ಕ relative, detection *dets)
{
    ಪೂರ್ಣಾಙ್ಕ j;
    ಪ್ರತಿ(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        ಯದಿ(l.type == YOLO){
            ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
            dets += ಎಣಿಕೆ;
        }
        ಯದಿ(l.type == REGION){
            get_region_detections(l, w, h, net->w, net->h, thresh, map, hier, relative, dets);
            dets += l.w*l.h*l.n;
        }
        ಯದಿ(l.type == DETECTION){
            get_detection_detections(l, w, h, thresh, dets);
            dets += l.w*l.h*l.n;
        }
    }
}

detection *get_network_boxes(network *net, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಭಿನ್ನ thresh, ಭಿನ್ನ hier, ಪೂರ್ಣಾಙ್ಕ *map, ಪೂರ್ಣಾಙ್ಕ relative, ಪೂರ್ಣಾಙ್ಕ *ಅಂಕೆ)
{
    detection *dets = make_network_boxes(net, thresh, ಅಂಕೆ);
    fill_network_boxes(net, w, h, thresh, hier, map, relative, dets);
    ಪ್ರತ್ಯಾಗಮ dets;
}

ಶೂನ್ಯ free_detections(detection *dets, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        ಮುಕ್ತ(dets[i].prob);
        ಯದಿ(dets[i].mask) ಮುಕ್ತ(dets[i].mask);
    }
    ಮುಕ್ತ(dets);
}

ಭಿನ್ನ *network_predict_image(network *net, image im)
{
    image imr = letterbox_image(im, net->w, net->h);
    set_batch_network(net, 1);
    ಭಿನ್ನ *p = network_predict(net, imr.data);
    free_image(imr);
    ಪ್ರತ್ಯಾಗಮ p;
}

ಪೂರ್ಣಾಙ್ಕ network_width(network *net){ಪ್ರತ್ಯಾಗಮ net->w;}
ಪೂರ್ಣಾಙ್ಕ network_height(network *net){ಪ್ರತ್ಯಾಗಮ net->h;}

matrix network_predict_data_multi(network *net, data test, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಪೂರ್ಣಾಙ್ಕ i,j,b,m;
    ಪೂರ್ಣಾಙ್ಕ k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    ಭಿನ್ನ *X = calloc(net->batch*test.X.rows, ಪರಿಮಾಣ(ಭಿನ್ನ));
    ಪ್ರತಿ(i = 0; i < test.X.rows; i += net->batch){
        ಪ್ರತಿ(b = 0; b < net->batch; ++b){
            ಯದಿ(i+b == test.X.rows) ಭಙ್ಗ;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*ಪರಿಮಾಣ(ಭಿನ್ನ));
        }
        ಪ್ರತಿ(m = 0; m < n; ++m){
            ಭಿನ್ನ *out = network_predict(net, X);
            ಪ್ರತಿ(b = 0; b < net->batch; ++b){
                ಯದಿ(i+b == test.X.rows) ಭಙ್ಗ;
                ಪ್ರತಿ(j = 0; j < k; ++j){
                    pred.vals[i+b][j] += out[j+b*k]/n;
                }
            }
        }
    }
    ಮುಕ್ತ(X);
    ಪ್ರತ್ಯಾಗಮ pred;   
}

matrix network_predict_data(network *net, data test)
{
    ಪೂರ್ಣಾಙ್ಕ i,j,b;
    ಪೂರ್ಣಾಙ್ಕ k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    ಭಿನ್ನ *X = calloc(net->batch*test.X.cols, ಪರಿಮಾಣ(ಭಿನ್ನ));
    ಪ್ರತಿ(i = 0; i < test.X.rows; i += net->batch){
        ಪ್ರತಿ(b = 0; b < net->batch; ++b){
            ಯದಿ(i+b == test.X.rows) ಭಙ್ಗ;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*ಪರಿಮಾಣ(ಭಿನ್ನ));
        }
        ಭಿನ್ನ *out = network_predict(net, X);
        ಪ್ರತಿ(b = 0; b < net->batch; ++b){
            ಯದಿ(i+b == test.X.rows) ಭಙ್ಗ;
            ಪ್ರತಿ(j = 0; j < k; ++j){
                pred.vals[i+b][j] = out[j+b*k];
            }
        }
    }
    ಮುಕ್ತ(X);
    ಪ್ರತ್ಯಾಗಮ pred;   
}

ಶೂನ್ಯ print_network(network *net)
{
    ಪೂರ್ಣಾಙ್ಕ i,j;
    ಪ್ರತಿ(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ಭಿನ್ನ *output = l.output;
        ಪೂರ್ಣಾಙ್ಕ n = l.outputs;
        ಭಿನ್ನ mean = mean_array(output, n);
        ಭಿನ್ನ vari = variance_array(output, n);
        fprintf(stderr, "Layer %d - Mean: %f, Variance: %f\n",i,mean, vari);
        ಯದಿ(n > 100) n = 100;
        ಪ್ರತಿ(j = 0; j < n; ++j) fprintf(stderr, "%f, ", output[j]);
        ಯದಿ(n == 100)fprintf(stderr,".....\n");
        fprintf(stderr, "\n");
    }
}

ಶೂನ್ಯ compare_networks(network *n1, network *n2, data test)
{
    matrix g1 = network_predict_data(n1, test);
    matrix g2 = network_predict_data(n2, test);
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪೂರ್ಣಾಙ್ಕ a,b,c,d;
    a = b = c = d = 0;
    ಪ್ರತಿ(i = 0; i < g1.rows; ++i){
        ಪೂರ್ಣಾಙ್ಕ truth = max_index(test.y.vals[i], test.y.cols);
        ಪೂರ್ಣಾಙ್ಕ p1 = max_index(g1.vals[i], g1.cols);
        ಪೂರ್ಣಾಙ್ಕ p2 = max_index(g2.vals[i], g2.cols);
        ಯದಿ(p1 == truth){
            ಯದಿ(p2 == truth) ++d;
            ಅನ್ಯಥಾ ++c;
        }ಅನ್ಯಥಾ{
            ಯದಿ(p2 == truth) ++b;
            ಅನ್ಯಥಾ ++a;
        }
    }
    printf("%5d %5d\n%5d %5d\n", a, b, c, d);
    ಭಿನ್ನ ಅಂಕೆ = pow((abs(b - c) - 1.), 2.);
    ಭಿನ್ನ den = b + c;
    printf("%f\n", ಅಂಕೆ/den); 
}

ಭಿನ್ನ network_accuracy(network *net, data d)
{
    matrix guess = network_predict_data(net, d);
    ಭಿನ್ನ acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    ಪ್ರತ್ಯಾಗಮ acc;
}

ಭಿನ್ನ *network_accuracies(network *net, data d, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಸ್ಥಿರ ಭಿನ್ನ acc[2];
    matrix guess = network_predict_data(net, d);
    acc[0] = matrix_topk_accuracy(d.y, guess, 1);
    acc[1] = matrix_topk_accuracy(d.y, guess, n);
    free_matrix(guess);
    ಪ್ರತ್ಯಾಗಮ acc;
}

layer get_network_output_layer(network *net)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = net->n - 1; i >= 0; --i){
        ಯದಿ(net->layers[i].type != COST) ಭಙ್ಗ;
    }
    ಪ್ರತ್ಯಾಗಮ net->layers[i];
}

ಭಿನ್ನ network_accuracy_multi(network *net, data d, ಪೂರ್ಣಾಙ್ಕ n)
{
    matrix guess = network_predict_data_multi(net, d, n);
    ಭಿನ್ನ acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    ಪ್ರತ್ಯಾಗಮ acc;
}

ಶೂನ್ಯ free_network(network *net)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < net->n; ++i){
        free_layer(net->layers[i]);
    }
    ಮುಕ್ತ(net->layers);
    ಯದಿ(net->ಒಳಹರಿವು) ಮುಕ್ತ(net->ಒಳಹರಿವು);
    ಯದಿ(net->truth) ಮುಕ್ತ(net->truth);
#ifdef GPU
    ಯದಿ(net->ಒಳಹರಿವು_gpu) cuda_free(net->ಒಳಹರಿವು_gpu);
    ಯದಿ(net->truth_gpu) cuda_free(net->truth_gpu);
#endif
    ಮುಕ್ತ(net);
}

// Some day...
// ^ What the hell is this comment for?


layer network_output_layer(network *net)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = net->n - 1; i >= 0; --i){
        ಯದಿ(net->layers[i].type != COST) ಭಙ್ಗ;
    }
    ಪ್ರತ್ಯಾಗಮ net->layers[i];
}

ಪೂರ್ಣಾಙ್ಕ network_inputs(network *net)
{
    ಪ್ರತ್ಯಾಗಮ net->layers[0].inputs;
}

ಪೂರ್ಣಾಙ್ಕ network_outputs(network *net)
{
    ಪ್ರತ್ಯಾಗಮ network_output_layer(net).outputs;
}

ಭಿನ್ನ *network_output(network *net)
{
    ಪ್ರತ್ಯಾಗಮ network_output_layer(net).output;
}

#ifdef GPU

ಶೂನ್ಯ forward_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    cuda_ತಳ್ಳು_array(net.ಒಳಹರಿವು_gpu, net.ಒಳಹರಿವು, net.inputs*net.batch);
    ಯದಿ(net.truth){
        cuda_ತಳ್ಳು_array(net.truth_gpu, net.truth, net.truths*net.batch);
    }

    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        ಯದಿ(l.delta_gpu){
            fill_gpu(l.outputs * l.batch, 0, l.delta_gpu, 1);
        }
        l.forward_gpu(l, net);
        net.ಒಳಹರಿವು_gpu = l.output_gpu;
        net.ಒಳಹರಿವು = l.output;
        ಯದಿ(l.truth) {
            net.truth_gpu = l.output_gpu;
            net.truth = l.output;
        }
    }
    pull_network_output(netp);
    calc_network_cost(netp);
}

ಶೂನ್ಯ backward_network_gpu(network *netp)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    network net = *netp;
    network orig = net;
    cuda_set_device(net.gpu_index);
    ಪ್ರತಿ(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        ಯದಿ(l.stopbackward) ಭಙ್ಗ;
        ಯದಿ(i == 0){
            net = orig;
        }ಅನ್ಯಥಾ{
            layer prev = net.layers[i-1];
            net.ಒಳಹರಿವು = prev.output;
            net.delta = prev.delta;
            net.ಒಳಹರಿವು_gpu = prev.output_gpu;
            net.delta_gpu = prev.delta_gpu;
        }
        net.index = i;
        l.backward_gpu(l, net);
    }
}

ಶೂನ್ಯ update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    ಪೂರ್ಣಾಙ್ಕ i;
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

    ಪ್ರತಿ(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        ಯದಿ(l.update_gpu){
            l.update_gpu(l, a);
        }
    }
}

ಶೂನ್ಯ harmless_update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        ಯದಿ(l.weight_updates_gpu) fill_gpu(l.nweights, 0, l.weight_updates_gpu, 1);
        ಯದಿ(l.bias_updates_gpu) fill_gpu(l.nbiases, 0, l.bias_updates_gpu, 1);
        ಯದಿ(l.scale_updates_gpu) fill_gpu(l.nbiases, 0, l.scale_updates_gpu, 1);
    }
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    network *net;
    data d;
    ಭಿನ್ನ *err;
} train_args;

ಶೂನ್ಯ *train_thread(ಶೂನ್ಯ *ptr)
{
    train_args args = *(train_args*)ptr;
    ಮುಕ್ತ(ptr);
    cuda_set_device(args.net->gpu_index);
    *args.err = train_network(args.net, args.d);
    ಪ್ರತ್ಯಾಗಮ 0;
}

pthread_t train_network_in_thread(network *net, data d, ಭಿನ್ನ *err)
{
    pthread_t thread;
    train_args *ptr = (train_args *)calloc(1, ಪರಿಮಾಣ(train_args));
    ptr->net = net;
    ptr->d = d;
    ptr->err = err;
    ಯದಿ(pthread_create(&thread, 0, train_thread, ptr)) error("Thread creation failed");
    ಪ್ರತ್ಯಾಗಮ thread;
}

ಶೂನ್ಯ merge_weights(layer l, layer base)
{
    ಯದಿ (l.type == CONVOLUTIONAL) {
        axpy_cpu(l.n, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.nweights, 1, l.weight_updates, 1, base.weights, 1);
        ಯದಿ (l.scales) {
            axpy_cpu(l.n, 1, l.scale_updates, 1, base.scales, 1);
        }
    } ಅನ್ಯಥಾ ಯದಿ(l.type == CONNECTED) {
        axpy_cpu(l.outputs, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.outputs*l.inputs, 1, l.weight_updates, 1, base.weights, 1);
    }
}

ಶೂನ್ಯ scale_weights(layer l, ಭಿನ್ನ s)
{
    ಯದಿ (l.type == CONVOLUTIONAL) {
        scal_cpu(l.n, s, l.biases, 1);
        scal_cpu(l.nweights, s, l.weights, 1);
        ಯದಿ (l.scales) {
            scal_cpu(l.n, s, l.scales, 1);
        }
    } ಅನ್ಯಥಾ ಯದಿ(l.type == CONNECTED) {
        scal_cpu(l.outputs, s, l.biases, 1);
        scal_cpu(l.outputs*l.inputs, s, l.weights, 1);
    }
}


ಶೂನ್ಯ pull_weights(layer l)
{
    ಯದಿ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.n);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.nweights);
        ಯದಿ(l.scales) cuda_pull_array(l.scales_gpu, l.scale_updates, l.n);
    } ಅನ್ಯಥಾ ಯದಿ(l.type == CONNECTED){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.outputs);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.outputs*l.inputs);
    }
}

ಶೂನ್ಯ ತಳ್ಳು_weights(layer l)
{
    ಯದಿ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_ತಳ್ಳು_array(l.biases_gpu, l.biases, l.n);
        cuda_ತಳ್ಳು_array(l.weights_gpu, l.weights, l.nweights);
        ಯದಿ(l.scales) cuda_ತಳ್ಳು_array(l.scales_gpu, l.scales, l.n);
    } ಅನ್ಯಥಾ ಯದಿ(l.type == CONNECTED){
        cuda_ತಳ್ಳು_array(l.biases_gpu, l.biases, l.outputs);
        cuda_ತಳ್ಳು_array(l.weights_gpu, l.weights, l.outputs*l.inputs);
    }
}

ಶೂನ್ಯ distribute_weights(layer l, layer base)
{
    ಯದಿ (l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL) {
        cuda_ತಳ್ಳು_array(l.biases_gpu, base.biases, l.n);
        cuda_ತಳ್ಳು_array(l.weights_gpu, base.weights, l.nweights);
        ಯದಿ (base.scales) cuda_ತಳ್ಳು_array(l.scales_gpu, base.scales, l.n);
    } ಅನ್ಯಥಾ ಯದಿ (l.type == CONNECTED) {
        cuda_ತಳ್ಳು_array(l.biases_gpu, base.biases, l.outputs);
        cuda_ತಳ್ಳು_array(l.weights_gpu, base.weights, l.outputs*l.inputs);
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

ಶೂನ್ಯ sync_layer(network **nets, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ j)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    network *net = nets[0];
    layer base = net->layers[j];
    scale_weights(base, 0);
    ಪ್ರತಿ (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        pull_weights(l);
        merge_weights(l, base);
    }
    scale_weights(base, 1./n);
    ಪ್ರತಿ (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        distribute_weights(l, base);
    }
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ{
    network **nets;
    ಪೂರ್ಣಾಙ್ಕ n;
    ಪೂರ್ಣಾಙ್ಕ j;
} sync_args;

ಶೂನ್ಯ *sync_layer_thread(ಶೂನ್ಯ *ptr)
{
    sync_args args = *(sync_args*)ptr;
    sync_layer(args.nets, args.n, args.j);
    ಮುಕ್ತ(ptr);
    ಪ್ರತ್ಯಾಗಮ 0;
}

pthread_t sync_layer_in_thread(network **nets, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ j)
{
    pthread_t thread;
    sync_args *ptr = (sync_args *)calloc(1, ಪರಿಮಾಣ(sync_args));
    ptr->nets = nets;
    ptr->n = n;
    ptr->j = j;
    ಯದಿ(pthread_create(&thread, 0, sync_layer_thread, ptr)) error("Thread creation failed");
    ಪ್ರತ್ಯಾಗಮ thread;
}

ಶೂನ್ಯ sync_nets(network **nets, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ interval)
{
    ಪೂರ್ಣಾಙ್ಕ j;
    ಪೂರ್ಣಾಙ್ಕ layers = nets[0]->n;
    pthread_t *threads = (pthread_t *) calloc(layers, ಪರಿಮಾಣ(pthread_t));

    *(nets[0]->seen) += interval * (n-1) * nets[0]->batch * nets[0]->subdivisions;
    ಪ್ರತಿ (j = 0; j < n; ++j){
        *(nets[j]->seen) = *(nets[0]->seen);
    }
    ಪ್ರತಿ (j = 0; j < layers; ++j) {
        threads[j] = sync_layer_in_thread(nets, n, j);
    }
    ಪ್ರತಿ (j = 0; j < layers; ++j) {
        pthread_join(threads[j], 0);
    }
    ಮುಕ್ತ(threads);
}

ಭಿನ್ನ train_networks(network **nets, ಪೂರ್ಣಾಙ್ಕ n, data d, ಪೂರ್ಣಾಙ್ಕ interval)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪೂರ್ಣಾಙ್ಕ batch = nets[0]->batch;
    ಪೂರ್ಣಾಙ್ಕ subdivisions = nets[0]->subdivisions;
    ಪ್ರತಿಜ್ಞಾ(batch * subdivisions * n == d.X.rows);
    pthread_t *threads = (pthread_t *) calloc(n, ಪರಿಮಾಣ(pthread_t));
    ಭಿನ್ನ *errors = (ಭಿನ್ನ *) calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ));

    ಭಿನ್ನ sum = 0;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        data p = get_data_part(d, i, n);
        threads[i] = train_network_in_thread(nets[i], p, errors + i);
    }
    ಪ್ರತಿ(i = 0; i < n; ++i){
        pthread_join(threads[i], 0);
        //printf("%f\n", errors[i]);
        sum += errors[i];
    }
    //cudaDeviceSynchronize();
    ಯದಿ (get_current_batch(nets[0]) % interval == 0) {
        printf("Syncing... ");
        fflush(stdout);
        sync_nets(nets, n, interval);
        printf("Done!\n");
    }
    //cudaDeviceSynchronize();
    ಮುಕ್ತ(threads);
    ಮುಕ್ತ(errors);
    ಪ್ರತ್ಯಾಗಮ (ಭಿನ್ನ)sum/(n);
}

ಶೂನ್ಯ pull_network_output(network *net)
{
    layer l = get_network_output_layer(net);
    cuda_pull_array(l.output_gpu, l.output, l.outputs*l.batch);
}

#endif

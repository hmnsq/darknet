#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "image.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "data.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "crop_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "connected_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gru_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "rnn_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "crnn_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "local_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "convolutional_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "activation_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "detection_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "region_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "yolo_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "normalization_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "batchnorm_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "maxpool_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "reorg_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "avgpool_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cost_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "softmax_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dropout_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "route_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "upsample_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "shortcut_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parser.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "data.h"

ਲੱਦੋ_ਦਲੀਲਾਂ get_ਅਧਾਰ_ਦਲੀਲਾਂ(ਜਾਲ *net)
{
    ਲੱਦੋ_ਦਲੀਲਾਂ ਦਲੀਲਾਂ = {0};
    ਦਲੀਲਾਂ.w = net->w;
    ਦਲੀਲਾਂ.h = net->h;
    ਦਲੀਲਾਂ.ਮਾਪ = net->w;

    ਦਲੀਲਾਂ.min = net->min_crop;
    ਦਲੀਲਾਂ.max = net->max_crop;
    ਦਲੀਲਾਂ.angle = net->angle;
    ਦਲੀਲਾਂ.aspect = net->aspect;
    ਦਲੀਲਾਂ.exposure = net->exposure;
    ਦਲੀਲਾਂ.center = net->center;
    ਦਲੀਲਾਂ.saturation = net->saturation;
    ਦਲੀਲਾਂ.hue = net->hue;
    ਵਾਪਸ ਦਲੀਲਾਂ;
}

ਜਾਲ *ਲੱਦੋ_ਜਾਲ(ਅੱਖਰ *cfg, ਅੱਖਰ *weights, ਪੂਰਨ clear)
{
    ਜਾਲ *net = parse_ਜਾਲ_cfg(cfg);
    ਜੇ(weights && weights[0] != 0){
        ਲੱਦੋ_weights(net, weights);
    }
    ਜੇ(clear) (*net->ਵੇਖਿਆ) = 0;
    ਵਾਪਸ net;
}

size_t get_current_batch(ਜਾਲ *net)
{
    size_t batch_ਅੰਕ = (*net->ਵੇਖਿਆ)/(net->batch*net->subdivisions);
    ਵਾਪਸ batch_ਅੰਕ;
}

ਖਾਲੀ reset_ਜਾਲ_state(ਜਾਲ *net, ਪੂਰਨ b)
{
    ਪੂਰਨ i;
    ਲਈ (i = 0; i < net->n; ++i) {
        #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
        layer l = net->layers[i];
        ਜੇ(l.state_gpu){
            fill_gpu(l.outputs, 0, l.state_gpu + l.outputs*b, 1);
        }
        ਜੇ(l.h_gpu){
            fill_gpu(l.outputs, 0, l.h_gpu + l.outputs*b, 1);
        }
        #ਜੇ_ਅੰਤ
    }
}

ਖਾਲੀ reset_rnn(ਜਾਲ *net)
{
    reset_ਜਾਲ_state(net, 0);
}

ਦਸ਼ਮਲਵ get_current_ਦਰ(ਜਾਲ *net)
{
    size_t batch_ਅੰਕ = get_current_batch(net);
    ਪੂਰਨ i;
    ਦਸ਼ਮਲਵ ਦਰ;
    ਜੇ (batch_ਅੰਕ < net->burn_ਵਿੱਚ) ਵਾਪਸ net->learning_ਦਰ * ਘਾਤ((ਦਸ਼ਮਲਵ)batch_ਅੰਕ / net->burn_ਵਿੱਚ, net->power);
    ਚੋਣ (net->policy) {
        ਹਾਲਤ CONSTANT:
            ਵਾਪਸ net->learning_ਦਰ;
        ਹਾਲਤ ਕਦਮ_2:
            ਵਾਪਸ net->learning_ਦਰ * ਘਾਤ(net->scale, batch_ਅੰਕ/net->ਕਦਮ);
        ਹਾਲਤ ਕਦਮਾਂ_2:
            ਦਰ = net->learning_ਦਰ;
            ਲਈ(i = 0; i < net->ਅੰਕ_ਕਦਮਾਂ; ++i){
                ਜੇ(net->ਕਦਮਾਂ[i] > batch_ਅੰਕ) ਵਾਪਸ ਦਰ;
                ਦਰ *= net->scales[i];
            }
            ਵਾਪਸ ਦਰ;
        ਹਾਲਤ EXP:
            ਵਾਪਸ net->learning_ਦਰ * ਘਾਤ(net->gamma, batch_ਅੰਕ);
        ਹਾਲਤ POLY:
            ਵਾਪਸ net->learning_ਦਰ * ਘਾਤ(1 - (ਦਸ਼ਮਲਵ)batch_ਅੰਕ / net->max_batches, net->power);
        ਹਾਲਤ RANDOM:
            ਵਾਪਸ net->learning_ਦਰ * ਘਾਤ(rand_ਇਕਸਾਰ(0,1), net->power);
        ਹਾਲਤ SIG:
            ਵਾਪਸ net->learning_ਦਰ * (1./(1.+ਘਾਤਾਂਕ(net->gamma*(batch_ਅੰਕ - net->ਕਦਮ))));
        ਮੂਲ:
            fprintf(stderr, "Policy is weird!\n");
            ਵਾਪਸ net->learning_ਦਰ;
    }
}

ਅੱਖਰ *get_layer_string(LAYER_TYPE a)
{
    ਚੋਣ(a){
        ਹਾਲਤ CONVOLUTIONAL:
            ਵਾਪਸ "convolutional";
        ਹਾਲਤ ਸਰਗਰਮ:
            ਵਾਪਸ "activation";
        ਹਾਲਤ LOCAL:
            ਵਾਪਸ "local";
        ਹਾਲਤ DECONVOLUTIONAL:
            ਵਾਪਸ "deconvolutional";
        ਹਾਲਤ CONNECTED:
            ਵਾਪਸ "connected";
        ਹਾਲਤ RNN:
            ਵਾਪਸ "rnn";
        ਹਾਲਤ GRU:
            ਵਾਪਸ "gru";
        ਹਾਲਤ LSTM:
	    ਵਾਪਸ "lstm";
        ਹਾਲਤ CRNN:
            ਵਾਪਸ "crnn";
        ਹਾਲਤ MAXPOOL:
            ਵਾਪਸ "maxpool";
        ਹਾਲਤ REORG:
            ਵਾਪਸ "reorg";
        ਹਾਲਤ AVGPOOL:
            ਵਾਪਸ "avgpool";
        ਹਾਲਤ SOFTMAX:
            ਵਾਪਸ "softmax";
        ਹਾਲਤ DETECTION:
            ਵਾਪਸ "detection";
        ਹਾਲਤ REGION:
            ਵਾਪਸ "region";
        ਹਾਲਤ YOLO:
            ਵਾਪਸ "yolo";
        ਹਾਲਤ DROPOUT:
            ਵਾਪਸ "dropout";
        ਹਾਲਤ CROP:
            ਵਾਪਸ "crop";
        ਹਾਲਤ ਲਾਗਤ:
            ਵਾਪਸ "cost";
        ਹਾਲਤ ROUTE:
            ਵਾਪਸ "route";
        ਹਾਲਤ SHORTCUT:
            ਵਾਪਸ "shortcut";
        ਹਾਲਤ NORMALIZATION:
            ਵਾਪਸ "normalization";
        ਹਾਲਤ BATCHNORM:
            ਵਾਪਸ "batchnorm";
        ਮੂਲ:
            ਰੋਕੋ;
    }
    ਵਾਪਸ "none";
}

ਜਾਲ *make_ਜਾਲ(ਪੂਰਨ n)
{
    ਜਾਲ *net = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(ਜਾਲ));
    net->n = n;
    net->layers = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(net->n, ਆਕਾਰ(layer));
    net->ਵੇਖਿਆ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(size_t));
    net->t    = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(ਪੂਰਨ));
    net->ਲਾਗਤ_2 = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਵਾਪਸ net;
}

ਖਾਲੀ forward_ਜਾਲ(ਜਾਲ *netp)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    ਜੇ(netp->gpu_ਸੂਚਕਾਂਕ >= 0){
        forward_ਜਾਲ_gpu(netp);   
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ
    ਜਾਲ net = *netp;
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < net.n; ++i){
        net.ਸੂਚਕਾਂਕ = i;
        layer l = net.layers[i];
        ਜੇ(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.ਨਿਵੇਸ਼ = l.output;
        ਜੇ(l.truth) {
            net.truth = l.output;
        }
    }
    calc_ਜਾਲ_ਲਾਗਤ(netp);
}

ਖਾਲੀ update_ਜਾਲ(ਜਾਲ *netp)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    ਜੇ(netp->gpu_ਸੂਚਕਾਂਕ >= 0){
        update_ਜਾਲ_gpu(netp);   
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ
    ਜਾਲ net = *netp;
    ਪੂਰਨ i;
    update_ਦਲੀਲਾਂ a = {0};
    a.batch = net.batch*net.subdivisions;
    a.learning_ਦਰ = get_current_ਦਰ(netp);
    a.momentum = net.momentum;
    a.decay = net.decay;
    a.adam = net.adam;
    a.B1 = net.B1;
    a.B2 = net.B2;
    a.eps = net.eps;
    ++*net.t;
    a.t = *net.t;

    ਲਈ(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        ਜੇ(l.update){
            l.update(l, a);
        }
    }
}

ਖਾਲੀ calc_ਜਾਲ_ਲਾਗਤ(ਜਾਲ *netp)
{
    ਜਾਲ net = *netp;
    ਪੂਰਨ i;
    ਦਸ਼ਮਲਵ ਜੋੜ = 0;
    ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    ਲਈ(i = 0; i < net.n; ++i){
        ਜੇ(net.layers[i].ਲਾਗਤ_2){
            ਜੋੜ += net.layers[i].ਲਾਗਤ_2[0];
            ++ਗਿਣਤੀ_2;
        }
    }
    *net.ਲਾਗਤ_2 = ਜੋੜ/ਗਿਣਤੀ_2;
}

ਪੂਰਨ get_predicted_class_ਜਾਲ(ਜਾਲ *net)
{
    ਵਾਪਸ max_ਸੂਚਕਾਂਕ(net->output, net->outputs);
}

ਖਾਲੀ backward_ਜਾਲ(ਜਾਲ *netp)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    ਜੇ(netp->gpu_ਸੂਚਕਾਂਕ >= 0){
        backward_ਜਾਲ_gpu(netp);   
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ
    ਜਾਲ net = *netp;
    ਪੂਰਨ i;
    ਜਾਲ orig = net;
    ਲਈ(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        ਜੇ(l.stopbackward) ਰੋਕੋ;
        ਜੇ(i == 0){
            net = orig;
        }ਨਹੀਂਤਾਂ{
            layer prev = net.layers[i-1];
            net.ਨਿਵੇਸ਼ = prev.output;
            net.delta = prev.delta;
        }
        net.ਸੂਚਕਾਂਕ = i;
        l.backward(l, net);
    }
}

ਦਸ਼ਮਲਵ train_ਜਾਲ_datum(ਜਾਲ *net)
{
    *net->ਵੇਖਿਆ += net->batch;
    net->train = 1;
    forward_ਜਾਲ(net);
    backward_ਜਾਲ(net);
    ਦਸ਼ਮਲਵ error = *net->ਲਾਗਤ_2;
    ਜੇ(((*net->ਵੇਖਿਆ)/net->batch)%net->subdivisions == 0) update_ਜਾਲ(net);
    ਵਾਪਸ error;
}

ਦਸ਼ਮਲਵ train_ਜਾਲ_sgd(ਜਾਲ *net, ਅੰਕੜੇ d, ਪੂਰਨ n)
{
    ਪੂਰਨ batch = net->batch;

    ਪੂਰਨ i;
    ਦਸ਼ਮਲਵ ਜੋੜ = 0;
    ਲਈ(i = 0; i < n; ++i){
        get_random_batch(d, batch, net->ਨਿਵੇਸ਼, net->truth);
        ਦਸ਼ਮਲਵ err = train_ਜਾਲ_datum(net);
        ਜੋੜ += err;
    }
    ਵਾਪਸ (ਦਸ਼ਮਲਵ)ਜੋੜ/(n*batch);
}

ਦਸ਼ਮਲਵ train_ਜਾਲ(ਜਾਲ *net, ਅੰਕੜੇ d)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(d.X.ਪੰਕਤੀਆਂ % net->batch == 0);
    ਪੂਰਨ batch = net->batch;
    ਪੂਰਨ n = d.X.ਪੰਕਤੀਆਂ / batch;

    ਪੂਰਨ i;
    ਦਸ਼ਮਲਵ ਜੋੜ = 0;
    ਲਈ(i = 0; i < n; ++i){
        get_ਅਗਲਾ_batch(d, batch, i*batch, net->ਨਿਵੇਸ਼, net->truth);
        ਦਸ਼ਮਲਵ err = train_ਜਾਲ_datum(net);
        ਜੋੜ += err;
    }
    ਵਾਪਸ (ਦਸ਼ਮਲਵ)ਜੋੜ/(n*batch);
}

ਖਾਲੀ ਸੈੱਟ_ਆਰਜ਼ੀ_ਜਾਲ(ਜਾਲ *net, ਦਸ਼ਮਲਵ t)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < net->n; ++i){
        net->layers[i].temperature = t;
    }
}


ਖਾਲੀ ਸੈੱਟ_batch_ਜਾਲ(ਜਾਲ *net, ਪੂਰਨ b)
{
    net->batch = b;
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CUDNN
        ਜੇ(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_ਤਿਆਰੀ(net->layers + i);
        }
        ਜੇ(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, 1, l->ਬਾਹਰਲਾ_c, l->ਬਾਹਰਲਾ_h, l->ਬਾਹਰਲਾ_w);
            cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, 1, l->ਬਾਹਰਲਾ_c, 1, 1); 
        }
#ਜੇ_ਅੰਤ
    }
}

ਪੂਰਨ resize_ਜਾਲ(ਜਾਲ *net, ਪੂਰਨ w, ਪੂਰਨ h)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    cuda_ਸੈੱਟ_device(net->gpu_ਸੂਚਕਾਂਕ);
    cuda_free(net->workspace);
#ਜੇ_ਅੰਤ
    ਪੂਰਨ i;
    //if(w == net->w && h == net->h) return 0;
    net->w = w;
    net->h = h;
    ਪੂਰਨ inputs = 0;
    size_t workspace_ਮਾਪ = 0;
    //fprintf(stderr, "Resizing to %d x %d...\n", w, h);
    //fflush(stderr);
    ਲਈ (i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ਜੇ(l.type == CONVOLUTIONAL){
            resize_convolutional_layer(&l, w, h);
        }ਨਹੀਂਤਾਂ ਜੇ(l.type == CROP){
            resize_crop_layer(&l, w, h);
        }ਨਹੀਂਤਾਂ ਜੇ(l.type == MAXPOOL){
            resize_maxpool_layer(&l, w, h);
        }ਨਹੀਂਤਾਂ ਜੇ(l.type == REGION){
            resize_region_layer(&l, w, h);
        }ਨਹੀਂਤਾਂ ਜੇ(l.type == YOLO){
            resize_yolo_layer(&l, w, h);
        }ਨਹੀਂਤਾਂ ਜੇ(l.type == ROUTE){
            resize_route_layer(&l, net);
        }ਨਹੀਂਤਾਂ ਜੇ(l.type == SHORTCUT){
            resize_shortcut_layer(&l, w, h);
        }ਨਹੀਂਤਾਂ ਜੇ(l.type == UPSAMPLE){
            resize_upsample_layer(&l, w, h);
        }ਨਹੀਂਤਾਂ ਜੇ(l.type == REORG){
            resize_reorg_layer(&l, w, h);
        }ਨਹੀਂਤਾਂ ਜੇ(l.type == AVGPOOL){
            resize_avgpool_layer(&l, w, h);
        }ਨਹੀਂਤਾਂ ਜੇ(l.type == NORMALIZATION){
            resize_normalization_layer(&l, w, h);
        }ਨਹੀਂਤਾਂ ਜੇ(l.type == ਲਾਗਤ){
            resize_ਲਾਗਤ_layer(&l, inputs);
        }ਨਹੀਂਤਾਂ{
            error("Cannot resize this type of layer");
        }
        ਜੇ(l.workspace_ਮਾਪ > workspace_ਮਾਪ) workspace_ਮਾਪ = l.workspace_ਮਾਪ;
        ਜੇ(l.workspace_ਮਾਪ > 2000000000) ਪ੍ਰਤਿਜ੍ਞਾ(0);
        inputs = l.outputs;
        net->layers[i] = l;
        w = l.ਬਾਹਰਲਾ_w;
        h = l.ਬਾਹਰਲਾ_h;
        ਜੇ(l.type == AVGPOOL) ਰੋਕੋ;
    }
    layer ਬਾਹਰਲਾ = get_ਜਾਲ_output_layer(net);
    net->inputs = net->layers[0].inputs;
    net->outputs = ਬਾਹਰਲਾ.outputs;
    net->truths = ਬਾਹਰਲਾ.outputs;
    ਜੇ(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = ਬਾਹਰਲਾ.output;
    ਮੁਕਤ(net->ਨਿਵੇਸ਼);
    ਮੁਕਤ(net->truth);
    net->ਨਿਵੇਸ਼ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(net->inputs*net->batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    net->truth = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(net->truths*net->batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    ਜੇ(gpu_ਸੂਚਕਾਂਕ >= 0){
        cuda_free(net->ਨਿਵੇਸ਼_gpu);
        cuda_free(net->truth_gpu);
        net->ਨਿਵੇਸ਼_gpu = cuda_make_array(net->ਨਿਵੇਸ਼, net->inputs*net->batch);
        net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
        ਜੇ(workspace_ਮਾਪ){
            net->workspace = cuda_make_array(0, (workspace_ਮਾਪ-1)/ਆਕਾਰ(ਦਸ਼ਮਲਵ)+1);
        }
    }ਨਹੀਂਤਾਂ {
        ਮੁਕਤ(net->workspace);
        net->workspace = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, workspace_ਮਾਪ);
    }
#ਨਹੀਂਤਾਂ
    ਮੁਕਤ(net->workspace);
    net->workspace = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, workspace_ਮਾਪ);
#ਜੇ_ਅੰਤ
    //fprintf(stderr, " Done!\n");
    ਵਾਪਸ 0;
}

layer get_ਜਾਲ_detection_layer(ਜਾਲ *net)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < net->n; ++i){
        ਜੇ(net->layers[i].type == DETECTION){
            ਵਾਪਸ net->layers[i];
        }
    }
    fprintf(stderr, "Detection layer not found!!\n");
    layer l = {0};
    ਵਾਪਸ l;
}

image get_ਜਾਲ_image_layer(ਜਾਲ *net, ਪੂਰਨ i)
{
    layer l = net->layers[i];
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    //cuda_pull_array(l.output_gpu, l.output, l.outputs);
#ਜੇ_ਅੰਤ
    ਜੇ (l.ਬਾਹਰਲਾ_w && l.ਬਾਹਰਲਾ_h && l.ਬਾਹਰਲਾ_c){
        ਵਾਪਸ float_ਮੰਜ਼ਿਲ_image(l.ਬਾਹਰਲਾ_w, l.ਬਾਹਰਲਾ_h, l.ਬਾਹਰਲਾ_c, l.output);
    }
    image def = {0};
    ਵਾਪਸ def;
}

image get_ਜਾਲ_image(ਜਾਲ *net)
{
    ਪੂਰਨ i;
    ਲਈ(i = net->n-1; i >= 0; --i){
        image m = get_ਜਾਲ_image_layer(net, i);
        ਜੇ(m.h != 0) ਵਾਪਸ m;
    }
    image def = {0};
    ਵਾਪਸ def;
}

ਖਾਲੀ visualize_ਜਾਲ(ਜਾਲ *net)
{
    image *prev = 0;
    ਪੂਰਨ i;
    ਅੱਖਰ buff[256];
    ਲਈ(i = 0; i < net->n; ++i){
        sprintf(buff, "Layer %d", i);
        layer l = net->layers[i];
        ਜੇ(l.type == CONVOLUTIONAL){
            prev = visualize_convolutional_layer(l, buff, prev);
        }
    } 
}

ਖਾਲੀ ਸਿਖਰ_predictions(ਜਾਲ *net, ਪੂਰਨ k, ਪੂਰਨ *ਸੂਚਕਾਂਕ)
{
    ਸਿਖਰ_k(net->output, net->outputs, k, ਸੂਚਕਾਂਕ);
}


ਦਸ਼ਮਲਵ *ਜਾਲ_predict(ਜਾਲ *net, ਦਸ਼ਮਲਵ *ਨਿਵੇਸ਼)
{
    ਜਾਲ orig = *net;
    net->ਨਿਵੇਸ਼ = ਨਿਵੇਸ਼;
    net->truth = 0;
    net->train = 0;
    net->delta = 0;
    forward_ਜਾਲ(net);
    ਦਸ਼ਮਲਵ *ਬਾਹਰਲਾ = net->output;
    *net = orig;
    ਵਾਪਸ ਬਾਹਰਲਾ;
}

ਪੂਰਨ ਅੰਕ_detections(ਜਾਲ *net, ਦਸ਼ਮਲਵ thresh)
{
    ਪੂਰਨ i;
    ਪੂਰਨ s = 0;
    ਲਈ(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ਜੇ(l.type == YOLO){
            s += yolo_ਅੰਕ_detections(l, thresh);
        }
        ਜੇ(l.type == DETECTION || l.type == REGION){
            s += l.w*l.h*l.n;
        }
    }
    ਵਾਪਸ s;
}

detection *make_ਜਾਲ_boxes(ਜਾਲ *net, ਦਸ਼ਮਲਵ thresh, ਪੂਰਨ *ਅੰਕ)
{
    layer l = net->layers[net->n - 1];
    ਪੂਰਨ i;
    ਪੂਰਨ nboxes = ਅੰਕ_detections(net, thresh);
    ਜੇ(ਅੰਕ) *ਅੰਕ = nboxes;
    detection *dets = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(nboxes, ਆਕਾਰ(detection));
    ਲਈ(i = 0; i < nboxes; ++i){
        dets[i].prob = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.classes, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        ਜੇ(l.coords > 4){
            dets[i].ਨਕਾਬ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.coords-4, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        }
    }
    ਵਾਪਸ dets;
}

ਖਾਲੀ fill_ਜਾਲ_boxes(ਜਾਲ *net, ਪੂਰਨ w, ਪੂਰਨ h, ਦਸ਼ਮਲਵ thresh, ਦਸ਼ਮਲਵ hier, ਪੂਰਨ *map, ਪੂਰਨ relative, detection *dets)
{
    ਪੂਰਨ j;
    ਲਈ(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        ਜੇ(l.type == YOLO){
            ਪੂਰਨ ਗਿਣਤੀ_2 = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
            dets += ਗਿਣਤੀ_2;
        }
        ਜੇ(l.type == REGION){
            get_region_detections(l, w, h, net->w, net->h, thresh, map, hier, relative, dets);
            dets += l.w*l.h*l.n;
        }
        ਜੇ(l.type == DETECTION){
            get_detection_detections(l, w, h, thresh, dets);
            dets += l.w*l.h*l.n;
        }
    }
}

detection *get_ਜਾਲ_boxes(ਜਾਲ *net, ਪੂਰਨ w, ਪੂਰਨ h, ਦਸ਼ਮਲਵ thresh, ਦਸ਼ਮਲਵ hier, ਪੂਰਨ *map, ਪੂਰਨ relative, ਪੂਰਨ *ਅੰਕ)
{
    detection *dets = make_ਜਾਲ_boxes(net, thresh, ਅੰਕ);
    fill_ਜਾਲ_boxes(net, w, h, thresh, hier, map, relative, dets);
    ਵਾਪਸ dets;
}

ਖਾਲੀ free_detections(detection *dets, ਪੂਰਨ n)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < n; ++i){
        ਮੁਕਤ(dets[i].prob);
        ਜੇ(dets[i].ਨਕਾਬ) ਮੁਕਤ(dets[i].ਨਕਾਬ);
    }
    ਮੁਕਤ(dets);
}

ਦਸ਼ਮਲਵ *ਜਾਲ_predict_image(ਜਾਲ *net, image im)
{
    image imr = letterbox_image(im, net->w, net->h);
    ਸੈੱਟ_batch_ਜਾਲ(net, 1);
    ਦਸ਼ਮਲਵ *p = ਜਾਲ_predict(net, imr.ਅੰਕੜੇ);
    free_image(imr);
    ਵਾਪਸ p;
}

ਪੂਰਨ ਜਾਲ_ਚੌੜਾਈ(ਜਾਲ *net){ਵਾਪਸ net->w;}
ਪੂਰਨ ਜਾਲ_ਉਚਾਈ(ਜਾਲ *net){ਵਾਪਸ net->h;}

ਮੈਟ੍ਰਿਕਸ ਜਾਲ_predict_ਅੰਕੜੇ_multi(ਜਾਲ *net, ਅੰਕੜੇ test, ਪੂਰਨ n)
{
    ਪੂਰਨ i,j,b,m;
    ਪੂਰਨ k = net->outputs;
    ਮੈਟ੍ਰਿਕਸ pred = make_ਮੈਟ੍ਰਿਕਸ(test.X.ਪੰਕਤੀਆਂ, k);
    ਦਸ਼ਮਲਵ *X = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(net->batch*test.X.ਪੰਕਤੀਆਂ, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਲਈ(i = 0; i < test.X.ਪੰਕਤੀਆਂ; i += net->batch){
        ਲਈ(b = 0; b < net->batch; ++b){
            ਜੇ(i+b == test.X.ਪੰਕਤੀਆਂ) ਰੋਕੋ;
            ਸਮ੍ਰਿਤੀ_ਨਕਲ(X+b*test.X.ਖਾਨੇ_ਗਿਣਤੀ, test.X.vals[i+b], test.X.ਖਾਨੇ_ਗਿਣਤੀ*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        }
        ਲਈ(m = 0; m < n; ++m){
            ਦਸ਼ਮਲਵ *ਬਾਹਰਲਾ = ਜਾਲ_predict(net, X);
            ਲਈ(b = 0; b < net->batch; ++b){
                ਜੇ(i+b == test.X.ਪੰਕਤੀਆਂ) ਰੋਕੋ;
                ਲਈ(j = 0; j < k; ++j){
                    pred.vals[i+b][j] += ਬਾਹਰਲਾ[j+b*k]/n;
                }
            }
        }
    }
    ਮੁਕਤ(X);
    ਵਾਪਸ pred;   
}

ਮੈਟ੍ਰਿਕਸ ਜਾਲ_predict_ਅੰਕੜੇ(ਜਾਲ *net, ਅੰਕੜੇ test)
{
    ਪੂਰਨ i,j,b;
    ਪੂਰਨ k = net->outputs;
    ਮੈਟ੍ਰਿਕਸ pred = make_ਮੈਟ੍ਰਿਕਸ(test.X.ਪੰਕਤੀਆਂ, k);
    ਦਸ਼ਮਲਵ *X = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(net->batch*test.X.ਖਾਨੇ_ਗਿਣਤੀ, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਲਈ(i = 0; i < test.X.ਪੰਕਤੀਆਂ; i += net->batch){
        ਲਈ(b = 0; b < net->batch; ++b){
            ਜੇ(i+b == test.X.ਪੰਕਤੀਆਂ) ਰੋਕੋ;
            ਸਮ੍ਰਿਤੀ_ਨਕਲ(X+b*test.X.ਖਾਨੇ_ਗਿਣਤੀ, test.X.vals[i+b], test.X.ਖਾਨੇ_ਗਿਣਤੀ*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        }
        ਦਸ਼ਮਲਵ *ਬਾਹਰਲਾ = ਜਾਲ_predict(net, X);
        ਲਈ(b = 0; b < net->batch; ++b){
            ਜੇ(i+b == test.X.ਪੰਕਤੀਆਂ) ਰੋਕੋ;
            ਲਈ(j = 0; j < k; ++j){
                pred.vals[i+b][j] = ਬਾਹਰਲਾ[j+b*k];
            }
        }
    }
    ਮੁਕਤ(X);
    ਵਾਪਸ pred;   
}

ਖਾਲੀ print_ਜਾਲ(ਜਾਲ *net)
{
    ਪੂਰਨ i,j;
    ਲਈ(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ਦਸ਼ਮਲਵ *output = l.output;
        ਪੂਰਨ n = l.outputs;
        ਦਸ਼ਮਲਵ ਔਸਤ = ਔਸਤ_array(output, n);
        ਦਸ਼ਮਲਵ vari = variance_array(output, n);
        fprintf(stderr, "Layer %d - Mean: %f, Variance: %f\n",i,ਔਸਤ, vari);
        ਜੇ(n > 100) n = 100;
        ਲਈ(j = 0; j < n; ++j) fprintf(stderr, "%f, ", output[j]);
        ਜੇ(n == 100)fprintf(stderr,".....\n");
        fprintf(stderr, "\n");
    }
}

ਖਾਲੀ compare_networks(ਜਾਲ *n1, ਜਾਲ *n2, ਅੰਕੜੇ test)
{
    ਮੈਟ੍ਰਿਕਸ g1 = ਜਾਲ_predict_ਅੰਕੜੇ(n1, test);
    ਮੈਟ੍ਰਿਕਸ g2 = ਜਾਲ_predict_ਅੰਕੜੇ(n2, test);
    ਪੂਰਨ i;
    ਪੂਰਨ a,b,c,d;
    a = b = c = d = 0;
    ਲਈ(i = 0; i < g1.ਪੰਕਤੀਆਂ; ++i){
        ਪੂਰਨ truth = max_ਸੂਚਕਾਂਕ(test.y.vals[i], test.y.ਖਾਨੇ_ਗਿਣਤੀ);
        ਪੂਰਨ p1 = max_ਸੂਚਕਾਂਕ(g1.vals[i], g1.ਖਾਨੇ_ਗਿਣਤੀ);
        ਪੂਰਨ p2 = max_ਸੂਚਕਾਂਕ(g2.vals[i], g2.ਖਾਨੇ_ਗਿਣਤੀ);
        ਜੇ(p1 == truth){
            ਜੇ(p2 == truth) ++d;
            ਨਹੀਂਤਾਂ ++c;
        }ਨਹੀਂਤਾਂ{
            ਜੇ(p2 == truth) ++b;
            ਨਹੀਂਤਾਂ ++a;
        }
    }
    ਲਿਖੋ("%5d %5d\n%5d %5d\n", a, b, c, d);
    ਦਸ਼ਮਲਵ ਅੰਕ = ਘਾਤ((ਨਿਰਪੇਖ(b - c) - 1.), 2.);
    ਦਸ਼ਮਲਵ den = b + c;
    ਲਿਖੋ("%f\n", ਅੰਕ/den); 
}

ਦਸ਼ਮਲਵ ਜਾਲ_accuracy(ਜਾਲ *net, ਅੰਕੜੇ d)
{
    ਮੈਟ੍ਰਿਕਸ guess = ਜਾਲ_predict_ਅੰਕੜੇ(net, d);
    ਦਸ਼ਮਲਵ acc = ਮੈਟ੍ਰਿਕਸ_topk_accuracy(d.y, guess,1);
    free_ਮੈਟ੍ਰਿਕਸ(guess);
    ਵਾਪਸ acc;
}

ਦਸ਼ਮਲਵ *ਜਾਲ_accuracies(ਜਾਲ *net, ਅੰਕੜੇ d, ਪੂਰਨ n)
{
    ਸਥਿਰ ਦਸ਼ਮਲਵ acc[2];
    ਮੈਟ੍ਰਿਕਸ guess = ਜਾਲ_predict_ਅੰਕੜੇ(net, d);
    acc[0] = ਮੈਟ੍ਰਿਕਸ_topk_accuracy(d.y, guess, 1);
    acc[1] = ਮੈਟ੍ਰਿਕਸ_topk_accuracy(d.y, guess, n);
    free_ਮੈਟ੍ਰਿਕਸ(guess);
    ਵਾਪਸ acc;
}

layer get_ਜਾਲ_output_layer(ਜਾਲ *net)
{
    ਪੂਰਨ i;
    ਲਈ(i = net->n - 1; i >= 0; --i){
        ਜੇ(net->layers[i].type != ਲਾਗਤ) ਰੋਕੋ;
    }
    ਵਾਪਸ net->layers[i];
}

ਦਸ਼ਮਲਵ ਜਾਲ_accuracy_multi(ਜਾਲ *net, ਅੰਕੜੇ d, ਪੂਰਨ n)
{
    ਮੈਟ੍ਰਿਕਸ guess = ਜਾਲ_predict_ਅੰਕੜੇ_multi(net, d, n);
    ਦਸ਼ਮਲਵ acc = ਮੈਟ੍ਰਿਕਸ_topk_accuracy(d.y, guess,1);
    free_ਮੈਟ੍ਰਿਕਸ(guess);
    ਵਾਪਸ acc;
}

ਖਾਲੀ free_ਜਾਲ(ਜਾਲ *net)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < net->n; ++i){
        free_layer(net->layers[i]);
    }
    ਮੁਕਤ(net->layers);
    ਜੇ(net->ਨਿਵੇਸ਼) ਮੁਕਤ(net->ਨਿਵੇਸ਼);
    ਜੇ(net->truth) ਮੁਕਤ(net->truth);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    ਜੇ(net->ਨਿਵੇਸ਼_gpu) cuda_free(net->ਨਿਵੇਸ਼_gpu);
    ਜੇ(net->truth_gpu) cuda_free(net->truth_gpu);
#ਜੇ_ਅੰਤ
    ਮੁਕਤ(net);
}

// Some day...
// ^ What the hell is this comment for?


layer ਜਾਲ_output_layer(ਜਾਲ *net)
{
    ਪੂਰਨ i;
    ਲਈ(i = net->n - 1; i >= 0; --i){
        ਜੇ(net->layers[i].type != ਲਾਗਤ) ਰੋਕੋ;
    }
    ਵਾਪਸ net->layers[i];
}

ਪੂਰਨ ਜਾਲ_inputs(ਜਾਲ *net)
{
    ਵਾਪਸ net->layers[0].inputs;
}

ਪੂਰਨ ਜਾਲ_outputs(ਜਾਲ *net)
{
    ਵਾਪਸ ਜਾਲ_output_layer(net).outputs;
}

ਦਸ਼ਮਲਵ *ਜਾਲ_output(ਜਾਲ *net)
{
    ਵਾਪਸ ਜਾਲ_output_layer(net).output;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU

ਖਾਲੀ forward_ਜਾਲ_gpu(ਜਾਲ *netp)
{
    ਜਾਲ net = *netp;
    cuda_ਸੈੱਟ_device(net.gpu_ਸੂਚਕਾਂਕ);
    cuda_ਧੱਕੋ_array(net.ਨਿਵੇਸ਼_gpu, net.ਨਿਵੇਸ਼, net.inputs*net.batch);
    ਜੇ(net.truth){
        cuda_ਧੱਕੋ_array(net.truth_gpu, net.truth, net.truths*net.batch);
    }

    ਪੂਰਨ i;
    ਲਈ(i = 0; i < net.n; ++i){
        net.ਸੂਚਕਾਂਕ = i;
        layer l = net.layers[i];
        ਜੇ(l.delta_gpu){
            fill_gpu(l.outputs * l.batch, 0, l.delta_gpu, 1);
        }
        l.forward_gpu(l, net);
        net.ਨਿਵੇਸ਼_gpu = l.output_gpu;
        net.ਨਿਵੇਸ਼ = l.output;
        ਜੇ(l.truth) {
            net.truth_gpu = l.output_gpu;
            net.truth = l.output;
        }
    }
    pull_ਜਾਲ_output(netp);
    calc_ਜਾਲ_ਲਾਗਤ(netp);
}

ਖਾਲੀ backward_ਜਾਲ_gpu(ਜਾਲ *netp)
{
    ਪੂਰਨ i;
    ਜਾਲ net = *netp;
    ਜਾਲ orig = net;
    cuda_ਸੈੱਟ_device(net.gpu_ਸੂਚਕਾਂਕ);
    ਲਈ(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        ਜੇ(l.stopbackward) ਰੋਕੋ;
        ਜੇ(i == 0){
            net = orig;
        }ਨਹੀਂਤਾਂ{
            layer prev = net.layers[i-1];
            net.ਨਿਵੇਸ਼ = prev.output;
            net.delta = prev.delta;
            net.ਨਿਵੇਸ਼_gpu = prev.output_gpu;
            net.delta_gpu = prev.delta_gpu;
        }
        net.ਸੂਚਕਾਂਕ = i;
        l.backward_gpu(l, net);
    }
}

ਖਾਲੀ update_ਜਾਲ_gpu(ਜਾਲ *netp)
{
    ਜਾਲ net = *netp;
    cuda_ਸੈੱਟ_device(net.gpu_ਸੂਚਕਾਂਕ);
    ਪੂਰਨ i;
    update_ਦਲੀਲਾਂ a = {0};
    a.batch = net.batch*net.subdivisions;
    a.learning_ਦਰ = get_current_ਦਰ(netp);
    a.momentum = net.momentum;
    a.decay = net.decay;
    a.adam = net.adam;
    a.B1 = net.B1;
    a.B2 = net.B2;
    a.eps = net.eps;
    ++*net.t;
    a.t = (*net.t);

    ਲਈ(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        ਜੇ(l.update_gpu){
            l.update_gpu(l, a);
        }
    }
}

ਖਾਲੀ harmless_update_ਜਾਲ_gpu(ਜਾਲ *netp)
{
    ਜਾਲ net = *netp;
    cuda_ਸੈੱਟ_device(net.gpu_ਸੂਚਕਾਂਕ);
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        ਜੇ(l.ਭਾਰ_updates_gpu) fill_gpu(l.nweights, 0, l.ਭਾਰ_updates_gpu, 1);
        ਜੇ(l.bias_updates_gpu) fill_gpu(l.nbiases, 0, l.bias_updates_gpu, 1);
        ਜੇ(l.scale_updates_gpu) fill_gpu(l.nbiases, 0, l.scale_updates_gpu, 1);
    }
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਜਾਲ *net;
    ਅੰਕੜੇ d;
    ਦਸ਼ਮਲਵ *err;
} train_ਦਲੀਲਾਂ;

ਖਾਲੀ *train_thread(ਖਾਲੀ *ptr)
{
    train_ਦਲੀਲਾਂ ਦਲੀਲਾਂ = *(train_ਦਲੀਲਾਂ*)ptr;
    ਮੁਕਤ(ptr);
    cuda_ਸੈੱਟ_device(ਦਲੀਲਾਂ.net->gpu_ਸੂਚਕਾਂਕ);
    *ਦਲੀਲਾਂ.err = train_ਜਾਲ(ਦਲੀਲਾਂ.net, ਦਲੀਲਾਂ.d);
    ਵਾਪਸ 0;
}

pthread_t train_ਜਾਲ_ਵਿੱਚ_thread(ਜਾਲ *net, ਅੰਕੜੇ d, ਦਸ਼ਮਲਵ *err)
{
    pthread_t thread;
    train_ਦਲੀਲਾਂ *ptr = (train_ਦਲੀਲਾਂ *)ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(train_ਦਲੀਲਾਂ));
    ptr->net = net;
    ptr->d = d;
    ptr->err = err;
    ਜੇ(pthread_create(&thread, 0, train_thread, ptr)) error("Thread creation failed");
    ਵਾਪਸ thread;
}

ਖਾਲੀ ਮਿਲਾਓ_weights(layer l, layer ਅਧਾਰ)
{
    ਜੇ (l.type == CONVOLUTIONAL) {
        axpy_cpu(l.n, 1, l.bias_updates, 1, ਅਧਾਰ.biases, 1);
        axpy_cpu(l.nweights, 1, l.ਭਾਰ_updates, 1, ਅਧਾਰ.weights, 1);
        ਜੇ (l.scales) {
            axpy_cpu(l.n, 1, l.scale_updates, 1, ਅਧਾਰ.scales, 1);
        }
    } ਨਹੀਂਤਾਂ ਜੇ(l.type == CONNECTED) {
        axpy_cpu(l.outputs, 1, l.bias_updates, 1, ਅਧਾਰ.biases, 1);
        axpy_cpu(l.outputs*l.inputs, 1, l.ਭਾਰ_updates, 1, ਅਧਾਰ.weights, 1);
    }
}

ਖਾਲੀ scale_weights(layer l, ਦਸ਼ਮਲਵ s)
{
    ਜੇ (l.type == CONVOLUTIONAL) {
        scal_cpu(l.n, s, l.biases, 1);
        scal_cpu(l.nweights, s, l.weights, 1);
        ਜੇ (l.scales) {
            scal_cpu(l.n, s, l.scales, 1);
        }
    } ਨਹੀਂਤਾਂ ਜੇ(l.type == CONNECTED) {
        scal_cpu(l.outputs, s, l.biases, 1);
        scal_cpu(l.outputs*l.inputs, s, l.weights, 1);
    }
}


ਖਾਲੀ pull_weights(layer l)
{
    ਜੇ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.n);
        cuda_pull_array(l.weights_gpu, l.ਭਾਰ_updates, l.nweights);
        ਜੇ(l.scales) cuda_pull_array(l.scales_gpu, l.scale_updates, l.n);
    } ਨਹੀਂਤਾਂ ਜੇ(l.type == CONNECTED){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.outputs);
        cuda_pull_array(l.weights_gpu, l.ਭਾਰ_updates, l.outputs*l.inputs);
    }
}

ਖਾਲੀ ਧੱਕੋ_weights(layer l)
{
    ਜੇ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_ਧੱਕੋ_array(l.biases_gpu, l.biases, l.n);
        cuda_ਧੱਕੋ_array(l.weights_gpu, l.weights, l.nweights);
        ਜੇ(l.scales) cuda_ਧੱਕੋ_array(l.scales_gpu, l.scales, l.n);
    } ਨਹੀਂਤਾਂ ਜੇ(l.type == CONNECTED){
        cuda_ਧੱਕੋ_array(l.biases_gpu, l.biases, l.outputs);
        cuda_ਧੱਕੋ_array(l.weights_gpu, l.weights, l.outputs*l.inputs);
    }
}

ਖਾਲੀ distribute_weights(layer l, layer ਅਧਾਰ)
{
    ਜੇ (l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL) {
        cuda_ਧੱਕੋ_array(l.biases_gpu, ਅਧਾਰ.biases, l.n);
        cuda_ਧੱਕੋ_array(l.weights_gpu, ਅਧਾਰ.weights, l.nweights);
        ਜੇ (ਅਧਾਰ.scales) cuda_ਧੱਕੋ_array(l.scales_gpu, ਅਧਾਰ.scales, l.n);
    } ਨਹੀਂਤਾਂ ਜੇ (l.type == CONNECTED) {
        cuda_ਧੱਕੋ_array(l.biases_gpu, ਅਧਾਰ.biases, l.outputs);
        cuda_ਧੱਕੋ_array(l.weights_gpu, ਅਧਾਰ.weights, l.outputs*l.inputs);
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

ਖਾਲੀ sync_layer(ਜਾਲ **nets, ਪੂਰਨ n, ਪੂਰਨ j)
{
    ਪੂਰਨ i;
    ਜਾਲ *net = nets[0];
    layer ਅਧਾਰ = net->layers[j];
    scale_weights(ਅਧਾਰ, 0);
    ਲਈ (i = 0; i < n; ++i) {
        cuda_ਸੈੱਟ_device(nets[i]->gpu_ਸੂਚਕਾਂਕ);
        layer l = nets[i]->layers[j];
        pull_weights(l);
        ਮਿਲਾਓ_weights(l, ਅਧਾਰ);
    }
    scale_weights(ਅਧਾਰ, 1./n);
    ਲਈ (i = 0; i < n; ++i) {
        cuda_ਸੈੱਟ_device(nets[i]->gpu_ਸੂਚਕਾਂਕ);
        layer l = nets[i]->layers[j];
        distribute_weights(l, ਅਧਾਰ);
    }
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ{
    ਜਾਲ **nets;
    ਪੂਰਨ n;
    ਪੂਰਨ j;
} sync_ਦਲੀਲਾਂ;

ਖਾਲੀ *sync_layer_thread(ਖਾਲੀ *ptr)
{
    sync_ਦਲੀਲਾਂ ਦਲੀਲਾਂ = *(sync_ਦਲੀਲਾਂ*)ptr;
    sync_layer(ਦਲੀਲਾਂ.nets, ਦਲੀਲਾਂ.n, ਦਲੀਲਾਂ.j);
    ਮੁਕਤ(ptr);
    ਵਾਪਸ 0;
}

pthread_t sync_layer_ਵਿੱਚ_thread(ਜਾਲ **nets, ਪੂਰਨ n, ਪੂਰਨ j)
{
    pthread_t thread;
    sync_ਦਲੀਲਾਂ *ptr = (sync_ਦਲੀਲਾਂ *)ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(sync_ਦਲੀਲਾਂ));
    ptr->nets = nets;
    ptr->n = n;
    ptr->j = j;
    ਜੇ(pthread_create(&thread, 0, sync_layer_thread, ptr)) error("Thread creation failed");
    ਵਾਪਸ thread;
}

ਖਾਲੀ sync_nets(ਜਾਲ **nets, ਪੂਰਨ n, ਪੂਰਨ interval)
{
    ਪੂਰਨ j;
    ਪੂਰਨ layers = nets[0]->n;
    pthread_t *ਧਾਗੇ = (pthread_t *) ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(layers, ਆਕਾਰ(pthread_t));

    *(nets[0]->ਵੇਖਿਆ) += interval * (n-1) * nets[0]->batch * nets[0]->subdivisions;
    ਲਈ (j = 0; j < n; ++j){
        *(nets[j]->ਵੇਖਿਆ) = *(nets[0]->ਵੇਖਿਆ);
    }
    ਲਈ (j = 0; j < layers; ++j) {
        ਧਾਗੇ[j] = sync_layer_ਵਿੱਚ_thread(nets, n, j);
    }
    ਲਈ (j = 0; j < layers; ++j) {
        pthread_join(ਧਾਗੇ[j], 0);
    }
    ਮੁਕਤ(ਧਾਗੇ);
}

ਦਸ਼ਮਲਵ train_networks(ਜਾਲ **nets, ਪੂਰਨ n, ਅੰਕੜੇ d, ਪੂਰਨ interval)
{
    ਪੂਰਨ i;
    ਪੂਰਨ batch = nets[0]->batch;
    ਪੂਰਨ subdivisions = nets[0]->subdivisions;
    ਪ੍ਰਤਿਜ੍ਞਾ(batch * subdivisions * n == d.X.ਪੰਕਤੀਆਂ);
    pthread_t *ਧਾਗੇ = (pthread_t *) ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(pthread_t));
    ਦਸ਼ਮਲਵ *errors = (ਦਸ਼ਮਲਵ *) ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    ਦਸ਼ਮਲਵ ਜੋੜ = 0;
    ਲਈ(i = 0; i < n; ++i){
        ਅੰਕੜੇ p = get_ਅੰਕੜੇ_part(d, i, n);
        ਧਾਗੇ[i] = train_ਜਾਲ_ਵਿੱਚ_thread(nets[i], p, errors + i);
    }
    ਲਈ(i = 0; i < n; ++i){
        pthread_join(ਧਾਗੇ[i], 0);
        //printf("%f\n", errors[i]);
        ਜੋੜ += errors[i];
    }
    //cudaDeviceSynchronize();
    ਜੇ (get_current_batch(nets[0]) % interval == 0) {
        ਲਿਖੋ("Syncing... ");
        fflush(stdout);
        sync_nets(nets, n, interval);
        ਲਿਖੋ("Done!\n");
    }
    //cudaDeviceSynchronize();
    ਮੁਕਤ(ਧਾਗੇ);
    ਮੁਕਤ(errors);
    ਵਾਪਸ (ਦਸ਼ਮਲਵ)ਜੋੜ/(n);
}

ਖਾਲੀ pull_ਜਾਲ_output(ਜਾਲ *net)
{
    layer l = get_ਜਾਲ_output_layer(net);
    cuda_pull_array(l.output_gpu, l.output, l.outputs*l.batch);
}

#ਜੇ_ਅੰਤ

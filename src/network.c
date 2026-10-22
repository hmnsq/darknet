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

ತುಂಬು_ವಾದಗಳು get_ತಳ_ವಾದಗಳು(ಜಾಲ *net)
{
    ತುಂಬು_ವಾದಗಳು ವಾದಗಳು = {0};
    ವಾದಗಳು.w = net->w;
    ವಾದಗಳು.h = net->h;
    ವಾದಗಳು.ಗಾತ್ರ_2 = net->w;

    ವಾದಗಳು.min = net->min_crop;
    ವಾದಗಳು.max = net->max_crop;
    ವಾದಗಳು.angle = net->angle;
    ವಾದಗಳು.aspect = net->aspect;
    ವಾದಗಳು.exposure = net->exposure;
    ವಾದಗಳು.center = net->center;
    ವಾದಗಳು.saturation = net->saturation;
    ವಾದಗಳು.hue = net->hue;
    ಹಿಂತಿರುಗಿ ವಾದಗಳು;
}

ಜಾಲ *ತುಂಬು_ಜಾಲ(ಅಕ್ಷರ *cfg, ಅಕ್ಷರ *weights, ಪೂರ್ಣಾಂಕ clear)
{
    ಜಾಲ *net = parse_ಜಾಲ_cfg(cfg);
    ಆದರೆ(weights && weights[0] != 0){
        ತುಂಬು_weights(net, weights);
    }
    ಆದರೆ(clear) (*net->ನೋಡಿದ) = 0;
    ಹಿಂತಿರುಗಿ net;
}

size_t get_current_batch(ಜಾಲ *net)
{
    size_t batch_ಅಂಕೆ = (*net->ನೋಡಿದ)/(net->batch*net->subdivisions);
    ಹಿಂತಿರುಗಿ batch_ಅಂಕೆ;
}

ಶೂನ್ಯ reset_ಜಾಲ_state(ಜಾಲ *net, ಪೂರ್ಣಾಂಕ b)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ (i = 0; i < net->n; ++i) {
        #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
        layer l = net->layers[i];
        ಆದರೆ(l.state_gpu){
            fill_gpu(l.outputs, 0, l.state_gpu + l.outputs*b, 1);
        }
        ಆದರೆ(l.h_gpu){
            fill_gpu(l.outputs, 0, l.h_gpu + l.outputs*b, 1);
        }
        #ಆದರೆ_ಅಂತ್ಯ
    }
}

ಶೂನ್ಯ reset_rnn(ಜಾಲ *net)
{
    reset_ಜಾಲ_state(net, 0);
}

ದಶಮಾಂಶ get_current_ದರ(ಜಾಲ *net)
{
    size_t batch_ಅಂಕೆ = get_current_batch(net);
    ಪೂರ್ಣಾಂಕ i;
    ದಶಮಾಂಶ ದರ;
    ಆದರೆ (batch_ಅಂಕೆ < net->burn_ಒಳ) ಹಿಂತಿರುಗಿ net->learning_ದರ * ಘಾತ((ದಶಮಾಂಶ)batch_ಅಂಕೆ / net->burn_ಒಳ, net->power);
    ಆಯ್ಕೆ (net->policy) {
        ಸ್ಥಿತಿ CONSTANT:
            ಹಿಂತಿರುಗಿ net->learning_ದರ;
        ಸ್ಥಿತಿ ಹೆಜ್ಜೆ_2:
            ಹಿಂತಿರುಗಿ net->learning_ದರ * ಘಾತ(net->scale, batch_ಅಂಕೆ/net->ಹೆಜ್ಜೆ);
        ಸ್ಥಿತಿ ಹೆಜ್ಜೆಗಳು_2:
            ದರ = net->learning_ದರ;
            ಗಾಗಿ(i = 0; i < net->ಅಂಕೆ_ಹೆಜ್ಜೆಗಳು; ++i){
                ಆದರೆ(net->ಹೆಜ್ಜೆಗಳು[i] > batch_ಅಂಕೆ) ಹಿಂತಿರುಗಿ ದರ;
                ದರ *= net->scales[i];
            }
            ಹಿಂತಿರುಗಿ ದರ;
        ಸ್ಥಿತಿ EXP:
            ಹಿಂತಿರುಗಿ net->learning_ದರ * ಘಾತ(net->gamma, batch_ಅಂಕೆ);
        ಸ್ಥಿತಿ POLY:
            ಹಿಂತಿರುಗಿ net->learning_ದರ * ಘಾತ(1 - (ದಶಮಾಂಶ)batch_ಅಂಕೆ / net->max_batches, net->power);
        ಸ್ಥಿತಿ RANDOM:
            ಹಿಂತಿರುಗಿ net->learning_ದರ * ಘಾತ(rand_ಏಕರೂಪ(0,1), net->power);
        ಸ್ಥಿತಿ SIG:
            ಹಿಂತಿರುಗಿ net->learning_ದರ * (1./(1.+ಘಾತಾಂಕ(net->gamma*(batch_ಅಂಕೆ - net->ಹೆಜ್ಜೆ))));
        ಪೂರ್ವನಿಯೋಜಿತ:
            fprintf(stderr, "Policy is weird!\n");
            ಹಿಂತಿರುಗಿ net->learning_ದರ;
    }
}

ಅಕ್ಷರ *get_layer_string(LAYER_TYPE a)
{
    ಆಯ್ಕೆ(a){
        ಸ್ಥಿತಿ CONVOLUTIONAL:
            ಹಿಂತಿರುಗಿ "convolutional";
        ಸ್ಥಿತಿ ಸಕ್ರಿಯ:
            ಹಿಂತಿರುಗಿ "activation";
        ಸ್ಥಿತಿ LOCAL:
            ಹಿಂತಿರುಗಿ "local";
        ಸ್ಥಿತಿ DECONVOLUTIONAL:
            ಹಿಂತಿರುಗಿ "deconvolutional";
        ಸ್ಥಿತಿ CONNECTED:
            ಹಿಂತಿರುಗಿ "connected";
        ಸ್ಥಿತಿ RNN:
            ಹಿಂತಿರುಗಿ "rnn";
        ಸ್ಥಿತಿ GRU:
            ಹಿಂತಿರುಗಿ "gru";
        ಸ್ಥಿತಿ LSTM:
	    ಹಿಂತಿರುಗಿ "lstm";
        ಸ್ಥಿತಿ CRNN:
            ಹಿಂತಿರುಗಿ "crnn";
        ಸ್ಥಿತಿ MAXPOOL:
            ಹಿಂತಿರುಗಿ "maxpool";
        ಸ್ಥಿತಿ REORG:
            ಹಿಂತಿರುಗಿ "reorg";
        ಸ್ಥಿತಿ AVGPOOL:
            ಹಿಂತಿರುಗಿ "avgpool";
        ಸ್ಥಿತಿ SOFTMAX:
            ಹಿಂತಿರುಗಿ "softmax";
        ಸ್ಥಿತಿ DETECTION:
            ಹಿಂತಿರುಗಿ "detection";
        ಸ್ಥಿತಿ REGION:
            ಹಿಂತಿರುಗಿ "region";
        ಸ್ಥಿತಿ YOLO:
            ಹಿಂತಿರುಗಿ "yolo";
        ಸ್ಥಿತಿ DROPOUT:
            ಹಿಂತಿರುಗಿ "dropout";
        ಸ್ಥಿತಿ CROP:
            ಹಿಂತಿರುಗಿ "crop";
        ಸ್ಥಿತಿ ವೆಚ್ಚ:
            ಹಿಂತಿರುಗಿ "cost";
        ಸ್ಥಿತಿ ROUTE:
            ಹಿಂತಿರುಗಿ "route";
        ಸ್ಥಿತಿ SHORTCUT:
            ಹಿಂತಿರುಗಿ "shortcut";
        ಸ್ಥಿತಿ NORMALIZATION:
            ಹಿಂತಿರುಗಿ "normalization";
        ಸ್ಥಿತಿ BATCHNORM:
            ಹಿಂತಿರುಗಿ "batchnorm";
        ಪೂರ್ವನಿಯೋಜಿತ:
            ನಿಲ್ಲಿಸು;
    }
    ಹಿಂತಿರುಗಿ "none";
}

ಜಾಲ *make_ಜಾಲ(ಪೂರ್ಣಾಂಕ n)
{
    ಜಾಲ *net = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(ಜಾಲ));
    net->n = n;
    net->layers = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(net->n, ಗಾತ್ರ(layer));
    net->ನೋಡಿದ = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(size_t));
    net->t    = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ));
    net->ವೆಚ್ಚ_2 = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಹಿಂತಿರುಗಿ net;
}

ಶೂನ್ಯ forward_ಜಾಲ(ಜಾಲ *netp)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    ಆದರೆ(netp->gpu_ಸೂಚ್ಯಂಕ >= 0){
        forward_ಜಾಲ_gpu(netp);   
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ
    ಜಾಲ net = *netp;
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < net.n; ++i){
        net.ಸೂಚ್ಯಂಕ = i;
        layer l = net.layers[i];
        ಆದರೆ(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.ಒಳಹರಿವು = l.output;
        ಆದರೆ(l.truth) {
            net.truth = l.output;
        }
    }
    calc_ಜಾಲ_ವೆಚ್ಚ(netp);
}

ಶೂನ್ಯ update_ಜಾಲ(ಜಾಲ *netp)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    ಆದರೆ(netp->gpu_ಸೂಚ್ಯಂಕ >= 0){
        update_ಜಾಲ_gpu(netp);   
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ
    ಜಾಲ net = *netp;
    ಪೂರ್ಣಾಂಕ i;
    update_ವಾದಗಳು a = {0};
    a.batch = net.batch*net.subdivisions;
    a.learning_ದರ = get_current_ದರ(netp);
    a.momentum = net.momentum;
    a.decay = net.decay;
    a.adam = net.adam;
    a.B1 = net.B1;
    a.B2 = net.B2;
    a.eps = net.eps;
    ++*net.t;
    a.t = *net.t;

    ಗಾಗಿ(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        ಆದರೆ(l.update){
            l.update(l, a);
        }
    }
}

ಶೂನ್ಯ calc_ಜಾಲ_ವೆಚ್ಚ(ಜಾಲ *netp)
{
    ಜಾಲ net = *netp;
    ಪೂರ್ಣಾಂಕ i;
    ದಶಮಾಂಶ ಮೊತ್ತ = 0;
    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
    ಗಾಗಿ(i = 0; i < net.n; ++i){
        ಆದರೆ(net.layers[i].ವೆಚ್ಚ_2){
            ಮೊತ್ತ += net.layers[i].ವೆಚ್ಚ_2[0];
            ++ಎಣಿಕೆ_2;
        }
    }
    *net.ವೆಚ್ಚ_2 = ಮೊತ್ತ/ಎಣಿಕೆ_2;
}

ಪೂರ್ಣಾಂಕ get_predicted_class_ಜಾಲ(ಜಾಲ *net)
{
    ಹಿಂತಿರುಗಿ max_ಸೂಚ್ಯಂಕ(net->output, net->outputs);
}

ಶೂನ್ಯ backward_ಜಾಲ(ಜಾಲ *netp)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    ಆದರೆ(netp->gpu_ಸೂಚ್ಯಂಕ >= 0){
        backward_ಜಾಲ_gpu(netp);   
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ
    ಜಾಲ net = *netp;
    ಪೂರ್ಣಾಂಕ i;
    ಜಾಲ orig = net;
    ಗಾಗಿ(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        ಆದರೆ(l.stopbackward) ನಿಲ್ಲಿಸು;
        ಆದರೆ(i == 0){
            net = orig;
        }ಇಲ್ಲದಿದ್ದರೆ{
            layer prev = net.layers[i-1];
            net.ಒಳಹರಿವು = prev.output;
            net.delta = prev.delta;
        }
        net.ಸೂಚ್ಯಂಕ = i;
        l.backward(l, net);
    }
}

ದಶಮಾಂಶ train_ಜಾಲ_datum(ಜಾಲ *net)
{
    *net->ನೋಡಿದ += net->batch;
    net->train = 1;
    forward_ಜಾಲ(net);
    backward_ಜಾಲ(net);
    ದಶಮಾಂಶ error = *net->ವೆಚ್ಚ_2;
    ಆದರೆ(((*net->ನೋಡಿದ)/net->batch)%net->subdivisions == 0) update_ಜಾಲ(net);
    ಹಿಂತಿರುಗಿ error;
}

ದಶಮಾಂಶ train_ಜಾಲ_sgd(ಜಾಲ *net, ದತ್ತಾಂಶ d, ಪೂರ್ಣಾಂಕ n)
{
    ಪೂರ್ಣಾಂಕ batch = net->batch;

    ಪೂರ್ಣಾಂಕ i;
    ದಶಮಾಂಶ ಮೊತ್ತ = 0;
    ಗಾಗಿ(i = 0; i < n; ++i){
        get_random_batch(d, batch, net->ಒಳಹರಿವು, net->truth);
        ದಶಮಾಂಶ err = train_ಜಾಲ_datum(net);
        ಮೊತ್ತ += err;
    }
    ಹಿಂತಿರುಗಿ (ದಶಮಾಂಶ)ಮೊತ್ತ/(n*batch);
}

ದಶಮಾಂಶ train_ಜಾಲ(ಜಾಲ *net, ದತ್ತಾಂಶ d)
{
    ಪ್ರತಿಜ್ಞಾ(d.X.ಅಡ್ಡಸಾಲುಗಳು % net->batch == 0);
    ಪೂರ್ಣಾಂಕ batch = net->batch;
    ಪೂರ್ಣಾಂಕ n = d.X.ಅಡ್ಡಸಾಲುಗಳು / batch;

    ಪೂರ್ಣಾಂಕ i;
    ದಶಮಾಂಶ ಮೊತ್ತ = 0;
    ಗಾಗಿ(i = 0; i < n; ++i){
        get_ಮುಂದಿನ_batch(d, batch, i*batch, net->ಒಳಹರಿವು, net->truth);
        ದಶಮಾಂಶ err = train_ಜಾಲ_datum(net);
        ಮೊತ್ತ += err;
    }
    ಹಿಂತಿರುಗಿ (ದಶಮಾಂಶ)ಮೊತ್ತ/(n*batch);
}

ಶೂನ್ಯ ಗಣ_ತಾತ್ಕಾಲಿಕ_ಜಾಲ(ಜಾಲ *net, ದಶಮಾಂಶ t)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < net->n; ++i){
        net->layers[i].temperature = t;
    }
}


ಶೂನ್ಯ ಗಣ_batch_ಜಾಲ(ಜಾಲ *net, ಪೂರ್ಣಾಂಕ b)
{
    net->batch = b;
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CUDNN
        ಆದರೆ(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_ಸಿದ್ಧತೆ(net->layers + i);
        }
        ಆದರೆ(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnn_ಗಣ_Tensor4d_Descriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, 1, l->ಹೊರಹೋಗು_c, l->ಹೊರಹೋಗು_h, l->ಹೊರಹೋಗು_w);
            cudnn_ಗಣ_Tensor4d_Descriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, 1, l->ಹೊರಹೋಗು_c, 1, 1); 
        }
#ಆದರೆ_ಅಂತ್ಯ
    }
}

ಪೂರ್ಣಾಂಕ resize_ಜಾಲ(ಜಾಲ *net, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    cuda_ಗಣ_device(net->gpu_ಸೂಚ್ಯಂಕ);
    cuda_free(net->workspace);
#ಆದರೆ_ಅಂತ್ಯ
    ಪೂರ್ಣಾಂಕ i;
    //if(w == net->w && h == net->h) return 0;
    net->w = w;
    net->h = h;
    ಪೂರ್ಣಾಂಕ inputs = 0;
    size_t workspace_ಗಾತ್ರ = 0;
    //fprintf(stderr, "Resizing to %d x %d...\n", w, h);
    //fflush(stderr);
    ಗಾಗಿ (i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ಆದರೆ(l.type == CONVOLUTIONAL){
            resize_convolutional_layer(&l, w, h);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(l.type == CROP){
            resize_crop_layer(&l, w, h);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(l.type == MAXPOOL){
            resize_maxpool_layer(&l, w, h);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(l.type == REGION){
            resize_region_layer(&l, w, h);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(l.type == YOLO){
            resize_yolo_layer(&l, w, h);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(l.type == ROUTE){
            resize_route_layer(&l, net);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(l.type == SHORTCUT){
            resize_shortcut_layer(&l, w, h);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(l.type == UPSAMPLE){
            resize_upsample_layer(&l, w, h);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(l.type == REORG){
            resize_reorg_layer(&l, w, h);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(l.type == AVGPOOL){
            resize_avgpool_layer(&l, w, h);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(l.type == NORMALIZATION){
            resize_normalization_layer(&l, w, h);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(l.type == ವೆಚ್ಚ){
            resize_ವೆಚ್ಚ_layer(&l, inputs);
        }ಇಲ್ಲದಿದ್ದರೆ{
            error("Cannot resize this type of layer");
        }
        ಆದರೆ(l.workspace_ಗಾತ್ರ > workspace_ಗಾತ್ರ) workspace_ಗಾತ್ರ = l.workspace_ಗಾತ್ರ;
        ಆದರೆ(l.workspace_ಗಾತ್ರ > 2000000000) ಪ್ರತಿಜ್ಞಾ(0);
        inputs = l.outputs;
        net->layers[i] = l;
        w = l.ಹೊರಹೋಗು_w;
        h = l.ಹೊರಹೋಗು_h;
        ಆದರೆ(l.type == AVGPOOL) ನಿಲ್ಲಿಸು;
    }
    layer ಹೊರಹೋಗು = get_ಜಾಲ_output_layer(net);
    net->inputs = net->layers[0].inputs;
    net->outputs = ಹೊರಹೋಗು.outputs;
    net->truths = ಹೊರಹೋಗು.outputs;
    ಆದರೆ(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = ಹೊರಹೋಗು.output;
    ಬಿಡುಗಡೆ(net->ಒಳಹರಿವು);
    ಬಿಡುಗಡೆ(net->truth);
    net->ಒಳಹರಿವು = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(net->inputs*net->batch, ಗಾತ್ರ(ದಶಮಾಂಶ));
    net->truth = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(net->truths*net->batch, ಗಾತ್ರ(ದಶಮಾಂಶ));
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    ಆದರೆ(gpu_ಸೂಚ್ಯಂಕ >= 0){
        cuda_free(net->ಒಳಹರಿವು_gpu);
        cuda_free(net->truth_gpu);
        net->ಒಳಹರಿವು_gpu = cuda_make_array(net->ಒಳಹರಿವು, net->inputs*net->batch);
        net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
        ಆದರೆ(workspace_ಗಾತ್ರ){
            net->workspace = cuda_make_array(0, (workspace_ಗಾತ್ರ-1)/ಗಾತ್ರ(ದಶಮಾಂಶ)+1);
        }
    }ಇಲ್ಲದಿದ್ದರೆ {
        ಬಿಡುಗಡೆ(net->workspace);
        net->workspace = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, workspace_ಗಾತ್ರ);
    }
#ಇಲ್ಲದಿದ್ದರೆ
    ಬಿಡುಗಡೆ(net->workspace);
    net->workspace = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, workspace_ಗಾತ್ರ);
#ಆದರೆ_ಅಂತ್ಯ
    //fprintf(stderr, " Done!\n");
    ಹಿಂತಿರುಗಿ 0;
}

layer get_ಜಾಲ_detection_layer(ಜಾಲ *net)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < net->n; ++i){
        ಆದರೆ(net->layers[i].type == DETECTION){
            ಹಿಂತಿರುಗಿ net->layers[i];
        }
    }
    fprintf(stderr, "Detection layer not found!!\n");
    layer l = {0};
    ಹಿಂತಿರುಗಿ l;
}

image get_ಜಾಲ_image_layer(ಜಾಲ *net, ಪೂರ್ಣಾಂಕ i)
{
    layer l = net->layers[i];
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    //cuda_pull_array(l.output_gpu, l.output, l.outputs);
#ಆದರೆ_ಅಂತ್ಯ
    ಆದರೆ (l.ಹೊರಹೋಗು_w && l.ಹೊರಹೋಗು_h && l.ಹೊರಹೋಗು_c){
        ಹಿಂತಿರುಗಿ float_ಗಮ್ಯ_image(l.ಹೊರಹೋಗು_w, l.ಹೊರಹೋಗು_h, l.ಹೊರಹೋಗು_c, l.output);
    }
    image def = {0};
    ಹಿಂತಿರುಗಿ def;
}

image get_ಜಾಲ_image(ಜಾಲ *net)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = net->n-1; i >= 0; --i){
        image m = get_ಜಾಲ_image_layer(net, i);
        ಆದರೆ(m.h != 0) ಹಿಂತಿರುಗಿ m;
    }
    image def = {0};
    ಹಿಂತಿರುಗಿ def;
}

ಶೂನ್ಯ visualize_ಜಾಲ(ಜಾಲ *net)
{
    image *prev = 0;
    ಪೂರ್ಣಾಂಕ i;
    ಅಕ್ಷರ buff[256];
    ಗಾಗಿ(i = 0; i < net->n; ++i){
        sprintf(buff, "Layer %d", i);
        layer l = net->layers[i];
        ಆದರೆ(l.type == CONVOLUTIONAL){
            prev = visualize_convolutional_layer(l, buff, prev);
        }
    } 
}

ಶೂನ್ಯ ತುದಿ_predictions(ಜಾಲ *net, ಪೂರ್ಣಾಂಕ k, ಪೂರ್ಣಾಂಕ *ಸೂಚ್ಯಂಕ)
{
    ತುದಿ_k(net->output, net->outputs, k, ಸೂಚ್ಯಂಕ);
}


ದಶಮಾಂಶ *ಜಾಲ_predict(ಜಾಲ *net, ದಶಮಾಂಶ *ಒಳಹರಿವು)
{
    ಜಾಲ orig = *net;
    net->ಒಳಹರಿವು = ಒಳಹರಿವು;
    net->truth = 0;
    net->train = 0;
    net->delta = 0;
    forward_ಜಾಲ(net);
    ದಶಮಾಂಶ *ಹೊರಹೋಗು = net->output;
    *net = orig;
    ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು;
}

ಪೂರ್ಣಾಂಕ ಅಂಕೆ_detections(ಜಾಲ *net, ದಶಮಾಂಶ thresh)
{
    ಪೂರ್ಣಾಂಕ i;
    ಪೂರ್ಣಾಂಕ s = 0;
    ಗಾಗಿ(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ಆದರೆ(l.type == YOLO){
            s += yolo_ಅಂಕೆ_detections(l, thresh);
        }
        ಆದರೆ(l.type == DETECTION || l.type == REGION){
            s += l.w*l.h*l.n;
        }
    }
    ಹಿಂತಿರುಗಿ s;
}

detection *make_ಜಾಲ_boxes(ಜಾಲ *net, ದಶಮಾಂಶ thresh, ಪೂರ್ಣಾಂಕ *ಅಂಕೆ)
{
    layer l = net->layers[net->n - 1];
    ಪೂರ್ಣಾಂಕ i;
    ಪೂರ್ಣಾಂಕ nboxes = ಅಂಕೆ_detections(net, thresh);
    ಆದರೆ(ಅಂಕೆ) *ಅಂಕೆ = nboxes;
    detection *dets = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(nboxes, ಗಾತ್ರ(detection));
    ಗಾಗಿ(i = 0; i < nboxes; ++i){
        dets[i].prob = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.classes, ಗಾತ್ರ(ದಶಮಾಂಶ));
        ಆದರೆ(l.coords > 4){
            dets[i].ಮುಖವಾಡ = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.coords-4, ಗಾತ್ರ(ದಶಮಾಂಶ));
        }
    }
    ಹಿಂತಿರುಗಿ dets;
}

ಶೂನ್ಯ fill_ಜಾಲ_boxes(ಜಾಲ *net, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ದಶಮಾಂಶ thresh, ದಶಮಾಂಶ hier, ಪೂರ್ಣಾಂಕ *map, ಪೂರ್ಣಾಂಕ relative, detection *dets)
{
    ಪೂರ್ಣಾಂಕ j;
    ಗಾಗಿ(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        ಆದರೆ(l.type == YOLO){
            ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
            dets += ಎಣಿಕೆ_2;
        }
        ಆದರೆ(l.type == REGION){
            get_region_detections(l, w, h, net->w, net->h, thresh, map, hier, relative, dets);
            dets += l.w*l.h*l.n;
        }
        ಆದರೆ(l.type == DETECTION){
            get_detection_detections(l, w, h, thresh, dets);
            dets += l.w*l.h*l.n;
        }
    }
}

detection *get_ಜಾಲ_boxes(ಜಾಲ *net, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ದಶಮಾಂಶ thresh, ದಶಮಾಂಶ hier, ಪೂರ್ಣಾಂಕ *map, ಪೂರ್ಣಾಂಕ relative, ಪೂರ್ಣಾಂಕ *ಅಂಕೆ)
{
    detection *dets = make_ಜಾಲ_boxes(net, thresh, ಅಂಕೆ);
    fill_ಜಾಲ_boxes(net, w, h, thresh, hier, map, relative, dets);
    ಹಿಂತಿರುಗಿ dets;
}

ಶೂನ್ಯ free_detections(detection *dets, ಪೂರ್ಣಾಂಕ n)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < n; ++i){
        ಬಿಡುಗಡೆ(dets[i].prob);
        ಆದರೆ(dets[i].ಮುಖವಾಡ) ಬಿಡುಗಡೆ(dets[i].ಮುಖವಾಡ);
    }
    ಬಿಡುಗಡೆ(dets);
}

ದಶಮಾಂಶ *ಜಾಲ_predict_image(ಜಾಲ *net, image im)
{
    image imr = letterbox_image(im, net->w, net->h);
    ಗಣ_batch_ಜಾಲ(net, 1);
    ದಶಮಾಂಶ *p = ಜಾಲ_predict(net, imr.ದತ್ತಾಂಶ);
    free_image(imr);
    ಹಿಂತಿರುಗಿ p;
}

ಪೂರ್ಣಾಂಕ ಜಾಲ_ಅಗಲ(ಜಾಲ *net){ಹಿಂತಿರುಗಿ net->w;}
ಪೂರ್ಣಾಂಕ ಜಾಲ_ಎತ್ತರ(ಜಾಲ *net){ಹಿಂತಿರುಗಿ net->h;}

ಮಾತೃಕೆ ಜಾಲ_predict_ದತ್ತಾಂಶ_multi(ಜಾಲ *net, ದತ್ತಾಂಶ test, ಪೂರ್ಣಾಂಕ n)
{
    ಪೂರ್ಣಾಂಕ i,j,b,m;
    ಪೂರ್ಣಾಂಕ k = net->outputs;
    ಮಾತೃಕೆ pred = make_ಮಾತೃಕೆ(test.X.ಅಡ್ಡಸಾಲುಗಳು, k);
    ದಶಮಾಂಶ *X = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(net->batch*test.X.ಅಡ್ಡಸಾಲುಗಳು, ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಗಾಗಿ(i = 0; i < test.X.ಅಡ್ಡಸಾಲುಗಳು; i += net->batch){
        ಗಾಗಿ(b = 0; b < net->batch; ++b){
            ಆದರೆ(i+b == test.X.ಅಡ್ಡಸಾಲುಗಳು) ನಿಲ್ಲಿಸು;
            ಸ್ಮರಣೆ_ನಕಲು(X+b*test.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, test.X.vals[i+b], test.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ*ಗಾತ್ರ(ದಶಮಾಂಶ));
        }
        ಗಾಗಿ(m = 0; m < n; ++m){
            ದಶಮಾಂಶ *ಹೊರಹೋಗು = ಜಾಲ_predict(net, X);
            ಗಾಗಿ(b = 0; b < net->batch; ++b){
                ಆದರೆ(i+b == test.X.ಅಡ್ಡಸಾಲುಗಳು) ನಿಲ್ಲಿಸು;
                ಗಾಗಿ(j = 0; j < k; ++j){
                    pred.vals[i+b][j] += ಹೊರಹೋಗು[j+b*k]/n;
                }
            }
        }
    }
    ಬಿಡುಗಡೆ(X);
    ಹಿಂತಿರುಗಿ pred;   
}

ಮಾತೃಕೆ ಜಾಲ_predict_ದತ್ತಾಂಶ(ಜಾಲ *net, ದತ್ತಾಂಶ test)
{
    ಪೂರ್ಣಾಂಕ i,j,b;
    ಪೂರ್ಣಾಂಕ k = net->outputs;
    ಮಾತೃಕೆ pred = make_ಮಾತೃಕೆ(test.X.ಅಡ್ಡಸಾಲುಗಳು, k);
    ದಶಮಾಂಶ *X = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(net->batch*test.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಗಾಗಿ(i = 0; i < test.X.ಅಡ್ಡಸಾಲುಗಳು; i += net->batch){
        ಗಾಗಿ(b = 0; b < net->batch; ++b){
            ಆದರೆ(i+b == test.X.ಅಡ್ಡಸಾಲುಗಳು) ನಿಲ್ಲಿಸು;
            ಸ್ಮರಣೆ_ನಕಲು(X+b*test.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, test.X.vals[i+b], test.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ*ಗಾತ್ರ(ದಶಮಾಂಶ));
        }
        ದಶಮಾಂಶ *ಹೊರಹೋಗು = ಜಾಲ_predict(net, X);
        ಗಾಗಿ(b = 0; b < net->batch; ++b){
            ಆದರೆ(i+b == test.X.ಅಡ್ಡಸಾಲುಗಳು) ನಿಲ್ಲಿಸು;
            ಗಾಗಿ(j = 0; j < k; ++j){
                pred.vals[i+b][j] = ಹೊರಹೋಗು[j+b*k];
            }
        }
    }
    ಬಿಡುಗಡೆ(X);
    ಹಿಂತಿರುಗಿ pred;   
}

ಶೂನ್ಯ print_ಜಾಲ(ಜಾಲ *net)
{
    ಪೂರ್ಣಾಂಕ i,j;
    ಗಾಗಿ(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ದಶಮಾಂಶ *output = l.output;
        ಪೂರ್ಣಾಂಕ n = l.outputs;
        ದಶಮಾಂಶ ಸರಾಸರಿ = ಸರಾಸರಿ_array(output, n);
        ದಶಮಾಂಶ vari = variance_array(output, n);
        fprintf(stderr, "Layer %d - Mean: %f, Variance: %f\n",i,ಸರಾಸರಿ, vari);
        ಆದರೆ(n > 100) n = 100;
        ಗಾಗಿ(j = 0; j < n; ++j) fprintf(stderr, "%f, ", output[j]);
        ಆದರೆ(n == 100)fprintf(stderr,".....\n");
        fprintf(stderr, "\n");
    }
}

ಶೂನ್ಯ compare_networks(ಜಾಲ *n1, ಜಾಲ *n2, ದತ್ತಾಂಶ test)
{
    ಮಾತೃಕೆ g1 = ಜಾಲ_predict_ದತ್ತಾಂಶ(n1, test);
    ಮಾತೃಕೆ g2 = ಜಾಲ_predict_ದತ್ತಾಂಶ(n2, test);
    ಪೂರ್ಣಾಂಕ i;
    ಪೂರ್ಣಾಂಕ a,b,c,d;
    a = b = c = d = 0;
    ಗಾಗಿ(i = 0; i < g1.ಅಡ್ಡಸಾಲುಗಳು; ++i){
        ಪೂರ್ಣಾಂಕ truth = max_ಸೂಚ್ಯಂಕ(test.y.vals[i], test.y.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
        ಪೂರ್ಣಾಂಕ p1 = max_ಸೂಚ್ಯಂಕ(g1.vals[i], g1.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
        ಪೂರ್ಣಾಂಕ p2 = max_ಸೂಚ್ಯಂಕ(g2.vals[i], g2.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
        ಆದರೆ(p1 == truth){
            ಆದರೆ(p2 == truth) ++d;
            ಇಲ್ಲದಿದ್ದರೆ ++c;
        }ಇಲ್ಲದಿದ್ದರೆ{
            ಆದರೆ(p2 == truth) ++b;
            ಇಲ್ಲದಿದ್ದರೆ ++a;
        }
    }
    ಬರೆ("%5d %5d\n%5d %5d\n", a, b, c, d);
    ದಶಮಾಂಶ ಅಂಕೆ = ಘಾತ((ನಿರಪೇಕ್ಷ(b - c) - 1.), 2.);
    ದಶಮಾಂಶ den = b + c;
    ಬರೆ("%f\n", ಅಂಕೆ/den); 
}

ದಶಮಾಂಶ ಜಾಲ_accuracy(ಜಾಲ *net, ದತ್ತಾಂಶ d)
{
    ಮಾತೃಕೆ guess = ಜಾಲ_predict_ದತ್ತಾಂಶ(net, d);
    ದಶಮಾಂಶ acc = ಮಾತೃಕೆ_topk_accuracy(d.y, guess,1);
    free_ಮಾತೃಕೆ(guess);
    ಹಿಂತಿರುಗಿ acc;
}

ದಶಮಾಂಶ *ಜಾಲ_accuracies(ಜಾಲ *net, ದತ್ತಾಂಶ d, ಪೂರ್ಣಾಂಕ n)
{
    ಸ್ಥಿರ ದಶಮಾಂಶ acc[2];
    ಮಾತೃಕೆ guess = ಜಾಲ_predict_ದತ್ತಾಂಶ(net, d);
    acc[0] = ಮಾತೃಕೆ_topk_accuracy(d.y, guess, 1);
    acc[1] = ಮಾತೃಕೆ_topk_accuracy(d.y, guess, n);
    free_ಮಾತೃಕೆ(guess);
    ಹಿಂತಿರುಗಿ acc;
}

layer get_ಜಾಲ_output_layer(ಜಾಲ *net)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = net->n - 1; i >= 0; --i){
        ಆದರೆ(net->layers[i].type != ವೆಚ್ಚ) ನಿಲ್ಲಿಸು;
    }
    ಹಿಂತಿರುಗಿ net->layers[i];
}

ದಶಮಾಂಶ ಜಾಲ_accuracy_multi(ಜಾಲ *net, ದತ್ತಾಂಶ d, ಪೂರ್ಣಾಂಕ n)
{
    ಮಾತೃಕೆ guess = ಜಾಲ_predict_ದತ್ತಾಂಶ_multi(net, d, n);
    ದಶಮಾಂಶ acc = ಮಾತೃಕೆ_topk_accuracy(d.y, guess,1);
    free_ಮಾತೃಕೆ(guess);
    ಹಿಂತಿರುಗಿ acc;
}

ಶೂನ್ಯ free_ಜಾಲ(ಜಾಲ *net)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < net->n; ++i){
        free_layer(net->layers[i]);
    }
    ಬಿಡುಗಡೆ(net->layers);
    ಆದರೆ(net->ಒಳಹರಿವು) ಬಿಡುಗಡೆ(net->ಒಳಹರಿವು);
    ಆದರೆ(net->truth) ಬಿಡುಗಡೆ(net->truth);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    ಆದರೆ(net->ಒಳಹರಿವು_gpu) cuda_free(net->ಒಳಹರಿವು_gpu);
    ಆದರೆ(net->truth_gpu) cuda_free(net->truth_gpu);
#ಆದರೆ_ಅಂತ್ಯ
    ಬಿಡುಗಡೆ(net);
}

// Some day...
// ^ What the hell is this comment for?


layer ಜಾಲ_output_layer(ಜಾಲ *net)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = net->n - 1; i >= 0; --i){
        ಆದರೆ(net->layers[i].type != ವೆಚ್ಚ) ನಿಲ್ಲಿಸು;
    }
    ಹಿಂತಿರುಗಿ net->layers[i];
}

ಪೂರ್ಣಾಂಕ ಜಾಲ_inputs(ಜಾಲ *net)
{
    ಹಿಂತಿರುಗಿ net->layers[0].inputs;
}

ಪೂರ್ಣಾಂಕ ಜಾಲ_outputs(ಜಾಲ *net)
{
    ಹಿಂತಿರುಗಿ ಜಾಲ_output_layer(net).outputs;
}

ದಶಮಾಂಶ *ಜಾಲ_output(ಜಾಲ *net)
{
    ಹಿಂತಿರುಗಿ ಜಾಲ_output_layer(net).output;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU

ಶೂನ್ಯ forward_ಜಾಲ_gpu(ಜಾಲ *netp)
{
    ಜಾಲ net = *netp;
    cuda_ಗಣ_device(net.gpu_ಸೂಚ್ಯಂಕ);
    cuda_ತಳ್ಳು_array(net.ಒಳಹರಿವು_gpu, net.ಒಳಹರಿವು, net.inputs*net.batch);
    ಆದರೆ(net.truth){
        cuda_ತಳ್ಳು_array(net.truth_gpu, net.truth, net.truths*net.batch);
    }

    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < net.n; ++i){
        net.ಸೂಚ್ಯಂಕ = i;
        layer l = net.layers[i];
        ಆದರೆ(l.delta_gpu){
            fill_gpu(l.outputs * l.batch, 0, l.delta_gpu, 1);
        }
        l.forward_gpu(l, net);
        net.ಒಳಹರಿವು_gpu = l.output_gpu;
        net.ಒಳಹರಿವು = l.output;
        ಆದರೆ(l.truth) {
            net.truth_gpu = l.output_gpu;
            net.truth = l.output;
        }
    }
    pull_ಜಾಲ_output(netp);
    calc_ಜಾಲ_ವೆಚ್ಚ(netp);
}

ಶೂನ್ಯ backward_ಜಾಲ_gpu(ಜಾಲ *netp)
{
    ಪೂರ್ಣಾಂಕ i;
    ಜಾಲ net = *netp;
    ಜಾಲ orig = net;
    cuda_ಗಣ_device(net.gpu_ಸೂಚ್ಯಂಕ);
    ಗಾಗಿ(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        ಆದರೆ(l.stopbackward) ನಿಲ್ಲಿಸು;
        ಆದರೆ(i == 0){
            net = orig;
        }ಇಲ್ಲದಿದ್ದರೆ{
            layer prev = net.layers[i-1];
            net.ಒಳಹರಿವು = prev.output;
            net.delta = prev.delta;
            net.ಒಳಹರಿವು_gpu = prev.output_gpu;
            net.delta_gpu = prev.delta_gpu;
        }
        net.ಸೂಚ್ಯಂಕ = i;
        l.backward_gpu(l, net);
    }
}

ಶೂನ್ಯ update_ಜಾಲ_gpu(ಜಾಲ *netp)
{
    ಜಾಲ net = *netp;
    cuda_ಗಣ_device(net.gpu_ಸೂಚ್ಯಂಕ);
    ಪೂರ್ಣಾಂಕ i;
    update_ವಾದಗಳು a = {0};
    a.batch = net.batch*net.subdivisions;
    a.learning_ದರ = get_current_ದರ(netp);
    a.momentum = net.momentum;
    a.decay = net.decay;
    a.adam = net.adam;
    a.B1 = net.B1;
    a.B2 = net.B2;
    a.eps = net.eps;
    ++*net.t;
    a.t = (*net.t);

    ಗಾಗಿ(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        ಆದರೆ(l.update_gpu){
            l.update_gpu(l, a);
        }
    }
}

ಶೂನ್ಯ harmless_update_ಜಾಲ_gpu(ಜಾಲ *netp)
{
    ಜಾಲ net = *netp;
    cuda_ಗಣ_device(net.gpu_ಸೂಚ್ಯಂಕ);
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        ಆದರೆ(l.ತೂಕ_updates_gpu) fill_gpu(l.nweights, 0, l.ತೂಕ_updates_gpu, 1);
        ಆದರೆ(l.bias_updates_gpu) fill_gpu(l.nbiases, 0, l.bias_updates_gpu, 1);
        ಆದರೆ(l.scale_updates_gpu) fill_gpu(l.nbiases, 0, l.scale_updates_gpu, 1);
    }
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಜಾಲ *net;
    ದತ್ತಾಂಶ d;
    ದಶಮಾಂಶ *err;
} train_ವಾದಗಳು;

ಶೂನ್ಯ *train_thread(ಶೂನ್ಯ *ptr)
{
    train_ವಾದಗಳು ವಾದಗಳು = *(train_ವಾದಗಳು*)ptr;
    ಬಿಡುಗಡೆ(ptr);
    cuda_ಗಣ_device(ವಾದಗಳು.net->gpu_ಸೂಚ್ಯಂಕ);
    *ವಾದಗಳು.err = train_ಜಾಲ(ವಾದಗಳು.net, ವಾದಗಳು.d);
    ಹಿಂತಿರುಗಿ 0;
}

pthread_t train_ಜಾಲ_ಒಳ_thread(ಜಾಲ *net, ದತ್ತಾಂಶ d, ದಶಮಾಂಶ *err)
{
    pthread_t thread;
    train_ವಾದಗಳು *ptr = (train_ವಾದಗಳು *)ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(train_ವಾದಗಳು));
    ptr->net = net;
    ptr->d = d;
    ptr->err = err;
    ಆದರೆ(pthread_create(&thread, 0, train_thread, ptr)) error("Thread creation failed");
    ಹಿಂತಿರುಗಿ thread;
}

ಶೂನ್ಯ ವಿಲೀನ_weights(layer l, layer ತಳ)
{
    ಆದರೆ (l.type == CONVOLUTIONAL) {
        axpy_cpu(l.n, 1, l.bias_updates, 1, ತಳ.biases, 1);
        axpy_cpu(l.nweights, 1, l.ತೂಕ_updates, 1, ತಳ.weights, 1);
        ಆದರೆ (l.scales) {
            axpy_cpu(l.n, 1, l.scale_updates, 1, ತಳ.scales, 1);
        }
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(l.type == CONNECTED) {
        axpy_cpu(l.outputs, 1, l.bias_updates, 1, ತಳ.biases, 1);
        axpy_cpu(l.outputs*l.inputs, 1, l.ತೂಕ_updates, 1, ತಳ.weights, 1);
    }
}

ಶೂನ್ಯ scale_weights(layer l, ದಶಮಾಂಶ s)
{
    ಆದರೆ (l.type == CONVOLUTIONAL) {
        scal_cpu(l.n, s, l.biases, 1);
        scal_cpu(l.nweights, s, l.weights, 1);
        ಆದರೆ (l.scales) {
            scal_cpu(l.n, s, l.scales, 1);
        }
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(l.type == CONNECTED) {
        scal_cpu(l.outputs, s, l.biases, 1);
        scal_cpu(l.outputs*l.inputs, s, l.weights, 1);
    }
}


ಶೂನ್ಯ pull_weights(layer l)
{
    ಆದರೆ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.n);
        cuda_pull_array(l.weights_gpu, l.ತೂಕ_updates, l.nweights);
        ಆದರೆ(l.scales) cuda_pull_array(l.scales_gpu, l.scale_updates, l.n);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(l.type == CONNECTED){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.outputs);
        cuda_pull_array(l.weights_gpu, l.ತೂಕ_updates, l.outputs*l.inputs);
    }
}

ಶೂನ್ಯ ತಳ್ಳು_weights(layer l)
{
    ಆದರೆ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_ತಳ್ಳು_array(l.biases_gpu, l.biases, l.n);
        cuda_ತಳ್ಳು_array(l.weights_gpu, l.weights, l.nweights);
        ಆದರೆ(l.scales) cuda_ತಳ್ಳು_array(l.scales_gpu, l.scales, l.n);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(l.type == CONNECTED){
        cuda_ತಳ್ಳು_array(l.biases_gpu, l.biases, l.outputs);
        cuda_ತಳ್ಳು_array(l.weights_gpu, l.weights, l.outputs*l.inputs);
    }
}

ಶೂನ್ಯ distribute_weights(layer l, layer ತಳ)
{
    ಆದರೆ (l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL) {
        cuda_ತಳ್ಳು_array(l.biases_gpu, ತಳ.biases, l.n);
        cuda_ತಳ್ಳು_array(l.weights_gpu, ತಳ.weights, l.nweights);
        ಆದರೆ (ತಳ.scales) cuda_ತಳ್ಳು_array(l.scales_gpu, ತಳ.scales, l.n);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (l.type == CONNECTED) {
        cuda_ತಳ್ಳು_array(l.biases_gpu, ತಳ.biases, l.outputs);
        cuda_ತಳ್ಳು_array(l.weights_gpu, ತಳ.weights, l.outputs*l.inputs);
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

ಶೂನ್ಯ sync_layer(ಜಾಲ **nets, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ j)
{
    ಪೂರ್ಣಾಂಕ i;
    ಜಾಲ *net = nets[0];
    layer ತಳ = net->layers[j];
    scale_weights(ತಳ, 0);
    ಗಾಗಿ (i = 0; i < n; ++i) {
        cuda_ಗಣ_device(nets[i]->gpu_ಸೂಚ್ಯಂಕ);
        layer l = nets[i]->layers[j];
        pull_weights(l);
        ವಿಲೀನ_weights(l, ತಳ);
    }
    scale_weights(ತಳ, 1./n);
    ಗಾಗಿ (i = 0; i < n; ++i) {
        cuda_ಗಣ_device(nets[i]->gpu_ಸೂಚ್ಯಂಕ);
        layer l = nets[i]->layers[j];
        distribute_weights(l, ತಳ);
    }
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ{
    ಜಾಲ **nets;
    ಪೂರ್ಣಾಂಕ n;
    ಪೂರ್ಣಾಂಕ j;
} sync_ವಾದಗಳು;

ಶೂನ್ಯ *sync_layer_thread(ಶೂನ್ಯ *ptr)
{
    sync_ವಾದಗಳು ವಾದಗಳು = *(sync_ವಾದಗಳು*)ptr;
    sync_layer(ವಾದಗಳು.nets, ವಾದಗಳು.n, ವಾದಗಳು.j);
    ಬಿಡುಗಡೆ(ptr);
    ಹಿಂತಿರುಗಿ 0;
}

pthread_t sync_layer_ಒಳ_thread(ಜಾಲ **nets, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ j)
{
    pthread_t thread;
    sync_ವಾದಗಳು *ptr = (sync_ವಾದಗಳು *)ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(sync_ವಾದಗಳು));
    ptr->nets = nets;
    ptr->n = n;
    ptr->j = j;
    ಆದರೆ(pthread_create(&thread, 0, sync_layer_thread, ptr)) error("Thread creation failed");
    ಹಿಂತಿರುಗಿ thread;
}

ಶೂನ್ಯ sync_nets(ಜಾಲ **nets, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ interval)
{
    ಪೂರ್ಣಾಂಕ j;
    ಪೂರ್ಣಾಂಕ layers = nets[0]->n;
    pthread_t *ಎಳೆಗಳು = (pthread_t *) ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(layers, ಗಾತ್ರ(pthread_t));

    *(nets[0]->ನೋಡಿದ) += interval * (n-1) * nets[0]->batch * nets[0]->subdivisions;
    ಗಾಗಿ (j = 0; j < n; ++j){
        *(nets[j]->ನೋಡಿದ) = *(nets[0]->ನೋಡಿದ);
    }
    ಗಾಗಿ (j = 0; j < layers; ++j) {
        ಎಳೆಗಳು[j] = sync_layer_ಒಳ_thread(nets, n, j);
    }
    ಗಾಗಿ (j = 0; j < layers; ++j) {
        pthread_join(ಎಳೆಗಳು[j], 0);
    }
    ಬಿಡುಗಡೆ(ಎಳೆಗಳು);
}

ದಶಮಾಂಶ train_networks(ಜಾಲ **nets, ಪೂರ್ಣಾಂಕ n, ದತ್ತಾಂಶ d, ಪೂರ್ಣಾಂಕ interval)
{
    ಪೂರ್ಣಾಂಕ i;
    ಪೂರ್ಣಾಂಕ batch = nets[0]->batch;
    ಪೂರ್ಣಾಂಕ subdivisions = nets[0]->subdivisions;
    ಪ್ರತಿಜ್ಞಾ(batch * subdivisions * n == d.X.ಅಡ್ಡಸಾಲುಗಳು);
    pthread_t *ಎಳೆಗಳು = (pthread_t *) ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(pthread_t));
    ದಶಮಾಂಶ *errors = (ದಶಮಾಂಶ *) ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ದಶಮಾಂಶ));

    ದಶಮಾಂಶ ಮೊತ್ತ = 0;
    ಗಾಗಿ(i = 0; i < n; ++i){
        ದತ್ತಾಂಶ p = get_ದತ್ತಾಂಶ_part(d, i, n);
        ಎಳೆಗಳು[i] = train_ಜಾಲ_ಒಳ_thread(nets[i], p, errors + i);
    }
    ಗಾಗಿ(i = 0; i < n; ++i){
        pthread_join(ಎಳೆಗಳು[i], 0);
        //printf("%f\n", errors[i]);
        ಮೊತ್ತ += errors[i];
    }
    //cudaDeviceSynchronize();
    ಆದರೆ (get_current_batch(nets[0]) % interval == 0) {
        ಬರೆ("Syncing... ");
        fflush(stdout);
        sync_nets(nets, n, interval);
        ಬರೆ("Done!\n");
    }
    //cudaDeviceSynchronize();
    ಬಿಡುಗಡೆ(ಎಳೆಗಳು);
    ಬಿಡುಗಡೆ(errors);
    ಹಿಂತಿರುಗಿ (ದಶಮಾಂಶ)ಮೊತ್ತ/(n);
}

ಶೂನ್ಯ pull_ಜಾಲ_output(ಜಾಲ *net)
{
    layer l = get_ಜಾಲ_output_layer(net);
    cuda_pull_array(l.output_gpu, l.output, l.outputs*l.batch);
}

#ಆದರೆ_ಅಂತ್ಯ

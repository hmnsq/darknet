#ضمن <stdio.h>
#ضمن <time.h>
#ضمن <assert.h>
#ضمن "network.h"
#ضمن "image.h"
#ضمن "data.h"
#ضمن "utils.h"
#ضمن "blas.h"

#ضمن "crop_layer.h"
#ضمن "connected_layer.h"
#ضمن "gru_layer.h"
#ضمن "rnn_layer.h"
#ضمن "crnn_layer.h"
#ضمن "local_layer.h"
#ضمن "convolutional_layer.h"
#ضمن "activation_layer.h"
#ضمن "detection_layer.h"
#ضمن "region_layer.h"
#ضمن "yolo_layer.h"
#ضمن "normalization_layer.h"
#ضمن "batchnorm_layer.h"
#ضمن "maxpool_layer.h"
#ضمن "reorg_layer.h"
#ضمن "avgpool_layer.h"
#ضمن "cost_layer.h"
#ضمن "softmax_layer.h"
#ضمن "dropout_layer.h"
#ضمن "route_layer.h"
#ضمن "upsample_layer.h"
#ضمن "shortcut_layer.h"
#ضمن "parser.h"
#ضمن "data.h"

ڀريو_دليلون get_بنياد_دليلون(ڄار *net)
{
    ڀريو_دليلون دليلون = {0};
    دليلون.w = net->w;
    دليلون.h = net->h;
    دليلون.سائيز = net->w;

    دليلون.min = net->min_crop;
    دليلون.max = net->max_crop;
    دليلون.angle = net->angle;
    دليلون.aspect = net->aspect;
    دليلون.exposure = net->exposure;
    دليلون.center = net->center;
    دليلون.saturation = net->saturation;
    دليلون.hue = net->hue;
    أرجع دليلون;
}

ڄار *ڀريو_ڄار(حرف *cfg, حرف *weights, صحيح clear)
{
    ڄار *net = parse_ڄار_cfg(cfg);
    إذا(weights && weights[0] != 0){
        ڀريو_weights(net, weights);
    }
    إذا(clear) (*net->ڏٺل) = 0;
    أرجع net;
}

size_t get_current_batch(ڄار *net)
{
    size_t batch_انگ = (*net->ڏٺل)/(net->batch*net->subdivisions);
    أرجع batch_انگ;
}

فراغ reset_ڄار_state(ڄار *net, صحيح b)
{
    صحيح i;
    لكل (i = 0; i < net->n; ++i) {
        #جيڪڏهن_وصف_ٿيل GPU
        layer l = net->layers[i];
        إذا(l.state_gpu){
            fill_gpu(l.outputs, 0, l.state_gpu + l.outputs*b, 1);
        }
        إذا(l.h_gpu){
            fill_gpu(l.outputs, 0, l.h_gpu + l.outputs*b, 1);
        }
        #جيڪڏهن_پورو
    }
}

فراغ reset_rnn(ڄار *net)
{
    reset_ڄار_state(net, 0);
}

عائم get_current_شرح(ڄار *net)
{
    size_t batch_انگ = get_current_batch(net);
    صحيح i;
    عائم شرح;
    إذا (batch_انگ < net->burn_اندريون) أرجع net->learning_شرح * قوت((عائم)batch_انگ / net->burn_اندريون, net->power);
    بدل (net->policy) {
        حالة CONSTANT:
            أرجع net->learning_شرح;
        حالة قدم_2:
            أرجع net->learning_شرح * قوت(net->scale, batch_انگ/net->قدم);
        حالة قدمن_2:
            شرح = net->learning_شرح;
            لكل(i = 0; i < net->انگ_قدمن; ++i){
                إذا(net->قدمن[i] > batch_انگ) أرجع شرح;
                شرح *= net->scales[i];
            }
            أرجع شرح;
        حالة EXP:
            أرجع net->learning_شرح * قوت(net->gamma, batch_انگ);
        حالة POLY:
            أرجع net->learning_شرح * قوت(1 - (عائم)batch_انگ / net->max_batches, net->power);
        حالة RANDOM:
            أرجع net->learning_شرح * قوت(rand_هڪجهڙو(0,1), net->power);
        حالة SIG:
            أرجع net->learning_شرح * (1./(1.+قوت_نما(net->gamma*(batch_انگ - net->قدم))));
        افتراضي:
            fprintf(stderr, "Policy is weird!\n");
            أرجع net->learning_شرح;
    }
}

حرف *get_layer_string(LAYER_TYPE a)
{
    بدل(a){
        حالة CONVOLUTIONAL:
            أرجع "convolutional";
        حالة سرگرم:
            أرجع "activation";
        حالة LOCAL:
            أرجع "local";
        حالة DECONVOLUTIONAL:
            أرجع "deconvolutional";
        حالة CONNECTED:
            أرجع "connected";
        حالة RNN:
            أرجع "rnn";
        حالة GRU:
            أرجع "gru";
        حالة LSTM:
	    أرجع "lstm";
        حالة CRNN:
            أرجع "crnn";
        حالة MAXPOOL:
            أرجع "maxpool";
        حالة REORG:
            أرجع "reorg";
        حالة AVGPOOL:
            أرجع "avgpool";
        حالة SOFTMAX:
            أرجع "softmax";
        حالة DETECTION:
            أرجع "detection";
        حالة REGION:
            أرجع "region";
        حالة YOLO:
            أرجع "yolo";
        حالة DROPOUT:
            أرجع "dropout";
        حالة CROP:
            أرجع "crop";
        حالة لاڳت:
            أرجع "cost";
        حالة ROUTE:
            أرجع "route";
        حالة SHORTCUT:
            أرجع "shortcut";
        حالة NORMALIZATION:
            أرجع "normalization";
        حالة BATCHNORM:
            أرجع "batchnorm";
        افتراضي:
            اكسر;
    }
    أرجع "none";
}

ڄار *make_ڄار(صحيح n)
{
    ڄار *net = صفر_يادگيري_ڏيو(1, حجم(ڄار));
    net->n = n;
    net->layers = صفر_يادگيري_ڏيو(net->n, حجم(layer));
    net->ڏٺل = صفر_يادگيري_ڏيو(1, حجم(size_t));
    net->t    = صفر_يادگيري_ڏيو(1, حجم(صحيح));
    net->لاڳت_2 = صفر_يادگيري_ڏيو(1, حجم(عائم));
    أرجع net;
}

فراغ forward_ڄار(ڄار *netp)
{
#جيڪڏهن_وصف_ٿيل GPU
    إذا(netp->gpu_اشاريو >= 0){
        forward_ڄار_gpu(netp);   
        أرجع;
    }
#جيڪڏهن_پورو
    ڄار net = *netp;
    صحيح i;
    لكل(i = 0; i < net.n; ++i){
        net.اشاريو = i;
        layer l = net.layers[i];
        إذا(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.داخلا = l.output;
        إذا(l.truth) {
            net.truth = l.output;
        }
    }
    calc_ڄار_لاڳت(netp);
}

فراغ update_ڄار(ڄار *netp)
{
#جيڪڏهن_وصف_ٿيل GPU
    إذا(netp->gpu_اشاريو >= 0){
        update_ڄار_gpu(netp);   
        أرجع;
    }
#جيڪڏهن_پورو
    ڄار net = *netp;
    صحيح i;
    update_دليلون a = {0};
    a.batch = net.batch*net.subdivisions;
    a.learning_شرح = get_current_شرح(netp);
    a.momentum = net.momentum;
    a.decay = net.decay;
    a.adam = net.adam;
    a.B1 = net.B1;
    a.B2 = net.B2;
    a.eps = net.eps;
    ++*net.t;
    a.t = *net.t;

    لكل(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        إذا(l.update){
            l.update(l, a);
        }
    }
}

فراغ calc_ڄار_لاڳت(ڄار *netp)
{
    ڄار net = *netp;
    صحيح i;
    عائم جوڙ = 0;
    صحيح ڳڻپ = 0;
    لكل(i = 0; i < net.n; ++i){
        إذا(net.layers[i].لاڳت_2){
            جوڙ += net.layers[i].لاڳت_2[0];
            ++ڳڻپ;
        }
    }
    *net.لاڳت_2 = جوڙ/ڳڻپ;
}

صحيح get_predicted_class_ڄار(ڄار *net)
{
    أرجع max_اشاريو(net->output, net->outputs);
}

فراغ backward_ڄار(ڄار *netp)
{
#جيڪڏهن_وصف_ٿيل GPU
    إذا(netp->gpu_اشاريو >= 0){
        backward_ڄار_gpu(netp);   
        أرجع;
    }
#جيڪڏهن_پورو
    ڄار net = *netp;
    صحيح i;
    ڄار orig = net;
    لكل(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        إذا(l.stopbackward) اكسر;
        إذا(i == 0){
            net = orig;
        }وإلا{
            layer prev = net.layers[i-1];
            net.داخلا = prev.output;
            net.delta = prev.delta;
        }
        net.اشاريو = i;
        l.backward(l, net);
    }
}

عائم train_ڄار_datum(ڄار *net)
{
    *net->ڏٺل += net->batch;
    net->train = 1;
    forward_ڄار(net);
    backward_ڄار(net);
    عائم error = *net->لاڳت_2;
    إذا(((*net->ڏٺل)/net->batch)%net->subdivisions == 0) update_ڄار(net);
    أرجع error;
}

عائم train_ڄار_sgd(ڄار *net, معلومات d, صحيح n)
{
    صحيح batch = net->batch;

    صحيح i;
    عائم جوڙ = 0;
    لكل(i = 0; i < n; ++i){
        get_random_batch(d, batch, net->داخلا, net->truth);
        عائم err = train_ڄار_datum(net);
        جوڙ += err;
    }
    أرجع (عائم)جوڙ/(n*batch);
}

عائم train_ڄار(ڄار *net, معلومات d)
{
    أكد(d.X.صفون % net->batch == 0);
    صحيح batch = net->batch;
    صحيح n = d.X.صفون / batch;

    صحيح i;
    عائم جوڙ = 0;
    لكل(i = 0; i < n; ++i){
        get_ايندڙ_batch(d, batch, i*batch, net->داخلا, net->truth);
        عائم err = train_ڄار_datum(net);
        جوڙ += err;
    }
    أرجع (عائم)جوڙ/(n*batch);
}

فراغ سيٽ_عارضي_ڄار(ڄار *net, عائم t)
{
    صحيح i;
    لكل(i = 0; i < net->n; ++i){
        net->layers[i].temperature = t;
    }
}


فراغ سيٽ_batch_ڄار(ڄار *net, صحيح b)
{
    net->batch = b;
    صحيح i;
    لكل(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#جيڪڏهن_وصف_ٿيل CUDNN
        إذا(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_تياري(net->layers + i);
        }
        إذا(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnn_سيٽ_Tensor4d_Descriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, 1, l->ٻاهريون_c, l->ٻاهريون_h, l->ٻاهريون_w);
            cudnn_سيٽ_Tensor4d_Descriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, 1, l->ٻاهريون_c, 1, 1); 
        }
#جيڪڏهن_پورو
    }
}

صحيح resize_ڄار(ڄار *net, صحيح w, صحيح h)
{
#جيڪڏهن_وصف_ٿيل GPU
    cuda_سيٽ_device(net->gpu_اشاريو);
    cuda_free(net->workspace);
#جيڪڏهن_پورو
    صحيح i;
    //if(w == net->w && h == net->h) return 0;
    net->w = w;
    net->h = h;
    صحيح inputs = 0;
    size_t workspace_سائيز = 0;
    //fprintf(stderr, "Resizing to %d x %d...\n", w, h);
    //fflush(stderr);
    لكل (i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        إذا(l.type == CONVOLUTIONAL){
            resize_convolutional_layer(&l, w, h);
        }وإلا إذا(l.type == CROP){
            resize_crop_layer(&l, w, h);
        }وإلا إذا(l.type == MAXPOOL){
            resize_maxpool_layer(&l, w, h);
        }وإلا إذا(l.type == REGION){
            resize_region_layer(&l, w, h);
        }وإلا إذا(l.type == YOLO){
            resize_yolo_layer(&l, w, h);
        }وإلا إذا(l.type == ROUTE){
            resize_route_layer(&l, net);
        }وإلا إذا(l.type == SHORTCUT){
            resize_shortcut_layer(&l, w, h);
        }وإلا إذا(l.type == UPSAMPLE){
            resize_upsample_layer(&l, w, h);
        }وإلا إذا(l.type == REORG){
            resize_reorg_layer(&l, w, h);
        }وإلا إذا(l.type == AVGPOOL){
            resize_avgpool_layer(&l, w, h);
        }وإلا إذا(l.type == NORMALIZATION){
            resize_normalization_layer(&l, w, h);
        }وإلا إذا(l.type == لاڳت){
            resize_لاڳت_layer(&l, inputs);
        }وإلا{
            error("Cannot resize this type of layer");
        }
        إذا(l.workspace_سائيز > workspace_سائيز) workspace_سائيز = l.workspace_سائيز;
        إذا(l.workspace_سائيز > 2000000000) أكد(0);
        inputs = l.outputs;
        net->layers[i] = l;
        w = l.ٻاهريون_w;
        h = l.ٻاهريون_h;
        إذا(l.type == AVGPOOL) اكسر;
    }
    layer ٻاهريون = get_ڄار_output_layer(net);
    net->inputs = net->layers[0].inputs;
    net->outputs = ٻاهريون.outputs;
    net->truths = ٻاهريون.outputs;
    إذا(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = ٻاهريون.output;
    حرر(net->داخلا);
    حرر(net->truth);
    net->داخلا = صفر_يادگيري_ڏيو(net->inputs*net->batch, حجم(عائم));
    net->truth = صفر_يادگيري_ڏيو(net->truths*net->batch, حجم(عائم));
#جيڪڏهن_وصف_ٿيل GPU
    إذا(gpu_اشاريو >= 0){
        cuda_free(net->داخلا_gpu);
        cuda_free(net->truth_gpu);
        net->داخلا_gpu = cuda_make_array(net->داخلا, net->inputs*net->batch);
        net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
        إذا(workspace_سائيز){
            net->workspace = cuda_make_array(0, (workspace_سائيز-1)/حجم(عائم)+1);
        }
    }وإلا {
        حرر(net->workspace);
        net->workspace = صفر_يادگيري_ڏيو(1, workspace_سائيز);
    }
#وإلا
    حرر(net->workspace);
    net->workspace = صفر_يادگيري_ڏيو(1, workspace_سائيز);
#جيڪڏهن_پورو
    //fprintf(stderr, " Done!\n");
    أرجع 0;
}

layer get_ڄار_detection_layer(ڄار *net)
{
    صحيح i;
    لكل(i = 0; i < net->n; ++i){
        إذا(net->layers[i].type == DETECTION){
            أرجع net->layers[i];
        }
    }
    fprintf(stderr, "Detection layer not found!!\n");
    layer l = {0};
    أرجع l;
}

image get_ڄار_image_layer(ڄار *net, صحيح i)
{
    layer l = net->layers[i];
#جيڪڏهن_وصف_ٿيل GPU
    //cuda_pull_array(l.output_gpu, l.output, l.outputs);
#جيڪڏهن_پورو
    إذا (l.ٻاهريون_w && l.ٻاهريون_h && l.ٻاهريون_c){
        أرجع float_منزل_image(l.ٻاهريون_w, l.ٻاهريون_h, l.ٻاهريون_c, l.output);
    }
    image def = {0};
    أرجع def;
}

image get_ڄار_image(ڄار *net)
{
    صحيح i;
    لكل(i = net->n-1; i >= 0; --i){
        image m = get_ڄار_image_layer(net, i);
        إذا(m.h != 0) أرجع m;
    }
    image def = {0};
    أرجع def;
}

فراغ visualize_ڄار(ڄار *net)
{
    image *prev = 0;
    صحيح i;
    حرف buff[256];
    لكل(i = 0; i < net->n; ++i){
        sprintf(buff, "Layer %d", i);
        layer l = net->layers[i];
        إذا(l.type == CONVOLUTIONAL){
            prev = visualize_convolutional_layer(l, buff, prev);
        }
    } 
}

فراغ چوٽي_predictions(ڄار *net, صحيح k, صحيح *اشاريو)
{
    چوٽي_k(net->output, net->outputs, k, اشاريو);
}


عائم *ڄار_predict(ڄار *net, عائم *داخلا)
{
    ڄار orig = *net;
    net->داخلا = داخلا;
    net->truth = 0;
    net->train = 0;
    net->delta = 0;
    forward_ڄار(net);
    عائم *ٻاهريون = net->output;
    *net = orig;
    أرجع ٻاهريون;
}

صحيح انگ_detections(ڄار *net, عائم thresh)
{
    صحيح i;
    صحيح s = 0;
    لكل(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        إذا(l.type == YOLO){
            s += yolo_انگ_detections(l, thresh);
        }
        إذا(l.type == DETECTION || l.type == REGION){
            s += l.w*l.h*l.n;
        }
    }
    أرجع s;
}

detection *make_ڄار_boxes(ڄار *net, عائم thresh, صحيح *انگ)
{
    layer l = net->layers[net->n - 1];
    صحيح i;
    صحيح nboxes = انگ_detections(net, thresh);
    إذا(انگ) *انگ = nboxes;
    detection *dets = صفر_يادگيري_ڏيو(nboxes, حجم(detection));
    لكل(i = 0; i < nboxes; ++i){
        dets[i].prob = صفر_يادگيري_ڏيو(l.classes, حجم(عائم));
        إذا(l.coords > 4){
            dets[i].نقاب = صفر_يادگيري_ڏيو(l.coords-4, حجم(عائم));
        }
    }
    أرجع dets;
}

فراغ fill_ڄار_boxes(ڄار *net, صحيح w, صحيح h, عائم thresh, عائم hier, صحيح *map, صحيح relative, detection *dets)
{
    صحيح j;
    لكل(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        إذا(l.type == YOLO){
            صحيح ڳڻپ = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
            dets += ڳڻپ;
        }
        إذا(l.type == REGION){
            get_region_detections(l, w, h, net->w, net->h, thresh, map, hier, relative, dets);
            dets += l.w*l.h*l.n;
        }
        إذا(l.type == DETECTION){
            get_detection_detections(l, w, h, thresh, dets);
            dets += l.w*l.h*l.n;
        }
    }
}

detection *get_ڄار_boxes(ڄار *net, صحيح w, صحيح h, عائم thresh, عائم hier, صحيح *map, صحيح relative, صحيح *انگ)
{
    detection *dets = make_ڄار_boxes(net, thresh, انگ);
    fill_ڄار_boxes(net, w, h, thresh, hier, map, relative, dets);
    أرجع dets;
}

فراغ free_detections(detection *dets, صحيح n)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        حرر(dets[i].prob);
        إذا(dets[i].نقاب) حرر(dets[i].نقاب);
    }
    حرر(dets);
}

عائم *ڄار_predict_image(ڄار *net, image im)
{
    image imr = letterbox_image(im, net->w, net->h);
    سيٽ_batch_ڄار(net, 1);
    عائم *p = ڄار_predict(net, imr.معلومات);
    free_image(imr);
    أرجع p;
}

صحيح ڄار_ويڪر(ڄار *net){أرجع net->w;}
صحيح ڄار_اوچائي(ڄار *net){أرجع net->h;}

ميٽرڪس ڄار_predict_معلومات_multi(ڄار *net, معلومات test, صحيح n)
{
    صحيح i,j,b,m;
    صحيح k = net->outputs;
    ميٽرڪس pred = make_ميٽرڪس(test.X.صفون, k);
    عائم *X = صفر_يادگيري_ڏيو(net->batch*test.X.صفون, حجم(عائم));
    لكل(i = 0; i < test.X.صفون; i += net->batch){
        لكل(b = 0; b < net->batch; ++b){
            إذا(i+b == test.X.صفون) اكسر;
            يادگيري_نقل(X+b*test.X.کانن_ڳڻپ, test.X.vals[i+b], test.X.کانن_ڳڻپ*حجم(عائم));
        }
        لكل(m = 0; m < n; ++m){
            عائم *ٻاهريون = ڄار_predict(net, X);
            لكل(b = 0; b < net->batch; ++b){
                إذا(i+b == test.X.صفون) اكسر;
                لكل(j = 0; j < k; ++j){
                    pred.vals[i+b][j] += ٻاهريون[j+b*k]/n;
                }
            }
        }
    }
    حرر(X);
    أرجع pred;   
}

ميٽرڪس ڄار_predict_معلومات(ڄار *net, معلومات test)
{
    صحيح i,j,b;
    صحيح k = net->outputs;
    ميٽرڪس pred = make_ميٽرڪس(test.X.صفون, k);
    عائم *X = صفر_يادگيري_ڏيو(net->batch*test.X.کانن_ڳڻپ, حجم(عائم));
    لكل(i = 0; i < test.X.صفون; i += net->batch){
        لكل(b = 0; b < net->batch; ++b){
            إذا(i+b == test.X.صفون) اكسر;
            يادگيري_نقل(X+b*test.X.کانن_ڳڻپ, test.X.vals[i+b], test.X.کانن_ڳڻپ*حجم(عائم));
        }
        عائم *ٻاهريون = ڄار_predict(net, X);
        لكل(b = 0; b < net->batch; ++b){
            إذا(i+b == test.X.صفون) اكسر;
            لكل(j = 0; j < k; ++j){
                pred.vals[i+b][j] = ٻاهريون[j+b*k];
            }
        }
    }
    حرر(X);
    أرجع pred;   
}

فراغ print_ڄار(ڄار *net)
{
    صحيح i,j;
    لكل(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        عائم *output = l.output;
        صحيح n = l.outputs;
        عائم سراسري = سراسري_array(output, n);
        عائم vari = variance_array(output, n);
        fprintf(stderr, "Layer %d - Mean: %f, Variance: %f\n",i,سراسري, vari);
        إذا(n > 100) n = 100;
        لكل(j = 0; j < n; ++j) fprintf(stderr, "%f, ", output[j]);
        إذا(n == 100)fprintf(stderr,".....\n");
        fprintf(stderr, "\n");
    }
}

فراغ compare_networks(ڄار *n1, ڄار *n2, معلومات test)
{
    ميٽرڪس g1 = ڄار_predict_معلومات(n1, test);
    ميٽرڪس g2 = ڄار_predict_معلومات(n2, test);
    صحيح i;
    صحيح a,b,c,d;
    a = b = c = d = 0;
    لكل(i = 0; i < g1.صفون; ++i){
        صحيح truth = max_اشاريو(test.y.vals[i], test.y.کانن_ڳڻپ);
        صحيح p1 = max_اشاريو(g1.vals[i], g1.کانن_ڳڻپ);
        صحيح p2 = max_اشاريو(g2.vals[i], g2.کانن_ڳڻپ);
        إذا(p1 == truth){
            إذا(p2 == truth) ++d;
            وإلا ++c;
        }وإلا{
            إذا(p2 == truth) ++b;
            وإلا ++a;
        }
    }
    لکو("%5d %5d\n%5d %5d\n", a, b, c, d);
    عائم انگ = قوت((مطلق_قدر(b - c) - 1.), 2.);
    عائم den = b + c;
    لکو("%f\n", انگ/den); 
}

عائم ڄار_accuracy(ڄار *net, معلومات d)
{
    ميٽرڪس guess = ڄار_predict_معلومات(net, d);
    عائم acc = ميٽرڪس_topk_accuracy(d.y, guess,1);
    free_ميٽرڪس(guess);
    أرجع acc;
}

عائم *ڄار_accuracies(ڄار *net, معلومات d, صحيح n)
{
    ساكن عائم acc[2];
    ميٽرڪس guess = ڄار_predict_معلومات(net, d);
    acc[0] = ميٽرڪس_topk_accuracy(d.y, guess, 1);
    acc[1] = ميٽرڪس_topk_accuracy(d.y, guess, n);
    free_ميٽرڪس(guess);
    أرجع acc;
}

layer get_ڄار_output_layer(ڄار *net)
{
    صحيح i;
    لكل(i = net->n - 1; i >= 0; --i){
        إذا(net->layers[i].type != لاڳت) اكسر;
    }
    أرجع net->layers[i];
}

عائم ڄار_accuracy_multi(ڄار *net, معلومات d, صحيح n)
{
    ميٽرڪس guess = ڄار_predict_معلومات_multi(net, d, n);
    عائم acc = ميٽرڪس_topk_accuracy(d.y, guess,1);
    free_ميٽرڪس(guess);
    أرجع acc;
}

فراغ free_ڄار(ڄار *net)
{
    صحيح i;
    لكل(i = 0; i < net->n; ++i){
        free_layer(net->layers[i]);
    }
    حرر(net->layers);
    إذا(net->داخلا) حرر(net->داخلا);
    إذا(net->truth) حرر(net->truth);
#جيڪڏهن_وصف_ٿيل GPU
    إذا(net->داخلا_gpu) cuda_free(net->داخلا_gpu);
    إذا(net->truth_gpu) cuda_free(net->truth_gpu);
#جيڪڏهن_پورو
    حرر(net);
}

// Some day...
// ^ What the hell is this comment for?


layer ڄار_output_layer(ڄار *net)
{
    صحيح i;
    لكل(i = net->n - 1; i >= 0; --i){
        إذا(net->layers[i].type != لاڳت) اكسر;
    }
    أرجع net->layers[i];
}

صحيح ڄار_inputs(ڄار *net)
{
    أرجع net->layers[0].inputs;
}

صحيح ڄار_outputs(ڄار *net)
{
    أرجع ڄار_output_layer(net).outputs;
}

عائم *ڄار_output(ڄار *net)
{
    أرجع ڄار_output_layer(net).output;
}

#جيڪڏهن_وصف_ٿيل GPU

فراغ forward_ڄار_gpu(ڄار *netp)
{
    ڄار net = *netp;
    cuda_سيٽ_device(net.gpu_اشاريو);
    cuda_ڌڪو_array(net.داخلا_gpu, net.داخلا, net.inputs*net.batch);
    إذا(net.truth){
        cuda_ڌڪو_array(net.truth_gpu, net.truth, net.truths*net.batch);
    }

    صحيح i;
    لكل(i = 0; i < net.n; ++i){
        net.اشاريو = i;
        layer l = net.layers[i];
        إذا(l.delta_gpu){
            fill_gpu(l.outputs * l.batch, 0, l.delta_gpu, 1);
        }
        l.forward_gpu(l, net);
        net.داخلا_gpu = l.output_gpu;
        net.داخلا = l.output;
        إذا(l.truth) {
            net.truth_gpu = l.output_gpu;
            net.truth = l.output;
        }
    }
    pull_ڄار_output(netp);
    calc_ڄار_لاڳت(netp);
}

فراغ backward_ڄار_gpu(ڄار *netp)
{
    صحيح i;
    ڄار net = *netp;
    ڄار orig = net;
    cuda_سيٽ_device(net.gpu_اشاريو);
    لكل(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        إذا(l.stopbackward) اكسر;
        إذا(i == 0){
            net = orig;
        }وإلا{
            layer prev = net.layers[i-1];
            net.داخلا = prev.output;
            net.delta = prev.delta;
            net.داخلا_gpu = prev.output_gpu;
            net.delta_gpu = prev.delta_gpu;
        }
        net.اشاريو = i;
        l.backward_gpu(l, net);
    }
}

فراغ update_ڄار_gpu(ڄار *netp)
{
    ڄار net = *netp;
    cuda_سيٽ_device(net.gpu_اشاريو);
    صحيح i;
    update_دليلون a = {0};
    a.batch = net.batch*net.subdivisions;
    a.learning_شرح = get_current_شرح(netp);
    a.momentum = net.momentum;
    a.decay = net.decay;
    a.adam = net.adam;
    a.B1 = net.B1;
    a.B2 = net.B2;
    a.eps = net.eps;
    ++*net.t;
    a.t = (*net.t);

    لكل(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        إذا(l.update_gpu){
            l.update_gpu(l, a);
        }
    }
}

فراغ harmless_update_ڄار_gpu(ڄار *netp)
{
    ڄار net = *netp;
    cuda_سيٽ_device(net.gpu_اشاريو);
    صحيح i;
    لكل(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        إذا(l.وزن_updates_gpu) fill_gpu(l.nweights, 0, l.وزن_updates_gpu, 1);
        إذا(l.bias_updates_gpu) fill_gpu(l.nbiases, 0, l.bias_updates_gpu, 1);
        إذا(l.scale_updates_gpu) fill_gpu(l.nbiases, 0, l.scale_updates_gpu, 1);
    }
}

تعريفنوع بنية {
    ڄار *net;
    معلومات d;
    عائم *err;
} train_دليلون;

فراغ *train_thread(فراغ *ptr)
{
    train_دليلون دليلون = *(train_دليلون*)ptr;
    حرر(ptr);
    cuda_سيٽ_device(دليلون.net->gpu_اشاريو);
    *دليلون.err = train_ڄار(دليلون.net, دليلون.d);
    أرجع 0;
}

pthread_t train_ڄار_اندريون_thread(ڄار *net, معلومات d, عائم *err)
{
    pthread_t thread;
    train_دليلون *ptr = (train_دليلون *)صفر_يادگيري_ڏيو(1, حجم(train_دليلون));
    ptr->net = net;
    ptr->d = d;
    ptr->err = err;
    إذا(pthread_create(&thread, 0, train_thread, ptr)) error("Thread creation failed");
    أرجع thread;
}

فراغ ملايو_weights(layer l, layer بنياد)
{
    إذا (l.type == CONVOLUTIONAL) {
        axpy_cpu(l.n, 1, l.bias_updates, 1, بنياد.biases, 1);
        axpy_cpu(l.nweights, 1, l.وزن_updates, 1, بنياد.weights, 1);
        إذا (l.scales) {
            axpy_cpu(l.n, 1, l.scale_updates, 1, بنياد.scales, 1);
        }
    } وإلا إذا(l.type == CONNECTED) {
        axpy_cpu(l.outputs, 1, l.bias_updates, 1, بنياد.biases, 1);
        axpy_cpu(l.outputs*l.inputs, 1, l.وزن_updates, 1, بنياد.weights, 1);
    }
}

فراغ scale_weights(layer l, عائم s)
{
    إذا (l.type == CONVOLUTIONAL) {
        scal_cpu(l.n, s, l.biases, 1);
        scal_cpu(l.nweights, s, l.weights, 1);
        إذا (l.scales) {
            scal_cpu(l.n, s, l.scales, 1);
        }
    } وإلا إذا(l.type == CONNECTED) {
        scal_cpu(l.outputs, s, l.biases, 1);
        scal_cpu(l.outputs*l.inputs, s, l.weights, 1);
    }
}


فراغ pull_weights(layer l)
{
    إذا(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.n);
        cuda_pull_array(l.weights_gpu, l.وزن_updates, l.nweights);
        إذا(l.scales) cuda_pull_array(l.scales_gpu, l.scale_updates, l.n);
    } وإلا إذا(l.type == CONNECTED){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.outputs);
        cuda_pull_array(l.weights_gpu, l.وزن_updates, l.outputs*l.inputs);
    }
}

فراغ ڌڪو_weights(layer l)
{
    إذا(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_ڌڪو_array(l.biases_gpu, l.biases, l.n);
        cuda_ڌڪو_array(l.weights_gpu, l.weights, l.nweights);
        إذا(l.scales) cuda_ڌڪو_array(l.scales_gpu, l.scales, l.n);
    } وإلا إذا(l.type == CONNECTED){
        cuda_ڌڪو_array(l.biases_gpu, l.biases, l.outputs);
        cuda_ڌڪو_array(l.weights_gpu, l.weights, l.outputs*l.inputs);
    }
}

فراغ distribute_weights(layer l, layer بنياد)
{
    إذا (l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL) {
        cuda_ڌڪو_array(l.biases_gpu, بنياد.biases, l.n);
        cuda_ڌڪو_array(l.weights_gpu, بنياد.weights, l.nweights);
        إذا (بنياد.scales) cuda_ڌڪو_array(l.scales_gpu, بنياد.scales, l.n);
    } وإلا إذا (l.type == CONNECTED) {
        cuda_ڌڪو_array(l.biases_gpu, بنياد.biases, l.outputs);
        cuda_ڌڪو_array(l.weights_gpu, بنياد.weights, l.outputs*l.inputs);
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

فراغ sync_layer(ڄار **nets, صحيح n, صحيح j)
{
    صحيح i;
    ڄار *net = nets[0];
    layer بنياد = net->layers[j];
    scale_weights(بنياد, 0);
    لكل (i = 0; i < n; ++i) {
        cuda_سيٽ_device(nets[i]->gpu_اشاريو);
        layer l = nets[i]->layers[j];
        pull_weights(l);
        ملايو_weights(l, بنياد);
    }
    scale_weights(بنياد, 1./n);
    لكل (i = 0; i < n; ++i) {
        cuda_سيٽ_device(nets[i]->gpu_اشاريو);
        layer l = nets[i]->layers[j];
        distribute_weights(l, بنياد);
    }
}

تعريفنوع بنية{
    ڄار **nets;
    صحيح n;
    صحيح j;
} sync_دليلون;

فراغ *sync_layer_thread(فراغ *ptr)
{
    sync_دليلون دليلون = *(sync_دليلون*)ptr;
    sync_layer(دليلون.nets, دليلون.n, دليلون.j);
    حرر(ptr);
    أرجع 0;
}

pthread_t sync_layer_اندريون_thread(ڄار **nets, صحيح n, صحيح j)
{
    pthread_t thread;
    sync_دليلون *ptr = (sync_دليلون *)صفر_يادگيري_ڏيو(1, حجم(sync_دليلون));
    ptr->nets = nets;
    ptr->n = n;
    ptr->j = j;
    إذا(pthread_create(&thread, 0, sync_layer_thread, ptr)) error("Thread creation failed");
    أرجع thread;
}

فراغ sync_nets(ڄار **nets, صحيح n, صحيح interval)
{
    صحيح j;
    صحيح layers = nets[0]->n;
    pthread_t *ڌاڳا = (pthread_t *) صفر_يادگيري_ڏيو(layers, حجم(pthread_t));

    *(nets[0]->ڏٺل) += interval * (n-1) * nets[0]->batch * nets[0]->subdivisions;
    لكل (j = 0; j < n; ++j){
        *(nets[j]->ڏٺل) = *(nets[0]->ڏٺل);
    }
    لكل (j = 0; j < layers; ++j) {
        ڌاڳا[j] = sync_layer_اندريون_thread(nets, n, j);
    }
    لكل (j = 0; j < layers; ++j) {
        pthread_join(ڌاڳا[j], 0);
    }
    حرر(ڌاڳا);
}

عائم train_networks(ڄار **nets, صحيح n, معلومات d, صحيح interval)
{
    صحيح i;
    صحيح batch = nets[0]->batch;
    صحيح subdivisions = nets[0]->subdivisions;
    أكد(batch * subdivisions * n == d.X.صفون);
    pthread_t *ڌاڳا = (pthread_t *) صفر_يادگيري_ڏيو(n, حجم(pthread_t));
    عائم *errors = (عائم *) صفر_يادگيري_ڏيو(n, حجم(عائم));

    عائم جوڙ = 0;
    لكل(i = 0; i < n; ++i){
        معلومات p = get_معلومات_part(d, i, n);
        ڌاڳا[i] = train_ڄار_اندريون_thread(nets[i], p, errors + i);
    }
    لكل(i = 0; i < n; ++i){
        pthread_join(ڌاڳا[i], 0);
        //printf("%f\n", errors[i]);
        جوڙ += errors[i];
    }
    //cudaDeviceSynchronize();
    إذا (get_current_batch(nets[0]) % interval == 0) {
        لکو("Syncing... ");
        fflush(stdout);
        sync_nets(nets, n, interval);
        لکو("Done!\n");
    }
    //cudaDeviceSynchronize();
    حرر(ڌاڳا);
    حرر(errors);
    أرجع (عائم)جوڙ/(n);
}

فراغ pull_ڄار_output(ڄار *net)
{
    layer l = get_ڄار_output_layer(net);
    cuda_pull_array(l.output_gpu, l.output, l.outputs*l.batch);
}

#جيڪڏهن_پورو

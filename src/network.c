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

بارگیری_آرگومانها get_پایه_آرگومانها(شبکه *net)
{
    بارگیری_آرگومانها آرگومانها = {0};
    آرگومانها.w = net->w;
    آرگومانها.h = net->h;
    آرگومانها.اندازه = net->w;

    آرگومانها.min = net->min_crop;
    آرگومانها.max = net->max_crop;
    آرگومانها.angle = net->angle;
    آرگومانها.aspect = net->aspect;
    آرگومانها.exposure = net->exposure;
    آرگومانها.center = net->center;
    آرگومانها.saturation = net->saturation;
    آرگومانها.hue = net->hue;
    أرجع آرگومانها;
}

شبکه *بارگیری_شبکه(حرف *cfg, حرف *weights, صحيح clear)
{
    شبکه *net = parse_شبکه_cfg(cfg);
    إذا(weights && weights[0] != 0){
        بارگیری_weights(net, weights);
    }
    إذا(clear) (*net->دیده) = 0;
    أرجع net;
}

size_t get_current_batch(شبکه *net)
{
    size_t batch_رقم = (*net->دیده)/(net->batch*net->subdivisions);
    أرجع batch_رقم;
}

فراغ reset_شبکه_state(شبکه *net, صحيح b)
{
    صحيح i;
    لكل (i = 0; i < net->n; ++i) {
        #اگر_تعریف_شده GPU
        layer l = net->layers[i];
        إذا(l.state_gpu){
            fill_gpu(l.outputs, 0, l.state_gpu + l.outputs*b, 1);
        }
        إذا(l.h_gpu){
            fill_gpu(l.outputs, 0, l.h_gpu + l.outputs*b, 1);
        }
        #پایان_اگر
    }
}

فراغ reset_rnn(شبکه *net)
{
    reset_شبکه_state(net, 0);
}

عائم get_current_نرخ(شبکه *net)
{
    size_t batch_رقم = get_current_batch(net);
    صحيح i;
    عائم نرخ;
    إذا (batch_رقم < net->burn_درون) أرجع net->learning_نرخ * توان((عائم)batch_رقم / net->burn_درون, net->power);
    بدل (net->policy) {
        حالة CONSTANT:
            أرجع net->learning_نرخ;
        حالة گام_2:
            أرجع net->learning_نرخ * توان(net->scale, batch_رقم/net->گام);
        حالة گامها_2:
            نرخ = net->learning_نرخ;
            لكل(i = 0; i < net->رقم_گامها; ++i){
                إذا(net->گامها[i] > batch_رقم) أرجع نرخ;
                نرخ *= net->scales[i];
            }
            أرجع نرخ;
        حالة EXP:
            أرجع net->learning_نرخ * توان(net->gamma, batch_رقم);
        حالة POLY:
            أرجع net->learning_نرخ * توان(1 - (عائم)batch_رقم / net->max_batches, net->power);
        حالة RANDOM:
            أرجع net->learning_نرخ * توان(rand_یکنواخت(0,1), net->power);
        حالة SIG:
            أرجع net->learning_نرخ * (1./(1.+نمایی_تابع(net->gamma*(batch_رقم - net->گام))));
        افتراضي:
            fprintf(stderr, "Policy is weird!\n");
            أرجع net->learning_نرخ;
    }
}

حرف *get_layer_string(LAYER_TYPE a)
{
    بدل(a){
        حالة CONVOLUTIONAL:
            أرجع "convolutional";
        حالة فعال:
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
        حالة هزینه:
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

شبکه *make_شبکه(صحيح n)
{
    شبکه *net = حافظه_صفر_بده(1, حجم(شبکه));
    net->n = n;
    net->layers = حافظه_صفر_بده(net->n, حجم(layer));
    net->دیده = حافظه_صفر_بده(1, حجم(size_t));
    net->t    = حافظه_صفر_بده(1, حجم(صحيح));
    net->هزینه_2 = حافظه_صفر_بده(1, حجم(عائم));
    أرجع net;
}

فراغ forward_شبکه(شبکه *netp)
{
#اگر_تعریف_شده GPU
    إذا(netp->gpu_شاخص >= 0){
        forward_شبکه_gpu(netp);   
        أرجع;
    }
#پایان_اگر
    شبکه net = *netp;
    صحيح i;
    لكل(i = 0; i < net.n; ++i){
        net.شاخص = i;
        layer l = net.layers[i];
        إذا(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.درون_داد = l.output;
        إذا(l.truth) {
            net.truth = l.output;
        }
    }
    calc_شبکه_هزینه(netp);
}

فراغ update_شبکه(شبکه *netp)
{
#اگر_تعریف_شده GPU
    إذا(netp->gpu_شاخص >= 0){
        update_شبکه_gpu(netp);   
        أرجع;
    }
#پایان_اگر
    شبکه net = *netp;
    صحيح i;
    update_آرگومانها a = {0};
    a.batch = net.batch*net.subdivisions;
    a.learning_نرخ = get_current_نرخ(netp);
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

فراغ calc_شبکه_هزینه(شبکه *netp)
{
    شبکه net = *netp;
    صحيح i;
    عائم جمع = 0;
    صحيح شمار = 0;
    لكل(i = 0; i < net.n; ++i){
        إذا(net.layers[i].هزینه_2){
            جمع += net.layers[i].هزینه_2[0];
            ++شمار;
        }
    }
    *net.هزینه_2 = جمع/شمار;
}

صحيح get_predicted_class_شبکه(شبکه *net)
{
    أرجع max_شاخص(net->output, net->outputs);
}

فراغ backward_شبکه(شبکه *netp)
{
#اگر_تعریف_شده GPU
    إذا(netp->gpu_شاخص >= 0){
        backward_شبکه_gpu(netp);   
        أرجع;
    }
#پایان_اگر
    شبکه net = *netp;
    صحيح i;
    شبکه orig = net;
    لكل(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        إذا(l.stopbackward) اكسر;
        إذا(i == 0){
            net = orig;
        }وإلا{
            layer prev = net.layers[i-1];
            net.درون_داد = prev.output;
            net.delta = prev.delta;
        }
        net.شاخص = i;
        l.backward(l, net);
    }
}

عائم train_شبکه_datum(شبکه *net)
{
    *net->دیده += net->batch;
    net->train = 1;
    forward_شبکه(net);
    backward_شبکه(net);
    عائم error = *net->هزینه_2;
    إذا(((*net->دیده)/net->batch)%net->subdivisions == 0) update_شبکه(net);
    أرجع error;
}

عائم train_شبکه_sgd(شبکه *net, داده d, صحيح n)
{
    صحيح batch = net->batch;

    صحيح i;
    عائم جمع = 0;
    لكل(i = 0; i < n; ++i){
        get_random_batch(d, batch, net->درون_داد, net->truth);
        عائم err = train_شبکه_datum(net);
        جمع += err;
    }
    أرجع (عائم)جمع/(n*batch);
}

عائم train_شبکه(شبکه *net, داده d)
{
    أكد(d.X.ردیفها % net->batch == 0);
    صحيح batch = net->batch;
    صحيح n = d.X.ردیفها / batch;

    صحيح i;
    عائم جمع = 0;
    لكل(i = 0; i < n; ++i){
        get_بعدی_batch(d, batch, i*batch, net->درون_داد, net->truth);
        عائم err = train_شبکه_datum(net);
        جمع += err;
    }
    أرجع (عائم)جمع/(n*batch);
}

فراغ مجموعه_موقت_شبکه(شبکه *net, عائم t)
{
    صحيح i;
    لكل(i = 0; i < net->n; ++i){
        net->layers[i].temperature = t;
    }
}


فراغ مجموعه_batch_شبکه(شبکه *net, صحيح b)
{
    net->batch = b;
    صحيح i;
    لكل(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#اگر_تعریف_شده CUDNN
        إذا(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_راهاندازی(net->layers + i);
        }
        إذا(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnn_مجموعه_Tensor4d_Descriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_داده_FLOAT, 1, l->بیرون_c, l->بیرون_h, l->بیرون_w);
            cudnn_مجموعه_Tensor4d_Descriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_داده_FLOAT, 1, l->بیرون_c, 1, 1); 
        }
#پایان_اگر
    }
}

صحيح resize_شبکه(شبکه *net, صحيح w, صحيح h)
{
#اگر_تعریف_شده GPU
    cuda_مجموعه_device(net->gpu_شاخص);
    cuda_free(net->workspace);
#پایان_اگر
    صحيح i;
    //if(w == net->w && h == net->h) return 0;
    net->w = w;
    net->h = h;
    صحيح inputs = 0;
    size_t workspace_اندازه = 0;
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
        }وإلا إذا(l.type == هزینه){
            resize_هزینه_layer(&l, inputs);
        }وإلا{
            error("Cannot resize this type of layer");
        }
        إذا(l.workspace_اندازه > workspace_اندازه) workspace_اندازه = l.workspace_اندازه;
        إذا(l.workspace_اندازه > 2000000000) أكد(0);
        inputs = l.outputs;
        net->layers[i] = l;
        w = l.بیرون_w;
        h = l.بیرون_h;
        إذا(l.type == AVGPOOL) اكسر;
    }
    layer بیرون = get_شبکه_output_layer(net);
    net->inputs = net->layers[0].inputs;
    net->outputs = بیرون.outputs;
    net->truths = بیرون.outputs;
    إذا(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = بیرون.output;
    حرر(net->درون_داد);
    حرر(net->truth);
    net->درون_داد = حافظه_صفر_بده(net->inputs*net->batch, حجم(عائم));
    net->truth = حافظه_صفر_بده(net->truths*net->batch, حجم(عائم));
#اگر_تعریف_شده GPU
    إذا(gpu_شاخص >= 0){
        cuda_free(net->درون_داد_gpu);
        cuda_free(net->truth_gpu);
        net->درون_داد_gpu = cuda_make_array(net->درون_داد, net->inputs*net->batch);
        net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
        إذا(workspace_اندازه){
            net->workspace = cuda_make_array(0, (workspace_اندازه-1)/حجم(عائم)+1);
        }
    }وإلا {
        حرر(net->workspace);
        net->workspace = حافظه_صفر_بده(1, workspace_اندازه);
    }
#وإلا
    حرر(net->workspace);
    net->workspace = حافظه_صفر_بده(1, workspace_اندازه);
#پایان_اگر
    //fprintf(stderr, " Done!\n");
    أرجع 0;
}

layer get_شبکه_detection_layer(شبکه *net)
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

image get_شبکه_image_layer(شبکه *net, صحيح i)
{
    layer l = net->layers[i];
#اگر_تعریف_شده GPU
    //cuda_pull_array(l.output_gpu, l.output, l.outputs);
#پایان_اگر
    إذا (l.بیرون_w && l.بیرون_h && l.بیرون_c){
        أرجع float_مقصد_image(l.بیرون_w, l.بیرون_h, l.بیرون_c, l.output);
    }
    image def = {0};
    أرجع def;
}

image get_شبکه_image(شبکه *net)
{
    صحيح i;
    لكل(i = net->n-1; i >= 0; --i){
        image m = get_شبکه_image_layer(net, i);
        إذا(m.h != 0) أرجع m;
    }
    image def = {0};
    أرجع def;
}

فراغ visualize_شبکه(شبکه *net)
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

فراغ بالاترین_predictions(شبکه *net, صحيح k, صحيح *شاخص)
{
    بالاترین_k(net->output, net->outputs, k, شاخص);
}


عائم *شبکه_predict(شبکه *net, عائم *درون_داد)
{
    شبکه orig = *net;
    net->درون_داد = درون_داد;
    net->truth = 0;
    net->train = 0;
    net->delta = 0;
    forward_شبکه(net);
    عائم *بیرون = net->output;
    *net = orig;
    أرجع بیرون;
}

صحيح رقم_detections(شبکه *net, عائم thresh)
{
    صحيح i;
    صحيح s = 0;
    لكل(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        إذا(l.type == YOLO){
            s += yolo_رقم_detections(l, thresh);
        }
        إذا(l.type == DETECTION || l.type == REGION){
            s += l.w*l.h*l.n;
        }
    }
    أرجع s;
}

detection *make_شبکه_boxes(شبکه *net, عائم thresh, صحيح *رقم)
{
    layer l = net->layers[net->n - 1];
    صحيح i;
    صحيح nboxes = رقم_detections(net, thresh);
    إذا(رقم) *رقم = nboxes;
    detection *dets = حافظه_صفر_بده(nboxes, حجم(detection));
    لكل(i = 0; i < nboxes; ++i){
        dets[i].prob = حافظه_صفر_بده(l.classes, حجم(عائم));
        إذا(l.coords > 4){
            dets[i].ماسک = حافظه_صفر_بده(l.coords-4, حجم(عائم));
        }
    }
    أرجع dets;
}

فراغ fill_شبکه_boxes(شبکه *net, صحيح w, صحيح h, عائم thresh, عائم hier, صحيح *map, صحيح relative, detection *dets)
{
    صحيح j;
    لكل(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        إذا(l.type == YOLO){
            صحيح شمار = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
            dets += شمار;
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

detection *get_شبکه_boxes(شبکه *net, صحيح w, صحيح h, عائم thresh, عائم hier, صحيح *map, صحيح relative, صحيح *رقم)
{
    detection *dets = make_شبکه_boxes(net, thresh, رقم);
    fill_شبکه_boxes(net, w, h, thresh, hier, map, relative, dets);
    أرجع dets;
}

فراغ free_detections(detection *dets, صحيح n)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        حرر(dets[i].prob);
        إذا(dets[i].ماسک) حرر(dets[i].ماسک);
    }
    حرر(dets);
}

عائم *شبکه_predict_image(شبکه *net, image im)
{
    image imr = letterbox_image(im, net->w, net->h);
    مجموعه_batch_شبکه(net, 1);
    عائم *p = شبکه_predict(net, imr.داده);
    free_image(imr);
    أرجع p;
}

صحيح شبکه_عرض(شبکه *net){أرجع net->w;}
صحيح شبکه_ارتفاع(شبکه *net){أرجع net->h;}

ماتریس شبکه_predict_داده_multi(شبکه *net, داده test, صحيح n)
{
    صحيح i,j,b,m;
    صحيح k = net->outputs;
    ماتریس pred = make_ماتریس(test.X.ردیفها, k);
    عائم *X = حافظه_صفر_بده(net->batch*test.X.ردیفها, حجم(عائم));
    لكل(i = 0; i < test.X.ردیفها; i += net->batch){
        لكل(b = 0; b < net->batch; ++b){
            إذا(i+b == test.X.ردیفها) اكسر;
            کپی_حافظه(X+b*test.X.تعداد_ستون, test.X.vals[i+b], test.X.تعداد_ستون*حجم(عائم));
        }
        لكل(m = 0; m < n; ++m){
            عائم *بیرون = شبکه_predict(net, X);
            لكل(b = 0; b < net->batch; ++b){
                إذا(i+b == test.X.ردیفها) اكسر;
                لكل(j = 0; j < k; ++j){
                    pred.vals[i+b][j] += بیرون[j+b*k]/n;
                }
            }
        }
    }
    حرر(X);
    أرجع pred;   
}

ماتریس شبکه_predict_داده(شبکه *net, داده test)
{
    صحيح i,j,b;
    صحيح k = net->outputs;
    ماتریس pred = make_ماتریس(test.X.ردیفها, k);
    عائم *X = حافظه_صفر_بده(net->batch*test.X.تعداد_ستون, حجم(عائم));
    لكل(i = 0; i < test.X.ردیفها; i += net->batch){
        لكل(b = 0; b < net->batch; ++b){
            إذا(i+b == test.X.ردیفها) اكسر;
            کپی_حافظه(X+b*test.X.تعداد_ستون, test.X.vals[i+b], test.X.تعداد_ستون*حجم(عائم));
        }
        عائم *بیرون = شبکه_predict(net, X);
        لكل(b = 0; b < net->batch; ++b){
            إذا(i+b == test.X.ردیفها) اكسر;
            لكل(j = 0; j < k; ++j){
                pred.vals[i+b][j] = بیرون[j+b*k];
            }
        }
    }
    حرر(X);
    أرجع pred;   
}

فراغ print_شبکه(شبکه *net)
{
    صحيح i,j;
    لكل(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        عائم *output = l.output;
        صحيح n = l.outputs;
        عائم میانگین = میانگین_array(output, n);
        عائم vari = variance_array(output, n);
        fprintf(stderr, "Layer %d - Mean: %f, Variance: %f\n",i,میانگین, vari);
        إذا(n > 100) n = 100;
        لكل(j = 0; j < n; ++j) fprintf(stderr, "%f, ", output[j]);
        إذا(n == 100)fprintf(stderr,".....\n");
        fprintf(stderr, "\n");
    }
}

فراغ compare_networks(شبکه *n1, شبکه *n2, داده test)
{
    ماتریس g1 = شبکه_predict_داده(n1, test);
    ماتریس g2 = شبکه_predict_داده(n2, test);
    صحيح i;
    صحيح a,b,c,d;
    a = b = c = d = 0;
    لكل(i = 0; i < g1.ردیفها; ++i){
        صحيح truth = max_شاخص(test.y.vals[i], test.y.تعداد_ستون);
        صحيح p1 = max_شاخص(g1.vals[i], g1.تعداد_ستون);
        صحيح p2 = max_شاخص(g2.vals[i], g2.تعداد_ستون);
        إذا(p1 == truth){
            إذا(p2 == truth) ++d;
            وإلا ++c;
        }وإلا{
            إذا(p2 == truth) ++b;
            وإلا ++a;
        }
    }
    چاپ_کن("%5d %5d\n%5d %5d\n", a, b, c, d);
    عائم رقم = توان((قدر_مطلق(b - c) - 1.), 2.);
    عائم den = b + c;
    چاپ_کن("%f\n", رقم/den); 
}

عائم شبکه_accuracy(شبکه *net, داده d)
{
    ماتریس guess = شبکه_predict_داده(net, d);
    عائم acc = ماتریس_topk_accuracy(d.y, guess,1);
    free_ماتریس(guess);
    أرجع acc;
}

عائم *شبکه_accuracies(شبکه *net, داده d, صحيح n)
{
    ساكن عائم acc[2];
    ماتریس guess = شبکه_predict_داده(net, d);
    acc[0] = ماتریس_topk_accuracy(d.y, guess, 1);
    acc[1] = ماتریس_topk_accuracy(d.y, guess, n);
    free_ماتریس(guess);
    أرجع acc;
}

layer get_شبکه_output_layer(شبکه *net)
{
    صحيح i;
    لكل(i = net->n - 1; i >= 0; --i){
        إذا(net->layers[i].type != هزینه) اكسر;
    }
    أرجع net->layers[i];
}

عائم شبکه_accuracy_multi(شبکه *net, داده d, صحيح n)
{
    ماتریس guess = شبکه_predict_داده_multi(net, d, n);
    عائم acc = ماتریس_topk_accuracy(d.y, guess,1);
    free_ماتریس(guess);
    أرجع acc;
}

فراغ free_شبکه(شبکه *net)
{
    صحيح i;
    لكل(i = 0; i < net->n; ++i){
        free_layer(net->layers[i]);
    }
    حرر(net->layers);
    إذا(net->درون_داد) حرر(net->درون_داد);
    إذا(net->truth) حرر(net->truth);
#اگر_تعریف_شده GPU
    إذا(net->درون_داد_gpu) cuda_free(net->درون_داد_gpu);
    إذا(net->truth_gpu) cuda_free(net->truth_gpu);
#پایان_اگر
    حرر(net);
}

// Some day...
// ^ What the hell is this comment for?


layer شبکه_output_layer(شبکه *net)
{
    صحيح i;
    لكل(i = net->n - 1; i >= 0; --i){
        إذا(net->layers[i].type != هزینه) اكسر;
    }
    أرجع net->layers[i];
}

صحيح شبکه_inputs(شبکه *net)
{
    أرجع net->layers[0].inputs;
}

صحيح شبکه_outputs(شبکه *net)
{
    أرجع شبکه_output_layer(net).outputs;
}

عائم *شبکه_output(شبکه *net)
{
    أرجع شبکه_output_layer(net).output;
}

#اگر_تعریف_شده GPU

فراغ forward_شبکه_gpu(شبکه *netp)
{
    شبکه net = *netp;
    cuda_مجموعه_device(net.gpu_شاخص);
    cuda_افزودن_array(net.درون_داد_gpu, net.درون_داد, net.inputs*net.batch);
    إذا(net.truth){
        cuda_افزودن_array(net.truth_gpu, net.truth, net.truths*net.batch);
    }

    صحيح i;
    لكل(i = 0; i < net.n; ++i){
        net.شاخص = i;
        layer l = net.layers[i];
        إذا(l.delta_gpu){
            fill_gpu(l.outputs * l.batch, 0, l.delta_gpu, 1);
        }
        l.forward_gpu(l, net);
        net.درون_داد_gpu = l.output_gpu;
        net.درون_داد = l.output;
        إذا(l.truth) {
            net.truth_gpu = l.output_gpu;
            net.truth = l.output;
        }
    }
    pull_شبکه_output(netp);
    calc_شبکه_هزینه(netp);
}

فراغ backward_شبکه_gpu(شبکه *netp)
{
    صحيح i;
    شبکه net = *netp;
    شبکه orig = net;
    cuda_مجموعه_device(net.gpu_شاخص);
    لكل(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        إذا(l.stopbackward) اكسر;
        إذا(i == 0){
            net = orig;
        }وإلا{
            layer prev = net.layers[i-1];
            net.درون_داد = prev.output;
            net.delta = prev.delta;
            net.درون_داد_gpu = prev.output_gpu;
            net.delta_gpu = prev.delta_gpu;
        }
        net.شاخص = i;
        l.backward_gpu(l, net);
    }
}

فراغ update_شبکه_gpu(شبکه *netp)
{
    شبکه net = *netp;
    cuda_مجموعه_device(net.gpu_شاخص);
    صحيح i;
    update_آرگومانها a = {0};
    a.batch = net.batch*net.subdivisions;
    a.learning_نرخ = get_current_نرخ(netp);
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

فراغ harmless_update_شبکه_gpu(شبکه *netp)
{
    شبکه net = *netp;
    cuda_مجموعه_device(net.gpu_شاخص);
    صحيح i;
    لكل(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        إذا(l.وزن_updates_gpu) fill_gpu(l.nweights, 0, l.وزن_updates_gpu, 1);
        إذا(l.bias_updates_gpu) fill_gpu(l.nbiases, 0, l.bias_updates_gpu, 1);
        إذا(l.scale_updates_gpu) fill_gpu(l.nbiases, 0, l.scale_updates_gpu, 1);
    }
}

تعريفنوع بنية {
    شبکه *net;
    داده d;
    عائم *err;
} train_آرگومانها;

فراغ *train_thread(فراغ *ptr)
{
    train_آرگومانها آرگومانها = *(train_آرگومانها*)ptr;
    حرر(ptr);
    cuda_مجموعه_device(آرگومانها.net->gpu_شاخص);
    *آرگومانها.err = train_شبکه(آرگومانها.net, آرگومانها.d);
    أرجع 0;
}

pthread_t train_شبکه_درون_thread(شبکه *net, داده d, عائم *err)
{
    pthread_t thread;
    train_آرگومانها *ptr = (train_آرگومانها *)حافظه_صفر_بده(1, حجم(train_آرگومانها));
    ptr->net = net;
    ptr->d = d;
    ptr->err = err;
    إذا(pthread_create(&thread, 0, train_thread, ptr)) error("Thread creation failed");
    أرجع thread;
}

فراغ ادغام_weights(layer l, layer پایه)
{
    إذا (l.type == CONVOLUTIONAL) {
        axpy_cpu(l.n, 1, l.bias_updates, 1, پایه.biases, 1);
        axpy_cpu(l.nweights, 1, l.وزن_updates, 1, پایه.weights, 1);
        إذا (l.scales) {
            axpy_cpu(l.n, 1, l.scale_updates, 1, پایه.scales, 1);
        }
    } وإلا إذا(l.type == CONNECTED) {
        axpy_cpu(l.outputs, 1, l.bias_updates, 1, پایه.biases, 1);
        axpy_cpu(l.outputs*l.inputs, 1, l.وزن_updates, 1, پایه.weights, 1);
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

فراغ افزودن_weights(layer l)
{
    إذا(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_افزودن_array(l.biases_gpu, l.biases, l.n);
        cuda_افزودن_array(l.weights_gpu, l.weights, l.nweights);
        إذا(l.scales) cuda_افزودن_array(l.scales_gpu, l.scales, l.n);
    } وإلا إذا(l.type == CONNECTED){
        cuda_افزودن_array(l.biases_gpu, l.biases, l.outputs);
        cuda_افزودن_array(l.weights_gpu, l.weights, l.outputs*l.inputs);
    }
}

فراغ distribute_weights(layer l, layer پایه)
{
    إذا (l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL) {
        cuda_افزودن_array(l.biases_gpu, پایه.biases, l.n);
        cuda_افزودن_array(l.weights_gpu, پایه.weights, l.nweights);
        إذا (پایه.scales) cuda_افزودن_array(l.scales_gpu, پایه.scales, l.n);
    } وإلا إذا (l.type == CONNECTED) {
        cuda_افزودن_array(l.biases_gpu, پایه.biases, l.outputs);
        cuda_افزودن_array(l.weights_gpu, پایه.weights, l.outputs*l.inputs);
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

فراغ sync_layer(شبکه **nets, صحيح n, صحيح j)
{
    صحيح i;
    شبکه *net = nets[0];
    layer پایه = net->layers[j];
    scale_weights(پایه, 0);
    لكل (i = 0; i < n; ++i) {
        cuda_مجموعه_device(nets[i]->gpu_شاخص);
        layer l = nets[i]->layers[j];
        pull_weights(l);
        ادغام_weights(l, پایه);
    }
    scale_weights(پایه, 1./n);
    لكل (i = 0; i < n; ++i) {
        cuda_مجموعه_device(nets[i]->gpu_شاخص);
        layer l = nets[i]->layers[j];
        distribute_weights(l, پایه);
    }
}

تعريفنوع بنية{
    شبکه **nets;
    صحيح n;
    صحيح j;
} sync_آرگومانها;

فراغ *sync_layer_thread(فراغ *ptr)
{
    sync_آرگومانها آرگومانها = *(sync_آرگومانها*)ptr;
    sync_layer(آرگومانها.nets, آرگومانها.n, آرگومانها.j);
    حرر(ptr);
    أرجع 0;
}

pthread_t sync_layer_درون_thread(شبکه **nets, صحيح n, صحيح j)
{
    pthread_t thread;
    sync_آرگومانها *ptr = (sync_آرگومانها *)حافظه_صفر_بده(1, حجم(sync_آرگومانها));
    ptr->nets = nets;
    ptr->n = n;
    ptr->j = j;
    إذا(pthread_create(&thread, 0, sync_layer_thread, ptr)) error("Thread creation failed");
    أرجع thread;
}

فراغ sync_nets(شبکه **nets, صحيح n, صحيح interval)
{
    صحيح j;
    صحيح layers = nets[0]->n;
    pthread_t *نخها = (pthread_t *) حافظه_صفر_بده(layers, حجم(pthread_t));

    *(nets[0]->دیده) += interval * (n-1) * nets[0]->batch * nets[0]->subdivisions;
    لكل (j = 0; j < n; ++j){
        *(nets[j]->دیده) = *(nets[0]->دیده);
    }
    لكل (j = 0; j < layers; ++j) {
        نخها[j] = sync_layer_درون_thread(nets, n, j);
    }
    لكل (j = 0; j < layers; ++j) {
        pthread_join(نخها[j], 0);
    }
    حرر(نخها);
}

عائم train_networks(شبکه **nets, صحيح n, داده d, صحيح interval)
{
    صحيح i;
    صحيح batch = nets[0]->batch;
    صحيح subdivisions = nets[0]->subdivisions;
    أكد(batch * subdivisions * n == d.X.ردیفها);
    pthread_t *نخها = (pthread_t *) حافظه_صفر_بده(n, حجم(pthread_t));
    عائم *errors = (عائم *) حافظه_صفر_بده(n, حجم(عائم));

    عائم جمع = 0;
    لكل(i = 0; i < n; ++i){
        داده p = get_داده_part(d, i, n);
        نخها[i] = train_شبکه_درون_thread(nets[i], p, errors + i);
    }
    لكل(i = 0; i < n; ++i){
        pthread_join(نخها[i], 0);
        //printf("%f\n", errors[i]);
        جمع += errors[i];
    }
    //cudaDeviceSynchronize();
    إذا (get_current_batch(nets[0]) % interval == 0) {
        چاپ_کن("Syncing... ");
        fflush(stdout);
        sync_nets(nets, n, interval);
        چاپ_کن("Done!\n");
    }
    //cudaDeviceSynchronize();
    حرر(نخها);
    حرر(errors);
    أرجع (عائم)جمع/(n);
}

فراغ pull_شبکه_output(شبکه *net)
{
    layer l = get_شبکه_output_layer(net);
    cuda_pull_array(l.output_gpu, l.output, l.outputs*l.batch);
}

#پایان_اگر

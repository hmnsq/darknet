#شامل <stdio.h>
#شامل <time.h>
#شامل <assert.h>
#شامل "network.h"
#شامل "image.h"
#شامل "data.h"
#شامل "utils.h"
#شامل "blas.h"

#شامل "crop_layer.h"
#شامل "connected_layer.h"
#شامل "gru_layer.h"
#شامل "rnn_layer.h"
#شامل "crnn_layer.h"
#شامل "local_layer.h"
#شامل "convolutional_layer.h"
#شامل "activation_layer.h"
#شامل "detection_layer.h"
#شامل "region_layer.h"
#شامل "yolo_layer.h"
#شامل "normalization_layer.h"
#شامل "batchnorm_layer.h"
#شامل "maxpool_layer.h"
#شامل "reorg_layer.h"
#شامل "avgpool_layer.h"
#شامل "cost_layer.h"
#شامل "softmax_layer.h"
#شامل "dropout_layer.h"
#شامل "route_layer.h"
#شامل "upsample_layer.h"
#شامل "shortcut_layer.h"
#شامل "parser.h"
#شامل "data.h"

بھرو_دلائل get_بنیاد_دلائل(جال *net)
{
    بھرو_دلائل دلائل = {0};
    دلائل.w = net->w;
    دلائل.h = net->h;
    دلائل.سائز = net->w;

    دلائل.min = net->min_crop;
    دلائل.max = net->max_crop;
    دلائل.angle = net->angle;
    دلائل.aspect = net->aspect;
    دلائل.exposure = net->exposure;
    دلائل.center = net->center;
    دلائل.saturation = net->saturation;
    دلائل.hue = net->hue;
    واپس دلائل;
}

جال *بھرو_جال(حرف *cfg, حرف *weights, صحیح clear)
{
    جال *net = parse_جال_cfg(cfg);
    اگر(weights && weights[0] != 0){
        بھرو_weights(net, weights);
    }
    اگر(clear) (*net->نظر_آیا) = 0;
    واپس net;
}

size_t get_current_batch(جال *net)
{
    size_t batch_عدد = (*net->نظر_آیا)/(net->batch*net->subdivisions);
    واپس batch_عدد;
}

خالی reset_جال_state(جال *net, صحیح b)
{
    صحیح i;
    برائے (i = 0; i < net->n; ++i) {
        #اگر_تعریف_شدہ GPU
        layer l = net->layers[i];
        اگر(l.state_gpu){
            fill_gpu(l.outputs, 0, l.state_gpu + l.outputs*b, 1);
        }
        اگر(l.h_gpu){
            fill_gpu(l.outputs, 0, l.h_gpu + l.outputs*b, 1);
        }
        #اگر_ختم
    }
}

خالی reset_rnn(جال *net)
{
    reset_جال_state(net, 0);
}

اعشاری get_current_شرح(جال *net)
{
    size_t batch_عدد = get_current_batch(net);
    صحیح i;
    اعشاری شرح;
    اگر (batch_عدد < net->burn_اندر) واپس net->learning_شرح * قوت((اعشاری)batch_عدد / net->burn_اندر, net->power);
    انتخاب (net->policy) {
        حالت CONSTANT:
            واپس net->learning_شرح;
        حالت قدم_2:
            واپس net->learning_شرح * قوت(net->scale, batch_عدد/net->قدم);
        حالت اقدام_2:
            شرح = net->learning_شرح;
            برائے(i = 0; i < net->عدد_اقدام; ++i){
                اگر(net->اقدام[i] > batch_عدد) واپس شرح;
                شرح *= net->scales[i];
            }
            واپس شرح;
        حالت EXP:
            واپس net->learning_شرح * قوت(net->gamma, batch_عدد);
        حالت POLY:
            واپس net->learning_شرح * قوت(1 - (اعشاری)batch_عدد / net->max_batches, net->power);
        حالت RANDOM:
            واپس net->learning_شرح * قوت(rand_یکساں(0,1), net->power);
        حالت SIG:
            واپس net->learning_شرح * (1./(1.+قوت_نما(net->gamma*(batch_عدد - net->قدم))));
        طےشدہ:
            fprintf(stderr, "Policy is weird!\n");
            واپس net->learning_شرح;
    }
}

حرف *get_layer_string(LAYER_TYPE a)
{
    انتخاب(a){
        حالت CONVOLUTIONAL:
            واپس "convolutional";
        حالت فعال:
            واپس "activation";
        حالت LOCAL:
            واپس "local";
        حالت DECONVOLUTIONAL:
            واپس "deconvolutional";
        حالت CONNECTED:
            واپس "connected";
        حالت RNN:
            واپس "rnn";
        حالت GRU:
            واپس "gru";
        حالت LSTM:
	    واپس "lstm";
        حالت CRNN:
            واپس "crnn";
        حالت MAXPOOL:
            واپس "maxpool";
        حالت REORG:
            واپس "reorg";
        حالت AVGPOOL:
            واپس "avgpool";
        حالت SOFTMAX:
            واپس "softmax";
        حالت DETECTION:
            واپس "detection";
        حالت REGION:
            واپس "region";
        حالت YOLO:
            واپس "yolo";
        حالت DROPOUT:
            واپس "dropout";
        حالت CROP:
            واپس "crop";
        حالت لاگت:
            واپس "cost";
        حالت ROUTE:
            واپس "route";
        حالت SHORTCUT:
            واپس "shortcut";
        حالت NORMALIZATION:
            واپس "normalization";
        حالت BATCHNORM:
            واپس "batchnorm";
        طےشدہ:
            توڑو;
    }
    واپس "none";
}

جال *make_جال(صحیح n)
{
    جال *net = صفر_حافظہ_دو(1, حجم(جال));
    net->n = n;
    net->layers = صفر_حافظہ_دو(net->n, حجم(layer));
    net->نظر_آیا = صفر_حافظہ_دو(1, حجم(size_t));
    net->t    = صفر_حافظہ_دو(1, حجم(صحیح));
    net->لاگت_2 = صفر_حافظہ_دو(1, حجم(اعشاری));
    واپس net;
}

خالی forward_جال(جال *netp)
{
#اگر_تعریف_شدہ GPU
    اگر(netp->gpu_اشاریہ >= 0){
        forward_جال_gpu(netp);   
        واپس;
    }
#اگر_ختم
    جال net = *netp;
    صحیح i;
    برائے(i = 0; i < net.n; ++i){
        net.اشاریہ = i;
        layer l = net.layers[i];
        اگر(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.داخلہ = l.output;
        اگر(l.truth) {
            net.truth = l.output;
        }
    }
    calc_جال_لاگت(netp);
}

خالی update_جال(جال *netp)
{
#اگر_تعریف_شدہ GPU
    اگر(netp->gpu_اشاریہ >= 0){
        update_جال_gpu(netp);   
        واپس;
    }
#اگر_ختم
    جال net = *netp;
    صحیح i;
    update_دلائل a = {0};
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

    برائے(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        اگر(l.update){
            l.update(l, a);
        }
    }
}

خالی calc_جال_لاگت(جال *netp)
{
    جال net = *netp;
    صحیح i;
    اعشاری مجموع = 0;
    صحیح گنتی = 0;
    برائے(i = 0; i < net.n; ++i){
        اگر(net.layers[i].لاگت_2){
            مجموع += net.layers[i].لاگت_2[0];
            ++گنتی;
        }
    }
    *net.لاگت_2 = مجموع/گنتی;
}

صحیح get_predicted_class_جال(جال *net)
{
    واپس max_اشاریہ(net->output, net->outputs);
}

خالی backward_جال(جال *netp)
{
#اگر_تعریف_شدہ GPU
    اگر(netp->gpu_اشاریہ >= 0){
        backward_جال_gpu(netp);   
        واپس;
    }
#اگر_ختم
    جال net = *netp;
    صحیح i;
    جال orig = net;
    برائے(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        اگر(l.stopbackward) توڑو;
        اگر(i == 0){
            net = orig;
        }ورنہ{
            layer پچھلا = net.layers[i-1];
            net.داخلہ = پچھلا.output;
            net.delta = پچھلا.delta;
        }
        net.اشاریہ = i;
        l.backward(l, net);
    }
}

اعشاری train_جال_datum(جال *net)
{
    *net->نظر_آیا += net->batch;
    net->train = 1;
    forward_جال(net);
    backward_جال(net);
    اعشاری error = *net->لاگت_2;
    اگر(((*net->نظر_آیا)/net->batch)%net->subdivisions == 0) update_جال(net);
    واپس error;
}

اعشاری train_جال_sgd(جال *net, ڈیٹا d, صحیح n)
{
    صحیح batch = net->batch;

    صحیح i;
    اعشاری مجموع = 0;
    برائے(i = 0; i < n; ++i){
        get_random_batch(d, batch, net->داخلہ, net->truth);
        اعشاری err = train_جال_datum(net);
        مجموع += err;
    }
    واپس (اعشاری)مجموع/(n*batch);
}

اعشاری train_جال(جال *net, ڈیٹا d)
{
    أكد(d.X.صفیں % net->batch == 0);
    صحیح batch = net->batch;
    صحیح n = d.X.صفیں / batch;

    صحیح i;
    اعشاری مجموع = 0;
    برائے(i = 0; i < n; ++i){
        get_اگلا_batch(d, batch, i*batch, net->داخلہ, net->truth);
        اعشاری err = train_جال_datum(net);
        مجموع += err;
    }
    واپس (اعشاری)مجموع/(n*batch);
}

خالی سیٹ_عارضی_جال(جال *net, اعشاری t)
{
    صحیح i;
    برائے(i = 0; i < net->n; ++i){
        net->layers[i].temperature = t;
    }
}


خالی سیٹ_batch_جال(جال *net, صحیح b)
{
    net->batch = b;
    صحیح i;
    برائے(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#اگر_تعریف_شدہ CUDNN
        اگر(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_تیاری(net->layers + i);
        }
        اگر(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnn_سیٹ_Tensor4d_Descriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, 1, l->برآمد_c, l->برآمد_h, l->برآمد_w);
            cudnn_سیٹ_Tensor4d_Descriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, 1, l->برآمد_c, 1, 1); 
        }
#اگر_ختم
    }
}

صحیح resize_جال(جال *net, صحیح w, صحیح h)
{
#اگر_تعریف_شدہ GPU
    cuda_سیٹ_device(net->gpu_اشاریہ);
    cuda_free(net->workspace);
#اگر_ختم
    صحیح i;
    //if(w == net->w && h == net->h) return 0;
    net->w = w;
    net->h = h;
    صحیح inputs = 0;
    size_t workspace_سائز = 0;
    //fprintf(stderr, "Resizing to %d x %d...\n", w, h);
    //fflush(stderr);
    برائے (i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        اگر(l.type == CONVOLUTIONAL){
            resize_convolutional_layer(&l, w, h);
        }ورنہ اگر(l.type == CROP){
            resize_crop_layer(&l, w, h);
        }ورنہ اگر(l.type == MAXPOOL){
            resize_maxpool_layer(&l, w, h);
        }ورنہ اگر(l.type == REGION){
            resize_region_layer(&l, w, h);
        }ورنہ اگر(l.type == YOLO){
            resize_yolo_layer(&l, w, h);
        }ورنہ اگر(l.type == ROUTE){
            resize_route_layer(&l, net);
        }ورنہ اگر(l.type == SHORTCUT){
            resize_shortcut_layer(&l, w, h);
        }ورنہ اگر(l.type == UPSAMPLE){
            resize_upsample_layer(&l, w, h);
        }ورنہ اگر(l.type == REORG){
            resize_reorg_layer(&l, w, h);
        }ورنہ اگر(l.type == AVGPOOL){
            resize_avgpool_layer(&l, w, h);
        }ورنہ اگر(l.type == NORMALIZATION){
            resize_normalization_layer(&l, w, h);
        }ورنہ اگر(l.type == لاگت){
            resize_لاگت_layer(&l, inputs);
        }ورنہ{
            error("Cannot resize this type of layer");
        }
        اگر(l.workspace_سائز > workspace_سائز) workspace_سائز = l.workspace_سائز;
        اگر(l.workspace_سائز > 2000000000) أكد(0);
        inputs = l.outputs;
        net->layers[i] = l;
        w = l.برآمد_w;
        h = l.برآمد_h;
        اگر(l.type == AVGPOOL) توڑو;
    }
    layer برآمد = get_جال_output_layer(net);
    net->inputs = net->layers[0].inputs;
    net->outputs = برآمد.outputs;
    net->truths = برآمد.outputs;
    اگر(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = برآمد.output;
    آزاد(net->داخلہ);
    آزاد(net->truth);
    net->داخلہ = صفر_حافظہ_دو(net->inputs*net->batch, حجم(اعشاری));
    net->truth = صفر_حافظہ_دو(net->truths*net->batch, حجم(اعشاری));
#اگر_تعریف_شدہ GPU
    اگر(gpu_اشاریہ >= 0){
        cuda_free(net->داخلہ_gpu);
        cuda_free(net->truth_gpu);
        net->داخلہ_gpu = cuda_make_array(net->داخلہ, net->inputs*net->batch);
        net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
        اگر(workspace_سائز){
            net->workspace = cuda_make_array(0, (workspace_سائز-1)/حجم(اعشاری)+1);
        }
    }ورنہ {
        آزاد(net->workspace);
        net->workspace = صفر_حافظہ_دو(1, workspace_سائز);
    }
#ورنہ
    آزاد(net->workspace);
    net->workspace = صفر_حافظہ_دو(1, workspace_سائز);
#اگر_ختم
    //fprintf(stderr, " Done!\n");
    واپس 0;
}

layer get_جال_detection_layer(جال *net)
{
    صحیح i;
    برائے(i = 0; i < net->n; ++i){
        اگر(net->layers[i].type == DETECTION){
            واپس net->layers[i];
        }
    }
    fprintf(stderr, "Detection layer not found!!\n");
    layer l = {0};
    واپس l;
}

image get_جال_image_layer(جال *net, صحیح i)
{
    layer l = net->layers[i];
#اگر_تعریف_شدہ GPU
    //cuda_pull_array(l.output_gpu, l.output, l.outputs);
#اگر_ختم
    اگر (l.برآمد_w && l.برآمد_h && l.برآمد_c){
        واپس float_مقصد_image(l.برآمد_w, l.برآمد_h, l.برآمد_c, l.output);
    }
    image def = {0};
    واپس def;
}

image get_جال_image(جال *net)
{
    صحیح i;
    برائے(i = net->n-1; i >= 0; --i){
        image m = get_جال_image_layer(net, i);
        اگر(m.h != 0) واپس m;
    }
    image def = {0};
    واپس def;
}

خالی visualize_جال(جال *net)
{
    image *پچھلا = 0;
    صحیح i;
    حرف buff[256];
    برائے(i = 0; i < net->n; ++i){
        sprintf(buff, "Layer %d", i);
        layer l = net->layers[i];
        اگر(l.type == CONVOLUTIONAL){
            پچھلا = visualize_convolutional_layer(l, buff, پچھلا);
        }
    } 
}

خالی چوٹی_predictions(جال *net, صحیح k, صحیح *اشاریہ)
{
    چوٹی_k(net->output, net->outputs, k, اشاریہ);
}


اعشاری *جال_predict(جال *net, اعشاری *داخلہ)
{
    جال orig = *net;
    net->داخلہ = داخلہ;
    net->truth = 0;
    net->train = 0;
    net->delta = 0;
    forward_جال(net);
    اعشاری *برآمد = net->output;
    *net = orig;
    واپس برآمد;
}

صحیح عدد_detections(جال *net, اعشاری thresh)
{
    صحیح i;
    صحیح s = 0;
    برائے(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        اگر(l.type == YOLO){
            s += yolo_عدد_detections(l, thresh);
        }
        اگر(l.type == DETECTION || l.type == REGION){
            s += l.w*l.h*l.n;
        }
    }
    واپس s;
}

detection *make_جال_boxes(جال *net, اعشاری thresh, صحیح *عدد)
{
    layer l = net->layers[net->n - 1];
    صحیح i;
    صحیح nboxes = عدد_detections(net, thresh);
    اگر(عدد) *عدد = nboxes;
    detection *dets = صفر_حافظہ_دو(nboxes, حجم(detection));
    برائے(i = 0; i < nboxes; ++i){
        dets[i].prob = صفر_حافظہ_دو(l.classes, حجم(اعشاری));
        اگر(l.coords > 4){
            dets[i].نقاب = صفر_حافظہ_دو(l.coords-4, حجم(اعشاری));
        }
    }
    واپس dets;
}

خالی fill_جال_boxes(جال *net, صحیح w, صحیح h, اعشاری thresh, اعشاری hier, صحیح *map, صحیح relative, detection *dets)
{
    صحیح j;
    برائے(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        اگر(l.type == YOLO){
            صحیح گنتی = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
            dets += گنتی;
        }
        اگر(l.type == REGION){
            get_region_detections(l, w, h, net->w, net->h, thresh, map, hier, relative, dets);
            dets += l.w*l.h*l.n;
        }
        اگر(l.type == DETECTION){
            get_detection_detections(l, w, h, thresh, dets);
            dets += l.w*l.h*l.n;
        }
    }
}

detection *get_جال_boxes(جال *net, صحیح w, صحیح h, اعشاری thresh, اعشاری hier, صحیح *map, صحیح relative, صحیح *عدد)
{
    detection *dets = make_جال_boxes(net, thresh, عدد);
    fill_جال_boxes(net, w, h, thresh, hier, map, relative, dets);
    واپس dets;
}

خالی free_detections(detection *dets, صحیح n)
{
    صحیح i;
    برائے(i = 0; i < n; ++i){
        آزاد(dets[i].prob);
        اگر(dets[i].نقاب) آزاد(dets[i].نقاب);
    }
    آزاد(dets);
}

اعشاری *جال_predict_image(جال *net, image im)
{
    image imr = letterbox_image(im, net->w, net->h);
    سیٹ_batch_جال(net, 1);
    اعشاری *p = جال_predict(net, imr.ڈیٹا);
    free_image(imr);
    واپس p;
}

صحیح جال_چوڑائی(جال *net){واپس net->w;}
صحیح جال_اونچائی(جال *net){واپس net->h;}

میٹرکس جال_predict_ڈیٹا_multi(جال *net, ڈیٹا test, صحیح n)
{
    صحیح i,j,b,m;
    صحیح k = net->outputs;
    میٹرکس pred = make_میٹرکس(test.X.صفیں, k);
    اعشاری *X = صفر_حافظہ_دو(net->batch*test.X.صفیں, حجم(اعشاری));
    برائے(i = 0; i < test.X.صفیں; i += net->batch){
        برائے(b = 0; b < net->batch; ++b){
            اگر(i+b == test.X.صفیں) توڑو;
            حافظہ_نقل(X+b*test.X.ستون_تعداد, test.X.vals[i+b], test.X.ستون_تعداد*حجم(اعشاری));
        }
        برائے(m = 0; m < n; ++m){
            اعشاری *برآمد = جال_predict(net, X);
            برائے(b = 0; b < net->batch; ++b){
                اگر(i+b == test.X.صفیں) توڑو;
                برائے(j = 0; j < k; ++j){
                    pred.vals[i+b][j] += برآمد[j+b*k]/n;
                }
            }
        }
    }
    آزاد(X);
    واپس pred;   
}

میٹرکس جال_predict_ڈیٹا(جال *net, ڈیٹا test)
{
    صحیح i,j,b;
    صحیح k = net->outputs;
    میٹرکس pred = make_میٹرکس(test.X.صفیں, k);
    اعشاری *X = صفر_حافظہ_دو(net->batch*test.X.ستون_تعداد, حجم(اعشاری));
    برائے(i = 0; i < test.X.صفیں; i += net->batch){
        برائے(b = 0; b < net->batch; ++b){
            اگر(i+b == test.X.صفیں) توڑو;
            حافظہ_نقل(X+b*test.X.ستون_تعداد, test.X.vals[i+b], test.X.ستون_تعداد*حجم(اعشاری));
        }
        اعشاری *برآمد = جال_predict(net, X);
        برائے(b = 0; b < net->batch; ++b){
            اگر(i+b == test.X.صفیں) توڑو;
            برائے(j = 0; j < k; ++j){
                pred.vals[i+b][j] = برآمد[j+b*k];
            }
        }
    }
    آزاد(X);
    واپس pred;   
}

خالی print_جال(جال *net)
{
    صحیح i,j;
    برائے(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        اعشاری *output = l.output;
        صحیح n = l.outputs;
        اعشاری اوسط = اوسط_array(output, n);
        اعشاری vari = variance_array(output, n);
        fprintf(stderr, "Layer %d - Mean: %f, Variance: %f\n",i,اوسط, vari);
        اگر(n > 100) n = 100;
        برائے(j = 0; j < n; ++j) fprintf(stderr, "%f, ", output[j]);
        اگر(n == 100)fprintf(stderr,".....\n");
        fprintf(stderr, "\n");
    }
}

خالی compare_networks(جال *n1, جال *n2, ڈیٹا test)
{
    میٹرکس g1 = جال_predict_ڈیٹا(n1, test);
    میٹرکس g2 = جال_predict_ڈیٹا(n2, test);
    صحیح i;
    صحیح a,b,c,d;
    a = b = c = d = 0;
    برائے(i = 0; i < g1.صفیں; ++i){
        صحیح truth = max_اشاریہ(test.y.vals[i], test.y.ستون_تعداد);
        صحیح p1 = max_اشاریہ(g1.vals[i], g1.ستون_تعداد);
        صحیح p2 = max_اشاریہ(g2.vals[i], g2.ستون_تعداد);
        اگر(p1 == truth){
            اگر(p2 == truth) ++d;
            ورنہ ++c;
        }ورنہ{
            اگر(p2 == truth) ++b;
            ورنہ ++a;
        }
    }
    لکھو("%5d %5d\n%5d %5d\n", a, b, c, d);
    اعشاری عدد = قوت((مطلق_قدر(b - c) - 1.), 2.);
    اعشاری den = b + c;
    لکھو("%f\n", عدد/den); 
}

اعشاری جال_accuracy(جال *net, ڈیٹا d)
{
    میٹرکس guess = جال_predict_ڈیٹا(net, d);
    اعشاری acc = میٹرکس_topk_accuracy(d.y, guess,1);
    free_میٹرکس(guess);
    واپس acc;
}

اعشاری *جال_accuracies(جال *net, ڈیٹا d, صحیح n)
{
    ساکن اعشاری acc[2];
    میٹرکس guess = جال_predict_ڈیٹا(net, d);
    acc[0] = میٹرکس_topk_accuracy(d.y, guess, 1);
    acc[1] = میٹرکس_topk_accuracy(d.y, guess, n);
    free_میٹرکس(guess);
    واپس acc;
}

layer get_جال_output_layer(جال *net)
{
    صحیح i;
    برائے(i = net->n - 1; i >= 0; --i){
        اگر(net->layers[i].type != لاگت) توڑو;
    }
    واپس net->layers[i];
}

اعشاری جال_accuracy_multi(جال *net, ڈیٹا d, صحیح n)
{
    میٹرکس guess = جال_predict_ڈیٹا_multi(net, d, n);
    اعشاری acc = میٹرکس_topk_accuracy(d.y, guess,1);
    free_میٹرکس(guess);
    واپس acc;
}

خالی free_جال(جال *net)
{
    صحیح i;
    برائے(i = 0; i < net->n; ++i){
        free_layer(net->layers[i]);
    }
    آزاد(net->layers);
    اگر(net->داخلہ) آزاد(net->داخلہ);
    اگر(net->truth) آزاد(net->truth);
#اگر_تعریف_شدہ GPU
    اگر(net->داخلہ_gpu) cuda_free(net->داخلہ_gpu);
    اگر(net->truth_gpu) cuda_free(net->truth_gpu);
#اگر_ختم
    آزاد(net);
}

// Some day...
// ^ What the hell is this comment for?


layer جال_output_layer(جال *net)
{
    صحیح i;
    برائے(i = net->n - 1; i >= 0; --i){
        اگر(net->layers[i].type != لاگت) توڑو;
    }
    واپس net->layers[i];
}

صحیح جال_inputs(جال *net)
{
    واپس net->layers[0].inputs;
}

صحیح جال_outputs(جال *net)
{
    واپس جال_output_layer(net).outputs;
}

اعشاری *جال_output(جال *net)
{
    واپس جال_output_layer(net).output;
}

#اگر_تعریف_شدہ GPU

خالی forward_جال_gpu(جال *netp)
{
    جال net = *netp;
    cuda_سیٹ_device(net.gpu_اشاریہ);
    cuda_ڈالو_array(net.داخلہ_gpu, net.داخلہ, net.inputs*net.batch);
    اگر(net.truth){
        cuda_ڈالو_array(net.truth_gpu, net.truth, net.truths*net.batch);
    }

    صحیح i;
    برائے(i = 0; i < net.n; ++i){
        net.اشاریہ = i;
        layer l = net.layers[i];
        اگر(l.delta_gpu){
            fill_gpu(l.outputs * l.batch, 0, l.delta_gpu, 1);
        }
        l.forward_gpu(l, net);
        net.داخلہ_gpu = l.output_gpu;
        net.داخلہ = l.output;
        اگر(l.truth) {
            net.truth_gpu = l.output_gpu;
            net.truth = l.output;
        }
    }
    pull_جال_output(netp);
    calc_جال_لاگت(netp);
}

خالی backward_جال_gpu(جال *netp)
{
    صحیح i;
    جال net = *netp;
    جال orig = net;
    cuda_سیٹ_device(net.gpu_اشاریہ);
    برائے(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        اگر(l.stopbackward) توڑو;
        اگر(i == 0){
            net = orig;
        }ورنہ{
            layer پچھلا = net.layers[i-1];
            net.داخلہ = پچھلا.output;
            net.delta = پچھلا.delta;
            net.داخلہ_gpu = پچھلا.output_gpu;
            net.delta_gpu = پچھلا.delta_gpu;
        }
        net.اشاریہ = i;
        l.backward_gpu(l, net);
    }
}

خالی update_جال_gpu(جال *netp)
{
    جال net = *netp;
    cuda_سیٹ_device(net.gpu_اشاریہ);
    صحیح i;
    update_دلائل a = {0};
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

    برائے(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        اگر(l.update_gpu){
            l.update_gpu(l, a);
        }
    }
}

خالی harmless_update_جال_gpu(جال *netp)
{
    جال net = *netp;
    cuda_سیٹ_device(net.gpu_اشاریہ);
    صحیح i;
    برائے(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        اگر(l.وزن_updates_gpu) fill_gpu(l.nweights, 0, l.وزن_updates_gpu, 1);
        اگر(l.bias_updates_gpu) fill_gpu(l.nbiases, 0, l.bias_updates_gpu, 1);
        اگر(l.scale_updates_gpu) fill_gpu(l.nbiases, 0, l.scale_updates_gpu, 1);
    }
}

قسمتعریف ساخت {
    جال *net;
    ڈیٹا d;
    اعشاری *err;
} train_دلائل;

خالی *train_thread(خالی *ptr)
{
    train_دلائل دلائل = *(train_دلائل*)ptr;
    آزاد(ptr);
    cuda_سیٹ_device(دلائل.net->gpu_اشاریہ);
    *دلائل.err = train_جال(دلائل.net, دلائل.d);
    واپس 0;
}

pthread_t train_جال_اندر_thread(جال *net, ڈیٹا d, اعشاری *err)
{
    pthread_t thread;
    train_دلائل *ptr = (train_دلائل *)صفر_حافظہ_دو(1, حجم(train_دلائل));
    ptr->net = net;
    ptr->d = d;
    ptr->err = err;
    اگر(pthread_create(&thread, 0, train_thread, ptr)) error("Thread creation failed");
    واپس thread;
}

خالی ملاؤ_weights(layer l, layer بنیاد)
{
    اگر (l.type == CONVOLUTIONAL) {
        axpy_cpu(l.n, 1, l.bias_updates, 1, بنیاد.biases, 1);
        axpy_cpu(l.nweights, 1, l.وزن_updates, 1, بنیاد.weights, 1);
        اگر (l.scales) {
            axpy_cpu(l.n, 1, l.scale_updates, 1, بنیاد.scales, 1);
        }
    } ورنہ اگر(l.type == CONNECTED) {
        axpy_cpu(l.outputs, 1, l.bias_updates, 1, بنیاد.biases, 1);
        axpy_cpu(l.outputs*l.inputs, 1, l.وزن_updates, 1, بنیاد.weights, 1);
    }
}

خالی scale_weights(layer l, اعشاری s)
{
    اگر (l.type == CONVOLUTIONAL) {
        scal_cpu(l.n, s, l.biases, 1);
        scal_cpu(l.nweights, s, l.weights, 1);
        اگر (l.scales) {
            scal_cpu(l.n, s, l.scales, 1);
        }
    } ورنہ اگر(l.type == CONNECTED) {
        scal_cpu(l.outputs, s, l.biases, 1);
        scal_cpu(l.outputs*l.inputs, s, l.weights, 1);
    }
}


خالی pull_weights(layer l)
{
    اگر(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.n);
        cuda_pull_array(l.weights_gpu, l.وزن_updates, l.nweights);
        اگر(l.scales) cuda_pull_array(l.scales_gpu, l.scale_updates, l.n);
    } ورنہ اگر(l.type == CONNECTED){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.outputs);
        cuda_pull_array(l.weights_gpu, l.وزن_updates, l.outputs*l.inputs);
    }
}

خالی ڈالو_weights(layer l)
{
    اگر(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_ڈالو_array(l.biases_gpu, l.biases, l.n);
        cuda_ڈالو_array(l.weights_gpu, l.weights, l.nweights);
        اگر(l.scales) cuda_ڈالو_array(l.scales_gpu, l.scales, l.n);
    } ورنہ اگر(l.type == CONNECTED){
        cuda_ڈالو_array(l.biases_gpu, l.biases, l.outputs);
        cuda_ڈالو_array(l.weights_gpu, l.weights, l.outputs*l.inputs);
    }
}

خالی distribute_weights(layer l, layer بنیاد)
{
    اگر (l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL) {
        cuda_ڈالو_array(l.biases_gpu, بنیاد.biases, l.n);
        cuda_ڈالو_array(l.weights_gpu, بنیاد.weights, l.nweights);
        اگر (بنیاد.scales) cuda_ڈالو_array(l.scales_gpu, بنیاد.scales, l.n);
    } ورنہ اگر (l.type == CONNECTED) {
        cuda_ڈالو_array(l.biases_gpu, بنیاد.biases, l.outputs);
        cuda_ڈالو_array(l.weights_gpu, بنیاد.weights, l.outputs*l.inputs);
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

خالی sync_layer(جال **nets, صحیح n, صحیح j)
{
    صحیح i;
    جال *net = nets[0];
    layer بنیاد = net->layers[j];
    scale_weights(بنیاد, 0);
    برائے (i = 0; i < n; ++i) {
        cuda_سیٹ_device(nets[i]->gpu_اشاریہ);
        layer l = nets[i]->layers[j];
        pull_weights(l);
        ملاؤ_weights(l, بنیاد);
    }
    scale_weights(بنیاد, 1./n);
    برائے (i = 0; i < n; ++i) {
        cuda_سیٹ_device(nets[i]->gpu_اشاریہ);
        layer l = nets[i]->layers[j];
        distribute_weights(l, بنیاد);
    }
}

قسمتعریف ساخت{
    جال **nets;
    صحیح n;
    صحیح j;
} sync_دلائل;

خالی *sync_layer_thread(خالی *ptr)
{
    sync_دلائل دلائل = *(sync_دلائل*)ptr;
    sync_layer(دلائل.nets, دلائل.n, دلائل.j);
    آزاد(ptr);
    واپس 0;
}

pthread_t sync_layer_اندر_thread(جال **nets, صحیح n, صحیح j)
{
    pthread_t thread;
    sync_دلائل *ptr = (sync_دلائل *)صفر_حافظہ_دو(1, حجم(sync_دلائل));
    ptr->nets = nets;
    ptr->n = n;
    ptr->j = j;
    اگر(pthread_create(&thread, 0, sync_layer_thread, ptr)) error("Thread creation failed");
    واپس thread;
}

خالی sync_nets(جال **nets, صحیح n, صحیح interval)
{
    صحیح j;
    صحیح layers = nets[0]->n;
    pthread_t *دھاگے = (pthread_t *) صفر_حافظہ_دو(layers, حجم(pthread_t));

    *(nets[0]->نظر_آیا) += interval * (n-1) * nets[0]->batch * nets[0]->subdivisions;
    برائے (j = 0; j < n; ++j){
        *(nets[j]->نظر_آیا) = *(nets[0]->نظر_آیا);
    }
    برائے (j = 0; j < layers; ++j) {
        دھاگے[j] = sync_layer_اندر_thread(nets, n, j);
    }
    برائے (j = 0; j < layers; ++j) {
        pthread_join(دھاگے[j], 0);
    }
    آزاد(دھاگے);
}

اعشاری train_networks(جال **nets, صحیح n, ڈیٹا d, صحیح interval)
{
    صحیح i;
    صحیح batch = nets[0]->batch;
    صحیح subdivisions = nets[0]->subdivisions;
    أكد(batch * subdivisions * n == d.X.صفیں);
    pthread_t *دھاگے = (pthread_t *) صفر_حافظہ_دو(n, حجم(pthread_t));
    اعشاری *errors = (اعشاری *) صفر_حافظہ_دو(n, حجم(اعشاری));

    اعشاری مجموع = 0;
    برائے(i = 0; i < n; ++i){
        ڈیٹا p = get_ڈیٹا_part(d, i, n);
        دھاگے[i] = train_جال_اندر_thread(nets[i], p, errors + i);
    }
    برائے(i = 0; i < n; ++i){
        pthread_join(دھاگے[i], 0);
        //printf("%f\n", errors[i]);
        مجموع += errors[i];
    }
    //cudaDeviceSynchronize();
    اگر (get_current_batch(nets[0]) % interval == 0) {
        لکھو("Syncing... ");
        fflush(stdout);
        sync_nets(nets, n, interval);
        لکھو("Done!\n");
    }
    //cudaDeviceSynchronize();
    آزاد(دھاگے);
    آزاد(errors);
    واپس (اعشاری)مجموع/(n);
}

خالی pull_جال_output(جال *net)
{
    layer l = get_جال_output_layer(net);
    cuda_pull_array(l.output_gpu, l.output, l.outputs*l.batch);
}

#اگر_ختم

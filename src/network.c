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

لدو_دلیلاں get_ادھار_دلیلاں(جال *net)
{
    لدو_دلیلاں دلیلاں = {0};
    دلیلاں.w = net->w;
    دلیلاں.h = net->h;
    دلیلاں.ناپ = net->w;

    دلیلاں.min = net->min_crop;
    دلیلاں.max = net->max_crop;
    دلیلاں.angle = net->angle;
    دلیلاں.aspect = net->aspect;
    دلیلاں.exposure = net->exposure;
    دلیلاں.center = net->center;
    دلیلاں.saturation = net->saturation;
    دلیلاں.hue = net->hue;
    أرجع دلیلاں;
}

جال *لدو_جال(حرف *cfg, حرف *weights, صحيح clear)
{
    جال *net = parse_جال_cfg(cfg);
    إذا(weights && weights[0] != 0){
        لدو_weights(net, weights);
    }
    إذا(clear) (*net->تکیا) = 0;
    أرجع net;
}

size_t get_current_batch(جال *net)
{
    size_t batch_عدد = (*net->تکیا)/(net->batch*net->subdivisions);
    أرجع batch_عدد;
}

فراغ reset_جال_state(جال *net, صحيح b)
{
    صحيح i;
    لكل (i = 0; i < net->n; ++i) {
        #جے_تعریف_شدہ GPU
        layer l = net->layers[i];
        إذا(l.state_gpu){
            fill_gpu(l.outputs, 0, l.state_gpu + l.outputs*b, 1);
        }
        إذا(l.h_gpu){
            fill_gpu(l.outputs, 0, l.h_gpu + l.outputs*b, 1);
        }
        #جے_انت
    }
}

فراغ reset_rnn(جال *net)
{
    reset_جال_state(net, 0);
}

عائم get_current_شرح(جال *net)
{
    size_t batch_عدد = get_current_batch(net);
    صحيح i;
    عائم شرح;
    إذا (batch_عدد < net->burn_وچ) أرجع net->learning_شرح * قوت((عائم)batch_عدد / net->burn_وچ, net->power);
    بدل (net->policy) {
        حالة CONSTANT:
            أرجع net->learning_شرح;
        حالة قدم_2:
            أرجع net->learning_شرح * قوت(net->scale, batch_عدد/net->قدم);
        حالة قدماں_2:
            شرح = net->learning_شرح;
            لكل(i = 0; i < net->عدد_قدماں; ++i){
                إذا(net->قدماں[i] > batch_عدد) أرجع شرح;
                شرح *= net->scales[i];
            }
            أرجع شرح;
        حالة EXP:
            أرجع net->learning_شرح * قوت(net->gamma, batch_عدد);
        حالة POLY:
            أرجع net->learning_شرح * قوت(1 - (عائم)batch_عدد / net->max_batches, net->power);
        حالة RANDOM:
            أرجع net->learning_شرح * قوت(rand_اکسار(0,1), net->power);
        حالة SIG:
            أرجع net->learning_شرح * (1./(1.+قوت_نما(net->gamma*(batch_عدد - net->قدم))));
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
        حالة لاگت:
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

جال *make_جال(صحيح n)
{
    جال *net = صفر_حافظہ_دیو(1, حجم(جال));
    net->n = n;
    net->layers = صفر_حافظہ_دیو(net->n, حجم(layer));
    net->تکیا = صفر_حافظہ_دیو(1, حجم(size_t));
    net->t    = صفر_حافظہ_دیو(1, حجم(صحيح));
    net->لاگت_2 = صفر_حافظہ_دیو(1, حجم(عائم));
    أرجع net;
}

فراغ forward_جال(جال *netp)
{
#جے_تعریف_شدہ GPU
    إذا(netp->gpu_اشاریہ >= 0){
        forward_جال_gpu(netp);   
        أرجع;
    }
#جے_انت
    جال net = *netp;
    صحيح i;
    لكل(i = 0; i < net.n; ++i){
        net.اشاریہ = i;
        layer l = net.layers[i];
        إذا(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.داخلہ = l.output;
        إذا(l.truth) {
            net.truth = l.output;
        }
    }
    calc_جال_لاگت(netp);
}

فراغ update_جال(جال *netp)
{
#جے_تعریف_شدہ GPU
    إذا(netp->gpu_اشاریہ >= 0){
        update_جال_gpu(netp);   
        أرجع;
    }
#جے_انت
    جال net = *netp;
    صحيح i;
    update_دلیلاں a = {0};
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

فراغ calc_جال_لاگت(جال *netp)
{
    جال net = *netp;
    صحيح i;
    عائم جوڑ = 0;
    صحيح گنتی = 0;
    لكل(i = 0; i < net.n; ++i){
        إذا(net.layers[i].لاگت_2){
            جوڑ += net.layers[i].لاگت_2[0];
            ++گنتی;
        }
    }
    *net.لاگت_2 = جوڑ/گنتی;
}

صحيح get_predicted_class_جال(جال *net)
{
    أرجع max_اشاریہ(net->output, net->outputs);
}

فراغ backward_جال(جال *netp)
{
#جے_تعریف_شدہ GPU
    إذا(netp->gpu_اشاریہ >= 0){
        backward_جال_gpu(netp);   
        أرجع;
    }
#جے_انت
    جال net = *netp;
    صحيح i;
    جال orig = net;
    لكل(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        إذا(l.stopbackward) اكسر;
        إذا(i == 0){
            net = orig;
        }وإلا{
            layer prev = net.layers[i-1];
            net.داخلہ = prev.output;
            net.delta = prev.delta;
        }
        net.اشاریہ = i;
        l.backward(l, net);
    }
}

عائم train_جال_datum(جال *net)
{
    *net->تکیا += net->batch;
    net->train = 1;
    forward_جال(net);
    backward_جال(net);
    عائم error = *net->لاگت_2;
    إذا(((*net->تکیا)/net->batch)%net->subdivisions == 0) update_جال(net);
    أرجع error;
}

عائم train_جال_sgd(جال *net, انکڑے d, صحيح n)
{
    صحيح batch = net->batch;

    صحيح i;
    عائم جوڑ = 0;
    لكل(i = 0; i < n; ++i){
        get_random_batch(d, batch, net->داخلہ, net->truth);
        عائم err = train_جال_datum(net);
        جوڑ += err;
    }
    أرجع (عائم)جوڑ/(n*batch);
}

عائم train_جال(جال *net, انکڑے d)
{
    أكد(d.X.صفاں % net->batch == 0);
    صحيح batch = net->batch;
    صحيح n = d.X.صفاں / batch;

    صحيح i;
    عائم جوڑ = 0;
    لكل(i = 0; i < n; ++i){
        get_اگلا_batch(d, batch, i*batch, net->داخلہ, net->truth);
        عائم err = train_جال_datum(net);
        جوڑ += err;
    }
    أرجع (عائم)جوڑ/(n*batch);
}

فراغ سیٹ_عارضی_جال(جال *net, عائم t)
{
    صحيح i;
    لكل(i = 0; i < net->n; ++i){
        net->layers[i].temperature = t;
    }
}


فراغ سیٹ_batch_جال(جال *net, صحيح b)
{
    net->batch = b;
    صحيح i;
    لكل(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#جے_تعریف_شدہ CUDNN
        إذا(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_تیاری(net->layers + i);
        }
        إذا(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnn_سیٹ_Tensor4d_Descriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_انکڑے_FLOAT, 1, l->باہرلا_c, l->باہرلا_h, l->باہرلا_w);
            cudnn_سیٹ_Tensor4d_Descriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_انکڑے_FLOAT, 1, l->باہرلا_c, 1, 1); 
        }
#جے_انت
    }
}

صحيح resize_جال(جال *net, صحيح w, صحيح h)
{
#جے_تعریف_شدہ GPU
    cuda_سیٹ_device(net->gpu_اشاریہ);
    cuda_free(net->workspace);
#جے_انت
    صحيح i;
    //if(w == net->w && h == net->h) return 0;
    net->w = w;
    net->h = h;
    صحيح inputs = 0;
    size_t workspace_ناپ = 0;
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
        }وإلا إذا(l.type == لاگت){
            resize_لاگت_layer(&l, inputs);
        }وإلا{
            error("Cannot resize this type of layer");
        }
        إذا(l.workspace_ناپ > workspace_ناپ) workspace_ناپ = l.workspace_ناپ;
        إذا(l.workspace_ناپ > 2000000000) أكد(0);
        inputs = l.outputs;
        net->layers[i] = l;
        w = l.باہرلا_w;
        h = l.باہرلا_h;
        إذا(l.type == AVGPOOL) اكسر;
    }
    layer باہرلا = get_جال_output_layer(net);
    net->inputs = net->layers[0].inputs;
    net->outputs = باہرلا.outputs;
    net->truths = باہرلا.outputs;
    إذا(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = باہرلا.output;
    حرر(net->داخلہ);
    حرر(net->truth);
    net->داخلہ = صفر_حافظہ_دیو(net->inputs*net->batch, حجم(عائم));
    net->truth = صفر_حافظہ_دیو(net->truths*net->batch, حجم(عائم));
#جے_تعریف_شدہ GPU
    إذا(gpu_اشاریہ >= 0){
        cuda_free(net->داخلہ_gpu);
        cuda_free(net->truth_gpu);
        net->داخلہ_gpu = cuda_make_array(net->داخلہ, net->inputs*net->batch);
        net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
        إذا(workspace_ناپ){
            net->workspace = cuda_make_array(0, (workspace_ناپ-1)/حجم(عائم)+1);
        }
    }وإلا {
        حرر(net->workspace);
        net->workspace = صفر_حافظہ_دیو(1, workspace_ناپ);
    }
#وإلا
    حرر(net->workspace);
    net->workspace = صفر_حافظہ_دیو(1, workspace_ناپ);
#جے_انت
    //fprintf(stderr, " Done!\n");
    أرجع 0;
}

layer get_جال_detection_layer(جال *net)
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

image get_جال_image_layer(جال *net, صحيح i)
{
    layer l = net->layers[i];
#جے_تعریف_شدہ GPU
    //cuda_pull_array(l.output_gpu, l.output, l.outputs);
#جے_انت
    إذا (l.باہرلا_w && l.باہرلا_h && l.باہرلا_c){
        أرجع float_منزل_image(l.باہرلا_w, l.باہرلا_h, l.باہرلا_c, l.output);
    }
    image def = {0};
    أرجع def;
}

image get_جال_image(جال *net)
{
    صحيح i;
    لكل(i = net->n-1; i >= 0; --i){
        image m = get_جال_image_layer(net, i);
        إذا(m.h != 0) أرجع m;
    }
    image def = {0};
    أرجع def;
}

فراغ visualize_جال(جال *net)
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

فراغ چوٹی_predictions(جال *net, صحيح k, صحيح *اشاریہ)
{
    چوٹی_k(net->output, net->outputs, k, اشاریہ);
}


عائم *جال_predict(جال *net, عائم *داخلہ)
{
    جال orig = *net;
    net->داخلہ = داخلہ;
    net->truth = 0;
    net->train = 0;
    net->delta = 0;
    forward_جال(net);
    عائم *باہرلا = net->output;
    *net = orig;
    أرجع باہرلا;
}

صحيح عدد_detections(جال *net, عائم thresh)
{
    صحيح i;
    صحيح s = 0;
    لكل(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        إذا(l.type == YOLO){
            s += yolo_عدد_detections(l, thresh);
        }
        إذا(l.type == DETECTION || l.type == REGION){
            s += l.w*l.h*l.n;
        }
    }
    أرجع s;
}

detection *make_جال_boxes(جال *net, عائم thresh, صحيح *عدد)
{
    layer l = net->layers[net->n - 1];
    صحيح i;
    صحيح nboxes = عدد_detections(net, thresh);
    إذا(عدد) *عدد = nboxes;
    detection *dets = صفر_حافظہ_دیو(nboxes, حجم(detection));
    لكل(i = 0; i < nboxes; ++i){
        dets[i].prob = صفر_حافظہ_دیو(l.classes, حجم(عائم));
        إذا(l.coords > 4){
            dets[i].نقاب = صفر_حافظہ_دیو(l.coords-4, حجم(عائم));
        }
    }
    أرجع dets;
}

فراغ fill_جال_boxes(جال *net, صحيح w, صحيح h, عائم thresh, عائم hier, صحيح *map, صحيح relative, detection *dets)
{
    صحيح j;
    لكل(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        إذا(l.type == YOLO){
            صحيح گنتی = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
            dets += گنتی;
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

detection *get_جال_boxes(جال *net, صحيح w, صحيح h, عائم thresh, عائم hier, صحيح *map, صحيح relative, صحيح *عدد)
{
    detection *dets = make_جال_boxes(net, thresh, عدد);
    fill_جال_boxes(net, w, h, thresh, hier, map, relative, dets);
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

عائم *جال_predict_image(جال *net, image im)
{
    image imr = letterbox_image(im, net->w, net->h);
    سیٹ_batch_جال(net, 1);
    عائم *p = جال_predict(net, imr.انکڑے);
    free_image(imr);
    أرجع p;
}

صحيح جال_چوڑائی(جال *net){أرجع net->w;}
صحيح جال_اچائی(جال *net){أرجع net->h;}

میٹرکس جال_predict_انکڑے_multi(جال *net, انکڑے test, صحيح n)
{
    صحيح i,j,b,m;
    صحيح k = net->outputs;
    میٹرکس pred = make_میٹرکس(test.X.صفاں, k);
    عائم *X = صفر_حافظہ_دیو(net->batch*test.X.صفاں, حجم(عائم));
    لكل(i = 0; i < test.X.صفاں; i += net->batch){
        لكل(b = 0; b < net->batch; ++b){
            إذا(i+b == test.X.صفاں) اكسر;
            حافظہ_نقل(X+b*test.X.خانے_گنتی, test.X.vals[i+b], test.X.خانے_گنتی*حجم(عائم));
        }
        لكل(m = 0; m < n; ++m){
            عائم *باہرلا = جال_predict(net, X);
            لكل(b = 0; b < net->batch; ++b){
                إذا(i+b == test.X.صفاں) اكسر;
                لكل(j = 0; j < k; ++j){
                    pred.vals[i+b][j] += باہرلا[j+b*k]/n;
                }
            }
        }
    }
    حرر(X);
    أرجع pred;   
}

میٹرکس جال_predict_انکڑے(جال *net, انکڑے test)
{
    صحيح i,j,b;
    صحيح k = net->outputs;
    میٹرکس pred = make_میٹرکس(test.X.صفاں, k);
    عائم *X = صفر_حافظہ_دیو(net->batch*test.X.خانے_گنتی, حجم(عائم));
    لكل(i = 0; i < test.X.صفاں; i += net->batch){
        لكل(b = 0; b < net->batch; ++b){
            إذا(i+b == test.X.صفاں) اكسر;
            حافظہ_نقل(X+b*test.X.خانے_گنتی, test.X.vals[i+b], test.X.خانے_گنتی*حجم(عائم));
        }
        عائم *باہرلا = جال_predict(net, X);
        لكل(b = 0; b < net->batch; ++b){
            إذا(i+b == test.X.صفاں) اكسر;
            لكل(j = 0; j < k; ++j){
                pred.vals[i+b][j] = باہرلا[j+b*k];
            }
        }
    }
    حرر(X);
    أرجع pred;   
}

فراغ print_جال(جال *net)
{
    صحيح i,j;
    لكل(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        عائم *output = l.output;
        صحيح n = l.outputs;
        عائم اوسط = اوسط_array(output, n);
        عائم vari = variance_array(output, n);
        fprintf(stderr, "Layer %d - Mean: %f, Variance: %f\n",i,اوسط, vari);
        إذا(n > 100) n = 100;
        لكل(j = 0; j < n; ++j) fprintf(stderr, "%f, ", output[j]);
        إذا(n == 100)fprintf(stderr,".....\n");
        fprintf(stderr, "\n");
    }
}

فراغ compare_networks(جال *n1, جال *n2, انکڑے test)
{
    میٹرکس g1 = جال_predict_انکڑے(n1, test);
    میٹرکس g2 = جال_predict_انکڑے(n2, test);
    صحيح i;
    صحيح a,b,c,d;
    a = b = c = d = 0;
    لكل(i = 0; i < g1.صفاں; ++i){
        صحيح truth = max_اشاریہ(test.y.vals[i], test.y.خانے_گنتی);
        صحيح p1 = max_اشاریہ(g1.vals[i], g1.خانے_گنتی);
        صحيح p2 = max_اشاریہ(g2.vals[i], g2.خانے_گنتی);
        إذا(p1 == truth){
            إذا(p2 == truth) ++d;
            وإلا ++c;
        }وإلا{
            إذا(p2 == truth) ++b;
            وإلا ++a;
        }
    }
    لکھو("%5d %5d\n%5d %5d\n", a, b, c, d);
    عائم عدد = قوت((مطلق_قدر(b - c) - 1.), 2.);
    عائم den = b + c;
    لکھو("%f\n", عدد/den); 
}

عائم جال_accuracy(جال *net, انکڑے d)
{
    میٹرکس guess = جال_predict_انکڑے(net, d);
    عائم acc = میٹرکس_topk_accuracy(d.y, guess,1);
    free_میٹرکس(guess);
    أرجع acc;
}

عائم *جال_accuracies(جال *net, انکڑے d, صحيح n)
{
    ساكن عائم acc[2];
    میٹرکس guess = جال_predict_انکڑے(net, d);
    acc[0] = میٹرکس_topk_accuracy(d.y, guess, 1);
    acc[1] = میٹرکس_topk_accuracy(d.y, guess, n);
    free_میٹرکس(guess);
    أرجع acc;
}

layer get_جال_output_layer(جال *net)
{
    صحيح i;
    لكل(i = net->n - 1; i >= 0; --i){
        إذا(net->layers[i].type != لاگت) اكسر;
    }
    أرجع net->layers[i];
}

عائم جال_accuracy_multi(جال *net, انکڑے d, صحيح n)
{
    میٹرکس guess = جال_predict_انکڑے_multi(net, d, n);
    عائم acc = میٹرکس_topk_accuracy(d.y, guess,1);
    free_میٹرکس(guess);
    أرجع acc;
}

فراغ free_جال(جال *net)
{
    صحيح i;
    لكل(i = 0; i < net->n; ++i){
        free_layer(net->layers[i]);
    }
    حرر(net->layers);
    إذا(net->داخلہ) حرر(net->داخلہ);
    إذا(net->truth) حرر(net->truth);
#جے_تعریف_شدہ GPU
    إذا(net->داخلہ_gpu) cuda_free(net->داخلہ_gpu);
    إذا(net->truth_gpu) cuda_free(net->truth_gpu);
#جے_انت
    حرر(net);
}

// Some day...
// ^ What the hell is this comment for?


layer جال_output_layer(جال *net)
{
    صحيح i;
    لكل(i = net->n - 1; i >= 0; --i){
        إذا(net->layers[i].type != لاگت) اكسر;
    }
    أرجع net->layers[i];
}

صحيح جال_inputs(جال *net)
{
    أرجع net->layers[0].inputs;
}

صحيح جال_outputs(جال *net)
{
    أرجع جال_output_layer(net).outputs;
}

عائم *جال_output(جال *net)
{
    أرجع جال_output_layer(net).output;
}

#جے_تعریف_شدہ GPU

فراغ forward_جال_gpu(جال *netp)
{
    جال net = *netp;
    cuda_سیٹ_device(net.gpu_اشاریہ);
    cuda_ڈالو_array(net.داخلہ_gpu, net.داخلہ, net.inputs*net.batch);
    إذا(net.truth){
        cuda_ڈالو_array(net.truth_gpu, net.truth, net.truths*net.batch);
    }

    صحيح i;
    لكل(i = 0; i < net.n; ++i){
        net.اشاریہ = i;
        layer l = net.layers[i];
        إذا(l.delta_gpu){
            fill_gpu(l.outputs * l.batch, 0, l.delta_gpu, 1);
        }
        l.forward_gpu(l, net);
        net.داخلہ_gpu = l.output_gpu;
        net.داخلہ = l.output;
        إذا(l.truth) {
            net.truth_gpu = l.output_gpu;
            net.truth = l.output;
        }
    }
    pull_جال_output(netp);
    calc_جال_لاگت(netp);
}

فراغ backward_جال_gpu(جال *netp)
{
    صحيح i;
    جال net = *netp;
    جال orig = net;
    cuda_سیٹ_device(net.gpu_اشاریہ);
    لكل(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        إذا(l.stopbackward) اكسر;
        إذا(i == 0){
            net = orig;
        }وإلا{
            layer prev = net.layers[i-1];
            net.داخلہ = prev.output;
            net.delta = prev.delta;
            net.داخلہ_gpu = prev.output_gpu;
            net.delta_gpu = prev.delta_gpu;
        }
        net.اشاریہ = i;
        l.backward_gpu(l, net);
    }
}

فراغ update_جال_gpu(جال *netp)
{
    جال net = *netp;
    cuda_سیٹ_device(net.gpu_اشاریہ);
    صحيح i;
    update_دلیلاں a = {0};
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

فراغ harmless_update_جال_gpu(جال *netp)
{
    جال net = *netp;
    cuda_سیٹ_device(net.gpu_اشاریہ);
    صحيح i;
    لكل(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        إذا(l.بھار_updates_gpu) fill_gpu(l.nweights, 0, l.بھار_updates_gpu, 1);
        إذا(l.bias_updates_gpu) fill_gpu(l.nbiases, 0, l.bias_updates_gpu, 1);
        إذا(l.scale_updates_gpu) fill_gpu(l.nbiases, 0, l.scale_updates_gpu, 1);
    }
}

تعريفنوع بنية {
    جال *net;
    انکڑے d;
    عائم *err;
} train_دلیلاں;

فراغ *train_thread(فراغ *ptr)
{
    train_دلیلاں دلیلاں = *(train_دلیلاں*)ptr;
    حرر(ptr);
    cuda_سیٹ_device(دلیلاں.net->gpu_اشاریہ);
    *دلیلاں.err = train_جال(دلیلاں.net, دلیلاں.d);
    أرجع 0;
}

pthread_t train_جال_وچ_thread(جال *net, انکڑے d, عائم *err)
{
    pthread_t thread;
    train_دلیلاں *ptr = (train_دلیلاں *)صفر_حافظہ_دیو(1, حجم(train_دلیلاں));
    ptr->net = net;
    ptr->d = d;
    ptr->err = err;
    إذا(pthread_create(&thread, 0, train_thread, ptr)) error("Thread creation failed");
    أرجع thread;
}

فراغ ملاؤ_weights(layer l, layer ادھار)
{
    إذا (l.type == CONVOLUTIONAL) {
        axpy_cpu(l.n, 1, l.bias_updates, 1, ادھار.biases, 1);
        axpy_cpu(l.nweights, 1, l.بھار_updates, 1, ادھار.weights, 1);
        إذا (l.scales) {
            axpy_cpu(l.n, 1, l.scale_updates, 1, ادھار.scales, 1);
        }
    } وإلا إذا(l.type == CONNECTED) {
        axpy_cpu(l.outputs, 1, l.bias_updates, 1, ادھار.biases, 1);
        axpy_cpu(l.outputs*l.inputs, 1, l.بھار_updates, 1, ادھار.weights, 1);
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
        cuda_pull_array(l.weights_gpu, l.بھار_updates, l.nweights);
        إذا(l.scales) cuda_pull_array(l.scales_gpu, l.scale_updates, l.n);
    } وإلا إذا(l.type == CONNECTED){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.outputs);
        cuda_pull_array(l.weights_gpu, l.بھار_updates, l.outputs*l.inputs);
    }
}

فراغ ڈالو_weights(layer l)
{
    إذا(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_ڈالو_array(l.biases_gpu, l.biases, l.n);
        cuda_ڈالو_array(l.weights_gpu, l.weights, l.nweights);
        إذا(l.scales) cuda_ڈالو_array(l.scales_gpu, l.scales, l.n);
    } وإلا إذا(l.type == CONNECTED){
        cuda_ڈالو_array(l.biases_gpu, l.biases, l.outputs);
        cuda_ڈالو_array(l.weights_gpu, l.weights, l.outputs*l.inputs);
    }
}

فراغ distribute_weights(layer l, layer ادھار)
{
    إذا (l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL) {
        cuda_ڈالو_array(l.biases_gpu, ادھار.biases, l.n);
        cuda_ڈالو_array(l.weights_gpu, ادھار.weights, l.nweights);
        إذا (ادھار.scales) cuda_ڈالو_array(l.scales_gpu, ادھار.scales, l.n);
    } وإلا إذا (l.type == CONNECTED) {
        cuda_ڈالو_array(l.biases_gpu, ادھار.biases, l.outputs);
        cuda_ڈالو_array(l.weights_gpu, ادھار.weights, l.outputs*l.inputs);
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

فراغ sync_layer(جال **nets, صحيح n, صحيح j)
{
    صحيح i;
    جال *net = nets[0];
    layer ادھار = net->layers[j];
    scale_weights(ادھار, 0);
    لكل (i = 0; i < n; ++i) {
        cuda_سیٹ_device(nets[i]->gpu_اشاریہ);
        layer l = nets[i]->layers[j];
        pull_weights(l);
        ملاؤ_weights(l, ادھار);
    }
    scale_weights(ادھار, 1./n);
    لكل (i = 0; i < n; ++i) {
        cuda_سیٹ_device(nets[i]->gpu_اشاریہ);
        layer l = nets[i]->layers[j];
        distribute_weights(l, ادھار);
    }
}

تعريفنوع بنية{
    جال **nets;
    صحيح n;
    صحيح j;
} sync_دلیلاں;

فراغ *sync_layer_thread(فراغ *ptr)
{
    sync_دلیلاں دلیلاں = *(sync_دلیلاں*)ptr;
    sync_layer(دلیلاں.nets, دلیلاں.n, دلیلاں.j);
    حرر(ptr);
    أرجع 0;
}

pthread_t sync_layer_وچ_thread(جال **nets, صحيح n, صحيح j)
{
    pthread_t thread;
    sync_دلیلاں *ptr = (sync_دلیلاں *)صفر_حافظہ_دیو(1, حجم(sync_دلیلاں));
    ptr->nets = nets;
    ptr->n = n;
    ptr->j = j;
    إذا(pthread_create(&thread, 0, sync_layer_thread, ptr)) error("Thread creation failed");
    أرجع thread;
}

فراغ sync_nets(جال **nets, صحيح n, صحيح interval)
{
    صحيح j;
    صحيح layers = nets[0]->n;
    pthread_t *دھاگے = (pthread_t *) صفر_حافظہ_دیو(layers, حجم(pthread_t));

    *(nets[0]->تکیا) += interval * (n-1) * nets[0]->batch * nets[0]->subdivisions;
    لكل (j = 0; j < n; ++j){
        *(nets[j]->تکیا) = *(nets[0]->تکیا);
    }
    لكل (j = 0; j < layers; ++j) {
        دھاگے[j] = sync_layer_وچ_thread(nets, n, j);
    }
    لكل (j = 0; j < layers; ++j) {
        pthread_join(دھاگے[j], 0);
    }
    حرر(دھاگے);
}

عائم train_networks(جال **nets, صحيح n, انکڑے d, صحيح interval)
{
    صحيح i;
    صحيح batch = nets[0]->batch;
    صحيح subdivisions = nets[0]->subdivisions;
    أكد(batch * subdivisions * n == d.X.صفاں);
    pthread_t *دھاگے = (pthread_t *) صفر_حافظہ_دیو(n, حجم(pthread_t));
    عائم *errors = (عائم *) صفر_حافظہ_دیو(n, حجم(عائم));

    عائم جوڑ = 0;
    لكل(i = 0; i < n; ++i){
        انکڑے p = get_انکڑے_part(d, i, n);
        دھاگے[i] = train_جال_وچ_thread(nets[i], p, errors + i);
    }
    لكل(i = 0; i < n; ++i){
        pthread_join(دھاگے[i], 0);
        //printf("%f\n", errors[i]);
        جوڑ += errors[i];
    }
    //cudaDeviceSynchronize();
    إذا (get_current_batch(nets[0]) % interval == 0) {
        لکھو("Syncing... ");
        fflush(stdout);
        sync_nets(nets, n, interval);
        لکھو("Done!\n");
    }
    //cudaDeviceSynchronize();
    حرر(دھاگے);
    حرر(errors);
    أرجع (عائم)جوڑ/(n);
}

فراغ pull_جال_output(جال *net)
{
    layer l = get_جال_output_layer(net);
    cuda_pull_array(l.output_gpu, l.output, l.outputs*l.batch);
}

#جے_انت

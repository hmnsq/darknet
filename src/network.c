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

load_وسائط get_base_وسائط(network *net)
{
    load_وسائط وسائط = {0};
    وسائط.w = net->w;
    وسائط.h = net->h;
    وسائط.size = net->w;

    وسائط.min = net->min_crop;
    وسائط.max = net->max_crop;
    وسائط.angle = net->angle;
    وسائط.aspect = net->aspect;
    وسائط.exposure = net->exposure;
    وسائط.center = net->center;
    وسائط.saturation = net->saturation;
    وسائط.hue = net->hue;
    أرجع وسائط;
}

network *load_network(حرف *cfg, حرف *weights, صحيح clear)
{
    network *net = parse_network_cfg(cfg);
    إذا(weights && weights[0] != 0){
        load_weights(net, weights);
    }
    إذا(clear) (*net->seen) = 0;
    أرجع net;
}

size_t get_current_batch(network *net)
{
    size_t batch_رقم = (*net->seen)/(net->batch*net->subdivisions);
    أرجع batch_رقم;
}

فراغ reset_network_state(network *net, صحيح b)
{
    صحيح i;
    لكل (i = 0; i < net->n; ++i) {
        #ifdef GPU
        layer l = net->layers[i];
        إذا(l.state_gpu){
            fill_gpu(l.outputs, 0, l.state_gpu + l.outputs*b, 1);
        }
        إذا(l.h_gpu){
            fill_gpu(l.outputs, 0, l.h_gpu + l.outputs*b, 1);
        }
        #endif
    }
}

فراغ reset_rnn(network *net)
{
    reset_network_state(net, 0);
}

عائم get_current_rate(network *net)
{
    size_t batch_رقم = get_current_batch(net);
    صحيح i;
    عائم rate;
    إذا (batch_رقم < net->burn_in) أرجع net->learning_rate * pow((عائم)batch_رقم / net->burn_in, net->power);
    بدل (net->policy) {
        حالة CONSTANT:
            أرجع net->learning_rate;
        حالة STEP:
            أرجع net->learning_rate * pow(net->scale, batch_رقم/net->step);
        حالة STEPS:
            rate = net->learning_rate;
            لكل(i = 0; i < net->رقم_steps; ++i){
                إذا(net->steps[i] > batch_رقم) أرجع rate;
                rate *= net->scales[i];
            }
            أرجع rate;
        حالة EXP:
            أرجع net->learning_rate * pow(net->gamma, batch_رقم);
        حالة POLY:
            أرجع net->learning_rate * pow(1 - (عائم)batch_رقم / net->max_batches, net->power);
        حالة RANDOM:
            أرجع net->learning_rate * pow(rand_uniform(0,1), net->power);
        حالة SIG:
            أرجع net->learning_rate * (1./(1.+exp(net->gamma*(batch_رقم - net->step))));
        افتراضي:
            fprintf(stderr, "Policy is weird!\n");
            أرجع net->learning_rate;
    }
}

حرف *get_layer_string(LAYER_TYPE a)
{
    بدل(a){
        حالة CONVOLUTIONAL:
            أرجع "convolutional";
        حالة ACTIVE:
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
        حالة COST:
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

network *make_network(صحيح n)
{
    network *net = calloc(1, حجم(network));
    net->n = n;
    net->layers = calloc(net->n, حجم(layer));
    net->seen = calloc(1, حجم(size_t));
    net->t    = calloc(1, حجم(صحيح));
    net->cost = calloc(1, حجم(عائم));
    أرجع net;
}

فراغ forward_network(network *netp)
{
#ifdef GPU
    إذا(netp->gpu_index >= 0){
        forward_network_gpu(netp);   
        أرجع;
    }
#endif
    network net = *netp;
    صحيح i;
    لكل(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        إذا(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.مدخل = l.output;
        إذا(l.truth) {
            net.truth = l.output;
        }
    }
    calc_network_cost(netp);
}

فراغ update_network(network *netp)
{
#ifdef GPU
    إذا(netp->gpu_index >= 0){
        update_network_gpu(netp);   
        أرجع;
    }
#endif
    network net = *netp;
    صحيح i;
    update_وسائط a = {0};
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

    لكل(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        إذا(l.update){
            l.update(l, a);
        }
    }
}

فراغ calc_network_cost(network *netp)
{
    network net = *netp;
    صحيح i;
    عائم sum = 0;
    صحيح عدد = 0;
    لكل(i = 0; i < net.n; ++i){
        إذا(net.layers[i].cost){
            sum += net.layers[i].cost[0];
            ++عدد;
        }
    }
    *net.cost = sum/عدد;
}

صحيح get_predicted_class_network(network *net)
{
    أرجع max_index(net->output, net->outputs);
}

فراغ backward_network(network *netp)
{
#ifdef GPU
    إذا(netp->gpu_index >= 0){
        backward_network_gpu(netp);   
        أرجع;
    }
#endif
    network net = *netp;
    صحيح i;
    network orig = net;
    لكل(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        إذا(l.stopbackward) اكسر;
        إذا(i == 0){
            net = orig;
        }وإلا{
            layer prev = net.layers[i-1];
            net.مدخل = prev.output;
            net.delta = prev.delta;
        }
        net.index = i;
        l.backward(l, net);
    }
}

عائم train_network_datum(network *net)
{
    *net->seen += net->batch;
    net->train = 1;
    forward_network(net);
    backward_network(net);
    عائم error = *net->cost;
    إذا(((*net->seen)/net->batch)%net->subdivisions == 0) update_network(net);
    أرجع error;
}

عائم train_network_sgd(network *net, data d, صحيح n)
{
    صحيح batch = net->batch;

    صحيح i;
    عائم sum = 0;
    لكل(i = 0; i < n; ++i){
        get_random_batch(d, batch, net->مدخل, net->truth);
        عائم err = train_network_datum(net);
        sum += err;
    }
    أرجع (عائم)sum/(n*batch);
}

عائم train_network(network *net, data d)
{
    أكد(d.X.rows % net->batch == 0);
    صحيح batch = net->batch;
    صحيح n = d.X.rows / batch;

    صحيح i;
    عائم sum = 0;
    لكل(i = 0; i < n; ++i){
        get_next_batch(d, batch, i*batch, net->مدخل, net->truth);
        عائم err = train_network_datum(net);
        sum += err;
    }
    أرجع (عائم)sum/(n*batch);
}

فراغ set_temp_network(network *net, عائم t)
{
    صحيح i;
    لكل(i = 0; i < net->n; ++i){
        net->layers[i].temperature = t;
    }
}


فراغ set_batch_network(network *net, صحيح b)
{
    net->batch = b;
    صحيح i;
    لكل(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#ifdef CUDNN
        إذا(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_setup(net->layers + i);
        }
        إذا(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, l->out_h, l->out_w);
            cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 
        }
#endif
    }
}

صحيح resize_network(network *net, صحيح w, صحيح h)
{
#ifdef GPU
    cuda_set_device(net->gpu_index);
    cuda_free(net->workspace);
#endif
    صحيح i;
    //if(w == net->w && h == net->h) return 0;
    net->w = w;
    net->h = h;
    صحيح inputs = 0;
    size_t workspace_size = 0;
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
        }وإلا إذا(l.type == COST){
            resize_cost_layer(&l, inputs);
        }وإلا{
            error("Cannot resize this type of layer");
        }
        إذا(l.workspace_size > workspace_size) workspace_size = l.workspace_size;
        إذا(l.workspace_size > 2000000000) أكد(0);
        inputs = l.outputs;
        net->layers[i] = l;
        w = l.out_w;
        h = l.out_h;
        إذا(l.type == AVGPOOL) اكسر;
    }
    layer out = get_network_output_layer(net);
    net->inputs = net->layers[0].inputs;
    net->outputs = out.outputs;
    net->truths = out.outputs;
    إذا(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    حرر(net->مدخل);
    حرر(net->truth);
    net->مدخل = calloc(net->inputs*net->batch, حجم(عائم));
    net->truth = calloc(net->truths*net->batch, حجم(عائم));
#ifdef GPU
    إذا(gpu_index >= 0){
        cuda_free(net->مدخل_gpu);
        cuda_free(net->truth_gpu);
        net->مدخل_gpu = cuda_make_array(net->مدخل, net->inputs*net->batch);
        net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
        إذا(workspace_size){
            net->workspace = cuda_make_array(0, (workspace_size-1)/حجم(عائم)+1);
        }
    }وإلا {
        حرر(net->workspace);
        net->workspace = calloc(1, workspace_size);
    }
#وإلا
    حرر(net->workspace);
    net->workspace = calloc(1, workspace_size);
#endif
    //fprintf(stderr, " Done!\n");
    أرجع 0;
}

layer get_network_detection_layer(network *net)
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

image get_network_image_layer(network *net, صحيح i)
{
    layer l = net->layers[i];
#ifdef GPU
    //cuda_pull_array(l.output_gpu, l.output, l.outputs);
#endif
    إذا (l.out_w && l.out_h && l.out_c){
        أرجع float_to_image(l.out_w, l.out_h, l.out_c, l.output);
    }
    image def = {0};
    أرجع def;
}

image get_network_image(network *net)
{
    صحيح i;
    لكل(i = net->n-1; i >= 0; --i){
        image m = get_network_image_layer(net, i);
        إذا(m.h != 0) أرجع m;
    }
    image def = {0};
    أرجع def;
}

فراغ visualize_network(network *net)
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

فراغ قمة_predictions(network *net, صحيح k, صحيح *index)
{
    قمة_k(net->output, net->outputs, k, index);
}


عائم *network_predict(network *net, عائم *مدخل)
{
    network orig = *net;
    net->مدخل = مدخل;
    net->truth = 0;
    net->train = 0;
    net->delta = 0;
    forward_network(net);
    عائم *out = net->output;
    *net = orig;
    أرجع out;
}

صحيح رقم_detections(network *net, عائم thresh)
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

detection *make_network_boxes(network *net, عائم thresh, صحيح *رقم)
{
    layer l = net->layers[net->n - 1];
    صحيح i;
    صحيح nboxes = رقم_detections(net, thresh);
    إذا(رقم) *رقم = nboxes;
    detection *dets = calloc(nboxes, حجم(detection));
    لكل(i = 0; i < nboxes; ++i){
        dets[i].prob = calloc(l.classes, حجم(عائم));
        إذا(l.coords > 4){
            dets[i].mask = calloc(l.coords-4, حجم(عائم));
        }
    }
    أرجع dets;
}

فراغ fill_network_boxes(network *net, صحيح w, صحيح h, عائم thresh, عائم hier, صحيح *map, صحيح relative, detection *dets)
{
    صحيح j;
    لكل(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        إذا(l.type == YOLO){
            صحيح عدد = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
            dets += عدد;
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

detection *get_network_boxes(network *net, صحيح w, صحيح h, عائم thresh, عائم hier, صحيح *map, صحيح relative, صحيح *رقم)
{
    detection *dets = make_network_boxes(net, thresh, رقم);
    fill_network_boxes(net, w, h, thresh, hier, map, relative, dets);
    أرجع dets;
}

فراغ free_detections(detection *dets, صحيح n)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        حرر(dets[i].prob);
        إذا(dets[i].mask) حرر(dets[i].mask);
    }
    حرر(dets);
}

عائم *network_predict_image(network *net, image im)
{
    image imr = letterbox_image(im, net->w, net->h);
    set_batch_network(net, 1);
    عائم *p = network_predict(net, imr.data);
    free_image(imr);
    أرجع p;
}

صحيح network_width(network *net){أرجع net->w;}
صحيح network_height(network *net){أرجع net->h;}

matrix network_predict_data_multi(network *net, data test, صحيح n)
{
    صحيح i,j,b,m;
    صحيح k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    عائم *X = calloc(net->batch*test.X.rows, حجم(عائم));
    لكل(i = 0; i < test.X.rows; i += net->batch){
        لكل(b = 0; b < net->batch; ++b){
            إذا(i+b == test.X.rows) اكسر;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*حجم(عائم));
        }
        لكل(m = 0; m < n; ++m){
            عائم *out = network_predict(net, X);
            لكل(b = 0; b < net->batch; ++b){
                إذا(i+b == test.X.rows) اكسر;
                لكل(j = 0; j < k; ++j){
                    pred.vals[i+b][j] += out[j+b*k]/n;
                }
            }
        }
    }
    حرر(X);
    أرجع pred;   
}

matrix network_predict_data(network *net, data test)
{
    صحيح i,j,b;
    صحيح k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    عائم *X = calloc(net->batch*test.X.cols, حجم(عائم));
    لكل(i = 0; i < test.X.rows; i += net->batch){
        لكل(b = 0; b < net->batch; ++b){
            إذا(i+b == test.X.rows) اكسر;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*حجم(عائم));
        }
        عائم *out = network_predict(net, X);
        لكل(b = 0; b < net->batch; ++b){
            إذا(i+b == test.X.rows) اكسر;
            لكل(j = 0; j < k; ++j){
                pred.vals[i+b][j] = out[j+b*k];
            }
        }
    }
    حرر(X);
    أرجع pred;   
}

فراغ اطبع_network(network *net)
{
    صحيح i,j;
    لكل(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        عائم *output = l.output;
        صحيح n = l.outputs;
        عائم mean = mean_array(output, n);
        عائم vari = variance_array(output, n);
        fprintf(stderr, "Layer %d - Mean: %f, Variance: %f\n",i,mean, vari);
        إذا(n > 100) n = 100;
        لكل(j = 0; j < n; ++j) fprintf(stderr, "%f, ", output[j]);
        إذا(n == 100)fprintf(stderr,".....\n");
        fprintf(stderr, "\n");
    }
}

فراغ compare_networks(network *n1, network *n2, data test)
{
    matrix g1 = network_predict_data(n1, test);
    matrix g2 = network_predict_data(n2, test);
    صحيح i;
    صحيح a,b,c,d;
    a = b = c = d = 0;
    لكل(i = 0; i < g1.rows; ++i){
        صحيح truth = max_index(test.y.vals[i], test.y.cols);
        صحيح p1 = max_index(g1.vals[i], g1.cols);
        صحيح p2 = max_index(g2.vals[i], g2.cols);
        إذا(p1 == truth){
            إذا(p2 == truth) ++d;
            وإلا ++c;
        }وإلا{
            إذا(p2 == truth) ++b;
            وإلا ++a;
        }
    }
    printf("%5d %5d\n%5d %5d\n", a, b, c, d);
    عائم رقم = pow((abs(b - c) - 1.), 2.);
    عائم den = b + c;
    printf("%f\n", رقم/den); 
}

عائم network_accuracy(network *net, data d)
{
    matrix guess = network_predict_data(net, d);
    عائم acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    أرجع acc;
}

عائم *network_accuracies(network *net, data d, صحيح n)
{
    ساكن عائم acc[2];
    matrix guess = network_predict_data(net, d);
    acc[0] = matrix_topk_accuracy(d.y, guess, 1);
    acc[1] = matrix_topk_accuracy(d.y, guess, n);
    free_matrix(guess);
    أرجع acc;
}

layer get_network_output_layer(network *net)
{
    صحيح i;
    لكل(i = net->n - 1; i >= 0; --i){
        إذا(net->layers[i].type != COST) اكسر;
    }
    أرجع net->layers[i];
}

عائم network_accuracy_multi(network *net, data d, صحيح n)
{
    matrix guess = network_predict_data_multi(net, d, n);
    عائم acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    أرجع acc;
}

فراغ free_network(network *net)
{
    صحيح i;
    لكل(i = 0; i < net->n; ++i){
        free_layer(net->layers[i]);
    }
    حرر(net->layers);
    إذا(net->مدخل) حرر(net->مدخل);
    إذا(net->truth) حرر(net->truth);
#ifdef GPU
    إذا(net->مدخل_gpu) cuda_free(net->مدخل_gpu);
    إذا(net->truth_gpu) cuda_free(net->truth_gpu);
#endif
    حرر(net);
}

// Some day...
// ^ What the hell is this comment for?


layer network_output_layer(network *net)
{
    صحيح i;
    لكل(i = net->n - 1; i >= 0; --i){
        إذا(net->layers[i].type != COST) اكسر;
    }
    أرجع net->layers[i];
}

صحيح network_inputs(network *net)
{
    أرجع net->layers[0].inputs;
}

صحيح network_outputs(network *net)
{
    أرجع network_output_layer(net).outputs;
}

عائم *network_output(network *net)
{
    أرجع network_output_layer(net).output;
}

#ifdef GPU

فراغ forward_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    cuda_ادفع_array(net.مدخل_gpu, net.مدخل, net.inputs*net.batch);
    إذا(net.truth){
        cuda_ادفع_array(net.truth_gpu, net.truth, net.truths*net.batch);
    }

    صحيح i;
    لكل(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        إذا(l.delta_gpu){
            fill_gpu(l.outputs * l.batch, 0, l.delta_gpu, 1);
        }
        l.forward_gpu(l, net);
        net.مدخل_gpu = l.output_gpu;
        net.مدخل = l.output;
        إذا(l.truth) {
            net.truth_gpu = l.output_gpu;
            net.truth = l.output;
        }
    }
    pull_network_output(netp);
    calc_network_cost(netp);
}

فراغ backward_network_gpu(network *netp)
{
    صحيح i;
    network net = *netp;
    network orig = net;
    cuda_set_device(net.gpu_index);
    لكل(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        إذا(l.stopbackward) اكسر;
        إذا(i == 0){
            net = orig;
        }وإلا{
            layer prev = net.layers[i-1];
            net.مدخل = prev.output;
            net.delta = prev.delta;
            net.مدخل_gpu = prev.output_gpu;
            net.delta_gpu = prev.delta_gpu;
        }
        net.index = i;
        l.backward_gpu(l, net);
    }
}

فراغ update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    صحيح i;
    update_وسائط a = {0};
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

    لكل(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        إذا(l.update_gpu){
            l.update_gpu(l, a);
        }
    }
}

فراغ harmless_update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    صحيح i;
    لكل(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        إذا(l.weight_updates_gpu) fill_gpu(l.nweights, 0, l.weight_updates_gpu, 1);
        إذا(l.bias_updates_gpu) fill_gpu(l.nbiases, 0, l.bias_updates_gpu, 1);
        إذا(l.scale_updates_gpu) fill_gpu(l.nbiases, 0, l.scale_updates_gpu, 1);
    }
}

تعريفنوع بنية {
    network *net;
    data d;
    عائم *err;
} train_وسائط;

فراغ *train_thread(فراغ *ptr)
{
    train_وسائط وسائط = *(train_وسائط*)ptr;
    حرر(ptr);
    cuda_set_device(وسائط.net->gpu_index);
    *وسائط.err = train_network(وسائط.net, وسائط.d);
    أرجع 0;
}

pthread_t train_network_in_thread(network *net, data d, عائم *err)
{
    pthread_t thread;
    train_وسائط *ptr = (train_وسائط *)calloc(1, حجم(train_وسائط));
    ptr->net = net;
    ptr->d = d;
    ptr->err = err;
    إذا(pthread_create(&thread, 0, train_thread, ptr)) error("Thread creation failed");
    أرجع thread;
}

فراغ merge_weights(layer l, layer base)
{
    إذا (l.type == CONVOLUTIONAL) {
        axpy_cpu(l.n, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.nweights, 1, l.weight_updates, 1, base.weights, 1);
        إذا (l.scales) {
            axpy_cpu(l.n, 1, l.scale_updates, 1, base.scales, 1);
        }
    } وإلا إذا(l.type == CONNECTED) {
        axpy_cpu(l.outputs, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.outputs*l.inputs, 1, l.weight_updates, 1, base.weights, 1);
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
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.nweights);
        إذا(l.scales) cuda_pull_array(l.scales_gpu, l.scale_updates, l.n);
    } وإلا إذا(l.type == CONNECTED){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.outputs);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.outputs*l.inputs);
    }
}

فراغ ادفع_weights(layer l)
{
    إذا(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_ادفع_array(l.biases_gpu, l.biases, l.n);
        cuda_ادفع_array(l.weights_gpu, l.weights, l.nweights);
        إذا(l.scales) cuda_ادفع_array(l.scales_gpu, l.scales, l.n);
    } وإلا إذا(l.type == CONNECTED){
        cuda_ادفع_array(l.biases_gpu, l.biases, l.outputs);
        cuda_ادفع_array(l.weights_gpu, l.weights, l.outputs*l.inputs);
    }
}

فراغ distribute_weights(layer l, layer base)
{
    إذا (l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL) {
        cuda_ادفع_array(l.biases_gpu, base.biases, l.n);
        cuda_ادفع_array(l.weights_gpu, base.weights, l.nweights);
        إذا (base.scales) cuda_ادفع_array(l.scales_gpu, base.scales, l.n);
    } وإلا إذا (l.type == CONNECTED) {
        cuda_ادفع_array(l.biases_gpu, base.biases, l.outputs);
        cuda_ادفع_array(l.weights_gpu, base.weights, l.outputs*l.inputs);
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

فراغ sync_layer(network **nets, صحيح n, صحيح j)
{
    صحيح i;
    network *net = nets[0];
    layer base = net->layers[j];
    scale_weights(base, 0);
    لكل (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        pull_weights(l);
        merge_weights(l, base);
    }
    scale_weights(base, 1./n);
    لكل (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        distribute_weights(l, base);
    }
}

تعريفنوع بنية{
    network **nets;
    صحيح n;
    صحيح j;
} sync_وسائط;

فراغ *sync_layer_thread(فراغ *ptr)
{
    sync_وسائط وسائط = *(sync_وسائط*)ptr;
    sync_layer(وسائط.nets, وسائط.n, وسائط.j);
    حرر(ptr);
    أرجع 0;
}

pthread_t sync_layer_in_thread(network **nets, صحيح n, صحيح j)
{
    pthread_t thread;
    sync_وسائط *ptr = (sync_وسائط *)calloc(1, حجم(sync_وسائط));
    ptr->nets = nets;
    ptr->n = n;
    ptr->j = j;
    إذا(pthread_create(&thread, 0, sync_layer_thread, ptr)) error("Thread creation failed");
    أرجع thread;
}

فراغ sync_nets(network **nets, صحيح n, صحيح interval)
{
    صحيح j;
    صحيح layers = nets[0]->n;
    pthread_t *threads = (pthread_t *) calloc(layers, حجم(pthread_t));

    *(nets[0]->seen) += interval * (n-1) * nets[0]->batch * nets[0]->subdivisions;
    لكل (j = 0; j < n; ++j){
        *(nets[j]->seen) = *(nets[0]->seen);
    }
    لكل (j = 0; j < layers; ++j) {
        threads[j] = sync_layer_in_thread(nets, n, j);
    }
    لكل (j = 0; j < layers; ++j) {
        pthread_join(threads[j], 0);
    }
    حرر(threads);
}

عائم train_networks(network **nets, صحيح n, data d, صحيح interval)
{
    صحيح i;
    صحيح batch = nets[0]->batch;
    صحيح subdivisions = nets[0]->subdivisions;
    أكد(batch * subdivisions * n == d.X.rows);
    pthread_t *threads = (pthread_t *) calloc(n, حجم(pthread_t));
    عائم *errors = (عائم *) calloc(n, حجم(عائم));

    عائم sum = 0;
    لكل(i = 0; i < n; ++i){
        data p = get_data_part(d, i, n);
        threads[i] = train_network_in_thread(nets[i], p, errors + i);
    }
    لكل(i = 0; i < n; ++i){
        pthread_join(threads[i], 0);
        //printf("%f\n", errors[i]);
        sum += errors[i];
    }
    //cudaDeviceSynchronize();
    إذا (get_current_batch(nets[0]) % interval == 0) {
        printf("Syncing... ");
        fflush(stdout);
        sync_nets(nets, n, interval);
        printf("Done!\n");
    }
    //cudaDeviceSynchronize();
    حرر(threads);
    حرر(errors);
    أرجع (عائم)sum/(n);
}

فراغ pull_network_output(network *net)
{
    layer l = get_network_output_layer(net);
    cuda_pull_array(l.output_gpu, l.output, l.outputs*l.batch);
}

#endif

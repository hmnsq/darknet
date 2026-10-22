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
    بازگشت args;
}

network *load_network(نویسه *cfg, نویسه *weights, صحیح clear)
{
    network *net = parse_network_cfg(cfg);
    اگر(weights && weights[0] != 0){
        load_weights(net, weights);
    }
    اگر(clear) (*net->seen) = 0;
    بازگشت net;
}

size_t get_current_batch(network *net)
{
    size_t batch_رقم = (*net->seen)/(net->batch*net->subdivisions);
    بازگشت batch_رقم;
}

تهی reset_network_state(network *net, صحیح b)
{
    صحیح i;
    برای (i = 0; i < net->n; ++i) {
        #ifdef GPU
        layer l = net->layers[i];
        اگر(l.state_gpu){
            fill_gpu(l.outputs, 0, l.state_gpu + l.outputs*b, 1);
        }
        اگر(l.h_gpu){
            fill_gpu(l.outputs, 0, l.h_gpu + l.outputs*b, 1);
        }
        #endif
    }
}

تهی reset_rnn(network *net)
{
    reset_network_state(net, 0);
}

اعشاری get_current_rate(network *net)
{
    size_t batch_رقم = get_current_batch(net);
    صحیح i;
    اعشاری rate;
    اگر (batch_رقم < net->burn_in) بازگشت net->learning_rate * pow((اعشاری)batch_رقم / net->burn_in, net->power);
    انتخاب (net->policy) {
        حالت CONSTANT:
            بازگشت net->learning_rate;
        حالت STEP:
            بازگشت net->learning_rate * pow(net->scale, batch_رقم/net->step);
        حالت STEPS:
            rate = net->learning_rate;
            برای(i = 0; i < net->رقم_steps; ++i){
                اگر(net->steps[i] > batch_رقم) بازگشت rate;
                rate *= net->scales[i];
            }
            بازگشت rate;
        حالت EXP:
            بازگشت net->learning_rate * pow(net->gamma, batch_رقم);
        حالت POLY:
            بازگشت net->learning_rate * pow(1 - (اعشاری)batch_رقم / net->max_batches, net->power);
        حالت RANDOM:
            بازگشت net->learning_rate * pow(rand_uniform(0,1), net->power);
        حالت SIG:
            بازگشت net->learning_rate * (1./(1.+exp(net->gamma*(batch_رقم - net->step))));
        پیشفرض:
            fprintf(stderr, "Policy is weird!\n");
            بازگشت net->learning_rate;
    }
}

نویسه *get_layer_string(LAYER_TYPE a)
{
    انتخاب(a){
        حالت CONVOLUTIONAL:
            بازگشت "convolutional";
        حالت ACTIVE:
            بازگشت "activation";
        حالت LOCAL:
            بازگشت "local";
        حالت DECONVOLUTIONAL:
            بازگشت "deconvolutional";
        حالت CONNECTED:
            بازگشت "connected";
        حالت RNN:
            بازگشت "rnn";
        حالت GRU:
            بازگشت "gru";
        حالت LSTM:
	    بازگشت "lstm";
        حالت CRNN:
            بازگشت "crnn";
        حالت MAXPOOL:
            بازگشت "maxpool";
        حالت REORG:
            بازگشت "reorg";
        حالت AVGPOOL:
            بازگشت "avgpool";
        حالت SOFTMAX:
            بازگشت "softmax";
        حالت DETECTION:
            بازگشت "detection";
        حالت REGION:
            بازگشت "region";
        حالت YOLO:
            بازگشت "yolo";
        حالت DROPOUT:
            بازگشت "dropout";
        حالت CROP:
            بازگشت "crop";
        حالت COST:
            بازگشت "cost";
        حالت ROUTE:
            بازگشت "route";
        حالت SHORTCUT:
            بازگشت "shortcut";
        حالت NORMALIZATION:
            بازگشت "normalization";
        حالت BATCHNORM:
            بازگشت "batchnorm";
        پیشفرض:
            بشکن;
    }
    بازگشت "none";
}

network *make_network(صحیح n)
{
    network *net = calloc(1, اندازه(network));
    net->n = n;
    net->layers = calloc(net->n, اندازه(layer));
    net->seen = calloc(1, اندازه(size_t));
    net->t    = calloc(1, اندازه(صحیح));
    net->cost = calloc(1, اندازه(اعشاری));
    بازگشت net;
}

تهی forward_network(network *netp)
{
#ifdef GPU
    اگر(netp->gpu_index >= 0){
        forward_network_gpu(netp);   
        بازگشت;
    }
#endif
    network net = *netp;
    صحیح i;
    برای(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        اگر(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.درون_داد = l.output;
        اگر(l.truth) {
            net.truth = l.output;
        }
    }
    calc_network_cost(netp);
}

تهی update_network(network *netp)
{
#ifdef GPU
    اگر(netp->gpu_index >= 0){
        update_network_gpu(netp);   
        بازگشت;
    }
#endif
    network net = *netp;
    صحیح i;
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

    برای(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        اگر(l.update){
            l.update(l, a);
        }
    }
}

تهی calc_network_cost(network *netp)
{
    network net = *netp;
    صحیح i;
    اعشاری sum = 0;
    صحیح شمار = 0;
    برای(i = 0; i < net.n; ++i){
        اگر(net.layers[i].cost){
            sum += net.layers[i].cost[0];
            ++شمار;
        }
    }
    *net.cost = sum/شمار;
}

صحیح get_predicted_class_network(network *net)
{
    بازگشت max_index(net->output, net->outputs);
}

تهی backward_network(network *netp)
{
#ifdef GPU
    اگر(netp->gpu_index >= 0){
        backward_network_gpu(netp);   
        بازگشت;
    }
#endif
    network net = *netp;
    صحیح i;
    network orig = net;
    برای(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        اگر(l.stopbackward) بشکن;
        اگر(i == 0){
            net = orig;
        }وگرنه{
            layer prev = net.layers[i-1];
            net.درون_داد = prev.output;
            net.delta = prev.delta;
        }
        net.index = i;
        l.backward(l, net);
    }
}

اعشاری train_network_datum(network *net)
{
    *net->seen += net->batch;
    net->train = 1;
    forward_network(net);
    backward_network(net);
    اعشاری error = *net->cost;
    اگر(((*net->seen)/net->batch)%net->subdivisions == 0) update_network(net);
    بازگشت error;
}

اعشاری train_network_sgd(network *net, data d, صحیح n)
{
    صحیح batch = net->batch;

    صحیح i;
    اعشاری sum = 0;
    برای(i = 0; i < n; ++i){
        get_random_batch(d, batch, net->درون_داد, net->truth);
        اعشاری err = train_network_datum(net);
        sum += err;
    }
    بازگشت (اعشاری)sum/(n*batch);
}

اعشاری train_network(network *net, data d)
{
    أكد(d.X.rows % net->batch == 0);
    صحیح batch = net->batch;
    صحیح n = d.X.rows / batch;

    صحیح i;
    اعشاری sum = 0;
    برای(i = 0; i < n; ++i){
        get_next_batch(d, batch, i*batch, net->درون_داد, net->truth);
        اعشاری err = train_network_datum(net);
        sum += err;
    }
    بازگشت (اعشاری)sum/(n*batch);
}

تهی set_temp_network(network *net, اعشاری t)
{
    صحیح i;
    برای(i = 0; i < net->n; ++i){
        net->layers[i].temperature = t;
    }
}


تهی set_batch_network(network *net, صحیح b)
{
    net->batch = b;
    صحیح i;
    برای(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#ifdef CUDNN
        اگر(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_setup(net->layers + i);
        }
        اگر(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, l->out_h, l->out_w);
            cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 
        }
#endif
    }
}

صحیح resize_network(network *net, صحیح w, صحیح h)
{
#ifdef GPU
    cuda_set_device(net->gpu_index);
    cuda_free(net->workspace);
#endif
    صحیح i;
    //if(w == net->w && h == net->h) return 0;
    net->w = w;
    net->h = h;
    صحیح inputs = 0;
    size_t workspace_size = 0;
    //fprintf(stderr, "Resizing to %d x %d...\n", w, h);
    //fflush(stderr);
    برای (i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        اگر(l.type == CONVOLUTIONAL){
            resize_convolutional_layer(&l, w, h);
        }وگرنه اگر(l.type == CROP){
            resize_crop_layer(&l, w, h);
        }وگرنه اگر(l.type == MAXPOOL){
            resize_maxpool_layer(&l, w, h);
        }وگرنه اگر(l.type == REGION){
            resize_region_layer(&l, w, h);
        }وگرنه اگر(l.type == YOLO){
            resize_yolo_layer(&l, w, h);
        }وگرنه اگر(l.type == ROUTE){
            resize_route_layer(&l, net);
        }وگرنه اگر(l.type == SHORTCUT){
            resize_shortcut_layer(&l, w, h);
        }وگرنه اگر(l.type == UPSAMPLE){
            resize_upsample_layer(&l, w, h);
        }وگرنه اگر(l.type == REORG){
            resize_reorg_layer(&l, w, h);
        }وگرنه اگر(l.type == AVGPOOL){
            resize_avgpool_layer(&l, w, h);
        }وگرنه اگر(l.type == NORMALIZATION){
            resize_normalization_layer(&l, w, h);
        }وگرنه اگر(l.type == COST){
            resize_cost_layer(&l, inputs);
        }وگرنه{
            error("Cannot resize this type of layer");
        }
        اگر(l.workspace_size > workspace_size) workspace_size = l.workspace_size;
        اگر(l.workspace_size > 2000000000) أكد(0);
        inputs = l.outputs;
        net->layers[i] = l;
        w = l.out_w;
        h = l.out_h;
        اگر(l.type == AVGPOOL) بشکن;
    }
    layer out = get_network_output_layer(net);
    net->inputs = net->layers[0].inputs;
    net->outputs = out.outputs;
    net->truths = out.outputs;
    اگر(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    آزاد(net->درون_داد);
    آزاد(net->truth);
    net->درون_داد = calloc(net->inputs*net->batch, اندازه(اعشاری));
    net->truth = calloc(net->truths*net->batch, اندازه(اعشاری));
#ifdef GPU
    اگر(gpu_index >= 0){
        cuda_free(net->درون_داد_gpu);
        cuda_free(net->truth_gpu);
        net->درون_داد_gpu = cuda_make_array(net->درون_داد, net->inputs*net->batch);
        net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
        اگر(workspace_size){
            net->workspace = cuda_make_array(0, (workspace_size-1)/اندازه(اعشاری)+1);
        }
    }وگرنه {
        آزاد(net->workspace);
        net->workspace = calloc(1, workspace_size);
    }
#وگرنه
    آزاد(net->workspace);
    net->workspace = calloc(1, workspace_size);
#endif
    //fprintf(stderr, " Done!\n");
    بازگشت 0;
}

layer get_network_detection_layer(network *net)
{
    صحیح i;
    برای(i = 0; i < net->n; ++i){
        اگر(net->layers[i].type == DETECTION){
            بازگشت net->layers[i];
        }
    }
    fprintf(stderr, "Detection layer not found!!\n");
    layer l = {0};
    بازگشت l;
}

image get_network_image_layer(network *net, صحیح i)
{
    layer l = net->layers[i];
#ifdef GPU
    //cuda_pull_array(l.output_gpu, l.output, l.outputs);
#endif
    اگر (l.out_w && l.out_h && l.out_c){
        بازگشت float_to_image(l.out_w, l.out_h, l.out_c, l.output);
    }
    image def = {0};
    بازگشت def;
}

image get_network_image(network *net)
{
    صحیح i;
    برای(i = net->n-1; i >= 0; --i){
        image m = get_network_image_layer(net, i);
        اگر(m.h != 0) بازگشت m;
    }
    image def = {0};
    بازگشت def;
}

تهی visualize_network(network *net)
{
    image *prev = 0;
    صحیح i;
    نویسه buff[256];
    برای(i = 0; i < net->n; ++i){
        sprintf(buff, "Layer %d", i);
        layer l = net->layers[i];
        اگر(l.type == CONVOLUTIONAL){
            prev = visualize_convolutional_layer(l, buff, prev);
        }
    } 
}

تهی بالاترین_predictions(network *net, صحیح k, صحیح *index)
{
    بالاترین_k(net->output, net->outputs, k, index);
}


اعشاری *network_predict(network *net, اعشاری *درون_داد)
{
    network orig = *net;
    net->درون_داد = درون_داد;
    net->truth = 0;
    net->train = 0;
    net->delta = 0;
    forward_network(net);
    اعشاری *out = net->output;
    *net = orig;
    بازگشت out;
}

صحیح رقم_detections(network *net, اعشاری thresh)
{
    صحیح i;
    صحیح s = 0;
    برای(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        اگر(l.type == YOLO){
            s += yolo_رقم_detections(l, thresh);
        }
        اگر(l.type == DETECTION || l.type == REGION){
            s += l.w*l.h*l.n;
        }
    }
    بازگشت s;
}

detection *make_network_boxes(network *net, اعشاری thresh, صحیح *رقم)
{
    layer l = net->layers[net->n - 1];
    صحیح i;
    صحیح nboxes = رقم_detections(net, thresh);
    اگر(رقم) *رقم = nboxes;
    detection *dets = calloc(nboxes, اندازه(detection));
    برای(i = 0; i < nboxes; ++i){
        dets[i].prob = calloc(l.classes, اندازه(اعشاری));
        اگر(l.coords > 4){
            dets[i].mask = calloc(l.coords-4, اندازه(اعشاری));
        }
    }
    بازگشت dets;
}

تهی fill_network_boxes(network *net, صحیح w, صحیح h, اعشاری thresh, اعشاری hier, صحیح *map, صحیح relative, detection *dets)
{
    صحیح j;
    برای(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        اگر(l.type == YOLO){
            صحیح شمار = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
            dets += شمار;
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

detection *get_network_boxes(network *net, صحیح w, صحیح h, اعشاری thresh, اعشاری hier, صحیح *map, صحیح relative, صحیح *رقم)
{
    detection *dets = make_network_boxes(net, thresh, رقم);
    fill_network_boxes(net, w, h, thresh, hier, map, relative, dets);
    بازگشت dets;
}

تهی free_detections(detection *dets, صحیح n)
{
    صحیح i;
    برای(i = 0; i < n; ++i){
        آزاد(dets[i].prob);
        اگر(dets[i].mask) آزاد(dets[i].mask);
    }
    آزاد(dets);
}

اعشاری *network_predict_image(network *net, image im)
{
    image imr = letterbox_image(im, net->w, net->h);
    set_batch_network(net, 1);
    اعشاری *p = network_predict(net, imr.data);
    free_image(imr);
    بازگشت p;
}

صحیح network_width(network *net){بازگشت net->w;}
صحیح network_height(network *net){بازگشت net->h;}

matrix network_predict_data_multi(network *net, data test, صحیح n)
{
    صحیح i,j,b,m;
    صحیح k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    اعشاری *X = calloc(net->batch*test.X.rows, اندازه(اعشاری));
    برای(i = 0; i < test.X.rows; i += net->batch){
        برای(b = 0; b < net->batch; ++b){
            اگر(i+b == test.X.rows) بشکن;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*اندازه(اعشاری));
        }
        برای(m = 0; m < n; ++m){
            اعشاری *out = network_predict(net, X);
            برای(b = 0; b < net->batch; ++b){
                اگر(i+b == test.X.rows) بشکن;
                برای(j = 0; j < k; ++j){
                    pred.vals[i+b][j] += out[j+b*k]/n;
                }
            }
        }
    }
    آزاد(X);
    بازگشت pred;   
}

matrix network_predict_data(network *net, data test)
{
    صحیح i,j,b;
    صحیح k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    اعشاری *X = calloc(net->batch*test.X.cols, اندازه(اعشاری));
    برای(i = 0; i < test.X.rows; i += net->batch){
        برای(b = 0; b < net->batch; ++b){
            اگر(i+b == test.X.rows) بشکن;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*اندازه(اعشاری));
        }
        اعشاری *out = network_predict(net, X);
        برای(b = 0; b < net->batch; ++b){
            اگر(i+b == test.X.rows) بشکن;
            برای(j = 0; j < k; ++j){
                pred.vals[i+b][j] = out[j+b*k];
            }
        }
    }
    آزاد(X);
    بازگشت pred;   
}

تهی print_network(network *net)
{
    صحیح i,j;
    برای(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        اعشاری *output = l.output;
        صحیح n = l.outputs;
        اعشاری mean = mean_array(output, n);
        اعشاری vari = variance_array(output, n);
        fprintf(stderr, "Layer %d - Mean: %f, Variance: %f\n",i,mean, vari);
        اگر(n > 100) n = 100;
        برای(j = 0; j < n; ++j) fprintf(stderr, "%f, ", output[j]);
        اگر(n == 100)fprintf(stderr,".....\n");
        fprintf(stderr, "\n");
    }
}

تهی compare_networks(network *n1, network *n2, data test)
{
    matrix g1 = network_predict_data(n1, test);
    matrix g2 = network_predict_data(n2, test);
    صحیح i;
    صحیح a,b,c,d;
    a = b = c = d = 0;
    برای(i = 0; i < g1.rows; ++i){
        صحیح truth = max_index(test.y.vals[i], test.y.cols);
        صحیح p1 = max_index(g1.vals[i], g1.cols);
        صحیح p2 = max_index(g2.vals[i], g2.cols);
        اگر(p1 == truth){
            اگر(p2 == truth) ++d;
            وگرنه ++c;
        }وگرنه{
            اگر(p2 == truth) ++b;
            وگرنه ++a;
        }
    }
    printf("%5d %5d\n%5d %5d\n", a, b, c, d);
    اعشاری رقم = pow((abs(b - c) - 1.), 2.);
    اعشاری den = b + c;
    printf("%f\n", رقم/den); 
}

اعشاری network_accuracy(network *net, data d)
{
    matrix guess = network_predict_data(net, d);
    اعشاری acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    بازگشت acc;
}

اعشاری *network_accuracies(network *net, data d, صحیح n)
{
    ایستا اعشاری acc[2];
    matrix guess = network_predict_data(net, d);
    acc[0] = matrix_topk_accuracy(d.y, guess, 1);
    acc[1] = matrix_topk_accuracy(d.y, guess, n);
    free_matrix(guess);
    بازگشت acc;
}

layer get_network_output_layer(network *net)
{
    صحیح i;
    برای(i = net->n - 1; i >= 0; --i){
        اگر(net->layers[i].type != COST) بشکن;
    }
    بازگشت net->layers[i];
}

اعشاری network_accuracy_multi(network *net, data d, صحیح n)
{
    matrix guess = network_predict_data_multi(net, d, n);
    اعشاری acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    بازگشت acc;
}

تهی free_network(network *net)
{
    صحیح i;
    برای(i = 0; i < net->n; ++i){
        free_layer(net->layers[i]);
    }
    آزاد(net->layers);
    اگر(net->درون_داد) آزاد(net->درون_داد);
    اگر(net->truth) آزاد(net->truth);
#ifdef GPU
    اگر(net->درون_داد_gpu) cuda_free(net->درون_داد_gpu);
    اگر(net->truth_gpu) cuda_free(net->truth_gpu);
#endif
    آزاد(net);
}

// Some day...
// ^ What the hell is this comment for?


layer network_output_layer(network *net)
{
    صحیح i;
    برای(i = net->n - 1; i >= 0; --i){
        اگر(net->layers[i].type != COST) بشکن;
    }
    بازگشت net->layers[i];
}

صحیح network_inputs(network *net)
{
    بازگشت net->layers[0].inputs;
}

صحیح network_outputs(network *net)
{
    بازگشت network_output_layer(net).outputs;
}

اعشاری *network_output(network *net)
{
    بازگشت network_output_layer(net).output;
}

#ifdef GPU

تهی forward_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    cuda_افزودن_array(net.درون_داد_gpu, net.درون_داد, net.inputs*net.batch);
    اگر(net.truth){
        cuda_افزودن_array(net.truth_gpu, net.truth, net.truths*net.batch);
    }

    صحیح i;
    برای(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        اگر(l.delta_gpu){
            fill_gpu(l.outputs * l.batch, 0, l.delta_gpu, 1);
        }
        l.forward_gpu(l, net);
        net.درون_داد_gpu = l.output_gpu;
        net.درون_داد = l.output;
        اگر(l.truth) {
            net.truth_gpu = l.output_gpu;
            net.truth = l.output;
        }
    }
    pull_network_output(netp);
    calc_network_cost(netp);
}

تهی backward_network_gpu(network *netp)
{
    صحیح i;
    network net = *netp;
    network orig = net;
    cuda_set_device(net.gpu_index);
    برای(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        اگر(l.stopbackward) بشکن;
        اگر(i == 0){
            net = orig;
        }وگرنه{
            layer prev = net.layers[i-1];
            net.درون_داد = prev.output;
            net.delta = prev.delta;
            net.درون_داد_gpu = prev.output_gpu;
            net.delta_gpu = prev.delta_gpu;
        }
        net.index = i;
        l.backward_gpu(l, net);
    }
}

تهی update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    صحیح i;
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

    برای(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        اگر(l.update_gpu){
            l.update_gpu(l, a);
        }
    }
}

تهی harmless_update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    صحیح i;
    برای(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        اگر(l.weight_updates_gpu) fill_gpu(l.nweights, 0, l.weight_updates_gpu, 1);
        اگر(l.bias_updates_gpu) fill_gpu(l.nbiases, 0, l.bias_updates_gpu, 1);
        اگر(l.scale_updates_gpu) fill_gpu(l.nbiases, 0, l.scale_updates_gpu, 1);
    }
}

تعریفنوع ساختار {
    network *net;
    data d;
    اعشاری *err;
} train_args;

تهی *train_thread(تهی *ptr)
{
    train_args args = *(train_args*)ptr;
    آزاد(ptr);
    cuda_set_device(args.net->gpu_index);
    *args.err = train_network(args.net, args.d);
    بازگشت 0;
}

pthread_t train_network_in_thread(network *net, data d, اعشاری *err)
{
    pthread_t thread;
    train_args *ptr = (train_args *)calloc(1, اندازه(train_args));
    ptr->net = net;
    ptr->d = d;
    ptr->err = err;
    اگر(pthread_create(&thread, 0, train_thread, ptr)) error("Thread creation failed");
    بازگشت thread;
}

تهی merge_weights(layer l, layer base)
{
    اگر (l.type == CONVOLUTIONAL) {
        axpy_cpu(l.n, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.nweights, 1, l.weight_updates, 1, base.weights, 1);
        اگر (l.scales) {
            axpy_cpu(l.n, 1, l.scale_updates, 1, base.scales, 1);
        }
    } وگرنه اگر(l.type == CONNECTED) {
        axpy_cpu(l.outputs, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.outputs*l.inputs, 1, l.weight_updates, 1, base.weights, 1);
    }
}

تهی scale_weights(layer l, اعشاری s)
{
    اگر (l.type == CONVOLUTIONAL) {
        scal_cpu(l.n, s, l.biases, 1);
        scal_cpu(l.nweights, s, l.weights, 1);
        اگر (l.scales) {
            scal_cpu(l.n, s, l.scales, 1);
        }
    } وگرنه اگر(l.type == CONNECTED) {
        scal_cpu(l.outputs, s, l.biases, 1);
        scal_cpu(l.outputs*l.inputs, s, l.weights, 1);
    }
}


تهی pull_weights(layer l)
{
    اگر(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.n);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.nweights);
        اگر(l.scales) cuda_pull_array(l.scales_gpu, l.scale_updates, l.n);
    } وگرنه اگر(l.type == CONNECTED){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.outputs);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.outputs*l.inputs);
    }
}

تهی افزودن_weights(layer l)
{
    اگر(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_افزودن_array(l.biases_gpu, l.biases, l.n);
        cuda_افزودن_array(l.weights_gpu, l.weights, l.nweights);
        اگر(l.scales) cuda_افزودن_array(l.scales_gpu, l.scales, l.n);
    } وگرنه اگر(l.type == CONNECTED){
        cuda_افزودن_array(l.biases_gpu, l.biases, l.outputs);
        cuda_افزودن_array(l.weights_gpu, l.weights, l.outputs*l.inputs);
    }
}

تهی distribute_weights(layer l, layer base)
{
    اگر (l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL) {
        cuda_افزودن_array(l.biases_gpu, base.biases, l.n);
        cuda_افزودن_array(l.weights_gpu, base.weights, l.nweights);
        اگر (base.scales) cuda_افزودن_array(l.scales_gpu, base.scales, l.n);
    } وگرنه اگر (l.type == CONNECTED) {
        cuda_افزودن_array(l.biases_gpu, base.biases, l.outputs);
        cuda_افزودن_array(l.weights_gpu, base.weights, l.outputs*l.inputs);
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

تهی sync_layer(network **nets, صحیح n, صحیح j)
{
    صحیح i;
    network *net = nets[0];
    layer base = net->layers[j];
    scale_weights(base, 0);
    برای (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        pull_weights(l);
        merge_weights(l, base);
    }
    scale_weights(base, 1./n);
    برای (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        distribute_weights(l, base);
    }
}

تعریفنوع ساختار{
    network **nets;
    صحیح n;
    صحیح j;
} sync_args;

تهی *sync_layer_thread(تهی *ptr)
{
    sync_args args = *(sync_args*)ptr;
    sync_layer(args.nets, args.n, args.j);
    آزاد(ptr);
    بازگشت 0;
}

pthread_t sync_layer_in_thread(network **nets, صحیح n, صحیح j)
{
    pthread_t thread;
    sync_args *ptr = (sync_args *)calloc(1, اندازه(sync_args));
    ptr->nets = nets;
    ptr->n = n;
    ptr->j = j;
    اگر(pthread_create(&thread, 0, sync_layer_thread, ptr)) error("Thread creation failed");
    بازگشت thread;
}

تهی sync_nets(network **nets, صحیح n, صحیح interval)
{
    صحیح j;
    صحیح layers = nets[0]->n;
    pthread_t *threads = (pthread_t *) calloc(layers, اندازه(pthread_t));

    *(nets[0]->seen) += interval * (n-1) * nets[0]->batch * nets[0]->subdivisions;
    برای (j = 0; j < n; ++j){
        *(nets[j]->seen) = *(nets[0]->seen);
    }
    برای (j = 0; j < layers; ++j) {
        threads[j] = sync_layer_in_thread(nets, n, j);
    }
    برای (j = 0; j < layers; ++j) {
        pthread_join(threads[j], 0);
    }
    آزاد(threads);
}

اعشاری train_networks(network **nets, صحیح n, data d, صحیح interval)
{
    صحیح i;
    صحیح batch = nets[0]->batch;
    صحیح subdivisions = nets[0]->subdivisions;
    أكد(batch * subdivisions * n == d.X.rows);
    pthread_t *threads = (pthread_t *) calloc(n, اندازه(pthread_t));
    اعشاری *errors = (اعشاری *) calloc(n, اندازه(اعشاری));

    اعشاری sum = 0;
    برای(i = 0; i < n; ++i){
        data p = get_data_part(d, i, n);
        threads[i] = train_network_in_thread(nets[i], p, errors + i);
    }
    برای(i = 0; i < n; ++i){
        pthread_join(threads[i], 0);
        //printf("%f\n", errors[i]);
        sum += errors[i];
    }
    //cudaDeviceSynchronize();
    اگر (get_current_batch(nets[0]) % interval == 0) {
        printf("Syncing... ");
        fflush(stdout);
        sync_nets(nets, n, interval);
        printf("Done!\n");
    }
    //cudaDeviceSynchronize();
    آزاد(threads);
    آزاد(errors);
    بازگشت (اعشاری)sum/(n);
}

تهی pull_network_output(network *net)
{
    layer l = get_network_output_layer(net);
    cuda_pull_array(l.output_gpu, l.output, l.outputs*l.batch);
}

#endif

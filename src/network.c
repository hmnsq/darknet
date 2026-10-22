#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <time.h>
#அந்தர்பாவ <assert.h>
#அந்தர்பாவ "network.h"
#அந்தர்பாவ "image.h"
#அந்தர்பாவ "data.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "blas.h"

#அந்தர்பாவ "crop_layer.h"
#அந்தர்பாவ "connected_layer.h"
#அந்தர்பாவ "gru_layer.h"
#அந்தர்பாவ "rnn_layer.h"
#அந்தர்பாவ "crnn_layer.h"
#அந்தர்பாவ "local_layer.h"
#அந்தர்பாவ "convolutional_layer.h"
#அந்தர்பாவ "activation_layer.h"
#அந்தர்பாவ "detection_layer.h"
#அந்தர்பாவ "region_layer.h"
#அந்தர்பாவ "yolo_layer.h"
#அந்தர்பாவ "normalization_layer.h"
#அந்தர்பாவ "batchnorm_layer.h"
#அந்தர்பாவ "maxpool_layer.h"
#அந்தர்பாவ "reorg_layer.h"
#அந்தர்பாவ "avgpool_layer.h"
#அந்தர்பாவ "cost_layer.h"
#அந்தர்பாவ "softmax_layer.h"
#அந்தர்பாவ "dropout_layer.h"
#அந்தர்பாவ "route_layer.h"
#அந்தர்பாவ "upsample_layer.h"
#அந்தர்பாவ "shortcut_layer.h"
#அந்தர்பாவ "parser.h"
#அந்தர்பாவ "data.h"

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
    திரும்பு args;
}

network *load_network(எழுத்து *cfg, எழுத்து *weights, முழு clear)
{
    network *net = parse_network_cfg(cfg);
    எனில்(weights && weights[0] != 0){
        load_weights(net, weights);
    }
    எனில்(clear) (*net->seen) = 0;
    திரும்பு net;
}

size_t get_current_batch(network *net)
{
    size_t batch_இலக்கம் = (*net->seen)/(net->batch*net->subdivisions);
    திரும்பு batch_இலக்கம்;
}

வெறுமை reset_network_state(network *net, முழு b)
{
    முழு i;
    க்கு (i = 0; i < net->n; ++i) {
        #ifdef GPU
        layer l = net->layers[i];
        எனில்(l.state_gpu){
            fill_gpu(l.outputs, 0, l.state_gpu + l.outputs*b, 1);
        }
        எனில்(l.h_gpu){
            fill_gpu(l.outputs, 0, l.h_gpu + l.outputs*b, 1);
        }
        #endif
    }
}

வெறுமை reset_rnn(network *net)
{
    reset_network_state(net, 0);
}

தசம get_current_rate(network *net)
{
    size_t batch_இலக்கம் = get_current_batch(net);
    முழு i;
    தசம rate;
    எனில் (batch_இலக்கம் < net->burn_in) திரும்பு net->learning_rate * pow((தசம)batch_இலக்கம் / net->burn_in, net->power);
    தேர்வு (net->policy) {
        நிலை CONSTANT:
            திரும்பு net->learning_rate;
        நிலை STEP:
            திரும்பு net->learning_rate * pow(net->scale, batch_இலக்கம்/net->step);
        நிலை STEPS:
            rate = net->learning_rate;
            க்கு(i = 0; i < net->இலக்கம்_steps; ++i){
                எனில்(net->steps[i] > batch_இலக்கம்) திரும்பு rate;
                rate *= net->scales[i];
            }
            திரும்பு rate;
        நிலை EXP:
            திரும்பு net->learning_rate * pow(net->gamma, batch_இலக்கம்);
        நிலை POLY:
            திரும்பு net->learning_rate * pow(1 - (தசம)batch_இலக்கம் / net->max_batches, net->power);
        நிலை RANDOM:
            திரும்பு net->learning_rate * pow(rand_uniform(0,1), net->power);
        நிலை SIG:
            திரும்பு net->learning_rate * (1./(1.+exp(net->gamma*(batch_இலக்கம் - net->step))));
        இயல்பு:
            fprintf(stderr, "Policy is weird!\n");
            திரும்பு net->learning_rate;
    }
}

எழுத்து *get_layer_string(LAYER_TYPE a)
{
    தேர்வு(a){
        நிலை CONVOLUTIONAL:
            திரும்பு "convolutional";
        நிலை ACTIVE:
            திரும்பு "activation";
        நிலை LOCAL:
            திரும்பு "local";
        நிலை DECONVOLUTIONAL:
            திரும்பு "deconvolutional";
        நிலை CONNECTED:
            திரும்பு "connected";
        நிலை RNN:
            திரும்பு "rnn";
        நிலை GRU:
            திரும்பு "gru";
        நிலை LSTM:
	    திரும்பு "lstm";
        நிலை CRNN:
            திரும்பு "crnn";
        நிலை MAXPOOL:
            திரும்பு "maxpool";
        நிலை REORG:
            திரும்பு "reorg";
        நிலை AVGPOOL:
            திரும்பு "avgpool";
        நிலை SOFTMAX:
            திரும்பு "softmax";
        நிலை DETECTION:
            திரும்பு "detection";
        நிலை REGION:
            திரும்பு "region";
        நிலை YOLO:
            திரும்பு "yolo";
        நிலை DROPOUT:
            திரும்பு "dropout";
        நிலை CROP:
            திரும்பு "crop";
        நிலை COST:
            திரும்பு "cost";
        நிலை ROUTE:
            திரும்பு "route";
        நிலை SHORTCUT:
            திரும்பு "shortcut";
        நிலை NORMALIZATION:
            திரும்பு "normalization";
        நிலை BATCHNORM:
            திரும்பு "batchnorm";
        இயல்பு:
            நிறுத்து;
    }
    திரும்பு "none";
}

network *make_network(முழு n)
{
    network *net = calloc(1, அளவு(network));
    net->n = n;
    net->layers = calloc(net->n, அளவு(layer));
    net->seen = calloc(1, அளவு(size_t));
    net->t    = calloc(1, அளவு(முழு));
    net->cost = calloc(1, அளவு(தசம));
    திரும்பு net;
}

வெறுமை forward_network(network *netp)
{
#ifdef GPU
    எனில்(netp->gpu_index >= 0){
        forward_network_gpu(netp);   
        திரும்பு;
    }
#endif
    network net = *netp;
    முழு i;
    க்கு(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        எனில்(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.உள்ளீடு = l.output;
        எனில்(l.truth) {
            net.truth = l.output;
        }
    }
    calc_network_cost(netp);
}

வெறுமை update_network(network *netp)
{
#ifdef GPU
    எனில்(netp->gpu_index >= 0){
        update_network_gpu(netp);   
        திரும்பு;
    }
#endif
    network net = *netp;
    முழு i;
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

    க்கு(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        எனில்(l.update){
            l.update(l, a);
        }
    }
}

வெறுமை calc_network_cost(network *netp)
{
    network net = *netp;
    முழு i;
    தசம sum = 0;
    முழு மொத்தம் = 0;
    க்கு(i = 0; i < net.n; ++i){
        எனில்(net.layers[i].cost){
            sum += net.layers[i].cost[0];
            ++மொத்தம்;
        }
    }
    *net.cost = sum/மொத்தம்;
}

முழு get_predicted_class_network(network *net)
{
    திரும்பு max_index(net->output, net->outputs);
}

வெறுமை backward_network(network *netp)
{
#ifdef GPU
    எனில்(netp->gpu_index >= 0){
        backward_network_gpu(netp);   
        திரும்பு;
    }
#endif
    network net = *netp;
    முழு i;
    network orig = net;
    க்கு(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        எனில்(l.stopbackward) நிறுத்து;
        எனில்(i == 0){
            net = orig;
        }இல்லையெனில்{
            layer prev = net.layers[i-1];
            net.உள்ளீடு = prev.output;
            net.delta = prev.delta;
        }
        net.index = i;
        l.backward(l, net);
    }
}

தசம train_network_datum(network *net)
{
    *net->seen += net->batch;
    net->train = 1;
    forward_network(net);
    backward_network(net);
    தசம error = *net->cost;
    எனில்(((*net->seen)/net->batch)%net->subdivisions == 0) update_network(net);
    திரும்பு error;
}

தசம train_network_sgd(network *net, data d, முழு n)
{
    முழு batch = net->batch;

    முழு i;
    தசம sum = 0;
    க்கு(i = 0; i < n; ++i){
        get_random_batch(d, batch, net->உள்ளீடு, net->truth);
        தசம err = train_network_datum(net);
        sum += err;
    }
    திரும்பு (தசம)sum/(n*batch);
}

தசம train_network(network *net, data d)
{
    ப்ரதிஜ்ஞா(d.X.rows % net->batch == 0);
    முழு batch = net->batch;
    முழு n = d.X.rows / batch;

    முழு i;
    தசம sum = 0;
    க்கு(i = 0; i < n; ++i){
        get_next_batch(d, batch, i*batch, net->உள்ளீடு, net->truth);
        தசம err = train_network_datum(net);
        sum += err;
    }
    திரும்பு (தசம)sum/(n*batch);
}

வெறுமை set_temp_network(network *net, தசம t)
{
    முழு i;
    க்கு(i = 0; i < net->n; ++i){
        net->layers[i].temperature = t;
    }
}


வெறுமை set_batch_network(network *net, முழு b)
{
    net->batch = b;
    முழு i;
    க்கு(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#ifdef CUDNN
        எனில்(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_setup(net->layers + i);
        }
        எனில்(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, l->out_h, l->out_w);
            cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 
        }
#endif
    }
}

முழு resize_network(network *net, முழு w, முழு h)
{
#ifdef GPU
    cuda_set_device(net->gpu_index);
    cuda_free(net->workspace);
#endif
    முழு i;
    //if(w == net->w && h == net->h) return 0;
    net->w = w;
    net->h = h;
    முழு inputs = 0;
    size_t workspace_size = 0;
    //fprintf(stderr, "Resizing to %d x %d...\n", w, h);
    //fflush(stderr);
    க்கு (i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        எனில்(l.type == CONVOLUTIONAL){
            resize_convolutional_layer(&l, w, h);
        }இல்லையெனில் எனில்(l.type == CROP){
            resize_crop_layer(&l, w, h);
        }இல்லையெனில் எனில்(l.type == MAXPOOL){
            resize_maxpool_layer(&l, w, h);
        }இல்லையெனில் எனில்(l.type == REGION){
            resize_region_layer(&l, w, h);
        }இல்லையெனில் எனில்(l.type == YOLO){
            resize_yolo_layer(&l, w, h);
        }இல்லையெனில் எனில்(l.type == ROUTE){
            resize_route_layer(&l, net);
        }இல்லையெனில் எனில்(l.type == SHORTCUT){
            resize_shortcut_layer(&l, w, h);
        }இல்லையெனில் எனில்(l.type == UPSAMPLE){
            resize_upsample_layer(&l, w, h);
        }இல்லையெனில் எனில்(l.type == REORG){
            resize_reorg_layer(&l, w, h);
        }இல்லையெனில் எனில்(l.type == AVGPOOL){
            resize_avgpool_layer(&l, w, h);
        }இல்லையெனில் எனில்(l.type == NORMALIZATION){
            resize_normalization_layer(&l, w, h);
        }இல்லையெனில் எனில்(l.type == COST){
            resize_cost_layer(&l, inputs);
        }இல்லையெனில்{
            error("Cannot resize this type of layer");
        }
        எனில்(l.workspace_size > workspace_size) workspace_size = l.workspace_size;
        எனில்(l.workspace_size > 2000000000) ப்ரதிஜ்ஞா(0);
        inputs = l.outputs;
        net->layers[i] = l;
        w = l.out_w;
        h = l.out_h;
        எனில்(l.type == AVGPOOL) நிறுத்து;
    }
    layer out = get_network_output_layer(net);
    net->inputs = net->layers[0].inputs;
    net->outputs = out.outputs;
    net->truths = out.outputs;
    எனில்(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    விடுவி(net->உள்ளீடு);
    விடுவி(net->truth);
    net->உள்ளீடு = calloc(net->inputs*net->batch, அளவு(தசம));
    net->truth = calloc(net->truths*net->batch, அளவு(தசம));
#ifdef GPU
    எனில்(gpu_index >= 0){
        cuda_free(net->உள்ளீடு_gpu);
        cuda_free(net->truth_gpu);
        net->உள்ளீடு_gpu = cuda_make_array(net->உள்ளீடு, net->inputs*net->batch);
        net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
        எனில்(workspace_size){
            net->workspace = cuda_make_array(0, (workspace_size-1)/அளவு(தசம)+1);
        }
    }இல்லையெனில் {
        விடுவி(net->workspace);
        net->workspace = calloc(1, workspace_size);
    }
#இல்லையெனில்
    விடுவி(net->workspace);
    net->workspace = calloc(1, workspace_size);
#endif
    //fprintf(stderr, " Done!\n");
    திரும்பு 0;
}

layer get_network_detection_layer(network *net)
{
    முழு i;
    க்கு(i = 0; i < net->n; ++i){
        எனில்(net->layers[i].type == DETECTION){
            திரும்பு net->layers[i];
        }
    }
    fprintf(stderr, "Detection layer not found!!\n");
    layer l = {0};
    திரும்பு l;
}

image get_network_image_layer(network *net, முழு i)
{
    layer l = net->layers[i];
#ifdef GPU
    //cuda_pull_array(l.output_gpu, l.output, l.outputs);
#endif
    எனில் (l.out_w && l.out_h && l.out_c){
        திரும்பு float_to_image(l.out_w, l.out_h, l.out_c, l.output);
    }
    image def = {0};
    திரும்பு def;
}

image get_network_image(network *net)
{
    முழு i;
    க்கு(i = net->n-1; i >= 0; --i){
        image m = get_network_image_layer(net, i);
        எனில்(m.h != 0) திரும்பு m;
    }
    image def = {0};
    திரும்பு def;
}

வெறுமை visualize_network(network *net)
{
    image *prev = 0;
    முழு i;
    எழுத்து buff[256];
    க்கு(i = 0; i < net->n; ++i){
        sprintf(buff, "Layer %d", i);
        layer l = net->layers[i];
        எனில்(l.type == CONVOLUTIONAL){
            prev = visualize_convolutional_layer(l, buff, prev);
        }
    } 
}

வெறுமை உச்சி_predictions(network *net, முழு k, முழு *index)
{
    உச்சி_k(net->output, net->outputs, k, index);
}


தசம *network_predict(network *net, தசம *உள்ளீடு)
{
    network orig = *net;
    net->உள்ளீடு = உள்ளீடு;
    net->truth = 0;
    net->train = 0;
    net->delta = 0;
    forward_network(net);
    தசம *out = net->output;
    *net = orig;
    திரும்பு out;
}

முழு இலக்கம்_detections(network *net, தசம thresh)
{
    முழு i;
    முழு s = 0;
    க்கு(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        எனில்(l.type == YOLO){
            s += yolo_இலக்கம்_detections(l, thresh);
        }
        எனில்(l.type == DETECTION || l.type == REGION){
            s += l.w*l.h*l.n;
        }
    }
    திரும்பு s;
}

detection *make_network_boxes(network *net, தசம thresh, முழு *இலக்கம்)
{
    layer l = net->layers[net->n - 1];
    முழு i;
    முழு nboxes = இலக்கம்_detections(net, thresh);
    எனில்(இலக்கம்) *இலக்கம் = nboxes;
    detection *dets = calloc(nboxes, அளவு(detection));
    க்கு(i = 0; i < nboxes; ++i){
        dets[i].prob = calloc(l.classes, அளவு(தசம));
        எனில்(l.coords > 4){
            dets[i].mask = calloc(l.coords-4, அளவு(தசம));
        }
    }
    திரும்பு dets;
}

வெறுமை fill_network_boxes(network *net, முழு w, முழு h, தசம thresh, தசம hier, முழு *map, முழு relative, detection *dets)
{
    முழு j;
    க்கு(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        எனில்(l.type == YOLO){
            முழு மொத்தம் = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
            dets += மொத்தம்;
        }
        எனில்(l.type == REGION){
            get_region_detections(l, w, h, net->w, net->h, thresh, map, hier, relative, dets);
            dets += l.w*l.h*l.n;
        }
        எனில்(l.type == DETECTION){
            get_detection_detections(l, w, h, thresh, dets);
            dets += l.w*l.h*l.n;
        }
    }
}

detection *get_network_boxes(network *net, முழு w, முழு h, தசம thresh, தசம hier, முழு *map, முழு relative, முழு *இலக்கம்)
{
    detection *dets = make_network_boxes(net, thresh, இலக்கம்);
    fill_network_boxes(net, w, h, thresh, hier, map, relative, dets);
    திரும்பு dets;
}

வெறுமை free_detections(detection *dets, முழு n)
{
    முழு i;
    க்கு(i = 0; i < n; ++i){
        விடுவி(dets[i].prob);
        எனில்(dets[i].mask) விடுவி(dets[i].mask);
    }
    விடுவி(dets);
}

தசம *network_predict_image(network *net, image im)
{
    image imr = letterbox_image(im, net->w, net->h);
    set_batch_network(net, 1);
    தசம *p = network_predict(net, imr.data);
    free_image(imr);
    திரும்பு p;
}

முழு network_width(network *net){திரும்பு net->w;}
முழு network_height(network *net){திரும்பு net->h;}

matrix network_predict_data_multi(network *net, data test, முழு n)
{
    முழு i,j,b,m;
    முழு k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    தசம *X = calloc(net->batch*test.X.rows, அளவு(தசம));
    க்கு(i = 0; i < test.X.rows; i += net->batch){
        க்கு(b = 0; b < net->batch; ++b){
            எனில்(i+b == test.X.rows) நிறுத்து;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*அளவு(தசம));
        }
        க்கு(m = 0; m < n; ++m){
            தசம *out = network_predict(net, X);
            க்கு(b = 0; b < net->batch; ++b){
                எனில்(i+b == test.X.rows) நிறுத்து;
                க்கு(j = 0; j < k; ++j){
                    pred.vals[i+b][j] += out[j+b*k]/n;
                }
            }
        }
    }
    விடுவி(X);
    திரும்பு pred;   
}

matrix network_predict_data(network *net, data test)
{
    முழு i,j,b;
    முழு k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    தசம *X = calloc(net->batch*test.X.cols, அளவு(தசம));
    க்கு(i = 0; i < test.X.rows; i += net->batch){
        க்கு(b = 0; b < net->batch; ++b){
            எனில்(i+b == test.X.rows) நிறுத்து;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*அளவு(தசம));
        }
        தசம *out = network_predict(net, X);
        க்கு(b = 0; b < net->batch; ++b){
            எனில்(i+b == test.X.rows) நிறுத்து;
            க்கு(j = 0; j < k; ++j){
                pred.vals[i+b][j] = out[j+b*k];
            }
        }
    }
    விடுவி(X);
    திரும்பு pred;   
}

வெறுமை print_network(network *net)
{
    முழு i,j;
    க்கு(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        தசம *output = l.output;
        முழு n = l.outputs;
        தசம mean = mean_array(output, n);
        தசம vari = variance_array(output, n);
        fprintf(stderr, "Layer %d - Mean: %f, Variance: %f\n",i,mean, vari);
        எனில்(n > 100) n = 100;
        க்கு(j = 0; j < n; ++j) fprintf(stderr, "%f, ", output[j]);
        எனில்(n == 100)fprintf(stderr,".....\n");
        fprintf(stderr, "\n");
    }
}

வெறுமை compare_networks(network *n1, network *n2, data test)
{
    matrix g1 = network_predict_data(n1, test);
    matrix g2 = network_predict_data(n2, test);
    முழு i;
    முழு a,b,c,d;
    a = b = c = d = 0;
    க்கு(i = 0; i < g1.rows; ++i){
        முழு truth = max_index(test.y.vals[i], test.y.cols);
        முழு p1 = max_index(g1.vals[i], g1.cols);
        முழு p2 = max_index(g2.vals[i], g2.cols);
        எனில்(p1 == truth){
            எனில்(p2 == truth) ++d;
            இல்லையெனில் ++c;
        }இல்லையெனில்{
            எனில்(p2 == truth) ++b;
            இல்லையெனில் ++a;
        }
    }
    printf("%5d %5d\n%5d %5d\n", a, b, c, d);
    தசம இலக்கம் = pow((abs(b - c) - 1.), 2.);
    தசம den = b + c;
    printf("%f\n", இலக்கம்/den); 
}

தசம network_accuracy(network *net, data d)
{
    matrix guess = network_predict_data(net, d);
    தசம acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    திரும்பு acc;
}

தசம *network_accuracies(network *net, data d, முழு n)
{
    நிலையான தசம acc[2];
    matrix guess = network_predict_data(net, d);
    acc[0] = matrix_topk_accuracy(d.y, guess, 1);
    acc[1] = matrix_topk_accuracy(d.y, guess, n);
    free_matrix(guess);
    திரும்பு acc;
}

layer get_network_output_layer(network *net)
{
    முழு i;
    க்கு(i = net->n - 1; i >= 0; --i){
        எனில்(net->layers[i].type != COST) நிறுத்து;
    }
    திரும்பு net->layers[i];
}

தசம network_accuracy_multi(network *net, data d, முழு n)
{
    matrix guess = network_predict_data_multi(net, d, n);
    தசம acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    திரும்பு acc;
}

வெறுமை free_network(network *net)
{
    முழு i;
    க்கு(i = 0; i < net->n; ++i){
        free_layer(net->layers[i]);
    }
    விடுவி(net->layers);
    எனில்(net->உள்ளீடு) விடுவி(net->உள்ளீடு);
    எனில்(net->truth) விடுவி(net->truth);
#ifdef GPU
    எனில்(net->உள்ளீடு_gpu) cuda_free(net->உள்ளீடு_gpu);
    எனில்(net->truth_gpu) cuda_free(net->truth_gpu);
#endif
    விடுவி(net);
}

// Some day...
// ^ What the hell is this comment for?


layer network_output_layer(network *net)
{
    முழு i;
    க்கு(i = net->n - 1; i >= 0; --i){
        எனில்(net->layers[i].type != COST) நிறுத்து;
    }
    திரும்பு net->layers[i];
}

முழு network_inputs(network *net)
{
    திரும்பு net->layers[0].inputs;
}

முழு network_outputs(network *net)
{
    திரும்பு network_output_layer(net).outputs;
}

தசம *network_output(network *net)
{
    திரும்பு network_output_layer(net).output;
}

#ifdef GPU

வெறுமை forward_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    cuda_தள்ளு_array(net.உள்ளீடு_gpu, net.உள்ளீடு, net.inputs*net.batch);
    எனில்(net.truth){
        cuda_தள்ளு_array(net.truth_gpu, net.truth, net.truths*net.batch);
    }

    முழு i;
    க்கு(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        எனில்(l.delta_gpu){
            fill_gpu(l.outputs * l.batch, 0, l.delta_gpu, 1);
        }
        l.forward_gpu(l, net);
        net.உள்ளீடு_gpu = l.output_gpu;
        net.உள்ளீடு = l.output;
        எனில்(l.truth) {
            net.truth_gpu = l.output_gpu;
            net.truth = l.output;
        }
    }
    pull_network_output(netp);
    calc_network_cost(netp);
}

வெறுமை backward_network_gpu(network *netp)
{
    முழு i;
    network net = *netp;
    network orig = net;
    cuda_set_device(net.gpu_index);
    க்கு(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        எனில்(l.stopbackward) நிறுத்து;
        எனில்(i == 0){
            net = orig;
        }இல்லையெனில்{
            layer prev = net.layers[i-1];
            net.உள்ளீடு = prev.output;
            net.delta = prev.delta;
            net.உள்ளீடு_gpu = prev.output_gpu;
            net.delta_gpu = prev.delta_gpu;
        }
        net.index = i;
        l.backward_gpu(l, net);
    }
}

வெறுமை update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    முழு i;
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

    க்கு(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        எனில்(l.update_gpu){
            l.update_gpu(l, a);
        }
    }
}

வெறுமை harmless_update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    முழு i;
    க்கு(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        எனில்(l.weight_updates_gpu) fill_gpu(l.nweights, 0, l.weight_updates_gpu, 1);
        எனில்(l.bias_updates_gpu) fill_gpu(l.nbiases, 0, l.bias_updates_gpu, 1);
        எனில்(l.scale_updates_gpu) fill_gpu(l.nbiases, 0, l.scale_updates_gpu, 1);
    }
}

வகைவரை கட்டமைப்பு {
    network *net;
    data d;
    தசம *err;
} train_args;

வெறுமை *train_thread(வெறுமை *ptr)
{
    train_args args = *(train_args*)ptr;
    விடுவி(ptr);
    cuda_set_device(args.net->gpu_index);
    *args.err = train_network(args.net, args.d);
    திரும்பு 0;
}

pthread_t train_network_in_thread(network *net, data d, தசம *err)
{
    pthread_t thread;
    train_args *ptr = (train_args *)calloc(1, அளவு(train_args));
    ptr->net = net;
    ptr->d = d;
    ptr->err = err;
    எனில்(pthread_create(&thread, 0, train_thread, ptr)) error("Thread creation failed");
    திரும்பு thread;
}

வெறுமை merge_weights(layer l, layer base)
{
    எனில் (l.type == CONVOLUTIONAL) {
        axpy_cpu(l.n, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.nweights, 1, l.weight_updates, 1, base.weights, 1);
        எனில் (l.scales) {
            axpy_cpu(l.n, 1, l.scale_updates, 1, base.scales, 1);
        }
    } இல்லையெனில் எனில்(l.type == CONNECTED) {
        axpy_cpu(l.outputs, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.outputs*l.inputs, 1, l.weight_updates, 1, base.weights, 1);
    }
}

வெறுமை scale_weights(layer l, தசம s)
{
    எனில் (l.type == CONVOLUTIONAL) {
        scal_cpu(l.n, s, l.biases, 1);
        scal_cpu(l.nweights, s, l.weights, 1);
        எனில் (l.scales) {
            scal_cpu(l.n, s, l.scales, 1);
        }
    } இல்லையெனில் எனில்(l.type == CONNECTED) {
        scal_cpu(l.outputs, s, l.biases, 1);
        scal_cpu(l.outputs*l.inputs, s, l.weights, 1);
    }
}


வெறுமை pull_weights(layer l)
{
    எனில்(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.n);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.nweights);
        எனில்(l.scales) cuda_pull_array(l.scales_gpu, l.scale_updates, l.n);
    } இல்லையெனில் எனில்(l.type == CONNECTED){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.outputs);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.outputs*l.inputs);
    }
}

வெறுமை தள்ளு_weights(layer l)
{
    எனில்(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_தள்ளு_array(l.biases_gpu, l.biases, l.n);
        cuda_தள்ளு_array(l.weights_gpu, l.weights, l.nweights);
        எனில்(l.scales) cuda_தள்ளு_array(l.scales_gpu, l.scales, l.n);
    } இல்லையெனில் எனில்(l.type == CONNECTED){
        cuda_தள்ளு_array(l.biases_gpu, l.biases, l.outputs);
        cuda_தள்ளு_array(l.weights_gpu, l.weights, l.outputs*l.inputs);
    }
}

வெறுமை distribute_weights(layer l, layer base)
{
    எனில் (l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL) {
        cuda_தள்ளு_array(l.biases_gpu, base.biases, l.n);
        cuda_தள்ளு_array(l.weights_gpu, base.weights, l.nweights);
        எனில் (base.scales) cuda_தள்ளு_array(l.scales_gpu, base.scales, l.n);
    } இல்லையெனில் எனில் (l.type == CONNECTED) {
        cuda_தள்ளு_array(l.biases_gpu, base.biases, l.outputs);
        cuda_தள்ளு_array(l.weights_gpu, base.weights, l.outputs*l.inputs);
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

வெறுமை sync_layer(network **nets, முழு n, முழு j)
{
    முழு i;
    network *net = nets[0];
    layer base = net->layers[j];
    scale_weights(base, 0);
    க்கு (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        pull_weights(l);
        merge_weights(l, base);
    }
    scale_weights(base, 1./n);
    க்கு (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        distribute_weights(l, base);
    }
}

வகைவரை கட்டமைப்பு{
    network **nets;
    முழு n;
    முழு j;
} sync_args;

வெறுமை *sync_layer_thread(வெறுமை *ptr)
{
    sync_args args = *(sync_args*)ptr;
    sync_layer(args.nets, args.n, args.j);
    விடுவி(ptr);
    திரும்பு 0;
}

pthread_t sync_layer_in_thread(network **nets, முழு n, முழு j)
{
    pthread_t thread;
    sync_args *ptr = (sync_args *)calloc(1, அளவு(sync_args));
    ptr->nets = nets;
    ptr->n = n;
    ptr->j = j;
    எனில்(pthread_create(&thread, 0, sync_layer_thread, ptr)) error("Thread creation failed");
    திரும்பு thread;
}

வெறுமை sync_nets(network **nets, முழு n, முழு interval)
{
    முழு j;
    முழு layers = nets[0]->n;
    pthread_t *threads = (pthread_t *) calloc(layers, அளவு(pthread_t));

    *(nets[0]->seen) += interval * (n-1) * nets[0]->batch * nets[0]->subdivisions;
    க்கு (j = 0; j < n; ++j){
        *(nets[j]->seen) = *(nets[0]->seen);
    }
    க்கு (j = 0; j < layers; ++j) {
        threads[j] = sync_layer_in_thread(nets, n, j);
    }
    க்கு (j = 0; j < layers; ++j) {
        pthread_join(threads[j], 0);
    }
    விடுவி(threads);
}

தசம train_networks(network **nets, முழு n, data d, முழு interval)
{
    முழு i;
    முழு batch = nets[0]->batch;
    முழு subdivisions = nets[0]->subdivisions;
    ப்ரதிஜ்ஞா(batch * subdivisions * n == d.X.rows);
    pthread_t *threads = (pthread_t *) calloc(n, அளவு(pthread_t));
    தசம *errors = (தசம *) calloc(n, அளவு(தசம));

    தசம sum = 0;
    க்கு(i = 0; i < n; ++i){
        data p = get_data_part(d, i, n);
        threads[i] = train_network_in_thread(nets[i], p, errors + i);
    }
    க்கு(i = 0; i < n; ++i){
        pthread_join(threads[i], 0);
        //printf("%f\n", errors[i]);
        sum += errors[i];
    }
    //cudaDeviceSynchronize();
    எனில் (get_current_batch(nets[0]) % interval == 0) {
        printf("Syncing... ");
        fflush(stdout);
        sync_nets(nets, n, interval);
        printf("Done!\n");
    }
    //cudaDeviceSynchronize();
    விடுவி(threads);
    விடுவி(errors);
    திரும்பு (தசம)sum/(n);
}

வெறுமை pull_network_output(network *net)
{
    layer l = get_network_output_layer(net);
    cuda_pull_array(l.output_gpu, l.output, l.outputs*l.batch);
}

#endif

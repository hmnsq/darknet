#अन्तर्भाव <stdio.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव <assert.h>
#अन्तर्भाव "network.h"
#अन्तर्भाव "image.h"
#अन्तर्भाव "data.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "blas.h"

#अन्तर्भाव "crop_layer.h"
#अन्तर्भाव "connected_layer.h"
#अन्तर्भाव "gru_layer.h"
#अन्तर्भाव "rnn_layer.h"
#अन्तर्भाव "crnn_layer.h"
#अन्तर्भाव "local_layer.h"
#अन्तर्भाव "convolutional_layer.h"
#अन्तर्भाव "activation_layer.h"
#अन्तर्भाव "detection_layer.h"
#अन्तर्भाव "region_layer.h"
#अन्तर्भाव "yolo_layer.h"
#अन्तर्भाव "normalization_layer.h"
#अन्तर्भाव "batchnorm_layer.h"
#अन्तर्भाव "maxpool_layer.h"
#अन्तर्भाव "reorg_layer.h"
#अन्तर्भाव "avgpool_layer.h"
#अन्तर्भाव "cost_layer.h"
#अन्तर्भाव "softmax_layer.h"
#अन्तर्भाव "dropout_layer.h"
#अन्तर्भाव "route_layer.h"
#अन्तर्भाव "upsample_layer.h"
#अन्तर्भाव "shortcut_layer.h"
#अन्तर्भाव "parser.h"
#अन्तर्भाव "data.h"

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
    फर्काऊ args;
}

network *load_network(वर्ण *cfg, वर्ण *weights, पूर्णाङ्क clear)
{
    network *net = parse_network_cfg(cfg);
    यदि(weights && weights[0] != 0){
        load_weights(net, weights);
    }
    यदि(clear) (*net->seen) = 0;
    फर्काऊ net;
}

माप_प्रकार get_current_batch(network *net)
{
    माप_प्रकार batch_अङ्क = (*net->seen)/(net->batch*net->subdivisions);
    फर्काऊ batch_अङ्क;
}

शून्य reset_network_state(network *net, पूर्णाङ्क b)
{
    पूर्णाङ्क i;
    लागि (i = 0; i < net->n; ++i) {
        #ifdef GPU
        layer l = net->layers[i];
        यदि(l.state_gpu){
            fill_gpu(l.outputs, 0, l.state_gpu + l.outputs*b, 1);
        }
        यदि(l.h_gpu){
            fill_gpu(l.outputs, 0, l.h_gpu + l.outputs*b, 1);
        }
        #endif
    }
}

शून्य reset_rnn(network *net)
{
    reset_network_state(net, 0);
}

भिन्न get_current_rate(network *net)
{
    माप_प्रकार batch_अङ्क = get_current_batch(net);
    पूर्णाङ्क i;
    भिन्न rate;
    यदि (batch_अङ्क < net->burn_in) फर्काऊ net->learning_rate * घात((भिन्न)batch_अङ्क / net->burn_in, net->power);
    विकल्प (net->policy) {
        पक्ष CONSTANT:
            फर्काऊ net->learning_rate;
        पक्ष STEP:
            फर्काऊ net->learning_rate * घात(net->scale, batch_अङ्क/net->step);
        पक्ष STEPS:
            rate = net->learning_rate;
            लागि(i = 0; i < net->अङ्क_steps; ++i){
                यदि(net->steps[i] > batch_अङ्क) फर्काऊ rate;
                rate *= net->scales[i];
            }
            फर्काऊ rate;
        पक्ष EXP:
            फर्काऊ net->learning_rate * घात(net->gamma, batch_अङ्क);
        पक्ष POLY:
            फर्काऊ net->learning_rate * घात(1 - (भिन्न)batch_अङ्क / net->max_batches, net->power);
        पक्ष RANDOM:
            फर्काऊ net->learning_rate * घात(rand_uniform(0,1), net->power);
        पक्ष SIG:
            फर्काऊ net->learning_rate * (1./(1.+exp(net->gamma*(batch_अङ्क - net->step))));
        स्वभाव:
            ख_लिखो(मानक_त्रुटि, "Policy is weird!\n");
            फर्काऊ net->learning_rate;
    }
}

वर्ण *get_layer_string(LAYER_TYPE a)
{
    विकल्प(a){
        पक्ष CONVOLUTIONAL:
            फर्काऊ "convolutional";
        पक्ष ACTIVE:
            फर्काऊ "activation";
        पक्ष LOCAL:
            फर्काऊ "local";
        पक्ष DECONVOLUTIONAL:
            फर्काऊ "deconvolutional";
        पक्ष CONNECTED:
            फर्काऊ "connected";
        पक्ष RNN:
            फर्काऊ "rnn";
        पक्ष GRU:
            फर्काऊ "gru";
        पक्ष LSTM:
	    फर्काऊ "lstm";
        पक्ष CRNN:
            फर्काऊ "crnn";
        पक्ष MAXPOOL:
            फर्काऊ "maxpool";
        पक्ष REORG:
            फर्काऊ "reorg";
        पक्ष AVGPOOL:
            फर्काऊ "avgpool";
        पक्ष SOFTMAX:
            फर्काऊ "softmax";
        पक्ष DETECTION:
            फर्काऊ "detection";
        पक्ष REGION:
            फर्काऊ "region";
        पक्ष YOLO:
            फर्काऊ "yolo";
        पक्ष DROPOUT:
            फर्काऊ "dropout";
        पक्ष CROP:
            फर्काऊ "crop";
        पक्ष COST:
            फर्काऊ "cost";
        पक्ष ROUTE:
            फर्काऊ "route";
        पक्ष SHORTCUT:
            फर्काऊ "shortcut";
        पक्ष NORMALIZATION:
            फर्काऊ "normalization";
        पक्ष BATCHNORM:
            फर्काऊ "batchnorm";
        स्वभाव:
            रोक;
    }
    फर्काऊ "none";
}

network *make_network(पूर्णाङ्क n)
{
    network *net = सुस्मृति(1, परिमाण(network));
    net->n = n;
    net->layers = सुस्मृति(net->n, परिमाण(layer));
    net->seen = सुस्मृति(1, परिमाण(माप_प्रकार));
    net->t    = सुस्मृति(1, परिमाण(पूर्णाङ्क));
    net->cost = सुस्मृति(1, परिमाण(भिन्न));
    फर्काऊ net;
}

शून्य forward_network(network *netp)
{
#ifdef GPU
    यदि(netp->gpu_index >= 0){
        forward_network_gpu(netp);   
        फर्काऊ;
    }
#endif
    network net = *netp;
    पूर्णाङ्क i;
    लागि(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        यदि(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.आगत = l.output;
        यदि(l.truth) {
            net.truth = l.output;
        }
    }
    calc_network_cost(netp);
}

शून्य update_network(network *netp)
{
#ifdef GPU
    यदि(netp->gpu_index >= 0){
        update_network_gpu(netp);   
        फर्काऊ;
    }
#endif
    network net = *netp;
    पूर्णाङ्क i;
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

    लागि(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        यदि(l.update){
            l.update(l, a);
        }
    }
}

शून्य calc_network_cost(network *netp)
{
    network net = *netp;
    पूर्णाङ्क i;
    भिन्न sum = 0;
    पूर्णाङ्क गन्ती = 0;
    लागि(i = 0; i < net.n; ++i){
        यदि(net.layers[i].cost){
            sum += net.layers[i].cost[0];
            ++गन्ती;
        }
    }
    *net.cost = sum/गन्ती;
}

पूर्णाङ्क get_predicted_class_network(network *net)
{
    फर्काऊ max_index(net->output, net->outputs);
}

शून्य backward_network(network *netp)
{
#ifdef GPU
    यदि(netp->gpu_index >= 0){
        backward_network_gpu(netp);   
        फर्काऊ;
    }
#endif
    network net = *netp;
    पूर्णाङ्क i;
    network orig = net;
    लागि(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        यदि(l.stopbackward) रोक;
        यदि(i == 0){
            net = orig;
        }अन्यथा{
            layer prev = net.layers[i-1];
            net.आगत = prev.output;
            net.delta = prev.delta;
        }
        net.index = i;
        l.backward(l, net);
    }
}

भिन्न train_network_datum(network *net)
{
    *net->seen += net->batch;
    net->train = 1;
    forward_network(net);
    backward_network(net);
    भिन्न error = *net->cost;
    यदि(((*net->seen)/net->batch)%net->subdivisions == 0) update_network(net);
    फर्काऊ error;
}

भिन्न train_network_sgd(network *net, data d, पूर्णाङ्क n)
{
    पूर्णाङ्क batch = net->batch;

    पूर्णाङ्क i;
    भिन्न sum = 0;
    लागि(i = 0; i < n; ++i){
        get_random_batch(d, batch, net->आगत, net->truth);
        भिन्न err = train_network_datum(net);
        sum += err;
    }
    फर्काऊ (भिन्न)sum/(n*batch);
}

भिन्न train_network(network *net, data d)
{
    प्रतिज्ञा(d.X.rows % net->batch == 0);
    पूर्णाङ्क batch = net->batch;
    पूर्णाङ्क n = d.X.rows / batch;

    पूर्णाङ्क i;
    भिन्न sum = 0;
    लागि(i = 0; i < n; ++i){
        get_next_batch(d, batch, i*batch, net->आगत, net->truth);
        भिन्न err = train_network_datum(net);
        sum += err;
    }
    फर्काऊ (भिन्न)sum/(n*batch);
}

शून्य set_temp_network(network *net, भिन्न t)
{
    पूर्णाङ्क i;
    लागि(i = 0; i < net->n; ++i){
        net->layers[i].temperature = t;
    }
}


शून्य set_batch_network(network *net, पूर्णाङ्क b)
{
    net->batch = b;
    पूर्णाङ्क i;
    लागि(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#ifdef CUDNN
        यदि(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_setup(net->layers + i);
        }
        यदि(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, l->out_h, l->out_w);
            cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 
        }
#endif
    }
}

पूर्णाङ्क resize_network(network *net, पूर्णाङ्क w, पूर्णाङ्क h)
{
#ifdef GPU
    cuda_set_device(net->gpu_index);
    cuda_free(net->workspace);
#endif
    पूर्णाङ्क i;
    //if(w == net->w && h == net->h) return 0;
    net->w = w;
    net->h = h;
    पूर्णाङ्क inputs = 0;
    माप_प्रकार workspace_size = 0;
    //fprintf(stderr, "Resizing to %d x %d...\n", w, h);
    //fflush(stderr);
    लागि (i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        यदि(l.type == CONVOLUTIONAL){
            resize_convolutional_layer(&l, w, h);
        }अन्यथा यदि(l.type == CROP){
            resize_crop_layer(&l, w, h);
        }अन्यथा यदि(l.type == MAXPOOL){
            resize_maxpool_layer(&l, w, h);
        }अन्यथा यदि(l.type == REGION){
            resize_region_layer(&l, w, h);
        }अन्यथा यदि(l.type == YOLO){
            resize_yolo_layer(&l, w, h);
        }अन्यथा यदि(l.type == ROUTE){
            resize_route_layer(&l, net);
        }अन्यथा यदि(l.type == SHORTCUT){
            resize_shortcut_layer(&l, w, h);
        }अन्यथा यदि(l.type == UPSAMPLE){
            resize_upsample_layer(&l, w, h);
        }अन्यथा यदि(l.type == REORG){
            resize_reorg_layer(&l, w, h);
        }अन्यथा यदि(l.type == AVGPOOL){
            resize_avgpool_layer(&l, w, h);
        }अन्यथा यदि(l.type == NORMALIZATION){
            resize_normalization_layer(&l, w, h);
        }अन्यथा यदि(l.type == COST){
            resize_cost_layer(&l, inputs);
        }अन्यथा{
            error("Cannot resize this type of layer");
        }
        यदि(l.workspace_size > workspace_size) workspace_size = l.workspace_size;
        यदि(l.workspace_size > 2000000000) प्रतिज्ञा(0);
        inputs = l.outputs;
        net->layers[i] = l;
        w = l.out_w;
        h = l.out_h;
        यदि(l.type == AVGPOOL) रोक;
    }
    layer out = get_network_output_layer(net);
    net->inputs = net->layers[0].inputs;
    net->outputs = out.outputs;
    net->truths = out.outputs;
    यदि(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    मुक्त(net->आगत);
    मुक्त(net->truth);
    net->आगत = सुस्मृति(net->inputs*net->batch, परिमाण(भिन्न));
    net->truth = सुस्मृति(net->truths*net->batch, परिमाण(भिन्न));
#ifdef GPU
    यदि(gpu_index >= 0){
        cuda_free(net->आगत_gpu);
        cuda_free(net->truth_gpu);
        net->आगत_gpu = cuda_make_array(net->आगत, net->inputs*net->batch);
        net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
        यदि(workspace_size){
            net->workspace = cuda_make_array(0, (workspace_size-1)/परिमाण(भिन्न)+1);
        }
    }अन्यथा {
        मुक्त(net->workspace);
        net->workspace = सुस्मृति(1, workspace_size);
    }
#अन्यथा
    मुक्त(net->workspace);
    net->workspace = सुस्मृति(1, workspace_size);
#endif
    //fprintf(stderr, " Done!\n");
    फर्काऊ 0;
}

layer get_network_detection_layer(network *net)
{
    पूर्णाङ्क i;
    लागि(i = 0; i < net->n; ++i){
        यदि(net->layers[i].type == DETECTION){
            फर्काऊ net->layers[i];
        }
    }
    ख_लिखो(मानक_त्रुटि, "Detection layer not found!!\n");
    layer l = {0};
    फर्काऊ l;
}

image get_network_image_layer(network *net, पूर्णाङ्क i)
{
    layer l = net->layers[i];
#ifdef GPU
    //cuda_pull_array(l.output_gpu, l.output, l.outputs);
#endif
    यदि (l.out_w && l.out_h && l.out_c){
        फर्काऊ float_to_image(l.out_w, l.out_h, l.out_c, l.output);
    }
    image def = {0};
    फर्काऊ def;
}

image get_network_image(network *net)
{
    पूर्णाङ्क i;
    लागि(i = net->n-1; i >= 0; --i){
        image m = get_network_image_layer(net, i);
        यदि(m.h != 0) फर्काऊ m;
    }
    image def = {0};
    फर्काऊ def;
}

शून्य visualize_network(network *net)
{
    image *prev = 0;
    पूर्णाङ्क i;
    वर्ण buff[256];
    लागि(i = 0; i < net->n; ++i){
        प्र_लिखो(buff, "Layer %d", i);
        layer l = net->layers[i];
        यदि(l.type == CONVOLUTIONAL){
            prev = visualize_convolutional_layer(l, buff, prev);
        }
    } 
}

शून्य शीर्ष_predictions(network *net, पूर्णाङ्क k, पूर्णाङ्क *index)
{
    शीर्ष_k(net->output, net->outputs, k, index);
}


भिन्न *network_predict(network *net, भिन्न *आगत)
{
    network orig = *net;
    net->आगत = आगत;
    net->truth = 0;
    net->train = 0;
    net->delta = 0;
    forward_network(net);
    भिन्न *out = net->output;
    *net = orig;
    फर्काऊ out;
}

पूर्णाङ्क अङ्क_detections(network *net, भिन्न thresh)
{
    पूर्णाङ्क i;
    पूर्णाङ्क s = 0;
    लागि(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        यदि(l.type == YOLO){
            s += yolo_अङ्क_detections(l, thresh);
        }
        यदि(l.type == DETECTION || l.type == REGION){
            s += l.w*l.h*l.n;
        }
    }
    फर्काऊ s;
}

detection *make_network_boxes(network *net, भिन्न thresh, पूर्णाङ्क *अङ्क)
{
    layer l = net->layers[net->n - 1];
    पूर्णाङ्क i;
    पूर्णाङ्क nboxes = अङ्क_detections(net, thresh);
    यदि(अङ्क) *अङ्क = nboxes;
    detection *dets = सुस्मृति(nboxes, परिमाण(detection));
    लागि(i = 0; i < nboxes; ++i){
        dets[i].prob = सुस्मृति(l.classes, परिमाण(भिन्न));
        यदि(l.coords > 4){
            dets[i].mask = सुस्मृति(l.coords-4, परिमाण(भिन्न));
        }
    }
    फर्काऊ dets;
}

शून्य fill_network_boxes(network *net, पूर्णाङ्क w, पूर्णाङ्क h, भिन्न thresh, भिन्न hier, पूर्णाङ्क *map, पूर्णाङ्क relative, detection *dets)
{
    पूर्णाङ्क j;
    लागि(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        यदि(l.type == YOLO){
            पूर्णाङ्क गन्ती = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
            dets += गन्ती;
        }
        यदि(l.type == REGION){
            get_region_detections(l, w, h, net->w, net->h, thresh, map, hier, relative, dets);
            dets += l.w*l.h*l.n;
        }
        यदि(l.type == DETECTION){
            get_detection_detections(l, w, h, thresh, dets);
            dets += l.w*l.h*l.n;
        }
    }
}

detection *get_network_boxes(network *net, पूर्णाङ्क w, पूर्णाङ्क h, भिन्न thresh, भिन्न hier, पूर्णाङ्क *map, पूर्णाङ्क relative, पूर्णाङ्क *अङ्क)
{
    detection *dets = make_network_boxes(net, thresh, अङ्क);
    fill_network_boxes(net, w, h, thresh, hier, map, relative, dets);
    फर्काऊ dets;
}

शून्य free_detections(detection *dets, पूर्णाङ्क n)
{
    पूर्णाङ्क i;
    लागि(i = 0; i < n; ++i){
        मुक्त(dets[i].prob);
        यदि(dets[i].mask) मुक्त(dets[i].mask);
    }
    मुक्त(dets);
}

भिन्न *network_predict_image(network *net, image im)
{
    image imr = letterbox_image(im, net->w, net->h);
    set_batch_network(net, 1);
    भिन्न *p = network_predict(net, imr.data);
    free_image(imr);
    फर्काऊ p;
}

पूर्णाङ्क network_width(network *net){फर्काऊ net->w;}
पूर्णाङ्क network_height(network *net){फर्काऊ net->h;}

matrix network_predict_data_multi(network *net, data test, पूर्णाङ्क n)
{
    पूर्णाङ्क i,j,b,m;
    पूर्णाङ्क k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    भिन्न *X = सुस्मृति(net->batch*test.X.rows, परिमाण(भिन्न));
    लागि(i = 0; i < test.X.rows; i += net->batch){
        लागि(b = 0; b < net->batch; ++b){
            यदि(i+b == test.X.rows) रोक;
            स_नकल(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*परिमाण(भिन्न));
        }
        लागि(m = 0; m < n; ++m){
            भिन्न *out = network_predict(net, X);
            लागि(b = 0; b < net->batch; ++b){
                यदि(i+b == test.X.rows) रोक;
                लागि(j = 0; j < k; ++j){
                    pred.vals[i+b][j] += out[j+b*k]/n;
                }
            }
        }
    }
    मुक्त(X);
    फर्काऊ pred;   
}

matrix network_predict_data(network *net, data test)
{
    पूर्णाङ्क i,j,b;
    पूर्णाङ्क k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    भिन्न *X = सुस्मृति(net->batch*test.X.cols, परिमाण(भिन्न));
    लागि(i = 0; i < test.X.rows; i += net->batch){
        लागि(b = 0; b < net->batch; ++b){
            यदि(i+b == test.X.rows) रोक;
            स_नकल(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*परिमाण(भिन्न));
        }
        भिन्न *out = network_predict(net, X);
        लागि(b = 0; b < net->batch; ++b){
            यदि(i+b == test.X.rows) रोक;
            लागि(j = 0; j < k; ++j){
                pred.vals[i+b][j] = out[j+b*k];
            }
        }
    }
    मुक्त(X);
    फर्काऊ pred;   
}

शून्य print_network(network *net)
{
    पूर्णाङ्क i,j;
    लागि(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        भिन्न *output = l.output;
        पूर्णाङ्क n = l.outputs;
        भिन्न mean = mean_array(output, n);
        भिन्न vari = variance_array(output, n);
        ख_लिखो(मानक_त्रुटि, "Layer %d - Mean: %f, Variance: %f\n",i,mean, vari);
        यदि(n > 100) n = 100;
        लागि(j = 0; j < n; ++j) ख_लिखो(मानक_त्रुटि, "%f, ", output[j]);
        यदि(n == 100)ख_लिखो(मानक_त्रुटि,".....\n");
        ख_लिखो(मानक_त्रुटि, "\n");
    }
}

शून्य compare_networks(network *n1, network *n2, data test)
{
    matrix g1 = network_predict_data(n1, test);
    matrix g2 = network_predict_data(n2, test);
    पूर्णाङ्क i;
    पूर्णाङ्क a,b,c,d;
    a = b = c = d = 0;
    लागि(i = 0; i < g1.rows; ++i){
        पूर्णाङ्क truth = max_index(test.y.vals[i], test.y.cols);
        पूर्णाङ्क p1 = max_index(g1.vals[i], g1.cols);
        पूर्णाङ्क p2 = max_index(g2.vals[i], g2.cols);
        यदि(p1 == truth){
            यदि(p2 == truth) ++d;
            अन्यथा ++c;
        }अन्यथा{
            यदि(p2 == truth) ++b;
            अन्यथा ++a;
        }
    }
    म_लिखो("%5d %5d\n%5d %5d\n", a, b, c, d);
    भिन्न अङ्क = घात((असल(b - c) - 1.), 2.);
    भिन्न den = b + c;
    म_लिखो("%f\n", अङ्क/den); 
}

भिन्न network_accuracy(network *net, data d)
{
    matrix guess = network_predict_data(net, d);
    भिन्न acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    फर्काऊ acc;
}

भिन्न *network_accuracies(network *net, data d, पूर्णाङ्क n)
{
    स्थिर भिन्न acc[2];
    matrix guess = network_predict_data(net, d);
    acc[0] = matrix_topk_accuracy(d.y, guess, 1);
    acc[1] = matrix_topk_accuracy(d.y, guess, n);
    free_matrix(guess);
    फर्काऊ acc;
}

layer get_network_output_layer(network *net)
{
    पूर्णाङ्क i;
    लागि(i = net->n - 1; i >= 0; --i){
        यदि(net->layers[i].type != COST) रोक;
    }
    फर्काऊ net->layers[i];
}

भिन्न network_accuracy_multi(network *net, data d, पूर्णाङ्क n)
{
    matrix guess = network_predict_data_multi(net, d, n);
    भिन्न acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    फर्काऊ acc;
}

शून्य free_network(network *net)
{
    पूर्णाङ्क i;
    लागि(i = 0; i < net->n; ++i){
        free_layer(net->layers[i]);
    }
    मुक्त(net->layers);
    यदि(net->आगत) मुक्त(net->आगत);
    यदि(net->truth) मुक्त(net->truth);
#ifdef GPU
    यदि(net->आगत_gpu) cuda_free(net->आगत_gpu);
    यदि(net->truth_gpu) cuda_free(net->truth_gpu);
#endif
    मुक्त(net);
}

// Some day...
// ^ What the hell is this comment for?


layer network_output_layer(network *net)
{
    पूर्णाङ्क i;
    लागि(i = net->n - 1; i >= 0; --i){
        यदि(net->layers[i].type != COST) रोक;
    }
    फर्काऊ net->layers[i];
}

पूर्णाङ्क network_inputs(network *net)
{
    फर्काऊ net->layers[0].inputs;
}

पूर्णाङ्क network_outputs(network *net)
{
    फर्काऊ network_output_layer(net).outputs;
}

भिन्न *network_output(network *net)
{
    फर्काऊ network_output_layer(net).output;
}

#ifdef GPU

शून्य forward_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    cuda_थप_array(net.आगत_gpu, net.आगत, net.inputs*net.batch);
    यदि(net.truth){
        cuda_थप_array(net.truth_gpu, net.truth, net.truths*net.batch);
    }

    पूर्णाङ्क i;
    लागि(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        यदि(l.delta_gpu){
            fill_gpu(l.outputs * l.batch, 0, l.delta_gpu, 1);
        }
        l.forward_gpu(l, net);
        net.आगत_gpu = l.output_gpu;
        net.आगत = l.output;
        यदि(l.truth) {
            net.truth_gpu = l.output_gpu;
            net.truth = l.output;
        }
    }
    pull_network_output(netp);
    calc_network_cost(netp);
}

शून्य backward_network_gpu(network *netp)
{
    पूर्णाङ्क i;
    network net = *netp;
    network orig = net;
    cuda_set_device(net.gpu_index);
    लागि(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        यदि(l.stopbackward) रोक;
        यदि(i == 0){
            net = orig;
        }अन्यथा{
            layer prev = net.layers[i-1];
            net.आगत = prev.output;
            net.delta = prev.delta;
            net.आगत_gpu = prev.output_gpu;
            net.delta_gpu = prev.delta_gpu;
        }
        net.index = i;
        l.backward_gpu(l, net);
    }
}

शून्य update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    पूर्णाङ्क i;
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

    लागि(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        यदि(l.update_gpu){
            l.update_gpu(l, a);
        }
    }
}

शून्य harmless_update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    पूर्णाङ्क i;
    लागि(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        यदि(l.weight_updates_gpu) fill_gpu(l.nweights, 0, l.weight_updates_gpu, 1);
        यदि(l.bias_updates_gpu) fill_gpu(l.nbiases, 0, l.bias_updates_gpu, 1);
        यदि(l.scale_updates_gpu) fill_gpu(l.nbiases, 0, l.scale_updates_gpu, 1);
    }
}

प्रकारनिर्देश संरचना {
    network *net;
    data d;
    भिन्न *err;
} train_args;

शून्य *train_thread(शून्य *ptr)
{
    train_args args = *(train_args*)ptr;
    मुक्त(ptr);
    cuda_set_device(args.net->gpu_index);
    *args.err = train_network(args.net, args.d);
    फर्काऊ 0;
}

pthread_t train_network_in_thread(network *net, data d, भिन्न *err)
{
    pthread_t thread;
    train_args *ptr = (train_args *)सुस्मृति(1, परिमाण(train_args));
    ptr->net = net;
    ptr->d = d;
    ptr->err = err;
    यदि(pthread_create(&thread, 0, train_thread, ptr)) error("Thread creation failed");
    फर्काऊ thread;
}

शून्य merge_weights(layer l, layer base)
{
    यदि (l.type == CONVOLUTIONAL) {
        axpy_cpu(l.n, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.nweights, 1, l.weight_updates, 1, base.weights, 1);
        यदि (l.scales) {
            axpy_cpu(l.n, 1, l.scale_updates, 1, base.scales, 1);
        }
    } अन्यथा यदि(l.type == CONNECTED) {
        axpy_cpu(l.outputs, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.outputs*l.inputs, 1, l.weight_updates, 1, base.weights, 1);
    }
}

शून्य scale_weights(layer l, भिन्न s)
{
    यदि (l.type == CONVOLUTIONAL) {
        scal_cpu(l.n, s, l.biases, 1);
        scal_cpu(l.nweights, s, l.weights, 1);
        यदि (l.scales) {
            scal_cpu(l.n, s, l.scales, 1);
        }
    } अन्यथा यदि(l.type == CONNECTED) {
        scal_cpu(l.outputs, s, l.biases, 1);
        scal_cpu(l.outputs*l.inputs, s, l.weights, 1);
    }
}


शून्य pull_weights(layer l)
{
    यदि(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.n);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.nweights);
        यदि(l.scales) cuda_pull_array(l.scales_gpu, l.scale_updates, l.n);
    } अन्यथा यदि(l.type == CONNECTED){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.outputs);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.outputs*l.inputs);
    }
}

शून्य थप_weights(layer l)
{
    यदि(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_थप_array(l.biases_gpu, l.biases, l.n);
        cuda_थप_array(l.weights_gpu, l.weights, l.nweights);
        यदि(l.scales) cuda_थप_array(l.scales_gpu, l.scales, l.n);
    } अन्यथा यदि(l.type == CONNECTED){
        cuda_थप_array(l.biases_gpu, l.biases, l.outputs);
        cuda_थप_array(l.weights_gpu, l.weights, l.outputs*l.inputs);
    }
}

शून्य distribute_weights(layer l, layer base)
{
    यदि (l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL) {
        cuda_थप_array(l.biases_gpu, base.biases, l.n);
        cuda_थप_array(l.weights_gpu, base.weights, l.nweights);
        यदि (base.scales) cuda_थप_array(l.scales_gpu, base.scales, l.n);
    } अन्यथा यदि (l.type == CONNECTED) {
        cuda_थप_array(l.biases_gpu, base.biases, l.outputs);
        cuda_थप_array(l.weights_gpu, base.weights, l.outputs*l.inputs);
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

शून्य sync_layer(network **nets, पूर्णाङ्क n, पूर्णाङ्क j)
{
    पूर्णाङ्क i;
    network *net = nets[0];
    layer base = net->layers[j];
    scale_weights(base, 0);
    लागि (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        pull_weights(l);
        merge_weights(l, base);
    }
    scale_weights(base, 1./n);
    लागि (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        distribute_weights(l, base);
    }
}

प्रकारनिर्देश संरचना{
    network **nets;
    पूर्णाङ्क n;
    पूर्णाङ्क j;
} sync_args;

शून्य *sync_layer_thread(शून्य *ptr)
{
    sync_args args = *(sync_args*)ptr;
    sync_layer(args.nets, args.n, args.j);
    मुक्त(ptr);
    फर्काऊ 0;
}

pthread_t sync_layer_in_thread(network **nets, पूर्णाङ्क n, पूर्णाङ्क j)
{
    pthread_t thread;
    sync_args *ptr = (sync_args *)सुस्मृति(1, परिमाण(sync_args));
    ptr->nets = nets;
    ptr->n = n;
    ptr->j = j;
    यदि(pthread_create(&thread, 0, sync_layer_thread, ptr)) error("Thread creation failed");
    फर्काऊ thread;
}

शून्य sync_nets(network **nets, पूर्णाङ्क n, पूर्णाङ्क interval)
{
    पूर्णाङ्क j;
    पूर्णाङ्क layers = nets[0]->n;
    pthread_t *threads = (pthread_t *) सुस्मृति(layers, परिमाण(pthread_t));

    *(nets[0]->seen) += interval * (n-1) * nets[0]->batch * nets[0]->subdivisions;
    लागि (j = 0; j < n; ++j){
        *(nets[j]->seen) = *(nets[0]->seen);
    }
    लागि (j = 0; j < layers; ++j) {
        threads[j] = sync_layer_in_thread(nets, n, j);
    }
    लागि (j = 0; j < layers; ++j) {
        pthread_join(threads[j], 0);
    }
    मुक्त(threads);
}

भिन्न train_networks(network **nets, पूर्णाङ्क n, data d, पूर्णाङ्क interval)
{
    पूर्णाङ्क i;
    पूर्णाङ्क batch = nets[0]->batch;
    पूर्णाङ्क subdivisions = nets[0]->subdivisions;
    प्रतिज्ञा(batch * subdivisions * n == d.X.rows);
    pthread_t *threads = (pthread_t *) सुस्मृति(n, परिमाण(pthread_t));
    भिन्न *errors = (भिन्न *) सुस्मृति(n, परिमाण(भिन्न));

    भिन्न sum = 0;
    लागि(i = 0; i < n; ++i){
        data p = get_data_part(d, i, n);
        threads[i] = train_network_in_thread(nets[i], p, errors + i);
    }
    लागि(i = 0; i < n; ++i){
        pthread_join(threads[i], 0);
        //printf("%f\n", errors[i]);
        sum += errors[i];
    }
    //cudaDeviceSynchronize();
    यदि (get_current_batch(nets[0]) % interval == 0) {
        म_लिखो("Syncing... ");
        ख_साफ(मानक_निकास);
        sync_nets(nets, n, interval);
        म_लिखो("Done!\n");
    }
    //cudaDeviceSynchronize();
    मुक्त(threads);
    मुक्त(errors);
    फर्काऊ (भिन्न)sum/(n);
}

शून्य pull_network_output(network *net)
{
    layer l = get_network_output_layer(net);
    cuda_pull_array(l.output_gpu, l.output, l.outputs*l.batch);
}

#endif

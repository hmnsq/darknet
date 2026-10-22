#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <time.h>
#অন্তর্ভাব <assert.h>
#অন্তর্ভাব "network.h"
#অন্তর্ভাব "image.h"
#অন্তর্ভাব "data.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "blas.h"

#অন্তর্ভাব "crop_layer.h"
#অন্তর্ভাব "connected_layer.h"
#অন্তর্ভাব "gru_layer.h"
#অন্তর্ভাব "rnn_layer.h"
#অন্তর্ভাব "crnn_layer.h"
#অন্তর্ভাব "local_layer.h"
#অন্তর্ভাব "convolutional_layer.h"
#অন্তর্ভাব "activation_layer.h"
#অন্তর্ভাব "detection_layer.h"
#অন্তর্ভাব "region_layer.h"
#অন্তর্ভাব "yolo_layer.h"
#অন্তর্ভাব "normalization_layer.h"
#অন্তর্ভাব "batchnorm_layer.h"
#অন্তর্ভাব "maxpool_layer.h"
#অন্তর্ভাব "reorg_layer.h"
#অন্তর্ভাব "avgpool_layer.h"
#অন্তর্ভাব "cost_layer.h"
#অন্তর্ভাব "softmax_layer.h"
#অন্তর্ভাব "dropout_layer.h"
#অন্তর্ভাব "route_layer.h"
#অন্তর্ভাব "upsample_layer.h"
#অন্তর্ভাব "shortcut_layer.h"
#অন্তর্ভাব "parser.h"
#অন্তর্ভাব "data.h"

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
    প্রত্যাগম args;
}

network *load_network(বর্ণ *cfg, বর্ণ *weights, পূর্ণাঙ্ক clear)
{
    network *net = parse_network_cfg(cfg);
    যদি(weights && weights[0] != 0){
        load_weights(net, weights);
    }
    যদি(clear) (*net->seen) = 0;
    প্রত্যাগম net;
}

size_t get_current_batch(network *net)
{
    size_t batch_অঙ্ক = (*net->seen)/(net->batch*net->subdivisions);
    প্রত্যাগম batch_অঙ্ক;
}

শূন্য reset_network_state(network *net, পূর্ণাঙ্ক b)
{
    পূর্ণাঙ্ক i;
    প্রতি (i = 0; i < net->n; ++i) {
        #ifdef GPU
        layer l = net->layers[i];
        যদি(l.state_gpu){
            fill_gpu(l.outputs, 0, l.state_gpu + l.outputs*b, 1);
        }
        যদি(l.h_gpu){
            fill_gpu(l.outputs, 0, l.h_gpu + l.outputs*b, 1);
        }
        #endif
    }
}

শূন্য reset_rnn(network *net)
{
    reset_network_state(net, 0);
}

ভিন্ন get_current_rate(network *net)
{
    size_t batch_অঙ্ক = get_current_batch(net);
    পূর্ণাঙ্ক i;
    ভিন্ন rate;
    যদি (batch_অঙ্ক < net->burn_in) প্রত্যাগম net->learning_rate * pow((ভিন্ন)batch_অঙ্ক / net->burn_in, net->power);
    বিকল্প (net->policy) {
        পক্ষ CONSTANT:
            প্রত্যাগম net->learning_rate;
        পক্ষ STEP:
            প্রত্যাগম net->learning_rate * pow(net->scale, batch_অঙ্ক/net->step);
        পক্ষ STEPS:
            rate = net->learning_rate;
            প্রতি(i = 0; i < net->অঙ্ক_steps; ++i){
                যদি(net->steps[i] > batch_অঙ্ক) প্রত্যাগম rate;
                rate *= net->scales[i];
            }
            প্রত্যাগম rate;
        পক্ষ EXP:
            প্রত্যাগম net->learning_rate * pow(net->gamma, batch_অঙ্ক);
        পক্ষ POLY:
            প্রত্যাগম net->learning_rate * pow(1 - (ভিন্ন)batch_অঙ্ক / net->max_batches, net->power);
        পক্ষ RANDOM:
            প্রত্যাগম net->learning_rate * pow(rand_uniform(0,1), net->power);
        পক্ষ SIG:
            প্রত্যাগম net->learning_rate * (1./(1.+exp(net->gamma*(batch_অঙ্ক - net->step))));
        স্বভাব:
            fprintf(stderr, "Policy is weird!\n");
            প্রত্যাগম net->learning_rate;
    }
}

বর্ণ *get_layer_string(LAYER_TYPE a)
{
    বিকল্প(a){
        পক্ষ CONVOLUTIONAL:
            প্রত্যাগম "convolutional";
        পক্ষ ACTIVE:
            প্রত্যাগম "activation";
        পক্ষ LOCAL:
            প্রত্যাগম "local";
        পক্ষ DECONVOLUTIONAL:
            প্রত্যাগম "deconvolutional";
        পক্ষ CONNECTED:
            প্রত্যাগম "connected";
        পক্ষ RNN:
            প্রত্যাগম "rnn";
        পক্ষ GRU:
            প্রত্যাগম "gru";
        পক্ষ LSTM:
	    প্রত্যাগম "lstm";
        পক্ষ CRNN:
            প্রত্যাগম "crnn";
        পক্ষ MAXPOOL:
            প্রত্যাগম "maxpool";
        পক্ষ REORG:
            প্রত্যাগম "reorg";
        পক্ষ AVGPOOL:
            প্রত্যাগম "avgpool";
        পক্ষ SOFTMAX:
            প্রত্যাগম "softmax";
        পক্ষ DETECTION:
            প্রত্যাগম "detection";
        পক্ষ REGION:
            প্রত্যাগম "region";
        পক্ষ YOLO:
            প্রত্যাগম "yolo";
        পক্ষ DROPOUT:
            প্রত্যাগম "dropout";
        পক্ষ CROP:
            প্রত্যাগম "crop";
        পক্ষ COST:
            প্রত্যাগম "cost";
        পক্ষ ROUTE:
            প্রত্যাগম "route";
        পক্ষ SHORTCUT:
            প্রত্যাগম "shortcut";
        পক্ষ NORMALIZATION:
            প্রত্যাগম "normalization";
        পক্ষ BATCHNORM:
            প্রত্যাগম "batchnorm";
        স্বভাব:
            ভঙ্গ;
    }
    প্রত্যাগম "none";
}

network *make_network(পূর্ণাঙ্ক n)
{
    network *net = calloc(1, পরিমাণ(network));
    net->n = n;
    net->layers = calloc(net->n, পরিমাণ(layer));
    net->seen = calloc(1, পরিমাণ(size_t));
    net->t    = calloc(1, পরিমাণ(পূর্ণাঙ্ক));
    net->cost = calloc(1, পরিমাণ(ভিন্ন));
    প্রত্যাগম net;
}

শূন্য forward_network(network *netp)
{
#ifdef GPU
    যদি(netp->gpu_index >= 0){
        forward_network_gpu(netp);   
        প্রত্যাগম;
    }
#endif
    network net = *netp;
    পূর্ণাঙ্ক i;
    প্রতি(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        যদি(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.প্রবেশ = l.output;
        যদি(l.truth) {
            net.truth = l.output;
        }
    }
    calc_network_cost(netp);
}

শূন্য update_network(network *netp)
{
#ifdef GPU
    যদি(netp->gpu_index >= 0){
        update_network_gpu(netp);   
        প্রত্যাগম;
    }
#endif
    network net = *netp;
    পূর্ণাঙ্ক i;
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

    প্রতি(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        যদি(l.update){
            l.update(l, a);
        }
    }
}

শূন্য calc_network_cost(network *netp)
{
    network net = *netp;
    পূর্ণাঙ্ক i;
    ভিন্ন sum = 0;
    পূর্ণাঙ্ক গুনতি = 0;
    প্রতি(i = 0; i < net.n; ++i){
        যদি(net.layers[i].cost){
            sum += net.layers[i].cost[0];
            ++গুনতি;
        }
    }
    *net.cost = sum/গুনতি;
}

পূর্ণাঙ্ক get_predicted_class_network(network *net)
{
    প্রত্যাগম max_index(net->output, net->outputs);
}

শূন্য backward_network(network *netp)
{
#ifdef GPU
    যদি(netp->gpu_index >= 0){
        backward_network_gpu(netp);   
        প্রত্যাগম;
    }
#endif
    network net = *netp;
    পূর্ণাঙ্ক i;
    network orig = net;
    প্রতি(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        যদি(l.stopbackward) ভঙ্গ;
        যদি(i == 0){
            net = orig;
        }অন্যথা{
            layer prev = net.layers[i-1];
            net.প্রবেশ = prev.output;
            net.delta = prev.delta;
        }
        net.index = i;
        l.backward(l, net);
    }
}

ভিন্ন train_network_datum(network *net)
{
    *net->seen += net->batch;
    net->train = 1;
    forward_network(net);
    backward_network(net);
    ভিন্ন error = *net->cost;
    যদি(((*net->seen)/net->batch)%net->subdivisions == 0) update_network(net);
    প্রত্যাগম error;
}

ভিন্ন train_network_sgd(network *net, data d, পূর্ণাঙ্ক n)
{
    পূর্ণাঙ্ক batch = net->batch;

    পূর্ণাঙ্ক i;
    ভিন্ন sum = 0;
    প্রতি(i = 0; i < n; ++i){
        get_random_batch(d, batch, net->প্রবেশ, net->truth);
        ভিন্ন err = train_network_datum(net);
        sum += err;
    }
    প্রত্যাগম (ভিন্ন)sum/(n*batch);
}

ভিন্ন train_network(network *net, data d)
{
    প্রতিজ্ঞা(d.X.rows % net->batch == 0);
    পূর্ণাঙ্ক batch = net->batch;
    পূর্ণাঙ্ক n = d.X.rows / batch;

    পূর্ণাঙ্ক i;
    ভিন্ন sum = 0;
    প্রতি(i = 0; i < n; ++i){
        get_next_batch(d, batch, i*batch, net->প্রবেশ, net->truth);
        ভিন্ন err = train_network_datum(net);
        sum += err;
    }
    প্রত্যাগম (ভিন্ন)sum/(n*batch);
}

শূন্য set_temp_network(network *net, ভিন্ন t)
{
    পূর্ণাঙ্ক i;
    প্রতি(i = 0; i < net->n; ++i){
        net->layers[i].temperature = t;
    }
}


শূন্য set_batch_network(network *net, পূর্ণাঙ্ক b)
{
    net->batch = b;
    পূর্ণাঙ্ক i;
    প্রতি(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#ifdef CUDNN
        যদি(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_setup(net->layers + i);
        }
        যদি(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, l->out_h, l->out_w);
            cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 
        }
#endif
    }
}

পূর্ণাঙ্ক resize_network(network *net, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h)
{
#ifdef GPU
    cuda_set_device(net->gpu_index);
    cuda_free(net->workspace);
#endif
    পূর্ণাঙ্ক i;
    //if(w == net->w && h == net->h) return 0;
    net->w = w;
    net->h = h;
    পূর্ণাঙ্ক inputs = 0;
    size_t workspace_size = 0;
    //fprintf(stderr, "Resizing to %d x %d...\n", w, h);
    //fflush(stderr);
    প্রতি (i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        যদি(l.type == CONVOLUTIONAL){
            resize_convolutional_layer(&l, w, h);
        }অন্যথা যদি(l.type == CROP){
            resize_crop_layer(&l, w, h);
        }অন্যথা যদি(l.type == MAXPOOL){
            resize_maxpool_layer(&l, w, h);
        }অন্যথা যদি(l.type == REGION){
            resize_region_layer(&l, w, h);
        }অন্যথা যদি(l.type == YOLO){
            resize_yolo_layer(&l, w, h);
        }অন্যথা যদি(l.type == ROUTE){
            resize_route_layer(&l, net);
        }অন্যথা যদি(l.type == SHORTCUT){
            resize_shortcut_layer(&l, w, h);
        }অন্যথা যদি(l.type == UPSAMPLE){
            resize_upsample_layer(&l, w, h);
        }অন্যথা যদি(l.type == REORG){
            resize_reorg_layer(&l, w, h);
        }অন্যথা যদি(l.type == AVGPOOL){
            resize_avgpool_layer(&l, w, h);
        }অন্যথা যদি(l.type == NORMALIZATION){
            resize_normalization_layer(&l, w, h);
        }অন্যথা যদি(l.type == COST){
            resize_cost_layer(&l, inputs);
        }অন্যথা{
            error("Cannot resize this type of layer");
        }
        যদি(l.workspace_size > workspace_size) workspace_size = l.workspace_size;
        যদি(l.workspace_size > 2000000000) প্রতিজ্ঞা(0);
        inputs = l.outputs;
        net->layers[i] = l;
        w = l.out_w;
        h = l.out_h;
        যদি(l.type == AVGPOOL) ভঙ্গ;
    }
    layer out = get_network_output_layer(net);
    net->inputs = net->layers[0].inputs;
    net->outputs = out.outputs;
    net->truths = out.outputs;
    যদি(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    মুক্ত(net->প্রবেশ);
    মুক্ত(net->truth);
    net->প্রবেশ = calloc(net->inputs*net->batch, পরিমাণ(ভিন্ন));
    net->truth = calloc(net->truths*net->batch, পরিমাণ(ভিন্ন));
#ifdef GPU
    যদি(gpu_index >= 0){
        cuda_free(net->প্রবেশ_gpu);
        cuda_free(net->truth_gpu);
        net->প্রবেশ_gpu = cuda_make_array(net->প্রবেশ, net->inputs*net->batch);
        net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
        যদি(workspace_size){
            net->workspace = cuda_make_array(0, (workspace_size-1)/পরিমাণ(ভিন্ন)+1);
        }
    }অন্যথা {
        মুক্ত(net->workspace);
        net->workspace = calloc(1, workspace_size);
    }
#অন্যথা
    মুক্ত(net->workspace);
    net->workspace = calloc(1, workspace_size);
#endif
    //fprintf(stderr, " Done!\n");
    প্রত্যাগম 0;
}

layer get_network_detection_layer(network *net)
{
    পূর্ণাঙ্ক i;
    প্রতি(i = 0; i < net->n; ++i){
        যদি(net->layers[i].type == DETECTION){
            প্রত্যাগম net->layers[i];
        }
    }
    fprintf(stderr, "Detection layer not found!!\n");
    layer l = {0};
    প্রত্যাগম l;
}

image get_network_image_layer(network *net, পূর্ণাঙ্ক i)
{
    layer l = net->layers[i];
#ifdef GPU
    //cuda_pull_array(l.output_gpu, l.output, l.outputs);
#endif
    যদি (l.out_w && l.out_h && l.out_c){
        প্রত্যাগম float_to_image(l.out_w, l.out_h, l.out_c, l.output);
    }
    image def = {0};
    প্রত্যাগম def;
}

image get_network_image(network *net)
{
    পূর্ণাঙ্ক i;
    প্রতি(i = net->n-1; i >= 0; --i){
        image m = get_network_image_layer(net, i);
        যদি(m.h != 0) প্রত্যাগম m;
    }
    image def = {0};
    প্রত্যাগম def;
}

শূন্য visualize_network(network *net)
{
    image *prev = 0;
    পূর্ণাঙ্ক i;
    বর্ণ buff[256];
    প্রতি(i = 0; i < net->n; ++i){
        sprintf(buff, "Layer %d", i);
        layer l = net->layers[i];
        যদি(l.type == CONVOLUTIONAL){
            prev = visualize_convolutional_layer(l, buff, prev);
        }
    } 
}

শূন্য শীর্ষ_predictions(network *net, পূর্ণাঙ্ক k, পূর্ণাঙ্ক *index)
{
    শীর্ষ_k(net->output, net->outputs, k, index);
}


ভিন্ন *network_predict(network *net, ভিন্ন *প্রবেশ)
{
    network orig = *net;
    net->প্রবেশ = প্রবেশ;
    net->truth = 0;
    net->train = 0;
    net->delta = 0;
    forward_network(net);
    ভিন্ন *out = net->output;
    *net = orig;
    প্রত্যাগম out;
}

পূর্ণাঙ্ক অঙ্ক_detections(network *net, ভিন্ন thresh)
{
    পূর্ণাঙ্ক i;
    পূর্ণাঙ্ক s = 0;
    প্রতি(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        যদি(l.type == YOLO){
            s += yolo_অঙ্ক_detections(l, thresh);
        }
        যদি(l.type == DETECTION || l.type == REGION){
            s += l.w*l.h*l.n;
        }
    }
    প্রত্যাগম s;
}

detection *make_network_boxes(network *net, ভিন্ন thresh, পূর্ণাঙ্ক *অঙ্ক)
{
    layer l = net->layers[net->n - 1];
    পূর্ণাঙ্ক i;
    পূর্ণাঙ্ক nboxes = অঙ্ক_detections(net, thresh);
    যদি(অঙ্ক) *অঙ্ক = nboxes;
    detection *dets = calloc(nboxes, পরিমাণ(detection));
    প্রতি(i = 0; i < nboxes; ++i){
        dets[i].prob = calloc(l.classes, পরিমাণ(ভিন্ন));
        যদি(l.coords > 4){
            dets[i].mask = calloc(l.coords-4, পরিমাণ(ভিন্ন));
        }
    }
    প্রত্যাগম dets;
}

শূন্য fill_network_boxes(network *net, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, ভিন্ন thresh, ভিন্ন hier, পূর্ণাঙ্ক *map, পূর্ণাঙ্ক relative, detection *dets)
{
    পূর্ণাঙ্ক j;
    প্রতি(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        যদি(l.type == YOLO){
            পূর্ণাঙ্ক গুনতি = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
            dets += গুনতি;
        }
        যদি(l.type == REGION){
            get_region_detections(l, w, h, net->w, net->h, thresh, map, hier, relative, dets);
            dets += l.w*l.h*l.n;
        }
        যদি(l.type == DETECTION){
            get_detection_detections(l, w, h, thresh, dets);
            dets += l.w*l.h*l.n;
        }
    }
}

detection *get_network_boxes(network *net, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, ভিন্ন thresh, ভিন্ন hier, পূর্ণাঙ্ক *map, পূর্ণাঙ্ক relative, পূর্ণাঙ্ক *অঙ্ক)
{
    detection *dets = make_network_boxes(net, thresh, অঙ্ক);
    fill_network_boxes(net, w, h, thresh, hier, map, relative, dets);
    প্রত্যাগম dets;
}

শূন্য free_detections(detection *dets, পূর্ণাঙ্ক n)
{
    পূর্ণাঙ্ক i;
    প্রতি(i = 0; i < n; ++i){
        মুক্ত(dets[i].prob);
        যদি(dets[i].mask) মুক্ত(dets[i].mask);
    }
    মুক্ত(dets);
}

ভিন্ন *network_predict_image(network *net, image im)
{
    image imr = letterbox_image(im, net->w, net->h);
    set_batch_network(net, 1);
    ভিন্ন *p = network_predict(net, imr.data);
    free_image(imr);
    প্রত্যাগম p;
}

পূর্ণাঙ্ক network_width(network *net){প্রত্যাগম net->w;}
পূর্ণাঙ্ক network_height(network *net){প্রত্যাগম net->h;}

matrix network_predict_data_multi(network *net, data test, পূর্ণাঙ্ক n)
{
    পূর্ণাঙ্ক i,j,b,m;
    পূর্ণাঙ্ক k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    ভিন্ন *X = calloc(net->batch*test.X.rows, পরিমাণ(ভিন্ন));
    প্রতি(i = 0; i < test.X.rows; i += net->batch){
        প্রতি(b = 0; b < net->batch; ++b){
            যদি(i+b == test.X.rows) ভঙ্গ;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*পরিমাণ(ভিন্ন));
        }
        প্রতি(m = 0; m < n; ++m){
            ভিন্ন *out = network_predict(net, X);
            প্রতি(b = 0; b < net->batch; ++b){
                যদি(i+b == test.X.rows) ভঙ্গ;
                প্রতি(j = 0; j < k; ++j){
                    pred.vals[i+b][j] += out[j+b*k]/n;
                }
            }
        }
    }
    মুক্ত(X);
    প্রত্যাগম pred;   
}

matrix network_predict_data(network *net, data test)
{
    পূর্ণাঙ্ক i,j,b;
    পূর্ণাঙ্ক k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    ভিন্ন *X = calloc(net->batch*test.X.cols, পরিমাণ(ভিন্ন));
    প্রতি(i = 0; i < test.X.rows; i += net->batch){
        প্রতি(b = 0; b < net->batch; ++b){
            যদি(i+b == test.X.rows) ভঙ্গ;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*পরিমাণ(ভিন্ন));
        }
        ভিন্ন *out = network_predict(net, X);
        প্রতি(b = 0; b < net->batch; ++b){
            যদি(i+b == test.X.rows) ভঙ্গ;
            প্রতি(j = 0; j < k; ++j){
                pred.vals[i+b][j] = out[j+b*k];
            }
        }
    }
    মুক্ত(X);
    প্রত্যাগম pred;   
}

শূন্য print_network(network *net)
{
    পূর্ণাঙ্ক i,j;
    প্রতি(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ভিন্ন *output = l.output;
        পূর্ণাঙ্ক n = l.outputs;
        ভিন্ন mean = mean_array(output, n);
        ভিন্ন vari = variance_array(output, n);
        fprintf(stderr, "Layer %d - Mean: %f, Variance: %f\n",i,mean, vari);
        যদি(n > 100) n = 100;
        প্রতি(j = 0; j < n; ++j) fprintf(stderr, "%f, ", output[j]);
        যদি(n == 100)fprintf(stderr,".....\n");
        fprintf(stderr, "\n");
    }
}

শূন্য compare_networks(network *n1, network *n2, data test)
{
    matrix g1 = network_predict_data(n1, test);
    matrix g2 = network_predict_data(n2, test);
    পূর্ণাঙ্ক i;
    পূর্ণাঙ্ক a,b,c,d;
    a = b = c = d = 0;
    প্রতি(i = 0; i < g1.rows; ++i){
        পূর্ণাঙ্ক truth = max_index(test.y.vals[i], test.y.cols);
        পূর্ণাঙ্ক p1 = max_index(g1.vals[i], g1.cols);
        পূর্ণাঙ্ক p2 = max_index(g2.vals[i], g2.cols);
        যদি(p1 == truth){
            যদি(p2 == truth) ++d;
            অন্যথা ++c;
        }অন্যথা{
            যদি(p2 == truth) ++b;
            অন্যথা ++a;
        }
    }
    printf("%5d %5d\n%5d %5d\n", a, b, c, d);
    ভিন্ন অঙ্ক = pow((abs(b - c) - 1.), 2.);
    ভিন্ন den = b + c;
    printf("%f\n", অঙ্ক/den); 
}

ভিন্ন network_accuracy(network *net, data d)
{
    matrix guess = network_predict_data(net, d);
    ভিন্ন acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    প্রত্যাগম acc;
}

ভিন্ন *network_accuracies(network *net, data d, পূর্ণাঙ্ক n)
{
    স্থির ভিন্ন acc[2];
    matrix guess = network_predict_data(net, d);
    acc[0] = matrix_topk_accuracy(d.y, guess, 1);
    acc[1] = matrix_topk_accuracy(d.y, guess, n);
    free_matrix(guess);
    প্রত্যাগম acc;
}

layer get_network_output_layer(network *net)
{
    পূর্ণাঙ্ক i;
    প্রতি(i = net->n - 1; i >= 0; --i){
        যদি(net->layers[i].type != COST) ভঙ্গ;
    }
    প্রত্যাগম net->layers[i];
}

ভিন্ন network_accuracy_multi(network *net, data d, পূর্ণাঙ্ক n)
{
    matrix guess = network_predict_data_multi(net, d, n);
    ভিন্ন acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    প্রত্যাগম acc;
}

শূন্য free_network(network *net)
{
    পূর্ণাঙ্ক i;
    প্রতি(i = 0; i < net->n; ++i){
        free_layer(net->layers[i]);
    }
    মুক্ত(net->layers);
    যদি(net->প্রবেশ) মুক্ত(net->প্রবেশ);
    যদি(net->truth) মুক্ত(net->truth);
#ifdef GPU
    যদি(net->প্রবেশ_gpu) cuda_free(net->প্রবেশ_gpu);
    যদি(net->truth_gpu) cuda_free(net->truth_gpu);
#endif
    মুক্ত(net);
}

// Some day...
// ^ What the hell is this comment for?


layer network_output_layer(network *net)
{
    পূর্ণাঙ্ক i;
    প্রতি(i = net->n - 1; i >= 0; --i){
        যদি(net->layers[i].type != COST) ভঙ্গ;
    }
    প্রত্যাগম net->layers[i];
}

পূর্ণাঙ্ক network_inputs(network *net)
{
    প্রত্যাগম net->layers[0].inputs;
}

পূর্ণাঙ্ক network_outputs(network *net)
{
    প্রত্যাগম network_output_layer(net).outputs;
}

ভিন্ন *network_output(network *net)
{
    প্রত্যাগম network_output_layer(net).output;
}

#ifdef GPU

শূন্য forward_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    cuda_ঠেলো_array(net.প্রবেশ_gpu, net.প্রবেশ, net.inputs*net.batch);
    যদি(net.truth){
        cuda_ঠেলো_array(net.truth_gpu, net.truth, net.truths*net.batch);
    }

    পূর্ণাঙ্ক i;
    প্রতি(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        যদি(l.delta_gpu){
            fill_gpu(l.outputs * l.batch, 0, l.delta_gpu, 1);
        }
        l.forward_gpu(l, net);
        net.প্রবেশ_gpu = l.output_gpu;
        net.প্রবেশ = l.output;
        যদি(l.truth) {
            net.truth_gpu = l.output_gpu;
            net.truth = l.output;
        }
    }
    pull_network_output(netp);
    calc_network_cost(netp);
}

শূন্য backward_network_gpu(network *netp)
{
    পূর্ণাঙ্ক i;
    network net = *netp;
    network orig = net;
    cuda_set_device(net.gpu_index);
    প্রতি(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        যদি(l.stopbackward) ভঙ্গ;
        যদি(i == 0){
            net = orig;
        }অন্যথা{
            layer prev = net.layers[i-1];
            net.প্রবেশ = prev.output;
            net.delta = prev.delta;
            net.প্রবেশ_gpu = prev.output_gpu;
            net.delta_gpu = prev.delta_gpu;
        }
        net.index = i;
        l.backward_gpu(l, net);
    }
}

শূন্য update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    পূর্ণাঙ্ক i;
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

    প্রতি(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        যদি(l.update_gpu){
            l.update_gpu(l, a);
        }
    }
}

শূন্য harmless_update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    পূর্ণাঙ্ক i;
    প্রতি(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        যদি(l.weight_updates_gpu) fill_gpu(l.nweights, 0, l.weight_updates_gpu, 1);
        যদি(l.bias_updates_gpu) fill_gpu(l.nbiases, 0, l.bias_updates_gpu, 1);
        যদি(l.scale_updates_gpu) fill_gpu(l.nbiases, 0, l.scale_updates_gpu, 1);
    }
}

প্রকারনির্দেশ সংরচনা {
    network *net;
    data d;
    ভিন্ন *err;
} train_args;

শূন্য *train_thread(শূন্য *ptr)
{
    train_args args = *(train_args*)ptr;
    মুক্ত(ptr);
    cuda_set_device(args.net->gpu_index);
    *args.err = train_network(args.net, args.d);
    প্রত্যাগম 0;
}

pthread_t train_network_in_thread(network *net, data d, ভিন্ন *err)
{
    pthread_t thread;
    train_args *ptr = (train_args *)calloc(1, পরিমাণ(train_args));
    ptr->net = net;
    ptr->d = d;
    ptr->err = err;
    যদি(pthread_create(&thread, 0, train_thread, ptr)) error("Thread creation failed");
    প্রত্যাগম thread;
}

শূন্য merge_weights(layer l, layer base)
{
    যদি (l.type == CONVOLUTIONAL) {
        axpy_cpu(l.n, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.nweights, 1, l.weight_updates, 1, base.weights, 1);
        যদি (l.scales) {
            axpy_cpu(l.n, 1, l.scale_updates, 1, base.scales, 1);
        }
    } অন্যথা যদি(l.type == CONNECTED) {
        axpy_cpu(l.outputs, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.outputs*l.inputs, 1, l.weight_updates, 1, base.weights, 1);
    }
}

শূন্য scale_weights(layer l, ভিন্ন s)
{
    যদি (l.type == CONVOLUTIONAL) {
        scal_cpu(l.n, s, l.biases, 1);
        scal_cpu(l.nweights, s, l.weights, 1);
        যদি (l.scales) {
            scal_cpu(l.n, s, l.scales, 1);
        }
    } অন্যথা যদি(l.type == CONNECTED) {
        scal_cpu(l.outputs, s, l.biases, 1);
        scal_cpu(l.outputs*l.inputs, s, l.weights, 1);
    }
}


শূন্য pull_weights(layer l)
{
    যদি(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.n);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.nweights);
        যদি(l.scales) cuda_pull_array(l.scales_gpu, l.scale_updates, l.n);
    } অন্যথা যদি(l.type == CONNECTED){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.outputs);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.outputs*l.inputs);
    }
}

শূন্য ঠেলো_weights(layer l)
{
    যদি(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_ঠেলো_array(l.biases_gpu, l.biases, l.n);
        cuda_ঠেলো_array(l.weights_gpu, l.weights, l.nweights);
        যদি(l.scales) cuda_ঠেলো_array(l.scales_gpu, l.scales, l.n);
    } অন্যথা যদি(l.type == CONNECTED){
        cuda_ঠেলো_array(l.biases_gpu, l.biases, l.outputs);
        cuda_ঠেলো_array(l.weights_gpu, l.weights, l.outputs*l.inputs);
    }
}

শূন্য distribute_weights(layer l, layer base)
{
    যদি (l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL) {
        cuda_ঠেলো_array(l.biases_gpu, base.biases, l.n);
        cuda_ঠেলো_array(l.weights_gpu, base.weights, l.nweights);
        যদি (base.scales) cuda_ঠেলো_array(l.scales_gpu, base.scales, l.n);
    } অন্যথা যদি (l.type == CONNECTED) {
        cuda_ঠেলো_array(l.biases_gpu, base.biases, l.outputs);
        cuda_ঠেলো_array(l.weights_gpu, base.weights, l.outputs*l.inputs);
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

শূন্য sync_layer(network **nets, পূর্ণাঙ্ক n, পূর্ণাঙ্ক j)
{
    পূর্ণাঙ্ক i;
    network *net = nets[0];
    layer base = net->layers[j];
    scale_weights(base, 0);
    প্রতি (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        pull_weights(l);
        merge_weights(l, base);
    }
    scale_weights(base, 1./n);
    প্রতি (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        distribute_weights(l, base);
    }
}

প্রকারনির্দেশ সংরচনা{
    network **nets;
    পূর্ণাঙ্ক n;
    পূর্ণাঙ্ক j;
} sync_args;

শূন্য *sync_layer_thread(শূন্য *ptr)
{
    sync_args args = *(sync_args*)ptr;
    sync_layer(args.nets, args.n, args.j);
    মুক্ত(ptr);
    প্রত্যাগম 0;
}

pthread_t sync_layer_in_thread(network **nets, পূর্ণাঙ্ক n, পূর্ণাঙ্ক j)
{
    pthread_t thread;
    sync_args *ptr = (sync_args *)calloc(1, পরিমাণ(sync_args));
    ptr->nets = nets;
    ptr->n = n;
    ptr->j = j;
    যদি(pthread_create(&thread, 0, sync_layer_thread, ptr)) error("Thread creation failed");
    প্রত্যাগম thread;
}

শূন্য sync_nets(network **nets, পূর্ণাঙ্ক n, পূর্ণাঙ্ক interval)
{
    পূর্ণাঙ্ক j;
    পূর্ণাঙ্ক layers = nets[0]->n;
    pthread_t *threads = (pthread_t *) calloc(layers, পরিমাণ(pthread_t));

    *(nets[0]->seen) += interval * (n-1) * nets[0]->batch * nets[0]->subdivisions;
    প্রতি (j = 0; j < n; ++j){
        *(nets[j]->seen) = *(nets[0]->seen);
    }
    প্রতি (j = 0; j < layers; ++j) {
        threads[j] = sync_layer_in_thread(nets, n, j);
    }
    প্রতি (j = 0; j < layers; ++j) {
        pthread_join(threads[j], 0);
    }
    মুক্ত(threads);
}

ভিন্ন train_networks(network **nets, পূর্ণাঙ্ক n, data d, পূর্ণাঙ্ক interval)
{
    পূর্ণাঙ্ক i;
    পূর্ণাঙ্ক batch = nets[0]->batch;
    পূর্ণাঙ্ক subdivisions = nets[0]->subdivisions;
    প্রতিজ্ঞা(batch * subdivisions * n == d.X.rows);
    pthread_t *threads = (pthread_t *) calloc(n, পরিমাণ(pthread_t));
    ভিন্ন *errors = (ভিন্ন *) calloc(n, পরিমাণ(ভিন্ন));

    ভিন্ন sum = 0;
    প্রতি(i = 0; i < n; ++i){
        data p = get_data_part(d, i, n);
        threads[i] = train_network_in_thread(nets[i], p, errors + i);
    }
    প্রতি(i = 0; i < n; ++i){
        pthread_join(threads[i], 0);
        //printf("%f\n", errors[i]);
        sum += errors[i];
    }
    //cudaDeviceSynchronize();
    যদি (get_current_batch(nets[0]) % interval == 0) {
        printf("Syncing... ");
        fflush(stdout);
        sync_nets(nets, n, interval);
        printf("Done!\n");
    }
    //cudaDeviceSynchronize();
    মুক্ত(threads);
    মুক্ত(errors);
    প্রত্যাগম (ভিন্ন)sum/(n);
}

শূন্য pull_network_output(network *net)
{
    layer l = get_network_output_layer(net);
    cuda_pull_array(l.output_gpu, l.output, l.outputs*l.batch);
}

#endif

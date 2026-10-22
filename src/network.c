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
    args.আকার_2 = net->w;

    args.min = net->min_crop;
    args.max = net->max_crop;
    args.angle = net->angle;
    args.aspect = net->aspect;
    args.exposure = net->exposure;
    args.center = net->center;
    args.saturation = net->saturation;
    args.hue = net->hue;
    ফেরত args;
}

network *load_network(অক্ষর *cfg, অক্ষর *weights, পূর্ণসংখ্যা clear)
{
    network *net = parse_network_cfg(cfg);
    যদি(weights && weights[0] != 0){
        load_weights(net, weights);
    }
    যদি(clear) (*net->seen) = 0;
    ফেরত net;
}

size_t get_current_batch(network *net)
{
    size_t batch_অঙ্ক = (*net->seen)/(net->batch*net->subdivisions);
    ফেরত batch_অঙ্ক;
}

শূন্য reset_network_state(network *net, পূর্ণসংখ্যা b)
{
    পূর্ণসংখ্যা i;
    জন্য (i = 0; i < net->n; ++i) {
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

দশমিক get_current_rate(network *net)
{
    size_t batch_অঙ্ক = get_current_batch(net);
    পূর্ণসংখ্যা i;
    দশমিক rate;
    যদি (batch_অঙ্ক < net->burn_in) ফেরত net->learning_rate * pow((দশমিক)batch_অঙ্ক / net->burn_in, net->power);
    নির্বাচন (net->policy) {
        ক্ষেত্র CONSTANT:
            ফেরত net->learning_rate;
        ক্ষেত্র STEP:
            ফেরত net->learning_rate * pow(net->scale, batch_অঙ্ক/net->step);
        ক্ষেত্র STEPS:
            rate = net->learning_rate;
            জন্য(i = 0; i < net->অঙ্ক_steps; ++i){
                যদি(net->steps[i] > batch_অঙ্ক) ফেরত rate;
                rate *= net->scales[i];
            }
            ফেরত rate;
        ক্ষেত্র EXP:
            ফেরত net->learning_rate * pow(net->gamma, batch_অঙ্ক);
        ক্ষেত্র POLY:
            ফেরত net->learning_rate * pow(1 - (দশমিক)batch_অঙ্ক / net->max_batches, net->power);
        ক্ষেত্র RANDOM:
            ফেরত net->learning_rate * pow(rand_uniform(0,1), net->power);
        ক্ষেত্র SIG:
            ফেরত net->learning_rate * (1./(1.+exp(net->gamma*(batch_অঙ্ক - net->step))));
        পূর্বনির্ধারিত:
            fprintf(stderr, "Policy is weird!\n");
            ফেরত net->learning_rate;
    }
}

অক্ষর *get_layer_string(LAYER_TYPE a)
{
    নির্বাচন(a){
        ক্ষেত্র CONVOLUTIONAL:
            ফেরত "convolutional";
        ক্ষেত্র ACTIVE:
            ফেরত "activation";
        ক্ষেত্র LOCAL:
            ফেরত "local";
        ক্ষেত্র DECONVOLUTIONAL:
            ফেরত "deconvolutional";
        ক্ষেত্র CONNECTED:
            ফেরত "connected";
        ক্ষেত্র RNN:
            ফেরত "rnn";
        ক্ষেত্র GRU:
            ফেরত "gru";
        ক্ষেত্র LSTM:
	    ফেরত "lstm";
        ক্ষেত্র CRNN:
            ফেরত "crnn";
        ক্ষেত্র MAXPOOL:
            ফেরত "maxpool";
        ক্ষেত্র REORG:
            ফেরত "reorg";
        ক্ষেত্র AVGPOOL:
            ফেরত "avgpool";
        ক্ষেত্র SOFTMAX:
            ফেরত "softmax";
        ক্ষেত্র DETECTION:
            ফেরত "detection";
        ক্ষেত্র REGION:
            ফেরত "region";
        ক্ষেত্র YOLO:
            ফেরত "yolo";
        ক্ষেত্র DROPOUT:
            ফেরত "dropout";
        ক্ষেত্র CROP:
            ফেরত "crop";
        ক্ষেত্র COST:
            ফেরত "cost";
        ক্ষেত্র ROUTE:
            ফেরত "route";
        ক্ষেত্র SHORTCUT:
            ফেরত "shortcut";
        ক্ষেত্র NORMALIZATION:
            ফেরত "normalization";
        ক্ষেত্র BATCHNORM:
            ফেরত "batchnorm";
        পূর্বনির্ধারিত:
            বিরতি;
    }
    ফেরত "none";
}

network *make_network(পূর্ণসংখ্যা n)
{
    network *net = calloc(1, আকার(network));
    net->n = n;
    net->layers = calloc(net->n, আকার(layer));
    net->seen = calloc(1, আকার(size_t));
    net->t    = calloc(1, আকার(পূর্ণসংখ্যা));
    net->cost = calloc(1, আকার(দশমিক));
    ফেরত net;
}

শূন্য forward_network(network *netp)
{
#ifdef GPU
    যদি(netp->gpu_সূচক >= 0){
        forward_network_gpu(netp);   
        ফেরত;
    }
#endif
    network net = *netp;
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < net.n; ++i){
        net.সূচক = i;
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
    যদি(netp->gpu_সূচক >= 0){
        update_network_gpu(netp);   
        ফেরত;
    }
#endif
    network net = *netp;
    পূর্ণসংখ্যা i;
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

    জন্য(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        যদি(l.update){
            l.update(l, a);
        }
    }
}

শূন্য calc_network_cost(network *netp)
{
    network net = *netp;
    পূর্ণসংখ্যা i;
    দশমিক যোগ = 0;
    পূর্ণসংখ্যা গুনতি = 0;
    জন্য(i = 0; i < net.n; ++i){
        যদি(net.layers[i].cost){
            যোগ += net.layers[i].cost[0];
            ++গুনতি;
        }
    }
    *net.cost = যোগ/গুনতি;
}

পূর্ণসংখ্যা get_predicted_class_network(network *net)
{
    ফেরত max_সূচক(net->output, net->outputs);
}

শূন্য backward_network(network *netp)
{
#ifdef GPU
    যদি(netp->gpu_সূচক >= 0){
        backward_network_gpu(netp);   
        ফেরত;
    }
#endif
    network net = *netp;
    পূর্ণসংখ্যা i;
    network orig = net;
    জন্য(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        যদি(l.stopbackward) বিরতি;
        যদি(i == 0){
            net = orig;
        }অন্যথা{
            layer পূর্ব = net.layers[i-1];
            net.প্রবেশ = পূর্ব.output;
            net.delta = পূর্ব.delta;
        }
        net.সূচক = i;
        l.backward(l, net);
    }
}

দশমিক train_network_datum(network *net)
{
    *net->seen += net->batch;
    net->train = 1;
    forward_network(net);
    backward_network(net);
    দশমিক error = *net->cost;
    যদি(((*net->seen)/net->batch)%net->subdivisions == 0) update_network(net);
    ফেরত error;
}

দশমিক train_network_sgd(network *net, তথ্য d, পূর্ণসংখ্যা n)
{
    পূর্ণসংখ্যা batch = net->batch;

    পূর্ণসংখ্যা i;
    দশমিক যোগ = 0;
    জন্য(i = 0; i < n; ++i){
        get_random_batch(d, batch, net->প্রবেশ, net->truth);
        দশমিক err = train_network_datum(net);
        যোগ += err;
    }
    ফেরত (দশমিক)যোগ/(n*batch);
}

দশমিক train_network(network *net, তথ্য d)
{
    প্রতিজ্ঞা(d.X.rows % net->batch == 0);
    পূর্ণসংখ্যা batch = net->batch;
    পূর্ণসংখ্যা n = d.X.rows / batch;

    পূর্ণসংখ্যা i;
    দশমিক যোগ = 0;
    জন্য(i = 0; i < n; ++i){
        get_পরবর্তী_batch(d, batch, i*batch, net->প্রবেশ, net->truth);
        দশমিক err = train_network_datum(net);
        যোগ += err;
    }
    ফেরত (দশমিক)যোগ/(n*batch);
}

শূন্য set_অস্থায়ী_network(network *net, দশমিক t)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < net->n; ++i){
        net->layers[i].temperature = t;
    }
}


শূন্য set_batch_network(network *net, পূর্ণসংখ্যা b)
{
    net->batch = b;
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#ifdef CUDNN
        যদি(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_setup(net->layers + i);
        }
        যদি(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_তথ্য_FLOAT, 1, l->out_c, l->out_h, l->out_w);
            cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_তথ্য_FLOAT, 1, l->out_c, 1, 1); 
        }
#endif
    }
}

পূর্ণসংখ্যা resize_network(network *net, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
#ifdef GPU
    cuda_set_device(net->gpu_সূচক);
    cuda_free(net->workspace);
#endif
    পূর্ণসংখ্যা i;
    //if(w == net->w && h == net->h) return 0;
    net->w = w;
    net->h = h;
    পূর্ণসংখ্যা inputs = 0;
    size_t workspace_আকার = 0;
    //fprintf(stderr, "Resizing to %d x %d...\n", w, h);
    //fflush(stderr);
    জন্য (i = 0; i < net->n; ++i){
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
        যদি(l.workspace_আকার > workspace_আকার) workspace_আকার = l.workspace_আকার;
        যদি(l.workspace_আকার > 2000000000) প্রতিজ্ঞা(0);
        inputs = l.outputs;
        net->layers[i] = l;
        w = l.out_w;
        h = l.out_h;
        যদি(l.type == AVGPOOL) বিরতি;
    }
    layer out = get_network_output_layer(net);
    net->inputs = net->layers[0].inputs;
    net->outputs = out.outputs;
    net->truths = out.outputs;
    যদি(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    মুক্ত(net->প্রবেশ);
    মুক্ত(net->truth);
    net->প্রবেশ = calloc(net->inputs*net->batch, আকার(দশমিক));
    net->truth = calloc(net->truths*net->batch, আকার(দশমিক));
#ifdef GPU
    যদি(gpu_সূচক >= 0){
        cuda_free(net->প্রবেশ_gpu);
        cuda_free(net->truth_gpu);
        net->প্রবেশ_gpu = cuda_make_array(net->প্রবেশ, net->inputs*net->batch);
        net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
        যদি(workspace_আকার){
            net->workspace = cuda_make_array(0, (workspace_আকার-1)/আকার(দশমিক)+1);
        }
    }অন্যথা {
        মুক্ত(net->workspace);
        net->workspace = calloc(1, workspace_আকার);
    }
#অন্যথা
    মুক্ত(net->workspace);
    net->workspace = calloc(1, workspace_আকার);
#endif
    //fprintf(stderr, " Done!\n");
    ফেরত 0;
}

layer get_network_detection_layer(network *net)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < net->n; ++i){
        যদি(net->layers[i].type == DETECTION){
            ফেরত net->layers[i];
        }
    }
    fprintf(stderr, "Detection layer not found!!\n");
    layer l = {0};
    ফেরত l;
}

image get_network_image_layer(network *net, পূর্ণসংখ্যা i)
{
    layer l = net->layers[i];
#ifdef GPU
    //cuda_pull_array(l.output_gpu, l.output, l.outputs);
#endif
    যদি (l.out_w && l.out_h && l.out_c){
        ফেরত float_to_image(l.out_w, l.out_h, l.out_c, l.output);
    }
    image def = {0};
    ফেরত def;
}

image get_network_image(network *net)
{
    পূর্ণসংখ্যা i;
    জন্য(i = net->n-1; i >= 0; --i){
        image m = get_network_image_layer(net, i);
        যদি(m.h != 0) ফেরত m;
    }
    image def = {0};
    ফেরত def;
}

শূন্য visualize_network(network *net)
{
    image *পূর্ব = 0;
    পূর্ণসংখ্যা i;
    অক্ষর buff[256];
    জন্য(i = 0; i < net->n; ++i){
        sprintf(buff, "Layer %d", i);
        layer l = net->layers[i];
        যদি(l.type == CONVOLUTIONAL){
            পূর্ব = visualize_convolutional_layer(l, buff, পূর্ব);
        }
    } 
}

শূন্য শীর্ষ_predictions(network *net, পূর্ণসংখ্যা k, পূর্ণসংখ্যা *সূচক)
{
    শীর্ষ_k(net->output, net->outputs, k, সূচক);
}


দশমিক *network_predict(network *net, দশমিক *প্রবেশ)
{
    network orig = *net;
    net->প্রবেশ = প্রবেশ;
    net->truth = 0;
    net->train = 0;
    net->delta = 0;
    forward_network(net);
    দশমিক *out = net->output;
    *net = orig;
    ফেরত out;
}

পূর্ণসংখ্যা অঙ্ক_detections(network *net, দশমিক thresh)
{
    পূর্ণসংখ্যা i;
    পূর্ণসংখ্যা s = 0;
    জন্য(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        যদি(l.type == YOLO){
            s += yolo_অঙ্ক_detections(l, thresh);
        }
        যদি(l.type == DETECTION || l.type == REGION){
            s += l.w*l.h*l.n;
        }
    }
    ফেরত s;
}

detection *make_network_boxes(network *net, দশমিক thresh, পূর্ণসংখ্যা *অঙ্ক)
{
    layer l = net->layers[net->n - 1];
    পূর্ণসংখ্যা i;
    পূর্ণসংখ্যা nboxes = অঙ্ক_detections(net, thresh);
    যদি(অঙ্ক) *অঙ্ক = nboxes;
    detection *dets = calloc(nboxes, আকার(detection));
    জন্য(i = 0; i < nboxes; ++i){
        dets[i].prob = calloc(l.classes, আকার(দশমিক));
        যদি(l.coords > 4){
            dets[i].mask = calloc(l.coords-4, আকার(দশমিক));
        }
    }
    ফেরত dets;
}

শূন্য fill_network_boxes(network *net, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, দশমিক thresh, দশমিক hier, পূর্ণসংখ্যা *map, পূর্ণসংখ্যা relative, detection *dets)
{
    পূর্ণসংখ্যা j;
    জন্য(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        যদি(l.type == YOLO){
            পূর্ণসংখ্যা গুনতি = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
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

detection *get_network_boxes(network *net, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, দশমিক thresh, দশমিক hier, পূর্ণসংখ্যা *map, পূর্ণসংখ্যা relative, পূর্ণসংখ্যা *অঙ্ক)
{
    detection *dets = make_network_boxes(net, thresh, অঙ্ক);
    fill_network_boxes(net, w, h, thresh, hier, map, relative, dets);
    ফেরত dets;
}

শূন্য free_detections(detection *dets, পূর্ণসংখ্যা n)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < n; ++i){
        মুক্ত(dets[i].prob);
        যদি(dets[i].mask) মুক্ত(dets[i].mask);
    }
    মুক্ত(dets);
}

দশমিক *network_predict_image(network *net, image im)
{
    image imr = letterbox_image(im, net->w, net->h);
    set_batch_network(net, 1);
    দশমিক *p = network_predict(net, imr.তথ্য);
    free_image(imr);
    ফেরত p;
}

পূর্ণসংখ্যা network_width(network *net){ফেরত net->w;}
পূর্ণসংখ্যা network_height(network *net){ফেরত net->h;}

matrix network_predict_তথ্য_multi(network *net, তথ্য test, পূর্ণসংখ্যা n)
{
    পূর্ণসংখ্যা i,j,b,m;
    পূর্ণসংখ্যা k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    দশমিক *X = calloc(net->batch*test.X.rows, আকার(দশমিক));
    জন্য(i = 0; i < test.X.rows; i += net->batch){
        জন্য(b = 0; b < net->batch; ++b){
            যদি(i+b == test.X.rows) বিরতি;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*আকার(দশমিক));
        }
        জন্য(m = 0; m < n; ++m){
            দশমিক *out = network_predict(net, X);
            জন্য(b = 0; b < net->batch; ++b){
                যদি(i+b == test.X.rows) বিরতি;
                জন্য(j = 0; j < k; ++j){
                    pred.vals[i+b][j] += out[j+b*k]/n;
                }
            }
        }
    }
    মুক্ত(X);
    ফেরত pred;   
}

matrix network_predict_তথ্য(network *net, তথ্য test)
{
    পূর্ণসংখ্যা i,j,b;
    পূর্ণসংখ্যা k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    দশমিক *X = calloc(net->batch*test.X.cols, আকার(দশমিক));
    জন্য(i = 0; i < test.X.rows; i += net->batch){
        জন্য(b = 0; b < net->batch; ++b){
            যদি(i+b == test.X.rows) বিরতি;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*আকার(দশমিক));
        }
        দশমিক *out = network_predict(net, X);
        জন্য(b = 0; b < net->batch; ++b){
            যদি(i+b == test.X.rows) বিরতি;
            জন্য(j = 0; j < k; ++j){
                pred.vals[i+b][j] = out[j+b*k];
            }
        }
    }
    মুক্ত(X);
    ফেরত pred;   
}

শূন্য print_network(network *net)
{
    পূর্ণসংখ্যা i,j;
    জন্য(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        দশমিক *output = l.output;
        পূর্ণসংখ্যা n = l.outputs;
        দশমিক mean = mean_array(output, n);
        দশমিক vari = variance_array(output, n);
        fprintf(stderr, "Layer %d - Mean: %f, Variance: %f\n",i,mean, vari);
        যদি(n > 100) n = 100;
        জন্য(j = 0; j < n; ++j) fprintf(stderr, "%f, ", output[j]);
        যদি(n == 100)fprintf(stderr,".....\n");
        fprintf(stderr, "\n");
    }
}

শূন্য compare_networks(network *n1, network *n2, তথ্য test)
{
    matrix g1 = network_predict_তথ্য(n1, test);
    matrix g2 = network_predict_তথ্য(n2, test);
    পূর্ণসংখ্যা i;
    পূর্ণসংখ্যা a,b,c,d;
    a = b = c = d = 0;
    জন্য(i = 0; i < g1.rows; ++i){
        পূর্ণসংখ্যা truth = max_সূচক(test.y.vals[i], test.y.cols);
        পূর্ণসংখ্যা p1 = max_সূচক(g1.vals[i], g1.cols);
        পূর্ণসংখ্যা p2 = max_সূচক(g2.vals[i], g2.cols);
        যদি(p1 == truth){
            যদি(p2 == truth) ++d;
            অন্যথা ++c;
        }অন্যথা{
            যদি(p2 == truth) ++b;
            অন্যথা ++a;
        }
    }
    printf("%5d %5d\n%5d %5d\n", a, b, c, d);
    দশমিক অঙ্ক = pow((abs(b - c) - 1.), 2.);
    দশমিক den = b + c;
    printf("%f\n", অঙ্ক/den); 
}

দশমিক network_accuracy(network *net, তথ্য d)
{
    matrix guess = network_predict_তথ্য(net, d);
    দশমিক acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    ফেরত acc;
}

দশমিক *network_accuracies(network *net, তথ্য d, পূর্ণসংখ্যা n)
{
    স্থির দশমিক acc[2];
    matrix guess = network_predict_তথ্য(net, d);
    acc[0] = matrix_topk_accuracy(d.y, guess, 1);
    acc[1] = matrix_topk_accuracy(d.y, guess, n);
    free_matrix(guess);
    ফেরত acc;
}

layer get_network_output_layer(network *net)
{
    পূর্ণসংখ্যা i;
    জন্য(i = net->n - 1; i >= 0; --i){
        যদি(net->layers[i].type != COST) বিরতি;
    }
    ফেরত net->layers[i];
}

দশমিক network_accuracy_multi(network *net, তথ্য d, পূর্ণসংখ্যা n)
{
    matrix guess = network_predict_তথ্য_multi(net, d, n);
    দশমিক acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    ফেরত acc;
}

শূন্য free_network(network *net)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < net->n; ++i){
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
    পূর্ণসংখ্যা i;
    জন্য(i = net->n - 1; i >= 0; --i){
        যদি(net->layers[i].type != COST) বিরতি;
    }
    ফেরত net->layers[i];
}

পূর্ণসংখ্যা network_inputs(network *net)
{
    ফেরত net->layers[0].inputs;
}

পূর্ণসংখ্যা network_outputs(network *net)
{
    ফেরত network_output_layer(net).outputs;
}

দশমিক *network_output(network *net)
{
    ফেরত network_output_layer(net).output;
}

#ifdef GPU

শূন্য forward_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_সূচক);
    cuda_ঠেলো_array(net.প্রবেশ_gpu, net.প্রবেশ, net.inputs*net.batch);
    যদি(net.truth){
        cuda_ঠেলো_array(net.truth_gpu, net.truth, net.truths*net.batch);
    }

    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < net.n; ++i){
        net.সূচক = i;
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
    পূর্ণসংখ্যা i;
    network net = *netp;
    network orig = net;
    cuda_set_device(net.gpu_সূচক);
    জন্য(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        যদি(l.stopbackward) বিরতি;
        যদি(i == 0){
            net = orig;
        }অন্যথা{
            layer পূর্ব = net.layers[i-1];
            net.প্রবেশ = পূর্ব.output;
            net.delta = পূর্ব.delta;
            net.প্রবেশ_gpu = পূর্ব.output_gpu;
            net.delta_gpu = পূর্ব.delta_gpu;
        }
        net.সূচক = i;
        l.backward_gpu(l, net);
    }
}

শূন্য update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_সূচক);
    পূর্ণসংখ্যা i;
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

    জন্য(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        যদি(l.update_gpu){
            l.update_gpu(l, a);
        }
    }
}

শূন্য harmless_update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_সূচক);
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        যদি(l.weight_updates_gpu) fill_gpu(l.nweights, 0, l.weight_updates_gpu, 1);
        যদি(l.bias_updates_gpu) fill_gpu(l.nbiases, 0, l.bias_updates_gpu, 1);
        যদি(l.scale_updates_gpu) fill_gpu(l.nbiases, 0, l.scale_updates_gpu, 1);
    }
}

প্রকারলেখ কাঠামো {
    network *net;
    তথ্য d;
    দশমিক *err;
} train_args;

শূন্য *train_thread(শূন্য *ptr)
{
    train_args args = *(train_args*)ptr;
    মুক্ত(ptr);
    cuda_set_device(args.net->gpu_সূচক);
    *args.err = train_network(args.net, args.d);
    ফেরত 0;
}

pthread_t train_network_in_thread(network *net, তথ্য d, দশমিক *err)
{
    pthread_t thread;
    train_args *ptr = (train_args *)calloc(1, আকার(train_args));
    ptr->net = net;
    ptr->d = d;
    ptr->err = err;
    যদি(pthread_create(&thread, 0, train_thread, ptr)) error("Thread creation failed");
    ফেরত thread;
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

শূন্য scale_weights(layer l, দশমিক s)
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

শূন্য sync_layer(network **nets, পূর্ণসংখ্যা n, পূর্ণসংখ্যা j)
{
    পূর্ণসংখ্যা i;
    network *net = nets[0];
    layer base = net->layers[j];
    scale_weights(base, 0);
    জন্য (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_সূচক);
        layer l = nets[i]->layers[j];
        pull_weights(l);
        merge_weights(l, base);
    }
    scale_weights(base, 1./n);
    জন্য (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_সূচক);
        layer l = nets[i]->layers[j];
        distribute_weights(l, base);
    }
}

প্রকারলেখ কাঠামো{
    network **nets;
    পূর্ণসংখ্যা n;
    পূর্ণসংখ্যা j;
} sync_args;

শূন্য *sync_layer_thread(শূন্য *ptr)
{
    sync_args args = *(sync_args*)ptr;
    sync_layer(args.nets, args.n, args.j);
    মুক্ত(ptr);
    ফেরত 0;
}

pthread_t sync_layer_in_thread(network **nets, পূর্ণসংখ্যা n, পূর্ণসংখ্যা j)
{
    pthread_t thread;
    sync_args *ptr = (sync_args *)calloc(1, আকার(sync_args));
    ptr->nets = nets;
    ptr->n = n;
    ptr->j = j;
    যদি(pthread_create(&thread, 0, sync_layer_thread, ptr)) error("Thread creation failed");
    ফেরত thread;
}

শূন্য sync_nets(network **nets, পূর্ণসংখ্যা n, পূর্ণসংখ্যা interval)
{
    পূর্ণসংখ্যা j;
    পূর্ণসংখ্যা layers = nets[0]->n;
    pthread_t *threads = (pthread_t *) calloc(layers, আকার(pthread_t));

    *(nets[0]->seen) += interval * (n-1) * nets[0]->batch * nets[0]->subdivisions;
    জন্য (j = 0; j < n; ++j){
        *(nets[j]->seen) = *(nets[0]->seen);
    }
    জন্য (j = 0; j < layers; ++j) {
        threads[j] = sync_layer_in_thread(nets, n, j);
    }
    জন্য (j = 0; j < layers; ++j) {
        pthread_join(threads[j], 0);
    }
    মুক্ত(threads);
}

দশমিক train_networks(network **nets, পূর্ণসংখ্যা n, তথ্য d, পূর্ণসংখ্যা interval)
{
    পূর্ণসংখ্যা i;
    পূর্ণসংখ্যা batch = nets[0]->batch;
    পূর্ণসংখ্যা subdivisions = nets[0]->subdivisions;
    প্রতিজ্ঞা(batch * subdivisions * n == d.X.rows);
    pthread_t *threads = (pthread_t *) calloc(n, আকার(pthread_t));
    দশমিক *errors = (দশমিক *) calloc(n, আকার(দশমিক));

    দশমিক যোগ = 0;
    জন্য(i = 0; i < n; ++i){
        তথ্য p = get_তথ্য_part(d, i, n);
        threads[i] = train_network_in_thread(nets[i], p, errors + i);
    }
    জন্য(i = 0; i < n; ++i){
        pthread_join(threads[i], 0);
        //printf("%f\n", errors[i]);
        যোগ += errors[i];
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
    ফেরত (দশমিক)যোগ/(n);
}

শূন্য pull_network_output(network *net)
{
    layer l = get_network_output_layer(net);
    cuda_pull_array(l.output_gpu, l.output, l.outputs*l.batch);
}

#endif

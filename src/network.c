#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <time.h>
#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ "network.h"
#અન્તર્ભાવ "image.h"
#અન્તર્ભાવ "data.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "blas.h"

#અન્તર્ભાવ "crop_layer.h"
#અન્તર્ભાવ "connected_layer.h"
#અન્તર્ભાવ "gru_layer.h"
#અન્તર્ભાવ "rnn_layer.h"
#અન્તર્ભાવ "crnn_layer.h"
#અન્તર્ભાવ "local_layer.h"
#અન્તર્ભાવ "convolutional_layer.h"
#અન્તર્ભાવ "activation_layer.h"
#અન્તર્ભાવ "detection_layer.h"
#અન્તર્ભાવ "region_layer.h"
#અન્તર્ભાવ "yolo_layer.h"
#અન્તર્ભાવ "normalization_layer.h"
#અન્તર્ભાવ "batchnorm_layer.h"
#અન્તર્ભાવ "maxpool_layer.h"
#અન્તર્ભાવ "reorg_layer.h"
#અન્તર્ભાવ "avgpool_layer.h"
#અન્તર્ભાવ "cost_layer.h"
#અન્તર્ભાવ "softmax_layer.h"
#અન્તર્ભાવ "dropout_layer.h"
#અન્તર્ભાવ "route_layer.h"
#અન્તર્ભાવ "upsample_layer.h"
#અન્તર્ભાવ "shortcut_layer.h"
#અન્તર્ભાવ "parser.h"
#અન્તર્ભાવ "data.h"

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
    પાછા args;
}

network *load_network(અક્ષર *cfg, અક્ષર *weights, પૂર્ણાંક clear)
{
    network *net = parse_network_cfg(cfg);
    જો(weights && weights[0] != 0){
        load_weights(net, weights);
    }
    જો(clear) (*net->seen) = 0;
    પાછા net;
}

size_t get_current_batch(network *net)
{
    size_t batch_અંક = (*net->seen)/(net->batch*net->subdivisions);
    પાછા batch_અંક;
}

શૂન્ય reset_network_state(network *net, પૂર્ણાંક b)
{
    પૂર્ણાંક i;
    માટે (i = 0; i < net->n; ++i) {
        #ifdef GPU
        layer l = net->layers[i];
        જો(l.state_gpu){
            fill_gpu(l.outputs, 0, l.state_gpu + l.outputs*b, 1);
        }
        જો(l.h_gpu){
            fill_gpu(l.outputs, 0, l.h_gpu + l.outputs*b, 1);
        }
        #endif
    }
}

શૂન્ય reset_rnn(network *net)
{
    reset_network_state(net, 0);
}

દશાંશ get_current_rate(network *net)
{
    size_t batch_અંક = get_current_batch(net);
    પૂર્ણાંક i;
    દશાંશ rate;
    જો (batch_અંક < net->burn_in) પાછા net->learning_rate * pow((દશાંશ)batch_અંક / net->burn_in, net->power);
    પસંદગી (net->policy) {
        સ્થિતિ CONSTANT:
            પાછા net->learning_rate;
        સ્થિતિ STEP:
            પાછા net->learning_rate * pow(net->scale, batch_અંક/net->step);
        સ્થિતિ STEPS:
            rate = net->learning_rate;
            માટે(i = 0; i < net->અંક_steps; ++i){
                જો(net->steps[i] > batch_અંક) પાછા rate;
                rate *= net->scales[i];
            }
            પાછા rate;
        સ્થિતિ EXP:
            પાછા net->learning_rate * pow(net->gamma, batch_અંક);
        સ્થિતિ POLY:
            પાછા net->learning_rate * pow(1 - (દશાંશ)batch_અંક / net->max_batches, net->power);
        સ્થિતિ RANDOM:
            પાછા net->learning_rate * pow(rand_uniform(0,1), net->power);
        સ્થિતિ SIG:
            પાછા net->learning_rate * (1./(1.+exp(net->gamma*(batch_અંક - net->step))));
        મૂળભૂત:
            fprintf(stderr, "Policy is weird!\n");
            પાછા net->learning_rate;
    }
}

અક્ષર *get_layer_string(LAYER_TYPE a)
{
    પસંદગી(a){
        સ્થિતિ CONVOLUTIONAL:
            પાછા "convolutional";
        સ્થિતિ ACTIVE:
            પાછા "activation";
        સ્થિતિ LOCAL:
            પાછા "local";
        સ્થિતિ DECONVOLUTIONAL:
            પાછા "deconvolutional";
        સ્થિતિ CONNECTED:
            પાછા "connected";
        સ્થિતિ RNN:
            પાછા "rnn";
        સ્થિતિ GRU:
            પાછા "gru";
        સ્થિતિ LSTM:
	    પાછા "lstm";
        સ્થિતિ CRNN:
            પાછા "crnn";
        સ્થિતિ MAXPOOL:
            પાછા "maxpool";
        સ્થિતિ REORG:
            પાછા "reorg";
        સ્થિતિ AVGPOOL:
            પાછા "avgpool";
        સ્થિતિ SOFTMAX:
            પાછા "softmax";
        સ્થિતિ DETECTION:
            પાછા "detection";
        સ્થિતિ REGION:
            પાછા "region";
        સ્થિતિ YOLO:
            પાછા "yolo";
        સ્થિતિ DROPOUT:
            પાછા "dropout";
        સ્થિતિ CROP:
            પાછા "crop";
        સ્થિતિ COST:
            પાછા "cost";
        સ્થિતિ ROUTE:
            પાછા "route";
        સ્થિતિ SHORTCUT:
            પાછા "shortcut";
        સ્થિતિ NORMALIZATION:
            પાછા "normalization";
        સ્થિતિ BATCHNORM:
            પાછા "batchnorm";
        મૂળભૂત:
            તોડો;
    }
    પાછા "none";
}

network *make_network(પૂર્ણાંક n)
{
    network *net = calloc(1, કદ(network));
    net->n = n;
    net->layers = calloc(net->n, કદ(layer));
    net->seen = calloc(1, કદ(size_t));
    net->t    = calloc(1, કદ(પૂર્ણાંક));
    net->cost = calloc(1, કદ(દશાંશ));
    પાછા net;
}

શૂન્ય forward_network(network *netp)
{
#ifdef GPU
    જો(netp->gpu_index >= 0){
        forward_network_gpu(netp);   
        પાછા;
    }
#endif
    network net = *netp;
    પૂર્ણાંક i;
    માટે(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        જો(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.આગત = l.output;
        જો(l.truth) {
            net.truth = l.output;
        }
    }
    calc_network_cost(netp);
}

શૂન્ય update_network(network *netp)
{
#ifdef GPU
    જો(netp->gpu_index >= 0){
        update_network_gpu(netp);   
        પાછા;
    }
#endif
    network net = *netp;
    પૂર્ણાંક i;
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

    માટે(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        જો(l.update){
            l.update(l, a);
        }
    }
}

શૂન્ય calc_network_cost(network *netp)
{
    network net = *netp;
    પૂર્ણાંક i;
    દશાંશ sum = 0;
    પૂર્ણાંક ગણતરી = 0;
    માટે(i = 0; i < net.n; ++i){
        જો(net.layers[i].cost){
            sum += net.layers[i].cost[0];
            ++ગણતરી;
        }
    }
    *net.cost = sum/ગણતરી;
}

પૂર્ણાંક get_predicted_class_network(network *net)
{
    પાછા max_index(net->output, net->outputs);
}

શૂન્ય backward_network(network *netp)
{
#ifdef GPU
    જો(netp->gpu_index >= 0){
        backward_network_gpu(netp);   
        પાછા;
    }
#endif
    network net = *netp;
    પૂર્ણાંક i;
    network orig = net;
    માટે(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        જો(l.stopbackward) તોડો;
        જો(i == 0){
            net = orig;
        }નહીંતર{
            layer prev = net.layers[i-1];
            net.આગત = prev.output;
            net.delta = prev.delta;
        }
        net.index = i;
        l.backward(l, net);
    }
}

દશાંશ train_network_datum(network *net)
{
    *net->seen += net->batch;
    net->train = 1;
    forward_network(net);
    backward_network(net);
    દશાંશ error = *net->cost;
    જો(((*net->seen)/net->batch)%net->subdivisions == 0) update_network(net);
    પાછા error;
}

દશાંશ train_network_sgd(network *net, data d, પૂર્ણાંક n)
{
    પૂર્ણાંક batch = net->batch;

    પૂર્ણાંક i;
    દશાંશ sum = 0;
    માટે(i = 0; i < n; ++i){
        get_random_batch(d, batch, net->આગત, net->truth);
        દશાંશ err = train_network_datum(net);
        sum += err;
    }
    પાછા (દશાંશ)sum/(n*batch);
}

દશાંશ train_network(network *net, data d)
{
    પ્રતિજ્ઞા(d.X.rows % net->batch == 0);
    પૂર્ણાંક batch = net->batch;
    પૂર્ણાંક n = d.X.rows / batch;

    પૂર્ણાંક i;
    દશાંશ sum = 0;
    માટે(i = 0; i < n; ++i){
        get_next_batch(d, batch, i*batch, net->આગત, net->truth);
        દશાંશ err = train_network_datum(net);
        sum += err;
    }
    પાછા (દશાંશ)sum/(n*batch);
}

શૂન્ય set_temp_network(network *net, દશાંશ t)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < net->n; ++i){
        net->layers[i].temperature = t;
    }
}


શૂન્ય set_batch_network(network *net, પૂર્ણાંક b)
{
    net->batch = b;
    પૂર્ણાંક i;
    માટે(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#ifdef CUDNN
        જો(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_setup(net->layers + i);
        }
        જો(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, l->out_h, l->out_w);
            cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 
        }
#endif
    }
}

પૂર્ણાંક resize_network(network *net, પૂર્ણાંક w, પૂર્ણાંક h)
{
#ifdef GPU
    cuda_set_device(net->gpu_index);
    cuda_free(net->workspace);
#endif
    પૂર્ણાંક i;
    //if(w == net->w && h == net->h) return 0;
    net->w = w;
    net->h = h;
    પૂર્ણાંક inputs = 0;
    size_t workspace_size = 0;
    //fprintf(stderr, "Resizing to %d x %d...\n", w, h);
    //fflush(stderr);
    માટે (i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        જો(l.type == CONVOLUTIONAL){
            resize_convolutional_layer(&l, w, h);
        }નહીંતર જો(l.type == CROP){
            resize_crop_layer(&l, w, h);
        }નહીંતર જો(l.type == MAXPOOL){
            resize_maxpool_layer(&l, w, h);
        }નહીંતર જો(l.type == REGION){
            resize_region_layer(&l, w, h);
        }નહીંતર જો(l.type == YOLO){
            resize_yolo_layer(&l, w, h);
        }નહીંતર જો(l.type == ROUTE){
            resize_route_layer(&l, net);
        }નહીંતર જો(l.type == SHORTCUT){
            resize_shortcut_layer(&l, w, h);
        }નહીંતર જો(l.type == UPSAMPLE){
            resize_upsample_layer(&l, w, h);
        }નહીંતર જો(l.type == REORG){
            resize_reorg_layer(&l, w, h);
        }નહીંતર જો(l.type == AVGPOOL){
            resize_avgpool_layer(&l, w, h);
        }નહીંતર જો(l.type == NORMALIZATION){
            resize_normalization_layer(&l, w, h);
        }નહીંતર જો(l.type == COST){
            resize_cost_layer(&l, inputs);
        }નહીંતર{
            error("Cannot resize this type of layer");
        }
        જો(l.workspace_size > workspace_size) workspace_size = l.workspace_size;
        જો(l.workspace_size > 2000000000) પ્રતિજ્ઞા(0);
        inputs = l.outputs;
        net->layers[i] = l;
        w = l.out_w;
        h = l.out_h;
        જો(l.type == AVGPOOL) તોડો;
    }
    layer out = get_network_output_layer(net);
    net->inputs = net->layers[0].inputs;
    net->outputs = out.outputs;
    net->truths = out.outputs;
    જો(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    મુક્ત(net->આગત);
    મુક્ત(net->truth);
    net->આગત = calloc(net->inputs*net->batch, કદ(દશાંશ));
    net->truth = calloc(net->truths*net->batch, કદ(દશાંશ));
#ifdef GPU
    જો(gpu_index >= 0){
        cuda_free(net->આગત_gpu);
        cuda_free(net->truth_gpu);
        net->આગત_gpu = cuda_make_array(net->આગત, net->inputs*net->batch);
        net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
        જો(workspace_size){
            net->workspace = cuda_make_array(0, (workspace_size-1)/કદ(દશાંશ)+1);
        }
    }નહીંતર {
        મુક્ત(net->workspace);
        net->workspace = calloc(1, workspace_size);
    }
#નહીંતર
    મુક્ત(net->workspace);
    net->workspace = calloc(1, workspace_size);
#endif
    //fprintf(stderr, " Done!\n");
    પાછા 0;
}

layer get_network_detection_layer(network *net)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < net->n; ++i){
        જો(net->layers[i].type == DETECTION){
            પાછા net->layers[i];
        }
    }
    fprintf(stderr, "Detection layer not found!!\n");
    layer l = {0};
    પાછા l;
}

image get_network_image_layer(network *net, પૂર્ણાંક i)
{
    layer l = net->layers[i];
#ifdef GPU
    //cuda_pull_array(l.output_gpu, l.output, l.outputs);
#endif
    જો (l.out_w && l.out_h && l.out_c){
        પાછા float_to_image(l.out_w, l.out_h, l.out_c, l.output);
    }
    image def = {0};
    પાછા def;
}

image get_network_image(network *net)
{
    પૂર્ણાંક i;
    માટે(i = net->n-1; i >= 0; --i){
        image m = get_network_image_layer(net, i);
        જો(m.h != 0) પાછા m;
    }
    image def = {0};
    પાછા def;
}

શૂન્ય visualize_network(network *net)
{
    image *prev = 0;
    પૂર્ણાંક i;
    અક્ષર buff[256];
    માટે(i = 0; i < net->n; ++i){
        sprintf(buff, "Layer %d", i);
        layer l = net->layers[i];
        જો(l.type == CONVOLUTIONAL){
            prev = visualize_convolutional_layer(l, buff, prev);
        }
    } 
}

શૂન્ય ટોચ_predictions(network *net, પૂર્ણાંક k, પૂર્ણાંક *index)
{
    ટોચ_k(net->output, net->outputs, k, index);
}


દશાંશ *network_predict(network *net, દશાંશ *આગત)
{
    network orig = *net;
    net->આગત = આગત;
    net->truth = 0;
    net->train = 0;
    net->delta = 0;
    forward_network(net);
    દશાંશ *out = net->output;
    *net = orig;
    પાછા out;
}

પૂર્ણાંક અંક_detections(network *net, દશાંશ thresh)
{
    પૂર્ણાંક i;
    પૂર્ણાંક s = 0;
    માટે(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        જો(l.type == YOLO){
            s += yolo_અંક_detections(l, thresh);
        }
        જો(l.type == DETECTION || l.type == REGION){
            s += l.w*l.h*l.n;
        }
    }
    પાછા s;
}

detection *make_network_boxes(network *net, દશાંશ thresh, પૂર્ણાંક *અંક)
{
    layer l = net->layers[net->n - 1];
    પૂર્ણાંક i;
    પૂર્ણાંક nboxes = અંક_detections(net, thresh);
    જો(અંક) *અંક = nboxes;
    detection *dets = calloc(nboxes, કદ(detection));
    માટે(i = 0; i < nboxes; ++i){
        dets[i].prob = calloc(l.classes, કદ(દશાંશ));
        જો(l.coords > 4){
            dets[i].mask = calloc(l.coords-4, કદ(દશાંશ));
        }
    }
    પાછા dets;
}

શૂન્ય fill_network_boxes(network *net, પૂર્ણાંક w, પૂર્ણાંક h, દશાંશ thresh, દશાંશ hier, પૂર્ણાંક *map, પૂર્ણાંક relative, detection *dets)
{
    પૂર્ણાંક j;
    માટે(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        જો(l.type == YOLO){
            પૂર્ણાંક ગણતરી = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
            dets += ગણતરી;
        }
        જો(l.type == REGION){
            get_region_detections(l, w, h, net->w, net->h, thresh, map, hier, relative, dets);
            dets += l.w*l.h*l.n;
        }
        જો(l.type == DETECTION){
            get_detection_detections(l, w, h, thresh, dets);
            dets += l.w*l.h*l.n;
        }
    }
}

detection *get_network_boxes(network *net, પૂર્ણાંક w, પૂર્ણાંક h, દશાંશ thresh, દશાંશ hier, પૂર્ણાંક *map, પૂર્ણાંક relative, પૂર્ણાંક *અંક)
{
    detection *dets = make_network_boxes(net, thresh, અંક);
    fill_network_boxes(net, w, h, thresh, hier, map, relative, dets);
    પાછા dets;
}

શૂન્ય free_detections(detection *dets, પૂર્ણાંક n)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < n; ++i){
        મુક્ત(dets[i].prob);
        જો(dets[i].mask) મુક્ત(dets[i].mask);
    }
    મુક્ત(dets);
}

દશાંશ *network_predict_image(network *net, image im)
{
    image imr = letterbox_image(im, net->w, net->h);
    set_batch_network(net, 1);
    દશાંશ *p = network_predict(net, imr.data);
    free_image(imr);
    પાછા p;
}

પૂર્ણાંક network_width(network *net){પાછા net->w;}
પૂર્ણાંક network_height(network *net){પાછા net->h;}

matrix network_predict_data_multi(network *net, data test, પૂર્ણાંક n)
{
    પૂર્ણાંક i,j,b,m;
    પૂર્ણાંક k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    દશાંશ *X = calloc(net->batch*test.X.rows, કદ(દશાંશ));
    માટે(i = 0; i < test.X.rows; i += net->batch){
        માટે(b = 0; b < net->batch; ++b){
            જો(i+b == test.X.rows) તોડો;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*કદ(દશાંશ));
        }
        માટે(m = 0; m < n; ++m){
            દશાંશ *out = network_predict(net, X);
            માટે(b = 0; b < net->batch; ++b){
                જો(i+b == test.X.rows) તોડો;
                માટે(j = 0; j < k; ++j){
                    pred.vals[i+b][j] += out[j+b*k]/n;
                }
            }
        }
    }
    મુક્ત(X);
    પાછા pred;   
}

matrix network_predict_data(network *net, data test)
{
    પૂર્ણાંક i,j,b;
    પૂર્ણાંક k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    દશાંશ *X = calloc(net->batch*test.X.cols, કદ(દશાંશ));
    માટે(i = 0; i < test.X.rows; i += net->batch){
        માટે(b = 0; b < net->batch; ++b){
            જો(i+b == test.X.rows) તોડો;
            memcpy(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*કદ(દશાંશ));
        }
        દશાંશ *out = network_predict(net, X);
        માટે(b = 0; b < net->batch; ++b){
            જો(i+b == test.X.rows) તોડો;
            માટે(j = 0; j < k; ++j){
                pred.vals[i+b][j] = out[j+b*k];
            }
        }
    }
    મુક્ત(X);
    પાછા pred;   
}

શૂન્ય print_network(network *net)
{
    પૂર્ણાંક i,j;
    માટે(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        દશાંશ *output = l.output;
        પૂર્ણાંક n = l.outputs;
        દશાંશ mean = mean_array(output, n);
        દશાંશ vari = variance_array(output, n);
        fprintf(stderr, "Layer %d - Mean: %f, Variance: %f\n",i,mean, vari);
        જો(n > 100) n = 100;
        માટે(j = 0; j < n; ++j) fprintf(stderr, "%f, ", output[j]);
        જો(n == 100)fprintf(stderr,".....\n");
        fprintf(stderr, "\n");
    }
}

શૂન્ય compare_networks(network *n1, network *n2, data test)
{
    matrix g1 = network_predict_data(n1, test);
    matrix g2 = network_predict_data(n2, test);
    પૂર્ણાંક i;
    પૂર્ણાંક a,b,c,d;
    a = b = c = d = 0;
    માટે(i = 0; i < g1.rows; ++i){
        પૂર્ણાંક truth = max_index(test.y.vals[i], test.y.cols);
        પૂર્ણાંક p1 = max_index(g1.vals[i], g1.cols);
        પૂર્ણાંક p2 = max_index(g2.vals[i], g2.cols);
        જો(p1 == truth){
            જો(p2 == truth) ++d;
            નહીંતર ++c;
        }નહીંતર{
            જો(p2 == truth) ++b;
            નહીંતર ++a;
        }
    }
    printf("%5d %5d\n%5d %5d\n", a, b, c, d);
    દશાંશ અંક = pow((abs(b - c) - 1.), 2.);
    દશાંશ den = b + c;
    printf("%f\n", અંક/den); 
}

દશાંશ network_accuracy(network *net, data d)
{
    matrix guess = network_predict_data(net, d);
    દશાંશ acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    પાછા acc;
}

દશાંશ *network_accuracies(network *net, data d, પૂર્ણાંક n)
{
    સ્થિર દશાંશ acc[2];
    matrix guess = network_predict_data(net, d);
    acc[0] = matrix_topk_accuracy(d.y, guess, 1);
    acc[1] = matrix_topk_accuracy(d.y, guess, n);
    free_matrix(guess);
    પાછા acc;
}

layer get_network_output_layer(network *net)
{
    પૂર્ણાંક i;
    માટે(i = net->n - 1; i >= 0; --i){
        જો(net->layers[i].type != COST) તોડો;
    }
    પાછા net->layers[i];
}

દશાંશ network_accuracy_multi(network *net, data d, પૂર્ણાંક n)
{
    matrix guess = network_predict_data_multi(net, d, n);
    દશાંશ acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    પાછા acc;
}

શૂન્ય free_network(network *net)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < net->n; ++i){
        free_layer(net->layers[i]);
    }
    મુક્ત(net->layers);
    જો(net->આગત) મુક્ત(net->આગત);
    જો(net->truth) મુક્ત(net->truth);
#ifdef GPU
    જો(net->આગત_gpu) cuda_free(net->આગત_gpu);
    જો(net->truth_gpu) cuda_free(net->truth_gpu);
#endif
    મુક્ત(net);
}

// Some day...
// ^ What the hell is this comment for?


layer network_output_layer(network *net)
{
    પૂર્ણાંક i;
    માટે(i = net->n - 1; i >= 0; --i){
        જો(net->layers[i].type != COST) તોડો;
    }
    પાછા net->layers[i];
}

પૂર્ણાંક network_inputs(network *net)
{
    પાછા net->layers[0].inputs;
}

પૂર્ણાંક network_outputs(network *net)
{
    પાછા network_output_layer(net).outputs;
}

દશાંશ *network_output(network *net)
{
    પાછા network_output_layer(net).output;
}

#ifdef GPU

શૂન્ય forward_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    cuda_ધકેલો_array(net.આગત_gpu, net.આગત, net.inputs*net.batch);
    જો(net.truth){
        cuda_ધકેલો_array(net.truth_gpu, net.truth, net.truths*net.batch);
    }

    પૂર્ણાંક i;
    માટે(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        જો(l.delta_gpu){
            fill_gpu(l.outputs * l.batch, 0, l.delta_gpu, 1);
        }
        l.forward_gpu(l, net);
        net.આગત_gpu = l.output_gpu;
        net.આગત = l.output;
        જો(l.truth) {
            net.truth_gpu = l.output_gpu;
            net.truth = l.output;
        }
    }
    pull_network_output(netp);
    calc_network_cost(netp);
}

શૂન્ય backward_network_gpu(network *netp)
{
    પૂર્ણાંક i;
    network net = *netp;
    network orig = net;
    cuda_set_device(net.gpu_index);
    માટે(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        જો(l.stopbackward) તોડો;
        જો(i == 0){
            net = orig;
        }નહીંતર{
            layer prev = net.layers[i-1];
            net.આગત = prev.output;
            net.delta = prev.delta;
            net.આગત_gpu = prev.output_gpu;
            net.delta_gpu = prev.delta_gpu;
        }
        net.index = i;
        l.backward_gpu(l, net);
    }
}

શૂન્ય update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    પૂર્ણાંક i;
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

    માટે(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        જો(l.update_gpu){
            l.update_gpu(l, a);
        }
    }
}

શૂન્ય harmless_update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    પૂર્ણાંક i;
    માટે(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        જો(l.weight_updates_gpu) fill_gpu(l.nweights, 0, l.weight_updates_gpu, 1);
        જો(l.bias_updates_gpu) fill_gpu(l.nbiases, 0, l.bias_updates_gpu, 1);
        જો(l.scale_updates_gpu) fill_gpu(l.nbiases, 0, l.scale_updates_gpu, 1);
    }
}

પ્રકારવ્યાખ્યા સંરચના {
    network *net;
    data d;
    દશાંશ *err;
} train_args;

શૂન્ય *train_thread(શૂન્ય *ptr)
{
    train_args args = *(train_args*)ptr;
    મુક્ત(ptr);
    cuda_set_device(args.net->gpu_index);
    *args.err = train_network(args.net, args.d);
    પાછા 0;
}

pthread_t train_network_in_thread(network *net, data d, દશાંશ *err)
{
    pthread_t thread;
    train_args *ptr = (train_args *)calloc(1, કદ(train_args));
    ptr->net = net;
    ptr->d = d;
    ptr->err = err;
    જો(pthread_create(&thread, 0, train_thread, ptr)) error("Thread creation failed");
    પાછા thread;
}

શૂન્ય merge_weights(layer l, layer base)
{
    જો (l.type == CONVOLUTIONAL) {
        axpy_cpu(l.n, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.nweights, 1, l.weight_updates, 1, base.weights, 1);
        જો (l.scales) {
            axpy_cpu(l.n, 1, l.scale_updates, 1, base.scales, 1);
        }
    } નહીંતર જો(l.type == CONNECTED) {
        axpy_cpu(l.outputs, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.outputs*l.inputs, 1, l.weight_updates, 1, base.weights, 1);
    }
}

શૂન્ય scale_weights(layer l, દશાંશ s)
{
    જો (l.type == CONVOLUTIONAL) {
        scal_cpu(l.n, s, l.biases, 1);
        scal_cpu(l.nweights, s, l.weights, 1);
        જો (l.scales) {
            scal_cpu(l.n, s, l.scales, 1);
        }
    } નહીંતર જો(l.type == CONNECTED) {
        scal_cpu(l.outputs, s, l.biases, 1);
        scal_cpu(l.outputs*l.inputs, s, l.weights, 1);
    }
}


શૂન્ય pull_weights(layer l)
{
    જો(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.n);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.nweights);
        જો(l.scales) cuda_pull_array(l.scales_gpu, l.scale_updates, l.n);
    } નહીંતર જો(l.type == CONNECTED){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.outputs);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.outputs*l.inputs);
    }
}

શૂન્ય ધકેલો_weights(layer l)
{
    જો(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_ધકેલો_array(l.biases_gpu, l.biases, l.n);
        cuda_ધકેલો_array(l.weights_gpu, l.weights, l.nweights);
        જો(l.scales) cuda_ધકેલો_array(l.scales_gpu, l.scales, l.n);
    } નહીંતર જો(l.type == CONNECTED){
        cuda_ધકેલો_array(l.biases_gpu, l.biases, l.outputs);
        cuda_ધકેલો_array(l.weights_gpu, l.weights, l.outputs*l.inputs);
    }
}

શૂન્ય distribute_weights(layer l, layer base)
{
    જો (l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL) {
        cuda_ધકેલો_array(l.biases_gpu, base.biases, l.n);
        cuda_ધકેલો_array(l.weights_gpu, base.weights, l.nweights);
        જો (base.scales) cuda_ધકેલો_array(l.scales_gpu, base.scales, l.n);
    } નહીંતર જો (l.type == CONNECTED) {
        cuda_ધકેલો_array(l.biases_gpu, base.biases, l.outputs);
        cuda_ધકેલો_array(l.weights_gpu, base.weights, l.outputs*l.inputs);
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

શૂન્ય sync_layer(network **nets, પૂર્ણાંક n, પૂર્ણાંક j)
{
    પૂર્ણાંક i;
    network *net = nets[0];
    layer base = net->layers[j];
    scale_weights(base, 0);
    માટે (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        pull_weights(l);
        merge_weights(l, base);
    }
    scale_weights(base, 1./n);
    માટે (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        distribute_weights(l, base);
    }
}

પ્રકારવ્યાખ્યા સંરચના{
    network **nets;
    પૂર્ણાંક n;
    પૂર્ણાંક j;
} sync_args;

શૂન્ય *sync_layer_thread(શૂન્ય *ptr)
{
    sync_args args = *(sync_args*)ptr;
    sync_layer(args.nets, args.n, args.j);
    મુક્ત(ptr);
    પાછા 0;
}

pthread_t sync_layer_in_thread(network **nets, પૂર્ણાંક n, પૂર્ણાંક j)
{
    pthread_t thread;
    sync_args *ptr = (sync_args *)calloc(1, કદ(sync_args));
    ptr->nets = nets;
    ptr->n = n;
    ptr->j = j;
    જો(pthread_create(&thread, 0, sync_layer_thread, ptr)) error("Thread creation failed");
    પાછા thread;
}

શૂન્ય sync_nets(network **nets, પૂર્ણાંક n, પૂર્ણાંક interval)
{
    પૂર્ણાંક j;
    પૂર્ણાંક layers = nets[0]->n;
    pthread_t *threads = (pthread_t *) calloc(layers, કદ(pthread_t));

    *(nets[0]->seen) += interval * (n-1) * nets[0]->batch * nets[0]->subdivisions;
    માટે (j = 0; j < n; ++j){
        *(nets[j]->seen) = *(nets[0]->seen);
    }
    માટે (j = 0; j < layers; ++j) {
        threads[j] = sync_layer_in_thread(nets, n, j);
    }
    માટે (j = 0; j < layers; ++j) {
        pthread_join(threads[j], 0);
    }
    મુક્ત(threads);
}

દશાંશ train_networks(network **nets, પૂર્ણાંક n, data d, પૂર્ણાંક interval)
{
    પૂર્ણાંક i;
    પૂર્ણાંક batch = nets[0]->batch;
    પૂર્ણાંક subdivisions = nets[0]->subdivisions;
    પ્રતિજ્ઞા(batch * subdivisions * n == d.X.rows);
    pthread_t *threads = (pthread_t *) calloc(n, કદ(pthread_t));
    દશાંશ *errors = (દશાંશ *) calloc(n, કદ(દશાંશ));

    દશાંશ sum = 0;
    માટે(i = 0; i < n; ++i){
        data p = get_data_part(d, i, n);
        threads[i] = train_network_in_thread(nets[i], p, errors + i);
    }
    માટે(i = 0; i < n; ++i){
        pthread_join(threads[i], 0);
        //printf("%f\n", errors[i]);
        sum += errors[i];
    }
    //cudaDeviceSynchronize();
    જો (get_current_batch(nets[0]) % interval == 0) {
        printf("Syncing... ");
        fflush(stdout);
        sync_nets(nets, n, interval);
        printf("Done!\n");
    }
    //cudaDeviceSynchronize();
    મુક્ત(threads);
    મુક્ત(errors);
    પાછા (દશાંશ)sum/(n);
}

શૂન્ય pull_network_output(network *net)
{
    layer l = get_network_output_layer(net);
    cuda_pull_array(l.output_gpu, l.output, l.outputs*l.batch);
}

#endif

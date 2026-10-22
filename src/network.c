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
    परत args;
}

network *load_network(अक्षर *cfg, अक्षर *weights, पूर्णांक clear)
{
    network *net = parse_network_cfg(cfg);
    जर(weights && weights[0] != 0){
        load_weights(net, weights);
    }
    जर(clear) (*net->seen) = 0;
    परत net;
}

माप_प्रकार get_current_batch(network *net)
{
    माप_प्रकार batch_अंक = (*net->seen)/(net->batch*net->subdivisions);
    परत batch_अंक;
}

शून्य reset_network_state(network *net, पूर्णांक b)
{
    पूर्णांक i;
    साठी (i = 0; i < net->n; ++i) {
        #ifdef GPU
        layer l = net->layers[i];
        जर(l.state_gpu){
            fill_gpu(l.outputs, 0, l.state_gpu + l.outputs*b, 1);
        }
        जर(l.h_gpu){
            fill_gpu(l.outputs, 0, l.h_gpu + l.outputs*b, 1);
        }
        #endif
    }
}

शून्य reset_rnn(network *net)
{
    reset_network_state(net, 0);
}

दशांश get_current_rate(network *net)
{
    माप_प्रकार batch_अंक = get_current_batch(net);
    पूर्णांक i;
    दशांश rate;
    जर (batch_अंक < net->burn_in) परत net->learning_rate * घात((दशांश)batch_अंक / net->burn_in, net->power);
    निवड (net->policy) {
        स्थिती CONSTANT:
            परत net->learning_rate;
        स्थिती STEP:
            परत net->learning_rate * घात(net->scale, batch_अंक/net->step);
        स्थिती STEPS:
            rate = net->learning_rate;
            साठी(i = 0; i < net->अंक_steps; ++i){
                जर(net->steps[i] > batch_अंक) परत rate;
                rate *= net->scales[i];
            }
            परत rate;
        स्थिती EXP:
            परत net->learning_rate * घात(net->gamma, batch_अंक);
        स्थिती POLY:
            परत net->learning_rate * घात(1 - (दशांश)batch_अंक / net->max_batches, net->power);
        स्थिती RANDOM:
            परत net->learning_rate * घात(rand_uniform(0,1), net->power);
        स्थिती SIG:
            परत net->learning_rate * (1./(1.+exp(net->gamma*(batch_अंक - net->step))));
        पूर्वनिर्धारित:
            ख_लिखो(मानक_त्रुटि, "Policy is weird!\n");
            परत net->learning_rate;
    }
}

अक्षर *get_layer_string(LAYER_TYPE a)
{
    निवड(a){
        स्थिती CONVOLUTIONAL:
            परत "convolutional";
        स्थिती ACTIVE:
            परत "activation";
        स्थिती LOCAL:
            परत "local";
        स्थिती DECONVOLUTIONAL:
            परत "deconvolutional";
        स्थिती CONNECTED:
            परत "connected";
        स्थिती RNN:
            परत "rnn";
        स्थिती GRU:
            परत "gru";
        स्थिती LSTM:
	    परत "lstm";
        स्थिती CRNN:
            परत "crnn";
        स्थिती MAXPOOL:
            परत "maxpool";
        स्थिती REORG:
            परत "reorg";
        स्थिती AVGPOOL:
            परत "avgpool";
        स्थिती SOFTMAX:
            परत "softmax";
        स्थिती DETECTION:
            परत "detection";
        स्थिती REGION:
            परत "region";
        स्थिती YOLO:
            परत "yolo";
        स्थिती DROPOUT:
            परत "dropout";
        स्थिती CROP:
            परत "crop";
        स्थिती COST:
            परत "cost";
        स्थिती ROUTE:
            परत "route";
        स्थिती SHORTCUT:
            परत "shortcut";
        स्थिती NORMALIZATION:
            परत "normalization";
        स्थिती BATCHNORM:
            परत "batchnorm";
        पूर्वनिर्धारित:
            थांबा;
    }
    परत "none";
}

network *make_network(पूर्णांक n)
{
    network *net = सुस्मृति(1, आकार(network));
    net->n = n;
    net->layers = सुस्मृति(net->n, आकार(layer));
    net->seen = सुस्मृति(1, आकार(माप_प्रकार));
    net->t    = सुस्मृति(1, आकार(पूर्णांक));
    net->cost = सुस्मृति(1, आकार(दशांश));
    परत net;
}

शून्य forward_network(network *netp)
{
#ifdef GPU
    जर(netp->gpu_index >= 0){
        forward_network_gpu(netp);   
        परत;
    }
#endif
    network net = *netp;
    पूर्णांक i;
    साठी(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        जर(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.आदान = l.output;
        जर(l.truth) {
            net.truth = l.output;
        }
    }
    calc_network_cost(netp);
}

शून्य update_network(network *netp)
{
#ifdef GPU
    जर(netp->gpu_index >= 0){
        update_network_gpu(netp);   
        परत;
    }
#endif
    network net = *netp;
    पूर्णांक i;
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

    साठी(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        जर(l.update){
            l.update(l, a);
        }
    }
}

शून्य calc_network_cost(network *netp)
{
    network net = *netp;
    पूर्णांक i;
    दशांश sum = 0;
    पूर्णांक मोजणी = 0;
    साठी(i = 0; i < net.n; ++i){
        जर(net.layers[i].cost){
            sum += net.layers[i].cost[0];
            ++मोजणी;
        }
    }
    *net.cost = sum/मोजणी;
}

पूर्णांक get_predicted_class_network(network *net)
{
    परत max_index(net->output, net->outputs);
}

शून्य backward_network(network *netp)
{
#ifdef GPU
    जर(netp->gpu_index >= 0){
        backward_network_gpu(netp);   
        परत;
    }
#endif
    network net = *netp;
    पूर्णांक i;
    network orig = net;
    साठी(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        जर(l.stopbackward) थांबा;
        जर(i == 0){
            net = orig;
        }अन्यथा{
            layer prev = net.layers[i-1];
            net.आदान = prev.output;
            net.delta = prev.delta;
        }
        net.index = i;
        l.backward(l, net);
    }
}

दशांश train_network_datum(network *net)
{
    *net->seen += net->batch;
    net->train = 1;
    forward_network(net);
    backward_network(net);
    दशांश error = *net->cost;
    जर(((*net->seen)/net->batch)%net->subdivisions == 0) update_network(net);
    परत error;
}

दशांश train_network_sgd(network *net, data d, पूर्णांक n)
{
    पूर्णांक batch = net->batch;

    पूर्णांक i;
    दशांश sum = 0;
    साठी(i = 0; i < n; ++i){
        get_random_batch(d, batch, net->आदान, net->truth);
        दशांश err = train_network_datum(net);
        sum += err;
    }
    परत (दशांश)sum/(n*batch);
}

दशांश train_network(network *net, data d)
{
    प्रतिज्ञा(d.X.rows % net->batch == 0);
    पूर्णांक batch = net->batch;
    पूर्णांक n = d.X.rows / batch;

    पूर्णांक i;
    दशांश sum = 0;
    साठी(i = 0; i < n; ++i){
        get_next_batch(d, batch, i*batch, net->आदान, net->truth);
        दशांश err = train_network_datum(net);
        sum += err;
    }
    परत (दशांश)sum/(n*batch);
}

शून्य set_temp_network(network *net, दशांश t)
{
    पूर्णांक i;
    साठी(i = 0; i < net->n; ++i){
        net->layers[i].temperature = t;
    }
}


शून्य set_batch_network(network *net, पूर्णांक b)
{
    net->batch = b;
    पूर्णांक i;
    साठी(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#ifdef CUDNN
        जर(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_setup(net->layers + i);
        }
        जर(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, l->out_h, l->out_w);
            cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 
        }
#endif
    }
}

पूर्णांक resize_network(network *net, पूर्णांक w, पूर्णांक h)
{
#ifdef GPU
    cuda_set_device(net->gpu_index);
    cuda_free(net->workspace);
#endif
    पूर्णांक i;
    //if(w == net->w && h == net->h) return 0;
    net->w = w;
    net->h = h;
    पूर्णांक inputs = 0;
    माप_प्रकार workspace_size = 0;
    //fprintf(stderr, "Resizing to %d x %d...\n", w, h);
    //fflush(stderr);
    साठी (i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        जर(l.type == CONVOLUTIONAL){
            resize_convolutional_layer(&l, w, h);
        }अन्यथा जर(l.type == CROP){
            resize_crop_layer(&l, w, h);
        }अन्यथा जर(l.type == MAXPOOL){
            resize_maxpool_layer(&l, w, h);
        }अन्यथा जर(l.type == REGION){
            resize_region_layer(&l, w, h);
        }अन्यथा जर(l.type == YOLO){
            resize_yolo_layer(&l, w, h);
        }अन्यथा जर(l.type == ROUTE){
            resize_route_layer(&l, net);
        }अन्यथा जर(l.type == SHORTCUT){
            resize_shortcut_layer(&l, w, h);
        }अन्यथा जर(l.type == UPSAMPLE){
            resize_upsample_layer(&l, w, h);
        }अन्यथा जर(l.type == REORG){
            resize_reorg_layer(&l, w, h);
        }अन्यथा जर(l.type == AVGPOOL){
            resize_avgpool_layer(&l, w, h);
        }अन्यथा जर(l.type == NORMALIZATION){
            resize_normalization_layer(&l, w, h);
        }अन्यथा जर(l.type == COST){
            resize_cost_layer(&l, inputs);
        }अन्यथा{
            error("Cannot resize this type of layer");
        }
        जर(l.workspace_size > workspace_size) workspace_size = l.workspace_size;
        जर(l.workspace_size > 2000000000) प्रतिज्ञा(0);
        inputs = l.outputs;
        net->layers[i] = l;
        w = l.out_w;
        h = l.out_h;
        जर(l.type == AVGPOOL) थांबा;
    }
    layer out = get_network_output_layer(net);
    net->inputs = net->layers[0].inputs;
    net->outputs = out.outputs;
    net->truths = out.outputs;
    जर(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    मुक्त(net->आदान);
    मुक्त(net->truth);
    net->आदान = सुस्मृति(net->inputs*net->batch, आकार(दशांश));
    net->truth = सुस्मृति(net->truths*net->batch, आकार(दशांश));
#ifdef GPU
    जर(gpu_index >= 0){
        cuda_free(net->आदान_gpu);
        cuda_free(net->truth_gpu);
        net->आदान_gpu = cuda_make_array(net->आदान, net->inputs*net->batch);
        net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
        जर(workspace_size){
            net->workspace = cuda_make_array(0, (workspace_size-1)/आकार(दशांश)+1);
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
    परत 0;
}

layer get_network_detection_layer(network *net)
{
    पूर्णांक i;
    साठी(i = 0; i < net->n; ++i){
        जर(net->layers[i].type == DETECTION){
            परत net->layers[i];
        }
    }
    ख_लिखो(मानक_त्रुटि, "Detection layer not found!!\n");
    layer l = {0};
    परत l;
}

image get_network_image_layer(network *net, पूर्णांक i)
{
    layer l = net->layers[i];
#ifdef GPU
    //cuda_pull_array(l.output_gpu, l.output, l.outputs);
#endif
    जर (l.out_w && l.out_h && l.out_c){
        परत float_to_image(l.out_w, l.out_h, l.out_c, l.output);
    }
    image def = {0};
    परत def;
}

image get_network_image(network *net)
{
    पूर्णांक i;
    साठी(i = net->n-1; i >= 0; --i){
        image m = get_network_image_layer(net, i);
        जर(m.h != 0) परत m;
    }
    image def = {0};
    परत def;
}

शून्य visualize_network(network *net)
{
    image *prev = 0;
    पूर्णांक i;
    अक्षर buff[256];
    साठी(i = 0; i < net->n; ++i){
        प्र_लिखो(buff, "Layer %d", i);
        layer l = net->layers[i];
        जर(l.type == CONVOLUTIONAL){
            prev = visualize_convolutional_layer(l, buff, prev);
        }
    } 
}

शून्य शीर्ष_predictions(network *net, पूर्णांक k, पूर्णांक *index)
{
    शीर्ष_k(net->output, net->outputs, k, index);
}


दशांश *network_predict(network *net, दशांश *आदान)
{
    network orig = *net;
    net->आदान = आदान;
    net->truth = 0;
    net->train = 0;
    net->delta = 0;
    forward_network(net);
    दशांश *out = net->output;
    *net = orig;
    परत out;
}

पूर्णांक अंक_detections(network *net, दशांश thresh)
{
    पूर्णांक i;
    पूर्णांक s = 0;
    साठी(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        जर(l.type == YOLO){
            s += yolo_अंक_detections(l, thresh);
        }
        जर(l.type == DETECTION || l.type == REGION){
            s += l.w*l.h*l.n;
        }
    }
    परत s;
}

detection *make_network_boxes(network *net, दशांश thresh, पूर्णांक *अंक)
{
    layer l = net->layers[net->n - 1];
    पूर्णांक i;
    पूर्णांक nboxes = अंक_detections(net, thresh);
    जर(अंक) *अंक = nboxes;
    detection *dets = सुस्मृति(nboxes, आकार(detection));
    साठी(i = 0; i < nboxes; ++i){
        dets[i].prob = सुस्मृति(l.classes, आकार(दशांश));
        जर(l.coords > 4){
            dets[i].mask = सुस्मृति(l.coords-4, आकार(दशांश));
        }
    }
    परत dets;
}

शून्य fill_network_boxes(network *net, पूर्णांक w, पूर्णांक h, दशांश thresh, दशांश hier, पूर्णांक *map, पूर्णांक relative, detection *dets)
{
    पूर्णांक j;
    साठी(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        जर(l.type == YOLO){
            पूर्णांक मोजणी = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
            dets += मोजणी;
        }
        जर(l.type == REGION){
            get_region_detections(l, w, h, net->w, net->h, thresh, map, hier, relative, dets);
            dets += l.w*l.h*l.n;
        }
        जर(l.type == DETECTION){
            get_detection_detections(l, w, h, thresh, dets);
            dets += l.w*l.h*l.n;
        }
    }
}

detection *get_network_boxes(network *net, पूर्णांक w, पूर्णांक h, दशांश thresh, दशांश hier, पूर्णांक *map, पूर्णांक relative, पूर्णांक *अंक)
{
    detection *dets = make_network_boxes(net, thresh, अंक);
    fill_network_boxes(net, w, h, thresh, hier, map, relative, dets);
    परत dets;
}

शून्य free_detections(detection *dets, पूर्णांक n)
{
    पूर्णांक i;
    साठी(i = 0; i < n; ++i){
        मुक्त(dets[i].prob);
        जर(dets[i].mask) मुक्त(dets[i].mask);
    }
    मुक्त(dets);
}

दशांश *network_predict_image(network *net, image im)
{
    image imr = letterbox_image(im, net->w, net->h);
    set_batch_network(net, 1);
    दशांश *p = network_predict(net, imr.data);
    free_image(imr);
    परत p;
}

पूर्णांक network_width(network *net){परत net->w;}
पूर्णांक network_height(network *net){परत net->h;}

matrix network_predict_data_multi(network *net, data test, पूर्णांक n)
{
    पूर्णांक i,j,b,m;
    पूर्णांक k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    दशांश *X = सुस्मृति(net->batch*test.X.rows, आकार(दशांश));
    साठी(i = 0; i < test.X.rows; i += net->batch){
        साठी(b = 0; b < net->batch; ++b){
            जर(i+b == test.X.rows) थांबा;
            स_नकल(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*आकार(दशांश));
        }
        साठी(m = 0; m < n; ++m){
            दशांश *out = network_predict(net, X);
            साठी(b = 0; b < net->batch; ++b){
                जर(i+b == test.X.rows) थांबा;
                साठी(j = 0; j < k; ++j){
                    pred.vals[i+b][j] += out[j+b*k]/n;
                }
            }
        }
    }
    मुक्त(X);
    परत pred;   
}

matrix network_predict_data(network *net, data test)
{
    पूर्णांक i,j,b;
    पूर्णांक k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    दशांश *X = सुस्मृति(net->batch*test.X.cols, आकार(दशांश));
    साठी(i = 0; i < test.X.rows; i += net->batch){
        साठी(b = 0; b < net->batch; ++b){
            जर(i+b == test.X.rows) थांबा;
            स_नकल(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*आकार(दशांश));
        }
        दशांश *out = network_predict(net, X);
        साठी(b = 0; b < net->batch; ++b){
            जर(i+b == test.X.rows) थांबा;
            साठी(j = 0; j < k; ++j){
                pred.vals[i+b][j] = out[j+b*k];
            }
        }
    }
    मुक्त(X);
    परत pred;   
}

शून्य print_network(network *net)
{
    पूर्णांक i,j;
    साठी(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        दशांश *output = l.output;
        पूर्णांक n = l.outputs;
        दशांश mean = mean_array(output, n);
        दशांश vari = variance_array(output, n);
        ख_लिखो(मानक_त्रुटि, "Layer %d - Mean: %f, Variance: %f\n",i,mean, vari);
        जर(n > 100) n = 100;
        साठी(j = 0; j < n; ++j) ख_लिखो(मानक_त्रुटि, "%f, ", output[j]);
        जर(n == 100)ख_लिखो(मानक_त्रुटि,".....\n");
        ख_लिखो(मानक_त्रुटि, "\n");
    }
}

शून्य compare_networks(network *n1, network *n2, data test)
{
    matrix g1 = network_predict_data(n1, test);
    matrix g2 = network_predict_data(n2, test);
    पूर्णांक i;
    पूर्णांक a,b,c,d;
    a = b = c = d = 0;
    साठी(i = 0; i < g1.rows; ++i){
        पूर्णांक truth = max_index(test.y.vals[i], test.y.cols);
        पूर्णांक p1 = max_index(g1.vals[i], g1.cols);
        पूर्णांक p2 = max_index(g2.vals[i], g2.cols);
        जर(p1 == truth){
            जर(p2 == truth) ++d;
            अन्यथा ++c;
        }अन्यथा{
            जर(p2 == truth) ++b;
            अन्यथा ++a;
        }
    }
    म_लिखो("%5d %5d\n%5d %5d\n", a, b, c, d);
    दशांश अंक = घात((असल(b - c) - 1.), 2.);
    दशांश den = b + c;
    म_लिखो("%f\n", अंक/den); 
}

दशांश network_accuracy(network *net, data d)
{
    matrix guess = network_predict_data(net, d);
    दशांश acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    परत acc;
}

दशांश *network_accuracies(network *net, data d, पूर्णांक n)
{
    स्थिर दशांश acc[2];
    matrix guess = network_predict_data(net, d);
    acc[0] = matrix_topk_accuracy(d.y, guess, 1);
    acc[1] = matrix_topk_accuracy(d.y, guess, n);
    free_matrix(guess);
    परत acc;
}

layer get_network_output_layer(network *net)
{
    पूर्णांक i;
    साठी(i = net->n - 1; i >= 0; --i){
        जर(net->layers[i].type != COST) थांबा;
    }
    परत net->layers[i];
}

दशांश network_accuracy_multi(network *net, data d, पूर्णांक n)
{
    matrix guess = network_predict_data_multi(net, d, n);
    दशांश acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    परत acc;
}

शून्य free_network(network *net)
{
    पूर्णांक i;
    साठी(i = 0; i < net->n; ++i){
        free_layer(net->layers[i]);
    }
    मुक्त(net->layers);
    जर(net->आदान) मुक्त(net->आदान);
    जर(net->truth) मुक्त(net->truth);
#ifdef GPU
    जर(net->आदान_gpu) cuda_free(net->आदान_gpu);
    जर(net->truth_gpu) cuda_free(net->truth_gpu);
#endif
    मुक्त(net);
}

// Some day...
// ^ What the hell is this comment for?


layer network_output_layer(network *net)
{
    पूर्णांक i;
    साठी(i = net->n - 1; i >= 0; --i){
        जर(net->layers[i].type != COST) थांबा;
    }
    परत net->layers[i];
}

पूर्णांक network_inputs(network *net)
{
    परत net->layers[0].inputs;
}

पूर्णांक network_outputs(network *net)
{
    परत network_output_layer(net).outputs;
}

दशांश *network_output(network *net)
{
    परत network_output_layer(net).output;
}

#ifdef GPU

शून्य forward_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    cuda_ढकला_array(net.आदान_gpu, net.आदान, net.inputs*net.batch);
    जर(net.truth){
        cuda_ढकला_array(net.truth_gpu, net.truth, net.truths*net.batch);
    }

    पूर्णांक i;
    साठी(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        जर(l.delta_gpu){
            fill_gpu(l.outputs * l.batch, 0, l.delta_gpu, 1);
        }
        l.forward_gpu(l, net);
        net.आदान_gpu = l.output_gpu;
        net.आदान = l.output;
        जर(l.truth) {
            net.truth_gpu = l.output_gpu;
            net.truth = l.output;
        }
    }
    pull_network_output(netp);
    calc_network_cost(netp);
}

शून्य backward_network_gpu(network *netp)
{
    पूर्णांक i;
    network net = *netp;
    network orig = net;
    cuda_set_device(net.gpu_index);
    साठी(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        जर(l.stopbackward) थांबा;
        जर(i == 0){
            net = orig;
        }अन्यथा{
            layer prev = net.layers[i-1];
            net.आदान = prev.output;
            net.delta = prev.delta;
            net.आदान_gpu = prev.output_gpu;
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
    पूर्णांक i;
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

    साठी(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        जर(l.update_gpu){
            l.update_gpu(l, a);
        }
    }
}

शून्य harmless_update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_index);
    पूर्णांक i;
    साठी(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        जर(l.weight_updates_gpu) fill_gpu(l.nweights, 0, l.weight_updates_gpu, 1);
        जर(l.bias_updates_gpu) fill_gpu(l.nbiases, 0, l.bias_updates_gpu, 1);
        जर(l.scale_updates_gpu) fill_gpu(l.nbiases, 0, l.scale_updates_gpu, 1);
    }
}

प्रकारव्याख्या संरचना {
    network *net;
    data d;
    दशांश *err;
} train_args;

शून्य *train_thread(शून्य *ptr)
{
    train_args args = *(train_args*)ptr;
    मुक्त(ptr);
    cuda_set_device(args.net->gpu_index);
    *args.err = train_network(args.net, args.d);
    परत 0;
}

pthread_t train_network_in_thread(network *net, data d, दशांश *err)
{
    pthread_t thread;
    train_args *ptr = (train_args *)सुस्मृति(1, आकार(train_args));
    ptr->net = net;
    ptr->d = d;
    ptr->err = err;
    जर(pthread_create(&thread, 0, train_thread, ptr)) error("Thread creation failed");
    परत thread;
}

शून्य merge_weights(layer l, layer base)
{
    जर (l.type == CONVOLUTIONAL) {
        axpy_cpu(l.n, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.nweights, 1, l.weight_updates, 1, base.weights, 1);
        जर (l.scales) {
            axpy_cpu(l.n, 1, l.scale_updates, 1, base.scales, 1);
        }
    } अन्यथा जर(l.type == CONNECTED) {
        axpy_cpu(l.outputs, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.outputs*l.inputs, 1, l.weight_updates, 1, base.weights, 1);
    }
}

शून्य scale_weights(layer l, दशांश s)
{
    जर (l.type == CONVOLUTIONAL) {
        scal_cpu(l.n, s, l.biases, 1);
        scal_cpu(l.nweights, s, l.weights, 1);
        जर (l.scales) {
            scal_cpu(l.n, s, l.scales, 1);
        }
    } अन्यथा जर(l.type == CONNECTED) {
        scal_cpu(l.outputs, s, l.biases, 1);
        scal_cpu(l.outputs*l.inputs, s, l.weights, 1);
    }
}


शून्य pull_weights(layer l)
{
    जर(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.n);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.nweights);
        जर(l.scales) cuda_pull_array(l.scales_gpu, l.scale_updates, l.n);
    } अन्यथा जर(l.type == CONNECTED){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.outputs);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.outputs*l.inputs);
    }
}

शून्य ढकला_weights(layer l)
{
    जर(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_ढकला_array(l.biases_gpu, l.biases, l.n);
        cuda_ढकला_array(l.weights_gpu, l.weights, l.nweights);
        जर(l.scales) cuda_ढकला_array(l.scales_gpu, l.scales, l.n);
    } अन्यथा जर(l.type == CONNECTED){
        cuda_ढकला_array(l.biases_gpu, l.biases, l.outputs);
        cuda_ढकला_array(l.weights_gpu, l.weights, l.outputs*l.inputs);
    }
}

शून्य distribute_weights(layer l, layer base)
{
    जर (l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL) {
        cuda_ढकला_array(l.biases_gpu, base.biases, l.n);
        cuda_ढकला_array(l.weights_gpu, base.weights, l.nweights);
        जर (base.scales) cuda_ढकला_array(l.scales_gpu, base.scales, l.n);
    } अन्यथा जर (l.type == CONNECTED) {
        cuda_ढकला_array(l.biases_gpu, base.biases, l.outputs);
        cuda_ढकला_array(l.weights_gpu, base.weights, l.outputs*l.inputs);
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

शून्य sync_layer(network **nets, पूर्णांक n, पूर्णांक j)
{
    पूर्णांक i;
    network *net = nets[0];
    layer base = net->layers[j];
    scale_weights(base, 0);
    साठी (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        pull_weights(l);
        merge_weights(l, base);
    }
    scale_weights(base, 1./n);
    साठी (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_index);
        layer l = nets[i]->layers[j];
        distribute_weights(l, base);
    }
}

प्रकारव्याख्या संरचना{
    network **nets;
    पूर्णांक n;
    पूर्णांक j;
} sync_args;

शून्य *sync_layer_thread(शून्य *ptr)
{
    sync_args args = *(sync_args*)ptr;
    sync_layer(args.nets, args.n, args.j);
    मुक्त(ptr);
    परत 0;
}

pthread_t sync_layer_in_thread(network **nets, पूर्णांक n, पूर्णांक j)
{
    pthread_t thread;
    sync_args *ptr = (sync_args *)सुस्मृति(1, आकार(sync_args));
    ptr->nets = nets;
    ptr->n = n;
    ptr->j = j;
    जर(pthread_create(&thread, 0, sync_layer_thread, ptr)) error("Thread creation failed");
    परत thread;
}

शून्य sync_nets(network **nets, पूर्णांक n, पूर्णांक interval)
{
    पूर्णांक j;
    पूर्णांक layers = nets[0]->n;
    pthread_t *threads = (pthread_t *) सुस्मृति(layers, आकार(pthread_t));

    *(nets[0]->seen) += interval * (n-1) * nets[0]->batch * nets[0]->subdivisions;
    साठी (j = 0; j < n; ++j){
        *(nets[j]->seen) = *(nets[0]->seen);
    }
    साठी (j = 0; j < layers; ++j) {
        threads[j] = sync_layer_in_thread(nets, n, j);
    }
    साठी (j = 0; j < layers; ++j) {
        pthread_join(threads[j], 0);
    }
    मुक्त(threads);
}

दशांश train_networks(network **nets, पूर्णांक n, data d, पूर्णांक interval)
{
    पूर्णांक i;
    पूर्णांक batch = nets[0]->batch;
    पूर्णांक subdivisions = nets[0]->subdivisions;
    प्रतिज्ञा(batch * subdivisions * n == d.X.rows);
    pthread_t *threads = (pthread_t *) सुस्मृति(n, आकार(pthread_t));
    दशांश *errors = (दशांश *) सुस्मृति(n, आकार(दशांश));

    दशांश sum = 0;
    साठी(i = 0; i < n; ++i){
        data p = get_data_part(d, i, n);
        threads[i] = train_network_in_thread(nets[i], p, errors + i);
    }
    साठी(i = 0; i < n; ++i){
        pthread_join(threads[i], 0);
        //printf("%f\n", errors[i]);
        sum += errors[i];
    }
    //cudaDeviceSynchronize();
    जर (get_current_batch(nets[0]) % interval == 0) {
        म_लिखो("Syncing... ");
        ख_साफ(मानक_निकास);
        sync_nets(nets, n, interval);
        म_लिखो("Done!\n");
    }
    //cudaDeviceSynchronize();
    मुक्त(threads);
    मुक्त(errors);
    परत (दशांश)sum/(n);
}

शून्य pull_network_output(network *net)
{
    layer l = get_network_output_layer(net);
    cuda_pull_array(l.output_gpu, l.output, l.outputs*l.batch);
}

#endif

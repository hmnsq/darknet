#समावेश <stdio.h>
#समावेश <time.h>
#समावेश <assert.h>
#समावेश "network.h"
#समावेश "image.h"
#समावेश "data.h"
#समावेश "utils.h"
#समावेश "blas.h"

#समावेश "crop_layer.h"
#समावेश "connected_layer.h"
#समावेश "gru_layer.h"
#समावेश "rnn_layer.h"
#समावेश "crnn_layer.h"
#समावेश "local_layer.h"
#समावेश "convolutional_layer.h"
#समावेश "activation_layer.h"
#समावेश "detection_layer.h"
#समावेश "region_layer.h"
#समावेश "yolo_layer.h"
#समावेश "normalization_layer.h"
#समावेश "batchnorm_layer.h"
#समावेश "maxpool_layer.h"
#समावेश "reorg_layer.h"
#समावेश "avgpool_layer.h"
#समावेश "cost_layer.h"
#समावेश "softmax_layer.h"
#समावेश "dropout_layer.h"
#समावेश "route_layer.h"
#समावेश "upsample_layer.h"
#समावेश "shortcut_layer.h"
#समावेश "parser.h"
#समावेश "data.h"

load_args get_base_args(network *net)
{
    load_args args = {0};
    args.w = net->w;
    args.h = net->h;
    args.आकार = net->w;

    args.min = net->min_crop;
    args.max = net->max_crop;
    args.angle = net->angle;
    args.aspect = net->aspect;
    args.exposure = net->exposure;
    args.center = net->center;
    args.saturation = net->saturation;
    args.hue = net->hue;
    वापस args;
}

network *load_network(अक्षर *cfg, अक्षर *weights, पूर्णांक clear)
{
    network *net = parse_network_cfg(cfg);
    अगर(weights && weights[0] != 0){
        load_weights(net, weights);
    }
    अगर(clear) (*net->seen) = 0;
    वापस net;
}

माप_प्रकार get_current_batch(network *net)
{
    माप_प्रकार batch_अंक = (*net->seen)/(net->batch*net->subdivisions);
    वापस batch_अंक;
}

व्योम reset_network_state(network *net, पूर्णांक b)
{
    पूर्णांक i;
    क्रम (i = 0; i < net->n; ++i) {
        #अगर_घोषित GPU
        layer l = net->layers[i];
        अगर(l.state_gpu){
            fill_gpu(l.outputs, 0, l.state_gpu + l.outputs*b, 1);
        }
        अगर(l.h_gpu){
            fill_gpu(l.outputs, 0, l.h_gpu + l.outputs*b, 1);
        }
        #पूर्ण_अगर
    }
}

व्योम reset_rnn(network *net)
{
    reset_network_state(net, 0);
}

भग्न get_current_rate(network *net)
{
    माप_प्रकार batch_अंक = get_current_batch(net);
    पूर्णांक i;
    भग्न rate;
    अगर (batch_अंक < net->burn_in) वापस net->learning_rate * घात((भग्न)batch_अंक / net->burn_in, net->power);
    चयन (net->policy) {
        हाल CONSTANT:
            वापस net->learning_rate;
        हाल STEP:
            वापस net->learning_rate * घात(net->scale, batch_अंक/net->step);
        हाल STEPS:
            rate = net->learning_rate;
            क्रम(i = 0; i < net->अंक_steps; ++i){
                अगर(net->steps[i] > batch_अंक) वापस rate;
                rate *= net->scales[i];
            }
            वापस rate;
        हाल EXP:
            वापस net->learning_rate * घात(net->gamma, batch_अंक);
        हाल POLY:
            वापस net->learning_rate * घात(1 - (भग्न)batch_अंक / net->max_batches, net->power);
        हाल RANDOM:
            वापस net->learning_rate * घात(rand_uniform(0,1), net->power);
        हाल SIG:
            वापस net->learning_rate * (1./(1.+exp(net->gamma*(batch_अंक - net->step))));
        शेष:
            ख_लिखो(मानक_त्रुटि, "Policy is weird!\n");
            वापस net->learning_rate;
    }
}

अक्षर *get_layer_string(LAYER_TYPE a)
{
    चयन(a){
        हाल CONVOLUTIONAL:
            वापस "convolutional";
        हाल ACTIVE:
            वापस "activation";
        हाल LOCAL:
            वापस "local";
        हाल DECONVOLUTIONAL:
            वापस "deconvolutional";
        हाल CONNECTED:
            वापस "connected";
        हाल RNN:
            वापस "rnn";
        हाल GRU:
            वापस "gru";
        हाल LSTM:
	    वापस "lstm";
        हाल CRNN:
            वापस "crnn";
        हाल MAXPOOL:
            वापस "maxpool";
        हाल REORG:
            वापस "reorg";
        हाल AVGPOOL:
            वापस "avgpool";
        हाल SOFTMAX:
            वापस "softmax";
        हाल DETECTION:
            वापस "detection";
        हाल REGION:
            वापस "region";
        हाल YOLO:
            वापस "yolo";
        हाल DROPOUT:
            वापस "dropout";
        हाल CROP:
            वापस "crop";
        हाल COST:
            वापस "cost";
        हाल ROUTE:
            वापस "route";
        हाल SHORTCUT:
            वापस "shortcut";
        हाल NORMALIZATION:
            वापस "normalization";
        हाल BATCHNORM:
            वापस "batchnorm";
        शेष:
            अवरोध;
    }
    वापस "none";
}

network *make_network(पूर्णांक n)
{
    network *net = सुस्मृति(1, माप(network));
    net->n = n;
    net->layers = सुस्मृति(net->n, माप(layer));
    net->seen = सुस्मृति(1, माप(माप_प्रकार));
    net->t    = सुस्मृति(1, माप(पूर्णांक));
    net->cost = सुस्मृति(1, माप(भग्न));
    वापस net;
}

व्योम forward_network(network *netp)
{
#अगर_घोषित GPU
    अगर(netp->gpu_सूचकांक >= 0){
        forward_network_gpu(netp);   
        वापस;
    }
#पूर्ण_अगर
    network net = *netp;
    पूर्णांक i;
    क्रम(i = 0; i < net.n; ++i){
        net.सूचकांक = i;
        layer l = net.layers[i];
        अगर(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.निवेश = l.output;
        अगर(l.truth) {
            net.truth = l.output;
        }
    }
    calc_network_cost(netp);
}

व्योम update_network(network *netp)
{
#अगर_घोषित GPU
    अगर(netp->gpu_सूचकांक >= 0){
        update_network_gpu(netp);   
        वापस;
    }
#पूर्ण_अगर
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

    क्रम(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        अगर(l.update){
            l.update(l, a);
        }
    }
}

व्योम calc_network_cost(network *netp)
{
    network net = *netp;
    पूर्णांक i;
    भग्न योग = 0;
    पूर्णांक गिनती = 0;
    क्रम(i = 0; i < net.n; ++i){
        अगर(net.layers[i].cost){
            योग += net.layers[i].cost[0];
            ++गिनती;
        }
    }
    *net.cost = योग/गिनती;
}

पूर्णांक get_predicted_class_network(network *net)
{
    वापस max_सूचकांक(net->output, net->outputs);
}

व्योम backward_network(network *netp)
{
#अगर_घोषित GPU
    अगर(netp->gpu_सूचकांक >= 0){
        backward_network_gpu(netp);   
        वापस;
    }
#पूर्ण_अगर
    network net = *netp;
    पूर्णांक i;
    network orig = net;
    क्रम(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        अगर(l.stopbackward) अवरोध;
        अगर(i == 0){
            net = orig;
        }अन्यथा{
            layer पूर्व = net.layers[i-1];
            net.निवेश = पूर्व.output;
            net.delta = पूर्व.delta;
        }
        net.सूचकांक = i;
        l.backward(l, net);
    }
}

भग्न train_network_datum(network *net)
{
    *net->seen += net->batch;
    net->train = 1;
    forward_network(net);
    backward_network(net);
    भग्न error = *net->cost;
    अगर(((*net->seen)/net->batch)%net->subdivisions == 0) update_network(net);
    वापस error;
}

भग्न train_network_sgd(network *net, तथ्य d, पूर्णांक n)
{
    पूर्णांक batch = net->batch;

    पूर्णांक i;
    भग्न योग = 0;
    क्रम(i = 0; i < n; ++i){
        get_random_batch(d, batch, net->निवेश, net->truth);
        भग्न err = train_network_datum(net);
        योग += err;
    }
    वापस (भग्न)योग/(n*batch);
}

भग्न train_network(network *net, तथ्य d)
{
    निश्चित(d.X.rows % net->batch == 0);
    पूर्णांक batch = net->batch;
    पूर्णांक n = d.X.rows / batch;

    पूर्णांक i;
    भग्न योग = 0;
    क्रम(i = 0; i < n; ++i){
        get_अग्र_batch(d, batch, i*batch, net->निवेश, net->truth);
        भग्न err = train_network_datum(net);
        योग += err;
    }
    वापस (भग्न)योग/(n*batch);
}

व्योम set_अस्थायी_network(network *net, भग्न t)
{
    पूर्णांक i;
    क्रम(i = 0; i < net->n; ++i){
        net->layers[i].temperature = t;
    }
}


व्योम set_batch_network(network *net, पूर्णांक b)
{
    net->batch = b;
    पूर्णांक i;
    क्रम(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#अगर_घोषित CUDNN
        अगर(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_setup(net->layers + i);
        }
        अगर(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_तथ्य_FLOAT, 1, l->out_c, l->out_h, l->out_w);
            cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_तथ्य_FLOAT, 1, l->out_c, 1, 1); 
        }
#पूर्ण_अगर
    }
}

पूर्णांक resize_network(network *net, पूर्णांक w, पूर्णांक h)
{
#अगर_घोषित GPU
    cuda_set_device(net->gpu_सूचकांक);
    cuda_free(net->workspace);
#पूर्ण_अगर
    पूर्णांक i;
    //if(w == net->w && h == net->h) return 0;
    net->w = w;
    net->h = h;
    पूर्णांक inputs = 0;
    माप_प्रकार workspace_आकार = 0;
    //fprintf(stderr, "Resizing to %d x %d...\n", w, h);
    //fflush(stderr);
    क्रम (i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        अगर(l.type == CONVOLUTIONAL){
            resize_convolutional_layer(&l, w, h);
        }अन्यथा अगर(l.type == CROP){
            resize_crop_layer(&l, w, h);
        }अन्यथा अगर(l.type == MAXPOOL){
            resize_maxpool_layer(&l, w, h);
        }अन्यथा अगर(l.type == REGION){
            resize_region_layer(&l, w, h);
        }अन्यथा अगर(l.type == YOLO){
            resize_yolo_layer(&l, w, h);
        }अन्यथा अगर(l.type == ROUTE){
            resize_route_layer(&l, net);
        }अन्यथा अगर(l.type == SHORTCUT){
            resize_shortcut_layer(&l, w, h);
        }अन्यथा अगर(l.type == UPSAMPLE){
            resize_upsample_layer(&l, w, h);
        }अन्यथा अगर(l.type == REORG){
            resize_reorg_layer(&l, w, h);
        }अन्यथा अगर(l.type == AVGPOOL){
            resize_avgpool_layer(&l, w, h);
        }अन्यथा अगर(l.type == NORMALIZATION){
            resize_normalization_layer(&l, w, h);
        }अन्यथा अगर(l.type == COST){
            resize_cost_layer(&l, inputs);
        }अन्यथा{
            error("Cannot resize this type of layer");
        }
        अगर(l.workspace_आकार > workspace_आकार) workspace_आकार = l.workspace_आकार;
        अगर(l.workspace_आकार > 2000000000) निश्चित(0);
        inputs = l.outputs;
        net->layers[i] = l;
        w = l.out_w;
        h = l.out_h;
        अगर(l.type == AVGPOOL) अवरोध;
    }
    layer out = get_network_output_layer(net);
    net->inputs = net->layers[0].inputs;
    net->outputs = out.outputs;
    net->truths = out.outputs;
    अगर(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    मुक्त(net->निवेश);
    मुक्त(net->truth);
    net->निवेश = सुस्मृति(net->inputs*net->batch, माप(भग्न));
    net->truth = सुस्मृति(net->truths*net->batch, माप(भग्न));
#अगर_घोषित GPU
    अगर(gpu_सूचकांक >= 0){
        cuda_free(net->निवेश_gpu);
        cuda_free(net->truth_gpu);
        net->निवेश_gpu = cuda_make_array(net->निवेश, net->inputs*net->batch);
        net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
        अगर(workspace_आकार){
            net->workspace = cuda_make_array(0, (workspace_आकार-1)/माप(भग्न)+1);
        }
    }अन्यथा {
        मुक्त(net->workspace);
        net->workspace = सुस्मृति(1, workspace_आकार);
    }
#अन्यथा
    मुक्त(net->workspace);
    net->workspace = सुस्मृति(1, workspace_आकार);
#पूर्ण_अगर
    //fprintf(stderr, " Done!\n");
    वापस 0;
}

layer get_network_detection_layer(network *net)
{
    पूर्णांक i;
    क्रम(i = 0; i < net->n; ++i){
        अगर(net->layers[i].type == DETECTION){
            वापस net->layers[i];
        }
    }
    ख_लिखो(मानक_त्रुटि, "Detection layer not found!!\n");
    layer l = {0};
    वापस l;
}

image get_network_image_layer(network *net, पूर्णांक i)
{
    layer l = net->layers[i];
#अगर_घोषित GPU
    //cuda_pull_array(l.output_gpu, l.output, l.outputs);
#पूर्ण_अगर
    अगर (l.out_w && l.out_h && l.out_c){
        वापस float_to_image(l.out_w, l.out_h, l.out_c, l.output);
    }
    image def = {0};
    वापस def;
}

image get_network_image(network *net)
{
    पूर्णांक i;
    क्रम(i = net->n-1; i >= 0; --i){
        image m = get_network_image_layer(net, i);
        अगर(m.h != 0) वापस m;
    }
    image def = {0};
    वापस def;
}

व्योम visualize_network(network *net)
{
    image *पूर्व = 0;
    पूर्णांक i;
    अक्षर buff[256];
    क्रम(i = 0; i < net->n; ++i){
        प्र_लिखो(buff, "Layer %d", i);
        layer l = net->layers[i];
        अगर(l.type == CONVOLUTIONAL){
            पूर्व = visualize_convolutional_layer(l, buff, पूर्व);
        }
    } 
}

व्योम शीर्ष_predictions(network *net, पूर्णांक k, पूर्णांक *सूचकांक)
{
    शीर्ष_k(net->output, net->outputs, k, सूचकांक);
}


भग्न *network_predict(network *net, भग्न *निवेश)
{
    network orig = *net;
    net->निवेश = निवेश;
    net->truth = 0;
    net->train = 0;
    net->delta = 0;
    forward_network(net);
    भग्न *out = net->output;
    *net = orig;
    वापस out;
}

पूर्णांक अंक_detections(network *net, भग्न thresh)
{
    पूर्णांक i;
    पूर्णांक s = 0;
    क्रम(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        अगर(l.type == YOLO){
            s += yolo_अंक_detections(l, thresh);
        }
        अगर(l.type == DETECTION || l.type == REGION){
            s += l.w*l.h*l.n;
        }
    }
    वापस s;
}

detection *make_network_boxes(network *net, भग्न thresh, पूर्णांक *अंक)
{
    layer l = net->layers[net->n - 1];
    पूर्णांक i;
    पूर्णांक nboxes = अंक_detections(net, thresh);
    अगर(अंक) *अंक = nboxes;
    detection *dets = सुस्मृति(nboxes, माप(detection));
    क्रम(i = 0; i < nboxes; ++i){
        dets[i].prob = सुस्मृति(l.classes, माप(भग्न));
        अगर(l.coords > 4){
            dets[i].mask = सुस्मृति(l.coords-4, माप(भग्न));
        }
    }
    वापस dets;
}

व्योम fill_network_boxes(network *net, पूर्णांक w, पूर्णांक h, भग्न thresh, भग्न hier, पूर्णांक *map, पूर्णांक relative, detection *dets)
{
    पूर्णांक j;
    क्रम(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        अगर(l.type == YOLO){
            पूर्णांक गिनती = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
            dets += गिनती;
        }
        अगर(l.type == REGION){
            get_region_detections(l, w, h, net->w, net->h, thresh, map, hier, relative, dets);
            dets += l.w*l.h*l.n;
        }
        अगर(l.type == DETECTION){
            get_detection_detections(l, w, h, thresh, dets);
            dets += l.w*l.h*l.n;
        }
    }
}

detection *get_network_boxes(network *net, पूर्णांक w, पूर्णांक h, भग्न thresh, भग्न hier, पूर्णांक *map, पूर्णांक relative, पूर्णांक *अंक)
{
    detection *dets = make_network_boxes(net, thresh, अंक);
    fill_network_boxes(net, w, h, thresh, hier, map, relative, dets);
    वापस dets;
}

व्योम free_detections(detection *dets, पूर्णांक n)
{
    पूर्णांक i;
    क्रम(i = 0; i < n; ++i){
        मुक्त(dets[i].prob);
        अगर(dets[i].mask) मुक्त(dets[i].mask);
    }
    मुक्त(dets);
}

भग्न *network_predict_image(network *net, image im)
{
    image imr = letterbox_image(im, net->w, net->h);
    set_batch_network(net, 1);
    भग्न *p = network_predict(net, imr.तथ्य);
    free_image(imr);
    वापस p;
}

पूर्णांक network_width(network *net){वापस net->w;}
पूर्णांक network_height(network *net){वापस net->h;}

matrix network_predict_तथ्य_multi(network *net, तथ्य test, पूर्णांक n)
{
    पूर्णांक i,j,b,m;
    पूर्णांक k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    भग्न *X = सुस्मृति(net->batch*test.X.rows, माप(भग्न));
    क्रम(i = 0; i < test.X.rows; i += net->batch){
        क्रम(b = 0; b < net->batch; ++b){
            अगर(i+b == test.X.rows) अवरोध;
            स_नकल(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*माप(भग्न));
        }
        क्रम(m = 0; m < n; ++m){
            भग्न *out = network_predict(net, X);
            क्रम(b = 0; b < net->batch; ++b){
                अगर(i+b == test.X.rows) अवरोध;
                क्रम(j = 0; j < k; ++j){
                    pred.vals[i+b][j] += out[j+b*k]/n;
                }
            }
        }
    }
    मुक्त(X);
    वापस pred;   
}

matrix network_predict_तथ्य(network *net, तथ्य test)
{
    पूर्णांक i,j,b;
    पूर्णांक k = net->outputs;
    matrix pred = make_matrix(test.X.rows, k);
    भग्न *X = सुस्मृति(net->batch*test.X.cols, माप(भग्न));
    क्रम(i = 0; i < test.X.rows; i += net->batch){
        क्रम(b = 0; b < net->batch; ++b){
            अगर(i+b == test.X.rows) अवरोध;
            स_नकल(X+b*test.X.cols, test.X.vals[i+b], test.X.cols*माप(भग्न));
        }
        भग्न *out = network_predict(net, X);
        क्रम(b = 0; b < net->batch; ++b){
            अगर(i+b == test.X.rows) अवरोध;
            क्रम(j = 0; j < k; ++j){
                pred.vals[i+b][j] = out[j+b*k];
            }
        }
    }
    मुक्त(X);
    वापस pred;   
}

व्योम print_network(network *net)
{
    पूर्णांक i,j;
    क्रम(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        भग्न *output = l.output;
        पूर्णांक n = l.outputs;
        भग्न mean = mean_array(output, n);
        भग्न vari = variance_array(output, n);
        ख_लिखो(मानक_त्रुटि, "Layer %d - Mean: %f, Variance: %f\n",i,mean, vari);
        अगर(n > 100) n = 100;
        क्रम(j = 0; j < n; ++j) ख_लिखो(मानक_त्रुटि, "%f, ", output[j]);
        अगर(n == 100)ख_लिखो(मानक_त्रुटि,".....\n");
        ख_लिखो(मानक_त्रुटि, "\n");
    }
}

व्योम compare_networks(network *n1, network *n2, तथ्य test)
{
    matrix g1 = network_predict_तथ्य(n1, test);
    matrix g2 = network_predict_तथ्य(n2, test);
    पूर्णांक i;
    पूर्णांक a,b,c,d;
    a = b = c = d = 0;
    क्रम(i = 0; i < g1.rows; ++i){
        पूर्णांक truth = max_सूचकांक(test.y.vals[i], test.y.cols);
        पूर्णांक p1 = max_सूचकांक(g1.vals[i], g1.cols);
        पूर्णांक p2 = max_सूचकांक(g2.vals[i], g2.cols);
        अगर(p1 == truth){
            अगर(p2 == truth) ++d;
            अन्यथा ++c;
        }अन्यथा{
            अगर(p2 == truth) ++b;
            अन्यथा ++a;
        }
    }
    म_लिखो("%5d %5d\n%5d %5d\n", a, b, c, d);
    भग्न अंक = घात((असल(b - c) - 1.), 2.);
    भग्न den = b + c;
    म_लिखो("%f\n", अंक/den); 
}

भग्न network_accuracy(network *net, तथ्य d)
{
    matrix guess = network_predict_तथ्य(net, d);
    भग्न acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    वापस acc;
}

भग्न *network_accuracies(network *net, तथ्य d, पूर्णांक n)
{
    अटल भग्न acc[2];
    matrix guess = network_predict_तथ्य(net, d);
    acc[0] = matrix_topk_accuracy(d.y, guess, 1);
    acc[1] = matrix_topk_accuracy(d.y, guess, n);
    free_matrix(guess);
    वापस acc;
}

layer get_network_output_layer(network *net)
{
    पूर्णांक i;
    क्रम(i = net->n - 1; i >= 0; --i){
        अगर(net->layers[i].type != COST) अवरोध;
    }
    वापस net->layers[i];
}

भग्न network_accuracy_multi(network *net, तथ्य d, पूर्णांक n)
{
    matrix guess = network_predict_तथ्य_multi(net, d, n);
    भग्न acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    वापस acc;
}

व्योम free_network(network *net)
{
    पूर्णांक i;
    क्रम(i = 0; i < net->n; ++i){
        free_layer(net->layers[i]);
    }
    मुक्त(net->layers);
    अगर(net->निवेश) मुक्त(net->निवेश);
    अगर(net->truth) मुक्त(net->truth);
#अगर_घोषित GPU
    अगर(net->निवेश_gpu) cuda_free(net->निवेश_gpu);
    अगर(net->truth_gpu) cuda_free(net->truth_gpu);
#पूर्ण_अगर
    मुक्त(net);
}

// Some day...
// ^ What the hell is this comment for?


layer network_output_layer(network *net)
{
    पूर्णांक i;
    क्रम(i = net->n - 1; i >= 0; --i){
        अगर(net->layers[i].type != COST) अवरोध;
    }
    वापस net->layers[i];
}

पूर्णांक network_inputs(network *net)
{
    वापस net->layers[0].inputs;
}

पूर्णांक network_outputs(network *net)
{
    वापस network_output_layer(net).outputs;
}

भग्न *network_output(network *net)
{
    वापस network_output_layer(net).output;
}

#अगर_घोषित GPU

व्योम forward_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_सूचकांक);
    cuda_धकेलो_array(net.निवेश_gpu, net.निवेश, net.inputs*net.batch);
    अगर(net.truth){
        cuda_धकेलो_array(net.truth_gpu, net.truth, net.truths*net.batch);
    }

    पूर्णांक i;
    क्रम(i = 0; i < net.n; ++i){
        net.सूचकांक = i;
        layer l = net.layers[i];
        अगर(l.delta_gpu){
            fill_gpu(l.outputs * l.batch, 0, l.delta_gpu, 1);
        }
        l.forward_gpu(l, net);
        net.निवेश_gpu = l.output_gpu;
        net.निवेश = l.output;
        अगर(l.truth) {
            net.truth_gpu = l.output_gpu;
            net.truth = l.output;
        }
    }
    pull_network_output(netp);
    calc_network_cost(netp);
}

व्योम backward_network_gpu(network *netp)
{
    पूर्णांक i;
    network net = *netp;
    network orig = net;
    cuda_set_device(net.gpu_सूचकांक);
    क्रम(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        अगर(l.stopbackward) अवरोध;
        अगर(i == 0){
            net = orig;
        }अन्यथा{
            layer पूर्व = net.layers[i-1];
            net.निवेश = पूर्व.output;
            net.delta = पूर्व.delta;
            net.निवेश_gpu = पूर्व.output_gpu;
            net.delta_gpu = पूर्व.delta_gpu;
        }
        net.सूचकांक = i;
        l.backward_gpu(l, net);
    }
}

व्योम update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_सूचकांक);
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

    क्रम(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        अगर(l.update_gpu){
            l.update_gpu(l, a);
        }
    }
}

व्योम harmless_update_network_gpu(network *netp)
{
    network net = *netp;
    cuda_set_device(net.gpu_सूचकांक);
    पूर्णांक i;
    क्रम(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        अगर(l.weight_updates_gpu) fill_gpu(l.nweights, 0, l.weight_updates_gpu, 1);
        अगर(l.bias_updates_gpu) fill_gpu(l.nbiases, 0, l.bias_updates_gpu, 1);
        अगर(l.scale_updates_gpu) fill_gpu(l.nbiases, 0, l.scale_updates_gpu, 1);
    }
}

प्रकार काष्ठा {
    network *net;
    तथ्य d;
    भग्न *err;
} train_args;

व्योम *train_thread(व्योम *ptr)
{
    train_args args = *(train_args*)ptr;
    मुक्त(ptr);
    cuda_set_device(args.net->gpu_सूचकांक);
    *args.err = train_network(args.net, args.d);
    वापस 0;
}

pthread_t train_network_in_thread(network *net, तथ्य d, भग्न *err)
{
    pthread_t thread;
    train_args *ptr = (train_args *)सुस्मृति(1, माप(train_args));
    ptr->net = net;
    ptr->d = d;
    ptr->err = err;
    अगर(pthread_create(&thread, 0, train_thread, ptr)) error("Thread creation failed");
    वापस thread;
}

व्योम merge_weights(layer l, layer base)
{
    अगर (l.type == CONVOLUTIONAL) {
        axpy_cpu(l.n, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.nweights, 1, l.weight_updates, 1, base.weights, 1);
        अगर (l.scales) {
            axpy_cpu(l.n, 1, l.scale_updates, 1, base.scales, 1);
        }
    } अन्यथा अगर(l.type == CONNECTED) {
        axpy_cpu(l.outputs, 1, l.bias_updates, 1, base.biases, 1);
        axpy_cpu(l.outputs*l.inputs, 1, l.weight_updates, 1, base.weights, 1);
    }
}

व्योम scale_weights(layer l, भग्न s)
{
    अगर (l.type == CONVOLUTIONAL) {
        scal_cpu(l.n, s, l.biases, 1);
        scal_cpu(l.nweights, s, l.weights, 1);
        अगर (l.scales) {
            scal_cpu(l.n, s, l.scales, 1);
        }
    } अन्यथा अगर(l.type == CONNECTED) {
        scal_cpu(l.outputs, s, l.biases, 1);
        scal_cpu(l.outputs*l.inputs, s, l.weights, 1);
    }
}


व्योम pull_weights(layer l)
{
    अगर(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.n);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.nweights);
        अगर(l.scales) cuda_pull_array(l.scales_gpu, l.scale_updates, l.n);
    } अन्यथा अगर(l.type == CONNECTED){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.outputs);
        cuda_pull_array(l.weights_gpu, l.weight_updates, l.outputs*l.inputs);
    }
}

व्योम धकेलो_weights(layer l)
{
    अगर(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_धकेलो_array(l.biases_gpu, l.biases, l.n);
        cuda_धकेलो_array(l.weights_gpu, l.weights, l.nweights);
        अगर(l.scales) cuda_धकेलो_array(l.scales_gpu, l.scales, l.n);
    } अन्यथा अगर(l.type == CONNECTED){
        cuda_धकेलो_array(l.biases_gpu, l.biases, l.outputs);
        cuda_धकेलो_array(l.weights_gpu, l.weights, l.outputs*l.inputs);
    }
}

व्योम distribute_weights(layer l, layer base)
{
    अगर (l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL) {
        cuda_धकेलो_array(l.biases_gpu, base.biases, l.n);
        cuda_धकेलो_array(l.weights_gpu, base.weights, l.nweights);
        अगर (base.scales) cuda_धकेलो_array(l.scales_gpu, base.scales, l.n);
    } अन्यथा अगर (l.type == CONNECTED) {
        cuda_धकेलो_array(l.biases_gpu, base.biases, l.outputs);
        cuda_धकेलो_array(l.weights_gpu, base.weights, l.outputs*l.inputs);
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

व्योम sync_layer(network **nets, पूर्णांक n, पूर्णांक j)
{
    पूर्णांक i;
    network *net = nets[0];
    layer base = net->layers[j];
    scale_weights(base, 0);
    क्रम (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_सूचकांक);
        layer l = nets[i]->layers[j];
        pull_weights(l);
        merge_weights(l, base);
    }
    scale_weights(base, 1./n);
    क्रम (i = 0; i < n; ++i) {
        cuda_set_device(nets[i]->gpu_सूचकांक);
        layer l = nets[i]->layers[j];
        distribute_weights(l, base);
    }
}

प्रकार काष्ठा{
    network **nets;
    पूर्णांक n;
    पूर्णांक j;
} sync_args;

व्योम *sync_layer_thread(व्योम *ptr)
{
    sync_args args = *(sync_args*)ptr;
    sync_layer(args.nets, args.n, args.j);
    मुक्त(ptr);
    वापस 0;
}

pthread_t sync_layer_in_thread(network **nets, पूर्णांक n, पूर्णांक j)
{
    pthread_t thread;
    sync_args *ptr = (sync_args *)सुस्मृति(1, माप(sync_args));
    ptr->nets = nets;
    ptr->n = n;
    ptr->j = j;
    अगर(pthread_create(&thread, 0, sync_layer_thread, ptr)) error("Thread creation failed");
    वापस thread;
}

व्योम sync_nets(network **nets, पूर्णांक n, पूर्णांक interval)
{
    पूर्णांक j;
    पूर्णांक layers = nets[0]->n;
    pthread_t *threads = (pthread_t *) सुस्मृति(layers, माप(pthread_t));

    *(nets[0]->seen) += interval * (n-1) * nets[0]->batch * nets[0]->subdivisions;
    क्रम (j = 0; j < n; ++j){
        *(nets[j]->seen) = *(nets[0]->seen);
    }
    क्रम (j = 0; j < layers; ++j) {
        threads[j] = sync_layer_in_thread(nets, n, j);
    }
    क्रम (j = 0; j < layers; ++j) {
        pthread_join(threads[j], 0);
    }
    मुक्त(threads);
}

भग्न train_networks(network **nets, पूर्णांक n, तथ्य d, पूर्णांक interval)
{
    पूर्णांक i;
    पूर्णांक batch = nets[0]->batch;
    पूर्णांक subdivisions = nets[0]->subdivisions;
    निश्चित(batch * subdivisions * n == d.X.rows);
    pthread_t *threads = (pthread_t *) सुस्मृति(n, माप(pthread_t));
    भग्न *errors = (भग्न *) सुस्मृति(n, माप(भग्न));

    भग्न योग = 0;
    क्रम(i = 0; i < n; ++i){
        तथ्य p = get_तथ्य_part(d, i, n);
        threads[i] = train_network_in_thread(nets[i], p, errors + i);
    }
    क्रम(i = 0; i < n; ++i){
        pthread_join(threads[i], 0);
        //printf("%f\n", errors[i]);
        योग += errors[i];
    }
    //cudaDeviceSynchronize();
    अगर (get_current_batch(nets[0]) % interval == 0) {
        म_लिखो("Syncing... ");
        ख_साफ(मानक_निकास);
        sync_nets(nets, n, interval);
        म_लिखो("Done!\n");
    }
    //cudaDeviceSynchronize();
    मुक्त(threads);
    मुक्त(errors);
    वापस (भग्न)योग/(n);
}

व्योम pull_network_output(network *net)
{
    layer l = get_network_output_layer(net);
    cuda_pull_array(l.output_gpu, l.output, l.outputs*l.batch);
}

#पूर्ण_अगर

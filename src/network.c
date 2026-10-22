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

आरोपय_तर्काः get_आधारः_तर्काः(जालम् *net)
{
    आरोपय_तर्काः तर्काः = {0};
    तर्काः.w = net->w;
    तर्काः.h = net->h;
    तर्काः.आकार = net->w;

    तर्काः.min = net->min_crop;
    तर्काः.max = net->max_crop;
    तर्काः.angle = net->angle;
    तर्काः.aspect = net->aspect;
    तर्काः.exposure = net->exposure;
    तर्काः.center = net->center;
    तर्काः.saturation = net->saturation;
    तर्काः.hue = net->hue;
    प्रत्यागम तर्काः;
}

जालम् *आरोपय_जालम्(वर्ण *cfg, वर्ण *weights, पूर्णाङ्क clear)
{
    जालम् *net = parse_जालम्_cfg(cfg);
    यदि(weights && weights[0] != 0){
        आरोपय_weights(net, weights);
    }
    यदि(clear) (*net->आलोकितम्) = 0;
    प्रत्यागम net;
}

size_t get_current_batch(जालम् *net)
{
    size_t batch_अङ्कः = (*net->आलोकितम्)/(net->batch*net->subdivisions);
    प्रत्यागम batch_अङ्कः;
}

शून्य reset_जालम्_state(जालम् *net, पूर्णाङ्क b)
{
    पूर्णाङ्क i;
    प्रति (i = 0; i < net->n; ++i) {
        #यदि_परिभाषितम् GPU
        layer l = net->layers[i];
        यदि(l.state_gpu){
            fill_gpu(l.outputs, 0, l.state_gpu + l.outputs*b, 1);
        }
        यदि(l.h_gpu){
            fill_gpu(l.outputs, 0, l.h_gpu + l.outputs*b, 1);
        }
        #यदि_अन्तः
    }
}

शून्य reset_rnn(जालम् *net)
{
    reset_जालम्_state(net, 0);
}

भिन्न get_current_दरः(जालम् *net)
{
    size_t batch_अङ्कः = get_current_batch(net);
    पूर्णाङ्क i;
    भिन्न दरः;
    यदि (batch_अङ्कः < net->burn_अन्तर्) प्रत्यागम net->learning_दरः * घातः((भिन्न)batch_अङ्कः / net->burn_अन्तर्, net->power);
    विकल्प (net->policy) {
        पक्ष CONSTANT:
            प्रत्यागम net->learning_दरः;
        पक्ष चरणः_2:
            प्रत्यागम net->learning_दरः * घातः(net->scale, batch_अङ्कः/net->चरणः);
        पक्ष चरणाः_2:
            दरः = net->learning_दरः;
            प्रति(i = 0; i < net->अङ्कः_चरणाः; ++i){
                यदि(net->चरणाः[i] > batch_अङ्कः) प्रत्यागम दरः;
                दरः *= net->scales[i];
            }
            प्रत्यागम दरः;
        पक्ष EXP:
            प्रत्यागम net->learning_दरः * घातः(net->gamma, batch_अङ्कः);
        पक्ष POLY:
            प्रत्यागम net->learning_दरः * घातः(1 - (भिन्न)batch_अङ्कः / net->max_batches, net->power);
        पक्ष RANDOM:
            प्रत्यागम net->learning_दरः * घातः(rand_एकरूपम्(0,1), net->power);
        पक्ष SIG:
            प्रत्यागम net->learning_दरः * (1./(1.+घातांकः(net->gamma*(batch_अङ्कः - net->चरणः))));
        स्वभाव:
            fprintf(stderr, "Policy is weird!\n");
            प्रत्यागम net->learning_दरः;
    }
}

वर्ण *get_layer_string(LAYER_TYPE a)
{
    विकल्प(a){
        पक्ष CONVOLUTIONAL:
            प्रत्यागम "convolutional";
        पक्ष सक्रिय:
            प्रत्यागम "activation";
        पक्ष LOCAL:
            प्रत्यागम "local";
        पक्ष DECONVOLUTIONAL:
            प्रत्यागम "deconvolutional";
        पक्ष CONNECTED:
            प्रत्यागम "connected";
        पक्ष RNN:
            प्रत्यागम "rnn";
        पक्ष GRU:
            प्रत्यागम "gru";
        पक्ष LSTM:
	    प्रत्यागम "lstm";
        पक्ष CRNN:
            प्रत्यागम "crnn";
        पक्ष MAXPOOL:
            प्रत्यागम "maxpool";
        पक्ष REORG:
            प्रत्यागम "reorg";
        पक्ष AVGPOOL:
            प्रत्यागम "avgpool";
        पक्ष SOFTMAX:
            प्रत्यागम "softmax";
        पक्ष DETECTION:
            प्रत्यागम "detection";
        पक्ष REGION:
            प्रत्यागम "region";
        पक्ष YOLO:
            प्रत्यागम "yolo";
        पक्ष DROPOUT:
            प्रत्यागम "dropout";
        पक्ष CROP:
            प्रत्यागम "crop";
        पक्ष COST:
            प्रत्यागम "cost";
        पक्ष ROUTE:
            प्रत्यागम "route";
        पक्ष SHORTCUT:
            प्रत्यागम "shortcut";
        पक्ष NORMALIZATION:
            प्रत्यागम "normalization";
        पक्ष BATCHNORM:
            प्रत्यागम "batchnorm";
        स्वभाव:
            भङ्ग;
    }
    प्रत्यागम "none";
}

जालम् *make_जालम्(पूर्णाङ्क n)
{
    जालम् *net = शून्यस्मृतिं_देहि(1, परिमाण(जालम्));
    net->n = n;
    net->layers = शून्यस्मृतिं_देहि(net->n, परिमाण(layer));
    net->आलोकितम् = शून्यस्मृतिं_देहि(1, परिमाण(size_t));
    net->t    = शून्यस्मृतिं_देहि(1, परिमाण(पूर्णाङ्क));
    net->cost = शून्यस्मृतिं_देहि(1, परिमाण(भिन्न));
    प्रत्यागम net;
}

शून्य forward_जालम्(जालम् *netp)
{
#यदि_परिभाषितम् GPU
    यदि(netp->gpu_सूचकांक >= 0){
        forward_जालम्_gpu(netp);   
        प्रत्यागम;
    }
#यदि_अन्तः
    जालम् net = *netp;
    पूर्णाङ्क i;
    प्रति(i = 0; i < net.n; ++i){
        net.सूचकांक = i;
        layer l = net.layers[i];
        यदि(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.निवेशः = l.output;
        यदि(l.truth) {
            net.truth = l.output;
        }
    }
    calc_जालम्_cost(netp);
}

शून्य update_जालम्(जालम् *netp)
{
#यदि_परिभाषितम् GPU
    यदि(netp->gpu_सूचकांक >= 0){
        update_जालम्_gpu(netp);   
        प्रत्यागम;
    }
#यदि_अन्तः
    जालम् net = *netp;
    पूर्णाङ्क i;
    update_तर्काः a = {0};
    a.batch = net.batch*net.subdivisions;
    a.learning_दरः = get_current_दरः(netp);
    a.momentum = net.momentum;
    a.decay = net.decay;
    a.adam = net.adam;
    a.B1 = net.B1;
    a.B2 = net.B2;
    a.eps = net.eps;
    ++*net.t;
    a.t = *net.t;

    प्रति(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        यदि(l.update){
            l.update(l, a);
        }
    }
}

शून्य calc_जालम्_cost(जालम् *netp)
{
    जालम् net = *netp;
    पूर्णाङ्क i;
    भिन्न योग = 0;
    पूर्णाङ्क गणनम् = 0;
    प्रति(i = 0; i < net.n; ++i){
        यदि(net.layers[i].cost){
            योग += net.layers[i].cost[0];
            ++गणनम्;
        }
    }
    *net.cost = योग/गणनम्;
}

पूर्णाङ्क get_predicted_class_जालम्(जालम् *net)
{
    प्रत्यागम max_सूचकांक(net->output, net->outputs);
}

शून्य backward_जालम्(जालम् *netp)
{
#यदि_परिभाषितम् GPU
    यदि(netp->gpu_सूचकांक >= 0){
        backward_जालम्_gpu(netp);   
        प्रत्यागम;
    }
#यदि_अन्तः
    जालम् net = *netp;
    पूर्णाङ्क i;
    जालम् orig = net;
    प्रति(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        यदि(l.stopbackward) भङ्ग;
        यदि(i == 0){
            net = orig;
        }अन्यथा{
            layer पूर्व = net.layers[i-1];
            net.निवेशः = पूर्व.output;
            net.delta = पूर्व.delta;
        }
        net.सूचकांक = i;
        l.backward(l, net);
    }
}

भिन्न train_जालम्_datum(जालम् *net)
{
    *net->आलोकितम् += net->batch;
    net->train = 1;
    forward_जालम्(net);
    backward_जालम्(net);
    भिन्न error = *net->cost;
    यदि(((*net->आलोकितम्)/net->batch)%net->subdivisions == 0) update_जालम्(net);
    प्रत्यागम error;
}

भिन्न train_जालम्_sgd(जालम् *net, दत्त d, पूर्णाङ्क n)
{
    पूर्णाङ्क batch = net->batch;

    पूर्णाङ्क i;
    भिन्न योग = 0;
    प्रति(i = 0; i < n; ++i){
        get_random_batch(d, batch, net->निवेशः, net->truth);
        भिन्न err = train_जालम्_datum(net);
        योग += err;
    }
    प्रत्यागम (भिन्न)योग/(n*batch);
}

भिन्न train_जालम्(जालम् *net, दत्त d)
{
    प्रतिज्ञा(d.X.पङ्क्तिकाः % net->batch == 0);
    पूर्णाङ्क batch = net->batch;
    पूर्णाङ्क n = d.X.पङ्क्तिकाः / batch;

    पूर्णाङ्क i;
    भिन्न योग = 0;
    प्रति(i = 0; i < n; ++i){
        get_अग्र_batch(d, batch, i*batch, net->निवेशः, net->truth);
        भिन्न err = train_जालम्_datum(net);
        योग += err;
    }
    प्रत्यागम (भिन्न)योग/(n*batch);
}

शून्य समुच्चयः_अस्थायी_जालम्(जालम् *net, भिन्न t)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < net->n; ++i){
        net->layers[i].temperature = t;
    }
}


शून्य समुच्चयः_batch_जालम्(जालम् *net, पूर्णाङ्क b)
{
    net->batch = b;
    पूर्णाङ्क i;
    प्रति(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#यदि_परिभाषितम् CUDNN
        यदि(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_सज्जता(net->layers + i);
        }
        यदि(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnn_समुच्चयः_Tensor4d_Descriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_दत्त_FLOAT, 1, l->बहिर्गमनम्_c, l->बहिर्गमनम्_h, l->बहिर्गमनम्_w);
            cudnn_समुच्चयः_Tensor4d_Descriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_दत्त_FLOAT, 1, l->बहिर्गमनम्_c, 1, 1); 
        }
#यदि_अन्तः
    }
}

पूर्णाङ्क resize_जालम्(जालम् *net, पूर्णाङ्क w, पूर्णाङ्क h)
{
#यदि_परिभाषितम् GPU
    cuda_समुच्चयः_device(net->gpu_सूचकांक);
    cuda_free(net->workspace);
#यदि_अन्तः
    पूर्णाङ्क i;
    //if(w == net->w && h == net->h) return 0;
    net->w = w;
    net->h = h;
    पूर्णाङ्क inputs = 0;
    size_t workspace_आकार = 0;
    //fprintf(stderr, "Resizing to %d x %d...\n", w, h);
    //fflush(stderr);
    प्रति (i = 0; i < net->n; ++i){
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
        यदि(l.workspace_आकार > workspace_आकार) workspace_आकार = l.workspace_आकार;
        यदि(l.workspace_आकार > 2000000000) प्रतिज्ञा(0);
        inputs = l.outputs;
        net->layers[i] = l;
        w = l.बहिर्गमनम्_w;
        h = l.बहिर्गमनम्_h;
        यदि(l.type == AVGPOOL) भङ्ग;
    }
    layer बहिर्गमनम् = get_जालम्_output_layer(net);
    net->inputs = net->layers[0].inputs;
    net->outputs = बहिर्गमनम्.outputs;
    net->truths = बहिर्गमनम्.outputs;
    यदि(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = बहिर्गमनम्.output;
    मुक्त(net->निवेशः);
    मुक्त(net->truth);
    net->निवेशः = शून्यस्मृतिं_देहि(net->inputs*net->batch, परिमाण(भिन्न));
    net->truth = शून्यस्मृतिं_देहि(net->truths*net->batch, परिमाण(भिन्न));
#यदि_परिभाषितम् GPU
    यदि(gpu_सूचकांक >= 0){
        cuda_free(net->निवेशः_gpu);
        cuda_free(net->truth_gpu);
        net->निवेशः_gpu = cuda_make_array(net->निवेशः, net->inputs*net->batch);
        net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
        यदि(workspace_आकार){
            net->workspace = cuda_make_array(0, (workspace_आकार-1)/परिमाण(भिन्न)+1);
        }
    }अन्यथा {
        मुक्त(net->workspace);
        net->workspace = शून्यस्मृतिं_देहि(1, workspace_आकार);
    }
#अन्यथा
    मुक्त(net->workspace);
    net->workspace = शून्यस्मृतिं_देहि(1, workspace_आकार);
#यदि_अन्तः
    //fprintf(stderr, " Done!\n");
    प्रत्यागम 0;
}

layer get_जालम्_detection_layer(जालम् *net)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < net->n; ++i){
        यदि(net->layers[i].type == DETECTION){
            प्रत्यागम net->layers[i];
        }
    }
    fprintf(stderr, "Detection layer not found!!\n");
    layer l = {0};
    प्रत्यागम l;
}

image get_जालम्_image_layer(जालम् *net, पूर्णाङ्क i)
{
    layer l = net->layers[i];
#यदि_परिभाषितम् GPU
    //cuda_pull_array(l.output_gpu, l.output, l.outputs);
#यदि_अन्तः
    यदि (l.बहिर्गमनम्_w && l.बहिर्गमनम्_h && l.बहिर्गमनम्_c){
        प्रत्यागम float_गन्तव्यम्_image(l.बहिर्गमनम्_w, l.बहिर्गमनम्_h, l.बहिर्गमनम्_c, l.output);
    }
    image def = {0};
    प्रत्यागम def;
}

image get_जालम्_image(जालम् *net)
{
    पूर्णाङ्क i;
    प्रति(i = net->n-1; i >= 0; --i){
        image m = get_जालम्_image_layer(net, i);
        यदि(m.h != 0) प्रत्यागम m;
    }
    image def = {0};
    प्रत्यागम def;
}

शून्य visualize_जालम्(जालम् *net)
{
    image *पूर्व = 0;
    पूर्णाङ्क i;
    वर्ण buff[256];
    प्रति(i = 0; i < net->n; ++i){
        sprintf(buff, "Layer %d", i);
        layer l = net->layers[i];
        यदि(l.type == CONVOLUTIONAL){
            पूर्व = visualize_convolutional_layer(l, buff, पूर्व);
        }
    } 
}

शून्य शिखरम्_predictions(जालम् *net, पूर्णाङ्क k, पूर्णाङ्क *सूचकांक)
{
    शिखरम्_k(net->output, net->outputs, k, सूचकांक);
}


भिन्न *जालम्_predict(जालम् *net, भिन्न *निवेशः)
{
    जालम् orig = *net;
    net->निवेशः = निवेशः;
    net->truth = 0;
    net->train = 0;
    net->delta = 0;
    forward_जालम्(net);
    भिन्न *बहिर्गमनम् = net->output;
    *net = orig;
    प्रत्यागम बहिर्गमनम्;
}

पूर्णाङ्क अङ्कः_detections(जालम् *net, भिन्न thresh)
{
    पूर्णाङ्क i;
    पूर्णाङ्क s = 0;
    प्रति(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        यदि(l.type == YOLO){
            s += yolo_अङ्कः_detections(l, thresh);
        }
        यदि(l.type == DETECTION || l.type == REGION){
            s += l.w*l.h*l.n;
        }
    }
    प्रत्यागम s;
}

detection *make_जालम्_boxes(जालम् *net, भिन्न thresh, पूर्णाङ्क *अङ्कः)
{
    layer l = net->layers[net->n - 1];
    पूर्णाङ्क i;
    पूर्णाङ्क nboxes = अङ्कः_detections(net, thresh);
    यदि(अङ्कः) *अङ्कः = nboxes;
    detection *dets = शून्यस्मृतिं_देहि(nboxes, परिमाण(detection));
    प्रति(i = 0; i < nboxes; ++i){
        dets[i].prob = शून्यस्मृतिं_देहि(l.classes, परिमाण(भिन्न));
        यदि(l.coords > 4){
            dets[i].आवरणम् = शून्यस्मृतिं_देहि(l.coords-4, परिमाण(भिन्न));
        }
    }
    प्रत्यागम dets;
}

शून्य fill_जालम्_boxes(जालम् *net, पूर्णाङ्क w, पूर्णाङ्क h, भिन्न thresh, भिन्न hier, पूर्णाङ्क *map, पूर्णाङ्क relative, detection *dets)
{
    पूर्णाङ्क j;
    प्रति(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        यदि(l.type == YOLO){
            पूर्णाङ्क गणनम् = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
            dets += गणनम्;
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

detection *get_जालम्_boxes(जालम् *net, पूर्णाङ्क w, पूर्णाङ्क h, भिन्न thresh, भिन्न hier, पूर्णाङ्क *map, पूर्णाङ्क relative, पूर्णाङ्क *अङ्कः)
{
    detection *dets = make_जालम्_boxes(net, thresh, अङ्कः);
    fill_जालम्_boxes(net, w, h, thresh, hier, map, relative, dets);
    प्रत्यागम dets;
}

शून्य free_detections(detection *dets, पूर्णाङ्क n)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        मुक्त(dets[i].prob);
        यदि(dets[i].आवरणम्) मुक्त(dets[i].आवरणम्);
    }
    मुक्त(dets);
}

भिन्न *जालम्_predict_image(जालम् *net, image im)
{
    image imr = letterbox_image(im, net->w, net->h);
    समुच्चयः_batch_जालम्(net, 1);
    भिन्न *p = जालम्_predict(net, imr.दत्त);
    free_image(imr);
    प्रत्यागम p;
}

पूर्णाङ्क जालम्_विस्तारः(जालम् *net){प्रत्यागम net->w;}
पूर्णाङ्क जालम्_height(जालम् *net){प्रत्यागम net->h;}

matrix जालम्_predict_दत्त_multi(जालम् *net, दत्त test, पूर्णाङ्क n)
{
    पूर्णाङ्क i,j,b,m;
    पूर्णाङ्क k = net->outputs;
    matrix pred = make_matrix(test.X.पङ्क्तिकाः, k);
    भिन्न *X = शून्यस्मृतिं_देहि(net->batch*test.X.पङ्क्तिकाः, परिमाण(भिन्न));
    प्रति(i = 0; i < test.X.पङ्क्तिकाः; i += net->batch){
        प्रति(b = 0; b < net->batch; ++b){
            यदि(i+b == test.X.पङ्क्तिकाः) भङ्ग;
            स्मृतिं_प्रतिलिख(X+b*test.X.स्तम्भसङ्ख्या, test.X.vals[i+b], test.X.स्तम्भसङ्ख्या*परिमाण(भिन्न));
        }
        प्रति(m = 0; m < n; ++m){
            भिन्न *बहिर्गमनम् = जालम्_predict(net, X);
            प्रति(b = 0; b < net->batch; ++b){
                यदि(i+b == test.X.पङ्क्तिकाः) भङ्ग;
                प्रति(j = 0; j < k; ++j){
                    pred.vals[i+b][j] += बहिर्गमनम्[j+b*k]/n;
                }
            }
        }
    }
    मुक्त(X);
    प्रत्यागम pred;   
}

matrix जालम्_predict_दत्त(जालम् *net, दत्त test)
{
    पूर्णाङ्क i,j,b;
    पूर्णाङ्क k = net->outputs;
    matrix pred = make_matrix(test.X.पङ्क्तिकाः, k);
    भिन्न *X = शून्यस्मृतिं_देहि(net->batch*test.X.स्तम्भसङ्ख्या, परिमाण(भिन्न));
    प्रति(i = 0; i < test.X.पङ्क्तिकाः; i += net->batch){
        प्रति(b = 0; b < net->batch; ++b){
            यदि(i+b == test.X.पङ्क्तिकाः) भङ्ग;
            स्मृतिं_प्रतिलिख(X+b*test.X.स्तम्भसङ्ख्या, test.X.vals[i+b], test.X.स्तम्भसङ्ख्या*परिमाण(भिन्न));
        }
        भिन्न *बहिर्गमनम् = जालम्_predict(net, X);
        प्रति(b = 0; b < net->batch; ++b){
            यदि(i+b == test.X.पङ्क्तिकाः) भङ्ग;
            प्रति(j = 0; j < k; ++j){
                pred.vals[i+b][j] = बहिर्गमनम्[j+b*k];
            }
        }
    }
    मुक्त(X);
    प्रत्यागम pred;   
}

शून्य print_जालम्(जालम् *net)
{
    पूर्णाङ्क i,j;
    प्रति(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        भिन्न *output = l.output;
        पूर्णाङ्क n = l.outputs;
        भिन्न माध्यम् = माध्यम्_array(output, n);
        भिन्न vari = variance_array(output, n);
        fprintf(stderr, "Layer %d - Mean: %f, Variance: %f\n",i,माध्यम्, vari);
        यदि(n > 100) n = 100;
        प्रति(j = 0; j < n; ++j) fprintf(stderr, "%f, ", output[j]);
        यदि(n == 100)fprintf(stderr,".....\n");
        fprintf(stderr, "\n");
    }
}

शून्य compare_networks(जालम् *n1, जालम् *n2, दत्त test)
{
    matrix g1 = जालम्_predict_दत्त(n1, test);
    matrix g2 = जालम्_predict_दत्त(n2, test);
    पूर्णाङ्क i;
    पूर्णाङ्क a,b,c,d;
    a = b = c = d = 0;
    प्रति(i = 0; i < g1.पङ्क्तिकाः; ++i){
        पूर्णाङ्क truth = max_सूचकांक(test.y.vals[i], test.y.स्तम्भसङ्ख्या);
        पूर्णाङ्क p1 = max_सूचकांक(g1.vals[i], g1.स्तम्भसङ्ख्या);
        पूर्णाङ्क p2 = max_सूचकांक(g2.vals[i], g2.स्तम्भसङ्ख्या);
        यदि(p1 == truth){
            यदि(p2 == truth) ++d;
            अन्यथा ++c;
        }अन्यथा{
            यदि(p2 == truth) ++b;
            अन्यथा ++a;
        }
    }
    प्रारूप_लिख("%5d %5d\n%5d %5d\n", a, b, c, d);
    भिन्न अङ्कः = घातः((निरपेक्षम्(b - c) - 1.), 2.);
    भिन्न den = b + c;
    प्रारूप_लिख("%f\n", अङ्कः/den); 
}

भिन्न जालम्_accuracy(जालम् *net, दत्त d)
{
    matrix guess = जालम्_predict_दत्त(net, d);
    भिन्न acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    प्रत्यागम acc;
}

भिन्न *जालम्_accuracies(जालम् *net, दत्त d, पूर्णाङ्क n)
{
    स्थिर भिन्न acc[2];
    matrix guess = जालम्_predict_दत्त(net, d);
    acc[0] = matrix_topk_accuracy(d.y, guess, 1);
    acc[1] = matrix_topk_accuracy(d.y, guess, n);
    free_matrix(guess);
    प्रत्यागम acc;
}

layer get_जालम्_output_layer(जालम् *net)
{
    पूर्णाङ्क i;
    प्रति(i = net->n - 1; i >= 0; --i){
        यदि(net->layers[i].type != COST) भङ्ग;
    }
    प्रत्यागम net->layers[i];
}

भिन्न जालम्_accuracy_multi(जालम् *net, दत्त d, पूर्णाङ्क n)
{
    matrix guess = जालम्_predict_दत्त_multi(net, d, n);
    भिन्न acc = matrix_topk_accuracy(d.y, guess,1);
    free_matrix(guess);
    प्रत्यागम acc;
}

शून्य free_जालम्(जालम् *net)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < net->n; ++i){
        free_layer(net->layers[i]);
    }
    मुक्त(net->layers);
    यदि(net->निवेशः) मुक्त(net->निवेशः);
    यदि(net->truth) मुक्त(net->truth);
#यदि_परिभाषितम् GPU
    यदि(net->निवेशः_gpu) cuda_free(net->निवेशः_gpu);
    यदि(net->truth_gpu) cuda_free(net->truth_gpu);
#यदि_अन्तः
    मुक्त(net);
}

// Some day...
// ^ What the hell is this comment for?


layer जालम्_output_layer(जालम् *net)
{
    पूर्णाङ्क i;
    प्रति(i = net->n - 1; i >= 0; --i){
        यदि(net->layers[i].type != COST) भङ्ग;
    }
    प्रत्यागम net->layers[i];
}

पूर्णाङ्क जालम्_inputs(जालम् *net)
{
    प्रत्यागम net->layers[0].inputs;
}

पूर्णाङ्क जालम्_outputs(जालम् *net)
{
    प्रत्यागम जालम्_output_layer(net).outputs;
}

भिन्न *जालम्_output(जालम् *net)
{
    प्रत्यागम जालम्_output_layer(net).output;
}

#यदि_परिभाषितम् GPU

शून्य forward_जालम्_gpu(जालम् *netp)
{
    जालम् net = *netp;
    cuda_समुच्चयः_device(net.gpu_सूचकांक);
    cuda_प्रक्षिप_array(net.निवेशः_gpu, net.निवेशः, net.inputs*net.batch);
    यदि(net.truth){
        cuda_प्रक्षिप_array(net.truth_gpu, net.truth, net.truths*net.batch);
    }

    पूर्णाङ्क i;
    प्रति(i = 0; i < net.n; ++i){
        net.सूचकांक = i;
        layer l = net.layers[i];
        यदि(l.delta_gpu){
            fill_gpu(l.outputs * l.batch, 0, l.delta_gpu, 1);
        }
        l.forward_gpu(l, net);
        net.निवेशः_gpu = l.output_gpu;
        net.निवेशः = l.output;
        यदि(l.truth) {
            net.truth_gpu = l.output_gpu;
            net.truth = l.output;
        }
    }
    pull_जालम्_output(netp);
    calc_जालम्_cost(netp);
}

शून्य backward_जालम्_gpu(जालम् *netp)
{
    पूर्णाङ्क i;
    जालम् net = *netp;
    जालम् orig = net;
    cuda_समुच्चयः_device(net.gpu_सूचकांक);
    प्रति(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        यदि(l.stopbackward) भङ्ग;
        यदि(i == 0){
            net = orig;
        }अन्यथा{
            layer पूर्व = net.layers[i-1];
            net.निवेशः = पूर्व.output;
            net.delta = पूर्व.delta;
            net.निवेशः_gpu = पूर्व.output_gpu;
            net.delta_gpu = पूर्व.delta_gpu;
        }
        net.सूचकांक = i;
        l.backward_gpu(l, net);
    }
}

शून्य update_जालम्_gpu(जालम् *netp)
{
    जालम् net = *netp;
    cuda_समुच्चयः_device(net.gpu_सूचकांक);
    पूर्णाङ्क i;
    update_तर्काः a = {0};
    a.batch = net.batch*net.subdivisions;
    a.learning_दरः = get_current_दरः(netp);
    a.momentum = net.momentum;
    a.decay = net.decay;
    a.adam = net.adam;
    a.B1 = net.B1;
    a.B2 = net.B2;
    a.eps = net.eps;
    ++*net.t;
    a.t = (*net.t);

    प्रति(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        यदि(l.update_gpu){
            l.update_gpu(l, a);
        }
    }
}

शून्य harmless_update_जालम्_gpu(जालम् *netp)
{
    जालम् net = *netp;
    cuda_समुच्चयः_device(net.gpu_सूचकांक);
    पूर्णाङ्क i;
    प्रति(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        यदि(l.weight_updates_gpu) fill_gpu(l.nweights, 0, l.weight_updates_gpu, 1);
        यदि(l.bias_updates_gpu) fill_gpu(l.nbiases, 0, l.bias_updates_gpu, 1);
        यदि(l.scale_updates_gpu) fill_gpu(l.nbiases, 0, l.scale_updates_gpu, 1);
    }
}

प्रकारनिर्देश संरचना {
    जालम् *net;
    दत्त d;
    भिन्न *err;
} train_तर्काः;

शून्य *train_thread(शून्य *ptr)
{
    train_तर्काः तर्काः = *(train_तर्काः*)ptr;
    मुक्त(ptr);
    cuda_समुच्चयः_device(तर्काः.net->gpu_सूचकांक);
    *तर्काः.err = train_जालम्(तर्काः.net, तर्काः.d);
    प्रत्यागम 0;
}

pthread_t train_जालम्_अन्तर्_thread(जालम् *net, दत्त d, भिन्न *err)
{
    pthread_t thread;
    train_तर्काः *ptr = (train_तर्काः *)शून्यस्मृतिं_देहि(1, परिमाण(train_तर्काः));
    ptr->net = net;
    ptr->d = d;
    ptr->err = err;
    यदि(pthread_create(&thread, 0, train_thread, ptr)) error("Thread creation failed");
    प्रत्यागम thread;
}

शून्य merge_weights(layer l, layer आधारः)
{
    यदि (l.type == CONVOLUTIONAL) {
        axpy_cpu(l.n, 1, l.bias_updates, 1, आधारः.biases, 1);
        axpy_cpu(l.nweights, 1, l.weight_updates, 1, आधारः.weights, 1);
        यदि (l.scales) {
            axpy_cpu(l.n, 1, l.scale_updates, 1, आधारः.scales, 1);
        }
    } अन्यथा यदि(l.type == CONNECTED) {
        axpy_cpu(l.outputs, 1, l.bias_updates, 1, आधारः.biases, 1);
        axpy_cpu(l.outputs*l.inputs, 1, l.weight_updates, 1, आधारः.weights, 1);
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

शून्य प्रक्षिप_weights(layer l)
{
    यदि(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_प्रक्षिप_array(l.biases_gpu, l.biases, l.n);
        cuda_प्रक्षिप_array(l.weights_gpu, l.weights, l.nweights);
        यदि(l.scales) cuda_प्रक्षिप_array(l.scales_gpu, l.scales, l.n);
    } अन्यथा यदि(l.type == CONNECTED){
        cuda_प्रक्षिप_array(l.biases_gpu, l.biases, l.outputs);
        cuda_प्रक्षिप_array(l.weights_gpu, l.weights, l.outputs*l.inputs);
    }
}

शून्य distribute_weights(layer l, layer आधारः)
{
    यदि (l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL) {
        cuda_प्रक्षिप_array(l.biases_gpu, आधारः.biases, l.n);
        cuda_प्रक्षिप_array(l.weights_gpu, आधारः.weights, l.nweights);
        यदि (आधारः.scales) cuda_प्रक्षिप_array(l.scales_gpu, आधारः.scales, l.n);
    } अन्यथा यदि (l.type == CONNECTED) {
        cuda_प्रक्षिप_array(l.biases_gpu, आधारः.biases, l.outputs);
        cuda_प्रक्षिप_array(l.weights_gpu, आधारः.weights, l.outputs*l.inputs);
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

शून्य sync_layer(जालम् **nets, पूर्णाङ्क n, पूर्णाङ्क j)
{
    पूर्णाङ्क i;
    जालम् *net = nets[0];
    layer आधारः = net->layers[j];
    scale_weights(आधारः, 0);
    प्रति (i = 0; i < n; ++i) {
        cuda_समुच्चयः_device(nets[i]->gpu_सूचकांक);
        layer l = nets[i]->layers[j];
        pull_weights(l);
        merge_weights(l, आधारः);
    }
    scale_weights(आधारः, 1./n);
    प्रति (i = 0; i < n; ++i) {
        cuda_समुच्चयः_device(nets[i]->gpu_सूचकांक);
        layer l = nets[i]->layers[j];
        distribute_weights(l, आधारः);
    }
}

प्रकारनिर्देश संरचना{
    जालम् **nets;
    पूर्णाङ्क n;
    पूर्णाङ्क j;
} sync_तर्काः;

शून्य *sync_layer_thread(शून्य *ptr)
{
    sync_तर्काः तर्काः = *(sync_तर्काः*)ptr;
    sync_layer(तर्काः.nets, तर्काः.n, तर्काः.j);
    मुक्त(ptr);
    प्रत्यागम 0;
}

pthread_t sync_layer_अन्तर्_thread(जालम् **nets, पूर्णाङ्क n, पूर्णाङ्क j)
{
    pthread_t thread;
    sync_तर्काः *ptr = (sync_तर्काः *)शून्यस्मृतिं_देहि(1, परिमाण(sync_तर्काः));
    ptr->nets = nets;
    ptr->n = n;
    ptr->j = j;
    यदि(pthread_create(&thread, 0, sync_layer_thread, ptr)) error("Thread creation failed");
    प्रत्यागम thread;
}

शून्य sync_nets(जालम् **nets, पूर्णाङ्क n, पूर्णाङ्क interval)
{
    पूर्णाङ्क j;
    पूर्णाङ्क layers = nets[0]->n;
    pthread_t *तन्तवः = (pthread_t *) शून्यस्मृतिं_देहि(layers, परिमाण(pthread_t));

    *(nets[0]->आलोकितम्) += interval * (n-1) * nets[0]->batch * nets[0]->subdivisions;
    प्रति (j = 0; j < n; ++j){
        *(nets[j]->आलोकितम्) = *(nets[0]->आलोकितम्);
    }
    प्रति (j = 0; j < layers; ++j) {
        तन्तवः[j] = sync_layer_अन्तर्_thread(nets, n, j);
    }
    प्रति (j = 0; j < layers; ++j) {
        pthread_join(तन्तवः[j], 0);
    }
    मुक्त(तन्तवः);
}

भिन्न train_networks(जालम् **nets, पूर्णाङ्क n, दत्त d, पूर्णाङ्क interval)
{
    पूर्णाङ्क i;
    पूर्णाङ्क batch = nets[0]->batch;
    पूर्णाङ्क subdivisions = nets[0]->subdivisions;
    प्रतिज्ञा(batch * subdivisions * n == d.X.पङ्क्तिकाः);
    pthread_t *तन्तवः = (pthread_t *) शून्यस्मृतिं_देहि(n, परिमाण(pthread_t));
    भिन्न *errors = (भिन्न *) शून्यस्मृतिं_देहि(n, परिमाण(भिन्न));

    भिन्न योग = 0;
    प्रति(i = 0; i < n; ++i){
        दत्त p = get_दत्त_part(d, i, n);
        तन्तवः[i] = train_जालम्_अन्तर्_thread(nets[i], p, errors + i);
    }
    प्रति(i = 0; i < n; ++i){
        pthread_join(तन्तवः[i], 0);
        //printf("%f\n", errors[i]);
        योग += errors[i];
    }
    //cudaDeviceSynchronize();
    यदि (get_current_batch(nets[0]) % interval == 0) {
        प्रारूप_लिख("Syncing... ");
        fflush(stdout);
        sync_nets(nets, n, interval);
        प्रारूप_लिख("Done!\n");
    }
    //cudaDeviceSynchronize();
    मुक्त(तन्तवः);
    मुक्त(errors);
    प्रत्यागम (भिन्न)योग/(n);
}

शून्य pull_जालम्_output(जालम् *net)
{
    layer l = get_जालम्_output_layer(net);
    cuda_pull_array(l.output_gpu, l.output, l.outputs*l.batch);
}

#यदि_अन्तः

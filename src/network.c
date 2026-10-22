#అన్తర్భావ <stdio.h>
#అన్తర్భావ <time.h>
#అన్తర్భావ <assert.h>
#అన్తర్భావ "network.h"
#అన్తర్భావ "image.h"
#అన్తర్భావ "data.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "blas.h"

#అన్తర్భావ "crop_layer.h"
#అన్తర్భావ "connected_layer.h"
#అన్తర్భావ "gru_layer.h"
#అన్తర్భావ "rnn_layer.h"
#అన్తర్భావ "crnn_layer.h"
#అన్తర్భావ "local_layer.h"
#అన్తర్భావ "convolutional_layer.h"
#అన్తర్భావ "activation_layer.h"
#అన్తర్భావ "detection_layer.h"
#అన్తర్భావ "region_layer.h"
#అన్తర్భావ "yolo_layer.h"
#అన్తర్భావ "normalization_layer.h"
#అన్తర్భావ "batchnorm_layer.h"
#అన్తర్భావ "maxpool_layer.h"
#అన్తర్భావ "reorg_layer.h"
#అన్తర్భావ "avgpool_layer.h"
#అన్తర్భావ "cost_layer.h"
#అన్తర్భావ "softmax_layer.h"
#అన్తర్భావ "dropout_layer.h"
#అన్తర్భావ "route_layer.h"
#అన్తర్భావ "upsample_layer.h"
#అన్తర్భావ "shortcut_layer.h"
#అన్తర్భావ "parser.h"
#అన్తర్భావ "data.h"

ఎక్కించు_వాదనలు get_పునాది_వాదనలు(జాలం *net)
{
    ఎక్కించు_వాదనలు వాదనలు = {0};
    వాదనలు.w = net->w;
    వాదనలు.h = net->h;
    వాదనలు.కొలత = net->w;

    వాదనలు.min = net->min_crop;
    వాదనలు.max = net->max_crop;
    వాదనలు.angle = net->angle;
    వాదనలు.aspect = net->aspect;
    వాదనలు.exposure = net->exposure;
    వాదనలు.center = net->center;
    వాదనలు.saturation = net->saturation;
    వాదనలు.hue = net->hue;
    తిరిగి వాదనలు;
}

జాలం *ఎక్కించు_జాలం(అక్షరం *cfg, అక్షరం *weights, పూర్ణాంకం clear)
{
    జాలం *net = parse_జాలం_cfg(cfg);
    ఒకవేళ(weights && weights[0] != 0){
        ఎక్కించు_weights(net, weights);
    }
    ఒకవేళ(clear) (*net->చూసిన) = 0;
    తిరిగి net;
}

size_t get_current_batch(జాలం *net)
{
    size_t batch_అంకె = (*net->చూసిన)/(net->batch*net->subdivisions);
    తిరిగి batch_అంకె;
}

శూన్య reset_జాలం_state(జాలం *net, పూర్ణాంకం b)
{
    పూర్ణాంకం i;
    కొరకు (i = 0; i < net->n; ++i) {
        #నిర్వచితమైతే GPU
        layer l = net->layers[i];
        ఒకవేళ(l.state_gpu){
            fill_gpu(l.outputs, 0, l.state_gpu + l.outputs*b, 1);
        }
        ఒకవేళ(l.h_gpu){
            fill_gpu(l.outputs, 0, l.h_gpu + l.outputs*b, 1);
        }
        #అయితే_ముగింపు
    }
}

శూన్య reset_rnn(జాలం *net)
{
    reset_జాలం_state(net, 0);
}

దశాంశం get_current_రేటు(జాలం *net)
{
    size_t batch_అంకె = get_current_batch(net);
    పూర్ణాంకం i;
    దశాంశం రేటు;
    ఒకవేళ (batch_అంకె < net->burn_లోపల) తిరిగి net->learning_రేటు * ఘాతం((దశాంశం)batch_అంకె / net->burn_లోపల, net->power);
    ఎంపిక (net->policy) {
        స్థితి CONSTANT:
            తిరిగి net->learning_రేటు;
        స్థితి అడుగు_2:
            తిరిగి net->learning_రేటు * ఘాతం(net->scale, batch_అంకె/net->అడుగు);
        స్థితి అడుగులు_2:
            రేటు = net->learning_రేటు;
            కొరకు(i = 0; i < net->అంకె_అడుగులు; ++i){
                ఒకవేళ(net->అడుగులు[i] > batch_అంకె) తిరిగి రేటు;
                రేటు *= net->scales[i];
            }
            తిరిగి రేటు;
        స్థితి EXP:
            తిరిగి net->learning_రేటు * ఘాతం(net->gamma, batch_అంకె);
        స్థితి POLY:
            తిరిగి net->learning_రేటు * ఘాతం(1 - (దశాంశం)batch_అంకె / net->max_batches, net->power);
        స్థితి RANDOM:
            తిరిగి net->learning_రేటు * ఘాతం(rand_ఏకరీతి(0,1), net->power);
        స్థితి SIG:
            తిరిగి net->learning_రేటు * (1./(1.+ఘాతాంకం(net->gamma*(batch_అంకె - net->అడుగు))));
        అప్రమేయ:
            fprintf(stderr, "Policy is weird!\n");
            తిరిగి net->learning_రేటు;
    }
}

అక్షరం *get_layer_string(LAYER_TYPE a)
{
    ఎంపిక(a){
        స్థితి CONVOLUTIONAL:
            తిరిగి "convolutional";
        స్థితి చురుకైన:
            తిరిగి "activation";
        స్థితి LOCAL:
            తిరిగి "local";
        స్థితి DECONVOLUTIONAL:
            తిరిగి "deconvolutional";
        స్థితి CONNECTED:
            తిరిగి "connected";
        స్థితి RNN:
            తిరిగి "rnn";
        స్థితి GRU:
            తిరిగి "gru";
        స్థితి LSTM:
	    తిరిగి "lstm";
        స్థితి CRNN:
            తిరిగి "crnn";
        స్థితి MAXPOOL:
            తిరిగి "maxpool";
        స్థితి REORG:
            తిరిగి "reorg";
        స్థితి AVGPOOL:
            తిరిగి "avgpool";
        స్థితి SOFTMAX:
            తిరిగి "softmax";
        స్థితి DETECTION:
            తిరిగి "detection";
        స్థితి REGION:
            తిరిగి "region";
        స్థితి YOLO:
            తిరిగి "yolo";
        స్థితి DROPOUT:
            తిరిగి "dropout";
        స్థితి CROP:
            తిరిగి "crop";
        స్థితి ఖర్చు:
            తిరిగి "cost";
        స్థితి ROUTE:
            తిరిగి "route";
        స్థితి SHORTCUT:
            తిరిగి "shortcut";
        స్థితి NORMALIZATION:
            తిరిగి "normalization";
        స్థితి BATCHNORM:
            తిరిగి "batchnorm";
        అప్రమేయ:
            ఆపు;
    }
    తిరిగి "none";
}

జాలం *make_జాలం(పూర్ణాంకం n)
{
    జాలం *net = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(జాలం));
    net->n = n;
    net->layers = సున్నా_జ్ఞాపకం_ఇవ్వు(net->n, పరిమాణం(layer));
    net->చూసిన = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(size_t));
    net->t    = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(పూర్ణాంకం));
    net->ఖర్చు_2 = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(దశాంశం));
    తిరిగి net;
}

శూన్య forward_జాలం(జాలం *netp)
{
#నిర్వచితమైతే GPU
    ఒకవేళ(netp->gpu_సూచీ >= 0){
        forward_జాలం_gpu(netp);   
        తిరిగి;
    }
#అయితే_ముగింపు
    జాలం net = *netp;
    పూర్ణాంకం i;
    కొరకు(i = 0; i < net.n; ++i){
        net.సూచీ = i;
        layer l = net.layers[i];
        ఒకవేళ(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.ఆదానం = l.output;
        ఒకవేళ(l.truth) {
            net.truth = l.output;
        }
    }
    calc_జాలం_ఖర్చు(netp);
}

శూన్య update_జాలం(జాలం *netp)
{
#నిర్వచితమైతే GPU
    ఒకవేళ(netp->gpu_సూచీ >= 0){
        update_జాలం_gpu(netp);   
        తిరిగి;
    }
#అయితే_ముగింపు
    జాలం net = *netp;
    పూర్ణాంకం i;
    update_వాదనలు a = {0};
    a.batch = net.batch*net.subdivisions;
    a.learning_రేటు = get_current_రేటు(netp);
    a.momentum = net.momentum;
    a.decay = net.decay;
    a.adam = net.adam;
    a.B1 = net.B1;
    a.B2 = net.B2;
    a.eps = net.eps;
    ++*net.t;
    a.t = *net.t;

    కొరకు(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        ఒకవేళ(l.update){
            l.update(l, a);
        }
    }
}

శూన్య calc_జాలం_ఖర్చు(జాలం *netp)
{
    జాలం net = *netp;
    పూర్ణాంకం i;
    దశాంశం కూడిక = 0;
    పూర్ణాంకం లెక్క = 0;
    కొరకు(i = 0; i < net.n; ++i){
        ఒకవేళ(net.layers[i].ఖర్చు_2){
            కూడిక += net.layers[i].ఖర్చు_2[0];
            ++లెక్క;
        }
    }
    *net.ఖర్చు_2 = కూడిక/లెక్క;
}

పూర్ణాంకం get_predicted_class_జాలం(జాలం *net)
{
    తిరిగి max_సూచీ(net->output, net->outputs);
}

శూన్య backward_జాలం(జాలం *netp)
{
#నిర్వచితమైతే GPU
    ఒకవేళ(netp->gpu_సూచీ >= 0){
        backward_జాలం_gpu(netp);   
        తిరిగి;
    }
#అయితే_ముగింపు
    జాలం net = *netp;
    పూర్ణాంకం i;
    జాలం orig = net;
    కొరకు(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        ఒకవేళ(l.stopbackward) ఆపు;
        ఒకవేళ(i == 0){
            net = orig;
        }లేకపోతే{
            layer prev = net.layers[i-1];
            net.ఆదానం = prev.output;
            net.delta = prev.delta;
        }
        net.సూచీ = i;
        l.backward(l, net);
    }
}

దశాంశం train_జాలం_datum(జాలం *net)
{
    *net->చూసిన += net->batch;
    net->train = 1;
    forward_జాలం(net);
    backward_జాలం(net);
    దశాంశం error = *net->ఖర్చు_2;
    ఒకవేళ(((*net->చూసిన)/net->batch)%net->subdivisions == 0) update_జాలం(net);
    తిరిగి error;
}

దశాంశం train_జాలం_sgd(జాలం *net, దత్తాంశం d, పూర్ణాంకం n)
{
    పూర్ణాంకం batch = net->batch;

    పూర్ణాంకం i;
    దశాంశం కూడిక = 0;
    కొరకు(i = 0; i < n; ++i){
        get_random_batch(d, batch, net->ఆదానం, net->truth);
        దశాంశం err = train_జాలం_datum(net);
        కూడిక += err;
    }
    తిరిగి (దశాంశం)కూడిక/(n*batch);
}

దశాంశం train_జాలం(జాలం *net, దత్తాంశం d)
{
    ప్రతిజ్ఞా(d.X.అడ్డువరుసలు % net->batch == 0);
    పూర్ణాంకం batch = net->batch;
    పూర్ణాంకం n = d.X.అడ్డువరుసలు / batch;

    పూర్ణాంకం i;
    దశాంశం కూడిక = 0;
    కొరకు(i = 0; i < n; ++i){
        get_తదుపరి_batch(d, batch, i*batch, net->ఆదానం, net->truth);
        దశాంశం err = train_జాలం_datum(net);
        కూడిక += err;
    }
    తిరిగి (దశాంశం)కూడిక/(n*batch);
}

శూన్య సమితి_తాత్కాలిక_జాలం(జాలం *net, దశాంశం t)
{
    పూర్ణాంకం i;
    కొరకు(i = 0; i < net->n; ++i){
        net->layers[i].temperature = t;
    }
}


శూన్య సమితి_batch_జాలం(జాలం *net, పూర్ణాంకం b)
{
    net->batch = b;
    పూర్ణాంకం i;
    కొరకు(i = 0; i < net->n; ++i){
        net->layers[i].batch = b;
#నిర్వచితమైతే CUDNN
        ఒకవేళ(net->layers[i].type == CONVOLUTIONAL){
            cudnn_convolutional_అమరిక(net->layers + i);
        }
        ఒకవేళ(net->layers[i].type == DECONVOLUTIONAL){
            layer *l = net->layers + i;
            cudnn_సమితి_Tensor4d_Descriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, 1, l->బహిర్గతం_c, l->బహిర్గతం_h, l->బహిర్గతం_w);
            cudnn_సమితి_Tensor4d_Descriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, 1, l->బహిర్గతం_c, 1, 1); 
        }
#అయితే_ముగింపు
    }
}

పూర్ణాంకం resize_జాలం(జాలం *net, పూర్ణాంకం w, పూర్ణాంకం h)
{
#నిర్వచితమైతే GPU
    cuda_సమితి_device(net->gpu_సూచీ);
    cuda_free(net->workspace);
#అయితే_ముగింపు
    పూర్ణాంకం i;
    //if(w == net->w && h == net->h) return 0;
    net->w = w;
    net->h = h;
    పూర్ణాంకం inputs = 0;
    size_t workspace_కొలత = 0;
    //fprintf(stderr, "Resizing to %d x %d...\n", w, h);
    //fflush(stderr);
    కొరకు (i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ఒకవేళ(l.type == CONVOLUTIONAL){
            resize_convolutional_layer(&l, w, h);
        }లేకపోతే ఒకవేళ(l.type == CROP){
            resize_crop_layer(&l, w, h);
        }లేకపోతే ఒకవేళ(l.type == MAXPOOL){
            resize_maxpool_layer(&l, w, h);
        }లేకపోతే ఒకవేళ(l.type == REGION){
            resize_region_layer(&l, w, h);
        }లేకపోతే ఒకవేళ(l.type == YOLO){
            resize_yolo_layer(&l, w, h);
        }లేకపోతే ఒకవేళ(l.type == ROUTE){
            resize_route_layer(&l, net);
        }లేకపోతే ఒకవేళ(l.type == SHORTCUT){
            resize_shortcut_layer(&l, w, h);
        }లేకపోతే ఒకవేళ(l.type == UPSAMPLE){
            resize_upsample_layer(&l, w, h);
        }లేకపోతే ఒకవేళ(l.type == REORG){
            resize_reorg_layer(&l, w, h);
        }లేకపోతే ఒకవేళ(l.type == AVGPOOL){
            resize_avgpool_layer(&l, w, h);
        }లేకపోతే ఒకవేళ(l.type == NORMALIZATION){
            resize_normalization_layer(&l, w, h);
        }లేకపోతే ఒకవేళ(l.type == ఖర్చు){
            resize_ఖర్చు_layer(&l, inputs);
        }లేకపోతే{
            error("Cannot resize this type of layer");
        }
        ఒకవేళ(l.workspace_కొలత > workspace_కొలత) workspace_కొలత = l.workspace_కొలత;
        ఒకవేళ(l.workspace_కొలత > 2000000000) ప్రతిజ్ఞా(0);
        inputs = l.outputs;
        net->layers[i] = l;
        w = l.బహిర్గతం_w;
        h = l.బహిర్గతం_h;
        ఒకవేళ(l.type == AVGPOOL) ఆపు;
    }
    layer బహిర్గతం = get_జాలం_output_layer(net);
    net->inputs = net->layers[0].inputs;
    net->outputs = బహిర్గతం.outputs;
    net->truths = బహిర్గతం.outputs;
    ఒకవేళ(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = బహిర్గతం.output;
    విడుదల(net->ఆదానం);
    విడుదల(net->truth);
    net->ఆదానం = సున్నా_జ్ఞాపకం_ఇవ్వు(net->inputs*net->batch, పరిమాణం(దశాంశం));
    net->truth = సున్నా_జ్ఞాపకం_ఇవ్వు(net->truths*net->batch, పరిమాణం(దశాంశం));
#నిర్వచితమైతే GPU
    ఒకవేళ(gpu_సూచీ >= 0){
        cuda_free(net->ఆదానం_gpu);
        cuda_free(net->truth_gpu);
        net->ఆదానం_gpu = cuda_make_array(net->ఆదానం, net->inputs*net->batch);
        net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
        ఒకవేళ(workspace_కొలత){
            net->workspace = cuda_make_array(0, (workspace_కొలత-1)/పరిమాణం(దశాంశం)+1);
        }
    }లేకపోతే {
        విడుదల(net->workspace);
        net->workspace = సున్నా_జ్ఞాపకం_ఇవ్వు(1, workspace_కొలత);
    }
#లేకపోతే
    విడుదల(net->workspace);
    net->workspace = సున్నా_జ్ఞాపకం_ఇవ్వు(1, workspace_కొలత);
#అయితే_ముగింపు
    //fprintf(stderr, " Done!\n");
    తిరిగి 0;
}

layer get_జాలం_detection_layer(జాలం *net)
{
    పూర్ణాంకం i;
    కొరకు(i = 0; i < net->n; ++i){
        ఒకవేళ(net->layers[i].type == DETECTION){
            తిరిగి net->layers[i];
        }
    }
    fprintf(stderr, "Detection layer not found!!\n");
    layer l = {0};
    తిరిగి l;
}

image get_జాలం_image_layer(జాలం *net, పూర్ణాంకం i)
{
    layer l = net->layers[i];
#నిర్వచితమైతే GPU
    //cuda_pull_array(l.output_gpu, l.output, l.outputs);
#అయితే_ముగింపు
    ఒకవేళ (l.బహిర్గతం_w && l.బహిర్గతం_h && l.బహిర్గతం_c){
        తిరిగి float_గమ్యం_image(l.బహిర్గతం_w, l.బహిర్గతం_h, l.బహిర్గతం_c, l.output);
    }
    image def = {0};
    తిరిగి def;
}

image get_జాలం_image(జాలం *net)
{
    పూర్ణాంకం i;
    కొరకు(i = net->n-1; i >= 0; --i){
        image m = get_జాలం_image_layer(net, i);
        ఒకవేళ(m.h != 0) తిరిగి m;
    }
    image def = {0};
    తిరిగి def;
}

శూన్య visualize_జాలం(జాలం *net)
{
    image *prev = 0;
    పూర్ణాంకం i;
    అక్షరం buff[256];
    కొరకు(i = 0; i < net->n; ++i){
        sprintf(buff, "Layer %d", i);
        layer l = net->layers[i];
        ఒకవేళ(l.type == CONVOLUTIONAL){
            prev = visualize_convolutional_layer(l, buff, prev);
        }
    } 
}

శూన్య శిఖరం_predictions(జాలం *net, పూర్ణాంకం k, పూర్ణాంకం *సూచీ)
{
    శిఖరం_k(net->output, net->outputs, k, సూచీ);
}


దశాంశం *జాలం_predict(జాలం *net, దశాంశం *ఆదానం)
{
    జాలం orig = *net;
    net->ఆదానం = ఆదానం;
    net->truth = 0;
    net->train = 0;
    net->delta = 0;
    forward_జాలం(net);
    దశాంశం *బహిర్గతం = net->output;
    *net = orig;
    తిరిగి బహిర్గతం;
}

పూర్ణాంకం అంకె_detections(జాలం *net, దశాంశం thresh)
{
    పూర్ణాంకం i;
    పూర్ణాంకం s = 0;
    కొరకు(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ఒకవేళ(l.type == YOLO){
            s += yolo_అంకె_detections(l, thresh);
        }
        ఒకవేళ(l.type == DETECTION || l.type == REGION){
            s += l.w*l.h*l.n;
        }
    }
    తిరిగి s;
}

detection *make_జాలం_boxes(జాలం *net, దశాంశం thresh, పూర్ణాంకం *అంకె)
{
    layer l = net->layers[net->n - 1];
    పూర్ణాంకం i;
    పూర్ణాంకం nboxes = అంకె_detections(net, thresh);
    ఒకవేళ(అంకె) *అంకె = nboxes;
    detection *dets = సున్నా_జ్ఞాపకం_ఇవ్వు(nboxes, పరిమాణం(detection));
    కొరకు(i = 0; i < nboxes; ++i){
        dets[i].prob = సున్నా_జ్ఞాపకం_ఇవ్వు(l.classes, పరిమాణం(దశాంశం));
        ఒకవేళ(l.coords > 4){
            dets[i].ముసుగు = సున్నా_జ్ఞాపకం_ఇవ్వు(l.coords-4, పరిమాణం(దశాంశం));
        }
    }
    తిరిగి dets;
}

శూన్య fill_జాలం_boxes(జాలం *net, పూర్ణాంకం w, పూర్ణాంకం h, దశాంశం thresh, దశాంశం hier, పూర్ణాంకం *map, పూర్ణాంకం relative, detection *dets)
{
    పూర్ణాంకం j;
    కొరకు(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        ఒకవేళ(l.type == YOLO){
            పూర్ణాంకం లెక్క = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
            dets += లెక్క;
        }
        ఒకవేళ(l.type == REGION){
            get_region_detections(l, w, h, net->w, net->h, thresh, map, hier, relative, dets);
            dets += l.w*l.h*l.n;
        }
        ఒకవేళ(l.type == DETECTION){
            get_detection_detections(l, w, h, thresh, dets);
            dets += l.w*l.h*l.n;
        }
    }
}

detection *get_జాలం_boxes(జాలం *net, పూర్ణాంకం w, పూర్ణాంకం h, దశాంశం thresh, దశాంశం hier, పూర్ణాంకం *map, పూర్ణాంకం relative, పూర్ణాంకం *అంకె)
{
    detection *dets = make_జాలం_boxes(net, thresh, అంకె);
    fill_జాలం_boxes(net, w, h, thresh, hier, map, relative, dets);
    తిరిగి dets;
}

శూన్య free_detections(detection *dets, పూర్ణాంకం n)
{
    పూర్ణాంకం i;
    కొరకు(i = 0; i < n; ++i){
        విడుదల(dets[i].prob);
        ఒకవేళ(dets[i].ముసుగు) విడుదల(dets[i].ముసుగు);
    }
    విడుదల(dets);
}

దశాంశం *జాలం_predict_image(జాలం *net, image im)
{
    image imr = letterbox_image(im, net->w, net->h);
    సమితి_batch_జాలం(net, 1);
    దశాంశం *p = జాలం_predict(net, imr.దత్తాంశం);
    free_image(imr);
    తిరిగి p;
}

పూర్ణాంకం జాలం_వెడల్పు(జాలం *net){తిరిగి net->w;}
పూర్ణాంకం జాలం_ఎత్తు(జాలం *net){తిరిగి net->h;}

మాతృక జాలం_predict_దత్తాంశం_multi(జాలం *net, దత్తాంశం test, పూర్ణాంకం n)
{
    పూర్ణాంకం i,j,b,m;
    పూర్ణాంకం k = net->outputs;
    మాతృక pred = make_మాతృక(test.X.అడ్డువరుసలు, k);
    దశాంశం *X = సున్నా_జ్ఞాపకం_ఇవ్వు(net->batch*test.X.అడ్డువరుసలు, పరిమాణం(దశాంశం));
    కొరకు(i = 0; i < test.X.అడ్డువరుసలు; i += net->batch){
        కొరకు(b = 0; b < net->batch; ++b){
            ఒకవేళ(i+b == test.X.అడ్డువరుసలు) ఆపు;
            జ్ఞాపకం_నకలు(X+b*test.X.నిలువుల_సంఖ్య, test.X.vals[i+b], test.X.నిలువుల_సంఖ్య*పరిమాణం(దశాంశం));
        }
        కొరకు(m = 0; m < n; ++m){
            దశాంశం *బహిర్గతం = జాలం_predict(net, X);
            కొరకు(b = 0; b < net->batch; ++b){
                ఒకవేళ(i+b == test.X.అడ్డువరుసలు) ఆపు;
                కొరకు(j = 0; j < k; ++j){
                    pred.vals[i+b][j] += బహిర్గతం[j+b*k]/n;
                }
            }
        }
    }
    విడుదల(X);
    తిరిగి pred;   
}

మాతృక జాలం_predict_దత్తాంశం(జాలం *net, దత్తాంశం test)
{
    పూర్ణాంకం i,j,b;
    పూర్ణాంకం k = net->outputs;
    మాతృక pred = make_మాతృక(test.X.అడ్డువరుసలు, k);
    దశాంశం *X = సున్నా_జ్ఞాపకం_ఇవ్వు(net->batch*test.X.నిలువుల_సంఖ్య, పరిమాణం(దశాంశం));
    కొరకు(i = 0; i < test.X.అడ్డువరుసలు; i += net->batch){
        కొరకు(b = 0; b < net->batch; ++b){
            ఒకవేళ(i+b == test.X.అడ్డువరుసలు) ఆపు;
            జ్ఞాపకం_నకలు(X+b*test.X.నిలువుల_సంఖ్య, test.X.vals[i+b], test.X.నిలువుల_సంఖ్య*పరిమాణం(దశాంశం));
        }
        దశాంశం *బహిర్గతం = జాలం_predict(net, X);
        కొరకు(b = 0; b < net->batch; ++b){
            ఒకవేళ(i+b == test.X.అడ్డువరుసలు) ఆపు;
            కొరకు(j = 0; j < k; ++j){
                pred.vals[i+b][j] = బహిర్గతం[j+b*k];
            }
        }
    }
    విడుదల(X);
    తిరిగి pred;   
}

శూన్య print_జాలం(జాలం *net)
{
    పూర్ణాంకం i,j;
    కొరకు(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        దశాంశం *output = l.output;
        పూర్ణాంకం n = l.outputs;
        దశాంశం సగటు = సగటు_array(output, n);
        దశాంశం vari = variance_array(output, n);
        fprintf(stderr, "Layer %d - Mean: %f, Variance: %f\n",i,సగటు, vari);
        ఒకవేళ(n > 100) n = 100;
        కొరకు(j = 0; j < n; ++j) fprintf(stderr, "%f, ", output[j]);
        ఒకవేళ(n == 100)fprintf(stderr,".....\n");
        fprintf(stderr, "\n");
    }
}

శూన్య compare_networks(జాలం *n1, జాలం *n2, దత్తాంశం test)
{
    మాతృక g1 = జాలం_predict_దత్తాంశం(n1, test);
    మాతృక g2 = జాలం_predict_దత్తాంశం(n2, test);
    పూర్ణాంకం i;
    పూర్ణాంకం a,b,c,d;
    a = b = c = d = 0;
    కొరకు(i = 0; i < g1.అడ్డువరుసలు; ++i){
        పూర్ణాంకం truth = max_సూచీ(test.y.vals[i], test.y.నిలువుల_సంఖ్య);
        పూర్ణాంకం p1 = max_సూచీ(g1.vals[i], g1.నిలువుల_సంఖ్య);
        పూర్ణాంకం p2 = max_సూచీ(g2.vals[i], g2.నిలువుల_సంఖ్య);
        ఒకవేళ(p1 == truth){
            ఒకవేళ(p2 == truth) ++d;
            లేకపోతే ++c;
        }లేకపోతే{
            ఒకవేళ(p2 == truth) ++b;
            లేకపోతే ++a;
        }
    }
    రాయి("%5d %5d\n%5d %5d\n", a, b, c, d);
    దశాంశం అంకె = ఘాతం((నిరపేక్ష(b - c) - 1.), 2.);
    దశాంశం den = b + c;
    రాయి("%f\n", అంకె/den); 
}

దశాంశం జాలం_accuracy(జాలం *net, దత్తాంశం d)
{
    మాతృక guess = జాలం_predict_దత్తాంశం(net, d);
    దశాంశం acc = మాతృక_topk_accuracy(d.y, guess,1);
    free_మాతృక(guess);
    తిరిగి acc;
}

దశాంశం *జాలం_accuracies(జాలం *net, దత్తాంశం d, పూర్ణాంకం n)
{
    స్థిర దశాంశం acc[2];
    మాతృక guess = జాలం_predict_దత్తాంశం(net, d);
    acc[0] = మాతృక_topk_accuracy(d.y, guess, 1);
    acc[1] = మాతృక_topk_accuracy(d.y, guess, n);
    free_మాతృక(guess);
    తిరిగి acc;
}

layer get_జాలం_output_layer(జాలం *net)
{
    పూర్ణాంకం i;
    కొరకు(i = net->n - 1; i >= 0; --i){
        ఒకవేళ(net->layers[i].type != ఖర్చు) ఆపు;
    }
    తిరిగి net->layers[i];
}

దశాంశం జాలం_accuracy_multi(జాలం *net, దత్తాంశం d, పూర్ణాంకం n)
{
    మాతృక guess = జాలం_predict_దత్తాంశం_multi(net, d, n);
    దశాంశం acc = మాతృక_topk_accuracy(d.y, guess,1);
    free_మాతృక(guess);
    తిరిగి acc;
}

శూన్య free_జాలం(జాలం *net)
{
    పూర్ణాంకం i;
    కొరకు(i = 0; i < net->n; ++i){
        free_layer(net->layers[i]);
    }
    విడుదల(net->layers);
    ఒకవేళ(net->ఆదానం) విడుదల(net->ఆదానం);
    ఒకవేళ(net->truth) విడుదల(net->truth);
#నిర్వచితమైతే GPU
    ఒకవేళ(net->ఆదానం_gpu) cuda_free(net->ఆదానం_gpu);
    ఒకవేళ(net->truth_gpu) cuda_free(net->truth_gpu);
#అయితే_ముగింపు
    విడుదల(net);
}

// Some day...
// ^ What the hell is this comment for?


layer జాలం_output_layer(జాలం *net)
{
    పూర్ణాంకం i;
    కొరకు(i = net->n - 1; i >= 0; --i){
        ఒకవేళ(net->layers[i].type != ఖర్చు) ఆపు;
    }
    తిరిగి net->layers[i];
}

పూర్ణాంకం జాలం_inputs(జాలం *net)
{
    తిరిగి net->layers[0].inputs;
}

పూర్ణాంకం జాలం_outputs(జాలం *net)
{
    తిరిగి జాలం_output_layer(net).outputs;
}

దశాంశం *జాలం_output(జాలం *net)
{
    తిరిగి జాలం_output_layer(net).output;
}

#నిర్వచితమైతే GPU

శూన్య forward_జాలం_gpu(జాలం *netp)
{
    జాలం net = *netp;
    cuda_సమితి_device(net.gpu_సూచీ);
    cuda_నెట్టు_array(net.ఆదానం_gpu, net.ఆదానం, net.inputs*net.batch);
    ఒకవేళ(net.truth){
        cuda_నెట్టు_array(net.truth_gpu, net.truth, net.truths*net.batch);
    }

    పూర్ణాంకం i;
    కొరకు(i = 0; i < net.n; ++i){
        net.సూచీ = i;
        layer l = net.layers[i];
        ఒకవేళ(l.delta_gpu){
            fill_gpu(l.outputs * l.batch, 0, l.delta_gpu, 1);
        }
        l.forward_gpu(l, net);
        net.ఆదానం_gpu = l.output_gpu;
        net.ఆదానం = l.output;
        ఒకవేళ(l.truth) {
            net.truth_gpu = l.output_gpu;
            net.truth = l.output;
        }
    }
    pull_జాలం_output(netp);
    calc_జాలం_ఖర్చు(netp);
}

శూన్య backward_జాలం_gpu(జాలం *netp)
{
    పూర్ణాంకం i;
    జాలం net = *netp;
    జాలం orig = net;
    cuda_సమితి_device(net.gpu_సూచీ);
    కొరకు(i = net.n-1; i >= 0; --i){
        layer l = net.layers[i];
        ఒకవేళ(l.stopbackward) ఆపు;
        ఒకవేళ(i == 0){
            net = orig;
        }లేకపోతే{
            layer prev = net.layers[i-1];
            net.ఆదానం = prev.output;
            net.delta = prev.delta;
            net.ఆదానం_gpu = prev.output_gpu;
            net.delta_gpu = prev.delta_gpu;
        }
        net.సూచీ = i;
        l.backward_gpu(l, net);
    }
}

శూన్య update_జాలం_gpu(జాలం *netp)
{
    జాలం net = *netp;
    cuda_సమితి_device(net.gpu_సూచీ);
    పూర్ణాంకం i;
    update_వాదనలు a = {0};
    a.batch = net.batch*net.subdivisions;
    a.learning_రేటు = get_current_రేటు(netp);
    a.momentum = net.momentum;
    a.decay = net.decay;
    a.adam = net.adam;
    a.B1 = net.B1;
    a.B2 = net.B2;
    a.eps = net.eps;
    ++*net.t;
    a.t = (*net.t);

    కొరకు(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        ఒకవేళ(l.update_gpu){
            l.update_gpu(l, a);
        }
    }
}

శూన్య harmless_update_జాలం_gpu(జాలం *netp)
{
    జాలం net = *netp;
    cuda_సమితి_device(net.gpu_సూచీ);
    పూర్ణాంకం i;
    కొరకు(i = 0; i < net.n; ++i){
        layer l = net.layers[i];
        ఒకవేళ(l.బరువు_updates_gpu) fill_gpu(l.nweights, 0, l.బరువు_updates_gpu, 1);
        ఒకవేళ(l.bias_updates_gpu) fill_gpu(l.nbiases, 0, l.bias_updates_gpu, 1);
        ఒకవేళ(l.scale_updates_gpu) fill_gpu(l.nbiases, 0, l.scale_updates_gpu, 1);
    }
}

రకనిర్వచనం నిర్మాణం {
    జాలం *net;
    దత్తాంశం d;
    దశాంశం *err;
} train_వాదనలు;

శూన్య *train_thread(శూన్య *ptr)
{
    train_వాదనలు వాదనలు = *(train_వాదనలు*)ptr;
    విడుదల(ptr);
    cuda_సమితి_device(వాదనలు.net->gpu_సూచీ);
    *వాదనలు.err = train_జాలం(వాదనలు.net, వాదనలు.d);
    తిరిగి 0;
}

pthread_t train_జాలం_లోపల_thread(జాలం *net, దత్తాంశం d, దశాంశం *err)
{
    pthread_t thread;
    train_వాదనలు *ptr = (train_వాదనలు *)సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(train_వాదనలు));
    ptr->net = net;
    ptr->d = d;
    ptr->err = err;
    ఒకవేళ(pthread_create(&thread, 0, train_thread, ptr)) error("Thread creation failed");
    తిరిగి thread;
}

శూన్య విలీనం_weights(layer l, layer పునాది)
{
    ఒకవేళ (l.type == CONVOLUTIONAL) {
        axpy_cpu(l.n, 1, l.bias_updates, 1, పునాది.biases, 1);
        axpy_cpu(l.nweights, 1, l.బరువు_updates, 1, పునాది.weights, 1);
        ఒకవేళ (l.scales) {
            axpy_cpu(l.n, 1, l.scale_updates, 1, పునాది.scales, 1);
        }
    } లేకపోతే ఒకవేళ(l.type == CONNECTED) {
        axpy_cpu(l.outputs, 1, l.bias_updates, 1, పునాది.biases, 1);
        axpy_cpu(l.outputs*l.inputs, 1, l.బరువు_updates, 1, పునాది.weights, 1);
    }
}

శూన్య scale_weights(layer l, దశాంశం s)
{
    ఒకవేళ (l.type == CONVOLUTIONAL) {
        scal_cpu(l.n, s, l.biases, 1);
        scal_cpu(l.nweights, s, l.weights, 1);
        ఒకవేళ (l.scales) {
            scal_cpu(l.n, s, l.scales, 1);
        }
    } లేకపోతే ఒకవేళ(l.type == CONNECTED) {
        scal_cpu(l.outputs, s, l.biases, 1);
        scal_cpu(l.outputs*l.inputs, s, l.weights, 1);
    }
}


శూన్య pull_weights(layer l)
{
    ఒకవేళ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.n);
        cuda_pull_array(l.weights_gpu, l.బరువు_updates, l.nweights);
        ఒకవేళ(l.scales) cuda_pull_array(l.scales_gpu, l.scale_updates, l.n);
    } లేకపోతే ఒకవేళ(l.type == CONNECTED){
        cuda_pull_array(l.biases_gpu, l.bias_updates, l.outputs);
        cuda_pull_array(l.weights_gpu, l.బరువు_updates, l.outputs*l.inputs);
    }
}

శూన్య నెట్టు_weights(layer l)
{
    ఒకవేళ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
        cuda_నెట్టు_array(l.biases_gpu, l.biases, l.n);
        cuda_నెట్టు_array(l.weights_gpu, l.weights, l.nweights);
        ఒకవేళ(l.scales) cuda_నెట్టు_array(l.scales_gpu, l.scales, l.n);
    } లేకపోతే ఒకవేళ(l.type == CONNECTED){
        cuda_నెట్టు_array(l.biases_gpu, l.biases, l.outputs);
        cuda_నెట్టు_array(l.weights_gpu, l.weights, l.outputs*l.inputs);
    }
}

శూన్య distribute_weights(layer l, layer పునాది)
{
    ఒకవేళ (l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL) {
        cuda_నెట్టు_array(l.biases_gpu, పునాది.biases, l.n);
        cuda_నెట్టు_array(l.weights_gpu, పునాది.weights, l.nweights);
        ఒకవేళ (పునాది.scales) cuda_నెట్టు_array(l.scales_gpu, పునాది.scales, l.n);
    } లేకపోతే ఒకవేళ (l.type == CONNECTED) {
        cuda_నెట్టు_array(l.biases_gpu, పునాది.biases, l.outputs);
        cuda_నెట్టు_array(l.weights_gpu, పునాది.weights, l.outputs*l.inputs);
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

శూన్య sync_layer(జాలం **nets, పూర్ణాంకం n, పూర్ణాంకం j)
{
    పూర్ణాంకం i;
    జాలం *net = nets[0];
    layer పునాది = net->layers[j];
    scale_weights(పునాది, 0);
    కొరకు (i = 0; i < n; ++i) {
        cuda_సమితి_device(nets[i]->gpu_సూచీ);
        layer l = nets[i]->layers[j];
        pull_weights(l);
        విలీనం_weights(l, పునాది);
    }
    scale_weights(పునాది, 1./n);
    కొరకు (i = 0; i < n; ++i) {
        cuda_సమితి_device(nets[i]->gpu_సూచీ);
        layer l = nets[i]->layers[j];
        distribute_weights(l, పునాది);
    }
}

రకనిర్వచనం నిర్మాణం{
    జాలం **nets;
    పూర్ణాంకం n;
    పూర్ణాంకం j;
} sync_వాదనలు;

శూన్య *sync_layer_thread(శూన్య *ptr)
{
    sync_వాదనలు వాదనలు = *(sync_వాదనలు*)ptr;
    sync_layer(వాదనలు.nets, వాదనలు.n, వాదనలు.j);
    విడుదల(ptr);
    తిరిగి 0;
}

pthread_t sync_layer_లోపల_thread(జాలం **nets, పూర్ణాంకం n, పూర్ణాంకం j)
{
    pthread_t thread;
    sync_వాదనలు *ptr = (sync_వాదనలు *)సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(sync_వాదనలు));
    ptr->nets = nets;
    ptr->n = n;
    ptr->j = j;
    ఒకవేళ(pthread_create(&thread, 0, sync_layer_thread, ptr)) error("Thread creation failed");
    తిరిగి thread;
}

శూన్య sync_nets(జాలం **nets, పూర్ణాంకం n, పూర్ణాంకం interval)
{
    పూర్ణాంకం j;
    పూర్ణాంకం layers = nets[0]->n;
    pthread_t *దారాలు = (pthread_t *) సున్నా_జ్ఞాపకం_ఇవ్వు(layers, పరిమాణం(pthread_t));

    *(nets[0]->చూసిన) += interval * (n-1) * nets[0]->batch * nets[0]->subdivisions;
    కొరకు (j = 0; j < n; ++j){
        *(nets[j]->చూసిన) = *(nets[0]->చూసిన);
    }
    కొరకు (j = 0; j < layers; ++j) {
        దారాలు[j] = sync_layer_లోపల_thread(nets, n, j);
    }
    కొరకు (j = 0; j < layers; ++j) {
        pthread_join(దారాలు[j], 0);
    }
    విడుదల(దారాలు);
}

దశాంశం train_networks(జాలం **nets, పూర్ణాంకం n, దత్తాంశం d, పూర్ణాంకం interval)
{
    పూర్ణాంకం i;
    పూర్ణాంకం batch = nets[0]->batch;
    పూర్ణాంకం subdivisions = nets[0]->subdivisions;
    ప్రతిజ్ఞా(batch * subdivisions * n == d.X.అడ్డువరుసలు);
    pthread_t *దారాలు = (pthread_t *) సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(pthread_t));
    దశాంశం *errors = (దశాంశం *) సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం));

    దశాంశం కూడిక = 0;
    కొరకు(i = 0; i < n; ++i){
        దత్తాంశం p = get_దత్తాంశం_part(d, i, n);
        దారాలు[i] = train_జాలం_లోపల_thread(nets[i], p, errors + i);
    }
    కొరకు(i = 0; i < n; ++i){
        pthread_join(దారాలు[i], 0);
        //printf("%f\n", errors[i]);
        కూడిక += errors[i];
    }
    //cudaDeviceSynchronize();
    ఒకవేళ (get_current_batch(nets[0]) % interval == 0) {
        రాయి("Syncing... ");
        fflush(stdout);
        sync_nets(nets, n, interval);
        రాయి("Done!\n");
    }
    //cudaDeviceSynchronize();
    విడుదల(దారాలు);
    విడుదల(errors);
    తిరిగి (దశాంశం)కూడిక/(n);
}

శూన్య pull_జాలం_output(జాలం *net)
{
    layer l = get_జాలం_output_layer(net);
    cuda_pull_array(l.output_gpu, l.output, l.outputs*l.batch);
}

#అయితే_ముగింపు

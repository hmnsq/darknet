#అన్తర్భావ "yolo_layer.h"
#అన్తర్భావ "activations.h"
#అన్తర్భావ "blas.h"
#అన్తర్భావ "box.h"
#అన్తర్భావ "cuda.h"
#అన్తర్భావ "utils.h"

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <assert.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <stdlib.h>

layer make_yolo_layer(పూర్ణాంకం batch, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం n, పూర్ణాంకం మొత్తం, పూర్ణాంకం *ముసుగు, పూర్ణాంకం classes)
{
    పూర్ణాంకం i;
    layer l = {0};
    l.type = YOLO;

    l.n = n;
    l.మొత్తం = మొత్తం;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = n*(classes + 4 + 1);
    l.బహిర్గతం_w = l.w;
    l.బహిర్గతం_h = l.h;
    l.బహిర్గతం_c = l.c;
    l.classes = classes;
    l.ఖర్చు_2 = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(దశాంశం));
    l.biases = సున్నా_జ్ఞాపకం_ఇవ్వు(మొత్తం*2, పరిమాణం(దశాంశం));
    ఒకవేళ(ముసుగు) l.ముసుగు = ముసుగు;
    లేకపోతే{
        l.ముసుగు = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(పూర్ణాంకం));
        కొరకు(i = 0; i < n; ++i){
            l.ముసుగు[i] = i;
        }
    }
    l.bias_updates = సున్నా_జ్ఞాపకం_ఇవ్వు(n*2, పరిమాణం(దశాంశం));
    l.outputs = h*w*n*(classes + 4 + 1);
    l.inputs = l.outputs;
    l.truths = 90*(4 + 1);
    l.delta = సున్నా_జ్ఞాపకం_ఇవ్వు(batch*l.outputs, పరిమాణం(దశాంశం));
    l.output = సున్నా_జ్ఞాపకం_ఇవ్వు(batch*l.outputs, పరిమాణం(దశాంశం));
    కొరకు(i = 0; i < మొత్తం*2; ++i){
        l.biases[i] = .5;
    }

    l.forward = forward_yolo_layer;
    l.backward = backward_yolo_layer;
#నిర్వచితమైతే GPU
    l.forward_gpu = forward_yolo_layer_gpu;
    l.backward_gpu = backward_yolo_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#అయితే_ముగింపు

    fprintf(stderr, "yolo\n");
    srand(0);

    తిరిగి l;
}

శూన్య resize_yolo_layer(layer *l, పూర్ణాంకం w, పూర్ణాంకం h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + 4 + 1);
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*పరిమాణం(దశాంశం));
    l->delta = realloc(l->delta, l->batch*l->outputs*పరిమాణం(దశాంశం));

#నిర్వచితమైతే GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#అయితే_ముగింపు
}

box get_yolo_box(దశాంశం *x, దశాంశం *biases, పూర్ణాంకం n, పూర్ణాంకం సూచీ, పూర్ణాంకం i, పూర్ణాంకం j, పూర్ణాంకం lw, పూర్ణాంకం lh, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం stride)
{
    box b;
    b.x = (i + x[సూచీ + 0*stride]) / lw;
    b.y = (j + x[సూచీ + 1*stride]) / lh;
    b.w = ఘాతాంకం(x[సూచీ + 2*stride]) * biases[2*n]   / w;
    b.h = ఘాతాంకం(x[సూచీ + 3*stride]) * biases[2*n+1] / h;
    తిరిగి b;
}

దశాంశం delta_yolo_box(box truth, దశాంశం *x, దశాంశం *biases, పూర్ణాంకం n, పూర్ణాంకం సూచీ, పూర్ణాంకం i, పూర్ణాంకం j, పూర్ణాంకం lw, పూర్ణాంకం lh, పూర్ణాంకం w, పూర్ణాంకం h, దశాంశం *delta, దశాంశం scale, పూర్ణాంకం stride)
{
    box pred = get_yolo_box(x, biases, n, సూచీ, i, j, lw, lh, w, h, stride);
    దశాంశం iou = box_iou(pred, truth);

    దశాంశం tx = (truth.x*lw - i);
    దశాంశం ty = (truth.y*lh - j);
    దశాంశం tw = సంవర్గమానం(truth.w*w / biases[2*n]);
    దశాంశం th = సంవర్గమానం(truth.h*h / biases[2*n + 1]);

    delta[సూచీ + 0*stride] = scale * (tx - x[సూచీ + 0*stride]);
    delta[సూచీ + 1*stride] = scale * (ty - x[సూచీ + 1*stride]);
    delta[సూచీ + 2*stride] = scale * (tw - x[సూచీ + 2*stride]);
    delta[సూచీ + 3*stride] = scale * (th - x[సూచీ + 3*stride]);
    తిరిగి iou;
}


శూన్య delta_yolo_class(దశాంశం *output, దశాంశం *delta, పూర్ణాంకం సూచీ, పూర్ణాంకం class, పూర్ణాంకం classes, పూర్ణాంకం stride, దశాంశం *avg_cat)
{
    పూర్ణాంకం n;
    ఒకవేళ (delta[సూచీ]){
        delta[సూచీ + stride*class] = 1 - output[సూచీ + stride*class];
        ఒకవేళ(avg_cat) *avg_cat += output[సూచీ + stride*class];
        తిరిగి;
    }
    కొరకు(n = 0; n < classes; ++n){
        delta[సూచీ + stride*n] = ((n == class)?1 : 0) - output[సూచీ + stride*n];
        ఒకవేళ(n == class && avg_cat) *avg_cat += output[సూచీ + stride*n];
    }
}

స్థిర పూర్ణాంకం నమోదు_సూచీ(layer l, పూర్ణాంకం batch, పూర్ణాంకం location, పూర్ణాంకం నమోదు_2)
{
    పూర్ణాంకం n =   location / (l.w*l.h);
    పూర్ణాంకం loc = location % (l.w*l.h);
    తిరిగి batch*l.outputs + n*l.w*l.h*(4+l.classes+1) + నమోదు_2*l.w*l.h + loc;
}

శూన్య forward_yolo_layer(స్థిరరాశి layer l, జాలం net)
{
    పూర్ణాంకం i,j,b,t,n;
    జ్ఞాపకం_నకలు(l.output, net.ఆదానం, l.outputs*l.batch*పరిమాణం(దశాంశం));

#నిర్వచితం_కాకపోతే GPU
    కొరకు (b = 0; b < l.batch; ++b){
        కొరకు(n = 0; n < l.n; ++n){
            పూర్ణాంకం సూచీ = నమోదు_సూచీ(l, b, n*l.w*l.h, 0);
            activate_array(l.output + సూచీ, 2*l.w*l.h, LOGISTIC);
            సూచీ = నమోదు_సూచీ(l, b, n*l.w*l.h, 4);
            activate_array(l.output + సూచీ, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
#అయితే_ముగింపు

    memset(l.delta, 0, l.outputs * l.batch * పరిమాణం(దశాంశం));
    ఒకవేళ(!net.train) తిరిగి;
    దశాంశం avg_iou = 0;
    దశాంశం recall = 0;
    దశాంశం recall75 = 0;
    దశాంశం avg_cat = 0;
    దశాంశం avg_obj = 0;
    దశాంశం avg_anyobj = 0;
    పూర్ణాంకం లెక్క = 0;
    పూర్ణాంకం class_లెక్క = 0;
    *(l.ఖర్చు_2) = 0;
    కొరకు (b = 0; b < l.batch; ++b) {
        కొరకు (j = 0; j < l.h; ++j) {
            కొరకు (i = 0; i < l.w; ++i) {
                కొరకు (n = 0; n < l.n; ++n) {
                    పూర్ణాంకం box_సూచీ = నమోదు_సూచీ(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_yolo_box(l.output, l.biases, l.ముసుగు[n], box_సూచీ, i, j, l.w, l.h, net.w, net.h, l.w*l.h);
                    దశాంశం ఉత్తమ_iou = 0;
                    పూర్ణాంకం ఉత్తమ_t = 0;
                    కొరకు(t = 0; t < l.max_boxes; ++t){
                        box truth = float_గమ్యం_box(net.truth + t*(4 + 1) + b*l.truths, 1);
                        ఒకవేళ(!truth.x) ఆపు;
                        దశాంశం iou = box_iou(pred, truth);
                        ఒకవేళ (iou > ఉత్తమ_iou) {
                            ఉత్తమ_iou = iou;
                            ఉత్తమ_t = t;
                        }
                    }
                    పూర్ణాంకం obj_సూచీ = నమోదు_సూచీ(l, b, n*l.w*l.h + j*l.w + i, 4);
                    avg_anyobj += l.output[obj_సూచీ];
                    l.delta[obj_సూచీ] = 0 - l.output[obj_సూచీ];
                    ఒకవేళ (ఉత్తమ_iou > l.ignore_thresh) {
                        l.delta[obj_సూచీ] = 0;
                    }
                    ఒకవేళ (ఉత్తమ_iou > l.truth_thresh) {
                        l.delta[obj_సూచీ] = 1 - l.output[obj_సూచీ];

                        పూర్ణాంకం class = net.truth[ఉత్తమ_t*(4 + 1) + b*l.truths + 4];
                        ఒకవేళ (l.map) class = l.map[class];
                        పూర్ణాంకం class_సూచీ = నమోదు_సూచీ(l, b, n*l.w*l.h + j*l.w + i, 4 + 1);
                        delta_yolo_class(l.output, l.delta, class_సూచీ, class, l.classes, l.w*l.h, 0);
                        box truth = float_గమ్యం_box(net.truth + ఉత్తమ_t*(4 + 1) + b*l.truths, 1);
                        delta_yolo_box(truth, l.output, l.biases, l.ముసుగు[n], box_సూచీ, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);
                    }
                }
            }
        }
        కొరకు(t = 0; t < l.max_boxes; ++t){
            box truth = float_గమ్యం_box(net.truth + t*(4 + 1) + b*l.truths, 1);

            ఒకవేళ(!truth.x) ఆపు;
            దశాంశం ఉత్తమ_iou = 0;
            పూర్ణాంకం ఉత్తమ_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_జరుపు = truth;
            truth_జరుపు.x = truth_జరుపు.y = 0;
            కొరకు(n = 0; n < l.మొత్తం; ++n){
                box pred = {0};
                pred.w = l.biases[2*n]/net.w;
                pred.h = l.biases[2*n+1]/net.h;
                దశాంశం iou = box_iou(pred, truth_జరుపు);
                ఒకవేళ (iou > ఉత్తమ_iou){
                    ఉత్తమ_iou = iou;
                    ఉత్తమ_n = n;
                }
            }

            పూర్ణాంకం ముసుగు_n = int_సూచీ(l.ముసుగు, ఉత్తమ_n, l.n);
            ఒకవేళ(ముసుగు_n >= 0){
                పూర్ణాంకం box_సూచీ = నమోదు_సూచీ(l, b, ముసుగు_n*l.w*l.h + j*l.w + i, 0);
                దశాంశం iou = delta_yolo_box(truth, l.output, l.biases, ఉత్తమ_n, box_సూచీ, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);

                పూర్ణాంకం obj_సూచీ = నమోదు_సూచీ(l, b, ముసుగు_n*l.w*l.h + j*l.w + i, 4);
                avg_obj += l.output[obj_సూచీ];
                l.delta[obj_సూచీ] = 1 - l.output[obj_సూచీ];

                పూర్ణాంకం class = net.truth[t*(4 + 1) + b*l.truths + 4];
                ఒకవేళ (l.map) class = l.map[class];
                పూర్ణాంకం class_సూచీ = నమోదు_సూచీ(l, b, ముసుగు_n*l.w*l.h + j*l.w + i, 4 + 1);
                delta_yolo_class(l.output, l.delta, class_సూచీ, class, l.classes, l.w*l.h, &avg_cat);

                ++లెక్క;
                ++class_లెక్క;
                ఒకవేళ(iou > .5) recall += 1;
                ఒకవేళ(iou > .75) recall75 += 1;
                avg_iou += iou;
            }
        }
    }
    *(l.ఖర్చు_2) = ఘాతం(mag_array(l.delta, l.outputs * l.batch), 2);
    రాయి("Region %d Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, .5R: %f, .75R: %f,  count: %d\n", net.సూచీ, avg_iou/లెక్క, avg_cat/class_లెక్క, avg_obj/లెక్క, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/లెక్క, recall75/లెక్క, లెక్క);
}

శూన్య backward_yolo_layer(స్థిరరాశి layer l, జాలం net)
{
   axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

శూన్య correct_yolo_boxes(detection *dets, పూర్ణాంకం n, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం netw, పూర్ణాంకం neth, పూర్ణాంకం relative)
{
    పూర్ణాంకం i;
    పూర్ణాంకం new_w=0;
    పూర్ణాంకం new_h=0;
    ఒకవేళ (((దశాంశం)netw/w) < ((దశాంశం)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } లేకపోతే {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    కొరకు (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((దశాంశం)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((దశాంశం)new_h/neth); 
        b.w *= (దశాంశం)netw/new_w;
        b.h *= (దశాంశం)neth/new_h;
        ఒకవేళ(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

పూర్ణాంకం yolo_అంకె_detections(layer l, దశాంశం thresh)
{
    పూర్ణాంకం i, n;
    పూర్ణాంకం లెక్క = 0;
    కొరకు (i = 0; i < l.w*l.h; ++i){
        కొరకు(n = 0; n < l.n; ++n){
            పూర్ణాంకం obj_సూచీ  = నమోదు_సూచీ(l, 0, n*l.w*l.h + i, 4);
            ఒకవేళ(l.output[obj_సూచీ] > thresh){
                ++లెక్క;
            }
        }
    }
    తిరిగి లెక్క;
}

శూన్య avg_flipped_yolo(layer l)
{
    పూర్ణాంకం i,j,n,z;
    దశాంశం *flip = l.output + l.outputs;
    కొరకు (j = 0; j < l.h; ++j) {
        కొరకు (i = 0; i < l.w/2; ++i) {
            కొరకు (n = 0; n < l.n; ++n) {
                కొరకు(z = 0; z < l.classes + 4 + 1; ++z){
                    పూర్ణాంకం i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                    పూర్ణాంకం i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                    దశాంశం మార్పిడి = flip[i1];
                    flip[i1] = flip[i2];
                    flip[i2] = మార్పిడి;
                    ఒకవేళ(z == 0){
                        flip[i1] = -flip[i1];
                        flip[i2] = -flip[i2];
                    }
                }
            }
        }
    }
    కొరకు(i = 0; i < l.outputs; ++i){
        l.output[i] = (l.output[i] + flip[i])/2.;
    }
}

పూర్ణాంకం get_yolo_detections(layer l, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం netw, పూర్ణాంకం neth, దశాంశం thresh, పూర్ణాంకం *map, పూర్ణాంకం relative, detection *dets)
{
    పూర్ణాంకం i,j,n;
    దశాంశం *predictions = l.output;
    ఒకవేళ (l.batch == 2) avg_flipped_yolo(l);
    పూర్ణాంకం లెక్క = 0;
    కొరకు (i = 0; i < l.w*l.h; ++i){
        పూర్ణాంకం అడ్డువరుస = i / l.w;
        పూర్ణాంకం నిలువు = i % l.w;
        కొరకు(n = 0; n < l.n; ++n){
            పూర్ణాంకం obj_సూచీ  = నమోదు_సూచీ(l, 0, n*l.w*l.h + i, 4);
            దశాంశం objectness = predictions[obj_సూచీ];
            ఒకవేళ(objectness <= thresh) కొనసాగు;
            పూర్ణాంకం box_సూచీ  = నమోదు_సూచీ(l, 0, n*l.w*l.h + i, 0);
            dets[లెక్క].bbox = get_yolo_box(predictions, l.biases, l.ముసుగు[n], box_సూచీ, నిలువు, అడ్డువరుస, l.w, l.h, netw, neth, l.w*l.h);
            dets[లెక్క].objectness = objectness;
            dets[లెక్క].classes = l.classes;
            కొరకు(j = 0; j < l.classes; ++j){
                పూర్ణాంకం class_సూచీ = నమోదు_సూచీ(l, 0, n*l.w*l.h + i, 4 + 1 + j);
                దశాంశం prob = objectness*predictions[class_సూచీ];
                dets[లెక్క].prob[j] = (prob > thresh) ? prob : 0;
            }
            ++లెక్క;
        }
    }
    correct_yolo_boxes(dets, లెక్క, w, h, netw, neth, relative);
    తిరిగి లెక్క;
}

#నిర్వచితమైతే GPU

శూన్య forward_yolo_layer_gpu(స్థిరరాశి layer l, జాలం net)
{
    copy_gpu(l.batch*l.inputs, net.ఆదానం_gpu, 1, l.output_gpu, 1);
    పూర్ణాంకం b, n;
    కొరకు (b = 0; b < l.batch; ++b){
        కొరకు(n = 0; n < l.n; ++n){
            పూర్ణాంకం సూచీ = నమోదు_సూచీ(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + సూచీ, 2*l.w*l.h, LOGISTIC);
            సూచీ = నమోదు_సూచీ(l, b, n*l.w*l.h, 4);
            activate_array_gpu(l.output_gpu + సూచీ, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
    ఒకవేళ(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        తిరిగి;
    }

    cuda_pull_array(l.output_gpu, net.ఆదానం, l.batch*l.inputs);
    forward_yolo_layer(l, net);
    cuda_నెట్టు_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

శూన్య backward_yolo_layer_gpu(స్థిరరాశి layer l, జాలం net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#అయితే_ముగింపు


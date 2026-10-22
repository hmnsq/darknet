#ضمن "detection_layer.h"
#ضمن "activations.h"
#ضمن "softmax_layer.h"
#ضمن "blas.h"
#ضمن "box.h"
#ضمن "cuda.h"
#ضمن "utils.h"

#ضمن <stdio.h>
#ضمن <assert.h>
#ضمن <string.h>
#ضمن <stdlib.h>

detection_layer make_detection_layer(صحيح batch, صحيح inputs, صحيح n, صحيح side, صحيح classes, صحيح coords, صحيح rescore)
{
    detection_layer l = {0};
    l.type = DETECTION;

    l.n = n;
    l.batch = batch;
    l.inputs = inputs;
    l.classes = classes;
    l.coords = coords;
    l.rescore = rescore;
    l.side = side;
    l.w = side;
    l.h = side;
    أكد(side*side*((1 + l.coords)*l.n + l.classes) == inputs);
    l.لګښت_2 = حافظه_صفر_ورکړه(1, حجم(عائم));
    l.outputs = l.inputs;
    l.truths = l.side*l.side*(1+l.coords+l.classes);
    l.output = حافظه_صفر_ورکړه(batch*l.outputs, حجم(عائم));
    l.delta = حافظه_صفر_ورکړه(batch*l.outputs, حجم(عائم));

    l.forward = forward_detection_layer;
    l.backward = backward_detection_layer;
#که_تعریف_شوی GPU
    l.forward_gpu = forward_detection_layer_gpu;
    l.backward_gpu = backward_detection_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#د_که_پای

    fprintf(stderr, "Detection Layer\n");
    srand(0);

    أرجع l;
}

فراغ forward_detection_layer(ثابت detection_layer l, شبکه net)
{
    صحيح locations = l.side*l.side;
    صحيح i,j;
    حافظه_نقل(l.output, net.ننوت, l.outputs*l.batch*حجم(عائم));
    //if(l.reorg) reorg(l.output, l.w*l.h, size*l.n, l.batch, 1);
    صحيح b;
    إذا (l.softmax){
        لكل(b = 0; b < l.batch; ++b){
            صحيح شاخص = b*l.inputs;
            لكل (i = 0; i < locations; ++i) {
                صحيح دننه_واټن = i*l.classes;
                softmax(l.output + شاخص + دننه_واټن, l.classes, 1, 1,
                        l.output + شاخص + دننه_واټن);
            }
        }
    }
    إذا(net.train){
        عائم avg_iou = 0;
        عائم avg_cat = 0;
        عائم avg_allcat = 0;
        عائم avg_obj = 0;
        عائم avg_anyobj = 0;
        صحيح شمېر = 0;
        *(l.لګښت_2) = 0;
        صحيح اندازه = l.inputs * l.batch;
        memset(l.delta, 0, اندازه * حجم(عائم));
        لكل (b = 0; b < l.batch; ++b){
            صحيح شاخص = b*l.inputs;
            لكل (i = 0; i < locations; ++i) {
                صحيح truth_شاخص = (b*locations + i)*(1+l.coords+l.classes);
                صحيح is_obj = net.truth[truth_شاخص];
                لكل (j = 0; j < l.n; ++j) {
                    صحيح p_شاخص = شاخص + locations*l.classes + i*l.n + j;
                    l.delta[p_شاخص] = l.noobject_scale*(0 - l.output[p_شاخص]);
                    *(l.لګښت_2) += l.noobject_scale*توان(l.output[p_شاخص], 2);
                    avg_anyobj += l.output[p_شاخص];
                }

                صحيح غوره_شاخص = -1;
                عائم غوره_iou = 0;
                عائم غوره_rmse = 20;

                إذا (!is_obj){
                    استمر;
                }

                صحيح class_شاخص = شاخص + i*l.classes;
                لكل(j = 0; j < l.classes; ++j) {
                    l.delta[class_شاخص+j] = l.class_scale * (net.truth[truth_شاخص+1+j] - l.output[class_شاخص+j]);
                    *(l.لګښت_2) += l.class_scale * توان(net.truth[truth_شاخص+1+j] - l.output[class_شاخص+j], 2);
                    إذا(net.truth[truth_شاخص + 1 + j]) avg_cat += l.output[class_شاخص+j];
                    avg_allcat += l.output[class_شاخص+j];
                }

                box truth = float_منزل_box(net.truth + truth_شاخص + 1 + l.classes, 1);
                truth.x /= l.side;
                truth.y /= l.side;

                لكل(j = 0; j < l.n; ++j){
                    صحيح box_شاخص = شاخص + locations*(l.classes + l.n) + (i*l.n + j) * l.coords;
                    box بهر = float_منزل_box(l.output + box_شاخص, 1);
                    بهر.x /= l.side;
                    بهر.y /= l.side;

                    إذا (l.جذر){
                        بهر.w = بهر.w*بهر.w;
                        بهر.h = بهر.h*بهر.h;
                    }

                    عائم iou  = box_iou(بهر, truth);
                    //iou = 0;
                    عائم rmse = box_rmse(بهر, truth);
                    إذا(غوره_iou > 0 || iou > 0){
                        إذا(iou > غوره_iou){
                            غوره_iou = iou;
                            غوره_شاخص = j;
                        }
                    }وإلا{
                        إذا(rmse < غوره_rmse){
                            غوره_rmse = rmse;
                            غوره_شاخص = j;
                        }
                    }
                }

                إذا(l.forced){
                    إذا(truth.w*truth.h < .1){
                        غوره_شاخص = 1;
                    }وإلا{
                        غوره_شاخص = 0;
                    }
                }
                إذا(l.random && *(net.کتل_شوی) < 64000){
                    غوره_شاخص = rand()%l.n;
                }

                صحيح box_شاخص = شاخص + locations*(l.classes + l.n) + (i*l.n + غوره_شاخص) * l.coords;
                صحيح tbox_شاخص = truth_شاخص + 1 + l.classes;

                box بهر = float_منزل_box(l.output + box_شاخص, 1);
                بهر.x /= l.side;
                بهر.y /= l.side;
                إذا (l.جذر) {
                    بهر.w = بهر.w*بهر.w;
                    بهر.h = بهر.h*بهر.h;
                }
                عائم iou  = box_iou(بهر, truth);

                //printf("%d,", best_index);
                صحيح p_شاخص = شاخص + locations*l.classes + i*l.n + غوره_شاخص;
                *(l.لګښت_2) -= l.noobject_scale * توان(l.output[p_شاخص], 2);
                *(l.لګښت_2) += l.object_scale * توان(1-l.output[p_شاخص], 2);
                avg_obj += l.output[p_شاخص];
                l.delta[p_شاخص] = l.object_scale * (1.-l.output[p_شاخص]);

                إذا(l.rescore){
                    l.delta[p_شاخص] = l.object_scale * (iou - l.output[p_شاخص]);
                }

                l.delta[box_شاخص+0] = l.coord_scale*(net.truth[tbox_شاخص + 0] - l.output[box_شاخص + 0]);
                l.delta[box_شاخص+1] = l.coord_scale*(net.truth[tbox_شاخص + 1] - l.output[box_شاخص + 1]);
                l.delta[box_شاخص+2] = l.coord_scale*(net.truth[tbox_شاخص + 2] - l.output[box_شاخص + 2]);
                l.delta[box_شاخص+3] = l.coord_scale*(net.truth[tbox_شاخص + 3] - l.output[box_شاخص + 3]);
                إذا(l.جذر){
                    l.delta[box_شاخص+2] = l.coord_scale*(جذر(net.truth[tbox_شاخص + 2]) - l.output[box_شاخص + 2]);
                    l.delta[box_شاخص+3] = l.coord_scale*(جذر(net.truth[tbox_شاخص + 3]) - l.output[box_شاخص + 3]);
                }

                *(l.لګښت_2) += توان(1-iou, 2);
                avg_iou += iou;
                ++شمېر;
            }
        }

        إذا(0){
            عائم *costs = حافظه_صفر_ورکړه(l.batch*locations*l.n, حجم(عائم));
            لكل (b = 0; b < l.batch; ++b) {
                صحيح شاخص = b*l.inputs;
                لكل (i = 0; i < locations; ++i) {
                    لكل (j = 0; j < l.n; ++j) {
                        صحيح p_شاخص = شاخص + locations*l.classes + i*l.n + j;
                        costs[b*locations*l.n + i*l.n + j] = l.delta[p_شاخص]*l.delta[p_شاخص];
                    }
                }
            }
            صحيح indexes[100];
            سر_k(costs, l.batch*locations*l.n, 100, indexes);
            عائم cutoff = costs[indexes[99]];
            لكل (b = 0; b < l.batch; ++b) {
                صحيح شاخص = b*l.inputs;
                لكل (i = 0; i < locations; ++i) {
                    لكل (j = 0; j < l.n; ++j) {
                        صحيح p_شاخص = شاخص + locations*l.classes + i*l.n + j;
                        إذا (l.delta[p_شاخص]*l.delta[p_شاخص] < cutoff) l.delta[p_شاخص] = 0;
                    }
                }
            }
            حرر(costs);
        }


        *(l.لګښت_2) = توان(mag_array(l.delta, l.outputs * l.batch), 2);


        چاپ_کړه("Detection Avg IOU: %f, Pos Cat: %f, All Cat: %f, Pos Obj: %f, Any Obj: %f, count: %d\n", avg_iou/شمېر, avg_cat/شمېر, avg_allcat/(شمېر*l.classes), avg_obj/شمېر, avg_anyobj/(l.batch*locations*l.n), شمېر);
        //if(l.reorg) reorg(l.delta, l.w*l.h, size*l.n, l.batch, 0);
    }
}

فراغ backward_detection_layer(ثابت detection_layer l, شبکه net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

فراغ get_detection_detections(layer l, صحيح w, صحيح h, عائم thresh, detection *dets)
{
    صحيح i,j,n;
    عائم *predictions = l.output;
    //int per_cell = 5*num+classes;
    لكل (i = 0; i < l.side*l.side; ++i){
        صحيح لیکه = i / l.side;
        صحيح ستون = i % l.side;
        لكل(n = 0; n < l.n; ++n){
            صحيح شاخص = i*l.n + n;
            صحيح p_شاخص = l.side*l.side*l.classes + i*l.n + n;
            عائم scale = predictions[p_شاخص];
            صحيح box_شاخص = l.side*l.side*(l.classes + l.n) + (i*l.n + n)*4;
            box b;
            b.x = (predictions[box_شاخص + 0] + ستون) / l.side * w;
            b.y = (predictions[box_شاخص + 1] + لیکه) / l.side * h;
            b.w = توان(predictions[box_شاخص + 2], (l.جذر?2:1)) * w;
            b.h = توان(predictions[box_شاخص + 3], (l.جذر?2:1)) * h;
            dets[شاخص].bbox = b;
            dets[شاخص].objectness = scale;
            لكل(j = 0; j < l.classes; ++j){
                صحيح class_شاخص = i*l.classes;
                عائم prob = scale*predictions[class_شاخص+j];
                dets[شاخص].prob[j] = (prob > thresh) ? prob : 0;
            }
        }
    }
}

#که_تعریف_شوی GPU

فراغ forward_detection_layer_gpu(ثابت detection_layer l, شبکه net)
{
    إذا(!net.train){
        copy_gpu(l.batch*l.inputs, net.ننوت_gpu, 1, l.output_gpu, 1);
        أرجع;
    }

    cuda_pull_array(net.ننوت_gpu, net.ننوت, l.batch*l.inputs);
    forward_detection_layer(l, net);
    cuda_ټېل_array(l.output_gpu, l.output, l.batch*l.outputs);
    cuda_ټېل_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

فراغ backward_detection_layer_gpu(detection_layer l, شبکه net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
    //copy_gpu(l.batch*l.inputs, l.delta_gpu, 1, net.delta_gpu, 1);
}
#د_که_پای


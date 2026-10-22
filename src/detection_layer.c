#అన్తర్భావ "detection_layer.h"
#అన్తర్భావ "activations.h"
#అన్తర్భావ "softmax_layer.h"
#అన్తర్భావ "blas.h"
#అన్తర్భావ "box.h"
#అన్తర్భావ "cuda.h"
#అన్తర్భావ "utils.h"

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <assert.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <stdlib.h>

detection_layer make_detection_layer(పూర్ణాంకం batch, పూర్ణాంకం inputs, పూర్ణాంకం n, పూర్ణాంకం side, పూర్ణాంకం classes, పూర్ణాంకం coords, పూర్ణాంకం rescore)
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
    ప్రతిజ్ఞా(side*side*((1 + l.coords)*l.n + l.classes) == inputs);
    l.ఖర్చు_2 = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(దశాంశం));
    l.outputs = l.inputs;
    l.truths = l.side*l.side*(1+l.coords+l.classes);
    l.output = సున్నా_జ్ఞాపకం_ఇవ్వు(batch*l.outputs, పరిమాణం(దశాంశం));
    l.delta = సున్నా_జ్ఞాపకం_ఇవ్వు(batch*l.outputs, పరిమాణం(దశాంశం));

    l.forward = forward_detection_layer;
    l.backward = backward_detection_layer;
#నిర్వచితమైతే GPU
    l.forward_gpu = forward_detection_layer_gpu;
    l.backward_gpu = backward_detection_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#అయితే_ముగింపు

    fprintf(stderr, "Detection Layer\n");
    srand(0);

    తిరిగి l;
}

శూన్య forward_detection_layer(స్థిరరాశి detection_layer l, జాలం net)
{
    పూర్ణాంకం locations = l.side*l.side;
    పూర్ణాంకం i,j;
    జ్ఞాపకం_నకలు(l.output, net.ఆదానం, l.outputs*l.batch*పరిమాణం(దశాంశం));
    //if(l.reorg) reorg(l.output, l.w*l.h, size*l.n, l.batch, 1);
    పూర్ణాంకం b;
    ఒకవేళ (l.softmax){
        కొరకు(b = 0; b < l.batch; ++b){
            పూర్ణాంకం సూచీ = b*l.inputs;
            కొరకు (i = 0; i < locations; ++i) {
                పూర్ణాంకం స్థానభ్రంశం = i*l.classes;
                softmax(l.output + సూచీ + స్థానభ్రంశం, l.classes, 1, 1,
                        l.output + సూచీ + స్థానభ్రంశం);
            }
        }
    }
    ఒకవేళ(net.train){
        దశాంశం avg_iou = 0;
        దశాంశం avg_cat = 0;
        దశాంశం avg_allcat = 0;
        దశాంశం avg_obj = 0;
        దశాంశం avg_anyobj = 0;
        పూర్ణాంకం లెక్క = 0;
        *(l.ఖర్చు_2) = 0;
        పూర్ణాంకం కొలత = l.inputs * l.batch;
        memset(l.delta, 0, కొలత * పరిమాణం(దశాంశం));
        కొరకు (b = 0; b < l.batch; ++b){
            పూర్ణాంకం సూచీ = b*l.inputs;
            కొరకు (i = 0; i < locations; ++i) {
                పూర్ణాంకం truth_సూచీ = (b*locations + i)*(1+l.coords+l.classes);
                పూర్ణాంకం is_obj = net.truth[truth_సూచీ];
                కొరకు (j = 0; j < l.n; ++j) {
                    పూర్ణాంకం p_సూచీ = సూచీ + locations*l.classes + i*l.n + j;
                    l.delta[p_సూచీ] = l.noobject_scale*(0 - l.output[p_సూచీ]);
                    *(l.ఖర్చు_2) += l.noobject_scale*ఘాతం(l.output[p_సూచీ], 2);
                    avg_anyobj += l.output[p_సూచీ];
                }

                పూర్ణాంకం ఉత్తమ_సూచీ = -1;
                దశాంశం ఉత్తమ_iou = 0;
                దశాంశం ఉత్తమ_rmse = 20;

                ఒకవేళ (!is_obj){
                    కొనసాగు;
                }

                పూర్ణాంకం class_సూచీ = సూచీ + i*l.classes;
                కొరకు(j = 0; j < l.classes; ++j) {
                    l.delta[class_సూచీ+j] = l.class_scale * (net.truth[truth_సూచీ+1+j] - l.output[class_సూచీ+j]);
                    *(l.ఖర్చు_2) += l.class_scale * ఘాతం(net.truth[truth_సూచీ+1+j] - l.output[class_సూచీ+j], 2);
                    ఒకవేళ(net.truth[truth_సూచీ + 1 + j]) avg_cat += l.output[class_సూచీ+j];
                    avg_allcat += l.output[class_సూచీ+j];
                }

                box truth = float_గమ్యం_box(net.truth + truth_సూచీ + 1 + l.classes, 1);
                truth.x /= l.side;
                truth.y /= l.side;

                కొరకు(j = 0; j < l.n; ++j){
                    పూర్ణాంకం box_సూచీ = సూచీ + locations*(l.classes + l.n) + (i*l.n + j) * l.coords;
                    box బహిర్గతం = float_గమ్యం_box(l.output + box_సూచీ, 1);
                    బహిర్గతం.x /= l.side;
                    బహిర్గతం.y /= l.side;

                    ఒకవేళ (l.వర్గమూలం){
                        బహిర్గతం.w = బహిర్గతం.w*బహిర్గతం.w;
                        బహిర్గతం.h = బహిర్గతం.h*బహిర్గతం.h;
                    }

                    దశాంశం iou  = box_iou(బహిర్గతం, truth);
                    //iou = 0;
                    దశాంశం rmse = box_rmse(బహిర్గతం, truth);
                    ఒకవేళ(ఉత్తమ_iou > 0 || iou > 0){
                        ఒకవేళ(iou > ఉత్తమ_iou){
                            ఉత్తమ_iou = iou;
                            ఉత్తమ_సూచీ = j;
                        }
                    }లేకపోతే{
                        ఒకవేళ(rmse < ఉత్తమ_rmse){
                            ఉత్తమ_rmse = rmse;
                            ఉత్తమ_సూచీ = j;
                        }
                    }
                }

                ఒకవేళ(l.forced){
                    ఒకవేళ(truth.w*truth.h < .1){
                        ఉత్తమ_సూచీ = 1;
                    }లేకపోతే{
                        ఉత్తమ_సూచీ = 0;
                    }
                }
                ఒకవేళ(l.random && *(net.చూసిన) < 64000){
                    ఉత్తమ_సూచీ = rand()%l.n;
                }

                పూర్ణాంకం box_సూచీ = సూచీ + locations*(l.classes + l.n) + (i*l.n + ఉత్తమ_సూచీ) * l.coords;
                పూర్ణాంకం tbox_సూచీ = truth_సూచీ + 1 + l.classes;

                box బహిర్గతం = float_గమ్యం_box(l.output + box_సూచీ, 1);
                బహిర్గతం.x /= l.side;
                బహిర్గతం.y /= l.side;
                ఒకవేళ (l.వర్గమూలం) {
                    బహిర్గతం.w = బహిర్గతం.w*బహిర్గతం.w;
                    బహిర్గతం.h = బహిర్గతం.h*బహిర్గతం.h;
                }
                దశాంశం iou  = box_iou(బహిర్గతం, truth);

                //printf("%d,", best_index);
                పూర్ణాంకం p_సూచీ = సూచీ + locations*l.classes + i*l.n + ఉత్తమ_సూచీ;
                *(l.ఖర్చు_2) -= l.noobject_scale * ఘాతం(l.output[p_సూచీ], 2);
                *(l.ఖర్చు_2) += l.object_scale * ఘాతం(1-l.output[p_సూచీ], 2);
                avg_obj += l.output[p_సూచీ];
                l.delta[p_సూచీ] = l.object_scale * (1.-l.output[p_సూచీ]);

                ఒకవేళ(l.rescore){
                    l.delta[p_సూచీ] = l.object_scale * (iou - l.output[p_సూచీ]);
                }

                l.delta[box_సూచీ+0] = l.coord_scale*(net.truth[tbox_సూచీ + 0] - l.output[box_సూచీ + 0]);
                l.delta[box_సూచీ+1] = l.coord_scale*(net.truth[tbox_సూచీ + 1] - l.output[box_సూచీ + 1]);
                l.delta[box_సూచీ+2] = l.coord_scale*(net.truth[tbox_సూచీ + 2] - l.output[box_సూచీ + 2]);
                l.delta[box_సూచీ+3] = l.coord_scale*(net.truth[tbox_సూచీ + 3] - l.output[box_సూచీ + 3]);
                ఒకవేళ(l.వర్గమూలం){
                    l.delta[box_సూచీ+2] = l.coord_scale*(వర్గమూలం(net.truth[tbox_సూచీ + 2]) - l.output[box_సూచీ + 2]);
                    l.delta[box_సూచీ+3] = l.coord_scale*(వర్గమూలం(net.truth[tbox_సూచీ + 3]) - l.output[box_సూచీ + 3]);
                }

                *(l.ఖర్చు_2) += ఘాతం(1-iou, 2);
                avg_iou += iou;
                ++లెక్క;
            }
        }

        ఒకవేళ(0){
            దశాంశం *costs = సున్నా_జ్ఞాపకం_ఇవ్వు(l.batch*locations*l.n, పరిమాణం(దశాంశం));
            కొరకు (b = 0; b < l.batch; ++b) {
                పూర్ణాంకం సూచీ = b*l.inputs;
                కొరకు (i = 0; i < locations; ++i) {
                    కొరకు (j = 0; j < l.n; ++j) {
                        పూర్ణాంకం p_సూచీ = సూచీ + locations*l.classes + i*l.n + j;
                        costs[b*locations*l.n + i*l.n + j] = l.delta[p_సూచీ]*l.delta[p_సూచీ];
                    }
                }
            }
            పూర్ణాంకం indexes[100];
            శిఖరం_k(costs, l.batch*locations*l.n, 100, indexes);
            దశాంశం cutoff = costs[indexes[99]];
            కొరకు (b = 0; b < l.batch; ++b) {
                పూర్ణాంకం సూచీ = b*l.inputs;
                కొరకు (i = 0; i < locations; ++i) {
                    కొరకు (j = 0; j < l.n; ++j) {
                        పూర్ణాంకం p_సూచీ = సూచీ + locations*l.classes + i*l.n + j;
                        ఒకవేళ (l.delta[p_సూచీ]*l.delta[p_సూచీ] < cutoff) l.delta[p_సూచీ] = 0;
                    }
                }
            }
            విడుదల(costs);
        }


        *(l.ఖర్చు_2) = ఘాతం(mag_array(l.delta, l.outputs * l.batch), 2);


        రాయి("Detection Avg IOU: %f, Pos Cat: %f, All Cat: %f, Pos Obj: %f, Any Obj: %f, count: %d\n", avg_iou/లెక్క, avg_cat/లెక్క, avg_allcat/(లెక్క*l.classes), avg_obj/లెక్క, avg_anyobj/(l.batch*locations*l.n), లెక్క);
        //if(l.reorg) reorg(l.delta, l.w*l.h, size*l.n, l.batch, 0);
    }
}

శూన్య backward_detection_layer(స్థిరరాశి detection_layer l, జాలం net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

శూన్య get_detection_detections(layer l, పూర్ణాంకం w, పూర్ణాంకం h, దశాంశం thresh, detection *dets)
{
    పూర్ణాంకం i,j,n;
    దశాంశం *predictions = l.output;
    //int per_cell = 5*num+classes;
    కొరకు (i = 0; i < l.side*l.side; ++i){
        పూర్ణాంకం అడ్డువరుస = i / l.side;
        పూర్ణాంకం నిలువు = i % l.side;
        కొరకు(n = 0; n < l.n; ++n){
            పూర్ణాంకం సూచీ = i*l.n + n;
            పూర్ణాంకం p_సూచీ = l.side*l.side*l.classes + i*l.n + n;
            దశాంశం scale = predictions[p_సూచీ];
            పూర్ణాంకం box_సూచీ = l.side*l.side*(l.classes + l.n) + (i*l.n + n)*4;
            box b;
            b.x = (predictions[box_సూచీ + 0] + నిలువు) / l.side * w;
            b.y = (predictions[box_సూచీ + 1] + అడ్డువరుస) / l.side * h;
            b.w = ఘాతం(predictions[box_సూచీ + 2], (l.వర్గమూలం?2:1)) * w;
            b.h = ఘాతం(predictions[box_సూచీ + 3], (l.వర్గమూలం?2:1)) * h;
            dets[సూచీ].bbox = b;
            dets[సూచీ].objectness = scale;
            కొరకు(j = 0; j < l.classes; ++j){
                పూర్ణాంకం class_సూచీ = i*l.classes;
                దశాంశం prob = scale*predictions[class_సూచీ+j];
                dets[సూచీ].prob[j] = (prob > thresh) ? prob : 0;
            }
        }
    }
}

#నిర్వచితమైతే GPU

శూన్య forward_detection_layer_gpu(స్థిరరాశి detection_layer l, జాలం net)
{
    ఒకవేళ(!net.train){
        copy_gpu(l.batch*l.inputs, net.ఆదానం_gpu, 1, l.output_gpu, 1);
        తిరిగి;
    }

    cuda_pull_array(net.ఆదానం_gpu, net.ఆదానం, l.batch*l.inputs);
    forward_detection_layer(l, net);
    cuda_నెట్టు_array(l.output_gpu, l.output, l.batch*l.outputs);
    cuda_నెట్టు_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

శూన్య backward_detection_layer_gpu(detection_layer l, జాలం net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
    //copy_gpu(l.batch*l.inputs, l.delta_gpu, 1, net.delta_gpu, 1);
}
#అయితే_ముగింపు


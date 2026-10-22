#ಅನ್ತರ್ಭಾವ "detection_layer.h"
#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "softmax_layer.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "box.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>

detection_layer make_detection_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ inputs, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ side, ಪೂರ್ಣಾಂಕ classes, ಪೂರ್ಣಾಂಕ coords, ಪೂರ್ಣಾಂಕ rescore)
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
    ಪ್ರತಿಜ್ಞಾ(side*side*((1 + l.coords)*l.n + l.classes) == inputs);
    l.ವೆಚ್ಚ_2 = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.outputs = l.inputs;
    l.truths = l.side*l.side*(1+l.coords+l.classes);
    l.output = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*l.outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.delta = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*l.outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));

    l.forward = forward_detection_layer;
    l.backward = backward_detection_layer;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l.forward_gpu = forward_detection_layer_gpu;
    l.backward_gpu = backward_detection_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#ಆದರೆ_ಅಂತ್ಯ

    fprintf(stderr, "Detection Layer\n");
    srand(0);

    ಹಿಂತಿರುಗಿ l;
}

ಶೂನ್ಯ forward_detection_layer(ಸ್ಥಿರಾಂಕ detection_layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ locations = l.side*l.side;
    ಪೂರ್ಣಾಂಕ i,j;
    ಸ್ಮರಣೆ_ನಕಲು(l.output, net.ಒಳಹರಿವು, l.outputs*l.batch*ಗಾತ್ರ(ದಶಮಾಂಶ));
    //if(l.reorg) reorg(l.output, l.w*l.h, size*l.n, l.batch, 1);
    ಪೂರ್ಣಾಂಕ b;
    ಆದರೆ (l.softmax){
        ಗಾಗಿ(b = 0; b < l.batch; ++b){
            ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = b*l.inputs;
            ಗಾಗಿ (i = 0; i < locations; ++i) {
                ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ = i*l.classes;
                softmax(l.output + ಸೂಚ್ಯಂಕ + ಸ್ಥಾನಾಂತರ, l.classes, 1, 1,
                        l.output + ಸೂಚ್ಯಂಕ + ಸ್ಥಾನಾಂತರ);
            }
        }
    }
    ಆದರೆ(net.train){
        ದಶಮಾಂಶ avg_iou = 0;
        ದಶಮಾಂಶ avg_cat = 0;
        ದಶಮಾಂಶ avg_allcat = 0;
        ದಶಮಾಂಶ avg_obj = 0;
        ದಶಮಾಂಶ avg_anyobj = 0;
        ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
        *(l.ವೆಚ್ಚ_2) = 0;
        ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2 = l.inputs * l.batch;
        memset(l.delta, 0, ಗಾತ್ರ_2 * ಗಾತ್ರ(ದಶಮಾಂಶ));
        ಗಾಗಿ (b = 0; b < l.batch; ++b){
            ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = b*l.inputs;
            ಗಾಗಿ (i = 0; i < locations; ++i) {
                ಪೂರ್ಣಾಂಕ truth_ಸೂಚ್ಯಂಕ = (b*locations + i)*(1+l.coords+l.classes);
                ಪೂರ್ಣಾಂಕ is_obj = net.truth[truth_ಸೂಚ್ಯಂಕ];
                ಗಾಗಿ (j = 0; j < l.n; ++j) {
                    ಪೂರ್ಣಾಂಕ p_ಸೂಚ್ಯಂಕ = ಸೂಚ್ಯಂಕ + locations*l.classes + i*l.n + j;
                    l.delta[p_ಸೂಚ್ಯಂಕ] = l.noobject_scale*(0 - l.output[p_ಸೂಚ್ಯಂಕ]);
                    *(l.ವೆಚ್ಚ_2) += l.noobject_scale*ಘಾತ(l.output[p_ಸೂಚ್ಯಂಕ], 2);
                    avg_anyobj += l.output[p_ಸೂಚ್ಯಂಕ];
                }

                ಪೂರ್ಣಾಂಕ ಉತ್ತಮ_ಸೂಚ್ಯಂಕ = -1;
                ದಶಮಾಂಶ ಉತ್ತಮ_iou = 0;
                ದಶಮಾಂಶ ಉತ್ತಮ_rmse = 20;

                ಆದರೆ (!is_obj){
                    ಮುಂದುವರಿ;
                }

                ಪೂರ್ಣಾಂಕ class_ಸೂಚ್ಯಂಕ = ಸೂಚ್ಯಂಕ + i*l.classes;
                ಗಾಗಿ(j = 0; j < l.classes; ++j) {
                    l.delta[class_ಸೂಚ್ಯಂಕ+j] = l.class_scale * (net.truth[truth_ಸೂಚ್ಯಂಕ+1+j] - l.output[class_ಸೂಚ್ಯಂಕ+j]);
                    *(l.ವೆಚ್ಚ_2) += l.class_scale * ಘಾತ(net.truth[truth_ಸೂಚ್ಯಂಕ+1+j] - l.output[class_ಸೂಚ್ಯಂಕ+j], 2);
                    ಆದರೆ(net.truth[truth_ಸೂಚ್ಯಂಕ + 1 + j]) avg_cat += l.output[class_ಸೂಚ್ಯಂಕ+j];
                    avg_allcat += l.output[class_ಸೂಚ್ಯಂಕ+j];
                }

                box truth = float_ಗಮ್ಯ_box(net.truth + truth_ಸೂಚ್ಯಂಕ + 1 + l.classes, 1);
                truth.x /= l.side;
                truth.y /= l.side;

                ಗಾಗಿ(j = 0; j < l.n; ++j){
                    ಪೂರ್ಣಾಂಕ box_ಸೂಚ್ಯಂಕ = ಸೂಚ್ಯಂಕ + locations*(l.classes + l.n) + (i*l.n + j) * l.coords;
                    box ಹೊರಹೋಗು = float_ಗಮ್ಯ_box(l.output + box_ಸೂಚ್ಯಂಕ, 1);
                    ಹೊರಹೋಗು.x /= l.side;
                    ಹೊರಹೋಗು.y /= l.side;

                    ಆದರೆ (l.ವರ್ಗಮೂಲ){
                        ಹೊರಹೋಗು.w = ಹೊರಹೋಗು.w*ಹೊರಹೋಗು.w;
                        ಹೊರಹೋಗು.h = ಹೊರಹೋಗು.h*ಹೊರಹೋಗು.h;
                    }

                    ದಶಮಾಂಶ iou  = box_iou(ಹೊರಹೋಗು, truth);
                    //iou = 0;
                    ದಶಮಾಂಶ rmse = box_rmse(ಹೊರಹೋಗು, truth);
                    ಆದರೆ(ಉತ್ತಮ_iou > 0 || iou > 0){
                        ಆದರೆ(iou > ಉತ್ತಮ_iou){
                            ಉತ್ತಮ_iou = iou;
                            ಉತ್ತಮ_ಸೂಚ್ಯಂಕ = j;
                        }
                    }ಇಲ್ಲದಿದ್ದರೆ{
                        ಆದರೆ(rmse < ಉತ್ತಮ_rmse){
                            ಉತ್ತಮ_rmse = rmse;
                            ಉತ್ತಮ_ಸೂಚ್ಯಂಕ = j;
                        }
                    }
                }

                ಆದರೆ(l.forced){
                    ಆದರೆ(truth.w*truth.h < .1){
                        ಉತ್ತಮ_ಸೂಚ್ಯಂಕ = 1;
                    }ಇಲ್ಲದಿದ್ದರೆ{
                        ಉತ್ತಮ_ಸೂಚ್ಯಂಕ = 0;
                    }
                }
                ಆದರೆ(l.random && *(net.ನೋಡಿದ) < 64000){
                    ಉತ್ತಮ_ಸೂಚ್ಯಂಕ = rand()%l.n;
                }

                ಪೂರ್ಣಾಂಕ box_ಸೂಚ್ಯಂಕ = ಸೂಚ್ಯಂಕ + locations*(l.classes + l.n) + (i*l.n + ಉತ್ತಮ_ಸೂಚ್ಯಂಕ) * l.coords;
                ಪೂರ್ಣಾಂಕ tbox_ಸೂಚ್ಯಂಕ = truth_ಸೂಚ್ಯಂಕ + 1 + l.classes;

                box ಹೊರಹೋಗು = float_ಗಮ್ಯ_box(l.output + box_ಸೂಚ್ಯಂಕ, 1);
                ಹೊರಹೋಗು.x /= l.side;
                ಹೊರಹೋಗು.y /= l.side;
                ಆದರೆ (l.ವರ್ಗಮೂಲ) {
                    ಹೊರಹೋಗು.w = ಹೊರಹೋಗು.w*ಹೊರಹೋಗು.w;
                    ಹೊರಹೋಗು.h = ಹೊರಹೋಗು.h*ಹೊರಹೋಗು.h;
                }
                ದಶಮಾಂಶ iou  = box_iou(ಹೊರಹೋಗು, truth);

                //printf("%d,", best_index);
                ಪೂರ್ಣಾಂಕ p_ಸೂಚ್ಯಂಕ = ಸೂಚ್ಯಂಕ + locations*l.classes + i*l.n + ಉತ್ತಮ_ಸೂಚ್ಯಂಕ;
                *(l.ವೆಚ್ಚ_2) -= l.noobject_scale * ಘಾತ(l.output[p_ಸೂಚ್ಯಂಕ], 2);
                *(l.ವೆಚ್ಚ_2) += l.object_scale * ಘಾತ(1-l.output[p_ಸೂಚ್ಯಂಕ], 2);
                avg_obj += l.output[p_ಸೂಚ್ಯಂಕ];
                l.delta[p_ಸೂಚ್ಯಂಕ] = l.object_scale * (1.-l.output[p_ಸೂಚ್ಯಂಕ]);

                ಆದರೆ(l.rescore){
                    l.delta[p_ಸೂಚ್ಯಂಕ] = l.object_scale * (iou - l.output[p_ಸೂಚ್ಯಂಕ]);
                }

                l.delta[box_ಸೂಚ್ಯಂಕ+0] = l.coord_scale*(net.truth[tbox_ಸೂಚ್ಯಂಕ + 0] - l.output[box_ಸೂಚ್ಯಂಕ + 0]);
                l.delta[box_ಸೂಚ್ಯಂಕ+1] = l.coord_scale*(net.truth[tbox_ಸೂಚ್ಯಂಕ + 1] - l.output[box_ಸೂಚ್ಯಂಕ + 1]);
                l.delta[box_ಸೂಚ್ಯಂಕ+2] = l.coord_scale*(net.truth[tbox_ಸೂಚ್ಯಂಕ + 2] - l.output[box_ಸೂಚ್ಯಂಕ + 2]);
                l.delta[box_ಸೂಚ್ಯಂಕ+3] = l.coord_scale*(net.truth[tbox_ಸೂಚ್ಯಂಕ + 3] - l.output[box_ಸೂಚ್ಯಂಕ + 3]);
                ಆದರೆ(l.ವರ್ಗಮೂಲ){
                    l.delta[box_ಸೂಚ್ಯಂಕ+2] = l.coord_scale*(ವರ್ಗಮೂಲ(net.truth[tbox_ಸೂಚ್ಯಂಕ + 2]) - l.output[box_ಸೂಚ್ಯಂಕ + 2]);
                    l.delta[box_ಸೂಚ್ಯಂಕ+3] = l.coord_scale*(ವರ್ಗಮೂಲ(net.truth[tbox_ಸೂಚ್ಯಂಕ + 3]) - l.output[box_ಸೂಚ್ಯಂಕ + 3]);
                }

                *(l.ವೆಚ್ಚ_2) += ಘಾತ(1-iou, 2);
                avg_iou += iou;
                ++ಎಣಿಕೆ_2;
            }
        }

        ಆದರೆ(0){
            ದಶಮಾಂಶ *costs = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.batch*locations*l.n, ಗಾತ್ರ(ದಶಮಾಂಶ));
            ಗಾಗಿ (b = 0; b < l.batch; ++b) {
                ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = b*l.inputs;
                ಗಾಗಿ (i = 0; i < locations; ++i) {
                    ಗಾಗಿ (j = 0; j < l.n; ++j) {
                        ಪೂರ್ಣಾಂಕ p_ಸೂಚ್ಯಂಕ = ಸೂಚ್ಯಂಕ + locations*l.classes + i*l.n + j;
                        costs[b*locations*l.n + i*l.n + j] = l.delta[p_ಸೂಚ್ಯಂಕ]*l.delta[p_ಸೂಚ್ಯಂಕ];
                    }
                }
            }
            ಪೂರ್ಣಾಂಕ indexes[100];
            ತುದಿ_k(costs, l.batch*locations*l.n, 100, indexes);
            ದಶಮಾಂಶ cutoff = costs[indexes[99]];
            ಗಾಗಿ (b = 0; b < l.batch; ++b) {
                ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = b*l.inputs;
                ಗಾಗಿ (i = 0; i < locations; ++i) {
                    ಗಾಗಿ (j = 0; j < l.n; ++j) {
                        ಪೂರ್ಣಾಂಕ p_ಸೂಚ್ಯಂಕ = ಸೂಚ್ಯಂಕ + locations*l.classes + i*l.n + j;
                        ಆದರೆ (l.delta[p_ಸೂಚ್ಯಂಕ]*l.delta[p_ಸೂಚ್ಯಂಕ] < cutoff) l.delta[p_ಸೂಚ್ಯಂಕ] = 0;
                    }
                }
            }
            ಬಿಡುಗಡೆ(costs);
        }


        *(l.ವೆಚ್ಚ_2) = ಘಾತ(mag_array(l.delta, l.outputs * l.batch), 2);


        ಬರೆ("Detection Avg IOU: %f, Pos Cat: %f, All Cat: %f, Pos Obj: %f, Any Obj: %f, count: %d\n", avg_iou/ಎಣಿಕೆ_2, avg_cat/ಎಣಿಕೆ_2, avg_allcat/(ಎಣಿಕೆ_2*l.classes), avg_obj/ಎಣಿಕೆ_2, avg_anyobj/(l.batch*locations*l.n), ಎಣಿಕೆ_2);
        //if(l.reorg) reorg(l.delta, l.w*l.h, size*l.n, l.batch, 0);
    }
}

ಶೂನ್ಯ backward_detection_layer(ಸ್ಥಿರಾಂಕ detection_layer l, ಜಾಲ net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

ಶೂನ್ಯ get_detection_detections(layer l, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ದಶಮಾಂಶ thresh, detection *dets)
{
    ಪೂರ್ಣಾಂಕ i,j,n;
    ದಶಮಾಂಶ *predictions = l.output;
    //int per_cell = 5*num+classes;
    ಗಾಗಿ (i = 0; i < l.side*l.side; ++i){
        ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲು = i / l.side;
        ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು = i % l.side;
        ಗಾಗಿ(n = 0; n < l.n; ++n){
            ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = i*l.n + n;
            ಪೂರ್ಣಾಂಕ p_ಸೂಚ್ಯಂಕ = l.side*l.side*l.classes + i*l.n + n;
            ದಶಮಾಂಶ scale = predictions[p_ಸೂಚ್ಯಂಕ];
            ಪೂರ್ಣಾಂಕ box_ಸೂಚ್ಯಂಕ = l.side*l.side*(l.classes + l.n) + (i*l.n + n)*4;
            box b;
            b.x = (predictions[box_ಸೂಚ್ಯಂಕ + 0] + ಲಂಬಸಾಲು) / l.side * w;
            b.y = (predictions[box_ಸೂಚ್ಯಂಕ + 1] + ಅಡ್ಡಸಾಲು) / l.side * h;
            b.w = ಘಾತ(predictions[box_ಸೂಚ್ಯಂಕ + 2], (l.ವರ್ಗಮೂಲ?2:1)) * w;
            b.h = ಘಾತ(predictions[box_ಸೂಚ್ಯಂಕ + 3], (l.ವರ್ಗಮೂಲ?2:1)) * h;
            dets[ಸೂಚ್ಯಂಕ].bbox = b;
            dets[ಸೂಚ್ಯಂಕ].objectness = scale;
            ಗಾಗಿ(j = 0; j < l.classes; ++j){
                ಪೂರ್ಣಾಂಕ class_ಸೂಚ್ಯಂಕ = i*l.classes;
                ದಶಮಾಂಶ prob = scale*predictions[class_ಸೂಚ್ಯಂಕ+j];
                dets[ಸೂಚ್ಯಂಕ].prob[j] = (prob > thresh) ? prob : 0;
            }
        }
    }
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU

ಶೂನ್ಯ forward_detection_layer_gpu(ಸ್ಥಿರಾಂಕ detection_layer l, ಜಾಲ net)
{
    ಆದರೆ(!net.train){
        copy_gpu(l.batch*l.inputs, net.ಒಳಹರಿವು_gpu, 1, l.output_gpu, 1);
        ಹಿಂತಿರುಗಿ;
    }

    cuda_pull_array(net.ಒಳಹರಿವು_gpu, net.ಒಳಹರಿವು, l.batch*l.inputs);
    forward_detection_layer(l, net);
    cuda_ತಳ್ಳು_array(l.output_gpu, l.output, l.batch*l.outputs);
    cuda_ತಳ್ಳು_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

ಶೂನ್ಯ backward_detection_layer_gpu(detection_layer l, ಜಾಲ net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
    //copy_gpu(l.batch*l.inputs, l.delta_gpu, 1, net.delta_gpu, 1);
}
#ಆದರೆ_ಅಂತ್ಯ


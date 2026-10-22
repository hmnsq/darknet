#अन्तर्भाव "detection_layer.h"
#अन्तर्भाव "activations.h"
#अन्तर्भाव "softmax_layer.h"
#अन्तर्भाव "blas.h"
#अन्तर्भाव "box.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव "utils.h"

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <assert.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <stdlib.h>

detection_layer make_detection_layer(पूर्णाङ्क batch, पूर्णाङ्क inputs, पूर्णाङ्क n, पूर्णाङ्क side, पूर्णाङ्क classes, पूर्णाङ्क coords, पूर्णाङ्क rescore)
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
    प्रतिज्ञा(side*side*((1 + l.coords)*l.n + l.classes) == inputs);
    l.cost = शून्यस्मृतिं_देहि(1, परिमाण(भिन्न));
    l.outputs = l.inputs;
    l.truths = l.side*l.side*(1+l.coords+l.classes);
    l.output = शून्यस्मृतिं_देहि(batch*l.outputs, परिमाण(भिन्न));
    l.delta = शून्यस्मृतिं_देहि(batch*l.outputs, परिमाण(भिन्न));

    l.forward = forward_detection_layer;
    l.backward = backward_detection_layer;
#यदि_परिभाषितम् GPU
    l.forward_gpu = forward_detection_layer_gpu;
    l.backward_gpu = backward_detection_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#यदि_अन्तः

    fprintf(stderr, "Detection Layer\n");
    बीजं_रोपय(0);

    प्रत्यागम l;
}

शून्य forward_detection_layer(ध्रुव detection_layer l, जालम् net)
{
    पूर्णाङ्क locations = l.side*l.side;
    पूर्णाङ्क i,j;
    स्मृतिं_प्रतिलिख(l.output, net.निवेशः, l.outputs*l.batch*परिमाण(भिन्न));
    //if(l.reorg) reorg(l.output, l.w*l.h, size*l.n, l.batch, 1);
    पूर्णाङ्क b;
    यदि (l.softmax){
        प्रति(b = 0; b < l.batch; ++b){
            पूर्णाङ्क सूचकांक = b*l.inputs;
            प्रति (i = 0; i < locations; ++i) {
                पूर्णाङ्क विस्थापनम् = i*l.classes;
                softmax(l.output + सूचकांक + विस्थापनम्, l.classes, 1, 1,
                        l.output + सूचकांक + विस्थापनम्);
            }
        }
    }
    यदि(net.train){
        भिन्न avg_iou = 0;
        भिन्न avg_cat = 0;
        भिन्न avg_allcat = 0;
        भिन्न avg_obj = 0;
        भिन्न avg_anyobj = 0;
        पूर्णाङ्क गणनम् = 0;
        *(l.cost) = 0;
        पूर्णाङ्क आकार = l.inputs * l.batch;
        स्मृतिं_पूरय(l.delta, 0, आकार * परिमाण(भिन्न));
        प्रति (b = 0; b < l.batch; ++b){
            पूर्णाङ्क सूचकांक = b*l.inputs;
            प्रति (i = 0; i < locations; ++i) {
                पूर्णाङ्क truth_सूचकांक = (b*locations + i)*(1+l.coords+l.classes);
                पूर्णाङ्क is_obj = net.truth[truth_सूचकांक];
                प्रति (j = 0; j < l.n; ++j) {
                    पूर्णाङ्क p_सूचकांक = सूचकांक + locations*l.classes + i*l.n + j;
                    l.delta[p_सूचकांक] = l.noobject_scale*(0 - l.output[p_सूचकांक]);
                    *(l.cost) += l.noobject_scale*घातः(l.output[p_सूचकांक], 2);
                    avg_anyobj += l.output[p_सूचकांक];
                }

                पूर्णाङ्क best_सूचकांक = -1;
                भिन्न best_iou = 0;
                भिन्न best_rmse = 20;

                यदि (!is_obj){
                    अनुवर्त;
                }

                पूर्णाङ्क class_सूचकांक = सूचकांक + i*l.classes;
                प्रति(j = 0; j < l.classes; ++j) {
                    l.delta[class_सूचकांक+j] = l.class_scale * (net.truth[truth_सूचकांक+1+j] - l.output[class_सूचकांक+j]);
                    *(l.cost) += l.class_scale * घातः(net.truth[truth_सूचकांक+1+j] - l.output[class_सूचकांक+j], 2);
                    यदि(net.truth[truth_सूचकांक + 1 + j]) avg_cat += l.output[class_सूचकांक+j];
                    avg_allcat += l.output[class_सूचकांक+j];
                }

                box truth = float_गन्तव्यम्_box(net.truth + truth_सूचकांक + 1 + l.classes, 1);
                truth.x /= l.side;
                truth.y /= l.side;

                प्रति(j = 0; j < l.n; ++j){
                    पूर्णाङ्क box_सूचकांक = सूचकांक + locations*(l.classes + l.n) + (i*l.n + j) * l.coords;
                    box बहिर्गमनम् = float_गन्तव्यम्_box(l.output + box_सूचकांक, 1);
                    बहिर्गमनम्.x /= l.side;
                    बहिर्गमनम्.y /= l.side;

                    यदि (l.वर्गमूलम्){
                        बहिर्गमनम्.w = बहिर्गमनम्.w*बहिर्गमनम्.w;
                        बहिर्गमनम्.h = बहिर्गमनम्.h*बहिर्गमनम्.h;
                    }

                    भिन्न iou  = box_iou(बहिर्गमनम्, truth);
                    //iou = 0;
                    भिन्न rmse = box_rmse(बहिर्गमनम्, truth);
                    यदि(best_iou > 0 || iou > 0){
                        यदि(iou > best_iou){
                            best_iou = iou;
                            best_सूचकांक = j;
                        }
                    }अन्यथा{
                        यदि(rmse < best_rmse){
                            best_rmse = rmse;
                            best_सूचकांक = j;
                        }
                    }
                }

                यदि(l.forced){
                    यदि(truth.w*truth.h < .1){
                        best_सूचकांक = 1;
                    }अन्यथा{
                        best_सूचकांक = 0;
                    }
                }
                यदि(l.random && *(net.आलोकितम्) < 64000){
                    best_सूचकांक = यादृच्छिकम्()%l.n;
                }

                पूर्णाङ्क box_सूचकांक = सूचकांक + locations*(l.classes + l.n) + (i*l.n + best_सूचकांक) * l.coords;
                पूर्णाङ्क tbox_सूचकांक = truth_सूचकांक + 1 + l.classes;

                box बहिर्गमनम् = float_गन्तव्यम्_box(l.output + box_सूचकांक, 1);
                बहिर्गमनम्.x /= l.side;
                बहिर्गमनम्.y /= l.side;
                यदि (l.वर्गमूलम्) {
                    बहिर्गमनम्.w = बहिर्गमनम्.w*बहिर्गमनम्.w;
                    बहिर्गमनम्.h = बहिर्गमनम्.h*बहिर्गमनम्.h;
                }
                भिन्न iou  = box_iou(बहिर्गमनम्, truth);

                //printf("%d,", best_index);
                पूर्णाङ्क p_सूचकांक = सूचकांक + locations*l.classes + i*l.n + best_सूचकांक;
                *(l.cost) -= l.noobject_scale * घातः(l.output[p_सूचकांक], 2);
                *(l.cost) += l.object_scale * घातः(1-l.output[p_सूचकांक], 2);
                avg_obj += l.output[p_सूचकांक];
                l.delta[p_सूचकांक] = l.object_scale * (1.-l.output[p_सूचकांक]);

                यदि(l.rescore){
                    l.delta[p_सूचकांक] = l.object_scale * (iou - l.output[p_सूचकांक]);
                }

                l.delta[box_सूचकांक+0] = l.coord_scale*(net.truth[tbox_सूचकांक + 0] - l.output[box_सूचकांक + 0]);
                l.delta[box_सूचकांक+1] = l.coord_scale*(net.truth[tbox_सूचकांक + 1] - l.output[box_सूचकांक + 1]);
                l.delta[box_सूचकांक+2] = l.coord_scale*(net.truth[tbox_सूचकांक + 2] - l.output[box_सूचकांक + 2]);
                l.delta[box_सूचकांक+3] = l.coord_scale*(net.truth[tbox_सूचकांक + 3] - l.output[box_सूचकांक + 3]);
                यदि(l.वर्गमूलम्){
                    l.delta[box_सूचकांक+2] = l.coord_scale*(वर्गमूलम्(net.truth[tbox_सूचकांक + 2]) - l.output[box_सूचकांक + 2]);
                    l.delta[box_सूचकांक+3] = l.coord_scale*(वर्गमूलम्(net.truth[tbox_सूचकांक + 3]) - l.output[box_सूचकांक + 3]);
                }

                *(l.cost) += घातः(1-iou, 2);
                avg_iou += iou;
                ++गणनम्;
            }
        }

        यदि(0){
            भिन्न *costs = शून्यस्मृतिं_देहि(l.batch*locations*l.n, परिमाण(भिन्न));
            प्रति (b = 0; b < l.batch; ++b) {
                पूर्णाङ्क सूचकांक = b*l.inputs;
                प्रति (i = 0; i < locations; ++i) {
                    प्रति (j = 0; j < l.n; ++j) {
                        पूर्णाङ्क p_सूचकांक = सूचकांक + locations*l.classes + i*l.n + j;
                        costs[b*locations*l.n + i*l.n + j] = l.delta[p_सूचकांक]*l.delta[p_सूचकांक];
                    }
                }
            }
            पूर्णाङ्क indexes[100];
            शिखरम्_k(costs, l.batch*locations*l.n, 100, indexes);
            भिन्न cutoff = costs[indexes[99]];
            प्रति (b = 0; b < l.batch; ++b) {
                पूर्णाङ्क सूचकांक = b*l.inputs;
                प्रति (i = 0; i < locations; ++i) {
                    प्रति (j = 0; j < l.n; ++j) {
                        पूर्णाङ्क p_सूचकांक = सूचकांक + locations*l.classes + i*l.n + j;
                        यदि (l.delta[p_सूचकांक]*l.delta[p_सूचकांक] < cutoff) l.delta[p_सूचकांक] = 0;
                    }
                }
            }
            मुक्त(costs);
        }


        *(l.cost) = घातः(mag_array(l.delta, l.outputs * l.batch), 2);


        प्रारूप_लिख("Detection Avg IOU: %f, Pos Cat: %f, All Cat: %f, Pos Obj: %f, Any Obj: %f, count: %d\n", avg_iou/गणनम्, avg_cat/गणनम्, avg_allcat/(गणनम्*l.classes), avg_obj/गणनम्, avg_anyobj/(l.batch*locations*l.n), गणनम्);
        //if(l.reorg) reorg(l.delta, l.w*l.h, size*l.n, l.batch, 0);
    }
}

शून्य backward_detection_layer(ध्रुव detection_layer l, जालम् net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

शून्य get_detection_detections(layer l, पूर्णाङ्क w, पूर्णाङ्क h, भिन्न thresh, detection *dets)
{
    पूर्णाङ्क i,j,n;
    भिन्न *predictions = l.output;
    //int per_cell = 5*num+classes;
    प्रति (i = 0; i < l.side*l.side; ++i){
        पूर्णाङ्क row = i / l.side;
        पूर्णाङ्क col = i % l.side;
        प्रति(n = 0; n < l.n; ++n){
            पूर्णाङ्क सूचकांक = i*l.n + n;
            पूर्णाङ्क p_सूचकांक = l.side*l.side*l.classes + i*l.n + n;
            भिन्न scale = predictions[p_सूचकांक];
            पूर्णाङ्क box_सूचकांक = l.side*l.side*(l.classes + l.n) + (i*l.n + n)*4;
            box b;
            b.x = (predictions[box_सूचकांक + 0] + col) / l.side * w;
            b.y = (predictions[box_सूचकांक + 1] + row) / l.side * h;
            b.w = घातः(predictions[box_सूचकांक + 2], (l.वर्गमूलम्?2:1)) * w;
            b.h = घातः(predictions[box_सूचकांक + 3], (l.वर्गमूलम्?2:1)) * h;
            dets[सूचकांक].bbox = b;
            dets[सूचकांक].objectness = scale;
            प्रति(j = 0; j < l.classes; ++j){
                पूर्णाङ्क class_सूचकांक = i*l.classes;
                भिन्न prob = scale*predictions[class_सूचकांक+j];
                dets[सूचकांक].prob[j] = (prob > thresh) ? prob : 0;
            }
        }
    }
}

#यदि_परिभाषितम् GPU

शून्य forward_detection_layer_gpu(ध्रुव detection_layer l, जालम् net)
{
    यदि(!net.train){
        copy_gpu(l.batch*l.inputs, net.निवेशः_gpu, 1, l.output_gpu, 1);
        प्रत्यागम;
    }

    cuda_pull_array(net.निवेशः_gpu, net.निवेशः, l.batch*l.inputs);
    forward_detection_layer(l, net);
    cuda_प्रक्षिप_array(l.output_gpu, l.output, l.batch*l.outputs);
    cuda_प्रक्षिप_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

शून्य backward_detection_layer_gpu(detection_layer l, जालम् net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
    //copy_gpu(l.batch*l.inputs, l.delta_gpu, 1, net.delta_gpu, 1);
}
#यदि_अन्तः


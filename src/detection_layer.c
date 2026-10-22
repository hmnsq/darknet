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
    l.cost = सुस्मृति(1, परिमाण(भिन्न));
    l.outputs = l.inputs;
    l.truths = l.side*l.side*(1+l.coords+l.classes);
    l.output = सुस्मृति(batch*l.outputs, परिमाण(भिन्न));
    l.delta = सुस्मृति(batch*l.outputs, परिमाण(भिन्न));

    l.forward = forward_detection_layer;
    l.backward = backward_detection_layer;
#ifdef GPU
    l.forward_gpu = forward_detection_layer_gpu;
    l.backward_gpu = backward_detection_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#endif

    ख_लिखो(मानक_त्रुटि, "Detection Layer\n");
    बेक्रम(0);

    प्रत्यागम l;
}

शून्य forward_detection_layer(ध्रुव detection_layer l, network net)
{
    पूर्णाङ्क locations = l.side*l.side;
    पूर्णाङ्क i,j;
    स_नकल(l.output, net.निवेश, l.outputs*l.batch*परिमाण(भिन्न));
    //if(l.reorg) reorg(l.output, l.w*l.h, size*l.n, l.batch, 1);
    पूर्णाङ्क b;
    यदि (l.softmax){
        प्रति(b = 0; b < l.batch; ++b){
            पूर्णाङ्क index = b*l.inputs;
            प्रति (i = 0; i < locations; ++i) {
                पूर्णाङ्क offset = i*l.classes;
                softmax(l.output + index + offset, l.classes, 1, 1,
                        l.output + index + offset);
            }
        }
    }
    यदि(net.train){
        भिन्न avg_iou = 0;
        भिन्न avg_cat = 0;
        भिन्न avg_allcat = 0;
        भिन्न avg_obj = 0;
        भिन्न avg_anyobj = 0;
        पूर्णाङ्क गिनती = 0;
        *(l.cost) = 0;
        पूर्णाङ्क size = l.inputs * l.batch;
        स_रखो(l.delta, 0, size * परिमाण(भिन्न));
        प्रति (b = 0; b < l.batch; ++b){
            पूर्णाङ्क index = b*l.inputs;
            प्रति (i = 0; i < locations; ++i) {
                पूर्णाङ्क truth_index = (b*locations + i)*(1+l.coords+l.classes);
                पूर्णाङ्क is_obj = net.truth[truth_index];
                प्रति (j = 0; j < l.n; ++j) {
                    पूर्णाङ्क p_index = index + locations*l.classes + i*l.n + j;
                    l.delta[p_index] = l.noobject_scale*(0 - l.output[p_index]);
                    *(l.cost) += l.noobject_scale*घात(l.output[p_index], 2);
                    avg_anyobj += l.output[p_index];
                }

                पूर्णाङ्क best_index = -1;
                भिन्न best_iou = 0;
                भिन्न best_rmse = 20;

                यदि (!is_obj){
                    अनुवर्त;
                }

                पूर्णाङ्क class_index = index + i*l.classes;
                प्रति(j = 0; j < l.classes; ++j) {
                    l.delta[class_index+j] = l.class_scale * (net.truth[truth_index+1+j] - l.output[class_index+j]);
                    *(l.cost) += l.class_scale * घात(net.truth[truth_index+1+j] - l.output[class_index+j], 2);
                    यदि(net.truth[truth_index + 1 + j]) avg_cat += l.output[class_index+j];
                    avg_allcat += l.output[class_index+j];
                }

                box truth = float_to_box(net.truth + truth_index + 1 + l.classes, 1);
                truth.x /= l.side;
                truth.y /= l.side;

                प्रति(j = 0; j < l.n; ++j){
                    पूर्णाङ्क box_index = index + locations*(l.classes + l.n) + (i*l.n + j) * l.coords;
                    box out = float_to_box(l.output + box_index, 1);
                    out.x /= l.side;
                    out.y /= l.side;

                    यदि (l.वर्ग_मूल){
                        out.w = out.w*out.w;
                        out.h = out.h*out.h;
                    }

                    भिन्न iou  = box_iou(out, truth);
                    //iou = 0;
                    भिन्न rmse = box_rmse(out, truth);
                    यदि(best_iou > 0 || iou > 0){
                        यदि(iou > best_iou){
                            best_iou = iou;
                            best_index = j;
                        }
                    }अन्यथा{
                        यदि(rmse < best_rmse){
                            best_rmse = rmse;
                            best_index = j;
                        }
                    }
                }

                यदि(l.forced){
                    यदि(truth.w*truth.h < .1){
                        best_index = 1;
                    }अन्यथा{
                        best_index = 0;
                    }
                }
                यदि(l.random && *(net.seen) < 64000){
                    best_index = अक्रम()%l.n;
                }

                पूर्णाङ्क box_index = index + locations*(l.classes + l.n) + (i*l.n + best_index) * l.coords;
                पूर्णाङ्क tbox_index = truth_index + 1 + l.classes;

                box out = float_to_box(l.output + box_index, 1);
                out.x /= l.side;
                out.y /= l.side;
                यदि (l.वर्ग_मूल) {
                    out.w = out.w*out.w;
                    out.h = out.h*out.h;
                }
                भिन्न iou  = box_iou(out, truth);

                //printf("%d,", best_index);
                पूर्णाङ्क p_index = index + locations*l.classes + i*l.n + best_index;
                *(l.cost) -= l.noobject_scale * घात(l.output[p_index], 2);
                *(l.cost) += l.object_scale * घात(1-l.output[p_index], 2);
                avg_obj += l.output[p_index];
                l.delta[p_index] = l.object_scale * (1.-l.output[p_index]);

                यदि(l.rescore){
                    l.delta[p_index] = l.object_scale * (iou - l.output[p_index]);
                }

                l.delta[box_index+0] = l.coord_scale*(net.truth[tbox_index + 0] - l.output[box_index + 0]);
                l.delta[box_index+1] = l.coord_scale*(net.truth[tbox_index + 1] - l.output[box_index + 1]);
                l.delta[box_index+2] = l.coord_scale*(net.truth[tbox_index + 2] - l.output[box_index + 2]);
                l.delta[box_index+3] = l.coord_scale*(net.truth[tbox_index + 3] - l.output[box_index + 3]);
                यदि(l.वर्ग_मूल){
                    l.delta[box_index+2] = l.coord_scale*(वर्ग_मूल(net.truth[tbox_index + 2]) - l.output[box_index + 2]);
                    l.delta[box_index+3] = l.coord_scale*(वर्ग_मूल(net.truth[tbox_index + 3]) - l.output[box_index + 3]);
                }

                *(l.cost) += घात(1-iou, 2);
                avg_iou += iou;
                ++गिनती;
            }
        }

        यदि(0){
            भिन्न *costs = सुस्मृति(l.batch*locations*l.n, परिमाण(भिन्न));
            प्रति (b = 0; b < l.batch; ++b) {
                पूर्णाङ्क index = b*l.inputs;
                प्रति (i = 0; i < locations; ++i) {
                    प्रति (j = 0; j < l.n; ++j) {
                        पूर्णाङ्क p_index = index + locations*l.classes + i*l.n + j;
                        costs[b*locations*l.n + i*l.n + j] = l.delta[p_index]*l.delta[p_index];
                    }
                }
            }
            पूर्णाङ्क indexes[100];
            शीर्ष_k(costs, l.batch*locations*l.n, 100, indexes);
            भिन्न cutoff = costs[indexes[99]];
            प्रति (b = 0; b < l.batch; ++b) {
                पूर्णाङ्क index = b*l.inputs;
                प्रति (i = 0; i < locations; ++i) {
                    प्रति (j = 0; j < l.n; ++j) {
                        पूर्णाङ्क p_index = index + locations*l.classes + i*l.n + j;
                        यदि (l.delta[p_index]*l.delta[p_index] < cutoff) l.delta[p_index] = 0;
                    }
                }
            }
            मुक्त(costs);
        }


        *(l.cost) = घात(mag_array(l.delta, l.outputs * l.batch), 2);


        म_लिखो("Detection Avg IOU: %f, Pos Cat: %f, All Cat: %f, Pos Obj: %f, Any Obj: %f, count: %d\n", avg_iou/गिनती, avg_cat/गिनती, avg_allcat/(गिनती*l.classes), avg_obj/गिनती, avg_anyobj/(l.batch*locations*l.n), गिनती);
        //if(l.reorg) reorg(l.delta, l.w*l.h, size*l.n, l.batch, 0);
    }
}

शून्य backward_detection_layer(ध्रुव detection_layer l, network net)
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
            पूर्णाङ्क index = i*l.n + n;
            पूर्णाङ्क p_index = l.side*l.side*l.classes + i*l.n + n;
            भिन्न scale = predictions[p_index];
            पूर्णाङ्क box_index = l.side*l.side*(l.classes + l.n) + (i*l.n + n)*4;
            box b;
            b.x = (predictions[box_index + 0] + col) / l.side * w;
            b.y = (predictions[box_index + 1] + row) / l.side * h;
            b.w = घात(predictions[box_index + 2], (l.वर्ग_मूल?2:1)) * w;
            b.h = घात(predictions[box_index + 3], (l.वर्ग_मूल?2:1)) * h;
            dets[index].bbox = b;
            dets[index].objectness = scale;
            प्रति(j = 0; j < l.classes; ++j){
                पूर्णाङ्क class_index = i*l.classes;
                भिन्न prob = scale*predictions[class_index+j];
                dets[index].prob[j] = (prob > thresh) ? prob : 0;
            }
        }
    }
}

#ifdef GPU

शून्य forward_detection_layer_gpu(ध्रुव detection_layer l, network net)
{
    यदि(!net.train){
        copy_gpu(l.batch*l.inputs, net.निवेश_gpu, 1, l.output_gpu, 1);
        प्रत्यागम;
    }

    cuda_pull_array(net.निवेश_gpu, net.निवेश, l.batch*l.inputs);
    forward_detection_layer(l, net);
    cuda_धकेलो_array(l.output_gpu, l.output, l.batch*l.outputs);
    cuda_धकेलो_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

शून्य backward_detection_layer_gpu(detection_layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
    //copy_gpu(l.batch*l.inputs, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


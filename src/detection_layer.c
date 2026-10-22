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

detection_layer make_detection_layer(पूर्णांक batch, पूर्णांक inputs, पूर्णांक n, पूर्णांक side, पूर्णांक classes, पूर्णांक coords, पूर्णांक rescore)
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
    l.cost = सुस्मृति(1, आकार(दशांश));
    l.outputs = l.inputs;
    l.truths = l.side*l.side*(1+l.coords+l.classes);
    l.output = सुस्मृति(batch*l.outputs, आकार(दशांश));
    l.delta = सुस्मृति(batch*l.outputs, आकार(दशांश));

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

    परत l;
}

शून्य forward_detection_layer(स्थिरांक detection_layer l, network net)
{
    पूर्णांक locations = l.side*l.side;
    पूर्णांक i,j;
    स_नकल(l.output, net.आदान, l.outputs*l.batch*आकार(दशांश));
    //if(l.reorg) reorg(l.output, l.w*l.h, size*l.n, l.batch, 1);
    पूर्णांक b;
    जर (l.softmax){
        साठी(b = 0; b < l.batch; ++b){
            पूर्णांक index = b*l.inputs;
            साठी (i = 0; i < locations; ++i) {
                पूर्णांक offset = i*l.classes;
                softmax(l.output + index + offset, l.classes, 1, 1,
                        l.output + index + offset);
            }
        }
    }
    जर(net.train){
        दशांश avg_iou = 0;
        दशांश avg_cat = 0;
        दशांश avg_allcat = 0;
        दशांश avg_obj = 0;
        दशांश avg_anyobj = 0;
        पूर्णांक मोजणी = 0;
        *(l.cost) = 0;
        पूर्णांक size = l.inputs * l.batch;
        स_रखो(l.delta, 0, size * आकार(दशांश));
        साठी (b = 0; b < l.batch; ++b){
            पूर्णांक index = b*l.inputs;
            साठी (i = 0; i < locations; ++i) {
                पूर्णांक truth_index = (b*locations + i)*(1+l.coords+l.classes);
                पूर्णांक is_obj = net.truth[truth_index];
                साठी (j = 0; j < l.n; ++j) {
                    पूर्णांक p_index = index + locations*l.classes + i*l.n + j;
                    l.delta[p_index] = l.noobject_scale*(0 - l.output[p_index]);
                    *(l.cost) += l.noobject_scale*घात(l.output[p_index], 2);
                    avg_anyobj += l.output[p_index];
                }

                पूर्णांक best_index = -1;
                दशांश best_iou = 0;
                दशांश best_rmse = 20;

                जर (!is_obj){
                    सुरूठेवा;
                }

                पूर्णांक class_index = index + i*l.classes;
                साठी(j = 0; j < l.classes; ++j) {
                    l.delta[class_index+j] = l.class_scale * (net.truth[truth_index+1+j] - l.output[class_index+j]);
                    *(l.cost) += l.class_scale * घात(net.truth[truth_index+1+j] - l.output[class_index+j], 2);
                    जर(net.truth[truth_index + 1 + j]) avg_cat += l.output[class_index+j];
                    avg_allcat += l.output[class_index+j];
                }

                box truth = float_to_box(net.truth + truth_index + 1 + l.classes, 1);
                truth.x /= l.side;
                truth.y /= l.side;

                साठी(j = 0; j < l.n; ++j){
                    पूर्णांक box_index = index + locations*(l.classes + l.n) + (i*l.n + j) * l.coords;
                    box out = float_to_box(l.output + box_index, 1);
                    out.x /= l.side;
                    out.y /= l.side;

                    जर (l.वर्ग_मूल){
                        out.w = out.w*out.w;
                        out.h = out.h*out.h;
                    }

                    दशांश iou  = box_iou(out, truth);
                    //iou = 0;
                    दशांश rmse = box_rmse(out, truth);
                    जर(best_iou > 0 || iou > 0){
                        जर(iou > best_iou){
                            best_iou = iou;
                            best_index = j;
                        }
                    }अन्यथा{
                        जर(rmse < best_rmse){
                            best_rmse = rmse;
                            best_index = j;
                        }
                    }
                }

                जर(l.forced){
                    जर(truth.w*truth.h < .1){
                        best_index = 1;
                    }अन्यथा{
                        best_index = 0;
                    }
                }
                जर(l.random && *(net.seen) < 64000){
                    best_index = अक्रम()%l.n;
                }

                पूर्णांक box_index = index + locations*(l.classes + l.n) + (i*l.n + best_index) * l.coords;
                पूर्णांक tbox_index = truth_index + 1 + l.classes;

                box out = float_to_box(l.output + box_index, 1);
                out.x /= l.side;
                out.y /= l.side;
                जर (l.वर्ग_मूल) {
                    out.w = out.w*out.w;
                    out.h = out.h*out.h;
                }
                दशांश iou  = box_iou(out, truth);

                //printf("%d,", best_index);
                पूर्णांक p_index = index + locations*l.classes + i*l.n + best_index;
                *(l.cost) -= l.noobject_scale * घात(l.output[p_index], 2);
                *(l.cost) += l.object_scale * घात(1-l.output[p_index], 2);
                avg_obj += l.output[p_index];
                l.delta[p_index] = l.object_scale * (1.-l.output[p_index]);

                जर(l.rescore){
                    l.delta[p_index] = l.object_scale * (iou - l.output[p_index]);
                }

                l.delta[box_index+0] = l.coord_scale*(net.truth[tbox_index + 0] - l.output[box_index + 0]);
                l.delta[box_index+1] = l.coord_scale*(net.truth[tbox_index + 1] - l.output[box_index + 1]);
                l.delta[box_index+2] = l.coord_scale*(net.truth[tbox_index + 2] - l.output[box_index + 2]);
                l.delta[box_index+3] = l.coord_scale*(net.truth[tbox_index + 3] - l.output[box_index + 3]);
                जर(l.वर्ग_मूल){
                    l.delta[box_index+2] = l.coord_scale*(वर्ग_मूल(net.truth[tbox_index + 2]) - l.output[box_index + 2]);
                    l.delta[box_index+3] = l.coord_scale*(वर्ग_मूल(net.truth[tbox_index + 3]) - l.output[box_index + 3]);
                }

                *(l.cost) += घात(1-iou, 2);
                avg_iou += iou;
                ++मोजणी;
            }
        }

        जर(0){
            दशांश *costs = सुस्मृति(l.batch*locations*l.n, आकार(दशांश));
            साठी (b = 0; b < l.batch; ++b) {
                पूर्णांक index = b*l.inputs;
                साठी (i = 0; i < locations; ++i) {
                    साठी (j = 0; j < l.n; ++j) {
                        पूर्णांक p_index = index + locations*l.classes + i*l.n + j;
                        costs[b*locations*l.n + i*l.n + j] = l.delta[p_index]*l.delta[p_index];
                    }
                }
            }
            पूर्णांक indexes[100];
            शीर्ष_k(costs, l.batch*locations*l.n, 100, indexes);
            दशांश cutoff = costs[indexes[99]];
            साठी (b = 0; b < l.batch; ++b) {
                पूर्णांक index = b*l.inputs;
                साठी (i = 0; i < locations; ++i) {
                    साठी (j = 0; j < l.n; ++j) {
                        पूर्णांक p_index = index + locations*l.classes + i*l.n + j;
                        जर (l.delta[p_index]*l.delta[p_index] < cutoff) l.delta[p_index] = 0;
                    }
                }
            }
            मुक्त(costs);
        }


        *(l.cost) = घात(mag_array(l.delta, l.outputs * l.batch), 2);


        म_लिखो("Detection Avg IOU: %f, Pos Cat: %f, All Cat: %f, Pos Obj: %f, Any Obj: %f, count: %d\n", avg_iou/मोजणी, avg_cat/मोजणी, avg_allcat/(मोजणी*l.classes), avg_obj/मोजणी, avg_anyobj/(l.batch*locations*l.n), मोजणी);
        //if(l.reorg) reorg(l.delta, l.w*l.h, size*l.n, l.batch, 0);
    }
}

शून्य backward_detection_layer(स्थिरांक detection_layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

शून्य get_detection_detections(layer l, पूर्णांक w, पूर्णांक h, दशांश thresh, detection *dets)
{
    पूर्णांक i,j,n;
    दशांश *predictions = l.output;
    //int per_cell = 5*num+classes;
    साठी (i = 0; i < l.side*l.side; ++i){
        पूर्णांक row = i / l.side;
        पूर्णांक col = i % l.side;
        साठी(n = 0; n < l.n; ++n){
            पूर्णांक index = i*l.n + n;
            पूर्णांक p_index = l.side*l.side*l.classes + i*l.n + n;
            दशांश scale = predictions[p_index];
            पूर्णांक box_index = l.side*l.side*(l.classes + l.n) + (i*l.n + n)*4;
            box b;
            b.x = (predictions[box_index + 0] + col) / l.side * w;
            b.y = (predictions[box_index + 1] + row) / l.side * h;
            b.w = घात(predictions[box_index + 2], (l.वर्ग_मूल?2:1)) * w;
            b.h = घात(predictions[box_index + 3], (l.वर्ग_मूल?2:1)) * h;
            dets[index].bbox = b;
            dets[index].objectness = scale;
            साठी(j = 0; j < l.classes; ++j){
                पूर्णांक class_index = i*l.classes;
                दशांश prob = scale*predictions[class_index+j];
                dets[index].prob[j] = (prob > thresh) ? prob : 0;
            }
        }
    }
}

#ifdef GPU

शून्य forward_detection_layer_gpu(स्थिरांक detection_layer l, network net)
{
    जर(!net.train){
        copy_gpu(l.batch*l.inputs, net.आदान_gpu, 1, l.output_gpu, 1);
        परत;
    }

    cuda_pull_array(net.आदान_gpu, net.आदान, l.batch*l.inputs);
    forward_detection_layer(l, net);
    cuda_ढकला_array(l.output_gpu, l.output, l.batch*l.outputs);
    cuda_ढकला_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

शून्य backward_detection_layer_gpu(detection_layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
    //copy_gpu(l.batch*l.inputs, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


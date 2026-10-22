#समावेश "detection_layer.h"
#समावेश "activations.h"
#समावेश "softmax_layer.h"
#समावेश "blas.h"
#समावेश "box.h"
#समावेश "cuda.h"
#समावेश "utils.h"

#समावेश <stdio.h>
#समावेश <assert.h>
#समावेश <string.h>
#समावेश <stdlib.h>

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
    निश्चित(side*side*((1 + l.coords)*l.n + l.classes) == inputs);
    l.cost = सुस्मृति(1, माप(भग्न));
    l.outputs = l.inputs;
    l.truths = l.side*l.side*(1+l.coords+l.classes);
    l.output = सुस्मृति(batch*l.outputs, माप(भग्न));
    l.delta = सुस्मृति(batch*l.outputs, माप(भग्न));

    l.forward = forward_detection_layer;
    l.backward = backward_detection_layer;
#अगर_घोषित GPU
    l.forward_gpu = forward_detection_layer_gpu;
    l.backward_gpu = backward_detection_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#पूर्ण_अगर

    ख_लिखो(मानक_त्रुटि, "Detection Layer\n");
    बेक्रम(0);

    वापस l;
}

व्योम forward_detection_layer(स्थिर detection_layer l, network net)
{
    पूर्णांक locations = l.side*l.side;
    पूर्णांक i,j;
    स_नकल(l.output, net.निवेश, l.outputs*l.batch*माप(भग्न));
    //if(l.reorg) reorg(l.output, l.w*l.h, size*l.n, l.batch, 1);
    पूर्णांक b;
    अगर (l.softmax){
        क्रम(b = 0; b < l.batch; ++b){
            पूर्णांक सूचकांक = b*l.inputs;
            क्रम (i = 0; i < locations; ++i) {
                पूर्णांक offset = i*l.classes;
                softmax(l.output + सूचकांक + offset, l.classes, 1, 1,
                        l.output + सूचकांक + offset);
            }
        }
    }
    अगर(net.train){
        भग्न avg_iou = 0;
        भग्न avg_cat = 0;
        भग्न avg_allcat = 0;
        भग्न avg_obj = 0;
        भग्न avg_anyobj = 0;
        पूर्णांक गिनती = 0;
        *(l.cost) = 0;
        पूर्णांक आकार = l.inputs * l.batch;
        स_रखो(l.delta, 0, आकार * माप(भग्न));
        क्रम (b = 0; b < l.batch; ++b){
            पूर्णांक सूचकांक = b*l.inputs;
            क्रम (i = 0; i < locations; ++i) {
                पूर्णांक truth_सूचकांक = (b*locations + i)*(1+l.coords+l.classes);
                पूर्णांक is_obj = net.truth[truth_सूचकांक];
                क्रम (j = 0; j < l.n; ++j) {
                    पूर्णांक p_सूचकांक = सूचकांक + locations*l.classes + i*l.n + j;
                    l.delta[p_सूचकांक] = l.noobject_scale*(0 - l.output[p_सूचकांक]);
                    *(l.cost) += l.noobject_scale*घात(l.output[p_सूचकांक], 2);
                    avg_anyobj += l.output[p_सूचकांक];
                }

                पूर्णांक best_सूचकांक = -1;
                भग्न best_iou = 0;
                भग्न best_rmse = 20;

                अगर (!is_obj){
                    जारी;
                }

                पूर्णांक class_सूचकांक = सूचकांक + i*l.classes;
                क्रम(j = 0; j < l.classes; ++j) {
                    l.delta[class_सूचकांक+j] = l.class_scale * (net.truth[truth_सूचकांक+1+j] - l.output[class_सूचकांक+j]);
                    *(l.cost) += l.class_scale * घात(net.truth[truth_सूचकांक+1+j] - l.output[class_सूचकांक+j], 2);
                    अगर(net.truth[truth_सूचकांक + 1 + j]) avg_cat += l.output[class_सूचकांक+j];
                    avg_allcat += l.output[class_सूचकांक+j];
                }

                box truth = float_to_box(net.truth + truth_सूचकांक + 1 + l.classes, 1);
                truth.x /= l.side;
                truth.y /= l.side;

                क्रम(j = 0; j < l.n; ++j){
                    पूर्णांक box_सूचकांक = सूचकांक + locations*(l.classes + l.n) + (i*l.n + j) * l.coords;
                    box out = float_to_box(l.output + box_सूचकांक, 1);
                    out.x /= l.side;
                    out.y /= l.side;

                    अगर (l.वर्ग_मूल){
                        out.w = out.w*out.w;
                        out.h = out.h*out.h;
                    }

                    भग्न iou  = box_iou(out, truth);
                    //iou = 0;
                    भग्न rmse = box_rmse(out, truth);
                    अगर(best_iou > 0 || iou > 0){
                        अगर(iou > best_iou){
                            best_iou = iou;
                            best_सूचकांक = j;
                        }
                    }अन्यथा{
                        अगर(rmse < best_rmse){
                            best_rmse = rmse;
                            best_सूचकांक = j;
                        }
                    }
                }

                अगर(l.forced){
                    अगर(truth.w*truth.h < .1){
                        best_सूचकांक = 1;
                    }अन्यथा{
                        best_सूचकांक = 0;
                    }
                }
                अगर(l.random && *(net.seen) < 64000){
                    best_सूचकांक = अक्रम()%l.n;
                }

                पूर्णांक box_सूचकांक = सूचकांक + locations*(l.classes + l.n) + (i*l.n + best_सूचकांक) * l.coords;
                पूर्णांक tbox_सूचकांक = truth_सूचकांक + 1 + l.classes;

                box out = float_to_box(l.output + box_सूचकांक, 1);
                out.x /= l.side;
                out.y /= l.side;
                अगर (l.वर्ग_मूल) {
                    out.w = out.w*out.w;
                    out.h = out.h*out.h;
                }
                भग्न iou  = box_iou(out, truth);

                //printf("%d,", best_index);
                पूर्णांक p_सूचकांक = सूचकांक + locations*l.classes + i*l.n + best_सूचकांक;
                *(l.cost) -= l.noobject_scale * घात(l.output[p_सूचकांक], 2);
                *(l.cost) += l.object_scale * घात(1-l.output[p_सूचकांक], 2);
                avg_obj += l.output[p_सूचकांक];
                l.delta[p_सूचकांक] = l.object_scale * (1.-l.output[p_सूचकांक]);

                अगर(l.rescore){
                    l.delta[p_सूचकांक] = l.object_scale * (iou - l.output[p_सूचकांक]);
                }

                l.delta[box_सूचकांक+0] = l.coord_scale*(net.truth[tbox_सूचकांक + 0] - l.output[box_सूचकांक + 0]);
                l.delta[box_सूचकांक+1] = l.coord_scale*(net.truth[tbox_सूचकांक + 1] - l.output[box_सूचकांक + 1]);
                l.delta[box_सूचकांक+2] = l.coord_scale*(net.truth[tbox_सूचकांक + 2] - l.output[box_सूचकांक + 2]);
                l.delta[box_सूचकांक+3] = l.coord_scale*(net.truth[tbox_सूचकांक + 3] - l.output[box_सूचकांक + 3]);
                अगर(l.वर्ग_मूल){
                    l.delta[box_सूचकांक+2] = l.coord_scale*(वर्ग_मूल(net.truth[tbox_सूचकांक + 2]) - l.output[box_सूचकांक + 2]);
                    l.delta[box_सूचकांक+3] = l.coord_scale*(वर्ग_मूल(net.truth[tbox_सूचकांक + 3]) - l.output[box_सूचकांक + 3]);
                }

                *(l.cost) += घात(1-iou, 2);
                avg_iou += iou;
                ++गिनती;
            }
        }

        अगर(0){
            भग्न *costs = सुस्मृति(l.batch*locations*l.n, माप(भग्न));
            क्रम (b = 0; b < l.batch; ++b) {
                पूर्णांक सूचकांक = b*l.inputs;
                क्रम (i = 0; i < locations; ++i) {
                    क्रम (j = 0; j < l.n; ++j) {
                        पूर्णांक p_सूचकांक = सूचकांक + locations*l.classes + i*l.n + j;
                        costs[b*locations*l.n + i*l.n + j] = l.delta[p_सूचकांक]*l.delta[p_सूचकांक];
                    }
                }
            }
            पूर्णांक indexes[100];
            शीर्ष_k(costs, l.batch*locations*l.n, 100, indexes);
            भग्न cutoff = costs[indexes[99]];
            क्रम (b = 0; b < l.batch; ++b) {
                पूर्णांक सूचकांक = b*l.inputs;
                क्रम (i = 0; i < locations; ++i) {
                    क्रम (j = 0; j < l.n; ++j) {
                        पूर्णांक p_सूचकांक = सूचकांक + locations*l.classes + i*l.n + j;
                        अगर (l.delta[p_सूचकांक]*l.delta[p_सूचकांक] < cutoff) l.delta[p_सूचकांक] = 0;
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

व्योम backward_detection_layer(स्थिर detection_layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

व्योम get_detection_detections(layer l, पूर्णांक w, पूर्णांक h, भग्न thresh, detection *dets)
{
    पूर्णांक i,j,n;
    भग्न *predictions = l.output;
    //int per_cell = 5*num+classes;
    क्रम (i = 0; i < l.side*l.side; ++i){
        पूर्णांक row = i / l.side;
        पूर्णांक col = i % l.side;
        क्रम(n = 0; n < l.n; ++n){
            पूर्णांक सूचकांक = i*l.n + n;
            पूर्णांक p_सूचकांक = l.side*l.side*l.classes + i*l.n + n;
            भग्न scale = predictions[p_सूचकांक];
            पूर्णांक box_सूचकांक = l.side*l.side*(l.classes + l.n) + (i*l.n + n)*4;
            box b;
            b.x = (predictions[box_सूचकांक + 0] + col) / l.side * w;
            b.y = (predictions[box_सूचकांक + 1] + row) / l.side * h;
            b.w = घात(predictions[box_सूचकांक + 2], (l.वर्ग_मूल?2:1)) * w;
            b.h = घात(predictions[box_सूचकांक + 3], (l.वर्ग_मूल?2:1)) * h;
            dets[सूचकांक].bbox = b;
            dets[सूचकांक].objectness = scale;
            क्रम(j = 0; j < l.classes; ++j){
                पूर्णांक class_सूचकांक = i*l.classes;
                भग्न prob = scale*predictions[class_सूचकांक+j];
                dets[सूचकांक].prob[j] = (prob > thresh) ? prob : 0;
            }
        }
    }
}

#अगर_घोषित GPU

व्योम forward_detection_layer_gpu(स्थिर detection_layer l, network net)
{
    अगर(!net.train){
        copy_gpu(l.batch*l.inputs, net.निवेश_gpu, 1, l.output_gpu, 1);
        वापस;
    }

    cuda_pull_array(net.निवेश_gpu, net.निवेश, l.batch*l.inputs);
    forward_detection_layer(l, net);
    cuda_धकेलो_array(l.output_gpu, l.output, l.batch*l.outputs);
    cuda_धकेलो_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

व्योम backward_detection_layer_gpu(detection_layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
    //copy_gpu(l.batch*l.inputs, l.delta_gpu, 1, net.delta_gpu, 1);
}
#पूर्ण_अगर


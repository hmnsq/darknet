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
    l.cost = calloc(1, حجم(عائم));
    l.outputs = l.inputs;
    l.truths = l.side*l.side*(1+l.coords+l.classes);
    l.output = calloc(batch*l.outputs, حجم(عائم));
    l.delta = calloc(batch*l.outputs, حجم(عائم));

    l.forward = forward_detection_layer;
    l.backward = backward_detection_layer;
#ifdef GPU
    l.forward_gpu = forward_detection_layer_gpu;
    l.backward_gpu = backward_detection_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#endif

    fprintf(stderr, "Detection Layer\n");
    srand(0);

    أرجع l;
}

فراغ forward_detection_layer(ثابت detection_layer l, network net)
{
    صحيح locations = l.side*l.side;
    صحيح i,j;
    memcpy(l.output, net.input, l.outputs*l.batch*حجم(عائم));
    //if(l.reorg) reorg(l.output, l.w*l.h, size*l.n, l.batch, 1);
    صحيح b;
    𐤀𐤌 (l.softmax){
        لكل(b = 0; b < l.batch; ++b){
            صحيح index = b*l.inputs;
            لكل (i = 0; i < locations; ++i) {
                صحيح offset = i*l.classes;
                softmax(l.output + index + offset, l.classes, 1, 1,
                        l.output + index + offset);
            }
        }
    }
    𐤀𐤌(net.train){
        عائم avg_iou = 0;
        عائم avg_cat = 0;
        عائم avg_allcat = 0;
        عائم avg_obj = 0;
        عائم avg_anyobj = 0;
        صحيح count = 0;
        *(l.cost) = 0;
        صحيح size = l.inputs * l.batch;
        memset(l.delta, 0, size * حجم(عائم));
        لكل (b = 0; b < l.batch; ++b){
            صحيح index = b*l.inputs;
            لكل (i = 0; i < locations; ++i) {
                صحيح truth_index = (b*locations + i)*(1+l.coords+l.classes);
                صحيح is_obj = net.truth[truth_index];
                لكل (j = 0; j < l.n; ++j) {
                    صحيح p_index = index + locations*l.classes + i*l.n + j;
                    l.delta[p_index] = l.noobject_scale*(0 - l.output[p_index]);
                    *(l.cost) += l.noobject_scale*pow(l.output[p_index], 2);
                    avg_anyobj += l.output[p_index];
                }

                صحيح best_index = -1;
                عائم best_iou = 0;
                عائم best_rmse = 20;

                𐤀𐤌 (!is_obj){
                    استمر;
                }

                صحيح class_index = index + i*l.classes;
                لكل(j = 0; j < l.classes; ++j) {
                    l.delta[class_index+j] = l.class_scale * (net.truth[truth_index+1+j] - l.output[class_index+j]);
                    *(l.cost) += l.class_scale * pow(net.truth[truth_index+1+j] - l.output[class_index+j], 2);
                    𐤀𐤌(net.truth[truth_index + 1 + j]) avg_cat += l.output[class_index+j];
                    avg_allcat += l.output[class_index+j];
                }

                box truth = float_to_box(net.truth + truth_index + 1 + l.classes, 1);
                truth.x /= l.side;
                truth.y /= l.side;

                لكل(j = 0; j < l.n; ++j){
                    صحيح box_index = index + locations*(l.classes + l.n) + (i*l.n + j) * l.coords;
                    box out = float_to_box(l.output + box_index, 1);
                    out.x /= l.side;
                    out.y /= l.side;

                    𐤀𐤌 (l.sqrt){
                        out.w = out.w*out.w;
                        out.h = out.h*out.h;
                    }

                    عائم iou  = box_iou(out, truth);
                    //iou = 0;
                    عائم rmse = box_rmse(out, truth);
                    𐤀𐤌(best_iou > 0 || iou > 0){
                        𐤀𐤌(iou > best_iou){
                            best_iou = iou;
                            best_index = j;
                        }
                    }وإلا{
                        𐤀𐤌(rmse < best_rmse){
                            best_rmse = rmse;
                            best_index = j;
                        }
                    }
                }

                𐤀𐤌(l.forced){
                    𐤀𐤌(truth.w*truth.h < .1){
                        best_index = 1;
                    }وإلا{
                        best_index = 0;
                    }
                }
                𐤀𐤌(l.random && *(net.seen) < 64000){
                    best_index = rand()%l.n;
                }

                صحيح box_index = index + locations*(l.classes + l.n) + (i*l.n + best_index) * l.coords;
                صحيح tbox_index = truth_index + 1 + l.classes;

                box out = float_to_box(l.output + box_index, 1);
                out.x /= l.side;
                out.y /= l.side;
                𐤀𐤌 (l.sqrt) {
                    out.w = out.w*out.w;
                    out.h = out.h*out.h;
                }
                عائم iou  = box_iou(out, truth);

                //printf("%d,", best_index);
                صحيح p_index = index + locations*l.classes + i*l.n + best_index;
                *(l.cost) -= l.noobject_scale * pow(l.output[p_index], 2);
                *(l.cost) += l.object_scale * pow(1-l.output[p_index], 2);
                avg_obj += l.output[p_index];
                l.delta[p_index] = l.object_scale * (1.-l.output[p_index]);

                𐤀𐤌(l.rescore){
                    l.delta[p_index] = l.object_scale * (iou - l.output[p_index]);
                }

                l.delta[box_index+0] = l.coord_scale*(net.truth[tbox_index + 0] - l.output[box_index + 0]);
                l.delta[box_index+1] = l.coord_scale*(net.truth[tbox_index + 1] - l.output[box_index + 1]);
                l.delta[box_index+2] = l.coord_scale*(net.truth[tbox_index + 2] - l.output[box_index + 2]);
                l.delta[box_index+3] = l.coord_scale*(net.truth[tbox_index + 3] - l.output[box_index + 3]);
                𐤀𐤌(l.sqrt){
                    l.delta[box_index+2] = l.coord_scale*(sqrt(net.truth[tbox_index + 2]) - l.output[box_index + 2]);
                    l.delta[box_index+3] = l.coord_scale*(sqrt(net.truth[tbox_index + 3]) - l.output[box_index + 3]);
                }

                *(l.cost) += pow(1-iou, 2);
                avg_iou += iou;
                ++count;
            }
        }

        𐤀𐤌(0){
            عائم *costs = calloc(l.batch*locations*l.n, حجم(عائم));
            لكل (b = 0; b < l.batch; ++b) {
                صحيح index = b*l.inputs;
                لكل (i = 0; i < locations; ++i) {
                    لكل (j = 0; j < l.n; ++j) {
                        صحيح p_index = index + locations*l.classes + i*l.n + j;
                        costs[b*locations*l.n + i*l.n + j] = l.delta[p_index]*l.delta[p_index];
                    }
                }
            }
            صحيح indexes[100];
            top_k(costs, l.batch*locations*l.n, 100, indexes);
            عائم cutoff = costs[indexes[99]];
            لكل (b = 0; b < l.batch; ++b) {
                صحيح index = b*l.inputs;
                لكل (i = 0; i < locations; ++i) {
                    لكل (j = 0; j < l.n; ++j) {
                        صحيح p_index = index + locations*l.classes + i*l.n + j;
                        𐤀𐤌 (l.delta[p_index]*l.delta[p_index] < cutoff) l.delta[p_index] = 0;
                    }
                }
            }
            حرر(costs);
        }


        *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);


        printf("Detection Avg IOU: %f, Pos Cat: %f, All Cat: %f, Pos Obj: %f, Any Obj: %f, count: %d\n", avg_iou/count, avg_cat/count, avg_allcat/(count*l.classes), avg_obj/count, avg_anyobj/(l.batch*locations*l.n), count);
        //if(l.reorg) reorg(l.delta, l.w*l.h, size*l.n, l.batch, 0);
    }
}

فراغ backward_detection_layer(ثابت detection_layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

فراغ get_detection_detections(layer l, صحيح w, صحيح h, عائم thresh, detection *dets)
{
    صحيح i,j,n;
    عائم *predictions = l.output;
    //int per_cell = 5*num+classes;
    لكل (i = 0; i < l.side*l.side; ++i){
        صحيح row = i / l.side;
        صحيح col = i % l.side;
        لكل(n = 0; n < l.n; ++n){
            صحيح index = i*l.n + n;
            صحيح p_index = l.side*l.side*l.classes + i*l.n + n;
            عائم scale = predictions[p_index];
            صحيح box_index = l.side*l.side*(l.classes + l.n) + (i*l.n + n)*4;
            box b;
            b.x = (predictions[box_index + 0] + col) / l.side * w;
            b.y = (predictions[box_index + 1] + row) / l.side * h;
            b.w = pow(predictions[box_index + 2], (l.sqrt?2:1)) * w;
            b.h = pow(predictions[box_index + 3], (l.sqrt?2:1)) * h;
            dets[index].bbox = b;
            dets[index].objectness = scale;
            لكل(j = 0; j < l.classes; ++j){
                صحيح class_index = i*l.classes;
                عائم prob = scale*predictions[class_index+j];
                dets[index].prob[j] = (prob > thresh) ? prob : 0;
            }
        }
    }
}

#ifdef GPU

فراغ forward_detection_layer_gpu(ثابت detection_layer l, network net)
{
    𐤀𐤌(!net.train){
        copy_gpu(l.batch*l.inputs, net.input_gpu, 1, l.output_gpu, 1);
        أرجع;
    }

    cuda_pull_array(net.input_gpu, net.input, l.batch*l.inputs);
    forward_detection_layer(l, net);
    cuda_push_array(l.output_gpu, l.output, l.batch*l.outputs);
    cuda_push_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

فراغ backward_detection_layer_gpu(detection_layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
    //copy_gpu(l.batch*l.inputs, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


#شامل "detection_layer.h"
#شامل "activations.h"
#شامل "softmax_layer.h"
#شامل "blas.h"
#شامل "box.h"
#شامل "cuda.h"
#شامل "utils.h"

#شامل <stdio.h>
#شامل <assert.h>
#شامل <string.h>
#شامل <stdlib.h>

detection_layer make_detection_layer(صحیح batch, صحیح inputs, صحیح n, صحیح side, صحیح classes, صحیح coords, صحیح rescore)
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
    l.cost = calloc(1, اندازه(اعشاری));
    l.outputs = l.inputs;
    l.truths = l.side*l.side*(1+l.coords+l.classes);
    l.output = calloc(batch*l.outputs, اندازه(اعشاری));
    l.delta = calloc(batch*l.outputs, اندازه(اعشاری));

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

    بازگشت l;
}

تهی forward_detection_layer(ثابت detection_layer l, network net)
{
    صحیح locations = l.side*l.side;
    صحیح i,j;
    memcpy(l.output, net.درون_داد, l.outputs*l.batch*اندازه(اعشاری));
    //if(l.reorg) reorg(l.output, l.w*l.h, size*l.n, l.batch, 1);
    صحیح b;
    اگر (l.softmax){
        برای(b = 0; b < l.batch; ++b){
            صحیح index = b*l.inputs;
            برای (i = 0; i < locations; ++i) {
                صحیح offset = i*l.classes;
                softmax(l.output + index + offset, l.classes, 1, 1,
                        l.output + index + offset);
            }
        }
    }
    اگر(net.train){
        اعشاری avg_iou = 0;
        اعشاری avg_cat = 0;
        اعشاری avg_allcat = 0;
        اعشاری avg_obj = 0;
        اعشاری avg_anyobj = 0;
        صحیح شمار = 0;
        *(l.cost) = 0;
        صحیح size = l.inputs * l.batch;
        memset(l.delta, 0, size * اندازه(اعشاری));
        برای (b = 0; b < l.batch; ++b){
            صحیح index = b*l.inputs;
            برای (i = 0; i < locations; ++i) {
                صحیح truth_index = (b*locations + i)*(1+l.coords+l.classes);
                صحیح is_obj = net.truth[truth_index];
                برای (j = 0; j < l.n; ++j) {
                    صحیح p_index = index + locations*l.classes + i*l.n + j;
                    l.delta[p_index] = l.noobject_scale*(0 - l.output[p_index]);
                    *(l.cost) += l.noobject_scale*pow(l.output[p_index], 2);
                    avg_anyobj += l.output[p_index];
                }

                صحیح best_index = -1;
                اعشاری best_iou = 0;
                اعشاری best_rmse = 20;

                اگر (!is_obj){
                    ادامه;
                }

                صحیح class_index = index + i*l.classes;
                برای(j = 0; j < l.classes; ++j) {
                    l.delta[class_index+j] = l.class_scale * (net.truth[truth_index+1+j] - l.output[class_index+j]);
                    *(l.cost) += l.class_scale * pow(net.truth[truth_index+1+j] - l.output[class_index+j], 2);
                    اگر(net.truth[truth_index + 1 + j]) avg_cat += l.output[class_index+j];
                    avg_allcat += l.output[class_index+j];
                }

                box truth = float_to_box(net.truth + truth_index + 1 + l.classes, 1);
                truth.x /= l.side;
                truth.y /= l.side;

                برای(j = 0; j < l.n; ++j){
                    صحیح box_index = index + locations*(l.classes + l.n) + (i*l.n + j) * l.coords;
                    box out = float_to_box(l.output + box_index, 1);
                    out.x /= l.side;
                    out.y /= l.side;

                    اگر (l.sqrt){
                        out.w = out.w*out.w;
                        out.h = out.h*out.h;
                    }

                    اعشاری iou  = box_iou(out, truth);
                    //iou = 0;
                    اعشاری rmse = box_rmse(out, truth);
                    اگر(best_iou > 0 || iou > 0){
                        اگر(iou > best_iou){
                            best_iou = iou;
                            best_index = j;
                        }
                    }وگرنه{
                        اگر(rmse < best_rmse){
                            best_rmse = rmse;
                            best_index = j;
                        }
                    }
                }

                اگر(l.forced){
                    اگر(truth.w*truth.h < .1){
                        best_index = 1;
                    }وگرنه{
                        best_index = 0;
                    }
                }
                اگر(l.random && *(net.seen) < 64000){
                    best_index = rand()%l.n;
                }

                صحیح box_index = index + locations*(l.classes + l.n) + (i*l.n + best_index) * l.coords;
                صحیح tbox_index = truth_index + 1 + l.classes;

                box out = float_to_box(l.output + box_index, 1);
                out.x /= l.side;
                out.y /= l.side;
                اگر (l.sqrt) {
                    out.w = out.w*out.w;
                    out.h = out.h*out.h;
                }
                اعشاری iou  = box_iou(out, truth);

                //printf("%d,", best_index);
                صحیح p_index = index + locations*l.classes + i*l.n + best_index;
                *(l.cost) -= l.noobject_scale * pow(l.output[p_index], 2);
                *(l.cost) += l.object_scale * pow(1-l.output[p_index], 2);
                avg_obj += l.output[p_index];
                l.delta[p_index] = l.object_scale * (1.-l.output[p_index]);

                اگر(l.rescore){
                    l.delta[p_index] = l.object_scale * (iou - l.output[p_index]);
                }

                l.delta[box_index+0] = l.coord_scale*(net.truth[tbox_index + 0] - l.output[box_index + 0]);
                l.delta[box_index+1] = l.coord_scale*(net.truth[tbox_index + 1] - l.output[box_index + 1]);
                l.delta[box_index+2] = l.coord_scale*(net.truth[tbox_index + 2] - l.output[box_index + 2]);
                l.delta[box_index+3] = l.coord_scale*(net.truth[tbox_index + 3] - l.output[box_index + 3]);
                اگر(l.sqrt){
                    l.delta[box_index+2] = l.coord_scale*(sqrt(net.truth[tbox_index + 2]) - l.output[box_index + 2]);
                    l.delta[box_index+3] = l.coord_scale*(sqrt(net.truth[tbox_index + 3]) - l.output[box_index + 3]);
                }

                *(l.cost) += pow(1-iou, 2);
                avg_iou += iou;
                ++شمار;
            }
        }

        اگر(0){
            اعشاری *costs = calloc(l.batch*locations*l.n, اندازه(اعشاری));
            برای (b = 0; b < l.batch; ++b) {
                صحیح index = b*l.inputs;
                برای (i = 0; i < locations; ++i) {
                    برای (j = 0; j < l.n; ++j) {
                        صحیح p_index = index + locations*l.classes + i*l.n + j;
                        costs[b*locations*l.n + i*l.n + j] = l.delta[p_index]*l.delta[p_index];
                    }
                }
            }
            صحیح indexes[100];
            بالاترین_k(costs, l.batch*locations*l.n, 100, indexes);
            اعشاری cutoff = costs[indexes[99]];
            برای (b = 0; b < l.batch; ++b) {
                صحیح index = b*l.inputs;
                برای (i = 0; i < locations; ++i) {
                    برای (j = 0; j < l.n; ++j) {
                        صحیح p_index = index + locations*l.classes + i*l.n + j;
                        اگر (l.delta[p_index]*l.delta[p_index] < cutoff) l.delta[p_index] = 0;
                    }
                }
            }
            آزاد(costs);
        }


        *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);


        printf("Detection Avg IOU: %f, Pos Cat: %f, All Cat: %f, Pos Obj: %f, Any Obj: %f, count: %d\n", avg_iou/شمار, avg_cat/شمار, avg_allcat/(شمار*l.classes), avg_obj/شمار, avg_anyobj/(l.batch*locations*l.n), شمار);
        //if(l.reorg) reorg(l.delta, l.w*l.h, size*l.n, l.batch, 0);
    }
}

تهی backward_detection_layer(ثابت detection_layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

تهی get_detection_detections(layer l, صحیح w, صحیح h, اعشاری thresh, detection *dets)
{
    صحیح i,j,n;
    اعشاری *predictions = l.output;
    //int per_cell = 5*num+classes;
    برای (i = 0; i < l.side*l.side; ++i){
        صحیح row = i / l.side;
        صحیح col = i % l.side;
        برای(n = 0; n < l.n; ++n){
            صحیح index = i*l.n + n;
            صحیح p_index = l.side*l.side*l.classes + i*l.n + n;
            اعشاری scale = predictions[p_index];
            صحیح box_index = l.side*l.side*(l.classes + l.n) + (i*l.n + n)*4;
            box b;
            b.x = (predictions[box_index + 0] + col) / l.side * w;
            b.y = (predictions[box_index + 1] + row) / l.side * h;
            b.w = pow(predictions[box_index + 2], (l.sqrt?2:1)) * w;
            b.h = pow(predictions[box_index + 3], (l.sqrt?2:1)) * h;
            dets[index].bbox = b;
            dets[index].objectness = scale;
            برای(j = 0; j < l.classes; ++j){
                صحیح class_index = i*l.classes;
                اعشاری prob = scale*predictions[class_index+j];
                dets[index].prob[j] = (prob > thresh) ? prob : 0;
            }
        }
    }
}

#ifdef GPU

تهی forward_detection_layer_gpu(ثابت detection_layer l, network net)
{
    اگر(!net.train){
        copy_gpu(l.batch*l.inputs, net.درون_داد_gpu, 1, l.output_gpu, 1);
        بازگشت;
    }

    cuda_pull_array(net.درون_داد_gpu, net.درون_داد, l.batch*l.inputs);
    forward_detection_layer(l, net);
    cuda_افزودن_array(l.output_gpu, l.output, l.batch*l.outputs);
    cuda_افزودن_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

تهی backward_detection_layer_gpu(detection_layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
    //copy_gpu(l.batch*l.inputs, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


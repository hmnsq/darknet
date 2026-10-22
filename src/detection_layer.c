#অন্তর্ভাব "detection_layer.h"
#অন্তর্ভাব "activations.h"
#অন্তর্ভাব "softmax_layer.h"
#অন্তর্ভাব "blas.h"
#অন্তর্ভাব "box.h"
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব "utils.h"

#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <stdlib.h>

detection_layer make_detection_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক inputs, পূর্ণাঙ্ক n, পূর্ণাঙ্ক side, পূর্ণাঙ্ক classes, পূর্ণাঙ্ক coords, পূর্ণাঙ্ক rescore)
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
    প্রতিজ্ঞা(side*side*((1 + l.coords)*l.n + l.classes) == inputs);
    l.cost = calloc(1, পরিমাণ(ভিন্ন));
    l.outputs = l.inputs;
    l.truths = l.side*l.side*(1+l.coords+l.classes);
    l.output = calloc(batch*l.outputs, পরিমাণ(ভিন্ন));
    l.delta = calloc(batch*l.outputs, পরিমাণ(ভিন্ন));

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

    প্রত্যাগম l;
}

শূন্য forward_detection_layer(ধ্রুব detection_layer l, network net)
{
    পূর্ণাঙ্ক locations = l.side*l.side;
    পূর্ণাঙ্ক i,j;
    memcpy(l.output, net.প্রবেশ, l.outputs*l.batch*পরিমাণ(ভিন্ন));
    //if(l.reorg) reorg(l.output, l.w*l.h, size*l.n, l.batch, 1);
    পূর্ণাঙ্ক b;
    যদি (l.softmax){
        প্রতি(b = 0; b < l.batch; ++b){
            পূর্ণাঙ্ক index = b*l.inputs;
            প্রতি (i = 0; i < locations; ++i) {
                পূর্ণাঙ্ক offset = i*l.classes;
                softmax(l.output + index + offset, l.classes, 1, 1,
                        l.output + index + offset);
            }
        }
    }
    যদি(net.train){
        ভিন্ন avg_iou = 0;
        ভিন্ন avg_cat = 0;
        ভিন্ন avg_allcat = 0;
        ভিন্ন avg_obj = 0;
        ভিন্ন avg_anyobj = 0;
        পূর্ণাঙ্ক গুনতি = 0;
        *(l.cost) = 0;
        পূর্ণাঙ্ক size = l.inputs * l.batch;
        memset(l.delta, 0, size * পরিমাণ(ভিন্ন));
        প্রতি (b = 0; b < l.batch; ++b){
            পূর্ণাঙ্ক index = b*l.inputs;
            প্রতি (i = 0; i < locations; ++i) {
                পূর্ণাঙ্ক truth_index = (b*locations + i)*(1+l.coords+l.classes);
                পূর্ণাঙ্ক is_obj = net.truth[truth_index];
                প্রতি (j = 0; j < l.n; ++j) {
                    পূর্ণাঙ্ক p_index = index + locations*l.classes + i*l.n + j;
                    l.delta[p_index] = l.noobject_scale*(0 - l.output[p_index]);
                    *(l.cost) += l.noobject_scale*pow(l.output[p_index], 2);
                    avg_anyobj += l.output[p_index];
                }

                পূর্ণাঙ্ক best_index = -1;
                ভিন্ন best_iou = 0;
                ভিন্ন best_rmse = 20;

                যদি (!is_obj){
                    অনুবর্ত;
                }

                পূর্ণাঙ্ক class_index = index + i*l.classes;
                প্রতি(j = 0; j < l.classes; ++j) {
                    l.delta[class_index+j] = l.class_scale * (net.truth[truth_index+1+j] - l.output[class_index+j]);
                    *(l.cost) += l.class_scale * pow(net.truth[truth_index+1+j] - l.output[class_index+j], 2);
                    যদি(net.truth[truth_index + 1 + j]) avg_cat += l.output[class_index+j];
                    avg_allcat += l.output[class_index+j];
                }

                box truth = float_to_box(net.truth + truth_index + 1 + l.classes, 1);
                truth.x /= l.side;
                truth.y /= l.side;

                প্রতি(j = 0; j < l.n; ++j){
                    পূর্ণাঙ্ক box_index = index + locations*(l.classes + l.n) + (i*l.n + j) * l.coords;
                    box out = float_to_box(l.output + box_index, 1);
                    out.x /= l.side;
                    out.y /= l.side;

                    যদি (l.sqrt){
                        out.w = out.w*out.w;
                        out.h = out.h*out.h;
                    }

                    ভিন্ন iou  = box_iou(out, truth);
                    //iou = 0;
                    ভিন্ন rmse = box_rmse(out, truth);
                    যদি(best_iou > 0 || iou > 0){
                        যদি(iou > best_iou){
                            best_iou = iou;
                            best_index = j;
                        }
                    }অন্যথা{
                        যদি(rmse < best_rmse){
                            best_rmse = rmse;
                            best_index = j;
                        }
                    }
                }

                যদি(l.forced){
                    যদি(truth.w*truth.h < .1){
                        best_index = 1;
                    }অন্যথা{
                        best_index = 0;
                    }
                }
                যদি(l.random && *(net.seen) < 64000){
                    best_index = rand()%l.n;
                }

                পূর্ণাঙ্ক box_index = index + locations*(l.classes + l.n) + (i*l.n + best_index) * l.coords;
                পূর্ণাঙ্ক tbox_index = truth_index + 1 + l.classes;

                box out = float_to_box(l.output + box_index, 1);
                out.x /= l.side;
                out.y /= l.side;
                যদি (l.sqrt) {
                    out.w = out.w*out.w;
                    out.h = out.h*out.h;
                }
                ভিন্ন iou  = box_iou(out, truth);

                //printf("%d,", best_index);
                পূর্ণাঙ্ক p_index = index + locations*l.classes + i*l.n + best_index;
                *(l.cost) -= l.noobject_scale * pow(l.output[p_index], 2);
                *(l.cost) += l.object_scale * pow(1-l.output[p_index], 2);
                avg_obj += l.output[p_index];
                l.delta[p_index] = l.object_scale * (1.-l.output[p_index]);

                যদি(l.rescore){
                    l.delta[p_index] = l.object_scale * (iou - l.output[p_index]);
                }

                l.delta[box_index+0] = l.coord_scale*(net.truth[tbox_index + 0] - l.output[box_index + 0]);
                l.delta[box_index+1] = l.coord_scale*(net.truth[tbox_index + 1] - l.output[box_index + 1]);
                l.delta[box_index+2] = l.coord_scale*(net.truth[tbox_index + 2] - l.output[box_index + 2]);
                l.delta[box_index+3] = l.coord_scale*(net.truth[tbox_index + 3] - l.output[box_index + 3]);
                যদি(l.sqrt){
                    l.delta[box_index+2] = l.coord_scale*(sqrt(net.truth[tbox_index + 2]) - l.output[box_index + 2]);
                    l.delta[box_index+3] = l.coord_scale*(sqrt(net.truth[tbox_index + 3]) - l.output[box_index + 3]);
                }

                *(l.cost) += pow(1-iou, 2);
                avg_iou += iou;
                ++গুনতি;
            }
        }

        যদি(0){
            ভিন্ন *costs = calloc(l.batch*locations*l.n, পরিমাণ(ভিন্ন));
            প্রতি (b = 0; b < l.batch; ++b) {
                পূর্ণাঙ্ক index = b*l.inputs;
                প্রতি (i = 0; i < locations; ++i) {
                    প্রতি (j = 0; j < l.n; ++j) {
                        পূর্ণাঙ্ক p_index = index + locations*l.classes + i*l.n + j;
                        costs[b*locations*l.n + i*l.n + j] = l.delta[p_index]*l.delta[p_index];
                    }
                }
            }
            পূর্ণাঙ্ক indexes[100];
            শীর্ষ_k(costs, l.batch*locations*l.n, 100, indexes);
            ভিন্ন cutoff = costs[indexes[99]];
            প্রতি (b = 0; b < l.batch; ++b) {
                পূর্ণাঙ্ক index = b*l.inputs;
                প্রতি (i = 0; i < locations; ++i) {
                    প্রতি (j = 0; j < l.n; ++j) {
                        পূর্ণাঙ্ক p_index = index + locations*l.classes + i*l.n + j;
                        যদি (l.delta[p_index]*l.delta[p_index] < cutoff) l.delta[p_index] = 0;
                    }
                }
            }
            মুক্ত(costs);
        }


        *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);


        printf("Detection Avg IOU: %f, Pos Cat: %f, All Cat: %f, Pos Obj: %f, Any Obj: %f, count: %d\n", avg_iou/গুনতি, avg_cat/গুনতি, avg_allcat/(গুনতি*l.classes), avg_obj/গুনতি, avg_anyobj/(l.batch*locations*l.n), গুনতি);
        //if(l.reorg) reorg(l.delta, l.w*l.h, size*l.n, l.batch, 0);
    }
}

শূন্য backward_detection_layer(ধ্রুব detection_layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

শূন্য get_detection_detections(layer l, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, ভিন্ন thresh, detection *dets)
{
    পূর্ণাঙ্ক i,j,n;
    ভিন্ন *predictions = l.output;
    //int per_cell = 5*num+classes;
    প্রতি (i = 0; i < l.side*l.side; ++i){
        পূর্ণাঙ্ক row = i / l.side;
        পূর্ণাঙ্ক col = i % l.side;
        প্রতি(n = 0; n < l.n; ++n){
            পূর্ণাঙ্ক index = i*l.n + n;
            পূর্ণাঙ্ক p_index = l.side*l.side*l.classes + i*l.n + n;
            ভিন্ন scale = predictions[p_index];
            পূর্ণাঙ্ক box_index = l.side*l.side*(l.classes + l.n) + (i*l.n + n)*4;
            box b;
            b.x = (predictions[box_index + 0] + col) / l.side * w;
            b.y = (predictions[box_index + 1] + row) / l.side * h;
            b.w = pow(predictions[box_index + 2], (l.sqrt?2:1)) * w;
            b.h = pow(predictions[box_index + 3], (l.sqrt?2:1)) * h;
            dets[index].bbox = b;
            dets[index].objectness = scale;
            প্রতি(j = 0; j < l.classes; ++j){
                পূর্ণাঙ্ক class_index = i*l.classes;
                ভিন্ন prob = scale*predictions[class_index+j];
                dets[index].prob[j] = (prob > thresh) ? prob : 0;
            }
        }
    }
}

#ifdef GPU

শূন্য forward_detection_layer_gpu(ধ্রুব detection_layer l, network net)
{
    যদি(!net.train){
        copy_gpu(l.batch*l.inputs, net.প্রবেশ_gpu, 1, l.output_gpu, 1);
        প্রত্যাগম;
    }

    cuda_pull_array(net.প্রবেশ_gpu, net.প্রবেশ, l.batch*l.inputs);
    forward_detection_layer(l, net);
    cuda_ঠেলো_array(l.output_gpu, l.output, l.batch*l.outputs);
    cuda_ঠেলো_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

শূন্য backward_detection_layer_gpu(detection_layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
    //copy_gpu(l.batch*l.inputs, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


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

detection_layer make_detection_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা inputs, পূর্ণসংখ্যা n, পূর্ণসংখ্যা side, পূর্ণসংখ্যা classes, পূর্ণসংখ্যা coords, পূর্ণসংখ্যা rescore)
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
    l.cost = calloc(1, আকার(দশমিক));
    l.outputs = l.inputs;
    l.truths = l.side*l.side*(1+l.coords+l.classes);
    l.output = calloc(batch*l.outputs, আকার(দশমিক));
    l.delta = calloc(batch*l.outputs, আকার(দশমিক));

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

    ফেরত l;
}

শূন্য forward_detection_layer(ধ্রুবক detection_layer l, network net)
{
    পূর্ণসংখ্যা locations = l.side*l.side;
    পূর্ণসংখ্যা i,j;
    memcpy(l.output, net.প্রবেশ, l.outputs*l.batch*আকার(দশমিক));
    //if(l.reorg) reorg(l.output, l.w*l.h, size*l.n, l.batch, 1);
    পূর্ণসংখ্যা b;
    যদি (l.softmax){
        জন্য(b = 0; b < l.batch; ++b){
            পূর্ণসংখ্যা সূচক = b*l.inputs;
            জন্য (i = 0; i < locations; ++i) {
                পূর্ণসংখ্যা offset = i*l.classes;
                softmax(l.output + সূচক + offset, l.classes, 1, 1,
                        l.output + সূচক + offset);
            }
        }
    }
    যদি(net.train){
        দশমিক avg_iou = 0;
        দশমিক avg_cat = 0;
        দশমিক avg_allcat = 0;
        দশমিক avg_obj = 0;
        দশমিক avg_anyobj = 0;
        পূর্ণসংখ্যা গুনতি = 0;
        *(l.cost) = 0;
        পূর্ণসংখ্যা আকার_2 = l.inputs * l.batch;
        memset(l.delta, 0, আকার_2 * আকার(দশমিক));
        জন্য (b = 0; b < l.batch; ++b){
            পূর্ণসংখ্যা সূচক = b*l.inputs;
            জন্য (i = 0; i < locations; ++i) {
                পূর্ণসংখ্যা truth_সূচক = (b*locations + i)*(1+l.coords+l.classes);
                পূর্ণসংখ্যা is_obj = net.truth[truth_সূচক];
                জন্য (j = 0; j < l.n; ++j) {
                    পূর্ণসংখ্যা p_সূচক = সূচক + locations*l.classes + i*l.n + j;
                    l.delta[p_সূচক] = l.noobject_scale*(0 - l.output[p_সূচক]);
                    *(l.cost) += l.noobject_scale*pow(l.output[p_সূচক], 2);
                    avg_anyobj += l.output[p_সূচক];
                }

                পূর্ণসংখ্যা best_সূচক = -1;
                দশমিক best_iou = 0;
                দশমিক best_rmse = 20;

                যদি (!is_obj){
                    চালু;
                }

                পূর্ণসংখ্যা class_সূচক = সূচক + i*l.classes;
                জন্য(j = 0; j < l.classes; ++j) {
                    l.delta[class_সূচক+j] = l.class_scale * (net.truth[truth_সূচক+1+j] - l.output[class_সূচক+j]);
                    *(l.cost) += l.class_scale * pow(net.truth[truth_সূচক+1+j] - l.output[class_সূচক+j], 2);
                    যদি(net.truth[truth_সূচক + 1 + j]) avg_cat += l.output[class_সূচক+j];
                    avg_allcat += l.output[class_সূচক+j];
                }

                box truth = float_to_box(net.truth + truth_সূচক + 1 + l.classes, 1);
                truth.x /= l.side;
                truth.y /= l.side;

                জন্য(j = 0; j < l.n; ++j){
                    পূর্ণসংখ্যা box_সূচক = সূচক + locations*(l.classes + l.n) + (i*l.n + j) * l.coords;
                    box out = float_to_box(l.output + box_সূচক, 1);
                    out.x /= l.side;
                    out.y /= l.side;

                    যদি (l.sqrt){
                        out.w = out.w*out.w;
                        out.h = out.h*out.h;
                    }

                    দশমিক iou  = box_iou(out, truth);
                    //iou = 0;
                    দশমিক rmse = box_rmse(out, truth);
                    যদি(best_iou > 0 || iou > 0){
                        যদি(iou > best_iou){
                            best_iou = iou;
                            best_সূচক = j;
                        }
                    }অন্যথা{
                        যদি(rmse < best_rmse){
                            best_rmse = rmse;
                            best_সূচক = j;
                        }
                    }
                }

                যদি(l.forced){
                    যদি(truth.w*truth.h < .1){
                        best_সূচক = 1;
                    }অন্যথা{
                        best_সূচক = 0;
                    }
                }
                যদি(l.random && *(net.seen) < 64000){
                    best_সূচক = rand()%l.n;
                }

                পূর্ণসংখ্যা box_সূচক = সূচক + locations*(l.classes + l.n) + (i*l.n + best_সূচক) * l.coords;
                পূর্ণসংখ্যা tbox_সূচক = truth_সূচক + 1 + l.classes;

                box out = float_to_box(l.output + box_সূচক, 1);
                out.x /= l.side;
                out.y /= l.side;
                যদি (l.sqrt) {
                    out.w = out.w*out.w;
                    out.h = out.h*out.h;
                }
                দশমিক iou  = box_iou(out, truth);

                //printf("%d,", best_index);
                পূর্ণসংখ্যা p_সূচক = সূচক + locations*l.classes + i*l.n + best_সূচক;
                *(l.cost) -= l.noobject_scale * pow(l.output[p_সূচক], 2);
                *(l.cost) += l.object_scale * pow(1-l.output[p_সূচক], 2);
                avg_obj += l.output[p_সূচক];
                l.delta[p_সূচক] = l.object_scale * (1.-l.output[p_সূচক]);

                যদি(l.rescore){
                    l.delta[p_সূচক] = l.object_scale * (iou - l.output[p_সূচক]);
                }

                l.delta[box_সূচক+0] = l.coord_scale*(net.truth[tbox_সূচক + 0] - l.output[box_সূচক + 0]);
                l.delta[box_সূচক+1] = l.coord_scale*(net.truth[tbox_সূচক + 1] - l.output[box_সূচক + 1]);
                l.delta[box_সূচক+2] = l.coord_scale*(net.truth[tbox_সূচক + 2] - l.output[box_সূচক + 2]);
                l.delta[box_সূচক+3] = l.coord_scale*(net.truth[tbox_সূচক + 3] - l.output[box_সূচক + 3]);
                যদি(l.sqrt){
                    l.delta[box_সূচক+2] = l.coord_scale*(sqrt(net.truth[tbox_সূচক + 2]) - l.output[box_সূচক + 2]);
                    l.delta[box_সূচক+3] = l.coord_scale*(sqrt(net.truth[tbox_সূচক + 3]) - l.output[box_সূচক + 3]);
                }

                *(l.cost) += pow(1-iou, 2);
                avg_iou += iou;
                ++গুনতি;
            }
        }

        যদি(0){
            দশমিক *costs = calloc(l.batch*locations*l.n, আকার(দশমিক));
            জন্য (b = 0; b < l.batch; ++b) {
                পূর্ণসংখ্যা সূচক = b*l.inputs;
                জন্য (i = 0; i < locations; ++i) {
                    জন্য (j = 0; j < l.n; ++j) {
                        পূর্ণসংখ্যা p_সূচক = সূচক + locations*l.classes + i*l.n + j;
                        costs[b*locations*l.n + i*l.n + j] = l.delta[p_সূচক]*l.delta[p_সূচক];
                    }
                }
            }
            পূর্ণসংখ্যা indexes[100];
            শীর্ষ_k(costs, l.batch*locations*l.n, 100, indexes);
            দশমিক cutoff = costs[indexes[99]];
            জন্য (b = 0; b < l.batch; ++b) {
                পূর্ণসংখ্যা সূচক = b*l.inputs;
                জন্য (i = 0; i < locations; ++i) {
                    জন্য (j = 0; j < l.n; ++j) {
                        পূর্ণসংখ্যা p_সূচক = সূচক + locations*l.classes + i*l.n + j;
                        যদি (l.delta[p_সূচক]*l.delta[p_সূচক] < cutoff) l.delta[p_সূচক] = 0;
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

শূন্য backward_detection_layer(ধ্রুবক detection_layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

শূন্য get_detection_detections(layer l, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, দশমিক thresh, detection *dets)
{
    পূর্ণসংখ্যা i,j,n;
    দশমিক *predictions = l.output;
    //int per_cell = 5*num+classes;
    জন্য (i = 0; i < l.side*l.side; ++i){
        পূর্ণসংখ্যা row = i / l.side;
        পূর্ণসংখ্যা col = i % l.side;
        জন্য(n = 0; n < l.n; ++n){
            পূর্ণসংখ্যা সূচক = i*l.n + n;
            পূর্ণসংখ্যা p_সূচক = l.side*l.side*l.classes + i*l.n + n;
            দশমিক scale = predictions[p_সূচক];
            পূর্ণসংখ্যা box_সূচক = l.side*l.side*(l.classes + l.n) + (i*l.n + n)*4;
            box b;
            b.x = (predictions[box_সূচক + 0] + col) / l.side * w;
            b.y = (predictions[box_সূচক + 1] + row) / l.side * h;
            b.w = pow(predictions[box_সূচক + 2], (l.sqrt?2:1)) * w;
            b.h = pow(predictions[box_সূচক + 3], (l.sqrt?2:1)) * h;
            dets[সূচক].bbox = b;
            dets[সূচক].objectness = scale;
            জন্য(j = 0; j < l.classes; ++j){
                পূর্ণসংখ্যা class_সূচক = i*l.classes;
                দশমিক prob = scale*predictions[class_সূচক+j];
                dets[সূচক].prob[j] = (prob > thresh) ? prob : 0;
            }
        }
    }
}

#ifdef GPU

শূন্য forward_detection_layer_gpu(ধ্রুবক detection_layer l, network net)
{
    যদি(!net.train){
        copy_gpu(l.batch*l.inputs, net.প্রবেশ_gpu, 1, l.output_gpu, 1);
        ফেরত;
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


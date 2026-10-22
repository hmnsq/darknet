#ଅନ୍ତର୍ଭାଵ "detection_layer.h"
#ଅନ୍ତର୍ଭାଵ "activations.h"
#ଅନ୍ତର୍ଭାଵ "softmax_layer.h"
#ଅନ୍ତର୍ଭାଵ "blas.h"
#ଅନ୍ତର୍ଭାଵ "box.h"
#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"

#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>

detection_layer make_detection_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inputs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା side, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା coords, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rescore)
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
    ପ୍ରତିଜ୍ଞା(side*side*((1 + l.coords)*l.n + l.classes) == inputs);
    l.cost = calloc(1, ଆକାର(ଦଶମିକ));
    l.outputs = l.inputs;
    l.truths = l.side*l.side*(1+l.coords+l.classes);
    l.output = calloc(batch*l.outputs, ଆକାର(ଦଶମିକ));
    l.delta = calloc(batch*l.outputs, ଆକାର(ଦଶମିକ));

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

    ଫେରନ୍ତୁ l;
}

ଶୂନ୍ୟ forward_detection_layer(ଅଚଳ detection_layer l, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା locations = l.side*l.side;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j;
    memcpy(l.output, net.ନିବେଶ, l.outputs*l.batch*ଆକାର(ଦଶମିକ));
    //if(l.reorg) reorg(l.output, l.w*l.h, size*l.n, l.batch, 1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା b;
    ଯଦି (l.softmax){
        ପାଇଁ(b = 0; b < l.batch; ++b){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = b*l.inputs;
            ପାଇଁ (i = 0; i < locations; ++i) {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = i*l.classes;
                softmax(l.output + index + offset, l.classes, 1, 1,
                        l.output + index + offset);
            }
        }
    }
    ଯଦି(net.train){
        ଦଶମିକ avg_iou = 0;
        ଦଶମିକ avg_cat = 0;
        ଦଶମିକ avg_allcat = 0;
        ଦଶମିକ avg_obj = 0;
        ଦଶମିକ avg_anyobj = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
        *(l.cost) = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = l.inputs * l.batch;
        memset(l.delta, 0, size * ଆକାର(ଦଶମିକ));
        ପାଇଁ (b = 0; b < l.batch; ++b){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = b*l.inputs;
            ପାଇଁ (i = 0; i < locations; ++i) {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା truth_index = (b*locations + i)*(1+l.coords+l.classes);
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_obj = net.truth[truth_index];
                ପାଇଁ (j = 0; j < l.n; ++j) {
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା p_index = index + locations*l.classes + i*l.n + j;
                    l.delta[p_index] = l.noobject_scale*(0 - l.output[p_index]);
                    *(l.cost) += l.noobject_scale*pow(l.output[p_index], 2);
                    avg_anyobj += l.output[p_index];
                }

                ପୂର୍ଣ୍ଣସଂଖ୍ୟା best_index = -1;
                ଦଶମିକ best_iou = 0;
                ଦଶମିକ best_rmse = 20;

                ଯଦି (!is_obj){
                    ଜାରି;
                }

                ପୂର୍ଣ୍ଣସଂଖ୍ୟା class_index = index + i*l.classes;
                ପାଇଁ(j = 0; j < l.classes; ++j) {
                    l.delta[class_index+j] = l.class_scale * (net.truth[truth_index+1+j] - l.output[class_index+j]);
                    *(l.cost) += l.class_scale * pow(net.truth[truth_index+1+j] - l.output[class_index+j], 2);
                    ଯଦି(net.truth[truth_index + 1 + j]) avg_cat += l.output[class_index+j];
                    avg_allcat += l.output[class_index+j];
                }

                box truth = float_to_box(net.truth + truth_index + 1 + l.classes, 1);
                truth.x /= l.side;
                truth.y /= l.side;

                ପାଇଁ(j = 0; j < l.n; ++j){
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା box_index = index + locations*(l.classes + l.n) + (i*l.n + j) * l.coords;
                    box out = float_to_box(l.output + box_index, 1);
                    out.x /= l.side;
                    out.y /= l.side;

                    ଯଦି (l.sqrt){
                        out.w = out.w*out.w;
                        out.h = out.h*out.h;
                    }

                    ଦଶମିକ iou  = box_iou(out, truth);
                    //iou = 0;
                    ଦଶମିକ rmse = box_rmse(out, truth);
                    ଯଦି(best_iou > 0 || iou > 0){
                        ଯଦି(iou > best_iou){
                            best_iou = iou;
                            best_index = j;
                        }
                    }ଅନ୍ୟଥା{
                        ଯଦି(rmse < best_rmse){
                            best_rmse = rmse;
                            best_index = j;
                        }
                    }
                }

                ଯଦି(l.forced){
                    ଯଦି(truth.w*truth.h < .1){
                        best_index = 1;
                    }ଅନ୍ୟଥା{
                        best_index = 0;
                    }
                }
                ଯଦି(l.random && *(net.seen) < 64000){
                    best_index = rand()%l.n;
                }

                ପୂର୍ଣ୍ଣସଂଖ୍ୟା box_index = index + locations*(l.classes + l.n) + (i*l.n + best_index) * l.coords;
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା tbox_index = truth_index + 1 + l.classes;

                box out = float_to_box(l.output + box_index, 1);
                out.x /= l.side;
                out.y /= l.side;
                ଯଦି (l.sqrt) {
                    out.w = out.w*out.w;
                    out.h = out.h*out.h;
                }
                ଦଶମିକ iou  = box_iou(out, truth);

                //printf("%d,", best_index);
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା p_index = index + locations*l.classes + i*l.n + best_index;
                *(l.cost) -= l.noobject_scale * pow(l.output[p_index], 2);
                *(l.cost) += l.object_scale * pow(1-l.output[p_index], 2);
                avg_obj += l.output[p_index];
                l.delta[p_index] = l.object_scale * (1.-l.output[p_index]);

                ଯଦି(l.rescore){
                    l.delta[p_index] = l.object_scale * (iou - l.output[p_index]);
                }

                l.delta[box_index+0] = l.coord_scale*(net.truth[tbox_index + 0] - l.output[box_index + 0]);
                l.delta[box_index+1] = l.coord_scale*(net.truth[tbox_index + 1] - l.output[box_index + 1]);
                l.delta[box_index+2] = l.coord_scale*(net.truth[tbox_index + 2] - l.output[box_index + 2]);
                l.delta[box_index+3] = l.coord_scale*(net.truth[tbox_index + 3] - l.output[box_index + 3]);
                ଯଦି(l.sqrt){
                    l.delta[box_index+2] = l.coord_scale*(sqrt(net.truth[tbox_index + 2]) - l.output[box_index + 2]);
                    l.delta[box_index+3] = l.coord_scale*(sqrt(net.truth[tbox_index + 3]) - l.output[box_index + 3]);
                }

                *(l.cost) += pow(1-iou, 2);
                avg_iou += iou;
                ++ଗଣନା_2;
            }
        }

        ଯଦି(0){
            ଦଶମିକ *costs = calloc(l.batch*locations*l.n, ଆକାର(ଦଶମିକ));
            ପାଇଁ (b = 0; b < l.batch; ++b) {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = b*l.inputs;
                ପାଇଁ (i = 0; i < locations; ++i) {
                    ପାଇଁ (j = 0; j < l.n; ++j) {
                        ପୂର୍ଣ୍ଣସଂଖ୍ୟା p_index = index + locations*l.classes + i*l.n + j;
                        costs[b*locations*l.n + i*l.n + j] = l.delta[p_index]*l.delta[p_index];
                    }
                }
            }
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା indexes[100];
            ଶୀର୍ଷ_k(costs, l.batch*locations*l.n, 100, indexes);
            ଦଶମିକ cutoff = costs[indexes[99]];
            ପାଇଁ (b = 0; b < l.batch; ++b) {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = b*l.inputs;
                ପାଇଁ (i = 0; i < locations; ++i) {
                    ପାଇଁ (j = 0; j < l.n; ++j) {
                        ପୂର୍ଣ୍ଣସଂଖ୍ୟା p_index = index + locations*l.classes + i*l.n + j;
                        ଯଦି (l.delta[p_index]*l.delta[p_index] < cutoff) l.delta[p_index] = 0;
                    }
                }
            }
            ମୁକ୍ତ(costs);
        }


        *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);


        printf("Detection Avg IOU: %f, Pos Cat: %f, All Cat: %f, Pos Obj: %f, Any Obj: %f, count: %d\n", avg_iou/ଗଣନା_2, avg_cat/ଗଣନା_2, avg_allcat/(ଗଣନା_2*l.classes), avg_obj/ଗଣନା_2, avg_anyobj/(l.batch*locations*l.n), ଗଣନା_2);
        //if(l.reorg) reorg(l.delta, l.w*l.h, size*l.n, l.batch, 0);
    }
}

ଶୂନ୍ୟ backward_detection_layer(ଅଚଳ detection_layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

ଶୂନ୍ୟ get_detection_detections(layer l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ଦଶମିକ thresh, detection *dets)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,n;
    ଦଶମିକ *predictions = l.output;
    //int per_cell = 5*num+classes;
    ପାଇଁ (i = 0; i < l.side*l.side; ++i){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା row = i / l.side;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା col = i % l.side;
        ପାଇଁ(n = 0; n < l.n; ++n){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = i*l.n + n;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା p_index = l.side*l.side*l.classes + i*l.n + n;
            ଦଶମିକ scale = predictions[p_index];
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା box_index = l.side*l.side*(l.classes + l.n) + (i*l.n + n)*4;
            box b;
            b.x = (predictions[box_index + 0] + col) / l.side * w;
            b.y = (predictions[box_index + 1] + row) / l.side * h;
            b.w = pow(predictions[box_index + 2], (l.sqrt?2:1)) * w;
            b.h = pow(predictions[box_index + 3], (l.sqrt?2:1)) * h;
            dets[index].bbox = b;
            dets[index].objectness = scale;
            ପାଇଁ(j = 0; j < l.classes; ++j){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା class_index = i*l.classes;
                ଦଶମିକ prob = scale*predictions[class_index+j];
                dets[index].prob[j] = (prob > thresh) ? prob : 0;
            }
        }
    }
}

#ifdef GPU

ଶୂନ୍ୟ forward_detection_layer_gpu(ଅଚଳ detection_layer l, network net)
{
    ଯଦି(!net.train){
        copy_gpu(l.batch*l.inputs, net.ନିବେଶ_gpu, 1, l.output_gpu, 1);
        ଫେରନ୍ତୁ;
    }

    cuda_pull_array(net.ନିବେଶ_gpu, net.ନିବେଶ, l.batch*l.inputs);
    forward_detection_layer(l, net);
    cuda_ଠେଲ_array(l.output_gpu, l.output, l.batch*l.outputs);
    cuda_ଠେଲ_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

ଶୂନ୍ୟ backward_detection_layer_gpu(detection_layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
    //copy_gpu(l.batch*l.inputs, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


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

detection_layer make_detection_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ inputs, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ side, ಪೂರ್ಣಾಙ್ಕ classes, ಪೂರ್ಣಾಙ್ಕ coords, ಪೂರ್ಣಾಙ್ಕ rescore)
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
    l.cost = calloc(1, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.outputs = l.inputs;
    l.truths = l.side*l.side*(1+l.coords+l.classes);
    l.output = calloc(batch*l.outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.delta = calloc(batch*l.outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));

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

    ಪ್ರತ್ಯಾಗಮ l;
}

ಶೂನ್ಯ forward_detection_layer(ಧ್ರುವ detection_layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ locations = l.side*l.side;
    ಪೂರ್ಣಾಙ್ಕ i,j;
    memcpy(l.output, net.ಒಳಹರಿವು, l.outputs*l.batch*ಪರಿಮಾಣ(ಭಿನ್ನ));
    //if(l.reorg) reorg(l.output, l.w*l.h, size*l.n, l.batch, 1);
    ಪೂರ್ಣಾಙ್ಕ b;
    ಯದಿ (l.softmax){
        ಪ್ರತಿ(b = 0; b < l.batch; ++b){
            ಪೂರ್ಣಾಙ್ಕ index = b*l.inputs;
            ಪ್ರತಿ (i = 0; i < locations; ++i) {
                ಪೂರ್ಣಾಙ್ಕ offset = i*l.classes;
                softmax(l.output + index + offset, l.classes, 1, 1,
                        l.output + index + offset);
            }
        }
    }
    ಯದಿ(net.train){
        ಭಿನ್ನ avg_iou = 0;
        ಭಿನ್ನ avg_cat = 0;
        ಭಿನ್ನ avg_allcat = 0;
        ಭಿನ್ನ avg_obj = 0;
        ಭಿನ್ನ avg_anyobj = 0;
        ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
        *(l.cost) = 0;
        ಪೂರ್ಣಾಙ್ಕ size = l.inputs * l.batch;
        memset(l.delta, 0, size * ಪರಿಮಾಣ(ಭಿನ್ನ));
        ಪ್ರತಿ (b = 0; b < l.batch; ++b){
            ಪೂರ್ಣಾಙ್ಕ index = b*l.inputs;
            ಪ್ರತಿ (i = 0; i < locations; ++i) {
                ಪೂರ್ಣಾಙ್ಕ truth_index = (b*locations + i)*(1+l.coords+l.classes);
                ಪೂರ್ಣಾಙ್ಕ is_obj = net.truth[truth_index];
                ಪ್ರತಿ (j = 0; j < l.n; ++j) {
                    ಪೂರ್ಣಾಙ್ಕ p_index = index + locations*l.classes + i*l.n + j;
                    l.delta[p_index] = l.noobject_scale*(0 - l.output[p_index]);
                    *(l.cost) += l.noobject_scale*pow(l.output[p_index], 2);
                    avg_anyobj += l.output[p_index];
                }

                ಪೂರ್ಣಾಙ್ಕ best_index = -1;
                ಭಿನ್ನ best_iou = 0;
                ಭಿನ್ನ best_rmse = 20;

                ಯದಿ (!is_obj){
                    ಅನುವರ್ತ;
                }

                ಪೂರ್ಣಾಙ್ಕ class_index = index + i*l.classes;
                ಪ್ರತಿ(j = 0; j < l.classes; ++j) {
                    l.delta[class_index+j] = l.class_scale * (net.truth[truth_index+1+j] - l.output[class_index+j]);
                    *(l.cost) += l.class_scale * pow(net.truth[truth_index+1+j] - l.output[class_index+j], 2);
                    ಯದಿ(net.truth[truth_index + 1 + j]) avg_cat += l.output[class_index+j];
                    avg_allcat += l.output[class_index+j];
                }

                box truth = float_to_box(net.truth + truth_index + 1 + l.classes, 1);
                truth.x /= l.side;
                truth.y /= l.side;

                ಪ್ರತಿ(j = 0; j < l.n; ++j){
                    ಪೂರ್ಣಾಙ್ಕ box_index = index + locations*(l.classes + l.n) + (i*l.n + j) * l.coords;
                    box out = float_to_box(l.output + box_index, 1);
                    out.x /= l.side;
                    out.y /= l.side;

                    ಯದಿ (l.sqrt){
                        out.w = out.w*out.w;
                        out.h = out.h*out.h;
                    }

                    ಭಿನ್ನ iou  = box_iou(out, truth);
                    //iou = 0;
                    ಭಿನ್ನ rmse = box_rmse(out, truth);
                    ಯದಿ(best_iou > 0 || iou > 0){
                        ಯದಿ(iou > best_iou){
                            best_iou = iou;
                            best_index = j;
                        }
                    }ಅನ್ಯಥಾ{
                        ಯದಿ(rmse < best_rmse){
                            best_rmse = rmse;
                            best_index = j;
                        }
                    }
                }

                ಯದಿ(l.forced){
                    ಯದಿ(truth.w*truth.h < .1){
                        best_index = 1;
                    }ಅನ್ಯಥಾ{
                        best_index = 0;
                    }
                }
                ಯದಿ(l.random && *(net.seen) < 64000){
                    best_index = rand()%l.n;
                }

                ಪೂರ್ಣಾಙ್ಕ box_index = index + locations*(l.classes + l.n) + (i*l.n + best_index) * l.coords;
                ಪೂರ್ಣಾಙ್ಕ tbox_index = truth_index + 1 + l.classes;

                box out = float_to_box(l.output + box_index, 1);
                out.x /= l.side;
                out.y /= l.side;
                ಯದಿ (l.sqrt) {
                    out.w = out.w*out.w;
                    out.h = out.h*out.h;
                }
                ಭಿನ್ನ iou  = box_iou(out, truth);

                //printf("%d,", best_index);
                ಪೂರ್ಣಾಙ್ಕ p_index = index + locations*l.classes + i*l.n + best_index;
                *(l.cost) -= l.noobject_scale * pow(l.output[p_index], 2);
                *(l.cost) += l.object_scale * pow(1-l.output[p_index], 2);
                avg_obj += l.output[p_index];
                l.delta[p_index] = l.object_scale * (1.-l.output[p_index]);

                ಯದಿ(l.rescore){
                    l.delta[p_index] = l.object_scale * (iou - l.output[p_index]);
                }

                l.delta[box_index+0] = l.coord_scale*(net.truth[tbox_index + 0] - l.output[box_index + 0]);
                l.delta[box_index+1] = l.coord_scale*(net.truth[tbox_index + 1] - l.output[box_index + 1]);
                l.delta[box_index+2] = l.coord_scale*(net.truth[tbox_index + 2] - l.output[box_index + 2]);
                l.delta[box_index+3] = l.coord_scale*(net.truth[tbox_index + 3] - l.output[box_index + 3]);
                ಯದಿ(l.sqrt){
                    l.delta[box_index+2] = l.coord_scale*(sqrt(net.truth[tbox_index + 2]) - l.output[box_index + 2]);
                    l.delta[box_index+3] = l.coord_scale*(sqrt(net.truth[tbox_index + 3]) - l.output[box_index + 3]);
                }

                *(l.cost) += pow(1-iou, 2);
                avg_iou += iou;
                ++ಎಣಿಕೆ;
            }
        }

        ಯದಿ(0){
            ಭಿನ್ನ *costs = calloc(l.batch*locations*l.n, ಪರಿಮಾಣ(ಭಿನ್ನ));
            ಪ್ರತಿ (b = 0; b < l.batch; ++b) {
                ಪೂರ್ಣಾಙ್ಕ index = b*l.inputs;
                ಪ್ರತಿ (i = 0; i < locations; ++i) {
                    ಪ್ರತಿ (j = 0; j < l.n; ++j) {
                        ಪೂರ್ಣಾಙ್ಕ p_index = index + locations*l.classes + i*l.n + j;
                        costs[b*locations*l.n + i*l.n + j] = l.delta[p_index]*l.delta[p_index];
                    }
                }
            }
            ಪೂರ್ಣಾಙ್ಕ indexes[100];
            ತುದಿ_k(costs, l.batch*locations*l.n, 100, indexes);
            ಭಿನ್ನ cutoff = costs[indexes[99]];
            ಪ್ರತಿ (b = 0; b < l.batch; ++b) {
                ಪೂರ್ಣಾಙ್ಕ index = b*l.inputs;
                ಪ್ರತಿ (i = 0; i < locations; ++i) {
                    ಪ್ರತಿ (j = 0; j < l.n; ++j) {
                        ಪೂರ್ಣಾಙ್ಕ p_index = index + locations*l.classes + i*l.n + j;
                        ಯದಿ (l.delta[p_index]*l.delta[p_index] < cutoff) l.delta[p_index] = 0;
                    }
                }
            }
            ಮುಕ್ತ(costs);
        }


        *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);


        printf("Detection Avg IOU: %f, Pos Cat: %f, All Cat: %f, Pos Obj: %f, Any Obj: %f, count: %d\n", avg_iou/ಎಣಿಕೆ, avg_cat/ಎಣಿಕೆ, avg_allcat/(ಎಣಿಕೆ*l.classes), avg_obj/ಎಣಿಕೆ, avg_anyobj/(l.batch*locations*l.n), ಎಣಿಕೆ);
        //if(l.reorg) reorg(l.delta, l.w*l.h, size*l.n, l.batch, 0);
    }
}

ಶೂನ್ಯ backward_detection_layer(ಧ್ರುವ detection_layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

ಶೂನ್ಯ get_detection_detections(layer l, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಭಿನ್ನ thresh, detection *dets)
{
    ಪೂರ್ಣಾಙ್ಕ i,j,n;
    ಭಿನ್ನ *predictions = l.output;
    //int per_cell = 5*num+classes;
    ಪ್ರತಿ (i = 0; i < l.side*l.side; ++i){
        ಪೂರ್ಣಾಙ್ಕ row = i / l.side;
        ಪೂರ್ಣಾಙ್ಕ col = i % l.side;
        ಪ್ರತಿ(n = 0; n < l.n; ++n){
            ಪೂರ್ಣಾಙ್ಕ index = i*l.n + n;
            ಪೂರ್ಣಾಙ್ಕ p_index = l.side*l.side*l.classes + i*l.n + n;
            ಭಿನ್ನ scale = predictions[p_index];
            ಪೂರ್ಣಾಙ್ಕ box_index = l.side*l.side*(l.classes + l.n) + (i*l.n + n)*4;
            box b;
            b.x = (predictions[box_index + 0] + col) / l.side * w;
            b.y = (predictions[box_index + 1] + row) / l.side * h;
            b.w = pow(predictions[box_index + 2], (l.sqrt?2:1)) * w;
            b.h = pow(predictions[box_index + 3], (l.sqrt?2:1)) * h;
            dets[index].bbox = b;
            dets[index].objectness = scale;
            ಪ್ರತಿ(j = 0; j < l.classes; ++j){
                ಪೂರ್ಣಾಙ್ಕ class_index = i*l.classes;
                ಭಿನ್ನ prob = scale*predictions[class_index+j];
                dets[index].prob[j] = (prob > thresh) ? prob : 0;
            }
        }
    }
}

#ifdef GPU

ಶೂನ್ಯ forward_detection_layer_gpu(ಧ್ರುವ detection_layer l, network net)
{
    ಯದಿ(!net.train){
        copy_gpu(l.batch*l.inputs, net.ಒಳಹರಿವು_gpu, 1, l.output_gpu, 1);
        ಪ್ರತ್ಯಾಗಮ;
    }

    cuda_pull_array(net.ಒಳಹರಿವು_gpu, net.ಒಳಹರಿವು, l.batch*l.inputs);
    forward_detection_layer(l, net);
    cuda_ತಳ್ಳು_array(l.output_gpu, l.output, l.batch*l.outputs);
    cuda_ತಳ್ಳು_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

ಶೂನ್ಯ backward_detection_layer_gpu(detection_layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
    //copy_gpu(l.batch*l.inputs, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


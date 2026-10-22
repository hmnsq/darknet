#அந்தர்பாவ "detection_layer.h"
#அந்தர்பாவ "activations.h"
#அந்தர்பாவ "softmax_layer.h"
#அந்தர்பாவ "blas.h"
#அந்தர்பாவ "box.h"
#அந்தர்பாவ "cuda.h"
#அந்தர்பாவ "utils.h"

#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <stdlib.h>

detection_layer make_detection_layer(முழு batch, முழு inputs, முழு n, முழு side, முழு classes, முழு coords, முழு rescore)
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
    ப்ரதிஜ்ஞா(side*side*((1 + l.coords)*l.n + l.classes) == inputs);
    l.cost = calloc(1, அளவு(தசம));
    l.outputs = l.inputs;
    l.truths = l.side*l.side*(1+l.coords+l.classes);
    l.output = calloc(batch*l.outputs, அளவு(தசம));
    l.delta = calloc(batch*l.outputs, அளவு(தசம));

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

    திரும்பு l;
}

வெறுமை forward_detection_layer(மாறிலி detection_layer l, network net)
{
    முழு locations = l.side*l.side;
    முழு i,j;
    memcpy(l.output, net.உள்ளீடு, l.outputs*l.batch*அளவு(தசம));
    //if(l.reorg) reorg(l.output, l.w*l.h, size*l.n, l.batch, 1);
    முழு b;
    எனில் (l.softmax){
        க்கு(b = 0; b < l.batch; ++b){
            முழு index = b*l.inputs;
            க்கு (i = 0; i < locations; ++i) {
                முழு offset = i*l.classes;
                softmax(l.output + index + offset, l.classes, 1, 1,
                        l.output + index + offset);
            }
        }
    }
    எனில்(net.train){
        தசம avg_iou = 0;
        தசம avg_cat = 0;
        தசம avg_allcat = 0;
        தசம avg_obj = 0;
        தசம avg_anyobj = 0;
        முழு மொத்தம் = 0;
        *(l.cost) = 0;
        முழு size = l.inputs * l.batch;
        memset(l.delta, 0, size * அளவு(தசம));
        க்கு (b = 0; b < l.batch; ++b){
            முழு index = b*l.inputs;
            க்கு (i = 0; i < locations; ++i) {
                முழு truth_index = (b*locations + i)*(1+l.coords+l.classes);
                முழு is_obj = net.truth[truth_index];
                க்கு (j = 0; j < l.n; ++j) {
                    முழு p_index = index + locations*l.classes + i*l.n + j;
                    l.delta[p_index] = l.noobject_scale*(0 - l.output[p_index]);
                    *(l.cost) += l.noobject_scale*pow(l.output[p_index], 2);
                    avg_anyobj += l.output[p_index];
                }

                முழு best_index = -1;
                தசம best_iou = 0;
                தசம best_rmse = 20;

                எனில் (!is_obj){
                    தொடர்;
                }

                முழு class_index = index + i*l.classes;
                க்கு(j = 0; j < l.classes; ++j) {
                    l.delta[class_index+j] = l.class_scale * (net.truth[truth_index+1+j] - l.output[class_index+j]);
                    *(l.cost) += l.class_scale * pow(net.truth[truth_index+1+j] - l.output[class_index+j], 2);
                    எனில்(net.truth[truth_index + 1 + j]) avg_cat += l.output[class_index+j];
                    avg_allcat += l.output[class_index+j];
                }

                box truth = float_to_box(net.truth + truth_index + 1 + l.classes, 1);
                truth.x /= l.side;
                truth.y /= l.side;

                க்கு(j = 0; j < l.n; ++j){
                    முழு box_index = index + locations*(l.classes + l.n) + (i*l.n + j) * l.coords;
                    box out = float_to_box(l.output + box_index, 1);
                    out.x /= l.side;
                    out.y /= l.side;

                    எனில் (l.sqrt){
                        out.w = out.w*out.w;
                        out.h = out.h*out.h;
                    }

                    தசம iou  = box_iou(out, truth);
                    //iou = 0;
                    தசம rmse = box_rmse(out, truth);
                    எனில்(best_iou > 0 || iou > 0){
                        எனில்(iou > best_iou){
                            best_iou = iou;
                            best_index = j;
                        }
                    }இல்லையெனில்{
                        எனில்(rmse < best_rmse){
                            best_rmse = rmse;
                            best_index = j;
                        }
                    }
                }

                எனில்(l.forced){
                    எனில்(truth.w*truth.h < .1){
                        best_index = 1;
                    }இல்லையெனில்{
                        best_index = 0;
                    }
                }
                எனில்(l.random && *(net.seen) < 64000){
                    best_index = rand()%l.n;
                }

                முழு box_index = index + locations*(l.classes + l.n) + (i*l.n + best_index) * l.coords;
                முழு tbox_index = truth_index + 1 + l.classes;

                box out = float_to_box(l.output + box_index, 1);
                out.x /= l.side;
                out.y /= l.side;
                எனில் (l.sqrt) {
                    out.w = out.w*out.w;
                    out.h = out.h*out.h;
                }
                தசம iou  = box_iou(out, truth);

                //printf("%d,", best_index);
                முழு p_index = index + locations*l.classes + i*l.n + best_index;
                *(l.cost) -= l.noobject_scale * pow(l.output[p_index], 2);
                *(l.cost) += l.object_scale * pow(1-l.output[p_index], 2);
                avg_obj += l.output[p_index];
                l.delta[p_index] = l.object_scale * (1.-l.output[p_index]);

                எனில்(l.rescore){
                    l.delta[p_index] = l.object_scale * (iou - l.output[p_index]);
                }

                l.delta[box_index+0] = l.coord_scale*(net.truth[tbox_index + 0] - l.output[box_index + 0]);
                l.delta[box_index+1] = l.coord_scale*(net.truth[tbox_index + 1] - l.output[box_index + 1]);
                l.delta[box_index+2] = l.coord_scale*(net.truth[tbox_index + 2] - l.output[box_index + 2]);
                l.delta[box_index+3] = l.coord_scale*(net.truth[tbox_index + 3] - l.output[box_index + 3]);
                எனில்(l.sqrt){
                    l.delta[box_index+2] = l.coord_scale*(sqrt(net.truth[tbox_index + 2]) - l.output[box_index + 2]);
                    l.delta[box_index+3] = l.coord_scale*(sqrt(net.truth[tbox_index + 3]) - l.output[box_index + 3]);
                }

                *(l.cost) += pow(1-iou, 2);
                avg_iou += iou;
                ++மொத்தம்;
            }
        }

        எனில்(0){
            தசம *costs = calloc(l.batch*locations*l.n, அளவு(தசம));
            க்கு (b = 0; b < l.batch; ++b) {
                முழு index = b*l.inputs;
                க்கு (i = 0; i < locations; ++i) {
                    க்கு (j = 0; j < l.n; ++j) {
                        முழு p_index = index + locations*l.classes + i*l.n + j;
                        costs[b*locations*l.n + i*l.n + j] = l.delta[p_index]*l.delta[p_index];
                    }
                }
            }
            முழு indexes[100];
            உச்சி_k(costs, l.batch*locations*l.n, 100, indexes);
            தசம cutoff = costs[indexes[99]];
            க்கு (b = 0; b < l.batch; ++b) {
                முழு index = b*l.inputs;
                க்கு (i = 0; i < locations; ++i) {
                    க்கு (j = 0; j < l.n; ++j) {
                        முழு p_index = index + locations*l.classes + i*l.n + j;
                        எனில் (l.delta[p_index]*l.delta[p_index] < cutoff) l.delta[p_index] = 0;
                    }
                }
            }
            விடுவி(costs);
        }


        *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);


        printf("Detection Avg IOU: %f, Pos Cat: %f, All Cat: %f, Pos Obj: %f, Any Obj: %f, count: %d\n", avg_iou/மொத்தம், avg_cat/மொத்தம், avg_allcat/(மொத்தம்*l.classes), avg_obj/மொத்தம், avg_anyobj/(l.batch*locations*l.n), மொத்தம்);
        //if(l.reorg) reorg(l.delta, l.w*l.h, size*l.n, l.batch, 0);
    }
}

வெறுமை backward_detection_layer(மாறிலி detection_layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

வெறுமை get_detection_detections(layer l, முழு w, முழு h, தசம thresh, detection *dets)
{
    முழு i,j,n;
    தசம *predictions = l.output;
    //int per_cell = 5*num+classes;
    க்கு (i = 0; i < l.side*l.side; ++i){
        முழு row = i / l.side;
        முழு col = i % l.side;
        க்கு(n = 0; n < l.n; ++n){
            முழு index = i*l.n + n;
            முழு p_index = l.side*l.side*l.classes + i*l.n + n;
            தசம scale = predictions[p_index];
            முழு box_index = l.side*l.side*(l.classes + l.n) + (i*l.n + n)*4;
            box b;
            b.x = (predictions[box_index + 0] + col) / l.side * w;
            b.y = (predictions[box_index + 1] + row) / l.side * h;
            b.w = pow(predictions[box_index + 2], (l.sqrt?2:1)) * w;
            b.h = pow(predictions[box_index + 3], (l.sqrt?2:1)) * h;
            dets[index].bbox = b;
            dets[index].objectness = scale;
            க்கு(j = 0; j < l.classes; ++j){
                முழு class_index = i*l.classes;
                தசம prob = scale*predictions[class_index+j];
                dets[index].prob[j] = (prob > thresh) ? prob : 0;
            }
        }
    }
}

#ifdef GPU

வெறுமை forward_detection_layer_gpu(மாறிலி detection_layer l, network net)
{
    எனில்(!net.train){
        copy_gpu(l.batch*l.inputs, net.உள்ளீடு_gpu, 1, l.output_gpu, 1);
        திரும்பு;
    }

    cuda_pull_array(net.உள்ளீடு_gpu, net.உள்ளீடு, l.batch*l.inputs);
    forward_detection_layer(l, net);
    cuda_தள்ளு_array(l.output_gpu, l.output, l.batch*l.outputs);
    cuda_தள்ளு_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

வெறுமை backward_detection_layer_gpu(detection_layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
    //copy_gpu(l.batch*l.inputs, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


#અન્તર્ભાવ "detection_layer.h"
#અન્તર્ભાવ "activations.h"
#અન્તર્ભાવ "softmax_layer.h"
#અન્તર્ભાવ "blas.h"
#અન્તર્ભાવ "box.h"
#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ "utils.h"

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <stdlib.h>

detection_layer make_detection_layer(પૂર્ણાંક batch, પૂર્ણાંક inputs, પૂર્ણાંક n, પૂર્ણાંક side, પૂર્ણાંક classes, પૂર્ણાંક coords, પૂર્ણાંક rescore)
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
    પ્રતિજ્ઞા(side*side*((1 + l.coords)*l.n + l.classes) == inputs);
    l.cost = calloc(1, કદ(દશાંશ));
    l.outputs = l.inputs;
    l.truths = l.side*l.side*(1+l.coords+l.classes);
    l.output = calloc(batch*l.outputs, કદ(દશાંશ));
    l.delta = calloc(batch*l.outputs, કદ(દશાંશ));

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

    પાછા l;
}

શૂન્ય forward_detection_layer(અચલ detection_layer l, network net)
{
    પૂર્ણાંક locations = l.side*l.side;
    પૂર્ણાંક i,j;
    memcpy(l.output, net.આગત, l.outputs*l.batch*કદ(દશાંશ));
    //if(l.reorg) reorg(l.output, l.w*l.h, size*l.n, l.batch, 1);
    પૂર્ણાંક b;
    જો (l.softmax){
        માટે(b = 0; b < l.batch; ++b){
            પૂર્ણાંક index = b*l.inputs;
            માટે (i = 0; i < locations; ++i) {
                પૂર્ણાંક offset = i*l.classes;
                softmax(l.output + index + offset, l.classes, 1, 1,
                        l.output + index + offset);
            }
        }
    }
    જો(net.train){
        દશાંશ avg_iou = 0;
        દશાંશ avg_cat = 0;
        દશાંશ avg_allcat = 0;
        દશાંશ avg_obj = 0;
        દશાંશ avg_anyobj = 0;
        પૂર્ણાંક ગણતરી = 0;
        *(l.cost) = 0;
        પૂર્ણાંક size = l.inputs * l.batch;
        memset(l.delta, 0, size * કદ(દશાંશ));
        માટે (b = 0; b < l.batch; ++b){
            પૂર્ણાંક index = b*l.inputs;
            માટે (i = 0; i < locations; ++i) {
                પૂર્ણાંક truth_index = (b*locations + i)*(1+l.coords+l.classes);
                પૂર્ણાંક is_obj = net.truth[truth_index];
                માટે (j = 0; j < l.n; ++j) {
                    પૂર્ણાંક p_index = index + locations*l.classes + i*l.n + j;
                    l.delta[p_index] = l.noobject_scale*(0 - l.output[p_index]);
                    *(l.cost) += l.noobject_scale*pow(l.output[p_index], 2);
                    avg_anyobj += l.output[p_index];
                }

                પૂર્ણાંક best_index = -1;
                દશાંશ best_iou = 0;
                દશાંશ best_rmse = 20;

                જો (!is_obj){
                    ચાલુ;
                }

                પૂર્ણાંક class_index = index + i*l.classes;
                માટે(j = 0; j < l.classes; ++j) {
                    l.delta[class_index+j] = l.class_scale * (net.truth[truth_index+1+j] - l.output[class_index+j]);
                    *(l.cost) += l.class_scale * pow(net.truth[truth_index+1+j] - l.output[class_index+j], 2);
                    જો(net.truth[truth_index + 1 + j]) avg_cat += l.output[class_index+j];
                    avg_allcat += l.output[class_index+j];
                }

                box truth = float_to_box(net.truth + truth_index + 1 + l.classes, 1);
                truth.x /= l.side;
                truth.y /= l.side;

                માટે(j = 0; j < l.n; ++j){
                    પૂર્ણાંક box_index = index + locations*(l.classes + l.n) + (i*l.n + j) * l.coords;
                    box out = float_to_box(l.output + box_index, 1);
                    out.x /= l.side;
                    out.y /= l.side;

                    જો (l.sqrt){
                        out.w = out.w*out.w;
                        out.h = out.h*out.h;
                    }

                    દશાંશ iou  = box_iou(out, truth);
                    //iou = 0;
                    દશાંશ rmse = box_rmse(out, truth);
                    જો(best_iou > 0 || iou > 0){
                        જો(iou > best_iou){
                            best_iou = iou;
                            best_index = j;
                        }
                    }નહીંતર{
                        જો(rmse < best_rmse){
                            best_rmse = rmse;
                            best_index = j;
                        }
                    }
                }

                જો(l.forced){
                    જો(truth.w*truth.h < .1){
                        best_index = 1;
                    }નહીંતર{
                        best_index = 0;
                    }
                }
                જો(l.random && *(net.seen) < 64000){
                    best_index = rand()%l.n;
                }

                પૂર્ણાંક box_index = index + locations*(l.classes + l.n) + (i*l.n + best_index) * l.coords;
                પૂર્ણાંક tbox_index = truth_index + 1 + l.classes;

                box out = float_to_box(l.output + box_index, 1);
                out.x /= l.side;
                out.y /= l.side;
                જો (l.sqrt) {
                    out.w = out.w*out.w;
                    out.h = out.h*out.h;
                }
                દશાંશ iou  = box_iou(out, truth);

                //printf("%d,", best_index);
                પૂર્ણાંક p_index = index + locations*l.classes + i*l.n + best_index;
                *(l.cost) -= l.noobject_scale * pow(l.output[p_index], 2);
                *(l.cost) += l.object_scale * pow(1-l.output[p_index], 2);
                avg_obj += l.output[p_index];
                l.delta[p_index] = l.object_scale * (1.-l.output[p_index]);

                જો(l.rescore){
                    l.delta[p_index] = l.object_scale * (iou - l.output[p_index]);
                }

                l.delta[box_index+0] = l.coord_scale*(net.truth[tbox_index + 0] - l.output[box_index + 0]);
                l.delta[box_index+1] = l.coord_scale*(net.truth[tbox_index + 1] - l.output[box_index + 1]);
                l.delta[box_index+2] = l.coord_scale*(net.truth[tbox_index + 2] - l.output[box_index + 2]);
                l.delta[box_index+3] = l.coord_scale*(net.truth[tbox_index + 3] - l.output[box_index + 3]);
                જો(l.sqrt){
                    l.delta[box_index+2] = l.coord_scale*(sqrt(net.truth[tbox_index + 2]) - l.output[box_index + 2]);
                    l.delta[box_index+3] = l.coord_scale*(sqrt(net.truth[tbox_index + 3]) - l.output[box_index + 3]);
                }

                *(l.cost) += pow(1-iou, 2);
                avg_iou += iou;
                ++ગણતરી;
            }
        }

        જો(0){
            દશાંશ *costs = calloc(l.batch*locations*l.n, કદ(દશાંશ));
            માટે (b = 0; b < l.batch; ++b) {
                પૂર્ણાંક index = b*l.inputs;
                માટે (i = 0; i < locations; ++i) {
                    માટે (j = 0; j < l.n; ++j) {
                        પૂર્ણાંક p_index = index + locations*l.classes + i*l.n + j;
                        costs[b*locations*l.n + i*l.n + j] = l.delta[p_index]*l.delta[p_index];
                    }
                }
            }
            પૂર્ણાંક indexes[100];
            ટોચ_k(costs, l.batch*locations*l.n, 100, indexes);
            દશાંશ cutoff = costs[indexes[99]];
            માટે (b = 0; b < l.batch; ++b) {
                પૂર્ણાંક index = b*l.inputs;
                માટે (i = 0; i < locations; ++i) {
                    માટે (j = 0; j < l.n; ++j) {
                        પૂર્ણાંક p_index = index + locations*l.classes + i*l.n + j;
                        જો (l.delta[p_index]*l.delta[p_index] < cutoff) l.delta[p_index] = 0;
                    }
                }
            }
            મુક્ત(costs);
        }


        *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);


        printf("Detection Avg IOU: %f, Pos Cat: %f, All Cat: %f, Pos Obj: %f, Any Obj: %f, count: %d\n", avg_iou/ગણતરી, avg_cat/ગણતરી, avg_allcat/(ગણતરી*l.classes), avg_obj/ગણતરી, avg_anyobj/(l.batch*locations*l.n), ગણતરી);
        //if(l.reorg) reorg(l.delta, l.w*l.h, size*l.n, l.batch, 0);
    }
}

શૂન્ય backward_detection_layer(અચલ detection_layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

શૂન્ય get_detection_detections(layer l, પૂર્ણાંક w, પૂર્ણાંક h, દશાંશ thresh, detection *dets)
{
    પૂર્ણાંક i,j,n;
    દશાંશ *predictions = l.output;
    //int per_cell = 5*num+classes;
    માટે (i = 0; i < l.side*l.side; ++i){
        પૂર્ણાંક row = i / l.side;
        પૂર્ણાંક col = i % l.side;
        માટે(n = 0; n < l.n; ++n){
            પૂર્ણાંક index = i*l.n + n;
            પૂર્ણાંક p_index = l.side*l.side*l.classes + i*l.n + n;
            દશાંશ scale = predictions[p_index];
            પૂર્ણાંક box_index = l.side*l.side*(l.classes + l.n) + (i*l.n + n)*4;
            box b;
            b.x = (predictions[box_index + 0] + col) / l.side * w;
            b.y = (predictions[box_index + 1] + row) / l.side * h;
            b.w = pow(predictions[box_index + 2], (l.sqrt?2:1)) * w;
            b.h = pow(predictions[box_index + 3], (l.sqrt?2:1)) * h;
            dets[index].bbox = b;
            dets[index].objectness = scale;
            માટે(j = 0; j < l.classes; ++j){
                પૂર્ણાંક class_index = i*l.classes;
                દશાંશ prob = scale*predictions[class_index+j];
                dets[index].prob[j] = (prob > thresh) ? prob : 0;
            }
        }
    }
}

#ifdef GPU

શૂન્ય forward_detection_layer_gpu(અચલ detection_layer l, network net)
{
    જો(!net.train){
        copy_gpu(l.batch*l.inputs, net.આગત_gpu, 1, l.output_gpu, 1);
        પાછા;
    }

    cuda_pull_array(net.આગત_gpu, net.આગત, l.batch*l.inputs);
    forward_detection_layer(l, net);
    cuda_ધકેલો_array(l.output_gpu, l.output, l.batch*l.outputs);
    cuda_ધકેલો_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

શૂન્ય backward_detection_layer_gpu(detection_layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
    //copy_gpu(l.batch*l.inputs, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


#include "detection_layer.h"
#include "activations.h"
#include "softmax_layer.h"
#include "blas.h"
#include "box.h"
#include "cuda.h"
#include "utils.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

detection_layer make_detection_layer(정수 batch, 정수 inputs, 정수 n, 정수 side, 정수 classes, 정수 coords, 정수 rescore)
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
    assert(side*side*((1 + l.coords)*l.n + l.classes) == inputs);
    l.cost = calloc(1, sizeof(float));
    l.outputs = l.inputs;
    l.truths = l.side*l.side*(1+l.coords+l.classes);
    l.output = calloc(batch*l.outputs, sizeof(float));
    l.delta = calloc(batch*l.outputs, sizeof(float));

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

    반환 l;
}

void forward_detection_layer(const detection_layer l, network net)
{
    정수 locations = l.side*l.side;
    정수 i,j;
    memcpy(l.output, net.input, l.outputs*l.batch*sizeof(float));
    //if(l.reorg) reorg(l.output, l.w*l.h, size*l.n, l.batch, 1);
    정수 b;
    만약 (l.softmax){
        위해(b = 0; b < l.batch; ++b){
            정수 index = b*l.inputs;
            위해 (i = 0; i < locations; ++i) {
                정수 offset = i*l.classes;
                softmax(l.output + index + offset, l.classes, 1, 1,
                        l.output + index + offset);
            }
        }
    }
    만약(net.train){
        float avg_iou = 0;
        float avg_cat = 0;
        float avg_allcat = 0;
        float avg_obj = 0;
        float avg_anyobj = 0;
        정수 count = 0;
        *(l.cost) = 0;
        정수 size = l.inputs * l.batch;
        memset(l.delta, 0, size * sizeof(float));
        위해 (b = 0; b < l.batch; ++b){
            정수 index = b*l.inputs;
            위해 (i = 0; i < locations; ++i) {
                정수 truth_index = (b*locations + i)*(1+l.coords+l.classes);
                정수 is_obj = net.truth[truth_index];
                위해 (j = 0; j < l.n; ++j) {
                    정수 p_index = index + locations*l.classes + i*l.n + j;
                    l.delta[p_index] = l.noobject_scale*(0 - l.output[p_index]);
                    *(l.cost) += l.noobject_scale*pow(l.output[p_index], 2);
                    avg_anyobj += l.output[p_index];
                }

                정수 best_index = -1;
                float best_iou = 0;
                float best_rmse = 20;

                만약 (!is_obj){
                    continue;
                }

                정수 class_index = index + i*l.classes;
                위해(j = 0; j < l.classes; ++j) {
                    l.delta[class_index+j] = l.class_scale * (net.truth[truth_index+1+j] - l.output[class_index+j]);
                    *(l.cost) += l.class_scale * pow(net.truth[truth_index+1+j] - l.output[class_index+j], 2);
                    만약(net.truth[truth_index + 1 + j]) avg_cat += l.output[class_index+j];
                    avg_allcat += l.output[class_index+j];
                }

                box truth = float_to_box(net.truth + truth_index + 1 + l.classes, 1);
                truth.x /= l.side;
                truth.y /= l.side;

                위해(j = 0; j < l.n; ++j){
                    정수 box_index = index + locations*(l.classes + l.n) + (i*l.n + j) * l.coords;
                    box out = float_to_box(l.output + box_index, 1);
                    out.x /= l.side;
                    out.y /= l.side;

                    만약 (l.sqrt){
                        out.w = out.w*out.w;
                        out.h = out.h*out.h;
                    }

                    float iou  = box_iou(out, truth);
                    //iou = 0;
                    float rmse = box_rmse(out, truth);
                    만약(best_iou > 0 || iou > 0){
                        만약(iou > best_iou){
                            best_iou = iou;
                            best_index = j;
                        }
                    }아니면{
                        만약(rmse < best_rmse){
                            best_rmse = rmse;
                            best_index = j;
                        }
                    }
                }

                만약(l.forced){
                    만약(truth.w*truth.h < .1){
                        best_index = 1;
                    }아니면{
                        best_index = 0;
                    }
                }
                만약(l.random && *(net.seen) < 64000){
                    best_index = rand()%l.n;
                }

                정수 box_index = index + locations*(l.classes + l.n) + (i*l.n + best_index) * l.coords;
                정수 tbox_index = truth_index + 1 + l.classes;

                box out = float_to_box(l.output + box_index, 1);
                out.x /= l.side;
                out.y /= l.side;
                만약 (l.sqrt) {
                    out.w = out.w*out.w;
                    out.h = out.h*out.h;
                }
                float iou  = box_iou(out, truth);

                //printf("%d,", best_index);
                정수 p_index = index + locations*l.classes + i*l.n + best_index;
                *(l.cost) -= l.noobject_scale * pow(l.output[p_index], 2);
                *(l.cost) += l.object_scale * pow(1-l.output[p_index], 2);
                avg_obj += l.output[p_index];
                l.delta[p_index] = l.object_scale * (1.-l.output[p_index]);

                만약(l.rescore){
                    l.delta[p_index] = l.object_scale * (iou - l.output[p_index]);
                }

                l.delta[box_index+0] = l.coord_scale*(net.truth[tbox_index + 0] - l.output[box_index + 0]);
                l.delta[box_index+1] = l.coord_scale*(net.truth[tbox_index + 1] - l.output[box_index + 1]);
                l.delta[box_index+2] = l.coord_scale*(net.truth[tbox_index + 2] - l.output[box_index + 2]);
                l.delta[box_index+3] = l.coord_scale*(net.truth[tbox_index + 3] - l.output[box_index + 3]);
                만약(l.sqrt){
                    l.delta[box_index+2] = l.coord_scale*(sqrt(net.truth[tbox_index + 2]) - l.output[box_index + 2]);
                    l.delta[box_index+3] = l.coord_scale*(sqrt(net.truth[tbox_index + 3]) - l.output[box_index + 3]);
                }

                *(l.cost) += pow(1-iou, 2);
                avg_iou += iou;
                ++count;
            }
        }

        만약(0){
            float *costs = calloc(l.batch*locations*l.n, sizeof(float));
            위해 (b = 0; b < l.batch; ++b) {
                정수 index = b*l.inputs;
                위해 (i = 0; i < locations; ++i) {
                    위해 (j = 0; j < l.n; ++j) {
                        정수 p_index = index + locations*l.classes + i*l.n + j;
                        costs[b*locations*l.n + i*l.n + j] = l.delta[p_index]*l.delta[p_index];
                    }
                }
            }
            정수 indexes[100];
            top_k(costs, l.batch*locations*l.n, 100, indexes);
            float cutoff = costs[indexes[99]];
            위해 (b = 0; b < l.batch; ++b) {
                정수 index = b*l.inputs;
                위해 (i = 0; i < locations; ++i) {
                    위해 (j = 0; j < l.n; ++j) {
                        정수 p_index = index + locations*l.classes + i*l.n + j;
                        만약 (l.delta[p_index]*l.delta[p_index] < cutoff) l.delta[p_index] = 0;
                    }
                }
            }
            free(costs);
        }


        *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);


        printf("Detection Avg IOU: %f, Pos Cat: %f, All Cat: %f, Pos Obj: %f, Any Obj: %f, count: %d\n", avg_iou/count, avg_cat/count, avg_allcat/(count*l.classes), avg_obj/count, avg_anyobj/(l.batch*locations*l.n), count);
        //if(l.reorg) reorg(l.delta, l.w*l.h, size*l.n, l.batch, 0);
    }
}

void backward_detection_layer(const detection_layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

void get_detection_detections(layer l, 정수 w, 정수 h, float thresh, detection *dets)
{
    정수 i,j,n;
    float *predictions = l.output;
    //int per_cell = 5*num+classes;
    위해 (i = 0; i < l.side*l.side; ++i){
        정수 row = i / l.side;
        정수 col = i % l.side;
        위해(n = 0; n < l.n; ++n){
            정수 index = i*l.n + n;
            정수 p_index = l.side*l.side*l.classes + i*l.n + n;
            float scale = predictions[p_index];
            정수 box_index = l.side*l.side*(l.classes + l.n) + (i*l.n + n)*4;
            box b;
            b.x = (predictions[box_index + 0] + col) / l.side * w;
            b.y = (predictions[box_index + 1] + row) / l.side * h;
            b.w = pow(predictions[box_index + 2], (l.sqrt?2:1)) * w;
            b.h = pow(predictions[box_index + 3], (l.sqrt?2:1)) * h;
            dets[index].bbox = b;
            dets[index].objectness = scale;
            위해(j = 0; j < l.classes; ++j){
                정수 class_index = i*l.classes;
                float prob = scale*predictions[class_index+j];
                dets[index].prob[j] = (prob > thresh) ? prob : 0;
            }
        }
    }
}

#ifdef GPU

void forward_detection_layer_gpu(const detection_layer l, network net)
{
    만약(!net.train){
        copy_gpu(l.batch*l.inputs, net.input_gpu, 1, l.output_gpu, 1);
        반환;
    }

    cuda_pull_array(net.input_gpu, net.input, l.batch*l.inputs);
    forward_detection_layer(l, net);
    cuda_push_array(l.output_gpu, l.output, l.batch*l.outputs);
    cuda_push_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

void backward_detection_layer_gpu(detection_layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
    //copy_gpu(l.batch*l.inputs, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


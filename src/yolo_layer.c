#include "yolo_layer.h"
#include "activations.h"
#include "blas.h"
#include "box.h"
#include "cuda.h"
#include "utils.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

layer make_yolo_layer(정수 batch, 정수 w, 정수 h, 정수 n, 정수 total, 정수 *mask, 정수 classes)
{
    정수 i;
    layer l = {0};
    l.type = YOLO;

    l.n = n;
    l.total = total;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = n*(classes + 4 + 1);
    l.out_w = l.w;
    l.out_h = l.h;
    l.out_c = l.c;
    l.classes = classes;
    l.cost = calloc(1, sizeof(float));
    l.biases = calloc(total*2, sizeof(float));
    만약(mask) l.mask = mask;
    아니면{
        l.mask = calloc(n, sizeof(정수));
        위해(i = 0; i < n; ++i){
            l.mask[i] = i;
        }
    }
    l.bias_updates = calloc(n*2, sizeof(float));
    l.outputs = h*w*n*(classes + 4 + 1);
    l.inputs = l.outputs;
    l.truths = 90*(4 + 1);
    l.delta = calloc(batch*l.outputs, sizeof(float));
    l.output = calloc(batch*l.outputs, sizeof(float));
    위해(i = 0; i < total*2; ++i){
        l.biases[i] = .5;
    }

    l.forward = forward_yolo_layer;
    l.backward = backward_yolo_layer;
#ifdef GPU
    l.forward_gpu = forward_yolo_layer_gpu;
    l.backward_gpu = backward_yolo_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#endif

    fprintf(stderr, "yolo\n");
    srand(0);

    반환 l;
}

void resize_yolo_layer(layer *l, 정수 w, 정수 h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + 4 + 1);
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*sizeof(float));
    l->delta = realloc(l->delta, l->batch*l->outputs*sizeof(float));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

box get_yolo_box(float *x, float *biases, 정수 n, 정수 index, 정수 i, 정수 j, 정수 lw, 정수 lh, 정수 w, 정수 h, 정수 stride)
{
    box b;
    b.x = (i + x[index + 0*stride]) / lw;
    b.y = (j + x[index + 1*stride]) / lh;
    b.w = exp(x[index + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[index + 3*stride]) * biases[2*n+1] / h;
    반환 b;
}

float delta_yolo_box(box truth, float *x, float *biases, 정수 n, 정수 index, 정수 i, 정수 j, 정수 lw, 정수 lh, 정수 w, 정수 h, float *delta, float scale, 정수 stride)
{
    box pred = get_yolo_box(x, biases, n, index, i, j, lw, lh, w, h, stride);
    float iou = box_iou(pred, truth);

    float tx = (truth.x*lw - i);
    float ty = (truth.y*lh - j);
    float tw = log(truth.w*w / biases[2*n]);
    float th = log(truth.h*h / biases[2*n + 1]);

    delta[index + 0*stride] = scale * (tx - x[index + 0*stride]);
    delta[index + 1*stride] = scale * (ty - x[index + 1*stride]);
    delta[index + 2*stride] = scale * (tw - x[index + 2*stride]);
    delta[index + 3*stride] = scale * (th - x[index + 3*stride]);
    반환 iou;
}


void delta_yolo_class(float *output, float *delta, 정수 index, 정수 class, 정수 classes, 정수 stride, float *avg_cat)
{
    정수 n;
    만약 (delta[index]){
        delta[index + stride*class] = 1 - output[index + stride*class];
        만약(avg_cat) *avg_cat += output[index + stride*class];
        반환;
    }
    위해(n = 0; n < classes; ++n){
        delta[index + stride*n] = ((n == class)?1 : 0) - output[index + stride*n];
        만약(n == class && avg_cat) *avg_cat += output[index + stride*n];
    }
}

static 정수 entry_index(layer l, 정수 batch, 정수 location, 정수 entry)
{
    정수 n =   location / (l.w*l.h);
    정수 loc = location % (l.w*l.h);
    반환 batch*l.outputs + n*l.w*l.h*(4+l.classes+1) + entry*l.w*l.h + loc;
}

void forward_yolo_layer(const layer l, network net)
{
    정수 i,j,b,t,n;
    memcpy(l.output, net.input, l.outputs*l.batch*sizeof(float));

#ifndef GPU
    위해 (b = 0; b < l.batch; ++b){
        위해(n = 0; n < l.n; ++n){
            정수 index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array(l.output + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array(l.output + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
#endif

    memset(l.delta, 0, l.outputs * l.batch * sizeof(float));
    만약(!net.train) 반환;
    float avg_iou = 0;
    float recall = 0;
    float recall75 = 0;
    float avg_cat = 0;
    float avg_obj = 0;
    float avg_anyobj = 0;
    정수 count = 0;
    정수 class_count = 0;
    *(l.cost) = 0;
    위해 (b = 0; b < l.batch; ++b) {
        위해 (j = 0; j < l.h; ++j) {
            위해 (i = 0; i < l.w; ++i) {
                위해 (n = 0; n < l.n; ++n) {
                    정수 box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_yolo_box(l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.w*l.h);
                    float best_iou = 0;
                    정수 best_t = 0;
                    위해(t = 0; t < l.max_boxes; ++t){
                        box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);
                        만약(!truth.x) break;
                        float iou = box_iou(pred, truth);
                        만약 (iou > best_iou) {
                            best_iou = iou;
                            best_t = t;
                        }
                    }
                    정수 obj_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4);
                    avg_anyobj += l.output[obj_index];
                    l.delta[obj_index] = 0 - l.output[obj_index];
                    만약 (best_iou > l.ignore_thresh) {
                        l.delta[obj_index] = 0;
                    }
                    만약 (best_iou > l.truth_thresh) {
                        l.delta[obj_index] = 1 - l.output[obj_index];

                        정수 class = net.truth[best_t*(4 + 1) + b*l.truths + 4];
                        만약 (l.map) class = l.map[class];
                        정수 class_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4 + 1);
                        delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, 0);
                        box truth = float_to_box(net.truth + best_t*(4 + 1) + b*l.truths, 1);
                        delta_yolo_box(truth, l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);
                    }
                }
            }
        }
        위해(t = 0; t < l.max_boxes; ++t){
            box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);

            만약(!truth.x) break;
            float best_iou = 0;
            정수 best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = truth_shift.y = 0;
            위해(n = 0; n < l.total; ++n){
                box pred = {0};
                pred.w = l.biases[2*n]/net.w;
                pred.h = l.biases[2*n+1]/net.h;
                float iou = box_iou(pred, truth_shift);
                만약 (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            정수 mask_n = int_index(l.mask, best_n, l.n);
            만약(mask_n >= 0){
                정수 box_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 0);
                float iou = delta_yolo_box(truth, l.output, l.biases, best_n, box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);

                정수 obj_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4);
                avg_obj += l.output[obj_index];
                l.delta[obj_index] = 1 - l.output[obj_index];

                정수 class = net.truth[t*(4 + 1) + b*l.truths + 4];
                만약 (l.map) class = l.map[class];
                정수 class_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4 + 1);
                delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, &avg_cat);

                ++count;
                ++class_count;
                만약(iou > .5) recall += 1;
                만약(iou > .75) recall75 += 1;
                avg_iou += iou;
            }
        }
    }
    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("Region %d Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, .5R: %f, .75R: %f,  count: %d\n", net.index, avg_iou/count, avg_cat/class_count, avg_obj/count, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/count, recall75/count, count);
}

void backward_yolo_layer(const layer l, network net)
{
   axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

void correct_yolo_boxes(detection *dets, 정수 n, 정수 w, 정수 h, 정수 netw, 정수 neth, 정수 relative)
{
    정수 i;
    정수 new_w=0;
    정수 new_h=0;
    만약 (((float)netw/w) < ((float)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } 아니면 {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    위해 (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((float)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((float)new_h/neth); 
        b.w *= (float)netw/new_w;
        b.h *= (float)neth/new_h;
        만약(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

정수 yolo_num_detections(layer l, float thresh)
{
    정수 i, n;
    정수 count = 0;
    위해 (i = 0; i < l.w*l.h; ++i){
        위해(n = 0; n < l.n; ++n){
            정수 obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            만약(l.output[obj_index] > thresh){
                ++count;
            }
        }
    }
    반환 count;
}

void avg_flipped_yolo(layer l)
{
    정수 i,j,n,z;
    float *flip = l.output + l.outputs;
    위해 (j = 0; j < l.h; ++j) {
        위해 (i = 0; i < l.w/2; ++i) {
            위해 (n = 0; n < l.n; ++n) {
                위해(z = 0; z < l.classes + 4 + 1; ++z){
                    정수 i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                    정수 i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                    float swap = flip[i1];
                    flip[i1] = flip[i2];
                    flip[i2] = swap;
                    만약(z == 0){
                        flip[i1] = -flip[i1];
                        flip[i2] = -flip[i2];
                    }
                }
            }
        }
    }
    위해(i = 0; i < l.outputs; ++i){
        l.output[i] = (l.output[i] + flip[i])/2.;
    }
}

정수 get_yolo_detections(layer l, 정수 w, 정수 h, 정수 netw, 정수 neth, float thresh, 정수 *map, 정수 relative, detection *dets)
{
    정수 i,j,n;
    float *predictions = l.output;
    만약 (l.batch == 2) avg_flipped_yolo(l);
    정수 count = 0;
    위해 (i = 0; i < l.w*l.h; ++i){
        정수 row = i / l.w;
        정수 col = i % l.w;
        위해(n = 0; n < l.n; ++n){
            정수 obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            float objectness = predictions[obj_index];
            만약(objectness <= thresh) continue;
            정수 box_index  = entry_index(l, 0, n*l.w*l.h + i, 0);
            dets[count].bbox = get_yolo_box(predictions, l.biases, l.mask[n], box_index, col, row, l.w, l.h, netw, neth, l.w*l.h);
            dets[count].objectness = objectness;
            dets[count].classes = l.classes;
            위해(j = 0; j < l.classes; ++j){
                정수 class_index = entry_index(l, 0, n*l.w*l.h + i, 4 + 1 + j);
                float prob = objectness*predictions[class_index];
                dets[count].prob[j] = (prob > thresh) ? prob : 0;
            }
            ++count;
        }
    }
    correct_yolo_boxes(dets, count, w, h, netw, neth, relative);
    반환 count;
}

#ifdef GPU

void forward_yolo_layer_gpu(const layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.input_gpu, 1, l.output_gpu, 1);
    정수 b, n;
    위해 (b = 0; b < l.batch; ++b){
        위해(n = 0; n < l.n; ++n){
            정수 index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array_gpu(l.output_gpu + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
    만약(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        반환;
    }

    cuda_pull_array(l.output_gpu, net.input, l.batch*l.inputs);
    forward_yolo_layer(l, net);
    cuda_push_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

void backward_yolo_layer_gpu(const layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


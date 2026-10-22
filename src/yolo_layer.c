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

layer make_yolo_layer(ကိန်းပြည့် batch, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် n, ကိန်းပြည့် total, ကိန်းပြည့် *mask, ကိန်းပြည့် classes)
{
    ကိန်းပြည့် i;
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
    l.cost = calloc(1, parimaa_na(float));
    l.biases = calloc(total*2, parimaa_na(float));
    အကယ်၍(mask) l.mask = mask;
    မဟုတ်ပါက{
        l.mask = calloc(n, parimaa_na(ကိန်းပြည့်));
        အတွက်(i = 0; i < n; ++i){
            l.mask[i] = i;
        }
    }
    l.bias_updates = calloc(n*2, parimaa_na(float));
    l.outputs = h*w*n*(classes + 4 + 1);
    l.inputs = l.outputs;
    l.truths = 90*(4 + 1);
    l.delta = calloc(batch*l.outputs, parimaa_na(float));
    l.output = calloc(batch*l.outputs, parimaa_na(float));
    အတွက်(i = 0; i < total*2; ++i){
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

    ပြန်ပေး l;
}

void resize_yolo_layer(layer *l, ကိန်းပြည့် w, ကိန်းပြည့် h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + 4 + 1);
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*parimaa_na(float));
    l->delta = realloc(l->delta, l->batch*l->outputs*parimaa_na(float));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

box get_yolo_box(float *x, float *biases, ကိန်းပြည့် n, ကိန်းပြည့် index, ကိန်းပြည့် i, ကိန်းပြည့် j, ကိန်းပြည့် lw, ကိန်းပြည့် lh, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် stride)
{
    box b;
    b.x = (i + x[index + 0*stride]) / lw;
    b.y = (j + x[index + 1*stride]) / lh;
    b.w = exp(x[index + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[index + 3*stride]) * biases[2*n+1] / h;
    ပြန်ပေး b;
}

float delta_yolo_box(box truth, float *x, float *biases, ကိန်းပြည့် n, ကိန်းပြည့် index, ကိန်းပြည့် i, ကိန်းပြည့် j, ကိန်းပြည့် lw, ကိန်းပြည့် lh, ကိန်းပြည့် w, ကိန်းပြည့် h, float *delta, float scale, ကိန်းပြည့် stride)
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
    ပြန်ပေး iou;
}


void delta_yolo_class(float *output, float *delta, ကိန်းပြည့် index, ကိန်းပြည့် class, ကိန်းပြည့် classes, ကိန်းပြည့် stride, float *avg_cat)
{
    ကိန်းပြည့် n;
    အကယ်၍ (delta[index]){
        delta[index + stride*class] = 1 - output[index + stride*class];
        အကယ်၍(avg_cat) *avg_cat += output[index + stride*class];
        ပြန်ပေး;
    }
    အတွက်(n = 0; n < classes; ++n){
        delta[index + stride*n] = ((n == class)?1 : 0) - output[index + stride*n];
        အကယ်၍(n == class && avg_cat) *avg_cat += output[index + stride*n];
    }
}

static ကိန်းပြည့် entry_index(layer l, ကိန်းပြည့် batch, ကိန်းပြည့် location, ကိန်းပြည့် entry)
{
    ကိန်းပြည့် n =   location / (l.w*l.h);
    ကိန်းပြည့် loc = location % (l.w*l.h);
    ပြန်ပေး batch*l.outputs + n*l.w*l.h*(4+l.classes+1) + entry*l.w*l.h + loc;
}

void forward_yolo_layer(const layer l, network net)
{
    ကိန်းပြည့် i,j,b,t,n;
    memcpy(l.output, net.input, l.outputs*l.batch*parimaa_na(float));

#ifndef GPU
    အတွက် (b = 0; b < l.batch; ++b){
        အတွက်(n = 0; n < l.n; ++n){
            ကိန်းပြည့် index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array(l.output + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array(l.output + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
#endif

    memset(l.delta, 0, l.outputs * l.batch * parimaa_na(float));
    အကယ်၍(!net.train) ပြန်ပေး;
    float avg_iou = 0;
    float recall = 0;
    float recall75 = 0;
    float avg_cat = 0;
    float avg_obj = 0;
    float avg_anyobj = 0;
    ကိန်းပြည့် count = 0;
    ကိန်းပြည့် class_count = 0;
    *(l.cost) = 0;
    အတွက် (b = 0; b < l.batch; ++b) {
        အတွက် (j = 0; j < l.h; ++j) {
            အတွက် (i = 0; i < l.w; ++i) {
                အတွက် (n = 0; n < l.n; ++n) {
                    ကိန်းပြည့် box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_yolo_box(l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.w*l.h);
                    float best_iou = 0;
                    ကိန်းပြည့် best_t = 0;
                    အတွက်(t = 0; t < l.max_boxes; ++t){
                        box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);
                        အကယ်၍(!truth.x) bha_nga;
                        float iou = box_iou(pred, truth);
                        အကယ်၍ (iou > best_iou) {
                            best_iou = iou;
                            best_t = t;
                        }
                    }
                    ကိန်းပြည့် obj_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4);
                    avg_anyobj += l.output[obj_index];
                    l.delta[obj_index] = 0 - l.output[obj_index];
                    အကယ်၍ (best_iou > l.ignore_thresh) {
                        l.delta[obj_index] = 0;
                    }
                    အကယ်၍ (best_iou > l.truth_thresh) {
                        l.delta[obj_index] = 1 - l.output[obj_index];

                        ကိန်းပြည့် class = net.truth[best_t*(4 + 1) + b*l.truths + 4];
                        အကယ်၍ (l.map) class = l.map[class];
                        ကိန်းပြည့် class_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4 + 1);
                        delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, 0);
                        box truth = float_to_box(net.truth + best_t*(4 + 1) + b*l.truths, 1);
                        delta_yolo_box(truth, l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);
                    }
                }
            }
        }
        အတွက်(t = 0; t < l.max_boxes; ++t){
            box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);

            အကယ်၍(!truth.x) bha_nga;
            float best_iou = 0;
            ကိန်းပြည့် best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = truth_shift.y = 0;
            အတွက်(n = 0; n < l.total; ++n){
                box pred = {0};
                pred.w = l.biases[2*n]/net.w;
                pred.h = l.biases[2*n+1]/net.h;
                float iou = box_iou(pred, truth_shift);
                အကယ်၍ (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            ကိန်းပြည့် mask_n = int_index(l.mask, best_n, l.n);
            အကယ်၍(mask_n >= 0){
                ကိန်းပြည့် box_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 0);
                float iou = delta_yolo_box(truth, l.output, l.biases, best_n, box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);

                ကိန်းပြည့် obj_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4);
                avg_obj += l.output[obj_index];
                l.delta[obj_index] = 1 - l.output[obj_index];

                ကိန်းပြည့် class = net.truth[t*(4 + 1) + b*l.truths + 4];
                အကယ်၍ (l.map) class = l.map[class];
                ကိန်းပြည့် class_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4 + 1);
                delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, &avg_cat);

                ++count;
                ++class_count;
                အကယ်၍(iou > .5) recall += 1;
                အကယ်၍(iou > .75) recall75 += 1;
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

void correct_yolo_boxes(detection *dets, ကိန်းပြည့် n, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် netw, ကိန်းပြည့် neth, ကိန်းပြည့် relative)
{
    ကိန်းပြည့် i;
    ကိန်းပြည့် new_w=0;
    ကိန်းပြည့် new_h=0;
    အကယ်၍ (((float)netw/w) < ((float)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } မဟုတ်ပါက {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    အတွက် (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((float)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((float)new_h/neth); 
        b.w *= (float)netw/new_w;
        b.h *= (float)neth/new_h;
        အကယ်၍(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

ကိန်းပြည့် yolo_num_detections(layer l, float thresh)
{
    ကိန်းပြည့် i, n;
    ကိန်းပြည့် count = 0;
    အတွက် (i = 0; i < l.w*l.h; ++i){
        အတွက်(n = 0; n < l.n; ++n){
            ကိန်းပြည့် obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            အကယ်၍(l.output[obj_index] > thresh){
                ++count;
            }
        }
    }
    ပြန်ပေး count;
}

void avg_flipped_yolo(layer l)
{
    ကိန်းပြည့် i,j,n,z;
    float *flip = l.output + l.outputs;
    အတွက် (j = 0; j < l.h; ++j) {
        အတွက် (i = 0; i < l.w/2; ++i) {
            အတွက် (n = 0; n < l.n; ++n) {
                အတွက်(z = 0; z < l.classes + 4 + 1; ++z){
                    ကိန်းပြည့် i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                    ကိန်းပြည့် i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                    float swap = flip[i1];
                    flip[i1] = flip[i2];
                    flip[i2] = swap;
                    အကယ်၍(z == 0){
                        flip[i1] = -flip[i1];
                        flip[i2] = -flip[i2];
                    }
                }
            }
        }
    }
    အတွက်(i = 0; i < l.outputs; ++i){
        l.output[i] = (l.output[i] + flip[i])/2.;
    }
}

ကိန်းပြည့် get_yolo_detections(layer l, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် netw, ကိန်းပြည့် neth, float thresh, ကိန်းပြည့် *map, ကိန်းပြည့် relative, detection *dets)
{
    ကိန်းပြည့် i,j,n;
    float *predictions = l.output;
    အကယ်၍ (l.batch == 2) avg_flipped_yolo(l);
    ကိန်းပြည့် count = 0;
    အတွက် (i = 0; i < l.w*l.h; ++i){
        ကိန်းပြည့် row = i / l.w;
        ကိန်းပြည့် col = i % l.w;
        အတွက်(n = 0; n < l.n; ++n){
            ကိန်းပြည့် obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            float objectness = predictions[obj_index];
            အကယ်၍(objectness <= thresh) continue;
            ကိန်းပြည့် box_index  = entry_index(l, 0, n*l.w*l.h + i, 0);
            dets[count].bbox = get_yolo_box(predictions, l.biases, l.mask[n], box_index, col, row, l.w, l.h, netw, neth, l.w*l.h);
            dets[count].objectness = objectness;
            dets[count].classes = l.classes;
            အတွက်(j = 0; j < l.classes; ++j){
                ကိန်းပြည့် class_index = entry_index(l, 0, n*l.w*l.h + i, 4 + 1 + j);
                float prob = objectness*predictions[class_index];
                dets[count].prob[j] = (prob > thresh) ? prob : 0;
            }
            ++count;
        }
    }
    correct_yolo_boxes(dets, count, w, h, netw, neth, relative);
    ပြန်ပေး count;
}

#ifdef GPU

void forward_yolo_layer_gpu(const layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.input_gpu, 1, l.output_gpu, 1);
    ကိန်းပြည့် b, n;
    အတွက် (b = 0; b < l.batch; ++b){
        အတွက်(n = 0; n < l.n; ++n){
            ကိန်းပြည့် index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array_gpu(l.output_gpu + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
    အကယ်၍(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        ပြန်ပေး;
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


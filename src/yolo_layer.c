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

layer make_yolo_layer(ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ total, ຈຳນວນເຕັມ *mask, ຈຳນວນເຕັມ classes)
{
    ຈຳນວນເຕັມ i;
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
    ຖ້າ(mask) l.mask = mask;
    ບໍ່ດັ່ງນັ້ນ{
        l.mask = calloc(n, parimaa_na(ຈຳນວນເຕັມ));
        ສຳລັບ(i = 0; i < n; ++i){
            l.mask[i] = i;
        }
    }
    l.bias_updates = calloc(n*2, parimaa_na(float));
    l.outputs = h*w*n*(classes + 4 + 1);
    l.inputs = l.outputs;
    l.truths = 90*(4 + 1);
    l.delta = calloc(batch*l.outputs, parimaa_na(float));
    l.output = calloc(batch*l.outputs, parimaa_na(float));
    ສຳລັບ(i = 0; i < total*2; ++i){
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

    ສົ່ງຄືນ l;
}

void resize_yolo_layer(layer *l, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h)
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

box get_yolo_box(float *x, float *biases, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ index, ຈຳນວນເຕັມ i, ຈຳນວນເຕັມ j, ຈຳນວນເຕັມ lw, ຈຳນວນເຕັມ lh, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ stride)
{
    box b;
    b.x = (i + x[index + 0*stride]) / lw;
    b.y = (j + x[index + 1*stride]) / lh;
    b.w = exp(x[index + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[index + 3*stride]) * biases[2*n+1] / h;
    ສົ່ງຄືນ b;
}

float delta_yolo_box(box truth, float *x, float *biases, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ index, ຈຳນວນເຕັມ i, ຈຳນວນເຕັມ j, ຈຳນວນເຕັມ lw, ຈຳນວນເຕັມ lh, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, float *delta, float scale, ຈຳນວນເຕັມ stride)
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
    ສົ່ງຄືນ iou;
}


void delta_yolo_class(float *output, float *delta, ຈຳນວນເຕັມ index, ຈຳນວນເຕັມ class, ຈຳນວນເຕັມ classes, ຈຳນວນເຕັມ stride, float *avg_cat)
{
    ຈຳນວນເຕັມ n;
    ຖ້າ (delta[index]){
        delta[index + stride*class] = 1 - output[index + stride*class];
        ຖ້າ(avg_cat) *avg_cat += output[index + stride*class];
        ສົ່ງຄືນ;
    }
    ສຳລັບ(n = 0; n < classes; ++n){
        delta[index + stride*n] = ((n == class)?1 : 0) - output[index + stride*n];
        ຖ້າ(n == class && avg_cat) *avg_cat += output[index + stride*n];
    }
}

static ຈຳນວນເຕັມ entry_index(layer l, ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ location, ຈຳນວນເຕັມ entry)
{
    ຈຳນວນເຕັມ n =   location / (l.w*l.h);
    ຈຳນວນເຕັມ loc = location % (l.w*l.h);
    ສົ່ງຄືນ batch*l.outputs + n*l.w*l.h*(4+l.classes+1) + entry*l.w*l.h + loc;
}

void forward_yolo_layer(const layer l, network net)
{
    ຈຳນວນເຕັມ i,j,b,t,n;
    memcpy(l.output, net.input, l.outputs*l.batch*parimaa_na(float));

#ifndef GPU
    ສຳລັບ (b = 0; b < l.batch; ++b){
        ສຳລັບ(n = 0; n < l.n; ++n){
            ຈຳນວນເຕັມ index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array(l.output + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array(l.output + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
#endif

    memset(l.delta, 0, l.outputs * l.batch * parimaa_na(float));
    ຖ້າ(!net.train) ສົ່ງຄືນ;
    float avg_iou = 0;
    float recall = 0;
    float recall75 = 0;
    float avg_cat = 0;
    float avg_obj = 0;
    float avg_anyobj = 0;
    ຈຳນວນເຕັມ count = 0;
    ຈຳນວນເຕັມ class_count = 0;
    *(l.cost) = 0;
    ສຳລັບ (b = 0; b < l.batch; ++b) {
        ສຳລັບ (j = 0; j < l.h; ++j) {
            ສຳລັບ (i = 0; i < l.w; ++i) {
                ສຳລັບ (n = 0; n < l.n; ++n) {
                    ຈຳນວນເຕັມ box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_yolo_box(l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.w*l.h);
                    float best_iou = 0;
                    ຈຳນວນເຕັມ best_t = 0;
                    ສຳລັບ(t = 0; t < l.max_boxes; ++t){
                        box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);
                        ຖ້າ(!truth.x) bha_nga;
                        float iou = box_iou(pred, truth);
                        ຖ້າ (iou > best_iou) {
                            best_iou = iou;
                            best_t = t;
                        }
                    }
                    ຈຳນວນເຕັມ obj_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4);
                    avg_anyobj += l.output[obj_index];
                    l.delta[obj_index] = 0 - l.output[obj_index];
                    ຖ້າ (best_iou > l.ignore_thresh) {
                        l.delta[obj_index] = 0;
                    }
                    ຖ້າ (best_iou > l.truth_thresh) {
                        l.delta[obj_index] = 1 - l.output[obj_index];

                        ຈຳນວນເຕັມ class = net.truth[best_t*(4 + 1) + b*l.truths + 4];
                        ຖ້າ (l.map) class = l.map[class];
                        ຈຳນວນເຕັມ class_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4 + 1);
                        delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, 0);
                        box truth = float_to_box(net.truth + best_t*(4 + 1) + b*l.truths, 1);
                        delta_yolo_box(truth, l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);
                    }
                }
            }
        }
        ສຳລັບ(t = 0; t < l.max_boxes; ++t){
            box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);

            ຖ້າ(!truth.x) bha_nga;
            float best_iou = 0;
            ຈຳນວນເຕັມ best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = truth_shift.y = 0;
            ສຳລັບ(n = 0; n < l.total; ++n){
                box pred = {0};
                pred.w = l.biases[2*n]/net.w;
                pred.h = l.biases[2*n+1]/net.h;
                float iou = box_iou(pred, truth_shift);
                ຖ້າ (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            ຈຳນວນເຕັມ mask_n = int_index(l.mask, best_n, l.n);
            ຖ້າ(mask_n >= 0){
                ຈຳນວນເຕັມ box_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 0);
                float iou = delta_yolo_box(truth, l.output, l.biases, best_n, box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);

                ຈຳນວນເຕັມ obj_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4);
                avg_obj += l.output[obj_index];
                l.delta[obj_index] = 1 - l.output[obj_index];

                ຈຳນວນເຕັມ class = net.truth[t*(4 + 1) + b*l.truths + 4];
                ຖ້າ (l.map) class = l.map[class];
                ຈຳນວນເຕັມ class_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4 + 1);
                delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, &avg_cat);

                ++count;
                ++class_count;
                ຖ້າ(iou > .5) recall += 1;
                ຖ້າ(iou > .75) recall75 += 1;
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

void correct_yolo_boxes(detection *dets, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ netw, ຈຳນວນເຕັມ neth, ຈຳນວນເຕັມ relative)
{
    ຈຳນວນເຕັມ i;
    ຈຳນວນເຕັມ new_w=0;
    ຈຳນວນເຕັມ new_h=0;
    ຖ້າ (((float)netw/w) < ((float)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } ບໍ່ດັ່ງນັ້ນ {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    ສຳລັບ (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((float)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((float)new_h/neth); 
        b.w *= (float)netw/new_w;
        b.h *= (float)neth/new_h;
        ຖ້າ(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

ຈຳນວນເຕັມ yolo_num_detections(layer l, float thresh)
{
    ຈຳນວນເຕັມ i, n;
    ຈຳນວນເຕັມ count = 0;
    ສຳລັບ (i = 0; i < l.w*l.h; ++i){
        ສຳລັບ(n = 0; n < l.n; ++n){
            ຈຳນວນເຕັມ obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            ຖ້າ(l.output[obj_index] > thresh){
                ++count;
            }
        }
    }
    ສົ່ງຄືນ count;
}

void avg_flipped_yolo(layer l)
{
    ຈຳນວນເຕັມ i,j,n,z;
    float *flip = l.output + l.outputs;
    ສຳລັບ (j = 0; j < l.h; ++j) {
        ສຳລັບ (i = 0; i < l.w/2; ++i) {
            ສຳລັບ (n = 0; n < l.n; ++n) {
                ສຳລັບ(z = 0; z < l.classes + 4 + 1; ++z){
                    ຈຳນວນເຕັມ i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                    ຈຳນວນເຕັມ i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                    float swap = flip[i1];
                    flip[i1] = flip[i2];
                    flip[i2] = swap;
                    ຖ້າ(z == 0){
                        flip[i1] = -flip[i1];
                        flip[i2] = -flip[i2];
                    }
                }
            }
        }
    }
    ສຳລັບ(i = 0; i < l.outputs; ++i){
        l.output[i] = (l.output[i] + flip[i])/2.;
    }
}

ຈຳນວນເຕັມ get_yolo_detections(layer l, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ netw, ຈຳນວນເຕັມ neth, float thresh, ຈຳນວນເຕັມ *map, ຈຳນວນເຕັມ relative, detection *dets)
{
    ຈຳນວນເຕັມ i,j,n;
    float *predictions = l.output;
    ຖ້າ (l.batch == 2) avg_flipped_yolo(l);
    ຈຳນວນເຕັມ count = 0;
    ສຳລັບ (i = 0; i < l.w*l.h; ++i){
        ຈຳນວນເຕັມ row = i / l.w;
        ຈຳນວນເຕັມ col = i % l.w;
        ສຳລັບ(n = 0; n < l.n; ++n){
            ຈຳນວນເຕັມ obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            float objectness = predictions[obj_index];
            ຖ້າ(objectness <= thresh) continue;
            ຈຳນວນເຕັມ box_index  = entry_index(l, 0, n*l.w*l.h + i, 0);
            dets[count].bbox = get_yolo_box(predictions, l.biases, l.mask[n], box_index, col, row, l.w, l.h, netw, neth, l.w*l.h);
            dets[count].objectness = objectness;
            dets[count].classes = l.classes;
            ສຳລັບ(j = 0; j < l.classes; ++j){
                ຈຳນວນເຕັມ class_index = entry_index(l, 0, n*l.w*l.h + i, 4 + 1 + j);
                float prob = objectness*predictions[class_index];
                dets[count].prob[j] = (prob > thresh) ? prob : 0;
            }
            ++count;
        }
    }
    correct_yolo_boxes(dets, count, w, h, netw, neth, relative);
    ສົ່ງຄືນ count;
}

#ifdef GPU

void forward_yolo_layer_gpu(const layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.input_gpu, 1, l.output_gpu, 1);
    ຈຳນວນເຕັມ b, n;
    ສຳລັບ (b = 0; b < l.batch; ++b){
        ສຳລັບ(n = 0; n < l.n; ++n){
            ຈຳນວນເຕັມ index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array_gpu(l.output_gpu + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
    ຖ້າ(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        ສົ່ງຄືນ;
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


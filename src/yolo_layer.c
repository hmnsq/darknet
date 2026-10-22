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

layer make_yolo_layer(puur_naa_nka batch, puur_naa_nka w, puur_naa_nka h, puur_naa_nka n, puur_naa_nka total, puur_naa_nka *mask, puur_naa_nka classes)
{
    puur_naa_nka i;
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
    ᮚᮦᮔ᮪(mask) l.mask = mask;
    ᮜᮤᮚ{
        l.mask = calloc(n, parimaa_na(puur_naa_nka));
        for(i = 0; i < n; ++i){
            l.mask[i] = i;
        }
    }
    l.bias_updates = calloc(n*2, parimaa_na(float));
    l.outputs = h*w*n*(classes + 4 + 1);
    l.inputs = l.outputs;
    l.truths = 90*(4 + 1);
    l.delta = calloc(batch*l.outputs, parimaa_na(float));
    l.output = calloc(batch*l.outputs, parimaa_na(float));
    for(i = 0; i < total*2; ++i){
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

    ᮝᮜᮤ l;
}

void resize_yolo_layer(layer *l, puur_naa_nka w, puur_naa_nka h)
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

box get_yolo_box(float *x, float *biases, puur_naa_nka n, puur_naa_nka index, puur_naa_nka i, puur_naa_nka j, puur_naa_nka lw, puur_naa_nka lh, puur_naa_nka w, puur_naa_nka h, puur_naa_nka stride)
{
    box b;
    b.x = (i + x[index + 0*stride]) / lw;
    b.y = (j + x[index + 1*stride]) / lh;
    b.w = exp(x[index + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[index + 3*stride]) * biases[2*n+1] / h;
    ᮝᮜᮤ b;
}

float delta_yolo_box(box truth, float *x, float *biases, puur_naa_nka n, puur_naa_nka index, puur_naa_nka i, puur_naa_nka j, puur_naa_nka lw, puur_naa_nka lh, puur_naa_nka w, puur_naa_nka h, float *delta, float scale, puur_naa_nka stride)
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
    ᮝᮜᮤ iou;
}


void delta_yolo_class(float *output, float *delta, puur_naa_nka index, puur_naa_nka class, puur_naa_nka classes, puur_naa_nka stride, float *avg_cat)
{
    puur_naa_nka n;
    ᮚᮦᮔ᮪ (delta[index]){
        delta[index + stride*class] = 1 - output[index + stride*class];
        ᮚᮦᮔ᮪(avg_cat) *avg_cat += output[index + stride*class];
        ᮝᮜᮤ;
    }
    for(n = 0; n < classes; ++n){
        delta[index + stride*n] = ((n == class)?1 : 0) - output[index + stride*n];
        ᮚᮦᮔ᮪(n == class && avg_cat) *avg_cat += output[index + stride*n];
    }
}

static puur_naa_nka entry_index(layer l, puur_naa_nka batch, puur_naa_nka location, puur_naa_nka entry)
{
    puur_naa_nka n =   location / (l.w*l.h);
    puur_naa_nka loc = location % (l.w*l.h);
    ᮝᮜᮤ batch*l.outputs + n*l.w*l.h*(4+l.classes+1) + entry*l.w*l.h + loc;
}

void forward_yolo_layer(const layer l, network net)
{
    puur_naa_nka i,j,b,t,n;
    memcpy(l.output, net.input, l.outputs*l.batch*parimaa_na(float));

#ifndef GPU
    for (b = 0; b < l.batch; ++b){
        for(n = 0; n < l.n; ++n){
            puur_naa_nka index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array(l.output + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array(l.output + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
#endif

    memset(l.delta, 0, l.outputs * l.batch * parimaa_na(float));
    ᮚᮦᮔ᮪(!net.train) ᮝᮜᮤ;
    float avg_iou = 0;
    float recall = 0;
    float recall75 = 0;
    float avg_cat = 0;
    float avg_obj = 0;
    float avg_anyobj = 0;
    puur_naa_nka count = 0;
    puur_naa_nka class_count = 0;
    *(l.cost) = 0;
    for (b = 0; b < l.batch; ++b) {
        for (j = 0; j < l.h; ++j) {
            for (i = 0; i < l.w; ++i) {
                for (n = 0; n < l.n; ++n) {
                    puur_naa_nka box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_yolo_box(l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.w*l.h);
                    float best_iou = 0;
                    puur_naa_nka best_t = 0;
                    for(t = 0; t < l.max_boxes; ++t){
                        box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);
                        ᮚᮦᮔ᮪(!truth.x) bha_nga;
                        float iou = box_iou(pred, truth);
                        ᮚᮦᮔ᮪ (iou > best_iou) {
                            best_iou = iou;
                            best_t = t;
                        }
                    }
                    puur_naa_nka obj_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4);
                    avg_anyobj += l.output[obj_index];
                    l.delta[obj_index] = 0 - l.output[obj_index];
                    ᮚᮦᮔ᮪ (best_iou > l.ignore_thresh) {
                        l.delta[obj_index] = 0;
                    }
                    ᮚᮦᮔ᮪ (best_iou > l.truth_thresh) {
                        l.delta[obj_index] = 1 - l.output[obj_index];

                        puur_naa_nka class = net.truth[best_t*(4 + 1) + b*l.truths + 4];
                        ᮚᮦᮔ᮪ (l.map) class = l.map[class];
                        puur_naa_nka class_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4 + 1);
                        delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, 0);
                        box truth = float_to_box(net.truth + best_t*(4 + 1) + b*l.truths, 1);
                        delta_yolo_box(truth, l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);
                    }
                }
            }
        }
        for(t = 0; t < l.max_boxes; ++t){
            box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);

            ᮚᮦᮔ᮪(!truth.x) bha_nga;
            float best_iou = 0;
            puur_naa_nka best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = truth_shift.y = 0;
            for(n = 0; n < l.total; ++n){
                box pred = {0};
                pred.w = l.biases[2*n]/net.w;
                pred.h = l.biases[2*n+1]/net.h;
                float iou = box_iou(pred, truth_shift);
                ᮚᮦᮔ᮪ (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            puur_naa_nka mask_n = int_index(l.mask, best_n, l.n);
            ᮚᮦᮔ᮪(mask_n >= 0){
                puur_naa_nka box_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 0);
                float iou = delta_yolo_box(truth, l.output, l.biases, best_n, box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);

                puur_naa_nka obj_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4);
                avg_obj += l.output[obj_index];
                l.delta[obj_index] = 1 - l.output[obj_index];

                puur_naa_nka class = net.truth[t*(4 + 1) + b*l.truths + 4];
                ᮚᮦᮔ᮪ (l.map) class = l.map[class];
                puur_naa_nka class_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4 + 1);
                delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, &avg_cat);

                ++count;
                ++class_count;
                ᮚᮦᮔ᮪(iou > .5) recall += 1;
                ᮚᮦᮔ᮪(iou > .75) recall75 += 1;
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

void correct_yolo_boxes(detection *dets, puur_naa_nka n, puur_naa_nka w, puur_naa_nka h, puur_naa_nka netw, puur_naa_nka neth, puur_naa_nka relative)
{
    puur_naa_nka i;
    puur_naa_nka new_w=0;
    puur_naa_nka new_h=0;
    ᮚᮦᮔ᮪ (((float)netw/w) < ((float)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } ᮜᮤᮚ {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    for (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((float)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((float)new_h/neth); 
        b.w *= (float)netw/new_w;
        b.h *= (float)neth/new_h;
        ᮚᮦᮔ᮪(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

puur_naa_nka yolo_num_detections(layer l, float thresh)
{
    puur_naa_nka i, n;
    puur_naa_nka count = 0;
    for (i = 0; i < l.w*l.h; ++i){
        for(n = 0; n < l.n; ++n){
            puur_naa_nka obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            ᮚᮦᮔ᮪(l.output[obj_index] > thresh){
                ++count;
            }
        }
    }
    ᮝᮜᮤ count;
}

void avg_flipped_yolo(layer l)
{
    puur_naa_nka i,j,n,z;
    float *flip = l.output + l.outputs;
    for (j = 0; j < l.h; ++j) {
        for (i = 0; i < l.w/2; ++i) {
            for (n = 0; n < l.n; ++n) {
                for(z = 0; z < l.classes + 4 + 1; ++z){
                    puur_naa_nka i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                    puur_naa_nka i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                    float swap = flip[i1];
                    flip[i1] = flip[i2];
                    flip[i2] = swap;
                    ᮚᮦᮔ᮪(z == 0){
                        flip[i1] = -flip[i1];
                        flip[i2] = -flip[i2];
                    }
                }
            }
        }
    }
    for(i = 0; i < l.outputs; ++i){
        l.output[i] = (l.output[i] + flip[i])/2.;
    }
}

puur_naa_nka get_yolo_detections(layer l, puur_naa_nka w, puur_naa_nka h, puur_naa_nka netw, puur_naa_nka neth, float thresh, puur_naa_nka *map, puur_naa_nka relative, detection *dets)
{
    puur_naa_nka i,j,n;
    float *predictions = l.output;
    ᮚᮦᮔ᮪ (l.batch == 2) avg_flipped_yolo(l);
    puur_naa_nka count = 0;
    for (i = 0; i < l.w*l.h; ++i){
        puur_naa_nka row = i / l.w;
        puur_naa_nka col = i % l.w;
        for(n = 0; n < l.n; ++n){
            puur_naa_nka obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            float objectness = predictions[obj_index];
            ᮚᮦᮔ᮪(objectness <= thresh) continue;
            puur_naa_nka box_index  = entry_index(l, 0, n*l.w*l.h + i, 0);
            dets[count].bbox = get_yolo_box(predictions, l.biases, l.mask[n], box_index, col, row, l.w, l.h, netw, neth, l.w*l.h);
            dets[count].objectness = objectness;
            dets[count].classes = l.classes;
            for(j = 0; j < l.classes; ++j){
                puur_naa_nka class_index = entry_index(l, 0, n*l.w*l.h + i, 4 + 1 + j);
                float prob = objectness*predictions[class_index];
                dets[count].prob[j] = (prob > thresh) ? prob : 0;
            }
            ++count;
        }
    }
    correct_yolo_boxes(dets, count, w, h, netw, neth, relative);
    ᮝᮜᮤ count;
}

#ifdef GPU

void forward_yolo_layer_gpu(const layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.input_gpu, 1, l.output_gpu, 1);
    puur_naa_nka b, n;
    for (b = 0; b < l.batch; ++b){
        for(n = 0; n < l.n; ++n){
            puur_naa_nka index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array_gpu(l.output_gpu + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
    ᮚᮦᮔ᮪(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        ᮝᮜᮤ;
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


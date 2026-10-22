#include "region_layer.h"
#include "activations.h"
#include "blas.h"
#include "box.h"
#include "cuda.h"
#include "utils.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

layer make_region_layer(ကိန်းပြည့် batch, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် n, ကိန်းပြည့် classes, ကိန်းပြည့် coords)
{
    layer l = {0};
    l.type = REGION;

    l.n = n;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = n*(classes + coords + 1);
    l.out_w = l.w;
    l.out_h = l.h;
    l.out_c = l.c;
    l.classes = classes;
    l.coords = coords;
    l.cost = calloc(1, parimaa_na(float));
    l.biases = calloc(n*2, parimaa_na(float));
    l.bias_updates = calloc(n*2, parimaa_na(float));
    l.outputs = h*w*n*(classes + coords + 1);
    l.inputs = l.outputs;
    l.truths = 30*(l.coords + 1);
    l.delta = calloc(batch*l.outputs, parimaa_na(float));
    l.output = calloc(batch*l.outputs, parimaa_na(float));
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < n*2; ++i){
        l.biases[i] = .5;
    }

    l.forward = forward_region_layer;
    l.backward = backward_region_layer;
#ifdef GPU
    l.forward_gpu = forward_region_layer_gpu;
    l.backward_gpu = backward_region_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#endif

    fprintf(stderr, "detection\n");
    srand(0);

    ပြန်ပေး l;
}

void resize_region_layer(layer *l, ကိန်းပြည့် w, ကိန်းပြည့် h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + l->coords + 1);
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

box get_region_box(float *x, float *biases, ကိန်းပြည့် n, ကိန်းပြည့် index, ကိန်းပြည့် i, ကိန်းပြည့် j, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် stride)
{
    box b;
    b.x = (i + x[index + 0*stride]) / w;
    b.y = (j + x[index + 1*stride]) / h;
    b.w = exp(x[index + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[index + 3*stride]) * biases[2*n+1] / h;
    ပြန်ပေး b;
}

float delta_region_box(box truth, float *x, float *biases, ကိန်းပြည့် n, ကိန်းပြည့် index, ကိန်းပြည့် i, ကိန်းပြည့် j, ကိန်းပြည့် w, ကိန်းပြည့် h, float *delta, float scale, ကိန်းပြည့် stride)
{
    box pred = get_region_box(x, biases, n, index, i, j, w, h, stride);
    float iou = box_iou(pred, truth);

    float tx = (truth.x*w - i);
    float ty = (truth.y*h - j);
    float tw = log(truth.w*w / biases[2*n]);
    float th = log(truth.h*h / biases[2*n + 1]);

    delta[index + 0*stride] = scale * (tx - x[index + 0*stride]);
    delta[index + 1*stride] = scale * (ty - x[index + 1*stride]);
    delta[index + 2*stride] = scale * (tw - x[index + 2*stride]);
    delta[index + 3*stride] = scale * (th - x[index + 3*stride]);
    ပြန်ပေး iou;
}

void delta_region_mask(float *truth, float *x, ကိန်းပြည့် n, ကိန်းပြည့် index, float *delta, ကိန်းပြည့် stride, ကိန်းပြည့် scale)
{
    ကိန်းပြည့် i;
    အတွက်(i = 0; i < n; ++i){
        delta[index + i*stride] = scale*(truth[i] - x[index + i*stride]);
    }
}


void delta_region_class(float *output, float *delta, ကိန်းပြည့် index, ကိန်းပြည့် class, ကိန်းပြည့် classes, tree *hier, float scale, ကိန်းပြည့် stride, float *avg_cat, ကိန်းပြည့် tag)
{
    ကိန်းပြည့် i, n;
    အကယ်၍(hier){
        float pred = 1;
        နေစဉ်(class >= 0){
            pred *= output[index + stride*class];
            ကိန်းပြည့် g = hier->group[class];
            ကိန်းပြည့် offset = hier->group_offset[g];
            အတွက်(i = 0; i < hier->group_size[g]; ++i){
                delta[index + stride*(offset + i)] = scale * (0 - output[index + stride*(offset + i)]);
            }
            delta[index + stride*class] = scale * (1 - output[index + stride*class]);

            class = hier->parent[class];
        }
        *avg_cat += pred;
    } မဟုတ်ပါက {
        အကယ်၍ (delta[index] && tag){
            delta[index + stride*class] = scale * (1 - output[index + stride*class]);
            ပြန်ပေး;
        }
        အတွက်(n = 0; n < classes; ++n){
            delta[index + stride*n] = scale * (((n == class)?1 : 0) - output[index + stride*n]);
            အကယ်၍(n == class) *avg_cat += output[index + stride*n];
        }
    }
}

float logit(float x)
{
    ပြန်ပေး log(x/(1.-x));
}

float tisnan(float x)
{
    ပြန်ပေး (x != x);
}

ကိန်းပြည့် entry_index(layer l, ကိန်းပြည့် batch, ကိန်းပြည့် location, ကိန်းပြည့် entry)
{
    ကိန်းပြည့် n =   location / (l.w*l.h);
    ကိန်းပြည့် loc = location % (l.w*l.h);
    ပြန်ပေး batch*l.outputs + n*l.w*l.h*(l.coords+l.classes+1) + entry*l.w*l.h + loc;
}

void forward_region_layer(const layer l, network net)
{
    ကိန်းပြည့် i,j,b,t,n;
    memcpy(l.output, net.input, l.outputs*l.batch*parimaa_na(float));

#ifndef GPU
    အတွက် (b = 0; b < l.batch; ++b){
        အတွက်(n = 0; n < l.n; ++n){
            ကိန်းပြည့် index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array(l.output + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            အကယ်၍(!l.background) activate_array(l.output + index,   l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords + 1);
            အကယ်၍(!l.softmax && !l.softmax_tree) activate_array(l.output + index, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    အကယ်၍ (l.softmax_tree){
        ကိန်းပြည့် i;
        ကိန်းပြည့် count = l.coords + 1;
        အတွက် (i = 0; i < l.softmax_tree->groups; ++i) {
            ကိန်းပြည့် group_size = l.softmax_tree->group_size[i];
            softmax_cpu(net.input + count, group_size, l.batch, l.inputs, l.n*l.w*l.h, 1, l.n*l.w*l.h, l.temperature, l.output + count);
            count += group_size;
        }
    } မဟုတ်ပါက အကယ်၍ (l.softmax){
        ကိန်းပြည့် index = entry_index(l, 0, 0, l.coords + !l.background);
        softmax_cpu(net.input + index, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output + index);
    }
#endif

    memset(l.delta, 0, l.outputs * l.batch * parimaa_na(float));
    အကယ်၍(!net.train) ပြန်ပေး;
    float avg_iou = 0;
    float recall = 0;
    float avg_cat = 0;
    float avg_obj = 0;
    float avg_anyobj = 0;
    ကိန်းပြည့် count = 0;
    ကိန်းပြည့် class_count = 0;
    *(l.cost) = 0;
    အတွက် (b = 0; b < l.batch; ++b) {
        အကယ်၍(l.softmax_tree){
            ကိန်းပြည့် onlyclass = 0;
            အတွက်(t = 0; t < 30; ++t){
                box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                အကယ်၍(!truth.x) bha_nga;
                ကိန်းပြည့် class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
                float maxp = 0;
                ကိန်းပြည့် maxi = 0;
                အကယ်၍(truth.x > 100000 && truth.y > 100000){
                    အတွက်(n = 0; n < l.n*l.w*l.h; ++n){
                        ကိန်းပြည့် class_index = entry_index(l, b, n, l.coords + 1);
                        ကိန်းပြည့် obj_index = entry_index(l, b, n, l.coords);
                        float scale =  l.output[obj_index];
                        l.delta[obj_index] = l.noobject_scale * (0 - l.output[obj_index]);
                        float p = scale*get_hierarchy_probability(l.output + class_index, l.softmax_tree, class, l.w*l.h);
                        အကယ်၍(p > maxp){
                            maxp = p;
                            maxi = n;
                        }
                    }
                    ကိန်းပြည့် class_index = entry_index(l, b, maxi, l.coords + 1);
                    ကိန်းပြည့် obj_index = entry_index(l, b, maxi, l.coords);
                    delta_region_class(l.output, l.delta, class_index, class, l.classes, l.softmax_tree, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
                    အကယ်၍(l.output[obj_index] < .3) l.delta[obj_index] = l.object_scale * (.3 - l.output[obj_index]);
                    မဟုတ်ပါက  l.delta[obj_index] = 0;
                    l.delta[obj_index] = 0;
                    ++class_count;
                    onlyclass = 1;
                    bha_nga;
                }
            }
            အကယ်၍(onlyclass) continue;
        }
        အတွက် (j = 0; j < l.h; ++j) {
            အတွက် (i = 0; i < l.w; ++i) {
                အတွက် (n = 0; n < l.n; ++n) {
                    ကိန်းပြည့် box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_region_box(l.output, l.biases, n, box_index, i, j, l.w, l.h, l.w*l.h);
                    float best_iou = 0;
                    အတွက်(t = 0; t < 30; ++t){
                        box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                        အကယ်၍(!truth.x) bha_nga;
                        float iou = box_iou(pred, truth);
                        အကယ်၍ (iou > best_iou) {
                            best_iou = iou;
                        }
                    }
                    ကိန်းပြည့် obj_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, l.coords);
                    avg_anyobj += l.output[obj_index];
                    l.delta[obj_index] = l.noobject_scale * (0 - l.output[obj_index]);
                    အကယ်၍(l.background) l.delta[obj_index] = l.noobject_scale * (1 - l.output[obj_index]);
                    အကယ်၍ (best_iou > l.thresh) {
                        l.delta[obj_index] = 0;
                    }

                    အကယ်၍(*(net.seen) < 12800){
                        box truth = {0};
                        truth.x = (i + .5)/l.w;
                        truth.y = (j + .5)/l.h;
                        truth.w = l.biases[2*n]/l.w;
                        truth.h = l.biases[2*n+1]/l.h;
                        delta_region_box(truth, l.output, l.biases, n, box_index, i, j, l.w, l.h, l.delta, .01, l.w*l.h);
                    }
                }
            }
        }
        အတွက်(t = 0; t < 30; ++t){
            box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);

            အကယ်၍(!truth.x) bha_nga;
            float best_iou = 0;
            ကိန်းပြည့် best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = 0;
            truth_shift.y = 0;
            အတွက်(n = 0; n < l.n; ++n){
                ကိန်းပြည့် box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                box pred = get_region_box(l.output, l.biases, n, box_index, i, j, l.w, l.h, l.w*l.h);
                အကယ်၍(l.bias_match){
                    pred.w = l.biases[2*n]/l.w;
                    pred.h = l.biases[2*n+1]/l.h;
                }
                pred.x = 0;
                pred.y = 0;
                float iou = box_iou(pred, truth_shift);
                အကယ်၍ (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            ကိန်းပြည့် box_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, 0);
            float iou = delta_region_box(truth, l.output, l.biases, best_n, box_index, i, j, l.w, l.h, l.delta, l.coord_scale *  (2 - truth.w*truth.h), l.w*l.h);
            အကယ်၍(l.coords > 4){
                ကိန်းပြည့် mask_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, 4);
                delta_region_mask(net.truth + t*(l.coords + 1) + b*l.truths + 5, l.output, l.coords - 4, mask_index, l.delta, l.w*l.h, l.mask_scale);
            }
            အကယ်၍(iou > .5) recall += 1;
            avg_iou += iou;

            ကိန်းပြည့် obj_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, l.coords);
            avg_obj += l.output[obj_index];
            l.delta[obj_index] = l.object_scale * (1 - l.output[obj_index]);
            အကယ်၍ (l.rescore) {
                l.delta[obj_index] = l.object_scale * (iou - l.output[obj_index]);
            }
            အကယ်၍(l.background){
                l.delta[obj_index] = l.object_scale * (0 - l.output[obj_index]);
            }

            ကိန်းပြည့် class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
            အကယ်၍ (l.map) class = l.map[class];
            ကိန်းပြည့် class_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, l.coords + 1);
            delta_region_class(l.output, l.delta, class_index, class, l.classes, l.softmax_tree, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
            ++count;
            ++class_count;
        }
    }
    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("Region Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, Avg Recall: %f,  count: %d\n", avg_iou/count, avg_cat/class_count, avg_obj/count, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/count, count);
}

void backward_region_layer(const layer l, network net)
{
    /*
       int b;
       int size = l.coords + l.classes + 1;
       for (b = 0; b < l.batch*l.n; ++b){
       int index = (b*size + 4)*l.w*l.h;
       gradient_array(l.output + index, l.w*l.h, LOGISTIC, l.delta + index);
       }
       axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
     */
}

void correct_region_boxes(detection *dets, ကိန်းပြည့် n, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် netw, ကိန်းပြည့် neth, ကိန်းပြည့် relative)
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

void get_region_detections(layer l, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် netw, ကိန်းပြည့် neth, float thresh, ကိန်းပြည့် *map, float tree_thresh, ကိန်းပြည့် relative, detection *dets)
{
    ကိန်းပြည့် i,j,n,z;
    float *predictions = l.output;
    အကယ်၍ (l.batch == 2) {
        float *flip = l.output + l.outputs;
        အတွက် (j = 0; j < l.h; ++j) {
            အတွက် (i = 0; i < l.w/2; ++i) {
                အတွက် (n = 0; n < l.n; ++n) {
                    အတွက်(z = 0; z < l.classes + l.coords + 1; ++z){
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
    အတွက် (i = 0; i < l.w*l.h; ++i){
        ကိန်းပြည့် row = i / l.w;
        ကိန်းပြည့် col = i % l.w;
        အတွက်(n = 0; n < l.n; ++n){
            ကိန်းပြည့် index = n*l.w*l.h + i;
            အတွက်(j = 0; j < l.classes; ++j){
                dets[index].prob[j] = 0;
            }
            ကိန်းပြည့် obj_index  = entry_index(l, 0, n*l.w*l.h + i, l.coords);
            ကိန်းပြည့် box_index  = entry_index(l, 0, n*l.w*l.h + i, 0);
            ကိန်းပြည့် mask_index = entry_index(l, 0, n*l.w*l.h + i, 4);
            float scale = l.background ? 1 : predictions[obj_index];
            dets[index].bbox = get_region_box(predictions, l.biases, n, box_index, col, row, l.w, l.h, l.w*l.h);
            dets[index].objectness = scale > thresh ? scale : 0;
            အကယ်၍(dets[index].mask){
                အတွက်(j = 0; j < l.coords - 4; ++j){
                    dets[index].mask[j] = l.output[mask_index + j*l.w*l.h];
                }
            }

            ကိန်းပြည့် class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + !l.background);
            အကယ်၍(l.softmax_tree){

                hierarchy_predictions(predictions + class_index, l.classes, l.softmax_tree, 0, l.w*l.h);
                အကယ်၍(map){
                    အတွက်(j = 0; j < 200; ++j){
                        ကိန်းပြည့် class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + 1 + map[j]);
                        float prob = scale*predictions[class_index];
                        dets[index].prob[j] = (prob > thresh) ? prob : 0;
                    }
                } မဟုတ်ပါက {
                    ကိန်းပြည့် j =  hierarchy_top_prediction(predictions + class_index, l.softmax_tree, tree_thresh, l.w*l.h);
                    dets[index].prob[j] = (scale > thresh) ? scale : 0;
                }
            } မဟုတ်ပါက {
                အကယ်၍(dets[index].objectness){
                    အတွက်(j = 0; j < l.classes; ++j){
                        ကိန်းပြည့် class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + 1 + j);
                        float prob = scale*predictions[class_index];
                        dets[index].prob[j] = (prob > thresh) ? prob : 0;
                    }
                }
            }
        }
    }
    correct_region_boxes(dets, l.w*l.h*l.n, w, h, netw, neth, relative);
}

#ifdef GPU

void forward_region_layer_gpu(const layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.input_gpu, 1, l.output_gpu, 1);
    ကိန်းပြည့် b, n;
    အတွက် (b = 0; b < l.batch; ++b){
        အတွက်(n = 0; n < l.n; ++n){
            ကိန်းပြည့် index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC);
            အကယ်၍(l.coords > 4){
                index = entry_index(l, b, n*l.w*l.h, 4);
                activate_array_gpu(l.output_gpu + index, (l.coords - 4)*l.w*l.h, LOGISTIC);
            }
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            အကယ်၍(!l.background) activate_array_gpu(l.output_gpu + index,   l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords + 1);
            အကယ်၍(!l.softmax && !l.softmax_tree) activate_array_gpu(l.output_gpu + index, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    အကယ်၍ (l.softmax_tree){
        ကိန်းပြည့် index = entry_index(l, 0, 0, l.coords + 1);
        softmax_tree(net.input_gpu + index, l.w*l.h, l.batch*l.n, l.inputs/l.n, 1, l.output_gpu + index, *l.softmax_tree);
    } မဟုတ်ပါက အကယ်၍ (l.softmax) {
        ကိန်းပြည့် index = entry_index(l, 0, 0, l.coords + !l.background);
        softmax_gpu(net.input_gpu + index, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu + index);
    }
    အကယ်၍(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        ပြန်ပေး;
    }

    cuda_pull_array(l.output_gpu, net.input, l.batch*l.inputs);
    forward_region_layer(l, net);
    //cuda_push_array(l.output_gpu, l.output, l.batch*l.outputs);
    အကယ်၍(!net.train) ပြန်ပေး;
    cuda_push_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

void backward_region_layer_gpu(const layer l, network net)
{
    ကိန်းပြည့် b, n;
    အတွက် (b = 0; b < l.batch; ++b){
        အတွက်(n = 0; n < l.n; ++n){
            ကိန်းပြည့် index = entry_index(l, b, n*l.w*l.h, 0);
            gradient_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC, l.delta_gpu + index);
            အကယ်၍(l.coords > 4){
                index = entry_index(l, b, n*l.w*l.h, 4);
                gradient_array_gpu(l.output_gpu + index, (l.coords - 4)*l.w*l.h, LOGISTIC, l.delta_gpu + index);
            }
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            အကယ်၍(!l.background) gradient_array_gpu(l.output_gpu + index,   l.w*l.h, LOGISTIC, l.delta_gpu + index);
        }
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif

void zero_objectness(layer l)
{
    ကိန်းပြည့် i, n;
    အတွက် (i = 0; i < l.w*l.h; ++i){
        အတွက်(n = 0; n < l.n; ++n){
            ကိန်းပြည့် obj_index = entry_index(l, 0, n*l.w*l.h + i, l.coords);
            l.output[obj_index] = 0;
        }
    }
}


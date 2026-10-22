#ಅನ್ತರ್ಭಾವ "region_layer.h"
#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "box.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>

layer make_region_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ classes, ಪೂರ್ಣಾಙ್ಕ coords)
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
    l.cost = calloc(1, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.biases = calloc(n*2, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.bias_updates = calloc(n*2, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.outputs = h*w*n*(classes + coords + 1);
    l.inputs = l.outputs;
    l.truths = 30*(l.coords + 1);
    l.delta = calloc(batch*l.outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.output = calloc(batch*l.outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < n*2; ++i){
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

    ಪ್ರತ್ಯಾಗಮ l;
}

ಶೂನ್ಯ resize_region_layer(layer *l, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + l->coords + 1);
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*ಪರಿಮಾಣ(ಭಿನ್ನ));
    l->delta = realloc(l->delta, l->batch*l->outputs*ಪರಿಮಾಣ(ಭಿನ್ನ));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

box get_region_box(ಭಿನ್ನ *x, ಭಿನ್ನ *biases, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ index, ಪೂರ್ಣಾಙ್ಕ i, ಪೂರ್ಣಾಙ್ಕ j, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ stride)
{
    box b;
    b.x = (i + x[index + 0*stride]) / w;
    b.y = (j + x[index + 1*stride]) / h;
    b.w = exp(x[index + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[index + 3*stride]) * biases[2*n+1] / h;
    ಪ್ರತ್ಯಾಗಮ b;
}

ಭಿನ್ನ delta_region_box(box truth, ಭಿನ್ನ *x, ಭಿನ್ನ *biases, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ index, ಪೂರ್ಣಾಙ್ಕ i, ಪೂರ್ಣಾಙ್ಕ j, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಭಿನ್ನ *delta, ಭಿನ್ನ scale, ಪೂರ್ಣಾಙ್ಕ stride)
{
    box pred = get_region_box(x, biases, n, index, i, j, w, h, stride);
    ಭಿನ್ನ iou = box_iou(pred, truth);

    ಭಿನ್ನ tx = (truth.x*w - i);
    ಭಿನ್ನ ty = (truth.y*h - j);
    ಭಿನ್ನ tw = log(truth.w*w / biases[2*n]);
    ಭಿನ್ನ th = log(truth.h*h / biases[2*n + 1]);

    delta[index + 0*stride] = scale * (tx - x[index + 0*stride]);
    delta[index + 1*stride] = scale * (ty - x[index + 1*stride]);
    delta[index + 2*stride] = scale * (tw - x[index + 2*stride]);
    delta[index + 3*stride] = scale * (th - x[index + 3*stride]);
    ಪ್ರತ್ಯಾಗಮ iou;
}

ಶೂನ್ಯ delta_region_mask(ಭಿನ್ನ *truth, ಭಿನ್ನ *x, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ index, ಭಿನ್ನ *delta, ಪೂರ್ಣಾಙ್ಕ stride, ಪೂರ್ಣಾಙ್ಕ scale)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        delta[index + i*stride] = scale*(truth[i] - x[index + i*stride]);
    }
}


ಶೂನ್ಯ delta_region_class(ಭಿನ್ನ *output, ಭಿನ್ನ *delta, ಪೂರ್ಣಾಙ್ಕ index, ಪೂರ್ಣಾಙ್ಕ class, ಪೂರ್ಣಾಙ್ಕ classes, ವೃಕ್ಷ *hier, ಭಿನ್ನ scale, ಪೂರ್ಣಾಙ್ಕ stride, ಭಿನ್ನ *avg_cat, ಪೂರ್ಣಾಙ್ಕ tag)
{
    ಪೂರ್ಣಾಙ್ಕ i, n;
    ಯದಿ(hier){
        ಭಿನ್ನ pred = 1;
        ಯಾವತ್(class >= 0){
            pred *= output[index + stride*class];
            ಪೂರ್ಣಾಙ್ಕ g = hier->group[class];
            ಪೂರ್ಣಾಙ್ಕ offset = hier->group_offset[g];
            ಪ್ರತಿ(i = 0; i < hier->group_size[g]; ++i){
                delta[index + stride*(offset + i)] = scale * (0 - output[index + stride*(offset + i)]);
            }
            delta[index + stride*class] = scale * (1 - output[index + stride*class]);

            class = hier->parent[class];
        }
        *avg_cat += pred;
    } ಅನ್ಯಥಾ {
        ಯದಿ (delta[index] && tag){
            delta[index + stride*class] = scale * (1 - output[index + stride*class]);
            ಪ್ರತ್ಯಾಗಮ;
        }
        ಪ್ರತಿ(n = 0; n < classes; ++n){
            delta[index + stride*n] = scale * (((n == class)?1 : 0) - output[index + stride*n]);
            ಯದಿ(n == class) *avg_cat += output[index + stride*n];
        }
    }
}

ಭಿನ್ನ logit(ಭಿನ್ನ x)
{
    ಪ್ರತ್ಯಾಗಮ log(x/(1.-x));
}

ಭಿನ್ನ tisnan(ಭಿನ್ನ x)
{
    ಪ್ರತ್ಯಾಗಮ (x != x);
}

ಪೂರ್ಣಾಙ್ಕ entry_index(layer l, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ location, ಪೂರ್ಣಾಙ್ಕ entry)
{
    ಪೂರ್ಣಾಙ್ಕ n =   location / (l.w*l.h);
    ಪೂರ್ಣಾಙ್ಕ loc = location % (l.w*l.h);
    ಪ್ರತ್ಯಾಗಮ batch*l.outputs + n*l.w*l.h*(l.coords+l.classes+1) + entry*l.w*l.h + loc;
}

ಶೂನ್ಯ forward_region_layer(ಧ್ರುವ layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ i,j,b,t,n;
    memcpy(l.output, net.ಒಳಹರಿವು, l.outputs*l.batch*ಪರಿಮಾಣ(ಭಿನ್ನ));

#ifndef GPU
    ಪ್ರತಿ (b = 0; b < l.batch; ++b){
        ಪ್ರತಿ(n = 0; n < l.n; ++n){
            ಪೂರ್ಣಾಙ್ಕ index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array(l.output + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            ಯದಿ(!l.background) activate_array(l.output + index,   l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords + 1);
            ಯದಿ(!l.softmax && !l.softmax_ವೃಕ್ಷ) activate_array(l.output + index, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    ಯದಿ (l.softmax_ವೃಕ್ಷ){
        ಪೂರ್ಣಾಙ್ಕ i;
        ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = l.coords + 1;
        ಪ್ರತಿ (i = 0; i < l.softmax_ವೃಕ್ಷ->groups; ++i) {
            ಪೂರ್ಣಾಙ್ಕ group_size = l.softmax_ವೃಕ್ಷ->group_size[i];
            softmax_cpu(net.ಒಳಹರಿವು + ಎಣಿಕೆ, group_size, l.batch, l.inputs, l.n*l.w*l.h, 1, l.n*l.w*l.h, l.temperature, l.output + ಎಣಿಕೆ);
            ಎಣಿಕೆ += group_size;
        }
    } ಅನ್ಯಥಾ ಯದಿ (l.softmax){
        ಪೂರ್ಣಾಙ್ಕ index = entry_index(l, 0, 0, l.coords + !l.background);
        softmax_cpu(net.ಒಳಹರಿವು + index, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output + index);
    }
#endif

    memset(l.delta, 0, l.outputs * l.batch * ಪರಿಮಾಣ(ಭಿನ್ನ));
    ಯದಿ(!net.train) ಪ್ರತ್ಯಾಗಮ;
    ಭಿನ್ನ avg_iou = 0;
    ಭಿನ್ನ recall = 0;
    ಭಿನ್ನ avg_cat = 0;
    ಭಿನ್ನ avg_obj = 0;
    ಭಿನ್ನ avg_anyobj = 0;
    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
    ಪೂರ್ಣಾಙ್ಕ class_ಎಣಿಕೆ = 0;
    *(l.cost) = 0;
    ಪ್ರತಿ (b = 0; b < l.batch; ++b) {
        ಯದಿ(l.softmax_ವೃಕ್ಷ){
            ಪೂರ್ಣಾಙ್ಕ onlyclass = 0;
            ಪ್ರತಿ(t = 0; t < 30; ++t){
                box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                ಯದಿ(!truth.x) ಭಙ್ಗ;
                ಪೂರ್ಣಾಙ್ಕ class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
                ಭಿನ್ನ maxp = 0;
                ಪೂರ್ಣಾಙ್ಕ maxi = 0;
                ಯದಿ(truth.x > 100000 && truth.y > 100000){
                    ಪ್ರತಿ(n = 0; n < l.n*l.w*l.h; ++n){
                        ಪೂರ್ಣಾಙ್ಕ class_index = entry_index(l, b, n, l.coords + 1);
                        ಪೂರ್ಣಾಙ್ಕ obj_index = entry_index(l, b, n, l.coords);
                        ಭಿನ್ನ scale =  l.output[obj_index];
                        l.delta[obj_index] = l.noobject_scale * (0 - l.output[obj_index]);
                        ಭಿನ್ನ p = scale*get_hierarchy_probability(l.output + class_index, l.softmax_ವೃಕ್ಷ, class, l.w*l.h);
                        ಯದಿ(p > maxp){
                            maxp = p;
                            maxi = n;
                        }
                    }
                    ಪೂರ್ಣಾಙ್ಕ class_index = entry_index(l, b, maxi, l.coords + 1);
                    ಪೂರ್ಣಾಙ್ಕ obj_index = entry_index(l, b, maxi, l.coords);
                    delta_region_class(l.output, l.delta, class_index, class, l.classes, l.softmax_ವೃಕ್ಷ, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
                    ಯದಿ(l.output[obj_index] < .3) l.delta[obj_index] = l.object_scale * (.3 - l.output[obj_index]);
                    ಅನ್ಯಥಾ  l.delta[obj_index] = 0;
                    l.delta[obj_index] = 0;
                    ++class_ಎಣಿಕೆ;
                    onlyclass = 1;
                    ಭಙ್ಗ;
                }
            }
            ಯದಿ(onlyclass) ಅನುವರ್ತ;
        }
        ಪ್ರತಿ (j = 0; j < l.h; ++j) {
            ಪ್ರತಿ (i = 0; i < l.w; ++i) {
                ಪ್ರತಿ (n = 0; n < l.n; ++n) {
                    ಪೂರ್ಣಾಙ್ಕ box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_region_box(l.output, l.biases, n, box_index, i, j, l.w, l.h, l.w*l.h);
                    ಭಿನ್ನ best_iou = 0;
                    ಪ್ರತಿ(t = 0; t < 30; ++t){
                        box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                        ಯದಿ(!truth.x) ಭಙ್ಗ;
                        ಭಿನ್ನ iou = box_iou(pred, truth);
                        ಯದಿ (iou > best_iou) {
                            best_iou = iou;
                        }
                    }
                    ಪೂರ್ಣಾಙ್ಕ obj_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, l.coords);
                    avg_anyobj += l.output[obj_index];
                    l.delta[obj_index] = l.noobject_scale * (0 - l.output[obj_index]);
                    ಯದಿ(l.background) l.delta[obj_index] = l.noobject_scale * (1 - l.output[obj_index]);
                    ಯದಿ (best_iou > l.thresh) {
                        l.delta[obj_index] = 0;
                    }

                    ಯದಿ(*(net.seen) < 12800){
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
        ಪ್ರತಿ(t = 0; t < 30; ++t){
            box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);

            ಯದಿ(!truth.x) ಭಙ್ಗ;
            ಭಿನ್ನ best_iou = 0;
            ಪೂರ್ಣಾಙ್ಕ best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = 0;
            truth_shift.y = 0;
            ಪ್ರತಿ(n = 0; n < l.n; ++n){
                ಪೂರ್ಣಾಙ್ಕ box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                box pred = get_region_box(l.output, l.biases, n, box_index, i, j, l.w, l.h, l.w*l.h);
                ಯದಿ(l.bias_match){
                    pred.w = l.biases[2*n]/l.w;
                    pred.h = l.biases[2*n+1]/l.h;
                }
                pred.x = 0;
                pred.y = 0;
                ಭಿನ್ನ iou = box_iou(pred, truth_shift);
                ಯದಿ (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            ಪೂರ್ಣಾಙ್ಕ box_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, 0);
            ಭಿನ್ನ iou = delta_region_box(truth, l.output, l.biases, best_n, box_index, i, j, l.w, l.h, l.delta, l.coord_scale *  (2 - truth.w*truth.h), l.w*l.h);
            ಯದಿ(l.coords > 4){
                ಪೂರ್ಣಾಙ್ಕ mask_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, 4);
                delta_region_mask(net.truth + t*(l.coords + 1) + b*l.truths + 5, l.output, l.coords - 4, mask_index, l.delta, l.w*l.h, l.mask_scale);
            }
            ಯದಿ(iou > .5) recall += 1;
            avg_iou += iou;

            ಪೂರ್ಣಾಙ್ಕ obj_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, l.coords);
            avg_obj += l.output[obj_index];
            l.delta[obj_index] = l.object_scale * (1 - l.output[obj_index]);
            ಯದಿ (l.rescore) {
                l.delta[obj_index] = l.object_scale * (iou - l.output[obj_index]);
            }
            ಯದಿ(l.background){
                l.delta[obj_index] = l.object_scale * (0 - l.output[obj_index]);
            }

            ಪೂರ್ಣಾಙ್ಕ class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
            ಯದಿ (l.map) class = l.map[class];
            ಪೂರ್ಣಾಙ್ಕ class_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, l.coords + 1);
            delta_region_class(l.output, l.delta, class_index, class, l.classes, l.softmax_ವೃಕ್ಷ, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
            ++ಎಣಿಕೆ;
            ++class_ಎಣಿಕೆ;
        }
    }
    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("Region Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, Avg Recall: %f,  count: %d\n", avg_iou/ಎಣಿಕೆ, avg_cat/class_ಎಣಿಕೆ, avg_obj/ಎಣಿಕೆ, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/ಎಣಿಕೆ, ಎಣಿಕೆ);
}

ಶೂನ್ಯ backward_region_layer(ಧ್ರುವ layer l, network net)
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

ಶೂನ್ಯ correct_region_boxes(detection *dets, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ netw, ಪೂರ್ಣಾಙ್ಕ neth, ಪೂರ್ಣಾಙ್ಕ relative)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪೂರ್ಣಾಙ್ಕ new_w=0;
    ಪೂರ್ಣಾಙ್ಕ new_h=0;
    ಯದಿ (((ಭಿನ್ನ)netw/w) < ((ಭಿನ್ನ)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } ಅನ್ಯಥಾ {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    ಪ್ರತಿ (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((ಭಿನ್ನ)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((ಭಿನ್ನ)new_h/neth); 
        b.w *= (ಭಿನ್ನ)netw/new_w;
        b.h *= (ಭಿನ್ನ)neth/new_h;
        ಯದಿ(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

ಶೂನ್ಯ get_region_detections(layer l, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ netw, ಪೂರ್ಣಾಙ್ಕ neth, ಭಿನ್ನ thresh, ಪೂರ್ಣಾಙ್ಕ *map, ಭಿನ್ನ ವೃಕ್ಷ_thresh, ಪೂರ್ಣಾಙ್ಕ relative, detection *dets)
{
    ಪೂರ್ಣಾಙ್ಕ i,j,n,z;
    ಭಿನ್ನ *predictions = l.output;
    ಯದಿ (l.batch == 2) {
        ಭಿನ್ನ *flip = l.output + l.outputs;
        ಪ್ರತಿ (j = 0; j < l.h; ++j) {
            ಪ್ರತಿ (i = 0; i < l.w/2; ++i) {
                ಪ್ರತಿ (n = 0; n < l.n; ++n) {
                    ಪ್ರತಿ(z = 0; z < l.classes + l.coords + 1; ++z){
                        ಪೂರ್ಣಾಙ್ಕ i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                        ಪೂರ್ಣಾಙ್ಕ i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                        ಭಿನ್ನ swap = flip[i1];
                        flip[i1] = flip[i2];
                        flip[i2] = swap;
                        ಯದಿ(z == 0){
                            flip[i1] = -flip[i1];
                            flip[i2] = -flip[i2];
                        }
                    }
                }
            }
        }
        ಪ್ರತಿ(i = 0; i < l.outputs; ++i){
            l.output[i] = (l.output[i] + flip[i])/2.;
        }
    }
    ಪ್ರತಿ (i = 0; i < l.w*l.h; ++i){
        ಪೂರ್ಣಾಙ್ಕ row = i / l.w;
        ಪೂರ್ಣಾಙ್ಕ col = i % l.w;
        ಪ್ರತಿ(n = 0; n < l.n; ++n){
            ಪೂರ್ಣಾಙ್ಕ index = n*l.w*l.h + i;
            ಪ್ರತಿ(j = 0; j < l.classes; ++j){
                dets[index].prob[j] = 0;
            }
            ಪೂರ್ಣಾಙ್ಕ obj_index  = entry_index(l, 0, n*l.w*l.h + i, l.coords);
            ಪೂರ್ಣಾಙ್ಕ box_index  = entry_index(l, 0, n*l.w*l.h + i, 0);
            ಪೂರ್ಣಾಙ್ಕ mask_index = entry_index(l, 0, n*l.w*l.h + i, 4);
            ಭಿನ್ನ scale = l.background ? 1 : predictions[obj_index];
            dets[index].bbox = get_region_box(predictions, l.biases, n, box_index, col, row, l.w, l.h, l.w*l.h);
            dets[index].objectness = scale > thresh ? scale : 0;
            ಯದಿ(dets[index].mask){
                ಪ್ರತಿ(j = 0; j < l.coords - 4; ++j){
                    dets[index].mask[j] = l.output[mask_index + j*l.w*l.h];
                }
            }

            ಪೂರ್ಣಾಙ್ಕ class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + !l.background);
            ಯದಿ(l.softmax_ವೃಕ್ಷ){

                hierarchy_predictions(predictions + class_index, l.classes, l.softmax_ವೃಕ್ಷ, 0, l.w*l.h);
                ಯದಿ(map){
                    ಪ್ರತಿ(j = 0; j < 200; ++j){
                        ಪೂರ್ಣಾಙ್ಕ class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + 1 + map[j]);
                        ಭಿನ್ನ prob = scale*predictions[class_index];
                        dets[index].prob[j] = (prob > thresh) ? prob : 0;
                    }
                } ಅನ್ಯಥಾ {
                    ಪೂರ್ಣಾಙ್ಕ j =  hierarchy_ತುದಿ_prediction(predictions + class_index, l.softmax_ವೃಕ್ಷ, ವೃಕ್ಷ_thresh, l.w*l.h);
                    dets[index].prob[j] = (scale > thresh) ? scale : 0;
                }
            } ಅನ್ಯಥಾ {
                ಯದಿ(dets[index].objectness){
                    ಪ್ರತಿ(j = 0; j < l.classes; ++j){
                        ಪೂರ್ಣಾಙ್ಕ class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + 1 + j);
                        ಭಿನ್ನ prob = scale*predictions[class_index];
                        dets[index].prob[j] = (prob > thresh) ? prob : 0;
                    }
                }
            }
        }
    }
    correct_region_boxes(dets, l.w*l.h*l.n, w, h, netw, neth, relative);
}

#ifdef GPU

ಶೂನ್ಯ forward_region_layer_gpu(ಧ್ರುವ layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.ಒಳಹರಿವು_gpu, 1, l.output_gpu, 1);
    ಪೂರ್ಣಾಙ್ಕ b, n;
    ಪ್ರತಿ (b = 0; b < l.batch; ++b){
        ಪ್ರತಿ(n = 0; n < l.n; ++n){
            ಪೂರ್ಣಾಙ್ಕ index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC);
            ಯದಿ(l.coords > 4){
                index = entry_index(l, b, n*l.w*l.h, 4);
                activate_array_gpu(l.output_gpu + index, (l.coords - 4)*l.w*l.h, LOGISTIC);
            }
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            ಯದಿ(!l.background) activate_array_gpu(l.output_gpu + index,   l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords + 1);
            ಯದಿ(!l.softmax && !l.softmax_ವೃಕ್ಷ) activate_array_gpu(l.output_gpu + index, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    ಯದಿ (l.softmax_ವೃಕ್ಷ){
        ಪೂರ್ಣಾಙ್ಕ index = entry_index(l, 0, 0, l.coords + 1);
        softmax_ವೃಕ್ಷ(net.ಒಳಹರಿವು_gpu + index, l.w*l.h, l.batch*l.n, l.inputs/l.n, 1, l.output_gpu + index, *l.softmax_ವೃಕ್ಷ);
    } ಅನ್ಯಥಾ ಯದಿ (l.softmax) {
        ಪೂರ್ಣಾಙ್ಕ index = entry_index(l, 0, 0, l.coords + !l.background);
        softmax_gpu(net.ಒಳಹರಿವು_gpu + index, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu + index);
    }
    ಯದಿ(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        ಪ್ರತ್ಯಾಗಮ;
    }

    cuda_pull_array(l.output_gpu, net.ಒಳಹರಿವು, l.batch*l.inputs);
    forward_region_layer(l, net);
    //cuda_push_array(l.output_gpu, l.output, l.batch*l.outputs);
    ಯದಿ(!net.train) ಪ್ರತ್ಯಾಗಮ;
    cuda_ತಳ್ಳು_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

ಶೂನ್ಯ backward_region_layer_gpu(ಧ್ರುವ layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ b, n;
    ಪ್ರತಿ (b = 0; b < l.batch; ++b){
        ಪ್ರತಿ(n = 0; n < l.n; ++n){
            ಪೂರ್ಣಾಙ್ಕ index = entry_index(l, b, n*l.w*l.h, 0);
            gradient_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC, l.delta_gpu + index);
            ಯದಿ(l.coords > 4){
                index = entry_index(l, b, n*l.w*l.h, 4);
                gradient_array_gpu(l.output_gpu + index, (l.coords - 4)*l.w*l.h, LOGISTIC, l.delta_gpu + index);
            }
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            ಯದಿ(!l.background) gradient_array_gpu(l.output_gpu + index,   l.w*l.h, LOGISTIC, l.delta_gpu + index);
        }
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif

ಶೂನ್ಯ zero_objectness(layer l)
{
    ಪೂರ್ಣಾಙ್ಕ i, n;
    ಪ್ರತಿ (i = 0; i < l.w*l.h; ++i){
        ಪ್ರತಿ(n = 0; n < l.n; ++n){
            ಪೂರ್ಣಾಙ್ಕ obj_index = entry_index(l, 0, n*l.w*l.h + i, l.coords);
            l.output[obj_index] = 0;
        }
    }
}


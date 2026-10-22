#ଅନ୍ତର୍ଭାଵ "region_layer.h"
#ଅନ୍ତର୍ଭାଵ "activations.h"
#ଅନ୍ତର୍ଭାଵ "blas.h"
#ଅନ୍ତର୍ଭାଵ "box.h"
#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"

#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>

layer make_region_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା coords)
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
    l.cost = calloc(1, ଆକାର(ଦଶମିକ));
    l.biases = calloc(n*2, ଆକାର(ଦଶମିକ));
    l.bias_updates = calloc(n*2, ଆକାର(ଦଶମିକ));
    l.outputs = h*w*n*(classes + coords + 1);
    l.inputs = l.outputs;
    l.truths = 30*(l.coords + 1);
    l.delta = calloc(batch*l.outputs, ଆକାର(ଦଶମିକ));
    l.output = calloc(batch*l.outputs, ଆକାର(ଦଶମିକ));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n*2; ++i){
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

    ଫେରନ୍ତୁ l;
}

ଶୂନ୍ୟ resize_region_layer(layer *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + l->coords + 1);
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*ଆକାର(ଦଶମିକ));
    l->delta = realloc(l->delta, l->batch*l->outputs*ଆକାର(ଦଶମିକ));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

box get_region_box(ଦଶମିକ *x, ଦଶମିକ *biases, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା index, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ପୂର୍ଣ୍ଣସଂଖ୍ୟା j, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride)
{
    box b;
    b.x = (i + x[index + 0*stride]) / w;
    b.y = (j + x[index + 1*stride]) / h;
    b.w = exp(x[index + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[index + 3*stride]) * biases[2*n+1] / h;
    ଫେରନ୍ତୁ b;
}

ଦଶମିକ delta_region_box(box truth, ଦଶମିକ *x, ଦଶମିକ *biases, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା index, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ପୂର୍ଣ୍ଣସଂଖ୍ୟା j, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ଦଶମିକ *delta, ଦଶମିକ scale, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride)
{
    box pred = get_region_box(x, biases, n, index, i, j, w, h, stride);
    ଦଶମିକ iou = box_iou(pred, truth);

    ଦଶମିକ tx = (truth.x*w - i);
    ଦଶମିକ ty = (truth.y*h - j);
    ଦଶମିକ tw = log(truth.w*w / biases[2*n]);
    ଦଶମିକ th = log(truth.h*h / biases[2*n + 1]);

    delta[index + 0*stride] = scale * (tx - x[index + 0*stride]);
    delta[index + 1*stride] = scale * (ty - x[index + 1*stride]);
    delta[index + 2*stride] = scale * (tw - x[index + 2*stride]);
    delta[index + 3*stride] = scale * (th - x[index + 3*stride]);
    ଫେରନ୍ତୁ iou;
}

ଶୂନ୍ୟ delta_region_mask(ଦଶମିକ *truth, ଦଶମିକ *x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା index, ଦଶମିକ *delta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ପୂର୍ଣ୍ଣସଂଖ୍ୟା scale)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n; ++i){
        delta[index + i*stride] = scale*(truth[i] - x[index + i*stride]);
    }
}


ଶୂନ୍ୟ delta_region_class(ଦଶମିକ *output, ଦଶମିକ *delta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା index, ପୂର୍ଣ୍ଣସଂଖ୍ୟା class, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ବୃକ୍ଷ *hier, ଦଶମିକ scale, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ଦଶମିକ *avg_cat, ପୂର୍ଣ୍ଣସଂଖ୍ୟା tag)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, n;
    ଯଦି(hier){
        ଦଶମିକ pred = 1;
        ଯେତେବେଳେ(class >= 0){
            pred *= output[index + stride*class];
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା g = hier->group[class];
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = hier->group_offset[g];
            ପାଇଁ(i = 0; i < hier->group_size[g]; ++i){
                delta[index + stride*(offset + i)] = scale * (0 - output[index + stride*(offset + i)]);
            }
            delta[index + stride*class] = scale * (1 - output[index + stride*class]);

            class = hier->parent[class];
        }
        *avg_cat += pred;
    } ଅନ୍ୟଥା {
        ଯଦି (delta[index] && tag){
            delta[index + stride*class] = scale * (1 - output[index + stride*class]);
            ଫେରନ୍ତୁ;
        }
        ପାଇଁ(n = 0; n < classes; ++n){
            delta[index + stride*n] = scale * (((n == class)?1 : 0) - output[index + stride*n]);
            ଯଦି(n == class) *avg_cat += output[index + stride*n];
        }
    }
}

ଦଶମିକ logit(ଦଶମିକ x)
{
    ଫେରନ୍ତୁ log(x/(1.-x));
}

ଦଶମିକ tisnan(ଦଶମିକ x)
{
    ଫେରନ୍ତୁ (x != x);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା entry_index(layer l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା location, ପୂର୍ଣ୍ଣସଂଖ୍ୟା entry)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n =   location / (l.w*l.h);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା loc = location % (l.w*l.h);
    ଫେରନ୍ତୁ batch*l.outputs + n*l.w*l.h*(l.coords+l.classes+1) + entry*l.w*l.h + loc;
}

ଶୂନ୍ୟ forward_region_layer(ଅଚଳ layer l, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,b,t,n;
    memcpy(l.output, net.ନିବେଶ, l.outputs*l.batch*ଆକାର(ଦଶମିକ));

#ifndef GPU
    ପାଇଁ (b = 0; b < l.batch; ++b){
        ପାଇଁ(n = 0; n < l.n; ++n){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array(l.output + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            ଯଦି(!l.background) activate_array(l.output + index,   l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords + 1);
            ଯଦି(!l.softmax && !l.softmax_ବୃକ୍ଷ) activate_array(l.output + index, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    ଯଦି (l.softmax_ବୃକ୍ଷ){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = l.coords + 1;
        ପାଇଁ (i = 0; i < l.softmax_ବୃକ୍ଷ->groups; ++i) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା group_size = l.softmax_ବୃକ୍ଷ->group_size[i];
            softmax_cpu(net.ନିବେଶ + ଗଣନା_2, group_size, l.batch, l.inputs, l.n*l.w*l.h, 1, l.n*l.w*l.h, l.temperature, l.output + ଗଣନା_2);
            ଗଣନା_2 += group_size;
        }
    } ଅନ୍ୟଥା ଯଦି (l.softmax){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = entry_index(l, 0, 0, l.coords + !l.background);
        softmax_cpu(net.ନିବେଶ + index, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output + index);
    }
#endif

    memset(l.delta, 0, l.outputs * l.batch * ଆକାର(ଦଶମିକ));
    ଯଦି(!net.train) ଫେରନ୍ତୁ;
    ଦଶମିକ avg_iou = 0;
    ଦଶମିକ recall = 0;
    ଦଶମିକ avg_cat = 0;
    ଦଶମିକ avg_obj = 0;
    ଦଶମିକ avg_anyobj = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା class_ଗଣନା = 0;
    *(l.cost) = 0;
    ପାଇଁ (b = 0; b < l.batch; ++b) {
        ଯଦି(l.softmax_ବୃକ୍ଷ){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା onlyclass = 0;
            ପାଇଁ(t = 0; t < 30; ++t){
                box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                ଯଦି(!truth.x) ଭାଙ୍ଗ;
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
                ଦଶମିକ maxp = 0;
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା maxi = 0;
                ଯଦି(truth.x > 100000 && truth.y > 100000){
                    ପାଇଁ(n = 0; n < l.n*l.w*l.h; ++n){
                        ପୂର୍ଣ୍ଣସଂଖ୍ୟା class_index = entry_index(l, b, n, l.coords + 1);
                        ପୂର୍ଣ୍ଣସଂଖ୍ୟା obj_index = entry_index(l, b, n, l.coords);
                        ଦଶମିକ scale =  l.output[obj_index];
                        l.delta[obj_index] = l.noobject_scale * (0 - l.output[obj_index]);
                        ଦଶମିକ p = scale*get_hierarchy_probability(l.output + class_index, l.softmax_ବୃକ୍ଷ, class, l.w*l.h);
                        ଯଦି(p > maxp){
                            maxp = p;
                            maxi = n;
                        }
                    }
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା class_index = entry_index(l, b, maxi, l.coords + 1);
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା obj_index = entry_index(l, b, maxi, l.coords);
                    delta_region_class(l.output, l.delta, class_index, class, l.classes, l.softmax_ବୃକ୍ଷ, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
                    ଯଦି(l.output[obj_index] < .3) l.delta[obj_index] = l.object_scale * (.3 - l.output[obj_index]);
                    ଅନ୍ୟଥା  l.delta[obj_index] = 0;
                    l.delta[obj_index] = 0;
                    ++class_ଗଣନା;
                    onlyclass = 1;
                    ଭାଙ୍ଗ;
                }
            }
            ଯଦି(onlyclass) ଜାରି;
        }
        ପାଇଁ (j = 0; j < l.h; ++j) {
            ପାଇଁ (i = 0; i < l.w; ++i) {
                ପାଇଁ (n = 0; n < l.n; ++n) {
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_region_box(l.output, l.biases, n, box_index, i, j, l.w, l.h, l.w*l.h);
                    ଦଶମିକ best_iou = 0;
                    ପାଇଁ(t = 0; t < 30; ++t){
                        box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                        ଯଦି(!truth.x) ଭାଙ୍ଗ;
                        ଦଶମିକ iou = box_iou(pred, truth);
                        ଯଦି (iou > best_iou) {
                            best_iou = iou;
                        }
                    }
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା obj_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, l.coords);
                    avg_anyobj += l.output[obj_index];
                    l.delta[obj_index] = l.noobject_scale * (0 - l.output[obj_index]);
                    ଯଦି(l.background) l.delta[obj_index] = l.noobject_scale * (1 - l.output[obj_index]);
                    ଯଦି (best_iou > l.thresh) {
                        l.delta[obj_index] = 0;
                    }

                    ଯଦି(*(net.seen) < 12800){
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
        ପାଇଁ(t = 0; t < 30; ++t){
            box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);

            ଯଦି(!truth.x) ଭାଙ୍ଗ;
            ଦଶମିକ best_iou = 0;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = 0;
            truth_shift.y = 0;
            ପାଇଁ(n = 0; n < l.n; ++n){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                box pred = get_region_box(l.output, l.biases, n, box_index, i, j, l.w, l.h, l.w*l.h);
                ଯଦି(l.bias_match){
                    pred.w = l.biases[2*n]/l.w;
                    pred.h = l.biases[2*n+1]/l.h;
                }
                pred.x = 0;
                pred.y = 0;
                ଦଶମିକ iou = box_iou(pred, truth_shift);
                ଯଦି (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            ପୂର୍ଣ୍ଣସଂଖ୍ୟା box_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, 0);
            ଦଶମିକ iou = delta_region_box(truth, l.output, l.biases, best_n, box_index, i, j, l.w, l.h, l.delta, l.coord_scale *  (2 - truth.w*truth.h), l.w*l.h);
            ଯଦି(l.coords > 4){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା mask_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, 4);
                delta_region_mask(net.truth + t*(l.coords + 1) + b*l.truths + 5, l.output, l.coords - 4, mask_index, l.delta, l.w*l.h, l.mask_scale);
            }
            ଯଦି(iou > .5) recall += 1;
            avg_iou += iou;

            ପୂର୍ଣ୍ଣସଂଖ୍ୟା obj_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, l.coords);
            avg_obj += l.output[obj_index];
            l.delta[obj_index] = l.object_scale * (1 - l.output[obj_index]);
            ଯଦି (l.rescore) {
                l.delta[obj_index] = l.object_scale * (iou - l.output[obj_index]);
            }
            ଯଦି(l.background){
                l.delta[obj_index] = l.object_scale * (0 - l.output[obj_index]);
            }

            ପୂର୍ଣ୍ଣସଂଖ୍ୟା class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
            ଯଦି (l.map) class = l.map[class];
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା class_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, l.coords + 1);
            delta_region_class(l.output, l.delta, class_index, class, l.classes, l.softmax_ବୃକ୍ଷ, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
            ++ଗଣନା_2;
            ++class_ଗଣନା;
        }
    }
    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("Region Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, Avg Recall: %f,  count: %d\n", avg_iou/ଗଣନା_2, avg_cat/class_ଗଣନା, avg_obj/ଗଣନା_2, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/ଗଣନା_2, ଗଣନା_2);
}

ଶୂନ୍ୟ backward_region_layer(ଅଚଳ layer l, network net)
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

ଶୂନ୍ୟ correct_region_boxes(detection *dets, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା netw, ପୂର୍ଣ୍ଣସଂଖ୍ୟା neth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା relative)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_w=0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_h=0;
    ଯଦି (((ଦଶମିକ)netw/w) < ((ଦଶମିକ)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } ଅନ୍ୟଥା {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    ପାଇଁ (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((ଦଶମିକ)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((ଦଶମିକ)new_h/neth); 
        b.w *= (ଦଶମିକ)netw/new_w;
        b.h *= (ଦଶମିକ)neth/new_h;
        ଯଦି(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

ଶୂନ୍ୟ get_region_detections(layer l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା netw, ପୂର୍ଣ୍ଣସଂଖ୍ୟା neth, ଦଶମିକ thresh, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *map, ଦଶମିକ ବୃକ୍ଷ_thresh, ପୂର୍ଣ୍ଣସଂଖ୍ୟା relative, detection *dets)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,n,z;
    ଦଶମିକ *predictions = l.output;
    ଯଦି (l.batch == 2) {
        ଦଶମିକ *flip = l.output + l.outputs;
        ପାଇଁ (j = 0; j < l.h; ++j) {
            ପାଇଁ (i = 0; i < l.w/2; ++i) {
                ପାଇଁ (n = 0; n < l.n; ++n) {
                    ପାଇଁ(z = 0; z < l.classes + l.coords + 1; ++z){
                        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                        ଦଶମିକ swap = flip[i1];
                        flip[i1] = flip[i2];
                        flip[i2] = swap;
                        ଯଦି(z == 0){
                            flip[i1] = -flip[i1];
                            flip[i2] = -flip[i2];
                        }
                    }
                }
            }
        }
        ପାଇଁ(i = 0; i < l.outputs; ++i){
            l.output[i] = (l.output[i] + flip[i])/2.;
        }
    }
    ପାଇଁ (i = 0; i < l.w*l.h; ++i){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା row = i / l.w;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା col = i % l.w;
        ପାଇଁ(n = 0; n < l.n; ++n){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = n*l.w*l.h + i;
            ପାଇଁ(j = 0; j < l.classes; ++j){
                dets[index].prob[j] = 0;
            }
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା obj_index  = entry_index(l, 0, n*l.w*l.h + i, l.coords);
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା box_index  = entry_index(l, 0, n*l.w*l.h + i, 0);
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା mask_index = entry_index(l, 0, n*l.w*l.h + i, 4);
            ଦଶମିକ scale = l.background ? 1 : predictions[obj_index];
            dets[index].bbox = get_region_box(predictions, l.biases, n, box_index, col, row, l.w, l.h, l.w*l.h);
            dets[index].objectness = scale > thresh ? scale : 0;
            ଯଦି(dets[index].mask){
                ପାଇଁ(j = 0; j < l.coords - 4; ++j){
                    dets[index].mask[j] = l.output[mask_index + j*l.w*l.h];
                }
            }

            ପୂର୍ଣ୍ଣସଂଖ୍ୟା class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + !l.background);
            ଯଦି(l.softmax_ବୃକ୍ଷ){

                hierarchy_predictions(predictions + class_index, l.classes, l.softmax_ବୃକ୍ଷ, 0, l.w*l.h);
                ଯଦି(map){
                    ପାଇଁ(j = 0; j < 200; ++j){
                        ପୂର୍ଣ୍ଣସଂଖ୍ୟା class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + 1 + map[j]);
                        ଦଶମିକ prob = scale*predictions[class_index];
                        dets[index].prob[j] = (prob > thresh) ? prob : 0;
                    }
                } ଅନ୍ୟଥା {
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j =  hierarchy_ଶୀର୍ଷ_prediction(predictions + class_index, l.softmax_ବୃକ୍ଷ, ବୃକ୍ଷ_thresh, l.w*l.h);
                    dets[index].prob[j] = (scale > thresh) ? scale : 0;
                }
            } ଅନ୍ୟଥା {
                ଯଦି(dets[index].objectness){
                    ପାଇଁ(j = 0; j < l.classes; ++j){
                        ପୂର୍ଣ୍ଣସଂଖ୍ୟା class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + 1 + j);
                        ଦଶମିକ prob = scale*predictions[class_index];
                        dets[index].prob[j] = (prob > thresh) ? prob : 0;
                    }
                }
            }
        }
    }
    correct_region_boxes(dets, l.w*l.h*l.n, w, h, netw, neth, relative);
}

#ifdef GPU

ଶୂନ୍ୟ forward_region_layer_gpu(ଅଚଳ layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.ନିବେଶ_gpu, 1, l.output_gpu, 1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା b, n;
    ପାଇଁ (b = 0; b < l.batch; ++b){
        ପାଇଁ(n = 0; n < l.n; ++n){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC);
            ଯଦି(l.coords > 4){
                index = entry_index(l, b, n*l.w*l.h, 4);
                activate_array_gpu(l.output_gpu + index, (l.coords - 4)*l.w*l.h, LOGISTIC);
            }
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            ଯଦି(!l.background) activate_array_gpu(l.output_gpu + index,   l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords + 1);
            ଯଦି(!l.softmax && !l.softmax_ବୃକ୍ଷ) activate_array_gpu(l.output_gpu + index, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    ଯଦି (l.softmax_ବୃକ୍ଷ){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = entry_index(l, 0, 0, l.coords + 1);
        softmax_ବୃକ୍ଷ(net.ନିବେଶ_gpu + index, l.w*l.h, l.batch*l.n, l.inputs/l.n, 1, l.output_gpu + index, *l.softmax_ବୃକ୍ଷ);
    } ଅନ୍ୟଥା ଯଦି (l.softmax) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = entry_index(l, 0, 0, l.coords + !l.background);
        softmax_gpu(net.ନିବେଶ_gpu + index, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu + index);
    }
    ଯଦି(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        ଫେରନ୍ତୁ;
    }

    cuda_pull_array(l.output_gpu, net.ନିବେଶ, l.batch*l.inputs);
    forward_region_layer(l, net);
    //cuda_push_array(l.output_gpu, l.output, l.batch*l.outputs);
    ଯଦି(!net.train) ଫେରନ୍ତୁ;
    cuda_ଠେଲ_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

ଶୂନ୍ୟ backward_region_layer_gpu(ଅଚଳ layer l, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା b, n;
    ପାଇଁ (b = 0; b < l.batch; ++b){
        ପାଇଁ(n = 0; n < l.n; ++n){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = entry_index(l, b, n*l.w*l.h, 0);
            gradient_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC, l.delta_gpu + index);
            ଯଦି(l.coords > 4){
                index = entry_index(l, b, n*l.w*l.h, 4);
                gradient_array_gpu(l.output_gpu + index, (l.coords - 4)*l.w*l.h, LOGISTIC, l.delta_gpu + index);
            }
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            ଯଦି(!l.background) gradient_array_gpu(l.output_gpu + index,   l.w*l.h, LOGISTIC, l.delta_gpu + index);
        }
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif

ଶୂନ୍ୟ zero_objectness(layer l)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, n;
    ପାଇଁ (i = 0; i < l.w*l.h; ++i){
        ପାଇଁ(n = 0; n < l.n; ++n){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା obj_index = entry_index(l, 0, n*l.w*l.h + i, l.coords);
            l.output[obj_index] = 0;
        }
    }
}


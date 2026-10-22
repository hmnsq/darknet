#অন্তর্ভাব "region_layer.h"
#অন্তর্ভাব "activations.h"
#অন্তর্ভাব "blas.h"
#অন্তর্ভাব "box.h"
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব "utils.h"

#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <stdlib.h>

layer make_region_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক n, পূর্ণাঙ্ক classes, পূর্ণাঙ্ক coords)
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
    l.cost = calloc(1, পরিমাণ(ভিন্ন));
    l.biases = calloc(n*2, পরিমাণ(ভিন্ন));
    l.bias_updates = calloc(n*2, পরিমাণ(ভিন্ন));
    l.outputs = h*w*n*(classes + coords + 1);
    l.inputs = l.outputs;
    l.truths = 30*(l.coords + 1);
    l.delta = calloc(batch*l.outputs, পরিমাণ(ভিন্ন));
    l.output = calloc(batch*l.outputs, পরিমাণ(ভিন্ন));
    পূর্ণাঙ্ক i;
    প্রতি(i = 0; i < n*2; ++i){
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

    প্রত্যাগম l;
}

শূন্য resize_region_layer(layer *l, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + l->coords + 1);
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*পরিমাণ(ভিন্ন));
    l->delta = realloc(l->delta, l->batch*l->outputs*পরিমাণ(ভিন্ন));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

box get_region_box(ভিন্ন *x, ভিন্ন *biases, পূর্ণাঙ্ক n, পূর্ণাঙ্ক index, পূর্ণাঙ্ক i, পূর্ণাঙ্ক j, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক stride)
{
    box b;
    b.x = (i + x[index + 0*stride]) / w;
    b.y = (j + x[index + 1*stride]) / h;
    b.w = exp(x[index + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[index + 3*stride]) * biases[2*n+1] / h;
    প্রত্যাগম b;
}

ভিন্ন delta_region_box(box truth, ভিন্ন *x, ভিন্ন *biases, পূর্ণাঙ্ক n, পূর্ণাঙ্ক index, পূর্ণাঙ্ক i, পূর্ণাঙ্ক j, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, ভিন্ন *delta, ভিন্ন scale, পূর্ণাঙ্ক stride)
{
    box pred = get_region_box(x, biases, n, index, i, j, w, h, stride);
    ভিন্ন iou = box_iou(pred, truth);

    ভিন্ন tx = (truth.x*w - i);
    ভিন্ন ty = (truth.y*h - j);
    ভিন্ন tw = log(truth.w*w / biases[2*n]);
    ভিন্ন th = log(truth.h*h / biases[2*n + 1]);

    delta[index + 0*stride] = scale * (tx - x[index + 0*stride]);
    delta[index + 1*stride] = scale * (ty - x[index + 1*stride]);
    delta[index + 2*stride] = scale * (tw - x[index + 2*stride]);
    delta[index + 3*stride] = scale * (th - x[index + 3*stride]);
    প্রত্যাগম iou;
}

শূন্য delta_region_mask(ভিন্ন *truth, ভিন্ন *x, পূর্ণাঙ্ক n, পূর্ণাঙ্ক index, ভিন্ন *delta, পূর্ণাঙ্ক stride, পূর্ণাঙ্ক scale)
{
    পূর্ণাঙ্ক i;
    প্রতি(i = 0; i < n; ++i){
        delta[index + i*stride] = scale*(truth[i] - x[index + i*stride]);
    }
}


শূন্য delta_region_class(ভিন্ন *output, ভিন্ন *delta, পূর্ণাঙ্ক index, পূর্ণাঙ্ক class, পূর্ণাঙ্ক classes, বৃক্ষ *hier, ভিন্ন scale, পূর্ণাঙ্ক stride, ভিন্ন *avg_cat, পূর্ণাঙ্ক tag)
{
    পূর্ণাঙ্ক i, n;
    যদি(hier){
        ভিন্ন pred = 1;
        যাবত্(class >= 0){
            pred *= output[index + stride*class];
            পূর্ণাঙ্ক g = hier->group[class];
            পূর্ণাঙ্ক offset = hier->group_offset[g];
            প্রতি(i = 0; i < hier->group_size[g]; ++i){
                delta[index + stride*(offset + i)] = scale * (0 - output[index + stride*(offset + i)]);
            }
            delta[index + stride*class] = scale * (1 - output[index + stride*class]);

            class = hier->parent[class];
        }
        *avg_cat += pred;
    } অন্যথা {
        যদি (delta[index] && tag){
            delta[index + stride*class] = scale * (1 - output[index + stride*class]);
            প্রত্যাগম;
        }
        প্রতি(n = 0; n < classes; ++n){
            delta[index + stride*n] = scale * (((n == class)?1 : 0) - output[index + stride*n]);
            যদি(n == class) *avg_cat += output[index + stride*n];
        }
    }
}

ভিন্ন logit(ভিন্ন x)
{
    প্রত্যাগম log(x/(1.-x));
}

ভিন্ন tisnan(ভিন্ন x)
{
    প্রত্যাগম (x != x);
}

পূর্ণাঙ্ক entry_index(layer l, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক location, পূর্ণাঙ্ক entry)
{
    পূর্ণাঙ্ক n =   location / (l.w*l.h);
    পূর্ণাঙ্ক loc = location % (l.w*l.h);
    প্রত্যাগম batch*l.outputs + n*l.w*l.h*(l.coords+l.classes+1) + entry*l.w*l.h + loc;
}

শূন্য forward_region_layer(ধ্রুব layer l, network net)
{
    পূর্ণাঙ্ক i,j,b,t,n;
    memcpy(l.output, net.প্রবেশ, l.outputs*l.batch*পরিমাণ(ভিন্ন));

#ifndef GPU
    প্রতি (b = 0; b < l.batch; ++b){
        প্রতি(n = 0; n < l.n; ++n){
            পূর্ণাঙ্ক index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array(l.output + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            যদি(!l.background) activate_array(l.output + index,   l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords + 1);
            যদি(!l.softmax && !l.softmax_বৃক্ষ) activate_array(l.output + index, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    যদি (l.softmax_বৃক্ষ){
        পূর্ণাঙ্ক i;
        পূর্ণাঙ্ক গুনতি = l.coords + 1;
        প্রতি (i = 0; i < l.softmax_বৃক্ষ->groups; ++i) {
            পূর্ণাঙ্ক group_size = l.softmax_বৃক্ষ->group_size[i];
            softmax_cpu(net.প্রবেশ + গুনতি, group_size, l.batch, l.inputs, l.n*l.w*l.h, 1, l.n*l.w*l.h, l.temperature, l.output + গুনতি);
            গুনতি += group_size;
        }
    } অন্যথা যদি (l.softmax){
        পূর্ণাঙ্ক index = entry_index(l, 0, 0, l.coords + !l.background);
        softmax_cpu(net.প্রবেশ + index, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output + index);
    }
#endif

    memset(l.delta, 0, l.outputs * l.batch * পরিমাণ(ভিন্ন));
    যদি(!net.train) প্রত্যাগম;
    ভিন্ন avg_iou = 0;
    ভিন্ন recall = 0;
    ভিন্ন avg_cat = 0;
    ভিন্ন avg_obj = 0;
    ভিন্ন avg_anyobj = 0;
    পূর্ণাঙ্ক গুনতি = 0;
    পূর্ণাঙ্ক class_গুনতি = 0;
    *(l.cost) = 0;
    প্রতি (b = 0; b < l.batch; ++b) {
        যদি(l.softmax_বৃক্ষ){
            পূর্ণাঙ্ক onlyclass = 0;
            প্রতি(t = 0; t < 30; ++t){
                box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                যদি(!truth.x) ভঙ্গ;
                পূর্ণাঙ্ক class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
                ভিন্ন maxp = 0;
                পূর্ণাঙ্ক maxi = 0;
                যদি(truth.x > 100000 && truth.y > 100000){
                    প্রতি(n = 0; n < l.n*l.w*l.h; ++n){
                        পূর্ণাঙ্ক class_index = entry_index(l, b, n, l.coords + 1);
                        পূর্ণাঙ্ক obj_index = entry_index(l, b, n, l.coords);
                        ভিন্ন scale =  l.output[obj_index];
                        l.delta[obj_index] = l.noobject_scale * (0 - l.output[obj_index]);
                        ভিন্ন p = scale*get_hierarchy_probability(l.output + class_index, l.softmax_বৃক্ষ, class, l.w*l.h);
                        যদি(p > maxp){
                            maxp = p;
                            maxi = n;
                        }
                    }
                    পূর্ণাঙ্ক class_index = entry_index(l, b, maxi, l.coords + 1);
                    পূর্ণাঙ্ক obj_index = entry_index(l, b, maxi, l.coords);
                    delta_region_class(l.output, l.delta, class_index, class, l.classes, l.softmax_বৃক্ষ, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
                    যদি(l.output[obj_index] < .3) l.delta[obj_index] = l.object_scale * (.3 - l.output[obj_index]);
                    অন্যথা  l.delta[obj_index] = 0;
                    l.delta[obj_index] = 0;
                    ++class_গুনতি;
                    onlyclass = 1;
                    ভঙ্গ;
                }
            }
            যদি(onlyclass) অনুবর্ত;
        }
        প্রতি (j = 0; j < l.h; ++j) {
            প্রতি (i = 0; i < l.w; ++i) {
                প্রতি (n = 0; n < l.n; ++n) {
                    পূর্ণাঙ্ক box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_region_box(l.output, l.biases, n, box_index, i, j, l.w, l.h, l.w*l.h);
                    ভিন্ন best_iou = 0;
                    প্রতি(t = 0; t < 30; ++t){
                        box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                        যদি(!truth.x) ভঙ্গ;
                        ভিন্ন iou = box_iou(pred, truth);
                        যদি (iou > best_iou) {
                            best_iou = iou;
                        }
                    }
                    পূর্ণাঙ্ক obj_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, l.coords);
                    avg_anyobj += l.output[obj_index];
                    l.delta[obj_index] = l.noobject_scale * (0 - l.output[obj_index]);
                    যদি(l.background) l.delta[obj_index] = l.noobject_scale * (1 - l.output[obj_index]);
                    যদি (best_iou > l.thresh) {
                        l.delta[obj_index] = 0;
                    }

                    যদি(*(net.seen) < 12800){
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
        প্রতি(t = 0; t < 30; ++t){
            box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);

            যদি(!truth.x) ভঙ্গ;
            ভিন্ন best_iou = 0;
            পূর্ণাঙ্ক best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = 0;
            truth_shift.y = 0;
            প্রতি(n = 0; n < l.n; ++n){
                পূর্ণাঙ্ক box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                box pred = get_region_box(l.output, l.biases, n, box_index, i, j, l.w, l.h, l.w*l.h);
                যদি(l.bias_match){
                    pred.w = l.biases[2*n]/l.w;
                    pred.h = l.biases[2*n+1]/l.h;
                }
                pred.x = 0;
                pred.y = 0;
                ভিন্ন iou = box_iou(pred, truth_shift);
                যদি (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            পূর্ণাঙ্ক box_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, 0);
            ভিন্ন iou = delta_region_box(truth, l.output, l.biases, best_n, box_index, i, j, l.w, l.h, l.delta, l.coord_scale *  (2 - truth.w*truth.h), l.w*l.h);
            যদি(l.coords > 4){
                পূর্ণাঙ্ক mask_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, 4);
                delta_region_mask(net.truth + t*(l.coords + 1) + b*l.truths + 5, l.output, l.coords - 4, mask_index, l.delta, l.w*l.h, l.mask_scale);
            }
            যদি(iou > .5) recall += 1;
            avg_iou += iou;

            পূর্ণাঙ্ক obj_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, l.coords);
            avg_obj += l.output[obj_index];
            l.delta[obj_index] = l.object_scale * (1 - l.output[obj_index]);
            যদি (l.rescore) {
                l.delta[obj_index] = l.object_scale * (iou - l.output[obj_index]);
            }
            যদি(l.background){
                l.delta[obj_index] = l.object_scale * (0 - l.output[obj_index]);
            }

            পূর্ণাঙ্ক class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
            যদি (l.map) class = l.map[class];
            পূর্ণাঙ্ক class_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, l.coords + 1);
            delta_region_class(l.output, l.delta, class_index, class, l.classes, l.softmax_বৃক্ষ, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
            ++গুনতি;
            ++class_গুনতি;
        }
    }
    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("Region Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, Avg Recall: %f,  count: %d\n", avg_iou/গুনতি, avg_cat/class_গুনতি, avg_obj/গুনতি, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/গুনতি, গুনতি);
}

শূন্য backward_region_layer(ধ্রুব layer l, network net)
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

শূন্য correct_region_boxes(detection *dets, পূর্ণাঙ্ক n, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক netw, পূর্ণাঙ্ক neth, পূর্ণাঙ্ক relative)
{
    পূর্ণাঙ্ক i;
    পূর্ণাঙ্ক new_w=0;
    পূর্ণাঙ্ক new_h=0;
    যদি (((ভিন্ন)netw/w) < ((ভিন্ন)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } অন্যথা {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    প্রতি (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((ভিন্ন)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((ভিন্ন)new_h/neth); 
        b.w *= (ভিন্ন)netw/new_w;
        b.h *= (ভিন্ন)neth/new_h;
        যদি(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

শূন্য get_region_detections(layer l, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক netw, পূর্ণাঙ্ক neth, ভিন্ন thresh, পূর্ণাঙ্ক *map, ভিন্ন বৃক্ষ_thresh, পূর্ণাঙ্ক relative, detection *dets)
{
    পূর্ণাঙ্ক i,j,n,z;
    ভিন্ন *predictions = l.output;
    যদি (l.batch == 2) {
        ভিন্ন *flip = l.output + l.outputs;
        প্রতি (j = 0; j < l.h; ++j) {
            প্রতি (i = 0; i < l.w/2; ++i) {
                প্রতি (n = 0; n < l.n; ++n) {
                    প্রতি(z = 0; z < l.classes + l.coords + 1; ++z){
                        পূর্ণাঙ্ক i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                        পূর্ণাঙ্ক i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                        ভিন্ন swap = flip[i1];
                        flip[i1] = flip[i2];
                        flip[i2] = swap;
                        যদি(z == 0){
                            flip[i1] = -flip[i1];
                            flip[i2] = -flip[i2];
                        }
                    }
                }
            }
        }
        প্রতি(i = 0; i < l.outputs; ++i){
            l.output[i] = (l.output[i] + flip[i])/2.;
        }
    }
    প্রতি (i = 0; i < l.w*l.h; ++i){
        পূর্ণাঙ্ক row = i / l.w;
        পূর্ণাঙ্ক col = i % l.w;
        প্রতি(n = 0; n < l.n; ++n){
            পূর্ণাঙ্ক index = n*l.w*l.h + i;
            প্রতি(j = 0; j < l.classes; ++j){
                dets[index].prob[j] = 0;
            }
            পূর্ণাঙ্ক obj_index  = entry_index(l, 0, n*l.w*l.h + i, l.coords);
            পূর্ণাঙ্ক box_index  = entry_index(l, 0, n*l.w*l.h + i, 0);
            পূর্ণাঙ্ক mask_index = entry_index(l, 0, n*l.w*l.h + i, 4);
            ভিন্ন scale = l.background ? 1 : predictions[obj_index];
            dets[index].bbox = get_region_box(predictions, l.biases, n, box_index, col, row, l.w, l.h, l.w*l.h);
            dets[index].objectness = scale > thresh ? scale : 0;
            যদি(dets[index].mask){
                প্রতি(j = 0; j < l.coords - 4; ++j){
                    dets[index].mask[j] = l.output[mask_index + j*l.w*l.h];
                }
            }

            পূর্ণাঙ্ক class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + !l.background);
            যদি(l.softmax_বৃক্ষ){

                hierarchy_predictions(predictions + class_index, l.classes, l.softmax_বৃক্ষ, 0, l.w*l.h);
                যদি(map){
                    প্রতি(j = 0; j < 200; ++j){
                        পূর্ণাঙ্ক class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + 1 + map[j]);
                        ভিন্ন prob = scale*predictions[class_index];
                        dets[index].prob[j] = (prob > thresh) ? prob : 0;
                    }
                } অন্যথা {
                    পূর্ণাঙ্ক j =  hierarchy_শীর্ষ_prediction(predictions + class_index, l.softmax_বৃক্ষ, বৃক্ষ_thresh, l.w*l.h);
                    dets[index].prob[j] = (scale > thresh) ? scale : 0;
                }
            } অন্যথা {
                যদি(dets[index].objectness){
                    প্রতি(j = 0; j < l.classes; ++j){
                        পূর্ণাঙ্ক class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + 1 + j);
                        ভিন্ন prob = scale*predictions[class_index];
                        dets[index].prob[j] = (prob > thresh) ? prob : 0;
                    }
                }
            }
        }
    }
    correct_region_boxes(dets, l.w*l.h*l.n, w, h, netw, neth, relative);
}

#ifdef GPU

শূন্য forward_region_layer_gpu(ধ্রুব layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.প্রবেশ_gpu, 1, l.output_gpu, 1);
    পূর্ণাঙ্ক b, n;
    প্রতি (b = 0; b < l.batch; ++b){
        প্রতি(n = 0; n < l.n; ++n){
            পূর্ণাঙ্ক index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC);
            যদি(l.coords > 4){
                index = entry_index(l, b, n*l.w*l.h, 4);
                activate_array_gpu(l.output_gpu + index, (l.coords - 4)*l.w*l.h, LOGISTIC);
            }
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            যদি(!l.background) activate_array_gpu(l.output_gpu + index,   l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords + 1);
            যদি(!l.softmax && !l.softmax_বৃক্ষ) activate_array_gpu(l.output_gpu + index, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    যদি (l.softmax_বৃক্ষ){
        পূর্ণাঙ্ক index = entry_index(l, 0, 0, l.coords + 1);
        softmax_বৃক্ষ(net.প্রবেশ_gpu + index, l.w*l.h, l.batch*l.n, l.inputs/l.n, 1, l.output_gpu + index, *l.softmax_বৃক্ষ);
    } অন্যথা যদি (l.softmax) {
        পূর্ণাঙ্ক index = entry_index(l, 0, 0, l.coords + !l.background);
        softmax_gpu(net.প্রবেশ_gpu + index, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu + index);
    }
    যদি(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        প্রত্যাগম;
    }

    cuda_pull_array(l.output_gpu, net.প্রবেশ, l.batch*l.inputs);
    forward_region_layer(l, net);
    //cuda_push_array(l.output_gpu, l.output, l.batch*l.outputs);
    যদি(!net.train) প্রত্যাগম;
    cuda_ঠেলো_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

শূন্য backward_region_layer_gpu(ধ্রুব layer l, network net)
{
    পূর্ণাঙ্ক b, n;
    প্রতি (b = 0; b < l.batch; ++b){
        প্রতি(n = 0; n < l.n; ++n){
            পূর্ণাঙ্ক index = entry_index(l, b, n*l.w*l.h, 0);
            gradient_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC, l.delta_gpu + index);
            যদি(l.coords > 4){
                index = entry_index(l, b, n*l.w*l.h, 4);
                gradient_array_gpu(l.output_gpu + index, (l.coords - 4)*l.w*l.h, LOGISTIC, l.delta_gpu + index);
            }
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            যদি(!l.background) gradient_array_gpu(l.output_gpu + index,   l.w*l.h, LOGISTIC, l.delta_gpu + index);
        }
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif

শূন্য zero_objectness(layer l)
{
    পূর্ণাঙ্ক i, n;
    প্রতি (i = 0; i < l.w*l.h; ++i){
        প্রতি(n = 0; n < l.n; ++n){
            পূর্ণাঙ্ক obj_index = entry_index(l, 0, n*l.w*l.h + i, l.coords);
            l.output[obj_index] = 0;
        }
    }
}


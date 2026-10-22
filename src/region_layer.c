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

layer make_region_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা n, পূর্ণসংখ্যা classes, পূর্ণসংখ্যা coords)
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
    l.cost = calloc(1, আকার(দশমিক));
    l.biases = calloc(n*2, আকার(দশমিক));
    l.bias_updates = calloc(n*2, আকার(দশমিক));
    l.outputs = h*w*n*(classes + coords + 1);
    l.inputs = l.outputs;
    l.truths = 30*(l.coords + 1);
    l.delta = calloc(batch*l.outputs, আকার(দশমিক));
    l.output = calloc(batch*l.outputs, আকার(দশমিক));
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < n*2; ++i){
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

    ফেরত l;
}

শূন্য resize_region_layer(layer *l, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + l->coords + 1);
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*আকার(দশমিক));
    l->delta = realloc(l->delta, l->batch*l->outputs*আকার(দশমিক));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

box get_region_box(দশমিক *x, দশমিক *biases, পূর্ণসংখ্যা n, পূর্ণসংখ্যা সূচক, পূর্ণসংখ্যা i, পূর্ণসংখ্যা j, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা stride)
{
    box b;
    b.x = (i + x[সূচক + 0*stride]) / w;
    b.y = (j + x[সূচক + 1*stride]) / h;
    b.w = exp(x[সূচক + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[সূচক + 3*stride]) * biases[2*n+1] / h;
    ফেরত b;
}

দশমিক delta_region_box(box truth, দশমিক *x, দশমিক *biases, পূর্ণসংখ্যা n, পূর্ণসংখ্যা সূচক, পূর্ণসংখ্যা i, পূর্ণসংখ্যা j, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, দশমিক *delta, দশমিক scale, পূর্ণসংখ্যা stride)
{
    box pred = get_region_box(x, biases, n, সূচক, i, j, w, h, stride);
    দশমিক iou = box_iou(pred, truth);

    দশমিক tx = (truth.x*w - i);
    দশমিক ty = (truth.y*h - j);
    দশমিক tw = log(truth.w*w / biases[2*n]);
    দশমিক th = log(truth.h*h / biases[2*n + 1]);

    delta[সূচক + 0*stride] = scale * (tx - x[সূচক + 0*stride]);
    delta[সূচক + 1*stride] = scale * (ty - x[সূচক + 1*stride]);
    delta[সূচক + 2*stride] = scale * (tw - x[সূচক + 2*stride]);
    delta[সূচক + 3*stride] = scale * (th - x[সূচক + 3*stride]);
    ফেরত iou;
}

শূন্য delta_region_mask(দশমিক *truth, দশমিক *x, পূর্ণসংখ্যা n, পূর্ণসংখ্যা সূচক, দশমিক *delta, পূর্ণসংখ্যা stride, পূর্ণসংখ্যা scale)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < n; ++i){
        delta[সূচক + i*stride] = scale*(truth[i] - x[সূচক + i*stride]);
    }
}


শূন্য delta_region_class(দশমিক *output, দশমিক *delta, পূর্ণসংখ্যা সূচক, পূর্ণসংখ্যা class, পূর্ণসংখ্যা classes, বৃক্ষ *hier, দশমিক scale, পূর্ণসংখ্যা stride, দশমিক *avg_cat, পূর্ণসংখ্যা tag)
{
    পূর্ণসংখ্যা i, n;
    যদি(hier){
        দশমিক pred = 1;
        যখন(class >= 0){
            pred *= output[সূচক + stride*class];
            পূর্ণসংখ্যা g = hier->group[class];
            পূর্ণসংখ্যা offset = hier->group_offset[g];
            জন্য(i = 0; i < hier->group_আকার[g]; ++i){
                delta[সূচক + stride*(offset + i)] = scale * (0 - output[সূচক + stride*(offset + i)]);
            }
            delta[সূচক + stride*class] = scale * (1 - output[সূচক + stride*class]);

            class = hier->parent[class];
        }
        *avg_cat += pred;
    } অন্যথা {
        যদি (delta[সূচক] && tag){
            delta[সূচক + stride*class] = scale * (1 - output[সূচক + stride*class]);
            ফেরত;
        }
        জন্য(n = 0; n < classes; ++n){
            delta[সূচক + stride*n] = scale * (((n == class)?1 : 0) - output[সূচক + stride*n]);
            যদি(n == class) *avg_cat += output[সূচক + stride*n];
        }
    }
}

দশমিক logit(দশমিক x)
{
    ফেরত log(x/(1.-x));
}

দশমিক tisnan(দশমিক x)
{
    ফেরত (x != x);
}

পূর্ণসংখ্যা entry_সূচক(layer l, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা location, পূর্ণসংখ্যা entry)
{
    পূর্ণসংখ্যা n =   location / (l.w*l.h);
    পূর্ণসংখ্যা loc = location % (l.w*l.h);
    ফেরত batch*l.outputs + n*l.w*l.h*(l.coords+l.classes+1) + entry*l.w*l.h + loc;
}

শূন্য forward_region_layer(ধ্রুবক layer l, network net)
{
    পূর্ণসংখ্যা i,j,b,t,n;
    memcpy(l.output, net.প্রবেশ, l.outputs*l.batch*আকার(দশমিক));

#ifndef GPU
    জন্য (b = 0; b < l.batch; ++b){
        জন্য(n = 0; n < l.n; ++n){
            পূর্ণসংখ্যা সূচক = entry_সূচক(l, b, n*l.w*l.h, 0);
            activate_array(l.output + সূচক, 2*l.w*l.h, LOGISTIC);
            সূচক = entry_সূচক(l, b, n*l.w*l.h, l.coords);
            যদি(!l.background) activate_array(l.output + সূচক,   l.w*l.h, LOGISTIC);
            সূচক = entry_সূচক(l, b, n*l.w*l.h, l.coords + 1);
            যদি(!l.softmax && !l.softmax_বৃক্ষ) activate_array(l.output + সূচক, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    যদি (l.softmax_বৃক্ষ){
        পূর্ণসংখ্যা i;
        পূর্ণসংখ্যা গুনতি = l.coords + 1;
        জন্য (i = 0; i < l.softmax_বৃক্ষ->groups; ++i) {
            পূর্ণসংখ্যা group_আকার = l.softmax_বৃক্ষ->group_আকার[i];
            softmax_cpu(net.প্রবেশ + গুনতি, group_আকার, l.batch, l.inputs, l.n*l.w*l.h, 1, l.n*l.w*l.h, l.temperature, l.output + গুনতি);
            গুনতি += group_আকার;
        }
    } অন্যথা যদি (l.softmax){
        পূর্ণসংখ্যা সূচক = entry_সূচক(l, 0, 0, l.coords + !l.background);
        softmax_cpu(net.প্রবেশ + সূচক, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output + সূচক);
    }
#endif

    memset(l.delta, 0, l.outputs * l.batch * আকার(দশমিক));
    যদি(!net.train) ফেরত;
    দশমিক avg_iou = 0;
    দশমিক recall = 0;
    দশমিক avg_cat = 0;
    দশমিক avg_obj = 0;
    দশমিক avg_anyobj = 0;
    পূর্ণসংখ্যা গুনতি = 0;
    পূর্ণসংখ্যা class_গুনতি = 0;
    *(l.cost) = 0;
    জন্য (b = 0; b < l.batch; ++b) {
        যদি(l.softmax_বৃক্ষ){
            পূর্ণসংখ্যা onlyclass = 0;
            জন্য(t = 0; t < 30; ++t){
                box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                যদি(!truth.x) বিরতি;
                পূর্ণসংখ্যা class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
                দশমিক maxp = 0;
                পূর্ণসংখ্যা maxi = 0;
                যদি(truth.x > 100000 && truth.y > 100000){
                    জন্য(n = 0; n < l.n*l.w*l.h; ++n){
                        পূর্ণসংখ্যা class_সূচক = entry_সূচক(l, b, n, l.coords + 1);
                        পূর্ণসংখ্যা obj_সূচক = entry_সূচক(l, b, n, l.coords);
                        দশমিক scale =  l.output[obj_সূচক];
                        l.delta[obj_সূচক] = l.noobject_scale * (0 - l.output[obj_সূচক]);
                        দশমিক p = scale*get_hierarchy_probability(l.output + class_সূচক, l.softmax_বৃক্ষ, class, l.w*l.h);
                        যদি(p > maxp){
                            maxp = p;
                            maxi = n;
                        }
                    }
                    পূর্ণসংখ্যা class_সূচক = entry_সূচক(l, b, maxi, l.coords + 1);
                    পূর্ণসংখ্যা obj_সূচক = entry_সূচক(l, b, maxi, l.coords);
                    delta_region_class(l.output, l.delta, class_সূচক, class, l.classes, l.softmax_বৃক্ষ, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
                    যদি(l.output[obj_সূচক] < .3) l.delta[obj_সূচক] = l.object_scale * (.3 - l.output[obj_সূচক]);
                    অন্যথা  l.delta[obj_সূচক] = 0;
                    l.delta[obj_সূচক] = 0;
                    ++class_গুনতি;
                    onlyclass = 1;
                    বিরতি;
                }
            }
            যদি(onlyclass) চালু;
        }
        জন্য (j = 0; j < l.h; ++j) {
            জন্য (i = 0; i < l.w; ++i) {
                জন্য (n = 0; n < l.n; ++n) {
                    পূর্ণসংখ্যা box_সূচক = entry_সূচক(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_region_box(l.output, l.biases, n, box_সূচক, i, j, l.w, l.h, l.w*l.h);
                    দশমিক best_iou = 0;
                    জন্য(t = 0; t < 30; ++t){
                        box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                        যদি(!truth.x) বিরতি;
                        দশমিক iou = box_iou(pred, truth);
                        যদি (iou > best_iou) {
                            best_iou = iou;
                        }
                    }
                    পূর্ণসংখ্যা obj_সূচক = entry_সূচক(l, b, n*l.w*l.h + j*l.w + i, l.coords);
                    avg_anyobj += l.output[obj_সূচক];
                    l.delta[obj_সূচক] = l.noobject_scale * (0 - l.output[obj_সূচক]);
                    যদি(l.background) l.delta[obj_সূচক] = l.noobject_scale * (1 - l.output[obj_সূচক]);
                    যদি (best_iou > l.thresh) {
                        l.delta[obj_সূচক] = 0;
                    }

                    যদি(*(net.seen) < 12800){
                        box truth = {0};
                        truth.x = (i + .5)/l.w;
                        truth.y = (j + .5)/l.h;
                        truth.w = l.biases[2*n]/l.w;
                        truth.h = l.biases[2*n+1]/l.h;
                        delta_region_box(truth, l.output, l.biases, n, box_সূচক, i, j, l.w, l.h, l.delta, .01, l.w*l.h);
                    }
                }
            }
        }
        জন্য(t = 0; t < 30; ++t){
            box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);

            যদি(!truth.x) বিরতি;
            দশমিক best_iou = 0;
            পূর্ণসংখ্যা best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = 0;
            truth_shift.y = 0;
            জন্য(n = 0; n < l.n; ++n){
                পূর্ণসংখ্যা box_সূচক = entry_সূচক(l, b, n*l.w*l.h + j*l.w + i, 0);
                box pred = get_region_box(l.output, l.biases, n, box_সূচক, i, j, l.w, l.h, l.w*l.h);
                যদি(l.bias_match){
                    pred.w = l.biases[2*n]/l.w;
                    pred.h = l.biases[2*n+1]/l.h;
                }
                pred.x = 0;
                pred.y = 0;
                দশমিক iou = box_iou(pred, truth_shift);
                যদি (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            পূর্ণসংখ্যা box_সূচক = entry_সূচক(l, b, best_n*l.w*l.h + j*l.w + i, 0);
            দশমিক iou = delta_region_box(truth, l.output, l.biases, best_n, box_সূচক, i, j, l.w, l.h, l.delta, l.coord_scale *  (2 - truth.w*truth.h), l.w*l.h);
            যদি(l.coords > 4){
                পূর্ণসংখ্যা mask_সূচক = entry_সূচক(l, b, best_n*l.w*l.h + j*l.w + i, 4);
                delta_region_mask(net.truth + t*(l.coords + 1) + b*l.truths + 5, l.output, l.coords - 4, mask_সূচক, l.delta, l.w*l.h, l.mask_scale);
            }
            যদি(iou > .5) recall += 1;
            avg_iou += iou;

            পূর্ণসংখ্যা obj_সূচক = entry_সূচক(l, b, best_n*l.w*l.h + j*l.w + i, l.coords);
            avg_obj += l.output[obj_সূচক];
            l.delta[obj_সূচক] = l.object_scale * (1 - l.output[obj_সূচক]);
            যদি (l.rescore) {
                l.delta[obj_সূচক] = l.object_scale * (iou - l.output[obj_সূচক]);
            }
            যদি(l.background){
                l.delta[obj_সূচক] = l.object_scale * (0 - l.output[obj_সূচক]);
            }

            পূর্ণসংখ্যা class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
            যদি (l.map) class = l.map[class];
            পূর্ণসংখ্যা class_সূচক = entry_সূচক(l, b, best_n*l.w*l.h + j*l.w + i, l.coords + 1);
            delta_region_class(l.output, l.delta, class_সূচক, class, l.classes, l.softmax_বৃক্ষ, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
            ++গুনতি;
            ++class_গুনতি;
        }
    }
    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("Region Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, Avg Recall: %f,  count: %d\n", avg_iou/গুনতি, avg_cat/class_গুনতি, avg_obj/গুনতি, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/গুনতি, গুনতি);
}

শূন্য backward_region_layer(ধ্রুবক layer l, network net)
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

শূন্য correct_region_boxes(detection *dets, পূর্ণসংখ্যা n, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা netw, পূর্ণসংখ্যা neth, পূর্ণসংখ্যা relative)
{
    পূর্ণসংখ্যা i;
    পূর্ণসংখ্যা new_w=0;
    পূর্ণসংখ্যা new_h=0;
    যদি (((দশমিক)netw/w) < ((দশমিক)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } অন্যথা {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    জন্য (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((দশমিক)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((দশমিক)new_h/neth); 
        b.w *= (দশমিক)netw/new_w;
        b.h *= (দশমিক)neth/new_h;
        যদি(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

শূন্য get_region_detections(layer l, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা netw, পূর্ণসংখ্যা neth, দশমিক thresh, পূর্ণসংখ্যা *map, দশমিক বৃক্ষ_thresh, পূর্ণসংখ্যা relative, detection *dets)
{
    পূর্ণসংখ্যা i,j,n,z;
    দশমিক *predictions = l.output;
    যদি (l.batch == 2) {
        দশমিক *flip = l.output + l.outputs;
        জন্য (j = 0; j < l.h; ++j) {
            জন্য (i = 0; i < l.w/2; ++i) {
                জন্য (n = 0; n < l.n; ++n) {
                    জন্য(z = 0; z < l.classes + l.coords + 1; ++z){
                        পূর্ণসংখ্যা i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                        পূর্ণসংখ্যা i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                        দশমিক swap = flip[i1];
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
        জন্য(i = 0; i < l.outputs; ++i){
            l.output[i] = (l.output[i] + flip[i])/2.;
        }
    }
    জন্য (i = 0; i < l.w*l.h; ++i){
        পূর্ণসংখ্যা row = i / l.w;
        পূর্ণসংখ্যা col = i % l.w;
        জন্য(n = 0; n < l.n; ++n){
            পূর্ণসংখ্যা সূচক = n*l.w*l.h + i;
            জন্য(j = 0; j < l.classes; ++j){
                dets[সূচক].prob[j] = 0;
            }
            পূর্ণসংখ্যা obj_সূচক  = entry_সূচক(l, 0, n*l.w*l.h + i, l.coords);
            পূর্ণসংখ্যা box_সূচক  = entry_সূচক(l, 0, n*l.w*l.h + i, 0);
            পূর্ণসংখ্যা mask_সূচক = entry_সূচক(l, 0, n*l.w*l.h + i, 4);
            দশমিক scale = l.background ? 1 : predictions[obj_সূচক];
            dets[সূচক].bbox = get_region_box(predictions, l.biases, n, box_সূচক, col, row, l.w, l.h, l.w*l.h);
            dets[সূচক].objectness = scale > thresh ? scale : 0;
            যদি(dets[সূচক].mask){
                জন্য(j = 0; j < l.coords - 4; ++j){
                    dets[সূচক].mask[j] = l.output[mask_সূচক + j*l.w*l.h];
                }
            }

            পূর্ণসংখ্যা class_সূচক = entry_সূচক(l, 0, n*l.w*l.h + i, l.coords + !l.background);
            যদি(l.softmax_বৃক্ষ){

                hierarchy_predictions(predictions + class_সূচক, l.classes, l.softmax_বৃক্ষ, 0, l.w*l.h);
                যদি(map){
                    জন্য(j = 0; j < 200; ++j){
                        পূর্ণসংখ্যা class_সূচক = entry_সূচক(l, 0, n*l.w*l.h + i, l.coords + 1 + map[j]);
                        দশমিক prob = scale*predictions[class_সূচক];
                        dets[সূচক].prob[j] = (prob > thresh) ? prob : 0;
                    }
                } অন্যথা {
                    পূর্ণসংখ্যা j =  hierarchy_শীর্ষ_prediction(predictions + class_সূচক, l.softmax_বৃক্ষ, বৃক্ষ_thresh, l.w*l.h);
                    dets[সূচক].prob[j] = (scale > thresh) ? scale : 0;
                }
            } অন্যথা {
                যদি(dets[সূচক].objectness){
                    জন্য(j = 0; j < l.classes; ++j){
                        পূর্ণসংখ্যা class_সূচক = entry_সূচক(l, 0, n*l.w*l.h + i, l.coords + 1 + j);
                        দশমিক prob = scale*predictions[class_সূচক];
                        dets[সূচক].prob[j] = (prob > thresh) ? prob : 0;
                    }
                }
            }
        }
    }
    correct_region_boxes(dets, l.w*l.h*l.n, w, h, netw, neth, relative);
}

#ifdef GPU

শূন্য forward_region_layer_gpu(ধ্রুবক layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.প্রবেশ_gpu, 1, l.output_gpu, 1);
    পূর্ণসংখ্যা b, n;
    জন্য (b = 0; b < l.batch; ++b){
        জন্য(n = 0; n < l.n; ++n){
            পূর্ণসংখ্যা সূচক = entry_সূচক(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + সূচক, 2*l.w*l.h, LOGISTIC);
            যদি(l.coords > 4){
                সূচক = entry_সূচক(l, b, n*l.w*l.h, 4);
                activate_array_gpu(l.output_gpu + সূচক, (l.coords - 4)*l.w*l.h, LOGISTIC);
            }
            সূচক = entry_সূচক(l, b, n*l.w*l.h, l.coords);
            যদি(!l.background) activate_array_gpu(l.output_gpu + সূচক,   l.w*l.h, LOGISTIC);
            সূচক = entry_সূচক(l, b, n*l.w*l.h, l.coords + 1);
            যদি(!l.softmax && !l.softmax_বৃক্ষ) activate_array_gpu(l.output_gpu + সূচক, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    যদি (l.softmax_বৃক্ষ){
        পূর্ণসংখ্যা সূচক = entry_সূচক(l, 0, 0, l.coords + 1);
        softmax_বৃক্ষ(net.প্রবেশ_gpu + সূচক, l.w*l.h, l.batch*l.n, l.inputs/l.n, 1, l.output_gpu + সূচক, *l.softmax_বৃক্ষ);
    } অন্যথা যদি (l.softmax) {
        পূর্ণসংখ্যা সূচক = entry_সূচক(l, 0, 0, l.coords + !l.background);
        softmax_gpu(net.প্রবেশ_gpu + সূচক, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu + সূচক);
    }
    যদি(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        ফেরত;
    }

    cuda_pull_array(l.output_gpu, net.প্রবেশ, l.batch*l.inputs);
    forward_region_layer(l, net);
    //cuda_push_array(l.output_gpu, l.output, l.batch*l.outputs);
    যদি(!net.train) ফেরত;
    cuda_ঠেলো_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

শূন্য backward_region_layer_gpu(ধ্রুবক layer l, network net)
{
    পূর্ণসংখ্যা b, n;
    জন্য (b = 0; b < l.batch; ++b){
        জন্য(n = 0; n < l.n; ++n){
            পূর্ণসংখ্যা সূচক = entry_সূচক(l, b, n*l.w*l.h, 0);
            gradient_array_gpu(l.output_gpu + সূচক, 2*l.w*l.h, LOGISTIC, l.delta_gpu + সূচক);
            যদি(l.coords > 4){
                সূচক = entry_সূচক(l, b, n*l.w*l.h, 4);
                gradient_array_gpu(l.output_gpu + সূচক, (l.coords - 4)*l.w*l.h, LOGISTIC, l.delta_gpu + সূচক);
            }
            সূচক = entry_সূচক(l, b, n*l.w*l.h, l.coords);
            যদি(!l.background) gradient_array_gpu(l.output_gpu + সূচক,   l.w*l.h, LOGISTIC, l.delta_gpu + সূচক);
        }
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif

শূন্য zero_objectness(layer l)
{
    পূর্ণসংখ্যা i, n;
    জন্য (i = 0; i < l.w*l.h; ++i){
        জন্য(n = 0; n < l.n; ++n){
            পূর্ণসংখ্যা obj_সূচক = entry_সূচক(l, 0, n*l.w*l.h + i, l.coords);
            l.output[obj_সূচক] = 0;
        }
    }
}


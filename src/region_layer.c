#ضمن "region_layer.h"
#ضمن "activations.h"
#ضمن "blas.h"
#ضمن "box.h"
#ضمن "cuda.h"
#ضمن "utils.h"

#ضمن <stdio.h>
#ضمن <assert.h>
#ضمن <string.h>
#ضمن <stdlib.h>

layer make_region_layer(صحيح batch, صحيح w, صحيح h, صحيح n, صحيح classes, صحيح coords)
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
    l.cost = calloc(1, حجم(عائم));
    l.biases = calloc(n*2, حجم(عائم));
    l.bias_updates = calloc(n*2, حجم(عائم));
    l.outputs = h*w*n*(classes + coords + 1);
    l.inputs = l.outputs;
    l.truths = 30*(l.coords + 1);
    l.delta = calloc(batch*l.outputs, حجم(عائم));
    l.output = calloc(batch*l.outputs, حجم(عائم));
    صحيح i;
    لكل(i = 0; i < n*2; ++i){
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

    أرجع l;
}

فراغ resize_region_layer(layer *l, صحيح w, صحيح h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + l->coords + 1);
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*حجم(عائم));
    l->delta = realloc(l->delta, l->batch*l->outputs*حجم(عائم));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

box get_region_box(عائم *x, عائم *biases, صحيح n, صحيح index, صحيح i, صحيح j, صحيح w, صحيح h, صحيح stride)
{
    box b;
    b.x = (i + x[index + 0*stride]) / w;
    b.y = (j + x[index + 1*stride]) / h;
    b.w = exp(x[index + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[index + 3*stride]) * biases[2*n+1] / h;
    أرجع b;
}

عائم delta_region_box(box truth, عائم *x, عائم *biases, صحيح n, صحيح index, صحيح i, صحيح j, صحيح w, صحيح h, عائم *delta, عائم scale, صحيح stride)
{
    box pred = get_region_box(x, biases, n, index, i, j, w, h, stride);
    عائم iou = box_iou(pred, truth);

    عائم tx = (truth.x*w - i);
    عائم ty = (truth.y*h - j);
    عائم tw = log(truth.w*w / biases[2*n]);
    عائم th = log(truth.h*h / biases[2*n + 1]);

    delta[index + 0*stride] = scale * (tx - x[index + 0*stride]);
    delta[index + 1*stride] = scale * (ty - x[index + 1*stride]);
    delta[index + 2*stride] = scale * (tw - x[index + 2*stride]);
    delta[index + 3*stride] = scale * (th - x[index + 3*stride]);
    أرجع iou;
}

فراغ delta_region_mask(عائم *truth, عائم *x, صحيح n, صحيح index, عائم *delta, صحيح stride, صحيح scale)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        delta[index + i*stride] = scale*(truth[i] - x[index + i*stride]);
    }
}


فراغ delta_region_class(عائم *output, عائم *delta, صحيح index, صحيح class, صحيح classes, tree *hier, عائم scale, صحيح stride, عائم *avg_cat, صحيح tag)
{
    صحيح i, n;
    𐡄𐡍(hier){
        عائم pred = 1;
        طالما(class >= 0){
            pred *= output[index + stride*class];
            صحيح g = hier->group[class];
            صحيح offset = hier->group_offset[g];
            لكل(i = 0; i < hier->group_size[g]; ++i){
                delta[index + stride*(offset + i)] = scale * (0 - output[index + stride*(offset + i)]);
            }
            delta[index + stride*class] = scale * (1 - output[index + stride*class]);

            class = hier->parent[class];
        }
        *avg_cat += pred;
    } وإلا {
        𐡄𐡍 (delta[index] && tag){
            delta[index + stride*class] = scale * (1 - output[index + stride*class]);
            أرجع;
        }
        لكل(n = 0; n < classes; ++n){
            delta[index + stride*n] = scale * (((n == class)?1 : 0) - output[index + stride*n]);
            𐡄𐡍(n == class) *avg_cat += output[index + stride*n];
        }
    }
}

عائم logit(عائم x)
{
    أرجع log(x/(1.-x));
}

عائم tisnan(عائم x)
{
    أرجع (x != x);
}

صحيح entry_index(layer l, صحيح batch, صحيح location, صحيح entry)
{
    صحيح n =   location / (l.w*l.h);
    صحيح loc = location % (l.w*l.h);
    أرجع batch*l.outputs + n*l.w*l.h*(l.coords+l.classes+1) + entry*l.w*l.h + loc;
}

فراغ forward_region_layer(ثابت layer l, network net)
{
    صحيح i,j,b,t,n;
    memcpy(l.output, net.input, l.outputs*l.batch*حجم(عائم));

#ifndef GPU
    لكل (b = 0; b < l.batch; ++b){
        لكل(n = 0; n < l.n; ++n){
            صحيح index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array(l.output + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            𐡄𐡍(!l.background) activate_array(l.output + index,   l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords + 1);
            𐡄𐡍(!l.softmax && !l.softmax_tree) activate_array(l.output + index, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    𐡄𐡍 (l.softmax_tree){
        صحيح i;
        صحيح count = l.coords + 1;
        لكل (i = 0; i < l.softmax_tree->groups; ++i) {
            صحيح group_size = l.softmax_tree->group_size[i];
            softmax_cpu(net.input + count, group_size, l.batch, l.inputs, l.n*l.w*l.h, 1, l.n*l.w*l.h, l.temperature, l.output + count);
            count += group_size;
        }
    } وإلا 𐡄𐡍 (l.softmax){
        صحيح index = entry_index(l, 0, 0, l.coords + !l.background);
        softmax_cpu(net.input + index, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output + index);
    }
#endif

    memset(l.delta, 0, l.outputs * l.batch * حجم(عائم));
    𐡄𐡍(!net.train) أرجع;
    عائم avg_iou = 0;
    عائم recall = 0;
    عائم avg_cat = 0;
    عائم avg_obj = 0;
    عائم avg_anyobj = 0;
    صحيح count = 0;
    صحيح class_count = 0;
    *(l.cost) = 0;
    لكل (b = 0; b < l.batch; ++b) {
        𐡄𐡍(l.softmax_tree){
            صحيح onlyclass = 0;
            لكل(t = 0; t < 30; ++t){
                box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                𐡄𐡍(!truth.x) اكسر;
                صحيح class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
                عائم maxp = 0;
                صحيح maxi = 0;
                𐡄𐡍(truth.x > 100000 && truth.y > 100000){
                    لكل(n = 0; n < l.n*l.w*l.h; ++n){
                        صحيح class_index = entry_index(l, b, n, l.coords + 1);
                        صحيح obj_index = entry_index(l, b, n, l.coords);
                        عائم scale =  l.output[obj_index];
                        l.delta[obj_index] = l.noobject_scale * (0 - l.output[obj_index]);
                        عائم p = scale*get_hierarchy_probability(l.output + class_index, l.softmax_tree, class, l.w*l.h);
                        𐡄𐡍(p > maxp){
                            maxp = p;
                            maxi = n;
                        }
                    }
                    صحيح class_index = entry_index(l, b, maxi, l.coords + 1);
                    صحيح obj_index = entry_index(l, b, maxi, l.coords);
                    delta_region_class(l.output, l.delta, class_index, class, l.classes, l.softmax_tree, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
                    𐡄𐡍(l.output[obj_index] < .3) l.delta[obj_index] = l.object_scale * (.3 - l.output[obj_index]);
                    وإلا  l.delta[obj_index] = 0;
                    l.delta[obj_index] = 0;
                    ++class_count;
                    onlyclass = 1;
                    اكسر;
                }
            }
            𐡄𐡍(onlyclass) استمر;
        }
        لكل (j = 0; j < l.h; ++j) {
            لكل (i = 0; i < l.w; ++i) {
                لكل (n = 0; n < l.n; ++n) {
                    صحيح box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_region_box(l.output, l.biases, n, box_index, i, j, l.w, l.h, l.w*l.h);
                    عائم best_iou = 0;
                    لكل(t = 0; t < 30; ++t){
                        box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                        𐡄𐡍(!truth.x) اكسر;
                        عائم iou = box_iou(pred, truth);
                        𐡄𐡍 (iou > best_iou) {
                            best_iou = iou;
                        }
                    }
                    صحيح obj_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, l.coords);
                    avg_anyobj += l.output[obj_index];
                    l.delta[obj_index] = l.noobject_scale * (0 - l.output[obj_index]);
                    𐡄𐡍(l.background) l.delta[obj_index] = l.noobject_scale * (1 - l.output[obj_index]);
                    𐡄𐡍 (best_iou > l.thresh) {
                        l.delta[obj_index] = 0;
                    }

                    𐡄𐡍(*(net.seen) < 12800){
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
        لكل(t = 0; t < 30; ++t){
            box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);

            𐡄𐡍(!truth.x) اكسر;
            عائم best_iou = 0;
            صحيح best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = 0;
            truth_shift.y = 0;
            لكل(n = 0; n < l.n; ++n){
                صحيح box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                box pred = get_region_box(l.output, l.biases, n, box_index, i, j, l.w, l.h, l.w*l.h);
                𐡄𐡍(l.bias_match){
                    pred.w = l.biases[2*n]/l.w;
                    pred.h = l.biases[2*n+1]/l.h;
                }
                pred.x = 0;
                pred.y = 0;
                عائم iou = box_iou(pred, truth_shift);
                𐡄𐡍 (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            صحيح box_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, 0);
            عائم iou = delta_region_box(truth, l.output, l.biases, best_n, box_index, i, j, l.w, l.h, l.delta, l.coord_scale *  (2 - truth.w*truth.h), l.w*l.h);
            𐡄𐡍(l.coords > 4){
                صحيح mask_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, 4);
                delta_region_mask(net.truth + t*(l.coords + 1) + b*l.truths + 5, l.output, l.coords - 4, mask_index, l.delta, l.w*l.h, l.mask_scale);
            }
            𐡄𐡍(iou > .5) recall += 1;
            avg_iou += iou;

            صحيح obj_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, l.coords);
            avg_obj += l.output[obj_index];
            l.delta[obj_index] = l.object_scale * (1 - l.output[obj_index]);
            𐡄𐡍 (l.rescore) {
                l.delta[obj_index] = l.object_scale * (iou - l.output[obj_index]);
            }
            𐡄𐡍(l.background){
                l.delta[obj_index] = l.object_scale * (0 - l.output[obj_index]);
            }

            صحيح class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
            𐡄𐡍 (l.map) class = l.map[class];
            صحيح class_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, l.coords + 1);
            delta_region_class(l.output, l.delta, class_index, class, l.classes, l.softmax_tree, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
            ++count;
            ++class_count;
        }
    }
    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("Region Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, Avg Recall: %f,  count: %d\n", avg_iou/count, avg_cat/class_count, avg_obj/count, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/count, count);
}

فراغ backward_region_layer(ثابت layer l, network net)
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

فراغ correct_region_boxes(detection *dets, صحيح n, صحيح w, صحيح h, صحيح netw, صحيح neth, صحيح relative)
{
    صحيح i;
    صحيح new_w=0;
    صحيح new_h=0;
    𐡄𐡍 (((عائم)netw/w) < ((عائم)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } وإلا {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    لكل (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((عائم)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((عائم)new_h/neth); 
        b.w *= (عائم)netw/new_w;
        b.h *= (عائم)neth/new_h;
        𐡄𐡍(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

فراغ get_region_detections(layer l, صحيح w, صحيح h, صحيح netw, صحيح neth, عائم thresh, صحيح *map, عائم tree_thresh, صحيح relative, detection *dets)
{
    صحيح i,j,n,z;
    عائم *predictions = l.output;
    𐡄𐡍 (l.batch == 2) {
        عائم *flip = l.output + l.outputs;
        لكل (j = 0; j < l.h; ++j) {
            لكل (i = 0; i < l.w/2; ++i) {
                لكل (n = 0; n < l.n; ++n) {
                    لكل(z = 0; z < l.classes + l.coords + 1; ++z){
                        صحيح i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                        صحيح i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                        عائم swap = flip[i1];
                        flip[i1] = flip[i2];
                        flip[i2] = swap;
                        𐡄𐡍(z == 0){
                            flip[i1] = -flip[i1];
                            flip[i2] = -flip[i2];
                        }
                    }
                }
            }
        }
        لكل(i = 0; i < l.outputs; ++i){
            l.output[i] = (l.output[i] + flip[i])/2.;
        }
    }
    لكل (i = 0; i < l.w*l.h; ++i){
        صحيح row = i / l.w;
        صحيح col = i % l.w;
        لكل(n = 0; n < l.n; ++n){
            صحيح index = n*l.w*l.h + i;
            لكل(j = 0; j < l.classes; ++j){
                dets[index].prob[j] = 0;
            }
            صحيح obj_index  = entry_index(l, 0, n*l.w*l.h + i, l.coords);
            صحيح box_index  = entry_index(l, 0, n*l.w*l.h + i, 0);
            صحيح mask_index = entry_index(l, 0, n*l.w*l.h + i, 4);
            عائم scale = l.background ? 1 : predictions[obj_index];
            dets[index].bbox = get_region_box(predictions, l.biases, n, box_index, col, row, l.w, l.h, l.w*l.h);
            dets[index].objectness = scale > thresh ? scale : 0;
            𐡄𐡍(dets[index].mask){
                لكل(j = 0; j < l.coords - 4; ++j){
                    dets[index].mask[j] = l.output[mask_index + j*l.w*l.h];
                }
            }

            صحيح class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + !l.background);
            𐡄𐡍(l.softmax_tree){

                hierarchy_predictions(predictions + class_index, l.classes, l.softmax_tree, 0, l.w*l.h);
                𐡄𐡍(map){
                    لكل(j = 0; j < 200; ++j){
                        صحيح class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + 1 + map[j]);
                        عائم prob = scale*predictions[class_index];
                        dets[index].prob[j] = (prob > thresh) ? prob : 0;
                    }
                } وإلا {
                    صحيح j =  hierarchy_top_prediction(predictions + class_index, l.softmax_tree, tree_thresh, l.w*l.h);
                    dets[index].prob[j] = (scale > thresh) ? scale : 0;
                }
            } وإلا {
                𐡄𐡍(dets[index].objectness){
                    لكل(j = 0; j < l.classes; ++j){
                        صحيح class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + 1 + j);
                        عائم prob = scale*predictions[class_index];
                        dets[index].prob[j] = (prob > thresh) ? prob : 0;
                    }
                }
            }
        }
    }
    correct_region_boxes(dets, l.w*l.h*l.n, w, h, netw, neth, relative);
}

#ifdef GPU

فراغ forward_region_layer_gpu(ثابت layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.input_gpu, 1, l.output_gpu, 1);
    صحيح b, n;
    لكل (b = 0; b < l.batch; ++b){
        لكل(n = 0; n < l.n; ++n){
            صحيح index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC);
            𐡄𐡍(l.coords > 4){
                index = entry_index(l, b, n*l.w*l.h, 4);
                activate_array_gpu(l.output_gpu + index, (l.coords - 4)*l.w*l.h, LOGISTIC);
            }
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            𐡄𐡍(!l.background) activate_array_gpu(l.output_gpu + index,   l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords + 1);
            𐡄𐡍(!l.softmax && !l.softmax_tree) activate_array_gpu(l.output_gpu + index, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    𐡄𐡍 (l.softmax_tree){
        صحيح index = entry_index(l, 0, 0, l.coords + 1);
        softmax_tree(net.input_gpu + index, l.w*l.h, l.batch*l.n, l.inputs/l.n, 1, l.output_gpu + index, *l.softmax_tree);
    } وإلا 𐡄𐡍 (l.softmax) {
        صحيح index = entry_index(l, 0, 0, l.coords + !l.background);
        softmax_gpu(net.input_gpu + index, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu + index);
    }
    𐡄𐡍(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        أرجع;
    }

    cuda_pull_array(l.output_gpu, net.input, l.batch*l.inputs);
    forward_region_layer(l, net);
    //cuda_push_array(l.output_gpu, l.output, l.batch*l.outputs);
    𐡄𐡍(!net.train) أرجع;
    cuda_push_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

فراغ backward_region_layer_gpu(ثابت layer l, network net)
{
    صحيح b, n;
    لكل (b = 0; b < l.batch; ++b){
        لكل(n = 0; n < l.n; ++n){
            صحيح index = entry_index(l, b, n*l.w*l.h, 0);
            gradient_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC, l.delta_gpu + index);
            𐡄𐡍(l.coords > 4){
                index = entry_index(l, b, n*l.w*l.h, 4);
                gradient_array_gpu(l.output_gpu + index, (l.coords - 4)*l.w*l.h, LOGISTIC, l.delta_gpu + index);
            }
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            𐡄𐡍(!l.background) gradient_array_gpu(l.output_gpu + index,   l.w*l.h, LOGISTIC, l.delta_gpu + index);
        }
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif

فراغ zero_objectness(layer l)
{
    صحيح i, n;
    لكل (i = 0; i < l.w*l.h; ++i){
        لكل(n = 0; n < l.n; ++n){
            صحيح obj_index = entry_index(l, 0, n*l.w*l.h + i, l.coords);
            l.output[obj_index] = 0;
        }
    }
}


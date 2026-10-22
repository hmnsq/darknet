#شامل "region_layer.h"
#شامل "activations.h"
#شامل "blas.h"
#شامل "box.h"
#شامل "cuda.h"
#شامل "utils.h"

#شامل <stdio.h>
#شامل <assert.h>
#شامل <string.h>
#شامل <stdlib.h>

layer make_region_layer(صحیح batch, صحیح w, صحیح h, صحیح n, صحیح classes, صحیح coords)
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
    l.cost = calloc(1, اندازه(اعشاری));
    l.biases = calloc(n*2, اندازه(اعشاری));
    l.bias_updates = calloc(n*2, اندازه(اعشاری));
    l.outputs = h*w*n*(classes + coords + 1);
    l.inputs = l.outputs;
    l.truths = 30*(l.coords + 1);
    l.delta = calloc(batch*l.outputs, اندازه(اعشاری));
    l.output = calloc(batch*l.outputs, اندازه(اعشاری));
    صحیح i;
    برای(i = 0; i < n*2; ++i){
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

    بازگشت l;
}

تهی resize_region_layer(layer *l, صحیح w, صحیح h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + l->coords + 1);
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*اندازه(اعشاری));
    l->delta = realloc(l->delta, l->batch*l->outputs*اندازه(اعشاری));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

box get_region_box(اعشاری *x, اعشاری *biases, صحیح n, صحیح index, صحیح i, صحیح j, صحیح w, صحیح h, صحیح stride)
{
    box b;
    b.x = (i + x[index + 0*stride]) / w;
    b.y = (j + x[index + 1*stride]) / h;
    b.w = exp(x[index + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[index + 3*stride]) * biases[2*n+1] / h;
    بازگشت b;
}

اعشاری delta_region_box(box truth, اعشاری *x, اعشاری *biases, صحیح n, صحیح index, صحیح i, صحیح j, صحیح w, صحیح h, اعشاری *delta, اعشاری scale, صحیح stride)
{
    box pred = get_region_box(x, biases, n, index, i, j, w, h, stride);
    اعشاری iou = box_iou(pred, truth);

    اعشاری tx = (truth.x*w - i);
    اعشاری ty = (truth.y*h - j);
    اعشاری tw = log(truth.w*w / biases[2*n]);
    اعشاری th = log(truth.h*h / biases[2*n + 1]);

    delta[index + 0*stride] = scale * (tx - x[index + 0*stride]);
    delta[index + 1*stride] = scale * (ty - x[index + 1*stride]);
    delta[index + 2*stride] = scale * (tw - x[index + 2*stride]);
    delta[index + 3*stride] = scale * (th - x[index + 3*stride]);
    بازگشت iou;
}

تهی delta_region_mask(اعشاری *truth, اعشاری *x, صحیح n, صحیح index, اعشاری *delta, صحیح stride, صحیح scale)
{
    صحیح i;
    برای(i = 0; i < n; ++i){
        delta[index + i*stride] = scale*(truth[i] - x[index + i*stride]);
    }
}


تهی delta_region_class(اعشاری *output, اعشاری *delta, صحیح index, صحیح class, صحیح classes, درخت *hier, اعشاری scale, صحیح stride, اعشاری *avg_cat, صحیح tag)
{
    صحیح i, n;
    اگر(hier){
        اعشاری pred = 1;
        وقتی(class >= 0){
            pred *= output[index + stride*class];
            صحیح g = hier->group[class];
            صحیح offset = hier->group_offset[g];
            برای(i = 0; i < hier->group_size[g]; ++i){
                delta[index + stride*(offset + i)] = scale * (0 - output[index + stride*(offset + i)]);
            }
            delta[index + stride*class] = scale * (1 - output[index + stride*class]);

            class = hier->parent[class];
        }
        *avg_cat += pred;
    } وگرنه {
        اگر (delta[index] && tag){
            delta[index + stride*class] = scale * (1 - output[index + stride*class]);
            بازگشت;
        }
        برای(n = 0; n < classes; ++n){
            delta[index + stride*n] = scale * (((n == class)?1 : 0) - output[index + stride*n]);
            اگر(n == class) *avg_cat += output[index + stride*n];
        }
    }
}

اعشاری logit(اعشاری x)
{
    بازگشت log(x/(1.-x));
}

اعشاری tisnan(اعشاری x)
{
    بازگشت (x != x);
}

صحیح entry_index(layer l, صحیح batch, صحیح location, صحیح entry)
{
    صحیح n =   location / (l.w*l.h);
    صحیح loc = location % (l.w*l.h);
    بازگشت batch*l.outputs + n*l.w*l.h*(l.coords+l.classes+1) + entry*l.w*l.h + loc;
}

تهی forward_region_layer(ثابت layer l, network net)
{
    صحیح i,j,b,t,n;
    memcpy(l.output, net.درون_داد, l.outputs*l.batch*اندازه(اعشاری));

#ifndef GPU
    برای (b = 0; b < l.batch; ++b){
        برای(n = 0; n < l.n; ++n){
            صحیح index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array(l.output + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            اگر(!l.background) activate_array(l.output + index,   l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords + 1);
            اگر(!l.softmax && !l.softmax_درخت) activate_array(l.output + index, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    اگر (l.softmax_درخت){
        صحیح i;
        صحیح شمار = l.coords + 1;
        برای (i = 0; i < l.softmax_درخت->groups; ++i) {
            صحیح group_size = l.softmax_درخت->group_size[i];
            softmax_cpu(net.درون_داد + شمار, group_size, l.batch, l.inputs, l.n*l.w*l.h, 1, l.n*l.w*l.h, l.temperature, l.output + شمار);
            شمار += group_size;
        }
    } وگرنه اگر (l.softmax){
        صحیح index = entry_index(l, 0, 0, l.coords + !l.background);
        softmax_cpu(net.درون_داد + index, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output + index);
    }
#endif

    memset(l.delta, 0, l.outputs * l.batch * اندازه(اعشاری));
    اگر(!net.train) بازگشت;
    اعشاری avg_iou = 0;
    اعشاری recall = 0;
    اعشاری avg_cat = 0;
    اعشاری avg_obj = 0;
    اعشاری avg_anyobj = 0;
    صحیح شمار = 0;
    صحیح class_شمار = 0;
    *(l.cost) = 0;
    برای (b = 0; b < l.batch; ++b) {
        اگر(l.softmax_درخت){
            صحیح onlyclass = 0;
            برای(t = 0; t < 30; ++t){
                box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                اگر(!truth.x) بشکن;
                صحیح class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
                اعشاری maxp = 0;
                صحیح maxi = 0;
                اگر(truth.x > 100000 && truth.y > 100000){
                    برای(n = 0; n < l.n*l.w*l.h; ++n){
                        صحیح class_index = entry_index(l, b, n, l.coords + 1);
                        صحیح obj_index = entry_index(l, b, n, l.coords);
                        اعشاری scale =  l.output[obj_index];
                        l.delta[obj_index] = l.noobject_scale * (0 - l.output[obj_index]);
                        اعشاری p = scale*get_hierarchy_probability(l.output + class_index, l.softmax_درخت, class, l.w*l.h);
                        اگر(p > maxp){
                            maxp = p;
                            maxi = n;
                        }
                    }
                    صحیح class_index = entry_index(l, b, maxi, l.coords + 1);
                    صحیح obj_index = entry_index(l, b, maxi, l.coords);
                    delta_region_class(l.output, l.delta, class_index, class, l.classes, l.softmax_درخت, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
                    اگر(l.output[obj_index] < .3) l.delta[obj_index] = l.object_scale * (.3 - l.output[obj_index]);
                    وگرنه  l.delta[obj_index] = 0;
                    l.delta[obj_index] = 0;
                    ++class_شمار;
                    onlyclass = 1;
                    بشکن;
                }
            }
            اگر(onlyclass) ادامه;
        }
        برای (j = 0; j < l.h; ++j) {
            برای (i = 0; i < l.w; ++i) {
                برای (n = 0; n < l.n; ++n) {
                    صحیح box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_region_box(l.output, l.biases, n, box_index, i, j, l.w, l.h, l.w*l.h);
                    اعشاری best_iou = 0;
                    برای(t = 0; t < 30; ++t){
                        box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                        اگر(!truth.x) بشکن;
                        اعشاری iou = box_iou(pred, truth);
                        اگر (iou > best_iou) {
                            best_iou = iou;
                        }
                    }
                    صحیح obj_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, l.coords);
                    avg_anyobj += l.output[obj_index];
                    l.delta[obj_index] = l.noobject_scale * (0 - l.output[obj_index]);
                    اگر(l.background) l.delta[obj_index] = l.noobject_scale * (1 - l.output[obj_index]);
                    اگر (best_iou > l.thresh) {
                        l.delta[obj_index] = 0;
                    }

                    اگر(*(net.seen) < 12800){
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
        برای(t = 0; t < 30; ++t){
            box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);

            اگر(!truth.x) بشکن;
            اعشاری best_iou = 0;
            صحیح best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = 0;
            truth_shift.y = 0;
            برای(n = 0; n < l.n; ++n){
                صحیح box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                box pred = get_region_box(l.output, l.biases, n, box_index, i, j, l.w, l.h, l.w*l.h);
                اگر(l.bias_match){
                    pred.w = l.biases[2*n]/l.w;
                    pred.h = l.biases[2*n+1]/l.h;
                }
                pred.x = 0;
                pred.y = 0;
                اعشاری iou = box_iou(pred, truth_shift);
                اگر (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            صحیح box_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, 0);
            اعشاری iou = delta_region_box(truth, l.output, l.biases, best_n, box_index, i, j, l.w, l.h, l.delta, l.coord_scale *  (2 - truth.w*truth.h), l.w*l.h);
            اگر(l.coords > 4){
                صحیح mask_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, 4);
                delta_region_mask(net.truth + t*(l.coords + 1) + b*l.truths + 5, l.output, l.coords - 4, mask_index, l.delta, l.w*l.h, l.mask_scale);
            }
            اگر(iou > .5) recall += 1;
            avg_iou += iou;

            صحیح obj_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, l.coords);
            avg_obj += l.output[obj_index];
            l.delta[obj_index] = l.object_scale * (1 - l.output[obj_index]);
            اگر (l.rescore) {
                l.delta[obj_index] = l.object_scale * (iou - l.output[obj_index]);
            }
            اگر(l.background){
                l.delta[obj_index] = l.object_scale * (0 - l.output[obj_index]);
            }

            صحیح class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
            اگر (l.map) class = l.map[class];
            صحیح class_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, l.coords + 1);
            delta_region_class(l.output, l.delta, class_index, class, l.classes, l.softmax_درخت, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
            ++شمار;
            ++class_شمار;
        }
    }
    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("Region Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, Avg Recall: %f,  count: %d\n", avg_iou/شمار, avg_cat/class_شمار, avg_obj/شمار, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/شمار, شمار);
}

تهی backward_region_layer(ثابت layer l, network net)
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

تهی correct_region_boxes(detection *dets, صحیح n, صحیح w, صحیح h, صحیح netw, صحیح neth, صحیح relative)
{
    صحیح i;
    صحیح new_w=0;
    صحیح new_h=0;
    اگر (((اعشاری)netw/w) < ((اعشاری)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } وگرنه {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    برای (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((اعشاری)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((اعشاری)new_h/neth); 
        b.w *= (اعشاری)netw/new_w;
        b.h *= (اعشاری)neth/new_h;
        اگر(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

تهی get_region_detections(layer l, صحیح w, صحیح h, صحیح netw, صحیح neth, اعشاری thresh, صحیح *map, اعشاری درخت_thresh, صحیح relative, detection *dets)
{
    صحیح i,j,n,z;
    اعشاری *predictions = l.output;
    اگر (l.batch == 2) {
        اعشاری *flip = l.output + l.outputs;
        برای (j = 0; j < l.h; ++j) {
            برای (i = 0; i < l.w/2; ++i) {
                برای (n = 0; n < l.n; ++n) {
                    برای(z = 0; z < l.classes + l.coords + 1; ++z){
                        صحیح i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                        صحیح i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                        اعشاری swap = flip[i1];
                        flip[i1] = flip[i2];
                        flip[i2] = swap;
                        اگر(z == 0){
                            flip[i1] = -flip[i1];
                            flip[i2] = -flip[i2];
                        }
                    }
                }
            }
        }
        برای(i = 0; i < l.outputs; ++i){
            l.output[i] = (l.output[i] + flip[i])/2.;
        }
    }
    برای (i = 0; i < l.w*l.h; ++i){
        صحیح row = i / l.w;
        صحیح col = i % l.w;
        برای(n = 0; n < l.n; ++n){
            صحیح index = n*l.w*l.h + i;
            برای(j = 0; j < l.classes; ++j){
                dets[index].prob[j] = 0;
            }
            صحیح obj_index  = entry_index(l, 0, n*l.w*l.h + i, l.coords);
            صحیح box_index  = entry_index(l, 0, n*l.w*l.h + i, 0);
            صحیح mask_index = entry_index(l, 0, n*l.w*l.h + i, 4);
            اعشاری scale = l.background ? 1 : predictions[obj_index];
            dets[index].bbox = get_region_box(predictions, l.biases, n, box_index, col, row, l.w, l.h, l.w*l.h);
            dets[index].objectness = scale > thresh ? scale : 0;
            اگر(dets[index].mask){
                برای(j = 0; j < l.coords - 4; ++j){
                    dets[index].mask[j] = l.output[mask_index + j*l.w*l.h];
                }
            }

            صحیح class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + !l.background);
            اگر(l.softmax_درخت){

                hierarchy_predictions(predictions + class_index, l.classes, l.softmax_درخت, 0, l.w*l.h);
                اگر(map){
                    برای(j = 0; j < 200; ++j){
                        صحیح class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + 1 + map[j]);
                        اعشاری prob = scale*predictions[class_index];
                        dets[index].prob[j] = (prob > thresh) ? prob : 0;
                    }
                } وگرنه {
                    صحیح j =  hierarchy_بالاترین_prediction(predictions + class_index, l.softmax_درخت, درخت_thresh, l.w*l.h);
                    dets[index].prob[j] = (scale > thresh) ? scale : 0;
                }
            } وگرنه {
                اگر(dets[index].objectness){
                    برای(j = 0; j < l.classes; ++j){
                        صحیح class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + 1 + j);
                        اعشاری prob = scale*predictions[class_index];
                        dets[index].prob[j] = (prob > thresh) ? prob : 0;
                    }
                }
            }
        }
    }
    correct_region_boxes(dets, l.w*l.h*l.n, w, h, netw, neth, relative);
}

#ifdef GPU

تهی forward_region_layer_gpu(ثابت layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.درون_داد_gpu, 1, l.output_gpu, 1);
    صحیح b, n;
    برای (b = 0; b < l.batch; ++b){
        برای(n = 0; n < l.n; ++n){
            صحیح index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC);
            اگر(l.coords > 4){
                index = entry_index(l, b, n*l.w*l.h, 4);
                activate_array_gpu(l.output_gpu + index, (l.coords - 4)*l.w*l.h, LOGISTIC);
            }
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            اگر(!l.background) activate_array_gpu(l.output_gpu + index,   l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords + 1);
            اگر(!l.softmax && !l.softmax_درخت) activate_array_gpu(l.output_gpu + index, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    اگر (l.softmax_درخت){
        صحیح index = entry_index(l, 0, 0, l.coords + 1);
        softmax_درخت(net.درون_داد_gpu + index, l.w*l.h, l.batch*l.n, l.inputs/l.n, 1, l.output_gpu + index, *l.softmax_درخت);
    } وگرنه اگر (l.softmax) {
        صحیح index = entry_index(l, 0, 0, l.coords + !l.background);
        softmax_gpu(net.درون_داد_gpu + index, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu + index);
    }
    اگر(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        بازگشت;
    }

    cuda_pull_array(l.output_gpu, net.درون_داد, l.batch*l.inputs);
    forward_region_layer(l, net);
    //cuda_push_array(l.output_gpu, l.output, l.batch*l.outputs);
    اگر(!net.train) بازگشت;
    cuda_افزودن_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

تهی backward_region_layer_gpu(ثابت layer l, network net)
{
    صحیح b, n;
    برای (b = 0; b < l.batch; ++b){
        برای(n = 0; n < l.n; ++n){
            صحیح index = entry_index(l, b, n*l.w*l.h, 0);
            gradient_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC, l.delta_gpu + index);
            اگر(l.coords > 4){
                index = entry_index(l, b, n*l.w*l.h, 4);
                gradient_array_gpu(l.output_gpu + index, (l.coords - 4)*l.w*l.h, LOGISTIC, l.delta_gpu + index);
            }
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            اگر(!l.background) gradient_array_gpu(l.output_gpu + index,   l.w*l.h, LOGISTIC, l.delta_gpu + index);
        }
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif

تهی zero_objectness(layer l)
{
    صحیح i, n;
    برای (i = 0; i < l.w*l.h; ++i){
        برای(n = 0; n < l.n; ++n){
            صحیح obj_index = entry_index(l, 0, n*l.w*l.h + i, l.coords);
            l.output[obj_index] = 0;
        }
    }
}


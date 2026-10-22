#அந்தர்பாவ "region_layer.h"
#அந்தர்பாவ "activations.h"
#அந்தர்பாவ "blas.h"
#அந்தர்பாவ "box.h"
#அந்தர்பாவ "cuda.h"
#அந்தர்பாவ "utils.h"

#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <stdlib.h>

layer make_region_layer(முழு batch, முழு w, முழு h, முழு n, முழு classes, முழு coords)
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
    l.cost = calloc(1, அளவு(தசம));
    l.biases = calloc(n*2, அளவு(தசம));
    l.bias_updates = calloc(n*2, அளவு(தசம));
    l.outputs = h*w*n*(classes + coords + 1);
    l.inputs = l.outputs;
    l.truths = 30*(l.coords + 1);
    l.delta = calloc(batch*l.outputs, அளவு(தசம));
    l.output = calloc(batch*l.outputs, அளவு(தசம));
    முழு i;
    க்கு(i = 0; i < n*2; ++i){
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

    திரும்பு l;
}

வெறுமை resize_region_layer(layer *l, முழு w, முழு h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + l->coords + 1);
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*அளவு(தசம));
    l->delta = realloc(l->delta, l->batch*l->outputs*அளவு(தசம));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

box get_region_box(தசம *x, தசம *biases, முழு n, முழு index, முழு i, முழு j, முழு w, முழு h, முழு stride)
{
    box b;
    b.x = (i + x[index + 0*stride]) / w;
    b.y = (j + x[index + 1*stride]) / h;
    b.w = exp(x[index + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[index + 3*stride]) * biases[2*n+1] / h;
    திரும்பு b;
}

தசம delta_region_box(box truth, தசம *x, தசம *biases, முழு n, முழு index, முழு i, முழு j, முழு w, முழு h, தசம *delta, தசம scale, முழு stride)
{
    box pred = get_region_box(x, biases, n, index, i, j, w, h, stride);
    தசம iou = box_iou(pred, truth);

    தசம tx = (truth.x*w - i);
    தசம ty = (truth.y*h - j);
    தசம tw = log(truth.w*w / biases[2*n]);
    தசம th = log(truth.h*h / biases[2*n + 1]);

    delta[index + 0*stride] = scale * (tx - x[index + 0*stride]);
    delta[index + 1*stride] = scale * (ty - x[index + 1*stride]);
    delta[index + 2*stride] = scale * (tw - x[index + 2*stride]);
    delta[index + 3*stride] = scale * (th - x[index + 3*stride]);
    திரும்பு iou;
}

வெறுமை delta_region_mask(தசம *truth, தசம *x, முழு n, முழு index, தசம *delta, முழு stride, முழு scale)
{
    முழு i;
    க்கு(i = 0; i < n; ++i){
        delta[index + i*stride] = scale*(truth[i] - x[index + i*stride]);
    }
}


வெறுமை delta_region_class(தசம *output, தசம *delta, முழு index, முழு class, முழு classes, மரம் *hier, தசம scale, முழு stride, தசம *avg_cat, முழு tag)
{
    முழு i, n;
    எனில்(hier){
        தசம pred = 1;
        போது(class >= 0){
            pred *= output[index + stride*class];
            முழு g = hier->group[class];
            முழு offset = hier->group_offset[g];
            க்கு(i = 0; i < hier->group_size[g]; ++i){
                delta[index + stride*(offset + i)] = scale * (0 - output[index + stride*(offset + i)]);
            }
            delta[index + stride*class] = scale * (1 - output[index + stride*class]);

            class = hier->parent[class];
        }
        *avg_cat += pred;
    } இல்லையெனில் {
        எனில் (delta[index] && tag){
            delta[index + stride*class] = scale * (1 - output[index + stride*class]);
            திரும்பு;
        }
        க்கு(n = 0; n < classes; ++n){
            delta[index + stride*n] = scale * (((n == class)?1 : 0) - output[index + stride*n]);
            எனில்(n == class) *avg_cat += output[index + stride*n];
        }
    }
}

தசம logit(தசம x)
{
    திரும்பு log(x/(1.-x));
}

தசம tisnan(தசம x)
{
    திரும்பு (x != x);
}

முழு entry_index(layer l, முழு batch, முழு location, முழு entry)
{
    முழு n =   location / (l.w*l.h);
    முழு loc = location % (l.w*l.h);
    திரும்பு batch*l.outputs + n*l.w*l.h*(l.coords+l.classes+1) + entry*l.w*l.h + loc;
}

வெறுமை forward_region_layer(மாறிலி layer l, network net)
{
    முழு i,j,b,t,n;
    memcpy(l.output, net.உள்ளீடு, l.outputs*l.batch*அளவு(தசம));

#ifndef GPU
    க்கு (b = 0; b < l.batch; ++b){
        க்கு(n = 0; n < l.n; ++n){
            முழு index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array(l.output + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            எனில்(!l.background) activate_array(l.output + index,   l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords + 1);
            எனில்(!l.softmax && !l.softmax_மரம்) activate_array(l.output + index, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    எனில் (l.softmax_மரம்){
        முழு i;
        முழு மொத்தம் = l.coords + 1;
        க்கு (i = 0; i < l.softmax_மரம்->groups; ++i) {
            முழு group_size = l.softmax_மரம்->group_size[i];
            softmax_cpu(net.உள்ளீடு + மொத்தம், group_size, l.batch, l.inputs, l.n*l.w*l.h, 1, l.n*l.w*l.h, l.temperature, l.output + மொத்தம்);
            மொத்தம் += group_size;
        }
    } இல்லையெனில் எனில் (l.softmax){
        முழு index = entry_index(l, 0, 0, l.coords + !l.background);
        softmax_cpu(net.உள்ளீடு + index, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output + index);
    }
#endif

    memset(l.delta, 0, l.outputs * l.batch * அளவு(தசம));
    எனில்(!net.train) திரும்பு;
    தசம avg_iou = 0;
    தசம recall = 0;
    தசம avg_cat = 0;
    தசம avg_obj = 0;
    தசம avg_anyobj = 0;
    முழு மொத்தம் = 0;
    முழு class_மொத்தம் = 0;
    *(l.cost) = 0;
    க்கு (b = 0; b < l.batch; ++b) {
        எனில்(l.softmax_மரம்){
            முழு onlyclass = 0;
            க்கு(t = 0; t < 30; ++t){
                box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                எனில்(!truth.x) நிறுத்து;
                முழு class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
                தசம maxp = 0;
                முழு maxi = 0;
                எனில்(truth.x > 100000 && truth.y > 100000){
                    க்கு(n = 0; n < l.n*l.w*l.h; ++n){
                        முழு class_index = entry_index(l, b, n, l.coords + 1);
                        முழு obj_index = entry_index(l, b, n, l.coords);
                        தசம scale =  l.output[obj_index];
                        l.delta[obj_index] = l.noobject_scale * (0 - l.output[obj_index]);
                        தசம p = scale*get_hierarchy_probability(l.output + class_index, l.softmax_மரம், class, l.w*l.h);
                        எனில்(p > maxp){
                            maxp = p;
                            maxi = n;
                        }
                    }
                    முழு class_index = entry_index(l, b, maxi, l.coords + 1);
                    முழு obj_index = entry_index(l, b, maxi, l.coords);
                    delta_region_class(l.output, l.delta, class_index, class, l.classes, l.softmax_மரம், l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
                    எனில்(l.output[obj_index] < .3) l.delta[obj_index] = l.object_scale * (.3 - l.output[obj_index]);
                    இல்லையெனில்  l.delta[obj_index] = 0;
                    l.delta[obj_index] = 0;
                    ++class_மொத்தம்;
                    onlyclass = 1;
                    நிறுத்து;
                }
            }
            எனில்(onlyclass) தொடர்;
        }
        க்கு (j = 0; j < l.h; ++j) {
            க்கு (i = 0; i < l.w; ++i) {
                க்கு (n = 0; n < l.n; ++n) {
                    முழு box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_region_box(l.output, l.biases, n, box_index, i, j, l.w, l.h, l.w*l.h);
                    தசம best_iou = 0;
                    க்கு(t = 0; t < 30; ++t){
                        box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                        எனில்(!truth.x) நிறுத்து;
                        தசம iou = box_iou(pred, truth);
                        எனில் (iou > best_iou) {
                            best_iou = iou;
                        }
                    }
                    முழு obj_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, l.coords);
                    avg_anyobj += l.output[obj_index];
                    l.delta[obj_index] = l.noobject_scale * (0 - l.output[obj_index]);
                    எனில்(l.background) l.delta[obj_index] = l.noobject_scale * (1 - l.output[obj_index]);
                    எனில் (best_iou > l.thresh) {
                        l.delta[obj_index] = 0;
                    }

                    எனில்(*(net.seen) < 12800){
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
        க்கு(t = 0; t < 30; ++t){
            box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);

            எனில்(!truth.x) நிறுத்து;
            தசம best_iou = 0;
            முழு best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = 0;
            truth_shift.y = 0;
            க்கு(n = 0; n < l.n; ++n){
                முழு box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                box pred = get_region_box(l.output, l.biases, n, box_index, i, j, l.w, l.h, l.w*l.h);
                எனில்(l.bias_match){
                    pred.w = l.biases[2*n]/l.w;
                    pred.h = l.biases[2*n+1]/l.h;
                }
                pred.x = 0;
                pred.y = 0;
                தசம iou = box_iou(pred, truth_shift);
                எனில் (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            முழு box_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, 0);
            தசம iou = delta_region_box(truth, l.output, l.biases, best_n, box_index, i, j, l.w, l.h, l.delta, l.coord_scale *  (2 - truth.w*truth.h), l.w*l.h);
            எனில்(l.coords > 4){
                முழு mask_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, 4);
                delta_region_mask(net.truth + t*(l.coords + 1) + b*l.truths + 5, l.output, l.coords - 4, mask_index, l.delta, l.w*l.h, l.mask_scale);
            }
            எனில்(iou > .5) recall += 1;
            avg_iou += iou;

            முழு obj_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, l.coords);
            avg_obj += l.output[obj_index];
            l.delta[obj_index] = l.object_scale * (1 - l.output[obj_index]);
            எனில் (l.rescore) {
                l.delta[obj_index] = l.object_scale * (iou - l.output[obj_index]);
            }
            எனில்(l.background){
                l.delta[obj_index] = l.object_scale * (0 - l.output[obj_index]);
            }

            முழு class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
            எனில் (l.map) class = l.map[class];
            முழு class_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, l.coords + 1);
            delta_region_class(l.output, l.delta, class_index, class, l.classes, l.softmax_மரம், l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
            ++மொத்தம்;
            ++class_மொத்தம்;
        }
    }
    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("Region Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, Avg Recall: %f,  count: %d\n", avg_iou/மொத்தம், avg_cat/class_மொத்தம், avg_obj/மொத்தம், avg_anyobj/(l.w*l.h*l.n*l.batch), recall/மொத்தம், மொத்தம்);
}

வெறுமை backward_region_layer(மாறிலி layer l, network net)
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

வெறுமை correct_region_boxes(detection *dets, முழு n, முழு w, முழு h, முழு netw, முழு neth, முழு relative)
{
    முழு i;
    முழு new_w=0;
    முழு new_h=0;
    எனில் (((தசம)netw/w) < ((தசம)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } இல்லையெனில் {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    க்கு (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((தசம)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((தசம)new_h/neth); 
        b.w *= (தசம)netw/new_w;
        b.h *= (தசம)neth/new_h;
        எனில்(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

வெறுமை get_region_detections(layer l, முழு w, முழு h, முழு netw, முழு neth, தசம thresh, முழு *map, தசம மரம்_thresh, முழு relative, detection *dets)
{
    முழு i,j,n,z;
    தசம *predictions = l.output;
    எனில் (l.batch == 2) {
        தசம *flip = l.output + l.outputs;
        க்கு (j = 0; j < l.h; ++j) {
            க்கு (i = 0; i < l.w/2; ++i) {
                க்கு (n = 0; n < l.n; ++n) {
                    க்கு(z = 0; z < l.classes + l.coords + 1; ++z){
                        முழு i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                        முழு i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                        தசம swap = flip[i1];
                        flip[i1] = flip[i2];
                        flip[i2] = swap;
                        எனில்(z == 0){
                            flip[i1] = -flip[i1];
                            flip[i2] = -flip[i2];
                        }
                    }
                }
            }
        }
        க்கு(i = 0; i < l.outputs; ++i){
            l.output[i] = (l.output[i] + flip[i])/2.;
        }
    }
    க்கு (i = 0; i < l.w*l.h; ++i){
        முழு row = i / l.w;
        முழு col = i % l.w;
        க்கு(n = 0; n < l.n; ++n){
            முழு index = n*l.w*l.h + i;
            க்கு(j = 0; j < l.classes; ++j){
                dets[index].prob[j] = 0;
            }
            முழு obj_index  = entry_index(l, 0, n*l.w*l.h + i, l.coords);
            முழு box_index  = entry_index(l, 0, n*l.w*l.h + i, 0);
            முழு mask_index = entry_index(l, 0, n*l.w*l.h + i, 4);
            தசம scale = l.background ? 1 : predictions[obj_index];
            dets[index].bbox = get_region_box(predictions, l.biases, n, box_index, col, row, l.w, l.h, l.w*l.h);
            dets[index].objectness = scale > thresh ? scale : 0;
            எனில்(dets[index].mask){
                க்கு(j = 0; j < l.coords - 4; ++j){
                    dets[index].mask[j] = l.output[mask_index + j*l.w*l.h];
                }
            }

            முழு class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + !l.background);
            எனில்(l.softmax_மரம்){

                hierarchy_predictions(predictions + class_index, l.classes, l.softmax_மரம், 0, l.w*l.h);
                எனில்(map){
                    க்கு(j = 0; j < 200; ++j){
                        முழு class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + 1 + map[j]);
                        தசம prob = scale*predictions[class_index];
                        dets[index].prob[j] = (prob > thresh) ? prob : 0;
                    }
                } இல்லையெனில் {
                    முழு j =  hierarchy_உச்சி_prediction(predictions + class_index, l.softmax_மரம், மரம்_thresh, l.w*l.h);
                    dets[index].prob[j] = (scale > thresh) ? scale : 0;
                }
            } இல்லையெனில் {
                எனில்(dets[index].objectness){
                    க்கு(j = 0; j < l.classes; ++j){
                        முழு class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + 1 + j);
                        தசம prob = scale*predictions[class_index];
                        dets[index].prob[j] = (prob > thresh) ? prob : 0;
                    }
                }
            }
        }
    }
    correct_region_boxes(dets, l.w*l.h*l.n, w, h, netw, neth, relative);
}

#ifdef GPU

வெறுமை forward_region_layer_gpu(மாறிலி layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.உள்ளீடு_gpu, 1, l.output_gpu, 1);
    முழு b, n;
    க்கு (b = 0; b < l.batch; ++b){
        க்கு(n = 0; n < l.n; ++n){
            முழு index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC);
            எனில்(l.coords > 4){
                index = entry_index(l, b, n*l.w*l.h, 4);
                activate_array_gpu(l.output_gpu + index, (l.coords - 4)*l.w*l.h, LOGISTIC);
            }
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            எனில்(!l.background) activate_array_gpu(l.output_gpu + index,   l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords + 1);
            எனில்(!l.softmax && !l.softmax_மரம்) activate_array_gpu(l.output_gpu + index, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    எனில் (l.softmax_மரம்){
        முழு index = entry_index(l, 0, 0, l.coords + 1);
        softmax_மரம்(net.உள்ளீடு_gpu + index, l.w*l.h, l.batch*l.n, l.inputs/l.n, 1, l.output_gpu + index, *l.softmax_மரம்);
    } இல்லையெனில் எனில் (l.softmax) {
        முழு index = entry_index(l, 0, 0, l.coords + !l.background);
        softmax_gpu(net.உள்ளீடு_gpu + index, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu + index);
    }
    எனில்(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        திரும்பு;
    }

    cuda_pull_array(l.output_gpu, net.உள்ளீடு, l.batch*l.inputs);
    forward_region_layer(l, net);
    //cuda_push_array(l.output_gpu, l.output, l.batch*l.outputs);
    எனில்(!net.train) திரும்பு;
    cuda_தள்ளு_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

வெறுமை backward_region_layer_gpu(மாறிலி layer l, network net)
{
    முழு b, n;
    க்கு (b = 0; b < l.batch; ++b){
        க்கு(n = 0; n < l.n; ++n){
            முழு index = entry_index(l, b, n*l.w*l.h, 0);
            gradient_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC, l.delta_gpu + index);
            எனில்(l.coords > 4){
                index = entry_index(l, b, n*l.w*l.h, 4);
                gradient_array_gpu(l.output_gpu + index, (l.coords - 4)*l.w*l.h, LOGISTIC, l.delta_gpu + index);
            }
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            எனில்(!l.background) gradient_array_gpu(l.output_gpu + index,   l.w*l.h, LOGISTIC, l.delta_gpu + index);
        }
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif

வெறுமை zero_objectness(layer l)
{
    முழு i, n;
    க்கு (i = 0; i < l.w*l.h; ++i){
        க்கு(n = 0; n < l.n; ++n){
            முழு obj_index = entry_index(l, 0, n*l.w*l.h + i, l.coords);
            l.output[obj_index] = 0;
        }
    }
}


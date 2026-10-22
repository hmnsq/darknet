#അന്തര്ഭാവ "region_layer.h"
#അന്തര്ഭാവ "activations.h"
#അന്തര്ഭാവ "blas.h"
#അന്തര്ഭാവ "box.h"
#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ "utils.h"

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <stdlib.h>

layer make_region_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ classes, പൂർണ്ണസംഖ്യ coords)
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
    l.cost = calloc(1, വലുപ്പം(ദശാംശം));
    l.biases = calloc(n*2, വലുപ്പം(ദശാംശം));
    l.bias_updates = calloc(n*2, വലുപ്പം(ദശാംശം));
    l.outputs = h*w*n*(classes + coords + 1);
    l.inputs = l.outputs;
    l.truths = 30*(l.coords + 1);
    l.delta = calloc(batch*l.outputs, വലുപ്പം(ദശാംശം));
    l.output = calloc(batch*l.outputs, വലുപ്പം(ദശാംശം));
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < n*2; ++i){
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

    തിരികെ l;
}

ശൂന്യം resize_region_layer(layer *l, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + l->coords + 1);
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*വലുപ്പം(ദശാംശം));
    l->delta = realloc(l->delta, l->batch*l->outputs*വലുപ്പം(ദശാംശം));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

box get_region_box(ദശാംശം *x, ദശാംശം *biases, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ index, പൂർണ്ണസംഖ്യ i, പൂർണ്ണസംഖ്യ j, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ stride)
{
    box b;
    b.x = (i + x[index + 0*stride]) / w;
    b.y = (j + x[index + 1*stride]) / h;
    b.w = exp(x[index + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[index + 3*stride]) * biases[2*n+1] / h;
    തിരികെ b;
}

ദശാംശം delta_region_box(box truth, ദശാംശം *x, ദശാംശം *biases, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ index, പൂർണ്ണസംഖ്യ i, പൂർണ്ണസംഖ്യ j, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, ദശാംശം *delta, ദശാംശം scale, പൂർണ്ണസംഖ്യ stride)
{
    box pred = get_region_box(x, biases, n, index, i, j, w, h, stride);
    ദശാംശം iou = box_iou(pred, truth);

    ദശാംശം tx = (truth.x*w - i);
    ദശാംശം ty = (truth.y*h - j);
    ദശാംശം tw = log(truth.w*w / biases[2*n]);
    ദശാംശം th = log(truth.h*h / biases[2*n + 1]);

    delta[index + 0*stride] = scale * (tx - x[index + 0*stride]);
    delta[index + 1*stride] = scale * (ty - x[index + 1*stride]);
    delta[index + 2*stride] = scale * (tw - x[index + 2*stride]);
    delta[index + 3*stride] = scale * (th - x[index + 3*stride]);
    തിരികെ iou;
}

ശൂന്യം delta_region_mask(ദശാംശം *truth, ദശാംശം *x, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ index, ദശാംശം *delta, പൂർണ്ണസംഖ്യ stride, പൂർണ്ണസംഖ്യ scale)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < n; ++i){
        delta[index + i*stride] = scale*(truth[i] - x[index + i*stride]);
    }
}


ശൂന്യം delta_region_class(ദശാംശം *output, ദശാംശം *delta, പൂർണ്ണസംഖ്യ index, പൂർണ്ണസംഖ്യ class, പൂർണ്ണസംഖ്യ classes, വൃക്ഷം *hier, ദശാംശം scale, പൂർണ്ണസംഖ്യ stride, ദശാംശം *avg_cat, പൂർണ്ണസംഖ്യ tag)
{
    പൂർണ്ണസംഖ്യ i, n;
    എങ്കിൽ(hier){
        ദശാംശം pred = 1;
        ആകുമ്പോൾ(class >= 0){
            pred *= output[index + stride*class];
            പൂർണ്ണസംഖ്യ g = hier->group[class];
            പൂർണ്ണസംഖ്യ offset = hier->group_offset[g];
            വേണ്ടി(i = 0; i < hier->group_size[g]; ++i){
                delta[index + stride*(offset + i)] = scale * (0 - output[index + stride*(offset + i)]);
            }
            delta[index + stride*class] = scale * (1 - output[index + stride*class]);

            class = hier->parent[class];
        }
        *avg_cat += pred;
    } അല്ലെങ്കിൽ {
        എങ്കിൽ (delta[index] && tag){
            delta[index + stride*class] = scale * (1 - output[index + stride*class]);
            തിരികെ;
        }
        വേണ്ടി(n = 0; n < classes; ++n){
            delta[index + stride*n] = scale * (((n == class)?1 : 0) - output[index + stride*n]);
            എങ്കിൽ(n == class) *avg_cat += output[index + stride*n];
        }
    }
}

ദശാംശം logit(ദശാംശം x)
{
    തിരികെ log(x/(1.-x));
}

ദശാംശം tisnan(ദശാംശം x)
{
    തിരികെ (x != x);
}

പൂർണ്ണസംഖ്യ entry_index(layer l, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ location, പൂർണ്ണസംഖ്യ entry)
{
    പൂർണ്ണസംഖ്യ n =   location / (l.w*l.h);
    പൂർണ്ണസംഖ്യ loc = location % (l.w*l.h);
    തിരികെ batch*l.outputs + n*l.w*l.h*(l.coords+l.classes+1) + entry*l.w*l.h + loc;
}

ശൂന്യം forward_region_layer(സ്ഥിരം layer l, network net)
{
    പൂർണ്ണസംഖ്യ i,j,b,t,n;
    memcpy(l.output, net.നിവേശം, l.outputs*l.batch*വലുപ്പം(ദശാംശം));

#ifndef GPU
    വേണ്ടി (b = 0; b < l.batch; ++b){
        വേണ്ടി(n = 0; n < l.n; ++n){
            പൂർണ്ണസംഖ്യ index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array(l.output + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            എങ്കിൽ(!l.background) activate_array(l.output + index,   l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords + 1);
            എങ്കിൽ(!l.softmax && !l.softmax_വൃക്ഷം) activate_array(l.output + index, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    എങ്കിൽ (l.softmax_വൃക്ഷം){
        പൂർണ്ണസംഖ്യ i;
        പൂർണ്ണസംഖ്യ എണ്ണം = l.coords + 1;
        വേണ്ടി (i = 0; i < l.softmax_വൃക്ഷം->groups; ++i) {
            പൂർണ്ണസംഖ്യ group_size = l.softmax_വൃക്ഷം->group_size[i];
            softmax_cpu(net.നിവേശം + എണ്ണം, group_size, l.batch, l.inputs, l.n*l.w*l.h, 1, l.n*l.w*l.h, l.temperature, l.output + എണ്ണം);
            എണ്ണം += group_size;
        }
    } അല്ലെങ്കിൽ എങ്കിൽ (l.softmax){
        പൂർണ്ണസംഖ്യ index = entry_index(l, 0, 0, l.coords + !l.background);
        softmax_cpu(net.നിവേശം + index, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output + index);
    }
#endif

    memset(l.delta, 0, l.outputs * l.batch * വലുപ്പം(ദശാംശം));
    എങ്കിൽ(!net.train) തിരികെ;
    ദശാംശം avg_iou = 0;
    ദശാംശം recall = 0;
    ദശാംശം avg_cat = 0;
    ദശാംശം avg_obj = 0;
    ദശാംശം avg_anyobj = 0;
    പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    പൂർണ്ണസംഖ്യ class_എണ്ണം = 0;
    *(l.cost) = 0;
    വേണ്ടി (b = 0; b < l.batch; ++b) {
        എങ്കിൽ(l.softmax_വൃക്ഷം){
            പൂർണ്ണസംഖ്യ onlyclass = 0;
            വേണ്ടി(t = 0; t < 30; ++t){
                box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                എങ്കിൽ(!truth.x) നിർത്തുക;
                പൂർണ്ണസംഖ്യ class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
                ദശാംശം maxp = 0;
                പൂർണ്ണസംഖ്യ maxi = 0;
                എങ്കിൽ(truth.x > 100000 && truth.y > 100000){
                    വേണ്ടി(n = 0; n < l.n*l.w*l.h; ++n){
                        പൂർണ്ണസംഖ്യ class_index = entry_index(l, b, n, l.coords + 1);
                        പൂർണ്ണസംഖ്യ obj_index = entry_index(l, b, n, l.coords);
                        ദശാംശം scale =  l.output[obj_index];
                        l.delta[obj_index] = l.noobject_scale * (0 - l.output[obj_index]);
                        ദശാംശം p = scale*get_hierarchy_probability(l.output + class_index, l.softmax_വൃക്ഷം, class, l.w*l.h);
                        എങ്കിൽ(p > maxp){
                            maxp = p;
                            maxi = n;
                        }
                    }
                    പൂർണ്ണസംഖ്യ class_index = entry_index(l, b, maxi, l.coords + 1);
                    പൂർണ്ണസംഖ്യ obj_index = entry_index(l, b, maxi, l.coords);
                    delta_region_class(l.output, l.delta, class_index, class, l.classes, l.softmax_വൃക്ഷം, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
                    എങ്കിൽ(l.output[obj_index] < .3) l.delta[obj_index] = l.object_scale * (.3 - l.output[obj_index]);
                    അല്ലെങ്കിൽ  l.delta[obj_index] = 0;
                    l.delta[obj_index] = 0;
                    ++class_എണ്ണം;
                    onlyclass = 1;
                    നിർത്തുക;
                }
            }
            എങ്കിൽ(onlyclass) തുടരുക;
        }
        വേണ്ടി (j = 0; j < l.h; ++j) {
            വേണ്ടി (i = 0; i < l.w; ++i) {
                വേണ്ടി (n = 0; n < l.n; ++n) {
                    പൂർണ്ണസംഖ്യ box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_region_box(l.output, l.biases, n, box_index, i, j, l.w, l.h, l.w*l.h);
                    ദശാംശം best_iou = 0;
                    വേണ്ടി(t = 0; t < 30; ++t){
                        box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                        എങ്കിൽ(!truth.x) നിർത്തുക;
                        ദശാംശം iou = box_iou(pred, truth);
                        എങ്കിൽ (iou > best_iou) {
                            best_iou = iou;
                        }
                    }
                    പൂർണ്ണസംഖ്യ obj_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, l.coords);
                    avg_anyobj += l.output[obj_index];
                    l.delta[obj_index] = l.noobject_scale * (0 - l.output[obj_index]);
                    എങ്കിൽ(l.background) l.delta[obj_index] = l.noobject_scale * (1 - l.output[obj_index]);
                    എങ്കിൽ (best_iou > l.thresh) {
                        l.delta[obj_index] = 0;
                    }

                    എങ്കിൽ(*(net.seen) < 12800){
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
        വേണ്ടി(t = 0; t < 30; ++t){
            box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);

            എങ്കിൽ(!truth.x) നിർത്തുക;
            ദശാംശം best_iou = 0;
            പൂർണ്ണസംഖ്യ best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = 0;
            truth_shift.y = 0;
            വേണ്ടി(n = 0; n < l.n; ++n){
                പൂർണ്ണസംഖ്യ box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                box pred = get_region_box(l.output, l.biases, n, box_index, i, j, l.w, l.h, l.w*l.h);
                എങ്കിൽ(l.bias_match){
                    pred.w = l.biases[2*n]/l.w;
                    pred.h = l.biases[2*n+1]/l.h;
                }
                pred.x = 0;
                pred.y = 0;
                ദശാംശം iou = box_iou(pred, truth_shift);
                എങ്കിൽ (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            പൂർണ്ണസംഖ്യ box_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, 0);
            ദശാംശം iou = delta_region_box(truth, l.output, l.biases, best_n, box_index, i, j, l.w, l.h, l.delta, l.coord_scale *  (2 - truth.w*truth.h), l.w*l.h);
            എങ്കിൽ(l.coords > 4){
                പൂർണ്ണസംഖ്യ mask_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, 4);
                delta_region_mask(net.truth + t*(l.coords + 1) + b*l.truths + 5, l.output, l.coords - 4, mask_index, l.delta, l.w*l.h, l.mask_scale);
            }
            എങ്കിൽ(iou > .5) recall += 1;
            avg_iou += iou;

            പൂർണ്ണസംഖ്യ obj_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, l.coords);
            avg_obj += l.output[obj_index];
            l.delta[obj_index] = l.object_scale * (1 - l.output[obj_index]);
            എങ്കിൽ (l.rescore) {
                l.delta[obj_index] = l.object_scale * (iou - l.output[obj_index]);
            }
            എങ്കിൽ(l.background){
                l.delta[obj_index] = l.object_scale * (0 - l.output[obj_index]);
            }

            പൂർണ്ണസംഖ്യ class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
            എങ്കിൽ (l.map) class = l.map[class];
            പൂർണ്ണസംഖ്യ class_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, l.coords + 1);
            delta_region_class(l.output, l.delta, class_index, class, l.classes, l.softmax_വൃക്ഷം, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
            ++എണ്ണം;
            ++class_എണ്ണം;
        }
    }
    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("Region Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, Avg Recall: %f,  count: %d\n", avg_iou/എണ്ണം, avg_cat/class_എണ്ണം, avg_obj/എണ്ണം, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/എണ്ണം, എണ്ണം);
}

ശൂന്യം backward_region_layer(സ്ഥിരം layer l, network net)
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

ശൂന്യം correct_region_boxes(detection *dets, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ netw, പൂർണ്ണസംഖ്യ neth, പൂർണ്ണസംഖ്യ relative)
{
    പൂർണ്ണസംഖ്യ i;
    പൂർണ്ണസംഖ്യ new_w=0;
    പൂർണ്ണസംഖ്യ new_h=0;
    എങ്കിൽ (((ദശാംശം)netw/w) < ((ദശാംശം)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } അല്ലെങ്കിൽ {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    വേണ്ടി (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((ദശാംശം)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((ദശാംശം)new_h/neth); 
        b.w *= (ദശാംശം)netw/new_w;
        b.h *= (ദശാംശം)neth/new_h;
        എങ്കിൽ(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

ശൂന്യം get_region_detections(layer l, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ netw, പൂർണ്ണസംഖ്യ neth, ദശാംശം thresh, പൂർണ്ണസംഖ്യ *map, ദശാംശം വൃക്ഷം_thresh, പൂർണ്ണസംഖ്യ relative, detection *dets)
{
    പൂർണ്ണസംഖ്യ i,j,n,z;
    ദശാംശം *predictions = l.output;
    എങ്കിൽ (l.batch == 2) {
        ദശാംശം *flip = l.output + l.outputs;
        വേണ്ടി (j = 0; j < l.h; ++j) {
            വേണ്ടി (i = 0; i < l.w/2; ++i) {
                വേണ്ടി (n = 0; n < l.n; ++n) {
                    വേണ്ടി(z = 0; z < l.classes + l.coords + 1; ++z){
                        പൂർണ്ണസംഖ്യ i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                        പൂർണ്ണസംഖ്യ i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                        ദശാംശം swap = flip[i1];
                        flip[i1] = flip[i2];
                        flip[i2] = swap;
                        എങ്കിൽ(z == 0){
                            flip[i1] = -flip[i1];
                            flip[i2] = -flip[i2];
                        }
                    }
                }
            }
        }
        വേണ്ടി(i = 0; i < l.outputs; ++i){
            l.output[i] = (l.output[i] + flip[i])/2.;
        }
    }
    വേണ്ടി (i = 0; i < l.w*l.h; ++i){
        പൂർണ്ണസംഖ്യ row = i / l.w;
        പൂർണ്ണസംഖ്യ col = i % l.w;
        വേണ്ടി(n = 0; n < l.n; ++n){
            പൂർണ്ണസംഖ്യ index = n*l.w*l.h + i;
            വേണ്ടി(j = 0; j < l.classes; ++j){
                dets[index].prob[j] = 0;
            }
            പൂർണ്ണസംഖ്യ obj_index  = entry_index(l, 0, n*l.w*l.h + i, l.coords);
            പൂർണ്ണസംഖ്യ box_index  = entry_index(l, 0, n*l.w*l.h + i, 0);
            പൂർണ്ണസംഖ്യ mask_index = entry_index(l, 0, n*l.w*l.h + i, 4);
            ദശാംശം scale = l.background ? 1 : predictions[obj_index];
            dets[index].bbox = get_region_box(predictions, l.biases, n, box_index, col, row, l.w, l.h, l.w*l.h);
            dets[index].objectness = scale > thresh ? scale : 0;
            എങ്കിൽ(dets[index].mask){
                വേണ്ടി(j = 0; j < l.coords - 4; ++j){
                    dets[index].mask[j] = l.output[mask_index + j*l.w*l.h];
                }
            }

            പൂർണ്ണസംഖ്യ class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + !l.background);
            എങ്കിൽ(l.softmax_വൃക്ഷം){

                hierarchy_predictions(predictions + class_index, l.classes, l.softmax_വൃക്ഷം, 0, l.w*l.h);
                എങ്കിൽ(map){
                    വേണ്ടി(j = 0; j < 200; ++j){
                        പൂർണ്ണസംഖ്യ class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + 1 + map[j]);
                        ദശാംശം prob = scale*predictions[class_index];
                        dets[index].prob[j] = (prob > thresh) ? prob : 0;
                    }
                } അല്ലെങ്കിൽ {
                    പൂർണ്ണസംഖ്യ j =  hierarchy_മുകൾ_prediction(predictions + class_index, l.softmax_വൃക്ഷം, വൃക്ഷം_thresh, l.w*l.h);
                    dets[index].prob[j] = (scale > thresh) ? scale : 0;
                }
            } അല്ലെങ്കിൽ {
                എങ്കിൽ(dets[index].objectness){
                    വേണ്ടി(j = 0; j < l.classes; ++j){
                        പൂർണ്ണസംഖ്യ class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + 1 + j);
                        ദശാംശം prob = scale*predictions[class_index];
                        dets[index].prob[j] = (prob > thresh) ? prob : 0;
                    }
                }
            }
        }
    }
    correct_region_boxes(dets, l.w*l.h*l.n, w, h, netw, neth, relative);
}

#ifdef GPU

ശൂന്യം forward_region_layer_gpu(സ്ഥിരം layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.നിവേശം_gpu, 1, l.output_gpu, 1);
    പൂർണ്ണസംഖ്യ b, n;
    വേണ്ടി (b = 0; b < l.batch; ++b){
        വേണ്ടി(n = 0; n < l.n; ++n){
            പൂർണ്ണസംഖ്യ index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC);
            എങ്കിൽ(l.coords > 4){
                index = entry_index(l, b, n*l.w*l.h, 4);
                activate_array_gpu(l.output_gpu + index, (l.coords - 4)*l.w*l.h, LOGISTIC);
            }
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            എങ്കിൽ(!l.background) activate_array_gpu(l.output_gpu + index,   l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords + 1);
            എങ്കിൽ(!l.softmax && !l.softmax_വൃക്ഷം) activate_array_gpu(l.output_gpu + index, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    എങ്കിൽ (l.softmax_വൃക്ഷം){
        പൂർണ്ണസംഖ്യ index = entry_index(l, 0, 0, l.coords + 1);
        softmax_വൃക്ഷം(net.നിവേശം_gpu + index, l.w*l.h, l.batch*l.n, l.inputs/l.n, 1, l.output_gpu + index, *l.softmax_വൃക്ഷം);
    } അല്ലെങ്കിൽ എങ്കിൽ (l.softmax) {
        പൂർണ്ണസംഖ്യ index = entry_index(l, 0, 0, l.coords + !l.background);
        softmax_gpu(net.നിവേശം_gpu + index, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu + index);
    }
    എങ്കിൽ(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        തിരികെ;
    }

    cuda_pull_array(l.output_gpu, net.നിവേശം, l.batch*l.inputs);
    forward_region_layer(l, net);
    //cuda_push_array(l.output_gpu, l.output, l.batch*l.outputs);
    എങ്കിൽ(!net.train) തിരികെ;
    cuda_തള്ളുക_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

ശൂന്യം backward_region_layer_gpu(സ്ഥിരം layer l, network net)
{
    പൂർണ്ണസംഖ്യ b, n;
    വേണ്ടി (b = 0; b < l.batch; ++b){
        വേണ്ടി(n = 0; n < l.n; ++n){
            പൂർണ്ണസംഖ്യ index = entry_index(l, b, n*l.w*l.h, 0);
            gradient_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC, l.delta_gpu + index);
            എങ്കിൽ(l.coords > 4){
                index = entry_index(l, b, n*l.w*l.h, 4);
                gradient_array_gpu(l.output_gpu + index, (l.coords - 4)*l.w*l.h, LOGISTIC, l.delta_gpu + index);
            }
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            എങ്കിൽ(!l.background) gradient_array_gpu(l.output_gpu + index,   l.w*l.h, LOGISTIC, l.delta_gpu + index);
        }
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif

ശൂന്യം zero_objectness(layer l)
{
    പൂർണ്ണസംഖ്യ i, n;
    വേണ്ടി (i = 0; i < l.w*l.h; ++i){
        വേണ്ടി(n = 0; n < l.n; ++n){
            പൂർണ്ണസംഖ്യ obj_index = entry_index(l, 0, n*l.w*l.h + i, l.coords);
            l.output[obj_index] = 0;
        }
    }
}


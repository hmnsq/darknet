#અન્તર્ભાવ "region_layer.h"
#અન્તર્ભાવ "activations.h"
#અન્તર્ભાવ "blas.h"
#અન્તર્ભાવ "box.h"
#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ "utils.h"

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <stdlib.h>

layer make_region_layer(પૂર્ણાંક batch, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક n, પૂર્ણાંક classes, પૂર્ણાંક coords)
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
    l.cost = calloc(1, કદ(દશાંશ));
    l.biases = calloc(n*2, કદ(દશાંશ));
    l.bias_updates = calloc(n*2, કદ(દશાંશ));
    l.outputs = h*w*n*(classes + coords + 1);
    l.inputs = l.outputs;
    l.truths = 30*(l.coords + 1);
    l.delta = calloc(batch*l.outputs, કદ(દશાંશ));
    l.output = calloc(batch*l.outputs, કદ(દશાંશ));
    પૂર્ણાંક i;
    માટે(i = 0; i < n*2; ++i){
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

    પાછા l;
}

શૂન્ય resize_region_layer(layer *l, પૂર્ણાંક w, પૂર્ણાંક h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + l->coords + 1);
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*કદ(દશાંશ));
    l->delta = realloc(l->delta, l->batch*l->outputs*કદ(દશાંશ));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

box get_region_box(દશાંશ *x, દશાંશ *biases, પૂર્ણાંક n, પૂર્ણાંક index, પૂર્ણાંક i, પૂર્ણાંક j, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક stride)
{
    box b;
    b.x = (i + x[index + 0*stride]) / w;
    b.y = (j + x[index + 1*stride]) / h;
    b.w = exp(x[index + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[index + 3*stride]) * biases[2*n+1] / h;
    પાછા b;
}

દશાંશ delta_region_box(box truth, દશાંશ *x, દશાંશ *biases, પૂર્ણાંક n, પૂર્ણાંક index, પૂર્ણાંક i, પૂર્ણાંક j, પૂર્ણાંક w, પૂર્ણાંક h, દશાંશ *delta, દશાંશ scale, પૂર્ણાંક stride)
{
    box pred = get_region_box(x, biases, n, index, i, j, w, h, stride);
    દશાંશ iou = box_iou(pred, truth);

    દશાંશ tx = (truth.x*w - i);
    દશાંશ ty = (truth.y*h - j);
    દશાંશ tw = log(truth.w*w / biases[2*n]);
    દશાંશ th = log(truth.h*h / biases[2*n + 1]);

    delta[index + 0*stride] = scale * (tx - x[index + 0*stride]);
    delta[index + 1*stride] = scale * (ty - x[index + 1*stride]);
    delta[index + 2*stride] = scale * (tw - x[index + 2*stride]);
    delta[index + 3*stride] = scale * (th - x[index + 3*stride]);
    પાછા iou;
}

શૂન્ય delta_region_mask(દશાંશ *truth, દશાંશ *x, પૂર્ણાંક n, પૂર્ણાંક index, દશાંશ *delta, પૂર્ણાંક stride, પૂર્ણાંક scale)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < n; ++i){
        delta[index + i*stride] = scale*(truth[i] - x[index + i*stride]);
    }
}


શૂન્ય delta_region_class(દશાંશ *output, દશાંશ *delta, પૂર્ણાંક index, પૂર્ણાંક class, પૂર્ણાંક classes, વૃક્ષ *hier, દશાંશ scale, પૂર્ણાંક stride, દશાંશ *avg_cat, પૂર્ણાંક tag)
{
    પૂર્ણાંક i, n;
    જો(hier){
        દશાંશ pred = 1;
        જ્યારે(class >= 0){
            pred *= output[index + stride*class];
            પૂર્ણાંક g = hier->group[class];
            પૂર્ણાંક offset = hier->group_offset[g];
            માટે(i = 0; i < hier->group_size[g]; ++i){
                delta[index + stride*(offset + i)] = scale * (0 - output[index + stride*(offset + i)]);
            }
            delta[index + stride*class] = scale * (1 - output[index + stride*class]);

            class = hier->parent[class];
        }
        *avg_cat += pred;
    } નહીંતર {
        જો (delta[index] && tag){
            delta[index + stride*class] = scale * (1 - output[index + stride*class]);
            પાછા;
        }
        માટે(n = 0; n < classes; ++n){
            delta[index + stride*n] = scale * (((n == class)?1 : 0) - output[index + stride*n]);
            જો(n == class) *avg_cat += output[index + stride*n];
        }
    }
}

દશાંશ logit(દશાંશ x)
{
    પાછા log(x/(1.-x));
}

દશાંશ tisnan(દશાંશ x)
{
    પાછા (x != x);
}

પૂર્ણાંક entry_index(layer l, પૂર્ણાંક batch, પૂર્ણાંક location, પૂર્ણાંક entry)
{
    પૂર્ણાંક n =   location / (l.w*l.h);
    પૂર્ણાંક loc = location % (l.w*l.h);
    પાછા batch*l.outputs + n*l.w*l.h*(l.coords+l.classes+1) + entry*l.w*l.h + loc;
}

શૂન્ય forward_region_layer(અચલ layer l, network net)
{
    પૂર્ણાંક i,j,b,t,n;
    memcpy(l.output, net.આગત, l.outputs*l.batch*કદ(દશાંશ));

#ifndef GPU
    માટે (b = 0; b < l.batch; ++b){
        માટે(n = 0; n < l.n; ++n){
            પૂર્ણાંક index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array(l.output + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            જો(!l.background) activate_array(l.output + index,   l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords + 1);
            જો(!l.softmax && !l.softmax_વૃક્ષ) activate_array(l.output + index, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    જો (l.softmax_વૃક્ષ){
        પૂર્ણાંક i;
        પૂર્ણાંક ગણતરી = l.coords + 1;
        માટે (i = 0; i < l.softmax_વૃક્ષ->groups; ++i) {
            પૂર્ણાંક group_size = l.softmax_વૃક્ષ->group_size[i];
            softmax_cpu(net.આગત + ગણતરી, group_size, l.batch, l.inputs, l.n*l.w*l.h, 1, l.n*l.w*l.h, l.temperature, l.output + ગણતરી);
            ગણતરી += group_size;
        }
    } નહીંતર જો (l.softmax){
        પૂર્ણાંક index = entry_index(l, 0, 0, l.coords + !l.background);
        softmax_cpu(net.આગત + index, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output + index);
    }
#endif

    memset(l.delta, 0, l.outputs * l.batch * કદ(દશાંશ));
    જો(!net.train) પાછા;
    દશાંશ avg_iou = 0;
    દશાંશ recall = 0;
    દશાંશ avg_cat = 0;
    દશાંશ avg_obj = 0;
    દશાંશ avg_anyobj = 0;
    પૂર્ણાંક ગણતરી = 0;
    પૂર્ણાંક class_ગણતરી = 0;
    *(l.cost) = 0;
    માટે (b = 0; b < l.batch; ++b) {
        જો(l.softmax_વૃક્ષ){
            પૂર્ણાંક onlyclass = 0;
            માટે(t = 0; t < 30; ++t){
                box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                જો(!truth.x) તોડો;
                પૂર્ણાંક class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
                દશાંશ maxp = 0;
                પૂર્ણાંક maxi = 0;
                જો(truth.x > 100000 && truth.y > 100000){
                    માટે(n = 0; n < l.n*l.w*l.h; ++n){
                        પૂર્ણાંક class_index = entry_index(l, b, n, l.coords + 1);
                        પૂર્ણાંક obj_index = entry_index(l, b, n, l.coords);
                        દશાંશ scale =  l.output[obj_index];
                        l.delta[obj_index] = l.noobject_scale * (0 - l.output[obj_index]);
                        દશાંશ p = scale*get_hierarchy_probability(l.output + class_index, l.softmax_વૃક્ષ, class, l.w*l.h);
                        જો(p > maxp){
                            maxp = p;
                            maxi = n;
                        }
                    }
                    પૂર્ણાંક class_index = entry_index(l, b, maxi, l.coords + 1);
                    પૂર્ણાંક obj_index = entry_index(l, b, maxi, l.coords);
                    delta_region_class(l.output, l.delta, class_index, class, l.classes, l.softmax_વૃક્ષ, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
                    જો(l.output[obj_index] < .3) l.delta[obj_index] = l.object_scale * (.3 - l.output[obj_index]);
                    નહીંતર  l.delta[obj_index] = 0;
                    l.delta[obj_index] = 0;
                    ++class_ગણતરી;
                    onlyclass = 1;
                    તોડો;
                }
            }
            જો(onlyclass) ચાલુ;
        }
        માટે (j = 0; j < l.h; ++j) {
            માટે (i = 0; i < l.w; ++i) {
                માટે (n = 0; n < l.n; ++n) {
                    પૂર્ણાંક box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_region_box(l.output, l.biases, n, box_index, i, j, l.w, l.h, l.w*l.h);
                    દશાંશ best_iou = 0;
                    માટે(t = 0; t < 30; ++t){
                        box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                        જો(!truth.x) તોડો;
                        દશાંશ iou = box_iou(pred, truth);
                        જો (iou > best_iou) {
                            best_iou = iou;
                        }
                    }
                    પૂર્ણાંક obj_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, l.coords);
                    avg_anyobj += l.output[obj_index];
                    l.delta[obj_index] = l.noobject_scale * (0 - l.output[obj_index]);
                    જો(l.background) l.delta[obj_index] = l.noobject_scale * (1 - l.output[obj_index]);
                    જો (best_iou > l.thresh) {
                        l.delta[obj_index] = 0;
                    }

                    જો(*(net.seen) < 12800){
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
        માટે(t = 0; t < 30; ++t){
            box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);

            જો(!truth.x) તોડો;
            દશાંશ best_iou = 0;
            પૂર્ણાંક best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = 0;
            truth_shift.y = 0;
            માટે(n = 0; n < l.n; ++n){
                પૂર્ણાંક box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                box pred = get_region_box(l.output, l.biases, n, box_index, i, j, l.w, l.h, l.w*l.h);
                જો(l.bias_match){
                    pred.w = l.biases[2*n]/l.w;
                    pred.h = l.biases[2*n+1]/l.h;
                }
                pred.x = 0;
                pred.y = 0;
                દશાંશ iou = box_iou(pred, truth_shift);
                જો (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            પૂર્ણાંક box_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, 0);
            દશાંશ iou = delta_region_box(truth, l.output, l.biases, best_n, box_index, i, j, l.w, l.h, l.delta, l.coord_scale *  (2 - truth.w*truth.h), l.w*l.h);
            જો(l.coords > 4){
                પૂર્ણાંક mask_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, 4);
                delta_region_mask(net.truth + t*(l.coords + 1) + b*l.truths + 5, l.output, l.coords - 4, mask_index, l.delta, l.w*l.h, l.mask_scale);
            }
            જો(iou > .5) recall += 1;
            avg_iou += iou;

            પૂર્ણાંક obj_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, l.coords);
            avg_obj += l.output[obj_index];
            l.delta[obj_index] = l.object_scale * (1 - l.output[obj_index]);
            જો (l.rescore) {
                l.delta[obj_index] = l.object_scale * (iou - l.output[obj_index]);
            }
            જો(l.background){
                l.delta[obj_index] = l.object_scale * (0 - l.output[obj_index]);
            }

            પૂર્ણાંક class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
            જો (l.map) class = l.map[class];
            પૂર્ણાંક class_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, l.coords + 1);
            delta_region_class(l.output, l.delta, class_index, class, l.classes, l.softmax_વૃક્ષ, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
            ++ગણતરી;
            ++class_ગણતરી;
        }
    }
    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("Region Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, Avg Recall: %f,  count: %d\n", avg_iou/ગણતરી, avg_cat/class_ગણતરી, avg_obj/ગણતરી, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/ગણતરી, ગણતરી);
}

શૂન્ય backward_region_layer(અચલ layer l, network net)
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

શૂન્ય correct_region_boxes(detection *dets, પૂર્ણાંક n, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક netw, પૂર્ણાંક neth, પૂર્ણાંક relative)
{
    પૂર્ણાંક i;
    પૂર્ણાંક new_w=0;
    પૂર્ણાંક new_h=0;
    જો (((દશાંશ)netw/w) < ((દશાંશ)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } નહીંતર {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    માટે (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((દશાંશ)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((દશાંશ)new_h/neth); 
        b.w *= (દશાંશ)netw/new_w;
        b.h *= (દશાંશ)neth/new_h;
        જો(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

શૂન્ય get_region_detections(layer l, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક netw, પૂર્ણાંક neth, દશાંશ thresh, પૂર્ણાંક *map, દશાંશ વૃક્ષ_thresh, પૂર્ણાંક relative, detection *dets)
{
    પૂર્ણાંક i,j,n,z;
    દશાંશ *predictions = l.output;
    જો (l.batch == 2) {
        દશાંશ *flip = l.output + l.outputs;
        માટે (j = 0; j < l.h; ++j) {
            માટે (i = 0; i < l.w/2; ++i) {
                માટે (n = 0; n < l.n; ++n) {
                    માટે(z = 0; z < l.classes + l.coords + 1; ++z){
                        પૂર્ણાંક i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                        પૂર્ણાંક i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                        દશાંશ swap = flip[i1];
                        flip[i1] = flip[i2];
                        flip[i2] = swap;
                        જો(z == 0){
                            flip[i1] = -flip[i1];
                            flip[i2] = -flip[i2];
                        }
                    }
                }
            }
        }
        માટે(i = 0; i < l.outputs; ++i){
            l.output[i] = (l.output[i] + flip[i])/2.;
        }
    }
    માટે (i = 0; i < l.w*l.h; ++i){
        પૂર્ણાંક row = i / l.w;
        પૂર્ણાંક col = i % l.w;
        માટે(n = 0; n < l.n; ++n){
            પૂર્ણાંક index = n*l.w*l.h + i;
            માટે(j = 0; j < l.classes; ++j){
                dets[index].prob[j] = 0;
            }
            પૂર્ણાંક obj_index  = entry_index(l, 0, n*l.w*l.h + i, l.coords);
            પૂર્ણાંક box_index  = entry_index(l, 0, n*l.w*l.h + i, 0);
            પૂર્ણાંક mask_index = entry_index(l, 0, n*l.w*l.h + i, 4);
            દશાંશ scale = l.background ? 1 : predictions[obj_index];
            dets[index].bbox = get_region_box(predictions, l.biases, n, box_index, col, row, l.w, l.h, l.w*l.h);
            dets[index].objectness = scale > thresh ? scale : 0;
            જો(dets[index].mask){
                માટે(j = 0; j < l.coords - 4; ++j){
                    dets[index].mask[j] = l.output[mask_index + j*l.w*l.h];
                }
            }

            પૂર્ણાંક class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + !l.background);
            જો(l.softmax_વૃક્ષ){

                hierarchy_predictions(predictions + class_index, l.classes, l.softmax_વૃક્ષ, 0, l.w*l.h);
                જો(map){
                    માટે(j = 0; j < 200; ++j){
                        પૂર્ણાંક class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + 1 + map[j]);
                        દશાંશ prob = scale*predictions[class_index];
                        dets[index].prob[j] = (prob > thresh) ? prob : 0;
                    }
                } નહીંતર {
                    પૂર્ણાંક j =  hierarchy_ટોચ_prediction(predictions + class_index, l.softmax_વૃક્ષ, વૃક્ષ_thresh, l.w*l.h);
                    dets[index].prob[j] = (scale > thresh) ? scale : 0;
                }
            } નહીંતર {
                જો(dets[index].objectness){
                    માટે(j = 0; j < l.classes; ++j){
                        પૂર્ણાંક class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + 1 + j);
                        દશાંશ prob = scale*predictions[class_index];
                        dets[index].prob[j] = (prob > thresh) ? prob : 0;
                    }
                }
            }
        }
    }
    correct_region_boxes(dets, l.w*l.h*l.n, w, h, netw, neth, relative);
}

#ifdef GPU

શૂન્ય forward_region_layer_gpu(અચલ layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.આગત_gpu, 1, l.output_gpu, 1);
    પૂર્ણાંક b, n;
    માટે (b = 0; b < l.batch; ++b){
        માટે(n = 0; n < l.n; ++n){
            પૂર્ણાંક index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC);
            જો(l.coords > 4){
                index = entry_index(l, b, n*l.w*l.h, 4);
                activate_array_gpu(l.output_gpu + index, (l.coords - 4)*l.w*l.h, LOGISTIC);
            }
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            જો(!l.background) activate_array_gpu(l.output_gpu + index,   l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords + 1);
            જો(!l.softmax && !l.softmax_વૃક્ષ) activate_array_gpu(l.output_gpu + index, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    જો (l.softmax_વૃક્ષ){
        પૂર્ણાંક index = entry_index(l, 0, 0, l.coords + 1);
        softmax_વૃક્ષ(net.આગત_gpu + index, l.w*l.h, l.batch*l.n, l.inputs/l.n, 1, l.output_gpu + index, *l.softmax_વૃક્ષ);
    } નહીંતર જો (l.softmax) {
        પૂર્ણાંક index = entry_index(l, 0, 0, l.coords + !l.background);
        softmax_gpu(net.આગત_gpu + index, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu + index);
    }
    જો(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        પાછા;
    }

    cuda_pull_array(l.output_gpu, net.આગત, l.batch*l.inputs);
    forward_region_layer(l, net);
    //cuda_push_array(l.output_gpu, l.output, l.batch*l.outputs);
    જો(!net.train) પાછા;
    cuda_ધકેલો_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

શૂન્ય backward_region_layer_gpu(અચલ layer l, network net)
{
    પૂર્ણાંક b, n;
    માટે (b = 0; b < l.batch; ++b){
        માટે(n = 0; n < l.n; ++n){
            પૂર્ણાંક index = entry_index(l, b, n*l.w*l.h, 0);
            gradient_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC, l.delta_gpu + index);
            જો(l.coords > 4){
                index = entry_index(l, b, n*l.w*l.h, 4);
                gradient_array_gpu(l.output_gpu + index, (l.coords - 4)*l.w*l.h, LOGISTIC, l.delta_gpu + index);
            }
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            જો(!l.background) gradient_array_gpu(l.output_gpu + index,   l.w*l.h, LOGISTIC, l.delta_gpu + index);
        }
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif

શૂન્ય zero_objectness(layer l)
{
    પૂર્ણાંક i, n;
    માટે (i = 0; i < l.w*l.h; ++i){
        માટે(n = 0; n < l.n; ++n){
            પૂર્ણાંક obj_index = entry_index(l, 0, n*l.w*l.h + i, l.coords);
            l.output[obj_index] = 0;
        }
    }
}


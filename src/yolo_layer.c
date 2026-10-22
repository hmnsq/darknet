#അന്തര്ഭാവ "yolo_layer.h"
#അന്തര്ഭാവ "activations.h"
#അന്തര്ഭാവ "blas.h"
#അന്തര്ഭാവ "box.h"
#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ "utils.h"

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <stdlib.h>

layer make_yolo_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ total, പൂർണ്ണസംഖ്യ *mask, പൂർണ്ണസംഖ്യ classes)
{
    പൂർണ്ണസംഖ്യ i;
    layer l = {0};
    l.type = YOLO;

    l.n = n;
    l.total = total;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = n*(classes + 4 + 1);
    l.out_w = l.w;
    l.out_h = l.h;
    l.out_c = l.c;
    l.classes = classes;
    l.cost = calloc(1, വലുപ്പം(ദശാംശം));
    l.biases = calloc(total*2, വലുപ്പം(ദശാംശം));
    എങ്കിൽ(mask) l.mask = mask;
    അല്ലെങ്കിൽ{
        l.mask = calloc(n, വലുപ്പം(പൂർണ്ണസംഖ്യ));
        വേണ്ടി(i = 0; i < n; ++i){
            l.mask[i] = i;
        }
    }
    l.bias_updates = calloc(n*2, വലുപ്പം(ദശാംശം));
    l.outputs = h*w*n*(classes + 4 + 1);
    l.inputs = l.outputs;
    l.truths = 90*(4 + 1);
    l.delta = calloc(batch*l.outputs, വലുപ്പം(ദശാംശം));
    l.output = calloc(batch*l.outputs, വലുപ്പം(ദശാംശം));
    വേണ്ടി(i = 0; i < total*2; ++i){
        l.biases[i] = .5;
    }

    l.forward = forward_yolo_layer;
    l.backward = backward_yolo_layer;
#ifdef GPU
    l.forward_gpu = forward_yolo_layer_gpu;
    l.backward_gpu = backward_yolo_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#endif

    fprintf(stderr, "yolo\n");
    srand(0);

    തിരികെ l;
}

ശൂന്യം resize_yolo_layer(layer *l, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + 4 + 1);
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

box get_yolo_box(ദശാംശം *x, ദശാംശം *biases, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ index, പൂർണ്ണസംഖ്യ i, പൂർണ്ണസംഖ്യ j, പൂർണ്ണസംഖ്യ lw, പൂർണ്ണസംഖ്യ lh, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ stride)
{
    box b;
    b.x = (i + x[index + 0*stride]) / lw;
    b.y = (j + x[index + 1*stride]) / lh;
    b.w = exp(x[index + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[index + 3*stride]) * biases[2*n+1] / h;
    തിരികെ b;
}

ദശാംശം delta_yolo_box(box truth, ദശാംശം *x, ദശാംശം *biases, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ index, പൂർണ്ണസംഖ്യ i, പൂർണ്ണസംഖ്യ j, പൂർണ്ണസംഖ്യ lw, പൂർണ്ണസംഖ്യ lh, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, ദശാംശം *delta, ദശാംശം scale, പൂർണ്ണസംഖ്യ stride)
{
    box pred = get_yolo_box(x, biases, n, index, i, j, lw, lh, w, h, stride);
    ദശാംശം iou = box_iou(pred, truth);

    ദശാംശം tx = (truth.x*lw - i);
    ദശാംശം ty = (truth.y*lh - j);
    ദശാംശം tw = log(truth.w*w / biases[2*n]);
    ദശാംശം th = log(truth.h*h / biases[2*n + 1]);

    delta[index + 0*stride] = scale * (tx - x[index + 0*stride]);
    delta[index + 1*stride] = scale * (ty - x[index + 1*stride]);
    delta[index + 2*stride] = scale * (tw - x[index + 2*stride]);
    delta[index + 3*stride] = scale * (th - x[index + 3*stride]);
    തിരികെ iou;
}


ശൂന്യം delta_yolo_class(ദശാംശം *output, ദശാംശം *delta, പൂർണ്ണസംഖ്യ index, പൂർണ്ണസംഖ്യ class, പൂർണ്ണസംഖ്യ classes, പൂർണ്ണസംഖ്യ stride, ദശാംശം *avg_cat)
{
    പൂർണ്ണസംഖ്യ n;
    എങ്കിൽ (delta[index]){
        delta[index + stride*class] = 1 - output[index + stride*class];
        എങ്കിൽ(avg_cat) *avg_cat += output[index + stride*class];
        തിരികെ;
    }
    വേണ്ടി(n = 0; n < classes; ++n){
        delta[index + stride*n] = ((n == class)?1 : 0) - output[index + stride*n];
        എങ്കിൽ(n == class && avg_cat) *avg_cat += output[index + stride*n];
    }
}

സ്ഥിതം പൂർണ്ണസംഖ്യ entry_index(layer l, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ location, പൂർണ്ണസംഖ്യ entry)
{
    പൂർണ്ണസംഖ്യ n =   location / (l.w*l.h);
    പൂർണ്ണസംഖ്യ loc = location % (l.w*l.h);
    തിരികെ batch*l.outputs + n*l.w*l.h*(4+l.classes+1) + entry*l.w*l.h + loc;
}

ശൂന്യം forward_yolo_layer(സ്ഥിരം layer l, network net)
{
    പൂർണ്ണസംഖ്യ i,j,b,t,n;
    memcpy(l.output, net.നിവേശം, l.outputs*l.batch*വലുപ്പം(ദശാംശം));

#ifndef GPU
    വേണ്ടി (b = 0; b < l.batch; ++b){
        വേണ്ടി(n = 0; n < l.n; ++n){
            പൂർണ്ണസംഖ്യ index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array(l.output + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array(l.output + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
#endif

    memset(l.delta, 0, l.outputs * l.batch * വലുപ്പം(ദശാംശം));
    എങ്കിൽ(!net.train) തിരികെ;
    ദശാംശം avg_iou = 0;
    ദശാംശം recall = 0;
    ദശാംശം recall75 = 0;
    ദശാംശം avg_cat = 0;
    ദശാംശം avg_obj = 0;
    ദശാംശം avg_anyobj = 0;
    പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    പൂർണ്ണസംഖ്യ class_എണ്ണം = 0;
    *(l.cost) = 0;
    വേണ്ടി (b = 0; b < l.batch; ++b) {
        വേണ്ടി (j = 0; j < l.h; ++j) {
            വേണ്ടി (i = 0; i < l.w; ++i) {
                വേണ്ടി (n = 0; n < l.n; ++n) {
                    പൂർണ്ണസംഖ്യ box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_yolo_box(l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.w*l.h);
                    ദശാംശം best_iou = 0;
                    പൂർണ്ണസംഖ്യ best_t = 0;
                    വേണ്ടി(t = 0; t < l.max_boxes; ++t){
                        box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);
                        എങ്കിൽ(!truth.x) നിർത്തുക;
                        ദശാംശം iou = box_iou(pred, truth);
                        എങ്കിൽ (iou > best_iou) {
                            best_iou = iou;
                            best_t = t;
                        }
                    }
                    പൂർണ്ണസംഖ്യ obj_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4);
                    avg_anyobj += l.output[obj_index];
                    l.delta[obj_index] = 0 - l.output[obj_index];
                    എങ്കിൽ (best_iou > l.ignore_thresh) {
                        l.delta[obj_index] = 0;
                    }
                    എങ്കിൽ (best_iou > l.truth_thresh) {
                        l.delta[obj_index] = 1 - l.output[obj_index];

                        പൂർണ്ണസംഖ്യ class = net.truth[best_t*(4 + 1) + b*l.truths + 4];
                        എങ്കിൽ (l.map) class = l.map[class];
                        പൂർണ്ണസംഖ്യ class_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4 + 1);
                        delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, 0);
                        box truth = float_to_box(net.truth + best_t*(4 + 1) + b*l.truths, 1);
                        delta_yolo_box(truth, l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);
                    }
                }
            }
        }
        വേണ്ടി(t = 0; t < l.max_boxes; ++t){
            box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);

            എങ്കിൽ(!truth.x) നിർത്തുക;
            ദശാംശം best_iou = 0;
            പൂർണ്ണസംഖ്യ best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = truth_shift.y = 0;
            വേണ്ടി(n = 0; n < l.total; ++n){
                box pred = {0};
                pred.w = l.biases[2*n]/net.w;
                pred.h = l.biases[2*n+1]/net.h;
                ദശാംശം iou = box_iou(pred, truth_shift);
                എങ്കിൽ (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            പൂർണ്ണസംഖ്യ mask_n = int_index(l.mask, best_n, l.n);
            എങ്കിൽ(mask_n >= 0){
                പൂർണ്ണസംഖ്യ box_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 0);
                ദശാംശം iou = delta_yolo_box(truth, l.output, l.biases, best_n, box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);

                പൂർണ്ണസംഖ്യ obj_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4);
                avg_obj += l.output[obj_index];
                l.delta[obj_index] = 1 - l.output[obj_index];

                പൂർണ്ണസംഖ്യ class = net.truth[t*(4 + 1) + b*l.truths + 4];
                എങ്കിൽ (l.map) class = l.map[class];
                പൂർണ്ണസംഖ്യ class_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4 + 1);
                delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, &avg_cat);

                ++എണ്ണം;
                ++class_എണ്ണം;
                എങ്കിൽ(iou > .5) recall += 1;
                എങ്കിൽ(iou > .75) recall75 += 1;
                avg_iou += iou;
            }
        }
    }
    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("Region %d Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, .5R: %f, .75R: %f,  count: %d\n", net.index, avg_iou/എണ്ണം, avg_cat/class_എണ്ണം, avg_obj/എണ്ണം, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/എണ്ണം, recall75/എണ്ണം, എണ്ണം);
}

ശൂന്യം backward_yolo_layer(സ്ഥിരം layer l, network net)
{
   axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

ശൂന്യം correct_yolo_boxes(detection *dets, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ netw, പൂർണ്ണസംഖ്യ neth, പൂർണ്ണസംഖ്യ relative)
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

പൂർണ്ണസംഖ്യ yolo_അക്കം_detections(layer l, ദശാംശം thresh)
{
    പൂർണ്ണസംഖ്യ i, n;
    പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    വേണ്ടി (i = 0; i < l.w*l.h; ++i){
        വേണ്ടി(n = 0; n < l.n; ++n){
            പൂർണ്ണസംഖ്യ obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            എങ്കിൽ(l.output[obj_index] > thresh){
                ++എണ്ണം;
            }
        }
    }
    തിരികെ എണ്ണം;
}

ശൂന്യം avg_flipped_yolo(layer l)
{
    പൂർണ്ണസംഖ്യ i,j,n,z;
    ദശാംശം *flip = l.output + l.outputs;
    വേണ്ടി (j = 0; j < l.h; ++j) {
        വേണ്ടി (i = 0; i < l.w/2; ++i) {
            വേണ്ടി (n = 0; n < l.n; ++n) {
                വേണ്ടി(z = 0; z < l.classes + 4 + 1; ++z){
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

പൂർണ്ണസംഖ്യ get_yolo_detections(layer l, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ netw, പൂർണ്ണസംഖ്യ neth, ദശാംശം thresh, പൂർണ്ണസംഖ്യ *map, പൂർണ്ണസംഖ്യ relative, detection *dets)
{
    പൂർണ്ണസംഖ്യ i,j,n;
    ദശാംശം *predictions = l.output;
    എങ്കിൽ (l.batch == 2) avg_flipped_yolo(l);
    പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    വേണ്ടി (i = 0; i < l.w*l.h; ++i){
        പൂർണ്ണസംഖ്യ row = i / l.w;
        പൂർണ്ണസംഖ്യ col = i % l.w;
        വേണ്ടി(n = 0; n < l.n; ++n){
            പൂർണ്ണസംഖ്യ obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            ദശാംശം objectness = predictions[obj_index];
            എങ്കിൽ(objectness <= thresh) തുടരുക;
            പൂർണ്ണസംഖ്യ box_index  = entry_index(l, 0, n*l.w*l.h + i, 0);
            dets[എണ്ണം].bbox = get_yolo_box(predictions, l.biases, l.mask[n], box_index, col, row, l.w, l.h, netw, neth, l.w*l.h);
            dets[എണ്ണം].objectness = objectness;
            dets[എണ്ണം].classes = l.classes;
            വേണ്ടി(j = 0; j < l.classes; ++j){
                പൂർണ്ണസംഖ്യ class_index = entry_index(l, 0, n*l.w*l.h + i, 4 + 1 + j);
                ദശാംശം prob = objectness*predictions[class_index];
                dets[എണ്ണം].prob[j] = (prob > thresh) ? prob : 0;
            }
            ++എണ്ണം;
        }
    }
    correct_yolo_boxes(dets, എണ്ണം, w, h, netw, neth, relative);
    തിരികെ എണ്ണം;
}

#ifdef GPU

ശൂന്യം forward_yolo_layer_gpu(സ്ഥിരം layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.നിവേശം_gpu, 1, l.output_gpu, 1);
    പൂർണ്ണസംഖ്യ b, n;
    വേണ്ടി (b = 0; b < l.batch; ++b){
        വേണ്ടി(n = 0; n < l.n; ++n){
            പൂർണ്ണസംഖ്യ index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array_gpu(l.output_gpu + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
    എങ്കിൽ(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        തിരികെ;
    }

    cuda_pull_array(l.output_gpu, net.നിവേശം, l.batch*l.inputs);
    forward_yolo_layer(l, net);
    cuda_തള്ളുക_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

ശൂന്യം backward_yolo_layer_gpu(സ്ഥിരം layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


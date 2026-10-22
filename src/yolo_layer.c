#અન્તર્ભાવ "yolo_layer.h"
#અન્તર્ભાવ "activations.h"
#અન્તર્ભાવ "blas.h"
#અન્તર્ભાવ "box.h"
#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ "utils.h"

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <stdlib.h>

layer make_yolo_layer(પૂર્ણાંક batch, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક n, પૂર્ણાંક total, પૂર્ણાંક *mask, પૂર્ણાંક classes)
{
    પૂર્ણાંક i;
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
    l.cost = calloc(1, કદ(દશાંશ));
    l.biases = calloc(total*2, કદ(દશાંશ));
    જો(mask) l.mask = mask;
    નહીંતર{
        l.mask = calloc(n, કદ(પૂર્ણાંક));
        માટે(i = 0; i < n; ++i){
            l.mask[i] = i;
        }
    }
    l.bias_updates = calloc(n*2, કદ(દશાંશ));
    l.outputs = h*w*n*(classes + 4 + 1);
    l.inputs = l.outputs;
    l.truths = 90*(4 + 1);
    l.delta = calloc(batch*l.outputs, કદ(દશાંશ));
    l.output = calloc(batch*l.outputs, કદ(દશાંશ));
    માટે(i = 0; i < total*2; ++i){
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

    પાછા l;
}

શૂન્ય resize_yolo_layer(layer *l, પૂર્ણાંક w, પૂર્ણાંક h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + 4 + 1);
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

box get_yolo_box(દશાંશ *x, દશાંશ *biases, પૂર્ણાંક n, પૂર્ણાંક index, પૂર્ણાંક i, પૂર્ણાંક j, પૂર્ણાંક lw, પૂર્ણાંક lh, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક stride)
{
    box b;
    b.x = (i + x[index + 0*stride]) / lw;
    b.y = (j + x[index + 1*stride]) / lh;
    b.w = exp(x[index + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[index + 3*stride]) * biases[2*n+1] / h;
    પાછા b;
}

દશાંશ delta_yolo_box(box truth, દશાંશ *x, દશાંશ *biases, પૂર્ણાંક n, પૂર્ણાંક index, પૂર્ણાંક i, પૂર્ણાંક j, પૂર્ણાંક lw, પૂર્ણાંક lh, પૂર્ણાંક w, પૂર્ણાંક h, દશાંશ *delta, દશાંશ scale, પૂર્ણાંક stride)
{
    box pred = get_yolo_box(x, biases, n, index, i, j, lw, lh, w, h, stride);
    દશાંશ iou = box_iou(pred, truth);

    દશાંશ tx = (truth.x*lw - i);
    દશાંશ ty = (truth.y*lh - j);
    દશાંશ tw = log(truth.w*w / biases[2*n]);
    દશાંશ th = log(truth.h*h / biases[2*n + 1]);

    delta[index + 0*stride] = scale * (tx - x[index + 0*stride]);
    delta[index + 1*stride] = scale * (ty - x[index + 1*stride]);
    delta[index + 2*stride] = scale * (tw - x[index + 2*stride]);
    delta[index + 3*stride] = scale * (th - x[index + 3*stride]);
    પાછા iou;
}


શૂન્ય delta_yolo_class(દશાંશ *output, દશાંશ *delta, પૂર્ણાંક index, પૂર્ણાંક class, પૂર્ણાંક classes, પૂર્ણાંક stride, દશાંશ *avg_cat)
{
    પૂર્ણાંક n;
    જો (delta[index]){
        delta[index + stride*class] = 1 - output[index + stride*class];
        જો(avg_cat) *avg_cat += output[index + stride*class];
        પાછા;
    }
    માટે(n = 0; n < classes; ++n){
        delta[index + stride*n] = ((n == class)?1 : 0) - output[index + stride*n];
        જો(n == class && avg_cat) *avg_cat += output[index + stride*n];
    }
}

સ્થિર પૂર્ણાંક entry_index(layer l, પૂર્ણાંક batch, પૂર્ણાંક location, પૂર્ણાંક entry)
{
    પૂર્ણાંક n =   location / (l.w*l.h);
    પૂર્ણાંક loc = location % (l.w*l.h);
    પાછા batch*l.outputs + n*l.w*l.h*(4+l.classes+1) + entry*l.w*l.h + loc;
}

શૂન્ય forward_yolo_layer(અચલ layer l, network net)
{
    પૂર્ણાંક i,j,b,t,n;
    memcpy(l.output, net.આગત, l.outputs*l.batch*કદ(દશાંશ));

#ifndef GPU
    માટે (b = 0; b < l.batch; ++b){
        માટે(n = 0; n < l.n; ++n){
            પૂર્ણાંક index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array(l.output + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array(l.output + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
#endif

    memset(l.delta, 0, l.outputs * l.batch * કદ(દશાંશ));
    જો(!net.train) પાછા;
    દશાંશ avg_iou = 0;
    દશાંશ recall = 0;
    દશાંશ recall75 = 0;
    દશાંશ avg_cat = 0;
    દશાંશ avg_obj = 0;
    દશાંશ avg_anyobj = 0;
    પૂર્ણાંક ગણતરી = 0;
    પૂર્ણાંક class_ગણતરી = 0;
    *(l.cost) = 0;
    માટે (b = 0; b < l.batch; ++b) {
        માટે (j = 0; j < l.h; ++j) {
            માટે (i = 0; i < l.w; ++i) {
                માટે (n = 0; n < l.n; ++n) {
                    પૂર્ણાંક box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_yolo_box(l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.w*l.h);
                    દશાંશ best_iou = 0;
                    પૂર્ણાંક best_t = 0;
                    માટે(t = 0; t < l.max_boxes; ++t){
                        box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);
                        જો(!truth.x) તોડો;
                        દશાંશ iou = box_iou(pred, truth);
                        જો (iou > best_iou) {
                            best_iou = iou;
                            best_t = t;
                        }
                    }
                    પૂર્ણાંક obj_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4);
                    avg_anyobj += l.output[obj_index];
                    l.delta[obj_index] = 0 - l.output[obj_index];
                    જો (best_iou > l.ignore_thresh) {
                        l.delta[obj_index] = 0;
                    }
                    જો (best_iou > l.truth_thresh) {
                        l.delta[obj_index] = 1 - l.output[obj_index];

                        પૂર્ણાંક class = net.truth[best_t*(4 + 1) + b*l.truths + 4];
                        જો (l.map) class = l.map[class];
                        પૂર્ણાંક class_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4 + 1);
                        delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, 0);
                        box truth = float_to_box(net.truth + best_t*(4 + 1) + b*l.truths, 1);
                        delta_yolo_box(truth, l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);
                    }
                }
            }
        }
        માટે(t = 0; t < l.max_boxes; ++t){
            box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);

            જો(!truth.x) તોડો;
            દશાંશ best_iou = 0;
            પૂર્ણાંક best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = truth_shift.y = 0;
            માટે(n = 0; n < l.total; ++n){
                box pred = {0};
                pred.w = l.biases[2*n]/net.w;
                pred.h = l.biases[2*n+1]/net.h;
                દશાંશ iou = box_iou(pred, truth_shift);
                જો (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            પૂર્ણાંક mask_n = int_index(l.mask, best_n, l.n);
            જો(mask_n >= 0){
                પૂર્ણાંક box_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 0);
                દશાંશ iou = delta_yolo_box(truth, l.output, l.biases, best_n, box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);

                પૂર્ણાંક obj_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4);
                avg_obj += l.output[obj_index];
                l.delta[obj_index] = 1 - l.output[obj_index];

                પૂર્ણાંક class = net.truth[t*(4 + 1) + b*l.truths + 4];
                જો (l.map) class = l.map[class];
                પૂર્ણાંક class_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4 + 1);
                delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, &avg_cat);

                ++ગણતરી;
                ++class_ગણતરી;
                જો(iou > .5) recall += 1;
                જો(iou > .75) recall75 += 1;
                avg_iou += iou;
            }
        }
    }
    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("Region %d Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, .5R: %f, .75R: %f,  count: %d\n", net.index, avg_iou/ગણતરી, avg_cat/class_ગણતરી, avg_obj/ગણતરી, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/ગણતરી, recall75/ગણતરી, ગણતરી);
}

શૂન્ય backward_yolo_layer(અચલ layer l, network net)
{
   axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

શૂન્ય correct_yolo_boxes(detection *dets, પૂર્ણાંક n, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક netw, પૂર્ણાંક neth, પૂર્ણાંક relative)
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

પૂર્ણાંક yolo_અંક_detections(layer l, દશાંશ thresh)
{
    પૂર્ણાંક i, n;
    પૂર્ણાંક ગણતરી = 0;
    માટે (i = 0; i < l.w*l.h; ++i){
        માટે(n = 0; n < l.n; ++n){
            પૂર્ણાંક obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            જો(l.output[obj_index] > thresh){
                ++ગણતરી;
            }
        }
    }
    પાછા ગણતરી;
}

શૂન્ય avg_flipped_yolo(layer l)
{
    પૂર્ણાંક i,j,n,z;
    દશાંશ *flip = l.output + l.outputs;
    માટે (j = 0; j < l.h; ++j) {
        માટે (i = 0; i < l.w/2; ++i) {
            માટે (n = 0; n < l.n; ++n) {
                માટે(z = 0; z < l.classes + 4 + 1; ++z){
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

પૂર્ણાંક get_yolo_detections(layer l, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક netw, પૂર્ણાંક neth, દશાંશ thresh, પૂર્ણાંક *map, પૂર્ણાંક relative, detection *dets)
{
    પૂર્ણાંક i,j,n;
    દશાંશ *predictions = l.output;
    જો (l.batch == 2) avg_flipped_yolo(l);
    પૂર્ણાંક ગણતરી = 0;
    માટે (i = 0; i < l.w*l.h; ++i){
        પૂર્ણાંક row = i / l.w;
        પૂર્ણાંક col = i % l.w;
        માટે(n = 0; n < l.n; ++n){
            પૂર્ણાંક obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            દશાંશ objectness = predictions[obj_index];
            જો(objectness <= thresh) ચાલુ;
            પૂર્ણાંક box_index  = entry_index(l, 0, n*l.w*l.h + i, 0);
            dets[ગણતરી].bbox = get_yolo_box(predictions, l.biases, l.mask[n], box_index, col, row, l.w, l.h, netw, neth, l.w*l.h);
            dets[ગણતરી].objectness = objectness;
            dets[ગણતરી].classes = l.classes;
            માટે(j = 0; j < l.classes; ++j){
                પૂર્ણાંક class_index = entry_index(l, 0, n*l.w*l.h + i, 4 + 1 + j);
                દશાંશ prob = objectness*predictions[class_index];
                dets[ગણતરી].prob[j] = (prob > thresh) ? prob : 0;
            }
            ++ગણતરી;
        }
    }
    correct_yolo_boxes(dets, ગણતરી, w, h, netw, neth, relative);
    પાછા ગણતરી;
}

#ifdef GPU

શૂન્ય forward_yolo_layer_gpu(અચલ layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.આગત_gpu, 1, l.output_gpu, 1);
    પૂર્ણાંક b, n;
    માટે (b = 0; b < l.batch; ++b){
        માટે(n = 0; n < l.n; ++n){
            પૂર્ણાંક index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array_gpu(l.output_gpu + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
    જો(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        પાછા;
    }

    cuda_pull_array(l.output_gpu, net.આગત, l.batch*l.inputs);
    forward_yolo_layer(l, net);
    cuda_ધકેલો_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

શૂન્ય backward_yolo_layer_gpu(અચલ layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


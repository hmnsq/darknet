#ಅನ್ತರ್ಭಾವ "yolo_layer.h"
#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "box.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>

layer make_yolo_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ total, ಪೂರ್ಣಾಙ್ಕ *mask, ಪೂರ್ಣಾಙ್ಕ classes)
{
    ಪೂರ್ಣಾಙ್ಕ i;
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
    l.cost = calloc(1, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.biases = calloc(total*2, ಪರಿಮಾಣ(ಭಿನ್ನ));
    ಯದಿ(mask) l.mask = mask;
    ಅನ್ಯಥಾ{
        l.mask = calloc(n, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ));
        ಪ್ರತಿ(i = 0; i < n; ++i){
            l.mask[i] = i;
        }
    }
    l.bias_updates = calloc(n*2, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.outputs = h*w*n*(classes + 4 + 1);
    l.inputs = l.outputs;
    l.truths = 90*(4 + 1);
    l.delta = calloc(batch*l.outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.output = calloc(batch*l.outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
    ಪ್ರತಿ(i = 0; i < total*2; ++i){
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

    ಪ್ರತ್ಯಾಗಮ l;
}

ಶೂನ್ಯ resize_yolo_layer(layer *l, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + 4 + 1);
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*ಪರಿಮಾಣ(ಭಿನ್ನ));
    l->delta = realloc(l->delta, l->batch*l->outputs*ಪರಿಮಾಣ(ಭಿನ್ನ));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

box get_yolo_box(ಭಿನ್ನ *x, ಭಿನ್ನ *biases, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ index, ಪೂರ್ಣಾಙ್ಕ i, ಪೂರ್ಣಾಙ್ಕ j, ಪೂರ್ಣಾಙ್ಕ lw, ಪೂರ್ಣಾಙ್ಕ lh, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ stride)
{
    box b;
    b.x = (i + x[index + 0*stride]) / lw;
    b.y = (j + x[index + 1*stride]) / lh;
    b.w = exp(x[index + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[index + 3*stride]) * biases[2*n+1] / h;
    ಪ್ರತ್ಯಾಗಮ b;
}

ಭಿನ್ನ delta_yolo_box(box truth, ಭಿನ್ನ *x, ಭಿನ್ನ *biases, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ index, ಪೂರ್ಣಾಙ್ಕ i, ಪೂರ್ಣಾಙ್ಕ j, ಪೂರ್ಣಾಙ್ಕ lw, ಪೂರ್ಣಾಙ್ಕ lh, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಭಿನ್ನ *delta, ಭಿನ್ನ scale, ಪೂರ್ಣಾಙ್ಕ stride)
{
    box pred = get_yolo_box(x, biases, n, index, i, j, lw, lh, w, h, stride);
    ಭಿನ್ನ iou = box_iou(pred, truth);

    ಭಿನ್ನ tx = (truth.x*lw - i);
    ಭಿನ್ನ ty = (truth.y*lh - j);
    ಭಿನ್ನ tw = log(truth.w*w / biases[2*n]);
    ಭಿನ್ನ th = log(truth.h*h / biases[2*n + 1]);

    delta[index + 0*stride] = scale * (tx - x[index + 0*stride]);
    delta[index + 1*stride] = scale * (ty - x[index + 1*stride]);
    delta[index + 2*stride] = scale * (tw - x[index + 2*stride]);
    delta[index + 3*stride] = scale * (th - x[index + 3*stride]);
    ಪ್ರತ್ಯಾಗಮ iou;
}


ಶೂನ್ಯ delta_yolo_class(ಭಿನ್ನ *output, ಭಿನ್ನ *delta, ಪೂರ್ಣಾಙ್ಕ index, ಪೂರ್ಣಾಙ್ಕ class, ಪೂರ್ಣಾಙ್ಕ classes, ಪೂರ್ಣಾಙ್ಕ stride, ಭಿನ್ನ *avg_cat)
{
    ಪೂರ್ಣಾಙ್ಕ n;
    ಯದಿ (delta[index]){
        delta[index + stride*class] = 1 - output[index + stride*class];
        ಯದಿ(avg_cat) *avg_cat += output[index + stride*class];
        ಪ್ರತ್ಯಾಗಮ;
    }
    ಪ್ರತಿ(n = 0; n < classes; ++n){
        delta[index + stride*n] = ((n == class)?1 : 0) - output[index + stride*n];
        ಯದಿ(n == class && avg_cat) *avg_cat += output[index + stride*n];
    }
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ entry_index(layer l, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ location, ಪೂರ್ಣಾಙ್ಕ entry)
{
    ಪೂರ್ಣಾಙ್ಕ n =   location / (l.w*l.h);
    ಪೂರ್ಣಾಙ್ಕ loc = location % (l.w*l.h);
    ಪ್ರತ್ಯಾಗಮ batch*l.outputs + n*l.w*l.h*(4+l.classes+1) + entry*l.w*l.h + loc;
}

ಶೂನ್ಯ forward_yolo_layer(ಧ್ರುವ layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ i,j,b,t,n;
    memcpy(l.output, net.ಒಳಹರಿವು, l.outputs*l.batch*ಪರಿಮಾಣ(ಭಿನ್ನ));

#ifndef GPU
    ಪ್ರತಿ (b = 0; b < l.batch; ++b){
        ಪ್ರತಿ(n = 0; n < l.n; ++n){
            ಪೂರ್ಣಾಙ್ಕ index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array(l.output + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array(l.output + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
#endif

    memset(l.delta, 0, l.outputs * l.batch * ಪರಿಮಾಣ(ಭಿನ್ನ));
    ಯದಿ(!net.train) ಪ್ರತ್ಯಾಗಮ;
    ಭಿನ್ನ avg_iou = 0;
    ಭಿನ್ನ recall = 0;
    ಭಿನ್ನ recall75 = 0;
    ಭಿನ್ನ avg_cat = 0;
    ಭಿನ್ನ avg_obj = 0;
    ಭಿನ್ನ avg_anyobj = 0;
    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
    ಪೂರ್ಣಾಙ್ಕ class_ಎಣಿಕೆ = 0;
    *(l.cost) = 0;
    ಪ್ರತಿ (b = 0; b < l.batch; ++b) {
        ಪ್ರತಿ (j = 0; j < l.h; ++j) {
            ಪ್ರತಿ (i = 0; i < l.w; ++i) {
                ಪ್ರತಿ (n = 0; n < l.n; ++n) {
                    ಪೂರ್ಣಾಙ್ಕ box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_yolo_box(l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.w*l.h);
                    ಭಿನ್ನ best_iou = 0;
                    ಪೂರ್ಣಾಙ್ಕ best_t = 0;
                    ಪ್ರತಿ(t = 0; t < l.max_boxes; ++t){
                        box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);
                        ಯದಿ(!truth.x) ಭಙ್ಗ;
                        ಭಿನ್ನ iou = box_iou(pred, truth);
                        ಯದಿ (iou > best_iou) {
                            best_iou = iou;
                            best_t = t;
                        }
                    }
                    ಪೂರ್ಣಾಙ್ಕ obj_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4);
                    avg_anyobj += l.output[obj_index];
                    l.delta[obj_index] = 0 - l.output[obj_index];
                    ಯದಿ (best_iou > l.ignore_thresh) {
                        l.delta[obj_index] = 0;
                    }
                    ಯದಿ (best_iou > l.truth_thresh) {
                        l.delta[obj_index] = 1 - l.output[obj_index];

                        ಪೂರ್ಣಾಙ್ಕ class = net.truth[best_t*(4 + 1) + b*l.truths + 4];
                        ಯದಿ (l.map) class = l.map[class];
                        ಪೂರ್ಣಾಙ್ಕ class_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4 + 1);
                        delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, 0);
                        box truth = float_to_box(net.truth + best_t*(4 + 1) + b*l.truths, 1);
                        delta_yolo_box(truth, l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);
                    }
                }
            }
        }
        ಪ್ರತಿ(t = 0; t < l.max_boxes; ++t){
            box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);

            ಯದಿ(!truth.x) ಭಙ್ಗ;
            ಭಿನ್ನ best_iou = 0;
            ಪೂರ್ಣಾಙ್ಕ best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = truth_shift.y = 0;
            ಪ್ರತಿ(n = 0; n < l.total; ++n){
                box pred = {0};
                pred.w = l.biases[2*n]/net.w;
                pred.h = l.biases[2*n+1]/net.h;
                ಭಿನ್ನ iou = box_iou(pred, truth_shift);
                ಯದಿ (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            ಪೂರ್ಣಾಙ್ಕ mask_n = int_index(l.mask, best_n, l.n);
            ಯದಿ(mask_n >= 0){
                ಪೂರ್ಣಾಙ್ಕ box_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 0);
                ಭಿನ್ನ iou = delta_yolo_box(truth, l.output, l.biases, best_n, box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);

                ಪೂರ್ಣಾಙ್ಕ obj_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4);
                avg_obj += l.output[obj_index];
                l.delta[obj_index] = 1 - l.output[obj_index];

                ಪೂರ್ಣಾಙ್ಕ class = net.truth[t*(4 + 1) + b*l.truths + 4];
                ಯದಿ (l.map) class = l.map[class];
                ಪೂರ್ಣಾಙ್ಕ class_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4 + 1);
                delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, &avg_cat);

                ++ಎಣಿಕೆ;
                ++class_ಎಣಿಕೆ;
                ಯದಿ(iou > .5) recall += 1;
                ಯದಿ(iou > .75) recall75 += 1;
                avg_iou += iou;
            }
        }
    }
    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("Region %d Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, .5R: %f, .75R: %f,  count: %d\n", net.index, avg_iou/ಎಣಿಕೆ, avg_cat/class_ಎಣಿಕೆ, avg_obj/ಎಣಿಕೆ, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/ಎಣಿಕೆ, recall75/ಎಣಿಕೆ, ಎಣಿಕೆ);
}

ಶೂನ್ಯ backward_yolo_layer(ಧ್ರುವ layer l, network net)
{
   axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

ಶೂನ್ಯ correct_yolo_boxes(detection *dets, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ netw, ಪೂರ್ಣಾಙ್ಕ neth, ಪೂರ್ಣಾಙ್ಕ relative)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪೂರ್ಣಾಙ್ಕ new_w=0;
    ಪೂರ್ಣಾಙ್ಕ new_h=0;
    ಯದಿ (((ಭಿನ್ನ)netw/w) < ((ಭಿನ್ನ)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } ಅನ್ಯಥಾ {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    ಪ್ರತಿ (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((ಭಿನ್ನ)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((ಭಿನ್ನ)new_h/neth); 
        b.w *= (ಭಿನ್ನ)netw/new_w;
        b.h *= (ಭಿನ್ನ)neth/new_h;
        ಯದಿ(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

ಪೂರ್ಣಾಙ್ಕ yolo_ಅಂಕೆ_detections(layer l, ಭಿನ್ನ thresh)
{
    ಪೂರ್ಣಾಙ್ಕ i, n;
    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
    ಪ್ರತಿ (i = 0; i < l.w*l.h; ++i){
        ಪ್ರತಿ(n = 0; n < l.n; ++n){
            ಪೂರ್ಣಾಙ್ಕ obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            ಯದಿ(l.output[obj_index] > thresh){
                ++ಎಣಿಕೆ;
            }
        }
    }
    ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ;
}

ಶೂನ್ಯ avg_flipped_yolo(layer l)
{
    ಪೂರ್ಣಾಙ್ಕ i,j,n,z;
    ಭಿನ್ನ *flip = l.output + l.outputs;
    ಪ್ರತಿ (j = 0; j < l.h; ++j) {
        ಪ್ರತಿ (i = 0; i < l.w/2; ++i) {
            ಪ್ರತಿ (n = 0; n < l.n; ++n) {
                ಪ್ರತಿ(z = 0; z < l.classes + 4 + 1; ++z){
                    ಪೂರ್ಣಾಙ್ಕ i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                    ಪೂರ್ಣಾಙ್ಕ i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                    ಭಿನ್ನ swap = flip[i1];
                    flip[i1] = flip[i2];
                    flip[i2] = swap;
                    ಯದಿ(z == 0){
                        flip[i1] = -flip[i1];
                        flip[i2] = -flip[i2];
                    }
                }
            }
        }
    }
    ಪ್ರತಿ(i = 0; i < l.outputs; ++i){
        l.output[i] = (l.output[i] + flip[i])/2.;
    }
}

ಪೂರ್ಣಾಙ್ಕ get_yolo_detections(layer l, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ netw, ಪೂರ್ಣಾಙ್ಕ neth, ಭಿನ್ನ thresh, ಪೂರ್ಣಾಙ್ಕ *map, ಪೂರ್ಣಾಙ್ಕ relative, detection *dets)
{
    ಪೂರ್ಣಾಙ್ಕ i,j,n;
    ಭಿನ್ನ *predictions = l.output;
    ಯದಿ (l.batch == 2) avg_flipped_yolo(l);
    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
    ಪ್ರತಿ (i = 0; i < l.w*l.h; ++i){
        ಪೂರ್ಣಾಙ್ಕ row = i / l.w;
        ಪೂರ್ಣಾಙ್ಕ col = i % l.w;
        ಪ್ರತಿ(n = 0; n < l.n; ++n){
            ಪೂರ್ಣಾಙ್ಕ obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            ಭಿನ್ನ objectness = predictions[obj_index];
            ಯದಿ(objectness <= thresh) ಅನುವರ್ತ;
            ಪೂರ್ಣಾಙ್ಕ box_index  = entry_index(l, 0, n*l.w*l.h + i, 0);
            dets[ಎಣಿಕೆ].bbox = get_yolo_box(predictions, l.biases, l.mask[n], box_index, col, row, l.w, l.h, netw, neth, l.w*l.h);
            dets[ಎಣಿಕೆ].objectness = objectness;
            dets[ಎಣಿಕೆ].classes = l.classes;
            ಪ್ರತಿ(j = 0; j < l.classes; ++j){
                ಪೂರ್ಣಾಙ್ಕ class_index = entry_index(l, 0, n*l.w*l.h + i, 4 + 1 + j);
                ಭಿನ್ನ prob = objectness*predictions[class_index];
                dets[ಎಣಿಕೆ].prob[j] = (prob > thresh) ? prob : 0;
            }
            ++ಎಣಿಕೆ;
        }
    }
    correct_yolo_boxes(dets, ಎಣಿಕೆ, w, h, netw, neth, relative);
    ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ;
}

#ifdef GPU

ಶೂನ್ಯ forward_yolo_layer_gpu(ಧ್ರುವ layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.ಒಳಹರಿವು_gpu, 1, l.output_gpu, 1);
    ಪೂರ್ಣಾಙ್ಕ b, n;
    ಪ್ರತಿ (b = 0; b < l.batch; ++b){
        ಪ್ರತಿ(n = 0; n < l.n; ++n){
            ಪೂರ್ಣಾಙ್ಕ index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array_gpu(l.output_gpu + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
    ಯದಿ(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        ಪ್ರತ್ಯಾಗಮ;
    }

    cuda_pull_array(l.output_gpu, net.ಒಳಹರಿವು, l.batch*l.inputs);
    forward_yolo_layer(l, net);
    cuda_ತಳ್ಳು_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

ಶೂನ್ಯ backward_yolo_layer_gpu(ಧ್ರುವ layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


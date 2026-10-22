#ଅନ୍ତର୍ଭାଵ "yolo_layer.h"
#ଅନ୍ତର୍ଭାଵ "activations.h"
#ଅନ୍ତର୍ଭାଵ "blas.h"
#ଅନ୍ତର୍ଭାଵ "box.h"
#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"

#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>

layer make_yolo_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା total, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
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
    l.cost = calloc(1, ଆକାର(ଦଶମିକ));
    l.biases = calloc(total*2, ଆକାର(ଦଶମିକ));
    ଯଦି(mask) l.mask = mask;
    ଅନ୍ୟଥା{
        l.mask = calloc(n, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
        ପାଇଁ(i = 0; i < n; ++i){
            l.mask[i] = i;
        }
    }
    l.bias_updates = calloc(n*2, ଆକାର(ଦଶମିକ));
    l.outputs = h*w*n*(classes + 4 + 1);
    l.inputs = l.outputs;
    l.truths = 90*(4 + 1);
    l.delta = calloc(batch*l.outputs, ଆକାର(ଦଶମିକ));
    l.output = calloc(batch*l.outputs, ଆକାର(ଦଶମିକ));
    ପାଇଁ(i = 0; i < total*2; ++i){
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

    ଫେରନ୍ତୁ l;
}

ଶୂନ୍ୟ resize_yolo_layer(layer *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + 4 + 1);
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*ଆକାର(ଦଶମିକ));
    l->delta = realloc(l->delta, l->batch*l->outputs*ଆକାର(ଦଶମିକ));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

box get_yolo_box(ଦଶମିକ *x, ଦଶମିକ *biases, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା index, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ପୂର୍ଣ୍ଣସଂଖ୍ୟା j, ପୂର୍ଣ୍ଣସଂଖ୍ୟା lw, ପୂର୍ଣ୍ଣସଂଖ୍ୟା lh, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride)
{
    box b;
    b.x = (i + x[index + 0*stride]) / lw;
    b.y = (j + x[index + 1*stride]) / lh;
    b.w = exp(x[index + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[index + 3*stride]) * biases[2*n+1] / h;
    ଫେରନ୍ତୁ b;
}

ଦଶମିକ delta_yolo_box(box truth, ଦଶମିକ *x, ଦଶମିକ *biases, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା index, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ପୂର୍ଣ୍ଣସଂଖ୍ୟା j, ପୂର୍ଣ୍ଣସଂଖ୍ୟା lw, ପୂର୍ଣ୍ଣସଂଖ୍ୟା lh, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ଦଶମିକ *delta, ଦଶମିକ scale, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride)
{
    box pred = get_yolo_box(x, biases, n, index, i, j, lw, lh, w, h, stride);
    ଦଶମିକ iou = box_iou(pred, truth);

    ଦଶମିକ tx = (truth.x*lw - i);
    ଦଶମିକ ty = (truth.y*lh - j);
    ଦଶମିକ tw = log(truth.w*w / biases[2*n]);
    ଦଶମିକ th = log(truth.h*h / biases[2*n + 1]);

    delta[index + 0*stride] = scale * (tx - x[index + 0*stride]);
    delta[index + 1*stride] = scale * (ty - x[index + 1*stride]);
    delta[index + 2*stride] = scale * (tw - x[index + 2*stride]);
    delta[index + 3*stride] = scale * (th - x[index + 3*stride]);
    ଫେରନ୍ତୁ iou;
}


ଶୂନ୍ୟ delta_yolo_class(ଦଶମିକ *output, ଦଶମିକ *delta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା index, ପୂର୍ଣ୍ଣସଂଖ୍ୟା class, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ଦଶମିକ *avg_cat)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n;
    ଯଦି (delta[index]){
        delta[index + stride*class] = 1 - output[index + stride*class];
        ଯଦି(avg_cat) *avg_cat += output[index + stride*class];
        ଫେରନ୍ତୁ;
    }
    ପାଇଁ(n = 0; n < classes; ++n){
        delta[index + stride*n] = ((n == class)?1 : 0) - output[index + stride*n];
        ଯଦି(n == class && avg_cat) *avg_cat += output[index + stride*n];
    }
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା entry_index(layer l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା location, ପୂର୍ଣ୍ଣସଂଖ୍ୟା entry)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n =   location / (l.w*l.h);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା loc = location % (l.w*l.h);
    ଫେରନ୍ତୁ batch*l.outputs + n*l.w*l.h*(4+l.classes+1) + entry*l.w*l.h + loc;
}

ଶୂନ୍ୟ forward_yolo_layer(ଅଚଳ layer l, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,b,t,n;
    memcpy(l.output, net.ନିବେଶ, l.outputs*l.batch*ଆକାର(ଦଶମିକ));

#ifndef GPU
    ପାଇଁ (b = 0; b < l.batch; ++b){
        ପାଇଁ(n = 0; n < l.n; ++n){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array(l.output + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array(l.output + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
#endif

    memset(l.delta, 0, l.outputs * l.batch * ଆକାର(ଦଶମିକ));
    ଯଦି(!net.train) ଫେରନ୍ତୁ;
    ଦଶମିକ avg_iou = 0;
    ଦଶମିକ recall = 0;
    ଦଶମିକ recall75 = 0;
    ଦଶମିକ avg_cat = 0;
    ଦଶମିକ avg_obj = 0;
    ଦଶମିକ avg_anyobj = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା class_ଗଣନା = 0;
    *(l.cost) = 0;
    ପାଇଁ (b = 0; b < l.batch; ++b) {
        ପାଇଁ (j = 0; j < l.h; ++j) {
            ପାଇଁ (i = 0; i < l.w; ++i) {
                ପାଇଁ (n = 0; n < l.n; ++n) {
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_yolo_box(l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.w*l.h);
                    ଦଶମିକ best_iou = 0;
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା best_t = 0;
                    ପାଇଁ(t = 0; t < l.max_boxes; ++t){
                        box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);
                        ଯଦି(!truth.x) ଭାଙ୍ଗ;
                        ଦଶମିକ iou = box_iou(pred, truth);
                        ଯଦି (iou > best_iou) {
                            best_iou = iou;
                            best_t = t;
                        }
                    }
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା obj_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4);
                    avg_anyobj += l.output[obj_index];
                    l.delta[obj_index] = 0 - l.output[obj_index];
                    ଯଦି (best_iou > l.ignore_thresh) {
                        l.delta[obj_index] = 0;
                    }
                    ଯଦି (best_iou > l.truth_thresh) {
                        l.delta[obj_index] = 1 - l.output[obj_index];

                        ପୂର୍ଣ୍ଣସଂଖ୍ୟା class = net.truth[best_t*(4 + 1) + b*l.truths + 4];
                        ଯଦି (l.map) class = l.map[class];
                        ପୂର୍ଣ୍ଣସଂଖ୍ୟା class_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4 + 1);
                        delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, 0);
                        box truth = float_to_box(net.truth + best_t*(4 + 1) + b*l.truths, 1);
                        delta_yolo_box(truth, l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);
                    }
                }
            }
        }
        ପାଇଁ(t = 0; t < l.max_boxes; ++t){
            box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);

            ଯଦି(!truth.x) ଭାଙ୍ଗ;
            ଦଶମିକ best_iou = 0;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = truth_shift.y = 0;
            ପାଇଁ(n = 0; n < l.total; ++n){
                box pred = {0};
                pred.w = l.biases[2*n]/net.w;
                pred.h = l.biases[2*n+1]/net.h;
                ଦଶମିକ iou = box_iou(pred, truth_shift);
                ଯଦି (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            ପୂର୍ଣ୍ଣସଂଖ୍ୟା mask_n = int_index(l.mask, best_n, l.n);
            ଯଦି(mask_n >= 0){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା box_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 0);
                ଦଶମିକ iou = delta_yolo_box(truth, l.output, l.biases, best_n, box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);

                ପୂର୍ଣ୍ଣସଂଖ୍ୟା obj_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4);
                avg_obj += l.output[obj_index];
                l.delta[obj_index] = 1 - l.output[obj_index];

                ପୂର୍ଣ୍ଣସଂଖ୍ୟା class = net.truth[t*(4 + 1) + b*l.truths + 4];
                ଯଦି (l.map) class = l.map[class];
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା class_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4 + 1);
                delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, &avg_cat);

                ++ଗଣନା_2;
                ++class_ଗଣନା;
                ଯଦି(iou > .5) recall += 1;
                ଯଦି(iou > .75) recall75 += 1;
                avg_iou += iou;
            }
        }
    }
    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("Region %d Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, .5R: %f, .75R: %f,  count: %d\n", net.index, avg_iou/ଗଣନା_2, avg_cat/class_ଗଣନା, avg_obj/ଗଣନା_2, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/ଗଣନା_2, recall75/ଗଣନା_2, ଗଣନା_2);
}

ଶୂନ୍ୟ backward_yolo_layer(ଅଚଳ layer l, network net)
{
   axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

ଶୂନ୍ୟ correct_yolo_boxes(detection *dets, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା netw, ପୂର୍ଣ୍ଣସଂଖ୍ୟା neth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା relative)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_w=0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_h=0;
    ଯଦି (((ଦଶମିକ)netw/w) < ((ଦଶମିକ)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } ଅନ୍ୟଥା {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    ପାଇଁ (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((ଦଶମିକ)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((ଦଶମିକ)new_h/neth); 
        b.w *= (ଦଶମିକ)netw/new_w;
        b.h *= (ଦଶମିକ)neth/new_h;
        ଯଦି(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା yolo_ଅଙ୍କ_detections(layer l, ଦଶମିକ thresh)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, n;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    ପାଇଁ (i = 0; i < l.w*l.h; ++i){
        ପାଇଁ(n = 0; n < l.n; ++n){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            ଯଦି(l.output[obj_index] > thresh){
                ++ଗଣନା_2;
            }
        }
    }
    ଫେରନ୍ତୁ ଗଣନା_2;
}

ଶୂନ୍ୟ avg_flipped_yolo(layer l)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,n,z;
    ଦଶମିକ *flip = l.output + l.outputs;
    ପାଇଁ (j = 0; j < l.h; ++j) {
        ପାଇଁ (i = 0; i < l.w/2; ++i) {
            ପାଇଁ (n = 0; n < l.n; ++n) {
                ପାଇଁ(z = 0; z < l.classes + 4 + 1; ++z){
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                    ଦଶମିକ swap = flip[i1];
                    flip[i1] = flip[i2];
                    flip[i2] = swap;
                    ଯଦି(z == 0){
                        flip[i1] = -flip[i1];
                        flip[i2] = -flip[i2];
                    }
                }
            }
        }
    }
    ପାଇଁ(i = 0; i < l.outputs; ++i){
        l.output[i] = (l.output[i] + flip[i])/2.;
    }
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_yolo_detections(layer l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା netw, ପୂର୍ଣ୍ଣସଂଖ୍ୟା neth, ଦଶମିକ thresh, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *map, ପୂର୍ଣ୍ଣସଂଖ୍ୟା relative, detection *dets)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,n;
    ଦଶମିକ *predictions = l.output;
    ଯଦି (l.batch == 2) avg_flipped_yolo(l);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    ପାଇଁ (i = 0; i < l.w*l.h; ++i){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା row = i / l.w;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା col = i % l.w;
        ପାଇଁ(n = 0; n < l.n; ++n){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            ଦଶମିକ objectness = predictions[obj_index];
            ଯଦି(objectness <= thresh) ଜାରି;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା box_index  = entry_index(l, 0, n*l.w*l.h + i, 0);
            dets[ଗଣନା_2].bbox = get_yolo_box(predictions, l.biases, l.mask[n], box_index, col, row, l.w, l.h, netw, neth, l.w*l.h);
            dets[ଗଣନା_2].objectness = objectness;
            dets[ଗଣନା_2].classes = l.classes;
            ପାଇଁ(j = 0; j < l.classes; ++j){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା class_index = entry_index(l, 0, n*l.w*l.h + i, 4 + 1 + j);
                ଦଶମିକ prob = objectness*predictions[class_index];
                dets[ଗଣନା_2].prob[j] = (prob > thresh) ? prob : 0;
            }
            ++ଗଣନା_2;
        }
    }
    correct_yolo_boxes(dets, ଗଣନା_2, w, h, netw, neth, relative);
    ଫେରନ୍ତୁ ଗଣନା_2;
}

#ifdef GPU

ଶୂନ୍ୟ forward_yolo_layer_gpu(ଅଚଳ layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.ନିବେଶ_gpu, 1, l.output_gpu, 1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା b, n;
    ପାଇଁ (b = 0; b < l.batch; ++b){
        ପାଇଁ(n = 0; n < l.n; ++n){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array_gpu(l.output_gpu + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
    ଯଦି(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        ଫେରନ୍ତୁ;
    }

    cuda_pull_array(l.output_gpu, net.ନିବେଶ, l.batch*l.inputs);
    forward_yolo_layer(l, net);
    cuda_ଠେଲ_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

ଶୂନ୍ୟ backward_yolo_layer_gpu(ଅଚଳ layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


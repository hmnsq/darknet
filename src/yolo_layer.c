#அந்தர்பாவ "yolo_layer.h"
#அந்தர்பாவ "activations.h"
#அந்தர்பாவ "blas.h"
#அந்தர்பாவ "box.h"
#அந்தர்பாவ "cuda.h"
#அந்தர்பாவ "utils.h"

#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <stdlib.h>

layer make_yolo_layer(முழு batch, முழு w, முழு h, முழு n, முழு total, முழு *mask, முழு classes)
{
    முழு i;
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
    l.cost = calloc(1, அளவு(தசம));
    l.biases = calloc(total*2, அளவு(தசம));
    எனில்(mask) l.mask = mask;
    இல்லையெனில்{
        l.mask = calloc(n, அளவு(முழு));
        க்கு(i = 0; i < n; ++i){
            l.mask[i] = i;
        }
    }
    l.bias_updates = calloc(n*2, அளவு(தசம));
    l.outputs = h*w*n*(classes + 4 + 1);
    l.inputs = l.outputs;
    l.truths = 90*(4 + 1);
    l.delta = calloc(batch*l.outputs, அளவு(தசம));
    l.output = calloc(batch*l.outputs, அளவு(தசம));
    க்கு(i = 0; i < total*2; ++i){
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

    திரும்பு l;
}

வெறுமை resize_yolo_layer(layer *l, முழு w, முழு h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + 4 + 1);
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

box get_yolo_box(தசம *x, தசம *biases, முழு n, முழு index, முழு i, முழு j, முழு lw, முழு lh, முழு w, முழு h, முழு stride)
{
    box b;
    b.x = (i + x[index + 0*stride]) / lw;
    b.y = (j + x[index + 1*stride]) / lh;
    b.w = exp(x[index + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[index + 3*stride]) * biases[2*n+1] / h;
    திரும்பு b;
}

தசம delta_yolo_box(box truth, தசம *x, தசம *biases, முழு n, முழு index, முழு i, முழு j, முழு lw, முழு lh, முழு w, முழு h, தசம *delta, தசம scale, முழு stride)
{
    box pred = get_yolo_box(x, biases, n, index, i, j, lw, lh, w, h, stride);
    தசம iou = box_iou(pred, truth);

    தசம tx = (truth.x*lw - i);
    தசம ty = (truth.y*lh - j);
    தசம tw = log(truth.w*w / biases[2*n]);
    தசம th = log(truth.h*h / biases[2*n + 1]);

    delta[index + 0*stride] = scale * (tx - x[index + 0*stride]);
    delta[index + 1*stride] = scale * (ty - x[index + 1*stride]);
    delta[index + 2*stride] = scale * (tw - x[index + 2*stride]);
    delta[index + 3*stride] = scale * (th - x[index + 3*stride]);
    திரும்பு iou;
}


வெறுமை delta_yolo_class(தசம *output, தசம *delta, முழு index, முழு class, முழு classes, முழு stride, தசம *avg_cat)
{
    முழு n;
    எனில் (delta[index]){
        delta[index + stride*class] = 1 - output[index + stride*class];
        எனில்(avg_cat) *avg_cat += output[index + stride*class];
        திரும்பு;
    }
    க்கு(n = 0; n < classes; ++n){
        delta[index + stride*n] = ((n == class)?1 : 0) - output[index + stride*n];
        எனில்(n == class && avg_cat) *avg_cat += output[index + stride*n];
    }
}

நிலையான முழு entry_index(layer l, முழு batch, முழு location, முழு entry)
{
    முழு n =   location / (l.w*l.h);
    முழு loc = location % (l.w*l.h);
    திரும்பு batch*l.outputs + n*l.w*l.h*(4+l.classes+1) + entry*l.w*l.h + loc;
}

வெறுமை forward_yolo_layer(மாறிலி layer l, network net)
{
    முழு i,j,b,t,n;
    memcpy(l.output, net.உள்ளீடு, l.outputs*l.batch*அளவு(தசம));

#ifndef GPU
    க்கு (b = 0; b < l.batch; ++b){
        க்கு(n = 0; n < l.n; ++n){
            முழு index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array(l.output + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array(l.output + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
#endif

    memset(l.delta, 0, l.outputs * l.batch * அளவு(தசம));
    எனில்(!net.train) திரும்பு;
    தசம avg_iou = 0;
    தசம recall = 0;
    தசம recall75 = 0;
    தசம avg_cat = 0;
    தசம avg_obj = 0;
    தசம avg_anyobj = 0;
    முழு மொத்தம் = 0;
    முழு class_மொத்தம் = 0;
    *(l.cost) = 0;
    க்கு (b = 0; b < l.batch; ++b) {
        க்கு (j = 0; j < l.h; ++j) {
            க்கு (i = 0; i < l.w; ++i) {
                க்கு (n = 0; n < l.n; ++n) {
                    முழு box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_yolo_box(l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.w*l.h);
                    தசம best_iou = 0;
                    முழு best_t = 0;
                    க்கு(t = 0; t < l.max_boxes; ++t){
                        box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);
                        எனில்(!truth.x) நிறுத்து;
                        தசம iou = box_iou(pred, truth);
                        எனில் (iou > best_iou) {
                            best_iou = iou;
                            best_t = t;
                        }
                    }
                    முழு obj_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4);
                    avg_anyobj += l.output[obj_index];
                    l.delta[obj_index] = 0 - l.output[obj_index];
                    எனில் (best_iou > l.ignore_thresh) {
                        l.delta[obj_index] = 0;
                    }
                    எனில் (best_iou > l.truth_thresh) {
                        l.delta[obj_index] = 1 - l.output[obj_index];

                        முழு class = net.truth[best_t*(4 + 1) + b*l.truths + 4];
                        எனில் (l.map) class = l.map[class];
                        முழு class_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4 + 1);
                        delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, 0);
                        box truth = float_to_box(net.truth + best_t*(4 + 1) + b*l.truths, 1);
                        delta_yolo_box(truth, l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);
                    }
                }
            }
        }
        க்கு(t = 0; t < l.max_boxes; ++t){
            box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);

            எனில்(!truth.x) நிறுத்து;
            தசம best_iou = 0;
            முழு best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = truth_shift.y = 0;
            க்கு(n = 0; n < l.total; ++n){
                box pred = {0};
                pred.w = l.biases[2*n]/net.w;
                pred.h = l.biases[2*n+1]/net.h;
                தசம iou = box_iou(pred, truth_shift);
                எனில் (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            முழு mask_n = int_index(l.mask, best_n, l.n);
            எனில்(mask_n >= 0){
                முழு box_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 0);
                தசம iou = delta_yolo_box(truth, l.output, l.biases, best_n, box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);

                முழு obj_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4);
                avg_obj += l.output[obj_index];
                l.delta[obj_index] = 1 - l.output[obj_index];

                முழு class = net.truth[t*(4 + 1) + b*l.truths + 4];
                எனில் (l.map) class = l.map[class];
                முழு class_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4 + 1);
                delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, &avg_cat);

                ++மொத்தம்;
                ++class_மொத்தம்;
                எனில்(iou > .5) recall += 1;
                எனில்(iou > .75) recall75 += 1;
                avg_iou += iou;
            }
        }
    }
    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("Region %d Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, .5R: %f, .75R: %f,  count: %d\n", net.index, avg_iou/மொத்தம், avg_cat/class_மொத்தம், avg_obj/மொத்தம், avg_anyobj/(l.w*l.h*l.n*l.batch), recall/மொத்தம், recall75/மொத்தம், மொத்தம்);
}

வெறுமை backward_yolo_layer(மாறிலி layer l, network net)
{
   axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

வெறுமை correct_yolo_boxes(detection *dets, முழு n, முழு w, முழு h, முழு netw, முழு neth, முழு relative)
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

முழு yolo_இலக்கம்_detections(layer l, தசம thresh)
{
    முழு i, n;
    முழு மொத்தம் = 0;
    க்கு (i = 0; i < l.w*l.h; ++i){
        க்கு(n = 0; n < l.n; ++n){
            முழு obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            எனில்(l.output[obj_index] > thresh){
                ++மொத்தம்;
            }
        }
    }
    திரும்பு மொத்தம்;
}

வெறுமை avg_flipped_yolo(layer l)
{
    முழு i,j,n,z;
    தசம *flip = l.output + l.outputs;
    க்கு (j = 0; j < l.h; ++j) {
        க்கு (i = 0; i < l.w/2; ++i) {
            க்கு (n = 0; n < l.n; ++n) {
                க்கு(z = 0; z < l.classes + 4 + 1; ++z){
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

முழு get_yolo_detections(layer l, முழு w, முழு h, முழு netw, முழு neth, தசம thresh, முழு *map, முழு relative, detection *dets)
{
    முழு i,j,n;
    தசம *predictions = l.output;
    எனில் (l.batch == 2) avg_flipped_yolo(l);
    முழு மொத்தம் = 0;
    க்கு (i = 0; i < l.w*l.h; ++i){
        முழு row = i / l.w;
        முழு col = i % l.w;
        க்கு(n = 0; n < l.n; ++n){
            முழு obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            தசம objectness = predictions[obj_index];
            எனில்(objectness <= thresh) தொடர்;
            முழு box_index  = entry_index(l, 0, n*l.w*l.h + i, 0);
            dets[மொத்தம்].bbox = get_yolo_box(predictions, l.biases, l.mask[n], box_index, col, row, l.w, l.h, netw, neth, l.w*l.h);
            dets[மொத்தம்].objectness = objectness;
            dets[மொத்தம்].classes = l.classes;
            க்கு(j = 0; j < l.classes; ++j){
                முழு class_index = entry_index(l, 0, n*l.w*l.h + i, 4 + 1 + j);
                தசம prob = objectness*predictions[class_index];
                dets[மொத்தம்].prob[j] = (prob > thresh) ? prob : 0;
            }
            ++மொத்தம்;
        }
    }
    correct_yolo_boxes(dets, மொத்தம், w, h, netw, neth, relative);
    திரும்பு மொத்தம்;
}

#ifdef GPU

வெறுமை forward_yolo_layer_gpu(மாறிலி layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.உள்ளீடு_gpu, 1, l.output_gpu, 1);
    முழு b, n;
    க்கு (b = 0; b < l.batch; ++b){
        க்கு(n = 0; n < l.n; ++n){
            முழு index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array_gpu(l.output_gpu + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
    எனில்(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        திரும்பு;
    }

    cuda_pull_array(l.output_gpu, net.உள்ளீடு, l.batch*l.inputs);
    forward_yolo_layer(l, net);
    cuda_தள்ளு_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

வெறுமை backward_yolo_layer_gpu(மாறிலி layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


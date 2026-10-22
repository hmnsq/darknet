#অন্তর্ভাব "yolo_layer.h"
#অন্তর্ভাব "activations.h"
#অন্তর্ভাব "blas.h"
#অন্তর্ভাব "box.h"
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব "utils.h"

#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <stdlib.h>

layer make_yolo_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা n, পূর্ণসংখ্যা যোগ_2, পূর্ণসংখ্যা *mask, পূর্ণসংখ্যা classes)
{
    পূর্ণসংখ্যা i;
    layer l = {0};
    l.type = YOLO;

    l.n = n;
    l.যোগ_2 = যোগ_2;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = n*(classes + 4 + 1);
    l.out_w = l.w;
    l.out_h = l.h;
    l.out_c = l.c;
    l.classes = classes;
    l.cost = calloc(1, আকার(দশমিক));
    l.biases = calloc(যোগ_2*2, আকার(দশমিক));
    যদি(mask) l.mask = mask;
    অন্যথা{
        l.mask = calloc(n, আকার(পূর্ণসংখ্যা));
        জন্য(i = 0; i < n; ++i){
            l.mask[i] = i;
        }
    }
    l.bias_updates = calloc(n*2, আকার(দশমিক));
    l.outputs = h*w*n*(classes + 4 + 1);
    l.inputs = l.outputs;
    l.truths = 90*(4 + 1);
    l.delta = calloc(batch*l.outputs, আকার(দশমিক));
    l.output = calloc(batch*l.outputs, আকার(দশমিক));
    জন্য(i = 0; i < যোগ_2*2; ++i){
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

    ফেরত l;
}

শূন্য resize_yolo_layer(layer *l, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + 4 + 1);
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*আকার(দশমিক));
    l->delta = realloc(l->delta, l->batch*l->outputs*আকার(দশমিক));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

box get_yolo_box(দশমিক *x, দশমিক *biases, পূর্ণসংখ্যা n, পূর্ণসংখ্যা সূচক, পূর্ণসংখ্যা i, পূর্ণসংখ্যা j, পূর্ণসংখ্যা lw, পূর্ণসংখ্যা lh, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা stride)
{
    box b;
    b.x = (i + x[সূচক + 0*stride]) / lw;
    b.y = (j + x[সূচক + 1*stride]) / lh;
    b.w = exp(x[সূচক + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[সূচক + 3*stride]) * biases[2*n+1] / h;
    ফেরত b;
}

দশমিক delta_yolo_box(box truth, দশমিক *x, দশমিক *biases, পূর্ণসংখ্যা n, পূর্ণসংখ্যা সূচক, পূর্ণসংখ্যা i, পূর্ণসংখ্যা j, পূর্ণসংখ্যা lw, পূর্ণসংখ্যা lh, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, দশমিক *delta, দশমিক scale, পূর্ণসংখ্যা stride)
{
    box pred = get_yolo_box(x, biases, n, সূচক, i, j, lw, lh, w, h, stride);
    দশমিক iou = box_iou(pred, truth);

    দশমিক tx = (truth.x*lw - i);
    দশমিক ty = (truth.y*lh - j);
    দশমিক tw = log(truth.w*w / biases[2*n]);
    দশমিক th = log(truth.h*h / biases[2*n + 1]);

    delta[সূচক + 0*stride] = scale * (tx - x[সূচক + 0*stride]);
    delta[সূচক + 1*stride] = scale * (ty - x[সূচক + 1*stride]);
    delta[সূচক + 2*stride] = scale * (tw - x[সূচক + 2*stride]);
    delta[সূচক + 3*stride] = scale * (th - x[সূচক + 3*stride]);
    ফেরত iou;
}


শূন্য delta_yolo_class(দশমিক *output, দশমিক *delta, পূর্ণসংখ্যা সূচক, পূর্ণসংখ্যা class, পূর্ণসংখ্যা classes, পূর্ণসংখ্যা stride, দশমিক *avg_cat)
{
    পূর্ণসংখ্যা n;
    যদি (delta[সূচক]){
        delta[সূচক + stride*class] = 1 - output[সূচক + stride*class];
        যদি(avg_cat) *avg_cat += output[সূচক + stride*class];
        ফেরত;
    }
    জন্য(n = 0; n < classes; ++n){
        delta[সূচক + stride*n] = ((n == class)?1 : 0) - output[সূচক + stride*n];
        যদি(n == class && avg_cat) *avg_cat += output[সূচক + stride*n];
    }
}

স্থির পূর্ণসংখ্যা entry_সূচক(layer l, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা location, পূর্ণসংখ্যা entry)
{
    পূর্ণসংখ্যা n =   location / (l.w*l.h);
    পূর্ণসংখ্যা loc = location % (l.w*l.h);
    ফেরত batch*l.outputs + n*l.w*l.h*(4+l.classes+1) + entry*l.w*l.h + loc;
}

শূন্য forward_yolo_layer(ধ্রুবক layer l, network net)
{
    পূর্ণসংখ্যা i,j,b,t,n;
    memcpy(l.output, net.প্রবেশ, l.outputs*l.batch*আকার(দশমিক));

#ifndef GPU
    জন্য (b = 0; b < l.batch; ++b){
        জন্য(n = 0; n < l.n; ++n){
            পূর্ণসংখ্যা সূচক = entry_সূচক(l, b, n*l.w*l.h, 0);
            activate_array(l.output + সূচক, 2*l.w*l.h, LOGISTIC);
            সূচক = entry_সূচক(l, b, n*l.w*l.h, 4);
            activate_array(l.output + সূচক, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
#endif

    memset(l.delta, 0, l.outputs * l.batch * আকার(দশমিক));
    যদি(!net.train) ফেরত;
    দশমিক avg_iou = 0;
    দশমিক recall = 0;
    দশমিক recall75 = 0;
    দশমিক avg_cat = 0;
    দশমিক avg_obj = 0;
    দশমিক avg_anyobj = 0;
    পূর্ণসংখ্যা গুনতি = 0;
    পূর্ণসংখ্যা class_গুনতি = 0;
    *(l.cost) = 0;
    জন্য (b = 0; b < l.batch; ++b) {
        জন্য (j = 0; j < l.h; ++j) {
            জন্য (i = 0; i < l.w; ++i) {
                জন্য (n = 0; n < l.n; ++n) {
                    পূর্ণসংখ্যা box_সূচক = entry_সূচক(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_yolo_box(l.output, l.biases, l.mask[n], box_সূচক, i, j, l.w, l.h, net.w, net.h, l.w*l.h);
                    দশমিক best_iou = 0;
                    পূর্ণসংখ্যা best_t = 0;
                    জন্য(t = 0; t < l.max_boxes; ++t){
                        box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);
                        যদি(!truth.x) বিরতি;
                        দশমিক iou = box_iou(pred, truth);
                        যদি (iou > best_iou) {
                            best_iou = iou;
                            best_t = t;
                        }
                    }
                    পূর্ণসংখ্যা obj_সূচক = entry_সূচক(l, b, n*l.w*l.h + j*l.w + i, 4);
                    avg_anyobj += l.output[obj_সূচক];
                    l.delta[obj_সূচক] = 0 - l.output[obj_সূচক];
                    যদি (best_iou > l.ignore_thresh) {
                        l.delta[obj_সূচক] = 0;
                    }
                    যদি (best_iou > l.truth_thresh) {
                        l.delta[obj_সূচক] = 1 - l.output[obj_সূচক];

                        পূর্ণসংখ্যা class = net.truth[best_t*(4 + 1) + b*l.truths + 4];
                        যদি (l.map) class = l.map[class];
                        পূর্ণসংখ্যা class_সূচক = entry_সূচক(l, b, n*l.w*l.h + j*l.w + i, 4 + 1);
                        delta_yolo_class(l.output, l.delta, class_সূচক, class, l.classes, l.w*l.h, 0);
                        box truth = float_to_box(net.truth + best_t*(4 + 1) + b*l.truths, 1);
                        delta_yolo_box(truth, l.output, l.biases, l.mask[n], box_সূচক, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);
                    }
                }
            }
        }
        জন্য(t = 0; t < l.max_boxes; ++t){
            box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);

            যদি(!truth.x) বিরতি;
            দশমিক best_iou = 0;
            পূর্ণসংখ্যা best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = truth_shift.y = 0;
            জন্য(n = 0; n < l.যোগ_2; ++n){
                box pred = {0};
                pred.w = l.biases[2*n]/net.w;
                pred.h = l.biases[2*n+1]/net.h;
                দশমিক iou = box_iou(pred, truth_shift);
                যদি (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            পূর্ণসংখ্যা mask_n = int_সূচক(l.mask, best_n, l.n);
            যদি(mask_n >= 0){
                পূর্ণসংখ্যা box_সূচক = entry_সূচক(l, b, mask_n*l.w*l.h + j*l.w + i, 0);
                দশমিক iou = delta_yolo_box(truth, l.output, l.biases, best_n, box_সূচক, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);

                পূর্ণসংখ্যা obj_সূচক = entry_সূচক(l, b, mask_n*l.w*l.h + j*l.w + i, 4);
                avg_obj += l.output[obj_সূচক];
                l.delta[obj_সূচক] = 1 - l.output[obj_সূচক];

                পূর্ণসংখ্যা class = net.truth[t*(4 + 1) + b*l.truths + 4];
                যদি (l.map) class = l.map[class];
                পূর্ণসংখ্যা class_সূচক = entry_সূচক(l, b, mask_n*l.w*l.h + j*l.w + i, 4 + 1);
                delta_yolo_class(l.output, l.delta, class_সূচক, class, l.classes, l.w*l.h, &avg_cat);

                ++গুনতি;
                ++class_গুনতি;
                যদি(iou > .5) recall += 1;
                যদি(iou > .75) recall75 += 1;
                avg_iou += iou;
            }
        }
    }
    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("Region %d Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, .5R: %f, .75R: %f,  count: %d\n", net.সূচক, avg_iou/গুনতি, avg_cat/class_গুনতি, avg_obj/গুনতি, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/গুনতি, recall75/গুনতি, গুনতি);
}

শূন্য backward_yolo_layer(ধ্রুবক layer l, network net)
{
   axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

শূন্য correct_yolo_boxes(detection *dets, পূর্ণসংখ্যা n, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা netw, পূর্ণসংখ্যা neth, পূর্ণসংখ্যা relative)
{
    পূর্ণসংখ্যা i;
    পূর্ণসংখ্যা new_w=0;
    পূর্ণসংখ্যা new_h=0;
    যদি (((দশমিক)netw/w) < ((দশমিক)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } অন্যথা {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    জন্য (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((দশমিক)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((দশমিক)new_h/neth); 
        b.w *= (দশমিক)netw/new_w;
        b.h *= (দশমিক)neth/new_h;
        যদি(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

পূর্ণসংখ্যা yolo_অঙ্ক_detections(layer l, দশমিক thresh)
{
    পূর্ণসংখ্যা i, n;
    পূর্ণসংখ্যা গুনতি = 0;
    জন্য (i = 0; i < l.w*l.h; ++i){
        জন্য(n = 0; n < l.n; ++n){
            পূর্ণসংখ্যা obj_সূচক  = entry_সূচক(l, 0, n*l.w*l.h + i, 4);
            যদি(l.output[obj_সূচক] > thresh){
                ++গুনতি;
            }
        }
    }
    ফেরত গুনতি;
}

শূন্য avg_flipped_yolo(layer l)
{
    পূর্ণসংখ্যা i,j,n,z;
    দশমিক *flip = l.output + l.outputs;
    জন্য (j = 0; j < l.h; ++j) {
        জন্য (i = 0; i < l.w/2; ++i) {
            জন্য (n = 0; n < l.n; ++n) {
                জন্য(z = 0; z < l.classes + 4 + 1; ++z){
                    পূর্ণসংখ্যা i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                    পূর্ণসংখ্যা i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                    দশমিক swap = flip[i1];
                    flip[i1] = flip[i2];
                    flip[i2] = swap;
                    যদি(z == 0){
                        flip[i1] = -flip[i1];
                        flip[i2] = -flip[i2];
                    }
                }
            }
        }
    }
    জন্য(i = 0; i < l.outputs; ++i){
        l.output[i] = (l.output[i] + flip[i])/2.;
    }
}

পূর্ণসংখ্যা get_yolo_detections(layer l, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা netw, পূর্ণসংখ্যা neth, দশমিক thresh, পূর্ণসংখ্যা *map, পূর্ণসংখ্যা relative, detection *dets)
{
    পূর্ণসংখ্যা i,j,n;
    দশমিক *predictions = l.output;
    যদি (l.batch == 2) avg_flipped_yolo(l);
    পূর্ণসংখ্যা গুনতি = 0;
    জন্য (i = 0; i < l.w*l.h; ++i){
        পূর্ণসংখ্যা row = i / l.w;
        পূর্ণসংখ্যা col = i % l.w;
        জন্য(n = 0; n < l.n; ++n){
            পূর্ণসংখ্যা obj_সূচক  = entry_সূচক(l, 0, n*l.w*l.h + i, 4);
            দশমিক objectness = predictions[obj_সূচক];
            যদি(objectness <= thresh) চালু;
            পূর্ণসংখ্যা box_সূচক  = entry_সূচক(l, 0, n*l.w*l.h + i, 0);
            dets[গুনতি].bbox = get_yolo_box(predictions, l.biases, l.mask[n], box_সূচক, col, row, l.w, l.h, netw, neth, l.w*l.h);
            dets[গুনতি].objectness = objectness;
            dets[গুনতি].classes = l.classes;
            জন্য(j = 0; j < l.classes; ++j){
                পূর্ণসংখ্যা class_সূচক = entry_সূচক(l, 0, n*l.w*l.h + i, 4 + 1 + j);
                দশমিক prob = objectness*predictions[class_সূচক];
                dets[গুনতি].prob[j] = (prob > thresh) ? prob : 0;
            }
            ++গুনতি;
        }
    }
    correct_yolo_boxes(dets, গুনতি, w, h, netw, neth, relative);
    ফেরত গুনতি;
}

#ifdef GPU

শূন্য forward_yolo_layer_gpu(ধ্রুবক layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.প্রবেশ_gpu, 1, l.output_gpu, 1);
    পূর্ণসংখ্যা b, n;
    জন্য (b = 0; b < l.batch; ++b){
        জন্য(n = 0; n < l.n; ++n){
            পূর্ণসংখ্যা সূচক = entry_সূচক(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + সূচক, 2*l.w*l.h, LOGISTIC);
            সূচক = entry_সূচক(l, b, n*l.w*l.h, 4);
            activate_array_gpu(l.output_gpu + সূচক, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
    যদি(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        ফেরত;
    }

    cuda_pull_array(l.output_gpu, net.প্রবেশ, l.batch*l.inputs);
    forward_yolo_layer(l, net);
    cuda_ঠেলো_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

শূন্য backward_yolo_layer_gpu(ধ্রুবক layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


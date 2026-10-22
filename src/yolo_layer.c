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

layer make_yolo_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক n, পূর্ণাঙ্ক total, পূর্ণাঙ্ক *mask, পূর্ণাঙ্ক classes)
{
    পূর্ণাঙ্ক i;
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
    l.cost = calloc(1, পরিমাণ(ভিন্ন));
    l.biases = calloc(total*2, পরিমাণ(ভিন্ন));
    যদি(mask) l.mask = mask;
    অন্যথা{
        l.mask = calloc(n, পরিমাণ(পূর্ণাঙ্ক));
        প্রতি(i = 0; i < n; ++i){
            l.mask[i] = i;
        }
    }
    l.bias_updates = calloc(n*2, পরিমাণ(ভিন্ন));
    l.outputs = h*w*n*(classes + 4 + 1);
    l.inputs = l.outputs;
    l.truths = 90*(4 + 1);
    l.delta = calloc(batch*l.outputs, পরিমাণ(ভিন্ন));
    l.output = calloc(batch*l.outputs, পরিমাণ(ভিন্ন));
    প্রতি(i = 0; i < total*2; ++i){
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

    প্রত্যাগম l;
}

শূন্য resize_yolo_layer(layer *l, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + 4 + 1);
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*পরিমাণ(ভিন্ন));
    l->delta = realloc(l->delta, l->batch*l->outputs*পরিমাণ(ভিন্ন));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

box get_yolo_box(ভিন্ন *x, ভিন্ন *biases, পূর্ণাঙ্ক n, পূর্ণাঙ্ক index, পূর্ণাঙ্ক i, পূর্ণাঙ্ক j, পূর্ণাঙ্ক lw, পূর্ণাঙ্ক lh, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক stride)
{
    box b;
    b.x = (i + x[index + 0*stride]) / lw;
    b.y = (j + x[index + 1*stride]) / lh;
    b.w = exp(x[index + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[index + 3*stride]) * biases[2*n+1] / h;
    প্রত্যাগম b;
}

ভিন্ন delta_yolo_box(box truth, ভিন্ন *x, ভিন্ন *biases, পূর্ণাঙ্ক n, পূর্ণাঙ্ক index, পূর্ণাঙ্ক i, পূর্ণাঙ্ক j, পূর্ণাঙ্ক lw, পূর্ণাঙ্ক lh, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, ভিন্ন *delta, ভিন্ন scale, পূর্ণাঙ্ক stride)
{
    box pred = get_yolo_box(x, biases, n, index, i, j, lw, lh, w, h, stride);
    ভিন্ন iou = box_iou(pred, truth);

    ভিন্ন tx = (truth.x*lw - i);
    ভিন্ন ty = (truth.y*lh - j);
    ভিন্ন tw = log(truth.w*w / biases[2*n]);
    ভিন্ন th = log(truth.h*h / biases[2*n + 1]);

    delta[index + 0*stride] = scale * (tx - x[index + 0*stride]);
    delta[index + 1*stride] = scale * (ty - x[index + 1*stride]);
    delta[index + 2*stride] = scale * (tw - x[index + 2*stride]);
    delta[index + 3*stride] = scale * (th - x[index + 3*stride]);
    প্রত্যাগম iou;
}


শূন্য delta_yolo_class(ভিন্ন *output, ভিন্ন *delta, পূর্ণাঙ্ক index, পূর্ণাঙ্ক class, পূর্ণাঙ্ক classes, পূর্ণাঙ্ক stride, ভিন্ন *avg_cat)
{
    পূর্ণাঙ্ক n;
    যদি (delta[index]){
        delta[index + stride*class] = 1 - output[index + stride*class];
        যদি(avg_cat) *avg_cat += output[index + stride*class];
        প্রত্যাগম;
    }
    প্রতি(n = 0; n < classes; ++n){
        delta[index + stride*n] = ((n == class)?1 : 0) - output[index + stride*n];
        যদি(n == class && avg_cat) *avg_cat += output[index + stride*n];
    }
}

স্থির পূর্ণাঙ্ক entry_index(layer l, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক location, পূর্ণাঙ্ক entry)
{
    পূর্ণাঙ্ক n =   location / (l.w*l.h);
    পূর্ণাঙ্ক loc = location % (l.w*l.h);
    প্রত্যাগম batch*l.outputs + n*l.w*l.h*(4+l.classes+1) + entry*l.w*l.h + loc;
}

শূন্য forward_yolo_layer(ধ্রুব layer l, network net)
{
    পূর্ণাঙ্ক i,j,b,t,n;
    memcpy(l.output, net.প্রবেশ, l.outputs*l.batch*পরিমাণ(ভিন্ন));

#ifndef GPU
    প্রতি (b = 0; b < l.batch; ++b){
        প্রতি(n = 0; n < l.n; ++n){
            পূর্ণাঙ্ক index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array(l.output + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array(l.output + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
#endif

    memset(l.delta, 0, l.outputs * l.batch * পরিমাণ(ভিন্ন));
    যদি(!net.train) প্রত্যাগম;
    ভিন্ন avg_iou = 0;
    ভিন্ন recall = 0;
    ভিন্ন recall75 = 0;
    ভিন্ন avg_cat = 0;
    ভিন্ন avg_obj = 0;
    ভিন্ন avg_anyobj = 0;
    পূর্ণাঙ্ক গুনতি = 0;
    পূর্ণাঙ্ক class_গুনতি = 0;
    *(l.cost) = 0;
    প্রতি (b = 0; b < l.batch; ++b) {
        প্রতি (j = 0; j < l.h; ++j) {
            প্রতি (i = 0; i < l.w; ++i) {
                প্রতি (n = 0; n < l.n; ++n) {
                    পূর্ণাঙ্ক box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_yolo_box(l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.w*l.h);
                    ভিন্ন best_iou = 0;
                    পূর্ণাঙ্ক best_t = 0;
                    প্রতি(t = 0; t < l.max_boxes; ++t){
                        box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);
                        যদি(!truth.x) ভঙ্গ;
                        ভিন্ন iou = box_iou(pred, truth);
                        যদি (iou > best_iou) {
                            best_iou = iou;
                            best_t = t;
                        }
                    }
                    পূর্ণাঙ্ক obj_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4);
                    avg_anyobj += l.output[obj_index];
                    l.delta[obj_index] = 0 - l.output[obj_index];
                    যদি (best_iou > l.ignore_thresh) {
                        l.delta[obj_index] = 0;
                    }
                    যদি (best_iou > l.truth_thresh) {
                        l.delta[obj_index] = 1 - l.output[obj_index];

                        পূর্ণাঙ্ক class = net.truth[best_t*(4 + 1) + b*l.truths + 4];
                        যদি (l.map) class = l.map[class];
                        পূর্ণাঙ্ক class_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4 + 1);
                        delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, 0);
                        box truth = float_to_box(net.truth + best_t*(4 + 1) + b*l.truths, 1);
                        delta_yolo_box(truth, l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);
                    }
                }
            }
        }
        প্রতি(t = 0; t < l.max_boxes; ++t){
            box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);

            যদি(!truth.x) ভঙ্গ;
            ভিন্ন best_iou = 0;
            পূর্ণাঙ্ক best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = truth_shift.y = 0;
            প্রতি(n = 0; n < l.total; ++n){
                box pred = {0};
                pred.w = l.biases[2*n]/net.w;
                pred.h = l.biases[2*n+1]/net.h;
                ভিন্ন iou = box_iou(pred, truth_shift);
                যদি (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            পূর্ণাঙ্ক mask_n = int_index(l.mask, best_n, l.n);
            যদি(mask_n >= 0){
                পূর্ণাঙ্ক box_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 0);
                ভিন্ন iou = delta_yolo_box(truth, l.output, l.biases, best_n, box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);

                পূর্ণাঙ্ক obj_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4);
                avg_obj += l.output[obj_index];
                l.delta[obj_index] = 1 - l.output[obj_index];

                পূর্ণাঙ্ক class = net.truth[t*(4 + 1) + b*l.truths + 4];
                যদি (l.map) class = l.map[class];
                পূর্ণাঙ্ক class_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4 + 1);
                delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, &avg_cat);

                ++গুনতি;
                ++class_গুনতি;
                যদি(iou > .5) recall += 1;
                যদি(iou > .75) recall75 += 1;
                avg_iou += iou;
            }
        }
    }
    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("Region %d Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, .5R: %f, .75R: %f,  count: %d\n", net.index, avg_iou/গুনতি, avg_cat/class_গুনতি, avg_obj/গুনতি, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/গুনতি, recall75/গুনতি, গুনতি);
}

শূন্য backward_yolo_layer(ধ্রুব layer l, network net)
{
   axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

শূন্য correct_yolo_boxes(detection *dets, পূর্ণাঙ্ক n, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক netw, পূর্ণাঙ্ক neth, পূর্ণাঙ্ক relative)
{
    পূর্ণাঙ্ক i;
    পূর্ণাঙ্ক new_w=0;
    পূর্ণাঙ্ক new_h=0;
    যদি (((ভিন্ন)netw/w) < ((ভিন্ন)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } অন্যথা {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    প্রতি (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((ভিন্ন)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((ভিন্ন)new_h/neth); 
        b.w *= (ভিন্ন)netw/new_w;
        b.h *= (ভিন্ন)neth/new_h;
        যদি(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

পূর্ণাঙ্ক yolo_অঙ্ক_detections(layer l, ভিন্ন thresh)
{
    পূর্ণাঙ্ক i, n;
    পূর্ণাঙ্ক গুনতি = 0;
    প্রতি (i = 0; i < l.w*l.h; ++i){
        প্রতি(n = 0; n < l.n; ++n){
            পূর্ণাঙ্ক obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            যদি(l.output[obj_index] > thresh){
                ++গুনতি;
            }
        }
    }
    প্রত্যাগম গুনতি;
}

শূন্য avg_flipped_yolo(layer l)
{
    পূর্ণাঙ্ক i,j,n,z;
    ভিন্ন *flip = l.output + l.outputs;
    প্রতি (j = 0; j < l.h; ++j) {
        প্রতি (i = 0; i < l.w/2; ++i) {
            প্রতি (n = 0; n < l.n; ++n) {
                প্রতি(z = 0; z < l.classes + 4 + 1; ++z){
                    পূর্ণাঙ্ক i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                    পূর্ণাঙ্ক i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                    ভিন্ন swap = flip[i1];
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
    প্রতি(i = 0; i < l.outputs; ++i){
        l.output[i] = (l.output[i] + flip[i])/2.;
    }
}

পূর্ণাঙ্ক get_yolo_detections(layer l, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক netw, পূর্ণাঙ্ক neth, ভিন্ন thresh, পূর্ণাঙ্ক *map, পূর্ণাঙ্ক relative, detection *dets)
{
    পূর্ণাঙ্ক i,j,n;
    ভিন্ন *predictions = l.output;
    যদি (l.batch == 2) avg_flipped_yolo(l);
    পূর্ণাঙ্ক গুনতি = 0;
    প্রতি (i = 0; i < l.w*l.h; ++i){
        পূর্ণাঙ্ক row = i / l.w;
        পূর্ণাঙ্ক col = i % l.w;
        প্রতি(n = 0; n < l.n; ++n){
            পূর্ণাঙ্ক obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            ভিন্ন objectness = predictions[obj_index];
            যদি(objectness <= thresh) অনুবর্ত;
            পূর্ণাঙ্ক box_index  = entry_index(l, 0, n*l.w*l.h + i, 0);
            dets[গুনতি].bbox = get_yolo_box(predictions, l.biases, l.mask[n], box_index, col, row, l.w, l.h, netw, neth, l.w*l.h);
            dets[গুনতি].objectness = objectness;
            dets[গুনতি].classes = l.classes;
            প্রতি(j = 0; j < l.classes; ++j){
                পূর্ণাঙ্ক class_index = entry_index(l, 0, n*l.w*l.h + i, 4 + 1 + j);
                ভিন্ন prob = objectness*predictions[class_index];
                dets[গুনতি].prob[j] = (prob > thresh) ? prob : 0;
            }
            ++গুনতি;
        }
    }
    correct_yolo_boxes(dets, গুনতি, w, h, netw, neth, relative);
    প্রত্যাগম গুনতি;
}

#ifdef GPU

শূন্য forward_yolo_layer_gpu(ধ্রুব layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.প্রবেশ_gpu, 1, l.output_gpu, 1);
    পূর্ণাঙ্ক b, n;
    প্রতি (b = 0; b < l.batch; ++b){
        প্রতি(n = 0; n < l.n; ++n){
            পূর্ণাঙ্ক index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array_gpu(l.output_gpu + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
    যদি(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        প্রত্যাগম;
    }

    cuda_pull_array(l.output_gpu, net.প্রবেশ, l.batch*l.inputs);
    forward_yolo_layer(l, net);
    cuda_ঠেলো_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

শূন্য backward_yolo_layer_gpu(ধ্রুব layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


#شامل "yolo_layer.h"
#شامل "activations.h"
#شامل "blas.h"
#شامل "box.h"
#شامل "cuda.h"
#شامل "utils.h"

#شامل <stdio.h>
#شامل <assert.h>
#شامل <string.h>
#شامل <stdlib.h>

layer make_yolo_layer(صحیح batch, صحیح w, صحیح h, صحیح n, صحیح total, صحیح *mask, صحیح classes)
{
    صحیح i;
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
    l.cost = calloc(1, اندازه(اعشاری));
    l.biases = calloc(total*2, اندازه(اعشاری));
    اگر(mask) l.mask = mask;
    وگرنه{
        l.mask = calloc(n, اندازه(صحیح));
        برای(i = 0; i < n; ++i){
            l.mask[i] = i;
        }
    }
    l.bias_updates = calloc(n*2, اندازه(اعشاری));
    l.outputs = h*w*n*(classes + 4 + 1);
    l.inputs = l.outputs;
    l.truths = 90*(4 + 1);
    l.delta = calloc(batch*l.outputs, اندازه(اعشاری));
    l.output = calloc(batch*l.outputs, اندازه(اعشاری));
    برای(i = 0; i < total*2; ++i){
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

    بازگشت l;
}

تهی resize_yolo_layer(layer *l, صحیح w, صحیح h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + 4 + 1);
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*اندازه(اعشاری));
    l->delta = realloc(l->delta, l->batch*l->outputs*اندازه(اعشاری));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

box get_yolo_box(اعشاری *x, اعشاری *biases, صحیح n, صحیح index, صحیح i, صحیح j, صحیح lw, صحیح lh, صحیح w, صحیح h, صحیح stride)
{
    box b;
    b.x = (i + x[index + 0*stride]) / lw;
    b.y = (j + x[index + 1*stride]) / lh;
    b.w = exp(x[index + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[index + 3*stride]) * biases[2*n+1] / h;
    بازگشت b;
}

اعشاری delta_yolo_box(box truth, اعشاری *x, اعشاری *biases, صحیح n, صحیح index, صحیح i, صحیح j, صحیح lw, صحیح lh, صحیح w, صحیح h, اعشاری *delta, اعشاری scale, صحیح stride)
{
    box pred = get_yolo_box(x, biases, n, index, i, j, lw, lh, w, h, stride);
    اعشاری iou = box_iou(pred, truth);

    اعشاری tx = (truth.x*lw - i);
    اعشاری ty = (truth.y*lh - j);
    اعشاری tw = log(truth.w*w / biases[2*n]);
    اعشاری th = log(truth.h*h / biases[2*n + 1]);

    delta[index + 0*stride] = scale * (tx - x[index + 0*stride]);
    delta[index + 1*stride] = scale * (ty - x[index + 1*stride]);
    delta[index + 2*stride] = scale * (tw - x[index + 2*stride]);
    delta[index + 3*stride] = scale * (th - x[index + 3*stride]);
    بازگشت iou;
}


تهی delta_yolo_class(اعشاری *output, اعشاری *delta, صحیح index, صحیح class, صحیح classes, صحیح stride, اعشاری *avg_cat)
{
    صحیح n;
    اگر (delta[index]){
        delta[index + stride*class] = 1 - output[index + stride*class];
        اگر(avg_cat) *avg_cat += output[index + stride*class];
        بازگشت;
    }
    برای(n = 0; n < classes; ++n){
        delta[index + stride*n] = ((n == class)?1 : 0) - output[index + stride*n];
        اگر(n == class && avg_cat) *avg_cat += output[index + stride*n];
    }
}

ایستا صحیح entry_index(layer l, صحیح batch, صحیح location, صحیح entry)
{
    صحیح n =   location / (l.w*l.h);
    صحیح loc = location % (l.w*l.h);
    بازگشت batch*l.outputs + n*l.w*l.h*(4+l.classes+1) + entry*l.w*l.h + loc;
}

تهی forward_yolo_layer(ثابت layer l, network net)
{
    صحیح i,j,b,t,n;
    memcpy(l.output, net.درون_داد, l.outputs*l.batch*اندازه(اعشاری));

#ifndef GPU
    برای (b = 0; b < l.batch; ++b){
        برای(n = 0; n < l.n; ++n){
            صحیح index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array(l.output + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array(l.output + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
#endif

    memset(l.delta, 0, l.outputs * l.batch * اندازه(اعشاری));
    اگر(!net.train) بازگشت;
    اعشاری avg_iou = 0;
    اعشاری recall = 0;
    اعشاری recall75 = 0;
    اعشاری avg_cat = 0;
    اعشاری avg_obj = 0;
    اعشاری avg_anyobj = 0;
    صحیح شمار = 0;
    صحیح class_شمار = 0;
    *(l.cost) = 0;
    برای (b = 0; b < l.batch; ++b) {
        برای (j = 0; j < l.h; ++j) {
            برای (i = 0; i < l.w; ++i) {
                برای (n = 0; n < l.n; ++n) {
                    صحیح box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_yolo_box(l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.w*l.h);
                    اعشاری best_iou = 0;
                    صحیح best_t = 0;
                    برای(t = 0; t < l.max_boxes; ++t){
                        box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);
                        اگر(!truth.x) بشکن;
                        اعشاری iou = box_iou(pred, truth);
                        اگر (iou > best_iou) {
                            best_iou = iou;
                            best_t = t;
                        }
                    }
                    صحیح obj_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4);
                    avg_anyobj += l.output[obj_index];
                    l.delta[obj_index] = 0 - l.output[obj_index];
                    اگر (best_iou > l.ignore_thresh) {
                        l.delta[obj_index] = 0;
                    }
                    اگر (best_iou > l.truth_thresh) {
                        l.delta[obj_index] = 1 - l.output[obj_index];

                        صحیح class = net.truth[best_t*(4 + 1) + b*l.truths + 4];
                        اگر (l.map) class = l.map[class];
                        صحیح class_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4 + 1);
                        delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, 0);
                        box truth = float_to_box(net.truth + best_t*(4 + 1) + b*l.truths, 1);
                        delta_yolo_box(truth, l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);
                    }
                }
            }
        }
        برای(t = 0; t < l.max_boxes; ++t){
            box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);

            اگر(!truth.x) بشکن;
            اعشاری best_iou = 0;
            صحیح best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = truth_shift.y = 0;
            برای(n = 0; n < l.total; ++n){
                box pred = {0};
                pred.w = l.biases[2*n]/net.w;
                pred.h = l.biases[2*n+1]/net.h;
                اعشاری iou = box_iou(pred, truth_shift);
                اگر (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            صحیح mask_n = int_index(l.mask, best_n, l.n);
            اگر(mask_n >= 0){
                صحیح box_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 0);
                اعشاری iou = delta_yolo_box(truth, l.output, l.biases, best_n, box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);

                صحیح obj_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4);
                avg_obj += l.output[obj_index];
                l.delta[obj_index] = 1 - l.output[obj_index];

                صحیح class = net.truth[t*(4 + 1) + b*l.truths + 4];
                اگر (l.map) class = l.map[class];
                صحیح class_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4 + 1);
                delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, &avg_cat);

                ++شمار;
                ++class_شمار;
                اگر(iou > .5) recall += 1;
                اگر(iou > .75) recall75 += 1;
                avg_iou += iou;
            }
        }
    }
    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("Region %d Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, .5R: %f, .75R: %f,  count: %d\n", net.index, avg_iou/شمار, avg_cat/class_شمار, avg_obj/شمار, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/شمار, recall75/شمار, شمار);
}

تهی backward_yolo_layer(ثابت layer l, network net)
{
   axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

تهی correct_yolo_boxes(detection *dets, صحیح n, صحیح w, صحیح h, صحیح netw, صحیح neth, صحیح relative)
{
    صحیح i;
    صحیح new_w=0;
    صحیح new_h=0;
    اگر (((اعشاری)netw/w) < ((اعشاری)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } وگرنه {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    برای (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((اعشاری)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((اعشاری)new_h/neth); 
        b.w *= (اعشاری)netw/new_w;
        b.h *= (اعشاری)neth/new_h;
        اگر(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

صحیح yolo_رقم_detections(layer l, اعشاری thresh)
{
    صحیح i, n;
    صحیح شمار = 0;
    برای (i = 0; i < l.w*l.h; ++i){
        برای(n = 0; n < l.n; ++n){
            صحیح obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            اگر(l.output[obj_index] > thresh){
                ++شمار;
            }
        }
    }
    بازگشت شمار;
}

تهی avg_flipped_yolo(layer l)
{
    صحیح i,j,n,z;
    اعشاری *flip = l.output + l.outputs;
    برای (j = 0; j < l.h; ++j) {
        برای (i = 0; i < l.w/2; ++i) {
            برای (n = 0; n < l.n; ++n) {
                برای(z = 0; z < l.classes + 4 + 1; ++z){
                    صحیح i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                    صحیح i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                    اعشاری swap = flip[i1];
                    flip[i1] = flip[i2];
                    flip[i2] = swap;
                    اگر(z == 0){
                        flip[i1] = -flip[i1];
                        flip[i2] = -flip[i2];
                    }
                }
            }
        }
    }
    برای(i = 0; i < l.outputs; ++i){
        l.output[i] = (l.output[i] + flip[i])/2.;
    }
}

صحیح get_yolo_detections(layer l, صحیح w, صحیح h, صحیح netw, صحیح neth, اعشاری thresh, صحیح *map, صحیح relative, detection *dets)
{
    صحیح i,j,n;
    اعشاری *predictions = l.output;
    اگر (l.batch == 2) avg_flipped_yolo(l);
    صحیح شمار = 0;
    برای (i = 0; i < l.w*l.h; ++i){
        صحیح row = i / l.w;
        صحیح col = i % l.w;
        برای(n = 0; n < l.n; ++n){
            صحیح obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            اعشاری objectness = predictions[obj_index];
            اگر(objectness <= thresh) ادامه;
            صحیح box_index  = entry_index(l, 0, n*l.w*l.h + i, 0);
            dets[شمار].bbox = get_yolo_box(predictions, l.biases, l.mask[n], box_index, col, row, l.w, l.h, netw, neth, l.w*l.h);
            dets[شمار].objectness = objectness;
            dets[شمار].classes = l.classes;
            برای(j = 0; j < l.classes; ++j){
                صحیح class_index = entry_index(l, 0, n*l.w*l.h + i, 4 + 1 + j);
                اعشاری prob = objectness*predictions[class_index];
                dets[شمار].prob[j] = (prob > thresh) ? prob : 0;
            }
            ++شمار;
        }
    }
    correct_yolo_boxes(dets, شمار, w, h, netw, neth, relative);
    بازگشت شمار;
}

#ifdef GPU

تهی forward_yolo_layer_gpu(ثابت layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.درون_داد_gpu, 1, l.output_gpu, 1);
    صحیح b, n;
    برای (b = 0; b < l.batch; ++b){
        برای(n = 0; n < l.n; ++n){
            صحیح index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array_gpu(l.output_gpu + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
    اگر(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        بازگشت;
    }

    cuda_pull_array(l.output_gpu, net.درون_داد, l.batch*l.inputs);
    forward_yolo_layer(l, net);
    cuda_افزودن_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

تهی backward_yolo_layer_gpu(ثابت layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


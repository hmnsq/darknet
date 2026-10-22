#अन्तर्भाव "yolo_layer.h"
#अन्तर्भाव "activations.h"
#अन्तर्भाव "blas.h"
#अन्तर्भाव "box.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव "utils.h"

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <assert.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <stdlib.h>

layer make_yolo_layer(पूर्णाङ्क batch, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क n, पूर्णाङ्क total, पूर्णाङ्क *mask, पूर्णाङ्क classes)
{
    पूर्णाङ्क i;
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
    l.cost = सुस्मृति(1, परिमाण(भिन्न));
    l.biases = सुस्मृति(total*2, परिमाण(भिन्न));
    यदि(mask) l.mask = mask;
    अन्यथा{
        l.mask = सुस्मृति(n, परिमाण(पूर्णाङ्क));
        लागि(i = 0; i < n; ++i){
            l.mask[i] = i;
        }
    }
    l.bias_updates = सुस्मृति(n*2, परिमाण(भिन्न));
    l.outputs = h*w*n*(classes + 4 + 1);
    l.inputs = l.outputs;
    l.truths = 90*(4 + 1);
    l.delta = सुस्मृति(batch*l.outputs, परिमाण(भिन्न));
    l.output = सुस्मृति(batch*l.outputs, परिमाण(भिन्न));
    लागि(i = 0; i < total*2; ++i){
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

    ख_लिखो(मानक_त्रुटि, "yolo\n");
    बेक्रम(0);

    फर्काऊ l;
}

शून्य resize_yolo_layer(layer *l, पूर्णाङ्क w, पूर्णाङ्क h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + 4 + 1);
    l->inputs = l->outputs;

    l->output = पुनः_स्मृति(l->output, l->batch*l->outputs*परिमाण(भिन्न));
    l->delta = पुनः_स्मृति(l->delta, l->batch*l->outputs*परिमाण(भिन्न));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

box get_yolo_box(भिन्न *x, भिन्न *biases, पूर्णाङ्क n, पूर्णाङ्क index, पूर्णाङ्क i, पूर्णाङ्क j, पूर्णाङ्क lw, पूर्णाङ्क lh, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क stride)
{
    box b;
    b.x = (i + x[index + 0*stride]) / lw;
    b.y = (j + x[index + 1*stride]) / lh;
    b.w = exp(x[index + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[index + 3*stride]) * biases[2*n+1] / h;
    फर्काऊ b;
}

भिन्न delta_yolo_box(box truth, भिन्न *x, भिन्न *biases, पूर्णाङ्क n, पूर्णाङ्क index, पूर्णाङ्क i, पूर्णाङ्क j, पूर्णाङ्क lw, पूर्णाङ्क lh, पूर्णाङ्क w, पूर्णाङ्क h, भिन्न *delta, भिन्न scale, पूर्णाङ्क stride)
{
    box pred = get_yolo_box(x, biases, n, index, i, j, lw, lh, w, h, stride);
    भिन्न iou = box_iou(pred, truth);

    भिन्न tx = (truth.x*lw - i);
    भिन्न ty = (truth.y*lh - j);
    भिन्न tw = log(truth.w*w / biases[2*n]);
    भिन्न th = log(truth.h*h / biases[2*n + 1]);

    delta[index + 0*stride] = scale * (tx - x[index + 0*stride]);
    delta[index + 1*stride] = scale * (ty - x[index + 1*stride]);
    delta[index + 2*stride] = scale * (tw - x[index + 2*stride]);
    delta[index + 3*stride] = scale * (th - x[index + 3*stride]);
    फर्काऊ iou;
}


शून्य delta_yolo_class(भिन्न *output, भिन्न *delta, पूर्णाङ्क index, पूर्णाङ्क class, पूर्णाङ्क classes, पूर्णाङ्क stride, भिन्न *avg_cat)
{
    पूर्णाङ्क n;
    यदि (delta[index]){
        delta[index + stride*class] = 1 - output[index + stride*class];
        यदि(avg_cat) *avg_cat += output[index + stride*class];
        फर्काऊ;
    }
    लागि(n = 0; n < classes; ++n){
        delta[index + stride*n] = ((n == class)?1 : 0) - output[index + stride*n];
        यदि(n == class && avg_cat) *avg_cat += output[index + stride*n];
    }
}

स्थिर पूर्णाङ्क entry_index(layer l, पूर्णाङ्क batch, पूर्णाङ्क location, पूर्णाङ्क entry)
{
    पूर्णाङ्क n =   location / (l.w*l.h);
    पूर्णाङ्क loc = location % (l.w*l.h);
    फर्काऊ batch*l.outputs + n*l.w*l.h*(4+l.classes+1) + entry*l.w*l.h + loc;
}

शून्य forward_yolo_layer(अचर layer l, network net)
{
    पूर्णाङ्क i,j,b,t,n;
    स_नकल(l.output, net.आगत, l.outputs*l.batch*परिमाण(भिन्न));

#ifndef GPU
    लागि (b = 0; b < l.batch; ++b){
        लागि(n = 0; n < l.n; ++n){
            पूर्णाङ्क index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array(l.output + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array(l.output + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
#endif

    स_रखो(l.delta, 0, l.outputs * l.batch * परिमाण(भिन्न));
    यदि(!net.train) फर्काऊ;
    भिन्न avg_iou = 0;
    भिन्न recall = 0;
    भिन्न recall75 = 0;
    भिन्न avg_cat = 0;
    भिन्न avg_obj = 0;
    भिन्न avg_anyobj = 0;
    पूर्णाङ्क गन्ती = 0;
    पूर्णाङ्क class_गन्ती = 0;
    *(l.cost) = 0;
    लागि (b = 0; b < l.batch; ++b) {
        लागि (j = 0; j < l.h; ++j) {
            लागि (i = 0; i < l.w; ++i) {
                लागि (n = 0; n < l.n; ++n) {
                    पूर्णाङ्क box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_yolo_box(l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.w*l.h);
                    भिन्न best_iou = 0;
                    पूर्णाङ्क best_t = 0;
                    लागि(t = 0; t < l.max_boxes; ++t){
                        box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);
                        यदि(!truth.x) रोक;
                        भिन्न iou = box_iou(pred, truth);
                        यदि (iou > best_iou) {
                            best_iou = iou;
                            best_t = t;
                        }
                    }
                    पूर्णाङ्क obj_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4);
                    avg_anyobj += l.output[obj_index];
                    l.delta[obj_index] = 0 - l.output[obj_index];
                    यदि (best_iou > l.ignore_thresh) {
                        l.delta[obj_index] = 0;
                    }
                    यदि (best_iou > l.truth_thresh) {
                        l.delta[obj_index] = 1 - l.output[obj_index];

                        पूर्णाङ्क class = net.truth[best_t*(4 + 1) + b*l.truths + 4];
                        यदि (l.map) class = l.map[class];
                        पूर्णाङ्क class_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4 + 1);
                        delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, 0);
                        box truth = float_to_box(net.truth + best_t*(4 + 1) + b*l.truths, 1);
                        delta_yolo_box(truth, l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);
                    }
                }
            }
        }
        लागि(t = 0; t < l.max_boxes; ++t){
            box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);

            यदि(!truth.x) रोक;
            भिन्न best_iou = 0;
            पूर्णाङ्क best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = truth_shift.y = 0;
            लागि(n = 0; n < l.total; ++n){
                box pred = {0};
                pred.w = l.biases[2*n]/net.w;
                pred.h = l.biases[2*n+1]/net.h;
                भिन्न iou = box_iou(pred, truth_shift);
                यदि (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            पूर्णाङ्क mask_n = int_index(l.mask, best_n, l.n);
            यदि(mask_n >= 0){
                पूर्णाङ्क box_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 0);
                भिन्न iou = delta_yolo_box(truth, l.output, l.biases, best_n, box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);

                पूर्णाङ्क obj_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4);
                avg_obj += l.output[obj_index];
                l.delta[obj_index] = 1 - l.output[obj_index];

                पूर्णाङ्क class = net.truth[t*(4 + 1) + b*l.truths + 4];
                यदि (l.map) class = l.map[class];
                पूर्णाङ्क class_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4 + 1);
                delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, &avg_cat);

                ++गन्ती;
                ++class_गन्ती;
                यदि(iou > .5) recall += 1;
                यदि(iou > .75) recall75 += 1;
                avg_iou += iou;
            }
        }
    }
    *(l.cost) = घात(mag_array(l.delta, l.outputs * l.batch), 2);
    म_लिखो("Region %d Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, .5R: %f, .75R: %f,  count: %d\n", net.index, avg_iou/गन्ती, avg_cat/class_गन्ती, avg_obj/गन्ती, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/गन्ती, recall75/गन्ती, गन्ती);
}

शून्य backward_yolo_layer(अचर layer l, network net)
{
   axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

शून्य correct_yolo_boxes(detection *dets, पूर्णाङ्क n, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क netw, पूर्णाङ्क neth, पूर्णाङ्क relative)
{
    पूर्णाङ्क i;
    पूर्णाङ्क new_w=0;
    पूर्णाङ्क new_h=0;
    यदि (((भिन्न)netw/w) < ((भिन्न)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } अन्यथा {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    लागि (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((भिन्न)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((भिन्न)new_h/neth); 
        b.w *= (भिन्न)netw/new_w;
        b.h *= (भिन्न)neth/new_h;
        यदि(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

पूर्णाङ्क yolo_अङ्क_detections(layer l, भिन्न thresh)
{
    पूर्णाङ्क i, n;
    पूर्णाङ्क गन्ती = 0;
    लागि (i = 0; i < l.w*l.h; ++i){
        लागि(n = 0; n < l.n; ++n){
            पूर्णाङ्क obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            यदि(l.output[obj_index] > thresh){
                ++गन्ती;
            }
        }
    }
    फर्काऊ गन्ती;
}

शून्य avg_flipped_yolo(layer l)
{
    पूर्णाङ्क i,j,n,z;
    भिन्न *flip = l.output + l.outputs;
    लागि (j = 0; j < l.h; ++j) {
        लागि (i = 0; i < l.w/2; ++i) {
            लागि (n = 0; n < l.n; ++n) {
                लागि(z = 0; z < l.classes + 4 + 1; ++z){
                    पूर्णाङ्क i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                    पूर्णाङ्क i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                    भिन्न swap = flip[i1];
                    flip[i1] = flip[i2];
                    flip[i2] = swap;
                    यदि(z == 0){
                        flip[i1] = -flip[i1];
                        flip[i2] = -flip[i2];
                    }
                }
            }
        }
    }
    लागि(i = 0; i < l.outputs; ++i){
        l.output[i] = (l.output[i] + flip[i])/2.;
    }
}

पूर्णाङ्क get_yolo_detections(layer l, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क netw, पूर्णाङ्क neth, भिन्न thresh, पूर्णाङ्क *map, पूर्णाङ्क relative, detection *dets)
{
    पूर्णाङ्क i,j,n;
    भिन्न *predictions = l.output;
    यदि (l.batch == 2) avg_flipped_yolo(l);
    पूर्णाङ्क गन्ती = 0;
    लागि (i = 0; i < l.w*l.h; ++i){
        पूर्णाङ्क row = i / l.w;
        पूर्णाङ्क col = i % l.w;
        लागि(n = 0; n < l.n; ++n){
            पूर्णाङ्क obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            भिन्न objectness = predictions[obj_index];
            यदि(objectness <= thresh) जारी;
            पूर्णाङ्क box_index  = entry_index(l, 0, n*l.w*l.h + i, 0);
            dets[गन्ती].bbox = get_yolo_box(predictions, l.biases, l.mask[n], box_index, col, row, l.w, l.h, netw, neth, l.w*l.h);
            dets[गन्ती].objectness = objectness;
            dets[गन्ती].classes = l.classes;
            लागि(j = 0; j < l.classes; ++j){
                पूर्णाङ्क class_index = entry_index(l, 0, n*l.w*l.h + i, 4 + 1 + j);
                भिन्न prob = objectness*predictions[class_index];
                dets[गन्ती].prob[j] = (prob > thresh) ? prob : 0;
            }
            ++गन्ती;
        }
    }
    correct_yolo_boxes(dets, गन्ती, w, h, netw, neth, relative);
    फर्काऊ गन्ती;
}

#ifdef GPU

शून्य forward_yolo_layer_gpu(अचर layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.आगत_gpu, 1, l.output_gpu, 1);
    पूर्णाङ्क b, n;
    लागि (b = 0; b < l.batch; ++b){
        लागि(n = 0; n < l.n; ++n){
            पूर्णाङ्क index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array_gpu(l.output_gpu + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
    यदि(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        फर्काऊ;
    }

    cuda_pull_array(l.output_gpu, net.आगत, l.batch*l.inputs);
    forward_yolo_layer(l, net);
    cuda_थप_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

शून्य backward_yolo_layer_gpu(अचर layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


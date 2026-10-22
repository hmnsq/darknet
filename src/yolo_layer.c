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

layer make_yolo_layer(पूर्णांक batch, पूर्णांक w, पूर्णांक h, पूर्णांक n, पूर्णांक total, पूर्णांक *mask, पूर्णांक classes)
{
    पूर्णांक i;
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
    l.cost = सुस्मृति(1, आकार(दशांश));
    l.biases = सुस्मृति(total*2, आकार(दशांश));
    जर(mask) l.mask = mask;
    अन्यथा{
        l.mask = सुस्मृति(n, आकार(पूर्णांक));
        साठी(i = 0; i < n; ++i){
            l.mask[i] = i;
        }
    }
    l.bias_updates = सुस्मृति(n*2, आकार(दशांश));
    l.outputs = h*w*n*(classes + 4 + 1);
    l.inputs = l.outputs;
    l.truths = 90*(4 + 1);
    l.delta = सुस्मृति(batch*l.outputs, आकार(दशांश));
    l.output = सुस्मृति(batch*l.outputs, आकार(दशांश));
    साठी(i = 0; i < total*2; ++i){
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

    परत l;
}

शून्य resize_yolo_layer(layer *l, पूर्णांक w, पूर्णांक h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + 4 + 1);
    l->inputs = l->outputs;

    l->output = पुनः_स्मृति(l->output, l->batch*l->outputs*आकार(दशांश));
    l->delta = पुनः_स्मृति(l->delta, l->batch*l->outputs*आकार(दशांश));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

box get_yolo_box(दशांश *x, दशांश *biases, पूर्णांक n, पूर्णांक index, पूर्णांक i, पूर्णांक j, पूर्णांक lw, पूर्णांक lh, पूर्णांक w, पूर्णांक h, पूर्णांक stride)
{
    box b;
    b.x = (i + x[index + 0*stride]) / lw;
    b.y = (j + x[index + 1*stride]) / lh;
    b.w = exp(x[index + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[index + 3*stride]) * biases[2*n+1] / h;
    परत b;
}

दशांश delta_yolo_box(box truth, दशांश *x, दशांश *biases, पूर्णांक n, पूर्णांक index, पूर्णांक i, पूर्णांक j, पूर्णांक lw, पूर्णांक lh, पूर्णांक w, पूर्णांक h, दशांश *delta, दशांश scale, पूर्णांक stride)
{
    box pred = get_yolo_box(x, biases, n, index, i, j, lw, lh, w, h, stride);
    दशांश iou = box_iou(pred, truth);

    दशांश tx = (truth.x*lw - i);
    दशांश ty = (truth.y*lh - j);
    दशांश tw = log(truth.w*w / biases[2*n]);
    दशांश th = log(truth.h*h / biases[2*n + 1]);

    delta[index + 0*stride] = scale * (tx - x[index + 0*stride]);
    delta[index + 1*stride] = scale * (ty - x[index + 1*stride]);
    delta[index + 2*stride] = scale * (tw - x[index + 2*stride]);
    delta[index + 3*stride] = scale * (th - x[index + 3*stride]);
    परत iou;
}


शून्य delta_yolo_class(दशांश *output, दशांश *delta, पूर्णांक index, पूर्णांक class, पूर्णांक classes, पूर्णांक stride, दशांश *avg_cat)
{
    पूर्णांक n;
    जर (delta[index]){
        delta[index + stride*class] = 1 - output[index + stride*class];
        जर(avg_cat) *avg_cat += output[index + stride*class];
        परत;
    }
    साठी(n = 0; n < classes; ++n){
        delta[index + stride*n] = ((n == class)?1 : 0) - output[index + stride*n];
        जर(n == class && avg_cat) *avg_cat += output[index + stride*n];
    }
}

स्थिर पूर्णांक entry_index(layer l, पूर्णांक batch, पूर्णांक location, पूर्णांक entry)
{
    पूर्णांक n =   location / (l.w*l.h);
    पूर्णांक loc = location % (l.w*l.h);
    परत batch*l.outputs + n*l.w*l.h*(4+l.classes+1) + entry*l.w*l.h + loc;
}

शून्य forward_yolo_layer(स्थिरांक layer l, network net)
{
    पूर्णांक i,j,b,t,n;
    स_नकल(l.output, net.आदान, l.outputs*l.batch*आकार(दशांश));

#ifndef GPU
    साठी (b = 0; b < l.batch; ++b){
        साठी(n = 0; n < l.n; ++n){
            पूर्णांक index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array(l.output + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array(l.output + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
#endif

    स_रखो(l.delta, 0, l.outputs * l.batch * आकार(दशांश));
    जर(!net.train) परत;
    दशांश avg_iou = 0;
    दशांश recall = 0;
    दशांश recall75 = 0;
    दशांश avg_cat = 0;
    दशांश avg_obj = 0;
    दशांश avg_anyobj = 0;
    पूर्णांक मोजणी = 0;
    पूर्णांक class_मोजणी = 0;
    *(l.cost) = 0;
    साठी (b = 0; b < l.batch; ++b) {
        साठी (j = 0; j < l.h; ++j) {
            साठी (i = 0; i < l.w; ++i) {
                साठी (n = 0; n < l.n; ++n) {
                    पूर्णांक box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_yolo_box(l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.w*l.h);
                    दशांश best_iou = 0;
                    पूर्णांक best_t = 0;
                    साठी(t = 0; t < l.max_boxes; ++t){
                        box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);
                        जर(!truth.x) थांबा;
                        दशांश iou = box_iou(pred, truth);
                        जर (iou > best_iou) {
                            best_iou = iou;
                            best_t = t;
                        }
                    }
                    पूर्णांक obj_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4);
                    avg_anyobj += l.output[obj_index];
                    l.delta[obj_index] = 0 - l.output[obj_index];
                    जर (best_iou > l.ignore_thresh) {
                        l.delta[obj_index] = 0;
                    }
                    जर (best_iou > l.truth_thresh) {
                        l.delta[obj_index] = 1 - l.output[obj_index];

                        पूर्णांक class = net.truth[best_t*(4 + 1) + b*l.truths + 4];
                        जर (l.map) class = l.map[class];
                        पूर्णांक class_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 4 + 1);
                        delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, 0);
                        box truth = float_to_box(net.truth + best_t*(4 + 1) + b*l.truths, 1);
                        delta_yolo_box(truth, l.output, l.biases, l.mask[n], box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);
                    }
                }
            }
        }
        साठी(t = 0; t < l.max_boxes; ++t){
            box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);

            जर(!truth.x) थांबा;
            दशांश best_iou = 0;
            पूर्णांक best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = truth_shift.y = 0;
            साठी(n = 0; n < l.total; ++n){
                box pred = {0};
                pred.w = l.biases[2*n]/net.w;
                pred.h = l.biases[2*n+1]/net.h;
                दशांश iou = box_iou(pred, truth_shift);
                जर (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            पूर्णांक mask_n = int_index(l.mask, best_n, l.n);
            जर(mask_n >= 0){
                पूर्णांक box_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 0);
                दशांश iou = delta_yolo_box(truth, l.output, l.biases, best_n, box_index, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);

                पूर्णांक obj_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4);
                avg_obj += l.output[obj_index];
                l.delta[obj_index] = 1 - l.output[obj_index];

                पूर्णांक class = net.truth[t*(4 + 1) + b*l.truths + 4];
                जर (l.map) class = l.map[class];
                पूर्णांक class_index = entry_index(l, b, mask_n*l.w*l.h + j*l.w + i, 4 + 1);
                delta_yolo_class(l.output, l.delta, class_index, class, l.classes, l.w*l.h, &avg_cat);

                ++मोजणी;
                ++class_मोजणी;
                जर(iou > .5) recall += 1;
                जर(iou > .75) recall75 += 1;
                avg_iou += iou;
            }
        }
    }
    *(l.cost) = घात(mag_array(l.delta, l.outputs * l.batch), 2);
    म_लिखो("Region %d Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, .5R: %f, .75R: %f,  count: %d\n", net.index, avg_iou/मोजणी, avg_cat/class_मोजणी, avg_obj/मोजणी, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/मोजणी, recall75/मोजणी, मोजणी);
}

शून्य backward_yolo_layer(स्थिरांक layer l, network net)
{
   axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

शून्य correct_yolo_boxes(detection *dets, पूर्णांक n, पूर्णांक w, पूर्णांक h, पूर्णांक netw, पूर्णांक neth, पूर्णांक relative)
{
    पूर्णांक i;
    पूर्णांक new_w=0;
    पूर्णांक new_h=0;
    जर (((दशांश)netw/w) < ((दशांश)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } अन्यथा {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    साठी (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((दशांश)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((दशांश)new_h/neth); 
        b.w *= (दशांश)netw/new_w;
        b.h *= (दशांश)neth/new_h;
        जर(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

पूर्णांक yolo_अंक_detections(layer l, दशांश thresh)
{
    पूर्णांक i, n;
    पूर्णांक मोजणी = 0;
    साठी (i = 0; i < l.w*l.h; ++i){
        साठी(n = 0; n < l.n; ++n){
            पूर्णांक obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            जर(l.output[obj_index] > thresh){
                ++मोजणी;
            }
        }
    }
    परत मोजणी;
}

शून्य avg_flipped_yolo(layer l)
{
    पूर्णांक i,j,n,z;
    दशांश *flip = l.output + l.outputs;
    साठी (j = 0; j < l.h; ++j) {
        साठी (i = 0; i < l.w/2; ++i) {
            साठी (n = 0; n < l.n; ++n) {
                साठी(z = 0; z < l.classes + 4 + 1; ++z){
                    पूर्णांक i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                    पूर्णांक i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                    दशांश swap = flip[i1];
                    flip[i1] = flip[i2];
                    flip[i2] = swap;
                    जर(z == 0){
                        flip[i1] = -flip[i1];
                        flip[i2] = -flip[i2];
                    }
                }
            }
        }
    }
    साठी(i = 0; i < l.outputs; ++i){
        l.output[i] = (l.output[i] + flip[i])/2.;
    }
}

पूर्णांक get_yolo_detections(layer l, पूर्णांक w, पूर्णांक h, पूर्णांक netw, पूर्णांक neth, दशांश thresh, पूर्णांक *map, पूर्णांक relative, detection *dets)
{
    पूर्णांक i,j,n;
    दशांश *predictions = l.output;
    जर (l.batch == 2) avg_flipped_yolo(l);
    पूर्णांक मोजणी = 0;
    साठी (i = 0; i < l.w*l.h; ++i){
        पूर्णांक row = i / l.w;
        पूर्णांक col = i % l.w;
        साठी(n = 0; n < l.n; ++n){
            पूर्णांक obj_index  = entry_index(l, 0, n*l.w*l.h + i, 4);
            दशांश objectness = predictions[obj_index];
            जर(objectness <= thresh) सुरूठेवा;
            पूर्णांक box_index  = entry_index(l, 0, n*l.w*l.h + i, 0);
            dets[मोजणी].bbox = get_yolo_box(predictions, l.biases, l.mask[n], box_index, col, row, l.w, l.h, netw, neth, l.w*l.h);
            dets[मोजणी].objectness = objectness;
            dets[मोजणी].classes = l.classes;
            साठी(j = 0; j < l.classes; ++j){
                पूर्णांक class_index = entry_index(l, 0, n*l.w*l.h + i, 4 + 1 + j);
                दशांश prob = objectness*predictions[class_index];
                dets[मोजणी].prob[j] = (prob > thresh) ? prob : 0;
            }
            ++मोजणी;
        }
    }
    correct_yolo_boxes(dets, मोजणी, w, h, netw, neth, relative);
    परत मोजणी;
}

#ifdef GPU

शून्य forward_yolo_layer_gpu(स्थिरांक layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.आदान_gpu, 1, l.output_gpu, 1);
    पूर्णांक b, n;
    साठी (b = 0; b < l.batch; ++b){
        साठी(n = 0; n < l.n; ++n){
            पूर्णांक index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, 4);
            activate_array_gpu(l.output_gpu + index, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
    जर(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        परत;
    }

    cuda_pull_array(l.output_gpu, net.आदान, l.batch*l.inputs);
    forward_yolo_layer(l, net);
    cuda_ढकला_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

शून्य backward_yolo_layer_gpu(स्थिरांक layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


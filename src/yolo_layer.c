#समावेश "yolo_layer.h"
#समावेश "activations.h"
#समावेश "blas.h"
#समावेश "box.h"
#समावेश "cuda.h"
#समावेश "utils.h"

#समावेश <stdio.h>
#समावेश <assert.h>
#समावेश <string.h>
#समावेश <stdlib.h>

layer make_yolo_layer(पूर्णांक batch, पूर्णांक w, पूर्णांक h, पूर्णांक n, पूर्णांक योग_2, पूर्णांक *mask, पूर्णांक classes)
{
    पूर्णांक i;
    layer l = {0};
    l.type = YOLO;

    l.n = n;
    l.योग_2 = योग_2;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = n*(classes + 4 + 1);
    l.out_w = l.w;
    l.out_h = l.h;
    l.out_c = l.c;
    l.classes = classes;
    l.cost = सुस्मृति(1, माप(भग्न));
    l.biases = सुस्मृति(योग_2*2, माप(भग्न));
    अगर(mask) l.mask = mask;
    अन्यथा{
        l.mask = सुस्मृति(n, माप(पूर्णांक));
        क्रम(i = 0; i < n; ++i){
            l.mask[i] = i;
        }
    }
    l.bias_updates = सुस्मृति(n*2, माप(भग्न));
    l.outputs = h*w*n*(classes + 4 + 1);
    l.inputs = l.outputs;
    l.truths = 90*(4 + 1);
    l.delta = सुस्मृति(batch*l.outputs, माप(भग्न));
    l.output = सुस्मृति(batch*l.outputs, माप(भग्न));
    क्रम(i = 0; i < योग_2*2; ++i){
        l.biases[i] = .5;
    }

    l.forward = forward_yolo_layer;
    l.backward = backward_yolo_layer;
#अगर_घोषित GPU
    l.forward_gpu = forward_yolo_layer_gpu;
    l.backward_gpu = backward_yolo_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#पूर्ण_अगर

    ख_लिखो(मानक_त्रुटि, "yolo\n");
    बेक्रम(0);

    वापस l;
}

व्योम resize_yolo_layer(layer *l, पूर्णांक w, पूर्णांक h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + 4 + 1);
    l->inputs = l->outputs;

    l->output = पुनः_स्मृति(l->output, l->batch*l->outputs*माप(भग्न));
    l->delta = पुनः_स्मृति(l->delta, l->batch*l->outputs*माप(भग्न));

#अगर_घोषित GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#पूर्ण_अगर
}

box get_yolo_box(भग्न *x, भग्न *biases, पूर्णांक n, पूर्णांक सूचकांक, पूर्णांक i, पूर्णांक j, पूर्णांक lw, पूर्णांक lh, पूर्णांक w, पूर्णांक h, पूर्णांक stride)
{
    box b;
    b.x = (i + x[सूचकांक + 0*stride]) / lw;
    b.y = (j + x[सूचकांक + 1*stride]) / lh;
    b.w = exp(x[सूचकांक + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[सूचकांक + 3*stride]) * biases[2*n+1] / h;
    वापस b;
}

भग्न delta_yolo_box(box truth, भग्न *x, भग्न *biases, पूर्णांक n, पूर्णांक सूचकांक, पूर्णांक i, पूर्णांक j, पूर्णांक lw, पूर्णांक lh, पूर्णांक w, पूर्णांक h, भग्न *delta, भग्न scale, पूर्णांक stride)
{
    box pred = get_yolo_box(x, biases, n, सूचकांक, i, j, lw, lh, w, h, stride);
    भग्न iou = box_iou(pred, truth);

    भग्न tx = (truth.x*lw - i);
    भग्न ty = (truth.y*lh - j);
    भग्न tw = log(truth.w*w / biases[2*n]);
    भग्न th = log(truth.h*h / biases[2*n + 1]);

    delta[सूचकांक + 0*stride] = scale * (tx - x[सूचकांक + 0*stride]);
    delta[सूचकांक + 1*stride] = scale * (ty - x[सूचकांक + 1*stride]);
    delta[सूचकांक + 2*stride] = scale * (tw - x[सूचकांक + 2*stride]);
    delta[सूचकांक + 3*stride] = scale * (th - x[सूचकांक + 3*stride]);
    वापस iou;
}


व्योम delta_yolo_class(भग्न *output, भग्न *delta, पूर्णांक सूचकांक, पूर्णांक class, पूर्णांक classes, पूर्णांक stride, भग्न *avg_cat)
{
    पूर्णांक n;
    अगर (delta[सूचकांक]){
        delta[सूचकांक + stride*class] = 1 - output[सूचकांक + stride*class];
        अगर(avg_cat) *avg_cat += output[सूचकांक + stride*class];
        वापस;
    }
    क्रम(n = 0; n < classes; ++n){
        delta[सूचकांक + stride*n] = ((n == class)?1 : 0) - output[सूचकांक + stride*n];
        अगर(n == class && avg_cat) *avg_cat += output[सूचकांक + stride*n];
    }
}

अटल पूर्णांक entry_सूचकांक(layer l, पूर्णांक batch, पूर्णांक location, पूर्णांक entry)
{
    पूर्णांक n =   location / (l.w*l.h);
    पूर्णांक loc = location % (l.w*l.h);
    वापस batch*l.outputs + n*l.w*l.h*(4+l.classes+1) + entry*l.w*l.h + loc;
}

व्योम forward_yolo_layer(स्थिर layer l, network net)
{
    पूर्णांक i,j,b,t,n;
    स_नकल(l.output, net.निवेश, l.outputs*l.batch*माप(भग्न));

#अगर_अघोषित GPU
    क्रम (b = 0; b < l.batch; ++b){
        क्रम(n = 0; n < l.n; ++n){
            पूर्णांक सूचकांक = entry_सूचकांक(l, b, n*l.w*l.h, 0);
            activate_array(l.output + सूचकांक, 2*l.w*l.h, LOGISTIC);
            सूचकांक = entry_सूचकांक(l, b, n*l.w*l.h, 4);
            activate_array(l.output + सूचकांक, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
#पूर्ण_अगर

    स_रखो(l.delta, 0, l.outputs * l.batch * माप(भग्न));
    अगर(!net.train) वापस;
    भग्न avg_iou = 0;
    भग्न recall = 0;
    भग्न recall75 = 0;
    भग्न avg_cat = 0;
    भग्न avg_obj = 0;
    भग्न avg_anyobj = 0;
    पूर्णांक गिनती = 0;
    पूर्णांक class_गिनती = 0;
    *(l.cost) = 0;
    क्रम (b = 0; b < l.batch; ++b) {
        क्रम (j = 0; j < l.h; ++j) {
            क्रम (i = 0; i < l.w; ++i) {
                क्रम (n = 0; n < l.n; ++n) {
                    पूर्णांक box_सूचकांक = entry_सूचकांक(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_yolo_box(l.output, l.biases, l.mask[n], box_सूचकांक, i, j, l.w, l.h, net.w, net.h, l.w*l.h);
                    भग्न best_iou = 0;
                    पूर्णांक best_t = 0;
                    क्रम(t = 0; t < l.max_boxes; ++t){
                        box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);
                        अगर(!truth.x) अवरोध;
                        भग्न iou = box_iou(pred, truth);
                        अगर (iou > best_iou) {
                            best_iou = iou;
                            best_t = t;
                        }
                    }
                    पूर्णांक obj_सूचकांक = entry_सूचकांक(l, b, n*l.w*l.h + j*l.w + i, 4);
                    avg_anyobj += l.output[obj_सूचकांक];
                    l.delta[obj_सूचकांक] = 0 - l.output[obj_सूचकांक];
                    अगर (best_iou > l.ignore_thresh) {
                        l.delta[obj_सूचकांक] = 0;
                    }
                    अगर (best_iou > l.truth_thresh) {
                        l.delta[obj_सूचकांक] = 1 - l.output[obj_सूचकांक];

                        पूर्णांक class = net.truth[best_t*(4 + 1) + b*l.truths + 4];
                        अगर (l.map) class = l.map[class];
                        पूर्णांक class_सूचकांक = entry_सूचकांक(l, b, n*l.w*l.h + j*l.w + i, 4 + 1);
                        delta_yolo_class(l.output, l.delta, class_सूचकांक, class, l.classes, l.w*l.h, 0);
                        box truth = float_to_box(net.truth + best_t*(4 + 1) + b*l.truths, 1);
                        delta_yolo_box(truth, l.output, l.biases, l.mask[n], box_सूचकांक, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);
                    }
                }
            }
        }
        क्रम(t = 0; t < l.max_boxes; ++t){
            box truth = float_to_box(net.truth + t*(4 + 1) + b*l.truths, 1);

            अगर(!truth.x) अवरोध;
            भग्न best_iou = 0;
            पूर्णांक best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = truth_shift.y = 0;
            क्रम(n = 0; n < l.योग_2; ++n){
                box pred = {0};
                pred.w = l.biases[2*n]/net.w;
                pred.h = l.biases[2*n+1]/net.h;
                भग्न iou = box_iou(pred, truth_shift);
                अगर (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            पूर्णांक mask_n = int_सूचकांक(l.mask, best_n, l.n);
            अगर(mask_n >= 0){
                पूर्णांक box_सूचकांक = entry_सूचकांक(l, b, mask_n*l.w*l.h + j*l.w + i, 0);
                भग्न iou = delta_yolo_box(truth, l.output, l.biases, best_n, box_सूचकांक, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);

                पूर्णांक obj_सूचकांक = entry_सूचकांक(l, b, mask_n*l.w*l.h + j*l.w + i, 4);
                avg_obj += l.output[obj_सूचकांक];
                l.delta[obj_सूचकांक] = 1 - l.output[obj_सूचकांक];

                पूर्णांक class = net.truth[t*(4 + 1) + b*l.truths + 4];
                अगर (l.map) class = l.map[class];
                पूर्णांक class_सूचकांक = entry_सूचकांक(l, b, mask_n*l.w*l.h + j*l.w + i, 4 + 1);
                delta_yolo_class(l.output, l.delta, class_सूचकांक, class, l.classes, l.w*l.h, &avg_cat);

                ++गिनती;
                ++class_गिनती;
                अगर(iou > .5) recall += 1;
                अगर(iou > .75) recall75 += 1;
                avg_iou += iou;
            }
        }
    }
    *(l.cost) = घात(mag_array(l.delta, l.outputs * l.batch), 2);
    म_लिखो("Region %d Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, .5R: %f, .75R: %f,  count: %d\n", net.सूचकांक, avg_iou/गिनती, avg_cat/class_गिनती, avg_obj/गिनती, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/गिनती, recall75/गिनती, गिनती);
}

व्योम backward_yolo_layer(स्थिर layer l, network net)
{
   axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

व्योम correct_yolo_boxes(detection *dets, पूर्णांक n, पूर्णांक w, पूर्णांक h, पूर्णांक netw, पूर्णांक neth, पूर्णांक relative)
{
    पूर्णांक i;
    पूर्णांक new_w=0;
    पूर्णांक new_h=0;
    अगर (((भग्न)netw/w) < ((भग्न)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } अन्यथा {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    क्रम (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((भग्न)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((भग्न)new_h/neth); 
        b.w *= (भग्न)netw/new_w;
        b.h *= (भग्न)neth/new_h;
        अगर(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

पूर्णांक yolo_अंक_detections(layer l, भग्न thresh)
{
    पूर्णांक i, n;
    पूर्णांक गिनती = 0;
    क्रम (i = 0; i < l.w*l.h; ++i){
        क्रम(n = 0; n < l.n; ++n){
            पूर्णांक obj_सूचकांक  = entry_सूचकांक(l, 0, n*l.w*l.h + i, 4);
            अगर(l.output[obj_सूचकांक] > thresh){
                ++गिनती;
            }
        }
    }
    वापस गिनती;
}

व्योम avg_flipped_yolo(layer l)
{
    पूर्णांक i,j,n,z;
    भग्न *flip = l.output + l.outputs;
    क्रम (j = 0; j < l.h; ++j) {
        क्रम (i = 0; i < l.w/2; ++i) {
            क्रम (n = 0; n < l.n; ++n) {
                क्रम(z = 0; z < l.classes + 4 + 1; ++z){
                    पूर्णांक i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                    पूर्णांक i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                    भग्न swap = flip[i1];
                    flip[i1] = flip[i2];
                    flip[i2] = swap;
                    अगर(z == 0){
                        flip[i1] = -flip[i1];
                        flip[i2] = -flip[i2];
                    }
                }
            }
        }
    }
    क्रम(i = 0; i < l.outputs; ++i){
        l.output[i] = (l.output[i] + flip[i])/2.;
    }
}

पूर्णांक get_yolo_detections(layer l, पूर्णांक w, पूर्णांक h, पूर्णांक netw, पूर्णांक neth, भग्न thresh, पूर्णांक *map, पूर्णांक relative, detection *dets)
{
    पूर्णांक i,j,n;
    भग्न *predictions = l.output;
    अगर (l.batch == 2) avg_flipped_yolo(l);
    पूर्णांक गिनती = 0;
    क्रम (i = 0; i < l.w*l.h; ++i){
        पूर्णांक row = i / l.w;
        पूर्णांक col = i % l.w;
        क्रम(n = 0; n < l.n; ++n){
            पूर्णांक obj_सूचकांक  = entry_सूचकांक(l, 0, n*l.w*l.h + i, 4);
            भग्न objectness = predictions[obj_सूचकांक];
            अगर(objectness <= thresh) जारी;
            पूर्णांक box_सूचकांक  = entry_सूचकांक(l, 0, n*l.w*l.h + i, 0);
            dets[गिनती].bbox = get_yolo_box(predictions, l.biases, l.mask[n], box_सूचकांक, col, row, l.w, l.h, netw, neth, l.w*l.h);
            dets[गिनती].objectness = objectness;
            dets[गिनती].classes = l.classes;
            क्रम(j = 0; j < l.classes; ++j){
                पूर्णांक class_सूचकांक = entry_सूचकांक(l, 0, n*l.w*l.h + i, 4 + 1 + j);
                भग्न prob = objectness*predictions[class_सूचकांक];
                dets[गिनती].prob[j] = (prob > thresh) ? prob : 0;
            }
            ++गिनती;
        }
    }
    correct_yolo_boxes(dets, गिनती, w, h, netw, neth, relative);
    वापस गिनती;
}

#अगर_घोषित GPU

व्योम forward_yolo_layer_gpu(स्थिर layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.निवेश_gpu, 1, l.output_gpu, 1);
    पूर्णांक b, n;
    क्रम (b = 0; b < l.batch; ++b){
        क्रम(n = 0; n < l.n; ++n){
            पूर्णांक सूचकांक = entry_सूचकांक(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + सूचकांक, 2*l.w*l.h, LOGISTIC);
            सूचकांक = entry_सूचकांक(l, b, n*l.w*l.h, 4);
            activate_array_gpu(l.output_gpu + सूचकांक, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
    अगर(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        वापस;
    }

    cuda_pull_array(l.output_gpu, net.निवेश, l.batch*l.inputs);
    forward_yolo_layer(l, net);
    cuda_धकेलो_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

व्योम backward_yolo_layer_gpu(स्थिर layer l, network net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#पूर्ण_अगर


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

layer make_yolo_layer(पूर्णाङ्क batch, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क n, पूर्णाङ्क योग_2, पूर्णाङ्क *आवरणम्, पूर्णाङ्क classes)
{
    पूर्णाङ्क i;
    layer l = {0};
    l.type = YOLO;

    l.n = n;
    l.योग_2 = योग_2;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = n*(classes + 4 + 1);
    l.बहिर्गमनम्_w = l.w;
    l.बहिर्गमनम्_h = l.h;
    l.बहिर्गमनम्_c = l.c;
    l.classes = classes;
    l.cost = शून्यस्मृतिं_देहि(1, परिमाण(भिन्न));
    l.biases = शून्यस्मृतिं_देहि(योग_2*2, परिमाण(भिन्न));
    यदि(आवरणम्) l.आवरणम् = आवरणम्;
    अन्यथा{
        l.आवरणम् = शून्यस्मृतिं_देहि(n, परिमाण(पूर्णाङ्क));
        प्रति(i = 0; i < n; ++i){
            l.आवरणम्[i] = i;
        }
    }
    l.bias_updates = शून्यस्मृतिं_देहि(n*2, परिमाण(भिन्न));
    l.outputs = h*w*n*(classes + 4 + 1);
    l.inputs = l.outputs;
    l.truths = 90*(4 + 1);
    l.delta = शून्यस्मृतिं_देहि(batch*l.outputs, परिमाण(भिन्न));
    l.output = शून्यस्मृतिं_देहि(batch*l.outputs, परिमाण(भिन्न));
    प्रति(i = 0; i < योग_2*2; ++i){
        l.biases[i] = .5;
    }

    l.forward = forward_yolo_layer;
    l.backward = backward_yolo_layer;
#यदि_परिभाषितम् GPU
    l.forward_gpu = forward_yolo_layer_gpu;
    l.backward_gpu = backward_yolo_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#यदि_अन्तः

    fprintf(stderr, "yolo\n");
    बीजं_रोपय(0);

    प्रत्यागम l;
}

शून्य resize_yolo_layer(layer *l, पूर्णाङ्क w, पूर्णाङ्क h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + 4 + 1);
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*परिमाण(भिन्न));
    l->delta = realloc(l->delta, l->batch*l->outputs*परिमाण(भिन्न));

#यदि_परिभाषितम् GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#यदि_अन्तः
}

box get_yolo_box(भिन्न *x, भिन्न *biases, पूर्णाङ्क n, पूर्णाङ्क सूचकांक, पूर्णाङ्क i, पूर्णाङ्क j, पूर्णाङ्क lw, पूर्णाङ्क lh, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क stride)
{
    box b;
    b.x = (i + x[सूचकांक + 0*stride]) / lw;
    b.y = (j + x[सूचकांक + 1*stride]) / lh;
    b.w = घातांकः(x[सूचकांक + 2*stride]) * biases[2*n]   / w;
    b.h = घातांकः(x[सूचकांक + 3*stride]) * biases[2*n+1] / h;
    प्रत्यागम b;
}

भिन्न delta_yolo_box(box truth, भिन्न *x, भिन्न *biases, पूर्णाङ्क n, पूर्णाङ्क सूचकांक, पूर्णाङ्क i, पूर्णाङ्क j, पूर्णाङ्क lw, पूर्णाङ्क lh, पूर्णाङ्क w, पूर्णाङ्क h, भिन्न *delta, भिन्न scale, पूर्णाङ्क stride)
{
    box pred = get_yolo_box(x, biases, n, सूचकांक, i, j, lw, lh, w, h, stride);
    भिन्न iou = box_iou(pred, truth);

    भिन्न tx = (truth.x*lw - i);
    भिन्न ty = (truth.y*lh - j);
    भिन्न tw = लघुगणकः(truth.w*w / biases[2*n]);
    भिन्न th = लघुगणकः(truth.h*h / biases[2*n + 1]);

    delta[सूचकांक + 0*stride] = scale * (tx - x[सूचकांक + 0*stride]);
    delta[सूचकांक + 1*stride] = scale * (ty - x[सूचकांक + 1*stride]);
    delta[सूचकांक + 2*stride] = scale * (tw - x[सूचकांक + 2*stride]);
    delta[सूचकांक + 3*stride] = scale * (th - x[सूचकांक + 3*stride]);
    प्रत्यागम iou;
}


शून्य delta_yolo_class(भिन्न *output, भिन्न *delta, पूर्णाङ्क सूचकांक, पूर्णाङ्क class, पूर्णाङ्क classes, पूर्णाङ्क stride, भिन्न *avg_cat)
{
    पूर्णाङ्क n;
    यदि (delta[सूचकांक]){
        delta[सूचकांक + stride*class] = 1 - output[सूचकांक + stride*class];
        यदि(avg_cat) *avg_cat += output[सूचकांक + stride*class];
        प्रत्यागम;
    }
    प्रति(n = 0; n < classes; ++n){
        delta[सूचकांक + stride*n] = ((n == class)?1 : 0) - output[सूचकांक + stride*n];
        यदि(n == class && avg_cat) *avg_cat += output[सूचकांक + stride*n];
    }
}

स्थिर पूर्णाङ्क प्रविष्टिः_सूचकांक(layer l, पूर्णाङ्क batch, पूर्णाङ्क location, पूर्णाङ्क प्रविष्टिः)
{
    पूर्णाङ्क n =   location / (l.w*l.h);
    पूर्णाङ्क loc = location % (l.w*l.h);
    प्रत्यागम batch*l.outputs + n*l.w*l.h*(4+l.classes+1) + प्रविष्टिः*l.w*l.h + loc;
}

शून्य forward_yolo_layer(ध्रुव layer l, जालम् net)
{
    पूर्णाङ्क i,j,b,t,n;
    स्मृतिं_प्रतिलिख(l.output, net.निवेशः, l.outputs*l.batch*परिमाण(भिन्न));

#यदि_अपरिभाषितम् GPU
    प्रति (b = 0; b < l.batch; ++b){
        प्रति(n = 0; n < l.n; ++n){
            पूर्णाङ्क सूचकांक = प्रविष्टिः_सूचकांक(l, b, n*l.w*l.h, 0);
            activate_array(l.output + सूचकांक, 2*l.w*l.h, LOGISTIC);
            सूचकांक = प्रविष्टिः_सूचकांक(l, b, n*l.w*l.h, 4);
            activate_array(l.output + सूचकांक, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
#यदि_अन्तः

    स्मृतिं_पूरय(l.delta, 0, l.outputs * l.batch * परिमाण(भिन्न));
    यदि(!net.train) प्रत्यागम;
    भिन्न avg_iou = 0;
    भिन्न recall = 0;
    भिन्न recall75 = 0;
    भिन्न avg_cat = 0;
    भिन्न avg_obj = 0;
    भिन्न avg_anyobj = 0;
    पूर्णाङ्क गणनम् = 0;
    पूर्णाङ्क class_गणनम् = 0;
    *(l.cost) = 0;
    प्रति (b = 0; b < l.batch; ++b) {
        प्रति (j = 0; j < l.h; ++j) {
            प्रति (i = 0; i < l.w; ++i) {
                प्रति (n = 0; n < l.n; ++n) {
                    पूर्णाङ्क box_सूचकांक = प्रविष्टिः_सूचकांक(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_yolo_box(l.output, l.biases, l.आवरणम्[n], box_सूचकांक, i, j, l.w, l.h, net.w, net.h, l.w*l.h);
                    भिन्न best_iou = 0;
                    पूर्णाङ्क best_t = 0;
                    प्रति(t = 0; t < l.max_boxes; ++t){
                        box truth = float_गन्तव्यम्_box(net.truth + t*(4 + 1) + b*l.truths, 1);
                        यदि(!truth.x) भङ्ग;
                        भिन्न iou = box_iou(pred, truth);
                        यदि (iou > best_iou) {
                            best_iou = iou;
                            best_t = t;
                        }
                    }
                    पूर्णाङ्क obj_सूचकांक = प्रविष्टिः_सूचकांक(l, b, n*l.w*l.h + j*l.w + i, 4);
                    avg_anyobj += l.output[obj_सूचकांक];
                    l.delta[obj_सूचकांक] = 0 - l.output[obj_सूचकांक];
                    यदि (best_iou > l.ignore_thresh) {
                        l.delta[obj_सूचकांक] = 0;
                    }
                    यदि (best_iou > l.truth_thresh) {
                        l.delta[obj_सूचकांक] = 1 - l.output[obj_सूचकांक];

                        पूर्णाङ्क class = net.truth[best_t*(4 + 1) + b*l.truths + 4];
                        यदि (l.map) class = l.map[class];
                        पूर्णाङ्क class_सूचकांक = प्रविष्टिः_सूचकांक(l, b, n*l.w*l.h + j*l.w + i, 4 + 1);
                        delta_yolo_class(l.output, l.delta, class_सूचकांक, class, l.classes, l.w*l.h, 0);
                        box truth = float_गन्तव्यम्_box(net.truth + best_t*(4 + 1) + b*l.truths, 1);
                        delta_yolo_box(truth, l.output, l.biases, l.आवरणम्[n], box_सूचकांक, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);
                    }
                }
            }
        }
        प्रति(t = 0; t < l.max_boxes; ++t){
            box truth = float_गन्तव्यम्_box(net.truth + t*(4 + 1) + b*l.truths, 1);

            यदि(!truth.x) भङ्ग;
            भिन्न best_iou = 0;
            पूर्णाङ्क best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_सरणम् = truth;
            truth_सरणम्.x = truth_सरणम्.y = 0;
            प्रति(n = 0; n < l.योग_2; ++n){
                box pred = {0};
                pred.w = l.biases[2*n]/net.w;
                pred.h = l.biases[2*n+1]/net.h;
                भिन्न iou = box_iou(pred, truth_सरणम्);
                यदि (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            पूर्णाङ्क आवरणम्_n = int_सूचकांक(l.आवरणम्, best_n, l.n);
            यदि(आवरणम्_n >= 0){
                पूर्णाङ्क box_सूचकांक = प्रविष्टिः_सूचकांक(l, b, आवरणम्_n*l.w*l.h + j*l.w + i, 0);
                भिन्न iou = delta_yolo_box(truth, l.output, l.biases, best_n, box_सूचकांक, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);

                पूर्णाङ्क obj_सूचकांक = प्रविष्टिः_सूचकांक(l, b, आवरणम्_n*l.w*l.h + j*l.w + i, 4);
                avg_obj += l.output[obj_सूचकांक];
                l.delta[obj_सूचकांक] = 1 - l.output[obj_सूचकांक];

                पूर्णाङ्क class = net.truth[t*(4 + 1) + b*l.truths + 4];
                यदि (l.map) class = l.map[class];
                पूर्णाङ्क class_सूचकांक = प्रविष्टिः_सूचकांक(l, b, आवरणम्_n*l.w*l.h + j*l.w + i, 4 + 1);
                delta_yolo_class(l.output, l.delta, class_सूचकांक, class, l.classes, l.w*l.h, &avg_cat);

                ++गणनम्;
                ++class_गणनम्;
                यदि(iou > .5) recall += 1;
                यदि(iou > .75) recall75 += 1;
                avg_iou += iou;
            }
        }
    }
    *(l.cost) = घातः(mag_array(l.delta, l.outputs * l.batch), 2);
    प्रारूप_लिख("Region %d Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, .5R: %f, .75R: %f,  count: %d\n", net.सूचकांक, avg_iou/गणनम्, avg_cat/class_गणनम्, avg_obj/गणनम्, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/गणनम्, recall75/गणनम्, गणनम्);
}

शून्य backward_yolo_layer(ध्रुव layer l, जालम् net)
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
    प्रति (i = 0; i < n; ++i){
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

पूर्णाङ्क yolo_अङ्कः_detections(layer l, भिन्न thresh)
{
    पूर्णाङ्क i, n;
    पूर्णाङ्क गणनम् = 0;
    प्रति (i = 0; i < l.w*l.h; ++i){
        प्रति(n = 0; n < l.n; ++n){
            पूर्णाङ्क obj_सूचकांक  = प्रविष्टिः_सूचकांक(l, 0, n*l.w*l.h + i, 4);
            यदि(l.output[obj_सूचकांक] > thresh){
                ++गणनम्;
            }
        }
    }
    प्रत्यागम गणनम्;
}

शून्य avg_flipped_yolo(layer l)
{
    पूर्णाङ्क i,j,n,z;
    भिन्न *flip = l.output + l.outputs;
    प्रति (j = 0; j < l.h; ++j) {
        प्रति (i = 0; i < l.w/2; ++i) {
            प्रति (n = 0; n < l.n; ++n) {
                प्रति(z = 0; z < l.classes + 4 + 1; ++z){
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
    प्रति(i = 0; i < l.outputs; ++i){
        l.output[i] = (l.output[i] + flip[i])/2.;
    }
}

पूर्णाङ्क get_yolo_detections(layer l, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क netw, पूर्णाङ्क neth, भिन्न thresh, पूर्णाङ्क *map, पूर्णाङ्क relative, detection *dets)
{
    पूर्णाङ्क i,j,n;
    भिन्न *predictions = l.output;
    यदि (l.batch == 2) avg_flipped_yolo(l);
    पूर्णाङ्क गणनम् = 0;
    प्रति (i = 0; i < l.w*l.h; ++i){
        पूर्णाङ्क row = i / l.w;
        पूर्णाङ्क col = i % l.w;
        प्रति(n = 0; n < l.n; ++n){
            पूर्णाङ्क obj_सूचकांक  = प्रविष्टिः_सूचकांक(l, 0, n*l.w*l.h + i, 4);
            भिन्न objectness = predictions[obj_सूचकांक];
            यदि(objectness <= thresh) अनुवर्त;
            पूर्णाङ्क box_सूचकांक  = प्रविष्टिः_सूचकांक(l, 0, n*l.w*l.h + i, 0);
            dets[गणनम्].bbox = get_yolo_box(predictions, l.biases, l.आवरणम्[n], box_सूचकांक, col, row, l.w, l.h, netw, neth, l.w*l.h);
            dets[गणनम्].objectness = objectness;
            dets[गणनम्].classes = l.classes;
            प्रति(j = 0; j < l.classes; ++j){
                पूर्णाङ्क class_सूचकांक = प्रविष्टिः_सूचकांक(l, 0, n*l.w*l.h + i, 4 + 1 + j);
                भिन्न prob = objectness*predictions[class_सूचकांक];
                dets[गणनम्].prob[j] = (prob > thresh) ? prob : 0;
            }
            ++गणनम्;
        }
    }
    correct_yolo_boxes(dets, गणनम्, w, h, netw, neth, relative);
    प्रत्यागम गणनम्;
}

#यदि_परिभाषितम् GPU

शून्य forward_yolo_layer_gpu(ध्रुव layer l, जालम् net)
{
    copy_gpu(l.batch*l.inputs, net.निवेशः_gpu, 1, l.output_gpu, 1);
    पूर्णाङ्क b, n;
    प्रति (b = 0; b < l.batch; ++b){
        प्रति(n = 0; n < l.n; ++n){
            पूर्णाङ्क सूचकांक = प्रविष्टिः_सूचकांक(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + सूचकांक, 2*l.w*l.h, LOGISTIC);
            सूचकांक = प्रविष्टिः_सूचकांक(l, b, n*l.w*l.h, 4);
            activate_array_gpu(l.output_gpu + सूचकांक, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
    यदि(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        प्रत्यागम;
    }

    cuda_pull_array(l.output_gpu, net.निवेशः, l.batch*l.inputs);
    forward_yolo_layer(l, net);
    cuda_प्रक्षिप_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

शून्य backward_yolo_layer_gpu(ध्रुव layer l, जालम् net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#यदि_अन्तः


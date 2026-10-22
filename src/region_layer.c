#अन्तर्भाव "region_layer.h"
#अन्तर्भाव "activations.h"
#अन्तर्भाव "blas.h"
#अन्तर्भाव "box.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव "utils.h"

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <assert.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <stdlib.h>

layer make_region_layer(पूर्णाङ्क batch, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क n, पूर्णाङ्क classes, पूर्णाङ्क coords)
{
    layer l = {0};
    l.type = REGION;

    l.n = n;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = n*(classes + coords + 1);
    l.out_w = l.w;
    l.out_h = l.h;
    l.out_c = l.c;
    l.classes = classes;
    l.coords = coords;
    l.cost = सुस्मृति(1, परिमाण(भिन्न));
    l.biases = सुस्मृति(n*2, परिमाण(भिन्न));
    l.bias_updates = सुस्मृति(n*2, परिमाण(भिन्न));
    l.outputs = h*w*n*(classes + coords + 1);
    l.inputs = l.outputs;
    l.truths = 30*(l.coords + 1);
    l.delta = सुस्मृति(batch*l.outputs, परिमाण(भिन्न));
    l.output = सुस्मृति(batch*l.outputs, परिमाण(भिन्न));
    पूर्णाङ्क i;
    लागि(i = 0; i < n*2; ++i){
        l.biases[i] = .5;
    }

    l.forward = forward_region_layer;
    l.backward = backward_region_layer;
#ifdef GPU
    l.forward_gpu = forward_region_layer_gpu;
    l.backward_gpu = backward_region_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#endif

    ख_लिखो(मानक_त्रुटि, "detection\n");
    बेक्रम(0);

    फर्काऊ l;
}

शून्य resize_region_layer(layer *l, पूर्णाङ्क w, पूर्णाङ्क h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + l->coords + 1);
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

box get_region_box(भिन्न *x, भिन्न *biases, पूर्णाङ्क n, पूर्णाङ्क index, पूर्णाङ्क i, पूर्णाङ्क j, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क stride)
{
    box b;
    b.x = (i + x[index + 0*stride]) / w;
    b.y = (j + x[index + 1*stride]) / h;
    b.w = exp(x[index + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[index + 3*stride]) * biases[2*n+1] / h;
    फर्काऊ b;
}

भिन्न delta_region_box(box truth, भिन्न *x, भिन्न *biases, पूर्णाङ्क n, पूर्णाङ्क index, पूर्णाङ्क i, पूर्णाङ्क j, पूर्णाङ्क w, पूर्णाङ्क h, भिन्न *delta, भिन्न scale, पूर्णाङ्क stride)
{
    box pred = get_region_box(x, biases, n, index, i, j, w, h, stride);
    भिन्न iou = box_iou(pred, truth);

    भिन्न tx = (truth.x*w - i);
    भिन्न ty = (truth.y*h - j);
    भिन्न tw = log(truth.w*w / biases[2*n]);
    भिन्न th = log(truth.h*h / biases[2*n + 1]);

    delta[index + 0*stride] = scale * (tx - x[index + 0*stride]);
    delta[index + 1*stride] = scale * (ty - x[index + 1*stride]);
    delta[index + 2*stride] = scale * (tw - x[index + 2*stride]);
    delta[index + 3*stride] = scale * (th - x[index + 3*stride]);
    फर्काऊ iou;
}

शून्य delta_region_mask(भिन्न *truth, भिन्न *x, पूर्णाङ्क n, पूर्णाङ्क index, भिन्न *delta, पूर्णाङ्क stride, पूर्णाङ्क scale)
{
    पूर्णाङ्क i;
    लागि(i = 0; i < n; ++i){
        delta[index + i*stride] = scale*(truth[i] - x[index + i*stride]);
    }
}


शून्य delta_region_class(भिन्न *output, भिन्न *delta, पूर्णाङ्क index, पूर्णाङ्क class, पूर्णाङ्क classes, रूख *hier, भिन्न scale, पूर्णाङ्क stride, भिन्न *avg_cat, पूर्णाङ्क tag)
{
    पूर्णाङ्क i, n;
    यदि(hier){
        भिन्न pred = 1;
        जबसम्म(class >= 0){
            pred *= output[index + stride*class];
            पूर्णाङ्क g = hier->group[class];
            पूर्णाङ्क offset = hier->group_offset[g];
            लागि(i = 0; i < hier->group_size[g]; ++i){
                delta[index + stride*(offset + i)] = scale * (0 - output[index + stride*(offset + i)]);
            }
            delta[index + stride*class] = scale * (1 - output[index + stride*class]);

            class = hier->parent[class];
        }
        *avg_cat += pred;
    } अन्यथा {
        यदि (delta[index] && tag){
            delta[index + stride*class] = scale * (1 - output[index + stride*class]);
            फर्काऊ;
        }
        लागि(n = 0; n < classes; ++n){
            delta[index + stride*n] = scale * (((n == class)?1 : 0) - output[index + stride*n]);
            यदि(n == class) *avg_cat += output[index + stride*n];
        }
    }
}

भिन्न logit(भिन्न x)
{
    फर्काऊ log(x/(1.-x));
}

भिन्न tisnan(भिन्न x)
{
    फर्काऊ (x != x);
}

पूर्णाङ्क entry_index(layer l, पूर्णाङ्क batch, पूर्णाङ्क location, पूर्णाङ्क entry)
{
    पूर्णाङ्क n =   location / (l.w*l.h);
    पूर्णाङ्क loc = location % (l.w*l.h);
    फर्काऊ batch*l.outputs + n*l.w*l.h*(l.coords+l.classes+1) + entry*l.w*l.h + loc;
}

शून्य forward_region_layer(अचर layer l, network net)
{
    पूर्णाङ्क i,j,b,t,n;
    स_नकल(l.output, net.आगत, l.outputs*l.batch*परिमाण(भिन्न));

#ifndef GPU
    लागि (b = 0; b < l.batch; ++b){
        लागि(n = 0; n < l.n; ++n){
            पूर्णाङ्क index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array(l.output + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            यदि(!l.background) activate_array(l.output + index,   l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords + 1);
            यदि(!l.softmax && !l.softmax_रूख) activate_array(l.output + index, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    यदि (l.softmax_रूख){
        पूर्णाङ्क i;
        पूर्णाङ्क गन्ती = l.coords + 1;
        लागि (i = 0; i < l.softmax_रूख->groups; ++i) {
            पूर्णाङ्क group_size = l.softmax_रूख->group_size[i];
            softmax_cpu(net.आगत + गन्ती, group_size, l.batch, l.inputs, l.n*l.w*l.h, 1, l.n*l.w*l.h, l.temperature, l.output + गन्ती);
            गन्ती += group_size;
        }
    } अन्यथा यदि (l.softmax){
        पूर्णाङ्क index = entry_index(l, 0, 0, l.coords + !l.background);
        softmax_cpu(net.आगत + index, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output + index);
    }
#endif

    स_रखो(l.delta, 0, l.outputs * l.batch * परिमाण(भिन्न));
    यदि(!net.train) फर्काऊ;
    भिन्न avg_iou = 0;
    भिन्न recall = 0;
    भिन्न avg_cat = 0;
    भिन्न avg_obj = 0;
    भिन्न avg_anyobj = 0;
    पूर्णाङ्क गन्ती = 0;
    पूर्णाङ्क class_गन्ती = 0;
    *(l.cost) = 0;
    लागि (b = 0; b < l.batch; ++b) {
        यदि(l.softmax_रूख){
            पूर्णाङ्क onlyclass = 0;
            लागि(t = 0; t < 30; ++t){
                box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                यदि(!truth.x) रोक;
                पूर्णाङ्क class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
                भिन्न maxp = 0;
                पूर्णाङ्क maxi = 0;
                यदि(truth.x > 100000 && truth.y > 100000){
                    लागि(n = 0; n < l.n*l.w*l.h; ++n){
                        पूर्णाङ्क class_index = entry_index(l, b, n, l.coords + 1);
                        पूर्णाङ्क obj_index = entry_index(l, b, n, l.coords);
                        भिन्न scale =  l.output[obj_index];
                        l.delta[obj_index] = l.noobject_scale * (0 - l.output[obj_index]);
                        भिन्न p = scale*get_hierarchy_probability(l.output + class_index, l.softmax_रूख, class, l.w*l.h);
                        यदि(p > maxp){
                            maxp = p;
                            maxi = n;
                        }
                    }
                    पूर्णाङ्क class_index = entry_index(l, b, maxi, l.coords + 1);
                    पूर्णाङ्क obj_index = entry_index(l, b, maxi, l.coords);
                    delta_region_class(l.output, l.delta, class_index, class, l.classes, l.softmax_रूख, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
                    यदि(l.output[obj_index] < .3) l.delta[obj_index] = l.object_scale * (.3 - l.output[obj_index]);
                    अन्यथा  l.delta[obj_index] = 0;
                    l.delta[obj_index] = 0;
                    ++class_गन्ती;
                    onlyclass = 1;
                    रोक;
                }
            }
            यदि(onlyclass) जारी;
        }
        लागि (j = 0; j < l.h; ++j) {
            लागि (i = 0; i < l.w; ++i) {
                लागि (n = 0; n < l.n; ++n) {
                    पूर्णाङ्क box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_region_box(l.output, l.biases, n, box_index, i, j, l.w, l.h, l.w*l.h);
                    भिन्न best_iou = 0;
                    लागि(t = 0; t < 30; ++t){
                        box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                        यदि(!truth.x) रोक;
                        भिन्न iou = box_iou(pred, truth);
                        यदि (iou > best_iou) {
                            best_iou = iou;
                        }
                    }
                    पूर्णाङ्क obj_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, l.coords);
                    avg_anyobj += l.output[obj_index];
                    l.delta[obj_index] = l.noobject_scale * (0 - l.output[obj_index]);
                    यदि(l.background) l.delta[obj_index] = l.noobject_scale * (1 - l.output[obj_index]);
                    यदि (best_iou > l.thresh) {
                        l.delta[obj_index] = 0;
                    }

                    यदि(*(net.seen) < 12800){
                        box truth = {0};
                        truth.x = (i + .5)/l.w;
                        truth.y = (j + .5)/l.h;
                        truth.w = l.biases[2*n]/l.w;
                        truth.h = l.biases[2*n+1]/l.h;
                        delta_region_box(truth, l.output, l.biases, n, box_index, i, j, l.w, l.h, l.delta, .01, l.w*l.h);
                    }
                }
            }
        }
        लागि(t = 0; t < 30; ++t){
            box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);

            यदि(!truth.x) रोक;
            भिन्न best_iou = 0;
            पूर्णाङ्क best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = 0;
            truth_shift.y = 0;
            लागि(n = 0; n < l.n; ++n){
                पूर्णाङ्क box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                box pred = get_region_box(l.output, l.biases, n, box_index, i, j, l.w, l.h, l.w*l.h);
                यदि(l.bias_match){
                    pred.w = l.biases[2*n]/l.w;
                    pred.h = l.biases[2*n+1]/l.h;
                }
                pred.x = 0;
                pred.y = 0;
                भिन्न iou = box_iou(pred, truth_shift);
                यदि (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            पूर्णाङ्क box_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, 0);
            भिन्न iou = delta_region_box(truth, l.output, l.biases, best_n, box_index, i, j, l.w, l.h, l.delta, l.coord_scale *  (2 - truth.w*truth.h), l.w*l.h);
            यदि(l.coords > 4){
                पूर्णाङ्क mask_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, 4);
                delta_region_mask(net.truth + t*(l.coords + 1) + b*l.truths + 5, l.output, l.coords - 4, mask_index, l.delta, l.w*l.h, l.mask_scale);
            }
            यदि(iou > .5) recall += 1;
            avg_iou += iou;

            पूर्णाङ्क obj_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, l.coords);
            avg_obj += l.output[obj_index];
            l.delta[obj_index] = l.object_scale * (1 - l.output[obj_index]);
            यदि (l.rescore) {
                l.delta[obj_index] = l.object_scale * (iou - l.output[obj_index]);
            }
            यदि(l.background){
                l.delta[obj_index] = l.object_scale * (0 - l.output[obj_index]);
            }

            पूर्णाङ्क class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
            यदि (l.map) class = l.map[class];
            पूर्णाङ्क class_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, l.coords + 1);
            delta_region_class(l.output, l.delta, class_index, class, l.classes, l.softmax_रूख, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
            ++गन्ती;
            ++class_गन्ती;
        }
    }
    *(l.cost) = घात(mag_array(l.delta, l.outputs * l.batch), 2);
    म_लिखो("Region Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, Avg Recall: %f,  count: %d\n", avg_iou/गन्ती, avg_cat/class_गन्ती, avg_obj/गन्ती, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/गन्ती, गन्ती);
}

शून्य backward_region_layer(अचर layer l, network net)
{
    /*
       int b;
       int size = l.coords + l.classes + 1;
       for (b = 0; b < l.batch*l.n; ++b){
       int index = (b*size + 4)*l.w*l.h;
       gradient_array(l.output + index, l.w*l.h, LOGISTIC, l.delta + index);
       }
       axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
     */
}

शून्य correct_region_boxes(detection *dets, पूर्णाङ्क n, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क netw, पूर्णाङ्क neth, पूर्णाङ्क relative)
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

शून्य get_region_detections(layer l, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क netw, पूर्णाङ्क neth, भिन्न thresh, पूर्णाङ्क *map, भिन्न रूख_thresh, पूर्णाङ्क relative, detection *dets)
{
    पूर्णाङ्क i,j,n,z;
    भिन्न *predictions = l.output;
    यदि (l.batch == 2) {
        भिन्न *flip = l.output + l.outputs;
        लागि (j = 0; j < l.h; ++j) {
            लागि (i = 0; i < l.w/2; ++i) {
                लागि (n = 0; n < l.n; ++n) {
                    लागि(z = 0; z < l.classes + l.coords + 1; ++z){
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
    लागि (i = 0; i < l.w*l.h; ++i){
        पूर्णाङ्क row = i / l.w;
        पूर्णाङ्क col = i % l.w;
        लागि(n = 0; n < l.n; ++n){
            पूर्णाङ्क index = n*l.w*l.h + i;
            लागि(j = 0; j < l.classes; ++j){
                dets[index].prob[j] = 0;
            }
            पूर्णाङ्क obj_index  = entry_index(l, 0, n*l.w*l.h + i, l.coords);
            पूर्णाङ्क box_index  = entry_index(l, 0, n*l.w*l.h + i, 0);
            पूर्णाङ्क mask_index = entry_index(l, 0, n*l.w*l.h + i, 4);
            भिन्न scale = l.background ? 1 : predictions[obj_index];
            dets[index].bbox = get_region_box(predictions, l.biases, n, box_index, col, row, l.w, l.h, l.w*l.h);
            dets[index].objectness = scale > thresh ? scale : 0;
            यदि(dets[index].mask){
                लागि(j = 0; j < l.coords - 4; ++j){
                    dets[index].mask[j] = l.output[mask_index + j*l.w*l.h];
                }
            }

            पूर्णाङ्क class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + !l.background);
            यदि(l.softmax_रूख){

                hierarchy_predictions(predictions + class_index, l.classes, l.softmax_रूख, 0, l.w*l.h);
                यदि(map){
                    लागि(j = 0; j < 200; ++j){
                        पूर्णाङ्क class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + 1 + map[j]);
                        भिन्न prob = scale*predictions[class_index];
                        dets[index].prob[j] = (prob > thresh) ? prob : 0;
                    }
                } अन्यथा {
                    पूर्णाङ्क j =  hierarchy_शीर्ष_prediction(predictions + class_index, l.softmax_रूख, रूख_thresh, l.w*l.h);
                    dets[index].prob[j] = (scale > thresh) ? scale : 0;
                }
            } अन्यथा {
                यदि(dets[index].objectness){
                    लागि(j = 0; j < l.classes; ++j){
                        पूर्णाङ्क class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + 1 + j);
                        भिन्न prob = scale*predictions[class_index];
                        dets[index].prob[j] = (prob > thresh) ? prob : 0;
                    }
                }
            }
        }
    }
    correct_region_boxes(dets, l.w*l.h*l.n, w, h, netw, neth, relative);
}

#ifdef GPU

शून्य forward_region_layer_gpu(अचर layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.आगत_gpu, 1, l.output_gpu, 1);
    पूर्णाङ्क b, n;
    लागि (b = 0; b < l.batch; ++b){
        लागि(n = 0; n < l.n; ++n){
            पूर्णाङ्क index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC);
            यदि(l.coords > 4){
                index = entry_index(l, b, n*l.w*l.h, 4);
                activate_array_gpu(l.output_gpu + index, (l.coords - 4)*l.w*l.h, LOGISTIC);
            }
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            यदि(!l.background) activate_array_gpu(l.output_gpu + index,   l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords + 1);
            यदि(!l.softmax && !l.softmax_रूख) activate_array_gpu(l.output_gpu + index, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    यदि (l.softmax_रूख){
        पूर्णाङ्क index = entry_index(l, 0, 0, l.coords + 1);
        softmax_रूख(net.आगत_gpu + index, l.w*l.h, l.batch*l.n, l.inputs/l.n, 1, l.output_gpu + index, *l.softmax_रूख);
    } अन्यथा यदि (l.softmax) {
        पूर्णाङ्क index = entry_index(l, 0, 0, l.coords + !l.background);
        softmax_gpu(net.आगत_gpu + index, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu + index);
    }
    यदि(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        फर्काऊ;
    }

    cuda_pull_array(l.output_gpu, net.आगत, l.batch*l.inputs);
    forward_region_layer(l, net);
    //cuda_push_array(l.output_gpu, l.output, l.batch*l.outputs);
    यदि(!net.train) फर्काऊ;
    cuda_थप_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

शून्य backward_region_layer_gpu(अचर layer l, network net)
{
    पूर्णाङ्क b, n;
    लागि (b = 0; b < l.batch; ++b){
        लागि(n = 0; n < l.n; ++n){
            पूर्णाङ्क index = entry_index(l, b, n*l.w*l.h, 0);
            gradient_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC, l.delta_gpu + index);
            यदि(l.coords > 4){
                index = entry_index(l, b, n*l.w*l.h, 4);
                gradient_array_gpu(l.output_gpu + index, (l.coords - 4)*l.w*l.h, LOGISTIC, l.delta_gpu + index);
            }
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            यदि(!l.background) gradient_array_gpu(l.output_gpu + index,   l.w*l.h, LOGISTIC, l.delta_gpu + index);
        }
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif

शून्य zero_objectness(layer l)
{
    पूर्णाङ्क i, n;
    लागि (i = 0; i < l.w*l.h; ++i){
        लागि(n = 0; n < l.n; ++n){
            पूर्णाङ्क obj_index = entry_index(l, 0, n*l.w*l.h + i, l.coords);
            l.output[obj_index] = 0;
        }
    }
}


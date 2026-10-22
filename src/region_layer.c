#समावेश "region_layer.h"
#समावेश "activations.h"
#समावेश "blas.h"
#समावेश "box.h"
#समावेश "cuda.h"
#समावेश "utils.h"

#समावेश <stdio.h>
#समावेश <assert.h>
#समावेश <string.h>
#समावेश <stdlib.h>

layer make_region_layer(पूर्णांक batch, पूर्णांक w, पूर्णांक h, पूर्णांक n, पूर्णांक classes, पूर्णांक coords)
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
    l.cost = सुस्मृति(1, माप(भग्न));
    l.biases = सुस्मृति(n*2, माप(भग्न));
    l.bias_updates = सुस्मृति(n*2, माप(भग्न));
    l.outputs = h*w*n*(classes + coords + 1);
    l.inputs = l.outputs;
    l.truths = 30*(l.coords + 1);
    l.delta = सुस्मृति(batch*l.outputs, माप(भग्न));
    l.output = सुस्मृति(batch*l.outputs, माप(भग्न));
    पूर्णांक i;
    क्रम(i = 0; i < n*2; ++i){
        l.biases[i] = .5;
    }

    l.forward = forward_region_layer;
    l.backward = backward_region_layer;
#अगर_घोषित GPU
    l.forward_gpu = forward_region_layer_gpu;
    l.backward_gpu = backward_region_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#पूर्ण_अगर

    ख_लिखो(मानक_त्रुटि, "detection\n");
    बेक्रम(0);

    वापस l;
}

व्योम resize_region_layer(layer *l, पूर्णांक w, पूर्णांक h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + l->coords + 1);
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

box get_region_box(भग्न *x, भग्न *biases, पूर्णांक n, पूर्णांक सूचकांक, पूर्णांक i, पूर्णांक j, पूर्णांक w, पूर्णांक h, पूर्णांक stride)
{
    box b;
    b.x = (i + x[सूचकांक + 0*stride]) / w;
    b.y = (j + x[सूचकांक + 1*stride]) / h;
    b.w = exp(x[सूचकांक + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[सूचकांक + 3*stride]) * biases[2*n+1] / h;
    वापस b;
}

भग्न delta_region_box(box truth, भग्न *x, भग्न *biases, पूर्णांक n, पूर्णांक सूचकांक, पूर्णांक i, पूर्णांक j, पूर्णांक w, पूर्णांक h, भग्न *delta, भग्न scale, पूर्णांक stride)
{
    box pred = get_region_box(x, biases, n, सूचकांक, i, j, w, h, stride);
    भग्न iou = box_iou(pred, truth);

    भग्न tx = (truth.x*w - i);
    भग्न ty = (truth.y*h - j);
    भग्न tw = log(truth.w*w / biases[2*n]);
    भग्न th = log(truth.h*h / biases[2*n + 1]);

    delta[सूचकांक + 0*stride] = scale * (tx - x[सूचकांक + 0*stride]);
    delta[सूचकांक + 1*stride] = scale * (ty - x[सूचकांक + 1*stride]);
    delta[सूचकांक + 2*stride] = scale * (tw - x[सूचकांक + 2*stride]);
    delta[सूचकांक + 3*stride] = scale * (th - x[सूचकांक + 3*stride]);
    वापस iou;
}

व्योम delta_region_mask(भग्न *truth, भग्न *x, पूर्णांक n, पूर्णांक सूचकांक, भग्न *delta, पूर्णांक stride, पूर्णांक scale)
{
    पूर्णांक i;
    क्रम(i = 0; i < n; ++i){
        delta[सूचकांक + i*stride] = scale*(truth[i] - x[सूचकांक + i*stride]);
    }
}


व्योम delta_region_class(भग्न *output, भग्न *delta, पूर्णांक सूचकांक, पूर्णांक class, पूर्णांक classes, वृक्ष *hier, भग्न scale, पूर्णांक stride, भग्न *avg_cat, पूर्णांक tag)
{
    पूर्णांक i, n;
    अगर(hier){
        भग्न pred = 1;
        जबतक(class >= 0){
            pred *= output[सूचकांक + stride*class];
            पूर्णांक g = hier->group[class];
            पूर्णांक offset = hier->group_offset[g];
            क्रम(i = 0; i < hier->group_आकार[g]; ++i){
                delta[सूचकांक + stride*(offset + i)] = scale * (0 - output[सूचकांक + stride*(offset + i)]);
            }
            delta[सूचकांक + stride*class] = scale * (1 - output[सूचकांक + stride*class]);

            class = hier->parent[class];
        }
        *avg_cat += pred;
    } अन्यथा {
        अगर (delta[सूचकांक] && tag){
            delta[सूचकांक + stride*class] = scale * (1 - output[सूचकांक + stride*class]);
            वापस;
        }
        क्रम(n = 0; n < classes; ++n){
            delta[सूचकांक + stride*n] = scale * (((n == class)?1 : 0) - output[सूचकांक + stride*n]);
            अगर(n == class) *avg_cat += output[सूचकांक + stride*n];
        }
    }
}

भग्न logit(भग्न x)
{
    वापस log(x/(1.-x));
}

भग्न tisnan(भग्न x)
{
    वापस (x != x);
}

पूर्णांक entry_सूचकांक(layer l, पूर्णांक batch, पूर्णांक location, पूर्णांक entry)
{
    पूर्णांक n =   location / (l.w*l.h);
    पूर्णांक loc = location % (l.w*l.h);
    वापस batch*l.outputs + n*l.w*l.h*(l.coords+l.classes+1) + entry*l.w*l.h + loc;
}

व्योम forward_region_layer(स्थिर layer l, network net)
{
    पूर्णांक i,j,b,t,n;
    स_नकल(l.output, net.निवेश, l.outputs*l.batch*माप(भग्न));

#अगर_अघोषित GPU
    क्रम (b = 0; b < l.batch; ++b){
        क्रम(n = 0; n < l.n; ++n){
            पूर्णांक सूचकांक = entry_सूचकांक(l, b, n*l.w*l.h, 0);
            activate_array(l.output + सूचकांक, 2*l.w*l.h, LOGISTIC);
            सूचकांक = entry_सूचकांक(l, b, n*l.w*l.h, l.coords);
            अगर(!l.background) activate_array(l.output + सूचकांक,   l.w*l.h, LOGISTIC);
            सूचकांक = entry_सूचकांक(l, b, n*l.w*l.h, l.coords + 1);
            अगर(!l.softmax && !l.softmax_वृक्ष) activate_array(l.output + सूचकांक, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    अगर (l.softmax_वृक्ष){
        पूर्णांक i;
        पूर्णांक गिनती = l.coords + 1;
        क्रम (i = 0; i < l.softmax_वृक्ष->groups; ++i) {
            पूर्णांक group_आकार = l.softmax_वृक्ष->group_आकार[i];
            softmax_cpu(net.निवेश + गिनती, group_आकार, l.batch, l.inputs, l.n*l.w*l.h, 1, l.n*l.w*l.h, l.temperature, l.output + गिनती);
            गिनती += group_आकार;
        }
    } अन्यथा अगर (l.softmax){
        पूर्णांक सूचकांक = entry_सूचकांक(l, 0, 0, l.coords + !l.background);
        softmax_cpu(net.निवेश + सूचकांक, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output + सूचकांक);
    }
#पूर्ण_अगर

    स_रखो(l.delta, 0, l.outputs * l.batch * माप(भग्न));
    अगर(!net.train) वापस;
    भग्न avg_iou = 0;
    भग्न recall = 0;
    भग्न avg_cat = 0;
    भग्न avg_obj = 0;
    भग्न avg_anyobj = 0;
    पूर्णांक गिनती = 0;
    पूर्णांक class_गिनती = 0;
    *(l.cost) = 0;
    क्रम (b = 0; b < l.batch; ++b) {
        अगर(l.softmax_वृक्ष){
            पूर्णांक onlyclass = 0;
            क्रम(t = 0; t < 30; ++t){
                box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                अगर(!truth.x) अवरोध;
                पूर्णांक class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
                भग्न maxp = 0;
                पूर्णांक maxi = 0;
                अगर(truth.x > 100000 && truth.y > 100000){
                    क्रम(n = 0; n < l.n*l.w*l.h; ++n){
                        पूर्णांक class_सूचकांक = entry_सूचकांक(l, b, n, l.coords + 1);
                        पूर्णांक obj_सूचकांक = entry_सूचकांक(l, b, n, l.coords);
                        भग्न scale =  l.output[obj_सूचकांक];
                        l.delta[obj_सूचकांक] = l.noobject_scale * (0 - l.output[obj_सूचकांक]);
                        भग्न p = scale*get_hierarchy_probability(l.output + class_सूचकांक, l.softmax_वृक्ष, class, l.w*l.h);
                        अगर(p > maxp){
                            maxp = p;
                            maxi = n;
                        }
                    }
                    पूर्णांक class_सूचकांक = entry_सूचकांक(l, b, maxi, l.coords + 1);
                    पूर्णांक obj_सूचकांक = entry_सूचकांक(l, b, maxi, l.coords);
                    delta_region_class(l.output, l.delta, class_सूचकांक, class, l.classes, l.softmax_वृक्ष, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
                    अगर(l.output[obj_सूचकांक] < .3) l.delta[obj_सूचकांक] = l.object_scale * (.3 - l.output[obj_सूचकांक]);
                    अन्यथा  l.delta[obj_सूचकांक] = 0;
                    l.delta[obj_सूचकांक] = 0;
                    ++class_गिनती;
                    onlyclass = 1;
                    अवरोध;
                }
            }
            अगर(onlyclass) जारी;
        }
        क्रम (j = 0; j < l.h; ++j) {
            क्रम (i = 0; i < l.w; ++i) {
                क्रम (n = 0; n < l.n; ++n) {
                    पूर्णांक box_सूचकांक = entry_सूचकांक(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_region_box(l.output, l.biases, n, box_सूचकांक, i, j, l.w, l.h, l.w*l.h);
                    भग्न best_iou = 0;
                    क्रम(t = 0; t < 30; ++t){
                        box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                        अगर(!truth.x) अवरोध;
                        भग्न iou = box_iou(pred, truth);
                        अगर (iou > best_iou) {
                            best_iou = iou;
                        }
                    }
                    पूर्णांक obj_सूचकांक = entry_सूचकांक(l, b, n*l.w*l.h + j*l.w + i, l.coords);
                    avg_anyobj += l.output[obj_सूचकांक];
                    l.delta[obj_सूचकांक] = l.noobject_scale * (0 - l.output[obj_सूचकांक]);
                    अगर(l.background) l.delta[obj_सूचकांक] = l.noobject_scale * (1 - l.output[obj_सूचकांक]);
                    अगर (best_iou > l.thresh) {
                        l.delta[obj_सूचकांक] = 0;
                    }

                    अगर(*(net.seen) < 12800){
                        box truth = {0};
                        truth.x = (i + .5)/l.w;
                        truth.y = (j + .5)/l.h;
                        truth.w = l.biases[2*n]/l.w;
                        truth.h = l.biases[2*n+1]/l.h;
                        delta_region_box(truth, l.output, l.biases, n, box_सूचकांक, i, j, l.w, l.h, l.delta, .01, l.w*l.h);
                    }
                }
            }
        }
        क्रम(t = 0; t < 30; ++t){
            box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);

            अगर(!truth.x) अवरोध;
            भग्न best_iou = 0;
            पूर्णांक best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = 0;
            truth_shift.y = 0;
            क्रम(n = 0; n < l.n; ++n){
                पूर्णांक box_सूचकांक = entry_सूचकांक(l, b, n*l.w*l.h + j*l.w + i, 0);
                box pred = get_region_box(l.output, l.biases, n, box_सूचकांक, i, j, l.w, l.h, l.w*l.h);
                अगर(l.bias_match){
                    pred.w = l.biases[2*n]/l.w;
                    pred.h = l.biases[2*n+1]/l.h;
                }
                pred.x = 0;
                pred.y = 0;
                भग्न iou = box_iou(pred, truth_shift);
                अगर (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            पूर्णांक box_सूचकांक = entry_सूचकांक(l, b, best_n*l.w*l.h + j*l.w + i, 0);
            भग्न iou = delta_region_box(truth, l.output, l.biases, best_n, box_सूचकांक, i, j, l.w, l.h, l.delta, l.coord_scale *  (2 - truth.w*truth.h), l.w*l.h);
            अगर(l.coords > 4){
                पूर्णांक mask_सूचकांक = entry_सूचकांक(l, b, best_n*l.w*l.h + j*l.w + i, 4);
                delta_region_mask(net.truth + t*(l.coords + 1) + b*l.truths + 5, l.output, l.coords - 4, mask_सूचकांक, l.delta, l.w*l.h, l.mask_scale);
            }
            अगर(iou > .5) recall += 1;
            avg_iou += iou;

            पूर्णांक obj_सूचकांक = entry_सूचकांक(l, b, best_n*l.w*l.h + j*l.w + i, l.coords);
            avg_obj += l.output[obj_सूचकांक];
            l.delta[obj_सूचकांक] = l.object_scale * (1 - l.output[obj_सूचकांक]);
            अगर (l.rescore) {
                l.delta[obj_सूचकांक] = l.object_scale * (iou - l.output[obj_सूचकांक]);
            }
            अगर(l.background){
                l.delta[obj_सूचकांक] = l.object_scale * (0 - l.output[obj_सूचकांक]);
            }

            पूर्णांक class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
            अगर (l.map) class = l.map[class];
            पूर्णांक class_सूचकांक = entry_सूचकांक(l, b, best_n*l.w*l.h + j*l.w + i, l.coords + 1);
            delta_region_class(l.output, l.delta, class_सूचकांक, class, l.classes, l.softmax_वृक्ष, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
            ++गिनती;
            ++class_गिनती;
        }
    }
    *(l.cost) = घात(mag_array(l.delta, l.outputs * l.batch), 2);
    म_लिखो("Region Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, Avg Recall: %f,  count: %d\n", avg_iou/गिनती, avg_cat/class_गिनती, avg_obj/गिनती, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/गिनती, गिनती);
}

व्योम backward_region_layer(स्थिर layer l, network net)
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

व्योम correct_region_boxes(detection *dets, पूर्णांक n, पूर्णांक w, पूर्णांक h, पूर्णांक netw, पूर्णांक neth, पूर्णांक relative)
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

व्योम get_region_detections(layer l, पूर्णांक w, पूर्णांक h, पूर्णांक netw, पूर्णांक neth, भग्न thresh, पूर्णांक *map, भग्न वृक्ष_thresh, पूर्णांक relative, detection *dets)
{
    पूर्णांक i,j,n,z;
    भग्न *predictions = l.output;
    अगर (l.batch == 2) {
        भग्न *flip = l.output + l.outputs;
        क्रम (j = 0; j < l.h; ++j) {
            क्रम (i = 0; i < l.w/2; ++i) {
                क्रम (n = 0; n < l.n; ++n) {
                    क्रम(z = 0; z < l.classes + l.coords + 1; ++z){
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
    क्रम (i = 0; i < l.w*l.h; ++i){
        पूर्णांक row = i / l.w;
        पूर्णांक col = i % l.w;
        क्रम(n = 0; n < l.n; ++n){
            पूर्णांक सूचकांक = n*l.w*l.h + i;
            क्रम(j = 0; j < l.classes; ++j){
                dets[सूचकांक].prob[j] = 0;
            }
            पूर्णांक obj_सूचकांक  = entry_सूचकांक(l, 0, n*l.w*l.h + i, l.coords);
            पूर्णांक box_सूचकांक  = entry_सूचकांक(l, 0, n*l.w*l.h + i, 0);
            पूर्णांक mask_सूचकांक = entry_सूचकांक(l, 0, n*l.w*l.h + i, 4);
            भग्न scale = l.background ? 1 : predictions[obj_सूचकांक];
            dets[सूचकांक].bbox = get_region_box(predictions, l.biases, n, box_सूचकांक, col, row, l.w, l.h, l.w*l.h);
            dets[सूचकांक].objectness = scale > thresh ? scale : 0;
            अगर(dets[सूचकांक].mask){
                क्रम(j = 0; j < l.coords - 4; ++j){
                    dets[सूचकांक].mask[j] = l.output[mask_सूचकांक + j*l.w*l.h];
                }
            }

            पूर्णांक class_सूचकांक = entry_सूचकांक(l, 0, n*l.w*l.h + i, l.coords + !l.background);
            अगर(l.softmax_वृक्ष){

                hierarchy_predictions(predictions + class_सूचकांक, l.classes, l.softmax_वृक्ष, 0, l.w*l.h);
                अगर(map){
                    क्रम(j = 0; j < 200; ++j){
                        पूर्णांक class_सूचकांक = entry_सूचकांक(l, 0, n*l.w*l.h + i, l.coords + 1 + map[j]);
                        भग्न prob = scale*predictions[class_सूचकांक];
                        dets[सूचकांक].prob[j] = (prob > thresh) ? prob : 0;
                    }
                } अन्यथा {
                    पूर्णांक j =  hierarchy_शीर्ष_prediction(predictions + class_सूचकांक, l.softmax_वृक्ष, वृक्ष_thresh, l.w*l.h);
                    dets[सूचकांक].prob[j] = (scale > thresh) ? scale : 0;
                }
            } अन्यथा {
                अगर(dets[सूचकांक].objectness){
                    क्रम(j = 0; j < l.classes; ++j){
                        पूर्णांक class_सूचकांक = entry_सूचकांक(l, 0, n*l.w*l.h + i, l.coords + 1 + j);
                        भग्न prob = scale*predictions[class_सूचकांक];
                        dets[सूचकांक].prob[j] = (prob > thresh) ? prob : 0;
                    }
                }
            }
        }
    }
    correct_region_boxes(dets, l.w*l.h*l.n, w, h, netw, neth, relative);
}

#अगर_घोषित GPU

व्योम forward_region_layer_gpu(स्थिर layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.निवेश_gpu, 1, l.output_gpu, 1);
    पूर्णांक b, n;
    क्रम (b = 0; b < l.batch; ++b){
        क्रम(n = 0; n < l.n; ++n){
            पूर्णांक सूचकांक = entry_सूचकांक(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + सूचकांक, 2*l.w*l.h, LOGISTIC);
            अगर(l.coords > 4){
                सूचकांक = entry_सूचकांक(l, b, n*l.w*l.h, 4);
                activate_array_gpu(l.output_gpu + सूचकांक, (l.coords - 4)*l.w*l.h, LOGISTIC);
            }
            सूचकांक = entry_सूचकांक(l, b, n*l.w*l.h, l.coords);
            अगर(!l.background) activate_array_gpu(l.output_gpu + सूचकांक,   l.w*l.h, LOGISTIC);
            सूचकांक = entry_सूचकांक(l, b, n*l.w*l.h, l.coords + 1);
            अगर(!l.softmax && !l.softmax_वृक्ष) activate_array_gpu(l.output_gpu + सूचकांक, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    अगर (l.softmax_वृक्ष){
        पूर्णांक सूचकांक = entry_सूचकांक(l, 0, 0, l.coords + 1);
        softmax_वृक्ष(net.निवेश_gpu + सूचकांक, l.w*l.h, l.batch*l.n, l.inputs/l.n, 1, l.output_gpu + सूचकांक, *l.softmax_वृक्ष);
    } अन्यथा अगर (l.softmax) {
        पूर्णांक सूचकांक = entry_सूचकांक(l, 0, 0, l.coords + !l.background);
        softmax_gpu(net.निवेश_gpu + सूचकांक, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu + सूचकांक);
    }
    अगर(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        वापस;
    }

    cuda_pull_array(l.output_gpu, net.निवेश, l.batch*l.inputs);
    forward_region_layer(l, net);
    //cuda_push_array(l.output_gpu, l.output, l.batch*l.outputs);
    अगर(!net.train) वापस;
    cuda_धकेलो_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

व्योम backward_region_layer_gpu(स्थिर layer l, network net)
{
    पूर्णांक b, n;
    क्रम (b = 0; b < l.batch; ++b){
        क्रम(n = 0; n < l.n; ++n){
            पूर्णांक सूचकांक = entry_सूचकांक(l, b, n*l.w*l.h, 0);
            gradient_array_gpu(l.output_gpu + सूचकांक, 2*l.w*l.h, LOGISTIC, l.delta_gpu + सूचकांक);
            अगर(l.coords > 4){
                सूचकांक = entry_सूचकांक(l, b, n*l.w*l.h, 4);
                gradient_array_gpu(l.output_gpu + सूचकांक, (l.coords - 4)*l.w*l.h, LOGISTIC, l.delta_gpu + सूचकांक);
            }
            सूचकांक = entry_सूचकांक(l, b, n*l.w*l.h, l.coords);
            अगर(!l.background) gradient_array_gpu(l.output_gpu + सूचकांक,   l.w*l.h, LOGISTIC, l.delta_gpu + सूचकांक);
        }
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#पूर्ण_अगर

व्योम zero_objectness(layer l)
{
    पूर्णांक i, n;
    क्रम (i = 0; i < l.w*l.h; ++i){
        क्रम(n = 0; n < l.n; ++n){
            पूर्णांक obj_सूचकांक = entry_सूचकांक(l, 0, n*l.w*l.h + i, l.coords);
            l.output[obj_सूचकांक] = 0;
        }
    }
}


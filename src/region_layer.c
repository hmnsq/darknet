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
    l.cost = सुस्मृति(1, आकार(दशांश));
    l.biases = सुस्मृति(n*2, आकार(दशांश));
    l.bias_updates = सुस्मृति(n*2, आकार(दशांश));
    l.outputs = h*w*n*(classes + coords + 1);
    l.inputs = l.outputs;
    l.truths = 30*(l.coords + 1);
    l.delta = सुस्मृति(batch*l.outputs, आकार(दशांश));
    l.output = सुस्मृति(batch*l.outputs, आकार(दशांश));
    पूर्णांक i;
    साठी(i = 0; i < n*2; ++i){
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

    परत l;
}

शून्य resize_region_layer(layer *l, पूर्णांक w, पूर्णांक h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + l->coords + 1);
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

box get_region_box(दशांश *x, दशांश *biases, पूर्णांक n, पूर्णांक index, पूर्णांक i, पूर्णांक j, पूर्णांक w, पूर्णांक h, पूर्णांक stride)
{
    box b;
    b.x = (i + x[index + 0*stride]) / w;
    b.y = (j + x[index + 1*stride]) / h;
    b.w = exp(x[index + 2*stride]) * biases[2*n]   / w;
    b.h = exp(x[index + 3*stride]) * biases[2*n+1] / h;
    परत b;
}

दशांश delta_region_box(box truth, दशांश *x, दशांश *biases, पूर्णांक n, पूर्णांक index, पूर्णांक i, पूर्णांक j, पूर्णांक w, पूर्णांक h, दशांश *delta, दशांश scale, पूर्णांक stride)
{
    box pred = get_region_box(x, biases, n, index, i, j, w, h, stride);
    दशांश iou = box_iou(pred, truth);

    दशांश tx = (truth.x*w - i);
    दशांश ty = (truth.y*h - j);
    दशांश tw = log(truth.w*w / biases[2*n]);
    दशांश th = log(truth.h*h / biases[2*n + 1]);

    delta[index + 0*stride] = scale * (tx - x[index + 0*stride]);
    delta[index + 1*stride] = scale * (ty - x[index + 1*stride]);
    delta[index + 2*stride] = scale * (tw - x[index + 2*stride]);
    delta[index + 3*stride] = scale * (th - x[index + 3*stride]);
    परत iou;
}

शून्य delta_region_mask(दशांश *truth, दशांश *x, पूर्णांक n, पूर्णांक index, दशांश *delta, पूर्णांक stride, पूर्णांक scale)
{
    पूर्णांक i;
    साठी(i = 0; i < n; ++i){
        delta[index + i*stride] = scale*(truth[i] - x[index + i*stride]);
    }
}


शून्य delta_region_class(दशांश *output, दशांश *delta, पूर्णांक index, पूर्णांक class, पूर्णांक classes, वृक्ष *hier, दशांश scale, पूर्णांक stride, दशांश *avg_cat, पूर्णांक tag)
{
    पूर्णांक i, n;
    जर(hier){
        दशांश pred = 1;
        पर्यंत(class >= 0){
            pred *= output[index + stride*class];
            पूर्णांक g = hier->group[class];
            पूर्णांक offset = hier->group_offset[g];
            साठी(i = 0; i < hier->group_size[g]; ++i){
                delta[index + stride*(offset + i)] = scale * (0 - output[index + stride*(offset + i)]);
            }
            delta[index + stride*class] = scale * (1 - output[index + stride*class]);

            class = hier->parent[class];
        }
        *avg_cat += pred;
    } अन्यथा {
        जर (delta[index] && tag){
            delta[index + stride*class] = scale * (1 - output[index + stride*class]);
            परत;
        }
        साठी(n = 0; n < classes; ++n){
            delta[index + stride*n] = scale * (((n == class)?1 : 0) - output[index + stride*n]);
            जर(n == class) *avg_cat += output[index + stride*n];
        }
    }
}

दशांश logit(दशांश x)
{
    परत log(x/(1.-x));
}

दशांश tisnan(दशांश x)
{
    परत (x != x);
}

पूर्णांक entry_index(layer l, पूर्णांक batch, पूर्णांक location, पूर्णांक entry)
{
    पूर्णांक n =   location / (l.w*l.h);
    पूर्णांक loc = location % (l.w*l.h);
    परत batch*l.outputs + n*l.w*l.h*(l.coords+l.classes+1) + entry*l.w*l.h + loc;
}

शून्य forward_region_layer(स्थिरांक layer l, network net)
{
    पूर्णांक i,j,b,t,n;
    स_नकल(l.output, net.आदान, l.outputs*l.batch*आकार(दशांश));

#ifndef GPU
    साठी (b = 0; b < l.batch; ++b){
        साठी(n = 0; n < l.n; ++n){
            पूर्णांक index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array(l.output + index, 2*l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            जर(!l.background) activate_array(l.output + index,   l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords + 1);
            जर(!l.softmax && !l.softmax_वृक्ष) activate_array(l.output + index, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    जर (l.softmax_वृक्ष){
        पूर्णांक i;
        पूर्णांक मोजणी = l.coords + 1;
        साठी (i = 0; i < l.softmax_वृक्ष->groups; ++i) {
            पूर्णांक group_size = l.softmax_वृक्ष->group_size[i];
            softmax_cpu(net.आदान + मोजणी, group_size, l.batch, l.inputs, l.n*l.w*l.h, 1, l.n*l.w*l.h, l.temperature, l.output + मोजणी);
            मोजणी += group_size;
        }
    } अन्यथा जर (l.softmax){
        पूर्णांक index = entry_index(l, 0, 0, l.coords + !l.background);
        softmax_cpu(net.आदान + index, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output + index);
    }
#endif

    स_रखो(l.delta, 0, l.outputs * l.batch * आकार(दशांश));
    जर(!net.train) परत;
    दशांश avg_iou = 0;
    दशांश recall = 0;
    दशांश avg_cat = 0;
    दशांश avg_obj = 0;
    दशांश avg_anyobj = 0;
    पूर्णांक मोजणी = 0;
    पूर्णांक class_मोजणी = 0;
    *(l.cost) = 0;
    साठी (b = 0; b < l.batch; ++b) {
        जर(l.softmax_वृक्ष){
            पूर्णांक onlyclass = 0;
            साठी(t = 0; t < 30; ++t){
                box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                जर(!truth.x) थांबा;
                पूर्णांक class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
                दशांश maxp = 0;
                पूर्णांक maxi = 0;
                जर(truth.x > 100000 && truth.y > 100000){
                    साठी(n = 0; n < l.n*l.w*l.h; ++n){
                        पूर्णांक class_index = entry_index(l, b, n, l.coords + 1);
                        पूर्णांक obj_index = entry_index(l, b, n, l.coords);
                        दशांश scale =  l.output[obj_index];
                        l.delta[obj_index] = l.noobject_scale * (0 - l.output[obj_index]);
                        दशांश p = scale*get_hierarchy_probability(l.output + class_index, l.softmax_वृक्ष, class, l.w*l.h);
                        जर(p > maxp){
                            maxp = p;
                            maxi = n;
                        }
                    }
                    पूर्णांक class_index = entry_index(l, b, maxi, l.coords + 1);
                    पूर्णांक obj_index = entry_index(l, b, maxi, l.coords);
                    delta_region_class(l.output, l.delta, class_index, class, l.classes, l.softmax_वृक्ष, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
                    जर(l.output[obj_index] < .3) l.delta[obj_index] = l.object_scale * (.3 - l.output[obj_index]);
                    अन्यथा  l.delta[obj_index] = 0;
                    l.delta[obj_index] = 0;
                    ++class_मोजणी;
                    onlyclass = 1;
                    थांबा;
                }
            }
            जर(onlyclass) सुरूठेवा;
        }
        साठी (j = 0; j < l.h; ++j) {
            साठी (i = 0; i < l.w; ++i) {
                साठी (n = 0; n < l.n; ++n) {
                    पूर्णांक box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_region_box(l.output, l.biases, n, box_index, i, j, l.w, l.h, l.w*l.h);
                    दशांश best_iou = 0;
                    साठी(t = 0; t < 30; ++t){
                        box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                        जर(!truth.x) थांबा;
                        दशांश iou = box_iou(pred, truth);
                        जर (iou > best_iou) {
                            best_iou = iou;
                        }
                    }
                    पूर्णांक obj_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, l.coords);
                    avg_anyobj += l.output[obj_index];
                    l.delta[obj_index] = l.noobject_scale * (0 - l.output[obj_index]);
                    जर(l.background) l.delta[obj_index] = l.noobject_scale * (1 - l.output[obj_index]);
                    जर (best_iou > l.thresh) {
                        l.delta[obj_index] = 0;
                    }

                    जर(*(net.seen) < 12800){
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
        साठी(t = 0; t < 30; ++t){
            box truth = float_to_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);

            जर(!truth.x) थांबा;
            दशांश best_iou = 0;
            पूर्णांक best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_shift = truth;
            truth_shift.x = 0;
            truth_shift.y = 0;
            साठी(n = 0; n < l.n; ++n){
                पूर्णांक box_index = entry_index(l, b, n*l.w*l.h + j*l.w + i, 0);
                box pred = get_region_box(l.output, l.biases, n, box_index, i, j, l.w, l.h, l.w*l.h);
                जर(l.bias_match){
                    pred.w = l.biases[2*n]/l.w;
                    pred.h = l.biases[2*n+1]/l.h;
                }
                pred.x = 0;
                pred.y = 0;
                दशांश iou = box_iou(pred, truth_shift);
                जर (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            पूर्णांक box_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, 0);
            दशांश iou = delta_region_box(truth, l.output, l.biases, best_n, box_index, i, j, l.w, l.h, l.delta, l.coord_scale *  (2 - truth.w*truth.h), l.w*l.h);
            जर(l.coords > 4){
                पूर्णांक mask_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, 4);
                delta_region_mask(net.truth + t*(l.coords + 1) + b*l.truths + 5, l.output, l.coords - 4, mask_index, l.delta, l.w*l.h, l.mask_scale);
            }
            जर(iou > .5) recall += 1;
            avg_iou += iou;

            पूर्णांक obj_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, l.coords);
            avg_obj += l.output[obj_index];
            l.delta[obj_index] = l.object_scale * (1 - l.output[obj_index]);
            जर (l.rescore) {
                l.delta[obj_index] = l.object_scale * (iou - l.output[obj_index]);
            }
            जर(l.background){
                l.delta[obj_index] = l.object_scale * (0 - l.output[obj_index]);
            }

            पूर्णांक class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
            जर (l.map) class = l.map[class];
            पूर्णांक class_index = entry_index(l, b, best_n*l.w*l.h + j*l.w + i, l.coords + 1);
            delta_region_class(l.output, l.delta, class_index, class, l.classes, l.softmax_वृक्ष, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
            ++मोजणी;
            ++class_मोजणी;
        }
    }
    *(l.cost) = घात(mag_array(l.delta, l.outputs * l.batch), 2);
    म_लिखो("Region Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, Avg Recall: %f,  count: %d\n", avg_iou/मोजणी, avg_cat/class_मोजणी, avg_obj/मोजणी, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/मोजणी, मोजणी);
}

शून्य backward_region_layer(स्थिरांक layer l, network net)
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

शून्य correct_region_boxes(detection *dets, पूर्णांक n, पूर्णांक w, पूर्णांक h, पूर्णांक netw, पूर्णांक neth, पूर्णांक relative)
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

शून्य get_region_detections(layer l, पूर्णांक w, पूर्णांक h, पूर्णांक netw, पूर्णांक neth, दशांश thresh, पूर्णांक *map, दशांश वृक्ष_thresh, पूर्णांक relative, detection *dets)
{
    पूर्णांक i,j,n,z;
    दशांश *predictions = l.output;
    जर (l.batch == 2) {
        दशांश *flip = l.output + l.outputs;
        साठी (j = 0; j < l.h; ++j) {
            साठी (i = 0; i < l.w/2; ++i) {
                साठी (n = 0; n < l.n; ++n) {
                    साठी(z = 0; z < l.classes + l.coords + 1; ++z){
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
    साठी (i = 0; i < l.w*l.h; ++i){
        पूर्णांक row = i / l.w;
        पूर्णांक col = i % l.w;
        साठी(n = 0; n < l.n; ++n){
            पूर्णांक index = n*l.w*l.h + i;
            साठी(j = 0; j < l.classes; ++j){
                dets[index].prob[j] = 0;
            }
            पूर्णांक obj_index  = entry_index(l, 0, n*l.w*l.h + i, l.coords);
            पूर्णांक box_index  = entry_index(l, 0, n*l.w*l.h + i, 0);
            पूर्णांक mask_index = entry_index(l, 0, n*l.w*l.h + i, 4);
            दशांश scale = l.background ? 1 : predictions[obj_index];
            dets[index].bbox = get_region_box(predictions, l.biases, n, box_index, col, row, l.w, l.h, l.w*l.h);
            dets[index].objectness = scale > thresh ? scale : 0;
            जर(dets[index].mask){
                साठी(j = 0; j < l.coords - 4; ++j){
                    dets[index].mask[j] = l.output[mask_index + j*l.w*l.h];
                }
            }

            पूर्णांक class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + !l.background);
            जर(l.softmax_वृक्ष){

                hierarchy_predictions(predictions + class_index, l.classes, l.softmax_वृक्ष, 0, l.w*l.h);
                जर(map){
                    साठी(j = 0; j < 200; ++j){
                        पूर्णांक class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + 1 + map[j]);
                        दशांश prob = scale*predictions[class_index];
                        dets[index].prob[j] = (prob > thresh) ? prob : 0;
                    }
                } अन्यथा {
                    पूर्णांक j =  hierarchy_शीर्ष_prediction(predictions + class_index, l.softmax_वृक्ष, वृक्ष_thresh, l.w*l.h);
                    dets[index].prob[j] = (scale > thresh) ? scale : 0;
                }
            } अन्यथा {
                जर(dets[index].objectness){
                    साठी(j = 0; j < l.classes; ++j){
                        पूर्णांक class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + 1 + j);
                        दशांश prob = scale*predictions[class_index];
                        dets[index].prob[j] = (prob > thresh) ? prob : 0;
                    }
                }
            }
        }
    }
    correct_region_boxes(dets, l.w*l.h*l.n, w, h, netw, neth, relative);
}

#ifdef GPU

शून्य forward_region_layer_gpu(स्थिरांक layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.आदान_gpu, 1, l.output_gpu, 1);
    पूर्णांक b, n;
    साठी (b = 0; b < l.batch; ++b){
        साठी(n = 0; n < l.n; ++n){
            पूर्णांक index = entry_index(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC);
            जर(l.coords > 4){
                index = entry_index(l, b, n*l.w*l.h, 4);
                activate_array_gpu(l.output_gpu + index, (l.coords - 4)*l.w*l.h, LOGISTIC);
            }
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            जर(!l.background) activate_array_gpu(l.output_gpu + index,   l.w*l.h, LOGISTIC);
            index = entry_index(l, b, n*l.w*l.h, l.coords + 1);
            जर(!l.softmax && !l.softmax_वृक्ष) activate_array_gpu(l.output_gpu + index, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    जर (l.softmax_वृक्ष){
        पूर्णांक index = entry_index(l, 0, 0, l.coords + 1);
        softmax_वृक्ष(net.आदान_gpu + index, l.w*l.h, l.batch*l.n, l.inputs/l.n, 1, l.output_gpu + index, *l.softmax_वृक्ष);
    } अन्यथा जर (l.softmax) {
        पूर्णांक index = entry_index(l, 0, 0, l.coords + !l.background);
        softmax_gpu(net.आदान_gpu + index, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu + index);
    }
    जर(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        परत;
    }

    cuda_pull_array(l.output_gpu, net.आदान, l.batch*l.inputs);
    forward_region_layer(l, net);
    //cuda_push_array(l.output_gpu, l.output, l.batch*l.outputs);
    जर(!net.train) परत;
    cuda_ढकला_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

शून्य backward_region_layer_gpu(स्थिरांक layer l, network net)
{
    पूर्णांक b, n;
    साठी (b = 0; b < l.batch; ++b){
        साठी(n = 0; n < l.n; ++n){
            पूर्णांक index = entry_index(l, b, n*l.w*l.h, 0);
            gradient_array_gpu(l.output_gpu + index, 2*l.w*l.h, LOGISTIC, l.delta_gpu + index);
            जर(l.coords > 4){
                index = entry_index(l, b, n*l.w*l.h, 4);
                gradient_array_gpu(l.output_gpu + index, (l.coords - 4)*l.w*l.h, LOGISTIC, l.delta_gpu + index);
            }
            index = entry_index(l, b, n*l.w*l.h, l.coords);
            जर(!l.background) gradient_array_gpu(l.output_gpu + index,   l.w*l.h, LOGISTIC, l.delta_gpu + index);
        }
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif

शून्य zero_objectness(layer l)
{
    पूर्णांक i, n;
    साठी (i = 0; i < l.w*l.h; ++i){
        साठी(n = 0; n < l.n; ++n){
            पूर्णांक obj_index = entry_index(l, 0, n*l.w*l.h + i, l.coords);
            l.output[obj_index] = 0;
        }
    }
}


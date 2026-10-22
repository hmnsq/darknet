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
    l.बहिर्गमनम्_w = l.w;
    l.बहिर्गमनम्_h = l.h;
    l.बहिर्गमनम्_c = l.c;
    l.classes = classes;
    l.coords = coords;
    l.cost = शून्यस्मृतिं_देहि(1, परिमाण(भिन्न));
    l.biases = शून्यस्मृतिं_देहि(n*2, परिमाण(भिन्न));
    l.bias_updates = शून्यस्मृतिं_देहि(n*2, परिमाण(भिन्न));
    l.outputs = h*w*n*(classes + coords + 1);
    l.inputs = l.outputs;
    l.truths = 30*(l.coords + 1);
    l.delta = शून्यस्मृतिं_देहि(batch*l.outputs, परिमाण(भिन्न));
    l.output = शून्यस्मृतिं_देहि(batch*l.outputs, परिमाण(भिन्न));
    पूर्णाङ्क i;
    प्रति(i = 0; i < n*2; ++i){
        l.biases[i] = .5;
    }

    l.forward = forward_region_layer;
    l.backward = backward_region_layer;
#यदि_परिभाषितम् GPU
    l.forward_gpu = forward_region_layer_gpu;
    l.backward_gpu = backward_region_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#यदि_अन्तः

    fprintf(stderr, "detection\n");
    बीजं_रोपय(0);

    प्रत्यागम l;
}

शून्य resize_region_layer(layer *l, पूर्णाङ्क w, पूर्णाङ्क h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + l->coords + 1);
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

box get_region_box(भिन्न *x, भिन्न *biases, पूर्णाङ्क n, पूर्णाङ्क सूचकांक, पूर्णाङ्क i, पूर्णाङ्क j, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क stride)
{
    box b;
    b.x = (i + x[सूचकांक + 0*stride]) / w;
    b.y = (j + x[सूचकांक + 1*stride]) / h;
    b.w = घातांकः(x[सूचकांक + 2*stride]) * biases[2*n]   / w;
    b.h = घातांकः(x[सूचकांक + 3*stride]) * biases[2*n+1] / h;
    प्रत्यागम b;
}

भिन्न delta_region_box(box truth, भिन्न *x, भिन्न *biases, पूर्णाङ्क n, पूर्णाङ्क सूचकांक, पूर्णाङ्क i, पूर्णाङ्क j, पूर्णाङ्क w, पूर्णाङ्क h, भिन्न *delta, भिन्न scale, पूर्णाङ्क stride)
{
    box pred = get_region_box(x, biases, n, सूचकांक, i, j, w, h, stride);
    भिन्न iou = box_iou(pred, truth);

    भिन्न tx = (truth.x*w - i);
    भिन्न ty = (truth.y*h - j);
    भिन्न tw = लघुगणकः(truth.w*w / biases[2*n]);
    भिन्न th = लघुगणकः(truth.h*h / biases[2*n + 1]);

    delta[सूचकांक + 0*stride] = scale * (tx - x[सूचकांक + 0*stride]);
    delta[सूचकांक + 1*stride] = scale * (ty - x[सूचकांक + 1*stride]);
    delta[सूचकांक + 2*stride] = scale * (tw - x[सूचकांक + 2*stride]);
    delta[सूचकांक + 3*stride] = scale * (th - x[सूचकांक + 3*stride]);
    प्रत्यागम iou;
}

शून्य delta_region_आवरणम्(भिन्न *truth, भिन्न *x, पूर्णाङ्क n, पूर्णाङ्क सूचकांक, भिन्न *delta, पूर्णाङ्क stride, पूर्णाङ्क scale)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        delta[सूचकांक + i*stride] = scale*(truth[i] - x[सूचकांक + i*stride]);
    }
}


शून्य delta_region_class(भिन्न *output, भिन्न *delta, पूर्णाङ्क सूचकांक, पूर्णाङ्क class, पूर्णाङ्क classes, वृक्षः *hier, भिन्न scale, पूर्णाङ्क stride, भिन्न *avg_cat, पूर्णाङ्क चिह्नकम्)
{
    पूर्णाङ्क i, n;
    यदि(hier){
        भिन्न pred = 1;
        यावत्(class >= 0){
            pred *= output[सूचकांक + stride*class];
            पूर्णाङ्क g = hier->गणः[class];
            पूर्णाङ्क विस्थापनम् = hier->गणः_विस्थापनम्[g];
            प्रति(i = 0; i < hier->गणः_आकार[g]; ++i){
                delta[सूचकांक + stride*(विस्थापनम् + i)] = scale * (0 - output[सूचकांक + stride*(विस्थापनम् + i)]);
            }
            delta[सूचकांक + stride*class] = scale * (1 - output[सूचकांक + stride*class]);

            class = hier->parent[class];
        }
        *avg_cat += pred;
    } अन्यथा {
        यदि (delta[सूचकांक] && चिह्नकम्){
            delta[सूचकांक + stride*class] = scale * (1 - output[सूचकांक + stride*class]);
            प्रत्यागम;
        }
        प्रति(n = 0; n < classes; ++n){
            delta[सूचकांक + stride*n] = scale * (((n == class)?1 : 0) - output[सूचकांक + stride*n]);
            यदि(n == class) *avg_cat += output[सूचकांक + stride*n];
        }
    }
}

भिन्न logit(भिन्न x)
{
    प्रत्यागम लघुगणकः(x/(1.-x));
}

भिन्न tisnan(भिन्न x)
{
    प्रत्यागम (x != x);
}

पूर्णाङ्क प्रविष्टिः_सूचकांक(layer l, पूर्णाङ्क batch, पूर्णाङ्क location, पूर्णाङ्क प्रविष्टिः)
{
    पूर्णाङ्क n =   location / (l.w*l.h);
    पूर्णाङ्क loc = location % (l.w*l.h);
    प्रत्यागम batch*l.outputs + n*l.w*l.h*(l.coords+l.classes+1) + प्रविष्टिः*l.w*l.h + loc;
}

शून्य forward_region_layer(ध्रुव layer l, जालम् net)
{
    पूर्णाङ्क i,j,b,t,n;
    स्मृतिं_प्रतिलिख(l.output, net.निवेशः, l.outputs*l.batch*परिमाण(भिन्न));

#यदि_अपरिभाषितम् GPU
    प्रति (b = 0; b < l.batch; ++b){
        प्रति(n = 0; n < l.n; ++n){
            पूर्णाङ्क सूचकांक = प्रविष्टिः_सूचकांक(l, b, n*l.w*l.h, 0);
            activate_array(l.output + सूचकांक, 2*l.w*l.h, LOGISTIC);
            सूचकांक = प्रविष्टिः_सूचकांक(l, b, n*l.w*l.h, l.coords);
            यदि(!l.background) activate_array(l.output + सूचकांक,   l.w*l.h, LOGISTIC);
            सूचकांक = प्रविष्टिः_सूचकांक(l, b, n*l.w*l.h, l.coords + 1);
            यदि(!l.softmax && !l.softmax_वृक्षः) activate_array(l.output + सूचकांक, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    यदि (l.softmax_वृक्षः){
        पूर्णाङ्क i;
        पूर्णाङ्क गणनम् = l.coords + 1;
        प्रति (i = 0; i < l.softmax_वृक्षः->groups; ++i) {
            पूर्णाङ्क गणः_आकार = l.softmax_वृक्षः->गणः_आकार[i];
            softmax_cpu(net.निवेशः + गणनम्, गणः_आकार, l.batch, l.inputs, l.n*l.w*l.h, 1, l.n*l.w*l.h, l.temperature, l.output + गणनम्);
            गणनम् += गणः_आकार;
        }
    } अन्यथा यदि (l.softmax){
        पूर्णाङ्क सूचकांक = प्रविष्टिः_सूचकांक(l, 0, 0, l.coords + !l.background);
        softmax_cpu(net.निवेशः + सूचकांक, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output + सूचकांक);
    }
#यदि_अन्तः

    स्मृतिं_पूरय(l.delta, 0, l.outputs * l.batch * परिमाण(भिन्न));
    यदि(!net.train) प्रत्यागम;
    भिन्न avg_iou = 0;
    भिन्न recall = 0;
    भिन्न avg_cat = 0;
    भिन्न avg_obj = 0;
    भिन्न avg_anyobj = 0;
    पूर्णाङ्क गणनम् = 0;
    पूर्णाङ्क class_गणनम् = 0;
    *(l.cost) = 0;
    प्रति (b = 0; b < l.batch; ++b) {
        यदि(l.softmax_वृक्षः){
            पूर्णाङ्क onlyclass = 0;
            प्रति(t = 0; t < 30; ++t){
                box truth = float_गन्तव्यम्_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                यदि(!truth.x) भङ्ग;
                पूर्णाङ्क class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
                भिन्न maxp = 0;
                पूर्णाङ्क maxi = 0;
                यदि(truth.x > 100000 && truth.y > 100000){
                    प्रति(n = 0; n < l.n*l.w*l.h; ++n){
                        पूर्णाङ्क class_सूचकांक = प्रविष्टिः_सूचकांक(l, b, n, l.coords + 1);
                        पूर्णाङ्क obj_सूचकांक = प्रविष्टिः_सूचकांक(l, b, n, l.coords);
                        भिन्न scale =  l.output[obj_सूचकांक];
                        l.delta[obj_सूचकांक] = l.noobject_scale * (0 - l.output[obj_सूचकांक]);
                        भिन्न p = scale*get_hierarchy_probability(l.output + class_सूचकांक, l.softmax_वृक्षः, class, l.w*l.h);
                        यदि(p > maxp){
                            maxp = p;
                            maxi = n;
                        }
                    }
                    पूर्णाङ्क class_सूचकांक = प्रविष्टिः_सूचकांक(l, b, maxi, l.coords + 1);
                    पूर्णाङ्क obj_सूचकांक = प्रविष्टिः_सूचकांक(l, b, maxi, l.coords);
                    delta_region_class(l.output, l.delta, class_सूचकांक, class, l.classes, l.softmax_वृक्षः, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
                    यदि(l.output[obj_सूचकांक] < .3) l.delta[obj_सूचकांक] = l.object_scale * (.3 - l.output[obj_सूचकांक]);
                    अन्यथा  l.delta[obj_सूचकांक] = 0;
                    l.delta[obj_सूचकांक] = 0;
                    ++class_गणनम्;
                    onlyclass = 1;
                    भङ्ग;
                }
            }
            यदि(onlyclass) अनुवर्त;
        }
        प्रति (j = 0; j < l.h; ++j) {
            प्रति (i = 0; i < l.w; ++i) {
                प्रति (n = 0; n < l.n; ++n) {
                    पूर्णाङ्क box_सूचकांक = प्रविष्टिः_सूचकांक(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_region_box(l.output, l.biases, n, box_सूचकांक, i, j, l.w, l.h, l.w*l.h);
                    भिन्न best_iou = 0;
                    प्रति(t = 0; t < 30; ++t){
                        box truth = float_गन्तव्यम्_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                        यदि(!truth.x) भङ्ग;
                        भिन्न iou = box_iou(pred, truth);
                        यदि (iou > best_iou) {
                            best_iou = iou;
                        }
                    }
                    पूर्णाङ्क obj_सूचकांक = प्रविष्टिः_सूचकांक(l, b, n*l.w*l.h + j*l.w + i, l.coords);
                    avg_anyobj += l.output[obj_सूचकांक];
                    l.delta[obj_सूचकांक] = l.noobject_scale * (0 - l.output[obj_सूचकांक]);
                    यदि(l.background) l.delta[obj_सूचकांक] = l.noobject_scale * (1 - l.output[obj_सूचकांक]);
                    यदि (best_iou > l.thresh) {
                        l.delta[obj_सूचकांक] = 0;
                    }

                    यदि(*(net.आलोकितम्) < 12800){
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
        प्रति(t = 0; t < 30; ++t){
            box truth = float_गन्तव्यम्_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);

            यदि(!truth.x) भङ्ग;
            भिन्न best_iou = 0;
            पूर्णाङ्क best_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_सरणम् = truth;
            truth_सरणम्.x = 0;
            truth_सरणम्.y = 0;
            प्रति(n = 0; n < l.n; ++n){
                पूर्णाङ्क box_सूचकांक = प्रविष्टिः_सूचकांक(l, b, n*l.w*l.h + j*l.w + i, 0);
                box pred = get_region_box(l.output, l.biases, n, box_सूचकांक, i, j, l.w, l.h, l.w*l.h);
                यदि(l.bias_मेलनम्){
                    pred.w = l.biases[2*n]/l.w;
                    pred.h = l.biases[2*n+1]/l.h;
                }
                pred.x = 0;
                pred.y = 0;
                भिन्न iou = box_iou(pred, truth_सरणम्);
                यदि (iou > best_iou){
                    best_iou = iou;
                    best_n = n;
                }
            }

            पूर्णाङ्क box_सूचकांक = प्रविष्टिः_सूचकांक(l, b, best_n*l.w*l.h + j*l.w + i, 0);
            भिन्न iou = delta_region_box(truth, l.output, l.biases, best_n, box_सूचकांक, i, j, l.w, l.h, l.delta, l.coord_scale *  (2 - truth.w*truth.h), l.w*l.h);
            यदि(l.coords > 4){
                पूर्णाङ्क आवरणम्_सूचकांक = प्रविष्टिः_सूचकांक(l, b, best_n*l.w*l.h + j*l.w + i, 4);
                delta_region_आवरणम्(net.truth + t*(l.coords + 1) + b*l.truths + 5, l.output, l.coords - 4, आवरणम्_सूचकांक, l.delta, l.w*l.h, l.आवरणम्_scale);
            }
            यदि(iou > .5) recall += 1;
            avg_iou += iou;

            पूर्णाङ्क obj_सूचकांक = प्रविष्टिः_सूचकांक(l, b, best_n*l.w*l.h + j*l.w + i, l.coords);
            avg_obj += l.output[obj_सूचकांक];
            l.delta[obj_सूचकांक] = l.object_scale * (1 - l.output[obj_सूचकांक]);
            यदि (l.rescore) {
                l.delta[obj_सूचकांक] = l.object_scale * (iou - l.output[obj_सूचकांक]);
            }
            यदि(l.background){
                l.delta[obj_सूचकांक] = l.object_scale * (0 - l.output[obj_सूचकांक]);
            }

            पूर्णाङ्क class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
            यदि (l.map) class = l.map[class];
            पूर्णाङ्क class_सूचकांक = प्रविष्टिः_सूचकांक(l, b, best_n*l.w*l.h + j*l.w + i, l.coords + 1);
            delta_region_class(l.output, l.delta, class_सूचकांक, class, l.classes, l.softmax_वृक्षः, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
            ++गणनम्;
            ++class_गणनम्;
        }
    }
    *(l.cost) = घातः(mag_array(l.delta, l.outputs * l.batch), 2);
    प्रारूप_लिख("Region Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, Avg Recall: %f,  count: %d\n", avg_iou/गणनम्, avg_cat/class_गणनम्, avg_obj/गणनम्, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/गणनम्, गणनम्);
}

शून्य backward_region_layer(ध्रुव layer l, जालम् net)
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

शून्य get_region_detections(layer l, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क netw, पूर्णाङ्क neth, भिन्न thresh, पूर्णाङ्क *map, भिन्न वृक्षः_thresh, पूर्णाङ्क relative, detection *dets)
{
    पूर्णाङ्क i,j,n,z;
    भिन्न *predictions = l.output;
    यदि (l.batch == 2) {
        भिन्न *flip = l.output + l.outputs;
        प्रति (j = 0; j < l.h; ++j) {
            प्रति (i = 0; i < l.w/2; ++i) {
                प्रति (n = 0; n < l.n; ++n) {
                    प्रति(z = 0; z < l.classes + l.coords + 1; ++z){
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
    प्रति (i = 0; i < l.w*l.h; ++i){
        पूर्णाङ्क row = i / l.w;
        पूर्णाङ्क col = i % l.w;
        प्रति(n = 0; n < l.n; ++n){
            पूर्णाङ्क सूचकांक = n*l.w*l.h + i;
            प्रति(j = 0; j < l.classes; ++j){
                dets[सूचकांक].prob[j] = 0;
            }
            पूर्णाङ्क obj_सूचकांक  = प्रविष्टिः_सूचकांक(l, 0, n*l.w*l.h + i, l.coords);
            पूर्णाङ्क box_सूचकांक  = प्रविष्टिः_सूचकांक(l, 0, n*l.w*l.h + i, 0);
            पूर्णाङ्क आवरणम्_सूचकांक = प्रविष्टिः_सूचकांक(l, 0, n*l.w*l.h + i, 4);
            भिन्न scale = l.background ? 1 : predictions[obj_सूचकांक];
            dets[सूचकांक].bbox = get_region_box(predictions, l.biases, n, box_सूचकांक, col, row, l.w, l.h, l.w*l.h);
            dets[सूचकांक].objectness = scale > thresh ? scale : 0;
            यदि(dets[सूचकांक].आवरणम्){
                प्रति(j = 0; j < l.coords - 4; ++j){
                    dets[सूचकांक].आवरणम्[j] = l.output[आवरणम्_सूचकांक + j*l.w*l.h];
                }
            }

            पूर्णाङ्क class_सूचकांक = प्रविष्टिः_सूचकांक(l, 0, n*l.w*l.h + i, l.coords + !l.background);
            यदि(l.softmax_वृक्षः){

                hierarchy_predictions(predictions + class_सूचकांक, l.classes, l.softmax_वृक्षः, 0, l.w*l.h);
                यदि(map){
                    प्रति(j = 0; j < 200; ++j){
                        पूर्णाङ्क class_सूचकांक = प्रविष्टिः_सूचकांक(l, 0, n*l.w*l.h + i, l.coords + 1 + map[j]);
                        भिन्न prob = scale*predictions[class_सूचकांक];
                        dets[सूचकांक].prob[j] = (prob > thresh) ? prob : 0;
                    }
                } अन्यथा {
                    पूर्णाङ्क j =  hierarchy_शिखरम्_prediction(predictions + class_सूचकांक, l.softmax_वृक्षः, वृक्षः_thresh, l.w*l.h);
                    dets[सूचकांक].prob[j] = (scale > thresh) ? scale : 0;
                }
            } अन्यथा {
                यदि(dets[सूचकांक].objectness){
                    प्रति(j = 0; j < l.classes; ++j){
                        पूर्णाङ्क class_सूचकांक = प्रविष्टिः_सूचकांक(l, 0, n*l.w*l.h + i, l.coords + 1 + j);
                        भिन्न prob = scale*predictions[class_सूचकांक];
                        dets[सूचकांक].prob[j] = (prob > thresh) ? prob : 0;
                    }
                }
            }
        }
    }
    correct_region_boxes(dets, l.w*l.h*l.n, w, h, netw, neth, relative);
}

#यदि_परिभाषितम् GPU

शून्य forward_region_layer_gpu(ध्रुव layer l, जालम् net)
{
    copy_gpu(l.batch*l.inputs, net.निवेशः_gpu, 1, l.output_gpu, 1);
    पूर्णाङ्क b, n;
    प्रति (b = 0; b < l.batch; ++b){
        प्रति(n = 0; n < l.n; ++n){
            पूर्णाङ्क सूचकांक = प्रविष्टिः_सूचकांक(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + सूचकांक, 2*l.w*l.h, LOGISTIC);
            यदि(l.coords > 4){
                सूचकांक = प्रविष्टिः_सूचकांक(l, b, n*l.w*l.h, 4);
                activate_array_gpu(l.output_gpu + सूचकांक, (l.coords - 4)*l.w*l.h, LOGISTIC);
            }
            सूचकांक = प्रविष्टिः_सूचकांक(l, b, n*l.w*l.h, l.coords);
            यदि(!l.background) activate_array_gpu(l.output_gpu + सूचकांक,   l.w*l.h, LOGISTIC);
            सूचकांक = प्रविष्टिः_सूचकांक(l, b, n*l.w*l.h, l.coords + 1);
            यदि(!l.softmax && !l.softmax_वृक्षः) activate_array_gpu(l.output_gpu + सूचकांक, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    यदि (l.softmax_वृक्षः){
        पूर्णाङ्क सूचकांक = प्रविष्टिः_सूचकांक(l, 0, 0, l.coords + 1);
        softmax_वृक्षः(net.निवेशः_gpu + सूचकांक, l.w*l.h, l.batch*l.n, l.inputs/l.n, 1, l.output_gpu + सूचकांक, *l.softmax_वृक्षः);
    } अन्यथा यदि (l.softmax) {
        पूर्णाङ्क सूचकांक = प्रविष्टिः_सूचकांक(l, 0, 0, l.coords + !l.background);
        softmax_gpu(net.निवेशः_gpu + सूचकांक, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu + सूचकांक);
    }
    यदि(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        प्रत्यागम;
    }

    cuda_pull_array(l.output_gpu, net.निवेशः, l.batch*l.inputs);
    forward_region_layer(l, net);
    //cuda_push_array(l.output_gpu, l.output, l.batch*l.outputs);
    यदि(!net.train) प्रत्यागम;
    cuda_प्रक्षिप_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

शून्य backward_region_layer_gpu(ध्रुव layer l, जालम् net)
{
    पूर्णाङ्क b, n;
    प्रति (b = 0; b < l.batch; ++b){
        प्रति(n = 0; n < l.n; ++n){
            पूर्णाङ्क सूचकांक = प्रविष्टिः_सूचकांक(l, b, n*l.w*l.h, 0);
            gradient_array_gpu(l.output_gpu + सूचकांक, 2*l.w*l.h, LOGISTIC, l.delta_gpu + सूचकांक);
            यदि(l.coords > 4){
                सूचकांक = प्रविष्टिः_सूचकांक(l, b, n*l.w*l.h, 4);
                gradient_array_gpu(l.output_gpu + सूचकांक, (l.coords - 4)*l.w*l.h, LOGISTIC, l.delta_gpu + सूचकांक);
            }
            सूचकांक = प्रविष्टिः_सूचकांक(l, b, n*l.w*l.h, l.coords);
            यदि(!l.background) gradient_array_gpu(l.output_gpu + सूचकांक,   l.w*l.h, LOGISTIC, l.delta_gpu + सूचकांक);
        }
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#यदि_अन्तः

शून्य zero_objectness(layer l)
{
    पूर्णाङ्क i, n;
    प्रति (i = 0; i < l.w*l.h; ++i){
        प्रति(n = 0; n < l.n; ++n){
            पूर्णाङ्क obj_सूचकांक = प्रविष्टिः_सूचकांक(l, 0, n*l.w*l.h + i, l.coords);
            l.output[obj_सूचकांक] = 0;
        }
    }
}


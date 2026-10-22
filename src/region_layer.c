#అన్తర్భావ "region_layer.h"
#అన్తర్భావ "activations.h"
#అన్తర్భావ "blas.h"
#అన్తర్భావ "box.h"
#అన్తర్భావ "cuda.h"
#అన్తర్భావ "utils.h"

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <assert.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <stdlib.h>

layer make_region_layer(పూర్ణాంకం batch, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం n, పూర్ణాంకం classes, పూర్ణాంకం coords)
{
    layer l = {0};
    l.type = REGION;

    l.n = n;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = n*(classes + coords + 1);
    l.బహిర్గతం_w = l.w;
    l.బహిర్గతం_h = l.h;
    l.బహిర్గతం_c = l.c;
    l.classes = classes;
    l.coords = coords;
    l.ఖర్చు_2 = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(దశాంశం));
    l.biases = సున్నా_జ్ఞాపకం_ఇవ్వు(n*2, పరిమాణం(దశాంశం));
    l.bias_updates = సున్నా_జ్ఞాపకం_ఇవ్వు(n*2, పరిమాణం(దశాంశం));
    l.outputs = h*w*n*(classes + coords + 1);
    l.inputs = l.outputs;
    l.truths = 30*(l.coords + 1);
    l.delta = సున్నా_జ్ఞాపకం_ఇవ్వు(batch*l.outputs, పరిమాణం(దశాంశం));
    l.output = సున్నా_జ్ఞాపకం_ఇవ్వు(batch*l.outputs, పరిమాణం(దశాంశం));
    పూర్ణాంకం i;
    కొరకు(i = 0; i < n*2; ++i){
        l.biases[i] = .5;
    }

    l.forward = forward_region_layer;
    l.backward = backward_region_layer;
#నిర్వచితమైతే GPU
    l.forward_gpu = forward_region_layer_gpu;
    l.backward_gpu = backward_region_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#అయితే_ముగింపు

    fprintf(stderr, "detection\n");
    srand(0);

    తిరిగి l;
}

శూన్య resize_region_layer(layer *l, పూర్ణాంకం w, పూర్ణాంకం h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + l->coords + 1);
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*పరిమాణం(దశాంశం));
    l->delta = realloc(l->delta, l->batch*l->outputs*పరిమాణం(దశాంశం));

#నిర్వచితమైతే GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#అయితే_ముగింపు
}

box get_region_box(దశాంశం *x, దశాంశం *biases, పూర్ణాంకం n, పూర్ణాంకం సూచీ, పూర్ణాంకం i, పూర్ణాంకం j, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం stride)
{
    box b;
    b.x = (i + x[సూచీ + 0*stride]) / w;
    b.y = (j + x[సూచీ + 1*stride]) / h;
    b.w = ఘాతాంకం(x[సూచీ + 2*stride]) * biases[2*n]   / w;
    b.h = ఘాతాంకం(x[సూచీ + 3*stride]) * biases[2*n+1] / h;
    తిరిగి b;
}

దశాంశం delta_region_box(box truth, దశాంశం *x, దశాంశం *biases, పూర్ణాంకం n, పూర్ణాంకం సూచీ, పూర్ణాంకం i, పూర్ణాంకం j, పూర్ణాంకం w, పూర్ణాంకం h, దశాంశం *delta, దశాంశం scale, పూర్ణాంకం stride)
{
    box pred = get_region_box(x, biases, n, సూచీ, i, j, w, h, stride);
    దశాంశం iou = box_iou(pred, truth);

    దశాంశం tx = (truth.x*w - i);
    దశాంశం ty = (truth.y*h - j);
    దశాంశం tw = సంవర్గమానం(truth.w*w / biases[2*n]);
    దశాంశం th = సంవర్గమానం(truth.h*h / biases[2*n + 1]);

    delta[సూచీ + 0*stride] = scale * (tx - x[సూచీ + 0*stride]);
    delta[సూచీ + 1*stride] = scale * (ty - x[సూచీ + 1*stride]);
    delta[సూచీ + 2*stride] = scale * (tw - x[సూచీ + 2*stride]);
    delta[సూచీ + 3*stride] = scale * (th - x[సూచీ + 3*stride]);
    తిరిగి iou;
}

శూన్య delta_region_ముసుగు(దశాంశం *truth, దశాంశం *x, పూర్ణాంకం n, పూర్ణాంకం సూచీ, దశాంశం *delta, పూర్ణాంకం stride, పూర్ణాంకం scale)
{
    పూర్ణాంకం i;
    కొరకు(i = 0; i < n; ++i){
        delta[సూచీ + i*stride] = scale*(truth[i] - x[సూచీ + i*stride]);
    }
}


శూన్య delta_region_class(దశాంశం *output, దశాంశం *delta, పూర్ణాంకం సూచీ, పూర్ణాంకం class, పూర్ణాంకం classes, వృక్షం *hier, దశాంశం scale, పూర్ణాంకం stride, దశాంశం *avg_cat, పూర్ణాంకం ట్యాగ్)
{
    పూర్ణాంకం i, n;
    ఒకవేళ(hier){
        దశాంశం pred = 1;
        అయితే(class >= 0){
            pred *= output[సూచీ + stride*class];
            పూర్ణాంకం g = hier->సమూహం[class];
            పూర్ణాంకం స్థానభ్రంశం = hier->సమూహం_స్థానభ్రంశం[g];
            కొరకు(i = 0; i < hier->సమూహం_కొలత[g]; ++i){
                delta[సూచీ + stride*(స్థానభ్రంశం + i)] = scale * (0 - output[సూచీ + stride*(స్థానభ్రంశం + i)]);
            }
            delta[సూచీ + stride*class] = scale * (1 - output[సూచీ + stride*class]);

            class = hier->మాతృ[class];
        }
        *avg_cat += pred;
    } లేకపోతే {
        ఒకవేళ (delta[సూచీ] && ట్యాగ్){
            delta[సూచీ + stride*class] = scale * (1 - output[సూచీ + stride*class]);
            తిరిగి;
        }
        కొరకు(n = 0; n < classes; ++n){
            delta[సూచీ + stride*n] = scale * (((n == class)?1 : 0) - output[సూచీ + stride*n]);
            ఒకవేళ(n == class) *avg_cat += output[సూచీ + stride*n];
        }
    }
}

దశాంశం logit(దశాంశం x)
{
    తిరిగి సంవర్గమానం(x/(1.-x));
}

దశాంశం tisnan(దశాంశం x)
{
    తిరిగి (x != x);
}

పూర్ణాంకం నమోదు_సూచీ(layer l, పూర్ణాంకం batch, పూర్ణాంకం location, పూర్ణాంకం నమోదు_2)
{
    పూర్ణాంకం n =   location / (l.w*l.h);
    పూర్ణాంకం loc = location % (l.w*l.h);
    తిరిగి batch*l.outputs + n*l.w*l.h*(l.coords+l.classes+1) + నమోదు_2*l.w*l.h + loc;
}

శూన్య forward_region_layer(స్థిరరాశి layer l, జాలం net)
{
    పూర్ణాంకం i,j,b,t,n;
    జ్ఞాపకం_నకలు(l.output, net.ఆదానం, l.outputs*l.batch*పరిమాణం(దశాంశం));

#నిర్వచితం_కాకపోతే GPU
    కొరకు (b = 0; b < l.batch; ++b){
        కొరకు(n = 0; n < l.n; ++n){
            పూర్ణాంకం సూచీ = నమోదు_సూచీ(l, b, n*l.w*l.h, 0);
            activate_array(l.output + సూచీ, 2*l.w*l.h, LOGISTIC);
            సూచీ = నమోదు_సూచీ(l, b, n*l.w*l.h, l.coords);
            ఒకవేళ(!l.background) activate_array(l.output + సూచీ,   l.w*l.h, LOGISTIC);
            సూచీ = నమోదు_సూచీ(l, b, n*l.w*l.h, l.coords + 1);
            ఒకవేళ(!l.softmax && !l.softmax_వృక్షం) activate_array(l.output + సూచీ, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    ఒకవేళ (l.softmax_వృక్షం){
        పూర్ణాంకం i;
        పూర్ణాంకం లెక్క = l.coords + 1;
        కొరకు (i = 0; i < l.softmax_వృక్షం->groups; ++i) {
            పూర్ణాంకం సమూహం_కొలత = l.softmax_వృక్షం->సమూహం_కొలత[i];
            softmax_cpu(net.ఆదానం + లెక్క, సమూహం_కొలత, l.batch, l.inputs, l.n*l.w*l.h, 1, l.n*l.w*l.h, l.temperature, l.output + లెక్క);
            లెక్క += సమూహం_కొలత;
        }
    } లేకపోతే ఒకవేళ (l.softmax){
        పూర్ణాంకం సూచీ = నమోదు_సూచీ(l, 0, 0, l.coords + !l.background);
        softmax_cpu(net.ఆదానం + సూచీ, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output + సూచీ);
    }
#అయితే_ముగింపు

    memset(l.delta, 0, l.outputs * l.batch * పరిమాణం(దశాంశం));
    ఒకవేళ(!net.train) తిరిగి;
    దశాంశం avg_iou = 0;
    దశాంశం recall = 0;
    దశాంశం avg_cat = 0;
    దశాంశం avg_obj = 0;
    దశాంశం avg_anyobj = 0;
    పూర్ణాంకం లెక్క = 0;
    పూర్ణాంకం class_లెక్క = 0;
    *(l.ఖర్చు_2) = 0;
    కొరకు (b = 0; b < l.batch; ++b) {
        ఒకవేళ(l.softmax_వృక్షం){
            పూర్ణాంకం onlyclass = 0;
            కొరకు(t = 0; t < 30; ++t){
                box truth = float_గమ్యం_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                ఒకవేళ(!truth.x) ఆపు;
                పూర్ణాంకం class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
                దశాంశం maxp = 0;
                పూర్ణాంకం maxi = 0;
                ఒకవేళ(truth.x > 100000 && truth.y > 100000){
                    కొరకు(n = 0; n < l.n*l.w*l.h; ++n){
                        పూర్ణాంకం class_సూచీ = నమోదు_సూచీ(l, b, n, l.coords + 1);
                        పూర్ణాంకం obj_సూచీ = నమోదు_సూచీ(l, b, n, l.coords);
                        దశాంశం scale =  l.output[obj_సూచీ];
                        l.delta[obj_సూచీ] = l.noobject_scale * (0 - l.output[obj_సూచీ]);
                        దశాంశం p = scale*get_hierarchy_probability(l.output + class_సూచీ, l.softmax_వృక్షం, class, l.w*l.h);
                        ఒకవేళ(p > maxp){
                            maxp = p;
                            maxi = n;
                        }
                    }
                    పూర్ణాంకం class_సూచీ = నమోదు_సూచీ(l, b, maxi, l.coords + 1);
                    పూర్ణాంకం obj_సూచీ = నమోదు_సూచీ(l, b, maxi, l.coords);
                    delta_region_class(l.output, l.delta, class_సూచీ, class, l.classes, l.softmax_వృక్షం, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
                    ఒకవేళ(l.output[obj_సూచీ] < .3) l.delta[obj_సూచీ] = l.object_scale * (.3 - l.output[obj_సూచీ]);
                    లేకపోతే  l.delta[obj_సూచీ] = 0;
                    l.delta[obj_సూచీ] = 0;
                    ++class_లెక్క;
                    onlyclass = 1;
                    ఆపు;
                }
            }
            ఒకవేళ(onlyclass) కొనసాగు;
        }
        కొరకు (j = 0; j < l.h; ++j) {
            కొరకు (i = 0; i < l.w; ++i) {
                కొరకు (n = 0; n < l.n; ++n) {
                    పూర్ణాంకం box_సూచీ = నమోదు_సూచీ(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_region_box(l.output, l.biases, n, box_సూచీ, i, j, l.w, l.h, l.w*l.h);
                    దశాంశం ఉత్తమ_iou = 0;
                    కొరకు(t = 0; t < 30; ++t){
                        box truth = float_గమ్యం_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                        ఒకవేళ(!truth.x) ఆపు;
                        దశాంశం iou = box_iou(pred, truth);
                        ఒకవేళ (iou > ఉత్తమ_iou) {
                            ఉత్తమ_iou = iou;
                        }
                    }
                    పూర్ణాంకం obj_సూచీ = నమోదు_సూచీ(l, b, n*l.w*l.h + j*l.w + i, l.coords);
                    avg_anyobj += l.output[obj_సూచీ];
                    l.delta[obj_సూచీ] = l.noobject_scale * (0 - l.output[obj_సూచీ]);
                    ఒకవేళ(l.background) l.delta[obj_సూచీ] = l.noobject_scale * (1 - l.output[obj_సూచీ]);
                    ఒకవేళ (ఉత్తమ_iou > l.thresh) {
                        l.delta[obj_సూచీ] = 0;
                    }

                    ఒకవేళ(*(net.చూసిన) < 12800){
                        box truth = {0};
                        truth.x = (i + .5)/l.w;
                        truth.y = (j + .5)/l.h;
                        truth.w = l.biases[2*n]/l.w;
                        truth.h = l.biases[2*n+1]/l.h;
                        delta_region_box(truth, l.output, l.biases, n, box_సూచీ, i, j, l.w, l.h, l.delta, .01, l.w*l.h);
                    }
                }
            }
        }
        కొరకు(t = 0; t < 30; ++t){
            box truth = float_గమ్యం_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);

            ఒకవేళ(!truth.x) ఆపు;
            దశాంశం ఉత్తమ_iou = 0;
            పూర్ణాంకం ఉత్తమ_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_జరుపు = truth;
            truth_జరుపు.x = 0;
            truth_జరుపు.y = 0;
            కొరకు(n = 0; n < l.n; ++n){
                పూర్ణాంకం box_సూచీ = నమోదు_సూచీ(l, b, n*l.w*l.h + j*l.w + i, 0);
                box pred = get_region_box(l.output, l.biases, n, box_సూచీ, i, j, l.w, l.h, l.w*l.h);
                ఒకవేళ(l.bias_సరిపోలిక){
                    pred.w = l.biases[2*n]/l.w;
                    pred.h = l.biases[2*n+1]/l.h;
                }
                pred.x = 0;
                pred.y = 0;
                దశాంశం iou = box_iou(pred, truth_జరుపు);
                ఒకవేళ (iou > ఉత్తమ_iou){
                    ఉత్తమ_iou = iou;
                    ఉత్తమ_n = n;
                }
            }

            పూర్ణాంకం box_సూచీ = నమోదు_సూచీ(l, b, ఉత్తమ_n*l.w*l.h + j*l.w + i, 0);
            దశాంశం iou = delta_region_box(truth, l.output, l.biases, ఉత్తమ_n, box_సూచీ, i, j, l.w, l.h, l.delta, l.coord_scale *  (2 - truth.w*truth.h), l.w*l.h);
            ఒకవేళ(l.coords > 4){
                పూర్ణాంకం ముసుగు_సూచీ = నమోదు_సూచీ(l, b, ఉత్తమ_n*l.w*l.h + j*l.w + i, 4);
                delta_region_ముసుగు(net.truth + t*(l.coords + 1) + b*l.truths + 5, l.output, l.coords - 4, ముసుగు_సూచీ, l.delta, l.w*l.h, l.ముసుగు_scale);
            }
            ఒకవేళ(iou > .5) recall += 1;
            avg_iou += iou;

            పూర్ణాంకం obj_సూచీ = నమోదు_సూచీ(l, b, ఉత్తమ_n*l.w*l.h + j*l.w + i, l.coords);
            avg_obj += l.output[obj_సూచీ];
            l.delta[obj_సూచీ] = l.object_scale * (1 - l.output[obj_సూచీ]);
            ఒకవేళ (l.rescore) {
                l.delta[obj_సూచీ] = l.object_scale * (iou - l.output[obj_సూచీ]);
            }
            ఒకవేళ(l.background){
                l.delta[obj_సూచీ] = l.object_scale * (0 - l.output[obj_సూచీ]);
            }

            పూర్ణాంకం class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
            ఒకవేళ (l.map) class = l.map[class];
            పూర్ణాంకం class_సూచీ = నమోదు_సూచీ(l, b, ఉత్తమ_n*l.w*l.h + j*l.w + i, l.coords + 1);
            delta_region_class(l.output, l.delta, class_సూచీ, class, l.classes, l.softmax_వృక్షం, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
            ++లెక్క;
            ++class_లెక్క;
        }
    }
    *(l.ఖర్చు_2) = ఘాతం(mag_array(l.delta, l.outputs * l.batch), 2);
    రాయి("Region Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, Avg Recall: %f,  count: %d\n", avg_iou/లెక్క, avg_cat/class_లెక్క, avg_obj/లెక్క, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/లెక్క, లెక్క);
}

శూన్య backward_region_layer(స్థిరరాశి layer l, జాలం net)
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

శూన్య correct_region_boxes(detection *dets, పూర్ణాంకం n, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం netw, పూర్ణాంకం neth, పూర్ణాంకం relative)
{
    పూర్ణాంకం i;
    పూర్ణాంకం new_w=0;
    పూర్ణాంకం new_h=0;
    ఒకవేళ (((దశాంశం)netw/w) < ((దశాంశం)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } లేకపోతే {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    కొరకు (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((దశాంశం)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((దశాంశం)new_h/neth); 
        b.w *= (దశాంశం)netw/new_w;
        b.h *= (దశాంశం)neth/new_h;
        ఒకవేళ(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

శూన్య get_region_detections(layer l, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం netw, పూర్ణాంకం neth, దశాంశం thresh, పూర్ణాంకం *map, దశాంశం వృక్షం_thresh, పూర్ణాంకం relative, detection *dets)
{
    పూర్ణాంకం i,j,n,z;
    దశాంశం *predictions = l.output;
    ఒకవేళ (l.batch == 2) {
        దశాంశం *flip = l.output + l.outputs;
        కొరకు (j = 0; j < l.h; ++j) {
            కొరకు (i = 0; i < l.w/2; ++i) {
                కొరకు (n = 0; n < l.n; ++n) {
                    కొరకు(z = 0; z < l.classes + l.coords + 1; ++z){
                        పూర్ణాంకం i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                        పూర్ణాంకం i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                        దశాంశం మార్పిడి = flip[i1];
                        flip[i1] = flip[i2];
                        flip[i2] = మార్పిడి;
                        ఒకవేళ(z == 0){
                            flip[i1] = -flip[i1];
                            flip[i2] = -flip[i2];
                        }
                    }
                }
            }
        }
        కొరకు(i = 0; i < l.outputs; ++i){
            l.output[i] = (l.output[i] + flip[i])/2.;
        }
    }
    కొరకు (i = 0; i < l.w*l.h; ++i){
        పూర్ణాంకం అడ్డువరుస = i / l.w;
        పూర్ణాంకం నిలువు = i % l.w;
        కొరకు(n = 0; n < l.n; ++n){
            పూర్ణాంకం సూచీ = n*l.w*l.h + i;
            కొరకు(j = 0; j < l.classes; ++j){
                dets[సూచీ].prob[j] = 0;
            }
            పూర్ణాంకం obj_సూచీ  = నమోదు_సూచీ(l, 0, n*l.w*l.h + i, l.coords);
            పూర్ణాంకం box_సూచీ  = నమోదు_సూచీ(l, 0, n*l.w*l.h + i, 0);
            పూర్ణాంకం ముసుగు_సూచీ = నమోదు_సూచీ(l, 0, n*l.w*l.h + i, 4);
            దశాంశం scale = l.background ? 1 : predictions[obj_సూచీ];
            dets[సూచీ].bbox = get_region_box(predictions, l.biases, n, box_సూచీ, నిలువు, అడ్డువరుస, l.w, l.h, l.w*l.h);
            dets[సూచీ].objectness = scale > thresh ? scale : 0;
            ఒకవేళ(dets[సూచీ].ముసుగు){
                కొరకు(j = 0; j < l.coords - 4; ++j){
                    dets[సూచీ].ముసుగు[j] = l.output[ముసుగు_సూచీ + j*l.w*l.h];
                }
            }

            పూర్ణాంకం class_సూచీ = నమోదు_సూచీ(l, 0, n*l.w*l.h + i, l.coords + !l.background);
            ఒకవేళ(l.softmax_వృక్షం){

                hierarchy_predictions(predictions + class_సూచీ, l.classes, l.softmax_వృక్షం, 0, l.w*l.h);
                ఒకవేళ(map){
                    కొరకు(j = 0; j < 200; ++j){
                        పూర్ణాంకం class_సూచీ = నమోదు_సూచీ(l, 0, n*l.w*l.h + i, l.coords + 1 + map[j]);
                        దశాంశం prob = scale*predictions[class_సూచీ];
                        dets[సూచీ].prob[j] = (prob > thresh) ? prob : 0;
                    }
                } లేకపోతే {
                    పూర్ణాంకం j =  hierarchy_శిఖరం_prediction(predictions + class_సూచీ, l.softmax_వృక్షం, వృక్షం_thresh, l.w*l.h);
                    dets[సూచీ].prob[j] = (scale > thresh) ? scale : 0;
                }
            } లేకపోతే {
                ఒకవేళ(dets[సూచీ].objectness){
                    కొరకు(j = 0; j < l.classes; ++j){
                        పూర్ణాంకం class_సూచీ = నమోదు_సూచీ(l, 0, n*l.w*l.h + i, l.coords + 1 + j);
                        దశాంశం prob = scale*predictions[class_సూచీ];
                        dets[సూచీ].prob[j] = (prob > thresh) ? prob : 0;
                    }
                }
            }
        }
    }
    correct_region_boxes(dets, l.w*l.h*l.n, w, h, netw, neth, relative);
}

#నిర్వచితమైతే GPU

శూన్య forward_region_layer_gpu(స్థిరరాశి layer l, జాలం net)
{
    copy_gpu(l.batch*l.inputs, net.ఆదానం_gpu, 1, l.output_gpu, 1);
    పూర్ణాంకం b, n;
    కొరకు (b = 0; b < l.batch; ++b){
        కొరకు(n = 0; n < l.n; ++n){
            పూర్ణాంకం సూచీ = నమోదు_సూచీ(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + సూచీ, 2*l.w*l.h, LOGISTIC);
            ఒకవేళ(l.coords > 4){
                సూచీ = నమోదు_సూచీ(l, b, n*l.w*l.h, 4);
                activate_array_gpu(l.output_gpu + సూచీ, (l.coords - 4)*l.w*l.h, LOGISTIC);
            }
            సూచీ = నమోదు_సూచీ(l, b, n*l.w*l.h, l.coords);
            ఒకవేళ(!l.background) activate_array_gpu(l.output_gpu + సూచీ,   l.w*l.h, LOGISTIC);
            సూచీ = నమోదు_సూచీ(l, b, n*l.w*l.h, l.coords + 1);
            ఒకవేళ(!l.softmax && !l.softmax_వృక్షం) activate_array_gpu(l.output_gpu + సూచీ, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    ఒకవేళ (l.softmax_వృక్షం){
        పూర్ణాంకం సూచీ = నమోదు_సూచీ(l, 0, 0, l.coords + 1);
        softmax_వృక్షం(net.ఆదానం_gpu + సూచీ, l.w*l.h, l.batch*l.n, l.inputs/l.n, 1, l.output_gpu + సూచీ, *l.softmax_వృక్షం);
    } లేకపోతే ఒకవేళ (l.softmax) {
        పూర్ణాంకం సూచీ = నమోదు_సూచీ(l, 0, 0, l.coords + !l.background);
        softmax_gpu(net.ఆదానం_gpu + సూచీ, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu + సూచీ);
    }
    ఒకవేళ(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        తిరిగి;
    }

    cuda_pull_array(l.output_gpu, net.ఆదానం, l.batch*l.inputs);
    forward_region_layer(l, net);
    //cuda_push_array(l.output_gpu, l.output, l.batch*l.outputs);
    ఒకవేళ(!net.train) తిరిగి;
    cuda_నెట్టు_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

శూన్య backward_region_layer_gpu(స్థిరరాశి layer l, జాలం net)
{
    పూర్ణాంకం b, n;
    కొరకు (b = 0; b < l.batch; ++b){
        కొరకు(n = 0; n < l.n; ++n){
            పూర్ణాంకం సూచీ = నమోదు_సూచీ(l, b, n*l.w*l.h, 0);
            gradient_array_gpu(l.output_gpu + సూచీ, 2*l.w*l.h, LOGISTIC, l.delta_gpu + సూచీ);
            ఒకవేళ(l.coords > 4){
                సూచీ = నమోదు_సూచీ(l, b, n*l.w*l.h, 4);
                gradient_array_gpu(l.output_gpu + సూచీ, (l.coords - 4)*l.w*l.h, LOGISTIC, l.delta_gpu + సూచీ);
            }
            సూచీ = నమోదు_సూచీ(l, b, n*l.w*l.h, l.coords);
            ఒకవేళ(!l.background) gradient_array_gpu(l.output_gpu + సూచీ,   l.w*l.h, LOGISTIC, l.delta_gpu + సూచీ);
        }
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#అయితే_ముగింపు

శూన్య zero_objectness(layer l)
{
    పూర్ణాంకం i, n;
    కొరకు (i = 0; i < l.w*l.h; ++i){
        కొరకు(n = 0; n < l.n; ++n){
            పూర్ణాంకం obj_సూచీ = నమోదు_సూచీ(l, 0, n*l.w*l.h + i, l.coords);
            l.output[obj_సూచీ] = 0;
        }
    }
}


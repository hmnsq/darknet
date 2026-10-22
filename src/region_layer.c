#ضمن "region_layer.h"
#ضمن "activations.h"
#ضمن "blas.h"
#ضمن "box.h"
#ضمن "cuda.h"
#ضمن "utils.h"

#ضمن <stdio.h>
#ضمن <assert.h>
#ضمن <string.h>
#ضمن <stdlib.h>

layer make_region_layer(صحيح batch, صحيح w, صحيح h, صحيح n, صحيح classes, صحيح coords)
{
    layer l = {0};
    l.type = REGION;

    l.n = n;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = n*(classes + coords + 1);
    l.بهر_w = l.w;
    l.بهر_h = l.h;
    l.بهر_c = l.c;
    l.classes = classes;
    l.coords = coords;
    l.لګښت_2 = حافظه_صفر_ورکړه(1, حجم(عائم));
    l.biases = حافظه_صفر_ورکړه(n*2, حجم(عائم));
    l.bias_updates = حافظه_صفر_ورکړه(n*2, حجم(عائم));
    l.outputs = h*w*n*(classes + coords + 1);
    l.inputs = l.outputs;
    l.truths = 30*(l.coords + 1);
    l.delta = حافظه_صفر_ورکړه(batch*l.outputs, حجم(عائم));
    l.output = حافظه_صفر_ورکړه(batch*l.outputs, حجم(عائم));
    صحيح i;
    لكل(i = 0; i < n*2; ++i){
        l.biases[i] = .5;
    }

    l.forward = forward_region_layer;
    l.backward = backward_region_layer;
#که_تعریف_شوی GPU
    l.forward_gpu = forward_region_layer_gpu;
    l.backward_gpu = backward_region_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#د_که_پای

    fprintf(stderr, "detection\n");
    srand(0);

    أرجع l;
}

فراغ resize_region_layer(layer *l, صحيح w, صحيح h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + l->coords + 1);
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*حجم(عائم));
    l->delta = realloc(l->delta, l->batch*l->outputs*حجم(عائم));

#که_تعریف_شوی GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#د_که_پای
}

box get_region_box(عائم *x, عائم *biases, صحيح n, صحيح شاخص, صحيح i, صحيح j, صحيح w, صحيح h, صحيح stride)
{
    box b;
    b.x = (i + x[شاخص + 0*stride]) / w;
    b.y = (j + x[شاخص + 1*stride]) / h;
    b.w = توانیز_تابع(x[شاخص + 2*stride]) * biases[2*n]   / w;
    b.h = توانیز_تابع(x[شاخص + 3*stride]) * biases[2*n+1] / h;
    أرجع b;
}

عائم delta_region_box(box truth, عائم *x, عائم *biases, صحيح n, صحيح شاخص, صحيح i, صحيح j, صحيح w, صحيح h, عائم *delta, عائم scale, صحيح stride)
{
    box pred = get_region_box(x, biases, n, شاخص, i, j, w, h, stride);
    عائم iou = box_iou(pred, truth);

    عائم tx = (truth.x*w - i);
    عائم ty = (truth.y*h - j);
    عائم tw = لوګاریتم(truth.w*w / biases[2*n]);
    عائم th = لوګاریتم(truth.h*h / biases[2*n + 1]);

    delta[شاخص + 0*stride] = scale * (tx - x[شاخص + 0*stride]);
    delta[شاخص + 1*stride] = scale * (ty - x[شاخص + 1*stride]);
    delta[شاخص + 2*stride] = scale * (tw - x[شاخص + 2*stride]);
    delta[شاخص + 3*stride] = scale * (th - x[شاخص + 3*stride]);
    أرجع iou;
}

فراغ delta_region_ماسک(عائم *truth, عائم *x, صحيح n, صحيح شاخص, عائم *delta, صحيح stride, صحيح scale)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        delta[شاخص + i*stride] = scale*(truth[i] - x[شاخص + i*stride]);
    }
}


فراغ delta_region_class(عائم *output, عائم *delta, صحيح شاخص, صحيح class, صحيح classes, ونه *hier, عائم scale, صحيح stride, عائم *avg_cat, صحيح نښان)
{
    صحيح i, n;
    إذا(hier){
        عائم pred = 1;
        طالما(class >= 0){
            pred *= output[شاخص + stride*class];
            صحيح g = hier->ډله[class];
            صحيح دننه_واټن = hier->ډله_دننه_واټن[g];
            لكل(i = 0; i < hier->ډله_اندازه[g]; ++i){
                delta[شاخص + stride*(دننه_واټن + i)] = scale * (0 - output[شاخص + stride*(دننه_واټن + i)]);
            }
            delta[شاخص + stride*class] = scale * (1 - output[شاخص + stride*class]);

            class = hier->پلار[class];
        }
        *avg_cat += pred;
    } وإلا {
        إذا (delta[شاخص] && نښان){
            delta[شاخص + stride*class] = scale * (1 - output[شاخص + stride*class]);
            أرجع;
        }
        لكل(n = 0; n < classes; ++n){
            delta[شاخص + stride*n] = scale * (((n == class)?1 : 0) - output[شاخص + stride*n]);
            إذا(n == class) *avg_cat += output[شاخص + stride*n];
        }
    }
}

عائم logit(عائم x)
{
    أرجع لوګاریتم(x/(1.-x));
}

عائم tisnan(عائم x)
{
    أرجع (x != x);
}

صحيح ثبت_شاخص(layer l, صحيح batch, صحيح location, صحيح ثبت)
{
    صحيح n =   location / (l.w*l.h);
    صحيح loc = location % (l.w*l.h);
    أرجع batch*l.outputs + n*l.w*l.h*(l.coords+l.classes+1) + ثبت*l.w*l.h + loc;
}

فراغ forward_region_layer(ثابت layer l, شبکه net)
{
    صحيح i,j,b,t,n;
    حافظه_نقل(l.output, net.ننوت, l.outputs*l.batch*حجم(عائم));

#که_نه_تعریف_شوی GPU
    لكل (b = 0; b < l.batch; ++b){
        لكل(n = 0; n < l.n; ++n){
            صحيح شاخص = ثبت_شاخص(l, b, n*l.w*l.h, 0);
            activate_array(l.output + شاخص, 2*l.w*l.h, LOGISTIC);
            شاخص = ثبت_شاخص(l, b, n*l.w*l.h, l.coords);
            إذا(!l.background) activate_array(l.output + شاخص,   l.w*l.h, LOGISTIC);
            شاخص = ثبت_شاخص(l, b, n*l.w*l.h, l.coords + 1);
            إذا(!l.softmax && !l.softmax_ونه) activate_array(l.output + شاخص, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    إذا (l.softmax_ونه){
        صحيح i;
        صحيح شمېر = l.coords + 1;
        لكل (i = 0; i < l.softmax_ونه->groups; ++i) {
            صحيح ډله_اندازه = l.softmax_ونه->ډله_اندازه[i];
            softmax_cpu(net.ننوت + شمېر, ډله_اندازه, l.batch, l.inputs, l.n*l.w*l.h, 1, l.n*l.w*l.h, l.temperature, l.output + شمېر);
            شمېر += ډله_اندازه;
        }
    } وإلا إذا (l.softmax){
        صحيح شاخص = ثبت_شاخص(l, 0, 0, l.coords + !l.background);
        softmax_cpu(net.ننوت + شاخص, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output + شاخص);
    }
#د_که_پای

    memset(l.delta, 0, l.outputs * l.batch * حجم(عائم));
    إذا(!net.train) أرجع;
    عائم avg_iou = 0;
    عائم recall = 0;
    عائم avg_cat = 0;
    عائم avg_obj = 0;
    عائم avg_anyobj = 0;
    صحيح شمېر = 0;
    صحيح class_شمېر = 0;
    *(l.لګښت_2) = 0;
    لكل (b = 0; b < l.batch; ++b) {
        إذا(l.softmax_ونه){
            صحيح onlyclass = 0;
            لكل(t = 0; t < 30; ++t){
                box truth = float_منزل_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                إذا(!truth.x) اكسر;
                صحيح class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
                عائم maxp = 0;
                صحيح maxi = 0;
                إذا(truth.x > 100000 && truth.y > 100000){
                    لكل(n = 0; n < l.n*l.w*l.h; ++n){
                        صحيح class_شاخص = ثبت_شاخص(l, b, n, l.coords + 1);
                        صحيح obj_شاخص = ثبت_شاخص(l, b, n, l.coords);
                        عائم scale =  l.output[obj_شاخص];
                        l.delta[obj_شاخص] = l.noobject_scale * (0 - l.output[obj_شاخص]);
                        عائم p = scale*get_hierarchy_probability(l.output + class_شاخص, l.softmax_ونه, class, l.w*l.h);
                        إذا(p > maxp){
                            maxp = p;
                            maxi = n;
                        }
                    }
                    صحيح class_شاخص = ثبت_شاخص(l, b, maxi, l.coords + 1);
                    صحيح obj_شاخص = ثبت_شاخص(l, b, maxi, l.coords);
                    delta_region_class(l.output, l.delta, class_شاخص, class, l.classes, l.softmax_ونه, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
                    إذا(l.output[obj_شاخص] < .3) l.delta[obj_شاخص] = l.object_scale * (.3 - l.output[obj_شاخص]);
                    وإلا  l.delta[obj_شاخص] = 0;
                    l.delta[obj_شاخص] = 0;
                    ++class_شمېر;
                    onlyclass = 1;
                    اكسر;
                }
            }
            إذا(onlyclass) استمر;
        }
        لكل (j = 0; j < l.h; ++j) {
            لكل (i = 0; i < l.w; ++i) {
                لكل (n = 0; n < l.n; ++n) {
                    صحيح box_شاخص = ثبت_شاخص(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_region_box(l.output, l.biases, n, box_شاخص, i, j, l.w, l.h, l.w*l.h);
                    عائم غوره_iou = 0;
                    لكل(t = 0; t < 30; ++t){
                        box truth = float_منزل_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                        إذا(!truth.x) اكسر;
                        عائم iou = box_iou(pred, truth);
                        إذا (iou > غوره_iou) {
                            غوره_iou = iou;
                        }
                    }
                    صحيح obj_شاخص = ثبت_شاخص(l, b, n*l.w*l.h + j*l.w + i, l.coords);
                    avg_anyobj += l.output[obj_شاخص];
                    l.delta[obj_شاخص] = l.noobject_scale * (0 - l.output[obj_شاخص]);
                    إذا(l.background) l.delta[obj_شاخص] = l.noobject_scale * (1 - l.output[obj_شاخص]);
                    إذا (غوره_iou > l.thresh) {
                        l.delta[obj_شاخص] = 0;
                    }

                    إذا(*(net.کتل_شوی) < 12800){
                        box truth = {0};
                        truth.x = (i + .5)/l.w;
                        truth.y = (j + .5)/l.h;
                        truth.w = l.biases[2*n]/l.w;
                        truth.h = l.biases[2*n+1]/l.h;
                        delta_region_box(truth, l.output, l.biases, n, box_شاخص, i, j, l.w, l.h, l.delta, .01, l.w*l.h);
                    }
                }
            }
        }
        لكل(t = 0; t < 30; ++t){
            box truth = float_منزل_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);

            إذا(!truth.x) اكسر;
            عائم غوره_iou = 0;
            صحيح غوره_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_ښويدنه = truth;
            truth_ښويدنه.x = 0;
            truth_ښويدنه.y = 0;
            لكل(n = 0; n < l.n; ++n){
                صحيح box_شاخص = ثبت_شاخص(l, b, n*l.w*l.h + j*l.w + i, 0);
                box pred = get_region_box(l.output, l.biases, n, box_شاخص, i, j, l.w, l.h, l.w*l.h);
                إذا(l.bias_سمون){
                    pred.w = l.biases[2*n]/l.w;
                    pred.h = l.biases[2*n+1]/l.h;
                }
                pred.x = 0;
                pred.y = 0;
                عائم iou = box_iou(pred, truth_ښويدنه);
                إذا (iou > غوره_iou){
                    غوره_iou = iou;
                    غوره_n = n;
                }
            }

            صحيح box_شاخص = ثبت_شاخص(l, b, غوره_n*l.w*l.h + j*l.w + i, 0);
            عائم iou = delta_region_box(truth, l.output, l.biases, غوره_n, box_شاخص, i, j, l.w, l.h, l.delta, l.coord_scale *  (2 - truth.w*truth.h), l.w*l.h);
            إذا(l.coords > 4){
                صحيح ماسک_شاخص = ثبت_شاخص(l, b, غوره_n*l.w*l.h + j*l.w + i, 4);
                delta_region_ماسک(net.truth + t*(l.coords + 1) + b*l.truths + 5, l.output, l.coords - 4, ماسک_شاخص, l.delta, l.w*l.h, l.ماسک_scale);
            }
            إذا(iou > .5) recall += 1;
            avg_iou += iou;

            صحيح obj_شاخص = ثبت_شاخص(l, b, غوره_n*l.w*l.h + j*l.w + i, l.coords);
            avg_obj += l.output[obj_شاخص];
            l.delta[obj_شاخص] = l.object_scale * (1 - l.output[obj_شاخص]);
            إذا (l.rescore) {
                l.delta[obj_شاخص] = l.object_scale * (iou - l.output[obj_شاخص]);
            }
            إذا(l.background){
                l.delta[obj_شاخص] = l.object_scale * (0 - l.output[obj_شاخص]);
            }

            صحيح class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
            إذا (l.map) class = l.map[class];
            صحيح class_شاخص = ثبت_شاخص(l, b, غوره_n*l.w*l.h + j*l.w + i, l.coords + 1);
            delta_region_class(l.output, l.delta, class_شاخص, class, l.classes, l.softmax_ونه, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
            ++شمېر;
            ++class_شمېر;
        }
    }
    *(l.لګښت_2) = توان(mag_array(l.delta, l.outputs * l.batch), 2);
    چاپ_کړه("Region Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, Avg Recall: %f,  count: %d\n", avg_iou/شمېر, avg_cat/class_شمېر, avg_obj/شمېر, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/شمېر, شمېر);
}

فراغ backward_region_layer(ثابت layer l, شبکه net)
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

فراغ correct_region_boxes(detection *dets, صحيح n, صحيح w, صحيح h, صحيح netw, صحيح neth, صحيح relative)
{
    صحيح i;
    صحيح new_w=0;
    صحيح new_h=0;
    إذا (((عائم)netw/w) < ((عائم)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } وإلا {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    لكل (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((عائم)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((عائم)new_h/neth); 
        b.w *= (عائم)netw/new_w;
        b.h *= (عائم)neth/new_h;
        إذا(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

فراغ get_region_detections(layer l, صحيح w, صحيح h, صحيح netw, صحيح neth, عائم thresh, صحيح *map, عائم ونه_thresh, صحيح relative, detection *dets)
{
    صحيح i,j,n,z;
    عائم *predictions = l.output;
    إذا (l.batch == 2) {
        عائم *flip = l.output + l.outputs;
        لكل (j = 0; j < l.h; ++j) {
            لكل (i = 0; i < l.w/2; ++i) {
                لكل (n = 0; n < l.n; ++n) {
                    لكل(z = 0; z < l.classes + l.coords + 1; ++z){
                        صحيح i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                        صحيح i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                        عائم بدلون = flip[i1];
                        flip[i1] = flip[i2];
                        flip[i2] = بدلون;
                        إذا(z == 0){
                            flip[i1] = -flip[i1];
                            flip[i2] = -flip[i2];
                        }
                    }
                }
            }
        }
        لكل(i = 0; i < l.outputs; ++i){
            l.output[i] = (l.output[i] + flip[i])/2.;
        }
    }
    لكل (i = 0; i < l.w*l.h; ++i){
        صحيح لیکه = i / l.w;
        صحيح ستون = i % l.w;
        لكل(n = 0; n < l.n; ++n){
            صحيح شاخص = n*l.w*l.h + i;
            لكل(j = 0; j < l.classes; ++j){
                dets[شاخص].prob[j] = 0;
            }
            صحيح obj_شاخص  = ثبت_شاخص(l, 0, n*l.w*l.h + i, l.coords);
            صحيح box_شاخص  = ثبت_شاخص(l, 0, n*l.w*l.h + i, 0);
            صحيح ماسک_شاخص = ثبت_شاخص(l, 0, n*l.w*l.h + i, 4);
            عائم scale = l.background ? 1 : predictions[obj_شاخص];
            dets[شاخص].bbox = get_region_box(predictions, l.biases, n, box_شاخص, ستون, لیکه, l.w, l.h, l.w*l.h);
            dets[شاخص].objectness = scale > thresh ? scale : 0;
            إذا(dets[شاخص].ماسک){
                لكل(j = 0; j < l.coords - 4; ++j){
                    dets[شاخص].ماسک[j] = l.output[ماسک_شاخص + j*l.w*l.h];
                }
            }

            صحيح class_شاخص = ثبت_شاخص(l, 0, n*l.w*l.h + i, l.coords + !l.background);
            إذا(l.softmax_ونه){

                hierarchy_predictions(predictions + class_شاخص, l.classes, l.softmax_ونه, 0, l.w*l.h);
                إذا(map){
                    لكل(j = 0; j < 200; ++j){
                        صحيح class_شاخص = ثبت_شاخص(l, 0, n*l.w*l.h + i, l.coords + 1 + map[j]);
                        عائم prob = scale*predictions[class_شاخص];
                        dets[شاخص].prob[j] = (prob > thresh) ? prob : 0;
                    }
                } وإلا {
                    صحيح j =  hierarchy_سر_prediction(predictions + class_شاخص, l.softmax_ونه, ونه_thresh, l.w*l.h);
                    dets[شاخص].prob[j] = (scale > thresh) ? scale : 0;
                }
            } وإلا {
                إذا(dets[شاخص].objectness){
                    لكل(j = 0; j < l.classes; ++j){
                        صحيح class_شاخص = ثبت_شاخص(l, 0, n*l.w*l.h + i, l.coords + 1 + j);
                        عائم prob = scale*predictions[class_شاخص];
                        dets[شاخص].prob[j] = (prob > thresh) ? prob : 0;
                    }
                }
            }
        }
    }
    correct_region_boxes(dets, l.w*l.h*l.n, w, h, netw, neth, relative);
}

#که_تعریف_شوی GPU

فراغ forward_region_layer_gpu(ثابت layer l, شبکه net)
{
    copy_gpu(l.batch*l.inputs, net.ننوت_gpu, 1, l.output_gpu, 1);
    صحيح b, n;
    لكل (b = 0; b < l.batch; ++b){
        لكل(n = 0; n < l.n; ++n){
            صحيح شاخص = ثبت_شاخص(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + شاخص, 2*l.w*l.h, LOGISTIC);
            إذا(l.coords > 4){
                شاخص = ثبت_شاخص(l, b, n*l.w*l.h, 4);
                activate_array_gpu(l.output_gpu + شاخص, (l.coords - 4)*l.w*l.h, LOGISTIC);
            }
            شاخص = ثبت_شاخص(l, b, n*l.w*l.h, l.coords);
            إذا(!l.background) activate_array_gpu(l.output_gpu + شاخص,   l.w*l.h, LOGISTIC);
            شاخص = ثبت_شاخص(l, b, n*l.w*l.h, l.coords + 1);
            إذا(!l.softmax && !l.softmax_ونه) activate_array_gpu(l.output_gpu + شاخص, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    إذا (l.softmax_ونه){
        صحيح شاخص = ثبت_شاخص(l, 0, 0, l.coords + 1);
        softmax_ونه(net.ننوت_gpu + شاخص, l.w*l.h, l.batch*l.n, l.inputs/l.n, 1, l.output_gpu + شاخص, *l.softmax_ونه);
    } وإلا إذا (l.softmax) {
        صحيح شاخص = ثبت_شاخص(l, 0, 0, l.coords + !l.background);
        softmax_gpu(net.ننوت_gpu + شاخص, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu + شاخص);
    }
    إذا(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        أرجع;
    }

    cuda_pull_array(l.output_gpu, net.ننوت, l.batch*l.inputs);
    forward_region_layer(l, net);
    //cuda_push_array(l.output_gpu, l.output, l.batch*l.outputs);
    إذا(!net.train) أرجع;
    cuda_ټېل_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

فراغ backward_region_layer_gpu(ثابت layer l, شبکه net)
{
    صحيح b, n;
    لكل (b = 0; b < l.batch; ++b){
        لكل(n = 0; n < l.n; ++n){
            صحيح شاخص = ثبت_شاخص(l, b, n*l.w*l.h, 0);
            gradient_array_gpu(l.output_gpu + شاخص, 2*l.w*l.h, LOGISTIC, l.delta_gpu + شاخص);
            إذا(l.coords > 4){
                شاخص = ثبت_شاخص(l, b, n*l.w*l.h, 4);
                gradient_array_gpu(l.output_gpu + شاخص, (l.coords - 4)*l.w*l.h, LOGISTIC, l.delta_gpu + شاخص);
            }
            شاخص = ثبت_شاخص(l, b, n*l.w*l.h, l.coords);
            إذا(!l.background) gradient_array_gpu(l.output_gpu + شاخص,   l.w*l.h, LOGISTIC, l.delta_gpu + شاخص);
        }
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#د_که_پای

فراغ zero_objectness(layer l)
{
    صحيح i, n;
    لكل (i = 0; i < l.w*l.h; ++i){
        لكل(n = 0; n < l.n; ++n){
            صحيح obj_شاخص = ثبت_شاخص(l, 0, n*l.w*l.h + i, l.coords);
            l.output[obj_شاخص] = 0;
        }
    }
}


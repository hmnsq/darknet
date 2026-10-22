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
    l.باہرلا_w = l.w;
    l.باہرلا_h = l.h;
    l.باہرلا_c = l.c;
    l.classes = classes;
    l.coords = coords;
    l.لاگت_2 = صفر_حافظہ_دیو(1, حجم(عائم));
    l.biases = صفر_حافظہ_دیو(n*2, حجم(عائم));
    l.bias_updates = صفر_حافظہ_دیو(n*2, حجم(عائم));
    l.outputs = h*w*n*(classes + coords + 1);
    l.inputs = l.outputs;
    l.truths = 30*(l.coords + 1);
    l.delta = صفر_حافظہ_دیو(batch*l.outputs, حجم(عائم));
    l.output = صفر_حافظہ_دیو(batch*l.outputs, حجم(عائم));
    صحيح i;
    لكل(i = 0; i < n*2; ++i){
        l.biases[i] = .5;
    }

    l.forward = forward_region_layer;
    l.backward = backward_region_layer;
#جے_تعریف_شدہ GPU
    l.forward_gpu = forward_region_layer_gpu;
    l.backward_gpu = backward_region_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#جے_انت

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

#جے_تعریف_شدہ GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#جے_انت
}

box get_region_box(عائم *x, عائم *biases, صحيح n, صحيح اشاریہ, صحيح i, صحيح j, صحيح w, صحيح h, صحيح stride)
{
    box b;
    b.x = (i + x[اشاریہ + 0*stride]) / w;
    b.y = (j + x[اشاریہ + 1*stride]) / h;
    b.w = قوت_نما(x[اشاریہ + 2*stride]) * biases[2*n]   / w;
    b.h = قوت_نما(x[اشاریہ + 3*stride]) * biases[2*n+1] / h;
    أرجع b;
}

عائم delta_region_box(box truth, عائم *x, عائم *biases, صحيح n, صحيح اشاریہ, صحيح i, صحيح j, صحيح w, صحيح h, عائم *delta, عائم scale, صحيح stride)
{
    box pred = get_region_box(x, biases, n, اشاریہ, i, j, w, h, stride);
    عائم iou = box_iou(pred, truth);

    عائم tx = (truth.x*w - i);
    عائم ty = (truth.y*h - j);
    عائم tw = لوگارتھم(truth.w*w / biases[2*n]);
    عائم th = لوگارتھم(truth.h*h / biases[2*n + 1]);

    delta[اشاریہ + 0*stride] = scale * (tx - x[اشاریہ + 0*stride]);
    delta[اشاریہ + 1*stride] = scale * (ty - x[اشاریہ + 1*stride]);
    delta[اشاریہ + 2*stride] = scale * (tw - x[اشاریہ + 2*stride]);
    delta[اشاریہ + 3*stride] = scale * (th - x[اشاریہ + 3*stride]);
    أرجع iou;
}

فراغ delta_region_نقاب(عائم *truth, عائم *x, صحيح n, صحيح اشاریہ, عائم *delta, صحيح stride, صحيح scale)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        delta[اشاریہ + i*stride] = scale*(truth[i] - x[اشاریہ + i*stride]);
    }
}


فراغ delta_region_class(عائم *output, عائم *delta, صحيح اشاریہ, صحيح class, صحيح classes, درخت *hier, عائم scale, صحيح stride, عائم *avg_cat, صحيح ٹیگ)
{
    صحيح i, n;
    إذا(hier){
        عائم pred = 1;
        طالما(class >= 0){
            pred *= output[اشاریہ + stride*class];
            صحيح g = hier->گروہ[class];
            صحيح وتھ = hier->گروہ_وتھ[g];
            لكل(i = 0; i < hier->گروہ_ناپ[g]; ++i){
                delta[اشاریہ + stride*(وتھ + i)] = scale * (0 - output[اشاریہ + stride*(وتھ + i)]);
            }
            delta[اشاریہ + stride*class] = scale * (1 - output[اشاریہ + stride*class]);

            class = hier->ماپا[class];
        }
        *avg_cat += pred;
    } وإلا {
        إذا (delta[اشاریہ] && ٹیگ){
            delta[اشاریہ + stride*class] = scale * (1 - output[اشاریہ + stride*class]);
            أرجع;
        }
        لكل(n = 0; n < classes; ++n){
            delta[اشاریہ + stride*n] = scale * (((n == class)?1 : 0) - output[اشاریہ + stride*n]);
            إذا(n == class) *avg_cat += output[اشاریہ + stride*n];
        }
    }
}

عائم logit(عائم x)
{
    أرجع لوگارتھم(x/(1.-x));
}

عائم tisnan(عائم x)
{
    أرجع (x != x);
}

صحيح اندراج_اشاریہ(layer l, صحيح batch, صحيح location, صحيح اندراج)
{
    صحيح n =   location / (l.w*l.h);
    صحيح loc = location % (l.w*l.h);
    أرجع batch*l.outputs + n*l.w*l.h*(l.coords+l.classes+1) + اندراج*l.w*l.h + loc;
}

فراغ forward_region_layer(ثابت layer l, جال net)
{
    صحيح i,j,b,t,n;
    حافظہ_نقل(l.output, net.داخلہ, l.outputs*l.batch*حجم(عائم));

#جے_بنا_تعریف GPU
    لكل (b = 0; b < l.batch; ++b){
        لكل(n = 0; n < l.n; ++n){
            صحيح اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, 0);
            activate_array(l.output + اشاریہ, 2*l.w*l.h, LOGISTIC);
            اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, l.coords);
            إذا(!l.background) activate_array(l.output + اشاریہ,   l.w*l.h, LOGISTIC);
            اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, l.coords + 1);
            إذا(!l.softmax && !l.softmax_درخت) activate_array(l.output + اشاریہ, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    إذا (l.softmax_درخت){
        صحيح i;
        صحيح گنتی = l.coords + 1;
        لكل (i = 0; i < l.softmax_درخت->groups; ++i) {
            صحيح گروہ_ناپ = l.softmax_درخت->گروہ_ناپ[i];
            softmax_cpu(net.داخلہ + گنتی, گروہ_ناپ, l.batch, l.inputs, l.n*l.w*l.h, 1, l.n*l.w*l.h, l.temperature, l.output + گنتی);
            گنتی += گروہ_ناپ;
        }
    } وإلا إذا (l.softmax){
        صحيح اشاریہ = اندراج_اشاریہ(l, 0, 0, l.coords + !l.background);
        softmax_cpu(net.داخلہ + اشاریہ, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output + اشاریہ);
    }
#جے_انت

    memset(l.delta, 0, l.outputs * l.batch * حجم(عائم));
    إذا(!net.train) أرجع;
    عائم avg_iou = 0;
    عائم recall = 0;
    عائم avg_cat = 0;
    عائم avg_obj = 0;
    عائم avg_anyobj = 0;
    صحيح گنتی = 0;
    صحيح class_گنتی = 0;
    *(l.لاگت_2) = 0;
    لكل (b = 0; b < l.batch; ++b) {
        إذا(l.softmax_درخت){
            صحيح onlyclass = 0;
            لكل(t = 0; t < 30; ++t){
                box truth = float_منزل_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                إذا(!truth.x) اكسر;
                صحيح class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
                عائم maxp = 0;
                صحيح maxi = 0;
                إذا(truth.x > 100000 && truth.y > 100000){
                    لكل(n = 0; n < l.n*l.w*l.h; ++n){
                        صحيح class_اشاریہ = اندراج_اشاریہ(l, b, n, l.coords + 1);
                        صحيح obj_اشاریہ = اندراج_اشاریہ(l, b, n, l.coords);
                        عائم scale =  l.output[obj_اشاریہ];
                        l.delta[obj_اشاریہ] = l.noobject_scale * (0 - l.output[obj_اشاریہ]);
                        عائم p = scale*get_hierarchy_probability(l.output + class_اشاریہ, l.softmax_درخت, class, l.w*l.h);
                        إذا(p > maxp){
                            maxp = p;
                            maxi = n;
                        }
                    }
                    صحيح class_اشاریہ = اندراج_اشاریہ(l, b, maxi, l.coords + 1);
                    صحيح obj_اشاریہ = اندراج_اشاریہ(l, b, maxi, l.coords);
                    delta_region_class(l.output, l.delta, class_اشاریہ, class, l.classes, l.softmax_درخت, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
                    إذا(l.output[obj_اشاریہ] < .3) l.delta[obj_اشاریہ] = l.object_scale * (.3 - l.output[obj_اشاریہ]);
                    وإلا  l.delta[obj_اشاریہ] = 0;
                    l.delta[obj_اشاریہ] = 0;
                    ++class_گنتی;
                    onlyclass = 1;
                    اكسر;
                }
            }
            إذا(onlyclass) استمر;
        }
        لكل (j = 0; j < l.h; ++j) {
            لكل (i = 0; i < l.w; ++i) {
                لكل (n = 0; n < l.n; ++n) {
                    صحيح box_اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_region_box(l.output, l.biases, n, box_اشاریہ, i, j, l.w, l.h, l.w*l.h);
                    عائم بہترین_iou = 0;
                    لكل(t = 0; t < 30; ++t){
                        box truth = float_منزل_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                        إذا(!truth.x) اكسر;
                        عائم iou = box_iou(pred, truth);
                        إذا (iou > بہترین_iou) {
                            بہترین_iou = iou;
                        }
                    }
                    صحيح obj_اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h + j*l.w + i, l.coords);
                    avg_anyobj += l.output[obj_اشاریہ];
                    l.delta[obj_اشاریہ] = l.noobject_scale * (0 - l.output[obj_اشاریہ]);
                    إذا(l.background) l.delta[obj_اشاریہ] = l.noobject_scale * (1 - l.output[obj_اشاریہ]);
                    إذا (بہترین_iou > l.thresh) {
                        l.delta[obj_اشاریہ] = 0;
                    }

                    إذا(*(net.تکیا) < 12800){
                        box truth = {0};
                        truth.x = (i + .5)/l.w;
                        truth.y = (j + .5)/l.h;
                        truth.w = l.biases[2*n]/l.w;
                        truth.h = l.biases[2*n+1]/l.h;
                        delta_region_box(truth, l.output, l.biases, n, box_اشاریہ, i, j, l.w, l.h, l.delta, .01, l.w*l.h);
                    }
                }
            }
        }
        لكل(t = 0; t < 30; ++t){
            box truth = float_منزل_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);

            إذا(!truth.x) اكسر;
            عائم بہترین_iou = 0;
            صحيح بہترین_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_کھسکاؤ = truth;
            truth_کھسکاؤ.x = 0;
            truth_کھسکاؤ.y = 0;
            لكل(n = 0; n < l.n; ++n){
                صحيح box_اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h + j*l.w + i, 0);
                box pred = get_region_box(l.output, l.biases, n, box_اشاریہ, i, j, l.w, l.h, l.w*l.h);
                إذا(l.bias_میل){
                    pred.w = l.biases[2*n]/l.w;
                    pred.h = l.biases[2*n+1]/l.h;
                }
                pred.x = 0;
                pred.y = 0;
                عائم iou = box_iou(pred, truth_کھسکاؤ);
                إذا (iou > بہترین_iou){
                    بہترین_iou = iou;
                    بہترین_n = n;
                }
            }

            صحيح box_اشاریہ = اندراج_اشاریہ(l, b, بہترین_n*l.w*l.h + j*l.w + i, 0);
            عائم iou = delta_region_box(truth, l.output, l.biases, بہترین_n, box_اشاریہ, i, j, l.w, l.h, l.delta, l.coord_scale *  (2 - truth.w*truth.h), l.w*l.h);
            إذا(l.coords > 4){
                صحيح نقاب_اشاریہ = اندراج_اشاریہ(l, b, بہترین_n*l.w*l.h + j*l.w + i, 4);
                delta_region_نقاب(net.truth + t*(l.coords + 1) + b*l.truths + 5, l.output, l.coords - 4, نقاب_اشاریہ, l.delta, l.w*l.h, l.نقاب_scale);
            }
            إذا(iou > .5) recall += 1;
            avg_iou += iou;

            صحيح obj_اشاریہ = اندراج_اشاریہ(l, b, بہترین_n*l.w*l.h + j*l.w + i, l.coords);
            avg_obj += l.output[obj_اشاریہ];
            l.delta[obj_اشاریہ] = l.object_scale * (1 - l.output[obj_اشاریہ]);
            إذا (l.rescore) {
                l.delta[obj_اشاریہ] = l.object_scale * (iou - l.output[obj_اشاریہ]);
            }
            إذا(l.background){
                l.delta[obj_اشاریہ] = l.object_scale * (0 - l.output[obj_اشاریہ]);
            }

            صحيح class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
            إذا (l.map) class = l.map[class];
            صحيح class_اشاریہ = اندراج_اشاریہ(l, b, بہترین_n*l.w*l.h + j*l.w + i, l.coords + 1);
            delta_region_class(l.output, l.delta, class_اشاریہ, class, l.classes, l.softmax_درخت, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
            ++گنتی;
            ++class_گنتی;
        }
    }
    *(l.لاگت_2) = قوت(mag_array(l.delta, l.outputs * l.batch), 2);
    لکھو("Region Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, Avg Recall: %f,  count: %d\n", avg_iou/گنتی, avg_cat/class_گنتی, avg_obj/گنتی, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/گنتی, گنتی);
}

فراغ backward_region_layer(ثابت layer l, جال net)
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

فراغ get_region_detections(layer l, صحيح w, صحيح h, صحيح netw, صحيح neth, عائم thresh, صحيح *map, عائم درخت_thresh, صحيح relative, detection *dets)
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
                        عائم وٹاندرا = flip[i1];
                        flip[i1] = flip[i2];
                        flip[i2] = وٹاندرا;
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
        صحيح صف = i / l.w;
        صحيح خانہ = i % l.w;
        لكل(n = 0; n < l.n; ++n){
            صحيح اشاریہ = n*l.w*l.h + i;
            لكل(j = 0; j < l.classes; ++j){
                dets[اشاریہ].prob[j] = 0;
            }
            صحيح obj_اشاریہ  = اندراج_اشاریہ(l, 0, n*l.w*l.h + i, l.coords);
            صحيح box_اشاریہ  = اندراج_اشاریہ(l, 0, n*l.w*l.h + i, 0);
            صحيح نقاب_اشاریہ = اندراج_اشاریہ(l, 0, n*l.w*l.h + i, 4);
            عائم scale = l.background ? 1 : predictions[obj_اشاریہ];
            dets[اشاریہ].bbox = get_region_box(predictions, l.biases, n, box_اشاریہ, خانہ, صف, l.w, l.h, l.w*l.h);
            dets[اشاریہ].objectness = scale > thresh ? scale : 0;
            إذا(dets[اشاریہ].نقاب){
                لكل(j = 0; j < l.coords - 4; ++j){
                    dets[اشاریہ].نقاب[j] = l.output[نقاب_اشاریہ + j*l.w*l.h];
                }
            }

            صحيح class_اشاریہ = اندراج_اشاریہ(l, 0, n*l.w*l.h + i, l.coords + !l.background);
            إذا(l.softmax_درخت){

                hierarchy_predictions(predictions + class_اشاریہ, l.classes, l.softmax_درخت, 0, l.w*l.h);
                إذا(map){
                    لكل(j = 0; j < 200; ++j){
                        صحيح class_اشاریہ = اندراج_اشاریہ(l, 0, n*l.w*l.h + i, l.coords + 1 + map[j]);
                        عائم prob = scale*predictions[class_اشاریہ];
                        dets[اشاریہ].prob[j] = (prob > thresh) ? prob : 0;
                    }
                } وإلا {
                    صحيح j =  hierarchy_چوٹی_prediction(predictions + class_اشاریہ, l.softmax_درخت, درخت_thresh, l.w*l.h);
                    dets[اشاریہ].prob[j] = (scale > thresh) ? scale : 0;
                }
            } وإلا {
                إذا(dets[اشاریہ].objectness){
                    لكل(j = 0; j < l.classes; ++j){
                        صحيح class_اشاریہ = اندراج_اشاریہ(l, 0, n*l.w*l.h + i, l.coords + 1 + j);
                        عائم prob = scale*predictions[class_اشاریہ];
                        dets[اشاریہ].prob[j] = (prob > thresh) ? prob : 0;
                    }
                }
            }
        }
    }
    correct_region_boxes(dets, l.w*l.h*l.n, w, h, netw, neth, relative);
}

#جے_تعریف_شدہ GPU

فراغ forward_region_layer_gpu(ثابت layer l, جال net)
{
    copy_gpu(l.batch*l.inputs, net.داخلہ_gpu, 1, l.output_gpu, 1);
    صحيح b, n;
    لكل (b = 0; b < l.batch; ++b){
        لكل(n = 0; n < l.n; ++n){
            صحيح اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + اشاریہ, 2*l.w*l.h, LOGISTIC);
            إذا(l.coords > 4){
                اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, 4);
                activate_array_gpu(l.output_gpu + اشاریہ, (l.coords - 4)*l.w*l.h, LOGISTIC);
            }
            اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, l.coords);
            إذا(!l.background) activate_array_gpu(l.output_gpu + اشاریہ,   l.w*l.h, LOGISTIC);
            اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, l.coords + 1);
            إذا(!l.softmax && !l.softmax_درخت) activate_array_gpu(l.output_gpu + اشاریہ, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    إذا (l.softmax_درخت){
        صحيح اشاریہ = اندراج_اشاریہ(l, 0, 0, l.coords + 1);
        softmax_درخت(net.داخلہ_gpu + اشاریہ, l.w*l.h, l.batch*l.n, l.inputs/l.n, 1, l.output_gpu + اشاریہ, *l.softmax_درخت);
    } وإلا إذا (l.softmax) {
        صحيح اشاریہ = اندراج_اشاریہ(l, 0, 0, l.coords + !l.background);
        softmax_gpu(net.داخلہ_gpu + اشاریہ, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu + اشاریہ);
    }
    إذا(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        أرجع;
    }

    cuda_pull_array(l.output_gpu, net.داخلہ, l.batch*l.inputs);
    forward_region_layer(l, net);
    //cuda_push_array(l.output_gpu, l.output, l.batch*l.outputs);
    إذا(!net.train) أرجع;
    cuda_ڈالو_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

فراغ backward_region_layer_gpu(ثابت layer l, جال net)
{
    صحيح b, n;
    لكل (b = 0; b < l.batch; ++b){
        لكل(n = 0; n < l.n; ++n){
            صحيح اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, 0);
            gradient_array_gpu(l.output_gpu + اشاریہ, 2*l.w*l.h, LOGISTIC, l.delta_gpu + اشاریہ);
            إذا(l.coords > 4){
                اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, 4);
                gradient_array_gpu(l.output_gpu + اشاریہ, (l.coords - 4)*l.w*l.h, LOGISTIC, l.delta_gpu + اشاریہ);
            }
            اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, l.coords);
            إذا(!l.background) gradient_array_gpu(l.output_gpu + اشاریہ,   l.w*l.h, LOGISTIC, l.delta_gpu + اشاریہ);
        }
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#جے_انت

فراغ zero_objectness(layer l)
{
    صحيح i, n;
    لكل (i = 0; i < l.w*l.h; ++i){
        لكل(n = 0; n < l.n; ++n){
            صحيح obj_اشاریہ = اندراج_اشاریہ(l, 0, n*l.w*l.h + i, l.coords);
            l.output[obj_اشاریہ] = 0;
        }
    }
}


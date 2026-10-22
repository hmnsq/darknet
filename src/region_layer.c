#شامل "region_layer.h"
#شامل "activations.h"
#شامل "blas.h"
#شامل "box.h"
#شامل "cuda.h"
#شامل "utils.h"

#شامل <stdio.h>
#شامل <assert.h>
#شامل <string.h>
#شامل <stdlib.h>

layer make_region_layer(صحیح batch, صحیح w, صحیح h, صحیح n, صحیح classes, صحیح coords)
{
    layer l = {0};
    l.type = REGION;

    l.n = n;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = n*(classes + coords + 1);
    l.برآمد_w = l.w;
    l.برآمد_h = l.h;
    l.برآمد_c = l.c;
    l.classes = classes;
    l.coords = coords;
    l.لاگت_2 = صفر_حافظہ_دو(1, حجم(اعشاری));
    l.biases = صفر_حافظہ_دو(n*2, حجم(اعشاری));
    l.bias_updates = صفر_حافظہ_دو(n*2, حجم(اعشاری));
    l.outputs = h*w*n*(classes + coords + 1);
    l.inputs = l.outputs;
    l.truths = 30*(l.coords + 1);
    l.delta = صفر_حافظہ_دو(batch*l.outputs, حجم(اعشاری));
    l.output = صفر_حافظہ_دو(batch*l.outputs, حجم(اعشاری));
    صحیح i;
    برائے(i = 0; i < n*2; ++i){
        l.biases[i] = .5;
    }

    l.forward = forward_region_layer;
    l.backward = backward_region_layer;
#اگر_تعریف_شدہ GPU
    l.forward_gpu = forward_region_layer_gpu;
    l.backward_gpu = backward_region_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#اگر_ختم

    fprintf(stderr, "detection\n");
    srand(0);

    واپس l;
}

خالی resize_region_layer(layer *l, صحیح w, صحیح h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + l->coords + 1);
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*حجم(اعشاری));
    l->delta = realloc(l->delta, l->batch*l->outputs*حجم(اعشاری));

#اگر_تعریف_شدہ GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#اگر_ختم
}

box get_region_box(اعشاری *x, اعشاری *biases, صحیح n, صحیح اشاریہ, صحیح i, صحیح j, صحیح w, صحیح h, صحیح stride)
{
    box b;
    b.x = (i + x[اشاریہ + 0*stride]) / w;
    b.y = (j + x[اشاریہ + 1*stride]) / h;
    b.w = قوت_نما(x[اشاریہ + 2*stride]) * biases[2*n]   / w;
    b.h = قوت_نما(x[اشاریہ + 3*stride]) * biases[2*n+1] / h;
    واپس b;
}

اعشاری delta_region_box(box truth, اعشاری *x, اعشاری *biases, صحیح n, صحیح اشاریہ, صحیح i, صحیح j, صحیح w, صحیح h, اعشاری *delta, اعشاری scale, صحیح stride)
{
    box pred = get_region_box(x, biases, n, اشاریہ, i, j, w, h, stride);
    اعشاری iou = box_iou(pred, truth);

    اعشاری tx = (truth.x*w - i);
    اعشاری ty = (truth.y*h - j);
    اعشاری tw = لوگارتھم(truth.w*w / biases[2*n]);
    اعشاری th = لوگارتھم(truth.h*h / biases[2*n + 1]);

    delta[اشاریہ + 0*stride] = scale * (tx - x[اشاریہ + 0*stride]);
    delta[اشاریہ + 1*stride] = scale * (ty - x[اشاریہ + 1*stride]);
    delta[اشاریہ + 2*stride] = scale * (tw - x[اشاریہ + 2*stride]);
    delta[اشاریہ + 3*stride] = scale * (th - x[اشاریہ + 3*stride]);
    واپس iou;
}

خالی delta_region_نقاب(اعشاری *truth, اعشاری *x, صحیح n, صحیح اشاریہ, اعشاری *delta, صحیح stride, صحیح scale)
{
    صحیح i;
    برائے(i = 0; i < n; ++i){
        delta[اشاریہ + i*stride] = scale*(truth[i] - x[اشاریہ + i*stride]);
    }
}


خالی delta_region_class(اعشاری *output, اعشاری *delta, صحیح اشاریہ, صحیح class, صحیح classes, درخت *hier, اعشاری scale, صحیح stride, اعشاری *avg_cat, صحیح لیبل)
{
    صحیح i, n;
    اگر(hier){
        اعشاری pred = 1;
        جب(class >= 0){
            pred *= output[اشاریہ + stride*class];
            صحیح g = hier->گروہ[class];
            صحیح انحراف = hier->گروہ_انحراف[g];
            برائے(i = 0; i < hier->گروہ_سائز[g]; ++i){
                delta[اشاریہ + stride*(انحراف + i)] = scale * (0 - output[اشاریہ + stride*(انحراف + i)]);
            }
            delta[اشاریہ + stride*class] = scale * (1 - output[اشاریہ + stride*class]);

            class = hier->والد[class];
        }
        *avg_cat += pred;
    } ورنہ {
        اگر (delta[اشاریہ] && لیبل){
            delta[اشاریہ + stride*class] = scale * (1 - output[اشاریہ + stride*class]);
            واپس;
        }
        برائے(n = 0; n < classes; ++n){
            delta[اشاریہ + stride*n] = scale * (((n == class)?1 : 0) - output[اشاریہ + stride*n]);
            اگر(n == class) *avg_cat += output[اشاریہ + stride*n];
        }
    }
}

اعشاری logit(اعشاری x)
{
    واپس لوگارتھم(x/(1.-x));
}

اعشاری tisnan(اعشاری x)
{
    واپس (x != x);
}

صحیح اندراج_اشاریہ(layer l, صحیح batch, صحیح location, صحیح اندراج_2)
{
    صحیح n =   location / (l.w*l.h);
    صحیح loc = location % (l.w*l.h);
    واپس batch*l.outputs + n*l.w*l.h*(l.coords+l.classes+1) + اندراج_2*l.w*l.h + loc;
}

خالی forward_region_layer(ثابت layer l, جال net)
{
    صحیح i,j,b,t,n;
    حافظہ_نقل(l.output, net.داخلہ, l.outputs*l.batch*حجم(اعشاری));

#اگر_غیر_تعریف_شدہ GPU
    برائے (b = 0; b < l.batch; ++b){
        برائے(n = 0; n < l.n; ++n){
            صحیح اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, 0);
            activate_array(l.output + اشاریہ, 2*l.w*l.h, LOGISTIC);
            اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, l.coords);
            اگر(!l.background) activate_array(l.output + اشاریہ,   l.w*l.h, LOGISTIC);
            اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, l.coords + 1);
            اگر(!l.softmax && !l.softmax_درخت) activate_array(l.output + اشاریہ, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    اگر (l.softmax_درخت){
        صحیح i;
        صحیح گنتی = l.coords + 1;
        برائے (i = 0; i < l.softmax_درخت->groups; ++i) {
            صحیح گروہ_سائز = l.softmax_درخت->گروہ_سائز[i];
            softmax_cpu(net.داخلہ + گنتی, گروہ_سائز, l.batch, l.inputs, l.n*l.w*l.h, 1, l.n*l.w*l.h, l.temperature, l.output + گنتی);
            گنتی += گروہ_سائز;
        }
    } ورنہ اگر (l.softmax){
        صحیح اشاریہ = اندراج_اشاریہ(l, 0, 0, l.coords + !l.background);
        softmax_cpu(net.داخلہ + اشاریہ, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output + اشاریہ);
    }
#اگر_ختم

    memset(l.delta, 0, l.outputs * l.batch * حجم(اعشاری));
    اگر(!net.train) واپس;
    اعشاری avg_iou = 0;
    اعشاری recall = 0;
    اعشاری avg_cat = 0;
    اعشاری avg_obj = 0;
    اعشاری avg_anyobj = 0;
    صحیح گنتی = 0;
    صحیح class_گنتی = 0;
    *(l.لاگت_2) = 0;
    برائے (b = 0; b < l.batch; ++b) {
        اگر(l.softmax_درخت){
            صحیح onlyclass = 0;
            برائے(t = 0; t < 30; ++t){
                box truth = float_مقصد_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                اگر(!truth.x) توڑو;
                صحیح class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
                اعشاری maxp = 0;
                صحیح maxi = 0;
                اگر(truth.x > 100000 && truth.y > 100000){
                    برائے(n = 0; n < l.n*l.w*l.h; ++n){
                        صحیح class_اشاریہ = اندراج_اشاریہ(l, b, n, l.coords + 1);
                        صحیح obj_اشاریہ = اندراج_اشاریہ(l, b, n, l.coords);
                        اعشاری scale =  l.output[obj_اشاریہ];
                        l.delta[obj_اشاریہ] = l.noobject_scale * (0 - l.output[obj_اشاریہ]);
                        اعشاری p = scale*get_hierarchy_probability(l.output + class_اشاریہ, l.softmax_درخت, class, l.w*l.h);
                        اگر(p > maxp){
                            maxp = p;
                            maxi = n;
                        }
                    }
                    صحیح class_اشاریہ = اندراج_اشاریہ(l, b, maxi, l.coords + 1);
                    صحیح obj_اشاریہ = اندراج_اشاریہ(l, b, maxi, l.coords);
                    delta_region_class(l.output, l.delta, class_اشاریہ, class, l.classes, l.softmax_درخت, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
                    اگر(l.output[obj_اشاریہ] < .3) l.delta[obj_اشاریہ] = l.object_scale * (.3 - l.output[obj_اشاریہ]);
                    ورنہ  l.delta[obj_اشاریہ] = 0;
                    l.delta[obj_اشاریہ] = 0;
                    ++class_گنتی;
                    onlyclass = 1;
                    توڑو;
                }
            }
            اگر(onlyclass) جاری;
        }
        برائے (j = 0; j < l.h; ++j) {
            برائے (i = 0; i < l.w; ++i) {
                برائے (n = 0; n < l.n; ++n) {
                    صحیح box_اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_region_box(l.output, l.biases, n, box_اشاریہ, i, j, l.w, l.h, l.w*l.h);
                    اعشاری بہترین_iou = 0;
                    برائے(t = 0; t < 30; ++t){
                        box truth = float_مقصد_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                        اگر(!truth.x) توڑو;
                        اعشاری iou = box_iou(pred, truth);
                        اگر (iou > بہترین_iou) {
                            بہترین_iou = iou;
                        }
                    }
                    صحیح obj_اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h + j*l.w + i, l.coords);
                    avg_anyobj += l.output[obj_اشاریہ];
                    l.delta[obj_اشاریہ] = l.noobject_scale * (0 - l.output[obj_اشاریہ]);
                    اگر(l.background) l.delta[obj_اشاریہ] = l.noobject_scale * (1 - l.output[obj_اشاریہ]);
                    اگر (بہترین_iou > l.thresh) {
                        l.delta[obj_اشاریہ] = 0;
                    }

                    اگر(*(net.نظر_آیا) < 12800){
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
        برائے(t = 0; t < 30; ++t){
            box truth = float_مقصد_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);

            اگر(!truth.x) توڑو;
            اعشاری بہترین_iou = 0;
            صحیح بہترین_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_سرکاؤ = truth;
            truth_سرکاؤ.x = 0;
            truth_سرکاؤ.y = 0;
            برائے(n = 0; n < l.n; ++n){
                صحیح box_اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h + j*l.w + i, 0);
                box pred = get_region_box(l.output, l.biases, n, box_اشاریہ, i, j, l.w, l.h, l.w*l.h);
                اگر(l.bias_مطابقت){
                    pred.w = l.biases[2*n]/l.w;
                    pred.h = l.biases[2*n+1]/l.h;
                }
                pred.x = 0;
                pred.y = 0;
                اعشاری iou = box_iou(pred, truth_سرکاؤ);
                اگر (iou > بہترین_iou){
                    بہترین_iou = iou;
                    بہترین_n = n;
                }
            }

            صحیح box_اشاریہ = اندراج_اشاریہ(l, b, بہترین_n*l.w*l.h + j*l.w + i, 0);
            اعشاری iou = delta_region_box(truth, l.output, l.biases, بہترین_n, box_اشاریہ, i, j, l.w, l.h, l.delta, l.coord_scale *  (2 - truth.w*truth.h), l.w*l.h);
            اگر(l.coords > 4){
                صحیح نقاب_اشاریہ = اندراج_اشاریہ(l, b, بہترین_n*l.w*l.h + j*l.w + i, 4);
                delta_region_نقاب(net.truth + t*(l.coords + 1) + b*l.truths + 5, l.output, l.coords - 4, نقاب_اشاریہ, l.delta, l.w*l.h, l.نقاب_scale);
            }
            اگر(iou > .5) recall += 1;
            avg_iou += iou;

            صحیح obj_اشاریہ = اندراج_اشاریہ(l, b, بہترین_n*l.w*l.h + j*l.w + i, l.coords);
            avg_obj += l.output[obj_اشاریہ];
            l.delta[obj_اشاریہ] = l.object_scale * (1 - l.output[obj_اشاریہ]);
            اگر (l.rescore) {
                l.delta[obj_اشاریہ] = l.object_scale * (iou - l.output[obj_اشاریہ]);
            }
            اگر(l.background){
                l.delta[obj_اشاریہ] = l.object_scale * (0 - l.output[obj_اشاریہ]);
            }

            صحیح class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
            اگر (l.map) class = l.map[class];
            صحیح class_اشاریہ = اندراج_اشاریہ(l, b, بہترین_n*l.w*l.h + j*l.w + i, l.coords + 1);
            delta_region_class(l.output, l.delta, class_اشاریہ, class, l.classes, l.softmax_درخت, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
            ++گنتی;
            ++class_گنتی;
        }
    }
    *(l.لاگت_2) = قوت(mag_array(l.delta, l.outputs * l.batch), 2);
    لکھو("Region Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, Avg Recall: %f,  count: %d\n", avg_iou/گنتی, avg_cat/class_گنتی, avg_obj/گنتی, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/گنتی, گنتی);
}

خالی backward_region_layer(ثابت layer l, جال net)
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

خالی correct_region_boxes(detection *dets, صحیح n, صحیح w, صحیح h, صحیح netw, صحیح neth, صحیح relative)
{
    صحیح i;
    صحیح new_w=0;
    صحیح new_h=0;
    اگر (((اعشاری)netw/w) < ((اعشاری)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } ورنہ {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    برائے (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((اعشاری)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((اعشاری)new_h/neth); 
        b.w *= (اعشاری)netw/new_w;
        b.h *= (اعشاری)neth/new_h;
        اگر(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

خالی get_region_detections(layer l, صحیح w, صحیح h, صحیح netw, صحیح neth, اعشاری thresh, صحیح *map, اعشاری درخت_thresh, صحیح relative, detection *dets)
{
    صحیح i,j,n,z;
    اعشاری *predictions = l.output;
    اگر (l.batch == 2) {
        اعشاری *flip = l.output + l.outputs;
        برائے (j = 0; j < l.h; ++j) {
            برائے (i = 0; i < l.w/2; ++i) {
                برائے (n = 0; n < l.n; ++n) {
                    برائے(z = 0; z < l.classes + l.coords + 1; ++z){
                        صحیح i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                        صحیح i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                        اعشاری تبادلہ = flip[i1];
                        flip[i1] = flip[i2];
                        flip[i2] = تبادلہ;
                        اگر(z == 0){
                            flip[i1] = -flip[i1];
                            flip[i2] = -flip[i2];
                        }
                    }
                }
            }
        }
        برائے(i = 0; i < l.outputs; ++i){
            l.output[i] = (l.output[i] + flip[i])/2.;
        }
    }
    برائے (i = 0; i < l.w*l.h; ++i){
        صحیح صف = i / l.w;
        صحیح ستون = i % l.w;
        برائے(n = 0; n < l.n; ++n){
            صحیح اشاریہ = n*l.w*l.h + i;
            برائے(j = 0; j < l.classes; ++j){
                dets[اشاریہ].prob[j] = 0;
            }
            صحیح obj_اشاریہ  = اندراج_اشاریہ(l, 0, n*l.w*l.h + i, l.coords);
            صحیح box_اشاریہ  = اندراج_اشاریہ(l, 0, n*l.w*l.h + i, 0);
            صحیح نقاب_اشاریہ = اندراج_اشاریہ(l, 0, n*l.w*l.h + i, 4);
            اعشاری scale = l.background ? 1 : predictions[obj_اشاریہ];
            dets[اشاریہ].bbox = get_region_box(predictions, l.biases, n, box_اشاریہ, ستون, صف, l.w, l.h, l.w*l.h);
            dets[اشاریہ].objectness = scale > thresh ? scale : 0;
            اگر(dets[اشاریہ].نقاب){
                برائے(j = 0; j < l.coords - 4; ++j){
                    dets[اشاریہ].نقاب[j] = l.output[نقاب_اشاریہ + j*l.w*l.h];
                }
            }

            صحیح class_اشاریہ = اندراج_اشاریہ(l, 0, n*l.w*l.h + i, l.coords + !l.background);
            اگر(l.softmax_درخت){

                hierarchy_predictions(predictions + class_اشاریہ, l.classes, l.softmax_درخت, 0, l.w*l.h);
                اگر(map){
                    برائے(j = 0; j < 200; ++j){
                        صحیح class_اشاریہ = اندراج_اشاریہ(l, 0, n*l.w*l.h + i, l.coords + 1 + map[j]);
                        اعشاری prob = scale*predictions[class_اشاریہ];
                        dets[اشاریہ].prob[j] = (prob > thresh) ? prob : 0;
                    }
                } ورنہ {
                    صحیح j =  hierarchy_چوٹی_prediction(predictions + class_اشاریہ, l.softmax_درخت, درخت_thresh, l.w*l.h);
                    dets[اشاریہ].prob[j] = (scale > thresh) ? scale : 0;
                }
            } ورنہ {
                اگر(dets[اشاریہ].objectness){
                    برائے(j = 0; j < l.classes; ++j){
                        صحیح class_اشاریہ = اندراج_اشاریہ(l, 0, n*l.w*l.h + i, l.coords + 1 + j);
                        اعشاری prob = scale*predictions[class_اشاریہ];
                        dets[اشاریہ].prob[j] = (prob > thresh) ? prob : 0;
                    }
                }
            }
        }
    }
    correct_region_boxes(dets, l.w*l.h*l.n, w, h, netw, neth, relative);
}

#اگر_تعریف_شدہ GPU

خالی forward_region_layer_gpu(ثابت layer l, جال net)
{
    copy_gpu(l.batch*l.inputs, net.داخلہ_gpu, 1, l.output_gpu, 1);
    صحیح b, n;
    برائے (b = 0; b < l.batch; ++b){
        برائے(n = 0; n < l.n; ++n){
            صحیح اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + اشاریہ, 2*l.w*l.h, LOGISTIC);
            اگر(l.coords > 4){
                اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, 4);
                activate_array_gpu(l.output_gpu + اشاریہ, (l.coords - 4)*l.w*l.h, LOGISTIC);
            }
            اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, l.coords);
            اگر(!l.background) activate_array_gpu(l.output_gpu + اشاریہ,   l.w*l.h, LOGISTIC);
            اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, l.coords + 1);
            اگر(!l.softmax && !l.softmax_درخت) activate_array_gpu(l.output_gpu + اشاریہ, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    اگر (l.softmax_درخت){
        صحیح اشاریہ = اندراج_اشاریہ(l, 0, 0, l.coords + 1);
        softmax_درخت(net.داخلہ_gpu + اشاریہ, l.w*l.h, l.batch*l.n, l.inputs/l.n, 1, l.output_gpu + اشاریہ, *l.softmax_درخت);
    } ورنہ اگر (l.softmax) {
        صحیح اشاریہ = اندراج_اشاریہ(l, 0, 0, l.coords + !l.background);
        softmax_gpu(net.داخلہ_gpu + اشاریہ, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu + اشاریہ);
    }
    اگر(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        واپس;
    }

    cuda_pull_array(l.output_gpu, net.داخلہ, l.batch*l.inputs);
    forward_region_layer(l, net);
    //cuda_push_array(l.output_gpu, l.output, l.batch*l.outputs);
    اگر(!net.train) واپس;
    cuda_ڈالو_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

خالی backward_region_layer_gpu(ثابت layer l, جال net)
{
    صحیح b, n;
    برائے (b = 0; b < l.batch; ++b){
        برائے(n = 0; n < l.n; ++n){
            صحیح اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, 0);
            gradient_array_gpu(l.output_gpu + اشاریہ, 2*l.w*l.h, LOGISTIC, l.delta_gpu + اشاریہ);
            اگر(l.coords > 4){
                اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, 4);
                gradient_array_gpu(l.output_gpu + اشاریہ, (l.coords - 4)*l.w*l.h, LOGISTIC, l.delta_gpu + اشاریہ);
            }
            اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, l.coords);
            اگر(!l.background) gradient_array_gpu(l.output_gpu + اشاریہ,   l.w*l.h, LOGISTIC, l.delta_gpu + اشاریہ);
        }
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#اگر_ختم

خالی zero_objectness(layer l)
{
    صحیح i, n;
    برائے (i = 0; i < l.w*l.h; ++i){
        برائے(n = 0; n < l.n; ++n){
            صحیح obj_اشاریہ = اندراج_اشاریہ(l, 0, n*l.w*l.h + i, l.coords);
            l.output[obj_اشاریہ] = 0;
        }
    }
}


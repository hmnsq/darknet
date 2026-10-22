#شامل "yolo_layer.h"
#شامل "activations.h"
#شامل "blas.h"
#شامل "box.h"
#شامل "cuda.h"
#شامل "utils.h"

#شامل <stdio.h>
#شامل <assert.h>
#شامل <string.h>
#شامل <stdlib.h>

layer make_yolo_layer(صحیح batch, صحیح w, صحیح h, صحیح n, صحیح کل, صحیح *نقاب, صحیح classes)
{
    صحیح i;
    layer l = {0};
    l.type = YOLO;

    l.n = n;
    l.کل = کل;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = n*(classes + 4 + 1);
    l.برآمد_w = l.w;
    l.برآمد_h = l.h;
    l.برآمد_c = l.c;
    l.classes = classes;
    l.لاگت_2 = صفر_حافظہ_دو(1, حجم(اعشاری));
    l.biases = صفر_حافظہ_دو(کل*2, حجم(اعشاری));
    اگر(نقاب) l.نقاب = نقاب;
    ورنہ{
        l.نقاب = صفر_حافظہ_دو(n, حجم(صحیح));
        برائے(i = 0; i < n; ++i){
            l.نقاب[i] = i;
        }
    }
    l.bias_updates = صفر_حافظہ_دو(n*2, حجم(اعشاری));
    l.outputs = h*w*n*(classes + 4 + 1);
    l.inputs = l.outputs;
    l.truths = 90*(4 + 1);
    l.delta = صفر_حافظہ_دو(batch*l.outputs, حجم(اعشاری));
    l.output = صفر_حافظہ_دو(batch*l.outputs, حجم(اعشاری));
    برائے(i = 0; i < کل*2; ++i){
        l.biases[i] = .5;
    }

    l.forward = forward_yolo_layer;
    l.backward = backward_yolo_layer;
#اگر_تعریف_شدہ GPU
    l.forward_gpu = forward_yolo_layer_gpu;
    l.backward_gpu = backward_yolo_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#اگر_ختم

    fprintf(stderr, "yolo\n");
    srand(0);

    واپس l;
}

خالی resize_yolo_layer(layer *l, صحیح w, صحیح h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + 4 + 1);
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

box get_yolo_box(اعشاری *x, اعشاری *biases, صحیح n, صحیح اشاریہ, صحیح i, صحیح j, صحیح lw, صحیح lh, صحیح w, صحیح h, صحیح stride)
{
    box b;
    b.x = (i + x[اشاریہ + 0*stride]) / lw;
    b.y = (j + x[اشاریہ + 1*stride]) / lh;
    b.w = قوت_نما(x[اشاریہ + 2*stride]) * biases[2*n]   / w;
    b.h = قوت_نما(x[اشاریہ + 3*stride]) * biases[2*n+1] / h;
    واپس b;
}

اعشاری delta_yolo_box(box truth, اعشاری *x, اعشاری *biases, صحیح n, صحیح اشاریہ, صحیح i, صحیح j, صحیح lw, صحیح lh, صحیح w, صحیح h, اعشاری *delta, اعشاری scale, صحیح stride)
{
    box pred = get_yolo_box(x, biases, n, اشاریہ, i, j, lw, lh, w, h, stride);
    اعشاری iou = box_iou(pred, truth);

    اعشاری tx = (truth.x*lw - i);
    اعشاری ty = (truth.y*lh - j);
    اعشاری tw = لوگارتھم(truth.w*w / biases[2*n]);
    اعشاری th = لوگارتھم(truth.h*h / biases[2*n + 1]);

    delta[اشاریہ + 0*stride] = scale * (tx - x[اشاریہ + 0*stride]);
    delta[اشاریہ + 1*stride] = scale * (ty - x[اشاریہ + 1*stride]);
    delta[اشاریہ + 2*stride] = scale * (tw - x[اشاریہ + 2*stride]);
    delta[اشاریہ + 3*stride] = scale * (th - x[اشاریہ + 3*stride]);
    واپس iou;
}


خالی delta_yolo_class(اعشاری *output, اعشاری *delta, صحیح اشاریہ, صحیح class, صحیح classes, صحیح stride, اعشاری *avg_cat)
{
    صحیح n;
    اگر (delta[اشاریہ]){
        delta[اشاریہ + stride*class] = 1 - output[اشاریہ + stride*class];
        اگر(avg_cat) *avg_cat += output[اشاریہ + stride*class];
        واپس;
    }
    برائے(n = 0; n < classes; ++n){
        delta[اشاریہ + stride*n] = ((n == class)?1 : 0) - output[اشاریہ + stride*n];
        اگر(n == class && avg_cat) *avg_cat += output[اشاریہ + stride*n];
    }
}

ساکن صحیح اندراج_اشاریہ(layer l, صحیح batch, صحیح location, صحیح اندراج_2)
{
    صحیح n =   location / (l.w*l.h);
    صحیح loc = location % (l.w*l.h);
    واپس batch*l.outputs + n*l.w*l.h*(4+l.classes+1) + اندراج_2*l.w*l.h + loc;
}

خالی forward_yolo_layer(ثابت layer l, جال net)
{
    صحیح i,j,b,t,n;
    حافظہ_نقل(l.output, net.داخلہ, l.outputs*l.batch*حجم(اعشاری));

#اگر_غیر_تعریف_شدہ GPU
    برائے (b = 0; b < l.batch; ++b){
        برائے(n = 0; n < l.n; ++n){
            صحیح اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, 0);
            activate_array(l.output + اشاریہ, 2*l.w*l.h, LOGISTIC);
            اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, 4);
            activate_array(l.output + اشاریہ, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
#اگر_ختم

    memset(l.delta, 0, l.outputs * l.batch * حجم(اعشاری));
    اگر(!net.train) واپس;
    اعشاری avg_iou = 0;
    اعشاری recall = 0;
    اعشاری recall75 = 0;
    اعشاری avg_cat = 0;
    اعشاری avg_obj = 0;
    اعشاری avg_anyobj = 0;
    صحیح گنتی = 0;
    صحیح class_گنتی = 0;
    *(l.لاگت_2) = 0;
    برائے (b = 0; b < l.batch; ++b) {
        برائے (j = 0; j < l.h; ++j) {
            برائے (i = 0; i < l.w; ++i) {
                برائے (n = 0; n < l.n; ++n) {
                    صحیح box_اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_yolo_box(l.output, l.biases, l.نقاب[n], box_اشاریہ, i, j, l.w, l.h, net.w, net.h, l.w*l.h);
                    اعشاری بہترین_iou = 0;
                    صحیح بہترین_t = 0;
                    برائے(t = 0; t < l.max_boxes; ++t){
                        box truth = float_مقصد_box(net.truth + t*(4 + 1) + b*l.truths, 1);
                        اگر(!truth.x) توڑو;
                        اعشاری iou = box_iou(pred, truth);
                        اگر (iou > بہترین_iou) {
                            بہترین_iou = iou;
                            بہترین_t = t;
                        }
                    }
                    صحیح obj_اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h + j*l.w + i, 4);
                    avg_anyobj += l.output[obj_اشاریہ];
                    l.delta[obj_اشاریہ] = 0 - l.output[obj_اشاریہ];
                    اگر (بہترین_iou > l.ignore_thresh) {
                        l.delta[obj_اشاریہ] = 0;
                    }
                    اگر (بہترین_iou > l.truth_thresh) {
                        l.delta[obj_اشاریہ] = 1 - l.output[obj_اشاریہ];

                        صحیح class = net.truth[بہترین_t*(4 + 1) + b*l.truths + 4];
                        اگر (l.map) class = l.map[class];
                        صحیح class_اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h + j*l.w + i, 4 + 1);
                        delta_yolo_class(l.output, l.delta, class_اشاریہ, class, l.classes, l.w*l.h, 0);
                        box truth = float_مقصد_box(net.truth + بہترین_t*(4 + 1) + b*l.truths, 1);
                        delta_yolo_box(truth, l.output, l.biases, l.نقاب[n], box_اشاریہ, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);
                    }
                }
            }
        }
        برائے(t = 0; t < l.max_boxes; ++t){
            box truth = float_مقصد_box(net.truth + t*(4 + 1) + b*l.truths, 1);

            اگر(!truth.x) توڑو;
            اعشاری بہترین_iou = 0;
            صحیح بہترین_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_سرکاؤ = truth;
            truth_سرکاؤ.x = truth_سرکاؤ.y = 0;
            برائے(n = 0; n < l.کل; ++n){
                box pred = {0};
                pred.w = l.biases[2*n]/net.w;
                pred.h = l.biases[2*n+1]/net.h;
                اعشاری iou = box_iou(pred, truth_سرکاؤ);
                اگر (iou > بہترین_iou){
                    بہترین_iou = iou;
                    بہترین_n = n;
                }
            }

            صحیح نقاب_n = int_اشاریہ(l.نقاب, بہترین_n, l.n);
            اگر(نقاب_n >= 0){
                صحیح box_اشاریہ = اندراج_اشاریہ(l, b, نقاب_n*l.w*l.h + j*l.w + i, 0);
                اعشاری iou = delta_yolo_box(truth, l.output, l.biases, بہترین_n, box_اشاریہ, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);

                صحیح obj_اشاریہ = اندراج_اشاریہ(l, b, نقاب_n*l.w*l.h + j*l.w + i, 4);
                avg_obj += l.output[obj_اشاریہ];
                l.delta[obj_اشاریہ] = 1 - l.output[obj_اشاریہ];

                صحیح class = net.truth[t*(4 + 1) + b*l.truths + 4];
                اگر (l.map) class = l.map[class];
                صحیح class_اشاریہ = اندراج_اشاریہ(l, b, نقاب_n*l.w*l.h + j*l.w + i, 4 + 1);
                delta_yolo_class(l.output, l.delta, class_اشاریہ, class, l.classes, l.w*l.h, &avg_cat);

                ++گنتی;
                ++class_گنتی;
                اگر(iou > .5) recall += 1;
                اگر(iou > .75) recall75 += 1;
                avg_iou += iou;
            }
        }
    }
    *(l.لاگت_2) = قوت(mag_array(l.delta, l.outputs * l.batch), 2);
    لکھو("Region %d Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, .5R: %f, .75R: %f,  count: %d\n", net.اشاریہ, avg_iou/گنتی, avg_cat/class_گنتی, avg_obj/گنتی, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/گنتی, recall75/گنتی, گنتی);
}

خالی backward_yolo_layer(ثابت layer l, جال net)
{
   axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

خالی correct_yolo_boxes(detection *dets, صحیح n, صحیح w, صحیح h, صحیح netw, صحیح neth, صحیح relative)
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

صحیح yolo_عدد_detections(layer l, اعشاری thresh)
{
    صحیح i, n;
    صحیح گنتی = 0;
    برائے (i = 0; i < l.w*l.h; ++i){
        برائے(n = 0; n < l.n; ++n){
            صحیح obj_اشاریہ  = اندراج_اشاریہ(l, 0, n*l.w*l.h + i, 4);
            اگر(l.output[obj_اشاریہ] > thresh){
                ++گنتی;
            }
        }
    }
    واپس گنتی;
}

خالی avg_flipped_yolo(layer l)
{
    صحیح i,j,n,z;
    اعشاری *flip = l.output + l.outputs;
    برائے (j = 0; j < l.h; ++j) {
        برائے (i = 0; i < l.w/2; ++i) {
            برائے (n = 0; n < l.n; ++n) {
                برائے(z = 0; z < l.classes + 4 + 1; ++z){
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

صحیح get_yolo_detections(layer l, صحیح w, صحیح h, صحیح netw, صحیح neth, اعشاری thresh, صحیح *map, صحیح relative, detection *dets)
{
    صحیح i,j,n;
    اعشاری *predictions = l.output;
    اگر (l.batch == 2) avg_flipped_yolo(l);
    صحیح گنتی = 0;
    برائے (i = 0; i < l.w*l.h; ++i){
        صحیح صف = i / l.w;
        صحیح ستون = i % l.w;
        برائے(n = 0; n < l.n; ++n){
            صحیح obj_اشاریہ  = اندراج_اشاریہ(l, 0, n*l.w*l.h + i, 4);
            اعشاری objectness = predictions[obj_اشاریہ];
            اگر(objectness <= thresh) جاری;
            صحیح box_اشاریہ  = اندراج_اشاریہ(l, 0, n*l.w*l.h + i, 0);
            dets[گنتی].bbox = get_yolo_box(predictions, l.biases, l.نقاب[n], box_اشاریہ, ستون, صف, l.w, l.h, netw, neth, l.w*l.h);
            dets[گنتی].objectness = objectness;
            dets[گنتی].classes = l.classes;
            برائے(j = 0; j < l.classes; ++j){
                صحیح class_اشاریہ = اندراج_اشاریہ(l, 0, n*l.w*l.h + i, 4 + 1 + j);
                اعشاری prob = objectness*predictions[class_اشاریہ];
                dets[گنتی].prob[j] = (prob > thresh) ? prob : 0;
            }
            ++گنتی;
        }
    }
    correct_yolo_boxes(dets, گنتی, w, h, netw, neth, relative);
    واپس گنتی;
}

#اگر_تعریف_شدہ GPU

خالی forward_yolo_layer_gpu(ثابت layer l, جال net)
{
    copy_gpu(l.batch*l.inputs, net.داخلہ_gpu, 1, l.output_gpu, 1);
    صحیح b, n;
    برائے (b = 0; b < l.batch; ++b){
        برائے(n = 0; n < l.n; ++n){
            صحیح اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + اشاریہ, 2*l.w*l.h, LOGISTIC);
            اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, 4);
            activate_array_gpu(l.output_gpu + اشاریہ, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
    اگر(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        واپس;
    }

    cuda_pull_array(l.output_gpu, net.داخلہ, l.batch*l.inputs);
    forward_yolo_layer(l, net);
    cuda_ڈالو_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

خالی backward_yolo_layer_gpu(ثابت layer l, جال net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#اگر_ختم


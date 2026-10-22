#ضمن "yolo_layer.h"
#ضمن "activations.h"
#ضمن "blas.h"
#ضمن "box.h"
#ضمن "cuda.h"
#ضمن "utils.h"

#ضمن <stdio.h>
#ضمن <assert.h>
#ضمن <string.h>
#ضمن <stdlib.h>

layer make_yolo_layer(صحيح batch, صحيح w, صحيح h, صحيح n, صحيح کل, صحيح *نقاب, صحيح classes)
{
    صحيح i;
    layer l = {0};
    l.type = YOLO;

    l.n = n;
    l.کل = کل;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = n*(classes + 4 + 1);
    l.باہرلا_w = l.w;
    l.باہرلا_h = l.h;
    l.باہرلا_c = l.c;
    l.classes = classes;
    l.لاگت_2 = صفر_حافظہ_دیو(1, حجم(عائم));
    l.biases = صفر_حافظہ_دیو(کل*2, حجم(عائم));
    إذا(نقاب) l.نقاب = نقاب;
    وإلا{
        l.نقاب = صفر_حافظہ_دیو(n, حجم(صحيح));
        لكل(i = 0; i < n; ++i){
            l.نقاب[i] = i;
        }
    }
    l.bias_updates = صفر_حافظہ_دیو(n*2, حجم(عائم));
    l.outputs = h*w*n*(classes + 4 + 1);
    l.inputs = l.outputs;
    l.truths = 90*(4 + 1);
    l.delta = صفر_حافظہ_دیو(batch*l.outputs, حجم(عائم));
    l.output = صفر_حافظہ_دیو(batch*l.outputs, حجم(عائم));
    لكل(i = 0; i < کل*2; ++i){
        l.biases[i] = .5;
    }

    l.forward = forward_yolo_layer;
    l.backward = backward_yolo_layer;
#جے_تعریف_شدہ GPU
    l.forward_gpu = forward_yolo_layer_gpu;
    l.backward_gpu = backward_yolo_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#جے_انت

    fprintf(stderr, "yolo\n");
    srand(0);

    أرجع l;
}

فراغ resize_yolo_layer(layer *l, صحيح w, صحيح h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + 4 + 1);
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

box get_yolo_box(عائم *x, عائم *biases, صحيح n, صحيح اشاریہ, صحيح i, صحيح j, صحيح lw, صحيح lh, صحيح w, صحيح h, صحيح stride)
{
    box b;
    b.x = (i + x[اشاریہ + 0*stride]) / lw;
    b.y = (j + x[اشاریہ + 1*stride]) / lh;
    b.w = قوت_نما(x[اشاریہ + 2*stride]) * biases[2*n]   / w;
    b.h = قوت_نما(x[اشاریہ + 3*stride]) * biases[2*n+1] / h;
    أرجع b;
}

عائم delta_yolo_box(box truth, عائم *x, عائم *biases, صحيح n, صحيح اشاریہ, صحيح i, صحيح j, صحيح lw, صحيح lh, صحيح w, صحيح h, عائم *delta, عائم scale, صحيح stride)
{
    box pred = get_yolo_box(x, biases, n, اشاریہ, i, j, lw, lh, w, h, stride);
    عائم iou = box_iou(pred, truth);

    عائم tx = (truth.x*lw - i);
    عائم ty = (truth.y*lh - j);
    عائم tw = لوگارتھم(truth.w*w / biases[2*n]);
    عائم th = لوگارتھم(truth.h*h / biases[2*n + 1]);

    delta[اشاریہ + 0*stride] = scale * (tx - x[اشاریہ + 0*stride]);
    delta[اشاریہ + 1*stride] = scale * (ty - x[اشاریہ + 1*stride]);
    delta[اشاریہ + 2*stride] = scale * (tw - x[اشاریہ + 2*stride]);
    delta[اشاریہ + 3*stride] = scale * (th - x[اشاریہ + 3*stride]);
    أرجع iou;
}


فراغ delta_yolo_class(عائم *output, عائم *delta, صحيح اشاریہ, صحيح class, صحيح classes, صحيح stride, عائم *avg_cat)
{
    صحيح n;
    إذا (delta[اشاریہ]){
        delta[اشاریہ + stride*class] = 1 - output[اشاریہ + stride*class];
        إذا(avg_cat) *avg_cat += output[اشاریہ + stride*class];
        أرجع;
    }
    لكل(n = 0; n < classes; ++n){
        delta[اشاریہ + stride*n] = ((n == class)?1 : 0) - output[اشاریہ + stride*n];
        إذا(n == class && avg_cat) *avg_cat += output[اشاریہ + stride*n];
    }
}

ساكن صحيح اندراج_اشاریہ(layer l, صحيح batch, صحيح location, صحيح اندراج)
{
    صحيح n =   location / (l.w*l.h);
    صحيح loc = location % (l.w*l.h);
    أرجع batch*l.outputs + n*l.w*l.h*(4+l.classes+1) + اندراج*l.w*l.h + loc;
}

فراغ forward_yolo_layer(ثابت layer l, جال net)
{
    صحيح i,j,b,t,n;
    حافظہ_نقل(l.output, net.داخلہ, l.outputs*l.batch*حجم(عائم));

#جے_بنا_تعریف GPU
    لكل (b = 0; b < l.batch; ++b){
        لكل(n = 0; n < l.n; ++n){
            صحيح اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, 0);
            activate_array(l.output + اشاریہ, 2*l.w*l.h, LOGISTIC);
            اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, 4);
            activate_array(l.output + اشاریہ, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
#جے_انت

    memset(l.delta, 0, l.outputs * l.batch * حجم(عائم));
    إذا(!net.train) أرجع;
    عائم avg_iou = 0;
    عائم recall = 0;
    عائم recall75 = 0;
    عائم avg_cat = 0;
    عائم avg_obj = 0;
    عائم avg_anyobj = 0;
    صحيح گنتی = 0;
    صحيح class_گنتی = 0;
    *(l.لاگت_2) = 0;
    لكل (b = 0; b < l.batch; ++b) {
        لكل (j = 0; j < l.h; ++j) {
            لكل (i = 0; i < l.w; ++i) {
                لكل (n = 0; n < l.n; ++n) {
                    صحيح box_اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_yolo_box(l.output, l.biases, l.نقاب[n], box_اشاریہ, i, j, l.w, l.h, net.w, net.h, l.w*l.h);
                    عائم بہترین_iou = 0;
                    صحيح بہترین_t = 0;
                    لكل(t = 0; t < l.max_boxes; ++t){
                        box truth = float_منزل_box(net.truth + t*(4 + 1) + b*l.truths, 1);
                        إذا(!truth.x) اكسر;
                        عائم iou = box_iou(pred, truth);
                        إذا (iou > بہترین_iou) {
                            بہترین_iou = iou;
                            بہترین_t = t;
                        }
                    }
                    صحيح obj_اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h + j*l.w + i, 4);
                    avg_anyobj += l.output[obj_اشاریہ];
                    l.delta[obj_اشاریہ] = 0 - l.output[obj_اشاریہ];
                    إذا (بہترین_iou > l.ignore_thresh) {
                        l.delta[obj_اشاریہ] = 0;
                    }
                    إذا (بہترین_iou > l.truth_thresh) {
                        l.delta[obj_اشاریہ] = 1 - l.output[obj_اشاریہ];

                        صحيح class = net.truth[بہترین_t*(4 + 1) + b*l.truths + 4];
                        إذا (l.map) class = l.map[class];
                        صحيح class_اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h + j*l.w + i, 4 + 1);
                        delta_yolo_class(l.output, l.delta, class_اشاریہ, class, l.classes, l.w*l.h, 0);
                        box truth = float_منزل_box(net.truth + بہترین_t*(4 + 1) + b*l.truths, 1);
                        delta_yolo_box(truth, l.output, l.biases, l.نقاب[n], box_اشاریہ, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);
                    }
                }
            }
        }
        لكل(t = 0; t < l.max_boxes; ++t){
            box truth = float_منزل_box(net.truth + t*(4 + 1) + b*l.truths, 1);

            إذا(!truth.x) اكسر;
            عائم بہترین_iou = 0;
            صحيح بہترین_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_کھسکاؤ = truth;
            truth_کھسکاؤ.x = truth_کھسکاؤ.y = 0;
            لكل(n = 0; n < l.کل; ++n){
                box pred = {0};
                pred.w = l.biases[2*n]/net.w;
                pred.h = l.biases[2*n+1]/net.h;
                عائم iou = box_iou(pred, truth_کھسکاؤ);
                إذا (iou > بہترین_iou){
                    بہترین_iou = iou;
                    بہترین_n = n;
                }
            }

            صحيح نقاب_n = int_اشاریہ(l.نقاب, بہترین_n, l.n);
            إذا(نقاب_n >= 0){
                صحيح box_اشاریہ = اندراج_اشاریہ(l, b, نقاب_n*l.w*l.h + j*l.w + i, 0);
                عائم iou = delta_yolo_box(truth, l.output, l.biases, بہترین_n, box_اشاریہ, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);

                صحيح obj_اشاریہ = اندراج_اشاریہ(l, b, نقاب_n*l.w*l.h + j*l.w + i, 4);
                avg_obj += l.output[obj_اشاریہ];
                l.delta[obj_اشاریہ] = 1 - l.output[obj_اشاریہ];

                صحيح class = net.truth[t*(4 + 1) + b*l.truths + 4];
                إذا (l.map) class = l.map[class];
                صحيح class_اشاریہ = اندراج_اشاریہ(l, b, نقاب_n*l.w*l.h + j*l.w + i, 4 + 1);
                delta_yolo_class(l.output, l.delta, class_اشاریہ, class, l.classes, l.w*l.h, &avg_cat);

                ++گنتی;
                ++class_گنتی;
                إذا(iou > .5) recall += 1;
                إذا(iou > .75) recall75 += 1;
                avg_iou += iou;
            }
        }
    }
    *(l.لاگت_2) = قوت(mag_array(l.delta, l.outputs * l.batch), 2);
    لکھو("Region %d Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, .5R: %f, .75R: %f,  count: %d\n", net.اشاریہ, avg_iou/گنتی, avg_cat/class_گنتی, avg_obj/گنتی, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/گنتی, recall75/گنتی, گنتی);
}

فراغ backward_yolo_layer(ثابت layer l, جال net)
{
   axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

فراغ correct_yolo_boxes(detection *dets, صحيح n, صحيح w, صحيح h, صحيح netw, صحيح neth, صحيح relative)
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

صحيح yolo_عدد_detections(layer l, عائم thresh)
{
    صحيح i, n;
    صحيح گنتی = 0;
    لكل (i = 0; i < l.w*l.h; ++i){
        لكل(n = 0; n < l.n; ++n){
            صحيح obj_اشاریہ  = اندراج_اشاریہ(l, 0, n*l.w*l.h + i, 4);
            إذا(l.output[obj_اشاریہ] > thresh){
                ++گنتی;
            }
        }
    }
    أرجع گنتی;
}

فراغ avg_flipped_yolo(layer l)
{
    صحيح i,j,n,z;
    عائم *flip = l.output + l.outputs;
    لكل (j = 0; j < l.h; ++j) {
        لكل (i = 0; i < l.w/2; ++i) {
            لكل (n = 0; n < l.n; ++n) {
                لكل(z = 0; z < l.classes + 4 + 1; ++z){
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

صحيح get_yolo_detections(layer l, صحيح w, صحيح h, صحيح netw, صحيح neth, عائم thresh, صحيح *map, صحيح relative, detection *dets)
{
    صحيح i,j,n;
    عائم *predictions = l.output;
    إذا (l.batch == 2) avg_flipped_yolo(l);
    صحيح گنتی = 0;
    لكل (i = 0; i < l.w*l.h; ++i){
        صحيح صف = i / l.w;
        صحيح خانہ = i % l.w;
        لكل(n = 0; n < l.n; ++n){
            صحيح obj_اشاریہ  = اندراج_اشاریہ(l, 0, n*l.w*l.h + i, 4);
            عائم objectness = predictions[obj_اشاریہ];
            إذا(objectness <= thresh) استمر;
            صحيح box_اشاریہ  = اندراج_اشاریہ(l, 0, n*l.w*l.h + i, 0);
            dets[گنتی].bbox = get_yolo_box(predictions, l.biases, l.نقاب[n], box_اشاریہ, خانہ, صف, l.w, l.h, netw, neth, l.w*l.h);
            dets[گنتی].objectness = objectness;
            dets[گنتی].classes = l.classes;
            لكل(j = 0; j < l.classes; ++j){
                صحيح class_اشاریہ = اندراج_اشاریہ(l, 0, n*l.w*l.h + i, 4 + 1 + j);
                عائم prob = objectness*predictions[class_اشاریہ];
                dets[گنتی].prob[j] = (prob > thresh) ? prob : 0;
            }
            ++گنتی;
        }
    }
    correct_yolo_boxes(dets, گنتی, w, h, netw, neth, relative);
    أرجع گنتی;
}

#جے_تعریف_شدہ GPU

فراغ forward_yolo_layer_gpu(ثابت layer l, جال net)
{
    copy_gpu(l.batch*l.inputs, net.داخلہ_gpu, 1, l.output_gpu, 1);
    صحيح b, n;
    لكل (b = 0; b < l.batch; ++b){
        لكل(n = 0; n < l.n; ++n){
            صحيح اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + اشاریہ, 2*l.w*l.h, LOGISTIC);
            اشاریہ = اندراج_اشاریہ(l, b, n*l.w*l.h, 4);
            activate_array_gpu(l.output_gpu + اشاریہ, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
    إذا(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        أرجع;
    }

    cuda_pull_array(l.output_gpu, net.داخلہ, l.batch*l.inputs);
    forward_yolo_layer(l, net);
    cuda_ڈالو_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

فراغ backward_yolo_layer_gpu(ثابت layer l, جال net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#جے_انت


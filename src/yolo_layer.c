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

layer make_yolo_layer(صحيح batch, صحيح w, صحيح h, صحيح n, صحيح ڪل, صحيح *نقاب, صحيح classes)
{
    صحيح i;
    layer l = {0};
    l.type = YOLO;

    l.n = n;
    l.ڪل = ڪل;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = n*(classes + 4 + 1);
    l.ٻاهريون_w = l.w;
    l.ٻاهريون_h = l.h;
    l.ٻاهريون_c = l.c;
    l.classes = classes;
    l.لاڳت_2 = صفر_يادگيري_ڏيو(1, حجم(عائم));
    l.biases = صفر_يادگيري_ڏيو(ڪل*2, حجم(عائم));
    إذا(نقاب) l.نقاب = نقاب;
    وإلا{
        l.نقاب = صفر_يادگيري_ڏيو(n, حجم(صحيح));
        لكل(i = 0; i < n; ++i){
            l.نقاب[i] = i;
        }
    }
    l.bias_updates = صفر_يادگيري_ڏيو(n*2, حجم(عائم));
    l.outputs = h*w*n*(classes + 4 + 1);
    l.inputs = l.outputs;
    l.truths = 90*(4 + 1);
    l.delta = صفر_يادگيري_ڏيو(batch*l.outputs, حجم(عائم));
    l.output = صفر_يادگيري_ڏيو(batch*l.outputs, حجم(عائم));
    لكل(i = 0; i < ڪل*2; ++i){
        l.biases[i] = .5;
    }

    l.forward = forward_yolo_layer;
    l.backward = backward_yolo_layer;
#جيڪڏهن_وصف_ٿيل GPU
    l.forward_gpu = forward_yolo_layer_gpu;
    l.backward_gpu = backward_yolo_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#جيڪڏهن_پورو

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

#جيڪڏهن_وصف_ٿيل GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#جيڪڏهن_پورو
}

box get_yolo_box(عائم *x, عائم *biases, صحيح n, صحيح اشاريو, صحيح i, صحيح j, صحيح lw, صحيح lh, صحيح w, صحيح h, صحيح stride)
{
    box b;
    b.x = (i + x[اشاريو + 0*stride]) / lw;
    b.y = (j + x[اشاريو + 1*stride]) / lh;
    b.w = قوت_نما(x[اشاريو + 2*stride]) * biases[2*n]   / w;
    b.h = قوت_نما(x[اشاريو + 3*stride]) * biases[2*n+1] / h;
    أرجع b;
}

عائم delta_yolo_box(box truth, عائم *x, عائم *biases, صحيح n, صحيح اشاريو, صحيح i, صحيح j, صحيح lw, صحيح lh, صحيح w, صحيح h, عائم *delta, عائم scale, صحيح stride)
{
    box pred = get_yolo_box(x, biases, n, اشاريو, i, j, lw, lh, w, h, stride);
    عائم iou = box_iou(pred, truth);

    عائم tx = (truth.x*lw - i);
    عائم ty = (truth.y*lh - j);
    عائم tw = لاگرٿم(truth.w*w / biases[2*n]);
    عائم th = لاگرٿم(truth.h*h / biases[2*n + 1]);

    delta[اشاريو + 0*stride] = scale * (tx - x[اشاريو + 0*stride]);
    delta[اشاريو + 1*stride] = scale * (ty - x[اشاريو + 1*stride]);
    delta[اشاريو + 2*stride] = scale * (tw - x[اشاريو + 2*stride]);
    delta[اشاريو + 3*stride] = scale * (th - x[اشاريو + 3*stride]);
    أرجع iou;
}


فراغ delta_yolo_class(عائم *output, عائم *delta, صحيح اشاريو, صحيح class, صحيح classes, صحيح stride, عائم *avg_cat)
{
    صحيح n;
    إذا (delta[اشاريو]){
        delta[اشاريو + stride*class] = 1 - output[اشاريو + stride*class];
        إذا(avg_cat) *avg_cat += output[اشاريو + stride*class];
        أرجع;
    }
    لكل(n = 0; n < classes; ++n){
        delta[اشاريو + stride*n] = ((n == class)?1 : 0) - output[اشاريو + stride*n];
        إذا(n == class && avg_cat) *avg_cat += output[اشاريو + stride*n];
    }
}

ساكن صحيح داخلا_اشاريو(layer l, صحيح batch, صحيح location, صحيح داخلا_2)
{
    صحيح n =   location / (l.w*l.h);
    صحيح loc = location % (l.w*l.h);
    أرجع batch*l.outputs + n*l.w*l.h*(4+l.classes+1) + داخلا_2*l.w*l.h + loc;
}

فراغ forward_yolo_layer(ثابت layer l, ڄار net)
{
    صحيح i,j,b,t,n;
    يادگيري_نقل(l.output, net.داخلا, l.outputs*l.batch*حجم(عائم));

#جيڪڏهن_وصف_نه_ٿيل GPU
    لكل (b = 0; b < l.batch; ++b){
        لكل(n = 0; n < l.n; ++n){
            صحيح اشاريو = داخلا_اشاريو(l, b, n*l.w*l.h, 0);
            activate_array(l.output + اشاريو, 2*l.w*l.h, LOGISTIC);
            اشاريو = داخلا_اشاريو(l, b, n*l.w*l.h, 4);
            activate_array(l.output + اشاريو, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
#جيڪڏهن_پورو

    memset(l.delta, 0, l.outputs * l.batch * حجم(عائم));
    إذا(!net.train) أرجع;
    عائم avg_iou = 0;
    عائم recall = 0;
    عائم recall75 = 0;
    عائم avg_cat = 0;
    عائم avg_obj = 0;
    عائم avg_anyobj = 0;
    صحيح ڳڻپ = 0;
    صحيح class_ڳڻپ = 0;
    *(l.لاڳت_2) = 0;
    لكل (b = 0; b < l.batch; ++b) {
        لكل (j = 0; j < l.h; ++j) {
            لكل (i = 0; i < l.w; ++i) {
                لكل (n = 0; n < l.n; ++n) {
                    صحيح box_اشاريو = داخلا_اشاريو(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_yolo_box(l.output, l.biases, l.نقاب[n], box_اشاريو, i, j, l.w, l.h, net.w, net.h, l.w*l.h);
                    عائم بهترين_iou = 0;
                    صحيح بهترين_t = 0;
                    لكل(t = 0; t < l.max_boxes; ++t){
                        box truth = float_منزل_box(net.truth + t*(4 + 1) + b*l.truths, 1);
                        إذا(!truth.x) اكسر;
                        عائم iou = box_iou(pred, truth);
                        إذا (iou > بهترين_iou) {
                            بهترين_iou = iou;
                            بهترين_t = t;
                        }
                    }
                    صحيح obj_اشاريو = داخلا_اشاريو(l, b, n*l.w*l.h + j*l.w + i, 4);
                    avg_anyobj += l.output[obj_اشاريو];
                    l.delta[obj_اشاريو] = 0 - l.output[obj_اشاريو];
                    إذا (بهترين_iou > l.ignore_thresh) {
                        l.delta[obj_اشاريو] = 0;
                    }
                    إذا (بهترين_iou > l.truth_thresh) {
                        l.delta[obj_اشاريو] = 1 - l.output[obj_اشاريو];

                        صحيح class = net.truth[بهترين_t*(4 + 1) + b*l.truths + 4];
                        إذا (l.map) class = l.map[class];
                        صحيح class_اشاريو = داخلا_اشاريو(l, b, n*l.w*l.h + j*l.w + i, 4 + 1);
                        delta_yolo_class(l.output, l.delta, class_اشاريو, class, l.classes, l.w*l.h, 0);
                        box truth = float_منزل_box(net.truth + بهترين_t*(4 + 1) + b*l.truths, 1);
                        delta_yolo_box(truth, l.output, l.biases, l.نقاب[n], box_اشاريو, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);
                    }
                }
            }
        }
        لكل(t = 0; t < l.max_boxes; ++t){
            box truth = float_منزل_box(net.truth + t*(4 + 1) + b*l.truths, 1);

            إذا(!truth.x) اكسر;
            عائم بهترين_iou = 0;
            صحيح بهترين_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_سرڪايو = truth;
            truth_سرڪايو.x = truth_سرڪايو.y = 0;
            لكل(n = 0; n < l.ڪل; ++n){
                box pred = {0};
                pred.w = l.biases[2*n]/net.w;
                pred.h = l.biases[2*n+1]/net.h;
                عائم iou = box_iou(pred, truth_سرڪايو);
                إذا (iou > بهترين_iou){
                    بهترين_iou = iou;
                    بهترين_n = n;
                }
            }

            صحيح نقاب_n = int_اشاريو(l.نقاب, بهترين_n, l.n);
            إذا(نقاب_n >= 0){
                صحيح box_اشاريو = داخلا_اشاريو(l, b, نقاب_n*l.w*l.h + j*l.w + i, 0);
                عائم iou = delta_yolo_box(truth, l.output, l.biases, بهترين_n, box_اشاريو, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);

                صحيح obj_اشاريو = داخلا_اشاريو(l, b, نقاب_n*l.w*l.h + j*l.w + i, 4);
                avg_obj += l.output[obj_اشاريو];
                l.delta[obj_اشاريو] = 1 - l.output[obj_اشاريو];

                صحيح class = net.truth[t*(4 + 1) + b*l.truths + 4];
                إذا (l.map) class = l.map[class];
                صحيح class_اشاريو = داخلا_اشاريو(l, b, نقاب_n*l.w*l.h + j*l.w + i, 4 + 1);
                delta_yolo_class(l.output, l.delta, class_اشاريو, class, l.classes, l.w*l.h, &avg_cat);

                ++ڳڻپ;
                ++class_ڳڻپ;
                إذا(iou > .5) recall += 1;
                إذا(iou > .75) recall75 += 1;
                avg_iou += iou;
            }
        }
    }
    *(l.لاڳت_2) = قوت(mag_array(l.delta, l.outputs * l.batch), 2);
    لکو("Region %d Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, .5R: %f, .75R: %f,  count: %d\n", net.اشاريو, avg_iou/ڳڻپ, avg_cat/class_ڳڻپ, avg_obj/ڳڻپ, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/ڳڻپ, recall75/ڳڻپ, ڳڻپ);
}

فراغ backward_yolo_layer(ثابت layer l, ڄار net)
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

صحيح yolo_انگ_detections(layer l, عائم thresh)
{
    صحيح i, n;
    صحيح ڳڻپ = 0;
    لكل (i = 0; i < l.w*l.h; ++i){
        لكل(n = 0; n < l.n; ++n){
            صحيح obj_اشاريو  = داخلا_اشاريو(l, 0, n*l.w*l.h + i, 4);
            إذا(l.output[obj_اشاريو] > thresh){
                ++ڳڻپ;
            }
        }
    }
    أرجع ڳڻپ;
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
                    عائم مٽا_سٽا = flip[i1];
                    flip[i1] = flip[i2];
                    flip[i2] = مٽا_سٽا;
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
    صحيح ڳڻپ = 0;
    لكل (i = 0; i < l.w*l.h; ++i){
        صحيح صف = i / l.w;
        صحيح کانو = i % l.w;
        لكل(n = 0; n < l.n; ++n){
            صحيح obj_اشاريو  = داخلا_اشاريو(l, 0, n*l.w*l.h + i, 4);
            عائم objectness = predictions[obj_اشاريو];
            إذا(objectness <= thresh) استمر;
            صحيح box_اشاريو  = داخلا_اشاريو(l, 0, n*l.w*l.h + i, 0);
            dets[ڳڻپ].bbox = get_yolo_box(predictions, l.biases, l.نقاب[n], box_اشاريو, کانو, صف, l.w, l.h, netw, neth, l.w*l.h);
            dets[ڳڻپ].objectness = objectness;
            dets[ڳڻپ].classes = l.classes;
            لكل(j = 0; j < l.classes; ++j){
                صحيح class_اشاريو = داخلا_اشاريو(l, 0, n*l.w*l.h + i, 4 + 1 + j);
                عائم prob = objectness*predictions[class_اشاريو];
                dets[ڳڻپ].prob[j] = (prob > thresh) ? prob : 0;
            }
            ++ڳڻپ;
        }
    }
    correct_yolo_boxes(dets, ڳڻپ, w, h, netw, neth, relative);
    أرجع ڳڻپ;
}

#جيڪڏهن_وصف_ٿيل GPU

فراغ forward_yolo_layer_gpu(ثابت layer l, ڄار net)
{
    copy_gpu(l.batch*l.inputs, net.داخلا_gpu, 1, l.output_gpu, 1);
    صحيح b, n;
    لكل (b = 0; b < l.batch; ++b){
        لكل(n = 0; n < l.n; ++n){
            صحيح اشاريو = داخلا_اشاريو(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + اشاريو, 2*l.w*l.h, LOGISTIC);
            اشاريو = داخلا_اشاريو(l, b, n*l.w*l.h, 4);
            activate_array_gpu(l.output_gpu + اشاريو, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
    إذا(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        أرجع;
    }

    cuda_pull_array(l.output_gpu, net.داخلا, l.batch*l.inputs);
    forward_yolo_layer(l, net);
    cuda_ڌڪو_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

فراغ backward_yolo_layer_gpu(ثابت layer l, ڄار net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#جيڪڏهن_پورو


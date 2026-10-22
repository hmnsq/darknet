#ਅਨ੍ਤਰ੍ਭਾਵ "yolo_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "activations.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "box.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>

layer make_yolo_layer(ਪੂਰਨ batch, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ n, ਪੂਰਨ ਕੁੱਲ, ਪੂਰਨ *ਨਕਾਬ, ਪੂਰਨ classes)
{
    ਪੂਰਨ i;
    layer l = {0};
    l.type = YOLO;

    l.n = n;
    l.ਕੁੱਲ = ਕੁੱਲ;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = n*(classes + 4 + 1);
    l.ਬਾਹਰਲਾ_w = l.w;
    l.ਬਾਹਰਲਾ_h = l.h;
    l.ਬਾਹਰਲਾ_c = l.c;
    l.classes = classes;
    l.ਲਾਗਤ_2 = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.biases = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਕੁੱਲ*2, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਜੇ(ਨਕਾਬ) l.ਨਕਾਬ = ਨਕਾਬ;
    ਨਹੀਂਤਾਂ{
        l.ਨਕਾਬ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਪੂਰਨ));
        ਲਈ(i = 0; i < n; ++i){
            l.ਨਕਾਬ[i] = i;
        }
    }
    l.bias_updates = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n*2, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.outputs = h*w*n*(classes + 4 + 1);
    l.inputs = l.outputs;
    l.truths = 90*(4 + 1);
    l.delta = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*l.outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.output = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*l.outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਲਈ(i = 0; i < ਕੁੱਲ*2; ++i){
        l.biases[i] = .5;
    }

    l.forward = forward_yolo_layer;
    l.backward = backward_yolo_layer;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_yolo_layer_gpu;
    l.backward_gpu = backward_yolo_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#ਜੇ_ਅੰਤ

    fprintf(stderr, "yolo\n");
    srand(0);

    ਵਾਪਸ l;
}

ਖਾਲੀ resize_yolo_layer(layer *l, ਪੂਰਨ w, ਪੂਰਨ h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + 4 + 1);
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l->delta = realloc(l->delta, l->batch*l->outputs*ਆਕਾਰ(ਦਸ਼ਮਲਵ));

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#ਜੇ_ਅੰਤ
}

box get_yolo_box(ਦਸ਼ਮਲਵ *x, ਦਸ਼ਮਲਵ *biases, ਪੂਰਨ n, ਪੂਰਨ ਸੂਚਕਾਂਕ, ਪੂਰਨ i, ਪੂਰਨ j, ਪੂਰਨ lw, ਪੂਰਨ lh, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ stride)
{
    box b;
    b.x = (i + x[ਸੂਚਕਾਂਕ + 0*stride]) / lw;
    b.y = (j + x[ਸੂਚਕਾਂਕ + 1*stride]) / lh;
    b.w = ਘਾਤਾਂਕ(x[ਸੂਚਕਾਂਕ + 2*stride]) * biases[2*n]   / w;
    b.h = ਘਾਤਾਂਕ(x[ਸੂਚਕਾਂਕ + 3*stride]) * biases[2*n+1] / h;
    ਵਾਪਸ b;
}

ਦਸ਼ਮਲਵ delta_yolo_box(box truth, ਦਸ਼ਮਲਵ *x, ਦਸ਼ਮਲਵ *biases, ਪੂਰਨ n, ਪੂਰਨ ਸੂਚਕਾਂਕ, ਪੂਰਨ i, ਪੂਰਨ j, ਪੂਰਨ lw, ਪੂਰਨ lh, ਪੂਰਨ w, ਪੂਰਨ h, ਦਸ਼ਮਲਵ *delta, ਦਸ਼ਮਲਵ scale, ਪੂਰਨ stride)
{
    box pred = get_yolo_box(x, biases, n, ਸੂਚਕਾਂਕ, i, j, lw, lh, w, h, stride);
    ਦਸ਼ਮਲਵ iou = box_iou(pred, truth);

    ਦਸ਼ਮਲਵ tx = (truth.x*lw - i);
    ਦਸ਼ਮਲਵ ty = (truth.y*lh - j);
    ਦਸ਼ਮਲਵ tw = ਲਘੂਗਣਕ(truth.w*w / biases[2*n]);
    ਦਸ਼ਮਲਵ th = ਲਘੂਗਣਕ(truth.h*h / biases[2*n + 1]);

    delta[ਸੂਚਕਾਂਕ + 0*stride] = scale * (tx - x[ਸੂਚਕਾਂਕ + 0*stride]);
    delta[ਸੂਚਕਾਂਕ + 1*stride] = scale * (ty - x[ਸੂਚਕਾਂਕ + 1*stride]);
    delta[ਸੂਚਕਾਂਕ + 2*stride] = scale * (tw - x[ਸੂਚਕਾਂਕ + 2*stride]);
    delta[ਸੂਚਕਾਂਕ + 3*stride] = scale * (th - x[ਸੂਚਕਾਂਕ + 3*stride]);
    ਵਾਪਸ iou;
}


ਖਾਲੀ delta_yolo_class(ਦਸ਼ਮਲਵ *output, ਦਸ਼ਮਲਵ *delta, ਪੂਰਨ ਸੂਚਕਾਂਕ, ਪੂਰਨ class, ਪੂਰਨ classes, ਪੂਰਨ stride, ਦਸ਼ਮਲਵ *avg_cat)
{
    ਪੂਰਨ n;
    ਜੇ (delta[ਸੂਚਕਾਂਕ]){
        delta[ਸੂਚਕਾਂਕ + stride*class] = 1 - output[ਸੂਚਕਾਂਕ + stride*class];
        ਜੇ(avg_cat) *avg_cat += output[ਸੂਚਕਾਂਕ + stride*class];
        ਵਾਪਸ;
    }
    ਲਈ(n = 0; n < classes; ++n){
        delta[ਸੂਚਕਾਂਕ + stride*n] = ((n == class)?1 : 0) - output[ਸੂਚਕਾਂਕ + stride*n];
        ਜੇ(n == class && avg_cat) *avg_cat += output[ਸੂਚਕਾਂਕ + stride*n];
    }
}

ਸਥਿਰ ਪੂਰਨ ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(layer l, ਪੂਰਨ batch, ਪੂਰਨ location, ਪੂਰਨ ਇੰਦਰਾਜ)
{
    ਪੂਰਨ n =   location / (l.w*l.h);
    ਪੂਰਨ loc = location % (l.w*l.h);
    ਵਾਪਸ batch*l.outputs + n*l.w*l.h*(4+l.classes+1) + ਇੰਦਰਾਜ*l.w*l.h + loc;
}

ਖਾਲੀ forward_yolo_layer(ਅਚਲ layer l, ਜਾਲ net)
{
    ਪੂਰਨ i,j,b,t,n;
    ਸਮ੍ਰਿਤੀ_ਨਕਲ(l.output, net.ਨਿਵੇਸ਼, l.outputs*l.batch*ਆਕਾਰ(ਦਸ਼ਮਲਵ));

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GPU
    ਲਈ (b = 0; b < l.batch; ++b){
        ਲਈ(n = 0; n < l.n; ++n){
            ਪੂਰਨ ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, n*l.w*l.h, 0);
            activate_array(l.output + ਸੂਚਕਾਂਕ, 2*l.w*l.h, LOGISTIC);
            ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, n*l.w*l.h, 4);
            activate_array(l.output + ਸੂਚਕਾਂਕ, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
#ਜੇ_ਅੰਤ

    memset(l.delta, 0, l.outputs * l.batch * ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਜੇ(!net.train) ਵਾਪਸ;
    ਦਸ਼ਮਲਵ avg_iou = 0;
    ਦਸ਼ਮਲਵ recall = 0;
    ਦਸ਼ਮਲਵ recall75 = 0;
    ਦਸ਼ਮਲਵ avg_cat = 0;
    ਦਸ਼ਮਲਵ avg_obj = 0;
    ਦਸ਼ਮਲਵ avg_anyobj = 0;
    ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    ਪੂਰਨ class_ਗਿਣਤੀ = 0;
    *(l.ਲਾਗਤ_2) = 0;
    ਲਈ (b = 0; b < l.batch; ++b) {
        ਲਈ (j = 0; j < l.h; ++j) {
            ਲਈ (i = 0; i < l.w; ++i) {
                ਲਈ (n = 0; n < l.n; ++n) {
                    ਪੂਰਨ box_ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_yolo_box(l.output, l.biases, l.ਨਕਾਬ[n], box_ਸੂਚਕਾਂਕ, i, j, l.w, l.h, net.w, net.h, l.w*l.h);
                    ਦਸ਼ਮਲਵ ਉੱਤਮ_iou = 0;
                    ਪੂਰਨ ਉੱਤਮ_t = 0;
                    ਲਈ(t = 0; t < l.max_boxes; ++t){
                        box truth = float_ਮੰਜ਼ਿਲ_box(net.truth + t*(4 + 1) + b*l.truths, 1);
                        ਜੇ(!truth.x) ਰੋਕੋ;
                        ਦਸ਼ਮਲਵ iou = box_iou(pred, truth);
                        ਜੇ (iou > ਉੱਤਮ_iou) {
                            ਉੱਤਮ_iou = iou;
                            ਉੱਤਮ_t = t;
                        }
                    }
                    ਪੂਰਨ obj_ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, n*l.w*l.h + j*l.w + i, 4);
                    avg_anyobj += l.output[obj_ਸੂਚਕਾਂਕ];
                    l.delta[obj_ਸੂਚਕਾਂਕ] = 0 - l.output[obj_ਸੂਚਕਾਂਕ];
                    ਜੇ (ਉੱਤਮ_iou > l.ignore_thresh) {
                        l.delta[obj_ਸੂਚਕਾਂਕ] = 0;
                    }
                    ਜੇ (ਉੱਤਮ_iou > l.truth_thresh) {
                        l.delta[obj_ਸੂਚਕਾਂਕ] = 1 - l.output[obj_ਸੂਚਕਾਂਕ];

                        ਪੂਰਨ class = net.truth[ਉੱਤਮ_t*(4 + 1) + b*l.truths + 4];
                        ਜੇ (l.map) class = l.map[class];
                        ਪੂਰਨ class_ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, n*l.w*l.h + j*l.w + i, 4 + 1);
                        delta_yolo_class(l.output, l.delta, class_ਸੂਚਕਾਂਕ, class, l.classes, l.w*l.h, 0);
                        box truth = float_ਮੰਜ਼ਿਲ_box(net.truth + ਉੱਤਮ_t*(4 + 1) + b*l.truths, 1);
                        delta_yolo_box(truth, l.output, l.biases, l.ਨਕਾਬ[n], box_ਸੂਚਕਾਂਕ, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);
                    }
                }
            }
        }
        ਲਈ(t = 0; t < l.max_boxes; ++t){
            box truth = float_ਮੰਜ਼ਿਲ_box(net.truth + t*(4 + 1) + b*l.truths, 1);

            ਜੇ(!truth.x) ਰੋਕੋ;
            ਦਸ਼ਮਲਵ ਉੱਤਮ_iou = 0;
            ਪੂਰਨ ਉੱਤਮ_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_ਖਿਸਕਾਓ = truth;
            truth_ਖਿਸਕਾਓ.x = truth_ਖਿਸਕਾਓ.y = 0;
            ਲਈ(n = 0; n < l.ਕੁੱਲ; ++n){
                box pred = {0};
                pred.w = l.biases[2*n]/net.w;
                pred.h = l.biases[2*n+1]/net.h;
                ਦਸ਼ਮਲਵ iou = box_iou(pred, truth_ਖਿਸਕਾਓ);
                ਜੇ (iou > ਉੱਤਮ_iou){
                    ਉੱਤਮ_iou = iou;
                    ਉੱਤਮ_n = n;
                }
            }

            ਪੂਰਨ ਨਕਾਬ_n = int_ਸੂਚਕਾਂਕ(l.ਨਕਾਬ, ਉੱਤਮ_n, l.n);
            ਜੇ(ਨਕਾਬ_n >= 0){
                ਪੂਰਨ box_ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, ਨਕਾਬ_n*l.w*l.h + j*l.w + i, 0);
                ਦਸ਼ਮਲਵ iou = delta_yolo_box(truth, l.output, l.biases, ਉੱਤਮ_n, box_ਸੂਚਕਾਂਕ, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);

                ਪੂਰਨ obj_ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, ਨਕਾਬ_n*l.w*l.h + j*l.w + i, 4);
                avg_obj += l.output[obj_ਸੂਚਕਾਂਕ];
                l.delta[obj_ਸੂਚਕਾਂਕ] = 1 - l.output[obj_ਸੂਚਕਾਂਕ];

                ਪੂਰਨ class = net.truth[t*(4 + 1) + b*l.truths + 4];
                ਜੇ (l.map) class = l.map[class];
                ਪੂਰਨ class_ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, ਨਕਾਬ_n*l.w*l.h + j*l.w + i, 4 + 1);
                delta_yolo_class(l.output, l.delta, class_ਸੂਚਕਾਂਕ, class, l.classes, l.w*l.h, &avg_cat);

                ++ਗਿਣਤੀ_2;
                ++class_ਗਿਣਤੀ;
                ਜੇ(iou > .5) recall += 1;
                ਜੇ(iou > .75) recall75 += 1;
                avg_iou += iou;
            }
        }
    }
    *(l.ਲਾਗਤ_2) = ਘਾਤ(mag_array(l.delta, l.outputs * l.batch), 2);
    ਲਿਖੋ("Region %d Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, .5R: %f, .75R: %f,  count: %d\n", net.ਸੂਚਕਾਂਕ, avg_iou/ਗਿਣਤੀ_2, avg_cat/class_ਗਿਣਤੀ, avg_obj/ਗਿਣਤੀ_2, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/ਗਿਣਤੀ_2, recall75/ਗਿਣਤੀ_2, ਗਿਣਤੀ_2);
}

ਖਾਲੀ backward_yolo_layer(ਅਚਲ layer l, ਜਾਲ net)
{
   axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

ਖਾਲੀ correct_yolo_boxes(detection *dets, ਪੂਰਨ n, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ netw, ਪੂਰਨ neth, ਪੂਰਨ relative)
{
    ਪੂਰਨ i;
    ਪੂਰਨ new_w=0;
    ਪੂਰਨ new_h=0;
    ਜੇ (((ਦਸ਼ਮਲਵ)netw/w) < ((ਦਸ਼ਮਲਵ)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } ਨਹੀਂਤਾਂ {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    ਲਈ (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((ਦਸ਼ਮਲਵ)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((ਦਸ਼ਮਲਵ)new_h/neth); 
        b.w *= (ਦਸ਼ਮਲਵ)netw/new_w;
        b.h *= (ਦਸ਼ਮਲਵ)neth/new_h;
        ਜੇ(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

ਪੂਰਨ yolo_ਅੰਕ_detections(layer l, ਦਸ਼ਮਲਵ thresh)
{
    ਪੂਰਨ i, n;
    ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    ਲਈ (i = 0; i < l.w*l.h; ++i){
        ਲਈ(n = 0; n < l.n; ++n){
            ਪੂਰਨ obj_ਸੂਚਕਾਂਕ  = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, 0, n*l.w*l.h + i, 4);
            ਜੇ(l.output[obj_ਸੂਚਕਾਂਕ] > thresh){
                ++ਗਿਣਤੀ_2;
            }
        }
    }
    ਵਾਪਸ ਗਿਣਤੀ_2;
}

ਖਾਲੀ avg_flipped_yolo(layer l)
{
    ਪੂਰਨ i,j,n,z;
    ਦਸ਼ਮਲਵ *flip = l.output + l.outputs;
    ਲਈ (j = 0; j < l.h; ++j) {
        ਲਈ (i = 0; i < l.w/2; ++i) {
            ਲਈ (n = 0; n < l.n; ++n) {
                ਲਈ(z = 0; z < l.classes + 4 + 1; ++z){
                    ਪੂਰਨ i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                    ਪੂਰਨ i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                    ਦਸ਼ਮਲਵ ਅਦਲਾ_ਬਦਲੀ = flip[i1];
                    flip[i1] = flip[i2];
                    flip[i2] = ਅਦਲਾ_ਬਦਲੀ;
                    ਜੇ(z == 0){
                        flip[i1] = -flip[i1];
                        flip[i2] = -flip[i2];
                    }
                }
            }
        }
    }
    ਲਈ(i = 0; i < l.outputs; ++i){
        l.output[i] = (l.output[i] + flip[i])/2.;
    }
}

ਪੂਰਨ get_yolo_detections(layer l, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ netw, ਪੂਰਨ neth, ਦਸ਼ਮਲਵ thresh, ਪੂਰਨ *map, ਪੂਰਨ relative, detection *dets)
{
    ਪੂਰਨ i,j,n;
    ਦਸ਼ਮਲਵ *predictions = l.output;
    ਜੇ (l.batch == 2) avg_flipped_yolo(l);
    ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    ਲਈ (i = 0; i < l.w*l.h; ++i){
        ਪੂਰਨ ਪੰਕਤੀ = i / l.w;
        ਪੂਰਨ ਖਾਨਾ = i % l.w;
        ਲਈ(n = 0; n < l.n; ++n){
            ਪੂਰਨ obj_ਸੂਚਕਾਂਕ  = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, 0, n*l.w*l.h + i, 4);
            ਦਸ਼ਮਲਵ objectness = predictions[obj_ਸੂਚਕਾਂਕ];
            ਜੇ(objectness <= thresh) ਜਾਰੀ;
            ਪੂਰਨ box_ਸੂਚਕਾਂਕ  = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, 0, n*l.w*l.h + i, 0);
            dets[ਗਿਣਤੀ_2].bbox = get_yolo_box(predictions, l.biases, l.ਨਕਾਬ[n], box_ਸੂਚਕਾਂਕ, ਖਾਨਾ, ਪੰਕਤੀ, l.w, l.h, netw, neth, l.w*l.h);
            dets[ਗਿਣਤੀ_2].objectness = objectness;
            dets[ਗਿਣਤੀ_2].classes = l.classes;
            ਲਈ(j = 0; j < l.classes; ++j){
                ਪੂਰਨ class_ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, 0, n*l.w*l.h + i, 4 + 1 + j);
                ਦਸ਼ਮਲਵ prob = objectness*predictions[class_ਸੂਚਕਾਂਕ];
                dets[ਗਿਣਤੀ_2].prob[j] = (prob > thresh) ? prob : 0;
            }
            ++ਗਿਣਤੀ_2;
        }
    }
    correct_yolo_boxes(dets, ਗਿਣਤੀ_2, w, h, netw, neth, relative);
    ਵਾਪਸ ਗਿਣਤੀ_2;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU

ਖਾਲੀ forward_yolo_layer_gpu(ਅਚਲ layer l, ਜਾਲ net)
{
    copy_gpu(l.batch*l.inputs, net.ਨਿਵੇਸ਼_gpu, 1, l.output_gpu, 1);
    ਪੂਰਨ b, n;
    ਲਈ (b = 0; b < l.batch; ++b){
        ਲਈ(n = 0; n < l.n; ++n){
            ਪੂਰਨ ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + ਸੂਚਕਾਂਕ, 2*l.w*l.h, LOGISTIC);
            ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, n*l.w*l.h, 4);
            activate_array_gpu(l.output_gpu + ਸੂਚਕਾਂਕ, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
    ਜੇ(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        ਵਾਪਸ;
    }

    cuda_pull_array(l.output_gpu, net.ਨਿਵੇਸ਼, l.batch*l.inputs);
    forward_yolo_layer(l, net);
    cuda_ਧੱਕੋ_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

ਖਾਲੀ backward_yolo_layer_gpu(ਅਚਲ layer l, ਜਾਲ net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#ਜੇ_ਅੰਤ


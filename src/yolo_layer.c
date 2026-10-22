#ಅನ್ತರ್ಭಾವ "yolo_layer.h"
#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "box.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>

layer make_yolo_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಒಟ್ಟು, ಪೂರ್ಣಾಂಕ *ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ classes)
{
    ಪೂರ್ಣಾಂಕ i;
    layer l = {0};
    l.type = YOLO;

    l.n = n;
    l.ಒಟ್ಟು = ಒಟ್ಟು;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = n*(classes + 4 + 1);
    l.ಹೊರಹೋಗು_w = l.w;
    l.ಹೊರಹೋಗು_h = l.h;
    l.ಹೊರಹೋಗು_c = l.c;
    l.classes = classes;
    l.ವೆಚ್ಚ_2 = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.biases = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(ಒಟ್ಟು*2, ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಆದರೆ(ಮುಖವಾಡ) l.ಮುಖವಾಡ = ಮುಖವಾಡ;
    ಇಲ್ಲದಿದ್ದರೆ{
        l.ಮುಖವಾಡ = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ));
        ಗಾಗಿ(i = 0; i < n; ++i){
            l.ಮುಖವಾಡ[i] = i;
        }
    }
    l.bias_updates = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n*2, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.outputs = h*w*n*(classes + 4 + 1);
    l.inputs = l.outputs;
    l.truths = 90*(4 + 1);
    l.delta = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*l.outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.output = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*l.outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಗಾಗಿ(i = 0; i < ಒಟ್ಟು*2; ++i){
        l.biases[i] = .5;
    }

    l.forward = forward_yolo_layer;
    l.backward = backward_yolo_layer;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l.forward_gpu = forward_yolo_layer_gpu;
    l.backward_gpu = backward_yolo_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#ಆದರೆ_ಅಂತ್ಯ

    fprintf(stderr, "yolo\n");
    srand(0);

    ಹಿಂತಿರುಗಿ l;
}

ಶೂನ್ಯ resize_yolo_layer(layer *l, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + 4 + 1);
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*ಗಾತ್ರ(ದಶಮಾಂಶ));
    l->delta = realloc(l->delta, l->batch*l->outputs*ಗಾತ್ರ(ದಶಮಾಂಶ));

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#ಆದರೆ_ಅಂತ್ಯ
}

box get_yolo_box(ದಶಮಾಂಶ *x, ದಶಮಾಂಶ *biases, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ, ಪೂರ್ಣಾಂಕ i, ಪೂರ್ಣಾಂಕ j, ಪೂರ್ಣಾಂಕ lw, ಪೂರ್ಣಾಂಕ lh, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ stride)
{
    box b;
    b.x = (i + x[ಸೂಚ್ಯಂಕ + 0*stride]) / lw;
    b.y = (j + x[ಸೂಚ್ಯಂಕ + 1*stride]) / lh;
    b.w = ಘಾತಾಂಕ(x[ಸೂಚ್ಯಂಕ + 2*stride]) * biases[2*n]   / w;
    b.h = ಘಾತಾಂಕ(x[ಸೂಚ್ಯಂಕ + 3*stride]) * biases[2*n+1] / h;
    ಹಿಂತಿರುಗಿ b;
}

ದಶಮಾಂಶ delta_yolo_box(box truth, ದಶಮಾಂಶ *x, ದಶಮಾಂಶ *biases, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ, ಪೂರ್ಣಾಂಕ i, ಪೂರ್ಣಾಂಕ j, ಪೂರ್ಣಾಂಕ lw, ಪೂರ್ಣಾಂಕ lh, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ದಶಮಾಂಶ *delta, ದಶಮಾಂಶ scale, ಪೂರ್ಣಾಂಕ stride)
{
    box pred = get_yolo_box(x, biases, n, ಸೂಚ್ಯಂಕ, i, j, lw, lh, w, h, stride);
    ದಶಮಾಂಶ iou = box_iou(pred, truth);

    ದಶಮಾಂಶ tx = (truth.x*lw - i);
    ದಶಮಾಂಶ ty = (truth.y*lh - j);
    ದಶಮಾಂಶ tw = ಲಘುಗಣಕ(truth.w*w / biases[2*n]);
    ದಶಮಾಂಶ th = ಲಘುಗಣಕ(truth.h*h / biases[2*n + 1]);

    delta[ಸೂಚ್ಯಂಕ + 0*stride] = scale * (tx - x[ಸೂಚ್ಯಂಕ + 0*stride]);
    delta[ಸೂಚ್ಯಂಕ + 1*stride] = scale * (ty - x[ಸೂಚ್ಯಂಕ + 1*stride]);
    delta[ಸೂಚ್ಯಂಕ + 2*stride] = scale * (tw - x[ಸೂಚ್ಯಂಕ + 2*stride]);
    delta[ಸೂಚ್ಯಂಕ + 3*stride] = scale * (th - x[ಸೂಚ್ಯಂಕ + 3*stride]);
    ಹಿಂತಿರುಗಿ iou;
}


ಶೂನ್ಯ delta_yolo_class(ದಶಮಾಂಶ *output, ದಶಮಾಂಶ *delta, ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ, ಪೂರ್ಣಾಂಕ class, ಪೂರ್ಣಾಂಕ classes, ಪೂರ್ಣಾಂಕ stride, ದಶಮಾಂಶ *avg_cat)
{
    ಪೂರ್ಣಾಂಕ n;
    ಆದರೆ (delta[ಸೂಚ್ಯಂಕ]){
        delta[ಸೂಚ್ಯಂಕ + stride*class] = 1 - output[ಸೂಚ್ಯಂಕ + stride*class];
        ಆದರೆ(avg_cat) *avg_cat += output[ಸೂಚ್ಯಂಕ + stride*class];
        ಹಿಂತಿರುಗಿ;
    }
    ಗಾಗಿ(n = 0; n < classes; ++n){
        delta[ಸೂಚ್ಯಂಕ + stride*n] = ((n == class)?1 : 0) - output[ಸೂಚ್ಯಂಕ + stride*n];
        ಆದರೆ(n == class && avg_cat) *avg_cat += output[ಸೂಚ್ಯಂಕ + stride*n];
    }
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ನಮೂದು_ಸೂಚ್ಯಂಕ(layer l, ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ location, ಪೂರ್ಣಾಂಕ ನಮೂದು)
{
    ಪೂರ್ಣಾಂಕ n =   location / (l.w*l.h);
    ಪೂರ್ಣಾಂಕ loc = location % (l.w*l.h);
    ಹಿಂತಿರುಗಿ batch*l.outputs + n*l.w*l.h*(4+l.classes+1) + ನಮೂದು*l.w*l.h + loc;
}

ಶೂನ್ಯ forward_yolo_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ i,j,b,t,n;
    ಸ್ಮರಣೆ_ನಕಲು(l.output, net.ಒಳಹರಿವು, l.outputs*l.batch*ಗಾತ್ರ(ದಶಮಾಂಶ));

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GPU
    ಗಾಗಿ (b = 0; b < l.batch; ++b){
        ಗಾಗಿ(n = 0; n < l.n; ++n){
            ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, n*l.w*l.h, 0);
            activate_array(l.output + ಸೂಚ್ಯಂಕ, 2*l.w*l.h, LOGISTIC);
            ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, n*l.w*l.h, 4);
            activate_array(l.output + ಸೂಚ್ಯಂಕ, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
#ಆದರೆ_ಅಂತ್ಯ

    memset(l.delta, 0, l.outputs * l.batch * ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಆದರೆ(!net.train) ಹಿಂತಿರುಗಿ;
    ದಶಮಾಂಶ avg_iou = 0;
    ದಶಮಾಂಶ recall = 0;
    ದಶಮಾಂಶ recall75 = 0;
    ದಶಮಾಂಶ avg_cat = 0;
    ದಶಮಾಂಶ avg_obj = 0;
    ದಶಮಾಂಶ avg_anyobj = 0;
    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
    ಪೂರ್ಣಾಂಕ class_ಎಣಿಕೆ = 0;
    *(l.ವೆಚ್ಚ_2) = 0;
    ಗಾಗಿ (b = 0; b < l.batch; ++b) {
        ಗಾಗಿ (j = 0; j < l.h; ++j) {
            ಗಾಗಿ (i = 0; i < l.w; ++i) {
                ಗಾಗಿ (n = 0; n < l.n; ++n) {
                    ಪೂರ್ಣಾಂಕ box_ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_yolo_box(l.output, l.biases, l.ಮುಖವಾಡ[n], box_ಸೂಚ್ಯಂಕ, i, j, l.w, l.h, net.w, net.h, l.w*l.h);
                    ದಶಮಾಂಶ ಉತ್ತಮ_iou = 0;
                    ಪೂರ್ಣಾಂಕ ಉತ್ತಮ_t = 0;
                    ಗಾಗಿ(t = 0; t < l.max_boxes; ++t){
                        box truth = float_ಗಮ್ಯ_box(net.truth + t*(4 + 1) + b*l.truths, 1);
                        ಆದರೆ(!truth.x) ನಿಲ್ಲಿಸು;
                        ದಶಮಾಂಶ iou = box_iou(pred, truth);
                        ಆದರೆ (iou > ಉತ್ತಮ_iou) {
                            ಉತ್ತಮ_iou = iou;
                            ಉತ್ತಮ_t = t;
                        }
                    }
                    ಪೂರ್ಣಾಂಕ obj_ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, n*l.w*l.h + j*l.w + i, 4);
                    avg_anyobj += l.output[obj_ಸೂಚ್ಯಂಕ];
                    l.delta[obj_ಸೂಚ್ಯಂಕ] = 0 - l.output[obj_ಸೂಚ್ಯಂಕ];
                    ಆದರೆ (ಉತ್ತಮ_iou > l.ignore_thresh) {
                        l.delta[obj_ಸೂಚ್ಯಂಕ] = 0;
                    }
                    ಆದರೆ (ಉತ್ತಮ_iou > l.truth_thresh) {
                        l.delta[obj_ಸೂಚ್ಯಂಕ] = 1 - l.output[obj_ಸೂಚ್ಯಂಕ];

                        ಪೂರ್ಣಾಂಕ class = net.truth[ಉತ್ತಮ_t*(4 + 1) + b*l.truths + 4];
                        ಆದರೆ (l.map) class = l.map[class];
                        ಪೂರ್ಣಾಂಕ class_ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, n*l.w*l.h + j*l.w + i, 4 + 1);
                        delta_yolo_class(l.output, l.delta, class_ಸೂಚ್ಯಂಕ, class, l.classes, l.w*l.h, 0);
                        box truth = float_ಗಮ್ಯ_box(net.truth + ಉತ್ತಮ_t*(4 + 1) + b*l.truths, 1);
                        delta_yolo_box(truth, l.output, l.biases, l.ಮುಖವಾಡ[n], box_ಸೂಚ್ಯಂಕ, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);
                    }
                }
            }
        }
        ಗಾಗಿ(t = 0; t < l.max_boxes; ++t){
            box truth = float_ಗಮ್ಯ_box(net.truth + t*(4 + 1) + b*l.truths, 1);

            ಆದರೆ(!truth.x) ನಿಲ್ಲಿಸು;
            ದಶಮಾಂಶ ಉತ್ತಮ_iou = 0;
            ಪೂರ್ಣಾಂಕ ಉತ್ತಮ_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_ಸರಿಸು = truth;
            truth_ಸರಿಸು.x = truth_ಸರಿಸು.y = 0;
            ಗಾಗಿ(n = 0; n < l.ಒಟ್ಟು; ++n){
                box pred = {0};
                pred.w = l.biases[2*n]/net.w;
                pred.h = l.biases[2*n+1]/net.h;
                ದಶಮಾಂಶ iou = box_iou(pred, truth_ಸರಿಸು);
                ಆದರೆ (iou > ಉತ್ತಮ_iou){
                    ಉತ್ತಮ_iou = iou;
                    ಉತ್ತಮ_n = n;
                }
            }

            ಪೂರ್ಣಾಂಕ ಮುಖವಾಡ_n = int_ಸೂಚ್ಯಂಕ(l.ಮುಖವಾಡ, ಉತ್ತಮ_n, l.n);
            ಆದರೆ(ಮುಖವಾಡ_n >= 0){
                ಪೂರ್ಣಾಂಕ box_ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, ಮುಖವಾಡ_n*l.w*l.h + j*l.w + i, 0);
                ದಶಮಾಂಶ iou = delta_yolo_box(truth, l.output, l.biases, ಉತ್ತಮ_n, box_ಸೂಚ್ಯಂಕ, i, j, l.w, l.h, net.w, net.h, l.delta, (2-truth.w*truth.h), l.w*l.h);

                ಪೂರ್ಣಾಂಕ obj_ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, ಮುಖವಾಡ_n*l.w*l.h + j*l.w + i, 4);
                avg_obj += l.output[obj_ಸೂಚ್ಯಂಕ];
                l.delta[obj_ಸೂಚ್ಯಂಕ] = 1 - l.output[obj_ಸೂಚ್ಯಂಕ];

                ಪೂರ್ಣಾಂಕ class = net.truth[t*(4 + 1) + b*l.truths + 4];
                ಆದರೆ (l.map) class = l.map[class];
                ಪೂರ್ಣಾಂಕ class_ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, ಮುಖವಾಡ_n*l.w*l.h + j*l.w + i, 4 + 1);
                delta_yolo_class(l.output, l.delta, class_ಸೂಚ್ಯಂಕ, class, l.classes, l.w*l.h, &avg_cat);

                ++ಎಣಿಕೆ_2;
                ++class_ಎಣಿಕೆ;
                ಆದರೆ(iou > .5) recall += 1;
                ಆದರೆ(iou > .75) recall75 += 1;
                avg_iou += iou;
            }
        }
    }
    *(l.ವೆಚ್ಚ_2) = ಘಾತ(mag_array(l.delta, l.outputs * l.batch), 2);
    ಬರೆ("Region %d Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, .5R: %f, .75R: %f,  count: %d\n", net.ಸೂಚ್ಯಂಕ, avg_iou/ಎಣಿಕೆ_2, avg_cat/class_ಎಣಿಕೆ, avg_obj/ಎಣಿಕೆ_2, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/ಎಣಿಕೆ_2, recall75/ಎಣಿಕೆ_2, ಎಣಿಕೆ_2);
}

ಶೂನ್ಯ backward_yolo_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{
   axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

ಶೂನ್ಯ correct_yolo_boxes(detection *dets, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ netw, ಪೂರ್ಣಾಂಕ neth, ಪೂರ್ಣಾಂಕ relative)
{
    ಪೂರ್ಣಾಂಕ i;
    ಪೂರ್ಣಾಂಕ new_w=0;
    ಪೂರ್ಣಾಂಕ new_h=0;
    ಆದರೆ (((ದಶಮಾಂಶ)netw/w) < ((ದಶಮಾಂಶ)neth/h)) {
        new_w = netw;
        new_h = (h * netw)/w;
    } ಇಲ್ಲದಿದ್ದರೆ {
        new_h = neth;
        new_w = (w * neth)/h;
    }
    ಗಾಗಿ (i = 0; i < n; ++i){
        box b = dets[i].bbox;
        b.x =  (b.x - (netw - new_w)/2./netw) / ((ದಶಮಾಂಶ)new_w/netw); 
        b.y =  (b.y - (neth - new_h)/2./neth) / ((ದಶಮಾಂಶ)new_h/neth); 
        b.w *= (ದಶಮಾಂಶ)netw/new_w;
        b.h *= (ದಶಮಾಂಶ)neth/new_h;
        ಆದರೆ(!relative){
            b.x *= w;
            b.w *= w;
            b.y *= h;
            b.h *= h;
        }
        dets[i].bbox = b;
    }
}

ಪೂರ್ಣಾಂಕ yolo_ಅಂಕೆ_detections(layer l, ದಶಮಾಂಶ thresh)
{
    ಪೂರ್ಣಾಂಕ i, n;
    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
    ಗಾಗಿ (i = 0; i < l.w*l.h; ++i){
        ಗಾಗಿ(n = 0; n < l.n; ++n){
            ಪೂರ್ಣಾಂಕ obj_ಸೂಚ್ಯಂಕ  = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, 0, n*l.w*l.h + i, 4);
            ಆದರೆ(l.output[obj_ಸೂಚ್ಯಂಕ] > thresh){
                ++ಎಣಿಕೆ_2;
            }
        }
    }
    ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_2;
}

ಶೂನ್ಯ avg_flipped_yolo(layer l)
{
    ಪೂರ್ಣಾಂಕ i,j,n,z;
    ದಶಮಾಂಶ *flip = l.output + l.outputs;
    ಗಾಗಿ (j = 0; j < l.h; ++j) {
        ಗಾಗಿ (i = 0; i < l.w/2; ++i) {
            ಗಾಗಿ (n = 0; n < l.n; ++n) {
                ಗಾಗಿ(z = 0; z < l.classes + 4 + 1; ++z){
                    ಪೂರ್ಣಾಂಕ i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                    ಪೂರ್ಣಾಂಕ i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1);
                    ದಶಮಾಂಶ ಅದಲುಬದಲು = flip[i1];
                    flip[i1] = flip[i2];
                    flip[i2] = ಅದಲುಬದಲು;
                    ಆದರೆ(z == 0){
                        flip[i1] = -flip[i1];
                        flip[i2] = -flip[i2];
                    }
                }
            }
        }
    }
    ಗಾಗಿ(i = 0; i < l.outputs; ++i){
        l.output[i] = (l.output[i] + flip[i])/2.;
    }
}

ಪೂರ್ಣಾಂಕ get_yolo_detections(layer l, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ netw, ಪೂರ್ಣಾಂಕ neth, ದಶಮಾಂಶ thresh, ಪೂರ್ಣಾಂಕ *map, ಪೂರ್ಣಾಂಕ relative, detection *dets)
{
    ಪೂರ್ಣಾಂಕ i,j,n;
    ದಶಮಾಂಶ *predictions = l.output;
    ಆದರೆ (l.batch == 2) avg_flipped_yolo(l);
    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
    ಗಾಗಿ (i = 0; i < l.w*l.h; ++i){
        ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲು = i / l.w;
        ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು = i % l.w;
        ಗಾಗಿ(n = 0; n < l.n; ++n){
            ಪೂರ್ಣಾಂಕ obj_ಸೂಚ್ಯಂಕ  = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, 0, n*l.w*l.h + i, 4);
            ದಶಮಾಂಶ objectness = predictions[obj_ಸೂಚ್ಯಂಕ];
            ಆದರೆ(objectness <= thresh) ಮುಂದುವರಿ;
            ಪೂರ್ಣಾಂಕ box_ಸೂಚ್ಯಂಕ  = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, 0, n*l.w*l.h + i, 0);
            dets[ಎಣಿಕೆ_2].bbox = get_yolo_box(predictions, l.biases, l.ಮುಖವಾಡ[n], box_ಸೂಚ್ಯಂಕ, ಲಂಬಸಾಲು, ಅಡ್ಡಸಾಲು, l.w, l.h, netw, neth, l.w*l.h);
            dets[ಎಣಿಕೆ_2].objectness = objectness;
            dets[ಎಣಿಕೆ_2].classes = l.classes;
            ಗಾಗಿ(j = 0; j < l.classes; ++j){
                ಪೂರ್ಣಾಂಕ class_ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, 0, n*l.w*l.h + i, 4 + 1 + j);
                ದಶಮಾಂಶ prob = objectness*predictions[class_ಸೂಚ್ಯಂಕ];
                dets[ಎಣಿಕೆ_2].prob[j] = (prob > thresh) ? prob : 0;
            }
            ++ಎಣಿಕೆ_2;
        }
    }
    correct_yolo_boxes(dets, ಎಣಿಕೆ_2, w, h, netw, neth, relative);
    ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_2;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU

ಶೂನ್ಯ forward_yolo_layer_gpu(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{
    copy_gpu(l.batch*l.inputs, net.ಒಳಹರಿವು_gpu, 1, l.output_gpu, 1);
    ಪೂರ್ಣಾಂಕ b, n;
    ಗಾಗಿ (b = 0; b < l.batch; ++b){
        ಗಾಗಿ(n = 0; n < l.n; ++n){
            ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + ಸೂಚ್ಯಂಕ, 2*l.w*l.h, LOGISTIC);
            ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, n*l.w*l.h, 4);
            activate_array_gpu(l.output_gpu + ಸೂಚ್ಯಂಕ, (1+l.classes)*l.w*l.h, LOGISTIC);
        }
    }
    ಆದರೆ(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        ಹಿಂತಿರುಗಿ;
    }

    cuda_pull_array(l.output_gpu, net.ಒಳಹರಿವು, l.batch*l.inputs);
    forward_yolo_layer(l, net);
    cuda_ತಳ್ಳು_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

ಶೂನ್ಯ backward_yolo_layer_gpu(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#ಆದರೆ_ಅಂತ್ಯ


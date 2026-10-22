#ಅನ್ತರ್ಭಾವ "region_layer.h"
#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "box.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>

layer make_region_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ classes, ಪೂರ್ಣಾಂಕ coords)
{
    layer l = {0};
    l.type = REGION;

    l.n = n;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = n*(classes + coords + 1);
    l.ಹೊರಹೋಗು_w = l.w;
    l.ಹೊರಹೋಗು_h = l.h;
    l.ಹೊರಹೋಗು_c = l.c;
    l.classes = classes;
    l.coords = coords;
    l.ವೆಚ್ಚ_2 = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.biases = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n*2, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.bias_updates = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n*2, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.outputs = h*w*n*(classes + coords + 1);
    l.inputs = l.outputs;
    l.truths = 30*(l.coords + 1);
    l.delta = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*l.outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.output = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*l.outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < n*2; ++i){
        l.biases[i] = .5;
    }

    l.forward = forward_region_layer;
    l.backward = backward_region_layer;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l.forward_gpu = forward_region_layer_gpu;
    l.backward_gpu = backward_region_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#ಆದರೆ_ಅಂತ್ಯ

    fprintf(stderr, "detection\n");
    srand(0);

    ಹಿಂತಿರುಗಿ l;
}

ಶೂನ್ಯ resize_region_layer(layer *l, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + l->coords + 1);
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

box get_region_box(ದಶಮಾಂಶ *x, ದಶಮಾಂಶ *biases, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ, ಪೂರ್ಣಾಂಕ i, ಪೂರ್ಣಾಂಕ j, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ stride)
{
    box b;
    b.x = (i + x[ಸೂಚ್ಯಂಕ + 0*stride]) / w;
    b.y = (j + x[ಸೂಚ್ಯಂಕ + 1*stride]) / h;
    b.w = ಘಾತಾಂಕ(x[ಸೂಚ್ಯಂಕ + 2*stride]) * biases[2*n]   / w;
    b.h = ಘಾತಾಂಕ(x[ಸೂಚ್ಯಂಕ + 3*stride]) * biases[2*n+1] / h;
    ಹಿಂತಿರುಗಿ b;
}

ದಶಮಾಂಶ delta_region_box(box truth, ದಶಮಾಂಶ *x, ದಶಮಾಂಶ *biases, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ, ಪೂರ್ಣಾಂಕ i, ಪೂರ್ಣಾಂಕ j, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ದಶಮಾಂಶ *delta, ದಶಮಾಂಶ scale, ಪೂರ್ಣಾಂಕ stride)
{
    box pred = get_region_box(x, biases, n, ಸೂಚ್ಯಂಕ, i, j, w, h, stride);
    ದಶಮಾಂಶ iou = box_iou(pred, truth);

    ದಶಮಾಂಶ tx = (truth.x*w - i);
    ದಶಮಾಂಶ ty = (truth.y*h - j);
    ದಶಮಾಂಶ tw = ಲಘುಗಣಕ(truth.w*w / biases[2*n]);
    ದಶಮಾಂಶ th = ಲಘುಗಣಕ(truth.h*h / biases[2*n + 1]);

    delta[ಸೂಚ್ಯಂಕ + 0*stride] = scale * (tx - x[ಸೂಚ್ಯಂಕ + 0*stride]);
    delta[ಸೂಚ್ಯಂಕ + 1*stride] = scale * (ty - x[ಸೂಚ್ಯಂಕ + 1*stride]);
    delta[ಸೂಚ್ಯಂಕ + 2*stride] = scale * (tw - x[ಸೂಚ್ಯಂಕ + 2*stride]);
    delta[ಸೂಚ್ಯಂಕ + 3*stride] = scale * (th - x[ಸೂಚ್ಯಂಕ + 3*stride]);
    ಹಿಂತಿರುಗಿ iou;
}

ಶೂನ್ಯ delta_region_ಮುಖವಾಡ(ದಶಮಾಂಶ *truth, ದಶಮಾಂಶ *x, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ, ದಶಮಾಂಶ *delta, ಪೂರ್ಣಾಂಕ stride, ಪೂರ್ಣಾಂಕ scale)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < n; ++i){
        delta[ಸೂಚ್ಯಂಕ + i*stride] = scale*(truth[i] - x[ಸೂಚ್ಯಂಕ + i*stride]);
    }
}


ಶೂನ್ಯ delta_region_class(ದಶಮಾಂಶ *output, ದಶಮಾಂಶ *delta, ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ, ಪೂರ್ಣಾಂಕ class, ಪೂರ್ಣಾಂಕ classes, ವೃಕ್ಷ *hier, ದಶಮಾಂಶ scale, ಪೂರ್ಣಾಂಕ stride, ದಶಮಾಂಶ *avg_cat, ಪೂರ್ಣಾಂಕ ಟ್ಯಾಗ್)
{
    ಪೂರ್ಣಾಂಕ i, n;
    ಆದರೆ(hier){
        ದಶಮಾಂಶ pred = 1;
        ಆಗ(class >= 0){
            pred *= output[ಸೂಚ್ಯಂಕ + stride*class];
            ಪೂರ್ಣಾಂಕ g = hier->ಗುಂಪು[class];
            ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ = hier->ಗುಂಪು_ಸ್ಥಾನಾಂತರ[g];
            ಗಾಗಿ(i = 0; i < hier->ಗುಂಪು_ಗಾತ್ರ[g]; ++i){
                delta[ಸೂಚ್ಯಂಕ + stride*(ಸ್ಥಾನಾಂತರ + i)] = scale * (0 - output[ಸೂಚ್ಯಂಕ + stride*(ಸ್ಥಾನಾಂತರ + i)]);
            }
            delta[ಸೂಚ್ಯಂಕ + stride*class] = scale * (1 - output[ಸೂಚ್ಯಂಕ + stride*class]);

            class = hier->ಪೋಷಕ[class];
        }
        *avg_cat += pred;
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಆದರೆ (delta[ಸೂಚ್ಯಂಕ] && ಟ್ಯಾಗ್){
            delta[ಸೂಚ್ಯಂಕ + stride*class] = scale * (1 - output[ಸೂಚ್ಯಂಕ + stride*class]);
            ಹಿಂತಿರುಗಿ;
        }
        ಗಾಗಿ(n = 0; n < classes; ++n){
            delta[ಸೂಚ್ಯಂಕ + stride*n] = scale * (((n == class)?1 : 0) - output[ಸೂಚ್ಯಂಕ + stride*n]);
            ಆದರೆ(n == class) *avg_cat += output[ಸೂಚ್ಯಂಕ + stride*n];
        }
    }
}

ದಶಮಾಂಶ logit(ದಶಮಾಂಶ x)
{
    ಹಿಂತಿರುಗಿ ಲಘುಗಣಕ(x/(1.-x));
}

ದಶಮಾಂಶ tisnan(ದಶಮಾಂಶ x)
{
    ಹಿಂತಿರುಗಿ (x != x);
}

ಪೂರ್ಣಾಂಕ ನಮೂದು_ಸೂಚ್ಯಂಕ(layer l, ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ location, ಪೂರ್ಣಾಂಕ ನಮೂದು)
{
    ಪೂರ್ಣಾಂಕ n =   location / (l.w*l.h);
    ಪೂರ್ಣಾಂಕ loc = location % (l.w*l.h);
    ಹಿಂತಿರುಗಿ batch*l.outputs + n*l.w*l.h*(l.coords+l.classes+1) + ನಮೂದು*l.w*l.h + loc;
}

ಶೂನ್ಯ forward_region_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ i,j,b,t,n;
    ಸ್ಮರಣೆ_ನಕಲು(l.output, net.ಒಳಹರಿವು, l.outputs*l.batch*ಗಾತ್ರ(ದಶಮಾಂಶ));

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GPU
    ಗಾಗಿ (b = 0; b < l.batch; ++b){
        ಗಾಗಿ(n = 0; n < l.n; ++n){
            ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, n*l.w*l.h, 0);
            activate_array(l.output + ಸೂಚ್ಯಂಕ, 2*l.w*l.h, LOGISTIC);
            ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, n*l.w*l.h, l.coords);
            ಆದರೆ(!l.background) activate_array(l.output + ಸೂಚ್ಯಂಕ,   l.w*l.h, LOGISTIC);
            ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, n*l.w*l.h, l.coords + 1);
            ಆದರೆ(!l.softmax && !l.softmax_ವೃಕ್ಷ) activate_array(l.output + ಸೂಚ್ಯಂಕ, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    ಆದರೆ (l.softmax_ವೃಕ್ಷ){
        ಪೂರ್ಣಾಂಕ i;
        ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = l.coords + 1;
        ಗಾಗಿ (i = 0; i < l.softmax_ವೃಕ್ಷ->groups; ++i) {
            ಪೂರ್ಣಾಂಕ ಗುಂಪು_ಗಾತ್ರ = l.softmax_ವೃಕ್ಷ->ಗುಂಪು_ಗಾತ್ರ[i];
            softmax_cpu(net.ಒಳಹರಿವು + ಎಣಿಕೆ_2, ಗುಂಪು_ಗಾತ್ರ, l.batch, l.inputs, l.n*l.w*l.h, 1, l.n*l.w*l.h, l.temperature, l.output + ಎಣಿಕೆ_2);
            ಎಣಿಕೆ_2 += ಗುಂಪು_ಗಾತ್ರ;
        }
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (l.softmax){
        ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, 0, 0, l.coords + !l.background);
        softmax_cpu(net.ಒಳಹರಿವು + ಸೂಚ್ಯಂಕ, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output + ಸೂಚ್ಯಂಕ);
    }
#ಆದರೆ_ಅಂತ್ಯ

    memset(l.delta, 0, l.outputs * l.batch * ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಆದರೆ(!net.train) ಹಿಂತಿರುಗಿ;
    ದಶಮಾಂಶ avg_iou = 0;
    ದಶಮಾಂಶ recall = 0;
    ದಶಮಾಂಶ avg_cat = 0;
    ದಶಮಾಂಶ avg_obj = 0;
    ದಶಮಾಂಶ avg_anyobj = 0;
    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
    ಪೂರ್ಣಾಂಕ class_ಎಣಿಕೆ = 0;
    *(l.ವೆಚ್ಚ_2) = 0;
    ಗಾಗಿ (b = 0; b < l.batch; ++b) {
        ಆದರೆ(l.softmax_ವೃಕ್ಷ){
            ಪೂರ್ಣಾಂಕ onlyclass = 0;
            ಗಾಗಿ(t = 0; t < 30; ++t){
                box truth = float_ಗಮ್ಯ_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                ಆದರೆ(!truth.x) ನಿಲ್ಲಿಸು;
                ಪೂರ್ಣಾಂಕ class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
                ದಶಮಾಂಶ maxp = 0;
                ಪೂರ್ಣಾಂಕ maxi = 0;
                ಆದರೆ(truth.x > 100000 && truth.y > 100000){
                    ಗಾಗಿ(n = 0; n < l.n*l.w*l.h; ++n){
                        ಪೂರ್ಣಾಂಕ class_ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, n, l.coords + 1);
                        ಪೂರ್ಣಾಂಕ obj_ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, n, l.coords);
                        ದಶಮಾಂಶ scale =  l.output[obj_ಸೂಚ್ಯಂಕ];
                        l.delta[obj_ಸೂಚ್ಯಂಕ] = l.noobject_scale * (0 - l.output[obj_ಸೂಚ್ಯಂಕ]);
                        ದಶಮಾಂಶ p = scale*get_hierarchy_probability(l.output + class_ಸೂಚ್ಯಂಕ, l.softmax_ವೃಕ್ಷ, class, l.w*l.h);
                        ಆದರೆ(p > maxp){
                            maxp = p;
                            maxi = n;
                        }
                    }
                    ಪೂರ್ಣಾಂಕ class_ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, maxi, l.coords + 1);
                    ಪೂರ್ಣಾಂಕ obj_ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, maxi, l.coords);
                    delta_region_class(l.output, l.delta, class_ಸೂಚ್ಯಂಕ, class, l.classes, l.softmax_ವೃಕ್ಷ, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
                    ಆದರೆ(l.output[obj_ಸೂಚ್ಯಂಕ] < .3) l.delta[obj_ಸೂಚ್ಯಂಕ] = l.object_scale * (.3 - l.output[obj_ಸೂಚ್ಯಂಕ]);
                    ಇಲ್ಲದಿದ್ದರೆ  l.delta[obj_ಸೂಚ್ಯಂಕ] = 0;
                    l.delta[obj_ಸೂಚ್ಯಂಕ] = 0;
                    ++class_ಎಣಿಕೆ;
                    onlyclass = 1;
                    ನಿಲ್ಲಿಸು;
                }
            }
            ಆದರೆ(onlyclass) ಮುಂದುವರಿ;
        }
        ಗಾಗಿ (j = 0; j < l.h; ++j) {
            ಗಾಗಿ (i = 0; i < l.w; ++i) {
                ಗಾಗಿ (n = 0; n < l.n; ++n) {
                    ಪೂರ್ಣಾಂಕ box_ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_region_box(l.output, l.biases, n, box_ಸೂಚ್ಯಂಕ, i, j, l.w, l.h, l.w*l.h);
                    ದಶಮಾಂಶ ಉತ್ತಮ_iou = 0;
                    ಗಾಗಿ(t = 0; t < 30; ++t){
                        box truth = float_ಗಮ್ಯ_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                        ಆದರೆ(!truth.x) ನಿಲ್ಲಿಸು;
                        ದಶಮಾಂಶ iou = box_iou(pred, truth);
                        ಆದರೆ (iou > ಉತ್ತಮ_iou) {
                            ಉತ್ತಮ_iou = iou;
                        }
                    }
                    ಪೂರ್ಣಾಂಕ obj_ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, n*l.w*l.h + j*l.w + i, l.coords);
                    avg_anyobj += l.output[obj_ಸೂಚ್ಯಂಕ];
                    l.delta[obj_ಸೂಚ್ಯಂಕ] = l.noobject_scale * (0 - l.output[obj_ಸೂಚ್ಯಂಕ]);
                    ಆದರೆ(l.background) l.delta[obj_ಸೂಚ್ಯಂಕ] = l.noobject_scale * (1 - l.output[obj_ಸೂಚ್ಯಂಕ]);
                    ಆದರೆ (ಉತ್ತಮ_iou > l.thresh) {
                        l.delta[obj_ಸೂಚ್ಯಂಕ] = 0;
                    }

                    ಆದರೆ(*(net.ನೋಡಿದ) < 12800){
                        box truth = {0};
                        truth.x = (i + .5)/l.w;
                        truth.y = (j + .5)/l.h;
                        truth.w = l.biases[2*n]/l.w;
                        truth.h = l.biases[2*n+1]/l.h;
                        delta_region_box(truth, l.output, l.biases, n, box_ಸೂಚ್ಯಂಕ, i, j, l.w, l.h, l.delta, .01, l.w*l.h);
                    }
                }
            }
        }
        ಗಾಗಿ(t = 0; t < 30; ++t){
            box truth = float_ಗಮ್ಯ_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);

            ಆದರೆ(!truth.x) ನಿಲ್ಲಿಸು;
            ದಶಮಾಂಶ ಉತ್ತಮ_iou = 0;
            ಪೂರ್ಣಾಂಕ ಉತ್ತಮ_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_ಸರಿಸು = truth;
            truth_ಸರಿಸು.x = 0;
            truth_ಸರಿಸು.y = 0;
            ಗಾಗಿ(n = 0; n < l.n; ++n){
                ಪೂರ್ಣಾಂಕ box_ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, n*l.w*l.h + j*l.w + i, 0);
                box pred = get_region_box(l.output, l.biases, n, box_ಸೂಚ್ಯಂಕ, i, j, l.w, l.h, l.w*l.h);
                ಆದರೆ(l.bias_ಹೊಂದಿಕೆ){
                    pred.w = l.biases[2*n]/l.w;
                    pred.h = l.biases[2*n+1]/l.h;
                }
                pred.x = 0;
                pred.y = 0;
                ದಶಮಾಂಶ iou = box_iou(pred, truth_ಸರಿಸು);
                ಆದರೆ (iou > ಉತ್ತಮ_iou){
                    ಉತ್ತಮ_iou = iou;
                    ಉತ್ತಮ_n = n;
                }
            }

            ಪೂರ್ಣಾಂಕ box_ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, ಉತ್ತಮ_n*l.w*l.h + j*l.w + i, 0);
            ದಶಮಾಂಶ iou = delta_region_box(truth, l.output, l.biases, ಉತ್ತಮ_n, box_ಸೂಚ್ಯಂಕ, i, j, l.w, l.h, l.delta, l.coord_scale *  (2 - truth.w*truth.h), l.w*l.h);
            ಆದರೆ(l.coords > 4){
                ಪೂರ್ಣಾಂಕ ಮುಖವಾಡ_ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, ಉತ್ತಮ_n*l.w*l.h + j*l.w + i, 4);
                delta_region_ಮುಖವಾಡ(net.truth + t*(l.coords + 1) + b*l.truths + 5, l.output, l.coords - 4, ಮುಖವಾಡ_ಸೂಚ್ಯಂಕ, l.delta, l.w*l.h, l.ಮುಖವಾಡ_scale);
            }
            ಆದರೆ(iou > .5) recall += 1;
            avg_iou += iou;

            ಪೂರ್ಣಾಂಕ obj_ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, ಉತ್ತಮ_n*l.w*l.h + j*l.w + i, l.coords);
            avg_obj += l.output[obj_ಸೂಚ್ಯಂಕ];
            l.delta[obj_ಸೂಚ್ಯಂಕ] = l.object_scale * (1 - l.output[obj_ಸೂಚ್ಯಂಕ]);
            ಆದರೆ (l.rescore) {
                l.delta[obj_ಸೂಚ್ಯಂಕ] = l.object_scale * (iou - l.output[obj_ಸೂಚ್ಯಂಕ]);
            }
            ಆದರೆ(l.background){
                l.delta[obj_ಸೂಚ್ಯಂಕ] = l.object_scale * (0 - l.output[obj_ಸೂಚ್ಯಂಕ]);
            }

            ಪೂರ್ಣಾಂಕ class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
            ಆದರೆ (l.map) class = l.map[class];
            ಪೂರ್ಣಾಂಕ class_ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, ಉತ್ತಮ_n*l.w*l.h + j*l.w + i, l.coords + 1);
            delta_region_class(l.output, l.delta, class_ಸೂಚ್ಯಂಕ, class, l.classes, l.softmax_ವೃಕ್ಷ, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
            ++ಎಣಿಕೆ_2;
            ++class_ಎಣಿಕೆ;
        }
    }
    *(l.ವೆಚ್ಚ_2) = ಘಾತ(mag_array(l.delta, l.outputs * l.batch), 2);
    ಬರೆ("Region Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, Avg Recall: %f,  count: %d\n", avg_iou/ಎಣಿಕೆ_2, avg_cat/class_ಎಣಿಕೆ, avg_obj/ಎಣಿಕೆ_2, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/ಎಣಿಕೆ_2, ಎಣಿಕೆ_2);
}

ಶೂನ್ಯ backward_region_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
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

ಶೂನ್ಯ correct_region_boxes(detection *dets, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ netw, ಪೂರ್ಣಾಂಕ neth, ಪೂರ್ಣಾಂಕ relative)
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

ಶೂನ್ಯ get_region_detections(layer l, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ netw, ಪೂರ್ಣಾಂಕ neth, ದಶಮಾಂಶ thresh, ಪೂರ್ಣಾಂಕ *map, ದಶಮಾಂಶ ವೃಕ್ಷ_thresh, ಪೂರ್ಣಾಂಕ relative, detection *dets)
{
    ಪೂರ್ಣಾಂಕ i,j,n,z;
    ದಶಮಾಂಶ *predictions = l.output;
    ಆದರೆ (l.batch == 2) {
        ದಶಮಾಂಶ *flip = l.output + l.outputs;
        ಗಾಗಿ (j = 0; j < l.h; ++j) {
            ಗಾಗಿ (i = 0; i < l.w/2; ++i) {
                ಗಾಗಿ (n = 0; n < l.n; ++n) {
                    ಗಾಗಿ(z = 0; z < l.classes + l.coords + 1; ++z){
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
    ಗಾಗಿ (i = 0; i < l.w*l.h; ++i){
        ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲು = i / l.w;
        ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು = i % l.w;
        ಗಾಗಿ(n = 0; n < l.n; ++n){
            ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = n*l.w*l.h + i;
            ಗಾಗಿ(j = 0; j < l.classes; ++j){
                dets[ಸೂಚ್ಯಂಕ].prob[j] = 0;
            }
            ಪೂರ್ಣಾಂಕ obj_ಸೂಚ್ಯಂಕ  = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, 0, n*l.w*l.h + i, l.coords);
            ಪೂರ್ಣಾಂಕ box_ಸೂಚ್ಯಂಕ  = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, 0, n*l.w*l.h + i, 0);
            ಪೂರ್ಣಾಂಕ ಮುಖವಾಡ_ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, 0, n*l.w*l.h + i, 4);
            ದಶಮಾಂಶ scale = l.background ? 1 : predictions[obj_ಸೂಚ್ಯಂಕ];
            dets[ಸೂಚ್ಯಂಕ].bbox = get_region_box(predictions, l.biases, n, box_ಸೂಚ್ಯಂಕ, ಲಂಬಸಾಲು, ಅಡ್ಡಸಾಲು, l.w, l.h, l.w*l.h);
            dets[ಸೂಚ್ಯಂಕ].objectness = scale > thresh ? scale : 0;
            ಆದರೆ(dets[ಸೂಚ್ಯಂಕ].ಮುಖವಾಡ){
                ಗಾಗಿ(j = 0; j < l.coords - 4; ++j){
                    dets[ಸೂಚ್ಯಂಕ].ಮುಖವಾಡ[j] = l.output[ಮುಖವಾಡ_ಸೂಚ್ಯಂಕ + j*l.w*l.h];
                }
            }

            ಪೂರ್ಣಾಂಕ class_ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, 0, n*l.w*l.h + i, l.coords + !l.background);
            ಆದರೆ(l.softmax_ವೃಕ್ಷ){

                hierarchy_predictions(predictions + class_ಸೂಚ್ಯಂಕ, l.classes, l.softmax_ವೃಕ್ಷ, 0, l.w*l.h);
                ಆದರೆ(map){
                    ಗಾಗಿ(j = 0; j < 200; ++j){
                        ಪೂರ್ಣಾಂಕ class_ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, 0, n*l.w*l.h + i, l.coords + 1 + map[j]);
                        ದಶಮಾಂಶ prob = scale*predictions[class_ಸೂಚ್ಯಂಕ];
                        dets[ಸೂಚ್ಯಂಕ].prob[j] = (prob > thresh) ? prob : 0;
                    }
                } ಇಲ್ಲದಿದ್ದರೆ {
                    ಪೂರ್ಣಾಂಕ j =  hierarchy_ತುದಿ_prediction(predictions + class_ಸೂಚ್ಯಂಕ, l.softmax_ವೃಕ್ಷ, ವೃಕ್ಷ_thresh, l.w*l.h);
                    dets[ಸೂಚ್ಯಂಕ].prob[j] = (scale > thresh) ? scale : 0;
                }
            } ಇಲ್ಲದಿದ್ದರೆ {
                ಆದರೆ(dets[ಸೂಚ್ಯಂಕ].objectness){
                    ಗಾಗಿ(j = 0; j < l.classes; ++j){
                        ಪೂರ್ಣಾಂಕ class_ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, 0, n*l.w*l.h + i, l.coords + 1 + j);
                        ದಶಮಾಂಶ prob = scale*predictions[class_ಸೂಚ್ಯಂಕ];
                        dets[ಸೂಚ್ಯಂಕ].prob[j] = (prob > thresh) ? prob : 0;
                    }
                }
            }
        }
    }
    correct_region_boxes(dets, l.w*l.h*l.n, w, h, netw, neth, relative);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU

ಶೂನ್ಯ forward_region_layer_gpu(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{
    copy_gpu(l.batch*l.inputs, net.ಒಳಹರಿವು_gpu, 1, l.output_gpu, 1);
    ಪೂರ್ಣಾಂಕ b, n;
    ಗಾಗಿ (b = 0; b < l.batch; ++b){
        ಗಾಗಿ(n = 0; n < l.n; ++n){
            ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + ಸೂಚ್ಯಂಕ, 2*l.w*l.h, LOGISTIC);
            ಆದರೆ(l.coords > 4){
                ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, n*l.w*l.h, 4);
                activate_array_gpu(l.output_gpu + ಸೂಚ್ಯಂಕ, (l.coords - 4)*l.w*l.h, LOGISTIC);
            }
            ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, n*l.w*l.h, l.coords);
            ಆದರೆ(!l.background) activate_array_gpu(l.output_gpu + ಸೂಚ್ಯಂಕ,   l.w*l.h, LOGISTIC);
            ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, n*l.w*l.h, l.coords + 1);
            ಆದರೆ(!l.softmax && !l.softmax_ವೃಕ್ಷ) activate_array_gpu(l.output_gpu + ಸೂಚ್ಯಂಕ, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    ಆದರೆ (l.softmax_ವೃಕ್ಷ){
        ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, 0, 0, l.coords + 1);
        softmax_ವೃಕ್ಷ(net.ಒಳಹರಿವು_gpu + ಸೂಚ್ಯಂಕ, l.w*l.h, l.batch*l.n, l.inputs/l.n, 1, l.output_gpu + ಸೂಚ್ಯಂಕ, *l.softmax_ವೃಕ್ಷ);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (l.softmax) {
        ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, 0, 0, l.coords + !l.background);
        softmax_gpu(net.ಒಳಹರಿವು_gpu + ಸೂಚ್ಯಂಕ, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu + ಸೂಚ್ಯಂಕ);
    }
    ಆದರೆ(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        ಹಿಂತಿರುಗಿ;
    }

    cuda_pull_array(l.output_gpu, net.ಒಳಹರಿವು, l.batch*l.inputs);
    forward_region_layer(l, net);
    //cuda_push_array(l.output_gpu, l.output, l.batch*l.outputs);
    ಆದರೆ(!net.train) ಹಿಂತಿರುಗಿ;
    cuda_ತಳ್ಳು_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

ಶೂನ್ಯ backward_region_layer_gpu(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ b, n;
    ಗಾಗಿ (b = 0; b < l.batch; ++b){
        ಗಾಗಿ(n = 0; n < l.n; ++n){
            ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, n*l.w*l.h, 0);
            gradient_array_gpu(l.output_gpu + ಸೂಚ್ಯಂಕ, 2*l.w*l.h, LOGISTIC, l.delta_gpu + ಸೂಚ್ಯಂಕ);
            ಆದರೆ(l.coords > 4){
                ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, n*l.w*l.h, 4);
                gradient_array_gpu(l.output_gpu + ಸೂಚ್ಯಂಕ, (l.coords - 4)*l.w*l.h, LOGISTIC, l.delta_gpu + ಸೂಚ್ಯಂಕ);
            }
            ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, b, n*l.w*l.h, l.coords);
            ಆದರೆ(!l.background) gradient_array_gpu(l.output_gpu + ಸೂಚ್ಯಂಕ,   l.w*l.h, LOGISTIC, l.delta_gpu + ಸೂಚ್ಯಂಕ);
        }
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ zero_objectness(layer l)
{
    ಪೂರ್ಣಾಂಕ i, n;
    ಗಾಗಿ (i = 0; i < l.w*l.h; ++i){
        ಗಾಗಿ(n = 0; n < l.n; ++n){
            ಪೂರ್ಣಾಂಕ obj_ಸೂಚ್ಯಂಕ = ನಮೂದು_ಸೂಚ್ಯಂಕ(l, 0, n*l.w*l.h + i, l.coords);
            l.output[obj_ಸೂಚ್ಯಂಕ] = 0;
        }
    }
}


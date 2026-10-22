#ਅਨ੍ਤਰ੍ਭਾਵ "region_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "activations.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "box.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>

layer make_region_layer(ਪੂਰਨ batch, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ n, ਪੂਰਨ classes, ਪੂਰਨ coords)
{
    layer l = {0};
    l.type = REGION;

    l.n = n;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = n*(classes + coords + 1);
    l.ਬਾਹਰਲਾ_w = l.w;
    l.ਬਾਹਰਲਾ_h = l.h;
    l.ਬਾਹਰਲਾ_c = l.c;
    l.classes = classes;
    l.coords = coords;
    l.ਲਾਗਤ_2 = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.biases = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n*2, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.bias_updates = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n*2, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.outputs = h*w*n*(classes + coords + 1);
    l.inputs = l.outputs;
    l.truths = 30*(l.coords + 1);
    l.delta = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*l.outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.output = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*l.outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < n*2; ++i){
        l.biases[i] = .5;
    }

    l.forward = forward_region_layer;
    l.backward = backward_region_layer;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_region_layer_gpu;
    l.backward_gpu = backward_region_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#ਜੇ_ਅੰਤ

    fprintf(stderr, "detection\n");
    srand(0);

    ਵਾਪਸ l;
}

ਖਾਲੀ resize_region_layer(layer *l, ਪੂਰਨ w, ਪੂਰਨ h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->n*(l->classes + l->coords + 1);
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

box get_region_box(ਦਸ਼ਮਲਵ *x, ਦਸ਼ਮਲਵ *biases, ਪੂਰਨ n, ਪੂਰਨ ਸੂਚਕਾਂਕ, ਪੂਰਨ i, ਪੂਰਨ j, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ stride)
{
    box b;
    b.x = (i + x[ਸੂਚਕਾਂਕ + 0*stride]) / w;
    b.y = (j + x[ਸੂਚਕਾਂਕ + 1*stride]) / h;
    b.w = ਘਾਤਾਂਕ(x[ਸੂਚਕਾਂਕ + 2*stride]) * biases[2*n]   / w;
    b.h = ਘਾਤਾਂਕ(x[ਸੂਚਕਾਂਕ + 3*stride]) * biases[2*n+1] / h;
    ਵਾਪਸ b;
}

ਦਸ਼ਮਲਵ delta_region_box(box truth, ਦਸ਼ਮਲਵ *x, ਦਸ਼ਮਲਵ *biases, ਪੂਰਨ n, ਪੂਰਨ ਸੂਚਕਾਂਕ, ਪੂਰਨ i, ਪੂਰਨ j, ਪੂਰਨ w, ਪੂਰਨ h, ਦਸ਼ਮਲਵ *delta, ਦਸ਼ਮਲਵ scale, ਪੂਰਨ stride)
{
    box pred = get_region_box(x, biases, n, ਸੂਚਕਾਂਕ, i, j, w, h, stride);
    ਦਸ਼ਮਲਵ iou = box_iou(pred, truth);

    ਦਸ਼ਮਲਵ tx = (truth.x*w - i);
    ਦਸ਼ਮਲਵ ty = (truth.y*h - j);
    ਦਸ਼ਮਲਵ tw = ਲਘੂਗਣਕ(truth.w*w / biases[2*n]);
    ਦਸ਼ਮਲਵ th = ਲਘੂਗਣਕ(truth.h*h / biases[2*n + 1]);

    delta[ਸੂਚਕਾਂਕ + 0*stride] = scale * (tx - x[ਸੂਚਕਾਂਕ + 0*stride]);
    delta[ਸੂਚਕਾਂਕ + 1*stride] = scale * (ty - x[ਸੂਚਕਾਂਕ + 1*stride]);
    delta[ਸੂਚਕਾਂਕ + 2*stride] = scale * (tw - x[ਸੂਚਕਾਂਕ + 2*stride]);
    delta[ਸੂਚਕਾਂਕ + 3*stride] = scale * (th - x[ਸੂਚਕਾਂਕ + 3*stride]);
    ਵਾਪਸ iou;
}

ਖਾਲੀ delta_region_ਨਕਾਬ(ਦਸ਼ਮਲਵ *truth, ਦਸ਼ਮਲਵ *x, ਪੂਰਨ n, ਪੂਰਨ ਸੂਚਕਾਂਕ, ਦਸ਼ਮਲਵ *delta, ਪੂਰਨ stride, ਪੂਰਨ scale)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < n; ++i){
        delta[ਸੂਚਕਾਂਕ + i*stride] = scale*(truth[i] - x[ਸੂਚਕਾਂਕ + i*stride]);
    }
}


ਖਾਲੀ delta_region_class(ਦਸ਼ਮਲਵ *output, ਦਸ਼ਮਲਵ *delta, ਪੂਰਨ ਸੂਚਕਾਂਕ, ਪੂਰਨ class, ਪੂਰਨ classes, ਰੁੱਖ *hier, ਦਸ਼ਮਲਵ scale, ਪੂਰਨ stride, ਦਸ਼ਮਲਵ *avg_cat, ਪੂਰਨ ਟੈਗ)
{
    ਪੂਰਨ i, n;
    ਜੇ(hier){
        ਦਸ਼ਮਲਵ pred = 1;
        ਜਦੋਂ(class >= 0){
            pred *= output[ਸੂਚਕਾਂਕ + stride*class];
            ਪੂਰਨ g = hier->ਸਮੂਹ[class];
            ਪੂਰਨ ਵਿੱਥ = hier->ਸਮੂਹ_ਵਿੱਥ[g];
            ਲਈ(i = 0; i < hier->ਸਮੂਹ_ਮਾਪ[g]; ++i){
                delta[ਸੂਚਕਾਂਕ + stride*(ਵਿੱਥ + i)] = scale * (0 - output[ਸੂਚਕਾਂਕ + stride*(ਵਿੱਥ + i)]);
            }
            delta[ਸੂਚਕਾਂਕ + stride*class] = scale * (1 - output[ਸੂਚਕਾਂਕ + stride*class]);

            class = hier->ਮਾਪਾ[class];
        }
        *avg_cat += pred;
    } ਨਹੀਂਤਾਂ {
        ਜੇ (delta[ਸੂਚਕਾਂਕ] && ਟੈਗ){
            delta[ਸੂਚਕਾਂਕ + stride*class] = scale * (1 - output[ਸੂਚਕਾਂਕ + stride*class]);
            ਵਾਪਸ;
        }
        ਲਈ(n = 0; n < classes; ++n){
            delta[ਸੂਚਕਾਂਕ + stride*n] = scale * (((n == class)?1 : 0) - output[ਸੂਚਕਾਂਕ + stride*n]);
            ਜੇ(n == class) *avg_cat += output[ਸੂਚਕਾਂਕ + stride*n];
        }
    }
}

ਦਸ਼ਮਲਵ logit(ਦਸ਼ਮਲਵ x)
{
    ਵਾਪਸ ਲਘੂਗਣਕ(x/(1.-x));
}

ਦਸ਼ਮਲਵ tisnan(ਦਸ਼ਮਲਵ x)
{
    ਵਾਪਸ (x != x);
}

ਪੂਰਨ ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(layer l, ਪੂਰਨ batch, ਪੂਰਨ location, ਪੂਰਨ ਇੰਦਰਾਜ)
{
    ਪੂਰਨ n =   location / (l.w*l.h);
    ਪੂਰਨ loc = location % (l.w*l.h);
    ਵਾਪਸ batch*l.outputs + n*l.w*l.h*(l.coords+l.classes+1) + ਇੰਦਰਾਜ*l.w*l.h + loc;
}

ਖਾਲੀ forward_region_layer(ਅਚਲ layer l, ਜਾਲ net)
{
    ਪੂਰਨ i,j,b,t,n;
    ਸਮ੍ਰਿਤੀ_ਨਕਲ(l.output, net.ਨਿਵੇਸ਼, l.outputs*l.batch*ਆਕਾਰ(ਦਸ਼ਮਲਵ));

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GPU
    ਲਈ (b = 0; b < l.batch; ++b){
        ਲਈ(n = 0; n < l.n; ++n){
            ਪੂਰਨ ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, n*l.w*l.h, 0);
            activate_array(l.output + ਸੂਚਕਾਂਕ, 2*l.w*l.h, LOGISTIC);
            ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, n*l.w*l.h, l.coords);
            ਜੇ(!l.background) activate_array(l.output + ਸੂਚਕਾਂਕ,   l.w*l.h, LOGISTIC);
            ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, n*l.w*l.h, l.coords + 1);
            ਜੇ(!l.softmax && !l.softmax_ਰੁੱਖ) activate_array(l.output + ਸੂਚਕਾਂਕ, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    ਜੇ (l.softmax_ਰੁੱਖ){
        ਪੂਰਨ i;
        ਪੂਰਨ ਗਿਣਤੀ_2 = l.coords + 1;
        ਲਈ (i = 0; i < l.softmax_ਰੁੱਖ->groups; ++i) {
            ਪੂਰਨ ਸਮੂਹ_ਮਾਪ = l.softmax_ਰੁੱਖ->ਸਮੂਹ_ਮਾਪ[i];
            softmax_cpu(net.ਨਿਵੇਸ਼ + ਗਿਣਤੀ_2, ਸਮੂਹ_ਮਾਪ, l.batch, l.inputs, l.n*l.w*l.h, 1, l.n*l.w*l.h, l.temperature, l.output + ਗਿਣਤੀ_2);
            ਗਿਣਤੀ_2 += ਸਮੂਹ_ਮਾਪ;
        }
    } ਨਹੀਂਤਾਂ ਜੇ (l.softmax){
        ਪੂਰਨ ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, 0, 0, l.coords + !l.background);
        softmax_cpu(net.ਨਿਵੇਸ਼ + ਸੂਚਕਾਂਕ, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output + ਸੂਚਕਾਂਕ);
    }
#ਜੇ_ਅੰਤ

    memset(l.delta, 0, l.outputs * l.batch * ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਜੇ(!net.train) ਵਾਪਸ;
    ਦਸ਼ਮਲਵ avg_iou = 0;
    ਦਸ਼ਮਲਵ recall = 0;
    ਦਸ਼ਮਲਵ avg_cat = 0;
    ਦਸ਼ਮਲਵ avg_obj = 0;
    ਦਸ਼ਮਲਵ avg_anyobj = 0;
    ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    ਪੂਰਨ class_ਗਿਣਤੀ = 0;
    *(l.ਲਾਗਤ_2) = 0;
    ਲਈ (b = 0; b < l.batch; ++b) {
        ਜੇ(l.softmax_ਰੁੱਖ){
            ਪੂਰਨ onlyclass = 0;
            ਲਈ(t = 0; t < 30; ++t){
                box truth = float_ਮੰਜ਼ਿਲ_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                ਜੇ(!truth.x) ਰੋਕੋ;
                ਪੂਰਨ class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
                ਦਸ਼ਮਲਵ maxp = 0;
                ਪੂਰਨ maxi = 0;
                ਜੇ(truth.x > 100000 && truth.y > 100000){
                    ਲਈ(n = 0; n < l.n*l.w*l.h; ++n){
                        ਪੂਰਨ class_ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, n, l.coords + 1);
                        ਪੂਰਨ obj_ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, n, l.coords);
                        ਦਸ਼ਮਲਵ scale =  l.output[obj_ਸੂਚਕਾਂਕ];
                        l.delta[obj_ਸੂਚਕਾਂਕ] = l.noobject_scale * (0 - l.output[obj_ਸੂਚਕਾਂਕ]);
                        ਦਸ਼ਮਲਵ p = scale*get_hierarchy_probability(l.output + class_ਸੂਚਕਾਂਕ, l.softmax_ਰੁੱਖ, class, l.w*l.h);
                        ਜੇ(p > maxp){
                            maxp = p;
                            maxi = n;
                        }
                    }
                    ਪੂਰਨ class_ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, maxi, l.coords + 1);
                    ਪੂਰਨ obj_ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, maxi, l.coords);
                    delta_region_class(l.output, l.delta, class_ਸੂਚਕਾਂਕ, class, l.classes, l.softmax_ਰੁੱਖ, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
                    ਜੇ(l.output[obj_ਸੂਚਕਾਂਕ] < .3) l.delta[obj_ਸੂਚਕਾਂਕ] = l.object_scale * (.3 - l.output[obj_ਸੂਚਕਾਂਕ]);
                    ਨਹੀਂਤਾਂ  l.delta[obj_ਸੂਚਕਾਂਕ] = 0;
                    l.delta[obj_ਸੂਚਕਾਂਕ] = 0;
                    ++class_ਗਿਣਤੀ;
                    onlyclass = 1;
                    ਰੋਕੋ;
                }
            }
            ਜੇ(onlyclass) ਜਾਰੀ;
        }
        ਲਈ (j = 0; j < l.h; ++j) {
            ਲਈ (i = 0; i < l.w; ++i) {
                ਲਈ (n = 0; n < l.n; ++n) {
                    ਪੂਰਨ box_ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, n*l.w*l.h + j*l.w + i, 0);
                    box pred = get_region_box(l.output, l.biases, n, box_ਸੂਚਕਾਂਕ, i, j, l.w, l.h, l.w*l.h);
                    ਦਸ਼ਮਲਵ ਉੱਤਮ_iou = 0;
                    ਲਈ(t = 0; t < 30; ++t){
                        box truth = float_ਮੰਜ਼ਿਲ_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);
                        ਜੇ(!truth.x) ਰੋਕੋ;
                        ਦਸ਼ਮਲਵ iou = box_iou(pred, truth);
                        ਜੇ (iou > ਉੱਤਮ_iou) {
                            ਉੱਤਮ_iou = iou;
                        }
                    }
                    ਪੂਰਨ obj_ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, n*l.w*l.h + j*l.w + i, l.coords);
                    avg_anyobj += l.output[obj_ਸੂਚਕਾਂਕ];
                    l.delta[obj_ਸੂਚਕਾਂਕ] = l.noobject_scale * (0 - l.output[obj_ਸੂਚਕਾਂਕ]);
                    ਜੇ(l.background) l.delta[obj_ਸੂਚਕਾਂਕ] = l.noobject_scale * (1 - l.output[obj_ਸੂਚਕਾਂਕ]);
                    ਜੇ (ਉੱਤਮ_iou > l.thresh) {
                        l.delta[obj_ਸੂਚਕਾਂਕ] = 0;
                    }

                    ਜੇ(*(net.ਵੇਖਿਆ) < 12800){
                        box truth = {0};
                        truth.x = (i + .5)/l.w;
                        truth.y = (j + .5)/l.h;
                        truth.w = l.biases[2*n]/l.w;
                        truth.h = l.biases[2*n+1]/l.h;
                        delta_region_box(truth, l.output, l.biases, n, box_ਸੂਚਕਾਂਕ, i, j, l.w, l.h, l.delta, .01, l.w*l.h);
                    }
                }
            }
        }
        ਲਈ(t = 0; t < 30; ++t){
            box truth = float_ਮੰਜ਼ਿਲ_box(net.truth + t*(l.coords + 1) + b*l.truths, 1);

            ਜੇ(!truth.x) ਰੋਕੋ;
            ਦਸ਼ਮਲਵ ਉੱਤਮ_iou = 0;
            ਪੂਰਨ ਉੱਤਮ_n = 0;
            i = (truth.x * l.w);
            j = (truth.y * l.h);
            box truth_ਖਿਸਕਾਓ = truth;
            truth_ਖਿਸਕਾਓ.x = 0;
            truth_ਖਿਸਕਾਓ.y = 0;
            ਲਈ(n = 0; n < l.n; ++n){
                ਪੂਰਨ box_ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, n*l.w*l.h + j*l.w + i, 0);
                box pred = get_region_box(l.output, l.biases, n, box_ਸੂਚਕਾਂਕ, i, j, l.w, l.h, l.w*l.h);
                ਜੇ(l.bias_ਮੇਲ){
                    pred.w = l.biases[2*n]/l.w;
                    pred.h = l.biases[2*n+1]/l.h;
                }
                pred.x = 0;
                pred.y = 0;
                ਦਸ਼ਮਲਵ iou = box_iou(pred, truth_ਖਿਸਕਾਓ);
                ਜੇ (iou > ਉੱਤਮ_iou){
                    ਉੱਤਮ_iou = iou;
                    ਉੱਤਮ_n = n;
                }
            }

            ਪੂਰਨ box_ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, ਉੱਤਮ_n*l.w*l.h + j*l.w + i, 0);
            ਦਸ਼ਮਲਵ iou = delta_region_box(truth, l.output, l.biases, ਉੱਤਮ_n, box_ਸੂਚਕਾਂਕ, i, j, l.w, l.h, l.delta, l.coord_scale *  (2 - truth.w*truth.h), l.w*l.h);
            ਜੇ(l.coords > 4){
                ਪੂਰਨ ਨਕਾਬ_ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, ਉੱਤਮ_n*l.w*l.h + j*l.w + i, 4);
                delta_region_ਨਕਾਬ(net.truth + t*(l.coords + 1) + b*l.truths + 5, l.output, l.coords - 4, ਨਕਾਬ_ਸੂਚਕਾਂਕ, l.delta, l.w*l.h, l.ਨਕਾਬ_scale);
            }
            ਜੇ(iou > .5) recall += 1;
            avg_iou += iou;

            ਪੂਰਨ obj_ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, ਉੱਤਮ_n*l.w*l.h + j*l.w + i, l.coords);
            avg_obj += l.output[obj_ਸੂਚਕਾਂਕ];
            l.delta[obj_ਸੂਚਕਾਂਕ] = l.object_scale * (1 - l.output[obj_ਸੂਚਕਾਂਕ]);
            ਜੇ (l.rescore) {
                l.delta[obj_ਸੂਚਕਾਂਕ] = l.object_scale * (iou - l.output[obj_ਸੂਚਕਾਂਕ]);
            }
            ਜੇ(l.background){
                l.delta[obj_ਸੂਚਕਾਂਕ] = l.object_scale * (0 - l.output[obj_ਸੂਚਕਾਂਕ]);
            }

            ਪੂਰਨ class = net.truth[t*(l.coords + 1) + b*l.truths + l.coords];
            ਜੇ (l.map) class = l.map[class];
            ਪੂਰਨ class_ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, ਉੱਤਮ_n*l.w*l.h + j*l.w + i, l.coords + 1);
            delta_region_class(l.output, l.delta, class_ਸੂਚਕਾਂਕ, class, l.classes, l.softmax_ਰੁੱਖ, l.class_scale, l.w*l.h, &avg_cat, !l.softmax);
            ++ਗਿਣਤੀ_2;
            ++class_ਗਿਣਤੀ;
        }
    }
    *(l.ਲਾਗਤ_2) = ਘਾਤ(mag_array(l.delta, l.outputs * l.batch), 2);
    ਲਿਖੋ("Region Avg IOU: %f, Class: %f, Obj: %f, No Obj: %f, Avg Recall: %f,  count: %d\n", avg_iou/ਗਿਣਤੀ_2, avg_cat/class_ਗਿਣਤੀ, avg_obj/ਗਿਣਤੀ_2, avg_anyobj/(l.w*l.h*l.n*l.batch), recall/ਗਿਣਤੀ_2, ਗਿਣਤੀ_2);
}

ਖਾਲੀ backward_region_layer(ਅਚਲ layer l, ਜਾਲ net)
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

ਖਾਲੀ correct_region_boxes(detection *dets, ਪੂਰਨ n, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ netw, ਪੂਰਨ neth, ਪੂਰਨ relative)
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

ਖਾਲੀ get_region_detections(layer l, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ netw, ਪੂਰਨ neth, ਦਸ਼ਮਲਵ thresh, ਪੂਰਨ *map, ਦਸ਼ਮਲਵ ਰੁੱਖ_thresh, ਪੂਰਨ relative, detection *dets)
{
    ਪੂਰਨ i,j,n,z;
    ਦਸ਼ਮਲਵ *predictions = l.output;
    ਜੇ (l.batch == 2) {
        ਦਸ਼ਮਲਵ *flip = l.output + l.outputs;
        ਲਈ (j = 0; j < l.h; ++j) {
            ਲਈ (i = 0; i < l.w/2; ++i) {
                ਲਈ (n = 0; n < l.n; ++n) {
                    ਲਈ(z = 0; z < l.classes + l.coords + 1; ++z){
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
    ਲਈ (i = 0; i < l.w*l.h; ++i){
        ਪੂਰਨ ਪੰਕਤੀ = i / l.w;
        ਪੂਰਨ ਖਾਨਾ = i % l.w;
        ਲਈ(n = 0; n < l.n; ++n){
            ਪੂਰਨ ਸੂਚਕਾਂਕ = n*l.w*l.h + i;
            ਲਈ(j = 0; j < l.classes; ++j){
                dets[ਸੂਚਕਾਂਕ].prob[j] = 0;
            }
            ਪੂਰਨ obj_ਸੂਚਕਾਂਕ  = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, 0, n*l.w*l.h + i, l.coords);
            ਪੂਰਨ box_ਸੂਚਕਾਂਕ  = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, 0, n*l.w*l.h + i, 0);
            ਪੂਰਨ ਨਕਾਬ_ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, 0, n*l.w*l.h + i, 4);
            ਦਸ਼ਮਲਵ scale = l.background ? 1 : predictions[obj_ਸੂਚਕਾਂਕ];
            dets[ਸੂਚਕਾਂਕ].bbox = get_region_box(predictions, l.biases, n, box_ਸੂਚਕਾਂਕ, ਖਾਨਾ, ਪੰਕਤੀ, l.w, l.h, l.w*l.h);
            dets[ਸੂਚਕਾਂਕ].objectness = scale > thresh ? scale : 0;
            ਜੇ(dets[ਸੂਚਕਾਂਕ].ਨਕਾਬ){
                ਲਈ(j = 0; j < l.coords - 4; ++j){
                    dets[ਸੂਚਕਾਂਕ].ਨਕਾਬ[j] = l.output[ਨਕਾਬ_ਸੂਚਕਾਂਕ + j*l.w*l.h];
                }
            }

            ਪੂਰਨ class_ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, 0, n*l.w*l.h + i, l.coords + !l.background);
            ਜੇ(l.softmax_ਰੁੱਖ){

                hierarchy_predictions(predictions + class_ਸੂਚਕਾਂਕ, l.classes, l.softmax_ਰੁੱਖ, 0, l.w*l.h);
                ਜੇ(map){
                    ਲਈ(j = 0; j < 200; ++j){
                        ਪੂਰਨ class_ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, 0, n*l.w*l.h + i, l.coords + 1 + map[j]);
                        ਦਸ਼ਮਲਵ prob = scale*predictions[class_ਸੂਚਕਾਂਕ];
                        dets[ਸੂਚਕਾਂਕ].prob[j] = (prob > thresh) ? prob : 0;
                    }
                } ਨਹੀਂਤਾਂ {
                    ਪੂਰਨ j =  hierarchy_ਸਿਖਰ_prediction(predictions + class_ਸੂਚਕਾਂਕ, l.softmax_ਰੁੱਖ, ਰੁੱਖ_thresh, l.w*l.h);
                    dets[ਸੂਚਕਾਂਕ].prob[j] = (scale > thresh) ? scale : 0;
                }
            } ਨਹੀਂਤਾਂ {
                ਜੇ(dets[ਸੂਚਕਾਂਕ].objectness){
                    ਲਈ(j = 0; j < l.classes; ++j){
                        ਪੂਰਨ class_ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, 0, n*l.w*l.h + i, l.coords + 1 + j);
                        ਦਸ਼ਮਲਵ prob = scale*predictions[class_ਸੂਚਕਾਂਕ];
                        dets[ਸੂਚਕਾਂਕ].prob[j] = (prob > thresh) ? prob : 0;
                    }
                }
            }
        }
    }
    correct_region_boxes(dets, l.w*l.h*l.n, w, h, netw, neth, relative);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU

ਖਾਲੀ forward_region_layer_gpu(ਅਚਲ layer l, ਜਾਲ net)
{
    copy_gpu(l.batch*l.inputs, net.ਨਿਵੇਸ਼_gpu, 1, l.output_gpu, 1);
    ਪੂਰਨ b, n;
    ਲਈ (b = 0; b < l.batch; ++b){
        ਲਈ(n = 0; n < l.n; ++n){
            ਪੂਰਨ ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, n*l.w*l.h, 0);
            activate_array_gpu(l.output_gpu + ਸੂਚਕਾਂਕ, 2*l.w*l.h, LOGISTIC);
            ਜੇ(l.coords > 4){
                ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, n*l.w*l.h, 4);
                activate_array_gpu(l.output_gpu + ਸੂਚਕਾਂਕ, (l.coords - 4)*l.w*l.h, LOGISTIC);
            }
            ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, n*l.w*l.h, l.coords);
            ਜੇ(!l.background) activate_array_gpu(l.output_gpu + ਸੂਚਕਾਂਕ,   l.w*l.h, LOGISTIC);
            ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, n*l.w*l.h, l.coords + 1);
            ਜੇ(!l.softmax && !l.softmax_ਰੁੱਖ) activate_array_gpu(l.output_gpu + ਸੂਚਕਾਂਕ, l.classes*l.w*l.h, LOGISTIC);
        }
    }
    ਜੇ (l.softmax_ਰੁੱਖ){
        ਪੂਰਨ ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, 0, 0, l.coords + 1);
        softmax_ਰੁੱਖ(net.ਨਿਵੇਸ਼_gpu + ਸੂਚਕਾਂਕ, l.w*l.h, l.batch*l.n, l.inputs/l.n, 1, l.output_gpu + ਸੂਚਕਾਂਕ, *l.softmax_ਰੁੱਖ);
    } ਨਹੀਂਤਾਂ ਜੇ (l.softmax) {
        ਪੂਰਨ ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, 0, 0, l.coords + !l.background);
        softmax_gpu(net.ਨਿਵੇਸ਼_gpu + ਸੂਚਕਾਂਕ, l.classes + l.background, l.batch*l.n, l.inputs/l.n, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu + ਸੂਚਕਾਂਕ);
    }
    ਜੇ(!net.train || l.onlyforward){
        cuda_pull_array(l.output_gpu, l.output, l.batch*l.outputs);
        ਵਾਪਸ;
    }

    cuda_pull_array(l.output_gpu, net.ਨਿਵੇਸ਼, l.batch*l.inputs);
    forward_region_layer(l, net);
    //cuda_push_array(l.output_gpu, l.output, l.batch*l.outputs);
    ਜੇ(!net.train) ਵਾਪਸ;
    cuda_ਧੱਕੋ_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

ਖਾਲੀ backward_region_layer_gpu(ਅਚਲ layer l, ਜਾਲ net)
{
    ਪੂਰਨ b, n;
    ਲਈ (b = 0; b < l.batch; ++b){
        ਲਈ(n = 0; n < l.n; ++n){
            ਪੂਰਨ ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, n*l.w*l.h, 0);
            gradient_array_gpu(l.output_gpu + ਸੂਚਕਾਂਕ, 2*l.w*l.h, LOGISTIC, l.delta_gpu + ਸੂਚਕਾਂਕ);
            ਜੇ(l.coords > 4){
                ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, n*l.w*l.h, 4);
                gradient_array_gpu(l.output_gpu + ਸੂਚਕਾਂਕ, (l.coords - 4)*l.w*l.h, LOGISTIC, l.delta_gpu + ਸੂਚਕਾਂਕ);
            }
            ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, b, n*l.w*l.h, l.coords);
            ਜੇ(!l.background) gradient_array_gpu(l.output_gpu + ਸੂਚਕਾਂਕ,   l.w*l.h, LOGISTIC, l.delta_gpu + ਸੂਚਕਾਂਕ);
        }
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#ਜੇ_ਅੰਤ

ਖਾਲੀ zero_objectness(layer l)
{
    ਪੂਰਨ i, n;
    ਲਈ (i = 0; i < l.w*l.h; ++i){
        ਲਈ(n = 0; n < l.n; ++n){
            ਪੂਰਨ obj_ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ_ਸੂਚਕਾਂਕ(l, 0, n*l.w*l.h + i, l.coords);
            l.output[obj_ਸੂਚਕਾਂਕ] = 0;
        }
    }
}


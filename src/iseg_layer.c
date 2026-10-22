#ਅਨ੍ਤਰ੍ਭਾਵ "iseg_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "activations.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "box.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>

layer make_iseg_layer(ਪੂਰਨ batch, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ classes, ਪੂਰਨ ids)
{
    layer l = {0};
    l.type = ISEG;

    l.h = h;
    l.w = w;
    l.c = classes + ids;
    l.ਬਾਹਰਲਾ_w = l.w;
    l.ਬਾਹਰਲਾ_h = l.h;
    l.ਬਾਹਰਲਾ_c = l.c;
    l.classes = classes;
    l.batch = batch;
    l.extra = ids;
    l.ਲਾਗਤ_2 = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.outputs = h*w*l.c;
    l.inputs = l.outputs;
    l.truths = 90*(l.w*l.h+1);
    l.delta = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*l.outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.output = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*l.outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    l.counts = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(90, ਆਕਾਰ(ਪੂਰਨ));
    l.sums = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(90, ਆਕਾਰ(ਦਸ਼ਮਲਵ*));
    ਜੇ(ids){
        ਪੂਰਨ i;
        ਲਈ(i = 0; i < 90; ++i){
            l.sums[i] = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(ids, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        }
    }

    l.forward = forward_iseg_layer;
    l.backward = backward_iseg_layer;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_iseg_layer_gpu;
    l.backward_gpu = backward_iseg_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#ਜੇ_ਅੰਤ

    fprintf(stderr, "iseg\n");
    srand(0);

    ਵਾਪਸ l;
}

ਖਾਲੀ resize_iseg_layer(layer *l, ਪੂਰਨ w, ਪੂਰਨ h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->c;
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

ਖਾਲੀ forward_iseg_layer(ਅਚਲ layer l, ਜਾਲ net)
{

    ਦੁੱਗਣਾ ਸਮਾਂ = what_ਸਮਾਂ_is_it_now();
    ਪੂਰਨ i,b,j,k;
    ਪੂਰਨ ids = l.extra;
    ਸਮ੍ਰਿਤੀ_ਨਕਲ(l.output, net.ਨਿਵੇਸ਼, l.outputs*l.batch*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    memset(l.delta, 0, l.outputs * l.batch * ਆਕਾਰ(ਦਸ਼ਮਲਵ));

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GPU
    ਲਈ (b = 0; b < l.batch; ++b){
        ਪੂਰਨ ਸੂਚਕਾਂਕ = b*l.outputs;
        activate_array(l.output + ਸੂਚਕਾਂਕ, l.classes*l.w*l.h, LOGISTIC);
    }
#ਜੇ_ਅੰਤ

    ਲਈ (b = 0; b < l.batch; ++b){
        // a priori, each pixel has no class
        ਲਈ(i = 0; i < l.classes; ++i){
            ਲਈ(k = 0; k < l.w*l.h; ++k){
                ਪੂਰਨ ਸੂਚਕਾਂਕ = b*l.outputs + i*l.w*l.h + k;
                l.delta[ਸੂਚਕਾਂਕ] = 0 - l.output[ਸੂਚਕਾਂਕ];
            }
        }

        // a priori, embedding should be small magnitude
        ਲਈ(i = 0; i < ids; ++i){
            ਲਈ(k = 0; k < l.w*l.h; ++k){
                ਪੂਰਨ ਸੂਚਕਾਂਕ = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[ਸੂਚਕਾਂਕ] = .1 * (0 - l.output[ਸੂਚਕਾਂਕ]);
            }
        }


        memset(l.counts, 0, 90*ਆਕਾਰ(ਪੂਰਨ));
        ਲਈ(i = 0; i < 90; ++i){
            fill_cpu(ids, 0, l.sums[i], 1);
            
            ਪੂਰਨ c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            ਜੇ(c < 0) ਰੋਕੋ;
            // add up metric embeddings for each instance
            ਲਈ(k = 0; k < l.w*l.h; ++k){
                ਪੂਰਨ ਸੂਚਕਾਂਕ = b*l.outputs + c*l.w*l.h + k;
                ਦਸ਼ਮਲਵ v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                ਜੇ(v){
                    l.delta[ਸੂਚਕਾਂਕ] = v - l.output[ਸੂਚਕਾਂਕ];
                    axpy_cpu(ids, 1, l.output + b*l.outputs + l.classes*l.w*l.h + k, l.w*l.h, l.sums[i], 1);
                    ++l.counts[i];
                }
            }
        }

        ਦਸ਼ਮਲਵ *mse = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(90, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        ਲਈ(i = 0; i < 90; ++i){
            ਪੂਰਨ c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            ਜੇ(c < 0) ਰੋਕੋ;
            ਲਈ(k = 0; k < l.w*l.h; ++k){
                ਦਸ਼ਮਲਵ v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                ਜੇ(v){
                    ਪੂਰਨ z;
                    ਦਸ਼ਮਲਵ ਜੋੜ = 0;
                    ਲਈ(z = 0; z < ids; ++z){
                        ਪੂਰਨ ਸੂਚਕਾਂਕ = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                        ਜੋੜ += ਘਾਤ(l.sums[i][z]/l.counts[i] - l.output[ਸੂਚਕਾਂਕ], 2);
                    }
                    mse[i] += ਜੋੜ;
                }
            }
            mse[i] /= l.counts[i];
        }

        // Calculate average embedding
        ਲਈ(i = 0; i < 90; ++i){
            ਜੇ(!l.counts[i]) ਜਾਰੀ;
            scal_cpu(ids, 1.f/l.counts[i], l.sums[i], 1);
            ਜੇ(b == 0 && net.gpu_ਸੂਚਕਾਂਕ == 0){
                ਲਿਖੋ("%4d, %6.3f, ", l.counts[i], mse[i]);
                ਲਈ(j = 0; j < ids; ++j){
                    ਲਿਖੋ("%6.3f,", l.sums[i][j]);
                }
                ਲਿਖੋ("\n");
            }
        }
        ਮੁਕਤ(mse);

        // Calculate embedding loss
        ਲਈ(i = 0; i < 90; ++i){
            ਜੇ(!l.counts[i]) ਜਾਰੀ;
            ਲਈ(k = 0; k < l.w*l.h; ++k){
                ਦਸ਼ਮਲਵ v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                ਜੇ(v){
                    ਲਈ(j = 0; j < 90; ++j){
                        ਜੇ(!l.counts[j])ਜਾਰੀ;
                        ਪੂਰਨ z;
                        ਲਈ(z = 0; z < ids; ++z){
                            ਪੂਰਨ ਸੂਚਕਾਂਕ = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                            ਦਸ਼ਮਲਵ diff = l.sums[j][z] - l.output[ਸੂਚਕਾਂਕ];
                            ਜੇ (j == i) l.delta[ਸੂਚਕਾਂਕ] +=   diff < 0? -.1 : .1;
                            ਨਹੀਂਤਾਂ        l.delta[ਸੂਚਕਾਂਕ] += -(diff < 0? -.1 : .1);
                        }
                    }
                }
            }
        }

        ਲਈ(i = 0; i < ids; ++i){
            ਲਈ(k = 0; k < l.w*l.h; ++k){
                ਪੂਰਨ ਸੂਚਕਾਂਕ = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[ਸੂਚਕਾਂਕ] *= .01;
            }
        }
    }

    *(l.ਲਾਗਤ_2) = ਘਾਤ(mag_array(l.delta, l.outputs * l.batch), 2);
    ਲਿਖੋ("took %lf sec\n", what_ਸਮਾਂ_is_it_now() - ਸਮਾਂ);
}

ਖਾਲੀ backward_iseg_layer(ਅਚਲ layer l, ਜਾਲ net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU

ਖਾਲੀ forward_iseg_layer_gpu(ਅਚਲ layer l, ਜਾਲ net)
{
    copy_gpu(l.batch*l.inputs, net.ਨਿਵੇਸ਼_gpu, 1, l.output_gpu, 1);
    ਪੂਰਨ b;
    ਲਈ (b = 0; b < l.batch; ++b){
        activate_array_gpu(l.output_gpu + b*l.outputs, l.classes*l.w*l.h, LOGISTIC);
        //if(l.extra) activate_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC);
    }

    cuda_pull_array(l.output_gpu, net.ਨਿਵੇਸ਼, l.batch*l.inputs);
    forward_iseg_layer(l, net);
    cuda_ਧੱਕੋ_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

ਖਾਲੀ backward_iseg_layer_gpu(ਅਚਲ layer l, ਜਾਲ net)
{
    ਪੂਰਨ b;
    ਲਈ (b = 0; b < l.batch; ++b){
        //if(l.extra) gradient_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC, l.delta_gpu + b*l.outputs + l.classes*l.w*l.h);
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#ਜੇ_ਅੰਤ


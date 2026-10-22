#ضمن "iseg_layer.h"
#ضمن "activations.h"
#ضمن "blas.h"
#ضمن "box.h"
#ضمن "cuda.h"
#ضمن "utils.h"

#ضمن <stdio.h>
#ضمن <assert.h>
#ضمن <string.h>
#ضمن <stdlib.h>

layer make_iseg_layer(صحيح batch, صحيح w, صحيح h, صحيح classes, صحيح ids)
{
    layer l = {0};
    l.type = ISEG;

    l.h = h;
    l.w = w;
    l.c = classes + ids;
    l.out_w = l.w;
    l.out_h = l.h;
    l.out_c = l.c;
    l.classes = classes;
    l.batch = batch;
    l.extra = ids;
    l.cost = calloc(1, حجم(عائم));
    l.outputs = h*w*l.c;
    l.inputs = l.outputs;
    l.truths = 90*(l.w*l.h+1);
    l.delta = calloc(batch*l.outputs, حجم(عائم));
    l.output = calloc(batch*l.outputs, حجم(عائم));

    l.counts = calloc(90, حجم(صحيح));
    l.sums = calloc(90, حجم(عائم*));
    אם(ids){
        صحيح i;
        עבור(i = 0; i < 90; ++i){
            l.sums[i] = calloc(ids, حجم(عائم));
        }
    }

    l.forward = forward_iseg_layer;
    l.backward = backward_iseg_layer;
#ifdef GPU
    l.forward_gpu = forward_iseg_layer_gpu;
    l.backward_gpu = backward_iseg_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#endif

    fprintf(stderr, "iseg\n");
    srand(0);

    החזר l;
}

فراغ resize_iseg_layer(layer *l, صحيح w, صحيح h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->c;
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*حجم(عائم));
    l->delta = realloc(l->delta, l->batch*l->outputs*حجم(عائم));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

فراغ forward_iseg_layer(ثابت layer l, network net)
{

    مزدوج time = what_time_is_it_now();
    صحيح i,b,j,k;
    صحيح ids = l.extra;
    memcpy(l.output, net.input, l.outputs*l.batch*حجم(عائم));
    memset(l.delta, 0, l.outputs * l.batch * حجم(عائم));

#ifndef GPU
    עבור (b = 0; b < l.batch; ++b){
        صحيح index = b*l.outputs;
        activate_array(l.output + index, l.classes*l.w*l.h, LOGISTIC);
    }
#endif

    עבור (b = 0; b < l.batch; ++b){
        // a priori, each pixel has no class
        עבור(i = 0; i < l.classes; ++i){
            עבור(k = 0; k < l.w*l.h; ++k){
                صحيح index = b*l.outputs + i*l.w*l.h + k;
                l.delta[index] = 0 - l.output[index];
            }
        }

        // a priori, embedding should be small magnitude
        עבור(i = 0; i < ids; ++i){
            עבור(k = 0; k < l.w*l.h; ++k){
                صحيح index = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[index] = .1 * (0 - l.output[index]);
            }
        }


        memset(l.counts, 0, 90*حجم(صحيح));
        עבור(i = 0; i < 90; ++i){
            fill_cpu(ids, 0, l.sums[i], 1);
            
            صحيح c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            אם(c < 0) اكسر;
            // add up metric embeddings for each instance
            עבור(k = 0; k < l.w*l.h; ++k){
                صحيح index = b*l.outputs + c*l.w*l.h + k;
                عائم v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                אם(v){
                    l.delta[index] = v - l.output[index];
                    axpy_cpu(ids, 1, l.output + b*l.outputs + l.classes*l.w*l.h + k, l.w*l.h, l.sums[i], 1);
                    ++l.counts[i];
                }
            }
        }

        عائم *mse = calloc(90, حجم(عائم));
        עבור(i = 0; i < 90; ++i){
            صحيح c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            אם(c < 0) اكسر;
            עבור(k = 0; k < l.w*l.h; ++k){
                عائم v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                אם(v){
                    صحيح z;
                    عائم sum = 0;
                    עבור(z = 0; z < ids; ++z){
                        صحيح index = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                        sum += pow(l.sums[i][z]/l.counts[i] - l.output[index], 2);
                    }
                    mse[i] += sum;
                }
            }
            mse[i] /= l.counts[i];
        }

        // Calculate average embedding
        עבור(i = 0; i < 90; ++i){
            אם(!l.counts[i]) استمر;
            scal_cpu(ids, 1.f/l.counts[i], l.sums[i], 1);
            אם(b == 0 && net.gpu_index == 0){
                printf("%4d, %6.3f, ", l.counts[i], mse[i]);
                עבור(j = 0; j < ids; ++j){
                    printf("%6.3f,", l.sums[i][j]);
                }
                printf("\n");
            }
        }
        حرر(mse);

        // Calculate embedding loss
        עבור(i = 0; i < 90; ++i){
            אם(!l.counts[i]) استمر;
            עבור(k = 0; k < l.w*l.h; ++k){
                عائم v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                אם(v){
                    עבור(j = 0; j < 90; ++j){
                        אם(!l.counts[j])استمر;
                        صحيح z;
                        עבור(z = 0; z < ids; ++z){
                            صحيح index = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                            عائم diff = l.sums[j][z] - l.output[index];
                            אם (j == i) l.delta[index] +=   diff < 0? -.1 : .1;
                            אחרת        l.delta[index] += -(diff < 0? -.1 : .1);
                        }
                    }
                }
            }
        }

        עבור(i = 0; i < ids; ++i){
            עבור(k = 0; k < l.w*l.h; ++k){
                صحيح index = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[index] *= .01;
            }
        }
    }

    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("took %lf sec\n", what_time_is_it_now() - time);
}

فراغ backward_iseg_layer(ثابت layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

فراغ forward_iseg_layer_gpu(ثابت layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.input_gpu, 1, l.output_gpu, 1);
    صحيح b;
    עבור (b = 0; b < l.batch; ++b){
        activate_array_gpu(l.output_gpu + b*l.outputs, l.classes*l.w*l.h, LOGISTIC);
        //if(l.extra) activate_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC);
    }

    cuda_pull_array(l.output_gpu, net.input, l.batch*l.inputs);
    forward_iseg_layer(l, net);
    cuda_push_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

فراغ backward_iseg_layer_gpu(ثابت layer l, network net)
{
    صحيح b;
    עבור (b = 0; b < l.batch; ++b){
        //if(l.extra) gradient_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC, l.delta_gpu + b*l.outputs + l.classes*l.w*l.h);
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


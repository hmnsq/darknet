#شامل "iseg_layer.h"
#شامل "activations.h"
#شامل "blas.h"
#شامل "box.h"
#شامل "cuda.h"
#شامل "utils.h"

#شامل <stdio.h>
#شامل <assert.h>
#شامل <string.h>
#شامل <stdlib.h>

layer make_iseg_layer(صحیح batch, صحیح w, صحیح h, صحیح classes, صحیح ids)
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
    l.cost = calloc(1, اندازه(اعشاری));
    l.outputs = h*w*l.c;
    l.inputs = l.outputs;
    l.truths = 90*(l.w*l.h+1);
    l.delta = calloc(batch*l.outputs, اندازه(اعشاری));
    l.output = calloc(batch*l.outputs, اندازه(اعشاری));

    l.counts = calloc(90, اندازه(صحیح));
    l.sums = calloc(90, اندازه(اعشاری*));
    اگر(ids){
        صحیح i;
        برای(i = 0; i < 90; ++i){
            l.sums[i] = calloc(ids, اندازه(اعشاری));
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

    بازگشت l;
}

تهی resize_iseg_layer(layer *l, صحیح w, صحیح h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->c;
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*اندازه(اعشاری));
    l->delta = realloc(l->delta, l->batch*l->outputs*اندازه(اعشاری));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

تهی forward_iseg_layer(ثابت layer l, network net)
{

    دوگانه time = what_time_is_it_now();
    صحیح i,b,j,k;
    صحیح ids = l.extra;
    memcpy(l.output, net.درون_داد, l.outputs*l.batch*اندازه(اعشاری));
    memset(l.delta, 0, l.outputs * l.batch * اندازه(اعشاری));

#ifndef GPU
    برای (b = 0; b < l.batch; ++b){
        صحیح index = b*l.outputs;
        activate_array(l.output + index, l.classes*l.w*l.h, LOGISTIC);
    }
#endif

    برای (b = 0; b < l.batch; ++b){
        // a priori, each pixel has no class
        برای(i = 0; i < l.classes; ++i){
            برای(k = 0; k < l.w*l.h; ++k){
                صحیح index = b*l.outputs + i*l.w*l.h + k;
                l.delta[index] = 0 - l.output[index];
            }
        }

        // a priori, embedding should be small magnitude
        برای(i = 0; i < ids; ++i){
            برای(k = 0; k < l.w*l.h; ++k){
                صحیح index = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[index] = .1 * (0 - l.output[index]);
            }
        }


        memset(l.counts, 0, 90*اندازه(صحیح));
        برای(i = 0; i < 90; ++i){
            fill_cpu(ids, 0, l.sums[i], 1);
            
            صحیح c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            اگر(c < 0) بشکن;
            // add up metric embeddings for each instance
            برای(k = 0; k < l.w*l.h; ++k){
                صحیح index = b*l.outputs + c*l.w*l.h + k;
                اعشاری v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                اگر(v){
                    l.delta[index] = v - l.output[index];
                    axpy_cpu(ids, 1, l.output + b*l.outputs + l.classes*l.w*l.h + k, l.w*l.h, l.sums[i], 1);
                    ++l.counts[i];
                }
            }
        }

        اعشاری *mse = calloc(90, اندازه(اعشاری));
        برای(i = 0; i < 90; ++i){
            صحیح c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            اگر(c < 0) بشکن;
            برای(k = 0; k < l.w*l.h; ++k){
                اعشاری v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                اگر(v){
                    صحیح z;
                    اعشاری sum = 0;
                    برای(z = 0; z < ids; ++z){
                        صحیح index = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                        sum += pow(l.sums[i][z]/l.counts[i] - l.output[index], 2);
                    }
                    mse[i] += sum;
                }
            }
            mse[i] /= l.counts[i];
        }

        // Calculate average embedding
        برای(i = 0; i < 90; ++i){
            اگر(!l.counts[i]) ادامه;
            scal_cpu(ids, 1.f/l.counts[i], l.sums[i], 1);
            اگر(b == 0 && net.gpu_index == 0){
                printf("%4d, %6.3f, ", l.counts[i], mse[i]);
                برای(j = 0; j < ids; ++j){
                    printf("%6.3f,", l.sums[i][j]);
                }
                printf("\n");
            }
        }
        آزاد(mse);

        // Calculate embedding loss
        برای(i = 0; i < 90; ++i){
            اگر(!l.counts[i]) ادامه;
            برای(k = 0; k < l.w*l.h; ++k){
                اعشاری v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                اگر(v){
                    برای(j = 0; j < 90; ++j){
                        اگر(!l.counts[j])ادامه;
                        صحیح z;
                        برای(z = 0; z < ids; ++z){
                            صحیح index = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                            اعشاری diff = l.sums[j][z] - l.output[index];
                            اگر (j == i) l.delta[index] +=   diff < 0? -.1 : .1;
                            وگرنه        l.delta[index] += -(diff < 0? -.1 : .1);
                        }
                    }
                }
            }
        }

        برای(i = 0; i < ids; ++i){
            برای(k = 0; k < l.w*l.h; ++k){
                صحیح index = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[index] *= .01;
            }
        }
    }

    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("took %lf sec\n", what_time_is_it_now() - time);
}

تهی backward_iseg_layer(ثابت layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

تهی forward_iseg_layer_gpu(ثابت layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.درون_داد_gpu, 1, l.output_gpu, 1);
    صحیح b;
    برای (b = 0; b < l.batch; ++b){
        activate_array_gpu(l.output_gpu + b*l.outputs, l.classes*l.w*l.h, LOGISTIC);
        //if(l.extra) activate_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC);
    }

    cuda_pull_array(l.output_gpu, net.درون_داد, l.batch*l.inputs);
    forward_iseg_layer(l, net);
    cuda_افزودن_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

تهی backward_iseg_layer_gpu(ثابت layer l, network net)
{
    صحیح b;
    برای (b = 0; b < l.batch; ++b){
        //if(l.extra) gradient_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC, l.delta_gpu + b*l.outputs + l.classes*l.w*l.h);
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


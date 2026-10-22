#অন্তর্ভাব "iseg_layer.h"
#অন্তর্ভাব "activations.h"
#অন্তর্ভাব "blas.h"
#অন্তর্ভাব "box.h"
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব "utils.h"

#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <stdlib.h>

layer make_iseg_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা classes, পূর্ণসংখ্যা ids)
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
    l.cost = calloc(1, আকার(দশমিক));
    l.outputs = h*w*l.c;
    l.inputs = l.outputs;
    l.truths = 90*(l.w*l.h+1);
    l.delta = calloc(batch*l.outputs, আকার(দশমিক));
    l.output = calloc(batch*l.outputs, আকার(দশমিক));

    l.counts = calloc(90, আকার(পূর্ণসংখ্যা));
    l.sums = calloc(90, আকার(দশমিক*));
    যদি(ids){
        পূর্ণসংখ্যা i;
        জন্য(i = 0; i < 90; ++i){
            l.sums[i] = calloc(ids, আকার(দশমিক));
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

    ফেরত l;
}

শূন্য resize_iseg_layer(layer *l, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->c;
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*আকার(দশমিক));
    l->delta = realloc(l->delta, l->batch*l->outputs*আকার(দশমিক));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

শূন্য forward_iseg_layer(ধ্রুবক layer l, network net)
{

    দ্বিগুণ time = what_time_is_it_now();
    পূর্ণসংখ্যা i,b,j,k;
    পূর্ণসংখ্যা ids = l.extra;
    memcpy(l.output, net.প্রবেশ, l.outputs*l.batch*আকার(দশমিক));
    memset(l.delta, 0, l.outputs * l.batch * আকার(দশমিক));

#ifndef GPU
    জন্য (b = 0; b < l.batch; ++b){
        পূর্ণসংখ্যা সূচক = b*l.outputs;
        activate_array(l.output + সূচক, l.classes*l.w*l.h, LOGISTIC);
    }
#endif

    জন্য (b = 0; b < l.batch; ++b){
        // a priori, each pixel has no class
        জন্য(i = 0; i < l.classes; ++i){
            জন্য(k = 0; k < l.w*l.h; ++k){
                পূর্ণসংখ্যা সূচক = b*l.outputs + i*l.w*l.h + k;
                l.delta[সূচক] = 0 - l.output[সূচক];
            }
        }

        // a priori, embedding should be small magnitude
        জন্য(i = 0; i < ids; ++i){
            জন্য(k = 0; k < l.w*l.h; ++k){
                পূর্ণসংখ্যা সূচক = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[সূচক] = .1 * (0 - l.output[সূচক]);
            }
        }


        memset(l.counts, 0, 90*আকার(পূর্ণসংখ্যা));
        জন্য(i = 0; i < 90; ++i){
            fill_cpu(ids, 0, l.sums[i], 1);
            
            পূর্ণসংখ্যা c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            যদি(c < 0) বিরতি;
            // add up metric embeddings for each instance
            জন্য(k = 0; k < l.w*l.h; ++k){
                পূর্ণসংখ্যা সূচক = b*l.outputs + c*l.w*l.h + k;
                দশমিক v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                যদি(v){
                    l.delta[সূচক] = v - l.output[সূচক];
                    axpy_cpu(ids, 1, l.output + b*l.outputs + l.classes*l.w*l.h + k, l.w*l.h, l.sums[i], 1);
                    ++l.counts[i];
                }
            }
        }

        দশমিক *mse = calloc(90, আকার(দশমিক));
        জন্য(i = 0; i < 90; ++i){
            পূর্ণসংখ্যা c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            যদি(c < 0) বিরতি;
            জন্য(k = 0; k < l.w*l.h; ++k){
                দশমিক v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                যদি(v){
                    পূর্ণসংখ্যা z;
                    দশমিক যোগ = 0;
                    জন্য(z = 0; z < ids; ++z){
                        পূর্ণসংখ্যা সূচক = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                        যোগ += pow(l.sums[i][z]/l.counts[i] - l.output[সূচক], 2);
                    }
                    mse[i] += যোগ;
                }
            }
            mse[i] /= l.counts[i];
        }

        // Calculate average embedding
        জন্য(i = 0; i < 90; ++i){
            যদি(!l.counts[i]) চালু;
            scal_cpu(ids, 1.f/l.counts[i], l.sums[i], 1);
            যদি(b == 0 && net.gpu_সূচক == 0){
                printf("%4d, %6.3f, ", l.counts[i], mse[i]);
                জন্য(j = 0; j < ids; ++j){
                    printf("%6.3f,", l.sums[i][j]);
                }
                printf("\n");
            }
        }
        মুক্ত(mse);

        // Calculate embedding loss
        জন্য(i = 0; i < 90; ++i){
            যদি(!l.counts[i]) চালু;
            জন্য(k = 0; k < l.w*l.h; ++k){
                দশমিক v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                যদি(v){
                    জন্য(j = 0; j < 90; ++j){
                        যদি(!l.counts[j])চালু;
                        পূর্ণসংখ্যা z;
                        জন্য(z = 0; z < ids; ++z){
                            পূর্ণসংখ্যা সূচক = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                            দশমিক diff = l.sums[j][z] - l.output[সূচক];
                            যদি (j == i) l.delta[সূচক] +=   diff < 0? -.1 : .1;
                            অন্যথা        l.delta[সূচক] += -(diff < 0? -.1 : .1);
                        }
                    }
                }
            }
        }

        জন্য(i = 0; i < ids; ++i){
            জন্য(k = 0; k < l.w*l.h; ++k){
                পূর্ণসংখ্যা সূচক = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[সূচক] *= .01;
            }
        }
    }

    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("took %lf sec\n", what_time_is_it_now() - time);
}

শূন্য backward_iseg_layer(ধ্রুবক layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

শূন্য forward_iseg_layer_gpu(ধ্রুবক layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.প্রবেশ_gpu, 1, l.output_gpu, 1);
    পূর্ণসংখ্যা b;
    জন্য (b = 0; b < l.batch; ++b){
        activate_array_gpu(l.output_gpu + b*l.outputs, l.classes*l.w*l.h, LOGISTIC);
        //if(l.extra) activate_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC);
    }

    cuda_pull_array(l.output_gpu, net.প্রবেশ, l.batch*l.inputs);
    forward_iseg_layer(l, net);
    cuda_ঠেলো_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

শূন্য backward_iseg_layer_gpu(ধ্রুবক layer l, network net)
{
    পূর্ণসংখ্যা b;
    জন্য (b = 0; b < l.batch; ++b){
        //if(l.extra) gradient_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC, l.delta_gpu + b*l.outputs + l.classes*l.w*l.h);
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


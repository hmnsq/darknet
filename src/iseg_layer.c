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

layer make_iseg_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক classes, পূর্ণাঙ্ক ids)
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
    l.cost = calloc(1, পরিমাণ(ভিন্ন));
    l.outputs = h*w*l.c;
    l.inputs = l.outputs;
    l.truths = 90*(l.w*l.h+1);
    l.delta = calloc(batch*l.outputs, পরিমাণ(ভিন্ন));
    l.output = calloc(batch*l.outputs, পরিমাণ(ভিন্ন));

    l.counts = calloc(90, পরিমাণ(পূর্ণাঙ্ক));
    l.sums = calloc(90, পরিমাণ(ভিন্ন*));
    যদি(ids){
        পূর্ণাঙ্ক i;
        প্রতি(i = 0; i < 90; ++i){
            l.sums[i] = calloc(ids, পরিমাণ(ভিন্ন));
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

    প্রত্যাগম l;
}

শূন্য resize_iseg_layer(layer *l, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->c;
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*পরিমাণ(ভিন্ন));
    l->delta = realloc(l->delta, l->batch*l->outputs*পরিমাণ(ভিন্ন));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

শূন্য forward_iseg_layer(ধ্রুব layer l, network net)
{

    দ্বিগুণ time = what_time_is_it_now();
    পূর্ণাঙ্ক i,b,j,k;
    পূর্ণাঙ্ক ids = l.extra;
    memcpy(l.output, net.প্রবেশ, l.outputs*l.batch*পরিমাণ(ভিন্ন));
    memset(l.delta, 0, l.outputs * l.batch * পরিমাণ(ভিন্ন));

#ifndef GPU
    প্রতি (b = 0; b < l.batch; ++b){
        পূর্ণাঙ্ক index = b*l.outputs;
        activate_array(l.output + index, l.classes*l.w*l.h, LOGISTIC);
    }
#endif

    প্রতি (b = 0; b < l.batch; ++b){
        // a priori, each pixel has no class
        প্রতি(i = 0; i < l.classes; ++i){
            প্রতি(k = 0; k < l.w*l.h; ++k){
                পূর্ণাঙ্ক index = b*l.outputs + i*l.w*l.h + k;
                l.delta[index] = 0 - l.output[index];
            }
        }

        // a priori, embedding should be small magnitude
        প্রতি(i = 0; i < ids; ++i){
            প্রতি(k = 0; k < l.w*l.h; ++k){
                পূর্ণাঙ্ক index = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[index] = .1 * (0 - l.output[index]);
            }
        }


        memset(l.counts, 0, 90*পরিমাণ(পূর্ণাঙ্ক));
        প্রতি(i = 0; i < 90; ++i){
            fill_cpu(ids, 0, l.sums[i], 1);
            
            পূর্ণাঙ্ক c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            যদি(c < 0) ভঙ্গ;
            // add up metric embeddings for each instance
            প্রতি(k = 0; k < l.w*l.h; ++k){
                পূর্ণাঙ্ক index = b*l.outputs + c*l.w*l.h + k;
                ভিন্ন v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                যদি(v){
                    l.delta[index] = v - l.output[index];
                    axpy_cpu(ids, 1, l.output + b*l.outputs + l.classes*l.w*l.h + k, l.w*l.h, l.sums[i], 1);
                    ++l.counts[i];
                }
            }
        }

        ভিন্ন *mse = calloc(90, পরিমাণ(ভিন্ন));
        প্রতি(i = 0; i < 90; ++i){
            পূর্ণাঙ্ক c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            যদি(c < 0) ভঙ্গ;
            প্রতি(k = 0; k < l.w*l.h; ++k){
                ভিন্ন v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                যদি(v){
                    পূর্ণাঙ্ক z;
                    ভিন্ন sum = 0;
                    প্রতি(z = 0; z < ids; ++z){
                        পূর্ণাঙ্ক index = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                        sum += pow(l.sums[i][z]/l.counts[i] - l.output[index], 2);
                    }
                    mse[i] += sum;
                }
            }
            mse[i] /= l.counts[i];
        }

        // Calculate average embedding
        প্রতি(i = 0; i < 90; ++i){
            যদি(!l.counts[i]) অনুবর্ত;
            scal_cpu(ids, 1.f/l.counts[i], l.sums[i], 1);
            যদি(b == 0 && net.gpu_index == 0){
                printf("%4d, %6.3f, ", l.counts[i], mse[i]);
                প্রতি(j = 0; j < ids; ++j){
                    printf("%6.3f,", l.sums[i][j]);
                }
                printf("\n");
            }
        }
        মুক্ত(mse);

        // Calculate embedding loss
        প্রতি(i = 0; i < 90; ++i){
            যদি(!l.counts[i]) অনুবর্ত;
            প্রতি(k = 0; k < l.w*l.h; ++k){
                ভিন্ন v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                যদি(v){
                    প্রতি(j = 0; j < 90; ++j){
                        যদি(!l.counts[j])অনুবর্ত;
                        পূর্ণাঙ্ক z;
                        প্রতি(z = 0; z < ids; ++z){
                            পূর্ণাঙ্ক index = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                            ভিন্ন diff = l.sums[j][z] - l.output[index];
                            যদি (j == i) l.delta[index] +=   diff < 0? -.1 : .1;
                            অন্যথা        l.delta[index] += -(diff < 0? -.1 : .1);
                        }
                    }
                }
            }
        }

        প্রতি(i = 0; i < ids; ++i){
            প্রতি(k = 0; k < l.w*l.h; ++k){
                পূর্ণাঙ্ক index = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[index] *= .01;
            }
        }
    }

    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("took %lf sec\n", what_time_is_it_now() - time);
}

শূন্য backward_iseg_layer(ধ্রুব layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

শূন্য forward_iseg_layer_gpu(ধ্রুব layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.প্রবেশ_gpu, 1, l.output_gpu, 1);
    পূর্ণাঙ্ক b;
    প্রতি (b = 0; b < l.batch; ++b){
        activate_array_gpu(l.output_gpu + b*l.outputs, l.classes*l.w*l.h, LOGISTIC);
        //if(l.extra) activate_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC);
    }

    cuda_pull_array(l.output_gpu, net.প্রবেশ, l.batch*l.inputs);
    forward_iseg_layer(l, net);
    cuda_ঠেলো_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

শূন্য backward_iseg_layer_gpu(ধ্রুব layer l, network net)
{
    পূর্ণাঙ্ক b;
    প্রতি (b = 0; b < l.batch; ++b){
        //if(l.extra) gradient_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC, l.delta_gpu + b*l.outputs + l.classes*l.w*l.h);
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


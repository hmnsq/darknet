#include "iseg_layer.h"
#include "activations.h"
#include "blas.h"
#include "box.h"
#include "cuda.h"
#include "utils.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

layer make_iseg_layer(ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ classes, ຈຳນວນເຕັມ ids)
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
    l.cost = calloc(1, parimaa_na(float));
    l.outputs = h*w*l.c;
    l.inputs = l.outputs;
    l.truths = 90*(l.w*l.h+1);
    l.delta = calloc(batch*l.outputs, parimaa_na(float));
    l.output = calloc(batch*l.outputs, parimaa_na(float));

    l.counts = calloc(90, parimaa_na(ຈຳນວນເຕັມ));
    l.sums = calloc(90, parimaa_na(float*));
    ຖ້າ(ids){
        ຈຳນວນເຕັມ i;
        ສຳລັບ(i = 0; i < 90; ++i){
            l.sums[i] = calloc(ids, parimaa_na(float));
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

    ສົ່ງຄືນ l;
}

void resize_iseg_layer(layer *l, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->c;
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*parimaa_na(float));
    l->delta = realloc(l->delta, l->batch*l->outputs*parimaa_na(float));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

void forward_iseg_layer(const layer l, network net)
{

    dviguu_na time = what_time_is_it_now();
    ຈຳນວນເຕັມ i,b,j,k;
    ຈຳນວນເຕັມ ids = l.extra;
    memcpy(l.output, net.input, l.outputs*l.batch*parimaa_na(float));
    memset(l.delta, 0, l.outputs * l.batch * parimaa_na(float));

#ifndef GPU
    ສຳລັບ (b = 0; b < l.batch; ++b){
        ຈຳນວນເຕັມ index = b*l.outputs;
        activate_array(l.output + index, l.classes*l.w*l.h, LOGISTIC);
    }
#endif

    ສຳລັບ (b = 0; b < l.batch; ++b){
        // a priori, each pixel has no class
        ສຳລັບ(i = 0; i < l.classes; ++i){
            ສຳລັບ(k = 0; k < l.w*l.h; ++k){
                ຈຳນວນເຕັມ index = b*l.outputs + i*l.w*l.h + k;
                l.delta[index] = 0 - l.output[index];
            }
        }

        // a priori, embedding should be small magnitude
        ສຳລັບ(i = 0; i < ids; ++i){
            ສຳລັບ(k = 0; k < l.w*l.h; ++k){
                ຈຳນວນເຕັມ index = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[index] = .1 * (0 - l.output[index]);
            }
        }


        memset(l.counts, 0, 90*parimaa_na(ຈຳນວນເຕັມ));
        ສຳລັບ(i = 0; i < 90; ++i){
            fill_cpu(ids, 0, l.sums[i], 1);
            
            ຈຳນວນເຕັມ c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            ຖ້າ(c < 0) bha_nga;
            // add up metric embeddings for each instance
            ສຳລັບ(k = 0; k < l.w*l.h; ++k){
                ຈຳນວນເຕັມ index = b*l.outputs + c*l.w*l.h + k;
                float v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                ຖ້າ(v){
                    l.delta[index] = v - l.output[index];
                    axpy_cpu(ids, 1, l.output + b*l.outputs + l.classes*l.w*l.h + k, l.w*l.h, l.sums[i], 1);
                    ++l.counts[i];
                }
            }
        }

        float *mse = calloc(90, parimaa_na(float));
        ສຳລັບ(i = 0; i < 90; ++i){
            ຈຳນວນເຕັມ c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            ຖ້າ(c < 0) bha_nga;
            ສຳລັບ(k = 0; k < l.w*l.h; ++k){
                float v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                ຖ້າ(v){
                    ຈຳນວນເຕັມ z;
                    float sum = 0;
                    ສຳລັບ(z = 0; z < ids; ++z){
                        ຈຳນວນເຕັມ index = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                        sum += pow(l.sums[i][z]/l.counts[i] - l.output[index], 2);
                    }
                    mse[i] += sum;
                }
            }
            mse[i] /= l.counts[i];
        }

        // Calculate average embedding
        ສຳລັບ(i = 0; i < 90; ++i){
            ຖ້າ(!l.counts[i]) continue;
            scal_cpu(ids, 1.f/l.counts[i], l.sums[i], 1);
            ຖ້າ(b == 0 && net.gpu_index == 0){
                printf("%4d, %6.3f, ", l.counts[i], mse[i]);
                ສຳລັບ(j = 0; j < ids; ++j){
                    printf("%6.3f,", l.sums[i][j]);
                }
                printf("\n");
            }
        }
        free(mse);

        // Calculate embedding loss
        ສຳລັບ(i = 0; i < 90; ++i){
            ຖ້າ(!l.counts[i]) continue;
            ສຳລັບ(k = 0; k < l.w*l.h; ++k){
                float v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                ຖ້າ(v){
                    ສຳລັບ(j = 0; j < 90; ++j){
                        ຖ້າ(!l.counts[j])continue;
                        ຈຳນວນເຕັມ z;
                        ສຳລັບ(z = 0; z < ids; ++z){
                            ຈຳນວນເຕັມ index = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                            float diff = l.sums[j][z] - l.output[index];
                            ຖ້າ (j == i) l.delta[index] +=   diff < 0? -.1 : .1;
                            ບໍ່ດັ່ງນັ້ນ        l.delta[index] += -(diff < 0? -.1 : .1);
                        }
                    }
                }
            }
        }

        ສຳລັບ(i = 0; i < ids; ++i){
            ສຳລັບ(k = 0; k < l.w*l.h; ++k){
                ຈຳນວນເຕັມ index = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[index] *= .01;
            }
        }
    }

    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("took %lf sec\n", what_time_is_it_now() - time);
}

void backward_iseg_layer(const layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

void forward_iseg_layer_gpu(const layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.input_gpu, 1, l.output_gpu, 1);
    ຈຳນວນເຕັມ b;
    ສຳລັບ (b = 0; b < l.batch; ++b){
        activate_array_gpu(l.output_gpu + b*l.outputs, l.classes*l.w*l.h, LOGISTIC);
        //if(l.extra) activate_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC);
    }

    cuda_pull_array(l.output_gpu, net.input, l.batch*l.inputs);
    forward_iseg_layer(l, net);
    cuda_push_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

void backward_iseg_layer_gpu(const layer l, network net)
{
    ຈຳນວນເຕັມ b;
    ສຳລັບ (b = 0; b < l.batch; ++b){
        //if(l.extra) gradient_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC, l.delta_gpu + b*l.outputs + l.classes*l.w*l.h);
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


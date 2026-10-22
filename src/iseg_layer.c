#ಅನ್ತರ್ಭಾವ "iseg_layer.h"
#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "box.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>

layer make_iseg_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ classes, ಪೂರ್ಣಾಙ್ಕ ids)
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
    l.cost = calloc(1, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.outputs = h*w*l.c;
    l.inputs = l.outputs;
    l.truths = 90*(l.w*l.h+1);
    l.delta = calloc(batch*l.outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.output = calloc(batch*l.outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));

    l.counts = calloc(90, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ));
    l.sums = calloc(90, ಪರಿಮಾಣ(ಭಿನ್ನ*));
    ಯದಿ(ids){
        ಪೂರ್ಣಾಙ್ಕ i;
        ಪ್ರತಿ(i = 0; i < 90; ++i){
            l.sums[i] = calloc(ids, ಪರಿಮಾಣ(ಭಿನ್ನ));
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

    ಪ್ರತ್ಯಾಗಮ l;
}

ಶೂನ್ಯ resize_iseg_layer(layer *l, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->c;
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*ಪರಿಮಾಣ(ಭಿನ್ನ));
    l->delta = realloc(l->delta, l->batch*l->outputs*ಪರಿಮಾಣ(ಭಿನ್ನ));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

ಶೂನ್ಯ forward_iseg_layer(ಧ್ರುವ layer l, network net)
{

    ದ್ವಿಗುಣ time = what_time_is_it_now();
    ಪೂರ್ಣಾಙ್ಕ i,b,j,k;
    ಪೂರ್ಣಾಙ್ಕ ids = l.extra;
    memcpy(l.output, net.ಒಳಹರಿವು, l.outputs*l.batch*ಪರಿಮಾಣ(ಭಿನ್ನ));
    memset(l.delta, 0, l.outputs * l.batch * ಪರಿಮಾಣ(ಭಿನ್ನ));

#ifndef GPU
    ಪ್ರತಿ (b = 0; b < l.batch; ++b){
        ಪೂರ್ಣಾಙ್ಕ index = b*l.outputs;
        activate_array(l.output + index, l.classes*l.w*l.h, LOGISTIC);
    }
#endif

    ಪ್ರತಿ (b = 0; b < l.batch; ++b){
        // a priori, each pixel has no class
        ಪ್ರತಿ(i = 0; i < l.classes; ++i){
            ಪ್ರತಿ(k = 0; k < l.w*l.h; ++k){
                ಪೂರ್ಣಾಙ್ಕ index = b*l.outputs + i*l.w*l.h + k;
                l.delta[index] = 0 - l.output[index];
            }
        }

        // a priori, embedding should be small magnitude
        ಪ್ರತಿ(i = 0; i < ids; ++i){
            ಪ್ರತಿ(k = 0; k < l.w*l.h; ++k){
                ಪೂರ್ಣಾಙ್ಕ index = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[index] = .1 * (0 - l.output[index]);
            }
        }


        memset(l.counts, 0, 90*ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ));
        ಪ್ರತಿ(i = 0; i < 90; ++i){
            fill_cpu(ids, 0, l.sums[i], 1);
            
            ಪೂರ್ಣಾಙ್ಕ c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            ಯದಿ(c < 0) ಭಙ್ಗ;
            // add up metric embeddings for each instance
            ಪ್ರತಿ(k = 0; k < l.w*l.h; ++k){
                ಪೂರ್ಣಾಙ್ಕ index = b*l.outputs + c*l.w*l.h + k;
                ಭಿನ್ನ v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                ಯದಿ(v){
                    l.delta[index] = v - l.output[index];
                    axpy_cpu(ids, 1, l.output + b*l.outputs + l.classes*l.w*l.h + k, l.w*l.h, l.sums[i], 1);
                    ++l.counts[i];
                }
            }
        }

        ಭಿನ್ನ *mse = calloc(90, ಪರಿಮಾಣ(ಭಿನ್ನ));
        ಪ್ರತಿ(i = 0; i < 90; ++i){
            ಪೂರ್ಣಾಙ್ಕ c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            ಯದಿ(c < 0) ಭಙ್ಗ;
            ಪ್ರತಿ(k = 0; k < l.w*l.h; ++k){
                ಭಿನ್ನ v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                ಯದಿ(v){
                    ಪೂರ್ಣಾಙ್ಕ z;
                    ಭಿನ್ನ sum = 0;
                    ಪ್ರತಿ(z = 0; z < ids; ++z){
                        ಪೂರ್ಣಾಙ್ಕ index = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                        sum += pow(l.sums[i][z]/l.counts[i] - l.output[index], 2);
                    }
                    mse[i] += sum;
                }
            }
            mse[i] /= l.counts[i];
        }

        // Calculate average embedding
        ಪ್ರತಿ(i = 0; i < 90; ++i){
            ಯದಿ(!l.counts[i]) ಅನುವರ್ತ;
            scal_cpu(ids, 1.f/l.counts[i], l.sums[i], 1);
            ಯದಿ(b == 0 && net.gpu_index == 0){
                printf("%4d, %6.3f, ", l.counts[i], mse[i]);
                ಪ್ರತಿ(j = 0; j < ids; ++j){
                    printf("%6.3f,", l.sums[i][j]);
                }
                printf("\n");
            }
        }
        ಮುಕ್ತ(mse);

        // Calculate embedding loss
        ಪ್ರತಿ(i = 0; i < 90; ++i){
            ಯದಿ(!l.counts[i]) ಅನುವರ್ತ;
            ಪ್ರತಿ(k = 0; k < l.w*l.h; ++k){
                ಭಿನ್ನ v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                ಯದಿ(v){
                    ಪ್ರತಿ(j = 0; j < 90; ++j){
                        ಯದಿ(!l.counts[j])ಅನುವರ್ತ;
                        ಪೂರ್ಣಾಙ್ಕ z;
                        ಪ್ರತಿ(z = 0; z < ids; ++z){
                            ಪೂರ್ಣಾಙ್ಕ index = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                            ಭಿನ್ನ diff = l.sums[j][z] - l.output[index];
                            ಯದಿ (j == i) l.delta[index] +=   diff < 0? -.1 : .1;
                            ಅನ್ಯಥಾ        l.delta[index] += -(diff < 0? -.1 : .1);
                        }
                    }
                }
            }
        }

        ಪ್ರತಿ(i = 0; i < ids; ++i){
            ಪ್ರತಿ(k = 0; k < l.w*l.h; ++k){
                ಪೂರ್ಣಾಙ್ಕ index = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[index] *= .01;
            }
        }
    }

    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("took %lf sec\n", what_time_is_it_now() - time);
}

ಶೂನ್ಯ backward_iseg_layer(ಧ್ರುವ layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

ಶೂನ್ಯ forward_iseg_layer_gpu(ಧ್ರುವ layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.ಒಳಹರಿವು_gpu, 1, l.output_gpu, 1);
    ಪೂರ್ಣಾಙ್ಕ b;
    ಪ್ರತಿ (b = 0; b < l.batch; ++b){
        activate_array_gpu(l.output_gpu + b*l.outputs, l.classes*l.w*l.h, LOGISTIC);
        //if(l.extra) activate_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC);
    }

    cuda_pull_array(l.output_gpu, net.ಒಳಹರಿವು, l.batch*l.inputs);
    forward_iseg_layer(l, net);
    cuda_ತಳ್ಳು_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

ಶೂನ್ಯ backward_iseg_layer_gpu(ಧ್ರುವ layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ b;
    ಪ್ರತಿ (b = 0; b < l.batch; ++b){
        //if(l.extra) gradient_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC, l.delta_gpu + b*l.outputs + l.classes*l.w*l.h);
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


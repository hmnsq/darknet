#अन्तर्भाव "iseg_layer.h"
#अन्तर्भाव "activations.h"
#अन्तर्भाव "blas.h"
#अन्तर्भाव "box.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव "utils.h"

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <assert.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <stdlib.h>

layer make_iseg_layer(पूर्णाङ्क batch, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क classes, पूर्णाङ्क ids)
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
    l.cost = calloc(1, परिमाण(भिन्न));
    l.outputs = h*w*l.c;
    l.inputs = l.outputs;
    l.truths = 90*(l.w*l.h+1);
    l.delta = calloc(batch*l.outputs, परिमाण(भिन्न));
    l.output = calloc(batch*l.outputs, परिमाण(भिन्न));

    l.counts = calloc(90, परिमाण(पूर्णाङ्क));
    l.sums = calloc(90, परिमाण(भिन्न*));
    सचे(ids){
        पूर्णाङ्क i;
        अत्थाय(i = 0; i < 90; ++i){
            l.sums[i] = calloc(ids, परिमाण(भिन्न));
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

    पटिनिवत्त l;
}

शून्य resize_iseg_layer(layer *l, पूर्णाङ्क w, पूर्णाङ्क h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->c;
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*परिमाण(भिन्न));
    l->delta = realloc(l->delta, l->batch*l->outputs*परिमाण(भिन्न));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

शून्य forward_iseg_layer(ध्रुव layer l, network net)
{

    द्विगुण time = what_time_is_it_now();
    पूर्णाङ्क i,b,j,k;
    पूर्णाङ्क ids = l.extra;
    memcpy(l.output, net.निवेशः, l.outputs*l.batch*परिमाण(भिन्न));
    memset(l.delta, 0, l.outputs * l.batch * परिमाण(भिन्न));

#ifndef GPU
    अत्थाय (b = 0; b < l.batch; ++b){
        पूर्णाङ्क index = b*l.outputs;
        activate_array(l.output + index, l.classes*l.w*l.h, LOGISTIC);
    }
#endif

    अत्थाय (b = 0; b < l.batch; ++b){
        // a priori, each pixel has no class
        अत्थाय(i = 0; i < l.classes; ++i){
            अत्थाय(k = 0; k < l.w*l.h; ++k){
                पूर्णाङ्क index = b*l.outputs + i*l.w*l.h + k;
                l.delta[index] = 0 - l.output[index];
            }
        }

        // a priori, embedding should be small magnitude
        अत्थाय(i = 0; i < ids; ++i){
            अत्थाय(k = 0; k < l.w*l.h; ++k){
                पूर्णाङ्क index = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[index] = .1 * (0 - l.output[index]);
            }
        }


        memset(l.counts, 0, 90*परिमाण(पूर्णाङ्क));
        अत्थाय(i = 0; i < 90; ++i){
            fill_cpu(ids, 0, l.sums[i], 1);
            
            पूर्णाङ्क c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            सचे(c < 0) भङ्ग;
            // add up metric embeddings for each instance
            अत्थाय(k = 0; k < l.w*l.h; ++k){
                पूर्णाङ्क index = b*l.outputs + c*l.w*l.h + k;
                भिन्न v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                सचे(v){
                    l.delta[index] = v - l.output[index];
                    axpy_cpu(ids, 1, l.output + b*l.outputs + l.classes*l.w*l.h + k, l.w*l.h, l.sums[i], 1);
                    ++l.counts[i];
                }
            }
        }

        भिन्न *mse = calloc(90, परिमाण(भिन्न));
        अत्थाय(i = 0; i < 90; ++i){
            पूर्णाङ्क c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            सचे(c < 0) भङ्ग;
            अत्थाय(k = 0; k < l.w*l.h; ++k){
                भिन्न v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                सचे(v){
                    पूर्णाङ्क z;
                    भिन्न sum = 0;
                    अत्थाय(z = 0; z < ids; ++z){
                        पूर्णाङ्क index = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                        sum += pow(l.sums[i][z]/l.counts[i] - l.output[index], 2);
                    }
                    mse[i] += sum;
                }
            }
            mse[i] /= l.counts[i];
        }

        // Calculate average embedding
        अत्थाय(i = 0; i < 90; ++i){
            सचे(!l.counts[i]) अनुवर्त;
            scal_cpu(ids, 1.f/l.counts[i], l.sums[i], 1);
            सचे(b == 0 && net.gpu_index == 0){
                printf("%4d, %6.3f, ", l.counts[i], mse[i]);
                अत्थाय(j = 0; j < ids; ++j){
                    printf("%6.3f,", l.sums[i][j]);
                }
                printf("\n");
            }
        }
        मुक्त(mse);

        // Calculate embedding loss
        अत्थाय(i = 0; i < 90; ++i){
            सचे(!l.counts[i]) अनुवर्त;
            अत्थाय(k = 0; k < l.w*l.h; ++k){
                भिन्न v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                सचे(v){
                    अत्थाय(j = 0; j < 90; ++j){
                        सचे(!l.counts[j])अनुवर्त;
                        पूर्णाङ्क z;
                        अत्थाय(z = 0; z < ids; ++z){
                            पूर्णाङ्क index = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                            भिन्न diff = l.sums[j][z] - l.output[index];
                            सचे (j == i) l.delta[index] +=   diff < 0? -.1 : .1;
                            अञ्ञथा        l.delta[index] += -(diff < 0? -.1 : .1);
                        }
                    }
                }
            }
        }

        अत्थाय(i = 0; i < ids; ++i){
            अत्थाय(k = 0; k < l.w*l.h; ++k){
                पूर्णाङ्क index = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[index] *= .01;
            }
        }
    }

    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("took %lf sec\n", what_time_is_it_now() - time);
}

शून्य backward_iseg_layer(ध्रुव layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

शून्य forward_iseg_layer_gpu(ध्रुव layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.निवेशः_gpu, 1, l.output_gpu, 1);
    पूर्णाङ्क b;
    अत्थाय (b = 0; b < l.batch; ++b){
        activate_array_gpu(l.output_gpu + b*l.outputs, l.classes*l.w*l.h, LOGISTIC);
        //if(l.extra) activate_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC);
    }

    cuda_pull_array(l.output_gpu, net.निवेशः, l.batch*l.inputs);
    forward_iseg_layer(l, net);
    cuda_प्रक्षिप_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

शून्य backward_iseg_layer_gpu(ध्रुव layer l, network net)
{
    पूर्णाङ्क b;
    अत्थाय (b = 0; b < l.batch; ++b){
        //if(l.extra) gradient_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC, l.delta_gpu + b*l.outputs + l.classes*l.w*l.h);
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


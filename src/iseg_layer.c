#ଅନ୍ତର୍ଭାଵ "iseg_layer.h"
#ଅନ୍ତର୍ଭାଵ "activations.h"
#ଅନ୍ତର୍ଭାଵ "blas.h"
#ଅନ୍ତର୍ଭାଵ "box.h"
#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"

#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>

layer make_iseg_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ids)
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
    l.cost = calloc(1, ଆକାର(ଦଶମିକ));
    l.outputs = h*w*l.c;
    l.inputs = l.outputs;
    l.truths = 90*(l.w*l.h+1);
    l.delta = calloc(batch*l.outputs, ଆକାର(ଦଶମିକ));
    l.output = calloc(batch*l.outputs, ଆକାର(ଦଶମିକ));

    l.counts = calloc(90, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
    l.sums = calloc(90, ଆକାର(ଦଶମିକ*));
    ଯଦି(ids){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
        ପାଇଁ(i = 0; i < 90; ++i){
            l.sums[i] = calloc(ids, ଆକାର(ଦଶମିକ));
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

    ଫେରନ୍ତୁ l;
}

ଶୂନ୍ୟ resize_iseg_layer(layer *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->c;
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*ଆକାର(ଦଶମିକ));
    l->delta = realloc(l->delta, l->batch*l->outputs*ଆକାର(ଦଶମିକ));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

ଶୂନ୍ୟ forward_iseg_layer(ଅଚଳ layer l, network net)
{

    ଦ୍ୱିଗୁଣ time = what_time_is_it_now();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,b,j,k;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ids = l.extra;
    memcpy(l.output, net.ନିବେଶ, l.outputs*l.batch*ଆକାର(ଦଶମିକ));
    memset(l.delta, 0, l.outputs * l.batch * ଆକାର(ଦଶମିକ));

#ifndef GPU
    ପାଇଁ (b = 0; b < l.batch; ++b){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = b*l.outputs;
        activate_array(l.output + index, l.classes*l.w*l.h, LOGISTIC);
    }
#endif

    ପାଇଁ (b = 0; b < l.batch; ++b){
        // a priori, each pixel has no class
        ପାଇଁ(i = 0; i < l.classes; ++i){
            ପାଇଁ(k = 0; k < l.w*l.h; ++k){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = b*l.outputs + i*l.w*l.h + k;
                l.delta[index] = 0 - l.output[index];
            }
        }

        // a priori, embedding should be small magnitude
        ପାଇଁ(i = 0; i < ids; ++i){
            ପାଇଁ(k = 0; k < l.w*l.h; ++k){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[index] = .1 * (0 - l.output[index]);
            }
        }


        memset(l.counts, 0, 90*ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
        ପାଇଁ(i = 0; i < 90; ++i){
            fill_cpu(ids, 0, l.sums[i], 1);
            
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            ଯଦି(c < 0) ଭାଙ୍ଗ;
            // add up metric embeddings for each instance
            ପାଇଁ(k = 0; k < l.w*l.h; ++k){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = b*l.outputs + c*l.w*l.h + k;
                ଦଶମିକ v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                ଯଦି(v){
                    l.delta[index] = v - l.output[index];
                    axpy_cpu(ids, 1, l.output + b*l.outputs + l.classes*l.w*l.h + k, l.w*l.h, l.sums[i], 1);
                    ++l.counts[i];
                }
            }
        }

        ଦଶମିକ *mse = calloc(90, ଆକାର(ଦଶମିକ));
        ପାଇଁ(i = 0; i < 90; ++i){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            ଯଦି(c < 0) ଭାଙ୍ଗ;
            ପାଇଁ(k = 0; k < l.w*l.h; ++k){
                ଦଶମିକ v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                ଯଦି(v){
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା z;
                    ଦଶମିକ sum = 0;
                    ପାଇଁ(z = 0; z < ids; ++z){
                        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                        sum += pow(l.sums[i][z]/l.counts[i] - l.output[index], 2);
                    }
                    mse[i] += sum;
                }
            }
            mse[i] /= l.counts[i];
        }

        // Calculate average embedding
        ପାଇଁ(i = 0; i < 90; ++i){
            ଯଦି(!l.counts[i]) ଜାରି;
            scal_cpu(ids, 1.f/l.counts[i], l.sums[i], 1);
            ଯଦି(b == 0 && net.gpu_index == 0){
                printf("%4d, %6.3f, ", l.counts[i], mse[i]);
                ପାଇଁ(j = 0; j < ids; ++j){
                    printf("%6.3f,", l.sums[i][j]);
                }
                printf("\n");
            }
        }
        ମୁକ୍ତ(mse);

        // Calculate embedding loss
        ପାଇଁ(i = 0; i < 90; ++i){
            ଯଦି(!l.counts[i]) ଜାରି;
            ପାଇଁ(k = 0; k < l.w*l.h; ++k){
                ଦଶମିକ v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                ଯଦି(v){
                    ପାଇଁ(j = 0; j < 90; ++j){
                        ଯଦି(!l.counts[j])ଜାରି;
                        ପୂର୍ଣ୍ଣସଂଖ୍ୟା z;
                        ପାଇଁ(z = 0; z < ids; ++z){
                            ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                            ଦଶମିକ diff = l.sums[j][z] - l.output[index];
                            ଯଦି (j == i) l.delta[index] +=   diff < 0? -.1 : .1;
                            ଅନ୍ୟଥା        l.delta[index] += -(diff < 0? -.1 : .1);
                        }
                    }
                }
            }
        }

        ପାଇଁ(i = 0; i < ids; ++i){
            ପାଇଁ(k = 0; k < l.w*l.h; ++k){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[index] *= .01;
            }
        }
    }

    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("took %lf sec\n", what_time_is_it_now() - time);
}

ଶୂନ୍ୟ backward_iseg_layer(ଅଚଳ layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

ଶୂନ୍ୟ forward_iseg_layer_gpu(ଅଚଳ layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.ନିବେଶ_gpu, 1, l.output_gpu, 1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା b;
    ପାଇଁ (b = 0; b < l.batch; ++b){
        activate_array_gpu(l.output_gpu + b*l.outputs, l.classes*l.w*l.h, LOGISTIC);
        //if(l.extra) activate_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC);
    }

    cuda_pull_array(l.output_gpu, net.ନିବେଶ, l.batch*l.inputs);
    forward_iseg_layer(l, net);
    cuda_ଠେଲ_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

ଶୂନ୍ୟ backward_iseg_layer_gpu(ଅଚଳ layer l, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା b;
    ପାଇଁ (b = 0; b < l.batch; ++b){
        //if(l.extra) gradient_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC, l.delta_gpu + b*l.outputs + l.classes*l.w*l.h);
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


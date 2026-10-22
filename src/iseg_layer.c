#அந்தர்பாவ "iseg_layer.h"
#அந்தர்பாவ "activations.h"
#அந்தர்பாவ "blas.h"
#அந்தர்பாவ "box.h"
#அந்தர்பாவ "cuda.h"
#அந்தர்பாவ "utils.h"

#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <stdlib.h>

layer make_iseg_layer(முழு batch, முழு w, முழு h, முழு classes, முழு ids)
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
    l.cost = calloc(1, அளவு(தசம));
    l.outputs = h*w*l.c;
    l.inputs = l.outputs;
    l.truths = 90*(l.w*l.h+1);
    l.delta = calloc(batch*l.outputs, அளவு(தசம));
    l.output = calloc(batch*l.outputs, அளவு(தசம));

    l.counts = calloc(90, அளவு(முழு));
    l.sums = calloc(90, அளவு(தசம*));
    எனில்(ids){
        முழு i;
        க்கு(i = 0; i < 90; ++i){
            l.sums[i] = calloc(ids, அளவு(தசம));
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

    திரும்பு l;
}

வெறுமை resize_iseg_layer(layer *l, முழு w, முழு h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->c;
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*அளவு(தசம));
    l->delta = realloc(l->delta, l->batch*l->outputs*அளவு(தசம));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

வெறுமை forward_iseg_layer(மாறிலி layer l, network net)
{

    இரட்டை time = what_time_is_it_now();
    முழு i,b,j,k;
    முழு ids = l.extra;
    memcpy(l.output, net.உள்ளீடு, l.outputs*l.batch*அளவு(தசம));
    memset(l.delta, 0, l.outputs * l.batch * அளவு(தசம));

#ifndef GPU
    க்கு (b = 0; b < l.batch; ++b){
        முழு index = b*l.outputs;
        activate_array(l.output + index, l.classes*l.w*l.h, LOGISTIC);
    }
#endif

    க்கு (b = 0; b < l.batch; ++b){
        // a priori, each pixel has no class
        க்கு(i = 0; i < l.classes; ++i){
            க்கு(k = 0; k < l.w*l.h; ++k){
                முழு index = b*l.outputs + i*l.w*l.h + k;
                l.delta[index] = 0 - l.output[index];
            }
        }

        // a priori, embedding should be small magnitude
        க்கு(i = 0; i < ids; ++i){
            க்கு(k = 0; k < l.w*l.h; ++k){
                முழு index = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[index] = .1 * (0 - l.output[index]);
            }
        }


        memset(l.counts, 0, 90*அளவு(முழு));
        க்கு(i = 0; i < 90; ++i){
            fill_cpu(ids, 0, l.sums[i], 1);
            
            முழு c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            எனில்(c < 0) நிறுத்து;
            // add up metric embeddings for each instance
            க்கு(k = 0; k < l.w*l.h; ++k){
                முழு index = b*l.outputs + c*l.w*l.h + k;
                தசம v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                எனில்(v){
                    l.delta[index] = v - l.output[index];
                    axpy_cpu(ids, 1, l.output + b*l.outputs + l.classes*l.w*l.h + k, l.w*l.h, l.sums[i], 1);
                    ++l.counts[i];
                }
            }
        }

        தசம *mse = calloc(90, அளவு(தசம));
        க்கு(i = 0; i < 90; ++i){
            முழு c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            எனில்(c < 0) நிறுத்து;
            க்கு(k = 0; k < l.w*l.h; ++k){
                தசம v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                எனில்(v){
                    முழு z;
                    தசம sum = 0;
                    க்கு(z = 0; z < ids; ++z){
                        முழு index = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                        sum += pow(l.sums[i][z]/l.counts[i] - l.output[index], 2);
                    }
                    mse[i] += sum;
                }
            }
            mse[i] /= l.counts[i];
        }

        // Calculate average embedding
        க்கு(i = 0; i < 90; ++i){
            எனில்(!l.counts[i]) தொடர்;
            scal_cpu(ids, 1.f/l.counts[i], l.sums[i], 1);
            எனில்(b == 0 && net.gpu_index == 0){
                printf("%4d, %6.3f, ", l.counts[i], mse[i]);
                க்கு(j = 0; j < ids; ++j){
                    printf("%6.3f,", l.sums[i][j]);
                }
                printf("\n");
            }
        }
        விடுவி(mse);

        // Calculate embedding loss
        க்கு(i = 0; i < 90; ++i){
            எனில்(!l.counts[i]) தொடர்;
            க்கு(k = 0; k < l.w*l.h; ++k){
                தசம v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                எனில்(v){
                    க்கு(j = 0; j < 90; ++j){
                        எனில்(!l.counts[j])தொடர்;
                        முழு z;
                        க்கு(z = 0; z < ids; ++z){
                            முழு index = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                            தசம diff = l.sums[j][z] - l.output[index];
                            எனில் (j == i) l.delta[index] +=   diff < 0? -.1 : .1;
                            இல்லையெனில்        l.delta[index] += -(diff < 0? -.1 : .1);
                        }
                    }
                }
            }
        }

        க்கு(i = 0; i < ids; ++i){
            க்கு(k = 0; k < l.w*l.h; ++k){
                முழு index = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[index] *= .01;
            }
        }
    }

    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("took %lf sec\n", what_time_is_it_now() - time);
}

வெறுமை backward_iseg_layer(மாறிலி layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

வெறுமை forward_iseg_layer_gpu(மாறிலி layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.உள்ளீடு_gpu, 1, l.output_gpu, 1);
    முழு b;
    க்கு (b = 0; b < l.batch; ++b){
        activate_array_gpu(l.output_gpu + b*l.outputs, l.classes*l.w*l.h, LOGISTIC);
        //if(l.extra) activate_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC);
    }

    cuda_pull_array(l.output_gpu, net.உள்ளீடு, l.batch*l.inputs);
    forward_iseg_layer(l, net);
    cuda_தள்ளு_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

வெறுமை backward_iseg_layer_gpu(மாறிலி layer l, network net)
{
    முழு b;
    க்கு (b = 0; b < l.batch; ++b){
        //if(l.extra) gradient_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC, l.delta_gpu + b*l.outputs + l.classes*l.w*l.h);
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


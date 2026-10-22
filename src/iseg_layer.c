#અન્તર્ભાવ "iseg_layer.h"
#અન્તર્ભાવ "activations.h"
#અન્તર્ભાવ "blas.h"
#અન્તર્ભાવ "box.h"
#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ "utils.h"

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <stdlib.h>

layer make_iseg_layer(પૂર્ણાંક batch, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક classes, પૂર્ણાંક ids)
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
    l.cost = calloc(1, કદ(દશાંશ));
    l.outputs = h*w*l.c;
    l.inputs = l.outputs;
    l.truths = 90*(l.w*l.h+1);
    l.delta = calloc(batch*l.outputs, કદ(દશાંશ));
    l.output = calloc(batch*l.outputs, કદ(દશાંશ));

    l.counts = calloc(90, કદ(પૂર્ણાંક));
    l.sums = calloc(90, કદ(દશાંશ*));
    જો(ids){
        પૂર્ણાંક i;
        માટે(i = 0; i < 90; ++i){
            l.sums[i] = calloc(ids, કદ(દશાંશ));
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

    પાછા l;
}

શૂન્ય resize_iseg_layer(layer *l, પૂર્ણાંક w, પૂર્ણાંક h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->c;
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*કદ(દશાંશ));
    l->delta = realloc(l->delta, l->batch*l->outputs*કદ(દશાંશ));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

શૂન્ય forward_iseg_layer(અચલ layer l, network net)
{

    દ્વિગુણ time = what_time_is_it_now();
    પૂર્ણાંક i,b,j,k;
    પૂર્ણાંક ids = l.extra;
    memcpy(l.output, net.આગત, l.outputs*l.batch*કદ(દશાંશ));
    memset(l.delta, 0, l.outputs * l.batch * કદ(દશાંશ));

#ifndef GPU
    માટે (b = 0; b < l.batch; ++b){
        પૂર્ણાંક index = b*l.outputs;
        activate_array(l.output + index, l.classes*l.w*l.h, LOGISTIC);
    }
#endif

    માટે (b = 0; b < l.batch; ++b){
        // a priori, each pixel has no class
        માટે(i = 0; i < l.classes; ++i){
            માટે(k = 0; k < l.w*l.h; ++k){
                પૂર્ણાંક index = b*l.outputs + i*l.w*l.h + k;
                l.delta[index] = 0 - l.output[index];
            }
        }

        // a priori, embedding should be small magnitude
        માટે(i = 0; i < ids; ++i){
            માટે(k = 0; k < l.w*l.h; ++k){
                પૂર્ણાંક index = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[index] = .1 * (0 - l.output[index]);
            }
        }


        memset(l.counts, 0, 90*કદ(પૂર્ણાંક));
        માટે(i = 0; i < 90; ++i){
            fill_cpu(ids, 0, l.sums[i], 1);
            
            પૂર્ણાંક c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            જો(c < 0) તોડો;
            // add up metric embeddings for each instance
            માટે(k = 0; k < l.w*l.h; ++k){
                પૂર્ણાંક index = b*l.outputs + c*l.w*l.h + k;
                દશાંશ v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                જો(v){
                    l.delta[index] = v - l.output[index];
                    axpy_cpu(ids, 1, l.output + b*l.outputs + l.classes*l.w*l.h + k, l.w*l.h, l.sums[i], 1);
                    ++l.counts[i];
                }
            }
        }

        દશાંશ *mse = calloc(90, કદ(દશાંશ));
        માટે(i = 0; i < 90; ++i){
            પૂર્ણાંક c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            જો(c < 0) તોડો;
            માટે(k = 0; k < l.w*l.h; ++k){
                દશાંશ v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                જો(v){
                    પૂર્ણાંક z;
                    દશાંશ sum = 0;
                    માટે(z = 0; z < ids; ++z){
                        પૂર્ણાંક index = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                        sum += pow(l.sums[i][z]/l.counts[i] - l.output[index], 2);
                    }
                    mse[i] += sum;
                }
            }
            mse[i] /= l.counts[i];
        }

        // Calculate average embedding
        માટે(i = 0; i < 90; ++i){
            જો(!l.counts[i]) ચાલુ;
            scal_cpu(ids, 1.f/l.counts[i], l.sums[i], 1);
            જો(b == 0 && net.gpu_index == 0){
                printf("%4d, %6.3f, ", l.counts[i], mse[i]);
                માટે(j = 0; j < ids; ++j){
                    printf("%6.3f,", l.sums[i][j]);
                }
                printf("\n");
            }
        }
        મુક્ત(mse);

        // Calculate embedding loss
        માટે(i = 0; i < 90; ++i){
            જો(!l.counts[i]) ચાલુ;
            માટે(k = 0; k < l.w*l.h; ++k){
                દશાંશ v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                જો(v){
                    માટે(j = 0; j < 90; ++j){
                        જો(!l.counts[j])ચાલુ;
                        પૂર્ણાંક z;
                        માટે(z = 0; z < ids; ++z){
                            પૂર્ણાંક index = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                            દશાંશ diff = l.sums[j][z] - l.output[index];
                            જો (j == i) l.delta[index] +=   diff < 0? -.1 : .1;
                            નહીંતર        l.delta[index] += -(diff < 0? -.1 : .1);
                        }
                    }
                }
            }
        }

        માટે(i = 0; i < ids; ++i){
            માટે(k = 0; k < l.w*l.h; ++k){
                પૂર્ણાંક index = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[index] *= .01;
            }
        }
    }

    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("took %lf sec\n", what_time_is_it_now() - time);
}

શૂન્ય backward_iseg_layer(અચલ layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

શૂન્ય forward_iseg_layer_gpu(અચલ layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.આગત_gpu, 1, l.output_gpu, 1);
    પૂર્ણાંક b;
    માટે (b = 0; b < l.batch; ++b){
        activate_array_gpu(l.output_gpu + b*l.outputs, l.classes*l.w*l.h, LOGISTIC);
        //if(l.extra) activate_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC);
    }

    cuda_pull_array(l.output_gpu, net.આગત, l.batch*l.inputs);
    forward_iseg_layer(l, net);
    cuda_ધકેલો_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

શૂન્ય backward_iseg_layer_gpu(અચલ layer l, network net)
{
    પૂર્ણાંક b;
    માટે (b = 0; b < l.batch; ++b){
        //if(l.extra) gradient_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC, l.delta_gpu + b*l.outputs + l.classes*l.w*l.h);
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


#അന്തര്ഭാവ "iseg_layer.h"
#അന്തര്ഭാവ "activations.h"
#അന്തര്ഭാവ "blas.h"
#അന്തര്ഭാവ "box.h"
#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ "utils.h"

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <stdlib.h>

layer make_iseg_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ classes, പൂർണ്ണസംഖ്യ ids)
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
    l.cost = calloc(1, വലുപ്പം(ദശാംശം));
    l.outputs = h*w*l.c;
    l.inputs = l.outputs;
    l.truths = 90*(l.w*l.h+1);
    l.delta = calloc(batch*l.outputs, വലുപ്പം(ദശാംശം));
    l.output = calloc(batch*l.outputs, വലുപ്പം(ദശാംശം));

    l.counts = calloc(90, വലുപ്പം(പൂർണ്ണസംഖ്യ));
    l.sums = calloc(90, വലുപ്പം(ദശാംശം*));
    എങ്കിൽ(ids){
        പൂർണ്ണസംഖ്യ i;
        വേണ്ടി(i = 0; i < 90; ++i){
            l.sums[i] = calloc(ids, വലുപ്പം(ദശാംശം));
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

    തിരികെ l;
}

ശൂന്യം resize_iseg_layer(layer *l, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->c;
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*വലുപ്പം(ദശാംശം));
    l->delta = realloc(l->delta, l->batch*l->outputs*വലുപ്പം(ദശാംശം));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

ശൂന്യം forward_iseg_layer(സ്ഥിരം layer l, network net)
{

    ഇരട്ട time = what_time_is_it_now();
    പൂർണ്ണസംഖ്യ i,b,j,k;
    പൂർണ്ണസംഖ്യ ids = l.extra;
    memcpy(l.output, net.നിവേശം, l.outputs*l.batch*വലുപ്പം(ദശാംശം));
    memset(l.delta, 0, l.outputs * l.batch * വലുപ്പം(ദശാംശം));

#ifndef GPU
    വേണ്ടി (b = 0; b < l.batch; ++b){
        പൂർണ്ണസംഖ്യ index = b*l.outputs;
        activate_array(l.output + index, l.classes*l.w*l.h, LOGISTIC);
    }
#endif

    വേണ്ടി (b = 0; b < l.batch; ++b){
        // a priori, each pixel has no class
        വേണ്ടി(i = 0; i < l.classes; ++i){
            വേണ്ടി(k = 0; k < l.w*l.h; ++k){
                പൂർണ്ണസംഖ്യ index = b*l.outputs + i*l.w*l.h + k;
                l.delta[index] = 0 - l.output[index];
            }
        }

        // a priori, embedding should be small magnitude
        വേണ്ടി(i = 0; i < ids; ++i){
            വേണ്ടി(k = 0; k < l.w*l.h; ++k){
                പൂർണ്ണസംഖ്യ index = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[index] = .1 * (0 - l.output[index]);
            }
        }


        memset(l.counts, 0, 90*വലുപ്പം(പൂർണ്ണസംഖ്യ));
        വേണ്ടി(i = 0; i < 90; ++i){
            fill_cpu(ids, 0, l.sums[i], 1);
            
            പൂർണ്ണസംഖ്യ c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            എങ്കിൽ(c < 0) നിർത്തുക;
            // add up metric embeddings for each instance
            വേണ്ടി(k = 0; k < l.w*l.h; ++k){
                പൂർണ്ണസംഖ്യ index = b*l.outputs + c*l.w*l.h + k;
                ദശാംശം v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                എങ്കിൽ(v){
                    l.delta[index] = v - l.output[index];
                    axpy_cpu(ids, 1, l.output + b*l.outputs + l.classes*l.w*l.h + k, l.w*l.h, l.sums[i], 1);
                    ++l.counts[i];
                }
            }
        }

        ദശാംശം *mse = calloc(90, വലുപ്പം(ദശാംശം));
        വേണ്ടി(i = 0; i < 90; ++i){
            പൂർണ്ണസംഖ്യ c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            എങ്കിൽ(c < 0) നിർത്തുക;
            വേണ്ടി(k = 0; k < l.w*l.h; ++k){
                ദശാംശം v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                എങ്കിൽ(v){
                    പൂർണ്ണസംഖ്യ z;
                    ദശാംശം sum = 0;
                    വേണ്ടി(z = 0; z < ids; ++z){
                        പൂർണ്ണസംഖ്യ index = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                        sum += pow(l.sums[i][z]/l.counts[i] - l.output[index], 2);
                    }
                    mse[i] += sum;
                }
            }
            mse[i] /= l.counts[i];
        }

        // Calculate average embedding
        വേണ്ടി(i = 0; i < 90; ++i){
            എങ്കിൽ(!l.counts[i]) തുടരുക;
            scal_cpu(ids, 1.f/l.counts[i], l.sums[i], 1);
            എങ്കിൽ(b == 0 && net.gpu_index == 0){
                printf("%4d, %6.3f, ", l.counts[i], mse[i]);
                വേണ്ടി(j = 0; j < ids; ++j){
                    printf("%6.3f,", l.sums[i][j]);
                }
                printf("\n");
            }
        }
        സ്വതന്ത്രം(mse);

        // Calculate embedding loss
        വേണ്ടി(i = 0; i < 90; ++i){
            എങ്കിൽ(!l.counts[i]) തുടരുക;
            വേണ്ടി(k = 0; k < l.w*l.h; ++k){
                ദശാംശം v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                എങ്കിൽ(v){
                    വേണ്ടി(j = 0; j < 90; ++j){
                        എങ്കിൽ(!l.counts[j])തുടരുക;
                        പൂർണ്ണസംഖ്യ z;
                        വേണ്ടി(z = 0; z < ids; ++z){
                            പൂർണ്ണസംഖ്യ index = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                            ദശാംശം diff = l.sums[j][z] - l.output[index];
                            എങ്കിൽ (j == i) l.delta[index] +=   diff < 0? -.1 : .1;
                            അല്ലെങ്കിൽ        l.delta[index] += -(diff < 0? -.1 : .1);
                        }
                    }
                }
            }
        }

        വേണ്ടി(i = 0; i < ids; ++i){
            വേണ്ടി(k = 0; k < l.w*l.h; ++k){
                പൂർണ്ണസംഖ്യ index = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[index] *= .01;
            }
        }
    }

    *(l.cost) = pow(mag_array(l.delta, l.outputs * l.batch), 2);
    printf("took %lf sec\n", what_time_is_it_now() - time);
}

ശൂന്യം backward_iseg_layer(സ്ഥിരം layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

ശൂന്യം forward_iseg_layer_gpu(സ്ഥിരം layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.നിവേശം_gpu, 1, l.output_gpu, 1);
    പൂർണ്ണസംഖ്യ b;
    വേണ്ടി (b = 0; b < l.batch; ++b){
        activate_array_gpu(l.output_gpu + b*l.outputs, l.classes*l.w*l.h, LOGISTIC);
        //if(l.extra) activate_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC);
    }

    cuda_pull_array(l.output_gpu, net.നിവേശം, l.batch*l.inputs);
    forward_iseg_layer(l, net);
    cuda_തള്ളുക_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

ശൂന്യം backward_iseg_layer_gpu(സ്ഥിരം layer l, network net)
{
    പൂർണ്ണസംഖ്യ b;
    വേണ്ടി (b = 0; b < l.batch; ++b){
        //if(l.extra) gradient_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC, l.delta_gpu + b*l.outputs + l.classes*l.w*l.h);
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


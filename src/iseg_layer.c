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
    l.बहिर्गमनम्_w = l.w;
    l.बहिर्गमनम्_h = l.h;
    l.बहिर्गमनम्_c = l.c;
    l.classes = classes;
    l.batch = batch;
    l.extra = ids;
    l.cost = शून्यस्मृतिं_देहि(1, परिमाण(भिन्न));
    l.outputs = h*w*l.c;
    l.inputs = l.outputs;
    l.truths = 90*(l.w*l.h+1);
    l.delta = शून्यस्मृतिं_देहि(batch*l.outputs, परिमाण(भिन्न));
    l.output = शून्यस्मृतिं_देहि(batch*l.outputs, परिमाण(भिन्न));

    l.counts = शून्यस्मृतिं_देहि(90, परिमाण(पूर्णाङ्क));
    l.sums = शून्यस्मृतिं_देहि(90, परिमाण(भिन्न*));
    यदि(ids){
        पूर्णाङ्क i;
        प्रति(i = 0; i < 90; ++i){
            l.sums[i] = शून्यस्मृतिं_देहि(ids, परिमाण(भिन्न));
        }
    }

    l.forward = forward_iseg_layer;
    l.backward = backward_iseg_layer;
#यदि_परिभाषितम् GPU
    l.forward_gpu = forward_iseg_layer_gpu;
    l.backward_gpu = backward_iseg_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#यदि_अन्तः

    fprintf(stderr, "iseg\n");
    बीजं_रोपय(0);

    प्रत्यागम l;
}

शून्य resize_iseg_layer(layer *l, पूर्णाङ्क w, पूर्णाङ्क h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->c;
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*परिमाण(भिन्न));
    l->delta = realloc(l->delta, l->batch*l->outputs*परिमाण(भिन्न));

#यदि_परिभाषितम् GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#यदि_अन्तः
}

शून्य forward_iseg_layer(ध्रुव layer l, जालम् net)
{

    द्विगुण time = what_time_is_it_now();
    पूर्णाङ्क i,b,j,k;
    पूर्णाङ्क ids = l.extra;
    स्मृतिं_प्रतिलिख(l.output, net.निवेशः, l.outputs*l.batch*परिमाण(भिन्न));
    स्मृतिं_पूरय(l.delta, 0, l.outputs * l.batch * परिमाण(भिन्न));

#यदि_अपरिभाषितम् GPU
    प्रति (b = 0; b < l.batch; ++b){
        पूर्णाङ्क सूचकांक = b*l.outputs;
        activate_array(l.output + सूचकांक, l.classes*l.w*l.h, LOGISTIC);
    }
#यदि_अन्तः

    प्रति (b = 0; b < l.batch; ++b){
        // a priori, each pixel has no class
        प्रति(i = 0; i < l.classes; ++i){
            प्रति(k = 0; k < l.w*l.h; ++k){
                पूर्णाङ्क सूचकांक = b*l.outputs + i*l.w*l.h + k;
                l.delta[सूचकांक] = 0 - l.output[सूचकांक];
            }
        }

        // a priori, embedding should be small magnitude
        प्रति(i = 0; i < ids; ++i){
            प्रति(k = 0; k < l.w*l.h; ++k){
                पूर्णाङ्क सूचकांक = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[सूचकांक] = .1 * (0 - l.output[सूचकांक]);
            }
        }


        स्मृतिं_पूरय(l.counts, 0, 90*परिमाण(पूर्णाङ्क));
        प्रति(i = 0; i < 90; ++i){
            fill_cpu(ids, 0, l.sums[i], 1);
            
            पूर्णाङ्क c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            यदि(c < 0) भङ्ग;
            // add up metric embeddings for each instance
            प्रति(k = 0; k < l.w*l.h; ++k){
                पूर्णाङ्क सूचकांक = b*l.outputs + c*l.w*l.h + k;
                भिन्न v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                यदि(v){
                    l.delta[सूचकांक] = v - l.output[सूचकांक];
                    axpy_cpu(ids, 1, l.output + b*l.outputs + l.classes*l.w*l.h + k, l.w*l.h, l.sums[i], 1);
                    ++l.counts[i];
                }
            }
        }

        भिन्न *mse = शून्यस्मृतिं_देहि(90, परिमाण(भिन्न));
        प्रति(i = 0; i < 90; ++i){
            पूर्णाङ्क c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            यदि(c < 0) भङ्ग;
            प्रति(k = 0; k < l.w*l.h; ++k){
                भिन्न v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                यदि(v){
                    पूर्णाङ्क z;
                    भिन्न योग = 0;
                    प्रति(z = 0; z < ids; ++z){
                        पूर्णाङ्क सूचकांक = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                        योग += घातः(l.sums[i][z]/l.counts[i] - l.output[सूचकांक], 2);
                    }
                    mse[i] += योग;
                }
            }
            mse[i] /= l.counts[i];
        }

        // Calculate average embedding
        प्रति(i = 0; i < 90; ++i){
            यदि(!l.counts[i]) अनुवर्त;
            scal_cpu(ids, 1.f/l.counts[i], l.sums[i], 1);
            यदि(b == 0 && net.gpu_सूचकांक == 0){
                प्रारूप_लिख("%4d, %6.3f, ", l.counts[i], mse[i]);
                प्रति(j = 0; j < ids; ++j){
                    प्रारूप_लिख("%6.3f,", l.sums[i][j]);
                }
                प्रारूप_लिख("\n");
            }
        }
        मुक्त(mse);

        // Calculate embedding loss
        प्रति(i = 0; i < 90; ++i){
            यदि(!l.counts[i]) अनुवर्त;
            प्रति(k = 0; k < l.w*l.h; ++k){
                भिन्न v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                यदि(v){
                    प्रति(j = 0; j < 90; ++j){
                        यदि(!l.counts[j])अनुवर्त;
                        पूर्णाङ्क z;
                        प्रति(z = 0; z < ids; ++z){
                            पूर्णाङ्क सूचकांक = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                            भिन्न diff = l.sums[j][z] - l.output[सूचकांक];
                            यदि (j == i) l.delta[सूचकांक] +=   diff < 0? -.1 : .1;
                            अन्यथा        l.delta[सूचकांक] += -(diff < 0? -.1 : .1);
                        }
                    }
                }
            }
        }

        प्रति(i = 0; i < ids; ++i){
            प्रति(k = 0; k < l.w*l.h; ++k){
                पूर्णाङ्क सूचकांक = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[सूचकांक] *= .01;
            }
        }
    }

    *(l.cost) = घातः(mag_array(l.delta, l.outputs * l.batch), 2);
    प्रारूप_लिख("took %lf sec\n", what_time_is_it_now() - time);
}

शून्य backward_iseg_layer(ध्रुव layer l, जालम् net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

#यदि_परिभाषितम् GPU

शून्य forward_iseg_layer_gpu(ध्रुव layer l, जालम् net)
{
    copy_gpu(l.batch*l.inputs, net.निवेशः_gpu, 1, l.output_gpu, 1);
    पूर्णाङ्क b;
    प्रति (b = 0; b < l.batch; ++b){
        activate_array_gpu(l.output_gpu + b*l.outputs, l.classes*l.w*l.h, LOGISTIC);
        //if(l.extra) activate_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC);
    }

    cuda_pull_array(l.output_gpu, net.निवेशः, l.batch*l.inputs);
    forward_iseg_layer(l, net);
    cuda_प्रक्षिप_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

शून्य backward_iseg_layer_gpu(ध्रुव layer l, जालम् net)
{
    पूर्णाङ्क b;
    प्रति (b = 0; b < l.batch; ++b){
        //if(l.extra) gradient_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC, l.delta_gpu + b*l.outputs + l.classes*l.w*l.h);
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#यदि_अन्तः


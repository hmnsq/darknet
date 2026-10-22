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
    l.cost = सुस्मृति(1, परिमाण(भिन्न));
    l.outputs = h*w*l.c;
    l.inputs = l.outputs;
    l.truths = 90*(l.w*l.h+1);
    l.delta = सुस्मृति(batch*l.outputs, परिमाण(भिन्न));
    l.output = सुस्मृति(batch*l.outputs, परिमाण(भिन्न));

    l.counts = सुस्मृति(90, परिमाण(पूर्णाङ्क));
    l.sums = सुस्मृति(90, परिमाण(भिन्न*));
    यदि(ids){
        पूर्णाङ्क i;
        लागि(i = 0; i < 90; ++i){
            l.sums[i] = सुस्मृति(ids, परिमाण(भिन्न));
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

    ख_लिखो(मानक_त्रुटि, "iseg\n");
    बेक्रम(0);

    फर्काऊ l;
}

शून्य resize_iseg_layer(layer *l, पूर्णाङ्क w, पूर्णाङ्क h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->c;
    l->inputs = l->outputs;

    l->output = पुनः_स्मृति(l->output, l->batch*l->outputs*परिमाण(भिन्न));
    l->delta = पुनः_स्मृति(l->delta, l->batch*l->outputs*परिमाण(भिन्न));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

शून्य forward_iseg_layer(अचर layer l, network net)
{

    द्विगुण समय = what_time_is_it_now();
    पूर्णाङ्क i,b,j,k;
    पूर्णाङ्क ids = l.extra;
    स_नकल(l.output, net.आगत, l.outputs*l.batch*परिमाण(भिन्न));
    स_रखो(l.delta, 0, l.outputs * l.batch * परिमाण(भिन्न));

#ifndef GPU
    लागि (b = 0; b < l.batch; ++b){
        पूर्णाङ्क index = b*l.outputs;
        activate_array(l.output + index, l.classes*l.w*l.h, LOGISTIC);
    }
#endif

    लागि (b = 0; b < l.batch; ++b){
        // a priori, each pixel has no class
        लागि(i = 0; i < l.classes; ++i){
            लागि(k = 0; k < l.w*l.h; ++k){
                पूर्णाङ्क index = b*l.outputs + i*l.w*l.h + k;
                l.delta[index] = 0 - l.output[index];
            }
        }

        // a priori, embedding should be small magnitude
        लागि(i = 0; i < ids; ++i){
            लागि(k = 0; k < l.w*l.h; ++k){
                पूर्णाङ्क index = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[index] = .1 * (0 - l.output[index]);
            }
        }


        स_रखो(l.counts, 0, 90*परिमाण(पूर्णाङ्क));
        लागि(i = 0; i < 90; ++i){
            fill_cpu(ids, 0, l.sums[i], 1);
            
            पूर्णाङ्क c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            यदि(c < 0) रोक;
            // add up metric embeddings for each instance
            लागि(k = 0; k < l.w*l.h; ++k){
                पूर्णाङ्क index = b*l.outputs + c*l.w*l.h + k;
                भिन्न v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                यदि(v){
                    l.delta[index] = v - l.output[index];
                    axpy_cpu(ids, 1, l.output + b*l.outputs + l.classes*l.w*l.h + k, l.w*l.h, l.sums[i], 1);
                    ++l.counts[i];
                }
            }
        }

        भिन्न *mse = सुस्मृति(90, परिमाण(भिन्न));
        लागि(i = 0; i < 90; ++i){
            पूर्णाङ्क c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            यदि(c < 0) रोक;
            लागि(k = 0; k < l.w*l.h; ++k){
                भिन्न v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                यदि(v){
                    पूर्णाङ्क z;
                    भिन्न sum = 0;
                    लागि(z = 0; z < ids; ++z){
                        पूर्णाङ्क index = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                        sum += घात(l.sums[i][z]/l.counts[i] - l.output[index], 2);
                    }
                    mse[i] += sum;
                }
            }
            mse[i] /= l.counts[i];
        }

        // Calculate average embedding
        लागि(i = 0; i < 90; ++i){
            यदि(!l.counts[i]) जारी;
            scal_cpu(ids, 1.f/l.counts[i], l.sums[i], 1);
            यदि(b == 0 && net.gpu_index == 0){
                म_लिखो("%4d, %6.3f, ", l.counts[i], mse[i]);
                लागि(j = 0; j < ids; ++j){
                    म_लिखो("%6.3f,", l.sums[i][j]);
                }
                म_लिखो("\n");
            }
        }
        मुक्त(mse);

        // Calculate embedding loss
        लागि(i = 0; i < 90; ++i){
            यदि(!l.counts[i]) जारी;
            लागि(k = 0; k < l.w*l.h; ++k){
                भिन्न v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                यदि(v){
                    लागि(j = 0; j < 90; ++j){
                        यदि(!l.counts[j])जारी;
                        पूर्णाङ्क z;
                        लागि(z = 0; z < ids; ++z){
                            पूर्णाङ्क index = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                            भिन्न diff = l.sums[j][z] - l.output[index];
                            यदि (j == i) l.delta[index] +=   diff < 0? -.1 : .1;
                            अन्यथा        l.delta[index] += -(diff < 0? -.1 : .1);
                        }
                    }
                }
            }
        }

        लागि(i = 0; i < ids; ++i){
            लागि(k = 0; k < l.w*l.h; ++k){
                पूर्णाङ्क index = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[index] *= .01;
            }
        }
    }

    *(l.cost) = घात(mag_array(l.delta, l.outputs * l.batch), 2);
    म_लिखो("took %lf sec\n", what_time_is_it_now() - समय);
}

शून्य backward_iseg_layer(अचर layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

शून्य forward_iseg_layer_gpu(अचर layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.आगत_gpu, 1, l.output_gpu, 1);
    पूर्णाङ्क b;
    लागि (b = 0; b < l.batch; ++b){
        activate_array_gpu(l.output_gpu + b*l.outputs, l.classes*l.w*l.h, LOGISTIC);
        //if(l.extra) activate_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC);
    }

    cuda_pull_array(l.output_gpu, net.आगत, l.batch*l.inputs);
    forward_iseg_layer(l, net);
    cuda_थप_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

शून्य backward_iseg_layer_gpu(अचर layer l, network net)
{
    पूर्णाङ्क b;
    लागि (b = 0; b < l.batch; ++b){
        //if(l.extra) gradient_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC, l.delta_gpu + b*l.outputs + l.classes*l.w*l.h);
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


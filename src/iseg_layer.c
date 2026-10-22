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

layer make_iseg_layer(पूर्णांक batch, पूर्णांक w, पूर्णांक h, पूर्णांक classes, पूर्णांक ids)
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
    l.cost = सुस्मृति(1, आकार(दशांश));
    l.outputs = h*w*l.c;
    l.inputs = l.outputs;
    l.truths = 90*(l.w*l.h+1);
    l.delta = सुस्मृति(batch*l.outputs, आकार(दशांश));
    l.output = सुस्मृति(batch*l.outputs, आकार(दशांश));

    l.counts = सुस्मृति(90, आकार(पूर्णांक));
    l.sums = सुस्मृति(90, आकार(दशांश*));
    जर(ids){
        पूर्णांक i;
        साठी(i = 0; i < 90; ++i){
            l.sums[i] = सुस्मृति(ids, आकार(दशांश));
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

    परत l;
}

शून्य resize_iseg_layer(layer *l, पूर्णांक w, पूर्णांक h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->c;
    l->inputs = l->outputs;

    l->output = पुनः_स्मृति(l->output, l->batch*l->outputs*आकार(दशांश));
    l->delta = पुनः_स्मृति(l->delta, l->batch*l->outputs*आकार(दशांश));

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#endif
}

शून्य forward_iseg_layer(स्थिरांक layer l, network net)
{

    द्विगुण समय = what_time_is_it_now();
    पूर्णांक i,b,j,k;
    पूर्णांक ids = l.extra;
    स_नकल(l.output, net.आदान, l.outputs*l.batch*आकार(दशांश));
    स_रखो(l.delta, 0, l.outputs * l.batch * आकार(दशांश));

#ifndef GPU
    साठी (b = 0; b < l.batch; ++b){
        पूर्णांक index = b*l.outputs;
        activate_array(l.output + index, l.classes*l.w*l.h, LOGISTIC);
    }
#endif

    साठी (b = 0; b < l.batch; ++b){
        // a priori, each pixel has no class
        साठी(i = 0; i < l.classes; ++i){
            साठी(k = 0; k < l.w*l.h; ++k){
                पूर्णांक index = b*l.outputs + i*l.w*l.h + k;
                l.delta[index] = 0 - l.output[index];
            }
        }

        // a priori, embedding should be small magnitude
        साठी(i = 0; i < ids; ++i){
            साठी(k = 0; k < l.w*l.h; ++k){
                पूर्णांक index = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[index] = .1 * (0 - l.output[index]);
            }
        }


        स_रखो(l.counts, 0, 90*आकार(पूर्णांक));
        साठी(i = 0; i < 90; ++i){
            fill_cpu(ids, 0, l.sums[i], 1);
            
            पूर्णांक c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            जर(c < 0) थांबा;
            // add up metric embeddings for each instance
            साठी(k = 0; k < l.w*l.h; ++k){
                पूर्णांक index = b*l.outputs + c*l.w*l.h + k;
                दशांश v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                जर(v){
                    l.delta[index] = v - l.output[index];
                    axpy_cpu(ids, 1, l.output + b*l.outputs + l.classes*l.w*l.h + k, l.w*l.h, l.sums[i], 1);
                    ++l.counts[i];
                }
            }
        }

        दशांश *mse = सुस्मृति(90, आकार(दशांश));
        साठी(i = 0; i < 90; ++i){
            पूर्णांक c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            जर(c < 0) थांबा;
            साठी(k = 0; k < l.w*l.h; ++k){
                दशांश v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                जर(v){
                    पूर्णांक z;
                    दशांश sum = 0;
                    साठी(z = 0; z < ids; ++z){
                        पूर्णांक index = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                        sum += घात(l.sums[i][z]/l.counts[i] - l.output[index], 2);
                    }
                    mse[i] += sum;
                }
            }
            mse[i] /= l.counts[i];
        }

        // Calculate average embedding
        साठी(i = 0; i < 90; ++i){
            जर(!l.counts[i]) सुरूठेवा;
            scal_cpu(ids, 1.f/l.counts[i], l.sums[i], 1);
            जर(b == 0 && net.gpu_index == 0){
                म_लिखो("%4d, %6.3f, ", l.counts[i], mse[i]);
                साठी(j = 0; j < ids; ++j){
                    म_लिखो("%6.3f,", l.sums[i][j]);
                }
                म_लिखो("\n");
            }
        }
        मुक्त(mse);

        // Calculate embedding loss
        साठी(i = 0; i < 90; ++i){
            जर(!l.counts[i]) सुरूठेवा;
            साठी(k = 0; k < l.w*l.h; ++k){
                दशांश v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                जर(v){
                    साठी(j = 0; j < 90; ++j){
                        जर(!l.counts[j])सुरूठेवा;
                        पूर्णांक z;
                        साठी(z = 0; z < ids; ++z){
                            पूर्णांक index = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                            दशांश diff = l.sums[j][z] - l.output[index];
                            जर (j == i) l.delta[index] +=   diff < 0? -.1 : .1;
                            अन्यथा        l.delta[index] += -(diff < 0? -.1 : .1);
                        }
                    }
                }
            }
        }

        साठी(i = 0; i < ids; ++i){
            साठी(k = 0; k < l.w*l.h; ++k){
                पूर्णांक index = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[index] *= .01;
            }
        }
    }

    *(l.cost) = घात(mag_array(l.delta, l.outputs * l.batch), 2);
    म_लिखो("took %lf sec\n", what_time_is_it_now() - समय);
}

शून्य backward_iseg_layer(स्थिरांक layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

शून्य forward_iseg_layer_gpu(स्थिरांक layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.आदान_gpu, 1, l.output_gpu, 1);
    पूर्णांक b;
    साठी (b = 0; b < l.batch; ++b){
        activate_array_gpu(l.output_gpu + b*l.outputs, l.classes*l.w*l.h, LOGISTIC);
        //if(l.extra) activate_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC);
    }

    cuda_pull_array(l.output_gpu, net.आदान, l.batch*l.inputs);
    forward_iseg_layer(l, net);
    cuda_ढकला_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

शून्य backward_iseg_layer_gpu(स्थिरांक layer l, network net)
{
    पूर्णांक b;
    साठी (b = 0; b < l.batch; ++b){
        //if(l.extra) gradient_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC, l.delta_gpu + b*l.outputs + l.classes*l.w*l.h);
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif


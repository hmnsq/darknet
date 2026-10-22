#समावेश "iseg_layer.h"
#समावेश "activations.h"
#समावेश "blas.h"
#समावेश "box.h"
#समावेश "cuda.h"
#समावेश "utils.h"

#समावेश <stdio.h>
#समावेश <assert.h>
#समावेश <string.h>
#समावेश <stdlib.h>

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
    l.cost = सुस्मृति(1, माप(भग्न));
    l.outputs = h*w*l.c;
    l.inputs = l.outputs;
    l.truths = 90*(l.w*l.h+1);
    l.delta = सुस्मृति(batch*l.outputs, माप(भग्न));
    l.output = सुस्मृति(batch*l.outputs, माप(भग्न));

    l.counts = सुस्मृति(90, माप(पूर्णांक));
    l.sums = सुस्मृति(90, माप(भग्न*));
    अगर(ids){
        पूर्णांक i;
        क्रम(i = 0; i < 90; ++i){
            l.sums[i] = सुस्मृति(ids, माप(भग्न));
        }
    }

    l.forward = forward_iseg_layer;
    l.backward = backward_iseg_layer;
#अगर_घोषित GPU
    l.forward_gpu = forward_iseg_layer_gpu;
    l.backward_gpu = backward_iseg_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#पूर्ण_अगर

    ख_लिखो(मानक_त्रुटि, "iseg\n");
    बेक्रम(0);

    वापस l;
}

व्योम resize_iseg_layer(layer *l, पूर्णांक w, पूर्णांक h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->c;
    l->inputs = l->outputs;

    l->output = पुनः_स्मृति(l->output, l->batch*l->outputs*माप(भग्न));
    l->delta = पुनः_स्मृति(l->delta, l->batch*l->outputs*माप(भग्न));

#अगर_घोषित GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#पूर्ण_अगर
}

व्योम forward_iseg_layer(स्थिर layer l, network net)
{

    द्विगुन समय = what_time_is_it_now();
    पूर्णांक i,b,j,k;
    पूर्णांक ids = l.extra;
    स_नकल(l.output, net.निवेश, l.outputs*l.batch*माप(भग्न));
    स_रखो(l.delta, 0, l.outputs * l.batch * माप(भग्न));

#अगर_अघोषित GPU
    क्रम (b = 0; b < l.batch; ++b){
        पूर्णांक सूचकांक = b*l.outputs;
        activate_array(l.output + सूचकांक, l.classes*l.w*l.h, LOGISTIC);
    }
#पूर्ण_अगर

    क्रम (b = 0; b < l.batch; ++b){
        // a priori, each pixel has no class
        क्रम(i = 0; i < l.classes; ++i){
            क्रम(k = 0; k < l.w*l.h; ++k){
                पूर्णांक सूचकांक = b*l.outputs + i*l.w*l.h + k;
                l.delta[सूचकांक] = 0 - l.output[सूचकांक];
            }
        }

        // a priori, embedding should be small magnitude
        क्रम(i = 0; i < ids; ++i){
            क्रम(k = 0; k < l.w*l.h; ++k){
                पूर्णांक सूचकांक = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[सूचकांक] = .1 * (0 - l.output[सूचकांक]);
            }
        }


        स_रखो(l.counts, 0, 90*माप(पूर्णांक));
        क्रम(i = 0; i < 90; ++i){
            fill_cpu(ids, 0, l.sums[i], 1);
            
            पूर्णांक c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            अगर(c < 0) अवरोध;
            // add up metric embeddings for each instance
            क्रम(k = 0; k < l.w*l.h; ++k){
                पूर्णांक सूचकांक = b*l.outputs + c*l.w*l.h + k;
                भग्न v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                अगर(v){
                    l.delta[सूचकांक] = v - l.output[सूचकांक];
                    axpy_cpu(ids, 1, l.output + b*l.outputs + l.classes*l.w*l.h + k, l.w*l.h, l.sums[i], 1);
                    ++l.counts[i];
                }
            }
        }

        भग्न *mse = सुस्मृति(90, माप(भग्न));
        क्रम(i = 0; i < 90; ++i){
            पूर्णांक c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            अगर(c < 0) अवरोध;
            क्रम(k = 0; k < l.w*l.h; ++k){
                भग्न v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                अगर(v){
                    पूर्णांक z;
                    भग्न योग = 0;
                    क्रम(z = 0; z < ids; ++z){
                        पूर्णांक सूचकांक = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                        योग += घात(l.sums[i][z]/l.counts[i] - l.output[सूचकांक], 2);
                    }
                    mse[i] += योग;
                }
            }
            mse[i] /= l.counts[i];
        }

        // Calculate average embedding
        क्रम(i = 0; i < 90; ++i){
            अगर(!l.counts[i]) जारी;
            scal_cpu(ids, 1.f/l.counts[i], l.sums[i], 1);
            अगर(b == 0 && net.gpu_सूचकांक == 0){
                म_लिखो("%4d, %6.3f, ", l.counts[i], mse[i]);
                क्रम(j = 0; j < ids; ++j){
                    म_लिखो("%6.3f,", l.sums[i][j]);
                }
                म_लिखो("\n");
            }
        }
        मुक्त(mse);

        // Calculate embedding loss
        क्रम(i = 0; i < 90; ++i){
            अगर(!l.counts[i]) जारी;
            क्रम(k = 0; k < l.w*l.h; ++k){
                भग्न v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                अगर(v){
                    क्रम(j = 0; j < 90; ++j){
                        अगर(!l.counts[j])जारी;
                        पूर्णांक z;
                        क्रम(z = 0; z < ids; ++z){
                            पूर्णांक सूचकांक = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                            भग्न diff = l.sums[j][z] - l.output[सूचकांक];
                            अगर (j == i) l.delta[सूचकांक] +=   diff < 0? -.1 : .1;
                            अन्यथा        l.delta[सूचकांक] += -(diff < 0? -.1 : .1);
                        }
                    }
                }
            }
        }

        क्रम(i = 0; i < ids; ++i){
            क्रम(k = 0; k < l.w*l.h; ++k){
                पूर्णांक सूचकांक = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[सूचकांक] *= .01;
            }
        }
    }

    *(l.cost) = घात(mag_array(l.delta, l.outputs * l.batch), 2);
    म_लिखो("took %lf sec\n", what_time_is_it_now() - समय);
}

व्योम backward_iseg_layer(स्थिर layer l, network net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

#अगर_घोषित GPU

व्योम forward_iseg_layer_gpu(स्थिर layer l, network net)
{
    copy_gpu(l.batch*l.inputs, net.निवेश_gpu, 1, l.output_gpu, 1);
    पूर्णांक b;
    क्रम (b = 0; b < l.batch; ++b){
        activate_array_gpu(l.output_gpu + b*l.outputs, l.classes*l.w*l.h, LOGISTIC);
        //if(l.extra) activate_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC);
    }

    cuda_pull_array(l.output_gpu, net.निवेश, l.batch*l.inputs);
    forward_iseg_layer(l, net);
    cuda_धकेलो_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

व्योम backward_iseg_layer_gpu(स्थिर layer l, network net)
{
    पूर्णांक b;
    क्रम (b = 0; b < l.batch; ++b){
        //if(l.extra) gradient_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC, l.delta_gpu + b*l.outputs + l.classes*l.w*l.h);
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#पूर्ण_अगर


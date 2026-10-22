#అన్తర్భావ "iseg_layer.h"
#అన్తర్భావ "activations.h"
#అన్తర్భావ "blas.h"
#అన్తర్భావ "box.h"
#అన్తర్భావ "cuda.h"
#అన్తర్భావ "utils.h"

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <assert.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <stdlib.h>

layer make_iseg_layer(పూర్ణాంకం batch, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం classes, పూర్ణాంకం ids)
{
    layer l = {0};
    l.type = ISEG;

    l.h = h;
    l.w = w;
    l.c = classes + ids;
    l.బహిర్గతం_w = l.w;
    l.బహిర్గతం_h = l.h;
    l.బహిర్గతం_c = l.c;
    l.classes = classes;
    l.batch = batch;
    l.extra = ids;
    l.ఖర్చు_2 = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(దశాంశం));
    l.outputs = h*w*l.c;
    l.inputs = l.outputs;
    l.truths = 90*(l.w*l.h+1);
    l.delta = సున్నా_జ్ఞాపకం_ఇవ్వు(batch*l.outputs, పరిమాణం(దశాంశం));
    l.output = సున్నా_జ్ఞాపకం_ఇవ్వు(batch*l.outputs, పరిమాణం(దశాంశం));

    l.counts = సున్నా_జ్ఞాపకం_ఇవ్వు(90, పరిమాణం(పూర్ణాంకం));
    l.sums = సున్నా_జ్ఞాపకం_ఇవ్వు(90, పరిమాణం(దశాంశం*));
    ఒకవేళ(ids){
        పూర్ణాంకం i;
        కొరకు(i = 0; i < 90; ++i){
            l.sums[i] = సున్నా_జ్ఞాపకం_ఇవ్వు(ids, పరిమాణం(దశాంశం));
        }
    }

    l.forward = forward_iseg_layer;
    l.backward = backward_iseg_layer;
#నిర్వచితమైతే GPU
    l.forward_gpu = forward_iseg_layer_gpu;
    l.backward_gpu = backward_iseg_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#అయితే_ముగింపు

    fprintf(stderr, "iseg\n");
    srand(0);

    తిరిగి l;
}

శూన్య resize_iseg_layer(layer *l, పూర్ణాంకం w, పూర్ణాంకం h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->c;
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*పరిమాణం(దశాంశం));
    l->delta = realloc(l->delta, l->batch*l->outputs*పరిమాణం(దశాంశం));

#నిర్వచితమైతే GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#అయితే_ముగింపు
}

శూన్య forward_iseg_layer(స్థిరరాశి layer l, జాలం net)
{

    ద్విగుణ సమయం = what_సమయం_is_it_now();
    పూర్ణాంకం i,b,j,k;
    పూర్ణాంకం ids = l.extra;
    జ్ఞాపకం_నకలు(l.output, net.ఆదానం, l.outputs*l.batch*పరిమాణం(దశాంశం));
    memset(l.delta, 0, l.outputs * l.batch * పరిమాణం(దశాంశం));

#నిర్వచితం_కాకపోతే GPU
    కొరకు (b = 0; b < l.batch; ++b){
        పూర్ణాంకం సూచీ = b*l.outputs;
        activate_array(l.output + సూచీ, l.classes*l.w*l.h, LOGISTIC);
    }
#అయితే_ముగింపు

    కొరకు (b = 0; b < l.batch; ++b){
        // a priori, each pixel has no class
        కొరకు(i = 0; i < l.classes; ++i){
            కొరకు(k = 0; k < l.w*l.h; ++k){
                పూర్ణాంకం సూచీ = b*l.outputs + i*l.w*l.h + k;
                l.delta[సూచీ] = 0 - l.output[సూచీ];
            }
        }

        // a priori, embedding should be small magnitude
        కొరకు(i = 0; i < ids; ++i){
            కొరకు(k = 0; k < l.w*l.h; ++k){
                పూర్ణాంకం సూచీ = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[సూచీ] = .1 * (0 - l.output[సూచీ]);
            }
        }


        memset(l.counts, 0, 90*పరిమాణం(పూర్ణాంకం));
        కొరకు(i = 0; i < 90; ++i){
            fill_cpu(ids, 0, l.sums[i], 1);
            
            పూర్ణాంకం c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            ఒకవేళ(c < 0) ఆపు;
            // add up metric embeddings for each instance
            కొరకు(k = 0; k < l.w*l.h; ++k){
                పూర్ణాంకం సూచీ = b*l.outputs + c*l.w*l.h + k;
                దశాంశం v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                ఒకవేళ(v){
                    l.delta[సూచీ] = v - l.output[సూచీ];
                    axpy_cpu(ids, 1, l.output + b*l.outputs + l.classes*l.w*l.h + k, l.w*l.h, l.sums[i], 1);
                    ++l.counts[i];
                }
            }
        }

        దశాంశం *mse = సున్నా_జ్ఞాపకం_ఇవ్వు(90, పరిమాణం(దశాంశం));
        కొరకు(i = 0; i < 90; ++i){
            పూర్ణాంకం c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            ఒకవేళ(c < 0) ఆపు;
            కొరకు(k = 0; k < l.w*l.h; ++k){
                దశాంశం v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                ఒకవేళ(v){
                    పూర్ణాంకం z;
                    దశాంశం కూడిక = 0;
                    కొరకు(z = 0; z < ids; ++z){
                        పూర్ణాంకం సూచీ = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                        కూడిక += ఘాతం(l.sums[i][z]/l.counts[i] - l.output[సూచీ], 2);
                    }
                    mse[i] += కూడిక;
                }
            }
            mse[i] /= l.counts[i];
        }

        // Calculate average embedding
        కొరకు(i = 0; i < 90; ++i){
            ఒకవేళ(!l.counts[i]) కొనసాగు;
            scal_cpu(ids, 1.f/l.counts[i], l.sums[i], 1);
            ఒకవేళ(b == 0 && net.gpu_సూచీ == 0){
                రాయి("%4d, %6.3f, ", l.counts[i], mse[i]);
                కొరకు(j = 0; j < ids; ++j){
                    రాయి("%6.3f,", l.sums[i][j]);
                }
                రాయి("\n");
            }
        }
        విడుదల(mse);

        // Calculate embedding loss
        కొరకు(i = 0; i < 90; ++i){
            ఒకవేళ(!l.counts[i]) కొనసాగు;
            కొరకు(k = 0; k < l.w*l.h; ++k){
                దశాంశం v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                ఒకవేళ(v){
                    కొరకు(j = 0; j < 90; ++j){
                        ఒకవేళ(!l.counts[j])కొనసాగు;
                        పూర్ణాంకం z;
                        కొరకు(z = 0; z < ids; ++z){
                            పూర్ణాంకం సూచీ = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                            దశాంశం diff = l.sums[j][z] - l.output[సూచీ];
                            ఒకవేళ (j == i) l.delta[సూచీ] +=   diff < 0? -.1 : .1;
                            లేకపోతే        l.delta[సూచీ] += -(diff < 0? -.1 : .1);
                        }
                    }
                }
            }
        }

        కొరకు(i = 0; i < ids; ++i){
            కొరకు(k = 0; k < l.w*l.h; ++k){
                పూర్ణాంకం సూచీ = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[సూచీ] *= .01;
            }
        }
    }

    *(l.ఖర్చు_2) = ఘాతం(mag_array(l.delta, l.outputs * l.batch), 2);
    రాయి("took %lf sec\n", what_సమయం_is_it_now() - సమయం);
}

శూన్య backward_iseg_layer(స్థిరరాశి layer l, జాలం net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

#నిర్వచితమైతే GPU

శూన్య forward_iseg_layer_gpu(స్థిరరాశి layer l, జాలం net)
{
    copy_gpu(l.batch*l.inputs, net.ఆదానం_gpu, 1, l.output_gpu, 1);
    పూర్ణాంకం b;
    కొరకు (b = 0; b < l.batch; ++b){
        activate_array_gpu(l.output_gpu + b*l.outputs, l.classes*l.w*l.h, LOGISTIC);
        //if(l.extra) activate_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC);
    }

    cuda_pull_array(l.output_gpu, net.ఆదానం, l.batch*l.inputs);
    forward_iseg_layer(l, net);
    cuda_నెట్టు_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

శూన్య backward_iseg_layer_gpu(స్థిరరాశి layer l, జాలం net)
{
    పూర్ణాంకం b;
    కొరకు (b = 0; b < l.batch; ++b){
        //if(l.extra) gradient_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC, l.delta_gpu + b*l.outputs + l.classes*l.w*l.h);
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#అయితే_ముగింపు


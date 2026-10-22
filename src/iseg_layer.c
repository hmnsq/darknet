#ಅನ್ತರ್ಭಾವ "iseg_layer.h"
#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "box.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>

layer make_iseg_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ classes, ಪೂರ್ಣಾಂಕ ids)
{
    layer l = {0};
    l.type = ISEG;

    l.h = h;
    l.w = w;
    l.c = classes + ids;
    l.ಹೊರಹೋಗು_w = l.w;
    l.ಹೊರಹೋಗು_h = l.h;
    l.ಹೊರಹೋಗು_c = l.c;
    l.classes = classes;
    l.batch = batch;
    l.extra = ids;
    l.ವೆಚ್ಚ_2 = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.outputs = h*w*l.c;
    l.inputs = l.outputs;
    l.truths = 90*(l.w*l.h+1);
    l.delta = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*l.outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.output = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*l.outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));

    l.counts = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(90, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ));
    l.sums = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(90, ಗಾತ್ರ(ದಶಮಾಂಶ*));
    ಆದರೆ(ids){
        ಪೂರ್ಣಾಂಕ i;
        ಗಾಗಿ(i = 0; i < 90; ++i){
            l.sums[i] = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(ids, ಗಾತ್ರ(ದಶಮಾಂಶ));
        }
    }

    l.forward = forward_iseg_layer;
    l.backward = backward_iseg_layer;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l.forward_gpu = forward_iseg_layer_gpu;
    l.backward_gpu = backward_iseg_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#ಆದರೆ_ಅಂತ್ಯ

    fprintf(stderr, "iseg\n");
    srand(0);

    ಹಿಂತಿರುಗಿ l;
}

ಶೂನ್ಯ resize_iseg_layer(layer *l, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->c;
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*ಗಾತ್ರ(ದಶಮಾಂಶ));
    l->delta = realloc(l->delta, l->batch*l->outputs*ಗಾತ್ರ(ದಶಮಾಂಶ));

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ forward_iseg_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{

    ದ್ವಿಗುಣ ಸಮಯ = what_ಸಮಯ_is_it_now();
    ಪೂರ್ಣಾಂಕ i,b,j,k;
    ಪೂರ್ಣಾಂಕ ids = l.extra;
    ಸ್ಮರಣೆ_ನಕಲು(l.output, net.ಒಳಹರಿವು, l.outputs*l.batch*ಗಾತ್ರ(ದಶಮಾಂಶ));
    memset(l.delta, 0, l.outputs * l.batch * ಗಾತ್ರ(ದಶಮಾಂಶ));

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GPU
    ಗಾಗಿ (b = 0; b < l.batch; ++b){
        ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = b*l.outputs;
        activate_array(l.output + ಸೂಚ್ಯಂಕ, l.classes*l.w*l.h, LOGISTIC);
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಗಾಗಿ (b = 0; b < l.batch; ++b){
        // a priori, each pixel has no class
        ಗಾಗಿ(i = 0; i < l.classes; ++i){
            ಗಾಗಿ(k = 0; k < l.w*l.h; ++k){
                ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = b*l.outputs + i*l.w*l.h + k;
                l.delta[ಸೂಚ್ಯಂಕ] = 0 - l.output[ಸೂಚ್ಯಂಕ];
            }
        }

        // a priori, embedding should be small magnitude
        ಗಾಗಿ(i = 0; i < ids; ++i){
            ಗಾಗಿ(k = 0; k < l.w*l.h; ++k){
                ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[ಸೂಚ್ಯಂಕ] = .1 * (0 - l.output[ಸೂಚ್ಯಂಕ]);
            }
        }


        memset(l.counts, 0, 90*ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ));
        ಗಾಗಿ(i = 0; i < 90; ++i){
            fill_cpu(ids, 0, l.sums[i], 1);
            
            ಪೂರ್ಣಾಂಕ c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            ಆದರೆ(c < 0) ನಿಲ್ಲಿಸು;
            // add up metric embeddings for each instance
            ಗಾಗಿ(k = 0; k < l.w*l.h; ++k){
                ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = b*l.outputs + c*l.w*l.h + k;
                ದಶಮಾಂಶ v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                ಆದರೆ(v){
                    l.delta[ಸೂಚ್ಯಂಕ] = v - l.output[ಸೂಚ್ಯಂಕ];
                    axpy_cpu(ids, 1, l.output + b*l.outputs + l.classes*l.w*l.h + k, l.w*l.h, l.sums[i], 1);
                    ++l.counts[i];
                }
            }
        }

        ದಶಮಾಂಶ *mse = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(90, ಗಾತ್ರ(ದಶಮಾಂಶ));
        ಗಾಗಿ(i = 0; i < 90; ++i){
            ಪೂರ್ಣಾಂಕ c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            ಆದರೆ(c < 0) ನಿಲ್ಲಿಸು;
            ಗಾಗಿ(k = 0; k < l.w*l.h; ++k){
                ದಶಮಾಂಶ v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                ಆದರೆ(v){
                    ಪೂರ್ಣಾಂಕ z;
                    ದಶಮಾಂಶ ಮೊತ್ತ = 0;
                    ಗಾಗಿ(z = 0; z < ids; ++z){
                        ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                        ಮೊತ್ತ += ಘಾತ(l.sums[i][z]/l.counts[i] - l.output[ಸೂಚ್ಯಂಕ], 2);
                    }
                    mse[i] += ಮೊತ್ತ;
                }
            }
            mse[i] /= l.counts[i];
        }

        // Calculate average embedding
        ಗಾಗಿ(i = 0; i < 90; ++i){
            ಆದರೆ(!l.counts[i]) ಮುಂದುವರಿ;
            scal_cpu(ids, 1.f/l.counts[i], l.sums[i], 1);
            ಆದರೆ(b == 0 && net.gpu_ಸೂಚ್ಯಂಕ == 0){
                ಬರೆ("%4d, %6.3f, ", l.counts[i], mse[i]);
                ಗಾಗಿ(j = 0; j < ids; ++j){
                    ಬರೆ("%6.3f,", l.sums[i][j]);
                }
                ಬರೆ("\n");
            }
        }
        ಬಿಡುಗಡೆ(mse);

        // Calculate embedding loss
        ಗಾಗಿ(i = 0; i < 90; ++i){
            ಆದರೆ(!l.counts[i]) ಮುಂದುವರಿ;
            ಗಾಗಿ(k = 0; k < l.w*l.h; ++k){
                ದಶಮಾಂಶ v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                ಆದರೆ(v){
                    ಗಾಗಿ(j = 0; j < 90; ++j){
                        ಆದರೆ(!l.counts[j])ಮುಂದುವರಿ;
                        ಪೂರ್ಣಾಂಕ z;
                        ಗಾಗಿ(z = 0; z < ids; ++z){
                            ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                            ದಶಮಾಂಶ diff = l.sums[j][z] - l.output[ಸೂಚ್ಯಂಕ];
                            ಆದರೆ (j == i) l.delta[ಸೂಚ್ಯಂಕ] +=   diff < 0? -.1 : .1;
                            ಇಲ್ಲದಿದ್ದರೆ        l.delta[ಸೂಚ್ಯಂಕ] += -(diff < 0? -.1 : .1);
                        }
                    }
                }
            }
        }

        ಗಾಗಿ(i = 0; i < ids; ++i){
            ಗಾಗಿ(k = 0; k < l.w*l.h; ++k){
                ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[ಸೂಚ್ಯಂಕ] *= .01;
            }
        }
    }

    *(l.ವೆಚ್ಚ_2) = ಘಾತ(mag_array(l.delta, l.outputs * l.batch), 2);
    ಬರೆ("took %lf sec\n", what_ಸಮಯ_is_it_now() - ಸಮಯ);
}

ಶೂನ್ಯ backward_iseg_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU

ಶೂನ್ಯ forward_iseg_layer_gpu(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{
    copy_gpu(l.batch*l.inputs, net.ಒಳಹರಿವು_gpu, 1, l.output_gpu, 1);
    ಪೂರ್ಣಾಂಕ b;
    ಗಾಗಿ (b = 0; b < l.batch; ++b){
        activate_array_gpu(l.output_gpu + b*l.outputs, l.classes*l.w*l.h, LOGISTIC);
        //if(l.extra) activate_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC);
    }

    cuda_pull_array(l.output_gpu, net.ಒಳಹರಿವು, l.batch*l.inputs);
    forward_iseg_layer(l, net);
    cuda_ತಳ್ಳು_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

ಶೂನ್ಯ backward_iseg_layer_gpu(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ b;
    ಗಾಗಿ (b = 0; b < l.batch; ++b){
        //if(l.extra) gradient_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC, l.delta_gpu + b*l.outputs + l.classes*l.w*l.h);
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#ಆದರೆ_ಅಂತ್ಯ


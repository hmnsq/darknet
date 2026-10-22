#شامل "iseg_layer.h"
#شامل "activations.h"
#شامل "blas.h"
#شامل "box.h"
#شامل "cuda.h"
#شامل "utils.h"

#شامل <stdio.h>
#شامل <assert.h>
#شامل <string.h>
#شامل <stdlib.h>

layer make_iseg_layer(صحیح batch, صحیح w, صحیح h, صحیح classes, صحیح ids)
{
    layer l = {0};
    l.type = ISEG;

    l.h = h;
    l.w = w;
    l.c = classes + ids;
    l.برآمد_w = l.w;
    l.برآمد_h = l.h;
    l.برآمد_c = l.c;
    l.classes = classes;
    l.batch = batch;
    l.extra = ids;
    l.لاگت_2 = صفر_حافظہ_دو(1, حجم(اعشاری));
    l.outputs = h*w*l.c;
    l.inputs = l.outputs;
    l.truths = 90*(l.w*l.h+1);
    l.delta = صفر_حافظہ_دو(batch*l.outputs, حجم(اعشاری));
    l.output = صفر_حافظہ_دو(batch*l.outputs, حجم(اعشاری));

    l.counts = صفر_حافظہ_دو(90, حجم(صحیح));
    l.sums = صفر_حافظہ_دو(90, حجم(اعشاری*));
    اگر(ids){
        صحیح i;
        برائے(i = 0; i < 90; ++i){
            l.sums[i] = صفر_حافظہ_دو(ids, حجم(اعشاری));
        }
    }

    l.forward = forward_iseg_layer;
    l.backward = backward_iseg_layer;
#اگر_تعریف_شدہ GPU
    l.forward_gpu = forward_iseg_layer_gpu;
    l.backward_gpu = backward_iseg_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#اگر_ختم

    fprintf(stderr, "iseg\n");
    srand(0);

    واپس l;
}

خالی resize_iseg_layer(layer *l, صحیح w, صحیح h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->c;
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*حجم(اعشاری));
    l->delta = realloc(l->delta, l->batch*l->outputs*حجم(اعشاری));

#اگر_تعریف_شدہ GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#اگر_ختم
}

خالی forward_iseg_layer(ثابت layer l, جال net)
{

    دوہرا وقت = what_وقت_is_it_now();
    صحیح i,b,j,k;
    صحیح ids = l.extra;
    حافظہ_نقل(l.output, net.داخلہ, l.outputs*l.batch*حجم(اعشاری));
    memset(l.delta, 0, l.outputs * l.batch * حجم(اعشاری));

#اگر_غیر_تعریف_شدہ GPU
    برائے (b = 0; b < l.batch; ++b){
        صحیح اشاریہ = b*l.outputs;
        activate_array(l.output + اشاریہ, l.classes*l.w*l.h, LOGISTIC);
    }
#اگر_ختم

    برائے (b = 0; b < l.batch; ++b){
        // a priori, each pixel has no class
        برائے(i = 0; i < l.classes; ++i){
            برائے(k = 0; k < l.w*l.h; ++k){
                صحیح اشاریہ = b*l.outputs + i*l.w*l.h + k;
                l.delta[اشاریہ] = 0 - l.output[اشاریہ];
            }
        }

        // a priori, embedding should be small magnitude
        برائے(i = 0; i < ids; ++i){
            برائے(k = 0; k < l.w*l.h; ++k){
                صحیح اشاریہ = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[اشاریہ] = .1 * (0 - l.output[اشاریہ]);
            }
        }


        memset(l.counts, 0, 90*حجم(صحیح));
        برائے(i = 0; i < 90; ++i){
            fill_cpu(ids, 0, l.sums[i], 1);
            
            صحیح c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            اگر(c < 0) توڑو;
            // add up metric embeddings for each instance
            برائے(k = 0; k < l.w*l.h; ++k){
                صحیح اشاریہ = b*l.outputs + c*l.w*l.h + k;
                اعشاری v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                اگر(v){
                    l.delta[اشاریہ] = v - l.output[اشاریہ];
                    axpy_cpu(ids, 1, l.output + b*l.outputs + l.classes*l.w*l.h + k, l.w*l.h, l.sums[i], 1);
                    ++l.counts[i];
                }
            }
        }

        اعشاری *mse = صفر_حافظہ_دو(90, حجم(اعشاری));
        برائے(i = 0; i < 90; ++i){
            صحیح c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            اگر(c < 0) توڑو;
            برائے(k = 0; k < l.w*l.h; ++k){
                اعشاری v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                اگر(v){
                    صحیح z;
                    اعشاری مجموع = 0;
                    برائے(z = 0; z < ids; ++z){
                        صحیح اشاریہ = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                        مجموع += قوت(l.sums[i][z]/l.counts[i] - l.output[اشاریہ], 2);
                    }
                    mse[i] += مجموع;
                }
            }
            mse[i] /= l.counts[i];
        }

        // Calculate average embedding
        برائے(i = 0; i < 90; ++i){
            اگر(!l.counts[i]) جاری;
            scal_cpu(ids, 1.f/l.counts[i], l.sums[i], 1);
            اگر(b == 0 && net.gpu_اشاریہ == 0){
                لکھو("%4d, %6.3f, ", l.counts[i], mse[i]);
                برائے(j = 0; j < ids; ++j){
                    لکھو("%6.3f,", l.sums[i][j]);
                }
                لکھو("\n");
            }
        }
        آزاد(mse);

        // Calculate embedding loss
        برائے(i = 0; i < 90; ++i){
            اگر(!l.counts[i]) جاری;
            برائے(k = 0; k < l.w*l.h; ++k){
                اعشاری v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                اگر(v){
                    برائے(j = 0; j < 90; ++j){
                        اگر(!l.counts[j])جاری;
                        صحیح z;
                        برائے(z = 0; z < ids; ++z){
                            صحیح اشاریہ = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                            اعشاری diff = l.sums[j][z] - l.output[اشاریہ];
                            اگر (j == i) l.delta[اشاریہ] +=   diff < 0? -.1 : .1;
                            ورنہ        l.delta[اشاریہ] += -(diff < 0? -.1 : .1);
                        }
                    }
                }
            }
        }

        برائے(i = 0; i < ids; ++i){
            برائے(k = 0; k < l.w*l.h; ++k){
                صحیح اشاریہ = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[اشاریہ] *= .01;
            }
        }
    }

    *(l.لاگت_2) = قوت(mag_array(l.delta, l.outputs * l.batch), 2);
    لکھو("took %lf sec\n", what_وقت_is_it_now() - وقت);
}

خالی backward_iseg_layer(ثابت layer l, جال net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

#اگر_تعریف_شدہ GPU

خالی forward_iseg_layer_gpu(ثابت layer l, جال net)
{
    copy_gpu(l.batch*l.inputs, net.داخلہ_gpu, 1, l.output_gpu, 1);
    صحیح b;
    برائے (b = 0; b < l.batch; ++b){
        activate_array_gpu(l.output_gpu + b*l.outputs, l.classes*l.w*l.h, LOGISTIC);
        //if(l.extra) activate_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC);
    }

    cuda_pull_array(l.output_gpu, net.داخلہ, l.batch*l.inputs);
    forward_iseg_layer(l, net);
    cuda_ڈالو_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

خالی backward_iseg_layer_gpu(ثابت layer l, جال net)
{
    صحیح b;
    برائے (b = 0; b < l.batch; ++b){
        //if(l.extra) gradient_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC, l.delta_gpu + b*l.outputs + l.classes*l.w*l.h);
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#اگر_ختم


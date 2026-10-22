#ضمن "iseg_layer.h"
#ضمن "activations.h"
#ضمن "blas.h"
#ضمن "box.h"
#ضمن "cuda.h"
#ضمن "utils.h"

#ضمن <stdio.h>
#ضمن <assert.h>
#ضمن <string.h>
#ضمن <stdlib.h>

layer make_iseg_layer(صحيح batch, صحيح w, صحيح h, صحيح classes, صحيح ids)
{
    layer l = {0};
    l.type = ISEG;

    l.h = h;
    l.w = w;
    l.c = classes + ids;
    l.باہرلا_w = l.w;
    l.باہرلا_h = l.h;
    l.باہرلا_c = l.c;
    l.classes = classes;
    l.batch = batch;
    l.extra = ids;
    l.لاگت_2 = صفر_حافظہ_دیو(1, حجم(عائم));
    l.outputs = h*w*l.c;
    l.inputs = l.outputs;
    l.truths = 90*(l.w*l.h+1);
    l.delta = صفر_حافظہ_دیو(batch*l.outputs, حجم(عائم));
    l.output = صفر_حافظہ_دیو(batch*l.outputs, حجم(عائم));

    l.counts = صفر_حافظہ_دیو(90, حجم(صحيح));
    l.sums = صفر_حافظہ_دیو(90, حجم(عائم*));
    إذا(ids){
        صحيح i;
        لكل(i = 0; i < 90; ++i){
            l.sums[i] = صفر_حافظہ_دیو(ids, حجم(عائم));
        }
    }

    l.forward = forward_iseg_layer;
    l.backward = backward_iseg_layer;
#جے_تعریف_شدہ GPU
    l.forward_gpu = forward_iseg_layer_gpu;
    l.backward_gpu = backward_iseg_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, batch*l.outputs);
    l.delta_gpu = cuda_make_array(l.delta, batch*l.outputs);
#جے_انت

    fprintf(stderr, "iseg\n");
    srand(0);

    أرجع l;
}

فراغ resize_iseg_layer(layer *l, صحيح w, صحيح h)
{
    l->w = w;
    l->h = h;

    l->outputs = h*w*l->c;
    l->inputs = l->outputs;

    l->output = realloc(l->output, l->batch*l->outputs*حجم(عائم));
    l->delta = realloc(l->delta, l->batch*l->outputs*حجم(عائم));

#جے_تعریف_شدہ GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =     cuda_make_array(l->delta, l->batch*l->outputs);
    l->output_gpu =    cuda_make_array(l->output, l->batch*l->outputs);
#جے_انت
}

فراغ forward_iseg_layer(ثابت layer l, جال net)
{

    مزدوج ویلا = what_ویلا_is_it_now();
    صحيح i,b,j,k;
    صحيح ids = l.extra;
    حافظہ_نقل(l.output, net.داخلہ, l.outputs*l.batch*حجم(عائم));
    memset(l.delta, 0, l.outputs * l.batch * حجم(عائم));

#جے_بنا_تعریف GPU
    لكل (b = 0; b < l.batch; ++b){
        صحيح اشاریہ = b*l.outputs;
        activate_array(l.output + اشاریہ, l.classes*l.w*l.h, LOGISTIC);
    }
#جے_انت

    لكل (b = 0; b < l.batch; ++b){
        // a priori, each pixel has no class
        لكل(i = 0; i < l.classes; ++i){
            لكل(k = 0; k < l.w*l.h; ++k){
                صحيح اشاریہ = b*l.outputs + i*l.w*l.h + k;
                l.delta[اشاریہ] = 0 - l.output[اشاریہ];
            }
        }

        // a priori, embedding should be small magnitude
        لكل(i = 0; i < ids; ++i){
            لكل(k = 0; k < l.w*l.h; ++k){
                صحيح اشاریہ = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[اشاریہ] = .1 * (0 - l.output[اشاریہ]);
            }
        }


        memset(l.counts, 0, 90*حجم(صحيح));
        لكل(i = 0; i < 90; ++i){
            fill_cpu(ids, 0, l.sums[i], 1);
            
            صحيح c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            إذا(c < 0) اكسر;
            // add up metric embeddings for each instance
            لكل(k = 0; k < l.w*l.h; ++k){
                صحيح اشاریہ = b*l.outputs + c*l.w*l.h + k;
                عائم v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                إذا(v){
                    l.delta[اشاریہ] = v - l.output[اشاریہ];
                    axpy_cpu(ids, 1, l.output + b*l.outputs + l.classes*l.w*l.h + k, l.w*l.h, l.sums[i], 1);
                    ++l.counts[i];
                }
            }
        }

        عائم *mse = صفر_حافظہ_دیو(90, حجم(عائم));
        لكل(i = 0; i < 90; ++i){
            صحيح c = net.truth[b*l.truths + i*(l.w*l.h+1)];
            إذا(c < 0) اكسر;
            لكل(k = 0; k < l.w*l.h; ++k){
                عائم v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                إذا(v){
                    صحيح z;
                    عائم جوڑ = 0;
                    لكل(z = 0; z < ids; ++z){
                        صحيح اشاریہ = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                        جوڑ += قوت(l.sums[i][z]/l.counts[i] - l.output[اشاریہ], 2);
                    }
                    mse[i] += جوڑ;
                }
            }
            mse[i] /= l.counts[i];
        }

        // Calculate average embedding
        لكل(i = 0; i < 90; ++i){
            إذا(!l.counts[i]) استمر;
            scal_cpu(ids, 1.f/l.counts[i], l.sums[i], 1);
            إذا(b == 0 && net.gpu_اشاریہ == 0){
                لکھو("%4d, %6.3f, ", l.counts[i], mse[i]);
                لكل(j = 0; j < ids; ++j){
                    لکھو("%6.3f,", l.sums[i][j]);
                }
                لکھو("\n");
            }
        }
        حرر(mse);

        // Calculate embedding loss
        لكل(i = 0; i < 90; ++i){
            إذا(!l.counts[i]) استمر;
            لكل(k = 0; k < l.w*l.h; ++k){
                عائم v = net.truth[b*l.truths + i*(l.w*l.h + 1) + 1 + k];
                إذا(v){
                    لكل(j = 0; j < 90; ++j){
                        إذا(!l.counts[j])استمر;
                        صحيح z;
                        لكل(z = 0; z < ids; ++z){
                            صحيح اشاریہ = b*l.outputs + (l.classes + z)*l.w*l.h + k;
                            عائم diff = l.sums[j][z] - l.output[اشاریہ];
                            إذا (j == i) l.delta[اشاریہ] +=   diff < 0? -.1 : .1;
                            وإلا        l.delta[اشاریہ] += -(diff < 0? -.1 : .1);
                        }
                    }
                }
            }
        }

        لكل(i = 0; i < ids; ++i){
            لكل(k = 0; k < l.w*l.h; ++k){
                صحيح اشاریہ = b*l.outputs + (i+l.classes)*l.w*l.h + k;
                l.delta[اشاریہ] *= .01;
            }
        }
    }

    *(l.لاگت_2) = قوت(mag_array(l.delta, l.outputs * l.batch), 2);
    لکھو("took %lf sec\n", what_ویلا_is_it_now() - ویلا);
}

فراغ backward_iseg_layer(ثابت layer l, جال net)
{
    axpy_cpu(l.batch*l.inputs, 1, l.delta, 1, net.delta, 1);
}

#جے_تعریف_شدہ GPU

فراغ forward_iseg_layer_gpu(ثابت layer l, جال net)
{
    copy_gpu(l.batch*l.inputs, net.داخلہ_gpu, 1, l.output_gpu, 1);
    صحيح b;
    لكل (b = 0; b < l.batch; ++b){
        activate_array_gpu(l.output_gpu + b*l.outputs, l.classes*l.w*l.h, LOGISTIC);
        //if(l.extra) activate_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC);
    }

    cuda_pull_array(l.output_gpu, net.داخلہ, l.batch*l.inputs);
    forward_iseg_layer(l, net);
    cuda_ڈالو_array(l.delta_gpu, l.delta, l.batch*l.outputs);
}

فراغ backward_iseg_layer_gpu(ثابت layer l, جال net)
{
    صحيح b;
    لكل (b = 0; b < l.batch; ++b){
        //if(l.extra) gradient_array_gpu(l.output_gpu + b*l.outputs + l.classes*l.w*l.h, l.extra*l.w*l.h, LOGISTIC, l.delta_gpu + b*l.outputs + l.classes*l.w*l.h);
    }
    axpy_gpu(l.batch*l.inputs, 1, l.delta_gpu, 1, net.delta_gpu, 1);
}
#جے_انت


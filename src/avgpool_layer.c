#ضمن "avgpool_layer.h"
#ضمن "cuda.h"
#ضمن <stdio.h>

avgpool_layer make_avgpool_layer(صحيح batch, صحيح w, صحيح h, صحيح c)
{
    fprintf(stderr, "avg                     %4d x%4d x%4d   ->  %4d\n",  w, h, c, c);
    avgpool_layer l = {0};
    l.type = AVGPOOL;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.باہرلا_w = 1;
    l.باہرلا_h = 1;
    l.باہرلا_c = c;
    l.outputs = l.باہرلا_c;
    l.inputs = h*w*c;
    صحيح output_ناپ = l.outputs * batch;
    l.output =  صفر_حافظہ_دیو(output_ناپ, حجم(عائم));
    l.delta =   صفر_حافظہ_دیو(output_ناپ, حجم(عائم));
    l.forward = forward_avgpool_layer;
    l.backward = backward_avgpool_layer;
    #جے_تعریف_شدہ GPU
    l.forward_gpu = forward_avgpool_layer_gpu;
    l.backward_gpu = backward_avgpool_layer_gpu;
    l.output_gpu  = cuda_make_array(l.output, output_ناپ);
    l.delta_gpu   = cuda_make_array(l.delta, output_ناپ);
    #جے_انت
    أرجع l;
}

فراغ resize_avgpool_layer(avgpool_layer *l, صحيح w, صحيح h)
{
    l->w = w;
    l->h = h;
    l->inputs = h*w*l->c;
}

فراغ forward_avgpool_layer(ثابت avgpool_layer l, جال net)
{
    صحيح b,i,k;

    لكل(b = 0; b < l.batch; ++b){
        لكل(k = 0; k < l.c; ++k){
            صحيح باہرلا_اشاریہ = k + b*l.c;
            l.output[باہرلا_اشاریہ] = 0;
            لكل(i = 0; i < l.h*l.w; ++i){
                صحيح وچ_اشاریہ = i + l.h*l.w*(k + b*l.c);
                l.output[باہرلا_اشاریہ] += net.داخلہ[وچ_اشاریہ];
            }
            l.output[باہرلا_اشاریہ] /= l.h*l.w;
        }
    }
}

فراغ backward_avgpool_layer(ثابت avgpool_layer l, جال net)
{
    صحيح b,i,k;

    لكل(b = 0; b < l.batch; ++b){
        لكل(k = 0; k < l.c; ++k){
            صحيح باہرلا_اشاریہ = k + b*l.c;
            لكل(i = 0; i < l.h*l.w; ++i){
                صحيح وچ_اشاریہ = i + l.h*l.w*(k + b*l.c);
                net.delta[وچ_اشاریہ] += l.delta[باہرلا_اشاریہ] / (l.h*l.w);
            }
        }
    }
}


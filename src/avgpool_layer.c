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
    l.ٻاهريون_w = 1;
    l.ٻاهريون_h = 1;
    l.ٻاهريون_c = c;
    l.outputs = l.ٻاهريون_c;
    l.inputs = h*w*c;
    صحيح output_سائيز = l.outputs * batch;
    l.output =  صفر_يادگيري_ڏيو(output_سائيز, حجم(عائم));
    l.delta =   صفر_يادگيري_ڏيو(output_سائيز, حجم(عائم));
    l.forward = forward_avgpool_layer;
    l.backward = backward_avgpool_layer;
    #جيڪڏهن_وصف_ٿيل GPU
    l.forward_gpu = forward_avgpool_layer_gpu;
    l.backward_gpu = backward_avgpool_layer_gpu;
    l.output_gpu  = cuda_make_array(l.output, output_سائيز);
    l.delta_gpu   = cuda_make_array(l.delta, output_سائيز);
    #جيڪڏهن_پورو
    أرجع l;
}

فراغ resize_avgpool_layer(avgpool_layer *l, صحيح w, صحيح h)
{
    l->w = w;
    l->h = h;
    l->inputs = h*w*l->c;
}

فراغ forward_avgpool_layer(ثابت avgpool_layer l, ڄار net)
{
    صحيح b,i,k;

    لكل(b = 0; b < l.batch; ++b){
        لكل(k = 0; k < l.c; ++k){
            صحيح ٻاهريون_اشاريو = k + b*l.c;
            l.output[ٻاهريون_اشاريو] = 0;
            لكل(i = 0; i < l.h*l.w; ++i){
                صحيح اندريون_اشاريو = i + l.h*l.w*(k + b*l.c);
                l.output[ٻاهريون_اشاريو] += net.داخلا[اندريون_اشاريو];
            }
            l.output[ٻاهريون_اشاريو] /= l.h*l.w;
        }
    }
}

فراغ backward_avgpool_layer(ثابت avgpool_layer l, ڄار net)
{
    صحيح b,i,k;

    لكل(b = 0; b < l.batch; ++b){
        لكل(k = 0; k < l.c; ++k){
            صحيح ٻاهريون_اشاريو = k + b*l.c;
            لكل(i = 0; i < l.h*l.w; ++i){
                صحيح اندريون_اشاريو = i + l.h*l.w*(k + b*l.c);
                net.delta[اندريون_اشاريو] += l.delta[ٻاهريون_اشاريو] / (l.h*l.w);
            }
        }
    }
}


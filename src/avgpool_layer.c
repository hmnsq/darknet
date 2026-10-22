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
    l.بیرون_w = 1;
    l.بیرون_h = 1;
    l.بیرون_c = c;
    l.outputs = l.بیرون_c;
    l.inputs = h*w*c;
    صحيح output_اندازه = l.outputs * batch;
    l.output =  حافظه_صفر_بده(output_اندازه, حجم(عائم));
    l.delta =   حافظه_صفر_بده(output_اندازه, حجم(عائم));
    l.forward = forward_avgpool_layer;
    l.backward = backward_avgpool_layer;
    #اگر_تعریف_شده GPU
    l.forward_gpu = forward_avgpool_layer_gpu;
    l.backward_gpu = backward_avgpool_layer_gpu;
    l.output_gpu  = cuda_make_array(l.output, output_اندازه);
    l.delta_gpu   = cuda_make_array(l.delta, output_اندازه);
    #پایان_اگر
    أرجع l;
}

فراغ resize_avgpool_layer(avgpool_layer *l, صحيح w, صحيح h)
{
    l->w = w;
    l->h = h;
    l->inputs = h*w*l->c;
}

فراغ forward_avgpool_layer(ثابت avgpool_layer l, شبکه net)
{
    صحيح b,i,k;

    لكل(b = 0; b < l.batch; ++b){
        لكل(k = 0; k < l.c; ++k){
            صحيح بیرون_شاخص = k + b*l.c;
            l.output[بیرون_شاخص] = 0;
            لكل(i = 0; i < l.h*l.w; ++i){
                صحيح درون_شاخص = i + l.h*l.w*(k + b*l.c);
                l.output[بیرون_شاخص] += net.درون_داد[درون_شاخص];
            }
            l.output[بیرون_شاخص] /= l.h*l.w;
        }
    }
}

فراغ backward_avgpool_layer(ثابت avgpool_layer l, شبکه net)
{
    صحيح b,i,k;

    لكل(b = 0; b < l.batch; ++b){
        لكل(k = 0; k < l.c; ++k){
            صحيح بیرون_شاخص = k + b*l.c;
            لكل(i = 0; i < l.h*l.w; ++i){
                صحيح درون_شاخص = i + l.h*l.w*(k + b*l.c);
                net.delta[درون_شاخص] += l.delta[بیرون_شاخص] / (l.h*l.w);
            }
        }
    }
}


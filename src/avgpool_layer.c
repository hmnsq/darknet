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
    l.بهر_w = 1;
    l.بهر_h = 1;
    l.بهر_c = c;
    l.outputs = l.بهر_c;
    l.inputs = h*w*c;
    صحيح output_اندازه = l.outputs * batch;
    l.output =  حافظه_صفر_ورکړه(output_اندازه, حجم(عائم));
    l.delta =   حافظه_صفر_ورکړه(output_اندازه, حجم(عائم));
    l.forward = forward_avgpool_layer;
    l.backward = backward_avgpool_layer;
    #که_تعریف_شوی GPU
    l.forward_gpu = forward_avgpool_layer_gpu;
    l.backward_gpu = backward_avgpool_layer_gpu;
    l.output_gpu  = cuda_make_array(l.output, output_اندازه);
    l.delta_gpu   = cuda_make_array(l.delta, output_اندازه);
    #د_که_پای
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
            صحيح بهر_شاخص = k + b*l.c;
            l.output[بهر_شاخص] = 0;
            لكل(i = 0; i < l.h*l.w; ++i){
                صحيح دننه_شاخص = i + l.h*l.w*(k + b*l.c);
                l.output[بهر_شاخص] += net.ننوت[دننه_شاخص];
            }
            l.output[بهر_شاخص] /= l.h*l.w;
        }
    }
}

فراغ backward_avgpool_layer(ثابت avgpool_layer l, شبکه net)
{
    صحيح b,i,k;

    لكل(b = 0; b < l.batch; ++b){
        لكل(k = 0; k < l.c; ++k){
            صحيح بهر_شاخص = k + b*l.c;
            لكل(i = 0; i < l.h*l.w; ++i){
                صحيح دننه_شاخص = i + l.h*l.w*(k + b*l.c);
                net.delta[دننه_شاخص] += l.delta[بهر_شاخص] / (l.h*l.w);
            }
        }
    }
}


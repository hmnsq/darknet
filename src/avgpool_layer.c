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
    l.out_w = 1;
    l.out_h = 1;
    l.out_c = c;
    l.outputs = l.out_c;
    l.inputs = h*w*c;
    صحيح output_size = l.outputs * batch;
    l.output =  calloc(output_size, حجم(عائم));
    l.delta =   calloc(output_size, حجم(عائم));
    l.forward = forward_avgpool_layer;
    l.backward = backward_avgpool_layer;
    #ifdef GPU
    l.forward_gpu = forward_avgpool_layer_gpu;
    l.backward_gpu = backward_avgpool_layer_gpu;
    l.output_gpu  = cuda_make_array(l.output, output_size);
    l.delta_gpu   = cuda_make_array(l.delta, output_size);
    #endif
    أرجع l;
}

فراغ resize_avgpool_layer(avgpool_layer *l, صحيح w, صحيح h)
{
    l->w = w;
    l->h = h;
    l->inputs = h*w*l->c;
}

فراغ forward_avgpool_layer(ثابت avgpool_layer l, network net)
{
    صحيح b,i,k;

    لكل(b = 0; b < l.batch; ++b){
        لكل(k = 0; k < l.c; ++k){
            صحيح out_index = k + b*l.c;
            l.output[out_index] = 0;
            لكل(i = 0; i < l.h*l.w; ++i){
                صحيح in_index = i + l.h*l.w*(k + b*l.c);
                l.output[out_index] += net.input[in_index];
            }
            l.output[out_index] /= l.h*l.w;
        }
    }
}

فراغ backward_avgpool_layer(ثابت avgpool_layer l, network net)
{
    صحيح b,i,k;

    لكل(b = 0; b < l.batch; ++b){
        لكل(k = 0; k < l.c; ++k){
            صحيح out_index = k + b*l.c;
            لكل(i = 0; i < l.h*l.w; ++i){
                صحيح in_index = i + l.h*l.w*(k + b*l.c);
                net.delta[in_index] += l.delta[out_index] / (l.h*l.w);
            }
        }
    }
}


#شامل "avgpool_layer.h"
#شامل "cuda.h"
#شامل <stdio.h>

avgpool_layer make_avgpool_layer(صحیح batch, صحیح w, صحیح h, صحیح c)
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
    صحیح output_size = l.outputs * batch;
    l.output =  calloc(output_size, اندازه(اعشاری));
    l.delta =   calloc(output_size, اندازه(اعشاری));
    l.forward = forward_avgpool_layer;
    l.backward = backward_avgpool_layer;
    #ifdef GPU
    l.forward_gpu = forward_avgpool_layer_gpu;
    l.backward_gpu = backward_avgpool_layer_gpu;
    l.output_gpu  = cuda_make_array(l.output, output_size);
    l.delta_gpu   = cuda_make_array(l.delta, output_size);
    #endif
    بازگشت l;
}

تهی resize_avgpool_layer(avgpool_layer *l, صحیح w, صحیح h)
{
    l->w = w;
    l->h = h;
    l->inputs = h*w*l->c;
}

تهی forward_avgpool_layer(ثابت avgpool_layer l, network net)
{
    صحیح b,i,k;

    برای(b = 0; b < l.batch; ++b){
        برای(k = 0; k < l.c; ++k){
            صحیح out_index = k + b*l.c;
            l.output[out_index] = 0;
            برای(i = 0; i < l.h*l.w; ++i){
                صحیح in_index = i + l.h*l.w*(k + b*l.c);
                l.output[out_index] += net.درون_داد[in_index];
            }
            l.output[out_index] /= l.h*l.w;
        }
    }
}

تهی backward_avgpool_layer(ثابت avgpool_layer l, network net)
{
    صحیح b,i,k;

    برای(b = 0; b < l.batch; ++b){
        برای(k = 0; k < l.c; ++k){
            صحیح out_index = k + b*l.c;
            برای(i = 0; i < l.h*l.w; ++i){
                صحیح in_index = i + l.h*l.w*(k + b*l.c);
                net.delta[in_index] += l.delta[out_index] / (l.h*l.w);
            }
        }
    }
}


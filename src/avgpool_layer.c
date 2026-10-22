#அந்தர்பாவ "avgpool_layer.h"
#அந்தர்பாவ "cuda.h"
#அந்தர்பாவ <stdio.h>

avgpool_layer make_avgpool_layer(முழு batch, முழு w, முழு h, முழு c)
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
    முழு output_size = l.outputs * batch;
    l.output =  calloc(output_size, அளவு(தசம));
    l.delta =   calloc(output_size, அளவு(தசம));
    l.forward = forward_avgpool_layer;
    l.backward = backward_avgpool_layer;
    #ifdef GPU
    l.forward_gpu = forward_avgpool_layer_gpu;
    l.backward_gpu = backward_avgpool_layer_gpu;
    l.output_gpu  = cuda_make_array(l.output, output_size);
    l.delta_gpu   = cuda_make_array(l.delta, output_size);
    #endif
    திரும்பு l;
}

வெறுமை resize_avgpool_layer(avgpool_layer *l, முழு w, முழு h)
{
    l->w = w;
    l->h = h;
    l->inputs = h*w*l->c;
}

வெறுமை forward_avgpool_layer(மாறிலி avgpool_layer l, network net)
{
    முழு b,i,k;

    க்கு(b = 0; b < l.batch; ++b){
        க்கு(k = 0; k < l.c; ++k){
            முழு out_index = k + b*l.c;
            l.output[out_index] = 0;
            க்கு(i = 0; i < l.h*l.w; ++i){
                முழு in_index = i + l.h*l.w*(k + b*l.c);
                l.output[out_index] += net.உள்ளீடு[in_index];
            }
            l.output[out_index] /= l.h*l.w;
        }
    }
}

வெறுமை backward_avgpool_layer(மாறிலி avgpool_layer l, network net)
{
    முழு b,i,k;

    க்கு(b = 0; b < l.batch; ++b){
        க்கு(k = 0; k < l.c; ++k){
            முழு out_index = k + b*l.c;
            க்கு(i = 0; i < l.h*l.w; ++i){
                முழு in_index = i + l.h*l.w*(k + b*l.c);
                net.delta[in_index] += l.delta[out_index] / (l.h*l.w);
            }
        }
    }
}


#अन्तर्भाव "avgpool_layer.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव <stdio.h>

avgpool_layer make_avgpool_layer(पूर्णाङ्क batch, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क c)
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
    पूर्णाङ्क output_size = l.outputs * batch;
    l.output =  calloc(output_size, परिमाण(भिन्न));
    l.delta =   calloc(output_size, परिमाण(भिन्न));
    l.forward = forward_avgpool_layer;
    l.backward = backward_avgpool_layer;
    #ifdef GPU
    l.forward_gpu = forward_avgpool_layer_gpu;
    l.backward_gpu = backward_avgpool_layer_gpu;
    l.output_gpu  = cuda_make_array(l.output, output_size);
    l.delta_gpu   = cuda_make_array(l.delta, output_size);
    #endif
    पटिनिवत्त l;
}

शून्य resize_avgpool_layer(avgpool_layer *l, पूर्णाङ्क w, पूर्णाङ्क h)
{
    l->w = w;
    l->h = h;
    l->inputs = h*w*l->c;
}

शून्य forward_avgpool_layer(ध्रुव avgpool_layer l, network net)
{
    पूर्णाङ्क b,i,k;

    अत्थाय(b = 0; b < l.batch; ++b){
        अत्थाय(k = 0; k < l.c; ++k){
            पूर्णाङ्क out_index = k + b*l.c;
            l.output[out_index] = 0;
            अत्थाय(i = 0; i < l.h*l.w; ++i){
                पूर्णाङ्क in_index = i + l.h*l.w*(k + b*l.c);
                l.output[out_index] += net.निवेशः[in_index];
            }
            l.output[out_index] /= l.h*l.w;
        }
    }
}

शून्य backward_avgpool_layer(ध्रुव avgpool_layer l, network net)
{
    पूर्णाङ्क b,i,k;

    अत्थाय(b = 0; b < l.batch; ++b){
        अत्थाय(k = 0; k < l.c; ++k){
            पूर्णाङ्क out_index = k + b*l.c;
            अत्थाय(i = 0; i < l.h*l.w; ++i){
                पूर्णाङ्क in_index = i + l.h*l.w*(k + b*l.c);
                net.delta[in_index] += l.delta[out_index] / (l.h*l.w);
            }
        }
    }
}


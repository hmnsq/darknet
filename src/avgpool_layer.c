#અન્તર્ભાવ "avgpool_layer.h"
#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ <stdio.h>

avgpool_layer make_avgpool_layer(પૂર્ણાંક batch, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક c)
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
    પૂર્ણાંક output_size = l.outputs * batch;
    l.output =  calloc(output_size, કદ(દશાંશ));
    l.delta =   calloc(output_size, કદ(દશાંશ));
    l.forward = forward_avgpool_layer;
    l.backward = backward_avgpool_layer;
    #ifdef GPU
    l.forward_gpu = forward_avgpool_layer_gpu;
    l.backward_gpu = backward_avgpool_layer_gpu;
    l.output_gpu  = cuda_make_array(l.output, output_size);
    l.delta_gpu   = cuda_make_array(l.delta, output_size);
    #endif
    પાછા l;
}

શૂન્ય resize_avgpool_layer(avgpool_layer *l, પૂર્ણાંક w, પૂર્ણાંક h)
{
    l->w = w;
    l->h = h;
    l->inputs = h*w*l->c;
}

શૂન્ય forward_avgpool_layer(અચલ avgpool_layer l, network net)
{
    પૂર્ણાંક b,i,k;

    માટે(b = 0; b < l.batch; ++b){
        માટે(k = 0; k < l.c; ++k){
            પૂર્ણાંક out_index = k + b*l.c;
            l.output[out_index] = 0;
            માટે(i = 0; i < l.h*l.w; ++i){
                પૂર્ણાંક in_index = i + l.h*l.w*(k + b*l.c);
                l.output[out_index] += net.આગત[in_index];
            }
            l.output[out_index] /= l.h*l.w;
        }
    }
}

શૂન્ય backward_avgpool_layer(અચલ avgpool_layer l, network net)
{
    પૂર્ણાંક b,i,k;

    માટે(b = 0; b < l.batch; ++b){
        માટે(k = 0; k < l.c; ++k){
            પૂર્ણાંક out_index = k + b*l.c;
            માટે(i = 0; i < l.h*l.w; ++i){
                પૂર્ણાંક in_index = i + l.h*l.w*(k + b*l.c);
                net.delta[in_index] += l.delta[out_index] / (l.h*l.w);
            }
        }
    }
}


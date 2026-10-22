#അന്തര്ഭാവ "avgpool_layer.h"
#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ <stdio.h>

avgpool_layer make_avgpool_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ c)
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
    പൂർണ്ണസംഖ്യ output_size = l.outputs * batch;
    l.output =  calloc(output_size, വലുപ്പം(ദശാംശം));
    l.delta =   calloc(output_size, വലുപ്പം(ദശാംശം));
    l.forward = forward_avgpool_layer;
    l.backward = backward_avgpool_layer;
    #ifdef GPU
    l.forward_gpu = forward_avgpool_layer_gpu;
    l.backward_gpu = backward_avgpool_layer_gpu;
    l.output_gpu  = cuda_make_array(l.output, output_size);
    l.delta_gpu   = cuda_make_array(l.delta, output_size);
    #endif
    തിരികെ l;
}

ശൂന്യം resize_avgpool_layer(avgpool_layer *l, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    l->w = w;
    l->h = h;
    l->inputs = h*w*l->c;
}

ശൂന്യം forward_avgpool_layer(സ്ഥിരം avgpool_layer l, network net)
{
    പൂർണ്ണസംഖ്യ b,i,k;

    വേണ്ടി(b = 0; b < l.batch; ++b){
        വേണ്ടി(k = 0; k < l.c; ++k){
            പൂർണ്ണസംഖ്യ out_index = k + b*l.c;
            l.output[out_index] = 0;
            വേണ്ടി(i = 0; i < l.h*l.w; ++i){
                പൂർണ്ണസംഖ്യ in_index = i + l.h*l.w*(k + b*l.c);
                l.output[out_index] += net.നിവേശം[in_index];
            }
            l.output[out_index] /= l.h*l.w;
        }
    }
}

ശൂന്യം backward_avgpool_layer(സ്ഥിരം avgpool_layer l, network net)
{
    പൂർണ്ണസംഖ്യ b,i,k;

    വേണ്ടി(b = 0; b < l.batch; ++b){
        വേണ്ടി(k = 0; k < l.c; ++k){
            പൂർണ്ണസംഖ്യ out_index = k + b*l.c;
            വേണ്ടി(i = 0; i < l.h*l.w; ++i){
                പൂർണ്ണസംഖ്യ in_index = i + l.h*l.w*(k + b*l.c);
                net.delta[in_index] += l.delta[out_index] / (l.h*l.w);
            }
        }
    }
}


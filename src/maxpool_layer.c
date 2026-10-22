#അന്തര്ഭാവ "maxpool_layer.h"
#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ <stdio.h>

image get_maxpool_image(maxpool_layer l)
{
    പൂർണ്ണസംഖ്യ h = l.out_h;
    പൂർണ്ണസംഖ്യ w = l.out_w;
    പൂർണ്ണസംഖ്യ c = l.c;
    തിരികെ float_to_image(w,h,c,l.output);
}

image get_maxpool_delta(maxpool_layer l)
{
    പൂർണ്ണസംഖ്യ h = l.out_h;
    പൂർണ്ണസംഖ്യ w = l.out_w;
    പൂർണ്ണസംഖ്യ c = l.c;
    തിരികെ float_to_image(w,h,c,l.delta);
}

maxpool_layer make_maxpool_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ size, പൂർണ്ണസംഖ്യ stride, പൂർണ്ണസംഖ്യ padding)
{
    maxpool_layer l = {0};
    l.type = MAXPOOL;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.pad = padding;
    l.out_w = (w + padding - size)/stride + 1;
    l.out_h = (h + padding - size)/stride + 1;
    l.out_c = c;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = h*w*c;
    l.size = size;
    l.stride = stride;
    പൂർണ്ണസംഖ്യ output_size = l.out_h * l.out_w * l.out_c * batch;
    l.indexes = calloc(output_size, വലുപ്പം(പൂർണ്ണസംഖ്യ));
    l.output =  calloc(output_size, വലുപ്പം(ദശാംശം));
    l.delta =   calloc(output_size, വലുപ്പം(ദശാംശം));
    l.forward = forward_maxpool_layer;
    l.backward = backward_maxpool_layer;
    #ifdef GPU
    l.forward_gpu = forward_maxpool_layer_gpu;
    l.backward_gpu = backward_maxpool_layer_gpu;
    l.indexes_gpu = cuda_make_int_array(0, output_size);
    l.output_gpu  = cuda_make_array(l.output, output_size);
    l.delta_gpu   = cuda_make_array(l.delta, output_size);
    #endif
    fprintf(stderr, "max          %d x %d / %d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", size, size, stride, w, h, c, l.out_w, l.out_h, l.out_c);
    തിരികെ l;
}

ശൂന്യം resize_maxpool_layer(maxpool_layer *l, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    l->h = h;
    l->w = w;
    l->inputs = h*w*l->c;

    l->out_w = (w + l->pad - l->size)/l->stride + 1;
    l->out_h = (h + l->pad - l->size)/l->stride + 1;
    l->outputs = l->out_w * l->out_h * l->c;
    പൂർണ്ണസംഖ്യ output_size = l->outputs * l->batch;

    l->indexes = realloc(l->indexes, output_size * വലുപ്പം(പൂർണ്ണസംഖ്യ));
    l->output = realloc(l->output, output_size * വലുപ്പം(ദശാംശം));
    l->delta = realloc(l->delta, output_size * വലുപ്പം(ദശാംശം));

    #ifdef GPU
    cuda_free((ദശാംശം *)l->indexes_gpu);
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->indexes_gpu = cuda_make_int_array(0, output_size);
    l->output_gpu  = cuda_make_array(l->output, output_size);
    l->delta_gpu   = cuda_make_array(l->delta,  output_size);
    #endif
}

ശൂന്യം forward_maxpool_layer(സ്ഥിരം maxpool_layer l, network net)
{
    പൂർണ്ണസംഖ്യ b,i,j,k,m,n;
    പൂർണ്ണസംഖ്യ w_offset = -l.pad/2;
    പൂർണ്ണസംഖ്യ h_offset = -l.pad/2;

    പൂർണ്ണസംഖ്യ h = l.out_h;
    പൂർണ്ണസംഖ്യ w = l.out_w;
    പൂർണ്ണസംഖ്യ c = l.c;

    വേണ്ടി(b = 0; b < l.batch; ++b){
        വേണ്ടി(k = 0; k < c; ++k){
            വേണ്ടി(i = 0; i < h; ++i){
                വേണ്ടി(j = 0; j < w; ++j){
                    പൂർണ്ണസംഖ്യ out_index = j + w*(i + h*(k + c*b));
                    ദശാംശം max = -FLT_MAX;
                    പൂർണ്ണസംഖ്യ max_i = -1;
                    വേണ്ടി(n = 0; n < l.size; ++n){
                        വേണ്ടി(m = 0; m < l.size; ++m){
                            പൂർണ്ണസംഖ്യ cur_h = h_offset + i*l.stride + n;
                            പൂർണ്ണസംഖ്യ cur_w = w_offset + j*l.stride + m;
                            പൂർണ്ണസംഖ്യ index = cur_w + l.w*(cur_h + l.h*(k + b*l.c));
                            പൂർണ്ണസംഖ്യ valid = (cur_h >= 0 && cur_h < l.h &&
                                         cur_w >= 0 && cur_w < l.w);
                            ദശാംശം val = (valid != 0) ? net.നിവേശം[index] : -FLT_MAX;
                            max_i = (val > max) ? index : max_i;
                            max   = (val > max) ? val   : max;
                        }
                    }
                    l.output[out_index] = max;
                    l.indexes[out_index] = max_i;
                }
            }
        }
    }
}

ശൂന്യം backward_maxpool_layer(സ്ഥിരം maxpool_layer l, network net)
{
    പൂർണ്ണസംഖ്യ i;
    പൂർണ്ണസംഖ്യ h = l.out_h;
    പൂർണ്ണസംഖ്യ w = l.out_w;
    പൂർണ്ണസംഖ്യ c = l.c;
    വേണ്ടി(i = 0; i < h*w*c*l.batch; ++i){
        പൂർണ്ണസംഖ്യ index = l.indexes[i];
        net.delta[index] += l.delta[i];
    }
}


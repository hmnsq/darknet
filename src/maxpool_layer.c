#અન્તર્ભાવ "maxpool_layer.h"
#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ <stdio.h>

image get_maxpool_image(maxpool_layer l)
{
    પૂર્ણાંક h = l.out_h;
    પૂર્ણાંક w = l.out_w;
    પૂર્ણાંક c = l.c;
    પાછા float_to_image(w,h,c,l.output);
}

image get_maxpool_delta(maxpool_layer l)
{
    પૂર્ણાંક h = l.out_h;
    પૂર્ણાંક w = l.out_w;
    પૂર્ણાંક c = l.c;
    પાછા float_to_image(w,h,c,l.delta);
}

maxpool_layer make_maxpool_layer(પૂર્ણાંક batch, પૂર્ણાંક h, પૂર્ણાંક w, પૂર્ણાંક c, પૂર્ણાંક size, પૂર્ણાંક stride, પૂર્ણાંક padding)
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
    પૂર્ણાંક output_size = l.out_h * l.out_w * l.out_c * batch;
    l.indexes = calloc(output_size, કદ(પૂર્ણાંક));
    l.output =  calloc(output_size, કદ(દશાંશ));
    l.delta =   calloc(output_size, કદ(દશાંશ));
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
    પાછા l;
}

શૂન્ય resize_maxpool_layer(maxpool_layer *l, પૂર્ણાંક w, પૂર્ણાંક h)
{
    l->h = h;
    l->w = w;
    l->inputs = h*w*l->c;

    l->out_w = (w + l->pad - l->size)/l->stride + 1;
    l->out_h = (h + l->pad - l->size)/l->stride + 1;
    l->outputs = l->out_w * l->out_h * l->c;
    પૂર્ણાંક output_size = l->outputs * l->batch;

    l->indexes = realloc(l->indexes, output_size * કદ(પૂર્ણાંક));
    l->output = realloc(l->output, output_size * કદ(દશાંશ));
    l->delta = realloc(l->delta, output_size * કદ(દશાંશ));

    #ifdef GPU
    cuda_free((દશાંશ *)l->indexes_gpu);
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->indexes_gpu = cuda_make_int_array(0, output_size);
    l->output_gpu  = cuda_make_array(l->output, output_size);
    l->delta_gpu   = cuda_make_array(l->delta,  output_size);
    #endif
}

શૂન્ય forward_maxpool_layer(અચલ maxpool_layer l, network net)
{
    પૂર્ણાંક b,i,j,k,m,n;
    પૂર્ણાંક w_offset = -l.pad/2;
    પૂર્ણાંક h_offset = -l.pad/2;

    પૂર્ણાંક h = l.out_h;
    પૂર્ણાંક w = l.out_w;
    પૂર્ણાંક c = l.c;

    માટે(b = 0; b < l.batch; ++b){
        માટે(k = 0; k < c; ++k){
            માટે(i = 0; i < h; ++i){
                માટે(j = 0; j < w; ++j){
                    પૂર્ણાંક out_index = j + w*(i + h*(k + c*b));
                    દશાંશ max = -FLT_MAX;
                    પૂર્ણાંક max_i = -1;
                    માટે(n = 0; n < l.size; ++n){
                        માટે(m = 0; m < l.size; ++m){
                            પૂર્ણાંક cur_h = h_offset + i*l.stride + n;
                            પૂર્ણાંક cur_w = w_offset + j*l.stride + m;
                            પૂર્ણાંક index = cur_w + l.w*(cur_h + l.h*(k + b*l.c));
                            પૂર્ણાંક valid = (cur_h >= 0 && cur_h < l.h &&
                                         cur_w >= 0 && cur_w < l.w);
                            દશાંશ val = (valid != 0) ? net.આગત[index] : -FLT_MAX;
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

શૂન્ય backward_maxpool_layer(અચલ maxpool_layer l, network net)
{
    પૂર્ણાંક i;
    પૂર્ણાંક h = l.out_h;
    પૂર્ણાંક w = l.out_w;
    પૂર્ણાંક c = l.c;
    માટે(i = 0; i < h*w*c*l.batch; ++i){
        પૂર્ણાંક index = l.indexes[i];
        net.delta[index] += l.delta[i];
    }
}


#அந்தர்பாவ "maxpool_layer.h"
#அந்தர்பாவ "cuda.h"
#அந்தர்பாவ <stdio.h>

image get_maxpool_image(maxpool_layer l)
{
    முழு h = l.out_h;
    முழு w = l.out_w;
    முழு c = l.c;
    திரும்பு float_to_image(w,h,c,l.output);
}

image get_maxpool_delta(maxpool_layer l)
{
    முழு h = l.out_h;
    முழு w = l.out_w;
    முழு c = l.c;
    திரும்பு float_to_image(w,h,c,l.delta);
}

maxpool_layer make_maxpool_layer(முழு batch, முழு h, முழு w, முழு c, முழு size, முழு stride, முழு padding)
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
    முழு output_size = l.out_h * l.out_w * l.out_c * batch;
    l.indexes = calloc(output_size, அளவு(முழு));
    l.output =  calloc(output_size, அளவு(தசம));
    l.delta =   calloc(output_size, அளவு(தசம));
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
    திரும்பு l;
}

வெறுமை resize_maxpool_layer(maxpool_layer *l, முழு w, முழு h)
{
    l->h = h;
    l->w = w;
    l->inputs = h*w*l->c;

    l->out_w = (w + l->pad - l->size)/l->stride + 1;
    l->out_h = (h + l->pad - l->size)/l->stride + 1;
    l->outputs = l->out_w * l->out_h * l->c;
    முழு output_size = l->outputs * l->batch;

    l->indexes = realloc(l->indexes, output_size * அளவு(முழு));
    l->output = realloc(l->output, output_size * அளவு(தசம));
    l->delta = realloc(l->delta, output_size * அளவு(தசம));

    #ifdef GPU
    cuda_free((தசம *)l->indexes_gpu);
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->indexes_gpu = cuda_make_int_array(0, output_size);
    l->output_gpu  = cuda_make_array(l->output, output_size);
    l->delta_gpu   = cuda_make_array(l->delta,  output_size);
    #endif
}

வெறுமை forward_maxpool_layer(மாறிலி maxpool_layer l, network net)
{
    முழு b,i,j,k,m,n;
    முழு w_offset = -l.pad/2;
    முழு h_offset = -l.pad/2;

    முழு h = l.out_h;
    முழு w = l.out_w;
    முழு c = l.c;

    க்கு(b = 0; b < l.batch; ++b){
        க்கு(k = 0; k < c; ++k){
            க்கு(i = 0; i < h; ++i){
                க்கு(j = 0; j < w; ++j){
                    முழு out_index = j + w*(i + h*(k + c*b));
                    தசம max = -FLT_MAX;
                    முழு max_i = -1;
                    க்கு(n = 0; n < l.size; ++n){
                        க்கு(m = 0; m < l.size; ++m){
                            முழு cur_h = h_offset + i*l.stride + n;
                            முழு cur_w = w_offset + j*l.stride + m;
                            முழு index = cur_w + l.w*(cur_h + l.h*(k + b*l.c));
                            முழு valid = (cur_h >= 0 && cur_h < l.h &&
                                         cur_w >= 0 && cur_w < l.w);
                            தசம val = (valid != 0) ? net.உள்ளீடு[index] : -FLT_MAX;
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

வெறுமை backward_maxpool_layer(மாறிலி maxpool_layer l, network net)
{
    முழு i;
    முழு h = l.out_h;
    முழு w = l.out_w;
    முழு c = l.c;
    க்கு(i = 0; i < h*w*c*l.batch; ++i){
        முழு index = l.indexes[i];
        net.delta[index] += l.delta[i];
    }
}


#شامل "maxpool_layer.h"
#شامل "cuda.h"
#شامل <stdio.h>

image get_maxpool_image(maxpool_layer l)
{
    صحیح h = l.out_h;
    صحیح w = l.out_w;
    صحیح c = l.c;
    بازگشت float_to_image(w,h,c,l.output);
}

image get_maxpool_delta(maxpool_layer l)
{
    صحیح h = l.out_h;
    صحیح w = l.out_w;
    صحیح c = l.c;
    بازگشت float_to_image(w,h,c,l.delta);
}

maxpool_layer make_maxpool_layer(صحیح batch, صحیح h, صحیح w, صحیح c, صحیح size, صحیح stride, صحیح padding)
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
    صحیح output_size = l.out_h * l.out_w * l.out_c * batch;
    l.indexes = calloc(output_size, اندازه(صحیح));
    l.output =  calloc(output_size, اندازه(اعشاری));
    l.delta =   calloc(output_size, اندازه(اعشاری));
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
    بازگشت l;
}

تهی resize_maxpool_layer(maxpool_layer *l, صحیح w, صحیح h)
{
    l->h = h;
    l->w = w;
    l->inputs = h*w*l->c;

    l->out_w = (w + l->pad - l->size)/l->stride + 1;
    l->out_h = (h + l->pad - l->size)/l->stride + 1;
    l->outputs = l->out_w * l->out_h * l->c;
    صحیح output_size = l->outputs * l->batch;

    l->indexes = realloc(l->indexes, output_size * اندازه(صحیح));
    l->output = realloc(l->output, output_size * اندازه(اعشاری));
    l->delta = realloc(l->delta, output_size * اندازه(اعشاری));

    #ifdef GPU
    cuda_free((اعشاری *)l->indexes_gpu);
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->indexes_gpu = cuda_make_int_array(0, output_size);
    l->output_gpu  = cuda_make_array(l->output, output_size);
    l->delta_gpu   = cuda_make_array(l->delta,  output_size);
    #endif
}

تهی forward_maxpool_layer(ثابت maxpool_layer l, network net)
{
    صحیح b,i,j,k,m,n;
    صحیح w_offset = -l.pad/2;
    صحیح h_offset = -l.pad/2;

    صحیح h = l.out_h;
    صحیح w = l.out_w;
    صحیح c = l.c;

    برای(b = 0; b < l.batch; ++b){
        برای(k = 0; k < c; ++k){
            برای(i = 0; i < h; ++i){
                برای(j = 0; j < w; ++j){
                    صحیح out_index = j + w*(i + h*(k + c*b));
                    اعشاری max = -FLT_MAX;
                    صحیح max_i = -1;
                    برای(n = 0; n < l.size; ++n){
                        برای(m = 0; m < l.size; ++m){
                            صحیح cur_h = h_offset + i*l.stride + n;
                            صحیح cur_w = w_offset + j*l.stride + m;
                            صحیح index = cur_w + l.w*(cur_h + l.h*(k + b*l.c));
                            صحیح valid = (cur_h >= 0 && cur_h < l.h &&
                                         cur_w >= 0 && cur_w < l.w);
                            اعشاری val = (valid != 0) ? net.درون_داد[index] : -FLT_MAX;
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

تهی backward_maxpool_layer(ثابت maxpool_layer l, network net)
{
    صحیح i;
    صحیح h = l.out_h;
    صحیح w = l.out_w;
    صحیح c = l.c;
    برای(i = 0; i < h*w*c*l.batch; ++i){
        صحیح index = l.indexes[i];
        net.delta[index] += l.delta[i];
    }
}


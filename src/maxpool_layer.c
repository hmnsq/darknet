#ضمن "maxpool_layer.h"
#ضمن "cuda.h"
#ضمن <stdio.h>

image get_maxpool_image(maxpool_layer l)
{
    صحيح h = l.out_h;
    صحيح w = l.out_w;
    صحيح c = l.c;
    ܦܢܐ float_to_image(w,h,c,l.output);
}

image get_maxpool_delta(maxpool_layer l)
{
    صحيح h = l.out_h;
    صحيح w = l.out_w;
    صحيح c = l.c;
    ܦܢܐ float_to_image(w,h,c,l.delta);
}

maxpool_layer make_maxpool_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح size, صحيح stride, صحيح padding)
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
    صحيح output_size = l.out_h * l.out_w * l.out_c * batch;
    l.indexes = calloc(output_size, حجم(صحيح));
    l.output =  calloc(output_size, حجم(عائم));
    l.delta =   calloc(output_size, حجم(عائم));
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
    ܦܢܐ l;
}

فراغ resize_maxpool_layer(maxpool_layer *l, صحيح w, صحيح h)
{
    l->h = h;
    l->w = w;
    l->inputs = h*w*l->c;

    l->out_w = (w + l->pad - l->size)/l->stride + 1;
    l->out_h = (h + l->pad - l->size)/l->stride + 1;
    l->outputs = l->out_w * l->out_h * l->c;
    صحيح output_size = l->outputs * l->batch;

    l->indexes = realloc(l->indexes, output_size * حجم(صحيح));
    l->output = realloc(l->output, output_size * حجم(عائم));
    l->delta = realloc(l->delta, output_size * حجم(عائم));

    #ifdef GPU
    cuda_free((عائم *)l->indexes_gpu);
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->indexes_gpu = cuda_make_int_array(0, output_size);
    l->output_gpu  = cuda_make_array(l->output, output_size);
    l->delta_gpu   = cuda_make_array(l->delta,  output_size);
    #endif
}

فراغ forward_maxpool_layer(ثابت maxpool_layer l, network net)
{
    صحيح b,i,j,k,m,n;
    صحيح w_offset = -l.pad/2;
    صحيح h_offset = -l.pad/2;

    صحيح h = l.out_h;
    صحيح w = l.out_w;
    صحيح c = l.c;

    لكل(b = 0; b < l.batch; ++b){
        لكل(k = 0; k < c; ++k){
            لكل(i = 0; i < h; ++i){
                لكل(j = 0; j < w; ++j){
                    صحيح out_index = j + w*(i + h*(k + c*b));
                    عائم max = -FLT_MAX;
                    صحيح max_i = -1;
                    لكل(n = 0; n < l.size; ++n){
                        لكل(m = 0; m < l.size; ++m){
                            صحيح cur_h = h_offset + i*l.stride + n;
                            صحيح cur_w = w_offset + j*l.stride + m;
                            صحيح index = cur_w + l.w*(cur_h + l.h*(k + b*l.c));
                            صحيح valid = (cur_h >= 0 && cur_h < l.h &&
                                         cur_w >= 0 && cur_w < l.w);
                            عائم val = (valid != 0) ? net.input[index] : -FLT_MAX;
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

فراغ backward_maxpool_layer(ثابت maxpool_layer l, network net)
{
    صحيح i;
    صحيح h = l.out_h;
    صحيح w = l.out_w;
    صحيح c = l.c;
    لكل(i = 0; i < h*w*c*l.batch; ++i){
        صحيح index = l.indexes[i];
        net.delta[index] += l.delta[i];
    }
}


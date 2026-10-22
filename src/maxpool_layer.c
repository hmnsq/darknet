#অন্তর্ভাব "maxpool_layer.h"
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব <stdio.h>

image get_maxpool_image(maxpool_layer l)
{
    পূর্ণসংখ্যা h = l.out_h;
    পূর্ণসংখ্যা w = l.out_w;
    পূর্ণসংখ্যা c = l.c;
    ফেরত float_to_image(w,h,c,l.output);
}

image get_maxpool_delta(maxpool_layer l)
{
    পূর্ণসংখ্যা h = l.out_h;
    পূর্ণসংখ্যা w = l.out_w;
    পূর্ণসংখ্যা c = l.c;
    ফেরত float_to_image(w,h,c,l.delta);
}

maxpool_layer make_maxpool_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা h, পূর্ণসংখ্যা w, পূর্ণসংখ্যা c, পূর্ণসংখ্যা আকার_2, পূর্ণসংখ্যা stride, পূর্ণসংখ্যা padding)
{
    maxpool_layer l = {0};
    l.type = MAXPOOL;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.pad = padding;
    l.out_w = (w + padding - আকার_2)/stride + 1;
    l.out_h = (h + padding - আকার_2)/stride + 1;
    l.out_c = c;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = h*w*c;
    l.আকার_2 = আকার_2;
    l.stride = stride;
    পূর্ণসংখ্যা output_আকার = l.out_h * l.out_w * l.out_c * batch;
    l.indexes = calloc(output_আকার, আকার(পূর্ণসংখ্যা));
    l.output =  calloc(output_আকার, আকার(দশমিক));
    l.delta =   calloc(output_আকার, আকার(দশমিক));
    l.forward = forward_maxpool_layer;
    l.backward = backward_maxpool_layer;
    #ifdef GPU
    l.forward_gpu = forward_maxpool_layer_gpu;
    l.backward_gpu = backward_maxpool_layer_gpu;
    l.indexes_gpu = cuda_make_int_array(0, output_আকার);
    l.output_gpu  = cuda_make_array(l.output, output_আকার);
    l.delta_gpu   = cuda_make_array(l.delta, output_আকার);
    #endif
    fprintf(stderr, "max          %d x %d / %d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", আকার_2, আকার_2, stride, w, h, c, l.out_w, l.out_h, l.out_c);
    ফেরত l;
}

শূন্য resize_maxpool_layer(maxpool_layer *l, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    l->h = h;
    l->w = w;
    l->inputs = h*w*l->c;

    l->out_w = (w + l->pad - l->আকার_2)/l->stride + 1;
    l->out_h = (h + l->pad - l->আকার_2)/l->stride + 1;
    l->outputs = l->out_w * l->out_h * l->c;
    পূর্ণসংখ্যা output_আকার = l->outputs * l->batch;

    l->indexes = realloc(l->indexes, output_আকার * আকার(পূর্ণসংখ্যা));
    l->output = realloc(l->output, output_আকার * আকার(দশমিক));
    l->delta = realloc(l->delta, output_আকার * আকার(দশমিক));

    #ifdef GPU
    cuda_free((দশমিক *)l->indexes_gpu);
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->indexes_gpu = cuda_make_int_array(0, output_আকার);
    l->output_gpu  = cuda_make_array(l->output, output_আকার);
    l->delta_gpu   = cuda_make_array(l->delta,  output_আকার);
    #endif
}

শূন্য forward_maxpool_layer(ধ্রুবক maxpool_layer l, network net)
{
    পূর্ণসংখ্যা b,i,j,k,m,n;
    পূর্ণসংখ্যা w_offset = -l.pad/2;
    পূর্ণসংখ্যা h_offset = -l.pad/2;

    পূর্ণসংখ্যা h = l.out_h;
    পূর্ণসংখ্যা w = l.out_w;
    পূর্ণসংখ্যা c = l.c;

    জন্য(b = 0; b < l.batch; ++b){
        জন্য(k = 0; k < c; ++k){
            জন্য(i = 0; i < h; ++i){
                জন্য(j = 0; j < w; ++j){
                    পূর্ণসংখ্যা out_সূচক = j + w*(i + h*(k + c*b));
                    দশমিক max = -FLT_MAX;
                    পূর্ণসংখ্যা max_i = -1;
                    জন্য(n = 0; n < l.আকার_2; ++n){
                        জন্য(m = 0; m < l.আকার_2; ++m){
                            পূর্ণসংখ্যা cur_h = h_offset + i*l.stride + n;
                            পূর্ণসংখ্যা cur_w = w_offset + j*l.stride + m;
                            পূর্ণসংখ্যা সূচক = cur_w + l.w*(cur_h + l.h*(k + b*l.c));
                            পূর্ণসংখ্যা valid = (cur_h >= 0 && cur_h < l.h &&
                                         cur_w >= 0 && cur_w < l.w);
                            দশমিক val = (valid != 0) ? net.প্রবেশ[সূচক] : -FLT_MAX;
                            max_i = (val > max) ? সূচক : max_i;
                            max   = (val > max) ? val   : max;
                        }
                    }
                    l.output[out_সূচক] = max;
                    l.indexes[out_সূচক] = max_i;
                }
            }
        }
    }
}

শূন্য backward_maxpool_layer(ধ্রুবক maxpool_layer l, network net)
{
    পূর্ণসংখ্যা i;
    পূর্ণসংখ্যা h = l.out_h;
    পূর্ণসংখ্যা w = l.out_w;
    পূর্ণসংখ্যা c = l.c;
    জন্য(i = 0; i < h*w*c*l.batch; ++i){
        পূর্ণসংখ্যা সূচক = l.indexes[i];
        net.delta[সূচক] += l.delta[i];
    }
}


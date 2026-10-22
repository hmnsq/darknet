#অন্তর্ভাব "maxpool_layer.h"
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব <stdio.h>

image get_maxpool_image(maxpool_layer l)
{
    পূর্ণাঙ্ক h = l.out_h;
    পূর্ণাঙ্ক w = l.out_w;
    পূর্ণাঙ্ক c = l.c;
    প্রত্যাগম float_to_image(w,h,c,l.output);
}

image get_maxpool_delta(maxpool_layer l)
{
    পূর্ণাঙ্ক h = l.out_h;
    পূর্ণাঙ্ক w = l.out_w;
    পূর্ণাঙ্ক c = l.c;
    প্রত্যাগম float_to_image(w,h,c,l.delta);
}

maxpool_layer make_maxpool_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক h, পূর্ণাঙ্ক w, পূর্ণাঙ্ক c, পূর্ণাঙ্ক size, পূর্ণাঙ্ক stride, পূর্ণাঙ্ক padding)
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
    পূর্ণাঙ্ক output_size = l.out_h * l.out_w * l.out_c * batch;
    l.indexes = calloc(output_size, পরিমাণ(পূর্ণাঙ্ক));
    l.output =  calloc(output_size, পরিমাণ(ভিন্ন));
    l.delta =   calloc(output_size, পরিমাণ(ভিন্ন));
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
    প্রত্যাগম l;
}

শূন্য resize_maxpool_layer(maxpool_layer *l, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h)
{
    l->h = h;
    l->w = w;
    l->inputs = h*w*l->c;

    l->out_w = (w + l->pad - l->size)/l->stride + 1;
    l->out_h = (h + l->pad - l->size)/l->stride + 1;
    l->outputs = l->out_w * l->out_h * l->c;
    পূর্ণাঙ্ক output_size = l->outputs * l->batch;

    l->indexes = realloc(l->indexes, output_size * পরিমাণ(পূর্ণাঙ্ক));
    l->output = realloc(l->output, output_size * পরিমাণ(ভিন্ন));
    l->delta = realloc(l->delta, output_size * পরিমাণ(ভিন্ন));

    #ifdef GPU
    cuda_free((ভিন্ন *)l->indexes_gpu);
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->indexes_gpu = cuda_make_int_array(0, output_size);
    l->output_gpu  = cuda_make_array(l->output, output_size);
    l->delta_gpu   = cuda_make_array(l->delta,  output_size);
    #endif
}

শূন্য forward_maxpool_layer(ধ্রুব maxpool_layer l, network net)
{
    পূর্ণাঙ্ক b,i,j,k,m,n;
    পূর্ণাঙ্ক w_offset = -l.pad/2;
    পূর্ণাঙ্ক h_offset = -l.pad/2;

    পূর্ণাঙ্ক h = l.out_h;
    পূর্ণাঙ্ক w = l.out_w;
    পূর্ণাঙ্ক c = l.c;

    প্রতি(b = 0; b < l.batch; ++b){
        প্রতি(k = 0; k < c; ++k){
            প্রতি(i = 0; i < h; ++i){
                প্রতি(j = 0; j < w; ++j){
                    পূর্ণাঙ্ক out_index = j + w*(i + h*(k + c*b));
                    ভিন্ন max = -FLT_MAX;
                    পূর্ণাঙ্ক max_i = -1;
                    প্রতি(n = 0; n < l.size; ++n){
                        প্রতি(m = 0; m < l.size; ++m){
                            পূর্ণাঙ্ক cur_h = h_offset + i*l.stride + n;
                            পূর্ণাঙ্ক cur_w = w_offset + j*l.stride + m;
                            পূর্ণাঙ্ক index = cur_w + l.w*(cur_h + l.h*(k + b*l.c));
                            পূর্ণাঙ্ক valid = (cur_h >= 0 && cur_h < l.h &&
                                         cur_w >= 0 && cur_w < l.w);
                            ভিন্ন val = (valid != 0) ? net.প্রবেশ[index] : -FLT_MAX;
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

শূন্য backward_maxpool_layer(ধ্রুব maxpool_layer l, network net)
{
    পূর্ণাঙ্ক i;
    পূর্ণাঙ্ক h = l.out_h;
    পূর্ণাঙ্ক w = l.out_w;
    পূর্ণাঙ্ক c = l.c;
    প্রতি(i = 0; i < h*w*c*l.batch; ++i){
        পূর্ণাঙ্ক index = l.indexes[i];
        net.delta[index] += l.delta[i];
    }
}


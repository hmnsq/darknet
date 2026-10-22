#अन्तर्भाव "maxpool_layer.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव <stdio.h>

image get_maxpool_image(maxpool_layer l)
{
    पूर्णाङ्क h = l.out_h;
    पूर्णाङ्क w = l.out_w;
    पूर्णाङ्क c = l.c;
    प्रत्यागम float_to_image(w,h,c,l.output);
}

image get_maxpool_delta(maxpool_layer l)
{
    पूर्णाङ्क h = l.out_h;
    पूर्णाङ्क w = l.out_w;
    पूर्णाङ्क c = l.c;
    प्रत्यागम float_to_image(w,h,c,l.delta);
}

maxpool_layer make_maxpool_layer(पूर्णाङ्क batch, पूर्णाङ्क h, पूर्णाङ्क w, पूर्णाङ्क c, पूर्णाङ्क size, पूर्णाङ्क stride, पूर्णाङ्क padding)
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
    पूर्णाङ्क output_size = l.out_h * l.out_w * l.out_c * batch;
    l.indexes = calloc(output_size, परिमाण(पूर्णाङ्क));
    l.output =  calloc(output_size, परिमाण(भिन्न));
    l.delta =   calloc(output_size, परिमाण(भिन्न));
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
    प्रत्यागम l;
}

शून्य resize_maxpool_layer(maxpool_layer *l, पूर्णाङ्क w, पूर्णाङ्क h)
{
    l->h = h;
    l->w = w;
    l->inputs = h*w*l->c;

    l->out_w = (w + l->pad - l->size)/l->stride + 1;
    l->out_h = (h + l->pad - l->size)/l->stride + 1;
    l->outputs = l->out_w * l->out_h * l->c;
    पूर्णाङ्क output_size = l->outputs * l->batch;

    l->indexes = realloc(l->indexes, output_size * परिमाण(पूर्णाङ्क));
    l->output = realloc(l->output, output_size * परिमाण(भिन्न));
    l->delta = realloc(l->delta, output_size * परिमाण(भिन्न));

    #ifdef GPU
    cuda_free((भिन्न *)l->indexes_gpu);
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->indexes_gpu = cuda_make_int_array(0, output_size);
    l->output_gpu  = cuda_make_array(l->output, output_size);
    l->delta_gpu   = cuda_make_array(l->delta,  output_size);
    #endif
}

शून्य forward_maxpool_layer(ध्रुव maxpool_layer l, network net)
{
    पूर्णाङ्क b,i,j,k,m,n;
    पूर्णाङ्क w_offset = -l.pad/2;
    पूर्णाङ्क h_offset = -l.pad/2;

    पूर्णाङ्क h = l.out_h;
    पूर्णाङ्क w = l.out_w;
    पूर्णाङ्क c = l.c;

    प्रति(b = 0; b < l.batch; ++b){
        प्रति(k = 0; k < c; ++k){
            प्रति(i = 0; i < h; ++i){
                प्रति(j = 0; j < w; ++j){
                    पूर्णाङ्क out_index = j + w*(i + h*(k + c*b));
                    भिन्न max = -FLT_MAX;
                    पूर्णाङ्क max_i = -1;
                    प्रति(n = 0; n < l.size; ++n){
                        प्रति(m = 0; m < l.size; ++m){
                            पूर्णाङ्क cur_h = h_offset + i*l.stride + n;
                            पूर्णाङ्क cur_w = w_offset + j*l.stride + m;
                            पूर्णाङ्क index = cur_w + l.w*(cur_h + l.h*(k + b*l.c));
                            पूर्णाङ्क valid = (cur_h >= 0 && cur_h < l.h &&
                                         cur_w >= 0 && cur_w < l.w);
                            भिन्न val = (valid != 0) ? net.निवेश[index] : -FLT_MAX;
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

शून्य backward_maxpool_layer(ध्रुव maxpool_layer l, network net)
{
    पूर्णाङ्क i;
    पूर्णाङ्क h = l.out_h;
    पूर्णाङ्क w = l.out_w;
    पूर्णाङ्क c = l.c;
    प्रति(i = 0; i < h*w*c*l.batch; ++i){
        पूर्णाङ्क index = l.indexes[i];
        net.delta[index] += l.delta[i];
    }
}


#include "maxpool_layer.h"
#include "cuda.h"
#include <stdio.h>

image get_maxpool_image(maxpool_layer l)
{
    จำนวนเต็ม h = l.out_h;
    จำนวนเต็ม w = l.out_w;
    จำนวนเต็ม c = l.c;
    คืน float_to_image(w,h,c,l.output);
}

image get_maxpool_delta(maxpool_layer l)
{
    จำนวนเต็ม h = l.out_h;
    จำนวนเต็ม w = l.out_w;
    จำนวนเต็ม c = l.c;
    คืน float_to_image(w,h,c,l.delta);
}

maxpool_layer make_maxpool_layer(จำนวนเต็ม batch, จำนวนเต็ม h, จำนวนเต็ม w, จำนวนเต็ม c, จำนวนเต็ม size, จำนวนเต็ม stride, จำนวนเต็ม padding)
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
    จำนวนเต็ม output_size = l.out_h * l.out_w * l.out_c * batch;
    l.indexes = calloc(output_size, parimaa_na(จำนวนเต็ม));
    l.output =  calloc(output_size, parimaa_na(float));
    l.delta =   calloc(output_size, parimaa_na(float));
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
    คืน l;
}

void resize_maxpool_layer(maxpool_layer *l, จำนวนเต็ม w, จำนวนเต็ม h)
{
    l->h = h;
    l->w = w;
    l->inputs = h*w*l->c;

    l->out_w = (w + l->pad - l->size)/l->stride + 1;
    l->out_h = (h + l->pad - l->size)/l->stride + 1;
    l->outputs = l->out_w * l->out_h * l->c;
    จำนวนเต็ม output_size = l->outputs * l->batch;

    l->indexes = realloc(l->indexes, output_size * parimaa_na(จำนวนเต็ม));
    l->output = realloc(l->output, output_size * parimaa_na(float));
    l->delta = realloc(l->delta, output_size * parimaa_na(float));

    #ifdef GPU
    cuda_free((float *)l->indexes_gpu);
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->indexes_gpu = cuda_make_int_array(0, output_size);
    l->output_gpu  = cuda_make_array(l->output, output_size);
    l->delta_gpu   = cuda_make_array(l->delta,  output_size);
    #endif
}

void forward_maxpool_layer(const maxpool_layer l, network net)
{
    จำนวนเต็ม b,i,j,k,m,n;
    จำนวนเต็ม w_offset = -l.pad/2;
    จำนวนเต็ม h_offset = -l.pad/2;

    จำนวนเต็ม h = l.out_h;
    จำนวนเต็ม w = l.out_w;
    จำนวนเต็ม c = l.c;

    สำหรับ(b = 0; b < l.batch; ++b){
        สำหรับ(k = 0; k < c; ++k){
            สำหรับ(i = 0; i < h; ++i){
                สำหรับ(j = 0; j < w; ++j){
                    จำนวนเต็ม out_index = j + w*(i + h*(k + c*b));
                    float max = -FLT_MAX;
                    จำนวนเต็ม max_i = -1;
                    สำหรับ(n = 0; n < l.size; ++n){
                        สำหรับ(m = 0; m < l.size; ++m){
                            จำนวนเต็ม cur_h = h_offset + i*l.stride + n;
                            จำนวนเต็ม cur_w = w_offset + j*l.stride + m;
                            จำนวนเต็ม index = cur_w + l.w*(cur_h + l.h*(k + b*l.c));
                            จำนวนเต็ม valid = (cur_h >= 0 && cur_h < l.h &&
                                         cur_w >= 0 && cur_w < l.w);
                            float val = (valid != 0) ? net.input[index] : -FLT_MAX;
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

void backward_maxpool_layer(const maxpool_layer l, network net)
{
    จำนวนเต็ม i;
    จำนวนเต็ม h = l.out_h;
    จำนวนเต็ม w = l.out_w;
    จำนวนเต็ม c = l.c;
    สำหรับ(i = 0; i < h*w*c*l.batch; ++i){
        จำนวนเต็ม index = l.indexes[i];
        net.delta[index] += l.delta[i];
    }
}


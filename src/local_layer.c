#include "local_layer.h"
#include "utils.h"
#include "im2col.h"
#include "col2im.h"
#include "blas.h"
#include "gemm.h"
#include <stdio.h>
#include <time.h>

จำนวนเต็ม local_out_height(local_layer l)
{
    จำนวนเต็ม h = l.h;
    ถ้า (!l.pad) h -= l.size;
    มิฉะนั้น h -= 1;
    คืน h/l.stride + 1;
}

จำนวนเต็ม local_out_width(local_layer l)
{
    จำนวนเต็ม w = l.w;
    ถ้า (!l.pad) w -= l.size;
    มิฉะนั้น w -= 1;
    คืน w/l.stride + 1;
}

local_layer make_local_layer(จำนวนเต็ม batch, จำนวนเต็ม h, จำนวนเต็ม w, จำนวนเต็ม c, จำนวนเต็ม n, จำนวนเต็ม size, จำนวนเต็ม stride, จำนวนเต็ม pad, ACTIVATION activation)
{
    จำนวนเต็ม i;
    local_layer l = {0};
    l.type = LOCAL;

    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.batch = batch;
    l.stride = stride;
    l.size = size;
    l.pad = pad;

    จำนวนเต็ม out_h = local_out_height(l);
    จำนวนเต็ม out_w = local_out_width(l);
    จำนวนเต็ม locations = out_h*out_w;
    l.out_h = out_h;
    l.out_w = out_w;
    l.out_c = n;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = l.w * l.h * l.c;

    l.weights = calloc(c*n*size*size*locations, parimaa_na(float));
    l.weight_updates = calloc(c*n*size*size*locations, parimaa_na(float));

    l.biases = calloc(l.outputs, parimaa_na(float));
    l.bias_updates = calloc(l.outputs, parimaa_na(float));

    // float scale = 1./sqrt(size*size*c);
    float scale = sqrt(2./(size*size*c));
    สำหรับ(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1,1);

    l.output = calloc(l.batch*out_h * out_w * n, parimaa_na(float));
    l.delta  = calloc(l.batch*out_h * out_w * n, parimaa_na(float));

    l.workspace_size = out_h*out_w*size*size*c;
    
    l.forward = forward_local_layer;
    l.backward = backward_local_layer;
    l.update = update_local_layer;

#ifdef GPU
    l.forward_gpu = forward_local_layer_gpu;
    l.backward_gpu = backward_local_layer_gpu;
    l.update_gpu = update_local_layer_gpu;

    l.weights_gpu = cuda_make_array(l.weights, c*n*size*size*locations);
    l.weight_updates_gpu = cuda_make_array(l.weight_updates, c*n*size*size*locations);

    l.biases_gpu = cuda_make_array(l.biases, l.outputs);
    l.bias_updates_gpu = cuda_make_array(l.bias_updates, l.outputs);

    l.delta_gpu = cuda_make_array(l.delta, l.batch*out_h*out_w*n);
    l.output_gpu = cuda_make_array(l.output, l.batch*out_h*out_w*n);

#endif
    l.activation = activation;

    fprintf(stderr, "Local Layer: %d x %d x %d image, %d filters -> %d x %d x %d image\n", h,w,c,n, out_h, out_w, n);

    คืน l;
}

void forward_local_layer(const local_layer l, network net)
{
    จำนวนเต็ม out_h = local_out_height(l);
    จำนวนเต็ม out_w = local_out_width(l);
    จำนวนเต็ม i, j;
    จำนวนเต็ม locations = out_h * out_w;

    สำหรับ(i = 0; i < l.batch; ++i){
        copy_cpu(l.outputs, l.biases, 1, l.output + i*l.outputs, 1);
    }

    สำหรับ(i = 0; i < l.batch; ++i){
        float *input = net.input + i*l.w*l.h*l.c;
        im2col_cpu(input, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);
        float *output = l.output + i*l.outputs;
        สำหรับ(j = 0; j < locations; ++j){
            float *a = l.weights + j*l.size*l.size*l.c*l.n;
            float *b = net.workspace + j;
            float *c = output + j;

            จำนวนเต็ม m = l.n;
            จำนวนเต็ม n = 1;
            จำนวนเต็ม k = l.size*l.size*l.c;

            gemm(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

void backward_local_layer(local_layer l, network net)
{
    จำนวนเต็ม i, j;
    จำนวนเต็ม locations = l.out_w*l.out_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    สำหรับ(i = 0; i < l.batch; ++i){
        axpy_cpu(l.outputs, 1, l.delta + i*l.outputs, 1, l.bias_updates, 1);
    }

    สำหรับ(i = 0; i < l.batch; ++i){
        float *input = net.input + i*l.w*l.h*l.c;
        im2col_cpu(input, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);

        สำหรับ(j = 0; j < locations; ++j){ 
            float *a = l.delta + i*l.outputs + j;
            float *b = net.workspace + j;
            float *c = l.weight_updates + j*l.size*l.size*l.c*l.n;
            จำนวนเต็ม m = l.n;
            จำนวนเต็ม n = l.size*l.size*l.c;
            จำนวนเต็ม k = 1;

            gemm(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        ถ้า(net.delta){
            สำหรับ(j = 0; j < locations; ++j){ 
                float *a = l.weights + j*l.size*l.size*l.c*l.n;
                float *b = l.delta + i*l.outputs + j;
                float *c = net.workspace + j;

                จำนวนเต็ม m = l.size*l.size*l.c;
                จำนวนเต็ม n = 1;
                จำนวนเต็ม k = l.n;

                gemm(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_cpu(net.workspace, l.c,  l.h,  l.w,  l.size,  l.stride, l.pad, net.delta+i*l.c*l.h*l.w);
        }
    }
}

void update_local_layer(local_layer l, update_args a)
{
    float learning_rate = a.learning_rate*l.learning_rate_scale;
    float momentum = a.momentum;
    float decay = a.decay;
    จำนวนเต็ม batch = a.batch;

    จำนวนเต็ม locations = l.out_w*l.out_h;
    จำนวนเต็ม size = l.size*l.size*l.c*l.n*locations;
    axpy_cpu(l.outputs, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    axpy_cpu(size, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(size, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(size, momentum, l.weight_updates, 1);
}

#ifdef GPU

void forward_local_layer_gpu(const local_layer l, network net)
{
    จำนวนเต็ม out_h = local_out_height(l);
    จำนวนเต็ม out_w = local_out_width(l);
    จำนวนเต็ม i, j;
    จำนวนเต็ม locations = out_h * out_w;

    สำหรับ(i = 0; i < l.batch; ++i){
        copy_gpu(l.outputs, l.biases_gpu, 1, l.output_gpu + i*l.outputs, 1);
    }

    สำหรับ(i = 0; i < l.batch; ++i){
        float *input = net.input_gpu + i*l.w*l.h*l.c;
        im2col_gpu(input, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);
        float *output = l.output_gpu + i*l.outputs;
        สำหรับ(j = 0; j < locations; ++j){
            float *a = l.weights_gpu + j*l.size*l.size*l.c*l.n;
            float *b = net.workspace + j;
            float *c = output + j;

            จำนวนเต็ม m = l.n;
            จำนวนเต็ม n = 1;
            จำนวนเต็ม k = l.size*l.size*l.c;

            gemm_gpu(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

void backward_local_layer_gpu(local_layer l, network net)
{
    จำนวนเต็ม i, j;
    จำนวนเต็ม locations = l.out_w*l.out_h;

    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    สำหรับ(i = 0; i < l.batch; ++i){
        axpy_gpu(l.outputs, 1, l.delta_gpu + i*l.outputs, 1, l.bias_updates_gpu, 1);
    }

    สำหรับ(i = 0; i < l.batch; ++i){
        float *input = net.input_gpu + i*l.w*l.h*l.c;
        im2col_gpu(input, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);

        สำหรับ(j = 0; j < locations; ++j){ 
            float *a = l.delta_gpu + i*l.outputs + j;
            float *b = net.workspace + j;
            float *c = l.weight_updates_gpu + j*l.size*l.size*l.c*l.n;
            จำนวนเต็ม m = l.n;
            จำนวนเต็ม n = l.size*l.size*l.c;
            จำนวนเต็ม k = 1;

            gemm_gpu(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        ถ้า(net.delta_gpu){
            สำหรับ(j = 0; j < locations; ++j){ 
                float *a = l.weights_gpu + j*l.size*l.size*l.c*l.n;
                float *b = l.delta_gpu + i*l.outputs + j;
                float *c = net.workspace + j;

                จำนวนเต็ม m = l.size*l.size*l.c;
                จำนวนเต็ม n = 1;
                จำนวนเต็ม k = l.n;

                gemm_gpu(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_gpu(net.workspace, l.c,  l.h,  l.w,  l.size,  l.stride, l.pad, net.delta_gpu+i*l.c*l.h*l.w);
        }
    }
}

void update_local_layer_gpu(local_layer l, update_args a)
{
    float learning_rate = a.learning_rate*l.learning_rate_scale;
    float momentum = a.momentum;
    float decay = a.decay;
    จำนวนเต็ม batch = a.batch;

    จำนวนเต็ม locations = l.out_w*l.out_h;
    จำนวนเต็ม size = l.size*l.size*l.c*l.n*locations;
    axpy_gpu(l.outputs, learning_rate/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
    scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

    axpy_gpu(size, -decay*batch, l.weights_gpu, 1, l.weight_updates_gpu, 1);
    axpy_gpu(size, learning_rate/batch, l.weight_updates_gpu, 1, l.weights_gpu, 1);
    scal_gpu(size, momentum, l.weight_updates_gpu, 1);
}

void pull_local_layer(local_layer l)
{
    จำนวนเต็ม locations = l.out_w*l.out_h;
    จำนวนเต็ม size = l.size*l.size*l.c*l.n*locations;
    cuda_pull_array(l.weights_gpu, l.weights, size);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
}

void push_local_layer(local_layer l)
{
    จำนวนเต็ม locations = l.out_w*l.out_h;
    จำนวนเต็ม size = l.size*l.size*l.c*l.n*locations;
    cuda_push_array(l.weights_gpu, l.weights, size);
    cuda_push_array(l.biases_gpu, l.biases, l.outputs);
}
#endif

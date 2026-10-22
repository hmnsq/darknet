#অন্তর্ভাব "local_layer.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "im2col.h"
#অন্তর্ভাব "col2im.h"
#অন্তর্ভাব "blas.h"
#অন্তর্ভাব "gemm.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <time.h>

পূর্ণসংখ্যা local_out_height(local_layer l)
{
    পূর্ণসংখ্যা h = l.h;
    যদি (!l.pad) h -= l.আকার_2;
    অন্যথা h -= 1;
    ফেরত h/l.stride + 1;
}

পূর্ণসংখ্যা local_out_width(local_layer l)
{
    পূর্ণসংখ্যা w = l.w;
    যদি (!l.pad) w -= l.আকার_2;
    অন্যথা w -= 1;
    ফেরত w/l.stride + 1;
}

local_layer make_local_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা h, পূর্ণসংখ্যা w, পূর্ণসংখ্যা c, পূর্ণসংখ্যা n, পূর্ণসংখ্যা আকার_2, পূর্ণসংখ্যা stride, পূর্ণসংখ্যা pad, ACTIVATION activation)
{
    পূর্ণসংখ্যা i;
    local_layer l = {0};
    l.type = LOCAL;

    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.batch = batch;
    l.stride = stride;
    l.আকার_2 = আকার_2;
    l.pad = pad;

    পূর্ণসংখ্যা out_h = local_out_height(l);
    পূর্ণসংখ্যা out_w = local_out_width(l);
    পূর্ণসংখ্যা locations = out_h*out_w;
    l.out_h = out_h;
    l.out_w = out_w;
    l.out_c = n;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = l.w * l.h * l.c;

    l.weights = calloc(c*n*আকার_2*আকার_2*locations, আকার(দশমিক));
    l.weight_updates = calloc(c*n*আকার_2*আকার_2*locations, আকার(দশমিক));

    l.biases = calloc(l.outputs, আকার(দশমিক));
    l.bias_updates = calloc(l.outputs, আকার(দশমিক));

    // float scale = 1./sqrt(size*size*c);
    দশমিক scale = sqrt(2./(আকার_2*আকার_2*c));
    জন্য(i = 0; i < c*n*আকার_2*আকার_2; ++i) l.weights[i] = scale*rand_uniform(-1,1);

    l.output = calloc(l.batch*out_h * out_w * n, আকার(দশমিক));
    l.delta  = calloc(l.batch*out_h * out_w * n, আকার(দশমিক));

    l.workspace_আকার = out_h*out_w*আকার_2*আকার_2*c;
    
    l.forward = forward_local_layer;
    l.backward = backward_local_layer;
    l.update = update_local_layer;

#ifdef GPU
    l.forward_gpu = forward_local_layer_gpu;
    l.backward_gpu = backward_local_layer_gpu;
    l.update_gpu = update_local_layer_gpu;

    l.weights_gpu = cuda_make_array(l.weights, c*n*আকার_2*আকার_2*locations);
    l.weight_updates_gpu = cuda_make_array(l.weight_updates, c*n*আকার_2*আকার_2*locations);

    l.biases_gpu = cuda_make_array(l.biases, l.outputs);
    l.bias_updates_gpu = cuda_make_array(l.bias_updates, l.outputs);

    l.delta_gpu = cuda_make_array(l.delta, l.batch*out_h*out_w*n);
    l.output_gpu = cuda_make_array(l.output, l.batch*out_h*out_w*n);

#endif
    l.activation = activation;

    fprintf(stderr, "Local Layer: %d x %d x %d image, %d filters -> %d x %d x %d image\n", h,w,c,n, out_h, out_w, n);

    ফেরত l;
}

শূন্য forward_local_layer(ধ্রুবক local_layer l, network net)
{
    পূর্ণসংখ্যা out_h = local_out_height(l);
    পূর্ণসংখ্যা out_w = local_out_width(l);
    পূর্ণসংখ্যা i, j;
    পূর্ণসংখ্যা locations = out_h * out_w;

    জন্য(i = 0; i < l.batch; ++i){
        copy_cpu(l.outputs, l.biases, 1, l.output + i*l.outputs, 1);
    }

    জন্য(i = 0; i < l.batch; ++i){
        দশমিক *প্রবেশ = net.প্রবেশ + i*l.w*l.h*l.c;
        im2col_cpu(প্রবেশ, l.c, l.h, l.w, 
                l.আকার_2, l.stride, l.pad, net.workspace);
        দশমিক *output = l.output + i*l.outputs;
        জন্য(j = 0; j < locations; ++j){
            দশমিক *a = l.weights + j*l.আকার_2*l.আকার_2*l.c*l.n;
            দশমিক *b = net.workspace + j;
            দশমিক *c = output + j;

            পূর্ণসংখ্যা m = l.n;
            পূর্ণসংখ্যা n = 1;
            পূর্ণসংখ্যা k = l.আকার_2*l.আকার_2*l.c;

            gemm(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

শূন্য backward_local_layer(local_layer l, network net)
{
    পূর্ণসংখ্যা i, j;
    পূর্ণসংখ্যা locations = l.out_w*l.out_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    জন্য(i = 0; i < l.batch; ++i){
        axpy_cpu(l.outputs, 1, l.delta + i*l.outputs, 1, l.bias_updates, 1);
    }

    জন্য(i = 0; i < l.batch; ++i){
        দশমিক *প্রবেশ = net.প্রবেশ + i*l.w*l.h*l.c;
        im2col_cpu(প্রবেশ, l.c, l.h, l.w, 
                l.আকার_2, l.stride, l.pad, net.workspace);

        জন্য(j = 0; j < locations; ++j){ 
            দশমিক *a = l.delta + i*l.outputs + j;
            দশমিক *b = net.workspace + j;
            দশমিক *c = l.weight_updates + j*l.আকার_2*l.আকার_2*l.c*l.n;
            পূর্ণসংখ্যা m = l.n;
            পূর্ণসংখ্যা n = l.আকার_2*l.আকার_2*l.c;
            পূর্ণসংখ্যা k = 1;

            gemm(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        যদি(net.delta){
            জন্য(j = 0; j < locations; ++j){ 
                দশমিক *a = l.weights + j*l.আকার_2*l.আকার_2*l.c*l.n;
                দশমিক *b = l.delta + i*l.outputs + j;
                দশমিক *c = net.workspace + j;

                পূর্ণসংখ্যা m = l.আকার_2*l.আকার_2*l.c;
                পূর্ণসংখ্যা n = 1;
                পূর্ণসংখ্যা k = l.n;

                gemm(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_cpu(net.workspace, l.c,  l.h,  l.w,  l.আকার_2,  l.stride, l.pad, net.delta+i*l.c*l.h*l.w);
        }
    }
}

শূন্য update_local_layer(local_layer l, update_args a)
{
    দশমিক learning_rate = a.learning_rate*l.learning_rate_scale;
    দশমিক momentum = a.momentum;
    দশমিক decay = a.decay;
    পূর্ণসংখ্যা batch = a.batch;

    পূর্ণসংখ্যা locations = l.out_w*l.out_h;
    পূর্ণসংখ্যা আকার_2 = l.আকার_2*l.আকার_2*l.c*l.n*locations;
    axpy_cpu(l.outputs, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    axpy_cpu(আকার_2, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(আকার_2, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(আকার_2, momentum, l.weight_updates, 1);
}

#ifdef GPU

শূন্য forward_local_layer_gpu(ধ্রুবক local_layer l, network net)
{
    পূর্ণসংখ্যা out_h = local_out_height(l);
    পূর্ণসংখ্যা out_w = local_out_width(l);
    পূর্ণসংখ্যা i, j;
    পূর্ণসংখ্যা locations = out_h * out_w;

    জন্য(i = 0; i < l.batch; ++i){
        copy_gpu(l.outputs, l.biases_gpu, 1, l.output_gpu + i*l.outputs, 1);
    }

    জন্য(i = 0; i < l.batch; ++i){
        দশমিক *প্রবেশ = net.প্রবেশ_gpu + i*l.w*l.h*l.c;
        im2col_gpu(প্রবেশ, l.c, l.h, l.w, 
                l.আকার_2, l.stride, l.pad, net.workspace);
        দশমিক *output = l.output_gpu + i*l.outputs;
        জন্য(j = 0; j < locations; ++j){
            দশমিক *a = l.weights_gpu + j*l.আকার_2*l.আকার_2*l.c*l.n;
            দশমিক *b = net.workspace + j;
            দশমিক *c = output + j;

            পূর্ণসংখ্যা m = l.n;
            পূর্ণসংখ্যা n = 1;
            পূর্ণসংখ্যা k = l.আকার_2*l.আকার_2*l.c;

            gemm_gpu(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

শূন্য backward_local_layer_gpu(local_layer l, network net)
{
    পূর্ণসংখ্যা i, j;
    পূর্ণসংখ্যা locations = l.out_w*l.out_h;

    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    জন্য(i = 0; i < l.batch; ++i){
        axpy_gpu(l.outputs, 1, l.delta_gpu + i*l.outputs, 1, l.bias_updates_gpu, 1);
    }

    জন্য(i = 0; i < l.batch; ++i){
        দশমিক *প্রবেশ = net.প্রবেশ_gpu + i*l.w*l.h*l.c;
        im2col_gpu(প্রবেশ, l.c, l.h, l.w, 
                l.আকার_2, l.stride, l.pad, net.workspace);

        জন্য(j = 0; j < locations; ++j){ 
            দশমিক *a = l.delta_gpu + i*l.outputs + j;
            দশমিক *b = net.workspace + j;
            দশমিক *c = l.weight_updates_gpu + j*l.আকার_2*l.আকার_2*l.c*l.n;
            পূর্ণসংখ্যা m = l.n;
            পূর্ণসংখ্যা n = l.আকার_2*l.আকার_2*l.c;
            পূর্ণসংখ্যা k = 1;

            gemm_gpu(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        যদি(net.delta_gpu){
            জন্য(j = 0; j < locations; ++j){ 
                দশমিক *a = l.weights_gpu + j*l.আকার_2*l.আকার_2*l.c*l.n;
                দশমিক *b = l.delta_gpu + i*l.outputs + j;
                দশমিক *c = net.workspace + j;

                পূর্ণসংখ্যা m = l.আকার_2*l.আকার_2*l.c;
                পূর্ণসংখ্যা n = 1;
                পূর্ণসংখ্যা k = l.n;

                gemm_gpu(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_gpu(net.workspace, l.c,  l.h,  l.w,  l.আকার_2,  l.stride, l.pad, net.delta_gpu+i*l.c*l.h*l.w);
        }
    }
}

শূন্য update_local_layer_gpu(local_layer l, update_args a)
{
    দশমিক learning_rate = a.learning_rate*l.learning_rate_scale;
    দশমিক momentum = a.momentum;
    দশমিক decay = a.decay;
    পূর্ণসংখ্যা batch = a.batch;

    পূর্ণসংখ্যা locations = l.out_w*l.out_h;
    পূর্ণসংখ্যা আকার_2 = l.আকার_2*l.আকার_2*l.c*l.n*locations;
    axpy_gpu(l.outputs, learning_rate/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
    scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

    axpy_gpu(আকার_2, -decay*batch, l.weights_gpu, 1, l.weight_updates_gpu, 1);
    axpy_gpu(আকার_2, learning_rate/batch, l.weight_updates_gpu, 1, l.weights_gpu, 1);
    scal_gpu(আকার_2, momentum, l.weight_updates_gpu, 1);
}

শূন্য pull_local_layer(local_layer l)
{
    পূর্ণসংখ্যা locations = l.out_w*l.out_h;
    পূর্ণসংখ্যা আকার_2 = l.আকার_2*l.আকার_2*l.c*l.n*locations;
    cuda_pull_array(l.weights_gpu, l.weights, আকার_2);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
}

শূন্য ঠেলো_local_layer(local_layer l)
{
    পূর্ণসংখ্যা locations = l.out_w*l.out_h;
    পূর্ণসংখ্যা আকার_2 = l.আকার_2*l.আকার_2*l.c*l.n*locations;
    cuda_ঠেলো_array(l.weights_gpu, l.weights, আকার_2);
    cuda_ঠেলো_array(l.biases_gpu, l.biases, l.outputs);
}
#endif

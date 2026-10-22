#شامل "local_layer.h"
#شامل "utils.h"
#شامل "im2col.h"
#شامل "col2im.h"
#شامل "blas.h"
#شامل "gemm.h"
#شامل <stdio.h>
#شامل <time.h>

صحیح local_out_height(local_layer l)
{
    صحیح h = l.h;
    اگر (!l.pad) h -= l.size;
    وگرنه h -= 1;
    بازگشت h/l.stride + 1;
}

صحیح local_out_width(local_layer l)
{
    صحیح w = l.w;
    اگر (!l.pad) w -= l.size;
    وگرنه w -= 1;
    بازگشت w/l.stride + 1;
}

local_layer make_local_layer(صحیح batch, صحیح h, صحیح w, صحیح c, صحیح n, صحیح size, صحیح stride, صحیح pad, ACTIVATION activation)
{
    صحیح i;
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

    صحیح out_h = local_out_height(l);
    صحیح out_w = local_out_width(l);
    صحیح locations = out_h*out_w;
    l.out_h = out_h;
    l.out_w = out_w;
    l.out_c = n;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = l.w * l.h * l.c;

    l.weights = calloc(c*n*size*size*locations, اندازه(اعشاری));
    l.weight_updates = calloc(c*n*size*size*locations, اندازه(اعشاری));

    l.biases = calloc(l.outputs, اندازه(اعشاری));
    l.bias_updates = calloc(l.outputs, اندازه(اعشاری));

    // float scale = 1./sqrt(size*size*c);
    اعشاری scale = sqrt(2./(size*size*c));
    برای(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1,1);

    l.output = calloc(l.batch*out_h * out_w * n, اندازه(اعشاری));
    l.delta  = calloc(l.batch*out_h * out_w * n, اندازه(اعشاری));

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

    بازگشت l;
}

تهی forward_local_layer(ثابت local_layer l, network net)
{
    صحیح out_h = local_out_height(l);
    صحیح out_w = local_out_width(l);
    صحیح i, j;
    صحیح locations = out_h * out_w;

    برای(i = 0; i < l.batch; ++i){
        copy_cpu(l.outputs, l.biases, 1, l.output + i*l.outputs, 1);
    }

    برای(i = 0; i < l.batch; ++i){
        اعشاری *درون_داد = net.درون_داد + i*l.w*l.h*l.c;
        im2col_cpu(درون_داد, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);
        اعشاری *output = l.output + i*l.outputs;
        برای(j = 0; j < locations; ++j){
            اعشاری *a = l.weights + j*l.size*l.size*l.c*l.n;
            اعشاری *b = net.workspace + j;
            اعشاری *c = output + j;

            صحیح m = l.n;
            صحیح n = 1;
            صحیح k = l.size*l.size*l.c;

            gemm(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

تهی backward_local_layer(local_layer l, network net)
{
    صحیح i, j;
    صحیح locations = l.out_w*l.out_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    برای(i = 0; i < l.batch; ++i){
        axpy_cpu(l.outputs, 1, l.delta + i*l.outputs, 1, l.bias_updates, 1);
    }

    برای(i = 0; i < l.batch; ++i){
        اعشاری *درون_داد = net.درون_داد + i*l.w*l.h*l.c;
        im2col_cpu(درون_داد, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);

        برای(j = 0; j < locations; ++j){ 
            اعشاری *a = l.delta + i*l.outputs + j;
            اعشاری *b = net.workspace + j;
            اعشاری *c = l.weight_updates + j*l.size*l.size*l.c*l.n;
            صحیح m = l.n;
            صحیح n = l.size*l.size*l.c;
            صحیح k = 1;

            gemm(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        اگر(net.delta){
            برای(j = 0; j < locations; ++j){ 
                اعشاری *a = l.weights + j*l.size*l.size*l.c*l.n;
                اعشاری *b = l.delta + i*l.outputs + j;
                اعشاری *c = net.workspace + j;

                صحیح m = l.size*l.size*l.c;
                صحیح n = 1;
                صحیح k = l.n;

                gemm(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_cpu(net.workspace, l.c,  l.h,  l.w,  l.size,  l.stride, l.pad, net.delta+i*l.c*l.h*l.w);
        }
    }
}

تهی update_local_layer(local_layer l, update_args a)
{
    اعشاری learning_rate = a.learning_rate*l.learning_rate_scale;
    اعشاری momentum = a.momentum;
    اعشاری decay = a.decay;
    صحیح batch = a.batch;

    صحیح locations = l.out_w*l.out_h;
    صحیح size = l.size*l.size*l.c*l.n*locations;
    axpy_cpu(l.outputs, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    axpy_cpu(size, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(size, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(size, momentum, l.weight_updates, 1);
}

#ifdef GPU

تهی forward_local_layer_gpu(ثابت local_layer l, network net)
{
    صحیح out_h = local_out_height(l);
    صحیح out_w = local_out_width(l);
    صحیح i, j;
    صحیح locations = out_h * out_w;

    برای(i = 0; i < l.batch; ++i){
        copy_gpu(l.outputs, l.biases_gpu, 1, l.output_gpu + i*l.outputs, 1);
    }

    برای(i = 0; i < l.batch; ++i){
        اعشاری *درون_داد = net.درون_داد_gpu + i*l.w*l.h*l.c;
        im2col_gpu(درون_داد, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);
        اعشاری *output = l.output_gpu + i*l.outputs;
        برای(j = 0; j < locations; ++j){
            اعشاری *a = l.weights_gpu + j*l.size*l.size*l.c*l.n;
            اعشاری *b = net.workspace + j;
            اعشاری *c = output + j;

            صحیح m = l.n;
            صحیح n = 1;
            صحیح k = l.size*l.size*l.c;

            gemm_gpu(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

تهی backward_local_layer_gpu(local_layer l, network net)
{
    صحیح i, j;
    صحیح locations = l.out_w*l.out_h;

    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    برای(i = 0; i < l.batch; ++i){
        axpy_gpu(l.outputs, 1, l.delta_gpu + i*l.outputs, 1, l.bias_updates_gpu, 1);
    }

    برای(i = 0; i < l.batch; ++i){
        اعشاری *درون_داد = net.درون_داد_gpu + i*l.w*l.h*l.c;
        im2col_gpu(درون_داد, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);

        برای(j = 0; j < locations; ++j){ 
            اعشاری *a = l.delta_gpu + i*l.outputs + j;
            اعشاری *b = net.workspace + j;
            اعشاری *c = l.weight_updates_gpu + j*l.size*l.size*l.c*l.n;
            صحیح m = l.n;
            صحیح n = l.size*l.size*l.c;
            صحیح k = 1;

            gemm_gpu(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        اگر(net.delta_gpu){
            برای(j = 0; j < locations; ++j){ 
                اعشاری *a = l.weights_gpu + j*l.size*l.size*l.c*l.n;
                اعشاری *b = l.delta_gpu + i*l.outputs + j;
                اعشاری *c = net.workspace + j;

                صحیح m = l.size*l.size*l.c;
                صحیح n = 1;
                صحیح k = l.n;

                gemm_gpu(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_gpu(net.workspace, l.c,  l.h,  l.w,  l.size,  l.stride, l.pad, net.delta_gpu+i*l.c*l.h*l.w);
        }
    }
}

تهی update_local_layer_gpu(local_layer l, update_args a)
{
    اعشاری learning_rate = a.learning_rate*l.learning_rate_scale;
    اعشاری momentum = a.momentum;
    اعشاری decay = a.decay;
    صحیح batch = a.batch;

    صحیح locations = l.out_w*l.out_h;
    صحیح size = l.size*l.size*l.c*l.n*locations;
    axpy_gpu(l.outputs, learning_rate/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
    scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

    axpy_gpu(size, -decay*batch, l.weights_gpu, 1, l.weight_updates_gpu, 1);
    axpy_gpu(size, learning_rate/batch, l.weight_updates_gpu, 1, l.weights_gpu, 1);
    scal_gpu(size, momentum, l.weight_updates_gpu, 1);
}

تهی pull_local_layer(local_layer l)
{
    صحیح locations = l.out_w*l.out_h;
    صحیح size = l.size*l.size*l.c*l.n*locations;
    cuda_pull_array(l.weights_gpu, l.weights, size);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
}

تهی افزودن_local_layer(local_layer l)
{
    صحیح locations = l.out_w*l.out_h;
    صحیح size = l.size*l.size*l.c*l.n*locations;
    cuda_افزودن_array(l.weights_gpu, l.weights, size);
    cuda_افزودن_array(l.biases_gpu, l.biases, l.outputs);
}
#endif

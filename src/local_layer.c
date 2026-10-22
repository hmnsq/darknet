#அந்தர்பாவ "local_layer.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "im2col.h"
#அந்தர்பாவ "col2im.h"
#அந்தர்பாவ "blas.h"
#அந்தர்பாவ "gemm.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <time.h>

முழு local_out_height(local_layer l)
{
    முழு h = l.h;
    எனில் (!l.pad) h -= l.size;
    இல்லையெனில் h -= 1;
    திரும்பு h/l.stride + 1;
}

முழு local_out_width(local_layer l)
{
    முழு w = l.w;
    எனில் (!l.pad) w -= l.size;
    இல்லையெனில் w -= 1;
    திரும்பு w/l.stride + 1;
}

local_layer make_local_layer(முழு batch, முழு h, முழு w, முழு c, முழு n, முழு size, முழு stride, முழு pad, ACTIVATION activation)
{
    முழு i;
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

    முழு out_h = local_out_height(l);
    முழு out_w = local_out_width(l);
    முழு locations = out_h*out_w;
    l.out_h = out_h;
    l.out_w = out_w;
    l.out_c = n;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = l.w * l.h * l.c;

    l.weights = calloc(c*n*size*size*locations, அளவு(தசம));
    l.weight_updates = calloc(c*n*size*size*locations, அளவு(தசம));

    l.biases = calloc(l.outputs, அளவு(தசம));
    l.bias_updates = calloc(l.outputs, அளவு(தசம));

    // float scale = 1./sqrt(size*size*c);
    தசம scale = sqrt(2./(size*size*c));
    க்கு(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1,1);

    l.output = calloc(l.batch*out_h * out_w * n, அளவு(தசம));
    l.delta  = calloc(l.batch*out_h * out_w * n, அளவு(தசம));

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

    திரும்பு l;
}

வெறுமை forward_local_layer(மாறிலி local_layer l, network net)
{
    முழு out_h = local_out_height(l);
    முழு out_w = local_out_width(l);
    முழு i, j;
    முழு locations = out_h * out_w;

    க்கு(i = 0; i < l.batch; ++i){
        copy_cpu(l.outputs, l.biases, 1, l.output + i*l.outputs, 1);
    }

    க்கு(i = 0; i < l.batch; ++i){
        தசம *உள்ளீடு = net.உள்ளீடு + i*l.w*l.h*l.c;
        im2col_cpu(உள்ளீடு, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);
        தசம *output = l.output + i*l.outputs;
        க்கு(j = 0; j < locations; ++j){
            தசம *a = l.weights + j*l.size*l.size*l.c*l.n;
            தசம *b = net.workspace + j;
            தசம *c = output + j;

            முழு m = l.n;
            முழு n = 1;
            முழு k = l.size*l.size*l.c;

            gemm(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

வெறுமை backward_local_layer(local_layer l, network net)
{
    முழு i, j;
    முழு locations = l.out_w*l.out_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    க்கு(i = 0; i < l.batch; ++i){
        axpy_cpu(l.outputs, 1, l.delta + i*l.outputs, 1, l.bias_updates, 1);
    }

    க்கு(i = 0; i < l.batch; ++i){
        தசம *உள்ளீடு = net.உள்ளீடு + i*l.w*l.h*l.c;
        im2col_cpu(உள்ளீடு, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);

        க்கு(j = 0; j < locations; ++j){ 
            தசம *a = l.delta + i*l.outputs + j;
            தசம *b = net.workspace + j;
            தசம *c = l.weight_updates + j*l.size*l.size*l.c*l.n;
            முழு m = l.n;
            முழு n = l.size*l.size*l.c;
            முழு k = 1;

            gemm(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        எனில்(net.delta){
            க்கு(j = 0; j < locations; ++j){ 
                தசம *a = l.weights + j*l.size*l.size*l.c*l.n;
                தசம *b = l.delta + i*l.outputs + j;
                தசம *c = net.workspace + j;

                முழு m = l.size*l.size*l.c;
                முழு n = 1;
                முழு k = l.n;

                gemm(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_cpu(net.workspace, l.c,  l.h,  l.w,  l.size,  l.stride, l.pad, net.delta+i*l.c*l.h*l.w);
        }
    }
}

வெறுமை update_local_layer(local_layer l, update_args a)
{
    தசம learning_rate = a.learning_rate*l.learning_rate_scale;
    தசம momentum = a.momentum;
    தசம decay = a.decay;
    முழு batch = a.batch;

    முழு locations = l.out_w*l.out_h;
    முழு size = l.size*l.size*l.c*l.n*locations;
    axpy_cpu(l.outputs, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    axpy_cpu(size, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(size, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(size, momentum, l.weight_updates, 1);
}

#ifdef GPU

வெறுமை forward_local_layer_gpu(மாறிலி local_layer l, network net)
{
    முழு out_h = local_out_height(l);
    முழு out_w = local_out_width(l);
    முழு i, j;
    முழு locations = out_h * out_w;

    க்கு(i = 0; i < l.batch; ++i){
        copy_gpu(l.outputs, l.biases_gpu, 1, l.output_gpu + i*l.outputs, 1);
    }

    க்கு(i = 0; i < l.batch; ++i){
        தசம *உள்ளீடு = net.உள்ளீடு_gpu + i*l.w*l.h*l.c;
        im2col_gpu(உள்ளீடு, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);
        தசம *output = l.output_gpu + i*l.outputs;
        க்கு(j = 0; j < locations; ++j){
            தசம *a = l.weights_gpu + j*l.size*l.size*l.c*l.n;
            தசம *b = net.workspace + j;
            தசம *c = output + j;

            முழு m = l.n;
            முழு n = 1;
            முழு k = l.size*l.size*l.c;

            gemm_gpu(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

வெறுமை backward_local_layer_gpu(local_layer l, network net)
{
    முழு i, j;
    முழு locations = l.out_w*l.out_h;

    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    க்கு(i = 0; i < l.batch; ++i){
        axpy_gpu(l.outputs, 1, l.delta_gpu + i*l.outputs, 1, l.bias_updates_gpu, 1);
    }

    க்கு(i = 0; i < l.batch; ++i){
        தசம *உள்ளீடு = net.உள்ளீடு_gpu + i*l.w*l.h*l.c;
        im2col_gpu(உள்ளீடு, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);

        க்கு(j = 0; j < locations; ++j){ 
            தசம *a = l.delta_gpu + i*l.outputs + j;
            தசம *b = net.workspace + j;
            தசம *c = l.weight_updates_gpu + j*l.size*l.size*l.c*l.n;
            முழு m = l.n;
            முழு n = l.size*l.size*l.c;
            முழு k = 1;

            gemm_gpu(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        எனில்(net.delta_gpu){
            க்கு(j = 0; j < locations; ++j){ 
                தசம *a = l.weights_gpu + j*l.size*l.size*l.c*l.n;
                தசம *b = l.delta_gpu + i*l.outputs + j;
                தசம *c = net.workspace + j;

                முழு m = l.size*l.size*l.c;
                முழு n = 1;
                முழு k = l.n;

                gemm_gpu(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_gpu(net.workspace, l.c,  l.h,  l.w,  l.size,  l.stride, l.pad, net.delta_gpu+i*l.c*l.h*l.w);
        }
    }
}

வெறுமை update_local_layer_gpu(local_layer l, update_args a)
{
    தசம learning_rate = a.learning_rate*l.learning_rate_scale;
    தசம momentum = a.momentum;
    தசம decay = a.decay;
    முழு batch = a.batch;

    முழு locations = l.out_w*l.out_h;
    முழு size = l.size*l.size*l.c*l.n*locations;
    axpy_gpu(l.outputs, learning_rate/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
    scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

    axpy_gpu(size, -decay*batch, l.weights_gpu, 1, l.weight_updates_gpu, 1);
    axpy_gpu(size, learning_rate/batch, l.weight_updates_gpu, 1, l.weights_gpu, 1);
    scal_gpu(size, momentum, l.weight_updates_gpu, 1);
}

வெறுமை pull_local_layer(local_layer l)
{
    முழு locations = l.out_w*l.out_h;
    முழு size = l.size*l.size*l.c*l.n*locations;
    cuda_pull_array(l.weights_gpu, l.weights, size);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
}

வெறுமை தள்ளு_local_layer(local_layer l)
{
    முழு locations = l.out_w*l.out_h;
    முழு size = l.size*l.size*l.c*l.n*locations;
    cuda_தள்ளு_array(l.weights_gpu, l.weights, size);
    cuda_தள்ளு_array(l.biases_gpu, l.biases, l.outputs);
}
#endif

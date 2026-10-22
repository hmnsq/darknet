#अन्तर्भाव "local_layer.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "im2col.h"
#अन्तर्भाव "col2im.h"
#अन्तर्भाव "blas.h"
#अन्तर्भाव "gemm.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <time.h>

पूर्णाङ्क local_out_height(local_layer l)
{
    पूर्णाङ्क h = l.h;
    यदि (!l.pad) h -= l.size;
    अन्यथा h -= 1;
    फर्काऊ h/l.stride + 1;
}

पूर्णाङ्क local_out_width(local_layer l)
{
    पूर्णाङ्क w = l.w;
    यदि (!l.pad) w -= l.size;
    अन्यथा w -= 1;
    फर्काऊ w/l.stride + 1;
}

local_layer make_local_layer(पूर्णाङ्क batch, पूर्णाङ्क h, पूर्णाङ्क w, पूर्णाङ्क c, पूर्णाङ्क n, पूर्णाङ्क size, पूर्णाङ्क stride, पूर्णाङ्क pad, ACTIVATION activation)
{
    पूर्णाङ्क i;
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

    पूर्णाङ्क out_h = local_out_height(l);
    पूर्णाङ्क out_w = local_out_width(l);
    पूर्णाङ्क locations = out_h*out_w;
    l.out_h = out_h;
    l.out_w = out_w;
    l.out_c = n;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = l.w * l.h * l.c;

    l.weights = सुस्मृति(c*n*size*size*locations, परिमाण(भिन्न));
    l.weight_updates = सुस्मृति(c*n*size*size*locations, परिमाण(भिन्न));

    l.biases = सुस्मृति(l.outputs, परिमाण(भिन्न));
    l.bias_updates = सुस्मृति(l.outputs, परिमाण(भिन्न));

    // float scale = 1./sqrt(size*size*c);
    भिन्न scale = वर्ग_मूल(2./(size*size*c));
    लागि(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1,1);

    l.output = सुस्मृति(l.batch*out_h * out_w * n, परिमाण(भिन्न));
    l.delta  = सुस्मृति(l.batch*out_h * out_w * n, परिमाण(भिन्न));

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

    ख_लिखो(मानक_त्रुटि, "Local Layer: %d x %d x %d image, %d filters -> %d x %d x %d image\n", h,w,c,n, out_h, out_w, n);

    फर्काऊ l;
}

शून्य forward_local_layer(अचर local_layer l, network net)
{
    पूर्णाङ्क out_h = local_out_height(l);
    पूर्णाङ्क out_w = local_out_width(l);
    पूर्णाङ्क i, j;
    पूर्णाङ्क locations = out_h * out_w;

    लागि(i = 0; i < l.batch; ++i){
        copy_cpu(l.outputs, l.biases, 1, l.output + i*l.outputs, 1);
    }

    लागि(i = 0; i < l.batch; ++i){
        भिन्न *आगत = net.आगत + i*l.w*l.h*l.c;
        im2col_cpu(आगत, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);
        भिन्न *output = l.output + i*l.outputs;
        लागि(j = 0; j < locations; ++j){
            भिन्न *a = l.weights + j*l.size*l.size*l.c*l.n;
            भिन्न *b = net.workspace + j;
            भिन्न *c = output + j;

            पूर्णाङ्क m = l.n;
            पूर्णाङ्क n = 1;
            पूर्णाङ्क k = l.size*l.size*l.c;

            gemm(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

शून्य backward_local_layer(local_layer l, network net)
{
    पूर्णाङ्क i, j;
    पूर्णाङ्क locations = l.out_w*l.out_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    लागि(i = 0; i < l.batch; ++i){
        axpy_cpu(l.outputs, 1, l.delta + i*l.outputs, 1, l.bias_updates, 1);
    }

    लागि(i = 0; i < l.batch; ++i){
        भिन्न *आगत = net.आगत + i*l.w*l.h*l.c;
        im2col_cpu(आगत, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);

        लागि(j = 0; j < locations; ++j){ 
            भिन्न *a = l.delta + i*l.outputs + j;
            भिन्न *b = net.workspace + j;
            भिन्न *c = l.weight_updates + j*l.size*l.size*l.c*l.n;
            पूर्णाङ्क m = l.n;
            पूर्णाङ्क n = l.size*l.size*l.c;
            पूर्णाङ्क k = 1;

            gemm(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        यदि(net.delta){
            लागि(j = 0; j < locations; ++j){ 
                भिन्न *a = l.weights + j*l.size*l.size*l.c*l.n;
                भिन्न *b = l.delta + i*l.outputs + j;
                भिन्न *c = net.workspace + j;

                पूर्णाङ्क m = l.size*l.size*l.c;
                पूर्णाङ्क n = 1;
                पूर्णाङ्क k = l.n;

                gemm(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_cpu(net.workspace, l.c,  l.h,  l.w,  l.size,  l.stride, l.pad, net.delta+i*l.c*l.h*l.w);
        }
    }
}

शून्य update_local_layer(local_layer l, update_args a)
{
    भिन्न learning_rate = a.learning_rate*l.learning_rate_scale;
    भिन्न momentum = a.momentum;
    भिन्न decay = a.decay;
    पूर्णाङ्क batch = a.batch;

    पूर्णाङ्क locations = l.out_w*l.out_h;
    पूर्णाङ्क size = l.size*l.size*l.c*l.n*locations;
    axpy_cpu(l.outputs, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    axpy_cpu(size, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(size, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(size, momentum, l.weight_updates, 1);
}

#ifdef GPU

शून्य forward_local_layer_gpu(अचर local_layer l, network net)
{
    पूर्णाङ्क out_h = local_out_height(l);
    पूर्णाङ्क out_w = local_out_width(l);
    पूर्णाङ्क i, j;
    पूर्णाङ्क locations = out_h * out_w;

    लागि(i = 0; i < l.batch; ++i){
        copy_gpu(l.outputs, l.biases_gpu, 1, l.output_gpu + i*l.outputs, 1);
    }

    लागि(i = 0; i < l.batch; ++i){
        भिन्न *आगत = net.आगत_gpu + i*l.w*l.h*l.c;
        im2col_gpu(आगत, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);
        भिन्न *output = l.output_gpu + i*l.outputs;
        लागि(j = 0; j < locations; ++j){
            भिन्न *a = l.weights_gpu + j*l.size*l.size*l.c*l.n;
            भिन्न *b = net.workspace + j;
            भिन्न *c = output + j;

            पूर्णाङ्क m = l.n;
            पूर्णाङ्क n = 1;
            पूर्णाङ्क k = l.size*l.size*l.c;

            gemm_gpu(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

शून्य backward_local_layer_gpu(local_layer l, network net)
{
    पूर्णाङ्क i, j;
    पूर्णाङ्क locations = l.out_w*l.out_h;

    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    लागि(i = 0; i < l.batch; ++i){
        axpy_gpu(l.outputs, 1, l.delta_gpu + i*l.outputs, 1, l.bias_updates_gpu, 1);
    }

    लागि(i = 0; i < l.batch; ++i){
        भिन्न *आगत = net.आगत_gpu + i*l.w*l.h*l.c;
        im2col_gpu(आगत, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);

        लागि(j = 0; j < locations; ++j){ 
            भिन्न *a = l.delta_gpu + i*l.outputs + j;
            भिन्न *b = net.workspace + j;
            भिन्न *c = l.weight_updates_gpu + j*l.size*l.size*l.c*l.n;
            पूर्णाङ्क m = l.n;
            पूर्णाङ्क n = l.size*l.size*l.c;
            पूर्णाङ्क k = 1;

            gemm_gpu(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        यदि(net.delta_gpu){
            लागि(j = 0; j < locations; ++j){ 
                भिन्न *a = l.weights_gpu + j*l.size*l.size*l.c*l.n;
                भिन्न *b = l.delta_gpu + i*l.outputs + j;
                भिन्न *c = net.workspace + j;

                पूर्णाङ्क m = l.size*l.size*l.c;
                पूर्णाङ्क n = 1;
                पूर्णाङ्क k = l.n;

                gemm_gpu(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_gpu(net.workspace, l.c,  l.h,  l.w,  l.size,  l.stride, l.pad, net.delta_gpu+i*l.c*l.h*l.w);
        }
    }
}

शून्य update_local_layer_gpu(local_layer l, update_args a)
{
    भिन्न learning_rate = a.learning_rate*l.learning_rate_scale;
    भिन्न momentum = a.momentum;
    भिन्न decay = a.decay;
    पूर्णाङ्क batch = a.batch;

    पूर्णाङ्क locations = l.out_w*l.out_h;
    पूर्णाङ्क size = l.size*l.size*l.c*l.n*locations;
    axpy_gpu(l.outputs, learning_rate/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
    scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

    axpy_gpu(size, -decay*batch, l.weights_gpu, 1, l.weight_updates_gpu, 1);
    axpy_gpu(size, learning_rate/batch, l.weight_updates_gpu, 1, l.weights_gpu, 1);
    scal_gpu(size, momentum, l.weight_updates_gpu, 1);
}

शून्य pull_local_layer(local_layer l)
{
    पूर्णाङ्क locations = l.out_w*l.out_h;
    पूर्णाङ्क size = l.size*l.size*l.c*l.n*locations;
    cuda_pull_array(l.weights_gpu, l.weights, size);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
}

शून्य थप_local_layer(local_layer l)
{
    पूर्णाङ्क locations = l.out_w*l.out_h;
    पूर्णाङ्क size = l.size*l.size*l.c*l.n*locations;
    cuda_थप_array(l.weights_gpu, l.weights, size);
    cuda_थप_array(l.biases_gpu, l.biases, l.outputs);
}
#endif

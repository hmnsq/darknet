#समावेश "local_layer.h"
#समावेश "utils.h"
#समावेश "im2col.h"
#समावेश "col2im.h"
#समावेश "blas.h"
#समावेश "gemm.h"
#समावेश <stdio.h>
#समावेश <time.h>

पूर्णांक local_out_height(local_layer l)
{
    पूर्णांक h = l.h;
    अगर (!l.pad) h -= l.आकार;
    अन्यथा h -= 1;
    वापस h/l.stride + 1;
}

पूर्णांक local_out_width(local_layer l)
{
    पूर्णांक w = l.w;
    अगर (!l.pad) w -= l.आकार;
    अन्यथा w -= 1;
    वापस w/l.stride + 1;
}

local_layer make_local_layer(पूर्णांक batch, पूर्णांक h, पूर्णांक w, पूर्णांक c, पूर्णांक n, पूर्णांक आकार, पूर्णांक stride, पूर्णांक pad, ACTIVATION activation)
{
    पूर्णांक i;
    local_layer l = {0};
    l.type = LOCAL;

    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.batch = batch;
    l.stride = stride;
    l.आकार = आकार;
    l.pad = pad;

    पूर्णांक out_h = local_out_height(l);
    पूर्णांक out_w = local_out_width(l);
    पूर्णांक locations = out_h*out_w;
    l.out_h = out_h;
    l.out_w = out_w;
    l.out_c = n;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = l.w * l.h * l.c;

    l.weights = सुस्मृति(c*n*आकार*आकार*locations, माप(भग्न));
    l.weight_updates = सुस्मृति(c*n*आकार*आकार*locations, माप(भग्न));

    l.biases = सुस्मृति(l.outputs, माप(भग्न));
    l.bias_updates = सुस्मृति(l.outputs, माप(भग्न));

    // float scale = 1./sqrt(size*size*c);
    भग्न scale = वर्ग_मूल(2./(आकार*आकार*c));
    क्रम(i = 0; i < c*n*आकार*आकार; ++i) l.weights[i] = scale*rand_uniform(-1,1);

    l.output = सुस्मृति(l.batch*out_h * out_w * n, माप(भग्न));
    l.delta  = सुस्मृति(l.batch*out_h * out_w * n, माप(भग्न));

    l.workspace_आकार = out_h*out_w*आकार*आकार*c;
    
    l.forward = forward_local_layer;
    l.backward = backward_local_layer;
    l.update = update_local_layer;

#अगर_घोषित GPU
    l.forward_gpu = forward_local_layer_gpu;
    l.backward_gpu = backward_local_layer_gpu;
    l.update_gpu = update_local_layer_gpu;

    l.weights_gpu = cuda_make_array(l.weights, c*n*आकार*आकार*locations);
    l.weight_updates_gpu = cuda_make_array(l.weight_updates, c*n*आकार*आकार*locations);

    l.biases_gpu = cuda_make_array(l.biases, l.outputs);
    l.bias_updates_gpu = cuda_make_array(l.bias_updates, l.outputs);

    l.delta_gpu = cuda_make_array(l.delta, l.batch*out_h*out_w*n);
    l.output_gpu = cuda_make_array(l.output, l.batch*out_h*out_w*n);

#पूर्ण_अगर
    l.activation = activation;

    ख_लिखो(मानक_त्रुटि, "Local Layer: %d x %d x %d image, %d filters -> %d x %d x %d image\n", h,w,c,n, out_h, out_w, n);

    वापस l;
}

व्योम forward_local_layer(स्थिर local_layer l, network net)
{
    पूर्णांक out_h = local_out_height(l);
    पूर्णांक out_w = local_out_width(l);
    पूर्णांक i, j;
    पूर्णांक locations = out_h * out_w;

    क्रम(i = 0; i < l.batch; ++i){
        copy_cpu(l.outputs, l.biases, 1, l.output + i*l.outputs, 1);
    }

    क्रम(i = 0; i < l.batch; ++i){
        भग्न *निवेश = net.निवेश + i*l.w*l.h*l.c;
        im2col_cpu(निवेश, l.c, l.h, l.w, 
                l.आकार, l.stride, l.pad, net.workspace);
        भग्न *output = l.output + i*l.outputs;
        क्रम(j = 0; j < locations; ++j){
            भग्न *a = l.weights + j*l.आकार*l.आकार*l.c*l.n;
            भग्न *b = net.workspace + j;
            भग्न *c = output + j;

            पूर्णांक m = l.n;
            पूर्णांक n = 1;
            पूर्णांक k = l.आकार*l.आकार*l.c;

            gemm(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

व्योम backward_local_layer(local_layer l, network net)
{
    पूर्णांक i, j;
    पूर्णांक locations = l.out_w*l.out_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    क्रम(i = 0; i < l.batch; ++i){
        axpy_cpu(l.outputs, 1, l.delta + i*l.outputs, 1, l.bias_updates, 1);
    }

    क्रम(i = 0; i < l.batch; ++i){
        भग्न *निवेश = net.निवेश + i*l.w*l.h*l.c;
        im2col_cpu(निवेश, l.c, l.h, l.w, 
                l.आकार, l.stride, l.pad, net.workspace);

        क्रम(j = 0; j < locations; ++j){ 
            भग्न *a = l.delta + i*l.outputs + j;
            भग्न *b = net.workspace + j;
            भग्न *c = l.weight_updates + j*l.आकार*l.आकार*l.c*l.n;
            पूर्णांक m = l.n;
            पूर्णांक n = l.आकार*l.आकार*l.c;
            पूर्णांक k = 1;

            gemm(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        अगर(net.delta){
            क्रम(j = 0; j < locations; ++j){ 
                भग्न *a = l.weights + j*l.आकार*l.आकार*l.c*l.n;
                भग्न *b = l.delta + i*l.outputs + j;
                भग्न *c = net.workspace + j;

                पूर्णांक m = l.आकार*l.आकार*l.c;
                पूर्णांक n = 1;
                पूर्णांक k = l.n;

                gemm(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_cpu(net.workspace, l.c,  l.h,  l.w,  l.आकार,  l.stride, l.pad, net.delta+i*l.c*l.h*l.w);
        }
    }
}

व्योम update_local_layer(local_layer l, update_args a)
{
    भग्न learning_rate = a.learning_rate*l.learning_rate_scale;
    भग्न momentum = a.momentum;
    भग्न decay = a.decay;
    पूर्णांक batch = a.batch;

    पूर्णांक locations = l.out_w*l.out_h;
    पूर्णांक आकार = l.आकार*l.आकार*l.c*l.n*locations;
    axpy_cpu(l.outputs, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    axpy_cpu(आकार, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(आकार, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(आकार, momentum, l.weight_updates, 1);
}

#अगर_घोषित GPU

व्योम forward_local_layer_gpu(स्थिर local_layer l, network net)
{
    पूर्णांक out_h = local_out_height(l);
    पूर्णांक out_w = local_out_width(l);
    पूर्णांक i, j;
    पूर्णांक locations = out_h * out_w;

    क्रम(i = 0; i < l.batch; ++i){
        copy_gpu(l.outputs, l.biases_gpu, 1, l.output_gpu + i*l.outputs, 1);
    }

    क्रम(i = 0; i < l.batch; ++i){
        भग्न *निवेश = net.निवेश_gpu + i*l.w*l.h*l.c;
        im2col_gpu(निवेश, l.c, l.h, l.w, 
                l.आकार, l.stride, l.pad, net.workspace);
        भग्न *output = l.output_gpu + i*l.outputs;
        क्रम(j = 0; j < locations; ++j){
            भग्न *a = l.weights_gpu + j*l.आकार*l.आकार*l.c*l.n;
            भग्न *b = net.workspace + j;
            भग्न *c = output + j;

            पूर्णांक m = l.n;
            पूर्णांक n = 1;
            पूर्णांक k = l.आकार*l.आकार*l.c;

            gemm_gpu(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

व्योम backward_local_layer_gpu(local_layer l, network net)
{
    पूर्णांक i, j;
    पूर्णांक locations = l.out_w*l.out_h;

    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    क्रम(i = 0; i < l.batch; ++i){
        axpy_gpu(l.outputs, 1, l.delta_gpu + i*l.outputs, 1, l.bias_updates_gpu, 1);
    }

    क्रम(i = 0; i < l.batch; ++i){
        भग्न *निवेश = net.निवेश_gpu + i*l.w*l.h*l.c;
        im2col_gpu(निवेश, l.c, l.h, l.w, 
                l.आकार, l.stride, l.pad, net.workspace);

        क्रम(j = 0; j < locations; ++j){ 
            भग्न *a = l.delta_gpu + i*l.outputs + j;
            भग्न *b = net.workspace + j;
            भग्न *c = l.weight_updates_gpu + j*l.आकार*l.आकार*l.c*l.n;
            पूर्णांक m = l.n;
            पूर्णांक n = l.आकार*l.आकार*l.c;
            पूर्णांक k = 1;

            gemm_gpu(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        अगर(net.delta_gpu){
            क्रम(j = 0; j < locations; ++j){ 
                भग्न *a = l.weights_gpu + j*l.आकार*l.आकार*l.c*l.n;
                भग्न *b = l.delta_gpu + i*l.outputs + j;
                भग्न *c = net.workspace + j;

                पूर्णांक m = l.आकार*l.आकार*l.c;
                पूर्णांक n = 1;
                पूर्णांक k = l.n;

                gemm_gpu(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_gpu(net.workspace, l.c,  l.h,  l.w,  l.आकार,  l.stride, l.pad, net.delta_gpu+i*l.c*l.h*l.w);
        }
    }
}

व्योम update_local_layer_gpu(local_layer l, update_args a)
{
    भग्न learning_rate = a.learning_rate*l.learning_rate_scale;
    भग्न momentum = a.momentum;
    भग्न decay = a.decay;
    पूर्णांक batch = a.batch;

    पूर्णांक locations = l.out_w*l.out_h;
    पूर्णांक आकार = l.आकार*l.आकार*l.c*l.n*locations;
    axpy_gpu(l.outputs, learning_rate/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
    scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

    axpy_gpu(आकार, -decay*batch, l.weights_gpu, 1, l.weight_updates_gpu, 1);
    axpy_gpu(आकार, learning_rate/batch, l.weight_updates_gpu, 1, l.weights_gpu, 1);
    scal_gpu(आकार, momentum, l.weight_updates_gpu, 1);
}

व्योम pull_local_layer(local_layer l)
{
    पूर्णांक locations = l.out_w*l.out_h;
    पूर्णांक आकार = l.आकार*l.आकार*l.c*l.n*locations;
    cuda_pull_array(l.weights_gpu, l.weights, आकार);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
}

व्योम धकेलो_local_layer(local_layer l)
{
    पूर्णांक locations = l.out_w*l.out_h;
    पूर्णांक आकार = l.आकार*l.आकार*l.c*l.n*locations;
    cuda_धकेलो_array(l.weights_gpu, l.weights, आकार);
    cuda_धकेलो_array(l.biases_gpu, l.biases, l.outputs);
}
#पूर्ण_अगर

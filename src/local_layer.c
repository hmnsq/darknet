#ಅನ್ತರ್ಭಾವ "local_layer.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "im2col.h"
#ಅನ್ತರ್ಭಾವ "col2im.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "gemm.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <time.h>

ಪೂರ್ಣಾಙ್ಕ local_out_height(local_layer l)
{
    ಪೂರ್ಣಾಙ್ಕ h = l.h;
    ಯದಿ (!l.pad) h -= l.size;
    ಅನ್ಯಥಾ h -= 1;
    ಪ್ರತ್ಯಾಗಮ h/l.stride + 1;
}

ಪೂರ್ಣಾಙ್ಕ local_out_width(local_layer l)
{
    ಪೂರ್ಣಾಙ್ಕ w = l.w;
    ಯದಿ (!l.pad) w -= l.size;
    ಅನ್ಯಥಾ w -= 1;
    ಪ್ರತ್ಯಾಗಮ w/l.stride + 1;
}

local_layer make_local_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ size, ಪೂರ್ಣಾಙ್ಕ stride, ಪೂರ್ಣಾಙ್ಕ pad, ACTIVATION activation)
{
    ಪೂರ್ಣಾಙ್ಕ i;
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

    ಪೂರ್ಣಾಙ್ಕ out_h = local_out_height(l);
    ಪೂರ್ಣಾಙ್ಕ out_w = local_out_width(l);
    ಪೂರ್ಣಾಙ್ಕ locations = out_h*out_w;
    l.out_h = out_h;
    l.out_w = out_w;
    l.out_c = n;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = l.w * l.h * l.c;

    l.weights = calloc(c*n*size*size*locations, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.weight_updates = calloc(c*n*size*size*locations, ಪರಿಮಾಣ(ಭಿನ್ನ));

    l.biases = calloc(l.outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.bias_updates = calloc(l.outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));

    // float scale = 1./sqrt(size*size*c);
    ಭಿನ್ನ scale = sqrt(2./(size*size*c));
    ಪ್ರತಿ(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1,1);

    l.output = calloc(l.batch*out_h * out_w * n, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.delta  = calloc(l.batch*out_h * out_w * n, ಪರಿಮಾಣ(ಭಿನ್ನ));

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

    ಪ್ರತ್ಯಾಗಮ l;
}

ಶೂನ್ಯ forward_local_layer(ಧ್ರುವ local_layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ out_h = local_out_height(l);
    ಪೂರ್ಣಾಙ್ಕ out_w = local_out_width(l);
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಪೂರ್ಣಾಙ್ಕ locations = out_h * out_w;

    ಪ್ರತಿ(i = 0; i < l.batch; ++i){
        copy_cpu(l.outputs, l.biases, 1, l.output + i*l.outputs, 1);
    }

    ಪ್ರತಿ(i = 0; i < l.batch; ++i){
        ಭಿನ್ನ *ಒಳಹರಿವು = net.ಒಳಹರಿವು + i*l.w*l.h*l.c;
        im2col_cpu(ಒಳಹರಿವು, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);
        ಭಿನ್ನ *output = l.output + i*l.outputs;
        ಪ್ರತಿ(j = 0; j < locations; ++j){
            ಭಿನ್ನ *a = l.weights + j*l.size*l.size*l.c*l.n;
            ಭಿನ್ನ *b = net.workspace + j;
            ಭಿನ್ನ *c = output + j;

            ಪೂರ್ಣಾಙ್ಕ m = l.n;
            ಪೂರ್ಣಾಙ್ಕ n = 1;
            ಪೂರ್ಣಾಙ್ಕ k = l.size*l.size*l.c;

            gemm(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

ಶೂನ್ಯ backward_local_layer(local_layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಪೂರ್ಣಾಙ್ಕ locations = l.out_w*l.out_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    ಪ್ರತಿ(i = 0; i < l.batch; ++i){
        axpy_cpu(l.outputs, 1, l.delta + i*l.outputs, 1, l.bias_updates, 1);
    }

    ಪ್ರತಿ(i = 0; i < l.batch; ++i){
        ಭಿನ್ನ *ಒಳಹರಿವು = net.ಒಳಹರಿವು + i*l.w*l.h*l.c;
        im2col_cpu(ಒಳಹರಿವು, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);

        ಪ್ರತಿ(j = 0; j < locations; ++j){ 
            ಭಿನ್ನ *a = l.delta + i*l.outputs + j;
            ಭಿನ್ನ *b = net.workspace + j;
            ಭಿನ್ನ *c = l.weight_updates + j*l.size*l.size*l.c*l.n;
            ಪೂರ್ಣಾಙ್ಕ m = l.n;
            ಪೂರ್ಣಾಙ್ಕ n = l.size*l.size*l.c;
            ಪೂರ್ಣಾಙ್ಕ k = 1;

            gemm(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        ಯದಿ(net.delta){
            ಪ್ರತಿ(j = 0; j < locations; ++j){ 
                ಭಿನ್ನ *a = l.weights + j*l.size*l.size*l.c*l.n;
                ಭಿನ್ನ *b = l.delta + i*l.outputs + j;
                ಭಿನ್ನ *c = net.workspace + j;

                ಪೂರ್ಣಾಙ್ಕ m = l.size*l.size*l.c;
                ಪೂರ್ಣಾಙ್ಕ n = 1;
                ಪೂರ್ಣಾಙ್ಕ k = l.n;

                gemm(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_cpu(net.workspace, l.c,  l.h,  l.w,  l.size,  l.stride, l.pad, net.delta+i*l.c*l.h*l.w);
        }
    }
}

ಶೂನ್ಯ update_local_layer(local_layer l, update_args a)
{
    ಭಿನ್ನ learning_rate = a.learning_rate*l.learning_rate_scale;
    ಭಿನ್ನ momentum = a.momentum;
    ಭಿನ್ನ decay = a.decay;
    ಪೂರ್ಣಾಙ್ಕ batch = a.batch;

    ಪೂರ್ಣಾಙ್ಕ locations = l.out_w*l.out_h;
    ಪೂರ್ಣಾಙ್ಕ size = l.size*l.size*l.c*l.n*locations;
    axpy_cpu(l.outputs, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    axpy_cpu(size, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(size, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(size, momentum, l.weight_updates, 1);
}

#ifdef GPU

ಶೂನ್ಯ forward_local_layer_gpu(ಧ್ರುವ local_layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ out_h = local_out_height(l);
    ಪೂರ್ಣಾಙ್ಕ out_w = local_out_width(l);
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಪೂರ್ಣಾಙ್ಕ locations = out_h * out_w;

    ಪ್ರತಿ(i = 0; i < l.batch; ++i){
        copy_gpu(l.outputs, l.biases_gpu, 1, l.output_gpu + i*l.outputs, 1);
    }

    ಪ್ರತಿ(i = 0; i < l.batch; ++i){
        ಭಿನ್ನ *ಒಳಹರಿವು = net.ಒಳಹರಿವು_gpu + i*l.w*l.h*l.c;
        im2col_gpu(ಒಳಹರಿವು, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);
        ಭಿನ್ನ *output = l.output_gpu + i*l.outputs;
        ಪ್ರತಿ(j = 0; j < locations; ++j){
            ಭಿನ್ನ *a = l.weights_gpu + j*l.size*l.size*l.c*l.n;
            ಭಿನ್ನ *b = net.workspace + j;
            ಭಿನ್ನ *c = output + j;

            ಪೂರ್ಣಾಙ್ಕ m = l.n;
            ಪೂರ್ಣಾಙ್ಕ n = 1;
            ಪೂರ್ಣಾಙ್ಕ k = l.size*l.size*l.c;

            gemm_gpu(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

ಶೂನ್ಯ backward_local_layer_gpu(local_layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಪೂರ್ಣಾಙ್ಕ locations = l.out_w*l.out_h;

    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    ಪ್ರತಿ(i = 0; i < l.batch; ++i){
        axpy_gpu(l.outputs, 1, l.delta_gpu + i*l.outputs, 1, l.bias_updates_gpu, 1);
    }

    ಪ್ರತಿ(i = 0; i < l.batch; ++i){
        ಭಿನ್ನ *ಒಳಹರಿವು = net.ಒಳಹರಿವು_gpu + i*l.w*l.h*l.c;
        im2col_gpu(ಒಳಹರಿವು, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);

        ಪ್ರತಿ(j = 0; j < locations; ++j){ 
            ಭಿನ್ನ *a = l.delta_gpu + i*l.outputs + j;
            ಭಿನ್ನ *b = net.workspace + j;
            ಭಿನ್ನ *c = l.weight_updates_gpu + j*l.size*l.size*l.c*l.n;
            ಪೂರ್ಣಾಙ್ಕ m = l.n;
            ಪೂರ್ಣಾಙ್ಕ n = l.size*l.size*l.c;
            ಪೂರ್ಣಾಙ್ಕ k = 1;

            gemm_gpu(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        ಯದಿ(net.delta_gpu){
            ಪ್ರತಿ(j = 0; j < locations; ++j){ 
                ಭಿನ್ನ *a = l.weights_gpu + j*l.size*l.size*l.c*l.n;
                ಭಿನ್ನ *b = l.delta_gpu + i*l.outputs + j;
                ಭಿನ್ನ *c = net.workspace + j;

                ಪೂರ್ಣಾಙ್ಕ m = l.size*l.size*l.c;
                ಪೂರ್ಣಾಙ್ಕ n = 1;
                ಪೂರ್ಣಾಙ್ಕ k = l.n;

                gemm_gpu(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_gpu(net.workspace, l.c,  l.h,  l.w,  l.size,  l.stride, l.pad, net.delta_gpu+i*l.c*l.h*l.w);
        }
    }
}

ಶೂನ್ಯ update_local_layer_gpu(local_layer l, update_args a)
{
    ಭಿನ್ನ learning_rate = a.learning_rate*l.learning_rate_scale;
    ಭಿನ್ನ momentum = a.momentum;
    ಭಿನ್ನ decay = a.decay;
    ಪೂರ್ಣಾಙ್ಕ batch = a.batch;

    ಪೂರ್ಣಾಙ್ಕ locations = l.out_w*l.out_h;
    ಪೂರ್ಣಾಙ್ಕ size = l.size*l.size*l.c*l.n*locations;
    axpy_gpu(l.outputs, learning_rate/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
    scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

    axpy_gpu(size, -decay*batch, l.weights_gpu, 1, l.weight_updates_gpu, 1);
    axpy_gpu(size, learning_rate/batch, l.weight_updates_gpu, 1, l.weights_gpu, 1);
    scal_gpu(size, momentum, l.weight_updates_gpu, 1);
}

ಶೂನ್ಯ pull_local_layer(local_layer l)
{
    ಪೂರ್ಣಾಙ್ಕ locations = l.out_w*l.out_h;
    ಪೂರ್ಣಾಙ್ಕ size = l.size*l.size*l.c*l.n*locations;
    cuda_pull_array(l.weights_gpu, l.weights, size);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
}

ಶೂನ್ಯ ತಳ್ಳು_local_layer(local_layer l)
{
    ಪೂರ್ಣಾಙ್ಕ locations = l.out_w*l.out_h;
    ಪೂರ್ಣಾಙ್ಕ size = l.size*l.size*l.c*l.n*locations;
    cuda_ತಳ್ಳು_array(l.weights_gpu, l.weights, size);
    cuda_ತಳ್ಳು_array(l.biases_gpu, l.biases, l.outputs);
}
#endif

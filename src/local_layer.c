#ಅನ್ತರ್ಭಾವ "local_layer.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "im2col.h"
#ಅನ್ತರ್ಭಾವ "col2im.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "gemm.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <time.h>

ಪೂರ್ಣಾಂಕ local_ಹೊರಹೋಗು_ಎತ್ತರ(local_layer l)
{
    ಪೂರ್ಣಾಂಕ h = l.h;
    ಆದರೆ (!l.pad) h -= l.ಗಾತ್ರ_2;
    ಇಲ್ಲದಿದ್ದರೆ h -= 1;
    ಹಿಂತಿರುಗಿ h/l.stride + 1;
}

ಪೂರ್ಣಾಂಕ local_ಹೊರಹೋಗು_ಅಗಲ(local_layer l)
{
    ಪೂರ್ಣಾಂಕ w = l.w;
    ಆದರೆ (!l.pad) w -= l.ಗಾತ್ರ_2;
    ಇಲ್ಲದಿದ್ದರೆ w -= 1;
    ಹಿಂತಿರುಗಿ w/l.stride + 1;
}

local_layer make_local_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ಪೂರ್ಣಾಂಕ stride, ಪೂರ್ಣಾಂಕ pad, ACTIVATION activation)
{
    ಪೂರ್ಣಾಂಕ i;
    local_layer l = {0};
    l.type = LOCAL;

    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.batch = batch;
    l.stride = stride;
    l.ಗಾತ್ರ_2 = ಗಾತ್ರ_2;
    l.pad = pad;

    ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_h = local_ಹೊರಹೋಗು_ಎತ್ತರ(l);
    ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_w = local_ಹೊರಹೋಗು_ಅಗಲ(l);
    ಪೂರ್ಣಾಂಕ locations = ಹೊರಹೋಗು_h*ಹೊರಹೋಗು_w;
    l.ಹೊರಹೋಗು_h = ಹೊರಹೋಗು_h;
    l.ಹೊರಹೋಗು_w = ಹೊರಹೋಗು_w;
    l.ಹೊರಹೋಗು_c = n;
    l.outputs = l.ಹೊರಹೋಗು_h * l.ಹೊರಹೋಗು_w * l.ಹೊರಹೋಗು_c;
    l.inputs = l.w * l.h * l.c;

    l.weights = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(c*n*ಗಾತ್ರ_2*ಗಾತ್ರ_2*locations, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.ತೂಕ_updates = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(c*n*ಗಾತ್ರ_2*ಗಾತ್ರ_2*locations, ಗಾತ್ರ(ದಶಮಾಂಶ));

    l.biases = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.bias_updates = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));

    // float scale = 1./sqrt(size*size*c);
    ದಶಮಾಂಶ scale = ವರ್ಗಮೂಲ(2./(ಗಾತ್ರ_2*ಗಾತ್ರ_2*c));
    ಗಾಗಿ(i = 0; i < c*n*ಗಾತ್ರ_2*ಗಾತ್ರ_2; ++i) l.weights[i] = scale*rand_ಏಕರೂಪ(-1,1);

    l.output = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.batch*ಹೊರಹೋಗು_h * ಹೊರಹೋಗು_w * n, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.delta  = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.batch*ಹೊರಹೋಗು_h * ಹೊರಹೋಗು_w * n, ಗಾತ್ರ(ದಶಮಾಂಶ));

    l.workspace_ಗಾತ್ರ = ಹೊರಹೋಗು_h*ಹೊರಹೋಗು_w*ಗಾತ್ರ_2*ಗಾತ್ರ_2*c;
    
    l.forward = forward_local_layer;
    l.backward = backward_local_layer;
    l.update = update_local_layer;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l.forward_gpu = forward_local_layer_gpu;
    l.backward_gpu = backward_local_layer_gpu;
    l.update_gpu = update_local_layer_gpu;

    l.weights_gpu = cuda_make_array(l.weights, c*n*ಗಾತ್ರ_2*ಗಾತ್ರ_2*locations);
    l.ತೂಕ_updates_gpu = cuda_make_array(l.ತೂಕ_updates, c*n*ಗಾತ್ರ_2*ಗಾತ್ರ_2*locations);

    l.biases_gpu = cuda_make_array(l.biases, l.outputs);
    l.bias_updates_gpu = cuda_make_array(l.bias_updates, l.outputs);

    l.delta_gpu = cuda_make_array(l.delta, l.batch*ಹೊರಹೋಗು_h*ಹೊರಹೋಗು_w*n);
    l.output_gpu = cuda_make_array(l.output, l.batch*ಹೊರಹೋಗು_h*ಹೊರಹೋಗು_w*n);

#ಆದರೆ_ಅಂತ್ಯ
    l.activation = activation;

    fprintf(stderr, "Local Layer: %d x %d x %d image, %d filters -> %d x %d x %d image\n", h,w,c,n, ಹೊರಹೋಗು_h, ಹೊರಹೋಗು_w, n);

    ಹಿಂತಿರುಗಿ l;
}

ಶೂನ್ಯ forward_local_layer(ಸ್ಥಿರಾಂಕ local_layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_h = local_ಹೊರಹೋಗು_ಎತ್ತರ(l);
    ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_w = local_ಹೊರಹೋಗು_ಅಗಲ(l);
    ಪೂರ್ಣಾಂಕ i, j;
    ಪೂರ್ಣಾಂಕ locations = ಹೊರಹೋಗು_h * ಹೊರಹೋಗು_w;

    ಗಾಗಿ(i = 0; i < l.batch; ++i){
        copy_cpu(l.outputs, l.biases, 1, l.output + i*l.outputs, 1);
    }

    ಗಾಗಿ(i = 0; i < l.batch; ++i){
        ದಶಮಾಂಶ *ಒಳಹರಿವು = net.ಒಳಹರಿವು + i*l.w*l.h*l.c;
        im2col_cpu(ಒಳಹರಿವು, l.c, l.h, l.w, 
                l.ಗಾತ್ರ_2, l.stride, l.pad, net.workspace);
        ದಶಮಾಂಶ *output = l.output + i*l.outputs;
        ಗಾಗಿ(j = 0; j < locations; ++j){
            ದಶಮಾಂಶ *a = l.weights + j*l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.c*l.n;
            ದಶಮಾಂಶ *b = net.workspace + j;
            ದಶಮಾಂಶ *c = output + j;

            ಪೂರ್ಣಾಂಕ m = l.n;
            ಪೂರ್ಣಾಂಕ n = 1;
            ಪೂರ್ಣಾಂಕ k = l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.c;

            gemm(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

ಶೂನ್ಯ backward_local_layer(local_layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ i, j;
    ಪೂರ್ಣಾಂಕ locations = l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    ಗಾಗಿ(i = 0; i < l.batch; ++i){
        axpy_cpu(l.outputs, 1, l.delta + i*l.outputs, 1, l.bias_updates, 1);
    }

    ಗಾಗಿ(i = 0; i < l.batch; ++i){
        ದಶಮಾಂಶ *ಒಳಹರಿವು = net.ಒಳಹರಿವು + i*l.w*l.h*l.c;
        im2col_cpu(ಒಳಹರಿವು, l.c, l.h, l.w, 
                l.ಗಾತ್ರ_2, l.stride, l.pad, net.workspace);

        ಗಾಗಿ(j = 0; j < locations; ++j){ 
            ದಶಮಾಂಶ *a = l.delta + i*l.outputs + j;
            ದಶಮಾಂಶ *b = net.workspace + j;
            ದಶಮಾಂಶ *c = l.ತೂಕ_updates + j*l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.c*l.n;
            ಪೂರ್ಣಾಂಕ m = l.n;
            ಪೂರ್ಣಾಂಕ n = l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.c;
            ಪೂರ್ಣಾಂಕ k = 1;

            gemm(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        ಆದರೆ(net.delta){
            ಗಾಗಿ(j = 0; j < locations; ++j){ 
                ದಶಮಾಂಶ *a = l.weights + j*l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.c*l.n;
                ದಶಮಾಂಶ *b = l.delta + i*l.outputs + j;
                ದಶಮಾಂಶ *c = net.workspace + j;

                ಪೂರ್ಣಾಂಕ m = l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.c;
                ಪೂರ್ಣಾಂಕ n = 1;
                ಪೂರ್ಣಾಂಕ k = l.n;

                gemm(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_cpu(net.workspace, l.c,  l.h,  l.w,  l.ಗಾತ್ರ_2,  l.stride, l.pad, net.delta+i*l.c*l.h*l.w);
        }
    }
}

ಶೂನ್ಯ update_local_layer(local_layer l, update_ವಾದಗಳು a)
{
    ದಶಮಾಂಶ learning_ದರ = a.learning_ದರ*l.learning_ದರ_scale;
    ದಶಮಾಂಶ momentum = a.momentum;
    ದಶಮಾಂಶ decay = a.decay;
    ಪೂರ್ಣಾಂಕ batch = a.batch;

    ಪೂರ್ಣಾಂಕ locations = l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h;
    ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2 = l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.c*l.n*locations;
    axpy_cpu(l.outputs, learning_ದರ/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    axpy_cpu(ಗಾತ್ರ_2, -decay*batch, l.weights, 1, l.ತೂಕ_updates, 1);
    axpy_cpu(ಗಾತ್ರ_2, learning_ದರ/batch, l.ತೂಕ_updates, 1, l.weights, 1);
    scal_cpu(ಗಾತ್ರ_2, momentum, l.ತೂಕ_updates, 1);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU

ಶೂನ್ಯ forward_local_layer_gpu(ಸ್ಥಿರಾಂಕ local_layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_h = local_ಹೊರಹೋಗು_ಎತ್ತರ(l);
    ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_w = local_ಹೊರಹೋಗು_ಅಗಲ(l);
    ಪೂರ್ಣಾಂಕ i, j;
    ಪೂರ್ಣಾಂಕ locations = ಹೊರಹೋಗು_h * ಹೊರಹೋಗು_w;

    ಗಾಗಿ(i = 0; i < l.batch; ++i){
        copy_gpu(l.outputs, l.biases_gpu, 1, l.output_gpu + i*l.outputs, 1);
    }

    ಗಾಗಿ(i = 0; i < l.batch; ++i){
        ದಶಮಾಂಶ *ಒಳಹರಿವು = net.ಒಳಹರಿವು_gpu + i*l.w*l.h*l.c;
        im2col_gpu(ಒಳಹರಿವು, l.c, l.h, l.w, 
                l.ಗಾತ್ರ_2, l.stride, l.pad, net.workspace);
        ದಶಮಾಂಶ *output = l.output_gpu + i*l.outputs;
        ಗಾಗಿ(j = 0; j < locations; ++j){
            ದಶಮಾಂಶ *a = l.weights_gpu + j*l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.c*l.n;
            ದಶಮಾಂಶ *b = net.workspace + j;
            ದಶಮಾಂಶ *c = output + j;

            ಪೂರ್ಣಾಂಕ m = l.n;
            ಪೂರ್ಣಾಂಕ n = 1;
            ಪೂರ್ಣಾಂಕ k = l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.c;

            gemm_gpu(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

ಶೂನ್ಯ backward_local_layer_gpu(local_layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ i, j;
    ಪೂರ್ಣಾಂಕ locations = l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h;

    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    ಗಾಗಿ(i = 0; i < l.batch; ++i){
        axpy_gpu(l.outputs, 1, l.delta_gpu + i*l.outputs, 1, l.bias_updates_gpu, 1);
    }

    ಗಾಗಿ(i = 0; i < l.batch; ++i){
        ದಶಮಾಂಶ *ಒಳಹರಿವು = net.ಒಳಹರಿವು_gpu + i*l.w*l.h*l.c;
        im2col_gpu(ಒಳಹರಿವು, l.c, l.h, l.w, 
                l.ಗಾತ್ರ_2, l.stride, l.pad, net.workspace);

        ಗಾಗಿ(j = 0; j < locations; ++j){ 
            ದಶಮಾಂಶ *a = l.delta_gpu + i*l.outputs + j;
            ದಶಮಾಂಶ *b = net.workspace + j;
            ದಶಮಾಂಶ *c = l.ತೂಕ_updates_gpu + j*l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.c*l.n;
            ಪೂರ್ಣಾಂಕ m = l.n;
            ಪೂರ್ಣಾಂಕ n = l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.c;
            ಪೂರ್ಣಾಂಕ k = 1;

            gemm_gpu(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        ಆದರೆ(net.delta_gpu){
            ಗಾಗಿ(j = 0; j < locations; ++j){ 
                ದಶಮಾಂಶ *a = l.weights_gpu + j*l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.c*l.n;
                ದಶಮಾಂಶ *b = l.delta_gpu + i*l.outputs + j;
                ದಶಮಾಂಶ *c = net.workspace + j;

                ಪೂರ್ಣಾಂಕ m = l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.c;
                ಪೂರ್ಣಾಂಕ n = 1;
                ಪೂರ್ಣಾಂಕ k = l.n;

                gemm_gpu(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_gpu(net.workspace, l.c,  l.h,  l.w,  l.ಗಾತ್ರ_2,  l.stride, l.pad, net.delta_gpu+i*l.c*l.h*l.w);
        }
    }
}

ಶೂನ್ಯ update_local_layer_gpu(local_layer l, update_ವಾದಗಳು a)
{
    ದಶಮಾಂಶ learning_ದರ = a.learning_ದರ*l.learning_ದರ_scale;
    ದಶಮಾಂಶ momentum = a.momentum;
    ದಶಮಾಂಶ decay = a.decay;
    ಪೂರ್ಣಾಂಕ batch = a.batch;

    ಪೂರ್ಣಾಂಕ locations = l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h;
    ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2 = l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.c*l.n*locations;
    axpy_gpu(l.outputs, learning_ದರ/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
    scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

    axpy_gpu(ಗಾತ್ರ_2, -decay*batch, l.weights_gpu, 1, l.ತೂಕ_updates_gpu, 1);
    axpy_gpu(ಗಾತ್ರ_2, learning_ದರ/batch, l.ತೂಕ_updates_gpu, 1, l.weights_gpu, 1);
    scal_gpu(ಗಾತ್ರ_2, momentum, l.ತೂಕ_updates_gpu, 1);
}

ಶೂನ್ಯ pull_local_layer(local_layer l)
{
    ಪೂರ್ಣಾಂಕ locations = l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h;
    ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2 = l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.c*l.n*locations;
    cuda_pull_array(l.weights_gpu, l.weights, ಗಾತ್ರ_2);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
}

ಶೂನ್ಯ ತಳ್ಳು_local_layer(local_layer l)
{
    ಪೂರ್ಣಾಂಕ locations = l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h;
    ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2 = l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.c*l.n*locations;
    cuda_ತಳ್ಳು_array(l.weights_gpu, l.weights, ಗಾತ್ರ_2);
    cuda_ತಳ್ಳು_array(l.biases_gpu, l.biases, l.outputs);
}
#ಆದರೆ_ಅಂತ್ಯ

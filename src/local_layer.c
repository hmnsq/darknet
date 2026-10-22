#અન્તર્ભાવ "local_layer.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "im2col.h"
#અન્તર્ભાવ "col2im.h"
#અન્તર્ભાવ "blas.h"
#અન્તર્ભાવ "gemm.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <time.h>

પૂર્ણાંક local_out_height(local_layer l)
{
    પૂર્ણાંક h = l.h;
    જો (!l.pad) h -= l.size;
    નહીંતર h -= 1;
    પાછા h/l.stride + 1;
}

પૂર્ણાંક local_out_width(local_layer l)
{
    પૂર્ણાંક w = l.w;
    જો (!l.pad) w -= l.size;
    નહીંતર w -= 1;
    પાછા w/l.stride + 1;
}

local_layer make_local_layer(પૂર્ણાંક batch, પૂર્ણાંક h, પૂર્ણાંક w, પૂર્ણાંક c, પૂર્ણાંક n, પૂર્ણાંક size, પૂર્ણાંક stride, પૂર્ણાંક pad, ACTIVATION activation)
{
    પૂર્ણાંક i;
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

    પૂર્ણાંક out_h = local_out_height(l);
    પૂર્ણાંક out_w = local_out_width(l);
    પૂર્ણાંક locations = out_h*out_w;
    l.out_h = out_h;
    l.out_w = out_w;
    l.out_c = n;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = l.w * l.h * l.c;

    l.weights = calloc(c*n*size*size*locations, કદ(દશાંશ));
    l.weight_updates = calloc(c*n*size*size*locations, કદ(દશાંશ));

    l.biases = calloc(l.outputs, કદ(દશાંશ));
    l.bias_updates = calloc(l.outputs, કદ(દશાંશ));

    // float scale = 1./sqrt(size*size*c);
    દશાંશ scale = sqrt(2./(size*size*c));
    માટે(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1,1);

    l.output = calloc(l.batch*out_h * out_w * n, કદ(દશાંશ));
    l.delta  = calloc(l.batch*out_h * out_w * n, કદ(દશાંશ));

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

    પાછા l;
}

શૂન્ય forward_local_layer(અચલ local_layer l, network net)
{
    પૂર્ણાંક out_h = local_out_height(l);
    પૂર્ણાંક out_w = local_out_width(l);
    પૂર્ણાંક i, j;
    પૂર્ણાંક locations = out_h * out_w;

    માટે(i = 0; i < l.batch; ++i){
        copy_cpu(l.outputs, l.biases, 1, l.output + i*l.outputs, 1);
    }

    માટે(i = 0; i < l.batch; ++i){
        દશાંશ *આગત = net.આગત + i*l.w*l.h*l.c;
        im2col_cpu(આગત, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);
        દશાંશ *output = l.output + i*l.outputs;
        માટે(j = 0; j < locations; ++j){
            દશાંશ *a = l.weights + j*l.size*l.size*l.c*l.n;
            દશાંશ *b = net.workspace + j;
            દશાંશ *c = output + j;

            પૂર્ણાંક m = l.n;
            પૂર્ણાંક n = 1;
            પૂર્ણાંક k = l.size*l.size*l.c;

            gemm(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

શૂન્ય backward_local_layer(local_layer l, network net)
{
    પૂર્ણાંક i, j;
    પૂર્ણાંક locations = l.out_w*l.out_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    માટે(i = 0; i < l.batch; ++i){
        axpy_cpu(l.outputs, 1, l.delta + i*l.outputs, 1, l.bias_updates, 1);
    }

    માટે(i = 0; i < l.batch; ++i){
        દશાંશ *આગત = net.આગત + i*l.w*l.h*l.c;
        im2col_cpu(આગત, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);

        માટે(j = 0; j < locations; ++j){ 
            દશાંશ *a = l.delta + i*l.outputs + j;
            દશાંશ *b = net.workspace + j;
            દશાંશ *c = l.weight_updates + j*l.size*l.size*l.c*l.n;
            પૂર્ણાંક m = l.n;
            પૂર્ણાંક n = l.size*l.size*l.c;
            પૂર્ણાંક k = 1;

            gemm(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        જો(net.delta){
            માટે(j = 0; j < locations; ++j){ 
                દશાંશ *a = l.weights + j*l.size*l.size*l.c*l.n;
                દશાંશ *b = l.delta + i*l.outputs + j;
                દશાંશ *c = net.workspace + j;

                પૂર્ણાંક m = l.size*l.size*l.c;
                પૂર્ણાંક n = 1;
                પૂર્ણાંક k = l.n;

                gemm(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_cpu(net.workspace, l.c,  l.h,  l.w,  l.size,  l.stride, l.pad, net.delta+i*l.c*l.h*l.w);
        }
    }
}

શૂન્ય update_local_layer(local_layer l, update_args a)
{
    દશાંશ learning_rate = a.learning_rate*l.learning_rate_scale;
    દશાંશ momentum = a.momentum;
    દશાંશ decay = a.decay;
    પૂર્ણાંક batch = a.batch;

    પૂર્ણાંક locations = l.out_w*l.out_h;
    પૂર્ણાંક size = l.size*l.size*l.c*l.n*locations;
    axpy_cpu(l.outputs, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    axpy_cpu(size, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(size, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(size, momentum, l.weight_updates, 1);
}

#ifdef GPU

શૂન્ય forward_local_layer_gpu(અચલ local_layer l, network net)
{
    પૂર્ણાંક out_h = local_out_height(l);
    પૂર્ણાંક out_w = local_out_width(l);
    પૂર્ણાંક i, j;
    પૂર્ણાંક locations = out_h * out_w;

    માટે(i = 0; i < l.batch; ++i){
        copy_gpu(l.outputs, l.biases_gpu, 1, l.output_gpu + i*l.outputs, 1);
    }

    માટે(i = 0; i < l.batch; ++i){
        દશાંશ *આગત = net.આગત_gpu + i*l.w*l.h*l.c;
        im2col_gpu(આગત, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);
        દશાંશ *output = l.output_gpu + i*l.outputs;
        માટે(j = 0; j < locations; ++j){
            દશાંશ *a = l.weights_gpu + j*l.size*l.size*l.c*l.n;
            દશાંશ *b = net.workspace + j;
            દશાંશ *c = output + j;

            પૂર્ણાંક m = l.n;
            પૂર્ણાંક n = 1;
            પૂર્ણાંક k = l.size*l.size*l.c;

            gemm_gpu(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

શૂન્ય backward_local_layer_gpu(local_layer l, network net)
{
    પૂર્ણાંક i, j;
    પૂર્ણાંક locations = l.out_w*l.out_h;

    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    માટે(i = 0; i < l.batch; ++i){
        axpy_gpu(l.outputs, 1, l.delta_gpu + i*l.outputs, 1, l.bias_updates_gpu, 1);
    }

    માટે(i = 0; i < l.batch; ++i){
        દશાંશ *આગત = net.આગત_gpu + i*l.w*l.h*l.c;
        im2col_gpu(આગત, l.c, l.h, l.w, 
                l.size, l.stride, l.pad, net.workspace);

        માટે(j = 0; j < locations; ++j){ 
            દશાંશ *a = l.delta_gpu + i*l.outputs + j;
            દશાંશ *b = net.workspace + j;
            દશાંશ *c = l.weight_updates_gpu + j*l.size*l.size*l.c*l.n;
            પૂર્ણાંક m = l.n;
            પૂર્ણાંક n = l.size*l.size*l.c;
            પૂર્ણાંક k = 1;

            gemm_gpu(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        જો(net.delta_gpu){
            માટે(j = 0; j < locations; ++j){ 
                દશાંશ *a = l.weights_gpu + j*l.size*l.size*l.c*l.n;
                દશાંશ *b = l.delta_gpu + i*l.outputs + j;
                દશાંશ *c = net.workspace + j;

                પૂર્ણાંક m = l.size*l.size*l.c;
                પૂર્ણાંક n = 1;
                પૂર્ણાંક k = l.n;

                gemm_gpu(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_gpu(net.workspace, l.c,  l.h,  l.w,  l.size,  l.stride, l.pad, net.delta_gpu+i*l.c*l.h*l.w);
        }
    }
}

શૂન્ય update_local_layer_gpu(local_layer l, update_args a)
{
    દશાંશ learning_rate = a.learning_rate*l.learning_rate_scale;
    દશાંશ momentum = a.momentum;
    દશાંશ decay = a.decay;
    પૂર્ણાંક batch = a.batch;

    પૂર્ણાંક locations = l.out_w*l.out_h;
    પૂર્ણાંક size = l.size*l.size*l.c*l.n*locations;
    axpy_gpu(l.outputs, learning_rate/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
    scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

    axpy_gpu(size, -decay*batch, l.weights_gpu, 1, l.weight_updates_gpu, 1);
    axpy_gpu(size, learning_rate/batch, l.weight_updates_gpu, 1, l.weights_gpu, 1);
    scal_gpu(size, momentum, l.weight_updates_gpu, 1);
}

શૂન્ય pull_local_layer(local_layer l)
{
    પૂર્ણાંક locations = l.out_w*l.out_h;
    પૂર્ણાંક size = l.size*l.size*l.c*l.n*locations;
    cuda_pull_array(l.weights_gpu, l.weights, size);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
}

શૂન્ય ધકેલો_local_layer(local_layer l)
{
    પૂર્ણાંક locations = l.out_w*l.out_h;
    પૂર્ણાંક size = l.size*l.size*l.c*l.n*locations;
    cuda_ધકેલો_array(l.weights_gpu, l.weights, size);
    cuda_ધકેલો_array(l.biases_gpu, l.biases, l.outputs);
}
#endif

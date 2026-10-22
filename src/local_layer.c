#ضمن "local_layer.h"
#ضمن "utils.h"
#ضمن "im2col.h"
#ضمن "col2im.h"
#ضمن "blas.h"
#ضمن "gemm.h"
#ضمن <stdio.h>
#ضمن <time.h>

صحيح local_ٻاهريون_اوچائي(local_layer l)
{
    صحيح h = l.h;
    إذا (!l.pad) h -= l.سائيز;
    وإلا h -= 1;
    أرجع h/l.stride + 1;
}

صحيح local_ٻاهريون_ويڪر(local_layer l)
{
    صحيح w = l.w;
    إذا (!l.pad) w -= l.سائيز;
    وإلا w -= 1;
    أرجع w/l.stride + 1;
}

local_layer make_local_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح n, صحيح سائيز, صحيح stride, صحيح pad, ACTIVATION activation)
{
    صحيح i;
    local_layer l = {0};
    l.type = LOCAL;

    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.batch = batch;
    l.stride = stride;
    l.سائيز = سائيز;
    l.pad = pad;

    صحيح ٻاهريون_h = local_ٻاهريون_اوچائي(l);
    صحيح ٻاهريون_w = local_ٻاهريون_ويڪر(l);
    صحيح locations = ٻاهريون_h*ٻاهريون_w;
    l.ٻاهريون_h = ٻاهريون_h;
    l.ٻاهريون_w = ٻاهريون_w;
    l.ٻاهريون_c = n;
    l.outputs = l.ٻاهريون_h * l.ٻاهريون_w * l.ٻاهريون_c;
    l.inputs = l.w * l.h * l.c;

    l.weights = صفر_يادگيري_ڏيو(c*n*سائيز*سائيز*locations, حجم(عائم));
    l.وزن_updates = صفر_يادگيري_ڏيو(c*n*سائيز*سائيز*locations, حجم(عائم));

    l.biases = صفر_يادگيري_ڏيو(l.outputs, حجم(عائم));
    l.bias_updates = صفر_يادگيري_ڏيو(l.outputs, حجم(عائم));

    // float scale = 1./sqrt(size*size*c);
    عائم scale = جذر(2./(سائيز*سائيز*c));
    لكل(i = 0; i < c*n*سائيز*سائيز; ++i) l.weights[i] = scale*rand_هڪجهڙو(-1,1);

    l.output = صفر_يادگيري_ڏيو(l.batch*ٻاهريون_h * ٻاهريون_w * n, حجم(عائم));
    l.delta  = صفر_يادگيري_ڏيو(l.batch*ٻاهريون_h * ٻاهريون_w * n, حجم(عائم));

    l.workspace_سائيز = ٻاهريون_h*ٻاهريون_w*سائيز*سائيز*c;
    
    l.forward = forward_local_layer;
    l.backward = backward_local_layer;
    l.update = update_local_layer;

#جيڪڏهن_وصف_ٿيل GPU
    l.forward_gpu = forward_local_layer_gpu;
    l.backward_gpu = backward_local_layer_gpu;
    l.update_gpu = update_local_layer_gpu;

    l.weights_gpu = cuda_make_array(l.weights, c*n*سائيز*سائيز*locations);
    l.وزن_updates_gpu = cuda_make_array(l.وزن_updates, c*n*سائيز*سائيز*locations);

    l.biases_gpu = cuda_make_array(l.biases, l.outputs);
    l.bias_updates_gpu = cuda_make_array(l.bias_updates, l.outputs);

    l.delta_gpu = cuda_make_array(l.delta, l.batch*ٻاهريون_h*ٻاهريون_w*n);
    l.output_gpu = cuda_make_array(l.output, l.batch*ٻاهريون_h*ٻاهريون_w*n);

#جيڪڏهن_پورو
    l.activation = activation;

    fprintf(stderr, "Local Layer: %d x %d x %d image, %d filters -> %d x %d x %d image\n", h,w,c,n, ٻاهريون_h, ٻاهريون_w, n);

    أرجع l;
}

فراغ forward_local_layer(ثابت local_layer l, ڄار net)
{
    صحيح ٻاهريون_h = local_ٻاهريون_اوچائي(l);
    صحيح ٻاهريون_w = local_ٻاهريون_ويڪر(l);
    صحيح i, j;
    صحيح locations = ٻاهريون_h * ٻاهريون_w;

    لكل(i = 0; i < l.batch; ++i){
        copy_cpu(l.outputs, l.biases, 1, l.output + i*l.outputs, 1);
    }

    لكل(i = 0; i < l.batch; ++i){
        عائم *داخلا = net.داخلا + i*l.w*l.h*l.c;
        im2col_cpu(داخلا, l.c, l.h, l.w, 
                l.سائيز, l.stride, l.pad, net.workspace);
        عائم *output = l.output + i*l.outputs;
        لكل(j = 0; j < locations; ++j){
            عائم *a = l.weights + j*l.سائيز*l.سائيز*l.c*l.n;
            عائم *b = net.workspace + j;
            عائم *c = output + j;

            صحيح m = l.n;
            صحيح n = 1;
            صحيح k = l.سائيز*l.سائيز*l.c;

            gemm(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

فراغ backward_local_layer(local_layer l, ڄار net)
{
    صحيح i, j;
    صحيح locations = l.ٻاهريون_w*l.ٻاهريون_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    لكل(i = 0; i < l.batch; ++i){
        axpy_cpu(l.outputs, 1, l.delta + i*l.outputs, 1, l.bias_updates, 1);
    }

    لكل(i = 0; i < l.batch; ++i){
        عائم *داخلا = net.داخلا + i*l.w*l.h*l.c;
        im2col_cpu(داخلا, l.c, l.h, l.w, 
                l.سائيز, l.stride, l.pad, net.workspace);

        لكل(j = 0; j < locations; ++j){ 
            عائم *a = l.delta + i*l.outputs + j;
            عائم *b = net.workspace + j;
            عائم *c = l.وزن_updates + j*l.سائيز*l.سائيز*l.c*l.n;
            صحيح m = l.n;
            صحيح n = l.سائيز*l.سائيز*l.c;
            صحيح k = 1;

            gemm(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        إذا(net.delta){
            لكل(j = 0; j < locations; ++j){ 
                عائم *a = l.weights + j*l.سائيز*l.سائيز*l.c*l.n;
                عائم *b = l.delta + i*l.outputs + j;
                عائم *c = net.workspace + j;

                صحيح m = l.سائيز*l.سائيز*l.c;
                صحيح n = 1;
                صحيح k = l.n;

                gemm(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_cpu(net.workspace, l.c,  l.h,  l.w,  l.سائيز,  l.stride, l.pad, net.delta+i*l.c*l.h*l.w);
        }
    }
}

فراغ update_local_layer(local_layer l, update_دليلون a)
{
    عائم learning_شرح = a.learning_شرح*l.learning_شرح_scale;
    عائم momentum = a.momentum;
    عائم decay = a.decay;
    صحيح batch = a.batch;

    صحيح locations = l.ٻاهريون_w*l.ٻاهريون_h;
    صحيح سائيز = l.سائيز*l.سائيز*l.c*l.n*locations;
    axpy_cpu(l.outputs, learning_شرح/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    axpy_cpu(سائيز, -decay*batch, l.weights, 1, l.وزن_updates, 1);
    axpy_cpu(سائيز, learning_شرح/batch, l.وزن_updates, 1, l.weights, 1);
    scal_cpu(سائيز, momentum, l.وزن_updates, 1);
}

#جيڪڏهن_وصف_ٿيل GPU

فراغ forward_local_layer_gpu(ثابت local_layer l, ڄار net)
{
    صحيح ٻاهريون_h = local_ٻاهريون_اوچائي(l);
    صحيح ٻاهريون_w = local_ٻاهريون_ويڪر(l);
    صحيح i, j;
    صحيح locations = ٻاهريون_h * ٻاهريون_w;

    لكل(i = 0; i < l.batch; ++i){
        copy_gpu(l.outputs, l.biases_gpu, 1, l.output_gpu + i*l.outputs, 1);
    }

    لكل(i = 0; i < l.batch; ++i){
        عائم *داخلا = net.داخلا_gpu + i*l.w*l.h*l.c;
        im2col_gpu(داخلا, l.c, l.h, l.w, 
                l.سائيز, l.stride, l.pad, net.workspace);
        عائم *output = l.output_gpu + i*l.outputs;
        لكل(j = 0; j < locations; ++j){
            عائم *a = l.weights_gpu + j*l.سائيز*l.سائيز*l.c*l.n;
            عائم *b = net.workspace + j;
            عائم *c = output + j;

            صحيح m = l.n;
            صحيح n = 1;
            صحيح k = l.سائيز*l.سائيز*l.c;

            gemm_gpu(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

فراغ backward_local_layer_gpu(local_layer l, ڄار net)
{
    صحيح i, j;
    صحيح locations = l.ٻاهريون_w*l.ٻاهريون_h;

    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    لكل(i = 0; i < l.batch; ++i){
        axpy_gpu(l.outputs, 1, l.delta_gpu + i*l.outputs, 1, l.bias_updates_gpu, 1);
    }

    لكل(i = 0; i < l.batch; ++i){
        عائم *داخلا = net.داخلا_gpu + i*l.w*l.h*l.c;
        im2col_gpu(داخلا, l.c, l.h, l.w, 
                l.سائيز, l.stride, l.pad, net.workspace);

        لكل(j = 0; j < locations; ++j){ 
            عائم *a = l.delta_gpu + i*l.outputs + j;
            عائم *b = net.workspace + j;
            عائم *c = l.وزن_updates_gpu + j*l.سائيز*l.سائيز*l.c*l.n;
            صحيح m = l.n;
            صحيح n = l.سائيز*l.سائيز*l.c;
            صحيح k = 1;

            gemm_gpu(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        إذا(net.delta_gpu){
            لكل(j = 0; j < locations; ++j){ 
                عائم *a = l.weights_gpu + j*l.سائيز*l.سائيز*l.c*l.n;
                عائم *b = l.delta_gpu + i*l.outputs + j;
                عائم *c = net.workspace + j;

                صحيح m = l.سائيز*l.سائيز*l.c;
                صحيح n = 1;
                صحيح k = l.n;

                gemm_gpu(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_gpu(net.workspace, l.c,  l.h,  l.w,  l.سائيز,  l.stride, l.pad, net.delta_gpu+i*l.c*l.h*l.w);
        }
    }
}

فراغ update_local_layer_gpu(local_layer l, update_دليلون a)
{
    عائم learning_شرح = a.learning_شرح*l.learning_شرح_scale;
    عائم momentum = a.momentum;
    عائم decay = a.decay;
    صحيح batch = a.batch;

    صحيح locations = l.ٻاهريون_w*l.ٻاهريون_h;
    صحيح سائيز = l.سائيز*l.سائيز*l.c*l.n*locations;
    axpy_gpu(l.outputs, learning_شرح/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
    scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

    axpy_gpu(سائيز, -decay*batch, l.weights_gpu, 1, l.وزن_updates_gpu, 1);
    axpy_gpu(سائيز, learning_شرح/batch, l.وزن_updates_gpu, 1, l.weights_gpu, 1);
    scal_gpu(سائيز, momentum, l.وزن_updates_gpu, 1);
}

فراغ pull_local_layer(local_layer l)
{
    صحيح locations = l.ٻاهريون_w*l.ٻاهريون_h;
    صحيح سائيز = l.سائيز*l.سائيز*l.c*l.n*locations;
    cuda_pull_array(l.weights_gpu, l.weights, سائيز);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
}

فراغ ڌڪو_local_layer(local_layer l)
{
    صحيح locations = l.ٻاهريون_w*l.ٻاهريون_h;
    صحيح سائيز = l.سائيز*l.سائيز*l.c*l.n*locations;
    cuda_ڌڪو_array(l.weights_gpu, l.weights, سائيز);
    cuda_ڌڪو_array(l.biases_gpu, l.biases, l.outputs);
}
#جيڪڏهن_پورو

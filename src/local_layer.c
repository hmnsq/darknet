#شامل "local_layer.h"
#شامل "utils.h"
#شامل "im2col.h"
#شامل "col2im.h"
#شامل "blas.h"
#شامل "gemm.h"
#شامل <stdio.h>
#شامل <time.h>

صحیح local_برآمد_اونچائی(local_layer l)
{
    صحیح h = l.h;
    اگر (!l.pad) h -= l.سائز;
    ورنہ h -= 1;
    واپس h/l.stride + 1;
}

صحیح local_برآمد_چوڑائی(local_layer l)
{
    صحیح w = l.w;
    اگر (!l.pad) w -= l.سائز;
    ورنہ w -= 1;
    واپس w/l.stride + 1;
}

local_layer make_local_layer(صحیح batch, صحیح h, صحیح w, صحیح c, صحیح n, صحیح سائز, صحیح stride, صحیح pad, ACTIVATION activation)
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
    l.سائز = سائز;
    l.pad = pad;

    صحیح برآمد_h = local_برآمد_اونچائی(l);
    صحیح برآمد_w = local_برآمد_چوڑائی(l);
    صحیح locations = برآمد_h*برآمد_w;
    l.برآمد_h = برآمد_h;
    l.برآمد_w = برآمد_w;
    l.برآمد_c = n;
    l.outputs = l.برآمد_h * l.برآمد_w * l.برآمد_c;
    l.inputs = l.w * l.h * l.c;

    l.weights = صفر_حافظہ_دو(c*n*سائز*سائز*locations, حجم(اعشاری));
    l.وزن_updates = صفر_حافظہ_دو(c*n*سائز*سائز*locations, حجم(اعشاری));

    l.biases = صفر_حافظہ_دو(l.outputs, حجم(اعشاری));
    l.bias_updates = صفر_حافظہ_دو(l.outputs, حجم(اعشاری));

    // float scale = 1./sqrt(size*size*c);
    اعشاری scale = جذر(2./(سائز*سائز*c));
    برائے(i = 0; i < c*n*سائز*سائز; ++i) l.weights[i] = scale*rand_یکساں(-1,1);

    l.output = صفر_حافظہ_دو(l.batch*برآمد_h * برآمد_w * n, حجم(اعشاری));
    l.delta  = صفر_حافظہ_دو(l.batch*برآمد_h * برآمد_w * n, حجم(اعشاری));

    l.workspace_سائز = برآمد_h*برآمد_w*سائز*سائز*c;
    
    l.forward = forward_local_layer;
    l.backward = backward_local_layer;
    l.update = update_local_layer;

#اگر_تعریف_شدہ GPU
    l.forward_gpu = forward_local_layer_gpu;
    l.backward_gpu = backward_local_layer_gpu;
    l.update_gpu = update_local_layer_gpu;

    l.weights_gpu = cuda_make_array(l.weights, c*n*سائز*سائز*locations);
    l.وزن_updates_gpu = cuda_make_array(l.وزن_updates, c*n*سائز*سائز*locations);

    l.biases_gpu = cuda_make_array(l.biases, l.outputs);
    l.bias_updates_gpu = cuda_make_array(l.bias_updates, l.outputs);

    l.delta_gpu = cuda_make_array(l.delta, l.batch*برآمد_h*برآمد_w*n);
    l.output_gpu = cuda_make_array(l.output, l.batch*برآمد_h*برآمد_w*n);

#اگر_ختم
    l.activation = activation;

    fprintf(stderr, "Local Layer: %d x %d x %d image, %d filters -> %d x %d x %d image\n", h,w,c,n, برآمد_h, برآمد_w, n);

    واپس l;
}

خالی forward_local_layer(ثابت local_layer l, جال net)
{
    صحیح برآمد_h = local_برآمد_اونچائی(l);
    صحیح برآمد_w = local_برآمد_چوڑائی(l);
    صحیح i, j;
    صحیح locations = برآمد_h * برآمد_w;

    برائے(i = 0; i < l.batch; ++i){
        copy_cpu(l.outputs, l.biases, 1, l.output + i*l.outputs, 1);
    }

    برائے(i = 0; i < l.batch; ++i){
        اعشاری *داخلہ = net.داخلہ + i*l.w*l.h*l.c;
        im2col_cpu(داخلہ, l.c, l.h, l.w, 
                l.سائز, l.stride, l.pad, net.workspace);
        اعشاری *output = l.output + i*l.outputs;
        برائے(j = 0; j < locations; ++j){
            اعشاری *a = l.weights + j*l.سائز*l.سائز*l.c*l.n;
            اعشاری *b = net.workspace + j;
            اعشاری *c = output + j;

            صحیح m = l.n;
            صحیح n = 1;
            صحیح k = l.سائز*l.سائز*l.c;

            gemm(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

خالی backward_local_layer(local_layer l, جال net)
{
    صحیح i, j;
    صحیح locations = l.برآمد_w*l.برآمد_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    برائے(i = 0; i < l.batch; ++i){
        axpy_cpu(l.outputs, 1, l.delta + i*l.outputs, 1, l.bias_updates, 1);
    }

    برائے(i = 0; i < l.batch; ++i){
        اعشاری *داخلہ = net.داخلہ + i*l.w*l.h*l.c;
        im2col_cpu(داخلہ, l.c, l.h, l.w, 
                l.سائز, l.stride, l.pad, net.workspace);

        برائے(j = 0; j < locations; ++j){ 
            اعشاری *a = l.delta + i*l.outputs + j;
            اعشاری *b = net.workspace + j;
            اعشاری *c = l.وزن_updates + j*l.سائز*l.سائز*l.c*l.n;
            صحیح m = l.n;
            صحیح n = l.سائز*l.سائز*l.c;
            صحیح k = 1;

            gemm(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        اگر(net.delta){
            برائے(j = 0; j < locations; ++j){ 
                اعشاری *a = l.weights + j*l.سائز*l.سائز*l.c*l.n;
                اعشاری *b = l.delta + i*l.outputs + j;
                اعشاری *c = net.workspace + j;

                صحیح m = l.سائز*l.سائز*l.c;
                صحیح n = 1;
                صحیح k = l.n;

                gemm(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_cpu(net.workspace, l.c,  l.h,  l.w,  l.سائز,  l.stride, l.pad, net.delta+i*l.c*l.h*l.w);
        }
    }
}

خالی update_local_layer(local_layer l, update_دلائل a)
{
    اعشاری learning_شرح = a.learning_شرح*l.learning_شرح_scale;
    اعشاری momentum = a.momentum;
    اعشاری decay = a.decay;
    صحیح batch = a.batch;

    صحیح locations = l.برآمد_w*l.برآمد_h;
    صحیح سائز = l.سائز*l.سائز*l.c*l.n*locations;
    axpy_cpu(l.outputs, learning_شرح/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    axpy_cpu(سائز, -decay*batch, l.weights, 1, l.وزن_updates, 1);
    axpy_cpu(سائز, learning_شرح/batch, l.وزن_updates, 1, l.weights, 1);
    scal_cpu(سائز, momentum, l.وزن_updates, 1);
}

#اگر_تعریف_شدہ GPU

خالی forward_local_layer_gpu(ثابت local_layer l, جال net)
{
    صحیح برآمد_h = local_برآمد_اونچائی(l);
    صحیح برآمد_w = local_برآمد_چوڑائی(l);
    صحیح i, j;
    صحیح locations = برآمد_h * برآمد_w;

    برائے(i = 0; i < l.batch; ++i){
        copy_gpu(l.outputs, l.biases_gpu, 1, l.output_gpu + i*l.outputs, 1);
    }

    برائے(i = 0; i < l.batch; ++i){
        اعشاری *داخلہ = net.داخلہ_gpu + i*l.w*l.h*l.c;
        im2col_gpu(داخلہ, l.c, l.h, l.w, 
                l.سائز, l.stride, l.pad, net.workspace);
        اعشاری *output = l.output_gpu + i*l.outputs;
        برائے(j = 0; j < locations; ++j){
            اعشاری *a = l.weights_gpu + j*l.سائز*l.سائز*l.c*l.n;
            اعشاری *b = net.workspace + j;
            اعشاری *c = output + j;

            صحیح m = l.n;
            صحیح n = 1;
            صحیح k = l.سائز*l.سائز*l.c;

            gemm_gpu(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

خالی backward_local_layer_gpu(local_layer l, جال net)
{
    صحیح i, j;
    صحیح locations = l.برآمد_w*l.برآمد_h;

    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    برائے(i = 0; i < l.batch; ++i){
        axpy_gpu(l.outputs, 1, l.delta_gpu + i*l.outputs, 1, l.bias_updates_gpu, 1);
    }

    برائے(i = 0; i < l.batch; ++i){
        اعشاری *داخلہ = net.داخلہ_gpu + i*l.w*l.h*l.c;
        im2col_gpu(داخلہ, l.c, l.h, l.w, 
                l.سائز, l.stride, l.pad, net.workspace);

        برائے(j = 0; j < locations; ++j){ 
            اعشاری *a = l.delta_gpu + i*l.outputs + j;
            اعشاری *b = net.workspace + j;
            اعشاری *c = l.وزن_updates_gpu + j*l.سائز*l.سائز*l.c*l.n;
            صحیح m = l.n;
            صحیح n = l.سائز*l.سائز*l.c;
            صحیح k = 1;

            gemm_gpu(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        اگر(net.delta_gpu){
            برائے(j = 0; j < locations; ++j){ 
                اعشاری *a = l.weights_gpu + j*l.سائز*l.سائز*l.c*l.n;
                اعشاری *b = l.delta_gpu + i*l.outputs + j;
                اعشاری *c = net.workspace + j;

                صحیح m = l.سائز*l.سائز*l.c;
                صحیح n = 1;
                صحیح k = l.n;

                gemm_gpu(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_gpu(net.workspace, l.c,  l.h,  l.w,  l.سائز,  l.stride, l.pad, net.delta_gpu+i*l.c*l.h*l.w);
        }
    }
}

خالی update_local_layer_gpu(local_layer l, update_دلائل a)
{
    اعشاری learning_شرح = a.learning_شرح*l.learning_شرح_scale;
    اعشاری momentum = a.momentum;
    اعشاری decay = a.decay;
    صحیح batch = a.batch;

    صحیح locations = l.برآمد_w*l.برآمد_h;
    صحیح سائز = l.سائز*l.سائز*l.c*l.n*locations;
    axpy_gpu(l.outputs, learning_شرح/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
    scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

    axpy_gpu(سائز, -decay*batch, l.weights_gpu, 1, l.وزن_updates_gpu, 1);
    axpy_gpu(سائز, learning_شرح/batch, l.وزن_updates_gpu, 1, l.weights_gpu, 1);
    scal_gpu(سائز, momentum, l.وزن_updates_gpu, 1);
}

خالی pull_local_layer(local_layer l)
{
    صحیح locations = l.برآمد_w*l.برآمد_h;
    صحیح سائز = l.سائز*l.سائز*l.c*l.n*locations;
    cuda_pull_array(l.weights_gpu, l.weights, سائز);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
}

خالی ڈالو_local_layer(local_layer l)
{
    صحیح locations = l.برآمد_w*l.برآمد_h;
    صحیح سائز = l.سائز*l.سائز*l.c*l.n*locations;
    cuda_ڈالو_array(l.weights_gpu, l.weights, سائز);
    cuda_ڈالو_array(l.biases_gpu, l.biases, l.outputs);
}
#اگر_ختم

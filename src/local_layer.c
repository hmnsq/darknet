#అన్తర్భావ "local_layer.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "im2col.h"
#అన్తర్భావ "col2im.h"
#అన్తర్భావ "blas.h"
#అన్తర్భావ "gemm.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <time.h>

పూర్ణాంకం local_బహిర్గతం_ఎత్తు(local_layer l)
{
    పూర్ణాంకం h = l.h;
    ఒకవేళ (!l.pad) h -= l.కొలత;
    లేకపోతే h -= 1;
    తిరిగి h/l.stride + 1;
}

పూర్ణాంకం local_బహిర్గతం_వెడల్పు(local_layer l)
{
    పూర్ణాంకం w = l.w;
    ఒకవేళ (!l.pad) w -= l.కొలత;
    లేకపోతే w -= 1;
    తిరిగి w/l.stride + 1;
}

local_layer make_local_layer(పూర్ణాంకం batch, పూర్ణాంకం h, పూర్ణాంకం w, పూర్ణాంకం c, పూర్ణాంకం n, పూర్ణాంకం కొలత, పూర్ణాంకం stride, పూర్ణాంకం pad, ACTIVATION activation)
{
    పూర్ణాంకం i;
    local_layer l = {0};
    l.type = LOCAL;

    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.batch = batch;
    l.stride = stride;
    l.కొలత = కొలత;
    l.pad = pad;

    పూర్ణాంకం బహిర్గతం_h = local_బహిర్గతం_ఎత్తు(l);
    పూర్ణాంకం బహిర్గతం_w = local_బహిర్గతం_వెడల్పు(l);
    పూర్ణాంకం locations = బహిర్గతం_h*బహిర్గతం_w;
    l.బహిర్గతం_h = బహిర్గతం_h;
    l.బహిర్గతం_w = బహిర్గతం_w;
    l.బహిర్గతం_c = n;
    l.outputs = l.బహిర్గతం_h * l.బహిర్గతం_w * l.బహిర్గతం_c;
    l.inputs = l.w * l.h * l.c;

    l.weights = సున్నా_జ్ఞాపకం_ఇవ్వు(c*n*కొలత*కొలత*locations, పరిమాణం(దశాంశం));
    l.బరువు_updates = సున్నా_జ్ఞాపకం_ఇవ్వు(c*n*కొలత*కొలత*locations, పరిమాణం(దశాంశం));

    l.biases = సున్నా_జ్ఞాపకం_ఇవ్వు(l.outputs, పరిమాణం(దశాంశం));
    l.bias_updates = సున్నా_జ్ఞాపకం_ఇవ్వు(l.outputs, పరిమాణం(దశాంశం));

    // float scale = 1./sqrt(size*size*c);
    దశాంశం scale = వర్గమూలం(2./(కొలత*కొలత*c));
    కొరకు(i = 0; i < c*n*కొలత*కొలత; ++i) l.weights[i] = scale*rand_ఏకరీతి(-1,1);

    l.output = సున్నా_జ్ఞాపకం_ఇవ్వు(l.batch*బహిర్గతం_h * బహిర్గతం_w * n, పరిమాణం(దశాంశం));
    l.delta  = సున్నా_జ్ఞాపకం_ఇవ్వు(l.batch*బహిర్గతం_h * బహిర్గతం_w * n, పరిమాణం(దశాంశం));

    l.workspace_కొలత = బహిర్గతం_h*బహిర్గతం_w*కొలత*కొలత*c;
    
    l.forward = forward_local_layer;
    l.backward = backward_local_layer;
    l.update = update_local_layer;

#నిర్వచితమైతే GPU
    l.forward_gpu = forward_local_layer_gpu;
    l.backward_gpu = backward_local_layer_gpu;
    l.update_gpu = update_local_layer_gpu;

    l.weights_gpu = cuda_make_array(l.weights, c*n*కొలత*కొలత*locations);
    l.బరువు_updates_gpu = cuda_make_array(l.బరువు_updates, c*n*కొలత*కొలత*locations);

    l.biases_gpu = cuda_make_array(l.biases, l.outputs);
    l.bias_updates_gpu = cuda_make_array(l.bias_updates, l.outputs);

    l.delta_gpu = cuda_make_array(l.delta, l.batch*బహిర్గతం_h*బహిర్గతం_w*n);
    l.output_gpu = cuda_make_array(l.output, l.batch*బహిర్గతం_h*బహిర్గతం_w*n);

#అయితే_ముగింపు
    l.activation = activation;

    fprintf(stderr, "Local Layer: %d x %d x %d image, %d filters -> %d x %d x %d image\n", h,w,c,n, బహిర్గతం_h, బహిర్గతం_w, n);

    తిరిగి l;
}

శూన్య forward_local_layer(స్థిరరాశి local_layer l, జాలం net)
{
    పూర్ణాంకం బహిర్గతం_h = local_బహిర్గతం_ఎత్తు(l);
    పూర్ణాంకం బహిర్గతం_w = local_బహిర్గతం_వెడల్పు(l);
    పూర్ణాంకం i, j;
    పూర్ణాంకం locations = బహిర్గతం_h * బహిర్గతం_w;

    కొరకు(i = 0; i < l.batch; ++i){
        copy_cpu(l.outputs, l.biases, 1, l.output + i*l.outputs, 1);
    }

    కొరకు(i = 0; i < l.batch; ++i){
        దశాంశం *ఆదానం = net.ఆదానం + i*l.w*l.h*l.c;
        im2col_cpu(ఆదానం, l.c, l.h, l.w, 
                l.కొలత, l.stride, l.pad, net.workspace);
        దశాంశం *output = l.output + i*l.outputs;
        కొరకు(j = 0; j < locations; ++j){
            దశాంశం *a = l.weights + j*l.కొలత*l.కొలత*l.c*l.n;
            దశాంశం *b = net.workspace + j;
            దశాంశం *c = output + j;

            పూర్ణాంకం m = l.n;
            పూర్ణాంకం n = 1;
            పూర్ణాంకం k = l.కొలత*l.కొలత*l.c;

            gemm(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

శూన్య backward_local_layer(local_layer l, జాలం net)
{
    పూర్ణాంకం i, j;
    పూర్ణాంకం locations = l.బహిర్గతం_w*l.బహిర్గతం_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    కొరకు(i = 0; i < l.batch; ++i){
        axpy_cpu(l.outputs, 1, l.delta + i*l.outputs, 1, l.bias_updates, 1);
    }

    కొరకు(i = 0; i < l.batch; ++i){
        దశాంశం *ఆదానం = net.ఆదానం + i*l.w*l.h*l.c;
        im2col_cpu(ఆదానం, l.c, l.h, l.w, 
                l.కొలత, l.stride, l.pad, net.workspace);

        కొరకు(j = 0; j < locations; ++j){ 
            దశాంశం *a = l.delta + i*l.outputs + j;
            దశాంశం *b = net.workspace + j;
            దశాంశం *c = l.బరువు_updates + j*l.కొలత*l.కొలత*l.c*l.n;
            పూర్ణాంకం m = l.n;
            పూర్ణాంకం n = l.కొలత*l.కొలత*l.c;
            పూర్ణాంకం k = 1;

            gemm(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        ఒకవేళ(net.delta){
            కొరకు(j = 0; j < locations; ++j){ 
                దశాంశం *a = l.weights + j*l.కొలత*l.కొలత*l.c*l.n;
                దశాంశం *b = l.delta + i*l.outputs + j;
                దశాంశం *c = net.workspace + j;

                పూర్ణాంకం m = l.కొలత*l.కొలత*l.c;
                పూర్ణాంకం n = 1;
                పూర్ణాంకం k = l.n;

                gemm(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_cpu(net.workspace, l.c,  l.h,  l.w,  l.కొలత,  l.stride, l.pad, net.delta+i*l.c*l.h*l.w);
        }
    }
}

శూన్య update_local_layer(local_layer l, update_వాదనలు a)
{
    దశాంశం learning_రేటు = a.learning_రేటు*l.learning_రేటు_scale;
    దశాంశం momentum = a.momentum;
    దశాంశం decay = a.decay;
    పూర్ణాంకం batch = a.batch;

    పూర్ణాంకం locations = l.బహిర్గతం_w*l.బహిర్గతం_h;
    పూర్ణాంకం కొలత = l.కొలత*l.కొలత*l.c*l.n*locations;
    axpy_cpu(l.outputs, learning_రేటు/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    axpy_cpu(కొలత, -decay*batch, l.weights, 1, l.బరువు_updates, 1);
    axpy_cpu(కొలత, learning_రేటు/batch, l.బరువు_updates, 1, l.weights, 1);
    scal_cpu(కొలత, momentum, l.బరువు_updates, 1);
}

#నిర్వచితమైతే GPU

శూన్య forward_local_layer_gpu(స్థిరరాశి local_layer l, జాలం net)
{
    పూర్ణాంకం బహిర్గతం_h = local_బహిర్గతం_ఎత్తు(l);
    పూర్ణాంకం బహిర్గతం_w = local_బహిర్గతం_వెడల్పు(l);
    పూర్ణాంకం i, j;
    పూర్ణాంకం locations = బహిర్గతం_h * బహిర్గతం_w;

    కొరకు(i = 0; i < l.batch; ++i){
        copy_gpu(l.outputs, l.biases_gpu, 1, l.output_gpu + i*l.outputs, 1);
    }

    కొరకు(i = 0; i < l.batch; ++i){
        దశాంశం *ఆదానం = net.ఆదానం_gpu + i*l.w*l.h*l.c;
        im2col_gpu(ఆదానం, l.c, l.h, l.w, 
                l.కొలత, l.stride, l.pad, net.workspace);
        దశాంశం *output = l.output_gpu + i*l.outputs;
        కొరకు(j = 0; j < locations; ++j){
            దశాంశం *a = l.weights_gpu + j*l.కొలత*l.కొలత*l.c*l.n;
            దశాంశం *b = net.workspace + j;
            దశాంశం *c = output + j;

            పూర్ణాంకం m = l.n;
            పూర్ణాంకం n = 1;
            పూర్ణాంకం k = l.కొలత*l.కొలత*l.c;

            gemm_gpu(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

శూన్య backward_local_layer_gpu(local_layer l, జాలం net)
{
    పూర్ణాంకం i, j;
    పూర్ణాంకం locations = l.బహిర్గతం_w*l.బహిర్గతం_h;

    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    కొరకు(i = 0; i < l.batch; ++i){
        axpy_gpu(l.outputs, 1, l.delta_gpu + i*l.outputs, 1, l.bias_updates_gpu, 1);
    }

    కొరకు(i = 0; i < l.batch; ++i){
        దశాంశం *ఆదానం = net.ఆదానం_gpu + i*l.w*l.h*l.c;
        im2col_gpu(ఆదానం, l.c, l.h, l.w, 
                l.కొలత, l.stride, l.pad, net.workspace);

        కొరకు(j = 0; j < locations; ++j){ 
            దశాంశం *a = l.delta_gpu + i*l.outputs + j;
            దశాంశం *b = net.workspace + j;
            దశాంశం *c = l.బరువు_updates_gpu + j*l.కొలత*l.కొలత*l.c*l.n;
            పూర్ణాంకం m = l.n;
            పూర్ణాంకం n = l.కొలత*l.కొలత*l.c;
            పూర్ణాంకం k = 1;

            gemm_gpu(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        ఒకవేళ(net.delta_gpu){
            కొరకు(j = 0; j < locations; ++j){ 
                దశాంశం *a = l.weights_gpu + j*l.కొలత*l.కొలత*l.c*l.n;
                దశాంశం *b = l.delta_gpu + i*l.outputs + j;
                దశాంశం *c = net.workspace + j;

                పూర్ణాంకం m = l.కొలత*l.కొలత*l.c;
                పూర్ణాంకం n = 1;
                పూర్ణాంకం k = l.n;

                gemm_gpu(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_gpu(net.workspace, l.c,  l.h,  l.w,  l.కొలత,  l.stride, l.pad, net.delta_gpu+i*l.c*l.h*l.w);
        }
    }
}

శూన్య update_local_layer_gpu(local_layer l, update_వాదనలు a)
{
    దశాంశం learning_రేటు = a.learning_రేటు*l.learning_రేటు_scale;
    దశాంశం momentum = a.momentum;
    దశాంశం decay = a.decay;
    పూర్ణాంకం batch = a.batch;

    పూర్ణాంకం locations = l.బహిర్గతం_w*l.బహిర్గతం_h;
    పూర్ణాంకం కొలత = l.కొలత*l.కొలత*l.c*l.n*locations;
    axpy_gpu(l.outputs, learning_రేటు/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
    scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

    axpy_gpu(కొలత, -decay*batch, l.weights_gpu, 1, l.బరువు_updates_gpu, 1);
    axpy_gpu(కొలత, learning_రేటు/batch, l.బరువు_updates_gpu, 1, l.weights_gpu, 1);
    scal_gpu(కొలత, momentum, l.బరువు_updates_gpu, 1);
}

శూన్య pull_local_layer(local_layer l)
{
    పూర్ణాంకం locations = l.బహిర్గతం_w*l.బహిర్గతం_h;
    పూర్ణాంకం కొలత = l.కొలత*l.కొలత*l.c*l.n*locations;
    cuda_pull_array(l.weights_gpu, l.weights, కొలత);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
}

శూన్య నెట్టు_local_layer(local_layer l)
{
    పూర్ణాంకం locations = l.బహిర్గతం_w*l.బహిర్గతం_h;
    పూర్ణాంకం కొలత = l.కొలత*l.కొలత*l.c*l.n*locations;
    cuda_నెట్టు_array(l.weights_gpu, l.weights, కొలత);
    cuda_నెట్టు_array(l.biases_gpu, l.biases, l.outputs);
}
#అయితే_ముగింపు

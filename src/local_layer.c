#अन्तर्भाव "local_layer.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "im2col.h"
#अन्तर्भाव "col2im.h"
#अन्तर्भाव "blas.h"
#अन्तर्भाव "gemm.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <time.h>

पूर्णाङ्क local_बहिर्गमनम्_height(local_layer l)
{
    पूर्णाङ्क h = l.h;
    यदि (!l.pad) h -= l.आकार;
    अन्यथा h -= 1;
    प्रत्यागम h/l.stride + 1;
}

पूर्णाङ्क local_बहिर्गमनम्_विस्तारः(local_layer l)
{
    पूर्णाङ्क w = l.w;
    यदि (!l.pad) w -= l.आकार;
    अन्यथा w -= 1;
    प्रत्यागम w/l.stride + 1;
}

local_layer make_local_layer(पूर्णाङ्क batch, पूर्णाङ्क h, पूर्णाङ्क w, पूर्णाङ्क c, पूर्णाङ्क n, पूर्णाङ्क आकार, पूर्णाङ्क stride, पूर्णाङ्क pad, ACTIVATION activation)
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
    l.आकार = आकार;
    l.pad = pad;

    पूर्णाङ्क बहिर्गमनम्_h = local_बहिर्गमनम्_height(l);
    पूर्णाङ्क बहिर्गमनम्_w = local_बहिर्गमनम्_विस्तारः(l);
    पूर्णाङ्क locations = बहिर्गमनम्_h*बहिर्गमनम्_w;
    l.बहिर्गमनम्_h = बहिर्गमनम्_h;
    l.बहिर्गमनम्_w = बहिर्गमनम्_w;
    l.बहिर्गमनम्_c = n;
    l.outputs = l.बहिर्गमनम्_h * l.बहिर्गमनम्_w * l.बहिर्गमनम्_c;
    l.inputs = l.w * l.h * l.c;

    l.weights = शून्यस्मृतिं_देहि(c*n*आकार*आकार*locations, परिमाण(भिन्न));
    l.weight_updates = शून्यस्मृतिं_देहि(c*n*आकार*आकार*locations, परिमाण(भिन्न));

    l.biases = शून्यस्मृतिं_देहि(l.outputs, परिमाण(भिन्न));
    l.bias_updates = शून्यस्मृतिं_देहि(l.outputs, परिमाण(भिन्न));

    // float scale = 1./sqrt(size*size*c);
    भिन्न scale = वर्गमूलम्(2./(आकार*आकार*c));
    प्रति(i = 0; i < c*n*आकार*आकार; ++i) l.weights[i] = scale*rand_एकरूपम्(-1,1);

    l.output = शून्यस्मृतिं_देहि(l.batch*बहिर्गमनम्_h * बहिर्गमनम्_w * n, परिमाण(भिन्न));
    l.delta  = शून्यस्मृतिं_देहि(l.batch*बहिर्गमनम्_h * बहिर्गमनम्_w * n, परिमाण(भिन्न));

    l.workspace_आकार = बहिर्गमनम्_h*बहिर्गमनम्_w*आकार*आकार*c;
    
    l.forward = forward_local_layer;
    l.backward = backward_local_layer;
    l.update = update_local_layer;

#यदि_परिभाषितम् GPU
    l.forward_gpu = forward_local_layer_gpu;
    l.backward_gpu = backward_local_layer_gpu;
    l.update_gpu = update_local_layer_gpu;

    l.weights_gpu = cuda_make_array(l.weights, c*n*आकार*आकार*locations);
    l.weight_updates_gpu = cuda_make_array(l.weight_updates, c*n*आकार*आकार*locations);

    l.biases_gpu = cuda_make_array(l.biases, l.outputs);
    l.bias_updates_gpu = cuda_make_array(l.bias_updates, l.outputs);

    l.delta_gpu = cuda_make_array(l.delta, l.batch*बहिर्गमनम्_h*बहिर्गमनम्_w*n);
    l.output_gpu = cuda_make_array(l.output, l.batch*बहिर्गमनम्_h*बहिर्गमनम्_w*n);

#यदि_अन्तः
    l.activation = activation;

    fprintf(stderr, "Local Layer: %d x %d x %d image, %d filters -> %d x %d x %d image\n", h,w,c,n, बहिर्गमनम्_h, बहिर्गमनम्_w, n);

    प्रत्यागम l;
}

शून्य forward_local_layer(ध्रुव local_layer l, जालम् net)
{
    पूर्णाङ्क बहिर्गमनम्_h = local_बहिर्गमनम्_height(l);
    पूर्णाङ्क बहिर्गमनम्_w = local_बहिर्गमनम्_विस्तारः(l);
    पूर्णाङ्क i, j;
    पूर्णाङ्क locations = बहिर्गमनम्_h * बहिर्गमनम्_w;

    प्रति(i = 0; i < l.batch; ++i){
        copy_cpu(l.outputs, l.biases, 1, l.output + i*l.outputs, 1);
    }

    प्रति(i = 0; i < l.batch; ++i){
        भिन्न *निवेशः = net.निवेशः + i*l.w*l.h*l.c;
        im2col_cpu(निवेशः, l.c, l.h, l.w, 
                l.आकार, l.stride, l.pad, net.workspace);
        भिन्न *output = l.output + i*l.outputs;
        प्रति(j = 0; j < locations; ++j){
            भिन्न *a = l.weights + j*l.आकार*l.आकार*l.c*l.n;
            भिन्न *b = net.workspace + j;
            भिन्न *c = output + j;

            पूर्णाङ्क m = l.n;
            पूर्णाङ्क n = 1;
            पूर्णाङ्क k = l.आकार*l.आकार*l.c;

            gemm(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

शून्य backward_local_layer(local_layer l, जालम् net)
{
    पूर्णाङ्क i, j;
    पूर्णाङ्क locations = l.बहिर्गमनम्_w*l.बहिर्गमनम्_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    प्रति(i = 0; i < l.batch; ++i){
        axpy_cpu(l.outputs, 1, l.delta + i*l.outputs, 1, l.bias_updates, 1);
    }

    प्रति(i = 0; i < l.batch; ++i){
        भिन्न *निवेशः = net.निवेशः + i*l.w*l.h*l.c;
        im2col_cpu(निवेशः, l.c, l.h, l.w, 
                l.आकार, l.stride, l.pad, net.workspace);

        प्रति(j = 0; j < locations; ++j){ 
            भिन्न *a = l.delta + i*l.outputs + j;
            भिन्न *b = net.workspace + j;
            भिन्न *c = l.weight_updates + j*l.आकार*l.आकार*l.c*l.n;
            पूर्णाङ्क m = l.n;
            पूर्णाङ्क n = l.आकार*l.आकार*l.c;
            पूर्णाङ्क k = 1;

            gemm(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        यदि(net.delta){
            प्रति(j = 0; j < locations; ++j){ 
                भिन्न *a = l.weights + j*l.आकार*l.आकार*l.c*l.n;
                भिन्न *b = l.delta + i*l.outputs + j;
                भिन्न *c = net.workspace + j;

                पूर्णाङ्क m = l.आकार*l.आकार*l.c;
                पूर्णाङ्क n = 1;
                पूर्णाङ्क k = l.n;

                gemm(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_cpu(net.workspace, l.c,  l.h,  l.w,  l.आकार,  l.stride, l.pad, net.delta+i*l.c*l.h*l.w);
        }
    }
}

शून्य update_local_layer(local_layer l, update_तर्काः a)
{
    भिन्न learning_दरः = a.learning_दरः*l.learning_दरः_scale;
    भिन्न momentum = a.momentum;
    भिन्न decay = a.decay;
    पूर्णाङ्क batch = a.batch;

    पूर्णाङ्क locations = l.बहिर्गमनम्_w*l.बहिर्गमनम्_h;
    पूर्णाङ्क आकार = l.आकार*l.आकार*l.c*l.n*locations;
    axpy_cpu(l.outputs, learning_दरः/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.outputs, momentum, l.bias_updates, 1);

    axpy_cpu(आकार, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(आकार, learning_दरः/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(आकार, momentum, l.weight_updates, 1);
}

#यदि_परिभाषितम् GPU

शून्य forward_local_layer_gpu(ध्रुव local_layer l, जालम् net)
{
    पूर्णाङ्क बहिर्गमनम्_h = local_बहिर्गमनम्_height(l);
    पूर्णाङ्क बहिर्गमनम्_w = local_बहिर्गमनम्_विस्तारः(l);
    पूर्णाङ्क i, j;
    पूर्णाङ्क locations = बहिर्गमनम्_h * बहिर्गमनम्_w;

    प्रति(i = 0; i < l.batch; ++i){
        copy_gpu(l.outputs, l.biases_gpu, 1, l.output_gpu + i*l.outputs, 1);
    }

    प्रति(i = 0; i < l.batch; ++i){
        भिन्न *निवेशः = net.निवेशः_gpu + i*l.w*l.h*l.c;
        im2col_gpu(निवेशः, l.c, l.h, l.w, 
                l.आकार, l.stride, l.pad, net.workspace);
        भिन्न *output = l.output_gpu + i*l.outputs;
        प्रति(j = 0; j < locations; ++j){
            भिन्न *a = l.weights_gpu + j*l.आकार*l.आकार*l.c*l.n;
            भिन्न *b = net.workspace + j;
            भिन्न *c = output + j;

            पूर्णाङ्क m = l.n;
            पूर्णाङ्क n = 1;
            पूर्णाङ्क k = l.आकार*l.आकार*l.c;

            gemm_gpu(0,0,m,n,k,1,a,k,b,locations,1,c,locations);
        }
    }
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

शून्य backward_local_layer_gpu(local_layer l, जालम् net)
{
    पूर्णाङ्क i, j;
    पूर्णाङ्क locations = l.बहिर्गमनम्_w*l.बहिर्गमनम्_h;

    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    प्रति(i = 0; i < l.batch; ++i){
        axpy_gpu(l.outputs, 1, l.delta_gpu + i*l.outputs, 1, l.bias_updates_gpu, 1);
    }

    प्रति(i = 0; i < l.batch; ++i){
        भिन्न *निवेशः = net.निवेशः_gpu + i*l.w*l.h*l.c;
        im2col_gpu(निवेशः, l.c, l.h, l.w, 
                l.आकार, l.stride, l.pad, net.workspace);

        प्रति(j = 0; j < locations; ++j){ 
            भिन्न *a = l.delta_gpu + i*l.outputs + j;
            भिन्न *b = net.workspace + j;
            भिन्न *c = l.weight_updates_gpu + j*l.आकार*l.आकार*l.c*l.n;
            पूर्णाङ्क m = l.n;
            पूर्णाङ्क n = l.आकार*l.आकार*l.c;
            पूर्णाङ्क k = 1;

            gemm_gpu(0,1,m,n,k,1,a,locations,b,locations,1,c,n);
        }

        यदि(net.delta_gpu){
            प्रति(j = 0; j < locations; ++j){ 
                भिन्न *a = l.weights_gpu + j*l.आकार*l.आकार*l.c*l.n;
                भिन्न *b = l.delta_gpu + i*l.outputs + j;
                भिन्न *c = net.workspace + j;

                पूर्णाङ्क m = l.आकार*l.आकार*l.c;
                पूर्णाङ्क n = 1;
                पूर्णाङ्क k = l.n;

                gemm_gpu(1,0,m,n,k,1,a,m,b,locations,0,c,locations);
            }

            col2im_gpu(net.workspace, l.c,  l.h,  l.w,  l.आकार,  l.stride, l.pad, net.delta_gpu+i*l.c*l.h*l.w);
        }
    }
}

शून्य update_local_layer_gpu(local_layer l, update_तर्काः a)
{
    भिन्न learning_दरः = a.learning_दरः*l.learning_दरः_scale;
    भिन्न momentum = a.momentum;
    भिन्न decay = a.decay;
    पूर्णाङ्क batch = a.batch;

    पूर्णाङ्क locations = l.बहिर्गमनम्_w*l.बहिर्गमनम्_h;
    पूर्णाङ्क आकार = l.आकार*l.आकार*l.c*l.n*locations;
    axpy_gpu(l.outputs, learning_दरः/batch, l.bias_updates_gpu, 1, l.biases_gpu, 1);
    scal_gpu(l.outputs, momentum, l.bias_updates_gpu, 1);

    axpy_gpu(आकार, -decay*batch, l.weights_gpu, 1, l.weight_updates_gpu, 1);
    axpy_gpu(आकार, learning_दरः/batch, l.weight_updates_gpu, 1, l.weights_gpu, 1);
    scal_gpu(आकार, momentum, l.weight_updates_gpu, 1);
}

शून्य pull_local_layer(local_layer l)
{
    पूर्णाङ्क locations = l.बहिर्गमनम्_w*l.बहिर्गमनम्_h;
    पूर्णाङ्क आकार = l.आकार*l.आकार*l.c*l.n*locations;
    cuda_pull_array(l.weights_gpu, l.weights, आकार);
    cuda_pull_array(l.biases_gpu, l.biases, l.outputs);
}

शून्य प्रक्षिप_local_layer(local_layer l)
{
    पूर्णाङ्क locations = l.बहिर्गमनम्_w*l.बहिर्गमनम्_h;
    पूर्णाङ्क आकार = l.आकार*l.आकार*l.c*l.n*locations;
    cuda_प्रक्षिप_array(l.weights_gpu, l.weights, आकार);
    cuda_प्रक्षिप_array(l.biases_gpu, l.biases, l.outputs);
}
#यदि_अन्तः

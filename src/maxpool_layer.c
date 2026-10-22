#అన్తర్భావ "maxpool_layer.h"
#అన్తర్భావ "cuda.h"
#అన్తర్భావ <stdio.h>

image get_maxpool_image(maxpool_layer l)
{
    పూర్ణాంకం h = l.బహిర్గతం_h;
    పూర్ణాంకం w = l.బహిర్గతం_w;
    పూర్ణాంకం c = l.c;
    తిరిగి float_గమ్యం_image(w,h,c,l.output);
}

image get_maxpool_delta(maxpool_layer l)
{
    పూర్ణాంకం h = l.బహిర్గతం_h;
    పూర్ణాంకం w = l.బహిర్గతం_w;
    పూర్ణాంకం c = l.c;
    తిరిగి float_గమ్యం_image(w,h,c,l.delta);
}

maxpool_layer make_maxpool_layer(పూర్ణాంకం batch, పూర్ణాంకం h, పూర్ణాంకం w, పూర్ణాంకం c, పూర్ణాంకం కొలత, పూర్ణాంకం stride, పూర్ణాంకం padding)
{
    maxpool_layer l = {0};
    l.type = MAXPOOL;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.pad = padding;
    l.బహిర్గతం_w = (w + padding - కొలత)/stride + 1;
    l.బహిర్గతం_h = (h + padding - కొలత)/stride + 1;
    l.బహిర్గతం_c = c;
    l.outputs = l.బహిర్గతం_h * l.బహిర్గతం_w * l.బహిర్గతం_c;
    l.inputs = h*w*c;
    l.కొలత = కొలత;
    l.stride = stride;
    పూర్ణాంకం output_కొలత = l.బహిర్గతం_h * l.బహిర్గతం_w * l.బహిర్గతం_c * batch;
    l.indexes = సున్నా_జ్ఞాపకం_ఇవ్వు(output_కొలత, పరిమాణం(పూర్ణాంకం));
    l.output =  సున్నా_జ్ఞాపకం_ఇవ్వు(output_కొలత, పరిమాణం(దశాంశం));
    l.delta =   సున్నా_జ్ఞాపకం_ఇవ్వు(output_కొలత, పరిమాణం(దశాంశం));
    l.forward = forward_maxpool_layer;
    l.backward = backward_maxpool_layer;
    #నిర్వచితమైతే GPU
    l.forward_gpu = forward_maxpool_layer_gpu;
    l.backward_gpu = backward_maxpool_layer_gpu;
    l.indexes_gpu = cuda_make_int_array(0, output_కొలత);
    l.output_gpu  = cuda_make_array(l.output, output_కొలత);
    l.delta_gpu   = cuda_make_array(l.delta, output_కొలత);
    #అయితే_ముగింపు
    fprintf(stderr, "max          %d x %d / %d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", కొలత, కొలత, stride, w, h, c, l.బహిర్గతం_w, l.బహిర్గతం_h, l.బహిర్గతం_c);
    తిరిగి l;
}

శూన్య resize_maxpool_layer(maxpool_layer *l, పూర్ణాంకం w, పూర్ణాంకం h)
{
    l->h = h;
    l->w = w;
    l->inputs = h*w*l->c;

    l->బహిర్గతం_w = (w + l->pad - l->కొలత)/l->stride + 1;
    l->బహిర్గతం_h = (h + l->pad - l->కొలత)/l->stride + 1;
    l->outputs = l->బహిర్గతం_w * l->బహిర్గతం_h * l->c;
    పూర్ణాంకం output_కొలత = l->outputs * l->batch;

    l->indexes = realloc(l->indexes, output_కొలత * పరిమాణం(పూర్ణాంకం));
    l->output = realloc(l->output, output_కొలత * పరిమాణం(దశాంశం));
    l->delta = realloc(l->delta, output_కొలత * పరిమాణం(దశాంశం));

    #నిర్వచితమైతే GPU
    cuda_free((దశాంశం *)l->indexes_gpu);
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->indexes_gpu = cuda_make_int_array(0, output_కొలత);
    l->output_gpu  = cuda_make_array(l->output, output_కొలత);
    l->delta_gpu   = cuda_make_array(l->delta,  output_కొలత);
    #అయితే_ముగింపు
}

శూన్య forward_maxpool_layer(స్థిరరాశి maxpool_layer l, జాలం net)
{
    పూర్ణాంకం b,i,j,k,m,n;
    పూర్ణాంకం w_స్థానభ్రంశం = -l.pad/2;
    పూర్ణాంకం h_స్థానభ్రంశం = -l.pad/2;

    పూర్ణాంకం h = l.బహిర్గతం_h;
    పూర్ణాంకం w = l.బహిర్గతం_w;
    పూర్ణాంకం c = l.c;

    కొరకు(b = 0; b < l.batch; ++b){
        కొరకు(k = 0; k < c; ++k){
            కొరకు(i = 0; i < h; ++i){
                కొరకు(j = 0; j < w; ++j){
                    పూర్ణాంకం బహిర్గతం_సూచీ = j + w*(i + h*(k + c*b));
                    దశాంశం max = -FLT_MAX;
                    పూర్ణాంకం max_i = -1;
                    కొరకు(n = 0; n < l.కొలత; ++n){
                        కొరకు(m = 0; m < l.కొలత; ++m){
                            పూర్ణాంకం cur_h = h_స్థానభ్రంశం + i*l.stride + n;
                            పూర్ణాంకం cur_w = w_స్థానభ్రంశం + j*l.stride + m;
                            పూర్ణాంకం సూచీ = cur_w + l.w*(cur_h + l.h*(k + b*l.c));
                            పూర్ణాంకం valid = (cur_h >= 0 && cur_h < l.h &&
                                         cur_w >= 0 && cur_w < l.w);
                            దశాంశం val = (valid != 0) ? net.ఆదానం[సూచీ] : -FLT_MAX;
                            max_i = (val > max) ? సూచీ : max_i;
                            max   = (val > max) ? val   : max;
                        }
                    }
                    l.output[బహిర్గతం_సూచీ] = max;
                    l.indexes[బహిర్గతం_సూచీ] = max_i;
                }
            }
        }
    }
}

శూన్య backward_maxpool_layer(స్థిరరాశి maxpool_layer l, జాలం net)
{
    పూర్ణాంకం i;
    పూర్ణాంకం h = l.బహిర్గతం_h;
    పూర్ణాంకం w = l.బహిర్గతం_w;
    పూర్ణాంకం c = l.c;
    కొరకు(i = 0; i < h*w*c*l.batch; ++i){
        పూర్ణాంకం సూచీ = l.indexes[i];
        net.delta[సూచీ] += l.delta[i];
    }
}


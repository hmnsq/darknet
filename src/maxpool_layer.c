#ضمن "maxpool_layer.h"
#ضمن "cuda.h"
#ضمن <stdio.h>

image get_maxpool_image(maxpool_layer l)
{
    صحيح h = l.بیرون_h;
    صحيح w = l.بیرون_w;
    صحيح c = l.c;
    أرجع float_مقصد_image(w,h,c,l.output);
}

image get_maxpool_delta(maxpool_layer l)
{
    صحيح h = l.بیرون_h;
    صحيح w = l.بیرون_w;
    صحيح c = l.c;
    أرجع float_مقصد_image(w,h,c,l.delta);
}

maxpool_layer make_maxpool_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح اندازه, صحيح stride, صحيح padding)
{
    maxpool_layer l = {0};
    l.type = MAXPOOL;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.pad = padding;
    l.بیرون_w = (w + padding - اندازه)/stride + 1;
    l.بیرون_h = (h + padding - اندازه)/stride + 1;
    l.بیرون_c = c;
    l.outputs = l.بیرون_h * l.بیرون_w * l.بیرون_c;
    l.inputs = h*w*c;
    l.اندازه = اندازه;
    l.stride = stride;
    صحيح output_اندازه = l.بیرون_h * l.بیرون_w * l.بیرون_c * batch;
    l.indexes = حافظه_صفر_بده(output_اندازه, حجم(صحيح));
    l.output =  حافظه_صفر_بده(output_اندازه, حجم(عائم));
    l.delta =   حافظه_صفر_بده(output_اندازه, حجم(عائم));
    l.forward = forward_maxpool_layer;
    l.backward = backward_maxpool_layer;
    #اگر_تعریف_شده GPU
    l.forward_gpu = forward_maxpool_layer_gpu;
    l.backward_gpu = backward_maxpool_layer_gpu;
    l.indexes_gpu = cuda_make_int_array(0, output_اندازه);
    l.output_gpu  = cuda_make_array(l.output, output_اندازه);
    l.delta_gpu   = cuda_make_array(l.delta, output_اندازه);
    #پایان_اگر
    fprintf(stderr, "max          %d x %d / %d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", اندازه, اندازه, stride, w, h, c, l.بیرون_w, l.بیرون_h, l.بیرون_c);
    أرجع l;
}

فراغ resize_maxpool_layer(maxpool_layer *l, صحيح w, صحيح h)
{
    l->h = h;
    l->w = w;
    l->inputs = h*w*l->c;

    l->بیرون_w = (w + l->pad - l->اندازه)/l->stride + 1;
    l->بیرون_h = (h + l->pad - l->اندازه)/l->stride + 1;
    l->outputs = l->بیرون_w * l->بیرون_h * l->c;
    صحيح output_اندازه = l->outputs * l->batch;

    l->indexes = realloc(l->indexes, output_اندازه * حجم(صحيح));
    l->output = realloc(l->output, output_اندازه * حجم(عائم));
    l->delta = realloc(l->delta, output_اندازه * حجم(عائم));

    #اگر_تعریف_شده GPU
    cuda_free((عائم *)l->indexes_gpu);
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->indexes_gpu = cuda_make_int_array(0, output_اندازه);
    l->output_gpu  = cuda_make_array(l->output, output_اندازه);
    l->delta_gpu   = cuda_make_array(l->delta,  output_اندازه);
    #پایان_اگر
}

فراغ forward_maxpool_layer(ثابت maxpool_layer l, شبکه net)
{
    صحيح b,i,j,k,m,n;
    صحيح w_جابه_جایی = -l.pad/2;
    صحيح h_جابه_جایی = -l.pad/2;

    صحيح h = l.بیرون_h;
    صحيح w = l.بیرون_w;
    صحيح c = l.c;

    لكل(b = 0; b < l.batch; ++b){
        لكل(k = 0; k < c; ++k){
            لكل(i = 0; i < h; ++i){
                لكل(j = 0; j < w; ++j){
                    صحيح بیرون_شاخص = j + w*(i + h*(k + c*b));
                    عائم max = -FLT_MAX;
                    صحيح max_i = -1;
                    لكل(n = 0; n < l.اندازه; ++n){
                        لكل(m = 0; m < l.اندازه; ++m){
                            صحيح cur_h = h_جابه_جایی + i*l.stride + n;
                            صحيح cur_w = w_جابه_جایی + j*l.stride + m;
                            صحيح شاخص = cur_w + l.w*(cur_h + l.h*(k + b*l.c));
                            صحيح valid = (cur_h >= 0 && cur_h < l.h &&
                                         cur_w >= 0 && cur_w < l.w);
                            عائم val = (valid != 0) ? net.درون_داد[شاخص] : -FLT_MAX;
                            max_i = (val > max) ? شاخص : max_i;
                            max   = (val > max) ? val   : max;
                        }
                    }
                    l.output[بیرون_شاخص] = max;
                    l.indexes[بیرون_شاخص] = max_i;
                }
            }
        }
    }
}

فراغ backward_maxpool_layer(ثابت maxpool_layer l, شبکه net)
{
    صحيح i;
    صحيح h = l.بیرون_h;
    صحيح w = l.بیرون_w;
    صحيح c = l.c;
    لكل(i = 0; i < h*w*c*l.batch; ++i){
        صحيح شاخص = l.indexes[i];
        net.delta[شاخص] += l.delta[i];
    }
}


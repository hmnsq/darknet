#شامل "maxpool_layer.h"
#شامل "cuda.h"
#شامل <stdio.h>

image get_maxpool_image(maxpool_layer l)
{
    صحیح h = l.برآمد_h;
    صحیح w = l.برآمد_w;
    صحیح c = l.c;
    واپس float_مقصد_image(w,h,c,l.output);
}

image get_maxpool_delta(maxpool_layer l)
{
    صحیح h = l.برآمد_h;
    صحیح w = l.برآمد_w;
    صحیح c = l.c;
    واپس float_مقصد_image(w,h,c,l.delta);
}

maxpool_layer make_maxpool_layer(صحیح batch, صحیح h, صحیح w, صحیح c, صحیح سائز, صحیح stride, صحیح padding)
{
    maxpool_layer l = {0};
    l.type = MAXPOOL;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.pad = padding;
    l.برآمد_w = (w + padding - سائز)/stride + 1;
    l.برآمد_h = (h + padding - سائز)/stride + 1;
    l.برآمد_c = c;
    l.outputs = l.برآمد_h * l.برآمد_w * l.برآمد_c;
    l.inputs = h*w*c;
    l.سائز = سائز;
    l.stride = stride;
    صحیح output_سائز = l.برآمد_h * l.برآمد_w * l.برآمد_c * batch;
    l.indexes = صفر_حافظہ_دو(output_سائز, حجم(صحیح));
    l.output =  صفر_حافظہ_دو(output_سائز, حجم(اعشاری));
    l.delta =   صفر_حافظہ_دو(output_سائز, حجم(اعشاری));
    l.forward = forward_maxpool_layer;
    l.backward = backward_maxpool_layer;
    #اگر_تعریف_شدہ GPU
    l.forward_gpu = forward_maxpool_layer_gpu;
    l.backward_gpu = backward_maxpool_layer_gpu;
    l.indexes_gpu = cuda_make_int_array(0, output_سائز);
    l.output_gpu  = cuda_make_array(l.output, output_سائز);
    l.delta_gpu   = cuda_make_array(l.delta, output_سائز);
    #اگر_ختم
    fprintf(stderr, "max          %d x %d / %d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", سائز, سائز, stride, w, h, c, l.برآمد_w, l.برآمد_h, l.برآمد_c);
    واپس l;
}

خالی resize_maxpool_layer(maxpool_layer *l, صحیح w, صحیح h)
{
    l->h = h;
    l->w = w;
    l->inputs = h*w*l->c;

    l->برآمد_w = (w + l->pad - l->سائز)/l->stride + 1;
    l->برآمد_h = (h + l->pad - l->سائز)/l->stride + 1;
    l->outputs = l->برآمد_w * l->برآمد_h * l->c;
    صحیح output_سائز = l->outputs * l->batch;

    l->indexes = realloc(l->indexes, output_سائز * حجم(صحیح));
    l->output = realloc(l->output, output_سائز * حجم(اعشاری));
    l->delta = realloc(l->delta, output_سائز * حجم(اعشاری));

    #اگر_تعریف_شدہ GPU
    cuda_free((اعشاری *)l->indexes_gpu);
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->indexes_gpu = cuda_make_int_array(0, output_سائز);
    l->output_gpu  = cuda_make_array(l->output, output_سائز);
    l->delta_gpu   = cuda_make_array(l->delta,  output_سائز);
    #اگر_ختم
}

خالی forward_maxpool_layer(ثابت maxpool_layer l, جال net)
{
    صحیح b,i,j,k,m,n;
    صحیح w_انحراف = -l.pad/2;
    صحیح h_انحراف = -l.pad/2;

    صحیح h = l.برآمد_h;
    صحیح w = l.برآمد_w;
    صحیح c = l.c;

    برائے(b = 0; b < l.batch; ++b){
        برائے(k = 0; k < c; ++k){
            برائے(i = 0; i < h; ++i){
                برائے(j = 0; j < w; ++j){
                    صحیح برآمد_اشاریہ = j + w*(i + h*(k + c*b));
                    اعشاری max = -FLT_MAX;
                    صحیح max_i = -1;
                    برائے(n = 0; n < l.سائز; ++n){
                        برائے(m = 0; m < l.سائز; ++m){
                            صحیح cur_h = h_انحراف + i*l.stride + n;
                            صحیح cur_w = w_انحراف + j*l.stride + m;
                            صحیح اشاریہ = cur_w + l.w*(cur_h + l.h*(k + b*l.c));
                            صحیح valid = (cur_h >= 0 && cur_h < l.h &&
                                         cur_w >= 0 && cur_w < l.w);
                            اعشاری val = (valid != 0) ? net.داخلہ[اشاریہ] : -FLT_MAX;
                            max_i = (val > max) ? اشاریہ : max_i;
                            max   = (val > max) ? val   : max;
                        }
                    }
                    l.output[برآمد_اشاریہ] = max;
                    l.indexes[برآمد_اشاریہ] = max_i;
                }
            }
        }
    }
}

خالی backward_maxpool_layer(ثابت maxpool_layer l, جال net)
{
    صحیح i;
    صحیح h = l.برآمد_h;
    صحیح w = l.برآمد_w;
    صحیح c = l.c;
    برائے(i = 0; i < h*w*c*l.batch; ++i){
        صحیح اشاریہ = l.indexes[i];
        net.delta[اشاریہ] += l.delta[i];
    }
}


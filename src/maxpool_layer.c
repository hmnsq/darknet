#ਅਨ੍ਤਰ੍ਭਾਵ "maxpool_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>

image get_maxpool_image(maxpool_layer l)
{
    ਪੂਰਨ h = l.ਬਾਹਰਲਾ_h;
    ਪੂਰਨ w = l.ਬਾਹਰਲਾ_w;
    ਪੂਰਨ c = l.c;
    ਵਾਪਸ float_ਮੰਜ਼ਿਲ_image(w,h,c,l.output);
}

image get_maxpool_delta(maxpool_layer l)
{
    ਪੂਰਨ h = l.ਬਾਹਰਲਾ_h;
    ਪੂਰਨ w = l.ਬਾਹਰਲਾ_w;
    ਪੂਰਨ c = l.c;
    ਵਾਪਸ float_ਮੰਜ਼ਿਲ_image(w,h,c,l.delta);
}

maxpool_layer make_maxpool_layer(ਪੂਰਨ batch, ਪੂਰਨ h, ਪੂਰਨ w, ਪੂਰਨ c, ਪੂਰਨ ਮਾਪ, ਪੂਰਨ stride, ਪੂਰਨ padding)
{
    maxpool_layer l = {0};
    l.type = MAXPOOL;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.pad = padding;
    l.ਬਾਹਰਲਾ_w = (w + padding - ਮਾਪ)/stride + 1;
    l.ਬਾਹਰਲਾ_h = (h + padding - ਮਾਪ)/stride + 1;
    l.ਬਾਹਰਲਾ_c = c;
    l.outputs = l.ਬਾਹਰਲਾ_h * l.ਬਾਹਰਲਾ_w * l.ਬਾਹਰਲਾ_c;
    l.inputs = h*w*c;
    l.ਮਾਪ = ਮਾਪ;
    l.stride = stride;
    ਪੂਰਨ output_ਮਾਪ = l.ਬਾਹਰਲਾ_h * l.ਬਾਹਰਲਾ_w * l.ਬਾਹਰਲਾ_c * batch;
    l.indexes = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(output_ਮਾਪ, ਆਕਾਰ(ਪੂਰਨ));
    l.output =  ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(output_ਮਾਪ, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.delta =   ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(output_ਮਾਪ, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.forward = forward_maxpool_layer;
    l.backward = backward_maxpool_layer;
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_maxpool_layer_gpu;
    l.backward_gpu = backward_maxpool_layer_gpu;
    l.indexes_gpu = cuda_make_int_array(0, output_ਮਾਪ);
    l.output_gpu  = cuda_make_array(l.output, output_ਮਾਪ);
    l.delta_gpu   = cuda_make_array(l.delta, output_ਮਾਪ);
    #ਜੇ_ਅੰਤ
    fprintf(stderr, "max          %d x %d / %d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", ਮਾਪ, ਮਾਪ, stride, w, h, c, l.ਬਾਹਰਲਾ_w, l.ਬਾਹਰਲਾ_h, l.ਬਾਹਰਲਾ_c);
    ਵਾਪਸ l;
}

ਖਾਲੀ resize_maxpool_layer(maxpool_layer *l, ਪੂਰਨ w, ਪੂਰਨ h)
{
    l->h = h;
    l->w = w;
    l->inputs = h*w*l->c;

    l->ਬਾਹਰਲਾ_w = (w + l->pad - l->ਮਾਪ)/l->stride + 1;
    l->ਬਾਹਰਲਾ_h = (h + l->pad - l->ਮਾਪ)/l->stride + 1;
    l->outputs = l->ਬਾਹਰਲਾ_w * l->ਬਾਹਰਲਾ_h * l->c;
    ਪੂਰਨ output_ਮਾਪ = l->outputs * l->batch;

    l->indexes = realloc(l->indexes, output_ਮਾਪ * ਆਕਾਰ(ਪੂਰਨ));
    l->output = realloc(l->output, output_ਮਾਪ * ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l->delta = realloc(l->delta, output_ਮਾਪ * ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    cuda_free((ਦਸ਼ਮਲਵ *)l->indexes_gpu);
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->indexes_gpu = cuda_make_int_array(0, output_ਮਾਪ);
    l->output_gpu  = cuda_make_array(l->output, output_ਮਾਪ);
    l->delta_gpu   = cuda_make_array(l->delta,  output_ਮਾਪ);
    #ਜੇ_ਅੰਤ
}

ਖਾਲੀ forward_maxpool_layer(ਅਚਲ maxpool_layer l, ਜਾਲ net)
{
    ਪੂਰਨ b,i,j,k,m,n;
    ਪੂਰਨ w_ਵਿੱਥ = -l.pad/2;
    ਪੂਰਨ h_ਵਿੱਥ = -l.pad/2;

    ਪੂਰਨ h = l.ਬਾਹਰਲਾ_h;
    ਪੂਰਨ w = l.ਬਾਹਰਲਾ_w;
    ਪੂਰਨ c = l.c;

    ਲਈ(b = 0; b < l.batch; ++b){
        ਲਈ(k = 0; k < c; ++k){
            ਲਈ(i = 0; i < h; ++i){
                ਲਈ(j = 0; j < w; ++j){
                    ਪੂਰਨ ਬਾਹਰਲਾ_ਸੂਚਕਾਂਕ = j + w*(i + h*(k + c*b));
                    ਦਸ਼ਮਲਵ max = -FLT_MAX;
                    ਪੂਰਨ max_i = -1;
                    ਲਈ(n = 0; n < l.ਮਾਪ; ++n){
                        ਲਈ(m = 0; m < l.ਮਾਪ; ++m){
                            ਪੂਰਨ cur_h = h_ਵਿੱਥ + i*l.stride + n;
                            ਪੂਰਨ cur_w = w_ਵਿੱਥ + j*l.stride + m;
                            ਪੂਰਨ ਸੂਚਕਾਂਕ = cur_w + l.w*(cur_h + l.h*(k + b*l.c));
                            ਪੂਰਨ valid = (cur_h >= 0 && cur_h < l.h &&
                                         cur_w >= 0 && cur_w < l.w);
                            ਦਸ਼ਮਲਵ val = (valid != 0) ? net.ਨਿਵੇਸ਼[ਸੂਚਕਾਂਕ] : -FLT_MAX;
                            max_i = (val > max) ? ਸੂਚਕਾਂਕ : max_i;
                            max   = (val > max) ? val   : max;
                        }
                    }
                    l.output[ਬਾਹਰਲਾ_ਸੂਚਕਾਂਕ] = max;
                    l.indexes[ਬਾਹਰਲਾ_ਸੂਚਕਾਂਕ] = max_i;
                }
            }
        }
    }
}

ਖਾਲੀ backward_maxpool_layer(ਅਚਲ maxpool_layer l, ਜਾਲ net)
{
    ਪੂਰਨ i;
    ਪੂਰਨ h = l.ਬਾਹਰਲਾ_h;
    ਪੂਰਨ w = l.ਬਾਹਰਲਾ_w;
    ਪੂਰਨ c = l.c;
    ਲਈ(i = 0; i < h*w*c*l.batch; ++i){
        ਪੂਰਨ ਸੂਚਕਾਂਕ = l.indexes[i];
        net.delta[ਸੂਚਕਾਂਕ] += l.delta[i];
    }
}


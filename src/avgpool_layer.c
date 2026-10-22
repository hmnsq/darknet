#ਅਨ੍ਤਰ੍ਭਾਵ "avgpool_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>

avgpool_layer make_avgpool_layer(ਪੂਰਨ batch, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ c)
{
    fprintf(stderr, "avg                     %4d x%4d x%4d   ->  %4d\n",  w, h, c, c);
    avgpool_layer l = {0};
    l.type = AVGPOOL;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.ਬਾਹਰਲਾ_w = 1;
    l.ਬਾਹਰਲਾ_h = 1;
    l.ਬਾਹਰਲਾ_c = c;
    l.outputs = l.ਬਾਹਰਲਾ_c;
    l.inputs = h*w*c;
    ਪੂਰਨ output_ਮਾਪ = l.outputs * batch;
    l.output =  ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(output_ਮਾਪ, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.delta =   ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(output_ਮਾਪ, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.forward = forward_avgpool_layer;
    l.backward = backward_avgpool_layer;
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_avgpool_layer_gpu;
    l.backward_gpu = backward_avgpool_layer_gpu;
    l.output_gpu  = cuda_make_array(l.output, output_ਮਾਪ);
    l.delta_gpu   = cuda_make_array(l.delta, output_ਮਾਪ);
    #ਜੇ_ਅੰਤ
    ਵਾਪਸ l;
}

ਖਾਲੀ resize_avgpool_layer(avgpool_layer *l, ਪੂਰਨ w, ਪੂਰਨ h)
{
    l->w = w;
    l->h = h;
    l->inputs = h*w*l->c;
}

ਖਾਲੀ forward_avgpool_layer(ਅਚਲ avgpool_layer l, ਜਾਲ net)
{
    ਪੂਰਨ b,i,k;

    ਲਈ(b = 0; b < l.batch; ++b){
        ਲਈ(k = 0; k < l.c; ++k){
            ਪੂਰਨ ਬਾਹਰਲਾ_ਸੂਚਕਾਂਕ = k + b*l.c;
            l.output[ਬਾਹਰਲਾ_ਸੂਚਕਾਂਕ] = 0;
            ਲਈ(i = 0; i < l.h*l.w; ++i){
                ਪੂਰਨ ਵਿੱਚ_ਸੂਚਕਾਂਕ = i + l.h*l.w*(k + b*l.c);
                l.output[ਬਾਹਰਲਾ_ਸੂਚਕਾਂਕ] += net.ਨਿਵੇਸ਼[ਵਿੱਚ_ਸੂਚਕਾਂਕ];
            }
            l.output[ਬਾਹਰਲਾ_ਸੂਚਕਾਂਕ] /= l.h*l.w;
        }
    }
}

ਖਾਲੀ backward_avgpool_layer(ਅਚਲ avgpool_layer l, ਜਾਲ net)
{
    ਪੂਰਨ b,i,k;

    ਲਈ(b = 0; b < l.batch; ++b){
        ਲਈ(k = 0; k < l.c; ++k){
            ਪੂਰਨ ਬਾਹਰਲਾ_ਸੂਚਕਾਂਕ = k + b*l.c;
            ਲਈ(i = 0; i < l.h*l.w; ++i){
                ਪੂਰਨ ਵਿੱਚ_ਸੂਚਕਾਂਕ = i + l.h*l.w*(k + b*l.c);
                net.delta[ਵਿੱਚ_ਸੂਚਕਾਂਕ] += l.delta[ਬਾਹਰਲਾ_ਸੂਚਕਾਂਕ] / (l.h*l.w);
            }
        }
    }
}


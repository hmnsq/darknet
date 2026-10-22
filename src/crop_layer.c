#ਅਨ੍ਤਰ੍ਭਾਵ "crop_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>

image get_crop_image(crop_layer l)
{
    ਪੂਰਨ h = l.ਬਾਹਰਲਾ_h;
    ਪੂਰਨ w = l.ਬਾਹਰਲਾ_w;
    ਪੂਰਨ c = l.ਬਾਹਰਲਾ_c;
    ਵਾਪਸ float_ਮੰਜ਼ਿਲ_image(w,h,c,l.output);
}

ਖਾਲੀ backward_crop_layer(ਅਚਲ crop_layer l, ਜਾਲ net){}
ਖਾਲੀ backward_crop_layer_gpu(ਅਚਲ crop_layer l, ਜਾਲ net){}

crop_layer make_crop_layer(ਪੂਰਨ batch, ਪੂਰਨ h, ਪੂਰਨ w, ਪੂਰਨ c, ਪੂਰਨ crop_ਉਚਾਈ, ਪੂਰਨ crop_ਚੌੜਾਈ, ਪੂਰਨ flip, ਦਸ਼ਮਲਵ angle, ਦਸ਼ਮਲਵ saturation, ਦਸ਼ਮਲਵ exposure)
{
    fprintf(stderr, "Crop Layer: %d x %d -> %d x %d x %d image\n", h,w,crop_ਉਚਾਈ,crop_ਚੌੜਾਈ,c);
    crop_layer l = {0};
    l.type = CROP;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.scale = (ਦਸ਼ਮਲਵ)crop_ਉਚਾਈ / h;
    l.flip = flip;
    l.angle = angle;
    l.saturation = saturation;
    l.exposure = exposure;
    l.ਬਾਹਰਲਾ_w = crop_ਚੌੜਾਈ;
    l.ਬਾਹਰਲਾ_h = crop_ਉਚਾਈ;
    l.ਬਾਹਰਲਾ_c = c;
    l.inputs = l.w * l.h * l.c;
    l.outputs = l.ਬਾਹਰਲਾ_w * l.ਬਾਹਰਲਾ_h * l.ਬਾਹਰਲਾ_c;
    l.output = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.outputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.forward = forward_crop_layer;
    l.backward = backward_crop_layer;

    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_crop_layer_gpu;
    l.backward_gpu = backward_crop_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    l.rand_gpu   = cuda_make_array(0, l.batch*8);
    #ਜੇ_ਅੰਤ
    ਵਾਪਸ l;
}

ਖਾਲੀ resize_crop_layer(layer *l, ਪੂਰਨ w, ਪੂਰਨ h)
{
    l->w = w;
    l->h = h;

    l->ਬਾਹਰਲਾ_w =  l->scale*w;
    l->ਬਾਹਰਲਾ_h =  l->scale*h;

    l->inputs = l->w * l->h * l->c;
    l->outputs = l->ਬਾਹਰਲਾ_h * l->ਬਾਹਰਲਾ_w * l->ਬਾਹਰਲਾ_c;

    l->output = realloc(l->output, l->batch*l->outputs*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    cuda_free(l->output_gpu);
    l->output_gpu = cuda_make_array(l->output, l->outputs*l->batch);
    #ਜੇ_ਅੰਤ
}


ਖਾਲੀ forward_crop_layer(ਅਚਲ crop_layer l, ਜਾਲ net)
{
    ਪੂਰਨ i,j,c,b,ਪੰਕਤੀ,ਖਾਨਾ;
    ਪੂਰਨ ਸੂਚਕਾਂਕ;
    ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    ਪੂਰਨ flip = (l.flip && rand()%2);
    ਪੂਰਨ dh = rand()%(l.h - l.ਬਾਹਰਲਾ_h + 1);
    ਪੂਰਨ dw = rand()%(l.w - l.ਬਾਹਰਲਾ_w + 1);
    ਦਸ਼ਮਲਵ scale = 2;
    ਦਸ਼ਮਲਵ trans = -1;
    ਜੇ(l.noadjust){
        scale = 1;
        trans = 0;
    }
    ਜੇ(!net.train){
        flip = 0;
        dh = (l.h - l.ਬਾਹਰਲਾ_h)/2;
        dw = (l.w - l.ਬਾਹਰਲਾ_w)/2;
    }
    ਲਈ(b = 0; b < l.batch; ++b){
        ਲਈ(c = 0; c < l.c; ++c){
            ਲਈ(i = 0; i < l.ਬਾਹਰਲਾ_h; ++i){
                ਲਈ(j = 0; j < l.ਬਾਹਰਲਾ_w; ++j){
                    ਜੇ(flip){
                        ਖਾਨਾ = l.w - dw - j - 1;    
                    }ਨਹੀਂਤਾਂ{
                        ਖਾਨਾ = j + dw;
                    }
                    ਪੰਕਤੀ = i + dh;
                    ਸੂਚਕਾਂਕ = ਖਾਨਾ+l.w*(ਪੰਕਤੀ+l.h*(c + l.c*b)); 
                    l.output[ਗਿਣਤੀ_2++] = net.ਨਿਵੇਸ਼[ਸੂਚਕਾਂਕ]*scale + trans;
                }
            }
        }
    }
}


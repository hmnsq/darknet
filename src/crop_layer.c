#ضمن "crop_layer.h"
#ضمن "cuda.h"
#ضمن <stdio.h>

image get_crop_image(crop_layer l)
{
    صحيح h = l.ٻاهريون_h;
    صحيح w = l.ٻاهريون_w;
    صحيح c = l.ٻاهريون_c;
    أرجع float_منزل_image(w,h,c,l.output);
}

فراغ backward_crop_layer(ثابت crop_layer l, ڄار net){}
فراغ backward_crop_layer_gpu(ثابت crop_layer l, ڄار net){}

crop_layer make_crop_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح crop_اوچائي, صحيح crop_ويڪر, صحيح flip, عائم angle, عائم saturation, عائم exposure)
{
    fprintf(stderr, "Crop Layer: %d x %d -> %d x %d x %d image\n", h,w,crop_اوچائي,crop_ويڪر,c);
    crop_layer l = {0};
    l.type = CROP;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.scale = (عائم)crop_اوچائي / h;
    l.flip = flip;
    l.angle = angle;
    l.saturation = saturation;
    l.exposure = exposure;
    l.ٻاهريون_w = crop_ويڪر;
    l.ٻاهريون_h = crop_اوچائي;
    l.ٻاهريون_c = c;
    l.inputs = l.w * l.h * l.c;
    l.outputs = l.ٻاهريون_w * l.ٻاهريون_h * l.ٻاهريون_c;
    l.output = صفر_يادگيري_ڏيو(l.outputs*batch, حجم(عائم));
    l.forward = forward_crop_layer;
    l.backward = backward_crop_layer;

    #جيڪڏهن_وصف_ٿيل GPU
    l.forward_gpu = forward_crop_layer_gpu;
    l.backward_gpu = backward_crop_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    l.rand_gpu   = cuda_make_array(0, l.batch*8);
    #جيڪڏهن_پورو
    أرجع l;
}

فراغ resize_crop_layer(layer *l, صحيح w, صحيح h)
{
    l->w = w;
    l->h = h;

    l->ٻاهريون_w =  l->scale*w;
    l->ٻاهريون_h =  l->scale*h;

    l->inputs = l->w * l->h * l->c;
    l->outputs = l->ٻاهريون_h * l->ٻاهريون_w * l->ٻاهريون_c;

    l->output = realloc(l->output, l->batch*l->outputs*حجم(عائم));
    #جيڪڏهن_وصف_ٿيل GPU
    cuda_free(l->output_gpu);
    l->output_gpu = cuda_make_array(l->output, l->outputs*l->batch);
    #جيڪڏهن_پورو
}


فراغ forward_crop_layer(ثابت crop_layer l, ڄار net)
{
    صحيح i,j,c,b,صف,کانو;
    صحيح اشاريو;
    صحيح ڳڻپ = 0;
    صحيح flip = (l.flip && rand()%2);
    صحيح dh = rand()%(l.h - l.ٻاهريون_h + 1);
    صحيح dw = rand()%(l.w - l.ٻاهريون_w + 1);
    عائم scale = 2;
    عائم trans = -1;
    إذا(l.noadjust){
        scale = 1;
        trans = 0;
    }
    إذا(!net.train){
        flip = 0;
        dh = (l.h - l.ٻاهريون_h)/2;
        dw = (l.w - l.ٻاهريون_w)/2;
    }
    لكل(b = 0; b < l.batch; ++b){
        لكل(c = 0; c < l.c; ++c){
            لكل(i = 0; i < l.ٻاهريون_h; ++i){
                لكل(j = 0; j < l.ٻاهريون_w; ++j){
                    إذا(flip){
                        کانو = l.w - dw - j - 1;    
                    }وإلا{
                        کانو = j + dw;
                    }
                    صف = i + dh;
                    اشاريو = کانو+l.w*(صف+l.h*(c + l.c*b)); 
                    l.output[ڳڻپ++] = net.داخلا[اشاريو]*scale + trans;
                }
            }
        }
    }
}


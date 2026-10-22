#ضمن "crop_layer.h"
#ضمن "cuda.h"
#ضمن <stdio.h>

image get_crop_image(crop_layer l)
{
    صحيح h = l.بیرون_h;
    صحيح w = l.بیرون_w;
    صحيح c = l.بیرون_c;
    أرجع float_مقصد_image(w,h,c,l.output);
}

فراغ backward_crop_layer(ثابت crop_layer l, شبکه net){}
فراغ backward_crop_layer_gpu(ثابت crop_layer l, شبکه net){}

crop_layer make_crop_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح crop_ارتفاع, صحيح crop_عرض, صحيح flip, عائم angle, عائم saturation, عائم exposure)
{
    fprintf(stderr, "Crop Layer: %d x %d -> %d x %d x %d image\n", h,w,crop_ارتفاع,crop_عرض,c);
    crop_layer l = {0};
    l.type = CROP;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.scale = (عائم)crop_ارتفاع / h;
    l.flip = flip;
    l.angle = angle;
    l.saturation = saturation;
    l.exposure = exposure;
    l.بیرون_w = crop_عرض;
    l.بیرون_h = crop_ارتفاع;
    l.بیرون_c = c;
    l.inputs = l.w * l.h * l.c;
    l.outputs = l.بیرون_w * l.بیرون_h * l.بیرون_c;
    l.output = حافظه_صفر_بده(l.outputs*batch, حجم(عائم));
    l.forward = forward_crop_layer;
    l.backward = backward_crop_layer;

    #اگر_تعریف_شده GPU
    l.forward_gpu = forward_crop_layer_gpu;
    l.backward_gpu = backward_crop_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    l.rand_gpu   = cuda_make_array(0, l.batch*8);
    #پایان_اگر
    أرجع l;
}

فراغ resize_crop_layer(layer *l, صحيح w, صحيح h)
{
    l->w = w;
    l->h = h;

    l->بیرون_w =  l->scale*w;
    l->بیرون_h =  l->scale*h;

    l->inputs = l->w * l->h * l->c;
    l->outputs = l->بیرون_h * l->بیرون_w * l->بیرون_c;

    l->output = realloc(l->output, l->batch*l->outputs*حجم(عائم));
    #اگر_تعریف_شده GPU
    cuda_free(l->output_gpu);
    l->output_gpu = cuda_make_array(l->output, l->outputs*l->batch);
    #پایان_اگر
}


فراغ forward_crop_layer(ثابت crop_layer l, شبکه net)
{
    صحيح i,j,c,b,ردیف,ستون;
    صحيح شاخص;
    صحيح شمار = 0;
    صحيح flip = (l.flip && rand()%2);
    صحيح dh = rand()%(l.h - l.بیرون_h + 1);
    صحيح dw = rand()%(l.w - l.بیرون_w + 1);
    عائم scale = 2;
    عائم trans = -1;
    إذا(l.noadjust){
        scale = 1;
        trans = 0;
    }
    إذا(!net.train){
        flip = 0;
        dh = (l.h - l.بیرون_h)/2;
        dw = (l.w - l.بیرون_w)/2;
    }
    لكل(b = 0; b < l.batch; ++b){
        لكل(c = 0; c < l.c; ++c){
            لكل(i = 0; i < l.بیرون_h; ++i){
                لكل(j = 0; j < l.بیرون_w; ++j){
                    إذا(flip){
                        ستون = l.w - dw - j - 1;    
                    }وإلا{
                        ستون = j + dw;
                    }
                    ردیف = i + dh;
                    شاخص = ستون+l.w*(ردیف+l.h*(c + l.c*b)); 
                    l.output[شمار++] = net.درون_داد[شاخص]*scale + trans;
                }
            }
        }
    }
}


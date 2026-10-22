#ضمن "crop_layer.h"
#ضمن "cuda.h"
#ضمن <stdio.h>

image get_crop_image(crop_layer l)
{
    صحيح h = l.بهر_h;
    صحيح w = l.بهر_w;
    صحيح c = l.بهر_c;
    أرجع float_منزل_image(w,h,c,l.output);
}

فراغ backward_crop_layer(ثابت crop_layer l, شبکه net){}
فراغ backward_crop_layer_gpu(ثابت crop_layer l, شبکه net){}

crop_layer make_crop_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح crop_لوړوالی, صحيح crop_پلنوالی, صحيح flip, عائم angle, عائم saturation, عائم exposure)
{
    fprintf(stderr, "Crop Layer: %d x %d -> %d x %d x %d image\n", h,w,crop_لوړوالی,crop_پلنوالی,c);
    crop_layer l = {0};
    l.type = CROP;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.scale = (عائم)crop_لوړوالی / h;
    l.flip = flip;
    l.angle = angle;
    l.saturation = saturation;
    l.exposure = exposure;
    l.بهر_w = crop_پلنوالی;
    l.بهر_h = crop_لوړوالی;
    l.بهر_c = c;
    l.inputs = l.w * l.h * l.c;
    l.outputs = l.بهر_w * l.بهر_h * l.بهر_c;
    l.output = حافظه_صفر_ورکړه(l.outputs*batch, حجم(عائم));
    l.forward = forward_crop_layer;
    l.backward = backward_crop_layer;

    #که_تعریف_شوی GPU
    l.forward_gpu = forward_crop_layer_gpu;
    l.backward_gpu = backward_crop_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    l.rand_gpu   = cuda_make_array(0, l.batch*8);
    #د_که_پای
    أرجع l;
}

فراغ resize_crop_layer(layer *l, صحيح w, صحيح h)
{
    l->w = w;
    l->h = h;

    l->بهر_w =  l->scale*w;
    l->بهر_h =  l->scale*h;

    l->inputs = l->w * l->h * l->c;
    l->outputs = l->بهر_h * l->بهر_w * l->بهر_c;

    l->output = realloc(l->output, l->batch*l->outputs*حجم(عائم));
    #که_تعریف_شوی GPU
    cuda_free(l->output_gpu);
    l->output_gpu = cuda_make_array(l->output, l->outputs*l->batch);
    #د_که_پای
}


فراغ forward_crop_layer(ثابت crop_layer l, شبکه net)
{
    صحيح i,j,c,b,لیکه,ستون;
    صحيح شاخص;
    صحيح شمېر = 0;
    صحيح flip = (l.flip && rand()%2);
    صحيح dh = rand()%(l.h - l.بهر_h + 1);
    صحيح dw = rand()%(l.w - l.بهر_w + 1);
    عائم scale = 2;
    عائم trans = -1;
    إذا(l.noadjust){
        scale = 1;
        trans = 0;
    }
    إذا(!net.train){
        flip = 0;
        dh = (l.h - l.بهر_h)/2;
        dw = (l.w - l.بهر_w)/2;
    }
    لكل(b = 0; b < l.batch; ++b){
        لكل(c = 0; c < l.c; ++c){
            لكل(i = 0; i < l.بهر_h; ++i){
                لكل(j = 0; j < l.بهر_w; ++j){
                    إذا(flip){
                        ستون = l.w - dw - j - 1;    
                    }وإلا{
                        ستون = j + dw;
                    }
                    لیکه = i + dh;
                    شاخص = ستون+l.w*(لیکه+l.h*(c + l.c*b)); 
                    l.output[شمېر++] = net.ننوت[شاخص]*scale + trans;
                }
            }
        }
    }
}


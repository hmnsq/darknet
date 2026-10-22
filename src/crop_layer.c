#شامل "crop_layer.h"
#شامل "cuda.h"
#شامل <stdio.h>

image get_crop_image(crop_layer l)
{
    صحیح h = l.برآمد_h;
    صحیح w = l.برآمد_w;
    صحیح c = l.برآمد_c;
    واپس float_مقصد_image(w,h,c,l.output);
}

خالی backward_crop_layer(ثابت crop_layer l, جال net){}
خالی backward_crop_layer_gpu(ثابت crop_layer l, جال net){}

crop_layer make_crop_layer(صحیح batch, صحیح h, صحیح w, صحیح c, صحیح crop_اونچائی, صحیح crop_چوڑائی, صحیح flip, اعشاری angle, اعشاری saturation, اعشاری exposure)
{
    fprintf(stderr, "Crop Layer: %d x %d -> %d x %d x %d image\n", h,w,crop_اونچائی,crop_چوڑائی,c);
    crop_layer l = {0};
    l.type = CROP;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.scale = (اعشاری)crop_اونچائی / h;
    l.flip = flip;
    l.angle = angle;
    l.saturation = saturation;
    l.exposure = exposure;
    l.برآمد_w = crop_چوڑائی;
    l.برآمد_h = crop_اونچائی;
    l.برآمد_c = c;
    l.inputs = l.w * l.h * l.c;
    l.outputs = l.برآمد_w * l.برآمد_h * l.برآمد_c;
    l.output = صفر_حافظہ_دو(l.outputs*batch, حجم(اعشاری));
    l.forward = forward_crop_layer;
    l.backward = backward_crop_layer;

    #اگر_تعریف_شدہ GPU
    l.forward_gpu = forward_crop_layer_gpu;
    l.backward_gpu = backward_crop_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    l.rand_gpu   = cuda_make_array(0, l.batch*8);
    #اگر_ختم
    واپس l;
}

خالی resize_crop_layer(layer *l, صحیح w, صحیح h)
{
    l->w = w;
    l->h = h;

    l->برآمد_w =  l->scale*w;
    l->برآمد_h =  l->scale*h;

    l->inputs = l->w * l->h * l->c;
    l->outputs = l->برآمد_h * l->برآمد_w * l->برآمد_c;

    l->output = realloc(l->output, l->batch*l->outputs*حجم(اعشاری));
    #اگر_تعریف_شدہ GPU
    cuda_free(l->output_gpu);
    l->output_gpu = cuda_make_array(l->output, l->outputs*l->batch);
    #اگر_ختم
}


خالی forward_crop_layer(ثابت crop_layer l, جال net)
{
    صحیح i,j,c,b,صف,ستون;
    صحیح اشاریہ;
    صحیح گنتی = 0;
    صحیح flip = (l.flip && rand()%2);
    صحیح dh = rand()%(l.h - l.برآمد_h + 1);
    صحیح dw = rand()%(l.w - l.برآمد_w + 1);
    اعشاری scale = 2;
    اعشاری trans = -1;
    اگر(l.noadjust){
        scale = 1;
        trans = 0;
    }
    اگر(!net.train){
        flip = 0;
        dh = (l.h - l.برآمد_h)/2;
        dw = (l.w - l.برآمد_w)/2;
    }
    برائے(b = 0; b < l.batch; ++b){
        برائے(c = 0; c < l.c; ++c){
            برائے(i = 0; i < l.برآمد_h; ++i){
                برائے(j = 0; j < l.برآمد_w; ++j){
                    اگر(flip){
                        ستون = l.w - dw - j - 1;    
                    }ورنہ{
                        ستون = j + dw;
                    }
                    صف = i + dh;
                    اشاریہ = ستون+l.w*(صف+l.h*(c + l.c*b)); 
                    l.output[گنتی++] = net.داخلہ[اشاریہ]*scale + trans;
                }
            }
        }
    }
}


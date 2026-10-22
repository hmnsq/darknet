#ضمن "crop_layer.h"
#ضمن "cuda.h"
#ضمن <stdio.h>

image get_crop_image(crop_layer l)
{
    صحيح h = l.out_h;
    صحيح w = l.out_w;
    صحيح c = l.out_c;
    أرجع float_to_image(w,h,c,l.output);
}

فراغ backward_crop_layer(ثابت crop_layer l, network net){}
فراغ backward_crop_layer_gpu(ثابت crop_layer l, network net){}

crop_layer make_crop_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح crop_height, صحيح crop_width, صحيح flip, عائم angle, عائم saturation, عائم exposure)
{
    fprintf(stderr, "Crop Layer: %d x %d -> %d x %d x %d image\n", h,w,crop_height,crop_width,c);
    crop_layer l = {0};
    l.type = CROP;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.scale = (عائم)crop_height / h;
    l.flip = flip;
    l.angle = angle;
    l.saturation = saturation;
    l.exposure = exposure;
    l.out_w = crop_width;
    l.out_h = crop_height;
    l.out_c = c;
    l.inputs = l.w * l.h * l.c;
    l.outputs = l.out_w * l.out_h * l.out_c;
    l.output = calloc(l.outputs*batch, حجم(عائم));
    l.forward = forward_crop_layer;
    l.backward = backward_crop_layer;

    #ifdef GPU
    l.forward_gpu = forward_crop_layer_gpu;
    l.backward_gpu = backward_crop_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    l.rand_gpu   = cuda_make_array(0, l.batch*8);
    #endif
    أرجع l;
}

فراغ resize_crop_layer(layer *l, صحيح w, صحيح h)
{
    l->w = w;
    l->h = h;

    l->out_w =  l->scale*w;
    l->out_h =  l->scale*h;

    l->inputs = l->w * l->h * l->c;
    l->outputs = l->out_h * l->out_w * l->out_c;

    l->output = realloc(l->output, l->batch*l->outputs*حجم(عائم));
    #ifdef GPU
    cuda_free(l->output_gpu);
    l->output_gpu = cuda_make_array(l->output, l->outputs*l->batch);
    #endif
}


فراغ forward_crop_layer(ثابت crop_layer l, network net)
{
    صحيح i,j,c,b,row,col;
    صحيح index;
    صحيح عدد = 0;
    صحيح flip = (l.flip && rand()%2);
    صحيح dh = rand()%(l.h - l.out_h + 1);
    صحيح dw = rand()%(l.w - l.out_w + 1);
    عائم scale = 2;
    عائم trans = -1;
    إذا(l.noadjust){
        scale = 1;
        trans = 0;
    }
    إذا(!net.train){
        flip = 0;
        dh = (l.h - l.out_h)/2;
        dw = (l.w - l.out_w)/2;
    }
    لكل(b = 0; b < l.batch; ++b){
        لكل(c = 0; c < l.c; ++c){
            لكل(i = 0; i < l.out_h; ++i){
                لكل(j = 0; j < l.out_w; ++j){
                    إذا(flip){
                        col = l.w - dw - j - 1;    
                    }وإلا{
                        col = j + dw;
                    }
                    row = i + dh;
                    index = col+l.w*(row+l.h*(c + l.c*b)); 
                    l.output[عدد++] = net.مدخل[index]*scale + trans;
                }
            }
        }
    }
}


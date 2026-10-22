#شامل "crop_layer.h"
#شامل "cuda.h"
#شامل <stdio.h>

image get_crop_image(crop_layer l)
{
    صحیح h = l.out_h;
    صحیح w = l.out_w;
    صحیح c = l.out_c;
    بازگشت float_to_image(w,h,c,l.output);
}

تهی backward_crop_layer(ثابت crop_layer l, network net){}
تهی backward_crop_layer_gpu(ثابت crop_layer l, network net){}

crop_layer make_crop_layer(صحیح batch, صحیح h, صحیح w, صحیح c, صحیح crop_height, صحیح crop_width, صحیح flip, اعشاری angle, اعشاری saturation, اعشاری exposure)
{
    fprintf(stderr, "Crop Layer: %d x %d -> %d x %d x %d image\n", h,w,crop_height,crop_width,c);
    crop_layer l = {0};
    l.type = CROP;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.scale = (اعشاری)crop_height / h;
    l.flip = flip;
    l.angle = angle;
    l.saturation = saturation;
    l.exposure = exposure;
    l.out_w = crop_width;
    l.out_h = crop_height;
    l.out_c = c;
    l.inputs = l.w * l.h * l.c;
    l.outputs = l.out_w * l.out_h * l.out_c;
    l.output = calloc(l.outputs*batch, اندازه(اعشاری));
    l.forward = forward_crop_layer;
    l.backward = backward_crop_layer;

    #ifdef GPU
    l.forward_gpu = forward_crop_layer_gpu;
    l.backward_gpu = backward_crop_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    l.rand_gpu   = cuda_make_array(0, l.batch*8);
    #endif
    بازگشت l;
}

تهی resize_crop_layer(layer *l, صحیح w, صحیح h)
{
    l->w = w;
    l->h = h;

    l->out_w =  l->scale*w;
    l->out_h =  l->scale*h;

    l->inputs = l->w * l->h * l->c;
    l->outputs = l->out_h * l->out_w * l->out_c;

    l->output = realloc(l->output, l->batch*l->outputs*اندازه(اعشاری));
    #ifdef GPU
    cuda_free(l->output_gpu);
    l->output_gpu = cuda_make_array(l->output, l->outputs*l->batch);
    #endif
}


تهی forward_crop_layer(ثابت crop_layer l, network net)
{
    صحیح i,j,c,b,row,col;
    صحیح index;
    صحیح شمار = 0;
    صحیح flip = (l.flip && rand()%2);
    صحیح dh = rand()%(l.h - l.out_h + 1);
    صحیح dw = rand()%(l.w - l.out_w + 1);
    اعشاری scale = 2;
    اعشاری trans = -1;
    اگر(l.noadjust){
        scale = 1;
        trans = 0;
    }
    اگر(!net.train){
        flip = 0;
        dh = (l.h - l.out_h)/2;
        dw = (l.w - l.out_w)/2;
    }
    برای(b = 0; b < l.batch; ++b){
        برای(c = 0; c < l.c; ++c){
            برای(i = 0; i < l.out_h; ++i){
                برای(j = 0; j < l.out_w; ++j){
                    اگر(flip){
                        col = l.w - dw - j - 1;    
                    }وگرنه{
                        col = j + dw;
                    }
                    row = i + dh;
                    index = col+l.w*(row+l.h*(c + l.c*b)); 
                    l.output[شمار++] = net.درون_داد[index]*scale + trans;
                }
            }
        }
    }
}


#অন্তর্ভাব "crop_layer.h"
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব <stdio.h>

image get_crop_image(crop_layer l)
{
    পূর্ণসংখ্যা h = l.out_h;
    পূর্ণসংখ্যা w = l.out_w;
    পূর্ণসংখ্যা c = l.out_c;
    ফেরত float_to_image(w,h,c,l.output);
}

শূন্য backward_crop_layer(ধ্রুবক crop_layer l, network net){}
শূন্য backward_crop_layer_gpu(ধ্রুবক crop_layer l, network net){}

crop_layer make_crop_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা h, পূর্ণসংখ্যা w, পূর্ণসংখ্যা c, পূর্ণসংখ্যা crop_height, পূর্ণসংখ্যা crop_width, পূর্ণসংখ্যা flip, দশমিক angle, দশমিক saturation, দশমিক exposure)
{
    fprintf(stderr, "Crop Layer: %d x %d -> %d x %d x %d image\n", h,w,crop_height,crop_width,c);
    crop_layer l = {0};
    l.type = CROP;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.scale = (দশমিক)crop_height / h;
    l.flip = flip;
    l.angle = angle;
    l.saturation = saturation;
    l.exposure = exposure;
    l.out_w = crop_width;
    l.out_h = crop_height;
    l.out_c = c;
    l.inputs = l.w * l.h * l.c;
    l.outputs = l.out_w * l.out_h * l.out_c;
    l.output = calloc(l.outputs*batch, আকার(দশমিক));
    l.forward = forward_crop_layer;
    l.backward = backward_crop_layer;

    #ifdef GPU
    l.forward_gpu = forward_crop_layer_gpu;
    l.backward_gpu = backward_crop_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    l.rand_gpu   = cuda_make_array(0, l.batch*8);
    #endif
    ফেরত l;
}

শূন্য resize_crop_layer(layer *l, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    l->w = w;
    l->h = h;

    l->out_w =  l->scale*w;
    l->out_h =  l->scale*h;

    l->inputs = l->w * l->h * l->c;
    l->outputs = l->out_h * l->out_w * l->out_c;

    l->output = realloc(l->output, l->batch*l->outputs*আকার(দশমিক));
    #ifdef GPU
    cuda_free(l->output_gpu);
    l->output_gpu = cuda_make_array(l->output, l->outputs*l->batch);
    #endif
}


শূন্য forward_crop_layer(ধ্রুবক crop_layer l, network net)
{
    পূর্ণসংখ্যা i,j,c,b,row,col;
    পূর্ণসংখ্যা সূচক;
    পূর্ণসংখ্যা গুনতি = 0;
    পূর্ণসংখ্যা flip = (l.flip && rand()%2);
    পূর্ণসংখ্যা dh = rand()%(l.h - l.out_h + 1);
    পূর্ণসংখ্যা dw = rand()%(l.w - l.out_w + 1);
    দশমিক scale = 2;
    দশমিক trans = -1;
    যদি(l.noadjust){
        scale = 1;
        trans = 0;
    }
    যদি(!net.train){
        flip = 0;
        dh = (l.h - l.out_h)/2;
        dw = (l.w - l.out_w)/2;
    }
    জন্য(b = 0; b < l.batch; ++b){
        জন্য(c = 0; c < l.c; ++c){
            জন্য(i = 0; i < l.out_h; ++i){
                জন্য(j = 0; j < l.out_w; ++j){
                    যদি(flip){
                        col = l.w - dw - j - 1;    
                    }অন্যথা{
                        col = j + dw;
                    }
                    row = i + dh;
                    সূচক = col+l.w*(row+l.h*(c + l.c*b)); 
                    l.output[গুনতি++] = net.প্রবেশ[সূচক]*scale + trans;
                }
            }
        }
    }
}


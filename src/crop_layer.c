#include "crop_layer.h"
#include "cuda.h"
#include <stdio.h>

image get_crop_image(crop_layer l)
{
    ចំនួនគត់ h = l.out_h;
    ចំនួនគត់ w = l.out_w;
    ចំនួនគត់ c = l.out_c;
    ត្រឡប់ float_to_image(w,h,c,l.output);
}

void backward_crop_layer(const crop_layer l, network net){}
void backward_crop_layer_gpu(const crop_layer l, network net){}

crop_layer make_crop_layer(ចំនួនគត់ batch, ចំនួនគត់ h, ចំនួនគត់ w, ចំនួនគត់ c, ចំនួនគត់ crop_height, ចំនួនគត់ crop_width, ចំនួនគត់ flip, float angle, float saturation, float exposure)
{
    fprintf(stderr, "Crop Layer: %d x %d -> %d x %d x %d image\n", h,w,crop_height,crop_width,c);
    crop_layer l = {0};
    l.type = CROP;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.scale = (float)crop_height / h;
    l.flip = flip;
    l.angle = angle;
    l.saturation = saturation;
    l.exposure = exposure;
    l.out_w = crop_width;
    l.out_h = crop_height;
    l.out_c = c;
    l.inputs = l.w * l.h * l.c;
    l.outputs = l.out_w * l.out_h * l.out_c;
    l.output = calloc(l.outputs*batch, parimaa_na(float));
    l.forward = forward_crop_layer;
    l.backward = backward_crop_layer;

    #ifdef GPU
    l.forward_gpu = forward_crop_layer_gpu;
    l.backward_gpu = backward_crop_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    l.rand_gpu   = cuda_make_array(0, l.batch*8);
    #endif
    ត្រឡប់ l;
}

void resize_crop_layer(layer *l, ចំនួនគត់ w, ចំនួនគត់ h)
{
    l->w = w;
    l->h = h;

    l->out_w =  l->scale*w;
    l->out_h =  l->scale*h;

    l->inputs = l->w * l->h * l->c;
    l->outputs = l->out_h * l->out_w * l->out_c;

    l->output = realloc(l->output, l->batch*l->outputs*parimaa_na(float));
    #ifdef GPU
    cuda_free(l->output_gpu);
    l->output_gpu = cuda_make_array(l->output, l->outputs*l->batch);
    #endif
}


void forward_crop_layer(const crop_layer l, network net)
{
    ចំនួនគត់ i,j,c,b,row,col;
    ចំនួនគត់ index;
    ចំនួនគត់ count = 0;
    ចំនួនគត់ flip = (l.flip && rand()%2);
    ចំនួនគត់ dh = rand()%(l.h - l.out_h + 1);
    ចំនួនគត់ dw = rand()%(l.w - l.out_w + 1);
    float scale = 2;
    float trans = -1;
    ប្រសិនបើ(l.noadjust){
        scale = 1;
        trans = 0;
    }
    ប្រសិនបើ(!net.train){
        flip = 0;
        dh = (l.h - l.out_h)/2;
        dw = (l.w - l.out_w)/2;
    }
    សម្រាប់(b = 0; b < l.batch; ++b){
        សម្រាប់(c = 0; c < l.c; ++c){
            សម្រាប់(i = 0; i < l.out_h; ++i){
                សម្រាប់(j = 0; j < l.out_w; ++j){
                    ប្រសិនបើ(flip){
                        col = l.w - dw - j - 1;    
                    }បើមិនដូច្នេះ{
                        col = j + dw;
                    }
                    row = i + dh;
                    index = col+l.w*(row+l.h*(c + l.c*b)); 
                    l.output[count++] = net.input[index]*scale + trans;
                }
            }
        }
    }
}


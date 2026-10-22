#include "crop_layer.h"
#include "cuda.h"
#include <stdio.h>

image get_crop_image(crop_layer l)
{
    puur_naa_nka h = l.out_h;
    puur_naa_nka w = l.out_w;
    puur_naa_nka c = l.out_c;
    ᨀᨘᨒ float_to_image(w,h,c,l.output);
}

void backward_crop_layer(const crop_layer l, network net){}
void backward_crop_layer_gpu(const crop_layer l, network net){}

crop_layer make_crop_layer(puur_naa_nka batch, puur_naa_nka h, puur_naa_nka w, puur_naa_nka c, puur_naa_nka crop_height, puur_naa_nka crop_width, puur_naa_nka flip, float angle, float saturation, float exposure)
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
    ᨀᨘᨒ l;
}

void resize_crop_layer(layer *l, puur_naa_nka w, puur_naa_nka h)
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
    puur_naa_nka i,j,c,b,row,col;
    puur_naa_nka index;
    puur_naa_nka count = 0;
    puur_naa_nka flip = (l.flip && rand()%2);
    puur_naa_nka dh = rand()%(l.h - l.out_h + 1);
    puur_naa_nka dw = rand()%(l.w - l.out_w + 1);
    float scale = 2;
    float trans = -1;
    ᨀᨘ(l.noadjust){
        scale = 1;
        trans = 0;
    }
    ᨀᨘ(!net.train){
        flip = 0;
        dh = (l.h - l.out_h)/2;
        dw = (l.w - l.out_w)/2;
    }
    for(b = 0; b < l.batch; ++b){
        for(c = 0; c < l.c; ++c){
            for(i = 0; i < l.out_h; ++i){
                for(j = 0; j < l.out_w; ++j){
                    ᨀᨘ(flip){
                        col = l.w - dw - j - 1;    
                    }ᨀᨘᨁ{
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


#include "crop_layer.h"
#include "cuda.h"
#include <stdio.h>

image get_crop_image(crop_layer l)
{
    ཧྲིལ h = l.out_h;
    ཧྲིལ w = l.out_w;
    ཧྲིལ c = l.out_c;
    ཕྱིར float_to_image(w,h,c,l.output);
}

void backward_crop_layer(const crop_layer l, network net){}
void backward_crop_layer_gpu(const crop_layer l, network net){}

crop_layer make_crop_layer(ཧྲིལ batch, ཧྲིལ h, ཧྲིལ w, ཧྲིལ c, ཧྲིལ crop_height, ཧྲིལ crop_width, ཧྲིལ flip, float angle, float saturation, float exposure)
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
    ཕྱིར l;
}

void resize_crop_layer(layer *l, ཧྲིལ w, ཧྲིལ h)
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
    ཧྲིལ i,j,c,b,row,col;
    ཧྲིལ index;
    ཧྲིལ count = 0;
    ཧྲིལ flip = (l.flip && rand()%2);
    ཧྲིལ dh = rand()%(l.h - l.out_h + 1);
    ཧྲིལ dw = rand()%(l.w - l.out_w + 1);
    float scale = 2;
    float trans = -1;
    གལ་ཏེ(l.noadjust){
        scale = 1;
        trans = 0;
    }
    གལ་ཏེ(!net.train){
        flip = 0;
        dh = (l.h - l.out_h)/2;
        dw = (l.w - l.out_w)/2;
    }
    ཆེད་དུ(b = 0; b < l.batch; ++b){
        ཆེད་དུ(c = 0; c < l.c; ++c){
            ཆེད་དུ(i = 0; i < l.out_h; ++i){
                ཆེད་དུ(j = 0; j < l.out_w; ++j){
                    གལ་ཏེ(flip){
                        col = l.w - dw - j - 1;    
                    }དེ་མིན{
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


#ଅନ୍ତର୍ଭାଵ "crop_layer.h"
#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>

image get_crop_image(crop_layer l)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା h = l.out_h;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା w = l.out_w;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = l.out_c;
    ଫେରନ୍ତୁ float_to_image(w,h,c,l.output);
}

ଶୂନ୍ୟ backward_crop_layer(ଅଚଳ crop_layer l, network net){}
ଶୂନ୍ୟ backward_crop_layer_gpu(ଅଚଳ crop_layer l, network net){}

crop_layer make_crop_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା crop_height, ପୂର୍ଣ୍ଣସଂଖ୍ୟା crop_width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flip, ଦଶମିକ angle, ଦଶମିକ saturation, ଦଶମିକ exposure)
{
    fprintf(stderr, "Crop Layer: %d x %d -> %d x %d x %d image\n", h,w,crop_height,crop_width,c);
    crop_layer l = {0};
    l.type = CROP;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.scale = (ଦଶମିକ)crop_height / h;
    l.flip = flip;
    l.angle = angle;
    l.saturation = saturation;
    l.exposure = exposure;
    l.out_w = crop_width;
    l.out_h = crop_height;
    l.out_c = c;
    l.inputs = l.w * l.h * l.c;
    l.outputs = l.out_w * l.out_h * l.out_c;
    l.output = calloc(l.outputs*batch, ଆକାର(ଦଶମିକ));
    l.forward = forward_crop_layer;
    l.backward = backward_crop_layer;

    #ifdef GPU
    l.forward_gpu = forward_crop_layer_gpu;
    l.backward_gpu = backward_crop_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    l.rand_gpu   = cuda_make_array(0, l.batch*8);
    #endif
    ଫେରନ୍ତୁ l;
}

ଶୂନ୍ୟ resize_crop_layer(layer *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    l->w = w;
    l->h = h;

    l->out_w =  l->scale*w;
    l->out_h =  l->scale*h;

    l->inputs = l->w * l->h * l->c;
    l->outputs = l->out_h * l->out_w * l->out_c;

    l->output = realloc(l->output, l->batch*l->outputs*ଆକାର(ଦଶମିକ));
    #ifdef GPU
    cuda_free(l->output_gpu);
    l->output_gpu = cuda_make_array(l->output, l->outputs*l->batch);
    #endif
}


ଶୂନ୍ୟ forward_crop_layer(ଅଚଳ crop_layer l, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,c,b,row,col;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା index;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା flip = (l.flip && rand()%2);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dh = rand()%(l.h - l.out_h + 1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dw = rand()%(l.w - l.out_w + 1);
    ଦଶମିକ scale = 2;
    ଦଶମିକ trans = -1;
    ଯଦି(l.noadjust){
        scale = 1;
        trans = 0;
    }
    ଯଦି(!net.train){
        flip = 0;
        dh = (l.h - l.out_h)/2;
        dw = (l.w - l.out_w)/2;
    }
    ପାଇଁ(b = 0; b < l.batch; ++b){
        ପାଇଁ(c = 0; c < l.c; ++c){
            ପାଇଁ(i = 0; i < l.out_h; ++i){
                ପାଇଁ(j = 0; j < l.out_w; ++j){
                    ଯଦି(flip){
                        col = l.w - dw - j - 1;    
                    }ଅନ୍ୟଥା{
                        col = j + dw;
                    }
                    row = i + dh;
                    index = col+l.w*(row+l.h*(c + l.c*b)); 
                    l.output[ଗଣନା_2++] = net.ନିବେଶ[index]*scale + trans;
                }
            }
        }
    }
}


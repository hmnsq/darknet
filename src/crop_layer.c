#अन्तर्भाव "crop_layer.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव <stdio.h>

image get_crop_image(crop_layer l)
{
    पूर्णाङ्क h = l.out_h;
    पूर्णाङ्क w = l.out_w;
    पूर्णाङ्क c = l.out_c;
    प्रत्यागम float_to_image(w,h,c,l.output);
}

शून्य backward_crop_layer(ध्रुव crop_layer l, network net){}
शून्य backward_crop_layer_gpu(ध्रुव crop_layer l, network net){}

crop_layer make_crop_layer(पूर्णाङ्क batch, पूर्णाङ्क h, पूर्णाङ्क w, पूर्णाङ्क c, पूर्णाङ्क crop_height, पूर्णाङ्क crop_width, पूर्णाङ्क flip, भिन्न angle, भिन्न saturation, भिन्न exposure)
{
    fprintf(stderr, "Crop Layer: %d x %d -> %d x %d x %d image\n", h,w,crop_height,crop_width,c);
    crop_layer l = {0};
    l.type = CROP;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.scale = (भिन्न)crop_height / h;
    l.flip = flip;
    l.angle = angle;
    l.saturation = saturation;
    l.exposure = exposure;
    l.out_w = crop_width;
    l.out_h = crop_height;
    l.out_c = c;
    l.inputs = l.w * l.h * l.c;
    l.outputs = l.out_w * l.out_h * l.out_c;
    l.output = calloc(l.outputs*batch, परिमाण(भिन्न));
    l.forward = forward_crop_layer;
    l.backward = backward_crop_layer;

    #ifdef GPU
    l.forward_gpu = forward_crop_layer_gpu;
    l.backward_gpu = backward_crop_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    l.rand_gpu   = cuda_make_array(0, l.batch*8);
    #endif
    प्रत्यागम l;
}

शून्य resize_crop_layer(layer *l, पूर्णाङ्क w, पूर्णाङ्क h)
{
    l->w = w;
    l->h = h;

    l->out_w =  l->scale*w;
    l->out_h =  l->scale*h;

    l->inputs = l->w * l->h * l->c;
    l->outputs = l->out_h * l->out_w * l->out_c;

    l->output = realloc(l->output, l->batch*l->outputs*परिमाण(भिन्न));
    #ifdef GPU
    cuda_free(l->output_gpu);
    l->output_gpu = cuda_make_array(l->output, l->outputs*l->batch);
    #endif
}


शून्य forward_crop_layer(ध्रुव crop_layer l, network net)
{
    पूर्णाङ्क i,j,c,b,row,col;
    पूर्णाङ्क index;
    पूर्णाङ्क गिनती = 0;
    पूर्णाङ्क flip = (l.flip && rand()%2);
    पूर्णाङ्क dh = rand()%(l.h - l.out_h + 1);
    पूर्णाङ्क dw = rand()%(l.w - l.out_w + 1);
    भिन्न scale = 2;
    भिन्न trans = -1;
    यदि(l.noadjust){
        scale = 1;
        trans = 0;
    }
    यदि(!net.train){
        flip = 0;
        dh = (l.h - l.out_h)/2;
        dw = (l.w - l.out_w)/2;
    }
    प्रति(b = 0; b < l.batch; ++b){
        प्रति(c = 0; c < l.c; ++c){
            प्रति(i = 0; i < l.out_h; ++i){
                प्रति(j = 0; j < l.out_w; ++j){
                    यदि(flip){
                        col = l.w - dw - j - 1;    
                    }अन्यथा{
                        col = j + dw;
                    }
                    row = i + dh;
                    index = col+l.w*(row+l.h*(c + l.c*b)); 
                    l.output[गिनती++] = net.निवेश[index]*scale + trans;
                }
            }
        }
    }
}


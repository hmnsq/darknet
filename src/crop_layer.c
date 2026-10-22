#अन्तर्भाव "crop_layer.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव <stdio.h>

image get_crop_image(crop_layer l)
{
    पूर्णाङ्क h = l.out_h;
    पूर्णाङ्क w = l.out_w;
    पूर्णाङ्क c = l.out_c;
    फर्काऊ float_to_image(w,h,c,l.output);
}

शून्य backward_crop_layer(अचर crop_layer l, network net){}
शून्य backward_crop_layer_gpu(अचर crop_layer l, network net){}

crop_layer make_crop_layer(पूर्णाङ्क batch, पूर्णाङ्क h, पूर्णाङ्क w, पूर्णाङ्क c, पूर्णाङ्क crop_height, पूर्णाङ्क crop_width, पूर्णाङ्क flip, भिन्न angle, भिन्न saturation, भिन्न exposure)
{
    ख_लिखो(मानक_त्रुटि, "Crop Layer: %d x %d -> %d x %d x %d image\n", h,w,crop_height,crop_width,c);
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
    l.output = सुस्मृति(l.outputs*batch, परिमाण(भिन्न));
    l.forward = forward_crop_layer;
    l.backward = backward_crop_layer;

    #ifdef GPU
    l.forward_gpu = forward_crop_layer_gpu;
    l.backward_gpu = backward_crop_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    l.rand_gpu   = cuda_make_array(0, l.batch*8);
    #endif
    फर्काऊ l;
}

शून्य resize_crop_layer(layer *l, पूर्णाङ्क w, पूर्णाङ्क h)
{
    l->w = w;
    l->h = h;

    l->out_w =  l->scale*w;
    l->out_h =  l->scale*h;

    l->inputs = l->w * l->h * l->c;
    l->outputs = l->out_h * l->out_w * l->out_c;

    l->output = पुनः_स्मृति(l->output, l->batch*l->outputs*परिमाण(भिन्न));
    #ifdef GPU
    cuda_free(l->output_gpu);
    l->output_gpu = cuda_make_array(l->output, l->outputs*l->batch);
    #endif
}


शून्य forward_crop_layer(अचर crop_layer l, network net)
{
    पूर्णाङ्क i,j,c,b,row,col;
    पूर्णाङ्क index;
    पूर्णाङ्क गन्ती = 0;
    पूर्णाङ्क flip = (l.flip && अक्रम()%2);
    पूर्णाङ्क dh = अक्रम()%(l.h - l.out_h + 1);
    पूर्णाङ्क dw = अक्रम()%(l.w - l.out_w + 1);
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
    लागि(b = 0; b < l.batch; ++b){
        लागि(c = 0; c < l.c; ++c){
            लागि(i = 0; i < l.out_h; ++i){
                लागि(j = 0; j < l.out_w; ++j){
                    यदि(flip){
                        col = l.w - dw - j - 1;    
                    }अन्यथा{
                        col = j + dw;
                    }
                    row = i + dh;
                    index = col+l.w*(row+l.h*(c + l.c*b)); 
                    l.output[गन्ती++] = net.आगत[index]*scale + trans;
                }
            }
        }
    }
}


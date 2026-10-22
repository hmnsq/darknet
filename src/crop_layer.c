#અન્તર્ભાવ "crop_layer.h"
#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ <stdio.h>

image get_crop_image(crop_layer l)
{
    પૂર્ણાંક h = l.out_h;
    પૂર્ણાંક w = l.out_w;
    પૂર્ણાંક c = l.out_c;
    પાછા float_to_image(w,h,c,l.output);
}

શૂન્ય backward_crop_layer(અચલ crop_layer l, network net){}
શૂન્ય backward_crop_layer_gpu(અચલ crop_layer l, network net){}

crop_layer make_crop_layer(પૂર્ણાંક batch, પૂર્ણાંક h, પૂર્ણાંક w, પૂર્ણાંક c, પૂર્ણાંક crop_height, પૂર્ણાંક crop_width, પૂર્ણાંક flip, દશાંશ angle, દશાંશ saturation, દશાંશ exposure)
{
    fprintf(stderr, "Crop Layer: %d x %d -> %d x %d x %d image\n", h,w,crop_height,crop_width,c);
    crop_layer l = {0};
    l.type = CROP;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.scale = (દશાંશ)crop_height / h;
    l.flip = flip;
    l.angle = angle;
    l.saturation = saturation;
    l.exposure = exposure;
    l.out_w = crop_width;
    l.out_h = crop_height;
    l.out_c = c;
    l.inputs = l.w * l.h * l.c;
    l.outputs = l.out_w * l.out_h * l.out_c;
    l.output = calloc(l.outputs*batch, કદ(દશાંશ));
    l.forward = forward_crop_layer;
    l.backward = backward_crop_layer;

    #ifdef GPU
    l.forward_gpu = forward_crop_layer_gpu;
    l.backward_gpu = backward_crop_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    l.rand_gpu   = cuda_make_array(0, l.batch*8);
    #endif
    પાછા l;
}

શૂન્ય resize_crop_layer(layer *l, પૂર્ણાંક w, પૂર્ણાંક h)
{
    l->w = w;
    l->h = h;

    l->out_w =  l->scale*w;
    l->out_h =  l->scale*h;

    l->inputs = l->w * l->h * l->c;
    l->outputs = l->out_h * l->out_w * l->out_c;

    l->output = realloc(l->output, l->batch*l->outputs*કદ(દશાંશ));
    #ifdef GPU
    cuda_free(l->output_gpu);
    l->output_gpu = cuda_make_array(l->output, l->outputs*l->batch);
    #endif
}


શૂન્ય forward_crop_layer(અચલ crop_layer l, network net)
{
    પૂર્ણાંક i,j,c,b,row,col;
    પૂર્ણાંક index;
    પૂર્ણાંક ગણતરી = 0;
    પૂર્ણાંક flip = (l.flip && rand()%2);
    પૂર્ણાંક dh = rand()%(l.h - l.out_h + 1);
    પૂર્ણાંક dw = rand()%(l.w - l.out_w + 1);
    દશાંશ scale = 2;
    દશાંશ trans = -1;
    જો(l.noadjust){
        scale = 1;
        trans = 0;
    }
    જો(!net.train){
        flip = 0;
        dh = (l.h - l.out_h)/2;
        dw = (l.w - l.out_w)/2;
    }
    માટે(b = 0; b < l.batch; ++b){
        માટે(c = 0; c < l.c; ++c){
            માટે(i = 0; i < l.out_h; ++i){
                માટે(j = 0; j < l.out_w; ++j){
                    જો(flip){
                        col = l.w - dw - j - 1;    
                    }નહીંતર{
                        col = j + dw;
                    }
                    row = i + dh;
                    index = col+l.w*(row+l.h*(c + l.c*b)); 
                    l.output[ગણતરી++] = net.આગત[index]*scale + trans;
                }
            }
        }
    }
}


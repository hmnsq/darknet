#అన్తర్భావ "crop_layer.h"
#అన్తర్భావ "cuda.h"
#అన్తర్భావ <stdio.h>

image get_crop_image(crop_layer l)
{
    పూర్ణాంకం h = l.బహిర్గతం_h;
    పూర్ణాంకం w = l.బహిర్గతం_w;
    పూర్ణాంకం c = l.బహిర్గతం_c;
    తిరిగి float_గమ్యం_image(w,h,c,l.output);
}

శూన్య backward_crop_layer(స్థిరరాశి crop_layer l, జాలం net){}
శూన్య backward_crop_layer_gpu(స్థిరరాశి crop_layer l, జాలం net){}

crop_layer make_crop_layer(పూర్ణాంకం batch, పూర్ణాంకం h, పూర్ణాంకం w, పూర్ణాంకం c, పూర్ణాంకం crop_ఎత్తు, పూర్ణాంకం crop_వెడల్పు, పూర్ణాంకం flip, దశాంశం angle, దశాంశం saturation, దశాంశం exposure)
{
    fprintf(stderr, "Crop Layer: %d x %d -> %d x %d x %d image\n", h,w,crop_ఎత్తు,crop_వెడల్పు,c);
    crop_layer l = {0};
    l.type = CROP;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.scale = (దశాంశం)crop_ఎత్తు / h;
    l.flip = flip;
    l.angle = angle;
    l.saturation = saturation;
    l.exposure = exposure;
    l.బహిర్గతం_w = crop_వెడల్పు;
    l.బహిర్గతం_h = crop_ఎత్తు;
    l.బహిర్గతం_c = c;
    l.inputs = l.w * l.h * l.c;
    l.outputs = l.బహిర్గతం_w * l.బహిర్గతం_h * l.బహిర్గతం_c;
    l.output = సున్నా_జ్ఞాపకం_ఇవ్వు(l.outputs*batch, పరిమాణం(దశాంశం));
    l.forward = forward_crop_layer;
    l.backward = backward_crop_layer;

    #నిర్వచితమైతే GPU
    l.forward_gpu = forward_crop_layer_gpu;
    l.backward_gpu = backward_crop_layer_gpu;
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    l.rand_gpu   = cuda_make_array(0, l.batch*8);
    #అయితే_ముగింపు
    తిరిగి l;
}

శూన్య resize_crop_layer(layer *l, పూర్ణాంకం w, పూర్ణాంకం h)
{
    l->w = w;
    l->h = h;

    l->బహిర్గతం_w =  l->scale*w;
    l->బహిర్గతం_h =  l->scale*h;

    l->inputs = l->w * l->h * l->c;
    l->outputs = l->బహిర్గతం_h * l->బహిర్గతం_w * l->బహిర్గతం_c;

    l->output = realloc(l->output, l->batch*l->outputs*పరిమాణం(దశాంశం));
    #నిర్వచితమైతే GPU
    cuda_free(l->output_gpu);
    l->output_gpu = cuda_make_array(l->output, l->outputs*l->batch);
    #అయితే_ముగింపు
}


శూన్య forward_crop_layer(స్థిరరాశి crop_layer l, జాలం net)
{
    పూర్ణాంకం i,j,c,b,అడ్డువరుస,నిలువు;
    పూర్ణాంకం సూచీ;
    పూర్ణాంకం లెక్క = 0;
    పూర్ణాంకం flip = (l.flip && rand()%2);
    పూర్ణాంకం dh = rand()%(l.h - l.బహిర్గతం_h + 1);
    పూర్ణాంకం dw = rand()%(l.w - l.బహిర్గతం_w + 1);
    దశాంశం scale = 2;
    దశాంశం trans = -1;
    ఒకవేళ(l.noadjust){
        scale = 1;
        trans = 0;
    }
    ఒకవేళ(!net.train){
        flip = 0;
        dh = (l.h - l.బహిర్గతం_h)/2;
        dw = (l.w - l.బహిర్గతం_w)/2;
    }
    కొరకు(b = 0; b < l.batch; ++b){
        కొరకు(c = 0; c < l.c; ++c){
            కొరకు(i = 0; i < l.బహిర్గతం_h; ++i){
                కొరకు(j = 0; j < l.బహిర్గతం_w; ++j){
                    ఒకవేళ(flip){
                        నిలువు = l.w - dw - j - 1;    
                    }లేకపోతే{
                        నిలువు = j + dw;
                    }
                    అడ్డువరుస = i + dh;
                    సూచీ = నిలువు+l.w*(అడ్డువరుస+l.h*(c + l.c*b)); 
                    l.output[లెక్క++] = net.ఆదానం[సూచీ]*scale + trans;
                }
            }
        }
    }
}


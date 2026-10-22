#অন্তর্ভাব "avgpool_layer.h"
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব <stdio.h>

avgpool_layer make_avgpool_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা c)
{
    fprintf(stderr, "avg                     %4d x%4d x%4d   ->  %4d\n",  w, h, c, c);
    avgpool_layer l = {0};
    l.type = AVGPOOL;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.out_w = 1;
    l.out_h = 1;
    l.out_c = c;
    l.outputs = l.out_c;
    l.inputs = h*w*c;
    পূর্ণসংখ্যা output_আকার = l.outputs * batch;
    l.output =  calloc(output_আকার, আকার(দশমিক));
    l.delta =   calloc(output_আকার, আকার(দশমিক));
    l.forward = forward_avgpool_layer;
    l.backward = backward_avgpool_layer;
    #ifdef GPU
    l.forward_gpu = forward_avgpool_layer_gpu;
    l.backward_gpu = backward_avgpool_layer_gpu;
    l.output_gpu  = cuda_make_array(l.output, output_আকার);
    l.delta_gpu   = cuda_make_array(l.delta, output_আকার);
    #endif
    ফেরত l;
}

শূন্য resize_avgpool_layer(avgpool_layer *l, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    l->w = w;
    l->h = h;
    l->inputs = h*w*l->c;
}

শূন্য forward_avgpool_layer(ধ্রুবক avgpool_layer l, network net)
{
    পূর্ণসংখ্যা b,i,k;

    জন্য(b = 0; b < l.batch; ++b){
        জন্য(k = 0; k < l.c; ++k){
            পূর্ণসংখ্যা out_সূচক = k + b*l.c;
            l.output[out_সূচক] = 0;
            জন্য(i = 0; i < l.h*l.w; ++i){
                পূর্ণসংখ্যা in_সূচক = i + l.h*l.w*(k + b*l.c);
                l.output[out_সূচক] += net.প্রবেশ[in_সূচক];
            }
            l.output[out_সূচক] /= l.h*l.w;
        }
    }
}

শূন্য backward_avgpool_layer(ধ্রুবক avgpool_layer l, network net)
{
    পূর্ণসংখ্যা b,i,k;

    জন্য(b = 0; b < l.batch; ++b){
        জন্য(k = 0; k < l.c; ++k){
            পূর্ণসংখ্যা out_সূচক = k + b*l.c;
            জন্য(i = 0; i < l.h*l.w; ++i){
                পূর্ণসংখ্যা in_সূচক = i + l.h*l.w*(k + b*l.c);
                net.delta[in_সূচক] += l.delta[out_সূচক] / (l.h*l.w);
            }
        }
    }
}


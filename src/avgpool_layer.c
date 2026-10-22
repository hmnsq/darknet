#অন্তর্ভাৱ "avgpool_layer.h"
#অন্তর্ভাৱ "cuda.h"
#অন্তর্ভাৱ <stdio.h>

avgpool_layer make_avgpool_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক c)
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
    পূর্ণাঙ্ক output_size = l.outputs * batch;
    l.output =  calloc(output_size, পরিমাণ(ভিন্ন));
    l.delta =   calloc(output_size, পরিমাণ(ভিন্ন));
    l.forward = forward_avgpool_layer;
    l.backward = backward_avgpool_layer;
    #ifdef GPU
    l.forward_gpu = forward_avgpool_layer_gpu;
    l.backward_gpu = backward_avgpool_layer_gpu;
    l.output_gpu  = cuda_make_array(l.output, output_size);
    l.delta_gpu   = cuda_make_array(l.delta, output_size);
    #endif
    ঘূৰাই l;
}

শূন্য resize_avgpool_layer(avgpool_layer *l, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h)
{
    l->w = w;
    l->h = h;
    l->inputs = h*w*l->c;
}

শূন্য forward_avgpool_layer(ধ্রুৱ avgpool_layer l, network net)
{
    পূর্ণাঙ্ক b,i,k;

    বাবে(b = 0; b < l.batch; ++b){
        বাবে(k = 0; k < l.c; ++k){
            পূর্ণাঙ্ক out_index = k + b*l.c;
            l.output[out_index] = 0;
            বাবে(i = 0; i < l.h*l.w; ++i){
                পূর্ণাঙ্ক in_index = i + l.h*l.w*(k + b*l.c);
                l.output[out_index] += net.প্ৰবেশ[in_index];
            }
            l.output[out_index] /= l.h*l.w;
        }
    }
}

শূন্য backward_avgpool_layer(ধ্রুৱ avgpool_layer l, network net)
{
    পূর্ণাঙ্ক b,i,k;

    বাবে(b = 0; b < l.batch; ++b){
        বাবে(k = 0; k < l.c; ++k){
            পূর্ণাঙ্ক out_index = k + b*l.c;
            বাবে(i = 0; i < l.h*l.w; ++i){
                পূর্ণাঙ্ক in_index = i + l.h*l.w*(k + b*l.c);
                net.delta[in_index] += l.delta[out_index] / (l.h*l.w);
            }
        }
    }
}


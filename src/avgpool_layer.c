#ଅନ୍ତର୍ଭାଵ "avgpool_layer.h"
#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>

avgpool_layer make_avgpool_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c)
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
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା output_size = l.outputs * batch;
    l.output =  calloc(output_size, ଆକାର(ଦଶମିକ));
    l.delta =   calloc(output_size, ଆକାର(ଦଶମିକ));
    l.forward = forward_avgpool_layer;
    l.backward = backward_avgpool_layer;
    #ifdef GPU
    l.forward_gpu = forward_avgpool_layer_gpu;
    l.backward_gpu = backward_avgpool_layer_gpu;
    l.output_gpu  = cuda_make_array(l.output, output_size);
    l.delta_gpu   = cuda_make_array(l.delta, output_size);
    #endif
    ଫେରନ୍ତୁ l;
}

ଶୂନ୍ୟ resize_avgpool_layer(avgpool_layer *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    l->w = w;
    l->h = h;
    l->inputs = h*w*l->c;
}

ଶୂନ୍ୟ forward_avgpool_layer(ଅଚଳ avgpool_layer l, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା b,i,k;

    ପାଇଁ(b = 0; b < l.batch; ++b){
        ପାଇଁ(k = 0; k < l.c; ++k){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_index = k + b*l.c;
            l.output[out_index] = 0;
            ପାଇଁ(i = 0; i < l.h*l.w; ++i){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା in_index = i + l.h*l.w*(k + b*l.c);
                l.output[out_index] += net.ନିବେଶ[in_index];
            }
            l.output[out_index] /= l.h*l.w;
        }
    }
}

ଶୂନ୍ୟ backward_avgpool_layer(ଅଚଳ avgpool_layer l, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା b,i,k;

    ପାଇଁ(b = 0; b < l.batch; ++b){
        ପାଇଁ(k = 0; k < l.c; ++k){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_index = k + b*l.c;
            ପାଇଁ(i = 0; i < l.h*l.w; ++i){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା in_index = i + l.h*l.w*(k + b*l.c);
                net.delta[in_index] += l.delta[out_index] / (l.h*l.w);
            }
        }
    }
}


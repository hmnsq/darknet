#अन्तर्भाव "avgpool_layer.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव <stdio.h>

avgpool_layer make_avgpool_layer(पूर्णाङ्क batch, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क c)
{
    fprintf(stderr, "avg                     %4d x%4d x%4d   ->  %4d\n",  w, h, c, c);
    avgpool_layer l = {0};
    l.type = AVGPOOL;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.बहिर्गमनम्_w = 1;
    l.बहिर्गमनम्_h = 1;
    l.बहिर्गमनम्_c = c;
    l.outputs = l.बहिर्गमनम्_c;
    l.inputs = h*w*c;
    पूर्णाङ्क output_आकार = l.outputs * batch;
    l.output =  शून्यस्मृतिं_देहि(output_आकार, परिमाण(भिन्न));
    l.delta =   शून्यस्मृतिं_देहि(output_आकार, परिमाण(भिन्न));
    l.forward = forward_avgpool_layer;
    l.backward = backward_avgpool_layer;
    #यदि_परिभाषितम् GPU
    l.forward_gpu = forward_avgpool_layer_gpu;
    l.backward_gpu = backward_avgpool_layer_gpu;
    l.output_gpu  = cuda_make_array(l.output, output_आकार);
    l.delta_gpu   = cuda_make_array(l.delta, output_आकार);
    #यदि_अन्तः
    प्रत्यागम l;
}

शून्य resize_avgpool_layer(avgpool_layer *l, पूर्णाङ्क w, पूर्णाङ्क h)
{
    l->w = w;
    l->h = h;
    l->inputs = h*w*l->c;
}

शून्य forward_avgpool_layer(ध्रुव avgpool_layer l, जालम् net)
{
    पूर्णाङ्क b,i,k;

    प्रति(b = 0; b < l.batch; ++b){
        प्रति(k = 0; k < l.c; ++k){
            पूर्णाङ्क बहिर्गमनम्_सूचकांक = k + b*l.c;
            l.output[बहिर्गमनम्_सूचकांक] = 0;
            प्रति(i = 0; i < l.h*l.w; ++i){
                पूर्णाङ्क अन्तर्_सूचकांक = i + l.h*l.w*(k + b*l.c);
                l.output[बहिर्गमनम्_सूचकांक] += net.निवेशः[अन्तर्_सूचकांक];
            }
            l.output[बहिर्गमनम्_सूचकांक] /= l.h*l.w;
        }
    }
}

शून्य backward_avgpool_layer(ध्रुव avgpool_layer l, जालम् net)
{
    पूर्णाङ्क b,i,k;

    प्रति(b = 0; b < l.batch; ++b){
        प्रति(k = 0; k < l.c; ++k){
            पूर्णाङ्क बहिर्गमनम्_सूचकांक = k + b*l.c;
            प्रति(i = 0; i < l.h*l.w; ++i){
                पूर्णाङ्क अन्तर्_सूचकांक = i + l.h*l.w*(k + b*l.c);
                net.delta[अन्तर्_सूचकांक] += l.delta[बहिर्गमनम्_सूचकांक] / (l.h*l.w);
            }
        }
    }
}


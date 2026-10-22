#شامل "avgpool_layer.h"
#شامل "cuda.h"
#شامل <stdio.h>

avgpool_layer make_avgpool_layer(صحیح batch, صحیح w, صحیح h, صحیح c)
{
    fprintf(stderr, "avg                     %4d x%4d x%4d   ->  %4d\n",  w, h, c, c);
    avgpool_layer l = {0};
    l.type = AVGPOOL;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.برآمد_w = 1;
    l.برآمد_h = 1;
    l.برآمد_c = c;
    l.outputs = l.برآمد_c;
    l.inputs = h*w*c;
    صحیح output_سائز = l.outputs * batch;
    l.output =  صفر_حافظہ_دو(output_سائز, حجم(اعشاری));
    l.delta =   صفر_حافظہ_دو(output_سائز, حجم(اعشاری));
    l.forward = forward_avgpool_layer;
    l.backward = backward_avgpool_layer;
    #اگر_تعریف_شدہ GPU
    l.forward_gpu = forward_avgpool_layer_gpu;
    l.backward_gpu = backward_avgpool_layer_gpu;
    l.output_gpu  = cuda_make_array(l.output, output_سائز);
    l.delta_gpu   = cuda_make_array(l.delta, output_سائز);
    #اگر_ختم
    واپس l;
}

خالی resize_avgpool_layer(avgpool_layer *l, صحیح w, صحیح h)
{
    l->w = w;
    l->h = h;
    l->inputs = h*w*l->c;
}

خالی forward_avgpool_layer(ثابت avgpool_layer l, جال net)
{
    صحیح b,i,k;

    برائے(b = 0; b < l.batch; ++b){
        برائے(k = 0; k < l.c; ++k){
            صحیح برآمد_اشاریہ = k + b*l.c;
            l.output[برآمد_اشاریہ] = 0;
            برائے(i = 0; i < l.h*l.w; ++i){
                صحیح اندر_اشاریہ = i + l.h*l.w*(k + b*l.c);
                l.output[برآمد_اشاریہ] += net.داخلہ[اندر_اشاریہ];
            }
            l.output[برآمد_اشاریہ] /= l.h*l.w;
        }
    }
}

خالی backward_avgpool_layer(ثابت avgpool_layer l, جال net)
{
    صحیح b,i,k;

    برائے(b = 0; b < l.batch; ++b){
        برائے(k = 0; k < l.c; ++k){
            صحیح برآمد_اشاریہ = k + b*l.c;
            برائے(i = 0; i < l.h*l.w; ++i){
                صحیح اندر_اشاریہ = i + l.h*l.w*(k + b*l.c);
                net.delta[اندر_اشاریہ] += l.delta[برآمد_اشاریہ] / (l.h*l.w);
            }
        }
    }
}


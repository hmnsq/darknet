#అన్తర్భావ "avgpool_layer.h"
#అన్తర్భావ "cuda.h"
#అన్తర్భావ <stdio.h>

avgpool_layer make_avgpool_layer(పూర్ణాంకం batch, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం c)
{
    fprintf(stderr, "avg                     %4d x%4d x%4d   ->  %4d\n",  w, h, c, c);
    avgpool_layer l = {0};
    l.type = AVGPOOL;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.బహిర్గతం_w = 1;
    l.బహిర్గతం_h = 1;
    l.బహిర్గతం_c = c;
    l.outputs = l.బహిర్గతం_c;
    l.inputs = h*w*c;
    పూర్ణాంకం output_కొలత = l.outputs * batch;
    l.output =  సున్నా_జ్ఞాపకం_ఇవ్వు(output_కొలత, పరిమాణం(దశాంశం));
    l.delta =   సున్నా_జ్ఞాపకం_ఇవ్వు(output_కొలత, పరిమాణం(దశాంశం));
    l.forward = forward_avgpool_layer;
    l.backward = backward_avgpool_layer;
    #నిర్వచితమైతే GPU
    l.forward_gpu = forward_avgpool_layer_gpu;
    l.backward_gpu = backward_avgpool_layer_gpu;
    l.output_gpu  = cuda_make_array(l.output, output_కొలత);
    l.delta_gpu   = cuda_make_array(l.delta, output_కొలత);
    #అయితే_ముగింపు
    తిరిగి l;
}

శూన్య resize_avgpool_layer(avgpool_layer *l, పూర్ణాంకం w, పూర్ణాంకం h)
{
    l->w = w;
    l->h = h;
    l->inputs = h*w*l->c;
}

శూన్య forward_avgpool_layer(స్థిరరాశి avgpool_layer l, జాలం net)
{
    పూర్ణాంకం b,i,k;

    కొరకు(b = 0; b < l.batch; ++b){
        కొరకు(k = 0; k < l.c; ++k){
            పూర్ణాంకం బహిర్గతం_సూచీ = k + b*l.c;
            l.output[బహిర్గతం_సూచీ] = 0;
            కొరకు(i = 0; i < l.h*l.w; ++i){
                పూర్ణాంకం లోపల_సూచీ = i + l.h*l.w*(k + b*l.c);
                l.output[బహిర్గతం_సూచీ] += net.ఆదానం[లోపల_సూచీ];
            }
            l.output[బహిర్గతం_సూచీ] /= l.h*l.w;
        }
    }
}

శూన్య backward_avgpool_layer(స్థిరరాశి avgpool_layer l, జాలం net)
{
    పూర్ణాంకం b,i,k;

    కొరకు(b = 0; b < l.batch; ++b){
        కొరకు(k = 0; k < l.c; ++k){
            పూర్ణాంకం బహిర్గతం_సూచీ = k + b*l.c;
            కొరకు(i = 0; i < l.h*l.w; ++i){
                పూర్ణాంకం లోపల_సూచీ = i + l.h*l.w*(k + b*l.c);
                net.delta[లోపల_సూచీ] += l.delta[బహిర్గతం_సూచీ] / (l.h*l.w);
            }
        }
    }
}


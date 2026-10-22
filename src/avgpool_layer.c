#ಅನ್ತರ್ಭಾವ "avgpool_layer.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>

avgpool_layer make_avgpool_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ c)
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
    ಪೂರ್ಣಾಙ್ಕ output_size = l.outputs * batch;
    l.output =  calloc(output_size, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.delta =   calloc(output_size, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.forward = forward_avgpool_layer;
    l.backward = backward_avgpool_layer;
    #ifdef GPU
    l.forward_gpu = forward_avgpool_layer_gpu;
    l.backward_gpu = backward_avgpool_layer_gpu;
    l.output_gpu  = cuda_make_array(l.output, output_size);
    l.delta_gpu   = cuda_make_array(l.delta, output_size);
    #endif
    ಪ್ರತ್ಯಾಗಮ l;
}

ಶೂನ್ಯ resize_avgpool_layer(avgpool_layer *l, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
    l->w = w;
    l->h = h;
    l->inputs = h*w*l->c;
}

ಶೂನ್ಯ forward_avgpool_layer(ಧ್ರುವ avgpool_layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ b,i,k;

    ಪ್ರತಿ(b = 0; b < l.batch; ++b){
        ಪ್ರತಿ(k = 0; k < l.c; ++k){
            ಪೂರ್ಣಾಙ್ಕ out_index = k + b*l.c;
            l.output[out_index] = 0;
            ಪ್ರತಿ(i = 0; i < l.h*l.w; ++i){
                ಪೂರ್ಣಾಙ್ಕ in_index = i + l.h*l.w*(k + b*l.c);
                l.output[out_index] += net.ಒಳಹರಿವು[in_index];
            }
            l.output[out_index] /= l.h*l.w;
        }
    }
}

ಶೂನ್ಯ backward_avgpool_layer(ಧ್ರುವ avgpool_layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ b,i,k;

    ಪ್ರತಿ(b = 0; b < l.batch; ++b){
        ಪ್ರತಿ(k = 0; k < l.c; ++k){
            ಪೂರ್ಣಾಙ್ಕ out_index = k + b*l.c;
            ಪ್ರತಿ(i = 0; i < l.h*l.w; ++i){
                ಪೂರ್ಣಾಙ್ಕ in_index = i + l.h*l.w*(k + b*l.c);
                net.delta[in_index] += l.delta[out_index] / (l.h*l.w);
            }
        }
    }
}


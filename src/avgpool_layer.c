#ಅನ್ತರ್ಭಾವ "avgpool_layer.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>

avgpool_layer make_avgpool_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ c)
{
    fprintf(stderr, "avg                     %4d x%4d x%4d   ->  %4d\n",  w, h, c, c);
    avgpool_layer l = {0};
    l.type = AVGPOOL;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.ಹೊರಹೋಗು_w = 1;
    l.ಹೊರಹೋಗು_h = 1;
    l.ಹೊರಹೋಗು_c = c;
    l.outputs = l.ಹೊರಹೋಗು_c;
    l.inputs = h*w*c;
    ಪೂರ್ಣಾಂಕ output_ಗಾತ್ರ = l.outputs * batch;
    l.output =  ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(output_ಗಾತ್ರ, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.delta =   ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(output_ಗಾತ್ರ, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.forward = forward_avgpool_layer;
    l.backward = backward_avgpool_layer;
    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l.forward_gpu = forward_avgpool_layer_gpu;
    l.backward_gpu = backward_avgpool_layer_gpu;
    l.output_gpu  = cuda_make_array(l.output, output_ಗಾತ್ರ);
    l.delta_gpu   = cuda_make_array(l.delta, output_ಗಾತ್ರ);
    #ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ l;
}

ಶೂನ್ಯ resize_avgpool_layer(avgpool_layer *l, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    l->w = w;
    l->h = h;
    l->inputs = h*w*l->c;
}

ಶೂನ್ಯ forward_avgpool_layer(ಸ್ಥಿರಾಂಕ avgpool_layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ b,i,k;

    ಗಾಗಿ(b = 0; b < l.batch; ++b){
        ಗಾಗಿ(k = 0; k < l.c; ++k){
            ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_ಸೂಚ್ಯಂಕ = k + b*l.c;
            l.output[ಹೊರಹೋಗು_ಸೂಚ್ಯಂಕ] = 0;
            ಗಾಗಿ(i = 0; i < l.h*l.w; ++i){
                ಪೂರ್ಣಾಂಕ ಒಳ_ಸೂಚ್ಯಂಕ = i + l.h*l.w*(k + b*l.c);
                l.output[ಹೊರಹೋಗು_ಸೂಚ್ಯಂಕ] += net.ಒಳಹರಿವು[ಒಳ_ಸೂಚ್ಯಂಕ];
            }
            l.output[ಹೊರಹೋಗು_ಸೂಚ್ಯಂಕ] /= l.h*l.w;
        }
    }
}

ಶೂನ್ಯ backward_avgpool_layer(ಸ್ಥಿರಾಂಕ avgpool_layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ b,i,k;

    ಗಾಗಿ(b = 0; b < l.batch; ++b){
        ಗಾಗಿ(k = 0; k < l.c; ++k){
            ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_ಸೂಚ್ಯಂಕ = k + b*l.c;
            ಗಾಗಿ(i = 0; i < l.h*l.w; ++i){
                ಪೂರ್ಣಾಂಕ ಒಳ_ಸೂಚ್ಯಂಕ = i + l.h*l.w*(k + b*l.c);
                net.delta[ಒಳ_ಸೂಚ್ಯಂಕ] += l.delta[ಹೊರಹೋಗು_ಸೂಚ್ಯಂಕ] / (l.h*l.w);
            }
        }
    }
}


#ಅನ್ತರ್ಭಾವ "reorg_layer.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "blas.h"

#ಅನ್ತರ್ಭಾವ <stdio.h>


layer make_reorg_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ stride, ಪೂರ್ಣಾಂಕ ಹಿಮ್ಮುಖ, ಪೂರ್ಣಾಂಕ flatten, ಪೂರ್ಣಾಂಕ extra)
{
    layer l = {0};
    l.type = REORG;
    l.batch = batch;
    l.stride = stride;
    l.extra = extra;
    l.h = h;
    l.w = w;
    l.c = c;
    l.flatten = flatten;
    ಆದರೆ(ಹಿಮ್ಮುಖ){
        l.ಹೊರಹೋಗು_w = w*stride;
        l.ಹೊರಹೋಗು_h = h*stride;
        l.ಹೊರಹೋಗು_c = c/(stride*stride);
    }ಇಲ್ಲದಿದ್ದರೆ{
        l.ಹೊರಹೋಗು_w = w/stride;
        l.ಹೊರಹೋಗು_h = h/stride;
        l.ಹೊರಹೋಗು_c = c*(stride*stride);
    }
    l.ಹಿಮ್ಮುಖ = ಹಿಮ್ಮುಖ;

    l.outputs = l.ಹೊರಹೋಗು_h * l.ಹೊರಹೋಗು_w * l.ಹೊರಹೋಗು_c;
    l.inputs = h*w*c;
    ಆದರೆ(l.extra){
        l.ಹೊರಹೋಗು_w = l.ಹೊರಹೋಗು_h = l.ಹೊರಹೋಗು_c = 0;
        l.outputs = l.inputs + l.extra;
    }

    ಆದರೆ(extra){
        fprintf(stderr, "reorg              %4d   ->  %4d\n",  l.inputs, l.outputs);
    } ಇಲ್ಲದಿದ್ದರೆ {
        fprintf(stderr, "reorg              /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n",  stride, w, h, c, l.ಹೊರಹೋಗು_w, l.ಹೊರಹೋಗು_h, l.ಹೊರಹೋಗು_c);
    }
    ಪೂರ್ಣಾಂಕ output_ಗಾತ್ರ = l.outputs * batch;
    l.output =  ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(output_ಗಾತ್ರ, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.delta =   ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(output_ಗಾತ್ರ, ಗಾತ್ರ(ದಶಮಾಂಶ));

    l.forward = forward_reorg_layer;
    l.backward = backward_reorg_layer;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l.forward_gpu = forward_reorg_layer_gpu;
    l.backward_gpu = backward_reorg_layer_gpu;

    l.output_gpu  = cuda_make_array(l.output, output_ಗಾತ್ರ);
    l.delta_gpu   = cuda_make_array(l.delta, output_ಗಾತ್ರ);
#ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ l;
}

ಶೂನ್ಯ resize_reorg_layer(layer *l, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    ಪೂರ್ಣಾಂಕ stride = l->stride;
    ಪೂರ್ಣಾಂಕ c = l->c;

    l->h = h;
    l->w = w;

    ಆದರೆ(l->ಹಿಮ್ಮುಖ){
        l->ಹೊರಹೋಗು_w = w*stride;
        l->ಹೊರಹೋಗು_h = h*stride;
        l->ಹೊರಹೋಗು_c = c/(stride*stride);
    }ಇಲ್ಲದಿದ್ದರೆ{
        l->ಹೊರಹೋಗು_w = w/stride;
        l->ಹೊರಹೋಗು_h = h/stride;
        l->ಹೊರಹೋಗು_c = c*(stride*stride);
    }

    l->outputs = l->ಹೊರಹೋಗು_h * l->ಹೊರಹೋಗು_w * l->ಹೊರಹೋಗು_c;
    l->inputs = l->outputs;
    ಪೂರ್ಣಾಂಕ output_ಗಾತ್ರ = l->outputs * l->batch;

    l->output = realloc(l->output, output_ಗಾತ್ರ * ಗಾತ್ರ(ದಶಮಾಂಶ));
    l->delta = realloc(l->delta, output_ಗಾತ್ರ * ಗಾತ್ರ(ದಶಮಾಂಶ));

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, output_ಗಾತ್ರ);
    l->delta_gpu   = cuda_make_array(l->delta,  output_ಗಾತ್ರ);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ forward_reorg_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ i;
    ಆದರೆ(l.flatten){
        ಸ್ಮರಣೆ_ನಕಲು(l.output, net.ಒಳಹರಿವು, l.outputs*l.batch*ಗಾತ್ರ(ದಶಮಾಂಶ));
        ಆದರೆ(l.ಹಿಮ್ಮುಖ){
            flatten(l.output, l.w*l.h, l.c, l.batch, 0);
        }ಇಲ್ಲದಿದ್ದರೆ{
            flatten(l.output, l.w*l.h, l.c, l.batch, 1);
        }
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (l.extra) {
        ಗಾಗಿ(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, net.ಒಳಹರಿವು + i*l.inputs, 1, l.output + i*l.outputs, 1);
        }
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (l.ಹಿಮ್ಮುಖ){
        reorg_cpu(net.ಒಳಹರಿವು, l.w, l.h, l.c, l.batch, l.stride, 1, l.output);
    } ಇಲ್ಲದಿದ್ದರೆ {
        reorg_cpu(net.ಒಳಹರಿವು, l.w, l.h, l.c, l.batch, l.stride, 0, l.output);
    }
}

ಶೂನ್ಯ backward_reorg_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ i;
    ಆದರೆ(l.flatten){
        ಸ್ಮರಣೆ_ನಕಲು(net.delta, l.delta, l.outputs*l.batch*ಗಾತ್ರ(ದಶಮಾಂಶ));
        ಆದರೆ(l.ಹಿಮ್ಮುಖ){
            flatten(net.delta, l.w*l.h, l.c, l.batch, 1);
        }ಇಲ್ಲದಿದ್ದರೆ{
            flatten(net.delta, l.w*l.h, l.c, l.batch, 0);
        }
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(l.ಹಿಮ್ಮುಖ){
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (l.extra) {
        ಗಾಗಿ(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, l.delta + i*l.outputs, 1, net.delta + i*l.inputs, 1);
        }
    }ಇಲ್ಲದಿದ್ದರೆ{
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta);
    }
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_reorg_layer_gpu(layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ i;
    ಆದರೆ(l.flatten){
        ಆದರೆ(l.ಹಿಮ್ಮುಖ){
            flatten_gpu(net.ಒಳಹರಿವು_gpu, l.w*l.h, l.c, l.batch, 0, l.output_gpu);
        }ಇಲ್ಲದಿದ್ದರೆ{
            flatten_gpu(net.ಒಳಹರಿವು_gpu, l.w*l.h, l.c, l.batch, 1, l.output_gpu);
        }
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (l.extra) {
        ಗಾಗಿ(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, net.ಒಳಹರಿವು_gpu + i*l.inputs, 1, l.output_gpu + i*l.outputs, 1);
        }
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (l.ಹಿಮ್ಮುಖ) {
        reorg_gpu(net.ಒಳಹರಿವು_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.output_gpu);
    }ಇಲ್ಲದಿದ್ದರೆ {
        reorg_gpu(net.ಒಳಹರಿವು_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.output_gpu);
    }
}

ಶೂನ್ಯ backward_reorg_layer_gpu(layer l, ಜಾಲ net)
{
    ಆದರೆ(l.flatten){
        ಆದರೆ(l.ಹಿಮ್ಮುಖ){
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 1, net.delta_gpu);
        }ಇಲ್ಲದಿದ್ದರೆ{
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 0, net.delta_gpu);
        }
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (l.extra) {
        ಪೂರ್ಣಾಂಕ i;
        ಗಾಗಿ(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, l.delta_gpu + i*l.outputs, 1, net.delta_gpu + i*l.inputs, 1);
        }
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(l.ಹಿಮ್ಮುಖ){
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta_gpu);
    } ಇಲ್ಲದಿದ್ದರೆ {
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta_gpu);
    }
}
#ಆದರೆ_ಅಂತ್ಯ

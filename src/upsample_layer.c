#ಅನ್ತರ್ಭಾವ "upsample_layer.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "blas.h"

#ಅನ್ತರ್ಭಾವ <stdio.h>

layer make_upsample_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ stride)
{
    layer l = {0};
    l.type = UPSAMPLE;
    l.batch = batch;
    l.w = w;
    l.h = h;
    l.c = c;
    l.ಹೊರಹೋಗು_w = w*stride;
    l.ಹೊರಹೋಗು_h = h*stride;
    l.ಹೊರಹೋಗು_c = c;
    ಆದರೆ(stride < 0){
        stride = -stride;
        l.ಹಿಮ್ಮುಖ=1;
        l.ಹೊರಹೋಗು_w = w/stride;
        l.ಹೊರಹೋಗು_h = h/stride;
    }
    l.stride = stride;
    l.outputs = l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h*l.ಹೊರಹೋಗು_c;
    l.inputs = l.w*l.h*l.c;
    l.delta =  ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.outputs*batch, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.output = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.outputs*batch, ಗಾತ್ರ(ದಶಮಾಂಶ));;

    l.forward = forward_upsample_layer;
    l.backward = backward_upsample_layer;
    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l.forward_gpu = forward_upsample_layer_gpu;
    l.backward_gpu = backward_upsample_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #ಆದರೆ_ಅಂತ್ಯ
    ಆದರೆ(l.ಹಿಮ್ಮುಖ) fprintf(stderr, "downsample         %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.ಹೊರಹೋಗು_w, l.ಹೊರಹೋಗು_h, l.ಹೊರಹೋಗು_c);
    ಇಲ್ಲದಿದ್ದರೆ fprintf(stderr, "upsample           %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.ಹೊರಹೋಗು_w, l.ಹೊರಹೋಗು_h, l.ಹೊರಹೋಗು_c);
    ಹಿಂತಿರುಗಿ l;
}

ಶೂನ್ಯ resize_upsample_layer(layer *l, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    l->w = w;
    l->h = h;
    l->ಹೊರಹೋಗು_w = w*l->stride;
    l->ಹೊರಹೋಗು_h = h*l->stride;
    ಆದರೆ(l->ಹಿಮ್ಮುಖ){
        l->ಹೊರಹೋಗು_w = w/l->stride;
        l->ಹೊರಹೋಗು_h = h/l->stride;
    }
    l->outputs = l->ಹೊರಹೋಗು_w*l->ಹೊರಹೋಗು_h*l->ಹೊರಹೋಗು_c;
    l->inputs = l->h*l->w*l->c;
    l->delta =  realloc(l->delta, l->outputs*l->batch*ಗಾತ್ರ(ದಶಮಾಂಶ));
    l->output = realloc(l->output, l->outputs*l->batch*ಗಾತ್ರ(ದಶಮಾಂಶ));

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#ಆದರೆ_ಅಂತ್ಯ
    
}

ಶೂನ್ಯ forward_upsample_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    ಆದರೆ(l.ಹಿಮ್ಮುಖ){
        upsample_cpu(l.output, l.ಹೊರಹೋಗು_w, l.ಹೊರಹೋಗು_h, l.c, l.batch, l.stride, 0, l.scale, net.ಒಳಹರಿವು);
    }ಇಲ್ಲದಿದ್ದರೆ{
        upsample_cpu(net.ಒಳಹರಿವು, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output);
    }
}

ಶೂನ್ಯ backward_upsample_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{
    ಆದರೆ(l.ಹಿಮ್ಮುಖ){
        upsample_cpu(l.delta, l.ಹೊರಹೋಗು_w, l.ಹೊರಹೋಗು_h, l.c, l.batch, l.stride, 1, l.scale, net.delta);
    }ಇಲ್ಲದಿದ್ದರೆ{
        upsample_cpu(net.delta, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta);
    }
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_upsample_layer_gpu(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);
    ಆದರೆ(l.ಹಿಮ್ಮುಖ){
        upsample_gpu(l.output_gpu, l.ಹೊರಹೋಗು_w, l.ಹೊರಹೋಗು_h, l.c, l.batch, l.stride, 0, l.scale, net.ಒಳಹರಿವು_gpu);
    }ಇಲ್ಲದಿದ್ದರೆ{
        upsample_gpu(net.ಒಳಹರಿವು_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output_gpu);
    }
}

ಶೂನ್ಯ backward_upsample_layer_gpu(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{
    ಆದರೆ(l.ಹಿಮ್ಮುಖ){
        upsample_gpu(l.delta_gpu, l.ಹೊರಹೋಗು_w, l.ಹೊರಹೋಗು_h, l.c, l.batch, l.stride, 1, l.scale, net.delta_gpu);
    }ಇಲ್ಲದಿದ್ದರೆ{
        upsample_gpu(net.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta_gpu);
    }
}
#ಆದರೆ_ಅಂತ್ಯ

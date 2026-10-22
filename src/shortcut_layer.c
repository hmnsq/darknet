#ಅನ್ತರ್ಭಾವ "shortcut_layer.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "activations.h"

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <assert.h>

layer make_shortcut_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ w2, ಪೂರ್ಣಾಂಕ h2, ಪೂರ್ಣಾಂಕ c2)
{
    fprintf(stderr, "res  %3d                %4d x%4d x%4d   ->  %4d x%4d x%4d\n",ಸೂಚ್ಯಂಕ, w2,h2,c2, w,h,c);
    layer l = {0};
    l.type = SHORTCUT;
    l.batch = batch;
    l.w = w2;
    l.h = h2;
    l.c = c2;
    l.ಹೊರಹೋಗು_w = w;
    l.ಹೊರಹೋಗು_h = h;
    l.ಹೊರಹೋಗು_c = c;
    l.outputs = w*h*c;
    l.inputs = l.outputs;

    l.ಸೂಚ್ಯಂಕ = ಸೂಚ್ಯಂಕ;

    l.delta =  ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.outputs*batch, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.output = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.outputs*batch, ಗಾತ್ರ(ದಶಮಾಂಶ));;

    l.forward = forward_shortcut_layer;
    l.backward = backward_shortcut_layer;
    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l.forward_gpu = forward_shortcut_layer_gpu;
    l.backward_gpu = backward_shortcut_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ l;
}

ಶೂನ್ಯ resize_shortcut_layer(layer *l, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    ಪ್ರತಿಜ್ಞಾ(l->w == l->ಹೊರಹೋಗು_w);
    ಪ್ರತಿಜ್ಞಾ(l->h == l->ಹೊರಹೋಗು_h);
    l->w = l->ಹೊರಹೋಗು_w = w;
    l->h = l->ಹೊರಹೋಗು_h = h;
    l->outputs = w*h*l->ಹೊರಹೋಗು_c;
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*ಗಾತ್ರ(ದಶಮಾಂಶ));
    l->output = realloc(l->output, l->outputs*l->batch*ಗಾತ್ರ(ದಶಮಾಂಶ));

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#ಆದರೆ_ಅಂತ್ಯ
    
}


ಶೂನ್ಯ forward_shortcut_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{
    copy_cpu(l.outputs*l.batch, net.ಒಳಹರಿವು, 1, l.output, 1);
    shortcut_cpu(l.batch, l.w, l.h, l.c, net.layers[l.ಸೂಚ್ಯಂಕ].output, l.ಹೊರಹೋಗು_w, l.ಹೊರಹೋಗು_h, l.ಹೊರಹೋಗು_c, l.alpha, l.beta, l.output);
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

ಶೂನ್ಯ backward_shortcut_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);
    axpy_cpu(l.outputs*l.batch, l.alpha, l.delta, 1, net.delta, 1);
    shortcut_cpu(l.batch, l.ಹೊರಹೋಗು_w, l.ಹೊರಹೋಗು_h, l.ಹೊರಹೋಗು_c, l.delta, l.w, l.h, l.c, 1, l.beta, net.layers[l.ಸೂಚ್ಯಂಕ].delta);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_shortcut_layer_gpu(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{
    copy_gpu(l.outputs*l.batch, net.ಒಳಹರಿವು_gpu, 1, l.output_gpu, 1);
    shortcut_gpu(l.batch, l.w, l.h, l.c, net.layers[l.ಸೂಚ್ಯಂಕ].output_gpu, l.ಹೊರಹೋಗು_w, l.ಹೊರಹೋಗು_h, l.ಹೊರಹೋಗು_c, l.alpha, l.beta, l.output_gpu);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

ಶೂನ್ಯ backward_shortcut_layer_gpu(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    axpy_gpu(l.outputs*l.batch, l.alpha, l.delta_gpu, 1, net.delta_gpu, 1);
    shortcut_gpu(l.batch, l.ಹೊರಹೋಗು_w, l.ಹೊರಹೋಗು_h, l.ಹೊರಹೋಗು_c, l.delta_gpu, l.w, l.h, l.c, 1, l.beta, net.layers[l.ಸೂಚ್ಯಂಕ].delta_gpu);
}
#ಆದರೆ_ಅಂತ್ಯ

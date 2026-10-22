#ಅನ್ತರ್ಭಾವ "reorg_layer.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "blas.h"

#ಅನ್ತರ್ಭಾವ <stdio.h>


layer make_reorg_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ stride, ಪೂರ್ಣಾಙ್ಕ reverse, ಪೂರ್ಣಾಙ್ಕ flatten, ಪೂರ್ಣಾಙ್ಕ extra)
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
    ಯದಿ(reverse){
        l.out_w = w*stride;
        l.out_h = h*stride;
        l.out_c = c/(stride*stride);
    }ಅನ್ಯಥಾ{
        l.out_w = w/stride;
        l.out_h = h/stride;
        l.out_c = c*(stride*stride);
    }
    l.reverse = reverse;

    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = h*w*c;
    ಯದಿ(l.extra){
        l.out_w = l.out_h = l.out_c = 0;
        l.outputs = l.inputs + l.extra;
    }

    ಯದಿ(extra){
        fprintf(stderr, "reorg              %4d   ->  %4d\n",  l.inputs, l.outputs);
    } ಅನ್ಯಥಾ {
        fprintf(stderr, "reorg              /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n",  stride, w, h, c, l.out_w, l.out_h, l.out_c);
    }
    ಪೂರ್ಣಾಙ್ಕ output_size = l.outputs * batch;
    l.output =  calloc(output_size, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.delta =   calloc(output_size, ಪರಿಮಾಣ(ಭಿನ್ನ));

    l.forward = forward_reorg_layer;
    l.backward = backward_reorg_layer;
#ifdef GPU
    l.forward_gpu = forward_reorg_layer_gpu;
    l.backward_gpu = backward_reorg_layer_gpu;

    l.output_gpu  = cuda_make_array(l.output, output_size);
    l.delta_gpu   = cuda_make_array(l.delta, output_size);
#endif
    ಪ್ರತ್ಯಾಗಮ l;
}

ಶೂನ್ಯ resize_reorg_layer(layer *l, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
    ಪೂರ್ಣಾಙ್ಕ stride = l->stride;
    ಪೂರ್ಣಾಙ್ಕ c = l->c;

    l->h = h;
    l->w = w;

    ಯದಿ(l->reverse){
        l->out_w = w*stride;
        l->out_h = h*stride;
        l->out_c = c/(stride*stride);
    }ಅನ್ಯಥಾ{
        l->out_w = w/stride;
        l->out_h = h/stride;
        l->out_c = c*(stride*stride);
    }

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->outputs;
    ಪೂರ್ಣಾಙ್ಕ output_size = l->outputs * l->batch;

    l->output = realloc(l->output, output_size * ಪರಿಮಾಣ(ಭಿನ್ನ));
    l->delta = realloc(l->delta, output_size * ಪರಿಮಾಣ(ಭಿನ್ನ));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, output_size);
    l->delta_gpu   = cuda_make_array(l->delta,  output_size);
#endif
}

ಶೂನ್ಯ forward_reorg_layer(ಧ್ರುವ layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಯದಿ(l.flatten){
        memcpy(l.output, net.ಒಳಹರಿವು, l.outputs*l.batch*ಪರಿಮಾಣ(ಭಿನ್ನ));
        ಯದಿ(l.reverse){
            flatten(l.output, l.w*l.h, l.c, l.batch, 0);
        }ಅನ್ಯಥಾ{
            flatten(l.output, l.w*l.h, l.c, l.batch, 1);
        }
    } ಅನ್ಯಥಾ ಯದಿ (l.extra) {
        ಪ್ರತಿ(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, net.ಒಳಹರಿವು + i*l.inputs, 1, l.output + i*l.outputs, 1);
        }
    } ಅನ್ಯಥಾ ಯದಿ (l.reverse){
        reorg_cpu(net.ಒಳಹರಿವು, l.w, l.h, l.c, l.batch, l.stride, 1, l.output);
    } ಅನ್ಯಥಾ {
        reorg_cpu(net.ಒಳಹರಿವು, l.w, l.h, l.c, l.batch, l.stride, 0, l.output);
    }
}

ಶೂನ್ಯ backward_reorg_layer(ಧ್ರುವ layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಯದಿ(l.flatten){
        memcpy(net.delta, l.delta, l.outputs*l.batch*ಪರಿಮಾಣ(ಭಿನ್ನ));
        ಯದಿ(l.reverse){
            flatten(net.delta, l.w*l.h, l.c, l.batch, 1);
        }ಅನ್ಯಥಾ{
            flatten(net.delta, l.w*l.h, l.c, l.batch, 0);
        }
    } ಅನ್ಯಥಾ ಯದಿ(l.reverse){
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta);
    } ಅನ್ಯಥಾ ಯದಿ (l.extra) {
        ಪ್ರತಿ(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, l.delta + i*l.outputs, 1, net.delta + i*l.inputs, 1);
        }
    }ಅನ್ಯಥಾ{
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta);
    }
}

#ifdef GPU
ಶೂನ್ಯ forward_reorg_layer_gpu(layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಯದಿ(l.flatten){
        ಯದಿ(l.reverse){
            flatten_gpu(net.ಒಳಹರಿವು_gpu, l.w*l.h, l.c, l.batch, 0, l.output_gpu);
        }ಅನ್ಯಥಾ{
            flatten_gpu(net.ಒಳಹರಿವು_gpu, l.w*l.h, l.c, l.batch, 1, l.output_gpu);
        }
    } ಅನ್ಯಥಾ ಯದಿ (l.extra) {
        ಪ್ರತಿ(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, net.ಒಳಹರಿವು_gpu + i*l.inputs, 1, l.output_gpu + i*l.outputs, 1);
        }
    } ಅನ್ಯಥಾ ಯದಿ (l.reverse) {
        reorg_gpu(net.ಒಳಹರಿವು_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.output_gpu);
    }ಅನ್ಯಥಾ {
        reorg_gpu(net.ಒಳಹರಿವು_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.output_gpu);
    }
}

ಶೂನ್ಯ backward_reorg_layer_gpu(layer l, network net)
{
    ಯದಿ(l.flatten){
        ಯದಿ(l.reverse){
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 1, net.delta_gpu);
        }ಅನ್ಯಥಾ{
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 0, net.delta_gpu);
        }
    } ಅನ್ಯಥಾ ಯದಿ (l.extra) {
        ಪೂರ್ಣಾಙ್ಕ i;
        ಪ್ರತಿ(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, l.delta_gpu + i*l.outputs, 1, net.delta_gpu + i*l.inputs, 1);
        }
    } ಅನ್ಯಥಾ ಯದಿ(l.reverse){
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta_gpu);
    } ಅನ್ಯಥಾ {
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta_gpu);
    }
}
#endif

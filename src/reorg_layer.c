#अन्तर्भाव "reorg_layer.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव "blas.h"

#अन्तर्भाव <stdio.h>


layer make_reorg_layer(पूर्णाङ्क batch, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क c, पूर्णाङ्क stride, पूर्णाङ्क reverse, पूर्णाङ्क flatten, पूर्णाङ्क extra)
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
    सचे(reverse){
        l.out_w = w*stride;
        l.out_h = h*stride;
        l.out_c = c/(stride*stride);
    }अञ्ञथा{
        l.out_w = w/stride;
        l.out_h = h/stride;
        l.out_c = c*(stride*stride);
    }
    l.reverse = reverse;

    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = h*w*c;
    सचे(l.extra){
        l.out_w = l.out_h = l.out_c = 0;
        l.outputs = l.inputs + l.extra;
    }

    सचे(extra){
        fprintf(stderr, "reorg              %4d   ->  %4d\n",  l.inputs, l.outputs);
    } अञ्ञथा {
        fprintf(stderr, "reorg              /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n",  stride, w, h, c, l.out_w, l.out_h, l.out_c);
    }
    पूर्णाङ्क output_size = l.outputs * batch;
    l.output =  calloc(output_size, परिमाण(भिन्न));
    l.delta =   calloc(output_size, परिमाण(भिन्न));

    l.forward = forward_reorg_layer;
    l.backward = backward_reorg_layer;
#ifdef GPU
    l.forward_gpu = forward_reorg_layer_gpu;
    l.backward_gpu = backward_reorg_layer_gpu;

    l.output_gpu  = cuda_make_array(l.output, output_size);
    l.delta_gpu   = cuda_make_array(l.delta, output_size);
#endif
    पटिनिवत्त l;
}

शून्य resize_reorg_layer(layer *l, पूर्णाङ्क w, पूर्णाङ्क h)
{
    पूर्णाङ्क stride = l->stride;
    पूर्णाङ्क c = l->c;

    l->h = h;
    l->w = w;

    सचे(l->reverse){
        l->out_w = w*stride;
        l->out_h = h*stride;
        l->out_c = c/(stride*stride);
    }अञ्ञथा{
        l->out_w = w/stride;
        l->out_h = h/stride;
        l->out_c = c*(stride*stride);
    }

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->outputs;
    पूर्णाङ्क output_size = l->outputs * l->batch;

    l->output = realloc(l->output, output_size * परिमाण(भिन्न));
    l->delta = realloc(l->delta, output_size * परिमाण(भिन्न));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, output_size);
    l->delta_gpu   = cuda_make_array(l->delta,  output_size);
#endif
}

शून्य forward_reorg_layer(ध्रुव layer l, network net)
{
    पूर्णाङ्क i;
    सचे(l.flatten){
        memcpy(l.output, net.निवेशः, l.outputs*l.batch*परिमाण(भिन्न));
        सचे(l.reverse){
            flatten(l.output, l.w*l.h, l.c, l.batch, 0);
        }अञ्ञथा{
            flatten(l.output, l.w*l.h, l.c, l.batch, 1);
        }
    } अञ्ञथा सचे (l.extra) {
        अत्थाय(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, net.निवेशः + i*l.inputs, 1, l.output + i*l.outputs, 1);
        }
    } अञ्ञथा सचे (l.reverse){
        reorg_cpu(net.निवेशः, l.w, l.h, l.c, l.batch, l.stride, 1, l.output);
    } अञ्ञथा {
        reorg_cpu(net.निवेशः, l.w, l.h, l.c, l.batch, l.stride, 0, l.output);
    }
}

शून्य backward_reorg_layer(ध्रुव layer l, network net)
{
    पूर्णाङ्क i;
    सचे(l.flatten){
        memcpy(net.delta, l.delta, l.outputs*l.batch*परिमाण(भिन्न));
        सचे(l.reverse){
            flatten(net.delta, l.w*l.h, l.c, l.batch, 1);
        }अञ्ञथा{
            flatten(net.delta, l.w*l.h, l.c, l.batch, 0);
        }
    } अञ्ञथा सचे(l.reverse){
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta);
    } अञ्ञथा सचे (l.extra) {
        अत्थाय(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, l.delta + i*l.outputs, 1, net.delta + i*l.inputs, 1);
        }
    }अञ्ञथा{
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta);
    }
}

#ifdef GPU
शून्य forward_reorg_layer_gpu(layer l, network net)
{
    पूर्णाङ्क i;
    सचे(l.flatten){
        सचे(l.reverse){
            flatten_gpu(net.निवेशः_gpu, l.w*l.h, l.c, l.batch, 0, l.output_gpu);
        }अञ्ञथा{
            flatten_gpu(net.निवेशः_gpu, l.w*l.h, l.c, l.batch, 1, l.output_gpu);
        }
    } अञ्ञथा सचे (l.extra) {
        अत्थाय(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, net.निवेशः_gpu + i*l.inputs, 1, l.output_gpu + i*l.outputs, 1);
        }
    } अञ्ञथा सचे (l.reverse) {
        reorg_gpu(net.निवेशः_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.output_gpu);
    }अञ्ञथा {
        reorg_gpu(net.निवेशः_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.output_gpu);
    }
}

शून्य backward_reorg_layer_gpu(layer l, network net)
{
    सचे(l.flatten){
        सचे(l.reverse){
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 1, net.delta_gpu);
        }अञ्ञथा{
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 0, net.delta_gpu);
        }
    } अञ्ञथा सचे (l.extra) {
        पूर्णाङ्क i;
        अत्थाय(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, l.delta_gpu + i*l.outputs, 1, net.delta_gpu + i*l.inputs, 1);
        }
    } अञ्ञथा सचे(l.reverse){
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta_gpu);
    } अञ्ञथा {
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta_gpu);
    }
}
#endif

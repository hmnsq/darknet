#ଅନ୍ତର୍ଭାଵ "maxpool_layer.h"
#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>

image get_maxpool_image(maxpool_layer l)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା h = l.out_h;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା w = l.out_w;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = l.c;
    ଫେରନ୍ତୁ float_to_image(w,h,c,l.output);
}

image get_maxpool_delta(maxpool_layer l)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା h = l.out_h;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା w = l.out_w;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = l.c;
    ଫେରନ୍ତୁ float_to_image(w,h,c,l.delta);
}

maxpool_layer make_maxpool_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ପୂର୍ଣ୍ଣସଂଖ୍ୟା padding)
{
    maxpool_layer l = {0};
    l.type = MAXPOOL;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.pad = padding;
    l.out_w = (w + padding - size)/stride + 1;
    l.out_h = (h + padding - size)/stride + 1;
    l.out_c = c;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = h*w*c;
    l.size = size;
    l.stride = stride;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା output_size = l.out_h * l.out_w * l.out_c * batch;
    l.indexes = calloc(output_size, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
    l.output =  calloc(output_size, ଆକାର(ଦଶମିକ));
    l.delta =   calloc(output_size, ଆକାର(ଦଶମିକ));
    l.forward = forward_maxpool_layer;
    l.backward = backward_maxpool_layer;
    #ifdef GPU
    l.forward_gpu = forward_maxpool_layer_gpu;
    l.backward_gpu = backward_maxpool_layer_gpu;
    l.indexes_gpu = cuda_make_int_array(0, output_size);
    l.output_gpu  = cuda_make_array(l.output, output_size);
    l.delta_gpu   = cuda_make_array(l.delta, output_size);
    #endif
    fprintf(stderr, "max          %d x %d / %d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", size, size, stride, w, h, c, l.out_w, l.out_h, l.out_c);
    ଫେରନ୍ତୁ l;
}

ଶୂନ୍ୟ resize_maxpool_layer(maxpool_layer *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    l->h = h;
    l->w = w;
    l->inputs = h*w*l->c;

    l->out_w = (w + l->pad - l->size)/l->stride + 1;
    l->out_h = (h + l->pad - l->size)/l->stride + 1;
    l->outputs = l->out_w * l->out_h * l->c;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା output_size = l->outputs * l->batch;

    l->indexes = realloc(l->indexes, output_size * ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
    l->output = realloc(l->output, output_size * ଆକାର(ଦଶମିକ));
    l->delta = realloc(l->delta, output_size * ଆକାର(ଦଶମିକ));

    #ifdef GPU
    cuda_free((ଦଶମିକ *)l->indexes_gpu);
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->indexes_gpu = cuda_make_int_array(0, output_size);
    l->output_gpu  = cuda_make_array(l->output, output_size);
    l->delta_gpu   = cuda_make_array(l->delta,  output_size);
    #endif
}

ଶୂନ୍ୟ forward_maxpool_layer(ଅଚଳ maxpool_layer l, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା b,i,j,k,m,n;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା w_offset = -l.pad/2;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା h_offset = -l.pad/2;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା h = l.out_h;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା w = l.out_w;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = l.c;

    ପାଇଁ(b = 0; b < l.batch; ++b){
        ପାଇଁ(k = 0; k < c; ++k){
            ପାଇଁ(i = 0; i < h; ++i){
                ପାଇଁ(j = 0; j < w; ++j){
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_index = j + w*(i + h*(k + c*b));
                    ଦଶମିକ max = -FLT_MAX;
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_i = -1;
                    ପାଇଁ(n = 0; n < l.size; ++n){
                        ପାଇଁ(m = 0; m < l.size; ++m){
                            ପୂର୍ଣ୍ଣସଂଖ୍ୟା cur_h = h_offset + i*l.stride + n;
                            ପୂର୍ଣ୍ଣସଂଖ୍ୟା cur_w = w_offset + j*l.stride + m;
                            ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = cur_w + l.w*(cur_h + l.h*(k + b*l.c));
                            ପୂର୍ଣ୍ଣସଂଖ୍ୟା valid = (cur_h >= 0 && cur_h < l.h &&
                                         cur_w >= 0 && cur_w < l.w);
                            ଦଶମିକ val = (valid != 0) ? net.ନିବେଶ[index] : -FLT_MAX;
                            max_i = (val > max) ? index : max_i;
                            max   = (val > max) ? val   : max;
                        }
                    }
                    l.output[out_index] = max;
                    l.indexes[out_index] = max_i;
                }
            }
        }
    }
}

ଶୂନ୍ୟ backward_maxpool_layer(ଅଚଳ maxpool_layer l, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା h = l.out_h;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା w = l.out_w;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = l.c;
    ପାଇଁ(i = 0; i < h*w*c*l.batch; ++i){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = l.indexes[i];
        net.delta[index] += l.delta[i];
    }
}


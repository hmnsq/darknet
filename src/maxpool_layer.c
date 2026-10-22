#अन्तर्भाव "maxpool_layer.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव <stdio.h>

image get_maxpool_image(maxpool_layer l)
{
    पूर्णांक h = l.out_h;
    पूर्णांक w = l.out_w;
    पूर्णांक c = l.c;
    परत float_to_image(w,h,c,l.output);
}

image get_maxpool_delta(maxpool_layer l)
{
    पूर्णांक h = l.out_h;
    पूर्णांक w = l.out_w;
    पूर्णांक c = l.c;
    परत float_to_image(w,h,c,l.delta);
}

maxpool_layer make_maxpool_layer(पूर्णांक batch, पूर्णांक h, पूर्णांक w, पूर्णांक c, पूर्णांक size, पूर्णांक stride, पूर्णांक padding)
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
    पूर्णांक output_size = l.out_h * l.out_w * l.out_c * batch;
    l.indexes = सुस्मृति(output_size, आकार(पूर्णांक));
    l.output =  सुस्मृति(output_size, आकार(दशांश));
    l.delta =   सुस्मृति(output_size, आकार(दशांश));
    l.forward = forward_maxpool_layer;
    l.backward = backward_maxpool_layer;
    #ifdef GPU
    l.forward_gpu = forward_maxpool_layer_gpu;
    l.backward_gpu = backward_maxpool_layer_gpu;
    l.indexes_gpu = cuda_make_int_array(0, output_size);
    l.output_gpu  = cuda_make_array(l.output, output_size);
    l.delta_gpu   = cuda_make_array(l.delta, output_size);
    #endif
    ख_लिखो(मानक_त्रुटि, "max          %d x %d / %d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", size, size, stride, w, h, c, l.out_w, l.out_h, l.out_c);
    परत l;
}

शून्य resize_maxpool_layer(maxpool_layer *l, पूर्णांक w, पूर्णांक h)
{
    l->h = h;
    l->w = w;
    l->inputs = h*w*l->c;

    l->out_w = (w + l->pad - l->size)/l->stride + 1;
    l->out_h = (h + l->pad - l->size)/l->stride + 1;
    l->outputs = l->out_w * l->out_h * l->c;
    पूर्णांक output_size = l->outputs * l->batch;

    l->indexes = पुनः_स्मृति(l->indexes, output_size * आकार(पूर्णांक));
    l->output = पुनः_स्मृति(l->output, output_size * आकार(दशांश));
    l->delta = पुनः_स्मृति(l->delta, output_size * आकार(दशांश));

    #ifdef GPU
    cuda_free((दशांश *)l->indexes_gpu);
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->indexes_gpu = cuda_make_int_array(0, output_size);
    l->output_gpu  = cuda_make_array(l->output, output_size);
    l->delta_gpu   = cuda_make_array(l->delta,  output_size);
    #endif
}

शून्य forward_maxpool_layer(स्थिरांक maxpool_layer l, network net)
{
    पूर्णांक b,i,j,k,m,n;
    पूर्णांक w_offset = -l.pad/2;
    पूर्णांक h_offset = -l.pad/2;

    पूर्णांक h = l.out_h;
    पूर्णांक w = l.out_w;
    पूर्णांक c = l.c;

    साठी(b = 0; b < l.batch; ++b){
        साठी(k = 0; k < c; ++k){
            साठी(i = 0; i < h; ++i){
                साठी(j = 0; j < w; ++j){
                    पूर्णांक out_index = j + w*(i + h*(k + c*b));
                    दशांश max = -भग्न_उच्च;
                    पूर्णांक max_i = -1;
                    साठी(n = 0; n < l.size; ++n){
                        साठी(m = 0; m < l.size; ++m){
                            पूर्णांक cur_h = h_offset + i*l.stride + n;
                            पूर्णांक cur_w = w_offset + j*l.stride + m;
                            पूर्णांक index = cur_w + l.w*(cur_h + l.h*(k + b*l.c));
                            पूर्णांक valid = (cur_h >= 0 && cur_h < l.h &&
                                         cur_w >= 0 && cur_w < l.w);
                            दशांश val = (valid != 0) ? net.आदान[index] : -भग्न_उच्च;
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

शून्य backward_maxpool_layer(स्थिरांक maxpool_layer l, network net)
{
    पूर्णांक i;
    पूर्णांक h = l.out_h;
    पूर्णांक w = l.out_w;
    पूर्णांक c = l.c;
    साठी(i = 0; i < h*w*c*l.batch; ++i){
        पूर्णांक index = l.indexes[i];
        net.delta[index] += l.delta[i];
    }
}


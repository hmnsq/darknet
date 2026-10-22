#समावेश "maxpool_layer.h"
#समावेश "cuda.h"
#समावेश <stdio.h>

image get_maxpool_image(maxpool_layer l)
{
    पूर्णांक h = l.out_h;
    पूर्णांक w = l.out_w;
    पूर्णांक c = l.c;
    वापस float_to_image(w,h,c,l.output);
}

image get_maxpool_delta(maxpool_layer l)
{
    पूर्णांक h = l.out_h;
    पूर्णांक w = l.out_w;
    पूर्णांक c = l.c;
    वापस float_to_image(w,h,c,l.delta);
}

maxpool_layer make_maxpool_layer(पूर्णांक batch, पूर्णांक h, पूर्णांक w, पूर्णांक c, पूर्णांक आकार, पूर्णांक stride, पूर्णांक padding)
{
    maxpool_layer l = {0};
    l.type = MAXPOOL;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.pad = padding;
    l.out_w = (w + padding - आकार)/stride + 1;
    l.out_h = (h + padding - आकार)/stride + 1;
    l.out_c = c;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = h*w*c;
    l.आकार = आकार;
    l.stride = stride;
    पूर्णांक output_आकार = l.out_h * l.out_w * l.out_c * batch;
    l.indexes = सुस्मृति(output_आकार, माप(पूर्णांक));
    l.output =  सुस्मृति(output_आकार, माप(भग्न));
    l.delta =   सुस्मृति(output_आकार, माप(भग्न));
    l.forward = forward_maxpool_layer;
    l.backward = backward_maxpool_layer;
    #अगर_घोषित GPU
    l.forward_gpu = forward_maxpool_layer_gpu;
    l.backward_gpu = backward_maxpool_layer_gpu;
    l.indexes_gpu = cuda_make_int_array(0, output_आकार);
    l.output_gpu  = cuda_make_array(l.output, output_आकार);
    l.delta_gpu   = cuda_make_array(l.delta, output_आकार);
    #पूर्ण_अगर
    ख_लिखो(मानक_त्रुटि, "max          %d x %d / %d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", आकार, आकार, stride, w, h, c, l.out_w, l.out_h, l.out_c);
    वापस l;
}

व्योम resize_maxpool_layer(maxpool_layer *l, पूर्णांक w, पूर्णांक h)
{
    l->h = h;
    l->w = w;
    l->inputs = h*w*l->c;

    l->out_w = (w + l->pad - l->आकार)/l->stride + 1;
    l->out_h = (h + l->pad - l->आकार)/l->stride + 1;
    l->outputs = l->out_w * l->out_h * l->c;
    पूर्णांक output_आकार = l->outputs * l->batch;

    l->indexes = पुनः_स्मृति(l->indexes, output_आकार * माप(पूर्णांक));
    l->output = पुनः_स्मृति(l->output, output_आकार * माप(भग्न));
    l->delta = पुनः_स्मृति(l->delta, output_आकार * माप(भग्न));

    #अगर_घोषित GPU
    cuda_free((भग्न *)l->indexes_gpu);
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->indexes_gpu = cuda_make_int_array(0, output_आकार);
    l->output_gpu  = cuda_make_array(l->output, output_आकार);
    l->delta_gpu   = cuda_make_array(l->delta,  output_आकार);
    #पूर्ण_अगर
}

व्योम forward_maxpool_layer(स्थिर maxpool_layer l, network net)
{
    पूर्णांक b,i,j,k,m,n;
    पूर्णांक w_offset = -l.pad/2;
    पूर्णांक h_offset = -l.pad/2;

    पूर्णांक h = l.out_h;
    पूर्णांक w = l.out_w;
    पूर्णांक c = l.c;

    क्रम(b = 0; b < l.batch; ++b){
        क्रम(k = 0; k < c; ++k){
            क्रम(i = 0; i < h; ++i){
                क्रम(j = 0; j < w; ++j){
                    पूर्णांक out_सूचकांक = j + w*(i + h*(k + c*b));
                    भग्न max = -भग्न_उच्च;
                    पूर्णांक max_i = -1;
                    क्रम(n = 0; n < l.आकार; ++n){
                        क्रम(m = 0; m < l.आकार; ++m){
                            पूर्णांक cur_h = h_offset + i*l.stride + n;
                            पूर्णांक cur_w = w_offset + j*l.stride + m;
                            पूर्णांक सूचकांक = cur_w + l.w*(cur_h + l.h*(k + b*l.c));
                            पूर्णांक valid = (cur_h >= 0 && cur_h < l.h &&
                                         cur_w >= 0 && cur_w < l.w);
                            भग्न val = (valid != 0) ? net.निवेश[सूचकांक] : -भग्न_उच्च;
                            max_i = (val > max) ? सूचकांक : max_i;
                            max   = (val > max) ? val   : max;
                        }
                    }
                    l.output[out_सूचकांक] = max;
                    l.indexes[out_सूचकांक] = max_i;
                }
            }
        }
    }
}

व्योम backward_maxpool_layer(स्थिर maxpool_layer l, network net)
{
    पूर्णांक i;
    पूर्णांक h = l.out_h;
    पूर्णांक w = l.out_w;
    पूर्णांक c = l.c;
    क्रम(i = 0; i < h*w*c*l.batch; ++i){
        पूर्णांक सूचकांक = l.indexes[i];
        net.delta[सूचकांक] += l.delta[i];
    }
}


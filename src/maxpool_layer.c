#अन्तर्भाव "maxpool_layer.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव <stdio.h>

image get_maxpool_image(maxpool_layer l)
{
    पूर्णाङ्क h = l.बहिर्गमनम्_h;
    पूर्णाङ्क w = l.बहिर्गमनम्_w;
    पूर्णाङ्क c = l.c;
    प्रत्यागम float_गन्तव्यम्_image(w,h,c,l.output);
}

image get_maxpool_delta(maxpool_layer l)
{
    पूर्णाङ्क h = l.बहिर्गमनम्_h;
    पूर्णाङ्क w = l.बहिर्गमनम्_w;
    पूर्णाङ्क c = l.c;
    प्रत्यागम float_गन्तव्यम्_image(w,h,c,l.delta);
}

maxpool_layer make_maxpool_layer(पूर्णाङ्क batch, पूर्णाङ्क h, पूर्णाङ्क w, पूर्णाङ्क c, पूर्णाङ्क आकार, पूर्णाङ्क stride, पूर्णाङ्क padding)
{
    maxpool_layer l = {0};
    l.type = MAXPOOL;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.pad = padding;
    l.बहिर्गमनम्_w = (w + padding - आकार)/stride + 1;
    l.बहिर्गमनम्_h = (h + padding - आकार)/stride + 1;
    l.बहिर्गमनम्_c = c;
    l.outputs = l.बहिर्गमनम्_h * l.बहिर्गमनम्_w * l.बहिर्गमनम्_c;
    l.inputs = h*w*c;
    l.आकार = आकार;
    l.stride = stride;
    पूर्णाङ्क output_आकार = l.बहिर्गमनम्_h * l.बहिर्गमनम्_w * l.बहिर्गमनम्_c * batch;
    l.indexes = शून्यस्मृतिं_देहि(output_आकार, परिमाण(पूर्णाङ्क));
    l.output =  शून्यस्मृतिं_देहि(output_आकार, परिमाण(भिन्न));
    l.delta =   शून्यस्मृतिं_देहि(output_आकार, परिमाण(भिन्न));
    l.forward = forward_maxpool_layer;
    l.backward = backward_maxpool_layer;
    #यदि_परिभाषितम् GPU
    l.forward_gpu = forward_maxpool_layer_gpu;
    l.backward_gpu = backward_maxpool_layer_gpu;
    l.indexes_gpu = cuda_make_int_array(0, output_आकार);
    l.output_gpu  = cuda_make_array(l.output, output_आकार);
    l.delta_gpu   = cuda_make_array(l.delta, output_आकार);
    #यदि_अन्तः
    fprintf(stderr, "max          %d x %d / %d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", आकार, आकार, stride, w, h, c, l.बहिर्गमनम्_w, l.बहिर्गमनम्_h, l.बहिर्गमनम्_c);
    प्रत्यागम l;
}

शून्य resize_maxpool_layer(maxpool_layer *l, पूर्णाङ्क w, पूर्णाङ्क h)
{
    l->h = h;
    l->w = w;
    l->inputs = h*w*l->c;

    l->बहिर्गमनम्_w = (w + l->pad - l->आकार)/l->stride + 1;
    l->बहिर्गमनम्_h = (h + l->pad - l->आकार)/l->stride + 1;
    l->outputs = l->बहिर्गमनम्_w * l->बहिर्गमनम्_h * l->c;
    पूर्णाङ्क output_आकार = l->outputs * l->batch;

    l->indexes = realloc(l->indexes, output_आकार * परिमाण(पूर्णाङ्क));
    l->output = realloc(l->output, output_आकार * परिमाण(भिन्न));
    l->delta = realloc(l->delta, output_आकार * परिमाण(भिन्न));

    #यदि_परिभाषितम् GPU
    cuda_free((भिन्न *)l->indexes_gpu);
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->indexes_gpu = cuda_make_int_array(0, output_आकार);
    l->output_gpu  = cuda_make_array(l->output, output_आकार);
    l->delta_gpu   = cuda_make_array(l->delta,  output_आकार);
    #यदि_अन्तः
}

शून्य forward_maxpool_layer(ध्रुव maxpool_layer l, जालम् net)
{
    पूर्णाङ्क b,i,j,k,m,n;
    पूर्णाङ्क w_विस्थापनम् = -l.pad/2;
    पूर्णाङ्क h_विस्थापनम् = -l.pad/2;

    पूर्णाङ्क h = l.बहिर्गमनम्_h;
    पूर्णाङ्क w = l.बहिर्गमनम्_w;
    पूर्णाङ्क c = l.c;

    प्रति(b = 0; b < l.batch; ++b){
        प्रति(k = 0; k < c; ++k){
            प्रति(i = 0; i < h; ++i){
                प्रति(j = 0; j < w; ++j){
                    पूर्णाङ्क बहिर्गमनम्_सूचकांक = j + w*(i + h*(k + c*b));
                    भिन्न max = -FLT_MAX;
                    पूर्णाङ्क max_i = -1;
                    प्रति(n = 0; n < l.आकार; ++n){
                        प्रति(m = 0; m < l.आकार; ++m){
                            पूर्णाङ्क cur_h = h_विस्थापनम् + i*l.stride + n;
                            पूर्णाङ्क cur_w = w_विस्थापनम् + j*l.stride + m;
                            पूर्णाङ्क सूचकांक = cur_w + l.w*(cur_h + l.h*(k + b*l.c));
                            पूर्णाङ्क valid = (cur_h >= 0 && cur_h < l.h &&
                                         cur_w >= 0 && cur_w < l.w);
                            भिन्न val = (valid != 0) ? net.निवेशः[सूचकांक] : -FLT_MAX;
                            max_i = (val > max) ? सूचकांक : max_i;
                            max   = (val > max) ? val   : max;
                        }
                    }
                    l.output[बहिर्गमनम्_सूचकांक] = max;
                    l.indexes[बहिर्गमनम्_सूचकांक] = max_i;
                }
            }
        }
    }
}

शून्य backward_maxpool_layer(ध्रुव maxpool_layer l, जालम् net)
{
    पूर्णाङ्क i;
    पूर्णाङ्क h = l.बहिर्गमनम्_h;
    पूर्णाङ्क w = l.बहिर्गमनम्_w;
    पूर्णाङ्क c = l.c;
    प्रति(i = 0; i < h*w*c*l.batch; ++i){
        पूर्णाङ्क सूचकांक = l.indexes[i];
        net.delta[सूचकांक] += l.delta[i];
    }
}


#ضمن "maxpool_layer.h"
#ضمن "cuda.h"
#ضمن <stdio.h>

image get_maxpool_image(maxpool_layer l)
{
    صحيح h = l.ٻاهريون_h;
    صحيح w = l.ٻاهريون_w;
    صحيح c = l.c;
    أرجع float_منزل_image(w,h,c,l.output);
}

image get_maxpool_delta(maxpool_layer l)
{
    صحيح h = l.ٻاهريون_h;
    صحيح w = l.ٻاهريون_w;
    صحيح c = l.c;
    أرجع float_منزل_image(w,h,c,l.delta);
}

maxpool_layer make_maxpool_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح سائيز, صحيح stride, صحيح padding)
{
    maxpool_layer l = {0};
    l.type = MAXPOOL;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.pad = padding;
    l.ٻاهريون_w = (w + padding - سائيز)/stride + 1;
    l.ٻاهريون_h = (h + padding - سائيز)/stride + 1;
    l.ٻاهريون_c = c;
    l.outputs = l.ٻاهريون_h * l.ٻاهريون_w * l.ٻاهريون_c;
    l.inputs = h*w*c;
    l.سائيز = سائيز;
    l.stride = stride;
    صحيح output_سائيز = l.ٻاهريون_h * l.ٻاهريون_w * l.ٻاهريون_c * batch;
    l.indexes = صفر_يادگيري_ڏيو(output_سائيز, حجم(صحيح));
    l.output =  صفر_يادگيري_ڏيو(output_سائيز, حجم(عائم));
    l.delta =   صفر_يادگيري_ڏيو(output_سائيز, حجم(عائم));
    l.forward = forward_maxpool_layer;
    l.backward = backward_maxpool_layer;
    #جيڪڏهن_وصف_ٿيل GPU
    l.forward_gpu = forward_maxpool_layer_gpu;
    l.backward_gpu = backward_maxpool_layer_gpu;
    l.indexes_gpu = cuda_make_int_array(0, output_سائيز);
    l.output_gpu  = cuda_make_array(l.output, output_سائيز);
    l.delta_gpu   = cuda_make_array(l.delta, output_سائيز);
    #جيڪڏهن_پورو
    fprintf(stderr, "max          %d x %d / %d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", سائيز, سائيز, stride, w, h, c, l.ٻاهريون_w, l.ٻاهريون_h, l.ٻاهريون_c);
    أرجع l;
}

فراغ resize_maxpool_layer(maxpool_layer *l, صحيح w, صحيح h)
{
    l->h = h;
    l->w = w;
    l->inputs = h*w*l->c;

    l->ٻاهريون_w = (w + l->pad - l->سائيز)/l->stride + 1;
    l->ٻاهريون_h = (h + l->pad - l->سائيز)/l->stride + 1;
    l->outputs = l->ٻاهريون_w * l->ٻاهريون_h * l->c;
    صحيح output_سائيز = l->outputs * l->batch;

    l->indexes = realloc(l->indexes, output_سائيز * حجم(صحيح));
    l->output = realloc(l->output, output_سائيز * حجم(عائم));
    l->delta = realloc(l->delta, output_سائيز * حجم(عائم));

    #جيڪڏهن_وصف_ٿيل GPU
    cuda_free((عائم *)l->indexes_gpu);
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->indexes_gpu = cuda_make_int_array(0, output_سائيز);
    l->output_gpu  = cuda_make_array(l->output, output_سائيز);
    l->delta_gpu   = cuda_make_array(l->delta,  output_سائيز);
    #جيڪڏهن_پورو
}

فراغ forward_maxpool_layer(ثابت maxpool_layer l, ڄار net)
{
    صحيح b,i,j,k,m,n;
    صحيح w_وٿي = -l.pad/2;
    صحيح h_وٿي = -l.pad/2;

    صحيح h = l.ٻاهريون_h;
    صحيح w = l.ٻاهريون_w;
    صحيح c = l.c;

    لكل(b = 0; b < l.batch; ++b){
        لكل(k = 0; k < c; ++k){
            لكل(i = 0; i < h; ++i){
                لكل(j = 0; j < w; ++j){
                    صحيح ٻاهريون_اشاريو = j + w*(i + h*(k + c*b));
                    عائم max = -FLT_MAX;
                    صحيح max_i = -1;
                    لكل(n = 0; n < l.سائيز; ++n){
                        لكل(m = 0; m < l.سائيز; ++m){
                            صحيح cur_h = h_وٿي + i*l.stride + n;
                            صحيح cur_w = w_وٿي + j*l.stride + m;
                            صحيح اشاريو = cur_w + l.w*(cur_h + l.h*(k + b*l.c));
                            صحيح valid = (cur_h >= 0 && cur_h < l.h &&
                                         cur_w >= 0 && cur_w < l.w);
                            عائم val = (valid != 0) ? net.داخلا[اشاريو] : -FLT_MAX;
                            max_i = (val > max) ? اشاريو : max_i;
                            max   = (val > max) ? val   : max;
                        }
                    }
                    l.output[ٻاهريون_اشاريو] = max;
                    l.indexes[ٻاهريون_اشاريو] = max_i;
                }
            }
        }
    }
}

فراغ backward_maxpool_layer(ثابت maxpool_layer l, ڄار net)
{
    صحيح i;
    صحيح h = l.ٻاهريون_h;
    صحيح w = l.ٻاهريون_w;
    صحيح c = l.c;
    لكل(i = 0; i < h*w*c*l.batch; ++i){
        صحيح اشاريو = l.indexes[i];
        net.delta[اشاريو] += l.delta[i];
    }
}


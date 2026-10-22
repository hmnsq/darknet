#ضمن "maxpool_layer.h"
#ضمن "cuda.h"
#ضمن <stdio.h>

image get_maxpool_image(maxpool_layer l)
{
    صحيح h = l.باہرلا_h;
    صحيح w = l.باہرلا_w;
    صحيح c = l.c;
    أرجع float_منزل_image(w,h,c,l.output);
}

image get_maxpool_delta(maxpool_layer l)
{
    صحيح h = l.باہرلا_h;
    صحيح w = l.باہرلا_w;
    صحيح c = l.c;
    أرجع float_منزل_image(w,h,c,l.delta);
}

maxpool_layer make_maxpool_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح ناپ, صحيح stride, صحيح padding)
{
    maxpool_layer l = {0};
    l.type = MAXPOOL;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.pad = padding;
    l.باہرلا_w = (w + padding - ناپ)/stride + 1;
    l.باہرلا_h = (h + padding - ناپ)/stride + 1;
    l.باہرلا_c = c;
    l.outputs = l.باہرلا_h * l.باہرلا_w * l.باہرلا_c;
    l.inputs = h*w*c;
    l.ناپ = ناپ;
    l.stride = stride;
    صحيح output_ناپ = l.باہرلا_h * l.باہرلا_w * l.باہرلا_c * batch;
    l.indexes = صفر_حافظہ_دیو(output_ناپ, حجم(صحيح));
    l.output =  صفر_حافظہ_دیو(output_ناپ, حجم(عائم));
    l.delta =   صفر_حافظہ_دیو(output_ناپ, حجم(عائم));
    l.forward = forward_maxpool_layer;
    l.backward = backward_maxpool_layer;
    #جے_تعریف_شدہ GPU
    l.forward_gpu = forward_maxpool_layer_gpu;
    l.backward_gpu = backward_maxpool_layer_gpu;
    l.indexes_gpu = cuda_make_int_array(0, output_ناپ);
    l.output_gpu  = cuda_make_array(l.output, output_ناپ);
    l.delta_gpu   = cuda_make_array(l.delta, output_ناپ);
    #جے_انت
    fprintf(stderr, "max          %d x %d / %d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", ناپ, ناپ, stride, w, h, c, l.باہرلا_w, l.باہرلا_h, l.باہرلا_c);
    أرجع l;
}

فراغ resize_maxpool_layer(maxpool_layer *l, صحيح w, صحيح h)
{
    l->h = h;
    l->w = w;
    l->inputs = h*w*l->c;

    l->باہرلا_w = (w + l->pad - l->ناپ)/l->stride + 1;
    l->باہرلا_h = (h + l->pad - l->ناپ)/l->stride + 1;
    l->outputs = l->باہرلا_w * l->باہرلا_h * l->c;
    صحيح output_ناپ = l->outputs * l->batch;

    l->indexes = realloc(l->indexes, output_ناپ * حجم(صحيح));
    l->output = realloc(l->output, output_ناپ * حجم(عائم));
    l->delta = realloc(l->delta, output_ناپ * حجم(عائم));

    #جے_تعریف_شدہ GPU
    cuda_free((عائم *)l->indexes_gpu);
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->indexes_gpu = cuda_make_int_array(0, output_ناپ);
    l->output_gpu  = cuda_make_array(l->output, output_ناپ);
    l->delta_gpu   = cuda_make_array(l->delta,  output_ناپ);
    #جے_انت
}

فراغ forward_maxpool_layer(ثابت maxpool_layer l, جال net)
{
    صحيح b,i,j,k,m,n;
    صحيح w_وتھ = -l.pad/2;
    صحيح h_وتھ = -l.pad/2;

    صحيح h = l.باہرلا_h;
    صحيح w = l.باہرلا_w;
    صحيح c = l.c;

    لكل(b = 0; b < l.batch; ++b){
        لكل(k = 0; k < c; ++k){
            لكل(i = 0; i < h; ++i){
                لكل(j = 0; j < w; ++j){
                    صحيح باہرلا_اشاریہ = j + w*(i + h*(k + c*b));
                    عائم max = -FLT_MAX;
                    صحيح max_i = -1;
                    لكل(n = 0; n < l.ناپ; ++n){
                        لكل(m = 0; m < l.ناپ; ++m){
                            صحيح cur_h = h_وتھ + i*l.stride + n;
                            صحيح cur_w = w_وتھ + j*l.stride + m;
                            صحيح اشاریہ = cur_w + l.w*(cur_h + l.h*(k + b*l.c));
                            صحيح valid = (cur_h >= 0 && cur_h < l.h &&
                                         cur_w >= 0 && cur_w < l.w);
                            عائم val = (valid != 0) ? net.داخلہ[اشاریہ] : -FLT_MAX;
                            max_i = (val > max) ? اشاریہ : max_i;
                            max   = (val > max) ? val   : max;
                        }
                    }
                    l.output[باہرلا_اشاریہ] = max;
                    l.indexes[باہرلا_اشاریہ] = max_i;
                }
            }
        }
    }
}

فراغ backward_maxpool_layer(ثابت maxpool_layer l, جال net)
{
    صحيح i;
    صحيح h = l.باہرلا_h;
    صحيح w = l.باہرلا_w;
    صحيح c = l.c;
    لكل(i = 0; i < h*w*c*l.batch; ++i){
        صحيح اشاریہ = l.indexes[i];
        net.delta[اشاریہ] += l.delta[i];
    }
}


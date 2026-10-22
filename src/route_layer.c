#ضمن "route_layer.h"
#ضمن "cuda.h"
#ضمن "blas.h"

#ضمن <stdio.h>

route_layer make_route_layer(صحيح batch, صحيح n, صحيح *داخلہ_layers, صحيح *داخلہ_sizes)
{
    fprintf(stderr,"route ");
    route_layer l = {0};
    l.type = ROUTE;
    l.batch = batch;
    l.n = n;
    l.داخلہ_layers = داخلہ_layers;
    l.داخلہ_sizes = داخلہ_sizes;
    صحيح i;
    صحيح outputs = 0;
    لكل(i = 0; i < n; ++i){
        fprintf(stderr," %d", داخلہ_layers[i]);
        outputs += داخلہ_sizes[i];
    }
    fprintf(stderr, "\n");
    l.outputs = outputs;
    l.inputs = outputs;
    l.delta =  صفر_حافظہ_دیو(outputs*batch, حجم(عائم));
    l.output = صفر_حافظہ_دیو(outputs*batch, حجم(عائم));;

    l.forward = forward_route_layer;
    l.backward = backward_route_layer;
    #جے_تعریف_شدہ GPU
    l.forward_gpu = forward_route_layer_gpu;
    l.backward_gpu = backward_route_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, outputs*batch);
    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    #جے_انت
    أرجع l;
}

فراغ resize_route_layer(route_layer *l, جال *net)
{
    صحيح i;
    layer پہلا = net->layers[l->داخلہ_layers[0]];
    l->باہرلا_w = پہلا.باہرلا_w;
    l->باہرلا_h = پہلا.باہرلا_h;
    l->باہرلا_c = پہلا.باہرلا_c;
    l->outputs = پہلا.outputs;
    l->داخلہ_sizes[0] = پہلا.outputs;
    لكل(i = 1; i < l->n; ++i){
        صحيح اشاریہ = l->داخلہ_layers[i];
        layer اگلا = net->layers[اشاریہ];
        l->outputs += اگلا.outputs;
        l->داخلہ_sizes[i] = اگلا.outputs;
        إذا(اگلا.باہرلا_w == پہلا.باہرلا_w && اگلا.باہرلا_h == پہلا.باہرلا_h){
            l->باہرلا_c += اگلا.باہرلا_c;
        }وإلا{
            لکھو("%d %d, %d %d\n", اگلا.باہرلا_w, اگلا.باہرلا_h, پہلا.باہرلا_w, پہلا.باہرلا_h);
            l->باہرلا_h = l->باہرلا_w = l->باہرلا_c = 0;
        }
    }
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*حجم(عائم));
    l->output = realloc(l->output, l->outputs*l->batch*حجم(عائم));

#جے_تعریف_شدہ GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#جے_انت
    
}

فراغ forward_route_layer(ثابت route_layer l, جال net)
{
    صحيح i, j;
    صحيح وتھ = 0;
    لكل(i = 0; i < l.n; ++i){
        صحيح اشاریہ = l.داخلہ_layers[i];
        عائم *داخلہ = net.layers[اشاریہ].output;
        صحيح داخلہ_ناپ = l.داخلہ_sizes[i];
        لكل(j = 0; j < l.batch; ++j){
            copy_cpu(داخلہ_ناپ, داخلہ + j*داخلہ_ناپ, 1, l.output + وتھ + j*l.outputs, 1);
        }
        وتھ += داخلہ_ناپ;
    }
}

فراغ backward_route_layer(ثابت route_layer l, جال net)
{
    صحيح i, j;
    صحيح وتھ = 0;
    لكل(i = 0; i < l.n; ++i){
        صحيح اشاریہ = l.داخلہ_layers[i];
        عائم *delta = net.layers[اشاریہ].delta;
        صحيح داخلہ_ناپ = l.داخلہ_sizes[i];
        لكل(j = 0; j < l.batch; ++j){
            axpy_cpu(داخلہ_ناپ, 1, l.delta + وتھ + j*l.outputs, 1, delta + j*داخلہ_ناپ, 1);
        }
        وتھ += داخلہ_ناپ;
    }
}

#جے_تعریف_شدہ GPU
فراغ forward_route_layer_gpu(ثابت route_layer l, جال net)
{
    صحيح i, j;
    صحيح وتھ = 0;
    لكل(i = 0; i < l.n; ++i){
        صحيح اشاریہ = l.داخلہ_layers[i];
        عائم *داخلہ = net.layers[اشاریہ].output_gpu;
        صحيح داخلہ_ناپ = l.داخلہ_sizes[i];
        لكل(j = 0; j < l.batch; ++j){
            copy_gpu(داخلہ_ناپ, داخلہ + j*داخلہ_ناپ, 1, l.output_gpu + وتھ + j*l.outputs, 1);
        }
        وتھ += داخلہ_ناپ;
    }
}

فراغ backward_route_layer_gpu(ثابت route_layer l, جال net)
{
    صحيح i, j;
    صحيح وتھ = 0;
    لكل(i = 0; i < l.n; ++i){
        صحيح اشاریہ = l.داخلہ_layers[i];
        عائم *delta = net.layers[اشاریہ].delta_gpu;
        صحيح داخلہ_ناپ = l.داخلہ_sizes[i];
        لكل(j = 0; j < l.batch; ++j){
            axpy_gpu(داخلہ_ناپ, 1, l.delta_gpu + وتھ + j*l.outputs, 1, delta + j*داخلہ_ناپ, 1);
        }
        وتھ += داخلہ_ناپ;
    }
}
#جے_انت

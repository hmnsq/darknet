#شامل "route_layer.h"
#شامل "cuda.h"
#شامل "blas.h"

#شامل <stdio.h>

route_layer make_route_layer(صحیح batch, صحیح n, صحیح *داخلہ_layers, صحیح *داخلہ_sizes)
{
    fprintf(stderr,"route ");
    route_layer l = {0};
    l.type = ROUTE;
    l.batch = batch;
    l.n = n;
    l.داخلہ_layers = داخلہ_layers;
    l.داخلہ_sizes = داخلہ_sizes;
    صحیح i;
    صحیح outputs = 0;
    برائے(i = 0; i < n; ++i){
        fprintf(stderr," %d", داخلہ_layers[i]);
        outputs += داخلہ_sizes[i];
    }
    fprintf(stderr, "\n");
    l.outputs = outputs;
    l.inputs = outputs;
    l.delta =  صفر_حافظہ_دو(outputs*batch, حجم(اعشاری));
    l.output = صفر_حافظہ_دو(outputs*batch, حجم(اعشاری));;

    l.forward = forward_route_layer;
    l.backward = backward_route_layer;
    #اگر_تعریف_شدہ GPU
    l.forward_gpu = forward_route_layer_gpu;
    l.backward_gpu = backward_route_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, outputs*batch);
    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    #اگر_ختم
    واپس l;
}

خالی resize_route_layer(route_layer *l, جال *net)
{
    صحیح i;
    layer پہلا = net->layers[l->داخلہ_layers[0]];
    l->برآمد_w = پہلا.برآمد_w;
    l->برآمد_h = پہلا.برآمد_h;
    l->برآمد_c = پہلا.برآمد_c;
    l->outputs = پہلا.outputs;
    l->داخلہ_sizes[0] = پہلا.outputs;
    برائے(i = 1; i < l->n; ++i){
        صحیح اشاریہ = l->داخلہ_layers[i];
        layer اگلا = net->layers[اشاریہ];
        l->outputs += اگلا.outputs;
        l->داخلہ_sizes[i] = اگلا.outputs;
        اگر(اگلا.برآمد_w == پہلا.برآمد_w && اگلا.برآمد_h == پہلا.برآمد_h){
            l->برآمد_c += اگلا.برآمد_c;
        }ورنہ{
            لکھو("%d %d, %d %d\n", اگلا.برآمد_w, اگلا.برآمد_h, پہلا.برآمد_w, پہلا.برآمد_h);
            l->برآمد_h = l->برآمد_w = l->برآمد_c = 0;
        }
    }
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*حجم(اعشاری));
    l->output = realloc(l->output, l->outputs*l->batch*حجم(اعشاری));

#اگر_تعریف_شدہ GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#اگر_ختم
    
}

خالی forward_route_layer(ثابت route_layer l, جال net)
{
    صحیح i, j;
    صحیح انحراف = 0;
    برائے(i = 0; i < l.n; ++i){
        صحیح اشاریہ = l.داخلہ_layers[i];
        اعشاری *داخلہ = net.layers[اشاریہ].output;
        صحیح داخلہ_سائز = l.داخلہ_sizes[i];
        برائے(j = 0; j < l.batch; ++j){
            copy_cpu(داخلہ_سائز, داخلہ + j*داخلہ_سائز, 1, l.output + انحراف + j*l.outputs, 1);
        }
        انحراف += داخلہ_سائز;
    }
}

خالی backward_route_layer(ثابت route_layer l, جال net)
{
    صحیح i, j;
    صحیح انحراف = 0;
    برائے(i = 0; i < l.n; ++i){
        صحیح اشاریہ = l.داخلہ_layers[i];
        اعشاری *delta = net.layers[اشاریہ].delta;
        صحیح داخلہ_سائز = l.داخلہ_sizes[i];
        برائے(j = 0; j < l.batch; ++j){
            axpy_cpu(داخلہ_سائز, 1, l.delta + انحراف + j*l.outputs, 1, delta + j*داخلہ_سائز, 1);
        }
        انحراف += داخلہ_سائز;
    }
}

#اگر_تعریف_شدہ GPU
خالی forward_route_layer_gpu(ثابت route_layer l, جال net)
{
    صحیح i, j;
    صحیح انحراف = 0;
    برائے(i = 0; i < l.n; ++i){
        صحیح اشاریہ = l.داخلہ_layers[i];
        اعشاری *داخلہ = net.layers[اشاریہ].output_gpu;
        صحیح داخلہ_سائز = l.داخلہ_sizes[i];
        برائے(j = 0; j < l.batch; ++j){
            copy_gpu(داخلہ_سائز, داخلہ + j*داخلہ_سائز, 1, l.output_gpu + انحراف + j*l.outputs, 1);
        }
        انحراف += داخلہ_سائز;
    }
}

خالی backward_route_layer_gpu(ثابت route_layer l, جال net)
{
    صحیح i, j;
    صحیح انحراف = 0;
    برائے(i = 0; i < l.n; ++i){
        صحیح اشاریہ = l.داخلہ_layers[i];
        اعشاری *delta = net.layers[اشاریہ].delta_gpu;
        صحیح داخلہ_سائز = l.داخلہ_sizes[i];
        برائے(j = 0; j < l.batch; ++j){
            axpy_gpu(داخلہ_سائز, 1, l.delta_gpu + انحراف + j*l.outputs, 1, delta + j*داخلہ_سائز, 1);
        }
        انحراف += داخلہ_سائز;
    }
}
#اگر_ختم

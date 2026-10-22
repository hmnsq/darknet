#ضمن "route_layer.h"
#ضمن "cuda.h"
#ضمن "blas.h"

#ضمن <stdio.h>

route_layer make_route_layer(صحيح batch, صحيح n, صحيح *داخلا_layers, صحيح *داخلا_sizes)
{
    fprintf(stderr,"route ");
    route_layer l = {0};
    l.type = ROUTE;
    l.batch = batch;
    l.n = n;
    l.داخلا_layers = داخلا_layers;
    l.داخلا_sizes = داخلا_sizes;
    صحيح i;
    صحيح outputs = 0;
    لكل(i = 0; i < n; ++i){
        fprintf(stderr," %d", داخلا_layers[i]);
        outputs += داخلا_sizes[i];
    }
    fprintf(stderr, "\n");
    l.outputs = outputs;
    l.inputs = outputs;
    l.delta =  صفر_يادگيري_ڏيو(outputs*batch, حجم(عائم));
    l.output = صفر_يادگيري_ڏيو(outputs*batch, حجم(عائم));;

    l.forward = forward_route_layer;
    l.backward = backward_route_layer;
    #جيڪڏهن_وصف_ٿيل GPU
    l.forward_gpu = forward_route_layer_gpu;
    l.backward_gpu = backward_route_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, outputs*batch);
    l.output_gpu = cuda_make_array(l.output, outputs*batch);
    #جيڪڏهن_پورو
    أرجع l;
}

فراغ resize_route_layer(route_layer *l, ڄار *net)
{
    صحيح i;
    layer پهريون = net->layers[l->داخلا_layers[0]];
    l->ٻاهريون_w = پهريون.ٻاهريون_w;
    l->ٻاهريون_h = پهريون.ٻاهريون_h;
    l->ٻاهريون_c = پهريون.ٻاهريون_c;
    l->outputs = پهريون.outputs;
    l->داخلا_sizes[0] = پهريون.outputs;
    لكل(i = 1; i < l->n; ++i){
        صحيح اشاريو = l->داخلا_layers[i];
        layer ايندڙ = net->layers[اشاريو];
        l->outputs += ايندڙ.outputs;
        l->داخلا_sizes[i] = ايندڙ.outputs;
        إذا(ايندڙ.ٻاهريون_w == پهريون.ٻاهريون_w && ايندڙ.ٻاهريون_h == پهريون.ٻاهريون_h){
            l->ٻاهريون_c += ايندڙ.ٻاهريون_c;
        }وإلا{
            لکو("%d %d, %d %d\n", ايندڙ.ٻاهريون_w, ايندڙ.ٻاهريون_h, پهريون.ٻاهريون_w, پهريون.ٻاهريون_h);
            l->ٻاهريون_h = l->ٻاهريون_w = l->ٻاهريون_c = 0;
        }
    }
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*حجم(عائم));
    l->output = realloc(l->output, l->outputs*l->batch*حجم(عائم));

#جيڪڏهن_وصف_ٿيل GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#جيڪڏهن_پورو
    
}

فراغ forward_route_layer(ثابت route_layer l, ڄار net)
{
    صحيح i, j;
    صحيح وٿي = 0;
    لكل(i = 0; i < l.n; ++i){
        صحيح اشاريو = l.داخلا_layers[i];
        عائم *داخلا = net.layers[اشاريو].output;
        صحيح داخلا_سائيز = l.داخلا_sizes[i];
        لكل(j = 0; j < l.batch; ++j){
            copy_cpu(داخلا_سائيز, داخلا + j*داخلا_سائيز, 1, l.output + وٿي + j*l.outputs, 1);
        }
        وٿي += داخلا_سائيز;
    }
}

فراغ backward_route_layer(ثابت route_layer l, ڄار net)
{
    صحيح i, j;
    صحيح وٿي = 0;
    لكل(i = 0; i < l.n; ++i){
        صحيح اشاريو = l.داخلا_layers[i];
        عائم *delta = net.layers[اشاريو].delta;
        صحيح داخلا_سائيز = l.داخلا_sizes[i];
        لكل(j = 0; j < l.batch; ++j){
            axpy_cpu(داخلا_سائيز, 1, l.delta + وٿي + j*l.outputs, 1, delta + j*داخلا_سائيز, 1);
        }
        وٿي += داخلا_سائيز;
    }
}

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_route_layer_gpu(ثابت route_layer l, ڄار net)
{
    صحيح i, j;
    صحيح وٿي = 0;
    لكل(i = 0; i < l.n; ++i){
        صحيح اشاريو = l.داخلا_layers[i];
        عائم *داخلا = net.layers[اشاريو].output_gpu;
        صحيح داخلا_سائيز = l.داخلا_sizes[i];
        لكل(j = 0; j < l.batch; ++j){
            copy_gpu(داخلا_سائيز, داخلا + j*داخلا_سائيز, 1, l.output_gpu + وٿي + j*l.outputs, 1);
        }
        وٿي += داخلا_سائيز;
    }
}

فراغ backward_route_layer_gpu(ثابت route_layer l, ڄار net)
{
    صحيح i, j;
    صحيح وٿي = 0;
    لكل(i = 0; i < l.n; ++i){
        صحيح اشاريو = l.داخلا_layers[i];
        عائم *delta = net.layers[اشاريو].delta_gpu;
        صحيح داخلا_سائيز = l.داخلا_sizes[i];
        لكل(j = 0; j < l.batch; ++j){
            axpy_gpu(داخلا_سائيز, 1, l.delta_gpu + وٿي + j*l.outputs, 1, delta + j*داخلا_سائيز, 1);
        }
        وٿي += داخلا_سائيز;
    }
}
#جيڪڏهن_پورو

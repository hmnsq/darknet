#شامل "cost_layer.h"
#شامل "utils.h"
#شامل "cuda.h"
#شامل "blas.h"
#شامل <math.h>
#شامل <string.h>
#شامل <stdlib.h>
#شامل <stdio.h>

لاگت_TYPE get_لاگت_type(حرف *s)
{
    اگر (strcmp(s, "seg")==0) واپس SEG;
    اگر (strcmp(s, "sse")==0) واپس SSE;
    اگر (strcmp(s, "masked")==0) واپس MASKED;
    اگر (strcmp(s, "smooth")==0) واپس SMOOTH;
    اگر (strcmp(s, "L1")==0) واپس L1;
    اگر (strcmp(s, "wgan")==0) واپس WGAN;
    fprintf(stderr, "Couldn't find cost type %s, going with SSE\n", s);
    واپس SSE;
}

حرف *get_لاگت_string(لاگت_TYPE a)
{
    انتخاب(a){
        حالت SEG:
            واپس "seg";
        حالت SSE:
            واپس "sse";
        حالت MASKED:
            واپس "masked";
        حالت SMOOTH:
            واپس "smooth";
        حالت L1:
            واپس "L1";
        حالت WGAN:
            واپس "wgan";
    }
    واپس "sse";
}

لاگت_layer make_لاگت_layer(صحیح batch, صحیح inputs, لاگت_TYPE لاگت_type, اعشاری scale)
{
    fprintf(stderr, "cost                                           %4d\n",  inputs);
    لاگت_layer l = {0};
    l.type = لاگت;

    l.scale = scale;
    l.batch = batch;
    l.inputs = inputs;
    l.outputs = inputs;
    l.لاگت_type = لاگت_type;
    l.delta = صفر_حافظہ_دو(inputs*batch, حجم(اعشاری));
    l.output = صفر_حافظہ_دو(inputs*batch, حجم(اعشاری));
    l.لاگت_2 = صفر_حافظہ_دو(1, حجم(اعشاری));

    l.forward = forward_لاگت_layer;
    l.backward = backward_لاگت_layer;
    #اگر_تعریف_شدہ GPU
    l.forward_gpu = forward_لاگت_layer_gpu;
    l.backward_gpu = backward_لاگت_layer_gpu;

    l.delta_gpu = cuda_make_array(l.output, inputs*batch);
    l.output_gpu = cuda_make_array(l.delta, inputs*batch);
    #اگر_ختم
    واپس l;
}

خالی resize_لاگت_layer(لاگت_layer *l, صحیح inputs)
{
    l->inputs = inputs;
    l->outputs = inputs;
    l->delta = realloc(l->delta, inputs*l->batch*حجم(اعشاری));
    l->output = realloc(l->output, inputs*l->batch*حجم(اعشاری));
#اگر_تعریف_شدہ GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);
    l->delta_gpu = cuda_make_array(l->delta, inputs*l->batch);
    l->output_gpu = cuda_make_array(l->output, inputs*l->batch);
#اگر_ختم
}

خالی forward_لاگت_layer(لاگت_layer l, جال net)
{
    اگر (!net.truth) واپس;
    اگر(l.لاگت_type == MASKED){
        صحیح i;
        برائے(i = 0; i < l.batch*l.inputs; ++i){
            اگر(net.truth[i] == SECRET_عدد) net.داخلہ[i] = SECRET_عدد;
        }
    }
    اگر(l.لاگت_type == SMOOTH){
        smooth_l1_cpu(l.batch*l.inputs, net.داخلہ, net.truth, l.delta, l.output);
    }ورنہ اگر(l.لاگت_type == L1){
        l1_cpu(l.batch*l.inputs, net.داخلہ, net.truth, l.delta, l.output);
    } ورنہ {
        l2_cpu(l.batch*l.inputs, net.داخلہ, net.truth, l.delta, l.output);
    }
    l.لاگت_2[0] = مجموع_array(l.output, l.batch*l.inputs);
}

خالی backward_لاگت_layer(ثابت لاگت_layer l, جال net)
{
    axpy_cpu(l.batch*l.inputs, l.scale, l.delta, 1, net.delta, 1);
}

#اگر_تعریف_شدہ GPU

خالی pull_لاگت_layer(لاگت_layer l)
{
    cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

خالی ڈالو_لاگت_layer(لاگت_layer l)
{
    cuda_ڈالو_array(l.delta_gpu, l.delta, l.batch*l.inputs);
}

صحیح float_abs_compare (ثابت خالی * a, ثابت خالی * b)
{
    اعشاری fa = *(ثابت اعشاری*) a;
    اگر(fa < 0) fa = -fa;
    اعشاری fb = *(ثابت اعشاری*) b;
    اگر(fb < 0) fb = -fb;
    واپس (fa > fb) - (fa < fb);
}

خالی forward_لاگت_layer_gpu(لاگت_layer l, جال net)
{
    اگر (!net.truth) واپس;
    اگر(l.smooth){
        scal_gpu(l.batch*l.inputs, (1-l.smooth), net.truth_gpu, 1);
        جوڑو_gpu(l.batch*l.inputs, l.smooth * 1./l.inputs, net.truth_gpu, 1);
    }

    اگر(l.لاگت_type == SMOOTH){
        smooth_l1_gpu(l.batch*l.inputs, net.داخلہ_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } ورنہ اگر (l.لاگت_type == L1){
        l1_gpu(l.batch*l.inputs, net.داخلہ_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } ورنہ اگر (l.لاگت_type == WGAN){
        wgan_gpu(l.batch*l.inputs, net.داخلہ_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    } ورنہ {
        l2_gpu(l.batch*l.inputs, net.داخلہ_gpu, net.truth_gpu, l.delta_gpu, l.output_gpu);
    }

    اگر (l.لاگت_type == SEG && l.noobject_scale != 1) {
        scale_نقاب_gpu(l.batch*l.inputs, l.delta_gpu, 0, net.truth_gpu, l.noobject_scale);
        scale_نقاب_gpu(l.batch*l.inputs, l.output_gpu, 0, net.truth_gpu, l.noobject_scale);
    }
    اگر (l.لاگت_type == MASKED) {
        نقاب_gpu(l.batch*l.inputs, net.delta_gpu, SECRET_عدد, net.truth_gpu, 0);
    }

    اگر(l.تناسب){
        cuda_pull_array(l.delta_gpu, l.delta, l.batch*l.inputs);
        qsort(l.delta, l.batch*l.inputs, حجم(اعشاری), float_abs_compare);
        صحیح n = (1-l.تناسب) * l.batch*l.inputs;
        اعشاری thresh = l.delta[n];
        thresh = 0;
        لکھو("%f\n", thresh);
        supp_gpu(l.batch*l.inputs, thresh, l.delta_gpu, 1);
    }

    اگر(l.thresh){
        supp_gpu(l.batch*l.inputs, l.thresh*1./l.inputs, l.delta_gpu, 1);
    }

    cuda_pull_array(l.output_gpu, l.output, l.batch*l.inputs);
    l.لاگت_2[0] = مجموع_array(l.output, l.batch*l.inputs);
}

خالی backward_لاگت_layer_gpu(ثابت لاگت_layer l, جال net)
{
    axpy_gpu(l.batch*l.inputs, l.scale, l.delta_gpu, 1, net.delta_gpu, 1);
}
#اگر_ختم


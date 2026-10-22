#ضمن "lstm_layer.h"
#ضمن "connected_layer.h"
#ضمن "utils.h"
#ضمن "cuda.h"
#ضمن "blas.h"
#ضمن "gemm.h"

#ضمن <math.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>

ساكن فراغ increment_layer(layer *l, صحيح ګامونه)
{
    صحيح عدد = l->outputs*l->batch*ګامونه;
    l->output += عدد;
    l->delta += عدد;
    l->x += عدد;
    l->x_norm += عدد;

#که_تعریف_شوی GPU
    l->output_gpu += عدد;
    l->delta_gpu += عدد;
    l->x_gpu += عدد;
    l->x_norm_gpu += عدد;
#د_که_پای
}

layer make_lstm_layer(صحيح batch, صحيح inputs, صحيح outputs, صحيح ګامونه, صحيح batch_normalize, صحيح adam)
{
    fprintf(stderr, "LSTM Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / ګامونه;
    layer l = { 0 };
    l.batch = batch;
    l.type = LSTM;
    l.ګامونه = ګامونه;
    l.inputs = inputs;

    l.uf = حافظه_ورکړه(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.uf) = make_connected_layer(batch*ګامونه, inputs, outputs, خطي, batch_normalize, adam);
    l.uf->batch = batch;

    l.ui = حافظه_ورکړه(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.ui) = make_connected_layer(batch*ګامونه, inputs, outputs, خطي, batch_normalize, adam);
    l.ui->batch = batch;

    l.ug = حافظه_ورکړه(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.ug) = make_connected_layer(batch*ګامونه, inputs, outputs, خطي, batch_normalize, adam);
    l.ug->batch = batch;

    l.uo = حافظه_ورکړه(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.uo) = make_connected_layer(batch*ګامونه, inputs, outputs, خطي, batch_normalize, adam);
    l.uo->batch = batch;

    l.wf = حافظه_ورکړه(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.wf) = make_connected_layer(batch*ګامونه, outputs, outputs, خطي, batch_normalize, adam);
    l.wf->batch = batch;

    l.wi = حافظه_ورکړه(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.wi) = make_connected_layer(batch*ګامونه, outputs, outputs, خطي, batch_normalize, adam);
    l.wi->batch = batch;

    l.wg = حافظه_ورکړه(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.wg) = make_connected_layer(batch*ګامونه, outputs, outputs, خطي, batch_normalize, adam);
    l.wg->batch = batch;

    l.wo = حافظه_ورکړه(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.wo) = make_connected_layer(batch*ګامونه, outputs, outputs, خطي, batch_normalize, adam);
    l.wo->batch = batch;

    l.batch_normalize = batch_normalize;
    l.outputs = outputs;

    l.output = حافظه_صفر_ورکړه(outputs*batch*ګامونه, حجم(عائم));
    l.state = حافظه_صفر_ورکړه(outputs*batch, حجم(عائم));

    l.forward = forward_lstm_layer;
    l.update = update_lstm_layer;

    l.prev_state_cpu =  حافظه_صفر_ورکړه(batch*outputs, حجم(عائم));
    l.prev_cell_cpu =   حافظه_صفر_ورکړه(batch*outputs, حجم(عائم));
    l.cell_cpu =        حافظه_صفر_ورکړه(batch*outputs*ګامونه, حجم(عائم));

    l.f_cpu =           حافظه_صفر_ورکړه(batch*outputs, حجم(عائم));
    l.i_cpu =           حافظه_صفر_ورکړه(batch*outputs, حجم(عائم));
    l.g_cpu =           حافظه_صفر_ورکړه(batch*outputs, حجم(عائم));
    l.o_cpu =           حافظه_صفر_ورکړه(batch*outputs, حجم(عائم));
    l.c_cpu =           حافظه_صفر_ورکړه(batch*outputs, حجم(عائم));
    l.h_cpu =           حافظه_صفر_ورکړه(batch*outputs, حجم(عائم));
    l.لنډمهاله_cpu =        حافظه_صفر_ورکړه(batch*outputs, حجم(عائم));
    l.temp2_cpu =       حافظه_صفر_ورکړه(batch*outputs, حجم(عائم));
    l.temp3_cpu =       حافظه_صفر_ورکړه(batch*outputs, حجم(عائم));
    l.dc_cpu =          حافظه_صفر_ورکړه(batch*outputs, حجم(عائم));
    l.dh_cpu =          حافظه_صفر_ورکړه(batch*outputs, حجم(عائم));

#که_تعریف_شوی GPU
    l.forward_gpu = forward_lstm_layer_gpu;
    l.backward_gpu = backward_lstm_layer_gpu;
    l.update_gpu = update_lstm_layer_gpu;

    l.output_gpu = cuda_make_array(0, batch*outputs*ګامونه);
    l.delta_gpu = cuda_make_array(0, batch*l.outputs*ګامونه);

    l.prev_state_gpu = cuda_make_array(0, batch*outputs);
    l.prev_cell_gpu = cuda_make_array(0, batch*outputs);
    l.cell_gpu = cuda_make_array(0, batch*outputs*ګامونه);

    l.f_gpu = cuda_make_array(0, batch*outputs);
    l.i_gpu = cuda_make_array(0, batch*outputs);
    l.g_gpu = cuda_make_array(0, batch*outputs);
    l.o_gpu = cuda_make_array(0, batch*outputs);
    l.c_gpu = cuda_make_array(0, batch*outputs);
    l.h_gpu = cuda_make_array(0, batch*outputs);
    l.لنډمهاله_gpu =  cuda_make_array(0, batch*outputs);
    l.temp2_gpu = cuda_make_array(0, batch*outputs);
    l.temp3_gpu = cuda_make_array(0, batch*outputs);
    l.dc_gpu = cuda_make_array(0, batch*outputs);
    l.dh_gpu = cuda_make_array(0, batch*outputs);
#که_تعریف_شوی CUDNN
        cudnn_سېټ_Tensor4d_Descriptor(l.wf->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.wf->بهر_c, l.wf->بهر_h, l.wf->بهر_w); 
        cudnn_سېټ_Tensor4d_Descriptor(l.wi->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.wi->بهر_c, l.wi->بهر_h, l.wi->بهر_w); 
        cudnn_سېټ_Tensor4d_Descriptor(l.wg->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.wg->بهر_c, l.wg->بهر_h, l.wg->بهر_w); 
        cudnn_سېټ_Tensor4d_Descriptor(l.wo->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.wo->بهر_c, l.wo->بهر_h, l.wo->بهر_w); 

        cudnn_سېټ_Tensor4d_Descriptor(l.uf->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.uf->بهر_c, l.uf->بهر_h, l.uf->بهر_w); 
        cudnn_سېټ_Tensor4d_Descriptor(l.ui->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.ui->بهر_c, l.ui->بهر_h, l.ui->بهر_w); 
        cudnn_سېټ_Tensor4d_Descriptor(l.ug->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.ug->بهر_c, l.ug->بهر_h, l.ug->بهر_w); 
        cudnn_سېټ_Tensor4d_Descriptor(l.uo->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, batch, l.uo->بهر_c, l.uo->بهر_h, l.uo->بهر_w); 
#د_که_پای

#د_که_پای

    أرجع l;
}

فراغ update_lstm_layer(layer l, update_دلیلونه a)
{
    update_connected_layer(*(l.wf), a);
    update_connected_layer(*(l.wi), a);
    update_connected_layer(*(l.wg), a);
    update_connected_layer(*(l.wo), a);
    update_connected_layer(*(l.uf), a);
    update_connected_layer(*(l.ui), a);
    update_connected_layer(*(l.ug), a);
    update_connected_layer(*(l.uo), a);
}

فراغ forward_lstm_layer(layer l, شبکه state)
{
    شبکه s = { 0 };
    s.train = state.train;
    صحيح i;
    layer wf = *(l.wf);
    layer wi = *(l.wi);
    layer wg = *(l.wg);
    layer wo = *(l.wo);

    layer uf = *(l.uf);
    layer ui = *(l.ui);
    layer ug = *(l.ug);
    layer uo = *(l.uo);

    fill_cpu(l.outputs * l.batch * l.ګامونه, 0, wf.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ګامونه, 0, wi.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ګامونه, 0, wg.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ګامونه, 0, wo.delta, 1);

    fill_cpu(l.outputs * l.batch * l.ګامونه, 0, uf.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ګامونه, 0, ui.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ګامونه, 0, ug.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ګامونه, 0, uo.delta, 1);
    إذا (state.train) {
        fill_cpu(l.outputs * l.batch * l.ګامونه, 0, l.delta, 1);
    }

    لكل (i = 0; i < l.ګامونه; ++i) {
        s.ننوت = l.h_cpu;
        forward_connected_layer(wf, s);							
        forward_connected_layer(wi, s);							
        forward_connected_layer(wg, s);							
        forward_connected_layer(wo, s);							

        s.ننوت = state.ننوت;
        forward_connected_layer(uf, s);							
        forward_connected_layer(ui, s);							
        forward_connected_layer(ug, s);							
        forward_connected_layer(uo, s);							

        copy_cpu(l.outputs*l.batch, wf.output, 1, l.f_cpu, 1);
        axpy_cpu(l.outputs*l.batch, 1, uf.output, 1, l.f_cpu, 1);

        copy_cpu(l.outputs*l.batch, wi.output, 1, l.i_cpu, 1);	
        axpy_cpu(l.outputs*l.batch, 1, ui.output, 1, l.i_cpu, 1);	

        copy_cpu(l.outputs*l.batch, wg.output, 1, l.g_cpu, 1);	
        axpy_cpu(l.outputs*l.batch, 1, ug.output, 1, l.g_cpu, 1);	

        copy_cpu(l.outputs*l.batch, wo.output, 1, l.o_cpu, 1);	
        axpy_cpu(l.outputs*l.batch, 1, uo.output, 1, l.o_cpu, 1);	

        activate_array(l.f_cpu, l.outputs*l.batch, LOGISTIC);		
        activate_array(l.i_cpu, l.outputs*l.batch, LOGISTIC);		
        activate_array(l.g_cpu, l.outputs*l.batch, TANH);			
        activate_array(l.o_cpu, l.outputs*l.batch, LOGISTIC);		

        copy_cpu(l.outputs*l.batch, l.i_cpu, 1, l.لنډمهاله_cpu, 1);		
        mul_cpu(l.outputs*l.batch, l.g_cpu, 1, l.لنډمهاله_cpu, 1);		
        mul_cpu(l.outputs*l.batch, l.f_cpu, 1, l.c_cpu, 1);			
        axpy_cpu(l.outputs*l.batch, 1, l.لنډمهاله_cpu, 1, l.c_cpu, 1);	

        copy_cpu(l.outputs*l.batch, l.c_cpu, 1, l.h_cpu, 1);			
        activate_array(l.h_cpu, l.outputs*l.batch, TANH);		
        mul_cpu(l.outputs*l.batch, l.o_cpu, 1, l.h_cpu, 1);	

        copy_cpu(l.outputs*l.batch, l.c_cpu, 1, l.cell_cpu, 1);		
        copy_cpu(l.outputs*l.batch, l.h_cpu, 1, l.output, 1);

        state.ننوت += l.inputs*l.batch;
        l.output    += l.outputs*l.batch;
        l.cell_cpu      += l.outputs*l.batch;

        increment_layer(&wf, 1);
        increment_layer(&wi, 1);
        increment_layer(&wg, 1);
        increment_layer(&wo, 1);

        increment_layer(&uf, 1);
        increment_layer(&ui, 1);
        increment_layer(&ug, 1);
        increment_layer(&uo, 1);
    }
}

فراغ backward_lstm_layer(layer l, شبکه state)
{
    شبکه s = { 0 };
    s.train = state.train;
    صحيح i;
    layer wf = *(l.wf);
    layer wi = *(l.wi);
    layer wg = *(l.wg);
    layer wo = *(l.wo);

    layer uf = *(l.uf);
    layer ui = *(l.ui);
    layer ug = *(l.ug);
    layer uo = *(l.uo);

    increment_layer(&wf, l.ګامونه - 1);
    increment_layer(&wi, l.ګامونه - 1);
    increment_layer(&wg, l.ګامونه - 1);
    increment_layer(&wo, l.ګامونه - 1);

    increment_layer(&uf, l.ګامونه - 1);
    increment_layer(&ui, l.ګامونه - 1);
    increment_layer(&ug, l.ګامونه - 1);
    increment_layer(&uo, l.ګامونه - 1);

    state.ننوت += l.inputs*l.batch*(l.ګامونه - 1);
    إذا (state.delta) state.delta += l.inputs*l.batch*(l.ګامونه - 1);

    l.output += l.outputs*l.batch*(l.ګامونه - 1);
    l.cell_cpu += l.outputs*l.batch*(l.ګامونه - 1);
    l.delta += l.outputs*l.batch*(l.ګامونه - 1);

    لكل (i = l.ګامونه - 1; i >= 0; --i) {
        إذا (i != 0) copy_cpu(l.outputs*l.batch, l.cell_cpu - l.outputs*l.batch, 1, l.prev_cell_cpu, 1);
        copy_cpu(l.outputs*l.batch, l.cell_cpu, 1, l.c_cpu, 1);
        إذا (i != 0) copy_cpu(l.outputs*l.batch, l.output - l.outputs*l.batch, 1, l.prev_state_cpu, 1);
        copy_cpu(l.outputs*l.batch, l.output, 1, l.h_cpu, 1);

        l.dh_cpu = (i == 0) ? 0 : l.delta - l.outputs*l.batch;

        copy_cpu(l.outputs*l.batch, wf.output, 1, l.f_cpu, 1);			
        axpy_cpu(l.outputs*l.batch, 1, uf.output, 1, l.f_cpu, 1);			

        copy_cpu(l.outputs*l.batch, wi.output, 1, l.i_cpu, 1);			
        axpy_cpu(l.outputs*l.batch, 1, ui.output, 1, l.i_cpu, 1);			

        copy_cpu(l.outputs*l.batch, wg.output, 1, l.g_cpu, 1);			
        axpy_cpu(l.outputs*l.batch, 1, ug.output, 1, l.g_cpu, 1);			

        copy_cpu(l.outputs*l.batch, wo.output, 1, l.o_cpu, 1);			
        axpy_cpu(l.outputs*l.batch, 1, uo.output, 1, l.o_cpu, 1);			

        activate_array(l.f_cpu, l.outputs*l.batch, LOGISTIC);			
        activate_array(l.i_cpu, l.outputs*l.batch, LOGISTIC);		
        activate_array(l.g_cpu, l.outputs*l.batch, TANH);			
        activate_array(l.o_cpu, l.outputs*l.batch, LOGISTIC);		

        copy_cpu(l.outputs*l.batch, l.delta, 1, l.temp3_cpu, 1);		

        copy_cpu(l.outputs*l.batch, l.c_cpu, 1, l.لنډمهاله_cpu, 1);			
        activate_array(l.لنډمهاله_cpu, l.outputs*l.batch, TANH);			

        copy_cpu(l.outputs*l.batch, l.temp3_cpu, 1, l.temp2_cpu, 1);		
        mul_cpu(l.outputs*l.batch, l.o_cpu, 1, l.temp2_cpu, 1);			

        gradient_array(l.لنډمهاله_cpu, l.outputs*l.batch, TANH, l.temp2_cpu);
        axpy_cpu(l.outputs*l.batch, 1, l.dc_cpu, 1, l.temp2_cpu, 1);		

        copy_cpu(l.outputs*l.batch, l.c_cpu, 1, l.لنډمهاله_cpu, 1);			
        activate_array(l.لنډمهاله_cpu, l.outputs*l.batch, TANH);			
        mul_cpu(l.outputs*l.batch, l.temp3_cpu, 1, l.لنډمهاله_cpu, 1);		
        gradient_array(l.o_cpu, l.outputs*l.batch, LOGISTIC, l.لنډمهاله_cpu);
        copy_cpu(l.outputs*l.batch, l.لنډمهاله_cpu, 1, wo.delta, 1);
        s.ننوت = l.prev_state_cpu;
        s.delta = l.dh_cpu;															
        backward_connected_layer(wo, s);	

        copy_cpu(l.outputs*l.batch, l.لنډمهاله_cpu, 1, uo.delta, 1);
        s.ننوت = state.ننوت;
        s.delta = state.delta;
        backward_connected_layer(uo, s);									

        copy_cpu(l.outputs*l.batch, l.temp2_cpu, 1, l.لنډمهاله_cpu, 1);			
        mul_cpu(l.outputs*l.batch, l.i_cpu, 1, l.لنډمهاله_cpu, 1);				
        gradient_array(l.g_cpu, l.outputs*l.batch, TANH, l.لنډمهاله_cpu);		
        copy_cpu(l.outputs*l.batch, l.لنډمهاله_cpu, 1, wg.delta, 1);
        s.ننوت = l.prev_state_cpu;
        s.delta = l.dh_cpu;														
        backward_connected_layer(wg, s);	

        copy_cpu(l.outputs*l.batch, l.لنډمهاله_cpu, 1, ug.delta, 1);
        s.ننوت = state.ننوت;
        s.delta = state.delta;
        backward_connected_layer(ug, s);																

        copy_cpu(l.outputs*l.batch, l.temp2_cpu, 1, l.لنډمهاله_cpu, 1);			
        mul_cpu(l.outputs*l.batch, l.g_cpu, 1, l.لنډمهاله_cpu, 1);				
        gradient_array(l.i_cpu, l.outputs*l.batch, LOGISTIC, l.لنډمهاله_cpu);	
        copy_cpu(l.outputs*l.batch, l.لنډمهاله_cpu, 1, wi.delta, 1);
        s.ننوت = l.prev_state_cpu;
        s.delta = l.dh_cpu;
        backward_connected_layer(wi, s);						

        copy_cpu(l.outputs*l.batch, l.لنډمهاله_cpu, 1, ui.delta, 1);
        s.ننوت = state.ننوت;
        s.delta = state.delta;
        backward_connected_layer(ui, s);									

        copy_cpu(l.outputs*l.batch, l.temp2_cpu, 1, l.لنډمهاله_cpu, 1);		
        mul_cpu(l.outputs*l.batch, l.prev_cell_cpu, 1, l.لنډمهاله_cpu, 1);
        gradient_array(l.f_cpu, l.outputs*l.batch, LOGISTIC, l.لنډمهاله_cpu);
        copy_cpu(l.outputs*l.batch, l.لنډمهاله_cpu, 1, wf.delta, 1);
        s.ننوت = l.prev_state_cpu;
        s.delta = l.dh_cpu;
        backward_connected_layer(wf, s);						

        copy_cpu(l.outputs*l.batch, l.لنډمهاله_cpu, 1, uf.delta, 1);
        s.ننوت = state.ننوت;
        s.delta = state.delta;
        backward_connected_layer(uf, s);									

        copy_cpu(l.outputs*l.batch, l.temp2_cpu, 1, l.لنډمهاله_cpu, 1);			
        mul_cpu(l.outputs*l.batch, l.f_cpu, 1, l.لنډمهاله_cpu, 1);				
        copy_cpu(l.outputs*l.batch, l.لنډمهاله_cpu, 1, l.dc_cpu, 1);				

        state.ننوت -= l.inputs*l.batch;
        إذا (state.delta) state.delta -= l.inputs*l.batch;
        l.output -= l.outputs*l.batch;
        l.cell_cpu -= l.outputs*l.batch;
        l.delta -= l.outputs*l.batch;

        increment_layer(&wf, -1);
        increment_layer(&wi, -1);
        increment_layer(&wg, -1);
        increment_layer(&wo, -1);

        increment_layer(&uf, -1);
        increment_layer(&ui, -1);
        increment_layer(&ug, -1);
        increment_layer(&uo, -1);
    }
}

#که_تعریف_شوی GPU
فراغ update_lstm_layer_gpu(layer l, update_دلیلونه a)
{
    update_connected_layer_gpu(*(l.wf), a);
    update_connected_layer_gpu(*(l.wi), a);
    update_connected_layer_gpu(*(l.wg), a);
    update_connected_layer_gpu(*(l.wo), a);
    update_connected_layer_gpu(*(l.uf), a);
    update_connected_layer_gpu(*(l.ui), a);
    update_connected_layer_gpu(*(l.ug), a);
    update_connected_layer_gpu(*(l.uo), a);
}

فراغ forward_lstm_layer_gpu(layer l, شبکه state)
{
    شبکه s = { 0 };
    s.train = state.train;
    صحيح i;
    layer wf = *(l.wf);
    layer wi = *(l.wi);
    layer wg = *(l.wg);
    layer wo = *(l.wo);

    layer uf = *(l.uf);
    layer ui = *(l.ui);
    layer ug = *(l.ug);
    layer uo = *(l.uo);

    fill_gpu(l.outputs * l.batch * l.ګامونه, 0, wf.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ګامونه, 0, wi.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ګامونه, 0, wg.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ګامونه, 0, wo.delta_gpu, 1);

    fill_gpu(l.outputs * l.batch * l.ګامونه, 0, uf.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ګامونه, 0, ui.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ګامونه, 0, ug.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ګامونه, 0, uo.delta_gpu, 1);
    إذا (state.train) {
        fill_gpu(l.outputs * l.batch * l.ګامونه, 0, l.delta_gpu, 1);
    }

    لكل (i = 0; i < l.ګامونه; ++i) {
        s.ننوت_gpu = l.h_gpu;
        forward_connected_layer_gpu(wf, s);							
        forward_connected_layer_gpu(wi, s);							
        forward_connected_layer_gpu(wg, s);							
        forward_connected_layer_gpu(wo, s);							

        s.ننوت_gpu = state.ننوت_gpu;
        forward_connected_layer_gpu(uf, s);							
        forward_connected_layer_gpu(ui, s);							
        forward_connected_layer_gpu(ug, s);							
        forward_connected_layer_gpu(uo, s);							

        copy_gpu(l.outputs*l.batch, wf.output_gpu, 1, l.f_gpu, 1);
        axpy_gpu(l.outputs*l.batch, 1, uf.output_gpu, 1, l.f_gpu, 1);

        copy_gpu(l.outputs*l.batch, wi.output_gpu, 1, l.i_gpu, 1);	
        axpy_gpu(l.outputs*l.batch, 1, ui.output_gpu, 1, l.i_gpu, 1);	

        copy_gpu(l.outputs*l.batch, wg.output_gpu, 1, l.g_gpu, 1);	
        axpy_gpu(l.outputs*l.batch, 1, ug.output_gpu, 1, l.g_gpu, 1);	

        copy_gpu(l.outputs*l.batch, wo.output_gpu, 1, l.o_gpu, 1);	
        axpy_gpu(l.outputs*l.batch, 1, uo.output_gpu, 1, l.o_gpu, 1);	

        activate_array_gpu(l.f_gpu, l.outputs*l.batch, LOGISTIC);		
        activate_array_gpu(l.i_gpu, l.outputs*l.batch, LOGISTIC);		
        activate_array_gpu(l.g_gpu, l.outputs*l.batch, TANH);			
        activate_array_gpu(l.o_gpu, l.outputs*l.batch, LOGISTIC);		

        copy_gpu(l.outputs*l.batch, l.i_gpu, 1, l.لنډمهاله_gpu, 1);		
        mul_gpu(l.outputs*l.batch, l.g_gpu, 1, l.لنډمهاله_gpu, 1);		
        mul_gpu(l.outputs*l.batch, l.f_gpu, 1, l.c_gpu, 1);			
        axpy_gpu(l.outputs*l.batch, 1, l.لنډمهاله_gpu, 1, l.c_gpu, 1);	

        copy_gpu(l.outputs*l.batch, l.c_gpu, 1, l.h_gpu, 1);			
        activate_array_gpu(l.h_gpu, l.outputs*l.batch, TANH);		
        mul_gpu(l.outputs*l.batch, l.o_gpu, 1, l.h_gpu, 1);	

        copy_gpu(l.outputs*l.batch, l.c_gpu, 1, l.cell_gpu, 1);		
        copy_gpu(l.outputs*l.batch, l.h_gpu, 1, l.output_gpu, 1);

        state.ننوت_gpu += l.inputs*l.batch;
        l.output_gpu    += l.outputs*l.batch;
        l.cell_gpu      += l.outputs*l.batch;

        increment_layer(&wf, 1);
        increment_layer(&wi, 1);
        increment_layer(&wg, 1);
        increment_layer(&wo, 1);

        increment_layer(&uf, 1);
        increment_layer(&ui, 1);
        increment_layer(&ug, 1);
        increment_layer(&uo, 1);
    }
}

فراغ backward_lstm_layer_gpu(layer l, شبکه state)
{
    شبکه s = { 0 };
    s.train = state.train;
    صحيح i;
    layer wf = *(l.wf);
    layer wi = *(l.wi);
    layer wg = *(l.wg);
    layer wo = *(l.wo);

    layer uf = *(l.uf);
    layer ui = *(l.ui);
    layer ug = *(l.ug);
    layer uo = *(l.uo);

    increment_layer(&wf, l.ګامونه - 1);
    increment_layer(&wi, l.ګامونه - 1);
    increment_layer(&wg, l.ګامونه - 1);
    increment_layer(&wo, l.ګامونه - 1);

    increment_layer(&uf, l.ګامونه - 1);
    increment_layer(&ui, l.ګامونه - 1);
    increment_layer(&ug, l.ګامونه - 1);
    increment_layer(&uo, l.ګامونه - 1);

    state.ننوت_gpu += l.inputs*l.batch*(l.ګامونه - 1);
    إذا (state.delta_gpu) state.delta_gpu += l.inputs*l.batch*(l.ګامونه - 1);

    l.output_gpu += l.outputs*l.batch*(l.ګامونه - 1);
    l.cell_gpu += l.outputs*l.batch*(l.ګامونه - 1);
    l.delta_gpu += l.outputs*l.batch*(l.ګامونه - 1);

    لكل (i = l.ګامونه - 1; i >= 0; --i) {
        إذا (i != 0) copy_gpu(l.outputs*l.batch, l.cell_gpu - l.outputs*l.batch, 1, l.prev_cell_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.cell_gpu, 1, l.c_gpu, 1);
        إذا (i != 0) copy_gpu(l.outputs*l.batch, l.output_gpu - l.outputs*l.batch, 1, l.prev_state_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.h_gpu, 1);

        l.dh_gpu = (i == 0) ? 0 : l.delta_gpu - l.outputs*l.batch;

        copy_gpu(l.outputs*l.batch, wf.output_gpu, 1, l.f_gpu, 1);			
        axpy_gpu(l.outputs*l.batch, 1, uf.output_gpu, 1, l.f_gpu, 1);			

        copy_gpu(l.outputs*l.batch, wi.output_gpu, 1, l.i_gpu, 1);			
        axpy_gpu(l.outputs*l.batch, 1, ui.output_gpu, 1, l.i_gpu, 1);			

        copy_gpu(l.outputs*l.batch, wg.output_gpu, 1, l.g_gpu, 1);			
        axpy_gpu(l.outputs*l.batch, 1, ug.output_gpu, 1, l.g_gpu, 1);			

        copy_gpu(l.outputs*l.batch, wo.output_gpu, 1, l.o_gpu, 1);			
        axpy_gpu(l.outputs*l.batch, 1, uo.output_gpu, 1, l.o_gpu, 1);			

        activate_array_gpu(l.f_gpu, l.outputs*l.batch, LOGISTIC);			
        activate_array_gpu(l.i_gpu, l.outputs*l.batch, LOGISTIC);		
        activate_array_gpu(l.g_gpu, l.outputs*l.batch, TANH);			
        activate_array_gpu(l.o_gpu, l.outputs*l.batch, LOGISTIC);		

        copy_gpu(l.outputs*l.batch, l.delta_gpu, 1, l.temp3_gpu, 1);		

        copy_gpu(l.outputs*l.batch, l.c_gpu, 1, l.لنډمهاله_gpu, 1);			
        activate_array_gpu(l.لنډمهاله_gpu, l.outputs*l.batch, TANH);			

        copy_gpu(l.outputs*l.batch, l.temp3_gpu, 1, l.temp2_gpu, 1);		
        mul_gpu(l.outputs*l.batch, l.o_gpu, 1, l.temp2_gpu, 1);			

        gradient_array_gpu(l.لنډمهاله_gpu, l.outputs*l.batch, TANH, l.temp2_gpu);
        axpy_gpu(l.outputs*l.batch, 1, l.dc_gpu, 1, l.temp2_gpu, 1);		

        copy_gpu(l.outputs*l.batch, l.c_gpu, 1, l.لنډمهاله_gpu, 1);			
        activate_array_gpu(l.لنډمهاله_gpu, l.outputs*l.batch, TANH);			
        mul_gpu(l.outputs*l.batch, l.temp3_gpu, 1, l.لنډمهاله_gpu, 1);		
        gradient_array_gpu(l.o_gpu, l.outputs*l.batch, LOGISTIC, l.لنډمهاله_gpu);
        copy_gpu(l.outputs*l.batch, l.لنډمهاله_gpu, 1, wo.delta_gpu, 1);
        s.ننوت_gpu = l.prev_state_gpu;
        s.delta_gpu = l.dh_gpu;															
        backward_connected_layer_gpu(wo, s);	

        copy_gpu(l.outputs*l.batch, l.لنډمهاله_gpu, 1, uo.delta_gpu, 1);
        s.ننوت_gpu = state.ننوت_gpu;
        s.delta_gpu = state.delta_gpu;
        backward_connected_layer_gpu(uo, s);									

        copy_gpu(l.outputs*l.batch, l.temp2_gpu, 1, l.لنډمهاله_gpu, 1);			
        mul_gpu(l.outputs*l.batch, l.i_gpu, 1, l.لنډمهاله_gpu, 1);				
        gradient_array_gpu(l.g_gpu, l.outputs*l.batch, TANH, l.لنډمهاله_gpu);		
        copy_gpu(l.outputs*l.batch, l.لنډمهاله_gpu, 1, wg.delta_gpu, 1);
        s.ننوت_gpu = l.prev_state_gpu;
        s.delta_gpu = l.dh_gpu;														
        backward_connected_layer_gpu(wg, s);	

        copy_gpu(l.outputs*l.batch, l.لنډمهاله_gpu, 1, ug.delta_gpu, 1);
        s.ننوت_gpu = state.ننوت_gpu;
        s.delta_gpu = state.delta_gpu;
        backward_connected_layer_gpu(ug, s);																

        copy_gpu(l.outputs*l.batch, l.temp2_gpu, 1, l.لنډمهاله_gpu, 1);			
        mul_gpu(l.outputs*l.batch, l.g_gpu, 1, l.لنډمهاله_gpu, 1);				
        gradient_array_gpu(l.i_gpu, l.outputs*l.batch, LOGISTIC, l.لنډمهاله_gpu);	
        copy_gpu(l.outputs*l.batch, l.لنډمهاله_gpu, 1, wi.delta_gpu, 1);
        s.ننوت_gpu = l.prev_state_gpu;
        s.delta_gpu = l.dh_gpu;
        backward_connected_layer_gpu(wi, s);						

        copy_gpu(l.outputs*l.batch, l.لنډمهاله_gpu, 1, ui.delta_gpu, 1);
        s.ننوت_gpu = state.ننوت_gpu;
        s.delta_gpu = state.delta_gpu;
        backward_connected_layer_gpu(ui, s);									

        copy_gpu(l.outputs*l.batch, l.temp2_gpu, 1, l.لنډمهاله_gpu, 1);		
        mul_gpu(l.outputs*l.batch, l.prev_cell_gpu, 1, l.لنډمهاله_gpu, 1);
        gradient_array_gpu(l.f_gpu, l.outputs*l.batch, LOGISTIC, l.لنډمهاله_gpu);
        copy_gpu(l.outputs*l.batch, l.لنډمهاله_gpu, 1, wf.delta_gpu, 1);
        s.ننوت_gpu = l.prev_state_gpu;
        s.delta_gpu = l.dh_gpu;
        backward_connected_layer_gpu(wf, s);						

        copy_gpu(l.outputs*l.batch, l.لنډمهاله_gpu, 1, uf.delta_gpu, 1);
        s.ننوت_gpu = state.ننوت_gpu;
        s.delta_gpu = state.delta_gpu;
        backward_connected_layer_gpu(uf, s);									

        copy_gpu(l.outputs*l.batch, l.temp2_gpu, 1, l.لنډمهاله_gpu, 1);			
        mul_gpu(l.outputs*l.batch, l.f_gpu, 1, l.لنډمهاله_gpu, 1);				
        copy_gpu(l.outputs*l.batch, l.لنډمهاله_gpu, 1, l.dc_gpu, 1);				

        state.ننوت_gpu -= l.inputs*l.batch;
        إذا (state.delta_gpu) state.delta_gpu -= l.inputs*l.batch;
        l.output_gpu -= l.outputs*l.batch;
        l.cell_gpu -= l.outputs*l.batch;
        l.delta_gpu -= l.outputs*l.batch;

        increment_layer(&wf, -1);
        increment_layer(&wi, -1);
        increment_layer(&wg, -1);
        increment_layer(&wo, -1);

        increment_layer(&uf, -1);
        increment_layer(&ui, -1);
        increment_layer(&ug, -1);
        increment_layer(&uo, -1);
    }
}
#د_که_پای

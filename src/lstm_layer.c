#شامل "lstm_layer.h"
#شامل "connected_layer.h"
#شامل "utils.h"
#شامل "cuda.h"
#شامل "blas.h"
#شامل "gemm.h"

#شامل <math.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <string.h>

ساکن خالی increment_layer(layer *l, صحیح اقدام)
{
    صحیح عدد = l->outputs*l->batch*اقدام;
    l->output += عدد;
    l->delta += عدد;
    l->x += عدد;
    l->x_norm += عدد;

#اگر_تعریف_شدہ GPU
    l->output_gpu += عدد;
    l->delta_gpu += عدد;
    l->x_gpu += عدد;
    l->x_norm_gpu += عدد;
#اگر_ختم
}

layer make_lstm_layer(صحیح batch, صحیح inputs, صحیح outputs, صحیح اقدام, صحیح batch_normalize, صحیح adam)
{
    fprintf(stderr, "LSTM Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / اقدام;
    layer l = { 0 };
    l.batch = batch;
    l.type = LSTM;
    l.اقدام = اقدام;
    l.inputs = inputs;

    l.uf = حافظہ_دو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.uf) = make_connected_layer(batch*اقدام, inputs, outputs, خطی, batch_normalize, adam);
    l.uf->batch = batch;

    l.ui = حافظہ_دو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.ui) = make_connected_layer(batch*اقدام, inputs, outputs, خطی, batch_normalize, adam);
    l.ui->batch = batch;

    l.ug = حافظہ_دو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.ug) = make_connected_layer(batch*اقدام, inputs, outputs, خطی, batch_normalize, adam);
    l.ug->batch = batch;

    l.uo = حافظہ_دو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.uo) = make_connected_layer(batch*اقدام, inputs, outputs, خطی, batch_normalize, adam);
    l.uo->batch = batch;

    l.wf = حافظہ_دو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.wf) = make_connected_layer(batch*اقدام, outputs, outputs, خطی, batch_normalize, adam);
    l.wf->batch = batch;

    l.wi = حافظہ_دو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.wi) = make_connected_layer(batch*اقدام, outputs, outputs, خطی, batch_normalize, adam);
    l.wi->batch = batch;

    l.wg = حافظہ_دو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.wg) = make_connected_layer(batch*اقدام, outputs, outputs, خطی, batch_normalize, adam);
    l.wg->batch = batch;

    l.wo = حافظہ_دو(حجم(layer));
    fprintf(stderr, "\t\t");
    *(l.wo) = make_connected_layer(batch*اقدام, outputs, outputs, خطی, batch_normalize, adam);
    l.wo->batch = batch;

    l.batch_normalize = batch_normalize;
    l.outputs = outputs;

    l.output = صفر_حافظہ_دو(outputs*batch*اقدام, حجم(اعشاری));
    l.state = صفر_حافظہ_دو(outputs*batch, حجم(اعشاری));

    l.forward = forward_lstm_layer;
    l.update = update_lstm_layer;

    l.پچھلا_state_cpu =  صفر_حافظہ_دو(batch*outputs, حجم(اعشاری));
    l.پچھلا_cell_cpu =   صفر_حافظہ_دو(batch*outputs, حجم(اعشاری));
    l.cell_cpu =        صفر_حافظہ_دو(batch*outputs*اقدام, حجم(اعشاری));

    l.f_cpu =           صفر_حافظہ_دو(batch*outputs, حجم(اعشاری));
    l.i_cpu =           صفر_حافظہ_دو(batch*outputs, حجم(اعشاری));
    l.g_cpu =           صفر_حافظہ_دو(batch*outputs, حجم(اعشاری));
    l.o_cpu =           صفر_حافظہ_دو(batch*outputs, حجم(اعشاری));
    l.c_cpu =           صفر_حافظہ_دو(batch*outputs, حجم(اعشاری));
    l.h_cpu =           صفر_حافظہ_دو(batch*outputs, حجم(اعشاری));
    l.عارضی_cpu =        صفر_حافظہ_دو(batch*outputs, حجم(اعشاری));
    l.temp2_cpu =       صفر_حافظہ_دو(batch*outputs, حجم(اعشاری));
    l.temp3_cpu =       صفر_حافظہ_دو(batch*outputs, حجم(اعشاری));
    l.dc_cpu =          صفر_حافظہ_دو(batch*outputs, حجم(اعشاری));
    l.dh_cpu =          صفر_حافظہ_دو(batch*outputs, حجم(اعشاری));

#اگر_تعریف_شدہ GPU
    l.forward_gpu = forward_lstm_layer_gpu;
    l.backward_gpu = backward_lstm_layer_gpu;
    l.update_gpu = update_lstm_layer_gpu;

    l.output_gpu = cuda_make_array(0, batch*outputs*اقدام);
    l.delta_gpu = cuda_make_array(0, batch*l.outputs*اقدام);

    l.پچھلا_state_gpu = cuda_make_array(0, batch*outputs);
    l.پچھلا_cell_gpu = cuda_make_array(0, batch*outputs);
    l.cell_gpu = cuda_make_array(0, batch*outputs*اقدام);

    l.f_gpu = cuda_make_array(0, batch*outputs);
    l.i_gpu = cuda_make_array(0, batch*outputs);
    l.g_gpu = cuda_make_array(0, batch*outputs);
    l.o_gpu = cuda_make_array(0, batch*outputs);
    l.c_gpu = cuda_make_array(0, batch*outputs);
    l.h_gpu = cuda_make_array(0, batch*outputs);
    l.عارضی_gpu =  cuda_make_array(0, batch*outputs);
    l.temp2_gpu = cuda_make_array(0, batch*outputs);
    l.temp3_gpu = cuda_make_array(0, batch*outputs);
    l.dc_gpu = cuda_make_array(0, batch*outputs);
    l.dh_gpu = cuda_make_array(0, batch*outputs);
#اگر_تعریف_شدہ CUDNN
        cudnn_سیٹ_Tensor4d_Descriptor(l.wf->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, batch, l.wf->برآمد_c, l.wf->برآمد_h, l.wf->برآمد_w); 
        cudnn_سیٹ_Tensor4d_Descriptor(l.wi->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, batch, l.wi->برآمد_c, l.wi->برآمد_h, l.wi->برآمد_w); 
        cudnn_سیٹ_Tensor4d_Descriptor(l.wg->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, batch, l.wg->برآمد_c, l.wg->برآمد_h, l.wg->برآمد_w); 
        cudnn_سیٹ_Tensor4d_Descriptor(l.wo->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, batch, l.wo->برآمد_c, l.wo->برآمد_h, l.wo->برآمد_w); 

        cudnn_سیٹ_Tensor4d_Descriptor(l.uf->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, batch, l.uf->برآمد_c, l.uf->برآمد_h, l.uf->برآمد_w); 
        cudnn_سیٹ_Tensor4d_Descriptor(l.ui->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, batch, l.ui->برآمد_c, l.ui->برآمد_h, l.ui->برآمد_w); 
        cudnn_سیٹ_Tensor4d_Descriptor(l.ug->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, batch, l.ug->برآمد_c, l.ug->برآمد_h, l.ug->برآمد_w); 
        cudnn_سیٹ_Tensor4d_Descriptor(l.uo->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, batch, l.uo->برآمد_c, l.uo->برآمد_h, l.uo->برآمد_w); 
#اگر_ختم

#اگر_ختم

    واپس l;
}

خالی update_lstm_layer(layer l, update_دلائل a)
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

خالی forward_lstm_layer(layer l, جال state)
{
    جال s = { 0 };
    s.train = state.train;
    صحیح i;
    layer wf = *(l.wf);
    layer wi = *(l.wi);
    layer wg = *(l.wg);
    layer wo = *(l.wo);

    layer uf = *(l.uf);
    layer ui = *(l.ui);
    layer ug = *(l.ug);
    layer uo = *(l.uo);

    fill_cpu(l.outputs * l.batch * l.اقدام, 0, wf.delta, 1);
    fill_cpu(l.outputs * l.batch * l.اقدام, 0, wi.delta, 1);
    fill_cpu(l.outputs * l.batch * l.اقدام, 0, wg.delta, 1);
    fill_cpu(l.outputs * l.batch * l.اقدام, 0, wo.delta, 1);

    fill_cpu(l.outputs * l.batch * l.اقدام, 0, uf.delta, 1);
    fill_cpu(l.outputs * l.batch * l.اقدام, 0, ui.delta, 1);
    fill_cpu(l.outputs * l.batch * l.اقدام, 0, ug.delta, 1);
    fill_cpu(l.outputs * l.batch * l.اقدام, 0, uo.delta, 1);
    اگر (state.train) {
        fill_cpu(l.outputs * l.batch * l.اقدام, 0, l.delta, 1);
    }

    برائے (i = 0; i < l.اقدام; ++i) {
        s.داخلہ = l.h_cpu;
        forward_connected_layer(wf, s);							
        forward_connected_layer(wi, s);							
        forward_connected_layer(wg, s);							
        forward_connected_layer(wo, s);							

        s.داخلہ = state.داخلہ;
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

        copy_cpu(l.outputs*l.batch, l.i_cpu, 1, l.عارضی_cpu, 1);		
        mul_cpu(l.outputs*l.batch, l.g_cpu, 1, l.عارضی_cpu, 1);		
        mul_cpu(l.outputs*l.batch, l.f_cpu, 1, l.c_cpu, 1);			
        axpy_cpu(l.outputs*l.batch, 1, l.عارضی_cpu, 1, l.c_cpu, 1);	

        copy_cpu(l.outputs*l.batch, l.c_cpu, 1, l.h_cpu, 1);			
        activate_array(l.h_cpu, l.outputs*l.batch, TANH);		
        mul_cpu(l.outputs*l.batch, l.o_cpu, 1, l.h_cpu, 1);	

        copy_cpu(l.outputs*l.batch, l.c_cpu, 1, l.cell_cpu, 1);		
        copy_cpu(l.outputs*l.batch, l.h_cpu, 1, l.output, 1);

        state.داخلہ += l.inputs*l.batch;
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

خالی backward_lstm_layer(layer l, جال state)
{
    جال s = { 0 };
    s.train = state.train;
    صحیح i;
    layer wf = *(l.wf);
    layer wi = *(l.wi);
    layer wg = *(l.wg);
    layer wo = *(l.wo);

    layer uf = *(l.uf);
    layer ui = *(l.ui);
    layer ug = *(l.ug);
    layer uo = *(l.uo);

    increment_layer(&wf, l.اقدام - 1);
    increment_layer(&wi, l.اقدام - 1);
    increment_layer(&wg, l.اقدام - 1);
    increment_layer(&wo, l.اقدام - 1);

    increment_layer(&uf, l.اقدام - 1);
    increment_layer(&ui, l.اقدام - 1);
    increment_layer(&ug, l.اقدام - 1);
    increment_layer(&uo, l.اقدام - 1);

    state.داخلہ += l.inputs*l.batch*(l.اقدام - 1);
    اگر (state.delta) state.delta += l.inputs*l.batch*(l.اقدام - 1);

    l.output += l.outputs*l.batch*(l.اقدام - 1);
    l.cell_cpu += l.outputs*l.batch*(l.اقدام - 1);
    l.delta += l.outputs*l.batch*(l.اقدام - 1);

    برائے (i = l.اقدام - 1; i >= 0; --i) {
        اگر (i != 0) copy_cpu(l.outputs*l.batch, l.cell_cpu - l.outputs*l.batch, 1, l.پچھلا_cell_cpu, 1);
        copy_cpu(l.outputs*l.batch, l.cell_cpu, 1, l.c_cpu, 1);
        اگر (i != 0) copy_cpu(l.outputs*l.batch, l.output - l.outputs*l.batch, 1, l.پچھلا_state_cpu, 1);
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

        copy_cpu(l.outputs*l.batch, l.c_cpu, 1, l.عارضی_cpu, 1);			
        activate_array(l.عارضی_cpu, l.outputs*l.batch, TANH);			

        copy_cpu(l.outputs*l.batch, l.temp3_cpu, 1, l.temp2_cpu, 1);		
        mul_cpu(l.outputs*l.batch, l.o_cpu, 1, l.temp2_cpu, 1);			

        gradient_array(l.عارضی_cpu, l.outputs*l.batch, TANH, l.temp2_cpu);
        axpy_cpu(l.outputs*l.batch, 1, l.dc_cpu, 1, l.temp2_cpu, 1);		

        copy_cpu(l.outputs*l.batch, l.c_cpu, 1, l.عارضی_cpu, 1);			
        activate_array(l.عارضی_cpu, l.outputs*l.batch, TANH);			
        mul_cpu(l.outputs*l.batch, l.temp3_cpu, 1, l.عارضی_cpu, 1);		
        gradient_array(l.o_cpu, l.outputs*l.batch, LOGISTIC, l.عارضی_cpu);
        copy_cpu(l.outputs*l.batch, l.عارضی_cpu, 1, wo.delta, 1);
        s.داخلہ = l.پچھلا_state_cpu;
        s.delta = l.dh_cpu;															
        backward_connected_layer(wo, s);	

        copy_cpu(l.outputs*l.batch, l.عارضی_cpu, 1, uo.delta, 1);
        s.داخلہ = state.داخلہ;
        s.delta = state.delta;
        backward_connected_layer(uo, s);									

        copy_cpu(l.outputs*l.batch, l.temp2_cpu, 1, l.عارضی_cpu, 1);			
        mul_cpu(l.outputs*l.batch, l.i_cpu, 1, l.عارضی_cpu, 1);				
        gradient_array(l.g_cpu, l.outputs*l.batch, TANH, l.عارضی_cpu);		
        copy_cpu(l.outputs*l.batch, l.عارضی_cpu, 1, wg.delta, 1);
        s.داخلہ = l.پچھلا_state_cpu;
        s.delta = l.dh_cpu;														
        backward_connected_layer(wg, s);	

        copy_cpu(l.outputs*l.batch, l.عارضی_cpu, 1, ug.delta, 1);
        s.داخلہ = state.داخلہ;
        s.delta = state.delta;
        backward_connected_layer(ug, s);																

        copy_cpu(l.outputs*l.batch, l.temp2_cpu, 1, l.عارضی_cpu, 1);			
        mul_cpu(l.outputs*l.batch, l.g_cpu, 1, l.عارضی_cpu, 1);				
        gradient_array(l.i_cpu, l.outputs*l.batch, LOGISTIC, l.عارضی_cpu);	
        copy_cpu(l.outputs*l.batch, l.عارضی_cpu, 1, wi.delta, 1);
        s.داخلہ = l.پچھلا_state_cpu;
        s.delta = l.dh_cpu;
        backward_connected_layer(wi, s);						

        copy_cpu(l.outputs*l.batch, l.عارضی_cpu, 1, ui.delta, 1);
        s.داخلہ = state.داخلہ;
        s.delta = state.delta;
        backward_connected_layer(ui, s);									

        copy_cpu(l.outputs*l.batch, l.temp2_cpu, 1, l.عارضی_cpu, 1);		
        mul_cpu(l.outputs*l.batch, l.پچھلا_cell_cpu, 1, l.عارضی_cpu, 1);
        gradient_array(l.f_cpu, l.outputs*l.batch, LOGISTIC, l.عارضی_cpu);
        copy_cpu(l.outputs*l.batch, l.عارضی_cpu, 1, wf.delta, 1);
        s.داخلہ = l.پچھلا_state_cpu;
        s.delta = l.dh_cpu;
        backward_connected_layer(wf, s);						

        copy_cpu(l.outputs*l.batch, l.عارضی_cpu, 1, uf.delta, 1);
        s.داخلہ = state.داخلہ;
        s.delta = state.delta;
        backward_connected_layer(uf, s);									

        copy_cpu(l.outputs*l.batch, l.temp2_cpu, 1, l.عارضی_cpu, 1);			
        mul_cpu(l.outputs*l.batch, l.f_cpu, 1, l.عارضی_cpu, 1);				
        copy_cpu(l.outputs*l.batch, l.عارضی_cpu, 1, l.dc_cpu, 1);				

        state.داخلہ -= l.inputs*l.batch;
        اگر (state.delta) state.delta -= l.inputs*l.batch;
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

#اگر_تعریف_شدہ GPU
خالی update_lstm_layer_gpu(layer l, update_دلائل a)
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

خالی forward_lstm_layer_gpu(layer l, جال state)
{
    جال s = { 0 };
    s.train = state.train;
    صحیح i;
    layer wf = *(l.wf);
    layer wi = *(l.wi);
    layer wg = *(l.wg);
    layer wo = *(l.wo);

    layer uf = *(l.uf);
    layer ui = *(l.ui);
    layer ug = *(l.ug);
    layer uo = *(l.uo);

    fill_gpu(l.outputs * l.batch * l.اقدام, 0, wf.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.اقدام, 0, wi.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.اقدام, 0, wg.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.اقدام, 0, wo.delta_gpu, 1);

    fill_gpu(l.outputs * l.batch * l.اقدام, 0, uf.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.اقدام, 0, ui.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.اقدام, 0, ug.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.اقدام, 0, uo.delta_gpu, 1);
    اگر (state.train) {
        fill_gpu(l.outputs * l.batch * l.اقدام, 0, l.delta_gpu, 1);
    }

    برائے (i = 0; i < l.اقدام; ++i) {
        s.داخلہ_gpu = l.h_gpu;
        forward_connected_layer_gpu(wf, s);							
        forward_connected_layer_gpu(wi, s);							
        forward_connected_layer_gpu(wg, s);							
        forward_connected_layer_gpu(wo, s);							

        s.داخلہ_gpu = state.داخلہ_gpu;
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

        copy_gpu(l.outputs*l.batch, l.i_gpu, 1, l.عارضی_gpu, 1);		
        mul_gpu(l.outputs*l.batch, l.g_gpu, 1, l.عارضی_gpu, 1);		
        mul_gpu(l.outputs*l.batch, l.f_gpu, 1, l.c_gpu, 1);			
        axpy_gpu(l.outputs*l.batch, 1, l.عارضی_gpu, 1, l.c_gpu, 1);	

        copy_gpu(l.outputs*l.batch, l.c_gpu, 1, l.h_gpu, 1);			
        activate_array_gpu(l.h_gpu, l.outputs*l.batch, TANH);		
        mul_gpu(l.outputs*l.batch, l.o_gpu, 1, l.h_gpu, 1);	

        copy_gpu(l.outputs*l.batch, l.c_gpu, 1, l.cell_gpu, 1);		
        copy_gpu(l.outputs*l.batch, l.h_gpu, 1, l.output_gpu, 1);

        state.داخلہ_gpu += l.inputs*l.batch;
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

خالی backward_lstm_layer_gpu(layer l, جال state)
{
    جال s = { 0 };
    s.train = state.train;
    صحیح i;
    layer wf = *(l.wf);
    layer wi = *(l.wi);
    layer wg = *(l.wg);
    layer wo = *(l.wo);

    layer uf = *(l.uf);
    layer ui = *(l.ui);
    layer ug = *(l.ug);
    layer uo = *(l.uo);

    increment_layer(&wf, l.اقدام - 1);
    increment_layer(&wi, l.اقدام - 1);
    increment_layer(&wg, l.اقدام - 1);
    increment_layer(&wo, l.اقدام - 1);

    increment_layer(&uf, l.اقدام - 1);
    increment_layer(&ui, l.اقدام - 1);
    increment_layer(&ug, l.اقدام - 1);
    increment_layer(&uo, l.اقدام - 1);

    state.داخلہ_gpu += l.inputs*l.batch*(l.اقدام - 1);
    اگر (state.delta_gpu) state.delta_gpu += l.inputs*l.batch*(l.اقدام - 1);

    l.output_gpu += l.outputs*l.batch*(l.اقدام - 1);
    l.cell_gpu += l.outputs*l.batch*(l.اقدام - 1);
    l.delta_gpu += l.outputs*l.batch*(l.اقدام - 1);

    برائے (i = l.اقدام - 1; i >= 0; --i) {
        اگر (i != 0) copy_gpu(l.outputs*l.batch, l.cell_gpu - l.outputs*l.batch, 1, l.پچھلا_cell_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.cell_gpu, 1, l.c_gpu, 1);
        اگر (i != 0) copy_gpu(l.outputs*l.batch, l.output_gpu - l.outputs*l.batch, 1, l.پچھلا_state_gpu, 1);
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

        copy_gpu(l.outputs*l.batch, l.c_gpu, 1, l.عارضی_gpu, 1);			
        activate_array_gpu(l.عارضی_gpu, l.outputs*l.batch, TANH);			

        copy_gpu(l.outputs*l.batch, l.temp3_gpu, 1, l.temp2_gpu, 1);		
        mul_gpu(l.outputs*l.batch, l.o_gpu, 1, l.temp2_gpu, 1);			

        gradient_array_gpu(l.عارضی_gpu, l.outputs*l.batch, TANH, l.temp2_gpu);
        axpy_gpu(l.outputs*l.batch, 1, l.dc_gpu, 1, l.temp2_gpu, 1);		

        copy_gpu(l.outputs*l.batch, l.c_gpu, 1, l.عارضی_gpu, 1);			
        activate_array_gpu(l.عارضی_gpu, l.outputs*l.batch, TANH);			
        mul_gpu(l.outputs*l.batch, l.temp3_gpu, 1, l.عارضی_gpu, 1);		
        gradient_array_gpu(l.o_gpu, l.outputs*l.batch, LOGISTIC, l.عارضی_gpu);
        copy_gpu(l.outputs*l.batch, l.عارضی_gpu, 1, wo.delta_gpu, 1);
        s.داخلہ_gpu = l.پچھلا_state_gpu;
        s.delta_gpu = l.dh_gpu;															
        backward_connected_layer_gpu(wo, s);	

        copy_gpu(l.outputs*l.batch, l.عارضی_gpu, 1, uo.delta_gpu, 1);
        s.داخلہ_gpu = state.داخلہ_gpu;
        s.delta_gpu = state.delta_gpu;
        backward_connected_layer_gpu(uo, s);									

        copy_gpu(l.outputs*l.batch, l.temp2_gpu, 1, l.عارضی_gpu, 1);			
        mul_gpu(l.outputs*l.batch, l.i_gpu, 1, l.عارضی_gpu, 1);				
        gradient_array_gpu(l.g_gpu, l.outputs*l.batch, TANH, l.عارضی_gpu);		
        copy_gpu(l.outputs*l.batch, l.عارضی_gpu, 1, wg.delta_gpu, 1);
        s.داخلہ_gpu = l.پچھلا_state_gpu;
        s.delta_gpu = l.dh_gpu;														
        backward_connected_layer_gpu(wg, s);	

        copy_gpu(l.outputs*l.batch, l.عارضی_gpu, 1, ug.delta_gpu, 1);
        s.داخلہ_gpu = state.داخلہ_gpu;
        s.delta_gpu = state.delta_gpu;
        backward_connected_layer_gpu(ug, s);																

        copy_gpu(l.outputs*l.batch, l.temp2_gpu, 1, l.عارضی_gpu, 1);			
        mul_gpu(l.outputs*l.batch, l.g_gpu, 1, l.عارضی_gpu, 1);				
        gradient_array_gpu(l.i_gpu, l.outputs*l.batch, LOGISTIC, l.عارضی_gpu);	
        copy_gpu(l.outputs*l.batch, l.عارضی_gpu, 1, wi.delta_gpu, 1);
        s.داخلہ_gpu = l.پچھلا_state_gpu;
        s.delta_gpu = l.dh_gpu;
        backward_connected_layer_gpu(wi, s);						

        copy_gpu(l.outputs*l.batch, l.عارضی_gpu, 1, ui.delta_gpu, 1);
        s.داخلہ_gpu = state.داخلہ_gpu;
        s.delta_gpu = state.delta_gpu;
        backward_connected_layer_gpu(ui, s);									

        copy_gpu(l.outputs*l.batch, l.temp2_gpu, 1, l.عارضی_gpu, 1);		
        mul_gpu(l.outputs*l.batch, l.پچھلا_cell_gpu, 1, l.عارضی_gpu, 1);
        gradient_array_gpu(l.f_gpu, l.outputs*l.batch, LOGISTIC, l.عارضی_gpu);
        copy_gpu(l.outputs*l.batch, l.عارضی_gpu, 1, wf.delta_gpu, 1);
        s.داخلہ_gpu = l.پچھلا_state_gpu;
        s.delta_gpu = l.dh_gpu;
        backward_connected_layer_gpu(wf, s);						

        copy_gpu(l.outputs*l.batch, l.عارضی_gpu, 1, uf.delta_gpu, 1);
        s.داخلہ_gpu = state.داخلہ_gpu;
        s.delta_gpu = state.delta_gpu;
        backward_connected_layer_gpu(uf, s);									

        copy_gpu(l.outputs*l.batch, l.temp2_gpu, 1, l.عارضی_gpu, 1);			
        mul_gpu(l.outputs*l.batch, l.f_gpu, 1, l.عارضی_gpu, 1);				
        copy_gpu(l.outputs*l.batch, l.عارضی_gpu, 1, l.dc_gpu, 1);				

        state.داخلہ_gpu -= l.inputs*l.batch;
        اگر (state.delta_gpu) state.delta_gpu -= l.inputs*l.batch;
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
#اگر_ختم

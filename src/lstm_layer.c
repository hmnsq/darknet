#ਅਨ੍ਤਰ੍ਭਾਵ "lstm_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "connected_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gemm.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

ਸਥਿਰ ਖਾਲੀ increment_layer(layer *l, ਪੂਰਨ ਕਦਮਾਂ)
{
    ਪੂਰਨ ਅੰਕ = l->outputs*l->batch*ਕਦਮਾਂ;
    l->output += ਅੰਕ;
    l->delta += ਅੰਕ;
    l->x += ਅੰਕ;
    l->x_norm += ਅੰਕ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l->output_gpu += ਅੰਕ;
    l->delta_gpu += ਅੰਕ;
    l->x_gpu += ਅੰਕ;
    l->x_norm_gpu += ਅੰਕ;
#ਜੇ_ਅੰਤ
}

layer make_lstm_layer(ਪੂਰਨ batch, ਪੂਰਨ inputs, ਪੂਰਨ outputs, ਪੂਰਨ ਕਦਮਾਂ, ਪੂਰਨ batch_normalize, ਪੂਰਨ adam)
{
    fprintf(stderr, "LSTM Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / ਕਦਮਾਂ;
    layer l = { 0 };
    l.batch = batch;
    l.type = LSTM;
    l.ਕਦਮਾਂ = ਕਦਮਾਂ;
    l.inputs = inputs;

    l.uf = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(layer));
    fprintf(stderr, "\t\t");
    *(l.uf) = make_connected_layer(batch*ਕਦਮਾਂ, inputs, outputs, ਰੇਖਿਕ, batch_normalize, adam);
    l.uf->batch = batch;

    l.ui = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(layer));
    fprintf(stderr, "\t\t");
    *(l.ui) = make_connected_layer(batch*ਕਦਮਾਂ, inputs, outputs, ਰੇਖਿਕ, batch_normalize, adam);
    l.ui->batch = batch;

    l.ug = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(layer));
    fprintf(stderr, "\t\t");
    *(l.ug) = make_connected_layer(batch*ਕਦਮਾਂ, inputs, outputs, ਰੇਖਿਕ, batch_normalize, adam);
    l.ug->batch = batch;

    l.uo = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(layer));
    fprintf(stderr, "\t\t");
    *(l.uo) = make_connected_layer(batch*ਕਦਮਾਂ, inputs, outputs, ਰੇਖਿਕ, batch_normalize, adam);
    l.uo->batch = batch;

    l.wf = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(layer));
    fprintf(stderr, "\t\t");
    *(l.wf) = make_connected_layer(batch*ਕਦਮਾਂ, outputs, outputs, ਰੇਖਿਕ, batch_normalize, adam);
    l.wf->batch = batch;

    l.wi = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(layer));
    fprintf(stderr, "\t\t");
    *(l.wi) = make_connected_layer(batch*ਕਦਮਾਂ, outputs, outputs, ਰੇਖਿਕ, batch_normalize, adam);
    l.wi->batch = batch;

    l.wg = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(layer));
    fprintf(stderr, "\t\t");
    *(l.wg) = make_connected_layer(batch*ਕਦਮਾਂ, outputs, outputs, ਰੇਖਿਕ, batch_normalize, adam);
    l.wg->batch = batch;

    l.wo = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(layer));
    fprintf(stderr, "\t\t");
    *(l.wo) = make_connected_layer(batch*ਕਦਮਾਂ, outputs, outputs, ਰੇਖਿਕ, batch_normalize, adam);
    l.wo->batch = batch;

    l.batch_normalize = batch_normalize;
    l.outputs = outputs;

    l.output = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(outputs*batch*ਕਦਮਾਂ, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.state = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(outputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    l.forward = forward_lstm_layer;
    l.update = update_lstm_layer;

    l.prev_state_cpu =  ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.prev_cell_cpu =   ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.cell_cpu =        ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*outputs*ਕਦਮਾਂ, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    l.f_cpu =           ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.i_cpu =           ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.g_cpu =           ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.o_cpu =           ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.c_cpu =           ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.h_cpu =           ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.ਆਰਜ਼ੀ_cpu =        ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.temp2_cpu =       ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.temp3_cpu =       ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.dc_cpu =          ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.dh_cpu =          ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(batch*outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_lstm_layer_gpu;
    l.backward_gpu = backward_lstm_layer_gpu;
    l.update_gpu = update_lstm_layer_gpu;

    l.output_gpu = cuda_make_array(0, batch*outputs*ਕਦਮਾਂ);
    l.delta_gpu = cuda_make_array(0, batch*l.outputs*ਕਦਮਾਂ);

    l.prev_state_gpu = cuda_make_array(0, batch*outputs);
    l.prev_cell_gpu = cuda_make_array(0, batch*outputs);
    l.cell_gpu = cuda_make_array(0, batch*outputs*ਕਦਮਾਂ);

    l.f_gpu = cuda_make_array(0, batch*outputs);
    l.i_gpu = cuda_make_array(0, batch*outputs);
    l.g_gpu = cuda_make_array(0, batch*outputs);
    l.o_gpu = cuda_make_array(0, batch*outputs);
    l.c_gpu = cuda_make_array(0, batch*outputs);
    l.h_gpu = cuda_make_array(0, batch*outputs);
    l.ਆਰਜ਼ੀ_gpu =  cuda_make_array(0, batch*outputs);
    l.temp2_gpu = cuda_make_array(0, batch*outputs);
    l.temp3_gpu = cuda_make_array(0, batch*outputs);
    l.dc_gpu = cuda_make_array(0, batch*outputs);
    l.dh_gpu = cuda_make_array(0, batch*outputs);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CUDNN
        cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l.wf->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, batch, l.wf->ਬਾਹਰਲਾ_c, l.wf->ਬਾਹਰਲਾ_h, l.wf->ਬਾਹਰਲਾ_w); 
        cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l.wi->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, batch, l.wi->ਬਾਹਰਲਾ_c, l.wi->ਬਾਹਰਲਾ_h, l.wi->ਬਾਹਰਲਾ_w); 
        cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l.wg->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, batch, l.wg->ਬਾਹਰਲਾ_c, l.wg->ਬਾਹਰਲਾ_h, l.wg->ਬਾਹਰਲਾ_w); 
        cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l.wo->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, batch, l.wo->ਬਾਹਰਲਾ_c, l.wo->ਬਾਹਰਲਾ_h, l.wo->ਬਾਹਰਲਾ_w); 

        cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l.uf->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, batch, l.uf->ਬਾਹਰਲਾ_c, l.uf->ਬਾਹਰਲਾ_h, l.uf->ਬਾਹਰਲਾ_w); 
        cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l.ui->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, batch, l.ui->ਬਾਹਰਲਾ_c, l.ui->ਬਾਹਰਲਾ_h, l.ui->ਬਾਹਰਲਾ_w); 
        cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l.ug->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, batch, l.ug->ਬਾਹਰਲਾ_c, l.ug->ਬਾਹਰਲਾ_h, l.ug->ਬਾਹਰਲਾ_w); 
        cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l.uo->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, batch, l.uo->ਬਾਹਰਲਾ_c, l.uo->ਬਾਹਰਲਾ_h, l.uo->ਬਾਹਰਲਾ_w); 
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ

    ਵਾਪਸ l;
}

ਖਾਲੀ update_lstm_layer(layer l, update_ਦਲੀਲਾਂ a)
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

ਖਾਲੀ forward_lstm_layer(layer l, ਜਾਲ state)
{
    ਜਾਲ s = { 0 };
    s.train = state.train;
    ਪੂਰਨ i;
    layer wf = *(l.wf);
    layer wi = *(l.wi);
    layer wg = *(l.wg);
    layer wo = *(l.wo);

    layer uf = *(l.uf);
    layer ui = *(l.ui);
    layer ug = *(l.ug);
    layer uo = *(l.uo);

    fill_cpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, wf.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, wi.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, wg.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, wo.delta, 1);

    fill_cpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, uf.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, ui.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, ug.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, uo.delta, 1);
    ਜੇ (state.train) {
        fill_cpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, l.delta, 1);
    }

    ਲਈ (i = 0; i < l.ਕਦਮਾਂ; ++i) {
        s.ਨਿਵੇਸ਼ = l.h_cpu;
        forward_connected_layer(wf, s);							
        forward_connected_layer(wi, s);							
        forward_connected_layer(wg, s);							
        forward_connected_layer(wo, s);							

        s.ਨਿਵੇਸ਼ = state.ਨਿਵੇਸ਼;
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

        copy_cpu(l.outputs*l.batch, l.i_cpu, 1, l.ਆਰਜ਼ੀ_cpu, 1);		
        mul_cpu(l.outputs*l.batch, l.g_cpu, 1, l.ਆਰਜ਼ੀ_cpu, 1);		
        mul_cpu(l.outputs*l.batch, l.f_cpu, 1, l.c_cpu, 1);			
        axpy_cpu(l.outputs*l.batch, 1, l.ਆਰਜ਼ੀ_cpu, 1, l.c_cpu, 1);	

        copy_cpu(l.outputs*l.batch, l.c_cpu, 1, l.h_cpu, 1);			
        activate_array(l.h_cpu, l.outputs*l.batch, TANH);		
        mul_cpu(l.outputs*l.batch, l.o_cpu, 1, l.h_cpu, 1);	

        copy_cpu(l.outputs*l.batch, l.c_cpu, 1, l.cell_cpu, 1);		
        copy_cpu(l.outputs*l.batch, l.h_cpu, 1, l.output, 1);

        state.ਨਿਵੇਸ਼ += l.inputs*l.batch;
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

ਖਾਲੀ backward_lstm_layer(layer l, ਜਾਲ state)
{
    ਜਾਲ s = { 0 };
    s.train = state.train;
    ਪੂਰਨ i;
    layer wf = *(l.wf);
    layer wi = *(l.wi);
    layer wg = *(l.wg);
    layer wo = *(l.wo);

    layer uf = *(l.uf);
    layer ui = *(l.ui);
    layer ug = *(l.ug);
    layer uo = *(l.uo);

    increment_layer(&wf, l.ਕਦਮਾਂ - 1);
    increment_layer(&wi, l.ਕਦਮਾਂ - 1);
    increment_layer(&wg, l.ਕਦਮਾਂ - 1);
    increment_layer(&wo, l.ਕਦਮਾਂ - 1);

    increment_layer(&uf, l.ਕਦਮਾਂ - 1);
    increment_layer(&ui, l.ਕਦਮਾਂ - 1);
    increment_layer(&ug, l.ਕਦਮਾਂ - 1);
    increment_layer(&uo, l.ਕਦਮਾਂ - 1);

    state.ਨਿਵੇਸ਼ += l.inputs*l.batch*(l.ਕਦਮਾਂ - 1);
    ਜੇ (state.delta) state.delta += l.inputs*l.batch*(l.ਕਦਮਾਂ - 1);

    l.output += l.outputs*l.batch*(l.ਕਦਮਾਂ - 1);
    l.cell_cpu += l.outputs*l.batch*(l.ਕਦਮਾਂ - 1);
    l.delta += l.outputs*l.batch*(l.ਕਦਮਾਂ - 1);

    ਲਈ (i = l.ਕਦਮਾਂ - 1; i >= 0; --i) {
        ਜੇ (i != 0) copy_cpu(l.outputs*l.batch, l.cell_cpu - l.outputs*l.batch, 1, l.prev_cell_cpu, 1);
        copy_cpu(l.outputs*l.batch, l.cell_cpu, 1, l.c_cpu, 1);
        ਜੇ (i != 0) copy_cpu(l.outputs*l.batch, l.output - l.outputs*l.batch, 1, l.prev_state_cpu, 1);
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

        copy_cpu(l.outputs*l.batch, l.c_cpu, 1, l.ਆਰਜ਼ੀ_cpu, 1);			
        activate_array(l.ਆਰਜ਼ੀ_cpu, l.outputs*l.batch, TANH);			

        copy_cpu(l.outputs*l.batch, l.temp3_cpu, 1, l.temp2_cpu, 1);		
        mul_cpu(l.outputs*l.batch, l.o_cpu, 1, l.temp2_cpu, 1);			

        gradient_array(l.ਆਰਜ਼ੀ_cpu, l.outputs*l.batch, TANH, l.temp2_cpu);
        axpy_cpu(l.outputs*l.batch, 1, l.dc_cpu, 1, l.temp2_cpu, 1);		

        copy_cpu(l.outputs*l.batch, l.c_cpu, 1, l.ਆਰਜ਼ੀ_cpu, 1);			
        activate_array(l.ਆਰਜ਼ੀ_cpu, l.outputs*l.batch, TANH);			
        mul_cpu(l.outputs*l.batch, l.temp3_cpu, 1, l.ਆਰਜ਼ੀ_cpu, 1);		
        gradient_array(l.o_cpu, l.outputs*l.batch, LOGISTIC, l.ਆਰਜ਼ੀ_cpu);
        copy_cpu(l.outputs*l.batch, l.ਆਰਜ਼ੀ_cpu, 1, wo.delta, 1);
        s.ਨਿਵੇਸ਼ = l.prev_state_cpu;
        s.delta = l.dh_cpu;															
        backward_connected_layer(wo, s);	

        copy_cpu(l.outputs*l.batch, l.ਆਰਜ਼ੀ_cpu, 1, uo.delta, 1);
        s.ਨਿਵੇਸ਼ = state.ਨਿਵੇਸ਼;
        s.delta = state.delta;
        backward_connected_layer(uo, s);									

        copy_cpu(l.outputs*l.batch, l.temp2_cpu, 1, l.ਆਰਜ਼ੀ_cpu, 1);			
        mul_cpu(l.outputs*l.batch, l.i_cpu, 1, l.ਆਰਜ਼ੀ_cpu, 1);				
        gradient_array(l.g_cpu, l.outputs*l.batch, TANH, l.ਆਰਜ਼ੀ_cpu);		
        copy_cpu(l.outputs*l.batch, l.ਆਰਜ਼ੀ_cpu, 1, wg.delta, 1);
        s.ਨਿਵੇਸ਼ = l.prev_state_cpu;
        s.delta = l.dh_cpu;														
        backward_connected_layer(wg, s);	

        copy_cpu(l.outputs*l.batch, l.ਆਰਜ਼ੀ_cpu, 1, ug.delta, 1);
        s.ਨਿਵੇਸ਼ = state.ਨਿਵੇਸ਼;
        s.delta = state.delta;
        backward_connected_layer(ug, s);																

        copy_cpu(l.outputs*l.batch, l.temp2_cpu, 1, l.ਆਰਜ਼ੀ_cpu, 1);			
        mul_cpu(l.outputs*l.batch, l.g_cpu, 1, l.ਆਰਜ਼ੀ_cpu, 1);				
        gradient_array(l.i_cpu, l.outputs*l.batch, LOGISTIC, l.ਆਰਜ਼ੀ_cpu);	
        copy_cpu(l.outputs*l.batch, l.ਆਰਜ਼ੀ_cpu, 1, wi.delta, 1);
        s.ਨਿਵੇਸ਼ = l.prev_state_cpu;
        s.delta = l.dh_cpu;
        backward_connected_layer(wi, s);						

        copy_cpu(l.outputs*l.batch, l.ਆਰਜ਼ੀ_cpu, 1, ui.delta, 1);
        s.ਨਿਵੇਸ਼ = state.ਨਿਵੇਸ਼;
        s.delta = state.delta;
        backward_connected_layer(ui, s);									

        copy_cpu(l.outputs*l.batch, l.temp2_cpu, 1, l.ਆਰਜ਼ੀ_cpu, 1);		
        mul_cpu(l.outputs*l.batch, l.prev_cell_cpu, 1, l.ਆਰਜ਼ੀ_cpu, 1);
        gradient_array(l.f_cpu, l.outputs*l.batch, LOGISTIC, l.ਆਰਜ਼ੀ_cpu);
        copy_cpu(l.outputs*l.batch, l.ਆਰਜ਼ੀ_cpu, 1, wf.delta, 1);
        s.ਨਿਵੇਸ਼ = l.prev_state_cpu;
        s.delta = l.dh_cpu;
        backward_connected_layer(wf, s);						

        copy_cpu(l.outputs*l.batch, l.ਆਰਜ਼ੀ_cpu, 1, uf.delta, 1);
        s.ਨਿਵੇਸ਼ = state.ਨਿਵੇਸ਼;
        s.delta = state.delta;
        backward_connected_layer(uf, s);									

        copy_cpu(l.outputs*l.batch, l.temp2_cpu, 1, l.ਆਰਜ਼ੀ_cpu, 1);			
        mul_cpu(l.outputs*l.batch, l.f_cpu, 1, l.ਆਰਜ਼ੀ_cpu, 1);				
        copy_cpu(l.outputs*l.batch, l.ਆਰਜ਼ੀ_cpu, 1, l.dc_cpu, 1);				

        state.ਨਿਵੇਸ਼ -= l.inputs*l.batch;
        ਜੇ (state.delta) state.delta -= l.inputs*l.batch;
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

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ update_lstm_layer_gpu(layer l, update_ਦਲੀਲਾਂ a)
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

ਖਾਲੀ forward_lstm_layer_gpu(layer l, ਜਾਲ state)
{
    ਜਾਲ s = { 0 };
    s.train = state.train;
    ਪੂਰਨ i;
    layer wf = *(l.wf);
    layer wi = *(l.wi);
    layer wg = *(l.wg);
    layer wo = *(l.wo);

    layer uf = *(l.uf);
    layer ui = *(l.ui);
    layer ug = *(l.ug);
    layer uo = *(l.uo);

    fill_gpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, wf.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, wi.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, wg.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, wo.delta_gpu, 1);

    fill_gpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, uf.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, ui.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, ug.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, uo.delta_gpu, 1);
    ਜੇ (state.train) {
        fill_gpu(l.outputs * l.batch * l.ਕਦਮਾਂ, 0, l.delta_gpu, 1);
    }

    ਲਈ (i = 0; i < l.ਕਦਮਾਂ; ++i) {
        s.ਨਿਵੇਸ਼_gpu = l.h_gpu;
        forward_connected_layer_gpu(wf, s);							
        forward_connected_layer_gpu(wi, s);							
        forward_connected_layer_gpu(wg, s);							
        forward_connected_layer_gpu(wo, s);							

        s.ਨਿਵੇਸ਼_gpu = state.ਨਿਵੇਸ਼_gpu;
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

        copy_gpu(l.outputs*l.batch, l.i_gpu, 1, l.ਆਰਜ਼ੀ_gpu, 1);		
        mul_gpu(l.outputs*l.batch, l.g_gpu, 1, l.ਆਰਜ਼ੀ_gpu, 1);		
        mul_gpu(l.outputs*l.batch, l.f_gpu, 1, l.c_gpu, 1);			
        axpy_gpu(l.outputs*l.batch, 1, l.ਆਰਜ਼ੀ_gpu, 1, l.c_gpu, 1);	

        copy_gpu(l.outputs*l.batch, l.c_gpu, 1, l.h_gpu, 1);			
        activate_array_gpu(l.h_gpu, l.outputs*l.batch, TANH);		
        mul_gpu(l.outputs*l.batch, l.o_gpu, 1, l.h_gpu, 1);	

        copy_gpu(l.outputs*l.batch, l.c_gpu, 1, l.cell_gpu, 1);		
        copy_gpu(l.outputs*l.batch, l.h_gpu, 1, l.output_gpu, 1);

        state.ਨਿਵੇਸ਼_gpu += l.inputs*l.batch;
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

ਖਾਲੀ backward_lstm_layer_gpu(layer l, ਜਾਲ state)
{
    ਜਾਲ s = { 0 };
    s.train = state.train;
    ਪੂਰਨ i;
    layer wf = *(l.wf);
    layer wi = *(l.wi);
    layer wg = *(l.wg);
    layer wo = *(l.wo);

    layer uf = *(l.uf);
    layer ui = *(l.ui);
    layer ug = *(l.ug);
    layer uo = *(l.uo);

    increment_layer(&wf, l.ਕਦਮਾਂ - 1);
    increment_layer(&wi, l.ਕਦਮਾਂ - 1);
    increment_layer(&wg, l.ਕਦਮਾਂ - 1);
    increment_layer(&wo, l.ਕਦਮਾਂ - 1);

    increment_layer(&uf, l.ਕਦਮਾਂ - 1);
    increment_layer(&ui, l.ਕਦਮਾਂ - 1);
    increment_layer(&ug, l.ਕਦਮਾਂ - 1);
    increment_layer(&uo, l.ਕਦਮਾਂ - 1);

    state.ਨਿਵੇਸ਼_gpu += l.inputs*l.batch*(l.ਕਦਮਾਂ - 1);
    ਜੇ (state.delta_gpu) state.delta_gpu += l.inputs*l.batch*(l.ਕਦਮਾਂ - 1);

    l.output_gpu += l.outputs*l.batch*(l.ਕਦਮਾਂ - 1);
    l.cell_gpu += l.outputs*l.batch*(l.ਕਦਮਾਂ - 1);
    l.delta_gpu += l.outputs*l.batch*(l.ਕਦਮਾਂ - 1);

    ਲਈ (i = l.ਕਦਮਾਂ - 1; i >= 0; --i) {
        ਜੇ (i != 0) copy_gpu(l.outputs*l.batch, l.cell_gpu - l.outputs*l.batch, 1, l.prev_cell_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.cell_gpu, 1, l.c_gpu, 1);
        ਜੇ (i != 0) copy_gpu(l.outputs*l.batch, l.output_gpu - l.outputs*l.batch, 1, l.prev_state_gpu, 1);
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

        copy_gpu(l.outputs*l.batch, l.c_gpu, 1, l.ਆਰਜ਼ੀ_gpu, 1);			
        activate_array_gpu(l.ਆਰਜ਼ੀ_gpu, l.outputs*l.batch, TANH);			

        copy_gpu(l.outputs*l.batch, l.temp3_gpu, 1, l.temp2_gpu, 1);		
        mul_gpu(l.outputs*l.batch, l.o_gpu, 1, l.temp2_gpu, 1);			

        gradient_array_gpu(l.ਆਰਜ਼ੀ_gpu, l.outputs*l.batch, TANH, l.temp2_gpu);
        axpy_gpu(l.outputs*l.batch, 1, l.dc_gpu, 1, l.temp2_gpu, 1);		

        copy_gpu(l.outputs*l.batch, l.c_gpu, 1, l.ਆਰਜ਼ੀ_gpu, 1);			
        activate_array_gpu(l.ਆਰਜ਼ੀ_gpu, l.outputs*l.batch, TANH);			
        mul_gpu(l.outputs*l.batch, l.temp3_gpu, 1, l.ਆਰਜ਼ੀ_gpu, 1);		
        gradient_array_gpu(l.o_gpu, l.outputs*l.batch, LOGISTIC, l.ਆਰਜ਼ੀ_gpu);
        copy_gpu(l.outputs*l.batch, l.ਆਰਜ਼ੀ_gpu, 1, wo.delta_gpu, 1);
        s.ਨਿਵੇਸ਼_gpu = l.prev_state_gpu;
        s.delta_gpu = l.dh_gpu;															
        backward_connected_layer_gpu(wo, s);	

        copy_gpu(l.outputs*l.batch, l.ਆਰਜ਼ੀ_gpu, 1, uo.delta_gpu, 1);
        s.ਨਿਵੇਸ਼_gpu = state.ਨਿਵੇਸ਼_gpu;
        s.delta_gpu = state.delta_gpu;
        backward_connected_layer_gpu(uo, s);									

        copy_gpu(l.outputs*l.batch, l.temp2_gpu, 1, l.ਆਰਜ਼ੀ_gpu, 1);			
        mul_gpu(l.outputs*l.batch, l.i_gpu, 1, l.ਆਰਜ਼ੀ_gpu, 1);				
        gradient_array_gpu(l.g_gpu, l.outputs*l.batch, TANH, l.ਆਰਜ਼ੀ_gpu);		
        copy_gpu(l.outputs*l.batch, l.ਆਰਜ਼ੀ_gpu, 1, wg.delta_gpu, 1);
        s.ਨਿਵੇਸ਼_gpu = l.prev_state_gpu;
        s.delta_gpu = l.dh_gpu;														
        backward_connected_layer_gpu(wg, s);	

        copy_gpu(l.outputs*l.batch, l.ਆਰਜ਼ੀ_gpu, 1, ug.delta_gpu, 1);
        s.ਨਿਵੇਸ਼_gpu = state.ਨਿਵੇਸ਼_gpu;
        s.delta_gpu = state.delta_gpu;
        backward_connected_layer_gpu(ug, s);																

        copy_gpu(l.outputs*l.batch, l.temp2_gpu, 1, l.ਆਰਜ਼ੀ_gpu, 1);			
        mul_gpu(l.outputs*l.batch, l.g_gpu, 1, l.ਆਰਜ਼ੀ_gpu, 1);				
        gradient_array_gpu(l.i_gpu, l.outputs*l.batch, LOGISTIC, l.ਆਰਜ਼ੀ_gpu);	
        copy_gpu(l.outputs*l.batch, l.ਆਰਜ਼ੀ_gpu, 1, wi.delta_gpu, 1);
        s.ਨਿਵੇਸ਼_gpu = l.prev_state_gpu;
        s.delta_gpu = l.dh_gpu;
        backward_connected_layer_gpu(wi, s);						

        copy_gpu(l.outputs*l.batch, l.ਆਰਜ਼ੀ_gpu, 1, ui.delta_gpu, 1);
        s.ਨਿਵੇਸ਼_gpu = state.ਨਿਵੇਸ਼_gpu;
        s.delta_gpu = state.delta_gpu;
        backward_connected_layer_gpu(ui, s);									

        copy_gpu(l.outputs*l.batch, l.temp2_gpu, 1, l.ਆਰਜ਼ੀ_gpu, 1);		
        mul_gpu(l.outputs*l.batch, l.prev_cell_gpu, 1, l.ਆਰਜ਼ੀ_gpu, 1);
        gradient_array_gpu(l.f_gpu, l.outputs*l.batch, LOGISTIC, l.ਆਰਜ਼ੀ_gpu);
        copy_gpu(l.outputs*l.batch, l.ਆਰਜ਼ੀ_gpu, 1, wf.delta_gpu, 1);
        s.ਨਿਵੇਸ਼_gpu = l.prev_state_gpu;
        s.delta_gpu = l.dh_gpu;
        backward_connected_layer_gpu(wf, s);						

        copy_gpu(l.outputs*l.batch, l.ਆਰਜ਼ੀ_gpu, 1, uf.delta_gpu, 1);
        s.ਨਿਵੇਸ਼_gpu = state.ਨਿਵੇਸ਼_gpu;
        s.delta_gpu = state.delta_gpu;
        backward_connected_layer_gpu(uf, s);									

        copy_gpu(l.outputs*l.batch, l.temp2_gpu, 1, l.ਆਰਜ਼ੀ_gpu, 1);			
        mul_gpu(l.outputs*l.batch, l.f_gpu, 1, l.ਆਰਜ਼ੀ_gpu, 1);				
        copy_gpu(l.outputs*l.batch, l.ਆਰਜ਼ੀ_gpu, 1, l.dc_gpu, 1);				

        state.ਨਿਵੇਸ਼_gpu -= l.inputs*l.batch;
        ਜੇ (state.delta_gpu) state.delta_gpu -= l.inputs*l.batch;
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
#ਜੇ_ਅੰਤ

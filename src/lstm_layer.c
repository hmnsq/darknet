#అన్తర్భావ "lstm_layer.h"
#అన్తర్భావ "connected_layer.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "cuda.h"
#అన్తర్భావ "blas.h"
#అన్తర్భావ "gemm.h"

#అన్తర్భావ <math.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <string.h>

స్థిర శూన్య increment_layer(layer *l, పూర్ణాంకం అడుగులు)
{
    పూర్ణాంకం అంకె = l->outputs*l->batch*అడుగులు;
    l->output += అంకె;
    l->delta += అంకె;
    l->x += అంకె;
    l->x_norm += అంకె;

#నిర్వచితమైతే GPU
    l->output_gpu += అంకె;
    l->delta_gpu += అంకె;
    l->x_gpu += అంకె;
    l->x_norm_gpu += అంకె;
#అయితే_ముగింపు
}

layer make_lstm_layer(పూర్ణాంకం batch, పూర్ణాంకం inputs, పూర్ణాంకం outputs, పూర్ణాంకం అడుగులు, పూర్ణాంకం batch_normalize, పూర్ణాంకం adam)
{
    fprintf(stderr, "LSTM Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / అడుగులు;
    layer l = { 0 };
    l.batch = batch;
    l.type = LSTM;
    l.అడుగులు = అడుగులు;
    l.inputs = inputs;

    l.uf = జ్ఞాపకం_ఇవ్వు(పరిమాణం(layer));
    fprintf(stderr, "\t\t");
    *(l.uf) = make_connected_layer(batch*అడుగులు, inputs, outputs, రేఖీయ, batch_normalize, adam);
    l.uf->batch = batch;

    l.ui = జ్ఞాపకం_ఇవ్వు(పరిమాణం(layer));
    fprintf(stderr, "\t\t");
    *(l.ui) = make_connected_layer(batch*అడుగులు, inputs, outputs, రేఖీయ, batch_normalize, adam);
    l.ui->batch = batch;

    l.ug = జ్ఞాపకం_ఇవ్వు(పరిమాణం(layer));
    fprintf(stderr, "\t\t");
    *(l.ug) = make_connected_layer(batch*అడుగులు, inputs, outputs, రేఖీయ, batch_normalize, adam);
    l.ug->batch = batch;

    l.uo = జ్ఞాపకం_ఇవ్వు(పరిమాణం(layer));
    fprintf(stderr, "\t\t");
    *(l.uo) = make_connected_layer(batch*అడుగులు, inputs, outputs, రేఖీయ, batch_normalize, adam);
    l.uo->batch = batch;

    l.wf = జ్ఞాపకం_ఇవ్వు(పరిమాణం(layer));
    fprintf(stderr, "\t\t");
    *(l.wf) = make_connected_layer(batch*అడుగులు, outputs, outputs, రేఖీయ, batch_normalize, adam);
    l.wf->batch = batch;

    l.wi = జ్ఞాపకం_ఇవ్వు(పరిమాణం(layer));
    fprintf(stderr, "\t\t");
    *(l.wi) = make_connected_layer(batch*అడుగులు, outputs, outputs, రేఖీయ, batch_normalize, adam);
    l.wi->batch = batch;

    l.wg = జ్ఞాపకం_ఇవ్వు(పరిమాణం(layer));
    fprintf(stderr, "\t\t");
    *(l.wg) = make_connected_layer(batch*అడుగులు, outputs, outputs, రేఖీయ, batch_normalize, adam);
    l.wg->batch = batch;

    l.wo = జ్ఞాపకం_ఇవ్వు(పరిమాణం(layer));
    fprintf(stderr, "\t\t");
    *(l.wo) = make_connected_layer(batch*అడుగులు, outputs, outputs, రేఖీయ, batch_normalize, adam);
    l.wo->batch = batch;

    l.batch_normalize = batch_normalize;
    l.outputs = outputs;

    l.output = సున్నా_జ్ఞాపకం_ఇవ్వు(outputs*batch*అడుగులు, పరిమాణం(దశాంశం));
    l.state = సున్నా_జ్ఞాపకం_ఇవ్వు(outputs*batch, పరిమాణం(దశాంశం));

    l.forward = forward_lstm_layer;
    l.update = update_lstm_layer;

    l.prev_state_cpu =  సున్నా_జ్ఞాపకం_ఇవ్వు(batch*outputs, పరిమాణం(దశాంశం));
    l.prev_cell_cpu =   సున్నా_జ్ఞాపకం_ఇవ్వు(batch*outputs, పరిమాణం(దశాంశం));
    l.cell_cpu =        సున్నా_జ్ఞాపకం_ఇవ్వు(batch*outputs*అడుగులు, పరిమాణం(దశాంశం));

    l.f_cpu =           సున్నా_జ్ఞాపకం_ఇవ్వు(batch*outputs, పరిమాణం(దశాంశం));
    l.i_cpu =           సున్నా_జ్ఞాపకం_ఇవ్వు(batch*outputs, పరిమాణం(దశాంశం));
    l.g_cpu =           సున్నా_జ్ఞాపకం_ఇవ్వు(batch*outputs, పరిమాణం(దశాంశం));
    l.o_cpu =           సున్నా_జ్ఞాపకం_ఇవ్వు(batch*outputs, పరిమాణం(దశాంశం));
    l.c_cpu =           సున్నా_జ్ఞాపకం_ఇవ్వు(batch*outputs, పరిమాణం(దశాంశం));
    l.h_cpu =           సున్నా_జ్ఞాపకం_ఇవ్వు(batch*outputs, పరిమాణం(దశాంశం));
    l.తాత్కాలిక_cpu =        సున్నా_జ్ఞాపకం_ఇవ్వు(batch*outputs, పరిమాణం(దశాంశం));
    l.temp2_cpu =       సున్నా_జ్ఞాపకం_ఇవ్వు(batch*outputs, పరిమాణం(దశాంశం));
    l.temp3_cpu =       సున్నా_జ్ఞాపకం_ఇవ్వు(batch*outputs, పరిమాణం(దశాంశం));
    l.dc_cpu =          సున్నా_జ్ఞాపకం_ఇవ్వు(batch*outputs, పరిమాణం(దశాంశం));
    l.dh_cpu =          సున్నా_జ్ఞాపకం_ఇవ్వు(batch*outputs, పరిమాణం(దశాంశం));

#నిర్వచితమైతే GPU
    l.forward_gpu = forward_lstm_layer_gpu;
    l.backward_gpu = backward_lstm_layer_gpu;
    l.update_gpu = update_lstm_layer_gpu;

    l.output_gpu = cuda_make_array(0, batch*outputs*అడుగులు);
    l.delta_gpu = cuda_make_array(0, batch*l.outputs*అడుగులు);

    l.prev_state_gpu = cuda_make_array(0, batch*outputs);
    l.prev_cell_gpu = cuda_make_array(0, batch*outputs);
    l.cell_gpu = cuda_make_array(0, batch*outputs*అడుగులు);

    l.f_gpu = cuda_make_array(0, batch*outputs);
    l.i_gpu = cuda_make_array(0, batch*outputs);
    l.g_gpu = cuda_make_array(0, batch*outputs);
    l.o_gpu = cuda_make_array(0, batch*outputs);
    l.c_gpu = cuda_make_array(0, batch*outputs);
    l.h_gpu = cuda_make_array(0, batch*outputs);
    l.తాత్కాలిక_gpu =  cuda_make_array(0, batch*outputs);
    l.temp2_gpu = cuda_make_array(0, batch*outputs);
    l.temp3_gpu = cuda_make_array(0, batch*outputs);
    l.dc_gpu = cuda_make_array(0, batch*outputs);
    l.dh_gpu = cuda_make_array(0, batch*outputs);
#నిర్వచితమైతే CUDNN
        cudnn_సమితి_Tensor4d_Descriptor(l.wf->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, batch, l.wf->బహిర్గతం_c, l.wf->బహిర్గతం_h, l.wf->బహిర్గతం_w); 
        cudnn_సమితి_Tensor4d_Descriptor(l.wi->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, batch, l.wi->బహిర్గతం_c, l.wi->బహిర్గతం_h, l.wi->బహిర్గతం_w); 
        cudnn_సమితి_Tensor4d_Descriptor(l.wg->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, batch, l.wg->బహిర్గతం_c, l.wg->బహిర్గతం_h, l.wg->బహిర్గతం_w); 
        cudnn_సమితి_Tensor4d_Descriptor(l.wo->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, batch, l.wo->బహిర్గతం_c, l.wo->బహిర్గతం_h, l.wo->బహిర్గతం_w); 

        cudnn_సమితి_Tensor4d_Descriptor(l.uf->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, batch, l.uf->బహిర్గతం_c, l.uf->బహిర్గతం_h, l.uf->బహిర్గతం_w); 
        cudnn_సమితి_Tensor4d_Descriptor(l.ui->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, batch, l.ui->బహిర్గతం_c, l.ui->బహిర్గతం_h, l.ui->బహిర్గతం_w); 
        cudnn_సమితి_Tensor4d_Descriptor(l.ug->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, batch, l.ug->బహిర్గతం_c, l.ug->బహిర్గతం_h, l.ug->బహిర్గతం_w); 
        cudnn_సమితి_Tensor4d_Descriptor(l.uo->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, batch, l.uo->బహిర్గతం_c, l.uo->బహిర్గతం_h, l.uo->బహిర్గతం_w); 
#అయితే_ముగింపు

#అయితే_ముగింపు

    తిరిగి l;
}

శూన్య update_lstm_layer(layer l, update_వాదనలు a)
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

శూన్య forward_lstm_layer(layer l, జాలం state)
{
    జాలం s = { 0 };
    s.train = state.train;
    పూర్ణాంకం i;
    layer wf = *(l.wf);
    layer wi = *(l.wi);
    layer wg = *(l.wg);
    layer wo = *(l.wo);

    layer uf = *(l.uf);
    layer ui = *(l.ui);
    layer ug = *(l.ug);
    layer uo = *(l.uo);

    fill_cpu(l.outputs * l.batch * l.అడుగులు, 0, wf.delta, 1);
    fill_cpu(l.outputs * l.batch * l.అడుగులు, 0, wi.delta, 1);
    fill_cpu(l.outputs * l.batch * l.అడుగులు, 0, wg.delta, 1);
    fill_cpu(l.outputs * l.batch * l.అడుగులు, 0, wo.delta, 1);

    fill_cpu(l.outputs * l.batch * l.అడుగులు, 0, uf.delta, 1);
    fill_cpu(l.outputs * l.batch * l.అడుగులు, 0, ui.delta, 1);
    fill_cpu(l.outputs * l.batch * l.అడుగులు, 0, ug.delta, 1);
    fill_cpu(l.outputs * l.batch * l.అడుగులు, 0, uo.delta, 1);
    ఒకవేళ (state.train) {
        fill_cpu(l.outputs * l.batch * l.అడుగులు, 0, l.delta, 1);
    }

    కొరకు (i = 0; i < l.అడుగులు; ++i) {
        s.ఆదానం = l.h_cpu;
        forward_connected_layer(wf, s);							
        forward_connected_layer(wi, s);							
        forward_connected_layer(wg, s);							
        forward_connected_layer(wo, s);							

        s.ఆదానం = state.ఆదానం;
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

        copy_cpu(l.outputs*l.batch, l.i_cpu, 1, l.తాత్కాలిక_cpu, 1);		
        mul_cpu(l.outputs*l.batch, l.g_cpu, 1, l.తాత్కాలిక_cpu, 1);		
        mul_cpu(l.outputs*l.batch, l.f_cpu, 1, l.c_cpu, 1);			
        axpy_cpu(l.outputs*l.batch, 1, l.తాత్కాలిక_cpu, 1, l.c_cpu, 1);	

        copy_cpu(l.outputs*l.batch, l.c_cpu, 1, l.h_cpu, 1);			
        activate_array(l.h_cpu, l.outputs*l.batch, TANH);		
        mul_cpu(l.outputs*l.batch, l.o_cpu, 1, l.h_cpu, 1);	

        copy_cpu(l.outputs*l.batch, l.c_cpu, 1, l.cell_cpu, 1);		
        copy_cpu(l.outputs*l.batch, l.h_cpu, 1, l.output, 1);

        state.ఆదానం += l.inputs*l.batch;
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

శూన్య backward_lstm_layer(layer l, జాలం state)
{
    జాలం s = { 0 };
    s.train = state.train;
    పూర్ణాంకం i;
    layer wf = *(l.wf);
    layer wi = *(l.wi);
    layer wg = *(l.wg);
    layer wo = *(l.wo);

    layer uf = *(l.uf);
    layer ui = *(l.ui);
    layer ug = *(l.ug);
    layer uo = *(l.uo);

    increment_layer(&wf, l.అడుగులు - 1);
    increment_layer(&wi, l.అడుగులు - 1);
    increment_layer(&wg, l.అడుగులు - 1);
    increment_layer(&wo, l.అడుగులు - 1);

    increment_layer(&uf, l.అడుగులు - 1);
    increment_layer(&ui, l.అడుగులు - 1);
    increment_layer(&ug, l.అడుగులు - 1);
    increment_layer(&uo, l.అడుగులు - 1);

    state.ఆదానం += l.inputs*l.batch*(l.అడుగులు - 1);
    ఒకవేళ (state.delta) state.delta += l.inputs*l.batch*(l.అడుగులు - 1);

    l.output += l.outputs*l.batch*(l.అడుగులు - 1);
    l.cell_cpu += l.outputs*l.batch*(l.అడుగులు - 1);
    l.delta += l.outputs*l.batch*(l.అడుగులు - 1);

    కొరకు (i = l.అడుగులు - 1; i >= 0; --i) {
        ఒకవేళ (i != 0) copy_cpu(l.outputs*l.batch, l.cell_cpu - l.outputs*l.batch, 1, l.prev_cell_cpu, 1);
        copy_cpu(l.outputs*l.batch, l.cell_cpu, 1, l.c_cpu, 1);
        ఒకవేళ (i != 0) copy_cpu(l.outputs*l.batch, l.output - l.outputs*l.batch, 1, l.prev_state_cpu, 1);
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

        copy_cpu(l.outputs*l.batch, l.c_cpu, 1, l.తాత్కాలిక_cpu, 1);			
        activate_array(l.తాత్కాలిక_cpu, l.outputs*l.batch, TANH);			

        copy_cpu(l.outputs*l.batch, l.temp3_cpu, 1, l.temp2_cpu, 1);		
        mul_cpu(l.outputs*l.batch, l.o_cpu, 1, l.temp2_cpu, 1);			

        gradient_array(l.తాత్కాలిక_cpu, l.outputs*l.batch, TANH, l.temp2_cpu);
        axpy_cpu(l.outputs*l.batch, 1, l.dc_cpu, 1, l.temp2_cpu, 1);		

        copy_cpu(l.outputs*l.batch, l.c_cpu, 1, l.తాత్కాలిక_cpu, 1);			
        activate_array(l.తాత్కాలిక_cpu, l.outputs*l.batch, TANH);			
        mul_cpu(l.outputs*l.batch, l.temp3_cpu, 1, l.తాత్కాలిక_cpu, 1);		
        gradient_array(l.o_cpu, l.outputs*l.batch, LOGISTIC, l.తాత్కాలిక_cpu);
        copy_cpu(l.outputs*l.batch, l.తాత్కాలిక_cpu, 1, wo.delta, 1);
        s.ఆదానం = l.prev_state_cpu;
        s.delta = l.dh_cpu;															
        backward_connected_layer(wo, s);	

        copy_cpu(l.outputs*l.batch, l.తాత్కాలిక_cpu, 1, uo.delta, 1);
        s.ఆదానం = state.ఆదానం;
        s.delta = state.delta;
        backward_connected_layer(uo, s);									

        copy_cpu(l.outputs*l.batch, l.temp2_cpu, 1, l.తాత్కాలిక_cpu, 1);			
        mul_cpu(l.outputs*l.batch, l.i_cpu, 1, l.తాత్కాలిక_cpu, 1);				
        gradient_array(l.g_cpu, l.outputs*l.batch, TANH, l.తాత్కాలిక_cpu);		
        copy_cpu(l.outputs*l.batch, l.తాత్కాలిక_cpu, 1, wg.delta, 1);
        s.ఆదానం = l.prev_state_cpu;
        s.delta = l.dh_cpu;														
        backward_connected_layer(wg, s);	

        copy_cpu(l.outputs*l.batch, l.తాత్కాలిక_cpu, 1, ug.delta, 1);
        s.ఆదానం = state.ఆదానం;
        s.delta = state.delta;
        backward_connected_layer(ug, s);																

        copy_cpu(l.outputs*l.batch, l.temp2_cpu, 1, l.తాత్కాలిక_cpu, 1);			
        mul_cpu(l.outputs*l.batch, l.g_cpu, 1, l.తాత్కాలిక_cpu, 1);				
        gradient_array(l.i_cpu, l.outputs*l.batch, LOGISTIC, l.తాత్కాలిక_cpu);	
        copy_cpu(l.outputs*l.batch, l.తాత్కాలిక_cpu, 1, wi.delta, 1);
        s.ఆదానం = l.prev_state_cpu;
        s.delta = l.dh_cpu;
        backward_connected_layer(wi, s);						

        copy_cpu(l.outputs*l.batch, l.తాత్కాలిక_cpu, 1, ui.delta, 1);
        s.ఆదానం = state.ఆదానం;
        s.delta = state.delta;
        backward_connected_layer(ui, s);									

        copy_cpu(l.outputs*l.batch, l.temp2_cpu, 1, l.తాత్కాలిక_cpu, 1);		
        mul_cpu(l.outputs*l.batch, l.prev_cell_cpu, 1, l.తాత్కాలిక_cpu, 1);
        gradient_array(l.f_cpu, l.outputs*l.batch, LOGISTIC, l.తాత్కాలిక_cpu);
        copy_cpu(l.outputs*l.batch, l.తాత్కాలిక_cpu, 1, wf.delta, 1);
        s.ఆదానం = l.prev_state_cpu;
        s.delta = l.dh_cpu;
        backward_connected_layer(wf, s);						

        copy_cpu(l.outputs*l.batch, l.తాత్కాలిక_cpu, 1, uf.delta, 1);
        s.ఆదానం = state.ఆదానం;
        s.delta = state.delta;
        backward_connected_layer(uf, s);									

        copy_cpu(l.outputs*l.batch, l.temp2_cpu, 1, l.తాత్కాలిక_cpu, 1);			
        mul_cpu(l.outputs*l.batch, l.f_cpu, 1, l.తాత్కాలిక_cpu, 1);				
        copy_cpu(l.outputs*l.batch, l.తాత్కాలిక_cpu, 1, l.dc_cpu, 1);				

        state.ఆదానం -= l.inputs*l.batch;
        ఒకవేళ (state.delta) state.delta -= l.inputs*l.batch;
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

#నిర్వచితమైతే GPU
శూన్య update_lstm_layer_gpu(layer l, update_వాదనలు a)
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

శూన్య forward_lstm_layer_gpu(layer l, జాలం state)
{
    జాలం s = { 0 };
    s.train = state.train;
    పూర్ణాంకం i;
    layer wf = *(l.wf);
    layer wi = *(l.wi);
    layer wg = *(l.wg);
    layer wo = *(l.wo);

    layer uf = *(l.uf);
    layer ui = *(l.ui);
    layer ug = *(l.ug);
    layer uo = *(l.uo);

    fill_gpu(l.outputs * l.batch * l.అడుగులు, 0, wf.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.అడుగులు, 0, wi.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.అడుగులు, 0, wg.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.అడుగులు, 0, wo.delta_gpu, 1);

    fill_gpu(l.outputs * l.batch * l.అడుగులు, 0, uf.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.అడుగులు, 0, ui.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.అడుగులు, 0, ug.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.అడుగులు, 0, uo.delta_gpu, 1);
    ఒకవేళ (state.train) {
        fill_gpu(l.outputs * l.batch * l.అడుగులు, 0, l.delta_gpu, 1);
    }

    కొరకు (i = 0; i < l.అడుగులు; ++i) {
        s.ఆదానం_gpu = l.h_gpu;
        forward_connected_layer_gpu(wf, s);							
        forward_connected_layer_gpu(wi, s);							
        forward_connected_layer_gpu(wg, s);							
        forward_connected_layer_gpu(wo, s);							

        s.ఆదానం_gpu = state.ఆదానం_gpu;
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

        copy_gpu(l.outputs*l.batch, l.i_gpu, 1, l.తాత్కాలిక_gpu, 1);		
        mul_gpu(l.outputs*l.batch, l.g_gpu, 1, l.తాత్కాలిక_gpu, 1);		
        mul_gpu(l.outputs*l.batch, l.f_gpu, 1, l.c_gpu, 1);			
        axpy_gpu(l.outputs*l.batch, 1, l.తాత్కాలిక_gpu, 1, l.c_gpu, 1);	

        copy_gpu(l.outputs*l.batch, l.c_gpu, 1, l.h_gpu, 1);			
        activate_array_gpu(l.h_gpu, l.outputs*l.batch, TANH);		
        mul_gpu(l.outputs*l.batch, l.o_gpu, 1, l.h_gpu, 1);	

        copy_gpu(l.outputs*l.batch, l.c_gpu, 1, l.cell_gpu, 1);		
        copy_gpu(l.outputs*l.batch, l.h_gpu, 1, l.output_gpu, 1);

        state.ఆదానం_gpu += l.inputs*l.batch;
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

శూన్య backward_lstm_layer_gpu(layer l, జాలం state)
{
    జాలం s = { 0 };
    s.train = state.train;
    పూర్ణాంకం i;
    layer wf = *(l.wf);
    layer wi = *(l.wi);
    layer wg = *(l.wg);
    layer wo = *(l.wo);

    layer uf = *(l.uf);
    layer ui = *(l.ui);
    layer ug = *(l.ug);
    layer uo = *(l.uo);

    increment_layer(&wf, l.అడుగులు - 1);
    increment_layer(&wi, l.అడుగులు - 1);
    increment_layer(&wg, l.అడుగులు - 1);
    increment_layer(&wo, l.అడుగులు - 1);

    increment_layer(&uf, l.అడుగులు - 1);
    increment_layer(&ui, l.అడుగులు - 1);
    increment_layer(&ug, l.అడుగులు - 1);
    increment_layer(&uo, l.అడుగులు - 1);

    state.ఆదానం_gpu += l.inputs*l.batch*(l.అడుగులు - 1);
    ఒకవేళ (state.delta_gpu) state.delta_gpu += l.inputs*l.batch*(l.అడుగులు - 1);

    l.output_gpu += l.outputs*l.batch*(l.అడుగులు - 1);
    l.cell_gpu += l.outputs*l.batch*(l.అడుగులు - 1);
    l.delta_gpu += l.outputs*l.batch*(l.అడుగులు - 1);

    కొరకు (i = l.అడుగులు - 1; i >= 0; --i) {
        ఒకవేళ (i != 0) copy_gpu(l.outputs*l.batch, l.cell_gpu - l.outputs*l.batch, 1, l.prev_cell_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.cell_gpu, 1, l.c_gpu, 1);
        ఒకవేళ (i != 0) copy_gpu(l.outputs*l.batch, l.output_gpu - l.outputs*l.batch, 1, l.prev_state_gpu, 1);
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

        copy_gpu(l.outputs*l.batch, l.c_gpu, 1, l.తాత్కాలిక_gpu, 1);			
        activate_array_gpu(l.తాత్కాలిక_gpu, l.outputs*l.batch, TANH);			

        copy_gpu(l.outputs*l.batch, l.temp3_gpu, 1, l.temp2_gpu, 1);		
        mul_gpu(l.outputs*l.batch, l.o_gpu, 1, l.temp2_gpu, 1);			

        gradient_array_gpu(l.తాత్కాలిక_gpu, l.outputs*l.batch, TANH, l.temp2_gpu);
        axpy_gpu(l.outputs*l.batch, 1, l.dc_gpu, 1, l.temp2_gpu, 1);		

        copy_gpu(l.outputs*l.batch, l.c_gpu, 1, l.తాత్కాలిక_gpu, 1);			
        activate_array_gpu(l.తాత్కాలిక_gpu, l.outputs*l.batch, TANH);			
        mul_gpu(l.outputs*l.batch, l.temp3_gpu, 1, l.తాత్కాలిక_gpu, 1);		
        gradient_array_gpu(l.o_gpu, l.outputs*l.batch, LOGISTIC, l.తాత్కాలిక_gpu);
        copy_gpu(l.outputs*l.batch, l.తాత్కాలిక_gpu, 1, wo.delta_gpu, 1);
        s.ఆదానం_gpu = l.prev_state_gpu;
        s.delta_gpu = l.dh_gpu;															
        backward_connected_layer_gpu(wo, s);	

        copy_gpu(l.outputs*l.batch, l.తాత్కాలిక_gpu, 1, uo.delta_gpu, 1);
        s.ఆదానం_gpu = state.ఆదానం_gpu;
        s.delta_gpu = state.delta_gpu;
        backward_connected_layer_gpu(uo, s);									

        copy_gpu(l.outputs*l.batch, l.temp2_gpu, 1, l.తాత్కాలిక_gpu, 1);			
        mul_gpu(l.outputs*l.batch, l.i_gpu, 1, l.తాత్కాలిక_gpu, 1);				
        gradient_array_gpu(l.g_gpu, l.outputs*l.batch, TANH, l.తాత్కాలిక_gpu);		
        copy_gpu(l.outputs*l.batch, l.తాత్కాలిక_gpu, 1, wg.delta_gpu, 1);
        s.ఆదానం_gpu = l.prev_state_gpu;
        s.delta_gpu = l.dh_gpu;														
        backward_connected_layer_gpu(wg, s);	

        copy_gpu(l.outputs*l.batch, l.తాత్కాలిక_gpu, 1, ug.delta_gpu, 1);
        s.ఆదానం_gpu = state.ఆదానం_gpu;
        s.delta_gpu = state.delta_gpu;
        backward_connected_layer_gpu(ug, s);																

        copy_gpu(l.outputs*l.batch, l.temp2_gpu, 1, l.తాత్కాలిక_gpu, 1);			
        mul_gpu(l.outputs*l.batch, l.g_gpu, 1, l.తాత్కాలిక_gpu, 1);				
        gradient_array_gpu(l.i_gpu, l.outputs*l.batch, LOGISTIC, l.తాత్కాలిక_gpu);	
        copy_gpu(l.outputs*l.batch, l.తాత్కాలిక_gpu, 1, wi.delta_gpu, 1);
        s.ఆదానం_gpu = l.prev_state_gpu;
        s.delta_gpu = l.dh_gpu;
        backward_connected_layer_gpu(wi, s);						

        copy_gpu(l.outputs*l.batch, l.తాత్కాలిక_gpu, 1, ui.delta_gpu, 1);
        s.ఆదానం_gpu = state.ఆదానం_gpu;
        s.delta_gpu = state.delta_gpu;
        backward_connected_layer_gpu(ui, s);									

        copy_gpu(l.outputs*l.batch, l.temp2_gpu, 1, l.తాత్కాలిక_gpu, 1);		
        mul_gpu(l.outputs*l.batch, l.prev_cell_gpu, 1, l.తాత్కాలిక_gpu, 1);
        gradient_array_gpu(l.f_gpu, l.outputs*l.batch, LOGISTIC, l.తాత్కాలిక_gpu);
        copy_gpu(l.outputs*l.batch, l.తాత్కాలిక_gpu, 1, wf.delta_gpu, 1);
        s.ఆదానం_gpu = l.prev_state_gpu;
        s.delta_gpu = l.dh_gpu;
        backward_connected_layer_gpu(wf, s);						

        copy_gpu(l.outputs*l.batch, l.తాత్కాలిక_gpu, 1, uf.delta_gpu, 1);
        s.ఆదానం_gpu = state.ఆదానం_gpu;
        s.delta_gpu = state.delta_gpu;
        backward_connected_layer_gpu(uf, s);									

        copy_gpu(l.outputs*l.batch, l.temp2_gpu, 1, l.తాత్కాలిక_gpu, 1);			
        mul_gpu(l.outputs*l.batch, l.f_gpu, 1, l.తాత్కాలిక_gpu, 1);				
        copy_gpu(l.outputs*l.batch, l.తాత్కాలిక_gpu, 1, l.dc_gpu, 1);				

        state.ఆదానం_gpu -= l.inputs*l.batch;
        ఒకవేళ (state.delta_gpu) state.delta_gpu -= l.inputs*l.batch;
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
#అయితే_ముగింపు

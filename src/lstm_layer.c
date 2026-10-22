#ಅನ್ತರ್ಭಾವ "lstm_layer.h"
#ಅನ್ತರ್ಭಾವ "connected_layer.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "gemm.h"

#ಅನ್ತರ್ಭಾವ <math.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>

ಸ್ಥಿರ ಶೂನ್ಯ increment_layer(layer *l, ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆಗಳು)
{
    ಪೂರ್ಣಾಂಕ ಅಂಕೆ = l->outputs*l->batch*ಹೆಜ್ಜೆಗಳು;
    l->output += ಅಂಕೆ;
    l->delta += ಅಂಕೆ;
    l->x += ಅಂಕೆ;
    l->x_norm += ಅಂಕೆ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l->output_gpu += ಅಂಕೆ;
    l->delta_gpu += ಅಂಕೆ;
    l->x_gpu += ಅಂಕೆ;
    l->x_norm_gpu += ಅಂಕೆ;
#ಆದರೆ_ಅಂತ್ಯ
}

layer make_lstm_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ inputs, ಪೂರ್ಣಾಂಕ outputs, ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆಗಳು, ಪೂರ್ಣಾಂಕ batch_normalize, ಪೂರ್ಣಾಂಕ adam)
{
    fprintf(stderr, "LSTM Layer: %d inputs, %d outputs\n", inputs, outputs);
    batch = batch / ಹೆಜ್ಜೆಗಳು;
    layer l = { 0 };
    l.batch = batch;
    l.type = LSTM;
    l.ಹೆಜ್ಜೆಗಳು = ಹೆಜ್ಜೆಗಳು;
    l.inputs = inputs;

    l.uf = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(layer));
    fprintf(stderr, "\t\t");
    *(l.uf) = make_connected_layer(batch*ಹೆಜ್ಜೆಗಳು, inputs, outputs, ರೇಖೀಯ, batch_normalize, adam);
    l.uf->batch = batch;

    l.ui = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(layer));
    fprintf(stderr, "\t\t");
    *(l.ui) = make_connected_layer(batch*ಹೆಜ್ಜೆಗಳು, inputs, outputs, ರೇಖೀಯ, batch_normalize, adam);
    l.ui->batch = batch;

    l.ug = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(layer));
    fprintf(stderr, "\t\t");
    *(l.ug) = make_connected_layer(batch*ಹೆಜ್ಜೆಗಳು, inputs, outputs, ರೇಖೀಯ, batch_normalize, adam);
    l.ug->batch = batch;

    l.uo = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(layer));
    fprintf(stderr, "\t\t");
    *(l.uo) = make_connected_layer(batch*ಹೆಜ್ಜೆಗಳು, inputs, outputs, ರೇಖೀಯ, batch_normalize, adam);
    l.uo->batch = batch;

    l.wf = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(layer));
    fprintf(stderr, "\t\t");
    *(l.wf) = make_connected_layer(batch*ಹೆಜ್ಜೆಗಳು, outputs, outputs, ರೇಖೀಯ, batch_normalize, adam);
    l.wf->batch = batch;

    l.wi = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(layer));
    fprintf(stderr, "\t\t");
    *(l.wi) = make_connected_layer(batch*ಹೆಜ್ಜೆಗಳು, outputs, outputs, ರೇಖೀಯ, batch_normalize, adam);
    l.wi->batch = batch;

    l.wg = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(layer));
    fprintf(stderr, "\t\t");
    *(l.wg) = make_connected_layer(batch*ಹೆಜ್ಜೆಗಳು, outputs, outputs, ರೇಖೀಯ, batch_normalize, adam);
    l.wg->batch = batch;

    l.wo = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(layer));
    fprintf(stderr, "\t\t");
    *(l.wo) = make_connected_layer(batch*ಹೆಜ್ಜೆಗಳು, outputs, outputs, ರೇಖೀಯ, batch_normalize, adam);
    l.wo->batch = batch;

    l.batch_normalize = batch_normalize;
    l.outputs = outputs;

    l.output = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(outputs*batch*ಹೆಜ್ಜೆಗಳು, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.state = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(outputs*batch, ಗಾತ್ರ(ದಶಮಾಂಶ));

    l.forward = forward_lstm_layer;
    l.update = update_lstm_layer;

    l.prev_state_cpu =  ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.prev_cell_cpu =   ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.cell_cpu =        ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*outputs*ಹೆಜ್ಜೆಗಳು, ಗಾತ್ರ(ದಶಮಾಂಶ));

    l.f_cpu =           ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.i_cpu =           ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.g_cpu =           ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.o_cpu =           ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.c_cpu =           ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.h_cpu =           ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.ತಾತ್ಕಾಲಿಕ_cpu =        ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.temp2_cpu =       ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.temp3_cpu =       ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.dc_cpu =          ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.dh_cpu =          ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(batch*outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l.forward_gpu = forward_lstm_layer_gpu;
    l.backward_gpu = backward_lstm_layer_gpu;
    l.update_gpu = update_lstm_layer_gpu;

    l.output_gpu = cuda_make_array(0, batch*outputs*ಹೆಜ್ಜೆಗಳು);
    l.delta_gpu = cuda_make_array(0, batch*l.outputs*ಹೆಜ್ಜೆಗಳು);

    l.prev_state_gpu = cuda_make_array(0, batch*outputs);
    l.prev_cell_gpu = cuda_make_array(0, batch*outputs);
    l.cell_gpu = cuda_make_array(0, batch*outputs*ಹೆಜ್ಜೆಗಳು);

    l.f_gpu = cuda_make_array(0, batch*outputs);
    l.i_gpu = cuda_make_array(0, batch*outputs);
    l.g_gpu = cuda_make_array(0, batch*outputs);
    l.o_gpu = cuda_make_array(0, batch*outputs);
    l.c_gpu = cuda_make_array(0, batch*outputs);
    l.h_gpu = cuda_make_array(0, batch*outputs);
    l.ತಾತ್ಕಾಲಿಕ_gpu =  cuda_make_array(0, batch*outputs);
    l.temp2_gpu = cuda_make_array(0, batch*outputs);
    l.temp3_gpu = cuda_make_array(0, batch*outputs);
    l.dc_gpu = cuda_make_array(0, batch*outputs);
    l.dh_gpu = cuda_make_array(0, batch*outputs);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CUDNN
        cudnn_ಗಣ_Tensor4d_Descriptor(l.wf->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, batch, l.wf->ಹೊರಹೋಗು_c, l.wf->ಹೊರಹೋಗು_h, l.wf->ಹೊರಹೋಗು_w); 
        cudnn_ಗಣ_Tensor4d_Descriptor(l.wi->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, batch, l.wi->ಹೊರಹೋಗು_c, l.wi->ಹೊರಹೋಗು_h, l.wi->ಹೊರಹೋಗು_w); 
        cudnn_ಗಣ_Tensor4d_Descriptor(l.wg->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, batch, l.wg->ಹೊರಹೋಗು_c, l.wg->ಹೊರಹೋಗು_h, l.wg->ಹೊರಹೋಗು_w); 
        cudnn_ಗಣ_Tensor4d_Descriptor(l.wo->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, batch, l.wo->ಹೊರಹೋಗು_c, l.wo->ಹೊರಹೋಗು_h, l.wo->ಹೊರಹೋಗು_w); 

        cudnn_ಗಣ_Tensor4d_Descriptor(l.uf->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, batch, l.uf->ಹೊರಹೋಗು_c, l.uf->ಹೊರಹೋಗು_h, l.uf->ಹೊರಹೋಗು_w); 
        cudnn_ಗಣ_Tensor4d_Descriptor(l.ui->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, batch, l.ui->ಹೊರಹೋಗು_c, l.ui->ಹೊರಹೋಗು_h, l.ui->ಹೊರಹೋಗು_w); 
        cudnn_ಗಣ_Tensor4d_Descriptor(l.ug->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, batch, l.ug->ಹೊರಹೋಗು_c, l.ug->ಹೊರಹೋಗು_h, l.ug->ಹೊರಹೋಗು_w); 
        cudnn_ಗಣ_Tensor4d_Descriptor(l.uo->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, batch, l.uo->ಹೊರಹೋಗು_c, l.uo->ಹೊರಹೋಗು_h, l.uo->ಹೊರಹೋಗು_w); 
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ l;
}

ಶೂನ್ಯ update_lstm_layer(layer l, update_ವಾದಗಳು a)
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

ಶೂನ್ಯ forward_lstm_layer(layer l, ಜಾಲ state)
{
    ಜಾಲ s = { 0 };
    s.train = state.train;
    ಪೂರ್ಣಾಂಕ i;
    layer wf = *(l.wf);
    layer wi = *(l.wi);
    layer wg = *(l.wg);
    layer wo = *(l.wo);

    layer uf = *(l.uf);
    layer ui = *(l.ui);
    layer ug = *(l.ug);
    layer uo = *(l.uo);

    fill_cpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, wf.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, wi.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, wg.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, wo.delta, 1);

    fill_cpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, uf.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, ui.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, ug.delta, 1);
    fill_cpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, uo.delta, 1);
    ಆದರೆ (state.train) {
        fill_cpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, l.delta, 1);
    }

    ಗಾಗಿ (i = 0; i < l.ಹೆಜ್ಜೆಗಳು; ++i) {
        s.ಒಳಹರಿವು = l.h_cpu;
        forward_connected_layer(wf, s);							
        forward_connected_layer(wi, s);							
        forward_connected_layer(wg, s);							
        forward_connected_layer(wo, s);							

        s.ಒಳಹರಿವು = state.ಒಳಹರಿವು;
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

        copy_cpu(l.outputs*l.batch, l.i_cpu, 1, l.ತಾತ್ಕಾಲಿಕ_cpu, 1);		
        mul_cpu(l.outputs*l.batch, l.g_cpu, 1, l.ತಾತ್ಕಾಲಿಕ_cpu, 1);		
        mul_cpu(l.outputs*l.batch, l.f_cpu, 1, l.c_cpu, 1);			
        axpy_cpu(l.outputs*l.batch, 1, l.ತಾತ್ಕಾಲಿಕ_cpu, 1, l.c_cpu, 1);	

        copy_cpu(l.outputs*l.batch, l.c_cpu, 1, l.h_cpu, 1);			
        activate_array(l.h_cpu, l.outputs*l.batch, TANH);		
        mul_cpu(l.outputs*l.batch, l.o_cpu, 1, l.h_cpu, 1);	

        copy_cpu(l.outputs*l.batch, l.c_cpu, 1, l.cell_cpu, 1);		
        copy_cpu(l.outputs*l.batch, l.h_cpu, 1, l.output, 1);

        state.ಒಳಹರಿವು += l.inputs*l.batch;
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

ಶೂನ್ಯ backward_lstm_layer(layer l, ಜಾಲ state)
{
    ಜಾಲ s = { 0 };
    s.train = state.train;
    ಪೂರ್ಣಾಂಕ i;
    layer wf = *(l.wf);
    layer wi = *(l.wi);
    layer wg = *(l.wg);
    layer wo = *(l.wo);

    layer uf = *(l.uf);
    layer ui = *(l.ui);
    layer ug = *(l.ug);
    layer uo = *(l.uo);

    increment_layer(&wf, l.ಹೆಜ್ಜೆಗಳು - 1);
    increment_layer(&wi, l.ಹೆಜ್ಜೆಗಳು - 1);
    increment_layer(&wg, l.ಹೆಜ್ಜೆಗಳು - 1);
    increment_layer(&wo, l.ಹೆಜ್ಜೆಗಳು - 1);

    increment_layer(&uf, l.ಹೆಜ್ಜೆಗಳು - 1);
    increment_layer(&ui, l.ಹೆಜ್ಜೆಗಳು - 1);
    increment_layer(&ug, l.ಹೆಜ್ಜೆಗಳು - 1);
    increment_layer(&uo, l.ಹೆಜ್ಜೆಗಳು - 1);

    state.ಒಳಹರಿವು += l.inputs*l.batch*(l.ಹೆಜ್ಜೆಗಳು - 1);
    ಆದರೆ (state.delta) state.delta += l.inputs*l.batch*(l.ಹೆಜ್ಜೆಗಳು - 1);

    l.output += l.outputs*l.batch*(l.ಹೆಜ್ಜೆಗಳು - 1);
    l.cell_cpu += l.outputs*l.batch*(l.ಹೆಜ್ಜೆಗಳು - 1);
    l.delta += l.outputs*l.batch*(l.ಹೆಜ್ಜೆಗಳು - 1);

    ಗಾಗಿ (i = l.ಹೆಜ್ಜೆಗಳು - 1; i >= 0; --i) {
        ಆದರೆ (i != 0) copy_cpu(l.outputs*l.batch, l.cell_cpu - l.outputs*l.batch, 1, l.prev_cell_cpu, 1);
        copy_cpu(l.outputs*l.batch, l.cell_cpu, 1, l.c_cpu, 1);
        ಆದರೆ (i != 0) copy_cpu(l.outputs*l.batch, l.output - l.outputs*l.batch, 1, l.prev_state_cpu, 1);
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

        copy_cpu(l.outputs*l.batch, l.c_cpu, 1, l.ತಾತ್ಕಾಲಿಕ_cpu, 1);			
        activate_array(l.ತಾತ್ಕಾಲಿಕ_cpu, l.outputs*l.batch, TANH);			

        copy_cpu(l.outputs*l.batch, l.temp3_cpu, 1, l.temp2_cpu, 1);		
        mul_cpu(l.outputs*l.batch, l.o_cpu, 1, l.temp2_cpu, 1);			

        gradient_array(l.ತಾತ್ಕಾಲಿಕ_cpu, l.outputs*l.batch, TANH, l.temp2_cpu);
        axpy_cpu(l.outputs*l.batch, 1, l.dc_cpu, 1, l.temp2_cpu, 1);		

        copy_cpu(l.outputs*l.batch, l.c_cpu, 1, l.ತಾತ್ಕಾಲಿಕ_cpu, 1);			
        activate_array(l.ತಾತ್ಕಾಲಿಕ_cpu, l.outputs*l.batch, TANH);			
        mul_cpu(l.outputs*l.batch, l.temp3_cpu, 1, l.ತಾತ್ಕಾಲಿಕ_cpu, 1);		
        gradient_array(l.o_cpu, l.outputs*l.batch, LOGISTIC, l.ತಾತ್ಕಾಲಿಕ_cpu);
        copy_cpu(l.outputs*l.batch, l.ತಾತ್ಕಾಲಿಕ_cpu, 1, wo.delta, 1);
        s.ಒಳಹರಿವು = l.prev_state_cpu;
        s.delta = l.dh_cpu;															
        backward_connected_layer(wo, s);	

        copy_cpu(l.outputs*l.batch, l.ತಾತ್ಕಾಲಿಕ_cpu, 1, uo.delta, 1);
        s.ಒಳಹರಿವು = state.ಒಳಹರಿವು;
        s.delta = state.delta;
        backward_connected_layer(uo, s);									

        copy_cpu(l.outputs*l.batch, l.temp2_cpu, 1, l.ತಾತ್ಕಾಲಿಕ_cpu, 1);			
        mul_cpu(l.outputs*l.batch, l.i_cpu, 1, l.ತಾತ್ಕಾಲಿಕ_cpu, 1);				
        gradient_array(l.g_cpu, l.outputs*l.batch, TANH, l.ತಾತ್ಕಾಲಿಕ_cpu);		
        copy_cpu(l.outputs*l.batch, l.ತಾತ್ಕಾಲಿಕ_cpu, 1, wg.delta, 1);
        s.ಒಳಹರಿವು = l.prev_state_cpu;
        s.delta = l.dh_cpu;														
        backward_connected_layer(wg, s);	

        copy_cpu(l.outputs*l.batch, l.ತಾತ್ಕಾಲಿಕ_cpu, 1, ug.delta, 1);
        s.ಒಳಹರಿವು = state.ಒಳಹರಿವು;
        s.delta = state.delta;
        backward_connected_layer(ug, s);																

        copy_cpu(l.outputs*l.batch, l.temp2_cpu, 1, l.ತಾತ್ಕಾಲಿಕ_cpu, 1);			
        mul_cpu(l.outputs*l.batch, l.g_cpu, 1, l.ತಾತ್ಕಾಲಿಕ_cpu, 1);				
        gradient_array(l.i_cpu, l.outputs*l.batch, LOGISTIC, l.ತಾತ್ಕಾಲಿಕ_cpu);	
        copy_cpu(l.outputs*l.batch, l.ತಾತ್ಕಾಲಿಕ_cpu, 1, wi.delta, 1);
        s.ಒಳಹರಿವು = l.prev_state_cpu;
        s.delta = l.dh_cpu;
        backward_connected_layer(wi, s);						

        copy_cpu(l.outputs*l.batch, l.ತಾತ್ಕಾಲಿಕ_cpu, 1, ui.delta, 1);
        s.ಒಳಹರಿವು = state.ಒಳಹರಿವು;
        s.delta = state.delta;
        backward_connected_layer(ui, s);									

        copy_cpu(l.outputs*l.batch, l.temp2_cpu, 1, l.ತಾತ್ಕಾಲಿಕ_cpu, 1);		
        mul_cpu(l.outputs*l.batch, l.prev_cell_cpu, 1, l.ತಾತ್ಕಾಲಿಕ_cpu, 1);
        gradient_array(l.f_cpu, l.outputs*l.batch, LOGISTIC, l.ತಾತ್ಕಾಲಿಕ_cpu);
        copy_cpu(l.outputs*l.batch, l.ತಾತ್ಕಾಲಿಕ_cpu, 1, wf.delta, 1);
        s.ಒಳಹರಿವು = l.prev_state_cpu;
        s.delta = l.dh_cpu;
        backward_connected_layer(wf, s);						

        copy_cpu(l.outputs*l.batch, l.ತಾತ್ಕಾಲಿಕ_cpu, 1, uf.delta, 1);
        s.ಒಳಹರಿವು = state.ಒಳಹರಿವು;
        s.delta = state.delta;
        backward_connected_layer(uf, s);									

        copy_cpu(l.outputs*l.batch, l.temp2_cpu, 1, l.ತಾತ್ಕಾಲಿಕ_cpu, 1);			
        mul_cpu(l.outputs*l.batch, l.f_cpu, 1, l.ತಾತ್ಕಾಲಿಕ_cpu, 1);				
        copy_cpu(l.outputs*l.batch, l.ತಾತ್ಕಾಲಿಕ_cpu, 1, l.dc_cpu, 1);				

        state.ಒಳಹರಿವು -= l.inputs*l.batch;
        ಆದರೆ (state.delta) state.delta -= l.inputs*l.batch;
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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ update_lstm_layer_gpu(layer l, update_ವಾದಗಳು a)
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

ಶೂನ್ಯ forward_lstm_layer_gpu(layer l, ಜಾಲ state)
{
    ಜಾಲ s = { 0 };
    s.train = state.train;
    ಪೂರ್ಣಾಂಕ i;
    layer wf = *(l.wf);
    layer wi = *(l.wi);
    layer wg = *(l.wg);
    layer wo = *(l.wo);

    layer uf = *(l.uf);
    layer ui = *(l.ui);
    layer ug = *(l.ug);
    layer uo = *(l.uo);

    fill_gpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, wf.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, wi.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, wg.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, wo.delta_gpu, 1);

    fill_gpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, uf.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, ui.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, ug.delta_gpu, 1);
    fill_gpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, uo.delta_gpu, 1);
    ಆದರೆ (state.train) {
        fill_gpu(l.outputs * l.batch * l.ಹೆಜ್ಜೆಗಳು, 0, l.delta_gpu, 1);
    }

    ಗಾಗಿ (i = 0; i < l.ಹೆಜ್ಜೆಗಳು; ++i) {
        s.ಒಳಹರಿವು_gpu = l.h_gpu;
        forward_connected_layer_gpu(wf, s);							
        forward_connected_layer_gpu(wi, s);							
        forward_connected_layer_gpu(wg, s);							
        forward_connected_layer_gpu(wo, s);							

        s.ಒಳಹರಿವು_gpu = state.ಒಳಹರಿವು_gpu;
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

        copy_gpu(l.outputs*l.batch, l.i_gpu, 1, l.ತಾತ್ಕಾಲಿಕ_gpu, 1);		
        mul_gpu(l.outputs*l.batch, l.g_gpu, 1, l.ತಾತ್ಕಾಲಿಕ_gpu, 1);		
        mul_gpu(l.outputs*l.batch, l.f_gpu, 1, l.c_gpu, 1);			
        axpy_gpu(l.outputs*l.batch, 1, l.ತಾತ್ಕಾಲಿಕ_gpu, 1, l.c_gpu, 1);	

        copy_gpu(l.outputs*l.batch, l.c_gpu, 1, l.h_gpu, 1);			
        activate_array_gpu(l.h_gpu, l.outputs*l.batch, TANH);		
        mul_gpu(l.outputs*l.batch, l.o_gpu, 1, l.h_gpu, 1);	

        copy_gpu(l.outputs*l.batch, l.c_gpu, 1, l.cell_gpu, 1);		
        copy_gpu(l.outputs*l.batch, l.h_gpu, 1, l.output_gpu, 1);

        state.ಒಳಹರಿವು_gpu += l.inputs*l.batch;
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

ಶೂನ್ಯ backward_lstm_layer_gpu(layer l, ಜಾಲ state)
{
    ಜಾಲ s = { 0 };
    s.train = state.train;
    ಪೂರ್ಣಾಂಕ i;
    layer wf = *(l.wf);
    layer wi = *(l.wi);
    layer wg = *(l.wg);
    layer wo = *(l.wo);

    layer uf = *(l.uf);
    layer ui = *(l.ui);
    layer ug = *(l.ug);
    layer uo = *(l.uo);

    increment_layer(&wf, l.ಹೆಜ್ಜೆಗಳು - 1);
    increment_layer(&wi, l.ಹೆಜ್ಜೆಗಳು - 1);
    increment_layer(&wg, l.ಹೆಜ್ಜೆಗಳು - 1);
    increment_layer(&wo, l.ಹೆಜ್ಜೆಗಳು - 1);

    increment_layer(&uf, l.ಹೆಜ್ಜೆಗಳು - 1);
    increment_layer(&ui, l.ಹೆಜ್ಜೆಗಳು - 1);
    increment_layer(&ug, l.ಹೆಜ್ಜೆಗಳು - 1);
    increment_layer(&uo, l.ಹೆಜ್ಜೆಗಳು - 1);

    state.ಒಳಹರಿವು_gpu += l.inputs*l.batch*(l.ಹೆಜ್ಜೆಗಳು - 1);
    ಆದರೆ (state.delta_gpu) state.delta_gpu += l.inputs*l.batch*(l.ಹೆಜ್ಜೆಗಳು - 1);

    l.output_gpu += l.outputs*l.batch*(l.ಹೆಜ್ಜೆಗಳು - 1);
    l.cell_gpu += l.outputs*l.batch*(l.ಹೆಜ್ಜೆಗಳು - 1);
    l.delta_gpu += l.outputs*l.batch*(l.ಹೆಜ್ಜೆಗಳು - 1);

    ಗಾಗಿ (i = l.ಹೆಜ್ಜೆಗಳು - 1; i >= 0; --i) {
        ಆದರೆ (i != 0) copy_gpu(l.outputs*l.batch, l.cell_gpu - l.outputs*l.batch, 1, l.prev_cell_gpu, 1);
        copy_gpu(l.outputs*l.batch, l.cell_gpu, 1, l.c_gpu, 1);
        ಆದರೆ (i != 0) copy_gpu(l.outputs*l.batch, l.output_gpu - l.outputs*l.batch, 1, l.prev_state_gpu, 1);
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

        copy_gpu(l.outputs*l.batch, l.c_gpu, 1, l.ತಾತ್ಕಾಲಿಕ_gpu, 1);			
        activate_array_gpu(l.ತಾತ್ಕಾಲಿಕ_gpu, l.outputs*l.batch, TANH);			

        copy_gpu(l.outputs*l.batch, l.temp3_gpu, 1, l.temp2_gpu, 1);		
        mul_gpu(l.outputs*l.batch, l.o_gpu, 1, l.temp2_gpu, 1);			

        gradient_array_gpu(l.ತಾತ್ಕಾಲಿಕ_gpu, l.outputs*l.batch, TANH, l.temp2_gpu);
        axpy_gpu(l.outputs*l.batch, 1, l.dc_gpu, 1, l.temp2_gpu, 1);		

        copy_gpu(l.outputs*l.batch, l.c_gpu, 1, l.ತಾತ್ಕಾಲಿಕ_gpu, 1);			
        activate_array_gpu(l.ತಾತ್ಕಾಲಿಕ_gpu, l.outputs*l.batch, TANH);			
        mul_gpu(l.outputs*l.batch, l.temp3_gpu, 1, l.ತಾತ್ಕಾಲಿಕ_gpu, 1);		
        gradient_array_gpu(l.o_gpu, l.outputs*l.batch, LOGISTIC, l.ತಾತ್ಕಾಲಿಕ_gpu);
        copy_gpu(l.outputs*l.batch, l.ತಾತ್ಕಾಲಿಕ_gpu, 1, wo.delta_gpu, 1);
        s.ಒಳಹರಿವು_gpu = l.prev_state_gpu;
        s.delta_gpu = l.dh_gpu;															
        backward_connected_layer_gpu(wo, s);	

        copy_gpu(l.outputs*l.batch, l.ತಾತ್ಕಾಲಿಕ_gpu, 1, uo.delta_gpu, 1);
        s.ಒಳಹರಿವು_gpu = state.ಒಳಹರಿವು_gpu;
        s.delta_gpu = state.delta_gpu;
        backward_connected_layer_gpu(uo, s);									

        copy_gpu(l.outputs*l.batch, l.temp2_gpu, 1, l.ತಾತ್ಕಾಲಿಕ_gpu, 1);			
        mul_gpu(l.outputs*l.batch, l.i_gpu, 1, l.ತಾತ್ಕಾಲಿಕ_gpu, 1);				
        gradient_array_gpu(l.g_gpu, l.outputs*l.batch, TANH, l.ತಾತ್ಕಾಲಿಕ_gpu);		
        copy_gpu(l.outputs*l.batch, l.ತಾತ್ಕಾಲಿಕ_gpu, 1, wg.delta_gpu, 1);
        s.ಒಳಹರಿವು_gpu = l.prev_state_gpu;
        s.delta_gpu = l.dh_gpu;														
        backward_connected_layer_gpu(wg, s);	

        copy_gpu(l.outputs*l.batch, l.ತಾತ್ಕಾಲಿಕ_gpu, 1, ug.delta_gpu, 1);
        s.ಒಳಹರಿವು_gpu = state.ಒಳಹರಿವು_gpu;
        s.delta_gpu = state.delta_gpu;
        backward_connected_layer_gpu(ug, s);																

        copy_gpu(l.outputs*l.batch, l.temp2_gpu, 1, l.ತಾತ್ಕಾಲಿಕ_gpu, 1);			
        mul_gpu(l.outputs*l.batch, l.g_gpu, 1, l.ತಾತ್ಕಾಲಿಕ_gpu, 1);				
        gradient_array_gpu(l.i_gpu, l.outputs*l.batch, LOGISTIC, l.ತಾತ್ಕಾಲಿಕ_gpu);	
        copy_gpu(l.outputs*l.batch, l.ತಾತ್ಕಾಲಿಕ_gpu, 1, wi.delta_gpu, 1);
        s.ಒಳಹರಿವು_gpu = l.prev_state_gpu;
        s.delta_gpu = l.dh_gpu;
        backward_connected_layer_gpu(wi, s);						

        copy_gpu(l.outputs*l.batch, l.ತಾತ್ಕಾಲಿಕ_gpu, 1, ui.delta_gpu, 1);
        s.ಒಳಹರಿವು_gpu = state.ಒಳಹರಿವು_gpu;
        s.delta_gpu = state.delta_gpu;
        backward_connected_layer_gpu(ui, s);									

        copy_gpu(l.outputs*l.batch, l.temp2_gpu, 1, l.ತಾತ್ಕಾಲಿಕ_gpu, 1);		
        mul_gpu(l.outputs*l.batch, l.prev_cell_gpu, 1, l.ತಾತ್ಕಾಲಿಕ_gpu, 1);
        gradient_array_gpu(l.f_gpu, l.outputs*l.batch, LOGISTIC, l.ತಾತ್ಕಾಲಿಕ_gpu);
        copy_gpu(l.outputs*l.batch, l.ತಾತ್ಕಾಲಿಕ_gpu, 1, wf.delta_gpu, 1);
        s.ಒಳಹರಿವು_gpu = l.prev_state_gpu;
        s.delta_gpu = l.dh_gpu;
        backward_connected_layer_gpu(wf, s);						

        copy_gpu(l.outputs*l.batch, l.ತಾತ್ಕಾಲಿಕ_gpu, 1, uf.delta_gpu, 1);
        s.ಒಳಹರಿವು_gpu = state.ಒಳಹರಿವು_gpu;
        s.delta_gpu = state.delta_gpu;
        backward_connected_layer_gpu(uf, s);									

        copy_gpu(l.outputs*l.batch, l.temp2_gpu, 1, l.ತಾತ್ಕಾಲಿಕ_gpu, 1);			
        mul_gpu(l.outputs*l.batch, l.f_gpu, 1, l.ತಾತ್ಕಾಲಿಕ_gpu, 1);				
        copy_gpu(l.outputs*l.batch, l.ತಾತ್ಕಾಲಿಕ_gpu, 1, l.dc_gpu, 1);				

        state.ಒಳಹರಿವು_gpu -= l.inputs*l.batch;
        ಆದರೆ (state.delta_gpu) state.delta_gpu -= l.inputs*l.batch;
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
#ಆದರೆ_ಅಂತ್ಯ

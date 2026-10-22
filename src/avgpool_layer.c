#समावेश "avgpool_layer.h"
#समावेश "cuda.h"
#समावेश <stdio.h>

avgpool_layer make_avgpool_layer(पूर्णांक batch, पूर्णांक w, पूर्णांक h, पूर्णांक c)
{
    ख_लिखो(मानक_त्रुटि, "avg                     %4d x%4d x%4d   ->  %4d\n",  w, h, c, c);
    avgpool_layer l = {0};
    l.type = AVGPOOL;
    l.batch = batch;
    l.h = h;
    l.w = w;
    l.c = c;
    l.out_w = 1;
    l.out_h = 1;
    l.out_c = c;
    l.outputs = l.out_c;
    l.inputs = h*w*c;
    पूर्णांक output_आकार = l.outputs * batch;
    l.output =  सुस्मृति(output_आकार, माप(भग्न));
    l.delta =   सुस्मृति(output_आकार, माप(भग्न));
    l.forward = forward_avgpool_layer;
    l.backward = backward_avgpool_layer;
    #अगर_घोषित GPU
    l.forward_gpu = forward_avgpool_layer_gpu;
    l.backward_gpu = backward_avgpool_layer_gpu;
    l.output_gpu  = cuda_make_array(l.output, output_आकार);
    l.delta_gpu   = cuda_make_array(l.delta, output_आकार);
    #पूर्ण_अगर
    वापस l;
}

व्योम resize_avgpool_layer(avgpool_layer *l, पूर्णांक w, पूर्णांक h)
{
    l->w = w;
    l->h = h;
    l->inputs = h*w*l->c;
}

व्योम forward_avgpool_layer(स्थिर avgpool_layer l, network net)
{
    पूर्णांक b,i,k;

    क्रम(b = 0; b < l.batch; ++b){
        क्रम(k = 0; k < l.c; ++k){
            पूर्णांक out_सूचकांक = k + b*l.c;
            l.output[out_सूचकांक] = 0;
            क्रम(i = 0; i < l.h*l.w; ++i){
                पूर्णांक in_सूचकांक = i + l.h*l.w*(k + b*l.c);
                l.output[out_सूचकांक] += net.निवेश[in_सूचकांक];
            }
            l.output[out_सूचकांक] /= l.h*l.w;
        }
    }
}

व्योम backward_avgpool_layer(स्थिर avgpool_layer l, network net)
{
    पूर्णांक b,i,k;

    क्रम(b = 0; b < l.batch; ++b){
        क्रम(k = 0; k < l.c; ++k){
            पूर्णांक out_सूचकांक = k + b*l.c;
            क्रम(i = 0; i < l.h*l.w; ++i){
                पूर्णांक in_सूचकांक = i + l.h*l.w*(k + b*l.c);
                net.delta[in_सूचकांक] += l.delta[out_सूचकांक] / (l.h*l.w);
            }
        }
    }
}


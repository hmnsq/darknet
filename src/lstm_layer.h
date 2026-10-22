#ifndef LSTM_LAYER_H
#define LSTM_LAYER_H

#অন্তর্ভাব "activations.h"
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"
#define USET

layer make_lstm_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা inputs, পূর্ণসংখ্যা outputs, পূর্ণসংখ্যা steps, পূর্ণসংখ্যা batch_normalize, পূর্ণসংখ্যা adam);

শূন্য forward_lstm_layer(layer l, network net); 
শূন্য update_lstm_layer(layer l, update_args a);

#ifdef GPU
শূন্য forward_lstm_layer_gpu(layer l, network net);
শূন্য backward_lstm_layer_gpu(layer l, network net);
শূন্য update_lstm_layer_gpu(layer l, update_args a); 

#endif
#endif

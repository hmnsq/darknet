#నిర్వచితం_కాకపోతే LSTM_LAYER_H
#నిర్వచించు LSTM_LAYER_H

#అన్తర్భావ "activations.h"
#అన్తర్భావ "layer.h"
#అన్తర్భావ "network.h"
#నిర్వచించు USET

layer make_lstm_layer(పూర్ణాంకం batch, పూర్ణాంకం inputs, పూర్ణాంకం outputs, పూర్ణాంకం అడుగులు, పూర్ణాంకం batch_normalize, పూర్ణాంకం adam);

శూన్య forward_lstm_layer(layer l, జాలం net); 
శూన్య update_lstm_layer(layer l, update_వాదనలు a);

#నిర్వచితమైతే GPU
శూన్య forward_lstm_layer_gpu(layer l, జాలం net);
శూన్య backward_lstm_layer_gpu(layer l, జాలం net);
శూన్య update_lstm_layer_gpu(layer l, update_వాదనలు a); 

#అయితే_ముగింపు
#అయితే_ముగింపు

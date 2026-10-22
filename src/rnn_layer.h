
#నిర్వచితం_కాకపోతే RNN_LAYER_H
#నిర్వచించు RNN_LAYER_H

#అన్తర్భావ "activations.h"
#అన్తర్భావ "layer.h"
#అన్తర్భావ "network.h"
#నిర్వచించు USET

layer make_rnn_layer(పూర్ణాంకం batch, పూర్ణాంకం inputs, పూర్ణాంకం outputs, పూర్ణాంకం అడుగులు, ACTIVATION activation, పూర్ణాంకం batch_normalize, పూర్ణాంకం adam);

శూన్య forward_rnn_layer(layer l, జాలం net);
శూన్య backward_rnn_layer(layer l, జాలం net);
శూన్య update_rnn_layer(layer l, update_వాదనలు a);

#నిర్వచితమైతే GPU
శూన్య forward_rnn_layer_gpu(layer l, జాలం net);
శూన్య backward_rnn_layer_gpu(layer l, జాలం net);
శూన్య update_rnn_layer_gpu(layer l, update_వాదనలు a);
శూన్య నెట్టు_rnn_layer(layer l);
శూన్య pull_rnn_layer(layer l);
#అయితే_ముగింపు

#అయితే_ముగింపు


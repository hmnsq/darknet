
#నిర్వచితం_కాకపోతే GRU_LAYER_H
#నిర్వచించు GRU_LAYER_H

#అన్తర్భావ "activations.h"
#అన్తర్భావ "layer.h"
#అన్తర్భావ "network.h"

layer make_gru_layer(పూర్ణాంకం batch, పూర్ణాంకం inputs, పూర్ణాంకం outputs, పూర్ణాంకం అడుగులు, పూర్ణాంకం batch_normalize, పూర్ణాంకం adam);

శూన్య forward_gru_layer(layer l, జాలం state);
శూన్య backward_gru_layer(layer l, జాలం state);
శూన్య update_gru_layer(layer l, update_వాదనలు a);

#నిర్వచితమైతే GPU
శూన్య forward_gru_layer_gpu(layer l, జాలం state);
శూన్య backward_gru_layer_gpu(layer l, జాలం state);
శూన్య update_gru_layer_gpu(layer l, update_వాదనలు a);
శూన్య నెట్టు_gru_layer(layer l);
శూన్య pull_gru_layer(layer l);
#అయితే_ముగింపు

#అయితే_ముగింపు



#నిర్వచితం_కాకపోతే CRNN_LAYER_H
#నిర్వచించు CRNN_LAYER_H

#అన్తర్భావ "activations.h"
#అన్తర్భావ "layer.h"
#అన్తర్భావ "network.h"

layer make_crnn_layer(పూర్ణాంకం batch, పూర్ణాంకం h, పూర్ణాంకం w, పూర్ణాంకం c, పూర్ణాంకం hidden_filters, పూర్ణాంకం output_filters, పూర్ణాంకం అడుగులు, ACTIVATION activation, పూర్ణాంకం batch_normalize);

శూన్య forward_crnn_layer(layer l, జాలం net);
శూన్య backward_crnn_layer(layer l, జాలం net);
శూన్య update_crnn_layer(layer l, update_వాదనలు a);

#నిర్వచితమైతే GPU
శూన్య forward_crnn_layer_gpu(layer l, జాలం net);
శూన్య backward_crnn_layer_gpu(layer l, జాలం net);
శూన్య update_crnn_layer_gpu(layer l, update_వాదనలు a);
శూన్య నెట్టు_crnn_layer(layer l);
శూన్య pull_crnn_layer(layer l);
#అయితే_ముగింపు

#అయితే_ముగింపు


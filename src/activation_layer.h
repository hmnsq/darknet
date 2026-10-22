#నిర్వచితం_కాకపోతే ACTIVATION_LAYER_H
#నిర్వచించు ACTIVATION_LAYER_H

#అన్తర్భావ "activations.h"
#అన్తర్భావ "layer.h"
#అన్తర్భావ "network.h"

layer make_activation_layer(పూర్ణాంకం batch, పూర్ణాంకం inputs, ACTIVATION activation);

శూన్య forward_activation_layer(layer l, జాలం net);
శూన్య backward_activation_layer(layer l, జాలం net);

#నిర్వచితమైతే GPU
శూన్య forward_activation_layer_gpu(layer l, జాలం net);
శూన్య backward_activation_layer_gpu(layer l, జాలం net);
#అయితే_ముగింపు

#అయితే_ముగింపు


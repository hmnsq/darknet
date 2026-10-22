#నిర్వచితం_కాకపోతే DROPOUT_LAYER_H
#నిర్వచించు DROPOUT_LAYER_H

#అన్తర్భావ "layer.h"
#అన్తర్భావ "network.h"

రకనిర్వచనం layer dropout_layer;

dropout_layer make_dropout_layer(పూర్ణాంకం batch, పూర్ణాంకం inputs, దశాంశం probability);

శూన్య forward_dropout_layer(dropout_layer l, జాలం net);
శూన్య backward_dropout_layer(dropout_layer l, జాలం net);
శూన్య resize_dropout_layer(dropout_layer *l, పూర్ణాంకం inputs);

#నిర్వచితమైతే GPU
శూన్య forward_dropout_layer_gpu(dropout_layer l, జాలం net);
శూన్య backward_dropout_layer_gpu(dropout_layer l, జాలం net);

#అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే BATCHNORM_LAYER_H
#నిర్వచించు BATCHNORM_LAYER_H

#అన్తర్భావ "image.h"
#అన్తర్భావ "layer.h"
#అన్తర్భావ "network.h"

layer make_batchnorm_layer(పూర్ణాంకం batch, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం c);
శూన్య forward_batchnorm_layer(layer l, జాలం net);
శూన్య backward_batchnorm_layer(layer l, జాలం net);

#నిర్వచితమైతే GPU
శూన్య forward_batchnorm_layer_gpu(layer l, జాలం net);
శూన్య backward_batchnorm_layer_gpu(layer l, జాలం net);
శూన్య pull_batchnorm_layer(layer l);
శూన్య నెట్టు_batchnorm_layer(layer l);
#అయితే_ముగింపు

#అయితే_ముగింపు

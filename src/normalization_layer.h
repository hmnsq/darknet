#నిర్వచితం_కాకపోతే NORMALIZATION_LAYER_H
#నిర్వచించు NORMALIZATION_LAYER_H

#అన్తర్భావ "image.h"
#అన్తర్భావ "layer.h"
#అన్తర్భావ "network.h"

layer make_normalization_layer(పూర్ణాంకం batch, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం c, పూర్ణాంకం కొలత, దశాంశం alpha, దశాంశం beta, దశాంశం kappa);
శూన్య resize_normalization_layer(layer *layer, పూర్ణాంకం h, పూర్ణాంకం w);
శూన్య forward_normalization_layer(స్థిరరాశి layer layer, జాలం net);
శూన్య backward_normalization_layer(స్థిరరాశి layer layer, జాలం net);
శూన్య visualize_normalization_layer(layer layer, అక్షరం *కిటికీ);

#నిర్వచితమైతే GPU
శూన్య forward_normalization_layer_gpu(స్థిరరాశి layer layer, జాలం net);
శూన్య backward_normalization_layer_gpu(స్థిరరాశి layer layer, జాలం net);
#అయితే_ముగింపు

#అయితే_ముగింపు

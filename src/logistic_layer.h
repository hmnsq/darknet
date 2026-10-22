#నిర్వచితం_కాకపోతే LOGISTIC_LAYER_H
#నిర్వచించు LOGISTIC_LAYER_H
#అన్తర్భావ "layer.h"
#అన్తర్భావ "network.h"

layer make_logistic_layer(పూర్ణాంకం batch, పూర్ణాంకం inputs);
శూన్య forward_logistic_layer(స్థిరరాశి layer l, జాలం net);
శూన్య backward_logistic_layer(స్థిరరాశి layer l, జాలం net);

#నిర్వచితమైతే GPU
శూన్య forward_logistic_layer_gpu(స్థిరరాశి layer l, జాలం net);
శూన్య backward_logistic_layer_gpu(స్థిరరాశి layer l, జాలం net);
#అయితే_ముగింపు

#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే REGION_LAYER_H
#నిర్వచించు REGION_LAYER_H

#అన్తర్భావ "darknet.h"
#అన్తర్భావ "layer.h"
#అన్తర్భావ "network.h"

layer make_region_layer(పూర్ణాంకం batch, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం n, పూర్ణాంకం classes, పూర్ణాంకం coords);
శూన్య forward_region_layer(స్థిరరాశి layer l, జాలం net);
శూన్య backward_region_layer(స్థిరరాశి layer l, జాలం net);
శూన్య resize_region_layer(layer *l, పూర్ణాంకం w, పూర్ణాంకం h);

#నిర్వచితమైతే GPU
శూన్య forward_region_layer_gpu(స్థిరరాశి layer l, జాలం net);
శూన్య backward_region_layer_gpu(layer l, జాలం net);
#అయితే_ముగింపు

#అయితే_ముగింపు

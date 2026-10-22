#నిర్వచితం_కాకపోతే REORG_LAYER_H
#నిర్వచించు REORG_LAYER_H

#అన్తర్భావ "image.h"
#అన్తర్భావ "cuda.h"
#అన్తర్భావ "layer.h"
#అన్తర్భావ "network.h"

layer make_reorg_layer(పూర్ణాంకం batch, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం c, పూర్ణాంకం stride, పూర్ణాంకం తిరగేయి, పూర్ణాంకం flatten, పూర్ణాంకం extra);
శూన్య resize_reorg_layer(layer *l, పూర్ణాంకం w, పూర్ణాంకం h);
శూన్య forward_reorg_layer(స్థిరరాశి layer l, జాలం net);
శూన్య backward_reorg_layer(స్థిరరాశి layer l, జాలం net);

#నిర్వచితమైతే GPU
శూన్య forward_reorg_layer_gpu(layer l, జాలం net);
శూన్య backward_reorg_layer_gpu(layer l, జాలం net);
#అయితే_ముగింపు

#అయితే_ముగింపు


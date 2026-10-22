#నిర్వచితం_కాకపోతే UPSAMPLE_LAYER_H
#నిర్వచించు UPSAMPLE_LAYER_H
#అన్తర్భావ "darknet.h"

layer make_upsample_layer(పూర్ణాంకం batch, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం c, పూర్ణాంకం stride);
శూన్య forward_upsample_layer(స్థిరరాశి layer l, జాలం net);
శూన్య backward_upsample_layer(స్థిరరాశి layer l, జాలం net);
శూన్య resize_upsample_layer(layer *l, పూర్ణాంకం w, పూర్ణాంకం h);

#నిర్వచితమైతే GPU
శూన్య forward_upsample_layer_gpu(స్థిరరాశి layer l, జాలం net);
శూన్య backward_upsample_layer_gpu(స్థిరరాశి layer l, జాలం net);
#అయితే_ముగింపు

#అయితే_ముగింపు

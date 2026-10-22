#నిర్వచితం_కాకపోతే ISEG_LAYER_H
#నిర్వచించు ISEG_LAYER_H

#అన్తర్భావ "darknet.h"
#అన్తర్భావ "layer.h"
#అన్తర్భావ "network.h"

layer make_iseg_layer(పూర్ణాంకం batch, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం classes, పూర్ణాంకం ids);
శూన్య forward_iseg_layer(స్థిరరాశి layer l, జాలం net);
శూన్య backward_iseg_layer(స్థిరరాశి layer l, జాలం net);
శూన్య resize_iseg_layer(layer *l, పూర్ణాంకం w, పూర్ణాంకం h);
పూర్ణాంకం iseg_అంకె_detections(layer l, దశాంశం thresh);

#నిర్వచితమైతే GPU
శూన్య forward_iseg_layer_gpu(స్థిరరాశి layer l, జాలం net);
శూన్య backward_iseg_layer_gpu(layer l, జాలం net);
#అయితే_ముగింపు

#అయితే_ముగింపు

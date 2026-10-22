#నిర్వచితం_కాకపోతే DETECTION_LAYER_H
#నిర్వచించు DETECTION_LAYER_H

#అన్తర్భావ "layer.h"
#అన్తర్భావ "network.h"

రకనిర్వచనం layer detection_layer;

detection_layer make_detection_layer(పూర్ణాంకం batch, పూర్ణాంకం inputs, పూర్ణాంకం n, పూర్ణాంకం కొలత, పూర్ణాంకం classes, పూర్ణాంకం coords, పూర్ణాంకం rescore);
శూన్య forward_detection_layer(స్థిరరాశి detection_layer l, జాలం net);
శూన్య backward_detection_layer(స్థిరరాశి detection_layer l, జాలం net);

#నిర్వచితమైతే GPU
శూన్య forward_detection_layer_gpu(స్థిరరాశి detection_layer l, జాలం net);
శూన్య backward_detection_layer_gpu(detection_layer l, జాలం net);
#అయితే_ముగింపు

#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే AVGPOOL_LAYER_H
#నిర్వచించు AVGPOOL_LAYER_H

#అన్తర్భావ "image.h"
#అన్తర్భావ "cuda.h"
#అన్తర్భావ "layer.h"
#అన్తర్భావ "network.h"

రకనిర్వచనం layer avgpool_layer;

image get_avgpool_image(avgpool_layer l);
avgpool_layer make_avgpool_layer(పూర్ణాంకం batch, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం c);
శూన్య resize_avgpool_layer(avgpool_layer *l, పూర్ణాంకం w, పూర్ణాంకం h);
శూన్య forward_avgpool_layer(స్థిరరాశి avgpool_layer l, జాలం net);
శూన్య backward_avgpool_layer(స్థిరరాశి avgpool_layer l, జాలం net);

#నిర్వచితమైతే GPU
శూన్య forward_avgpool_layer_gpu(avgpool_layer l, జాలం net);
శూన్య backward_avgpool_layer_gpu(avgpool_layer l, జాలం net);
#అయితే_ముగింపు

#అయితే_ముగింపు


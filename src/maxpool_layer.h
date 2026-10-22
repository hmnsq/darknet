#నిర్వచితం_కాకపోతే MAXPOOL_LAYER_H
#నిర్వచించు MAXPOOL_LAYER_H

#అన్తర్భావ "image.h"
#అన్తర్భావ "cuda.h"
#అన్తర్భావ "layer.h"
#అన్తర్భావ "network.h"

రకనిర్వచనం layer maxpool_layer;

image get_maxpool_image(maxpool_layer l);
maxpool_layer make_maxpool_layer(పూర్ణాంకం batch, పూర్ణాంకం h, పూర్ణాంకం w, పూర్ణాంకం c, పూర్ణాంకం కొలత, పూర్ణాంకం stride, పూర్ణాంకం padding);
శూన్య resize_maxpool_layer(maxpool_layer *l, పూర్ణాంకం w, పూర్ణాంకం h);
శూన్య forward_maxpool_layer(స్థిరరాశి maxpool_layer l, జాలం net);
శూన్య backward_maxpool_layer(స్థిరరాశి maxpool_layer l, జాలం net);

#నిర్వచితమైతే GPU
శూన్య forward_maxpool_layer_gpu(maxpool_layer l, జాలం net);
శూన్య backward_maxpool_layer_gpu(maxpool_layer l, జాలం net);
#అయితే_ముగింపు

#అయితే_ముగింపు


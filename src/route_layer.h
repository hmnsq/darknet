#నిర్వచితం_కాకపోతే ROUTE_LAYER_H
#నిర్వచించు ROUTE_LAYER_H
#అన్తర్భావ "network.h"
#అన్తర్భావ "layer.h"

రకనిర్వచనం layer route_layer;

route_layer make_route_layer(పూర్ణాంకం batch, పూర్ణాంకం n, పూర్ణాంకం *ఆదానం_layers, పూర్ణాంకం *ఆదానం_కొలత);
శూన్య forward_route_layer(స్థిరరాశి route_layer l, జాలం net);
శూన్య backward_route_layer(స్థిరరాశి route_layer l, జాలం net);
శూన్య resize_route_layer(route_layer *l, జాలం *net);

#నిర్వచితమైతే GPU
శూన్య forward_route_layer_gpu(స్థిరరాశి route_layer l, జాలం net);
శూన్య backward_route_layer_gpu(స్థిరరాశి route_layer l, జాలం net);
#అయితే_ముగింపు

#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే ఖర్చు_LAYER_H
#నిర్వచించు ఖర్చు_LAYER_H
#అన్తర్భావ "layer.h"
#అన్తర్భావ "network.h"

రకనిర్వచనం layer ఖర్చు_layer;

ఖర్చు_TYPE get_ఖర్చు_type(అక్షరం *s);
అక్షరం *get_ఖర్చు_string(ఖర్చు_TYPE a);
ఖర్చు_layer make_ఖర్చు_layer(పూర్ణాంకం batch, పూర్ణాంకం inputs, ఖర్చు_TYPE type, దశాంశం scale);
శూన్య forward_ఖర్చు_layer(స్థిరరాశి ఖర్చు_layer l, జాలం net);
శూన్య backward_ఖర్చు_layer(స్థిరరాశి ఖర్చు_layer l, జాలం net);
శూన్య resize_ఖర్చు_layer(ఖర్చు_layer *l, పూర్ణాంకం inputs);

#నిర్వచితమైతే GPU
శూన్య forward_ఖర్చు_layer_gpu(ఖర్చు_layer l, జాలం net);
శూన్య backward_ఖర్చు_layer_gpu(స్థిరరాశి ఖర్చు_layer l, జాలం net);
#అయితే_ముగింపు

#అయితే_ముగింపు

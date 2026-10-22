#నిర్వచితం_కాకపోతే SHORTCUT_LAYER_H
#నిర్వచించు SHORTCUT_LAYER_H

#అన్తర్భావ "layer.h"
#అన్తర్భావ "network.h"

layer make_shortcut_layer(పూర్ణాంకం batch, పూర్ణాంకం సూచీ, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం c, పూర్ణాంకం w2, పూర్ణాంకం h2, పూర్ణాంకం c2);
శూన్య forward_shortcut_layer(స్థిరరాశి layer l, జాలం net);
శూన్య backward_shortcut_layer(స్థిరరాశి layer l, జాలం net);
శూన్య resize_shortcut_layer(layer *l, పూర్ణాంకం w, పూర్ణాంకం h);

#నిర్వచితమైతే GPU
శూన్య forward_shortcut_layer_gpu(స్థిరరాశి layer l, జాలం net);
శూన్య backward_shortcut_layer_gpu(స్థిరరాశి layer l, జాలం net);
#అయితే_ముగింపు

#అయితే_ముగింపు

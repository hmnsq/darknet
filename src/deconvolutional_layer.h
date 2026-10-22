#నిర్వచితం_కాకపోతే DECONVOLUTIONAL_LAYER_H
#నిర్వచించు DECONVOLUTIONAL_LAYER_H

#అన్తర్భావ "cuda.h"
#అన్తర్భావ "image.h"
#అన్తర్భావ "activations.h"
#అన్తర్భావ "layer.h"
#అన్తర్భావ "network.h"

#నిర్వచితమైతే GPU
శూన్య forward_deconvolutional_layer_gpu(layer l, జాలం net);
శూన్య backward_deconvolutional_layer_gpu(layer l, జాలం net);
శూన్య update_deconvolutional_layer_gpu(layer l, update_వాదనలు a);
శూన్య నెట్టు_deconvolutional_layer(layer l);
శూన్య pull_deconvolutional_layer(layer l);
#అయితే_ముగింపు

layer make_deconvolutional_layer(పూర్ణాంకం batch, పూర్ణాంకం h, పూర్ణాంకం w, పూర్ణాంకం c, పూర్ణాంకం n, పూర్ణాంకం కొలత, పూర్ణాంకం stride, పూర్ణాంకం padding, ACTIVATION activation, పూర్ణాంకం batch_normalize, పూర్ణాంకం adam);
శూన్య resize_deconvolutional_layer(layer *l, పూర్ణాంకం h, పూర్ణాంకం w);
శూన్య forward_deconvolutional_layer(స్థిరరాశి layer l, జాలం net);
శూన్య update_deconvolutional_layer(layer l, update_వాదనలు a);
శూన్య backward_deconvolutional_layer(layer l, జాలం net);

#అయితే_ముగింపు


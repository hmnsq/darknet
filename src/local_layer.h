#నిర్వచితం_కాకపోతే LOCAL_LAYER_H
#నిర్వచించు LOCAL_LAYER_H

#అన్తర్భావ "cuda.h"
#అన్తర్భావ "image.h"
#అన్తర్భావ "activations.h"
#అన్తర్భావ "layer.h"
#అన్తర్భావ "network.h"

రకనిర్వచనం layer local_layer;

#నిర్వచితమైతే GPU
శూన్య forward_local_layer_gpu(local_layer layer, జాలం net);
శూన్య backward_local_layer_gpu(local_layer layer, జాలం net);
శూన్య update_local_layer_gpu(local_layer layer, update_వాదనలు a);

శూన్య నెట్టు_local_layer(local_layer layer);
శూన్య pull_local_layer(local_layer layer);
#అయితే_ముగింపు

local_layer make_local_layer(పూర్ణాంకం batch, పూర్ణాంకం h, పూర్ణాంకం w, పూర్ణాంకం c, పూర్ణాంకం n, పూర్ణాంకం కొలత, పూర్ణాంకం stride, పూర్ణాంకం pad, ACTIVATION activation);

శూన్య forward_local_layer(స్థిరరాశి local_layer layer, జాలం net);
శూన్య backward_local_layer(local_layer layer, జాలం net);
శూన్య update_local_layer(local_layer layer, update_వాదనలు a);

శూన్య bias_output(దశాంశం *output, దశాంశం *biases, పూర్ణాంకం batch, పూర్ణాంకం n, పూర్ణాంకం కొలత);
శూన్య backward_bias(దశాంశం *bias_updates, దశాంశం *delta, పూర్ణాంకం batch, పూర్ణాంకం n, పూర్ణాంకం కొలత);

#అయితే_ముగింపు


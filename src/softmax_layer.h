#నిర్వచితం_కాకపోతే SOFTMAX_LAYER_H
#నిర్వచించు SOFTMAX_LAYER_H
#అన్తర్భావ "layer.h"
#అన్తర్భావ "network.h"

రకనిర్వచనం layer softmax_layer;

శూన్య softmax_array(దశాంశం *ఆదానం, పూర్ణాంకం n, దశాంశం తాత్కాలిక, దశాంశం *output);
softmax_layer make_softmax_layer(పూర్ణాంకం batch, పూర్ణాంకం inputs, పూర్ణాంకం groups);
శూన్య forward_softmax_layer(స్థిరరాశి softmax_layer l, జాలం net);
శూన్య backward_softmax_layer(స్థిరరాశి softmax_layer l, జాలం net);

#నిర్వచితమైతే GPU
శూన్య pull_softmax_layer_output(స్థిరరాశి softmax_layer l);
శూన్య forward_softmax_layer_gpu(స్థిరరాశి softmax_layer l, జాలం net);
శూన్య backward_softmax_layer_gpu(స్థిరరాశి softmax_layer l, జాలం net);
#అయితే_ముగింపు

#అయితే_ముగింపు

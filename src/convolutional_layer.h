#నిర్వచితం_కాకపోతే CONVOLUTIONAL_LAYER_H
#నిర్వచించు CONVOLUTIONAL_LAYER_H

#అన్తర్భావ "cuda.h"
#అన్తర్భావ "image.h"
#అన్తర్భావ "activations.h"
#అన్తర్భావ "layer.h"
#అన్తర్భావ "network.h"

రకనిర్వచనం layer convolutional_layer;

#నిర్వచితమైతే GPU
శూన్య forward_convolutional_layer_gpu(convolutional_layer layer, జాలం net);
శూన్య backward_convolutional_layer_gpu(convolutional_layer layer, జాలం net);
శూన్య update_convolutional_layer_gpu(convolutional_layer layer, update_వాదనలు a);

శూన్య నెట్టు_convolutional_layer(convolutional_layer layer);
శూన్య pull_convolutional_layer(convolutional_layer layer);

శూన్య కూడు_bias_gpu(దశాంశం *output, దశాంశం *biases, పూర్ణాంకం batch, పూర్ణాంకం n, పూర్ణాంకం కొలత);
శూన్య backward_bias_gpu(దశాంశం *bias_updates, దశాంశం *delta, పూర్ణాంకం batch, పూర్ణాంకం n, పూర్ణాంకం కొలత);
శూన్య adam_update_gpu(దశాంశం *w, దశాంశం *d, దశాంశం *m, దశాంశం *v, దశాంశం B1, దశాంశం B2, దశాంశం eps, దశాంశం decay, దశాంశం రేటు, పూర్ణాంకం n, పూర్ణాంకం batch, పూర్ణాంకం t);
#నిర్వచితమైతే CUDNN
శూన్య cudnn_convolutional_అమరిక(layer *l);
#అయితే_ముగింపు
#అయితే_ముగింపు

convolutional_layer make_convolutional_layer(పూర్ణాంకం batch, పూర్ణాంకం h, పూర్ణాంకం w, పూర్ణాంకం c, పూర్ణాంకం n, పూర్ణాంకం groups, పూర్ణాంకం కొలత, పూర్ణాంకం stride, పూర్ణాంకం padding, ACTIVATION activation, పూర్ణాంకం batch_normalize, పూర్ణాంకం ద్విమాన, పూర్ణాంకం xnor, పూర్ణాంకం adam);
శూన్య resize_convolutional_layer(convolutional_layer *layer, పూర్ణాంకం w, పూర్ణాంకం h);
శూన్య forward_convolutional_layer(స్థిరరాశి convolutional_layer layer, జాలం net);
శూన్య update_convolutional_layer(convolutional_layer layer, update_వాదనలు a);
image *visualize_convolutional_layer(convolutional_layer layer, అక్షరం *కిటికీ, image *prev_weights);
శూన్య binarize_weights(దశాంశం *weights, పూర్ణాంకం n, పూర్ణాంకం కొలత, దశాంశం *ద్విమాన);
శూన్య మార్పిడి_ద్విమాన(convolutional_layer *l);
శూన్య binarize_weights2(దశాంశం *weights, పూర్ణాంకం n, పూర్ణాంకం కొలత, అక్షరం *ద్విమాన, దశాంశం *scales);

శూన్య backward_convolutional_layer(convolutional_layer layer, జాలం net);

శూన్య కూడు_bias(దశాంశం *output, దశాంశం *biases, పూర్ణాంకం batch, పూర్ణాంకం n, పూర్ణాంకం కొలత);
శూన్య backward_bias(దశాంశం *bias_updates, దశాంశం *delta, పూర్ణాంకం batch, పూర్ణాంకం n, పూర్ణాంకం కొలత);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_బరువు(convolutional_layer layer, పూర్ణాంకం i);

పూర్ణాంకం convolutional_బహిర్గతం_ఎత్తు(convolutional_layer layer);
పూర్ణాంకం convolutional_బహిర్గతం_వెడల్పు(convolutional_layer layer);

#అయితే_ముగింపు


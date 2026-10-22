#यदि_अपरिभाषितम् CONVOLUTIONAL_LAYER_H
#परिभाषय CONVOLUTIONAL_LAYER_H

#अन्तर्भाव "cuda.h"
#अन्तर्भाव "image.h"
#अन्तर्भाव "activations.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

प्रकारनिर्देश layer convolutional_layer;

#यदि_परिभाषितम् GPU
शून्य forward_convolutional_layer_gpu(convolutional_layer layer, जालम् net);
शून्य backward_convolutional_layer_gpu(convolutional_layer layer, जालम् net);
शून्य update_convolutional_layer_gpu(convolutional_layer layer, update_तर्काः a);

शून्य प्रक्षिप_convolutional_layer(convolutional_layer layer);
शून्य pull_convolutional_layer(convolutional_layer layer);

शून्य सङ्कलय_bias_gpu(भिन्न *output, भिन्न *biases, पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क आकार);
शून्य backward_bias_gpu(भिन्न *bias_updates, भिन्न *delta, पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क आकार);
शून्य adam_update_gpu(भिन्न *w, भिन्न *d, भिन्न *m, भिन्न *v, भिन्न B1, भिन्न B2, भिन्न eps, भिन्न decay, भिन्न दरः, पूर्णाङ्क n, पूर्णाङ्क batch, पूर्णाङ्क t);
#यदि_परिभाषितम् CUDNN
शून्य cudnn_convolutional_सज्जता(layer *l);
#यदि_अन्तः
#यदि_अन्तः

convolutional_layer make_convolutional_layer(पूर्णाङ्क batch, पूर्णाङ्क h, पूर्णाङ्क w, पूर्णाङ्क c, पूर्णाङ्क n, पूर्णाङ्क groups, पूर्णाङ्क आकार, पूर्णाङ्क stride, पूर्णाङ्क padding, ACTIVATION activation, पूर्णाङ्क batch_normalize, पूर्णाङ्क द्विमान, पूर्णाङ्क xnor, पूर्णाङ्क adam);
शून्य resize_convolutional_layer(convolutional_layer *layer, पूर्णाङ्क w, पूर्णाङ्क h);
शून्य forward_convolutional_layer(ध्रुव convolutional_layer layer, जालम् net);
शून्य update_convolutional_layer(convolutional_layer layer, update_तर्काः a);
image *visualize_convolutional_layer(convolutional_layer layer, वर्ण *गवाक्षः, image *पूर्व_weights);
शून्य binarize_weights(भिन्न *weights, पूर्णाङ्क n, पूर्णाङ्क आकार, भिन्न *द्विमान);
शून्य swap_द्विमान(convolutional_layer *l);
शून्य binarize_weights2(भिन्न *weights, पूर्णाङ्क n, पूर्णाङ्क आकार, वर्ण *द्विमान, भिन्न *scales);

शून्य backward_convolutional_layer(convolutional_layer layer, जालम् net);

शून्य सङ्कलय_bias(भिन्न *output, भिन्न *biases, पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क आकार);
शून्य backward_bias(भिन्न *bias_updates, भिन्न *delta, पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क आकार);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_weight(convolutional_layer layer, पूर्णाङ्क i);

पूर्णाङ्क convolutional_बहिर्गमनम्_height(convolutional_layer layer);
पूर्णाङ्क convolutional_बहिर्गमनम्_विस्तारः(convolutional_layer layer);

#यदि_अन्तः


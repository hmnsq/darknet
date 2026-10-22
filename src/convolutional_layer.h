#ifndef CONVOLUTIONAL_LAYER_H
#define CONVOLUTIONAL_LAYER_H

#अन्तर्भाव "cuda.h"
#अन्तर्भाव "image.h"
#अन्तर्भाव "activations.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

प्रकारव्याख्या layer convolutional_layer;

#ifdef GPU
शून्य forward_convolutional_layer_gpu(convolutional_layer layer, network net);
शून्य backward_convolutional_layer_gpu(convolutional_layer layer, network net);
शून्य update_convolutional_layer_gpu(convolutional_layer layer, update_args a);

शून्य ढकला_convolutional_layer(convolutional_layer layer);
शून्य pull_convolutional_layer(convolutional_layer layer);

शून्य add_bias_gpu(दशांश *output, दशांश *biases, पूर्णांक batch, पूर्णांक n, पूर्णांक size);
शून्य backward_bias_gpu(दशांश *bias_updates, दशांश *delta, पूर्णांक batch, पूर्णांक n, पूर्णांक size);
शून्य adam_update_gpu(दशांश *w, दशांश *d, दशांश *m, दशांश *v, दशांश B1, दशांश B2, दशांश eps, दशांश decay, दशांश rate, पूर्णांक n, पूर्णांक batch, पूर्णांक t);
#ifdef CUDNN
शून्य cudnn_convolutional_setup(layer *l);
#endif
#endif

convolutional_layer make_convolutional_layer(पूर्णांक batch, पूर्णांक h, पूर्णांक w, पूर्णांक c, पूर्णांक n, पूर्णांक groups, पूर्णांक size, पूर्णांक stride, पूर्णांक padding, ACTIVATION activation, पूर्णांक batch_normalize, पूर्णांक द्विमान, पूर्णांक xnor, पूर्णांक adam);
शून्य resize_convolutional_layer(convolutional_layer *layer, पूर्णांक w, पूर्णांक h);
शून्य forward_convolutional_layer(स्थिरांक convolutional_layer layer, network net);
शून्य update_convolutional_layer(convolutional_layer layer, update_args a);
image *visualize_convolutional_layer(convolutional_layer layer, अक्षर *window, image *prev_weights);
शून्य binarize_weights(दशांश *weights, पूर्णांक n, पूर्णांक size, दशांश *द्विमान);
शून्य swap_द्विमान(convolutional_layer *l);
शून्य binarize_weights2(दशांश *weights, पूर्णांक n, पूर्णांक size, अक्षर *द्विमान, दशांश *scales);

शून्य backward_convolutional_layer(convolutional_layer layer, network net);

शून्य add_bias(दशांश *output, दशांश *biases, पूर्णांक batch, पूर्णांक n, पूर्णांक size);
शून्य backward_bias(दशांश *bias_updates, दशांश *delta, पूर्णांक batch, पूर्णांक n, पूर्णांक size);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_weight(convolutional_layer layer, पूर्णांक i);

पूर्णांक convolutional_out_height(convolutional_layer layer);
पूर्णांक convolutional_out_width(convolutional_layer layer);

#endif


#अगर_अघोषित CONVOLUTIONAL_LAYER_H
#घोषणा CONVOLUTIONAL_LAYER_H

#समावेश "cuda.h"
#समावेश "image.h"
#समावेश "activations.h"
#समावेश "layer.h"
#समावेश "network.h"

प्रकार layer convolutional_layer;

#अगर_घोषित GPU
व्योम forward_convolutional_layer_gpu(convolutional_layer layer, network net);
व्योम backward_convolutional_layer_gpu(convolutional_layer layer, network net);
व्योम update_convolutional_layer_gpu(convolutional_layer layer, update_args a);

व्योम धकेलो_convolutional_layer(convolutional_layer layer);
व्योम pull_convolutional_layer(convolutional_layer layer);

व्योम add_bias_gpu(भग्न *output, भग्न *biases, पूर्णांक batch, पूर्णांक n, पूर्णांक आकार);
व्योम backward_bias_gpu(भग्न *bias_updates, भग्न *delta, पूर्णांक batch, पूर्णांक n, पूर्णांक आकार);
व्योम adam_update_gpu(भग्न *w, भग्न *d, भग्न *m, भग्न *v, भग्न B1, भग्न B2, भग्न eps, भग्न decay, भग्न rate, पूर्णांक n, पूर्णांक batch, पूर्णांक t);
#अगर_घोषित CUDNN
व्योम cudnn_convolutional_setup(layer *l);
#पूर्ण_अगर
#पूर्ण_अगर

convolutional_layer make_convolutional_layer(पूर्णांक batch, पूर्णांक h, पूर्णांक w, पूर्णांक c, पूर्णांक n, पूर्णांक groups, पूर्णांक आकार, पूर्णांक stride, पूर्णांक padding, ACTIVATION activation, पूर्णांक batch_normalize, पूर्णांक द्विआधारी, पूर्णांक xnor, पूर्णांक adam);
व्योम resize_convolutional_layer(convolutional_layer *layer, पूर्णांक w, पूर्णांक h);
व्योम forward_convolutional_layer(स्थिर convolutional_layer layer, network net);
व्योम update_convolutional_layer(convolutional_layer layer, update_args a);
image *visualize_convolutional_layer(convolutional_layer layer, अक्षर *window, image *पूर्व_weights);
व्योम binarize_weights(भग्न *weights, पूर्णांक n, पूर्णांक आकार, भग्न *द्विआधारी);
व्योम swap_द्विआधारी(convolutional_layer *l);
व्योम binarize_weights2(भग्न *weights, पूर्णांक n, पूर्णांक आकार, अक्षर *द्विआधारी, भग्न *scales);

व्योम backward_convolutional_layer(convolutional_layer layer, network net);

व्योम add_bias(भग्न *output, भग्न *biases, पूर्णांक batch, पूर्णांक n, पूर्णांक आकार);
व्योम backward_bias(भग्न *bias_updates, भग्न *delta, पूर्णांक batch, पूर्णांक n, पूर्णांक आकार);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_weight(convolutional_layer layer, पूर्णांक i);

पूर्णांक convolutional_out_height(convolutional_layer layer);
पूर्णांक convolutional_out_width(convolutional_layer layer);

#पूर्ण_अगर


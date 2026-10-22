#ifndef CONVOLUTIONAL_LAYER_H
#define CONVOLUTIONAL_LAYER_H

#அந்தர்பாவ "cuda.h"
#அந்தர்பாவ "image.h"
#அந்தர்பாவ "activations.h"
#அந்தர்பாவ "layer.h"
#அந்தர்பாவ "network.h"

வகைவரை layer convolutional_layer;

#ifdef GPU
வெறுமை forward_convolutional_layer_gpu(convolutional_layer layer, network net);
வெறுமை backward_convolutional_layer_gpu(convolutional_layer layer, network net);
வெறுமை update_convolutional_layer_gpu(convolutional_layer layer, update_args a);

வெறுமை தள்ளு_convolutional_layer(convolutional_layer layer);
வெறுமை pull_convolutional_layer(convolutional_layer layer);

வெறுமை add_bias_gpu(தசம *output, தசம *biases, முழு batch, முழு n, முழு size);
வெறுமை backward_bias_gpu(தசம *bias_updates, தசம *delta, முழு batch, முழு n, முழு size);
வெறுமை adam_update_gpu(தசம *w, தசம *d, தசம *m, தசம *v, தசம B1, தசம B2, தசம eps, தசம decay, தசம rate, முழு n, முழு batch, முழு t);
#ifdef CUDNN
வெறுமை cudnn_convolutional_setup(layer *l);
#endif
#endif

convolutional_layer make_convolutional_layer(முழு batch, முழு h, முழு w, முழு c, முழு n, முழு groups, முழு size, முழு stride, முழு padding, ACTIVATION activation, முழு batch_normalize, முழு இருமம், முழு xnor, முழு adam);
வெறுமை resize_convolutional_layer(convolutional_layer *layer, முழு w, முழு h);
வெறுமை forward_convolutional_layer(மாறிலி convolutional_layer layer, network net);
வெறுமை update_convolutional_layer(convolutional_layer layer, update_args a);
image *visualize_convolutional_layer(convolutional_layer layer, எழுத்து *window, image *prev_weights);
வெறுமை binarize_weights(தசம *weights, முழு n, முழு size, தசம *இருமம்);
வெறுமை swap_இருமம்(convolutional_layer *l);
வெறுமை binarize_weights2(தசம *weights, முழு n, முழு size, எழுத்து *இருமம், தசம *scales);

வெறுமை backward_convolutional_layer(convolutional_layer layer, network net);

வெறுமை add_bias(தசம *output, தசம *biases, முழு batch, முழு n, முழு size);
வெறுமை backward_bias(தசம *bias_updates, தசம *delta, முழு batch, முழு n, முழு size);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_weight(convolutional_layer layer, முழு i);

முழு convolutional_out_height(convolutional_layer layer);
முழு convolutional_out_width(convolutional_layer layer);

#endif


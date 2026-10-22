#ifndef CONVOLUTIONAL_LAYER_H
#define CONVOLUTIONAL_LAYER_H

#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব "image.h"
#অন্তর্ভাব "activations.h"
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

প্রকারলেখ layer convolutional_layer;

#ifdef GPU
শূন্য forward_convolutional_layer_gpu(convolutional_layer layer, network net);
শূন্য backward_convolutional_layer_gpu(convolutional_layer layer, network net);
শূন্য update_convolutional_layer_gpu(convolutional_layer layer, update_args a);

শূন্য ঠেলো_convolutional_layer(convolutional_layer layer);
শূন্য pull_convolutional_layer(convolutional_layer layer);

শূন্য add_bias_gpu(দশমিক *output, দশমিক *biases, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা n, পূর্ণসংখ্যা আকার_2);
শূন্য backward_bias_gpu(দশমিক *bias_updates, দশমিক *delta, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা n, পূর্ণসংখ্যা আকার_2);
শূন্য adam_update_gpu(দশমিক *w, দশমিক *d, দশমিক *m, দশমিক *v, দশমিক B1, দশমিক B2, দশমিক eps, দশমিক decay, দশমিক rate, পূর্ণসংখ্যা n, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা t);
#ifdef CUDNN
শূন্য cudnn_convolutional_setup(layer *l);
#endif
#endif

convolutional_layer make_convolutional_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা h, পূর্ণসংখ্যা w, পূর্ণসংখ্যা c, পূর্ণসংখ্যা n, পূর্ণসংখ্যা groups, পূর্ণসংখ্যা আকার_2, পূর্ণসংখ্যা stride, পূর্ণসংখ্যা padding, ACTIVATION activation, পূর্ণসংখ্যা batch_normalize, পূর্ণসংখ্যা দ্বিমিক, পূর্ণসংখ্যা xnor, পূর্ণসংখ্যা adam);
শূন্য resize_convolutional_layer(convolutional_layer *layer, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h);
শূন্য forward_convolutional_layer(ধ্রুবক convolutional_layer layer, network net);
শূন্য update_convolutional_layer(convolutional_layer layer, update_args a);
image *visualize_convolutional_layer(convolutional_layer layer, অক্ষর *window, image *পূর্ব_weights);
শূন্য binarize_weights(দশমিক *weights, পূর্ণসংখ্যা n, পূর্ণসংখ্যা আকার_2, দশমিক *দ্বিমিক);
শূন্য swap_দ্বিমিক(convolutional_layer *l);
শূন্য binarize_weights2(দশমিক *weights, পূর্ণসংখ্যা n, পূর্ণসংখ্যা আকার_2, অক্ষর *দ্বিমিক, দশমিক *scales);

শূন্য backward_convolutional_layer(convolutional_layer layer, network net);

শূন্য add_bias(দশমিক *output, দশমিক *biases, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা n, পূর্ণসংখ্যা আকার_2);
শূন্য backward_bias(দশমিক *bias_updates, দশমিক *delta, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা n, পূর্ণসংখ্যা আকার_2);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_weight(convolutional_layer layer, পূর্ণসংখ্যা i);

পূর্ণসংখ্যা convolutional_out_height(convolutional_layer layer);
পূর্ণসংখ্যা convolutional_out_width(convolutional_layer layer);

#endif


#ifndef CONVOLUTIONAL_LAYER_H
#define CONVOLUTIONAL_LAYER_H

#অন্তর্ভাৱ "cuda.h"
#অন্তর্ভাৱ "image.h"
#অন্তর্ভাৱ "activations.h"
#অন্তর্ভাৱ "layer.h"
#অন্তর্ভাৱ "network.h"

প্রকারনির্দেশ layer convolutional_layer;

#ifdef GPU
শূন্য forward_convolutional_layer_gpu(convolutional_layer layer, network net);
শূন্য backward_convolutional_layer_gpu(convolutional_layer layer, network net);
শূন্য update_convolutional_layer_gpu(convolutional_layer layer, update_args a);

শূন্য ঠেলো_convolutional_layer(convolutional_layer layer);
শূন্য pull_convolutional_layer(convolutional_layer layer);

শূন্য add_bias_gpu(ভিন্ন *output, ভিন্ন *biases, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক n, পূর্ণাঙ্ক size);
শূন্য backward_bias_gpu(ভিন্ন *bias_updates, ভিন্ন *delta, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক n, পূর্ণাঙ্ক size);
শূন্য adam_update_gpu(ভিন্ন *w, ভিন্ন *d, ভিন্ন *m, ভিন্ন *v, ভিন্ন B1, ভিন্ন B2, ভিন্ন eps, ভিন্ন decay, ভিন্ন rate, পূর্ণাঙ্ক n, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক t);
#ifdef CUDNN
শূন্য cudnn_convolutional_setup(layer *l);
#endif
#endif

convolutional_layer make_convolutional_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক h, পূর্ণাঙ্ক w, পূর্ণাঙ্ক c, পূর্ণাঙ্ক n, পূর্ণাঙ্ক groups, পূর্ণাঙ্ক size, পূর্ণাঙ্ক stride, পূর্ণাঙ্ক padding, ACTIVATION activation, পূর্ণাঙ্ক batch_normalize, পূর্ণাঙ্ক দ্বিমিক, পূর্ণাঙ্ক xnor, পূর্ণাঙ্ক adam);
শূন্য resize_convolutional_layer(convolutional_layer *layer, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h);
শূন্য forward_convolutional_layer(ধ্রুৱ convolutional_layer layer, network net);
শূন্য update_convolutional_layer(convolutional_layer layer, update_args a);
image *visualize_convolutional_layer(convolutional_layer layer, ৱর্ণ *window, image *prev_weights);
শূন্য binarize_weights(ভিন্ন *weights, পূর্ণাঙ্ক n, পূর্ণাঙ্ক size, ভিন্ন *দ্বিমিক);
শূন্য swap_দ্বিমিক(convolutional_layer *l);
শূন্য binarize_weights2(ভিন্ন *weights, পূর্ণাঙ্ক n, পূর্ণাঙ্ক size, ৱর্ণ *দ্বিমিক, ভিন্ন *scales);

শূন্য backward_convolutional_layer(convolutional_layer layer, network net);

শূন্য add_bias(ভিন্ন *output, ভিন্ন *biases, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক n, পূর্ণাঙ্ক size);
শূন্য backward_bias(ভিন্ন *bias_updates, ভিন্ন *delta, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক n, পূর্ণাঙ্ক size);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_weight(convolutional_layer layer, পূর্ণাঙ্ক i);

পূর্ণাঙ্ক convolutional_out_height(convolutional_layer layer);
পূর্ণাঙ্ক convolutional_out_width(convolutional_layer layer);

#endif


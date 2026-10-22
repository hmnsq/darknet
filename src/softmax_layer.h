#ifndef SOFTMAX_LAYER_H
#define SOFTMAX_LAYER_H
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

ಪ್ರಕಾರನಿರ್ದೇಶ layer softmax_layer;

ಶೂನ್ಯ softmax_array(ಭಿನ್ನ *ಒಳಹರಿವು, ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ temp, ಭಿನ್ನ *output);
softmax_layer make_softmax_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ inputs, ಪೂರ್ಣಾಙ್ಕ groups);
ಶೂನ್ಯ forward_softmax_layer(ಧ್ರುವ softmax_layer l, network net);
ಶೂನ್ಯ backward_softmax_layer(ಧ್ರುವ softmax_layer l, network net);

#ifdef GPU
ಶೂನ್ಯ pull_softmax_layer_output(ಧ್ರುವ softmax_layer l);
ಶೂನ್ಯ forward_softmax_layer_gpu(ಧ್ರುವ softmax_layer l, network net);
ಶೂನ್ಯ backward_softmax_layer_gpu(ಧ್ರುವ softmax_layer l, network net);
#endif

#endif

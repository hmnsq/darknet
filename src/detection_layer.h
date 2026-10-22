#ifndef DETECTION_LAYER_H
#define DETECTION_LAYER_H

#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

ಪ್ರಕಾರನಿರ್ದೇಶ layer detection_layer;

detection_layer make_detection_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ inputs, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ size, ಪೂರ್ಣಾಙ್ಕ classes, ಪೂರ್ಣಾಙ್ಕ coords, ಪೂರ್ಣಾಙ್ಕ rescore);
ಶೂನ್ಯ forward_detection_layer(ಧ್ರುವ detection_layer l, network net);
ಶೂನ್ಯ backward_detection_layer(ಧ್ರುವ detection_layer l, network net);

#ifdef GPU
ಶೂನ್ಯ forward_detection_layer_gpu(ಧ್ರುವ detection_layer l, network net);
ಶೂನ್ಯ backward_detection_layer_gpu(detection_layer l, network net);
#endif

#endif

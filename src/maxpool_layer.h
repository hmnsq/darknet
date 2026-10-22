#ifndef MAXPOOL_LAYER_H
#define MAXPOOL_LAYER_H

#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

ಪ್ರಕಾರನಿರ್ದೇಶ layer maxpool_layer;

image get_maxpool_image(maxpool_layer l);
maxpool_layer make_maxpool_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ size, ಪೂರ್ಣಾಙ್ಕ stride, ಪೂರ್ಣಾಙ್ಕ padding);
ಶೂನ್ಯ resize_maxpool_layer(maxpool_layer *l, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h);
ಶೂನ್ಯ forward_maxpool_layer(ಧ್ರುವ maxpool_layer l, network net);
ಶೂನ್ಯ backward_maxpool_layer(ಧ್ರುವ maxpool_layer l, network net);

#ifdef GPU
ಶೂನ್ಯ forward_maxpool_layer_gpu(maxpool_layer l, network net);
ಶೂನ್ಯ backward_maxpool_layer_gpu(maxpool_layer l, network net);
#endif

#endif


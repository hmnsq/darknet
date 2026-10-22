#ifndef REORG_LAYER_H
#define REORG_LAYER_H

#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

layer make_reorg_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ stride, ಪೂರ್ಣಾಙ್ಕ reverse, ಪೂರ್ಣಾಙ್ಕ flatten, ಪೂರ್ಣಾಙ್ಕ extra);
ಶೂನ್ಯ resize_reorg_layer(layer *l, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h);
ಶೂನ್ಯ forward_reorg_layer(ಧ್ರುವ layer l, network net);
ಶೂನ್ಯ backward_reorg_layer(ಧ್ರುವ layer l, network net);

#ifdef GPU
ಶೂನ್ಯ forward_reorg_layer_gpu(layer l, network net);
ಶೂನ್ಯ backward_reorg_layer_gpu(layer l, network net);
#endif

#endif


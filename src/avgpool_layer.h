#ifndef AVGPOOL_LAYER_H
#define AVGPOOL_LAYER_H

#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

ಪ್ರಕಾರನಿರ್ದೇಶ layer avgpool_layer;

image get_avgpool_image(avgpool_layer l);
avgpool_layer make_avgpool_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ c);
ಶೂನ್ಯ resize_avgpool_layer(avgpool_layer *l, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h);
ಶೂನ್ಯ forward_avgpool_layer(ಧ್ರುವ avgpool_layer l, network net);
ಶೂನ್ಯ backward_avgpool_layer(ಧ್ರುವ avgpool_layer l, network net);

#ifdef GPU
ಶೂನ್ಯ forward_avgpool_layer_gpu(avgpool_layer l, network net);
ಶೂನ್ಯ backward_avgpool_layer_gpu(avgpool_layer l, network net);
#endif

#endif


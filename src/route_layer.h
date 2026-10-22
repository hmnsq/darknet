#ifndef ROUTE_LAYER_H
#define ROUTE_LAYER_H
#ಅನ್ತರ್ಭಾವ "network.h"
#ಅನ್ತರ್ಭಾವ "layer.h"

ಪ್ರಕಾರನಿರ್ದೇಶ layer route_layer;

route_layer make_route_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ *ಒಳಹರಿವು_layers, ಪೂರ್ಣಾಙ್ಕ *ಒಳಹರಿವು_size);
ಶೂನ್ಯ forward_route_layer(ಧ್ರುವ route_layer l, network net);
ಶೂನ್ಯ backward_route_layer(ಧ್ರುವ route_layer l, network net);
ಶೂನ್ಯ resize_route_layer(route_layer *l, network *net);

#ifdef GPU
ಶೂನ್ಯ forward_route_layer_gpu(ಧ್ರುವ route_layer l, network net);
ಶೂನ್ಯ backward_route_layer_gpu(ಧ್ರುವ route_layer l, network net);
#endif

#endif

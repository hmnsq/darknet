#ifndef COST_LAYER_H
#define COST_LAYER_H
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

ಪ್ರಕಾರನಿರ್ದೇಶ layer cost_layer;

COST_TYPE get_cost_type(ವರ್ಣ *s);
ವರ್ಣ *get_cost_string(COST_TYPE a);
cost_layer make_cost_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ inputs, COST_TYPE type, ಭಿನ್ನ scale);
ಶೂನ್ಯ forward_cost_layer(ಧ್ರುವ cost_layer l, network net);
ಶೂನ್ಯ backward_cost_layer(ಧ್ರುವ cost_layer l, network net);
ಶೂನ್ಯ resize_cost_layer(cost_layer *l, ಪೂರ್ಣಾಙ್ಕ inputs);

#ifdef GPU
ಶೂನ್ಯ forward_cost_layer_gpu(cost_layer l, network net);
ಶೂನ್ಯ backward_cost_layer_gpu(ಧ್ರುವ cost_layer l, network net);
#endif

#endif

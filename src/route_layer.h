#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ROUTE_LAYER_H
#ವ್ಯಾಖ್ಯಾನಿಸು ROUTE_LAYER_H
#ಅನ್ತರ್ಭಾವ "network.h"
#ಅನ್ತರ್ಭಾವ "layer.h"

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ layer route_layer;

route_layer make_route_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ *ಒಳಹರಿವು_layers, ಪೂರ್ಣಾಂಕ *ಒಳಹರಿವು_ಗಾತ್ರ);
ಶೂನ್ಯ forward_route_layer(ಸ್ಥಿರಾಂಕ route_layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_route_layer(ಸ್ಥಿರಾಂಕ route_layer l, ಜಾಲ net);
ಶೂನ್ಯ resize_route_layer(route_layer *l, ಜಾಲ *net);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_route_layer_gpu(ಸ್ಥಿರಾಂಕ route_layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_route_layer_gpu(ಸ್ಥಿರಾಂಕ route_layer l, ಜಾಲ net);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ

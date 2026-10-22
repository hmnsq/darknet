#ifndef YOLO_LAYER_H
#define YOLO_LAYER_H

#ಅನ್ತರ್ಭಾವ "darknet.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

layer make_yolo_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ total, ಪೂರ್ಣಾಙ್ಕ *mask, ಪೂರ್ಣಾಙ್ಕ classes);
ಶೂನ್ಯ forward_yolo_layer(ಧ್ರುವ layer l, network net);
ಶೂನ್ಯ backward_yolo_layer(ಧ್ರುವ layer l, network net);
ಶೂನ್ಯ resize_yolo_layer(layer *l, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h);
ಪೂರ್ಣಾಙ್ಕ yolo_ಅಂಕೆ_detections(layer l, ಭಿನ್ನ thresh);

#ifdef GPU
ಶೂನ್ಯ forward_yolo_layer_gpu(ಧ್ರುವ layer l, network net);
ಶೂನ್ಯ backward_yolo_layer_gpu(layer l, network net);
#endif

#endif

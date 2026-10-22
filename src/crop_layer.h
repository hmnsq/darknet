#ifndef CROP_LAYER_H
#define CROP_LAYER_H

#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

ಪ್ರಕಾರನಿರ್ದೇಶ layer crop_layer;

image get_crop_image(crop_layer l);
crop_layer make_crop_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ crop_height, ಪೂರ್ಣಾಙ್ಕ crop_width, ಪೂರ್ಣಾಙ್ಕ flip, ಭಿನ್ನ angle, ಭಿನ್ನ saturation, ಭಿನ್ನ exposure);
ಶೂನ್ಯ forward_crop_layer(ಧ್ರುವ crop_layer l, network net);
ಶೂನ್ಯ resize_crop_layer(layer *l, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h);

#ifdef GPU
ಶೂನ್ಯ forward_crop_layer_gpu(crop_layer l, network net);
#endif

#endif


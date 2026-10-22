#ifndef CROP_LAYER_H
#define CROP_LAYER_H

#ଅନ୍ତର୍ଭାଵ "image.h"
#ଅନ୍ତର୍ଭାଵ "layer.h"
#ଅନ୍ତର୍ଭାଵ "network.h"

ପ୍ରକାରବ୍ୟାଖ୍ୟା layer crop_layer;

image get_crop_image(crop_layer l);
crop_layer make_crop_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା crop_height, ପୂର୍ଣ୍ଣସଂଖ୍ୟା crop_width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flip, ଦଶମିକ angle, ଦଶମିକ saturation, ଦଶମିକ exposure);
ଶୂନ୍ୟ forward_crop_layer(ଅଚଳ crop_layer l, network net);
ଶୂନ୍ୟ resize_crop_layer(layer *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h);

#ifdef GPU
ଶୂନ୍ୟ forward_crop_layer_gpu(crop_layer l, network net);
#endif

#endif


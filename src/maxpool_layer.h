#ifndef MAXPOOL_LAYER_H
#define MAXPOOL_LAYER_H

#ଅନ୍ତର୍ଭାଵ "image.h"
#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ "layer.h"
#ଅନ୍ତର୍ଭାଵ "network.h"

ପ୍ରକାରବ୍ୟାଖ୍ୟା layer maxpool_layer;

image get_maxpool_image(maxpool_layer l);
maxpool_layer make_maxpool_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ପୂର୍ଣ୍ଣସଂଖ୍ୟା padding);
ଶୂନ୍ୟ resize_maxpool_layer(maxpool_layer *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h);
ଶୂନ୍ୟ forward_maxpool_layer(ଅଚଳ maxpool_layer l, network net);
ଶୂନ୍ୟ backward_maxpool_layer(ଅଚଳ maxpool_layer l, network net);

#ifdef GPU
ଶୂନ୍ୟ forward_maxpool_layer_gpu(maxpool_layer l, network net);
ଶୂନ୍ୟ backward_maxpool_layer_gpu(maxpool_layer l, network net);
#endif

#endif


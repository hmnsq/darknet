#ifndef BATCHNORM_LAYER_H
#define BATCHNORM_LAYER_H

#ଅନ୍ତର୍ଭାଵ "image.h"
#ଅନ୍ତର୍ଭାଵ "layer.h"
#ଅନ୍ତର୍ଭାଵ "network.h"

layer make_batchnorm_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c);
ଶୂନ୍ୟ forward_batchnorm_layer(layer l, network net);
ଶୂନ୍ୟ backward_batchnorm_layer(layer l, network net);

#ifdef GPU
ଶୂନ୍ୟ forward_batchnorm_layer_gpu(layer l, network net);
ଶୂନ୍ୟ backward_batchnorm_layer_gpu(layer l, network net);
ଶୂନ୍ୟ pull_batchnorm_layer(layer l);
ଶୂନ୍ୟ ଠେଲ_batchnorm_layer(layer l);
#endif

#endif

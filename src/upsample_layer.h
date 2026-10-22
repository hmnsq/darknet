#ifndef UPSAMPLE_LAYER_H
#define UPSAMPLE_LAYER_H
#ଅନ୍ତର୍ଭାଵ "darknet.h"

layer make_upsample_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride);
ଶୂନ୍ୟ forward_upsample_layer(ଅଚଳ layer l, network net);
ଶୂନ୍ୟ backward_upsample_layer(ଅଚଳ layer l, network net);
ଶୂନ୍ୟ resize_upsample_layer(layer *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h);

#ifdef GPU
ଶୂନ୍ୟ forward_upsample_layer_gpu(ଅଚଳ layer l, network net);
ଶୂନ୍ୟ backward_upsample_layer_gpu(ଅଚଳ layer l, network net);
#endif

#endif

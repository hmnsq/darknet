#ifndef REORG_LAYER_H
#define REORG_LAYER_H

#ଅନ୍ତର୍ଭାଵ "image.h"
#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ "layer.h"
#ଅନ୍ତର୍ଭାଵ "network.h"

layer make_reorg_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ପୂର୍ଣ୍ଣସଂଖ୍ୟା reverse, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flatten, ପୂର୍ଣ୍ଣସଂଖ୍ୟା extra);
ଶୂନ୍ୟ resize_reorg_layer(layer *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h);
ଶୂନ୍ୟ forward_reorg_layer(ଅଚଳ layer l, network net);
ଶୂନ୍ୟ backward_reorg_layer(ଅଚଳ layer l, network net);

#ifdef GPU
ଶୂନ୍ୟ forward_reorg_layer_gpu(layer l, network net);
ଶୂନ୍ୟ backward_reorg_layer_gpu(layer l, network net);
#endif

#endif


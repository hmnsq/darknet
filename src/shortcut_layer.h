#ifndef SHORTCUT_LAYER_H
#define SHORTCUT_LAYER_H

#ଅନ୍ତର୍ଭାଵ "layer.h"
#ଅନ୍ତର୍ଭାଵ "network.h"

layer make_shortcut_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା index, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c2);
ଶୂନ୍ୟ forward_shortcut_layer(ଅଚଳ layer l, network net);
ଶୂନ୍ୟ backward_shortcut_layer(ଅଚଳ layer l, network net);
ଶୂନ୍ୟ resize_shortcut_layer(layer *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h);

#ifdef GPU
ଶୂନ୍ୟ forward_shortcut_layer_gpu(ଅଚଳ layer l, network net);
ଶୂନ୍ୟ backward_shortcut_layer_gpu(ଅଚଳ layer l, network net);
#endif

#endif

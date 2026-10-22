
#ifndef CRNN_LAYER_H
#define CRNN_LAYER_H

#ଅନ୍ତର୍ଭାଵ "activations.h"
#ଅନ୍ତର୍ଭାଵ "layer.h"
#ଅନ୍ତର୍ଭାଵ "network.h"

layer make_crnn_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା hidden_filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା output_filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା steps, ACTIVATION activation, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch_normalize);

ଶୂନ୍ୟ forward_crnn_layer(layer l, network net);
ଶୂନ୍ୟ backward_crnn_layer(layer l, network net);
ଶୂନ୍ୟ update_crnn_layer(layer l, update_args a);

#ifdef GPU
ଶୂନ୍ୟ forward_crnn_layer_gpu(layer l, network net);
ଶୂନ୍ୟ backward_crnn_layer_gpu(layer l, network net);
ଶୂନ୍ୟ update_crnn_layer_gpu(layer l, update_args a);
ଶୂନ୍ୟ ଠେଲ_crnn_layer(layer l);
ଶୂନ୍ୟ pull_crnn_layer(layer l);
#endif

#endif


#ifndef ACTIVATION_LAYER_H
#define ACTIVATION_LAYER_H

#અન્તર્ભાવ "activations.h"
#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "network.h"

layer make_activation_layer(પૂર્ણાંક batch, પૂર્ણાંક inputs, ACTIVATION activation);

શૂન્ય forward_activation_layer(layer l, network net);
શૂન્ય backward_activation_layer(layer l, network net);

#ifdef GPU
શૂન્ય forward_activation_layer_gpu(layer l, network net);
શૂન્ય backward_activation_layer_gpu(layer l, network net);
#endif

#endif


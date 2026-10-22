#ifndef CONNECTED_LAYER_H
#define CONNECTED_LAYER_H

#અન્તર્ભાવ "activations.h"
#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "network.h"

layer make_connected_layer(પૂર્ણાંક batch, પૂર્ણાંક inputs, પૂર્ણાંક outputs, ACTIVATION activation, પૂર્ણાંક batch_normalize, પૂર્ણાંક adam);

શૂન્ય forward_connected_layer(layer l, network net);
શૂન્ય backward_connected_layer(layer l, network net);
શૂન્ય update_connected_layer(layer l, update_args a);

#ifdef GPU
શૂન્ય forward_connected_layer_gpu(layer l, network net);
શૂન્ય backward_connected_layer_gpu(layer l, network net);
શૂન્ય update_connected_layer_gpu(layer l, update_args a);
શૂન્ય ધકેલો_connected_layer(layer l);
શૂન્ય pull_connected_layer(layer l);
#endif

#endif


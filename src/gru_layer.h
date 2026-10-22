
#ifndef GRU_LAYER_H
#define GRU_LAYER_H

#અન્તર્ભાવ "activations.h"
#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "network.h"

layer make_gru_layer(પૂર્ણાંક batch, પૂર્ણાંક inputs, પૂર્ણાંક outputs, પૂર્ણાંક steps, પૂર્ણાંક batch_normalize, પૂર્ણાંક adam);

શૂન્ય forward_gru_layer(layer l, network state);
શૂન્ય backward_gru_layer(layer l, network state);
શૂન્ય update_gru_layer(layer l, update_args a);

#ifdef GPU
શૂન્ય forward_gru_layer_gpu(layer l, network state);
શૂન્ય backward_gru_layer_gpu(layer l, network state);
શૂન્ય update_gru_layer_gpu(layer l, update_args a);
શૂન્ય ધકેલો_gru_layer(layer l);
શૂન્ય pull_gru_layer(layer l);
#endif

#endif


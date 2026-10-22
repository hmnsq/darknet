
#ifndef RNN_LAYER_H
#define RNN_LAYER_H

#અન્તર્ભાવ "activations.h"
#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "network.h"
#define USET

layer make_rnn_layer(પૂર્ણાંક batch, પૂર્ણાંક inputs, પૂર્ણાંક outputs, પૂર્ણાંક steps, ACTIVATION activation, પૂર્ણાંક batch_normalize, પૂર્ણાંક adam);

શૂન્ય forward_rnn_layer(layer l, network net);
શૂન્ય backward_rnn_layer(layer l, network net);
શૂન્ય update_rnn_layer(layer l, update_args a);

#ifdef GPU
શૂન્ય forward_rnn_layer_gpu(layer l, network net);
શૂન્ય backward_rnn_layer_gpu(layer l, network net);
શૂન્ય update_rnn_layer_gpu(layer l, update_args a);
શૂન્ય ધકેલો_rnn_layer(layer l);
શૂન્ય pull_rnn_layer(layer l);
#endif

#endif


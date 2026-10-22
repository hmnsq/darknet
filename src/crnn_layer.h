
#ifndef CRNN_LAYER_H
#define CRNN_LAYER_H

#અન્તર્ભાવ "activations.h"
#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "network.h"

layer make_crnn_layer(પૂર્ણાંક batch, પૂર્ણાંક h, પૂર્ણાંક w, પૂર્ણાંક c, પૂર્ણાંક hidden_filters, પૂર્ણાંક output_filters, પૂર્ણાંક steps, ACTIVATION activation, પૂર્ણાંક batch_normalize);

શૂન્ય forward_crnn_layer(layer l, network net);
શૂન્ય backward_crnn_layer(layer l, network net);
શૂન્ય update_crnn_layer(layer l, update_args a);

#ifdef GPU
શૂન્ય forward_crnn_layer_gpu(layer l, network net);
શૂન્ય backward_crnn_layer_gpu(layer l, network net);
શૂન્ય update_crnn_layer_gpu(layer l, update_args a);
શૂન્ય ધકેલો_crnn_layer(layer l);
શૂન્ય pull_crnn_layer(layer l);
#endif

#endif


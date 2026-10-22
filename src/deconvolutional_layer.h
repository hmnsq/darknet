#ifndef DECONVOLUTIONAL_LAYER_H
#define DECONVOLUTIONAL_LAYER_H

#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ "image.h"
#અન્તર્ભાવ "activations.h"
#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "network.h"

#ifdef GPU
શૂન્ય forward_deconvolutional_layer_gpu(layer l, network net);
શૂન્ય backward_deconvolutional_layer_gpu(layer l, network net);
શૂન્ય update_deconvolutional_layer_gpu(layer l, update_args a);
શૂન્ય ધકેલો_deconvolutional_layer(layer l);
શૂન્ય pull_deconvolutional_layer(layer l);
#endif

layer make_deconvolutional_layer(પૂર્ણાંક batch, પૂર્ણાંક h, પૂર્ણાંક w, પૂર્ણાંક c, પૂર્ણાંક n, પૂર્ણાંક size, પૂર્ણાંક stride, પૂર્ણાંક padding, ACTIVATION activation, પૂર્ણાંક batch_normalize, પૂર્ણાંક adam);
શૂન્ય resize_deconvolutional_layer(layer *l, પૂર્ણાંક h, પૂર્ણાંક w);
શૂન્ય forward_deconvolutional_layer(અચલ layer l, network net);
શૂન્ય update_deconvolutional_layer(layer l, update_args a);
શૂન્ય backward_deconvolutional_layer(layer l, network net);

#endif


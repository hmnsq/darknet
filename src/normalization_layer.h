#ifndef NORMALIZATION_LAYER_H
#define NORMALIZATION_LAYER_H

#અન્તર્ભાવ "image.h"
#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "network.h"

layer make_normalization_layer(પૂર્ણાંક batch, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક c, પૂર્ણાંક size, દશાંશ alpha, દશાંશ beta, દશાંશ kappa);
શૂન્ય resize_normalization_layer(layer *layer, પૂર્ણાંક h, પૂર્ણાંક w);
શૂન્ય forward_normalization_layer(અચલ layer layer, network net);
શૂન્ય backward_normalization_layer(અચલ layer layer, network net);
શૂન્ય visualize_normalization_layer(layer layer, અક્ષર *window);

#ifdef GPU
શૂન્ય forward_normalization_layer_gpu(અચલ layer layer, network net);
શૂન્ય backward_normalization_layer_gpu(અચલ layer layer, network net);
#endif

#endif

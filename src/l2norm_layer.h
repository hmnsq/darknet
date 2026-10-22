#ifndef L2NORM_LAYER_H
#define L2NORM_LAYER_H
#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "network.h"

layer make_l2norm_layer(પૂર્ણાંક batch, પૂર્ણાંક inputs);
શૂન્ય forward_l2norm_layer(અચલ layer l, network net);
શૂન્ય backward_l2norm_layer(અચલ layer l, network net);

#ifdef GPU
શૂન્ય forward_l2norm_layer_gpu(અચલ layer l, network net);
શૂન્ય backward_l2norm_layer_gpu(અચલ layer l, network net);
#endif

#endif

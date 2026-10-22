#ifndef LOGISTIC_LAYER_H
#define LOGISTIC_LAYER_H
#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "network.h"

layer make_logistic_layer(પૂર્ણાંક batch, પૂર્ણાંક inputs);
શૂન્ય forward_logistic_layer(અચલ layer l, network net);
શૂન્ય backward_logistic_layer(અચલ layer l, network net);

#ifdef GPU
શૂન્ય forward_logistic_layer_gpu(અચલ layer l, network net);
શૂન્ય backward_logistic_layer_gpu(અચલ layer l, network net);
#endif

#endif

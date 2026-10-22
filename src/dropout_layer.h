#ifndef DROPOUT_LAYER_H
#define DROPOUT_LAYER_H

#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "network.h"

પ્રકારવ્યાખ્યા layer dropout_layer;

dropout_layer make_dropout_layer(પૂર્ણાંક batch, પૂર્ણાંક inputs, દશાંશ probability);

શૂન્ય forward_dropout_layer(dropout_layer l, network net);
શૂન્ય backward_dropout_layer(dropout_layer l, network net);
શૂન્ય resize_dropout_layer(dropout_layer *l, પૂર્ણાંક inputs);

#ifdef GPU
શૂન્ય forward_dropout_layer_gpu(dropout_layer l, network net);
શૂન્ય backward_dropout_layer_gpu(dropout_layer l, network net);

#endif
#endif

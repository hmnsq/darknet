#ifndef SOFTMAX_LAYER_H
#define SOFTMAX_LAYER_H
#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "network.h"

પ્રકારવ્યાખ્યા layer softmax_layer;

શૂન્ય softmax_array(દશાંશ *આગત, પૂર્ણાંક n, દશાંશ temp, દશાંશ *output);
softmax_layer make_softmax_layer(પૂર્ણાંક batch, પૂર્ણાંક inputs, પૂર્ણાંક groups);
શૂન્ય forward_softmax_layer(અચલ softmax_layer l, network net);
શૂન્ય backward_softmax_layer(અચલ softmax_layer l, network net);

#ifdef GPU
શૂન્ય pull_softmax_layer_output(અચલ softmax_layer l);
શૂન્ય forward_softmax_layer_gpu(અચલ softmax_layer l, network net);
શૂન્ય backward_softmax_layer_gpu(અચલ softmax_layer l, network net);
#endif

#endif

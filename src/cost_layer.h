#ifndef COST_LAYER_H
#define COST_LAYER_H
#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "network.h"

પ્રકારવ્યાખ્યા layer cost_layer;

COST_TYPE get_cost_type(અક્ષર *s);
અક્ષર *get_cost_string(COST_TYPE a);
cost_layer make_cost_layer(પૂર્ણાંક batch, પૂર્ણાંક inputs, COST_TYPE type, દશાંશ scale);
શૂન્ય forward_cost_layer(અચલ cost_layer l, network net);
શૂન્ય backward_cost_layer(અચલ cost_layer l, network net);
શૂન્ય resize_cost_layer(cost_layer *l, પૂર્ણાંક inputs);

#ifdef GPU
શૂન્ય forward_cost_layer_gpu(cost_layer l, network net);
શૂન્ય backward_cost_layer_gpu(અચલ cost_layer l, network net);
#endif

#endif

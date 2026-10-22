#ifndef ROUTE_LAYER_H
#define ROUTE_LAYER_H
#અન્તર્ભાવ "network.h"
#અન્તર્ભાવ "layer.h"

પ્રકારવ્યાખ્યા layer route_layer;

route_layer make_route_layer(પૂર્ણાંક batch, પૂર્ણાંક n, પૂર્ણાંક *આગત_layers, પૂર્ણાંક *આગત_size);
શૂન્ય forward_route_layer(અચલ route_layer l, network net);
શૂન્ય backward_route_layer(અચલ route_layer l, network net);
શૂન્ય resize_route_layer(route_layer *l, network *net);

#ifdef GPU
શૂન્ય forward_route_layer_gpu(અચલ route_layer l, network net);
શૂન્ય backward_route_layer_gpu(અચલ route_layer l, network net);
#endif

#endif

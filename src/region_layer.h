#ifndef REGION_LAYER_H
#define REGION_LAYER_H

#અન્તર્ભાવ "darknet.h"
#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "network.h"

layer make_region_layer(પૂર્ણાંક batch, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક n, પૂર્ણાંક classes, પૂર્ણાંક coords);
શૂન્ય forward_region_layer(અચલ layer l, network net);
શૂન્ય backward_region_layer(અચલ layer l, network net);
શૂન્ય resize_region_layer(layer *l, પૂર્ણાંક w, પૂર્ણાંક h);

#ifdef GPU
શૂન્ય forward_region_layer_gpu(અચલ layer l, network net);
શૂન્ય backward_region_layer_gpu(layer l, network net);
#endif

#endif

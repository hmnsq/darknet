#ifndef REORG_LAYER_H
#define REORG_LAYER_H

#અન્તર્ભાવ "image.h"
#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "network.h"

layer make_reorg_layer(પૂર્ણાંક batch, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક c, પૂર્ણાંક stride, પૂર્ણાંક reverse, પૂર્ણાંક flatten, પૂર્ણાંક extra);
શૂન્ય resize_reorg_layer(layer *l, પૂર્ણાંક w, પૂર્ણાંક h);
શૂન્ય forward_reorg_layer(અચલ layer l, network net);
શૂન્ય backward_reorg_layer(અચલ layer l, network net);

#ifdef GPU
શૂન્ય forward_reorg_layer_gpu(layer l, network net);
શૂન્ય backward_reorg_layer_gpu(layer l, network net);
#endif

#endif


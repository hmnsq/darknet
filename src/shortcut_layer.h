#ifndef SHORTCUT_LAYER_H
#define SHORTCUT_LAYER_H

#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "network.h"

layer make_shortcut_layer(પૂર્ણાંક batch, પૂર્ણાંક index, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક c, પૂર્ણાંક w2, પૂર્ણાંક h2, પૂર્ણાંક c2);
શૂન્ય forward_shortcut_layer(અચલ layer l, network net);
શૂન્ય backward_shortcut_layer(અચલ layer l, network net);
શૂન્ય resize_shortcut_layer(layer *l, પૂર્ણાંક w, પૂર્ણાંક h);

#ifdef GPU
શૂન્ય forward_shortcut_layer_gpu(અચલ layer l, network net);
શૂન્ય backward_shortcut_layer_gpu(અચલ layer l, network net);
#endif

#endif

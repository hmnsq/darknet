#ifndef UPSAMPLE_LAYER_H
#define UPSAMPLE_LAYER_H
#અન્તર્ભાવ "darknet.h"

layer make_upsample_layer(પૂર્ણાંક batch, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક c, પૂર્ણાંક stride);
શૂન્ય forward_upsample_layer(અચલ layer l, network net);
શૂન્ય backward_upsample_layer(અચલ layer l, network net);
શૂન્ય resize_upsample_layer(layer *l, પૂર્ણાંક w, પૂર્ણાંક h);

#ifdef GPU
શૂન્ય forward_upsample_layer_gpu(અચલ layer l, network net);
શૂન્ય backward_upsample_layer_gpu(અચલ layer l, network net);
#endif

#endif

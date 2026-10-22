#ifndef ISEG_LAYER_H
#define ISEG_LAYER_H

#અન્તર્ભાવ "darknet.h"
#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "network.h"

layer make_iseg_layer(પૂર્ણાંક batch, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક classes, પૂર્ણાંક ids);
શૂન્ય forward_iseg_layer(અચલ layer l, network net);
શૂન્ય backward_iseg_layer(અચલ layer l, network net);
શૂન્ય resize_iseg_layer(layer *l, પૂર્ણાંક w, પૂર્ણાંક h);
પૂર્ણાંક iseg_અંક_detections(layer l, દશાંશ thresh);

#ifdef GPU
શૂન્ય forward_iseg_layer_gpu(અચલ layer l, network net);
શૂન્ય backward_iseg_layer_gpu(layer l, network net);
#endif

#endif

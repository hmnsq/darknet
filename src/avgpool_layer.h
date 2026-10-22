#ifndef AVGPOOL_LAYER_H
#define AVGPOOL_LAYER_H

#અન્તર્ભાવ "image.h"
#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "network.h"

પ્રકારવ્યાખ્યા layer avgpool_layer;

image get_avgpool_image(avgpool_layer l);
avgpool_layer make_avgpool_layer(પૂર્ણાંક batch, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક c);
શૂન્ય resize_avgpool_layer(avgpool_layer *l, પૂર્ણાંક w, પૂર્ણાંક h);
શૂન્ય forward_avgpool_layer(અચલ avgpool_layer l, network net);
શૂન્ય backward_avgpool_layer(અચલ avgpool_layer l, network net);

#ifdef GPU
શૂન્ય forward_avgpool_layer_gpu(avgpool_layer l, network net);
શૂન્ય backward_avgpool_layer_gpu(avgpool_layer l, network net);
#endif

#endif


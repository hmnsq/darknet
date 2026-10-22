#ifndef MAXPOOL_LAYER_H
#define MAXPOOL_LAYER_H

#અન્તર્ભાવ "image.h"
#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "network.h"

પ્રકારવ્યાખ્યા layer maxpool_layer;

image get_maxpool_image(maxpool_layer l);
maxpool_layer make_maxpool_layer(પૂર્ણાંક batch, પૂર્ણાંક h, પૂર્ણાંક w, પૂર્ણાંક c, પૂર્ણાંક size, પૂર્ણાંક stride, પૂર્ણાંક padding);
શૂન્ય resize_maxpool_layer(maxpool_layer *l, પૂર્ણાંક w, પૂર્ણાંક h);
શૂન્ય forward_maxpool_layer(અચલ maxpool_layer l, network net);
શૂન્ય backward_maxpool_layer(અચલ maxpool_layer l, network net);

#ifdef GPU
શૂન્ય forward_maxpool_layer_gpu(maxpool_layer l, network net);
શૂન્ય backward_maxpool_layer_gpu(maxpool_layer l, network net);
#endif

#endif


#ifndef BATCHNORM_LAYER_H
#define BATCHNORM_LAYER_H

#અન્તર્ભાવ "image.h"
#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "network.h"

layer make_batchnorm_layer(પૂર્ણાંક batch, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક c);
શૂન્ય forward_batchnorm_layer(layer l, network net);
શૂન્ય backward_batchnorm_layer(layer l, network net);

#ifdef GPU
શૂન્ય forward_batchnorm_layer_gpu(layer l, network net);
શૂન્ય backward_batchnorm_layer_gpu(layer l, network net);
શૂન્ય pull_batchnorm_layer(layer l);
શૂન્ય ધકેલો_batchnorm_layer(layer l);
#endif

#endif

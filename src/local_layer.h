#ifndef LOCAL_LAYER_H
#define LOCAL_LAYER_H

#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ "image.h"
#અન્તર્ભાવ "activations.h"
#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "network.h"

પ્રકારવ્યાખ્યા layer local_layer;

#ifdef GPU
શૂન્ય forward_local_layer_gpu(local_layer layer, network net);
શૂન્ય backward_local_layer_gpu(local_layer layer, network net);
શૂન્ય update_local_layer_gpu(local_layer layer, update_args a);

શૂન્ય ધકેલો_local_layer(local_layer layer);
શૂન્ય pull_local_layer(local_layer layer);
#endif

local_layer make_local_layer(પૂર્ણાંક batch, પૂર્ણાંક h, પૂર્ણાંક w, પૂર્ણાંક c, પૂર્ણાંક n, પૂર્ણાંક size, પૂર્ણાંક stride, પૂર્ણાંક pad, ACTIVATION activation);

શૂન્ય forward_local_layer(અચલ local_layer layer, network net);
શૂન્ય backward_local_layer(local_layer layer, network net);
શૂન્ય update_local_layer(local_layer layer, update_args a);

શૂન્ય bias_output(દશાંશ *output, દશાંશ *biases, પૂર્ણાંક batch, પૂર્ણાંક n, પૂર્ણાંક size);
શૂન્ય backward_bias(દશાંશ *bias_updates, દશાંશ *delta, પૂર્ણાંક batch, પૂર્ણાંક n, પૂર્ણાંક size);

#endif


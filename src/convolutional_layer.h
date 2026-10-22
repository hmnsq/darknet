#ifndef CONVOLUTIONAL_LAYER_H
#define CONVOLUTIONAL_LAYER_H

#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ "image.h"
#અન્તર્ભાવ "activations.h"
#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "network.h"

પ્રકારવ્યાખ્યા layer convolutional_layer;

#ifdef GPU
શૂન્ય forward_convolutional_layer_gpu(convolutional_layer layer, network net);
શૂન્ય backward_convolutional_layer_gpu(convolutional_layer layer, network net);
શૂન્ય update_convolutional_layer_gpu(convolutional_layer layer, update_args a);

શૂન્ય ધકેલો_convolutional_layer(convolutional_layer layer);
શૂન્ય pull_convolutional_layer(convolutional_layer layer);

શૂન્ય add_bias_gpu(દશાંશ *output, દશાંશ *biases, પૂર્ણાંક batch, પૂર્ણાંક n, પૂર્ણાંક size);
શૂન્ય backward_bias_gpu(દશાંશ *bias_updates, દશાંશ *delta, પૂર્ણાંક batch, પૂર્ણાંક n, પૂર્ણાંક size);
શૂન્ય adam_update_gpu(દશાંશ *w, દશાંશ *d, દશાંશ *m, દશાંશ *v, દશાંશ B1, દશાંશ B2, દશાંશ eps, દશાંશ decay, દશાંશ rate, પૂર્ણાંક n, પૂર્ણાંક batch, પૂર્ણાંક t);
#ifdef CUDNN
શૂન્ય cudnn_convolutional_setup(layer *l);
#endif
#endif

convolutional_layer make_convolutional_layer(પૂર્ણાંક batch, પૂર્ણાંક h, પૂર્ણાંક w, પૂર્ણાંક c, પૂર્ણાંક n, પૂર્ણાંક groups, પૂર્ણાંક size, પૂર્ણાંક stride, પૂર્ણાંક padding, ACTIVATION activation, પૂર્ણાંક batch_normalize, પૂર્ણાંક દ્વિઅંકી, પૂર્ણાંક xnor, પૂર્ણાંક adam);
શૂન્ય resize_convolutional_layer(convolutional_layer *layer, પૂર્ણાંક w, પૂર્ણાંક h);
શૂન્ય forward_convolutional_layer(અચલ convolutional_layer layer, network net);
શૂન્ય update_convolutional_layer(convolutional_layer layer, update_args a);
image *visualize_convolutional_layer(convolutional_layer layer, અક્ષર *window, image *prev_weights);
શૂન્ય binarize_weights(દશાંશ *weights, પૂર્ણાંક n, પૂર્ણાંક size, દશાંશ *દ્વિઅંકી);
શૂન્ય swap_દ્વિઅંકી(convolutional_layer *l);
શૂન્ય binarize_weights2(દશાંશ *weights, પૂર્ણાંક n, પૂર્ણાંક size, અક્ષર *દ્વિઅંકી, દશાંશ *scales);

શૂન્ય backward_convolutional_layer(convolutional_layer layer, network net);

શૂન્ય add_bias(દશાંશ *output, દશાંશ *biases, પૂર્ણાંક batch, પૂર્ણાંક n, પૂર્ણાંક size);
શૂન્ય backward_bias(દશાંશ *bias_updates, દશાંશ *delta, પૂર્ણાંક batch, પૂર્ણાંક n, પૂર્ણાંક size);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_weight(convolutional_layer layer, પૂર્ણાંક i);

પૂર્ણાંક convolutional_out_height(convolutional_layer layer);
પૂર્ણાંક convolutional_out_width(convolutional_layer layer);

#endif


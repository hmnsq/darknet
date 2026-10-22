#ifndef LOCAL_LAYER_H
#define LOCAL_LAYER_H

#अन्तर्भाव "cuda.h"
#अन्तर्भाव "image.h"
#अन्तर्भाव "activations.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

प्रकारव्याख्या layer local_layer;

#ifdef GPU
शून्य forward_local_layer_gpu(local_layer layer, network net);
शून्य backward_local_layer_gpu(local_layer layer, network net);
शून्य update_local_layer_gpu(local_layer layer, update_args a);

शून्य ढकला_local_layer(local_layer layer);
शून्य pull_local_layer(local_layer layer);
#endif

local_layer make_local_layer(पूर्णांक batch, पूर्णांक h, पूर्णांक w, पूर्णांक c, पूर्णांक n, पूर्णांक size, पूर्णांक stride, पूर्णांक pad, ACTIVATION activation);

शून्य forward_local_layer(स्थिरांक local_layer layer, network net);
शून्य backward_local_layer(local_layer layer, network net);
शून्य update_local_layer(local_layer layer, update_args a);

शून्य bias_output(दशांश *output, दशांश *biases, पूर्णांक batch, पूर्णांक n, पूर्णांक size);
शून्य backward_bias(दशांश *bias_updates, दशांश *delta, पूर्णांक batch, पूर्णांक n, पूर्णांक size);

#endif



#यदि_अपरिभाषितम् GRU_LAYER_H
#परिभाषय GRU_LAYER_H

#अन्तर्भाव "activations.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_gru_layer(पूर्णाङ्क batch, पूर्णाङ्क inputs, पूर्णाङ्क outputs, पूर्णाङ्क चरणाः, पूर्णाङ्क batch_normalize, पूर्णाङ्क adam);

शून्य forward_gru_layer(layer l, जालम् state);
शून्य backward_gru_layer(layer l, जालम् state);
शून्य update_gru_layer(layer l, update_तर्काः a);

#यदि_परिभाषितम् GPU
शून्य forward_gru_layer_gpu(layer l, जालम् state);
शून्य backward_gru_layer_gpu(layer l, जालम् state);
शून्य update_gru_layer_gpu(layer l, update_तर्काः a);
शून्य प्रक्षिप_gru_layer(layer l);
शून्य pull_gru_layer(layer l);
#यदि_अन्तः

#यदि_अन्तः


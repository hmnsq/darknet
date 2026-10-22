#यदि_अपरिभाषितम् LSTM_LAYER_H
#परिभाषय LSTM_LAYER_H

#अन्तर्भाव "activations.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"
#परिभाषय USET

layer make_lstm_layer(पूर्णाङ्क batch, पूर्णाङ्क inputs, पूर्णाङ्क outputs, पूर्णाङ्क चरणाः, पूर्णाङ्क batch_normalize, पूर्णाङ्क adam);

शून्य forward_lstm_layer(layer l, जालम् net); 
शून्य update_lstm_layer(layer l, update_तर्काः a);

#यदि_परिभाषितम् GPU
शून्य forward_lstm_layer_gpu(layer l, जालम् net);
शून्य backward_lstm_layer_gpu(layer l, जालम् net);
शून्य update_lstm_layer_gpu(layer l, update_तर्काः a); 

#यदि_अन्तः
#यदि_अन्तः

#جيڪڏهن_وصف_نه_ٿيل LOGISTIC_LAYER_H
#وصف_ڪريو LOGISTIC_LAYER_H
#ضمن "layer.h"
#ضمن "network.h"

layer make_logistic_layer(صحيح batch, صحيح inputs);
فراغ forward_logistic_layer(ثابت layer l, ڄار net);
فراغ backward_logistic_layer(ثابت layer l, ڄار net);

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_logistic_layer_gpu(ثابت layer l, ڄار net);
فراغ backward_logistic_layer_gpu(ثابت layer l, ڄار net);
#جيڪڏهن_پورو

#جيڪڏهن_پورو

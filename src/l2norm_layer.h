#جيڪڏهن_وصف_نه_ٿيل L2NORM_LAYER_H
#وصف_ڪريو L2NORM_LAYER_H
#ضمن "layer.h"
#ضمن "network.h"

layer make_l2norm_layer(صحيح batch, صحيح inputs);
فراغ forward_l2norm_layer(ثابت layer l, ڄار net);
فراغ backward_l2norm_layer(ثابت layer l, ڄار net);

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_l2norm_layer_gpu(ثابت layer l, ڄار net);
فراغ backward_l2norm_layer_gpu(ثابت layer l, ڄار net);
#جيڪڏهن_پورو

#جيڪڏهن_پورو

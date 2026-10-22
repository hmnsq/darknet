#جيڪڏهن_وصف_نه_ٿيل AVGPOOL_LAYER_H
#وصف_ڪريو AVGPOOL_LAYER_H

#ضمن "image.h"
#ضمن "cuda.h"
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer avgpool_layer;

image get_avgpool_image(avgpool_layer l);
avgpool_layer make_avgpool_layer(صحيح batch, صحيح w, صحيح h, صحيح c);
فراغ resize_avgpool_layer(avgpool_layer *l, صحيح w, صحيح h);
فراغ forward_avgpool_layer(ثابت avgpool_layer l, ڄار net);
فراغ backward_avgpool_layer(ثابت avgpool_layer l, ڄار net);

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_avgpool_layer_gpu(avgpool_layer l, ڄار net);
فراغ backward_avgpool_layer_gpu(avgpool_layer l, ڄار net);
#جيڪڏهن_پورو

#جيڪڏهن_پورو


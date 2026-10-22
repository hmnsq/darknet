#که_نه_تعریف_شوی AVGPOOL_LAYER_H
#تعریف_کړه AVGPOOL_LAYER_H

#ضمن "image.h"
#ضمن "cuda.h"
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer avgpool_layer;

image get_avgpool_image(avgpool_layer l);
avgpool_layer make_avgpool_layer(صحيح batch, صحيح w, صحيح h, صحيح c);
فراغ resize_avgpool_layer(avgpool_layer *l, صحيح w, صحيح h);
فراغ forward_avgpool_layer(ثابت avgpool_layer l, شبکه net);
فراغ backward_avgpool_layer(ثابت avgpool_layer l, شبکه net);

#که_تعریف_شوی GPU
فراغ forward_avgpool_layer_gpu(avgpool_layer l, شبکه net);
فراغ backward_avgpool_layer_gpu(avgpool_layer l, شبکه net);
#د_که_پای

#د_که_پای


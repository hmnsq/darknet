#که_نه_تعریف_شوی SHORTCUT_LAYER_H
#تعریف_کړه SHORTCUT_LAYER_H

#ضمن "layer.h"
#ضمن "network.h"

layer make_shortcut_layer(صحيح batch, صحيح شاخص, صحيح w, صحيح h, صحيح c, صحيح w2, صحيح h2, صحيح c2);
فراغ forward_shortcut_layer(ثابت layer l, شبکه net);
فراغ backward_shortcut_layer(ثابت layer l, شبکه net);
فراغ resize_shortcut_layer(layer *l, صحيح w, صحيح h);

#که_تعریف_شوی GPU
فراغ forward_shortcut_layer_gpu(ثابت layer l, شبکه net);
فراغ backward_shortcut_layer_gpu(ثابت layer l, شبکه net);
#د_که_پای

#د_که_پای

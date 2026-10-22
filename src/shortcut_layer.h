#اگر_تعریف_نشده SHORTCUT_LAYER_H
#تعریف_کن SHORTCUT_LAYER_H

#ضمن "layer.h"
#ضمن "network.h"

layer make_shortcut_layer(صحيح batch, صحيح شاخص, صحيح w, صحيح h, صحيح c, صحيح w2, صحيح h2, صحيح c2);
فراغ forward_shortcut_layer(ثابت layer l, شبکه net);
فراغ backward_shortcut_layer(ثابت layer l, شبکه net);
فراغ resize_shortcut_layer(layer *l, صحيح w, صحيح h);

#اگر_تعریف_شده GPU
فراغ forward_shortcut_layer_gpu(ثابت layer l, شبکه net);
فراغ backward_shortcut_layer_gpu(ثابت layer l, شبکه net);
#پایان_اگر

#پایان_اگر

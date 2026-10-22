#جے_بنا_تعریف SHORTCUT_LAYER_H
#تعریف_کرو SHORTCUT_LAYER_H

#ضمن "layer.h"
#ضمن "network.h"

layer make_shortcut_layer(صحيح batch, صحيح اشاریہ, صحيح w, صحيح h, صحيح c, صحيح w2, صحيح h2, صحيح c2);
فراغ forward_shortcut_layer(ثابت layer l, جال net);
فراغ backward_shortcut_layer(ثابت layer l, جال net);
فراغ resize_shortcut_layer(layer *l, صحيح w, صحيح h);

#جے_تعریف_شدہ GPU
فراغ forward_shortcut_layer_gpu(ثابت layer l, جال net);
فراغ backward_shortcut_layer_gpu(ثابت layer l, جال net);
#جے_انت

#جے_انت

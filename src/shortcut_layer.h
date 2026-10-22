#جيڪڏهن_وصف_نه_ٿيل SHORTCUT_LAYER_H
#وصف_ڪريو SHORTCUT_LAYER_H

#ضمن "layer.h"
#ضمن "network.h"

layer make_shortcut_layer(صحيح batch, صحيح اشاريو, صحيح w, صحيح h, صحيح c, صحيح w2, صحيح h2, صحيح c2);
فراغ forward_shortcut_layer(ثابت layer l, ڄار net);
فراغ backward_shortcut_layer(ثابت layer l, ڄار net);
فراغ resize_shortcut_layer(layer *l, صحيح w, صحيح h);

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_shortcut_layer_gpu(ثابت layer l, ڄار net);
فراغ backward_shortcut_layer_gpu(ثابت layer l, ڄار net);
#جيڪڏهن_پورو

#جيڪڏهن_پورو

#ifndef SHORTCUT_LAYER_H
#define SHORTCUT_LAYER_H

#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_shortcut_layer(पूर्णांक batch, पूर्णांक index, पूर्णांक w, पूर्णांक h, पूर्णांक c, पूर्णांक w2, पूर्णांक h2, पूर्णांक c2);
शून्य forward_shortcut_layer(स्थिरांक layer l, network net);
शून्य backward_shortcut_layer(स्थिरांक layer l, network net);
शून्य resize_shortcut_layer(layer *l, पूर्णांक w, पूर्णांक h);

#ifdef GPU
शून्य forward_shortcut_layer_gpu(स्थिरांक layer l, network net);
शून्य backward_shortcut_layer_gpu(स्थिरांक layer l, network net);
#endif

#endif

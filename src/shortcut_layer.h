#अगर_अघोषित SHORTCUT_LAYER_H
#घोषणा SHORTCUT_LAYER_H

#समावेश "layer.h"
#समावेश "network.h"

layer make_shortcut_layer(पूर्णांक batch, पूर्णांक सूचकांक, पूर्णांक w, पूर्णांक h, पूर्णांक c, पूर्णांक w2, पूर्णांक h2, पूर्णांक c2);
व्योम forward_shortcut_layer(स्थिर layer l, network net);
व्योम backward_shortcut_layer(स्थिर layer l, network net);
व्योम resize_shortcut_layer(layer *l, पूर्णांक w, पूर्णांक h);

#अगर_घोषित GPU
व्योम forward_shortcut_layer_gpu(स्थिर layer l, network net);
व्योम backward_shortcut_layer_gpu(स्थिर layer l, network net);
#पूर्ण_अगर

#पूर्ण_अगर

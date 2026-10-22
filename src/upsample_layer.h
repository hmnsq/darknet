#अगर_अघोषित UPSAMPLE_LAYER_H
#घोषणा UPSAMPLE_LAYER_H
#समावेश "darknet.h"

layer make_upsample_layer(पूर्णांक batch, पूर्णांक w, पूर्णांक h, पूर्णांक c, पूर्णांक stride);
व्योम forward_upsample_layer(स्थिर layer l, network net);
व्योम backward_upsample_layer(स्थिर layer l, network net);
व्योम resize_upsample_layer(layer *l, पूर्णांक w, पूर्णांक h);

#अगर_घोषित GPU
व्योम forward_upsample_layer_gpu(स्थिर layer l, network net);
व्योम backward_upsample_layer_gpu(स्थिर layer l, network net);
#पूर्ण_अगर

#पूर्ण_अगर

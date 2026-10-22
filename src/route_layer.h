#यदि_अपरिभाषितम् ROUTE_LAYER_H
#परिभाषय ROUTE_LAYER_H
#अन्तर्भाव "network.h"
#अन्तर्भाव "layer.h"

प्रकारनिर्देश layer route_layer;

route_layer make_route_layer(पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क *निवेशः_layers, पूर्णाङ्क *निवेशः_आकार);
शून्य forward_route_layer(ध्रुव route_layer l, जालम् net);
शून्य backward_route_layer(ध्रुव route_layer l, जालम् net);
शून्य resize_route_layer(route_layer *l, जालम् *net);

#यदि_परिभाषितम् GPU
शून्य forward_route_layer_gpu(ध्रुव route_layer l, जालम् net);
शून्य backward_route_layer_gpu(ध्रुव route_layer l, जालम् net);
#यदि_अन्तः

#यदि_अन्तः

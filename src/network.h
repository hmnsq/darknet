// Oh boy, why am I about to do this....
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਜਾਲ_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜਾਲ_H
#ਅਨ੍ਤਰ੍ਭਾਵ "darknet.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "image.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "data.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree.h"


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ pull_ਜਾਲ_output(ਜਾਲ *net);
#ਜੇ_ਅੰਤ

ਖਾਲੀ compare_networks(ਜਾਲ *n1, ਜਾਲ *n2, ਅੰਕੜੇ d);
ਅੱਖਰ *get_layer_string(LAYER_TYPE a);

ਜਾਲ *make_ਜਾਲ(ਪੂਰਨ n);


ਦਸ਼ਮਲਵ ਜਾਲ_accuracy_multi(ਜਾਲ *net, ਅੰਕੜੇ d, ਪੂਰਨ n);
ਪੂਰਨ get_predicted_class_ਜਾਲ(ਜਾਲ *net);
ਖਾਲੀ print_ਜਾਲ(ਜਾਲ *net);
ਪੂਰਨ resize_ਜਾਲ(ਜਾਲ *net, ਪੂਰਨ w, ਪੂਰਨ h);
ਖਾਲੀ calc_ਜਾਲ_ਲਾਗਤ(ਜਾਲ *net);

#ਜੇ_ਅੰਤ


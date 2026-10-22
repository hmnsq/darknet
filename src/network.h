// Oh boy, why am I about to do this....
#جيڪڏهن_وصف_نه_ٿيل ڄار_H
#وصف_ڪريو ڄار_H
#ضمن "darknet.h"

#ضمن "image.h"
#ضمن "layer.h"
#ضمن "data.h"
#ضمن "tree.h"


#جيڪڏهن_وصف_ٿيل GPU
فراغ pull_ڄار_output(ڄار *net);
#جيڪڏهن_پورو

فراغ compare_networks(ڄار *n1, ڄار *n2, معلومات d);
حرف *get_layer_string(LAYER_TYPE a);

ڄار *make_ڄار(صحيح n);


عائم ڄار_accuracy_multi(ڄار *net, معلومات d, صحيح n);
صحيح get_predicted_class_ڄار(ڄار *net);
فراغ print_ڄار(ڄار *net);
صحيح resize_ڄار(ڄار *net, صحيح w, صحيح h);
فراغ calc_ڄار_لاڳت(ڄار *net);

#جيڪڏهن_پورو


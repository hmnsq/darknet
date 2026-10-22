// Oh boy, why am I about to do this....
#که_نه_تعریف_شوی شبکه_H
#تعریف_کړه شبکه_H
#ضمن "darknet.h"

#ضمن "image.h"
#ضمن "layer.h"
#ضمن "data.h"
#ضمن "tree.h"


#که_تعریف_شوی GPU
فراغ pull_شبکه_output(شبکه *net);
#د_که_پای

فراغ compare_networks(شبکه *n1, شبکه *n2, معلومات d);
حرف *get_layer_string(LAYER_TYPE a);

شبکه *make_شبکه(صحيح n);


عائم شبکه_accuracy_multi(شبکه *net, معلومات d, صحيح n);
صحيح get_predicted_class_شبکه(شبکه *net);
فراغ print_شبکه(شبکه *net);
صحيح resize_شبکه(شبکه *net, صحيح w, صحيح h);
فراغ calc_شبکه_لګښت(شبکه *net);

#د_که_پای


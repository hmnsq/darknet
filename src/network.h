// Oh boy, why am I about to do this....
#اگر_تعریف_نشده شبکه_H
#تعریف_کن شبکه_H
#ضمن "darknet.h"

#ضمن "image.h"
#ضمن "layer.h"
#ضمن "data.h"
#ضمن "tree.h"


#اگر_تعریف_شده GPU
فراغ pull_شبکه_output(شبکه *net);
#پایان_اگر

فراغ compare_networks(شبکه *n1, شبکه *n2, داده d);
حرف *get_layer_string(LAYER_TYPE a);

شبکه *make_شبکه(صحيح n);


عائم شبکه_accuracy_multi(شبکه *net, داده d, صحيح n);
صحيح get_predicted_class_شبکه(شبکه *net);
فراغ print_شبکه(شبکه *net);
صحيح resize_شبکه(شبکه *net, صحيح w, صحيح h);
فراغ calc_شبکه_هزینه(شبکه *net);

#پایان_اگر


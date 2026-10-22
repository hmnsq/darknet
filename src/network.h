// Oh boy, why am I about to do this....
#جے_بنا_تعریف جال_H
#تعریف_کرو جال_H
#ضمن "darknet.h"

#ضمن "image.h"
#ضمن "layer.h"
#ضمن "data.h"
#ضمن "tree.h"


#جے_تعریف_شدہ GPU
فراغ pull_جال_output(جال *net);
#جے_انت

فراغ compare_networks(جال *n1, جال *n2, انکڑے d);
حرف *get_layer_string(LAYER_TYPE a);

جال *make_جال(صحيح n);


عائم جال_accuracy_multi(جال *net, انکڑے d, صحيح n);
صحيح get_predicted_class_جال(جال *net);
فراغ print_جال(جال *net);
صحيح resize_جال(جال *net, صحيح w, صحيح h);
فراغ calc_جال_لاگت(جال *net);

#جے_انت


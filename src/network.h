// Oh boy, why am I about to do this....
#اگر_غیر_تعریف_شدہ جال_H
#تعریف_کرو جال_H
#شامل "darknet.h"

#شامل "image.h"
#شامل "layer.h"
#شامل "data.h"
#شامل "tree.h"


#اگر_تعریف_شدہ GPU
خالی pull_جال_output(جال *net);
#اگر_ختم

خالی compare_networks(جال *n1, جال *n2, ڈیٹا d);
حرف *get_layer_string(LAYER_TYPE a);

جال *make_جال(صحیح n);


اعشاری جال_accuracy_multi(جال *net, ڈیٹا d, صحیح n);
صحیح get_predicted_class_جال(جال *net);
خالی print_جال(جال *net);
صحیح resize_جال(جال *net, صحیح w, صحیح h);
خالی calc_جال_لاگت(جال *net);

#اگر_ختم


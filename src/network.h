// Oh boy, why am I about to do this....
#నిర్వచితం_కాకపోతే జాలం_H
#నిర్వచించు జాలం_H
#అన్తర్భావ "darknet.h"

#అన్తర్భావ "image.h"
#అన్తర్భావ "layer.h"
#అన్తర్భావ "data.h"
#అన్తర్భావ "tree.h"


#నిర్వచితమైతే GPU
శూన్య pull_జాలం_output(జాలం *net);
#అయితే_ముగింపు

శూన్య compare_networks(జాలం *n1, జాలం *n2, దత్తాంశం d);
అక్షరం *get_layer_string(LAYER_TYPE a);

జాలం *make_జాలం(పూర్ణాంకం n);


దశాంశం జాలం_accuracy_multi(జాలం *net, దత్తాంశం d, పూర్ణాంకం n);
పూర్ణాంకం get_predicted_class_జాలం(జాలం *net);
శూన్య print_జాలం(జాలం *net);
పూర్ణాంకం resize_జాలం(జాలం *net, పూర్ణాంకం w, పూర్ణాంకం h);
శూన్య calc_జాలం_ఖర్చు(జాలం *net);

#అయితే_ముగింపు


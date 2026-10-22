#నిర్వచితం_కాకపోతే OPTION_LIST_H
#నిర్వచించు OPTION_LIST_H
#అన్తర్భావ "list.h"

రకనిర్వచనం నిర్మాణం{
    అక్షరం *కీలకం;
    అక్షరం *val;
    పూర్ణాంకం వాడిన;
} kvp;


పూర్ణాంకం read_option(అక్షరం *s, list *options);
శూన్య option_చొప్పించు(list *l, అక్షరం *కీలకం, అక్షరం *val);
అక్షరం *option_కనుగొను(list *l, అక్షరం *కీలకం);
దశాంశం option_కనుగొను_float(list *l, అక్షరం *కీలకం, దశాంశం def);
దశాంశం option_కనుగొను_float_quiet(list *l, అక్షరం *కీలకం, దశాంశం def);
శూన్య option_వాడని(list *l);

#అయితే_ముగింపు

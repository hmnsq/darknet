#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <assert.h>

#అన్తర్భావ "activation_layer.h"
#అన్తర్భావ "logistic_layer.h"
#అన్తర్భావ "l2norm_layer.h"
#అన్తర్భావ "activations.h"
#అన్తర్భావ "avgpool_layer.h"
#అన్తర్భావ "batchnorm_layer.h"
#అన్తర్భావ "blas.h"
#అన్తర్భావ "connected_layer.h"
#అన్తర్భావ "deconvolutional_layer.h"
#అన్తర్భావ "convolutional_layer.h"
#అన్తర్భావ "cost_layer.h"
#అన్తర్భావ "crnn_layer.h"
#అన్తర్భావ "crop_layer.h"
#అన్తర్భావ "detection_layer.h"
#అన్తర్భావ "dropout_layer.h"
#అన్తర్భావ "gru_layer.h"
#అన్తర్భావ "list.h"
#అన్తర్భావ "local_layer.h"
#అన్తర్భావ "maxpool_layer.h"
#అన్తర్భావ "normalization_layer.h"
#అన్తర్భావ "option_list.h"
#అన్తర్భావ "parser.h"
#అన్తర్భావ "region_layer.h"
#అన్తర్భావ "yolo_layer.h"
#అన్తర్భావ "iseg_layer.h"
#అన్తర్భావ "reorg_layer.h"
#అన్తర్భావ "rnn_layer.h"
#అన్తర్భావ "route_layer.h"
#అన్తర్భావ "upsample_layer.h"
#అన్తర్భావ "shortcut_layer.h"
#అన్తర్భావ "softmax_layer.h"
#అన్తర్భావ "lstm_layer.h"
#అన్తర్భావ "utils.h"

రకనిర్వచనం నిర్మాణం{
    అక్షరం *type;
    list *options;
}section;

list *read_cfg(అక్షరం *filename);

LAYER_TYPE string_గమ్యం_layer_type(అక్షరం * type)
{

    ఒకవేళ (strcmp(type, "[shortcut]")==0) తిరిగి SHORTCUT;
    ఒకవేళ (strcmp(type, "[crop]")==0) తిరిగి CROP;
    ఒకవేళ (strcmp(type, "[cost]")==0) తిరిగి ఖర్చు;
    ఒకవేళ (strcmp(type, "[detection]")==0) తిరిగి DETECTION;
    ఒకవేళ (strcmp(type, "[region]")==0) తిరిగి REGION;
    ఒకవేళ (strcmp(type, "[yolo]")==0) తిరిగి YOLO;
    ఒకవేళ (strcmp(type, "[iseg]")==0) తిరిగి ISEG;
    ఒకవేళ (strcmp(type, "[local]")==0) తిరిగి LOCAL;
    ఒకవేళ (strcmp(type, "[conv]")==0
            || strcmp(type, "[convolutional]")==0) తిరిగి CONVOLUTIONAL;
    ఒకవేళ (strcmp(type, "[deconv]")==0
            || strcmp(type, "[deconvolutional]")==0) తిరిగి DECONVOLUTIONAL;
    ఒకవేళ (strcmp(type, "[activation]")==0) తిరిగి చురుకైన;
    ఒకవేళ (strcmp(type, "[logistic]")==0) తిరిగి LOGXENT;
    ఒకవేళ (strcmp(type, "[l2norm]")==0) తిరిగి L2NORM;
    ఒకవేళ (strcmp(type, "[net]")==0
            || strcmp(type, "[network]")==0) తిరిగి జాలం_2;
    ఒకవేళ (strcmp(type, "[crnn]")==0) తిరిగి CRNN;
    ఒకవేళ (strcmp(type, "[gru]")==0) తిరిగి GRU;
    ఒకవేళ (strcmp(type, "[lstm]") == 0) తిరిగి LSTM;
    ఒకవేళ (strcmp(type, "[rnn]")==0) తిరిగి RNN;
    ఒకవేళ (strcmp(type, "[conn]")==0
            || strcmp(type, "[connected]")==0) తిరిగి CONNECTED;
    ఒకవేళ (strcmp(type, "[max]")==0
            || strcmp(type, "[maxpool]")==0) తిరిగి MAXPOOL;
    ఒకవేళ (strcmp(type, "[reorg]")==0) తిరిగి REORG;
    ఒకవేళ (strcmp(type, "[avg]")==0
            || strcmp(type, "[avgpool]")==0) తిరిగి AVGPOOL;
    ఒకవేళ (strcmp(type, "[dropout]")==0) తిరిగి DROPOUT;
    ఒకవేళ (strcmp(type, "[lrn]")==0
            || strcmp(type, "[normalization]")==0) తిరిగి NORMALIZATION;
    ఒకవేళ (strcmp(type, "[batchnorm]")==0) తిరిగి BATCHNORM;
    ఒకవేళ (strcmp(type, "[soft]")==0
            || strcmp(type, "[softmax]")==0) తిరిగి SOFTMAX;
    ఒకవేళ (strcmp(type, "[route]")==0) తిరిగి ROUTE;
    ఒకవేళ (strcmp(type, "[upsample]")==0) తిరిగి UPSAMPLE;
    తిరిగి BLANK;
}

శూన్య free_section(section *s)
{
    విడుదల(s->type);
    కణుపు *n = s->options->front;
    అయితే(n){
        kvp *pair = (kvp *)n->val;
        విడుదల(pair->కీలకం);
        విడుదల(pair);
        కణుపు *తదుపరి = n->తదుపరి;
        విడుదల(n);
        n = తదుపరి;
    }
    విడుదల(s->options);
    విడుదల(s);
}

శూన్య parse_దత్తాంశం(అక్షరం *దత్తాంశం, దశాంశం *a, పూర్ణాంకం n)
{
    పూర్ణాంకం i;
    ఒకవేళ(!దత్తాంశం) తిరిగి;
    అక్షరం *curr = దత్తాంశం;
    అక్షరం *తదుపరి = దత్తాంశం;
    పూర్ణాంకం పూర్తి = 0;
    కొరకు(i = 0; i < n && !పూర్తి; ++i){
        అయితే(*++తదుపరి !='\0' && *తదుపరి != ',');
        ఒకవేళ(*తదుపరి == '\0') పూర్తి = 1;
        *తదుపరి = '\0';
        అక్షరశ్రేణి_చదువు(curr, "%g", &a[i]);
        curr = తదుపరి+1;
    }
}

రకనిర్వచనం నిర్మాణం కొలత_params{
    పూర్ణాంకం batch;
    పూర్ణాంకం inputs;
    పూర్ణాంకం h;
    పూర్ణాంకం w;
    పూర్ణాంకం c;
    పూర్ణాంకం సూచీ;
    పూర్ణాంకం సమయం_అడుగులు;
    జాలం *net;
} కొలత_params;

local_layer parse_local(list *options, కొలత_params params)
{
    పూర్ణాంకం n = option_కనుగొను_int(options, "filters",1);
    పూర్ణాంకం కొలత = option_కనుగొను_int(options, "size",1);
    పూర్ణాంకం stride = option_కనుగొను_int(options, "stride",1);
    పూర్ణాంకం pad = option_కనుగొను_int(options, "pad",0);
    అక్షరం *activation_s = option_కనుగొను_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    పూర్ణాంకం batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ఒకవేళ(!(h && w && c)) error("Layer before local layer must output image.");

    local_layer layer = make_local_layer(batch,h,w,c,n,కొలత,stride,pad,activation);

    తిరిగి layer;
}

layer parse_deconvolutional(list *options, కొలత_params params)
{
    పూర్ణాంకం n = option_కనుగొను_int(options, "filters",1);
    పూర్ణాంకం కొలత = option_కనుగొను_int(options, "size",1);
    పూర్ణాంకం stride = option_కనుగొను_int(options, "stride",1);

    అక్షరం *activation_s = option_కనుగొను_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    పూర్ణాంకం batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ఒకవేళ(!(h && w && c)) error("Layer before deconvolutional layer must output image.");
    పూర్ణాంకం batch_normalize = option_కనుగొను_int_quiet(options, "batch_normalize", 0);
    పూర్ణాంకం pad = option_కనుగొను_int_quiet(options, "pad",0);
    పూర్ణాంకం padding = option_కనుగొను_int_quiet(options, "padding",0);
    ఒకవేళ(pad) padding = కొలత/2;

    layer l = make_deconvolutional_layer(batch,h,w,c,n,కొలత,stride,padding, activation, batch_normalize, params.net->adam);

    తిరిగి l;
}


convolutional_layer parse_convolutional(list *options, కొలత_params params)
{
    పూర్ణాంకం n = option_కనుగొను_int(options, "filters",1);
    పూర్ణాంకం కొలత = option_కనుగొను_int(options, "size",1);
    పూర్ణాంకం stride = option_కనుగొను_int(options, "stride",1);
    పూర్ణాంకం pad = option_కనుగొను_int_quiet(options, "pad",0);
    పూర్ణాంకం padding = option_కనుగొను_int_quiet(options, "padding",0);
    పూర్ణాంకం groups = option_కనుగొను_int_quiet(options, "groups", 1);
    ఒకవేళ(pad) padding = కొలత/2;

    అక్షరం *activation_s = option_కనుగొను_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    పూర్ణాంకం batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ఒకవేళ(!(h && w && c)) error("Layer before convolutional layer must output image.");
    పూర్ణాంకం batch_normalize = option_కనుగొను_int_quiet(options, "batch_normalize", 0);
    పూర్ణాంకం ద్విమాన = option_కనుగొను_int_quiet(options, "binary", 0);
    పూర్ణాంకం xnor = option_కనుగొను_int_quiet(options, "xnor", 0);

    convolutional_layer layer = make_convolutional_layer(batch,h,w,c,n,groups,కొలత,stride,padding,activation, batch_normalize, ద్విమాన, xnor, params.net->adam);
    layer.flipped = option_కనుగొను_int_quiet(options, "flipped", 0);
    layer.dot = option_కనుగొను_float_quiet(options, "dot", 0);

    తిరిగి layer;
}

layer parse_crnn(list *options, కొలత_params params)
{
    పూర్ణాంకం output_filters = option_కనుగొను_int(options, "output_filters",1);
    పూర్ణాంకం hidden_filters = option_కనుగొను_int(options, "hidden_filters",1);
    అక్షరం *activation_s = option_కనుగొను_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    పూర్ణాంకం batch_normalize = option_కనుగొను_int_quiet(options, "batch_normalize", 0);

    layer l = make_crnn_layer(params.batch, params.w, params.h, params.c, hidden_filters, output_filters, params.సమయం_అడుగులు, activation, batch_normalize);

    l.shortcut = option_కనుగొను_int_quiet(options, "shortcut", 0);

    తిరిగి l;
}

layer parse_rnn(list *options, కొలత_params params)
{
    పూర్ణాంకం output = option_కనుగొను_int(options, "output",1);
    అక్షరం *activation_s = option_కనుగొను_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    పూర్ణాంకం batch_normalize = option_కనుగొను_int_quiet(options, "batch_normalize", 0);

    layer l = make_rnn_layer(params.batch, params.inputs, output, params.సమయం_అడుగులు, activation, batch_normalize, params.net->adam);

    l.shortcut = option_కనుగొను_int_quiet(options, "shortcut", 0);

    తిరిగి l;
}

layer parse_gru(list *options, కొలత_params params)
{
    పూర్ణాంకం output = option_కనుగొను_int(options, "output",1);
    పూర్ణాంకం batch_normalize = option_కనుగొను_int_quiet(options, "batch_normalize", 0);

    layer l = make_gru_layer(params.batch, params.inputs, output, params.సమయం_అడుగులు, batch_normalize, params.net->adam);
    l.tanh = option_కనుగొను_int_quiet(options, "tanh", 0);

    తిరిగి l;
}

layer parse_lstm(list *options, కొలత_params params)
{
    పూర్ణాంకం output = option_కనుగొను_int(options, "output", 1);
    పూర్ణాంకం batch_normalize = option_కనుగొను_int_quiet(options, "batch_normalize", 0);

    layer l = make_lstm_layer(params.batch, params.inputs, output, params.సమయం_అడుగులు, batch_normalize, params.net->adam);

    తిరిగి l;
}

layer parse_connected(list *options, కొలత_params params)
{
    పూర్ణాంకం output = option_కనుగొను_int(options, "output",1);
    అక్షరం *activation_s = option_కనుగొను_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    పూర్ణాంకం batch_normalize = option_కనుగొను_int_quiet(options, "batch_normalize", 0);

    layer l = make_connected_layer(params.batch, params.inputs, output, activation, batch_normalize, params.net->adam);
    తిరిగి l;
}

layer parse_softmax(list *options, కొలత_params params)
{
    పూర్ణాంకం groups = option_కనుగొను_int_quiet(options, "groups",1);
    layer l = make_softmax_layer(params.batch, params.inputs, groups);
    l.temperature = option_కనుగొను_float_quiet(options, "temperature", 1);
    అక్షరం *వృక్షం_file = option_కనుగొను_str(options, "tree", 0);
    ఒకవేళ (వృక్షం_file) l.softmax_వృక్షం = read_వృక్షం(వృక్షం_file);
    l.w = params.w;
    l.h = params.h;
    l.c = params.c;
    l.spatial = option_కనుగొను_float_quiet(options, "spatial", 0);
    l.noloss =  option_కనుగొను_int_quiet(options, "noloss", 0);
    తిరిగి l;
}

పూర్ణాంకం *parse_yolo_ముసుగు(అక్షరం *a, పూర్ణాంకం *అంకె)
{
    పూర్ణాంకం *ముసుగు = 0;
    ఒకవేళ(a){
        పూర్ణాంకం len = అక్షరశ్రేణి_పొడవు(a);
        పూర్ణాంకం n = 1;
        పూర్ణాంకం i;
        కొరకు(i = 0; i < len; ++i){
            ఒకవేళ (a[i] == ',') ++n;
        }
        ముసుగు = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(పూర్ణాంకం));
        కొరకు(i = 0; i < n; ++i){
            పూర్ణాంకం val = atoi(a);
            ముసుగు[i] = val;
            a = strchr(a, ',')+1;
        }
        *అంకె = n;
    }
    తిరిగి ముసుగు;
}

layer parse_yolo(list *options, కొలత_params params)
{
    పూర్ణాంకం classes = option_కనుగొను_int(options, "classes", 20);
    పూర్ణాంకం మొత్తం = option_కనుగొను_int(options, "num", 1);
    పూర్ణాంకం అంకె = మొత్తం;

    అక్షరం *a = option_కనుగొను_str(options, "mask", 0);
    పూర్ణాంకం *ముసుగు = parse_yolo_ముసుగు(a, &అంకె);
    layer l = make_yolo_layer(params.batch, params.w, params.h, అంకె, మొత్తం, ముసుగు, classes);
    ప్రతిజ్ఞా(l.outputs == params.inputs);

    l.max_boxes = option_కనుగొను_int_quiet(options, "max",90);
    l.jitter = option_కనుగొను_float(options, "jitter", .2);

    l.ignore_thresh = option_కనుగొను_float(options, "ignore_thresh", .5);
    l.truth_thresh = option_కనుగొను_float(options, "truth_thresh", 1);
    l.random = option_కనుగొను_int_quiet(options, "random", 0);

    అక్షరం *map_file = option_కనుగొను_str(options, "map", 0);
    ఒకవేళ (map_file) l.map = read_map(map_file);

    a = option_కనుగొను_str(options, "anchors", 0);
    ఒకవేళ(a){
        పూర్ణాంకం len = అక్షరశ్రేణి_పొడవు(a);
        పూర్ణాంకం n = 1;
        పూర్ణాంకం i;
        కొరకు(i = 0; i < len; ++i){
            ఒకవేళ (a[i] == ',') ++n;
        }
        కొరకు(i = 0; i < n; ++i){
            దశాంశం bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    తిరిగి l;
}

layer parse_iseg(list *options, కొలత_params params)
{
    పూర్ణాంకం classes = option_కనుగొను_int(options, "classes", 20);
    పూర్ణాంకం ids = option_కనుగొను_int(options, "ids", 32);
    layer l = make_iseg_layer(params.batch, params.w, params.h, classes, ids);
    ప్రతిజ్ఞా(l.outputs == params.inputs);
    తిరిగి l;
}

layer parse_region(list *options, కొలత_params params)
{
    పూర్ణాంకం coords = option_కనుగొను_int(options, "coords", 4);
    పూర్ణాంకం classes = option_కనుగొను_int(options, "classes", 20);
    పూర్ణాంకం అంకె = option_కనుగొను_int(options, "num", 1);

    layer l = make_region_layer(params.batch, params.w, params.h, అంకె, classes, coords);
    ప్రతిజ్ఞా(l.outputs == params.inputs);

    l.సంవర్గమానం = option_కనుగొను_int_quiet(options, "log", 0);
    l.వర్గమూలం = option_కనుగొను_int_quiet(options, "sqrt", 0);

    l.softmax = option_కనుగొను_int(options, "softmax", 0);
    l.background = option_కనుగొను_int_quiet(options, "background", 0);
    l.max_boxes = option_కనుగొను_int_quiet(options, "max",30);
    l.jitter = option_కనుగొను_float(options, "jitter", .2);
    l.rescore = option_కనుగొను_int_quiet(options, "rescore",0);

    l.thresh = option_కనుగొను_float(options, "thresh", .5);
    l.classfix = option_కనుగొను_int_quiet(options, "classfix", 0);
    l.absolute = option_కనుగొను_int_quiet(options, "absolute", 0);
    l.random = option_కనుగొను_int_quiet(options, "random", 0);

    l.coord_scale = option_కనుగొను_float(options, "coord_scale", 1);
    l.object_scale = option_కనుగొను_float(options, "object_scale", 1);
    l.noobject_scale = option_కనుగొను_float(options, "noobject_scale", 1);
    l.ముసుగు_scale = option_కనుగొను_float(options, "mask_scale", 1);
    l.class_scale = option_కనుగొను_float(options, "class_scale", 1);
    l.bias_సరిపోలిక = option_కనుగొను_int_quiet(options, "bias_match",0);

    అక్షరం *వృక్షం_file = option_కనుగొను_str(options, "tree", 0);
    ఒకవేళ (వృక్షం_file) l.softmax_వృక్షం = read_వృక్షం(వృక్షం_file);
    అక్షరం *map_file = option_కనుగొను_str(options, "map", 0);
    ఒకవేళ (map_file) l.map = read_map(map_file);

    అక్షరం *a = option_కనుగొను_str(options, "anchors", 0);
    ఒకవేళ(a){
        పూర్ణాంకం len = అక్షరశ్రేణి_పొడవు(a);
        పూర్ణాంకం n = 1;
        పూర్ణాంకం i;
        కొరకు(i = 0; i < len; ++i){
            ఒకవేళ (a[i] == ',') ++n;
        }
        కొరకు(i = 0; i < n; ++i){
            దశాంశం bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    తిరిగి l;
}

detection_layer parse_detection(list *options, కొలత_params params)
{
    పూర్ణాంకం coords = option_కనుగొను_int(options, "coords", 1);
    పూర్ణాంకం classes = option_కనుగొను_int(options, "classes", 1);
    పూర్ణాంకం rescore = option_కనుగొను_int(options, "rescore", 0);
    పూర్ణాంకం అంకె = option_కనుగొను_int(options, "num", 1);
    పూర్ణాంకం side = option_కనుగొను_int(options, "side", 7);
    detection_layer layer = make_detection_layer(params.batch, params.inputs, అంకె, side, classes, coords, rescore);

    layer.softmax = option_కనుగొను_int(options, "softmax", 0);
    layer.వర్గమూలం = option_కనుగొను_int(options, "sqrt", 0);

    layer.max_boxes = option_కనుగొను_int_quiet(options, "max",90);
    layer.coord_scale = option_కనుగొను_float(options, "coord_scale", 1);
    layer.forced = option_కనుగొను_int(options, "forced", 0);
    layer.object_scale = option_కనుగొను_float(options, "object_scale", 1);
    layer.noobject_scale = option_కనుగొను_float(options, "noobject_scale", 1);
    layer.class_scale = option_కనుగొను_float(options, "class_scale", 1);
    layer.jitter = option_కనుగొను_float(options, "jitter", .2);
    layer.random = option_కనుగొను_int_quiet(options, "random", 0);
    layer.reorg = option_కనుగొను_int_quiet(options, "reorg", 0);
    తిరిగి layer;
}

ఖర్చు_layer parse_ఖర్చు(list *options, కొలత_params params)
{
    అక్షరం *type_s = option_కనుగొను_str(options, "type", "sse");
    ఖర్చు_TYPE type = get_ఖర్చు_type(type_s);
    దశాంశం scale = option_కనుగొను_float_quiet(options, "scale",1);
    ఖర్చు_layer layer = make_ఖర్చు_layer(params.batch, params.inputs, type, scale);
    layer.నిష్పత్తి =  option_కనుగొను_float_quiet(options, "ratio",0);
    layer.noobject_scale =  option_కనుగొను_float_quiet(options, "noobj", 1);
    layer.thresh =  option_కనుగొను_float_quiet(options, "thresh",0);
    తిరిగి layer;
}

crop_layer parse_crop(list *options, కొలత_params params)
{
    పూర్ణాంకం crop_ఎత్తు = option_కనుగొను_int(options, "crop_height",1);
    పూర్ణాంకం crop_వెడల్పు = option_కనుగొను_int(options, "crop_width",1);
    పూర్ణాంకం flip = option_కనుగొను_int(options, "flip",0);
    దశాంశం angle = option_కనుగొను_float(options, "angle",0);
    దశాంశం saturation = option_కనుగొను_float(options, "saturation",1);
    దశాంశం exposure = option_కనుగొను_float(options, "exposure",1);

    పూర్ణాంకం batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ఒకవేళ(!(h && w && c)) error("Layer before crop layer must output image.");

    పూర్ణాంకం noadjust = option_కనుగొను_int_quiet(options, "noadjust",0);

    crop_layer l = make_crop_layer(batch,h,w,c,crop_ఎత్తు,crop_వెడల్పు,flip, angle, saturation, exposure);
    l.జరుపు = option_కనుగొను_float(options, "shift", 0);
    l.noadjust = noadjust;
    తిరిగి l;
}

layer parse_reorg(list *options, కొలత_params params)
{
    పూర్ణాంకం stride = option_కనుగొను_int(options, "stride",1);
    పూర్ణాంకం తిరగేయి = option_కనుగొను_int_quiet(options, "reverse",0);
    పూర్ణాంకం flatten = option_కనుగొను_int_quiet(options, "flatten",0);
    పూర్ణాంకం extra = option_కనుగొను_int_quiet(options, "extra",0);

    పూర్ణాంకం batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ఒకవేళ(!(h && w && c)) error("Layer before reorg layer must output image.");

    layer layer = make_reorg_layer(batch,w,h,c,stride,తిరగేయి, flatten, extra);
    తిరిగి layer;
}

maxpool_layer parse_maxpool(list *options, కొలత_params params)
{
    పూర్ణాంకం stride = option_కనుగొను_int(options, "stride",1);
    పూర్ణాంకం కొలత = option_కనుగొను_int(options, "size",stride);
    పూర్ణాంకం padding = option_కనుగొను_int_quiet(options, "padding", కొలత-1);

    పూర్ణాంకం batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ఒకవేళ(!(h && w && c)) error("Layer before maxpool layer must output image.");

    maxpool_layer layer = make_maxpool_layer(batch,h,w,c,కొలత,stride,padding);
    తిరిగి layer;
}

avgpool_layer parse_avgpool(list *options, కొలత_params params)
{
    పూర్ణాంకం batch,w,h,c;
    w = params.w;
    h = params.h;
    c = params.c;
    batch=params.batch;
    ఒకవేళ(!(h && w && c)) error("Layer before avgpool layer must output image.");

    avgpool_layer layer = make_avgpool_layer(batch,w,h,c);
    తిరిగి layer;
}

dropout_layer parse_dropout(list *options, కొలత_params params)
{
    దశాంశం probability = option_కనుగొను_float(options, "probability", .5);
    dropout_layer layer = make_dropout_layer(params.batch, params.inputs, probability);
    layer.బహిర్గతం_w = params.w;
    layer.బహిర్గతం_h = params.h;
    layer.బహిర్గతం_c = params.c;
    తిరిగి layer;
}

layer parse_normalization(list *options, కొలత_params params)
{
    దశాంశం alpha = option_కనుగొను_float(options, "alpha", .0001);
    దశాంశం beta =  option_కనుగొను_float(options, "beta" , .75);
    దశాంశం kappa = option_కనుగొను_float(options, "kappa", 1);
    పూర్ణాంకం కొలత = option_కనుగొను_int(options, "size", 5);
    layer l = make_normalization_layer(params.batch, params.w, params.h, params.c, కొలత, alpha, beta, kappa);
    తిరిగి l;
}

layer parse_batchnorm(list *options, కొలత_params params)
{
    layer l = make_batchnorm_layer(params.batch, params.w, params.h, params.c);
    తిరిగి l;
}

layer parse_shortcut(list *options, కొలత_params params, జాలం *net)
{
    అక్షరం *l = option_కనుగొను(options, "from");
    పూర్ణాంకం సూచీ = atoi(l);
    ఒకవేళ(సూచీ < 0) సూచీ = params.సూచీ + సూచీ;

    పూర్ణాంకం batch = params.batch;
    layer from = net->layers[సూచీ];

    layer s = make_shortcut_layer(batch, సూచీ, params.w, params.h, params.c, from.బహిర్గతం_w, from.బహిర్గతం_h, from.బహిర్గతం_c);

    అక్షరం *activation_s = option_కనుగొను_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);
    s.activation = activation;
    s.alpha = option_కనుగొను_float_quiet(options, "alpha", 1);
    s.beta = option_కనుగొను_float_quiet(options, "beta", 1);
    తిరిగి s;
}


layer parse_l2norm(list *options, కొలత_params params)
{
    layer l = make_l2norm_layer(params.batch, params.inputs);
    l.h = l.బహిర్గతం_h = params.h;
    l.w = l.బహిర్గతం_w = params.w;
    l.c = l.బహిర్గతం_c = params.c;
    తిరిగి l;
}


layer parse_logistic(list *options, కొలత_params params)
{
    layer l = make_logistic_layer(params.batch, params.inputs);
    l.h = l.బహిర్గతం_h = params.h;
    l.w = l.బహిర్గతం_w = params.w;
    l.c = l.బహిర్గతం_c = params.c;
    తిరిగి l;
}

layer parse_activation(list *options, కొలత_params params)
{
    అక్షరం *activation_s = option_కనుగొను_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);

    layer l = make_activation_layer(params.batch, params.inputs, activation);

    l.h = l.బహిర్గతం_h = params.h;
    l.w = l.బహిర్గతం_w = params.w;
    l.c = l.బహిర్గతం_c = params.c;

    తిరిగి l;
}

layer parse_upsample(list *options, కొలత_params params, జాలం *net)
{

    పూర్ణాంకం stride = option_కనుగొను_int(options, "stride",2);
    layer l = make_upsample_layer(params.batch, params.w, params.h, params.c, stride);
    l.scale = option_కనుగొను_float_quiet(options, "scale", 1);
    తిరిగి l;
}

route_layer parse_route(list *options, కొలత_params params, జాలం *net)
{
    అక్షరం *l = option_కనుగొను(options, "layers");
    పూర్ణాంకం len = అక్షరశ్రేణి_పొడవు(l);
    ఒకవేళ(!l) error("Route Layer must specify input layers");
    పూర్ణాంకం n = 1;
    పూర్ణాంకం i;
    కొరకు(i = 0; i < len; ++i){
        ఒకవేళ (l[i] == ',') ++n;
    }

    పూర్ణాంకం *layers = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(పూర్ణాంకం));
    పూర్ణాంకం *sizes = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(పూర్ణాంకం));
    కొరకు(i = 0; i < n; ++i){
        పూర్ణాంకం సూచీ = atoi(l);
        l = strchr(l, ',')+1;
        ఒకవేళ(సూచీ < 0) సూచీ = params.సూచీ + సూచీ;
        layers[i] = సూచీ;
        sizes[i] = net->layers[సూచీ].outputs;
    }
    పూర్ణాంకం batch = params.batch;

    route_layer layer = make_route_layer(batch, n, layers, sizes);

    convolutional_layer మొదటి = net->layers[layers[0]];
    layer.బహిర్గతం_w = మొదటి.బహిర్గతం_w;
    layer.బహిర్గతం_h = మొదటి.బహిర్గతం_h;
    layer.బహిర్గతం_c = మొదటి.బహిర్గతం_c;
    కొరకు(i = 1; i < n; ++i){
        పూర్ణాంకం సూచీ = layers[i];
        convolutional_layer తదుపరి = net->layers[సూచీ];
        ఒకవేళ(తదుపరి.బహిర్గతం_w == మొదటి.బహిర్గతం_w && తదుపరి.బహిర్గతం_h == మొదటి.బహిర్గతం_h){
            layer.బహిర్గతం_c += తదుపరి.బహిర్గతం_c;
        }లేకపోతే{
            layer.బహిర్గతం_h = layer.బహిర్గతం_w = layer.బహిర్గతం_c = 0;
        }
    }

    తిరిగి layer;
}

learning_రేటు_policy get_policy(అక్షరం *s)
{
    ఒకవేళ (strcmp(s, "random")==0) తిరిగి RANDOM;
    ఒకవేళ (strcmp(s, "poly")==0) తిరిగి POLY;
    ఒకవేళ (strcmp(s, "constant")==0) తిరిగి CONSTANT;
    ఒకవేళ (strcmp(s, "step")==0) తిరిగి అడుగు_2;
    ఒకవేళ (strcmp(s, "exp")==0) తిరిగి EXP;
    ఒకవేళ (strcmp(s, "sigmoid")==0) తిరిగి SIG;
    ఒకవేళ (strcmp(s, "steps")==0) తిరిగి అడుగులు_2;
    fprintf(stderr, "Couldn't find policy %s, going with constant\n", s);
    తిరిగి CONSTANT;
}

శూన్య parse_net_options(list *options, జాలం *net)
{
    net->batch = option_కనుగొను_int(options, "batch",1);
    net->learning_రేటు = option_కనుగొను_float(options, "learning_rate", .001);
    net->momentum = option_కనుగొను_float(options, "momentum", .9);
    net->decay = option_కనుగొను_float(options, "decay", .0001);
    పూర్ణాంకం subdivs = option_కనుగొను_int(options, "subdivisions",1);
    net->సమయం_అడుగులు = option_కనుగొను_int_quiet(options, "time_steps",1);
    net->notruth = option_కనుగొను_int_quiet(options, "notruth",0);
    net->batch /= subdivs;
    net->batch *= net->సమయం_అడుగులు;
    net->subdivisions = subdivs;
    net->random = option_కనుగొను_int_quiet(options, "random", 0);

    net->adam = option_కనుగొను_int_quiet(options, "adam", 0);
    ఒకవేళ(net->adam){
        net->B1 = option_కనుగొను_float(options, "B1", .9);
        net->B2 = option_కనుగొను_float(options, "B2", .999);
        net->eps = option_కనుగొను_float(options, "eps", .0000001);
    }

    net->h = option_కనుగొను_int_quiet(options, "height",0);
    net->w = option_కనుగొను_int_quiet(options, "width",0);
    net->c = option_కనుగొను_int_quiet(options, "channels",0);
    net->inputs = option_కనుగొను_int_quiet(options, "inputs", net->h * net->w * net->c);
    net->max_crop = option_కనుగొను_int_quiet(options, "max_crop",net->w*2);
    net->min_crop = option_కనుగొను_int_quiet(options, "min_crop",net->w);
    net->max_నిష్పత్తి = option_కనుగొను_float_quiet(options, "max_ratio", (దశాంశం) net->max_crop / net->w);
    net->min_నిష్పత్తి = option_కనుగొను_float_quiet(options, "min_ratio", (దశాంశం) net->min_crop / net->w);
    net->center = option_కనుగొను_int_quiet(options, "center",0);
    net->clip = option_కనుగొను_float_quiet(options, "clip", 0);

    net->angle = option_కనుగొను_float_quiet(options, "angle", 0);
    net->aspect = option_కనుగొను_float_quiet(options, "aspect", 1);
    net->saturation = option_కనుగొను_float_quiet(options, "saturation", 1);
    net->exposure = option_కనుగొను_float_quiet(options, "exposure", 1);
    net->hue = option_కనుగొను_float_quiet(options, "hue", 0);

    ఒకవేళ(!net->inputs && !(net->h && net->w && net->c)) error("No input parameters supplied");

    అక్షరం *policy_s = option_కనుగొను_str(options, "policy", "constant");
    net->policy = get_policy(policy_s);
    net->burn_లోపల = option_కనుగొను_int_quiet(options, "burn_in", 0);
    net->power = option_కనుగొను_float_quiet(options, "power", 4);
    ఒకవేళ(net->policy == అడుగు_2){
        net->అడుగు = option_కనుగొను_int(options, "step", 1);
        net->scale = option_కనుగొను_float(options, "scale", 1);
    } లేకపోతే ఒకవేళ (net->policy == అడుగులు_2){
        అక్షరం *l = option_కనుగొను(options, "steps");
        అక్షరం *p = option_కనుగొను(options, "scales");
        ఒకవేళ(!l || !p) error("STEPS policy must have steps and scales in cfg file");

        పూర్ణాంకం len = అక్షరశ్రేణి_పొడవు(l);
        పూర్ణాంకం n = 1;
        పూర్ణాంకం i;
        కొరకు(i = 0; i < len; ++i){
            ఒకవేళ (l[i] == ',') ++n;
        }
        పూర్ణాంకం *అడుగులు = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(పూర్ణాంకం));
        దశాంశం *scales = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం));
        కొరకు(i = 0; i < n; ++i){
            పూర్ణాంకం అడుగు    = atoi(l);
            దశాంశం scale = atof(p);
            l = strchr(l, ',')+1;
            p = strchr(p, ',')+1;
            అడుగులు[i] = అడుగు;
            scales[i] = scale;
        }
        net->scales = scales;
        net->అడుగులు = అడుగులు;
        net->అంకె_అడుగులు = n;
    } లేకపోతే ఒకవేళ (net->policy == EXP){
        net->gamma = option_కనుగొను_float(options, "gamma", 1);
    } లేకపోతే ఒకవేళ (net->policy == SIG){
        net->gamma = option_కనుగొను_float(options, "gamma", 1);
        net->అడుగు = option_కనుగొను_int(options, "step", 1);
    } లేకపోతే ఒకవేళ (net->policy == POLY || net->policy == RANDOM){
    }
    net->max_batches = option_కనుగొను_int(options, "max_batches", 0);
}

పూర్ణాంకం is_జాలం(section *s)
{
    తిరిగి (strcmp(s->type, "[net]")==0
            || strcmp(s->type, "[network]")==0);
}

జాలం *parse_జాలం_cfg(అక్షరం *filename)
{
    list *sections = read_cfg(filename);
    కణుపు *n = sections->front;
    ఒకవేళ(!n) error("Config file has no sections");
    జాలం *net = make_జాలం(sections->కొలత - 1);
    net->gpu_సూచీ = gpu_సూచీ;
    కొలత_params params;

    section *s = (section *)n->val;
    list *options = s->options;
    ఒకవేళ(!is_జాలం(s)) error("First section must be [net] or [network]");
    parse_net_options(options, net);

    params.h = net->h;
    params.w = net->w;
    params.c = net->c;
    params.inputs = net->inputs;
    params.batch = net->batch;
    params.సమయం_అడుగులు = net->సమయం_అడుగులు;
    params.net = net;

    size_t workspace_కొలత = 0;
    n = n->తదుపరి;
    పూర్ణాంకం లెక్క = 0;
    free_section(s);
    fprintf(stderr, "layer     filters    size              input                output\n");
    అయితే(n){
        params.సూచీ = లెక్క;
        fprintf(stderr, "%5d ", లెక్క);
        s = (section *)n->val;
        options = s->options;
        layer l = {0};
        LAYER_TYPE lt = string_గమ్యం_layer_type(s->type);
        ఒకవేళ(lt == CONVOLUTIONAL){
            l = parse_convolutional(options, params);
        }లేకపోతే ఒకవేళ(lt == DECONVOLUTIONAL){
            l = parse_deconvolutional(options, params);
        }లేకపోతే ఒకవేళ(lt == LOCAL){
            l = parse_local(options, params);
        }లేకపోతే ఒకవేళ(lt == చురుకైన){
            l = parse_activation(options, params);
        }లేకపోతే ఒకవేళ(lt == LOGXENT){
            l = parse_logistic(options, params);
        }లేకపోతే ఒకవేళ(lt == L2NORM){
            l = parse_l2norm(options, params);
        }లేకపోతే ఒకవేళ(lt == RNN){
            l = parse_rnn(options, params);
        }లేకపోతే ఒకవేళ(lt == GRU){
            l = parse_gru(options, params);
        }లేకపోతే ఒకవేళ (lt == LSTM) {
            l = parse_lstm(options, params);
        }లేకపోతే ఒకవేళ(lt == CRNN){
            l = parse_crnn(options, params);
        }లేకపోతే ఒకవేళ(lt == CONNECTED){
            l = parse_connected(options, params);
        }లేకపోతే ఒకవేళ(lt == CROP){
            l = parse_crop(options, params);
        }లేకపోతే ఒకవేళ(lt == ఖర్చు){
            l = parse_ఖర్చు(options, params);
        }లేకపోతే ఒకవేళ(lt == REGION){
            l = parse_region(options, params);
        }లేకపోతే ఒకవేళ(lt == YOLO){
            l = parse_yolo(options, params);
        }లేకపోతే ఒకవేళ(lt == ISEG){
            l = parse_iseg(options, params);
        }లేకపోతే ఒకవేళ(lt == DETECTION){
            l = parse_detection(options, params);
        }లేకపోతే ఒకవేళ(lt == SOFTMAX){
            l = parse_softmax(options, params);
            net->hierarchy = l.softmax_వృక్షం;
        }లేకపోతే ఒకవేళ(lt == NORMALIZATION){
            l = parse_normalization(options, params);
        }లేకపోతే ఒకవేళ(lt == BATCHNORM){
            l = parse_batchnorm(options, params);
        }లేకపోతే ఒకవేళ(lt == MAXPOOL){
            l = parse_maxpool(options, params);
        }లేకపోతే ఒకవేళ(lt == REORG){
            l = parse_reorg(options, params);
        }లేకపోతే ఒకవేళ(lt == AVGPOOL){
            l = parse_avgpool(options, params);
        }లేకపోతే ఒకవేళ(lt == ROUTE){
            l = parse_route(options, params, net);
        }లేకపోతే ఒకవేళ(lt == UPSAMPLE){
            l = parse_upsample(options, params, net);
        }లేకపోతే ఒకవేళ(lt == SHORTCUT){
            l = parse_shortcut(options, params, net);
        }లేకపోతే ఒకవేళ(lt == DROPOUT){
            l = parse_dropout(options, params);
            l.output = net->layers[లెక్క-1].output;
            l.delta = net->layers[లెక్క-1].delta;
#నిర్వచితమైతే GPU
            l.output_gpu = net->layers[లెక్క-1].output_gpu;
            l.delta_gpu = net->layers[లెక్క-1].delta_gpu;
#అయితే_ముగింపు
        }లేకపోతే{
            fprintf(stderr, "Type not recognized: %s\n", s->type);
        }
        l.clip = net->clip;
        l.truth = option_కనుగొను_int_quiet(options, "truth", 0);
        l.onlyforward = option_కనుగొను_int_quiet(options, "onlyforward", 0);
        l.stopbackward = option_కనుగొను_int_quiet(options, "stopbackward", 0);
        l.dontsave = option_కనుగొను_int_quiet(options, "dontsave", 0);
        l.dontload = option_కనుగొను_int_quiet(options, "dontload", 0);
        l.numload = option_కనుగొను_int_quiet(options, "numload", 0);
        l.dontloadscales = option_కనుగొను_int_quiet(options, "dontloadscales", 0);
        l.learning_రేటు_scale = option_కనుగొను_float_quiet(options, "learning_rate", 1);
        l.smooth = option_కనుగొను_float_quiet(options, "smooth", 0);
        option_వాడని(options);
        net->layers[లెక్క] = l;
        ఒకవేళ (l.workspace_కొలత > workspace_కొలత) workspace_కొలత = l.workspace_కొలత;
        free_section(s);
        n = n->తదుపరి;
        ++లెక్క;
        ఒకవేళ(n){
            params.h = l.బహిర్గతం_h;
            params.w = l.బహిర్గతం_w;
            params.c = l.బహిర్గతం_c;
            params.inputs = l.outputs;
        }
    }
    free_list(sections);
    layer బహిర్గతం = get_జాలం_output_layer(net);
    net->outputs = బహిర్గతం.outputs;
    net->truths = బహిర్గతం.outputs;
    ఒకవేళ(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = బహిర్గతం.output;
    net->ఆదానం = సున్నా_జ్ఞాపకం_ఇవ్వు(net->inputs*net->batch, పరిమాణం(దశాంశం));
    net->truth = సున్నా_జ్ఞాపకం_ఇవ్వు(net->truths*net->batch, పరిమాణం(దశాంశం));
#నిర్వచితమైతే GPU
    net->output_gpu = బహిర్గతం.output_gpu;
    net->ఆదానం_gpu = cuda_make_array(net->ఆదానం, net->inputs*net->batch);
    net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
#అయితే_ముగింపు
    ఒకవేళ(workspace_కొలత){
        //printf("%ld\n", workspace_size);
#నిర్వచితమైతే GPU
        ఒకవేళ(gpu_సూచీ >= 0){
            net->workspace = cuda_make_array(0, (workspace_కొలత-1)/పరిమాణం(దశాంశం)+1);
        }లేకపోతే {
            net->workspace = సున్నా_జ్ఞాపకం_ఇవ్వు(1, workspace_కొలత);
        }
#లేకపోతే
        net->workspace = సున్నా_జ్ఞాపకం_ఇవ్వు(1, workspace_కొలత);
#అయితే_ముగింపు
    }
    తిరిగి net;
}

list *read_cfg(అక్షరం *filename)
{
    FILE *file = fopen(filename, "r");
    ఒకవేళ(file == 0) file_error(filename);
    అక్షరం *పంక్తి;
    పూర్ణాంకం nu = 0;
    list *options = make_list();
    section *current = 0;
    అయితే((పంక్తి=fgetl(file)) != 0){
        ++ nu;
        strip(పంక్తి);
        ఎంపిక(పంక్తి[0]){
            స్థితి '[':
                current = జ్ఞాపకం_ఇవ్వు(పరిమాణం(section));
                list_చొప్పించు(options, current);
                current->options = make_list();
                current->type = పంక్తి;
                ఆపు;
            స్థితి '\0':
            స్థితి '#':
            స్థితి ';':
                విడుదల(పంక్తి);
                ఆపు;
            అప్రమేయ:
                ఒకవేళ(!read_option(పంక్తి, current->options)){
                    fprintf(stderr, "Config file error line %d, could parse: %s\n", nu, పంక్తి);
                    విడుదల(పంక్తి);
                }
                ఆపు;
        }
    }
    fclose(file);
    తిరిగి options;
}

శూన్య save_convolutional_weights_ద్విమాన(layer l, FILE *fp)
{
#నిర్వచితమైతే GPU
    ఒకవేళ(gpu_సూచీ >= 0){
        pull_convolutional_layer(l);
    }
#అయితే_ముగింపు
    binarize_weights(l.weights, l.n, l.c*l.కొలత*l.కొలత, l.ద్విమాన_weights);
    పూర్ణాంకం కొలత = l.c*l.కొలత*l.కొలత;
    పూర్ణాంకం i, j, k;
    fwrite(l.biases, పరిమాణం(దశాంశం), l.n, fp);
    ఒకవేళ (l.batch_normalize){
        fwrite(l.scales, పరిమాణం(దశాంశం), l.n, fp);
        fwrite(l.rolling_సగటు, పరిమాణం(దశాంశం), l.n, fp);
        fwrite(l.rolling_variance, పరిమాణం(దశాంశం), l.n, fp);
    }
    కొరకు(i = 0; i < l.n; ++i){
        దశాంశం సగటు = l.ద్విమాన_weights[i*కొలత];
        ఒకవేళ(సగటు < 0) సగటు = -సగటు;
        fwrite(&సగటు, పరిమాణం(దశాంశం), 1, fp);
        కొరకు(j = 0; j < కొలత/8; ++j){
            పూర్ణాంకం సూచీ = i*కొలత + j*8;
            గుర్తులేని అక్షరం c = 0;
            కొరకు(k = 0; k < 8; ++k){
                ఒకవేళ (j*8 + k >= కొలత) ఆపు;
                ఒకవేళ (l.ద్విమాన_weights[సూచీ + k] > 0) c = (c | 1<<k);
            }
            fwrite(&c, పరిమాణం(అక్షరం), 1, fp);
        }
    }
}

శూన్య save_convolutional_weights(layer l, FILE *fp)
{
    ఒకవేళ(l.ద్విమాన){
        //save_convolutional_weights_binary(l, fp);
        //return;
    }
#నిర్వచితమైతే GPU
    ఒకవేళ(gpu_సూచీ >= 0){
        pull_convolutional_layer(l);
    }
#అయితే_ముగింపు
    పూర్ణాంకం అంకె = l.nweights;
    fwrite(l.biases, పరిమాణం(దశాంశం), l.n, fp);
    ఒకవేళ (l.batch_normalize){
        fwrite(l.scales, పరిమాణం(దశాంశం), l.n, fp);
        fwrite(l.rolling_సగటు, పరిమాణం(దశాంశం), l.n, fp);
        fwrite(l.rolling_variance, పరిమాణం(దశాంశం), l.n, fp);
    }
    fwrite(l.weights, పరిమాణం(దశాంశం), అంకె, fp);
}

శూన్య save_batchnorm_weights(layer l, FILE *fp)
{
#నిర్వచితమైతే GPU
    ఒకవేళ(gpu_సూచీ >= 0){
        pull_batchnorm_layer(l);
    }
#అయితే_ముగింపు
    fwrite(l.scales, పరిమాణం(దశాంశం), l.c, fp);
    fwrite(l.rolling_సగటు, పరిమాణం(దశాంశం), l.c, fp);
    fwrite(l.rolling_variance, పరిమాణం(దశాంశం), l.c, fp);
}

శూన్య save_connected_weights(layer l, FILE *fp)
{
#నిర్వచితమైతే GPU
    ఒకవేళ(gpu_సూచీ >= 0){
        pull_connected_layer(l);
    }
#అయితే_ముగింపు
    fwrite(l.biases, పరిమాణం(దశాంశం), l.outputs, fp);
    fwrite(l.weights, పరిమాణం(దశాంశం), l.outputs*l.inputs, fp);
    ఒకవేళ (l.batch_normalize){
        fwrite(l.scales, పరిమాణం(దశాంశం), l.outputs, fp);
        fwrite(l.rolling_సగటు, పరిమాణం(దశాంశం), l.outputs, fp);
        fwrite(l.rolling_variance, పరిమాణం(దశాంశం), l.outputs, fp);
    }
}

శూన్య save_weights_upto(జాలం *net, అక్షరం *filename, పూర్ణాంకం cutoff)
{
#నిర్వచితమైతే GPU
    ఒకవేళ(net->gpu_సూచీ >= 0){
        cuda_సమితి_device(net->gpu_సూచీ);
    }
#అయితే_ముగింపు
    fprintf(stderr, "Saving weights to %s\n", filename);
    FILE *fp = fopen(filename, "wb");
    ఒకవేళ(!fp) file_error(filename);

    పూర్ణాంకం major = 0;
    పూర్ణాంకం minor = 2;
    పూర్ణాంకం revision = 0;
    fwrite(&major, పరిమాణం(పూర్ణాంకం), 1, fp);
    fwrite(&minor, పరిమాణం(పూర్ణాంకం), 1, fp);
    fwrite(&revision, పరిమాణం(పూర్ణాంకం), 1, fp);
    fwrite(net->చూసిన, పరిమాణం(size_t), 1, fp);

    పూర్ణాంకం i;
    కొరకు(i = 0; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        ఒకవేళ (l.dontsave) కొనసాగు;
        ఒకవేళ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            save_convolutional_weights(l, fp);
        } ఒకవేళ(l.type == CONNECTED){
            save_connected_weights(l, fp);
        } ఒకవేళ(l.type == BATCHNORM){
            save_batchnorm_weights(l, fp);
        } ఒకవేళ(l.type == RNN){
            save_connected_weights(*(l.ఆదానం_layer), fp);
            save_connected_weights(*(l.self_layer), fp);
            save_connected_weights(*(l.output_layer), fp);
        } ఒకవేళ (l.type == LSTM) {
            save_connected_weights(*(l.wi), fp);
            save_connected_weights(*(l.wf), fp);
            save_connected_weights(*(l.wo), fp);
            save_connected_weights(*(l.wg), fp);
            save_connected_weights(*(l.ui), fp);
            save_connected_weights(*(l.uf), fp);
            save_connected_weights(*(l.uo), fp);
            save_connected_weights(*(l.ug), fp);
        } ఒకవేళ (l.type == GRU) {
            ఒకవేళ(1){
                save_connected_weights(*(l.wz), fp);
                save_connected_weights(*(l.wr), fp);
                save_connected_weights(*(l.wh), fp);
                save_connected_weights(*(l.uz), fp);
                save_connected_weights(*(l.ur), fp);
                save_connected_weights(*(l.uh), fp);
            }లేకపోతే{
                save_connected_weights(*(l.reset_layer), fp);
                save_connected_weights(*(l.update_layer), fp);
                save_connected_weights(*(l.state_layer), fp);
            }
        }  ఒకవేళ(l.type == CRNN){
            save_convolutional_weights(*(l.ఆదానం_layer), fp);
            save_convolutional_weights(*(l.self_layer), fp);
            save_convolutional_weights(*(l.output_layer), fp);
        } ఒకవేళ(l.type == LOCAL){
#నిర్వచితమైతే GPU
            ఒకవేళ(gpu_సూచీ >= 0){
                pull_local_layer(l);
            }
#అయితే_ముగింపు
            పూర్ణాంకం locations = l.బహిర్గతం_w*l.బహిర్గతం_h;
            పూర్ణాంకం కొలత = l.కొలత*l.కొలత*l.c*l.n*locations;
            fwrite(l.biases, పరిమాణం(దశాంశం), l.outputs, fp);
            fwrite(l.weights, పరిమాణం(దశాంశం), కొలత, fp);
        }
    }
    fclose(fp);
}
శూన్య save_weights(జాలం *net, అక్షరం *filename)
{
    save_weights_upto(net, filename, net->n);
}

శూన్య transpose_మాతృక(దశాంశం *a, పూర్ణాంకం అడ్డువరుసలు, పూర్ణాంకం నిలువుల_సంఖ్య)
{
    దశాంశం *transpose = సున్నా_జ్ఞాపకం_ఇవ్వు(అడ్డువరుసలు*నిలువుల_సంఖ్య, పరిమాణం(దశాంశం));
    పూర్ణాంకం x, y;
    కొరకు(x = 0; x < అడ్డువరుసలు; ++x){
        కొరకు(y = 0; y < నిలువుల_సంఖ్య; ++y){
            transpose[y*అడ్డువరుసలు + x] = a[x*నిలువుల_సంఖ్య + y];
        }
    }
    జ్ఞాపకం_నకలు(a, transpose, అడ్డువరుసలు*నిలువుల_సంఖ్య*పరిమాణం(దశాంశం));
    విడుదల(transpose);
}

శూన్య ఎక్కించు_connected_weights(layer l, FILE *fp, పూర్ణాంకం transpose)
{
    fread(l.biases, పరిమాణం(దశాంశం), l.outputs, fp);
    fread(l.weights, పరిమాణం(దశాంశం), l.outputs*l.inputs, fp);
    ఒకవేళ(transpose){
        transpose_మాతృక(l.weights, l.inputs, l.outputs);
    }
    //printf("Biases: %f mean %f variance\n", mean_array(l.biases, l.outputs), variance_array(l.biases, l.outputs));
    //printf("Weights: %f mean %f variance\n", mean_array(l.weights, l.outputs*l.inputs), variance_array(l.weights, l.outputs*l.inputs));
    ఒకవేళ (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, పరిమాణం(దశాంశం), l.outputs, fp);
        fread(l.rolling_సగటు, పరిమాణం(దశాంశం), l.outputs, fp);
        fread(l.rolling_variance, పరిమాణం(దశాంశం), l.outputs, fp);
        //printf("Scales: %f mean %f variance\n", mean_array(l.scales, l.outputs), variance_array(l.scales, l.outputs));
        //printf("rolling_mean: %f mean %f variance\n", mean_array(l.rolling_mean, l.outputs), variance_array(l.rolling_mean, l.outputs));
        //printf("rolling_variance: %f mean %f variance\n", mean_array(l.rolling_variance, l.outputs), variance_array(l.rolling_variance, l.outputs));
    }
#నిర్వచితమైతే GPU
    ఒకవేళ(gpu_సూచీ >= 0){
        నెట్టు_connected_layer(l);
    }
#అయితే_ముగింపు
}

శూన్య ఎక్కించు_batchnorm_weights(layer l, FILE *fp)
{
    fread(l.scales, పరిమాణం(దశాంశం), l.c, fp);
    fread(l.rolling_సగటు, పరిమాణం(దశాంశం), l.c, fp);
    fread(l.rolling_variance, పరిమాణం(దశాంశం), l.c, fp);
#నిర్వచితమైతే GPU
    ఒకవేళ(gpu_సూచీ >= 0){
        నెట్టు_batchnorm_layer(l);
    }
#అయితే_ముగింపు
}

శూన్య ఎక్కించు_convolutional_weights_ద్విమాన(layer l, FILE *fp)
{
    fread(l.biases, పరిమాణం(దశాంశం), l.n, fp);
    ఒకవేళ (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, పరిమాణం(దశాంశం), l.n, fp);
        fread(l.rolling_సగటు, పరిమాణం(దశాంశం), l.n, fp);
        fread(l.rolling_variance, పరిమాణం(దశాంశం), l.n, fp);
    }
    పూర్ణాంకం కొలత = l.c*l.కొలత*l.కొలత;
    పూర్ణాంకం i, j, k;
    కొరకు(i = 0; i < l.n; ++i){
        దశాంశం సగటు = 0;
        fread(&సగటు, పరిమాణం(దశాంశం), 1, fp);
        కొరకు(j = 0; j < కొలత/8; ++j){
            పూర్ణాంకం సూచీ = i*కొలత + j*8;
            గుర్తులేని అక్షరం c = 0;
            fread(&c, పరిమాణం(అక్షరం), 1, fp);
            కొరకు(k = 0; k < 8; ++k){
                ఒకవేళ (j*8 + k >= కొలత) ఆపు;
                l.weights[సూచీ + k] = (c & 1<<k) ? సగటు : -సగటు;
            }
        }
    }
#నిర్వచితమైతే GPU
    ఒకవేళ(gpu_సూచీ >= 0){
        నెట్టు_convolutional_layer(l);
    }
#అయితే_ముగింపు
}

శూన్య ఎక్కించు_convolutional_weights(layer l, FILE *fp)
{
    ఒకవేళ(l.ద్విమాన){
        //load_convolutional_weights_binary(l, fp);
        //return;
    }
    ఒకవేళ(l.numload) l.n = l.numload;
    పూర్ణాంకం అంకె = l.c/l.groups*l.n*l.కొలత*l.కొలత;
    fread(l.biases, పరిమాణం(దశాంశం), l.n, fp);
    ఒకవేళ (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, పరిమాణం(దశాంశం), l.n, fp);
        fread(l.rolling_సగటు, పరిమాణం(దశాంశం), l.n, fp);
        fread(l.rolling_variance, పరిమాణం(దశాంశం), l.n, fp);
        ఒకవేళ(0){
            పూర్ణాంకం i;
            కొరకు(i = 0; i < l.n; ++i){
                రాయి("%g, ", l.rolling_సగటు[i]);
            }
            రాయి("\n");
            కొరకు(i = 0; i < l.n; ++i){
                రాయి("%g, ", l.rolling_variance[i]);
            }
            రాయి("\n");
        }
        ఒకవేళ(0){
            fill_cpu(l.n, 0, l.rolling_సగటు, 1);
            fill_cpu(l.n, 0, l.rolling_variance, 1);
        }
        ఒకవేళ(0){
            పూర్ణాంకం i;
            కొరకు(i = 0; i < l.n; ++i){
                రాయి("%g, ", l.rolling_సగటు[i]);
            }
            రాయి("\n");
            కొరకు(i = 0; i < l.n; ++i){
                రాయి("%g, ", l.rolling_variance[i]);
            }
            రాయి("\n");
        }
    }
    fread(l.weights, పరిమాణం(దశాంశం), అంకె, fp);
    //if(l.c == 3) scal_cpu(num, 1./256, l.weights, 1);
    ఒకవేళ (l.flipped) {
        transpose_మాతృక(l.weights, l.c*l.కొలత*l.కొలత, l.n);
    }
    //if (l.binary) binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.weights);
#నిర్వచితమైతే GPU
    ఒకవేళ(gpu_సూచీ >= 0){
        నెట్టు_convolutional_layer(l);
    }
#అయితే_ముగింపు
}


శూన్య ఎక్కించు_weights_upto(జాలం *net, అక్షరం *filename, పూర్ణాంకం ప్రారంభం, పూర్ణాంకం cutoff)
{
#నిర్వచితమైతే GPU
    ఒకవేళ(net->gpu_సూచీ >= 0){
        cuda_సమితి_device(net->gpu_సూచీ);
    }
#అయితే_ముగింపు
    fprintf(stderr, "Loading weights from %s...", filename);
    fflush(stdout);
    FILE *fp = fopen(filename, "rb");
    ఒకవేళ(!fp) file_error(filename);

    పూర్ణాంకం major;
    పూర్ణాంకం minor;
    పూర్ణాంకం revision;
    fread(&major, పరిమాణం(పూర్ణాంకం), 1, fp);
    fread(&minor, పరిమాణం(పూర్ణాంకం), 1, fp);
    fread(&revision, పరిమాణం(పూర్ణాంకం), 1, fp);
    ఒకవేళ ((major*10 + minor) >= 2 && major < 1000 && minor < 1000){
        fread(net->చూసిన, పరిమాణం(size_t), 1, fp);
    } లేకపోతే {
        పూర్ణాంకం iseen = 0;
        fread(&iseen, పరిమాణం(పూర్ణాంకం), 1, fp);
        *net->చూసిన = iseen;
    }
    పూర్ణాంకం transpose = (major > 1000) || (minor > 1000);

    పూర్ణాంకం i;
    కొరకు(i = ప్రారంభం; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        ఒకవేళ (l.dontload) కొనసాగు;
        ఒకవేళ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            ఎక్కించు_convolutional_weights(l, fp);
        }
        ఒకవేళ(l.type == CONNECTED){
            ఎక్కించు_connected_weights(l, fp, transpose);
        }
        ఒకవేళ(l.type == BATCHNORM){
            ఎక్కించు_batchnorm_weights(l, fp);
        }
        ఒకవేళ(l.type == CRNN){
            ఎక్కించు_convolutional_weights(*(l.ఆదానం_layer), fp);
            ఎక్కించు_convolutional_weights(*(l.self_layer), fp);
            ఎక్కించు_convolutional_weights(*(l.output_layer), fp);
        }
        ఒకవేళ(l.type == RNN){
            ఎక్కించు_connected_weights(*(l.ఆదానం_layer), fp, transpose);
            ఎక్కించు_connected_weights(*(l.self_layer), fp, transpose);
            ఎక్కించు_connected_weights(*(l.output_layer), fp, transpose);
        }
        ఒకవేళ (l.type == LSTM) {
            ఎక్కించు_connected_weights(*(l.wi), fp, transpose);
            ఎక్కించు_connected_weights(*(l.wf), fp, transpose);
            ఎక్కించు_connected_weights(*(l.wo), fp, transpose);
            ఎక్కించు_connected_weights(*(l.wg), fp, transpose);
            ఎక్కించు_connected_weights(*(l.ui), fp, transpose);
            ఎక్కించు_connected_weights(*(l.uf), fp, transpose);
            ఎక్కించు_connected_weights(*(l.uo), fp, transpose);
            ఎక్కించు_connected_weights(*(l.ug), fp, transpose);
        }
        ఒకవేళ (l.type == GRU) {
            ఒకవేళ(1){
                ఎక్కించు_connected_weights(*(l.wz), fp, transpose);
                ఎక్కించు_connected_weights(*(l.wr), fp, transpose);
                ఎక్కించు_connected_weights(*(l.wh), fp, transpose);
                ఎక్కించు_connected_weights(*(l.uz), fp, transpose);
                ఎక్కించు_connected_weights(*(l.ur), fp, transpose);
                ఎక్కించు_connected_weights(*(l.uh), fp, transpose);
            }లేకపోతే{
                ఎక్కించు_connected_weights(*(l.reset_layer), fp, transpose);
                ఎక్కించు_connected_weights(*(l.update_layer), fp, transpose);
                ఎక్కించు_connected_weights(*(l.state_layer), fp, transpose);
            }
        }
        ఒకవేళ(l.type == LOCAL){
            పూర్ణాంకం locations = l.బహిర్గతం_w*l.బహిర్గతం_h;
            పూర్ణాంకం కొలత = l.కొలత*l.కొలత*l.c*l.n*locations;
            fread(l.biases, పరిమాణం(దశాంశం), l.outputs, fp);
            fread(l.weights, పరిమాణం(దశాంశం), కొలత, fp);
#నిర్వచితమైతే GPU
            ఒకవేళ(gpu_సూచీ >= 0){
                నెట్టు_local_layer(l);
            }
#అయితే_ముగింపు
        }
    }
    fprintf(stderr, "Done!\n");
    fclose(fp);
}

శూన్య ఎక్కించు_weights(జాలం *net, అక్షరం *filename)
{
    ఎక్కించు_weights_upto(net, filename, 0, net->n);
}


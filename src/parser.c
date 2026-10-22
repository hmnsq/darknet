#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <assert.h>

#അന്തര്ഭാവ "activation_layer.h"
#അന്തര്ഭാവ "logistic_layer.h"
#അന്തര്ഭാവ "l2norm_layer.h"
#അന്തര്ഭാവ "activations.h"
#അന്തര്ഭാവ "avgpool_layer.h"
#അന്തര്ഭാവ "batchnorm_layer.h"
#അന്തര്ഭാവ "blas.h"
#അന്തര്ഭാവ "connected_layer.h"
#അന്തര്ഭാവ "deconvolutional_layer.h"
#അന്തര്ഭാവ "convolutional_layer.h"
#അന്തര്ഭാവ "cost_layer.h"
#അന്തര്ഭാവ "crnn_layer.h"
#അന്തര്ഭാവ "crop_layer.h"
#അന്തര്ഭാവ "detection_layer.h"
#അന്തര്ഭാവ "dropout_layer.h"
#അന്തര്ഭാവ "gru_layer.h"
#അന്തര്ഭാവ "list.h"
#അന്തര്ഭാവ "local_layer.h"
#അന്തര്ഭാവ "maxpool_layer.h"
#അന്തര്ഭാവ "normalization_layer.h"
#അന്തര്ഭാവ "option_list.h"
#അന്തര്ഭാവ "parser.h"
#അന്തര്ഭാവ "region_layer.h"
#അന്തര്ഭാവ "yolo_layer.h"
#അന്തര്ഭാവ "iseg_layer.h"
#അന്തര്ഭാവ "reorg_layer.h"
#അന്തര്ഭാവ "rnn_layer.h"
#അന്തര്ഭാവ "route_layer.h"
#അന്തര്ഭാവ "upsample_layer.h"
#അന്തര്ഭാവ "shortcut_layer.h"
#അന്തര്ഭാവ "softmax_layer.h"
#അന്തര്ഭാവ "lstm_layer.h"
#അന്തര്ഭാവ "utils.h"

തരനിർവചനം ഘടന{
    അക്ഷരം *type;
    list *options;
}section;

list *read_cfg(അക്ഷരം *filename);

LAYER_TYPE string_to_layer_type(അക്ഷരം * type)
{

    എങ്കിൽ (strcmp(type, "[shortcut]")==0) തിരികെ SHORTCUT;
    എങ്കിൽ (strcmp(type, "[crop]")==0) തിരികെ CROP;
    എങ്കിൽ (strcmp(type, "[cost]")==0) തിരികെ COST;
    എങ്കിൽ (strcmp(type, "[detection]")==0) തിരികെ DETECTION;
    എങ്കിൽ (strcmp(type, "[region]")==0) തിരികെ REGION;
    എങ്കിൽ (strcmp(type, "[yolo]")==0) തിരികെ YOLO;
    എങ്കിൽ (strcmp(type, "[iseg]")==0) തിരികെ ISEG;
    എങ്കിൽ (strcmp(type, "[local]")==0) തിരികെ LOCAL;
    എങ്കിൽ (strcmp(type, "[conv]")==0
            || strcmp(type, "[convolutional]")==0) തിരികെ CONVOLUTIONAL;
    എങ്കിൽ (strcmp(type, "[deconv]")==0
            || strcmp(type, "[deconvolutional]")==0) തിരികെ DECONVOLUTIONAL;
    എങ്കിൽ (strcmp(type, "[activation]")==0) തിരികെ ACTIVE;
    എങ്കിൽ (strcmp(type, "[logistic]")==0) തിരികെ LOGXENT;
    എങ്കിൽ (strcmp(type, "[l2norm]")==0) തിരികെ L2NORM;
    എങ്കിൽ (strcmp(type, "[net]")==0
            || strcmp(type, "[network]")==0) തിരികെ NETWORK;
    എങ്കിൽ (strcmp(type, "[crnn]")==0) തിരികെ CRNN;
    എങ്കിൽ (strcmp(type, "[gru]")==0) തിരികെ GRU;
    എങ്കിൽ (strcmp(type, "[lstm]") == 0) തിരികെ LSTM;
    എങ്കിൽ (strcmp(type, "[rnn]")==0) തിരികെ RNN;
    എങ്കിൽ (strcmp(type, "[conn]")==0
            || strcmp(type, "[connected]")==0) തിരികെ CONNECTED;
    എങ്കിൽ (strcmp(type, "[max]")==0
            || strcmp(type, "[maxpool]")==0) തിരികെ MAXPOOL;
    എങ്കിൽ (strcmp(type, "[reorg]")==0) തിരികെ REORG;
    എങ്കിൽ (strcmp(type, "[avg]")==0
            || strcmp(type, "[avgpool]")==0) തിരികെ AVGPOOL;
    എങ്കിൽ (strcmp(type, "[dropout]")==0) തിരികെ DROPOUT;
    എങ്കിൽ (strcmp(type, "[lrn]")==0
            || strcmp(type, "[normalization]")==0) തിരികെ NORMALIZATION;
    എങ്കിൽ (strcmp(type, "[batchnorm]")==0) തിരികെ BATCHNORM;
    എങ്കിൽ (strcmp(type, "[soft]")==0
            || strcmp(type, "[softmax]")==0) തിരികെ SOFTMAX;
    എങ്കിൽ (strcmp(type, "[route]")==0) തിരികെ ROUTE;
    എങ്കിൽ (strcmp(type, "[upsample]")==0) തിരികെ UPSAMPLE;
    തിരികെ BLANK;
}

ശൂന്യം free_section(section *s)
{
    സ്വതന്ത്രം(s->type);
    കണ്ണി *n = s->options->front;
    ആകുമ്പോൾ(n){
        kvp *pair = (kvp *)n->val;
        സ്വതന്ത്രം(pair->താക്കോൽ);
        സ്വതന്ത്രം(pair);
        കണ്ണി *next = n->next;
        സ്വതന്ത്രം(n);
        n = next;
    }
    സ്വതന്ത്രം(s->options);
    സ്വതന്ത്രം(s);
}

ശൂന്യം parse_data(അക്ഷരം *data, ദശാംശം *a, പൂർണ്ണസംഖ്യ n)
{
    പൂർണ്ണസംഖ്യ i;
    എങ്കിൽ(!data) തിരികെ;
    അക്ഷരം *curr = data;
    അക്ഷരം *next = data;
    പൂർണ്ണസംഖ്യ done = 0;
    വേണ്ടി(i = 0; i < n && !done; ++i){
        ആകുമ്പോൾ(*++next !='\0' && *next != ',');
        എങ്കിൽ(*next == '\0') done = 1;
        *next = '\0';
        sscanf(curr, "%g", &a[i]);
        curr = next+1;
    }
}

തരനിർവചനം ഘടന size_params{
    പൂർണ്ണസംഖ്യ batch;
    പൂർണ്ണസംഖ്യ inputs;
    പൂർണ്ണസംഖ്യ h;
    പൂർണ്ണസംഖ്യ w;
    പൂർണ്ണസംഖ്യ c;
    പൂർണ്ണസംഖ്യ index;
    പൂർണ്ണസംഖ്യ time_steps;
    network *net;
} size_params;

local_layer parse_local(list *options, size_params params)
{
    പൂർണ്ണസംഖ്യ n = option_find_int(options, "filters",1);
    പൂർണ്ണസംഖ്യ size = option_find_int(options, "size",1);
    പൂർണ്ണസംഖ്യ stride = option_find_int(options, "stride",1);
    പൂർണ്ണസംഖ്യ pad = option_find_int(options, "pad",0);
    അക്ഷരം *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    പൂർണ്ണസംഖ്യ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    എങ്കിൽ(!(h && w && c)) error("Layer before local layer must output image.");

    local_layer layer = make_local_layer(batch,h,w,c,n,size,stride,pad,activation);

    തിരികെ layer;
}

layer parse_deconvolutional(list *options, size_params params)
{
    പൂർണ്ണസംഖ്യ n = option_find_int(options, "filters",1);
    പൂർണ്ണസംഖ്യ size = option_find_int(options, "size",1);
    പൂർണ്ണസംഖ്യ stride = option_find_int(options, "stride",1);

    അക്ഷരം *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    പൂർണ്ണസംഖ്യ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    എങ്കിൽ(!(h && w && c)) error("Layer before deconvolutional layer must output image.");
    പൂർണ്ണസംഖ്യ batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);
    പൂർണ്ണസംഖ്യ pad = option_find_int_quiet(options, "pad",0);
    പൂർണ്ണസംഖ്യ padding = option_find_int_quiet(options, "padding",0);
    എങ്കിൽ(pad) padding = size/2;

    layer l = make_deconvolutional_layer(batch,h,w,c,n,size,stride,padding, activation, batch_normalize, params.net->adam);

    തിരികെ l;
}


convolutional_layer parse_convolutional(list *options, size_params params)
{
    പൂർണ്ണസംഖ്യ n = option_find_int(options, "filters",1);
    പൂർണ്ണസംഖ്യ size = option_find_int(options, "size",1);
    പൂർണ്ണസംഖ്യ stride = option_find_int(options, "stride",1);
    പൂർണ്ണസംഖ്യ pad = option_find_int_quiet(options, "pad",0);
    പൂർണ്ണസംഖ്യ padding = option_find_int_quiet(options, "padding",0);
    പൂർണ്ണസംഖ്യ groups = option_find_int_quiet(options, "groups", 1);
    എങ്കിൽ(pad) padding = size/2;

    അക്ഷരം *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    പൂർണ്ണസംഖ്യ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    എങ്കിൽ(!(h && w && c)) error("Layer before convolutional layer must output image.");
    പൂർണ്ണസംഖ്യ batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);
    പൂർണ്ണസംഖ്യ ദ്വിമാന = option_find_int_quiet(options, "binary", 0);
    പൂർണ്ണസംഖ്യ xnor = option_find_int_quiet(options, "xnor", 0);

    convolutional_layer layer = make_convolutional_layer(batch,h,w,c,n,groups,size,stride,padding,activation, batch_normalize, ദ്വിമാന, xnor, params.net->adam);
    layer.flipped = option_find_int_quiet(options, "flipped", 0);
    layer.dot = option_find_float_quiet(options, "dot", 0);

    തിരികെ layer;
}

layer parse_crnn(list *options, size_params params)
{
    പൂർണ്ണസംഖ്യ output_filters = option_find_int(options, "output_filters",1);
    പൂർണ്ണസംഖ്യ hidden_filters = option_find_int(options, "hidden_filters",1);
    അക്ഷരം *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    പൂർണ്ണസംഖ്യ batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_crnn_layer(params.batch, params.w, params.h, params.c, hidden_filters, output_filters, params.time_steps, activation, batch_normalize);

    l.shortcut = option_find_int_quiet(options, "shortcut", 0);

    തിരികെ l;
}

layer parse_rnn(list *options, size_params params)
{
    പൂർണ്ണസംഖ്യ output = option_find_int(options, "output",1);
    അക്ഷരം *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    പൂർണ്ണസംഖ്യ batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_rnn_layer(params.batch, params.inputs, output, params.time_steps, activation, batch_normalize, params.net->adam);

    l.shortcut = option_find_int_quiet(options, "shortcut", 0);

    തിരികെ l;
}

layer parse_gru(list *options, size_params params)
{
    പൂർണ്ണസംഖ്യ output = option_find_int(options, "output",1);
    പൂർണ്ണസംഖ്യ batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_gru_layer(params.batch, params.inputs, output, params.time_steps, batch_normalize, params.net->adam);
    l.tanh = option_find_int_quiet(options, "tanh", 0);

    തിരികെ l;
}

layer parse_lstm(list *options, size_params params)
{
    പൂർണ്ണസംഖ്യ output = option_find_int(options, "output", 1);
    പൂർണ്ണസംഖ്യ batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_lstm_layer(params.batch, params.inputs, output, params.time_steps, batch_normalize, params.net->adam);

    തിരികെ l;
}

layer parse_connected(list *options, size_params params)
{
    പൂർണ്ണസംഖ്യ output = option_find_int(options, "output",1);
    അക്ഷരം *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    പൂർണ്ണസംഖ്യ batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_connected_layer(params.batch, params.inputs, output, activation, batch_normalize, params.net->adam);
    തിരികെ l;
}

layer parse_softmax(list *options, size_params params)
{
    പൂർണ്ണസംഖ്യ groups = option_find_int_quiet(options, "groups",1);
    layer l = make_softmax_layer(params.batch, params.inputs, groups);
    l.temperature = option_find_float_quiet(options, "temperature", 1);
    അക്ഷരം *വൃക്ഷം_file = option_find_str(options, "tree", 0);
    എങ്കിൽ (വൃക്ഷം_file) l.softmax_വൃക്ഷം = read_വൃക്ഷം(വൃക്ഷം_file);
    l.w = params.w;
    l.h = params.h;
    l.c = params.c;
    l.spatial = option_find_float_quiet(options, "spatial", 0);
    l.noloss =  option_find_int_quiet(options, "noloss", 0);
    തിരികെ l;
}

പൂർണ്ണസംഖ്യ *parse_yolo_mask(അക്ഷരം *a, പൂർണ്ണസംഖ്യ *അക്കം)
{
    പൂർണ്ണസംഖ്യ *mask = 0;
    എങ്കിൽ(a){
        പൂർണ്ണസംഖ്യ len = strlen(a);
        പൂർണ്ണസംഖ്യ n = 1;
        പൂർണ്ണസംഖ്യ i;
        വേണ്ടി(i = 0; i < len; ++i){
            എങ്കിൽ (a[i] == ',') ++n;
        }
        mask = calloc(n, വലുപ്പം(പൂർണ്ണസംഖ്യ));
        വേണ്ടി(i = 0; i < n; ++i){
            പൂർണ്ണസംഖ്യ val = atoi(a);
            mask[i] = val;
            a = strchr(a, ',')+1;
        }
        *അക്കം = n;
    }
    തിരികെ mask;
}

layer parse_yolo(list *options, size_params params)
{
    പൂർണ്ണസംഖ്യ classes = option_find_int(options, "classes", 20);
    പൂർണ്ണസംഖ്യ total = option_find_int(options, "num", 1);
    പൂർണ്ണസംഖ്യ അക്കം = total;

    അക്ഷരം *a = option_find_str(options, "mask", 0);
    പൂർണ്ണസംഖ്യ *mask = parse_yolo_mask(a, &അക്കം);
    layer l = make_yolo_layer(params.batch, params.w, params.h, അക്കം, total, mask, classes);
    പ്രതിജ്ഞാ(l.outputs == params.inputs);

    l.max_boxes = option_find_int_quiet(options, "max",90);
    l.jitter = option_find_float(options, "jitter", .2);

    l.ignore_thresh = option_find_float(options, "ignore_thresh", .5);
    l.truth_thresh = option_find_float(options, "truth_thresh", 1);
    l.random = option_find_int_quiet(options, "random", 0);

    അക്ഷരം *map_file = option_find_str(options, "map", 0);
    എങ്കിൽ (map_file) l.map = read_map(map_file);

    a = option_find_str(options, "anchors", 0);
    എങ്കിൽ(a){
        പൂർണ്ണസംഖ്യ len = strlen(a);
        പൂർണ്ണസംഖ്യ n = 1;
        പൂർണ്ണസംഖ്യ i;
        വേണ്ടി(i = 0; i < len; ++i){
            എങ്കിൽ (a[i] == ',') ++n;
        }
        വേണ്ടി(i = 0; i < n; ++i){
            ദശാംശം bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    തിരികെ l;
}

layer parse_iseg(list *options, size_params params)
{
    പൂർണ്ണസംഖ്യ classes = option_find_int(options, "classes", 20);
    പൂർണ്ണസംഖ്യ ids = option_find_int(options, "ids", 32);
    layer l = make_iseg_layer(params.batch, params.w, params.h, classes, ids);
    പ്രതിജ്ഞാ(l.outputs == params.inputs);
    തിരികെ l;
}

layer parse_region(list *options, size_params params)
{
    പൂർണ്ണസംഖ്യ coords = option_find_int(options, "coords", 4);
    പൂർണ്ണസംഖ്യ classes = option_find_int(options, "classes", 20);
    പൂർണ്ണസംഖ്യ അക്കം = option_find_int(options, "num", 1);

    layer l = make_region_layer(params.batch, params.w, params.h, അക്കം, classes, coords);
    പ്രതിജ്ഞാ(l.outputs == params.inputs);

    l.log = option_find_int_quiet(options, "log", 0);
    l.sqrt = option_find_int_quiet(options, "sqrt", 0);

    l.softmax = option_find_int(options, "softmax", 0);
    l.background = option_find_int_quiet(options, "background", 0);
    l.max_boxes = option_find_int_quiet(options, "max",30);
    l.jitter = option_find_float(options, "jitter", .2);
    l.rescore = option_find_int_quiet(options, "rescore",0);

    l.thresh = option_find_float(options, "thresh", .5);
    l.classfix = option_find_int_quiet(options, "classfix", 0);
    l.absolute = option_find_int_quiet(options, "absolute", 0);
    l.random = option_find_int_quiet(options, "random", 0);

    l.coord_scale = option_find_float(options, "coord_scale", 1);
    l.object_scale = option_find_float(options, "object_scale", 1);
    l.noobject_scale = option_find_float(options, "noobject_scale", 1);
    l.mask_scale = option_find_float(options, "mask_scale", 1);
    l.class_scale = option_find_float(options, "class_scale", 1);
    l.bias_match = option_find_int_quiet(options, "bias_match",0);

    അക്ഷരം *വൃക്ഷം_file = option_find_str(options, "tree", 0);
    എങ്കിൽ (വൃക്ഷം_file) l.softmax_വൃക്ഷം = read_വൃക്ഷം(വൃക്ഷം_file);
    അക്ഷരം *map_file = option_find_str(options, "map", 0);
    എങ്കിൽ (map_file) l.map = read_map(map_file);

    അക്ഷരം *a = option_find_str(options, "anchors", 0);
    എങ്കിൽ(a){
        പൂർണ്ണസംഖ്യ len = strlen(a);
        പൂർണ്ണസംഖ്യ n = 1;
        പൂർണ്ണസംഖ്യ i;
        വേണ്ടി(i = 0; i < len; ++i){
            എങ്കിൽ (a[i] == ',') ++n;
        }
        വേണ്ടി(i = 0; i < n; ++i){
            ദശാംശം bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    തിരികെ l;
}

detection_layer parse_detection(list *options, size_params params)
{
    പൂർണ്ണസംഖ്യ coords = option_find_int(options, "coords", 1);
    പൂർണ്ണസംഖ്യ classes = option_find_int(options, "classes", 1);
    പൂർണ്ണസംഖ്യ rescore = option_find_int(options, "rescore", 0);
    പൂർണ്ണസംഖ്യ അക്കം = option_find_int(options, "num", 1);
    പൂർണ്ണസംഖ്യ side = option_find_int(options, "side", 7);
    detection_layer layer = make_detection_layer(params.batch, params.inputs, അക്കം, side, classes, coords, rescore);

    layer.softmax = option_find_int(options, "softmax", 0);
    layer.sqrt = option_find_int(options, "sqrt", 0);

    layer.max_boxes = option_find_int_quiet(options, "max",90);
    layer.coord_scale = option_find_float(options, "coord_scale", 1);
    layer.forced = option_find_int(options, "forced", 0);
    layer.object_scale = option_find_float(options, "object_scale", 1);
    layer.noobject_scale = option_find_float(options, "noobject_scale", 1);
    layer.class_scale = option_find_float(options, "class_scale", 1);
    layer.jitter = option_find_float(options, "jitter", .2);
    layer.random = option_find_int_quiet(options, "random", 0);
    layer.reorg = option_find_int_quiet(options, "reorg", 0);
    തിരികെ layer;
}

cost_layer parse_cost(list *options, size_params params)
{
    അക്ഷരം *type_s = option_find_str(options, "type", "sse");
    COST_TYPE type = get_cost_type(type_s);
    ദശാംശം scale = option_find_float_quiet(options, "scale",1);
    cost_layer layer = make_cost_layer(params.batch, params.inputs, type, scale);
    layer.ratio =  option_find_float_quiet(options, "ratio",0);
    layer.noobject_scale =  option_find_float_quiet(options, "noobj", 1);
    layer.thresh =  option_find_float_quiet(options, "thresh",0);
    തിരികെ layer;
}

crop_layer parse_crop(list *options, size_params params)
{
    പൂർണ്ണസംഖ്യ crop_height = option_find_int(options, "crop_height",1);
    പൂർണ്ണസംഖ്യ crop_width = option_find_int(options, "crop_width",1);
    പൂർണ്ണസംഖ്യ flip = option_find_int(options, "flip",0);
    ദശാംശം angle = option_find_float(options, "angle",0);
    ദശാംശം saturation = option_find_float(options, "saturation",1);
    ദശാംശം exposure = option_find_float(options, "exposure",1);

    പൂർണ്ണസംഖ്യ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    എങ്കിൽ(!(h && w && c)) error("Layer before crop layer must output image.");

    പൂർണ്ണസംഖ്യ noadjust = option_find_int_quiet(options, "noadjust",0);

    crop_layer l = make_crop_layer(batch,h,w,c,crop_height,crop_width,flip, angle, saturation, exposure);
    l.shift = option_find_float(options, "shift", 0);
    l.noadjust = noadjust;
    തിരികെ l;
}

layer parse_reorg(list *options, size_params params)
{
    പൂർണ്ണസംഖ്യ stride = option_find_int(options, "stride",1);
    പൂർണ്ണസംഖ്യ reverse = option_find_int_quiet(options, "reverse",0);
    പൂർണ്ണസംഖ്യ flatten = option_find_int_quiet(options, "flatten",0);
    പൂർണ്ണസംഖ്യ extra = option_find_int_quiet(options, "extra",0);

    പൂർണ്ണസംഖ്യ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    എങ്കിൽ(!(h && w && c)) error("Layer before reorg layer must output image.");

    layer layer = make_reorg_layer(batch,w,h,c,stride,reverse, flatten, extra);
    തിരികെ layer;
}

maxpool_layer parse_maxpool(list *options, size_params params)
{
    പൂർണ്ണസംഖ്യ stride = option_find_int(options, "stride",1);
    പൂർണ്ണസംഖ്യ size = option_find_int(options, "size",stride);
    പൂർണ്ണസംഖ്യ padding = option_find_int_quiet(options, "padding", size-1);

    പൂർണ്ണസംഖ്യ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    എങ്കിൽ(!(h && w && c)) error("Layer before maxpool layer must output image.");

    maxpool_layer layer = make_maxpool_layer(batch,h,w,c,size,stride,padding);
    തിരികെ layer;
}

avgpool_layer parse_avgpool(list *options, size_params params)
{
    പൂർണ്ണസംഖ്യ batch,w,h,c;
    w = params.w;
    h = params.h;
    c = params.c;
    batch=params.batch;
    എങ്കിൽ(!(h && w && c)) error("Layer before avgpool layer must output image.");

    avgpool_layer layer = make_avgpool_layer(batch,w,h,c);
    തിരികെ layer;
}

dropout_layer parse_dropout(list *options, size_params params)
{
    ദശാംശം probability = option_find_float(options, "probability", .5);
    dropout_layer layer = make_dropout_layer(params.batch, params.inputs, probability);
    layer.out_w = params.w;
    layer.out_h = params.h;
    layer.out_c = params.c;
    തിരികെ layer;
}

layer parse_normalization(list *options, size_params params)
{
    ദശാംശം alpha = option_find_float(options, "alpha", .0001);
    ദശാംശം beta =  option_find_float(options, "beta" , .75);
    ദശാംശം kappa = option_find_float(options, "kappa", 1);
    പൂർണ്ണസംഖ്യ size = option_find_int(options, "size", 5);
    layer l = make_normalization_layer(params.batch, params.w, params.h, params.c, size, alpha, beta, kappa);
    തിരികെ l;
}

layer parse_batchnorm(list *options, size_params params)
{
    layer l = make_batchnorm_layer(params.batch, params.w, params.h, params.c);
    തിരികെ l;
}

layer parse_shortcut(list *options, size_params params, network *net)
{
    അക്ഷരം *l = option_find(options, "from");
    പൂർണ്ണസംഖ്യ index = atoi(l);
    എങ്കിൽ(index < 0) index = params.index + index;

    പൂർണ്ണസംഖ്യ batch = params.batch;
    layer from = net->layers[index];

    layer s = make_shortcut_layer(batch, index, params.w, params.h, params.c, from.out_w, from.out_h, from.out_c);

    അക്ഷരം *activation_s = option_find_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);
    s.activation = activation;
    s.alpha = option_find_float_quiet(options, "alpha", 1);
    s.beta = option_find_float_quiet(options, "beta", 1);
    തിരികെ s;
}


layer parse_l2norm(list *options, size_params params)
{
    layer l = make_l2norm_layer(params.batch, params.inputs);
    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;
    തിരികെ l;
}


layer parse_logistic(list *options, size_params params)
{
    layer l = make_logistic_layer(params.batch, params.inputs);
    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;
    തിരികെ l;
}

layer parse_activation(list *options, size_params params)
{
    അക്ഷരം *activation_s = option_find_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);

    layer l = make_activation_layer(params.batch, params.inputs, activation);

    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;

    തിരികെ l;
}

layer parse_upsample(list *options, size_params params, network *net)
{

    പൂർണ്ണസംഖ്യ stride = option_find_int(options, "stride",2);
    layer l = make_upsample_layer(params.batch, params.w, params.h, params.c, stride);
    l.scale = option_find_float_quiet(options, "scale", 1);
    തിരികെ l;
}

route_layer parse_route(list *options, size_params params, network *net)
{
    അക്ഷരം *l = option_find(options, "layers");
    പൂർണ്ണസംഖ്യ len = strlen(l);
    എങ്കിൽ(!l) error("Route Layer must specify input layers");
    പൂർണ്ണസംഖ്യ n = 1;
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < len; ++i){
        എങ്കിൽ (l[i] == ',') ++n;
    }

    പൂർണ്ണസംഖ്യ *layers = calloc(n, വലുപ്പം(പൂർണ്ണസംഖ്യ));
    പൂർണ്ണസംഖ്യ *sizes = calloc(n, വലുപ്പം(പൂർണ്ണസംഖ്യ));
    വേണ്ടി(i = 0; i < n; ++i){
        പൂർണ്ണസംഖ്യ index = atoi(l);
        l = strchr(l, ',')+1;
        എങ്കിൽ(index < 0) index = params.index + index;
        layers[i] = index;
        sizes[i] = net->layers[index].outputs;
    }
    പൂർണ്ണസംഖ്യ batch = params.batch;

    route_layer layer = make_route_layer(batch, n, layers, sizes);

    convolutional_layer first = net->layers[layers[0]];
    layer.out_w = first.out_w;
    layer.out_h = first.out_h;
    layer.out_c = first.out_c;
    വേണ്ടി(i = 1; i < n; ++i){
        പൂർണ്ണസംഖ്യ index = layers[i];
        convolutional_layer next = net->layers[index];
        എങ്കിൽ(next.out_w == first.out_w && next.out_h == first.out_h){
            layer.out_c += next.out_c;
        }അല്ലെങ്കിൽ{
            layer.out_h = layer.out_w = layer.out_c = 0;
        }
    }

    തിരികെ layer;
}

learning_rate_policy get_policy(അക്ഷരം *s)
{
    എങ്കിൽ (strcmp(s, "random")==0) തിരികെ RANDOM;
    എങ്കിൽ (strcmp(s, "poly")==0) തിരികെ POLY;
    എങ്കിൽ (strcmp(s, "constant")==0) തിരികെ CONSTANT;
    എങ്കിൽ (strcmp(s, "step")==0) തിരികെ STEP;
    എങ്കിൽ (strcmp(s, "exp")==0) തിരികെ EXP;
    എങ്കിൽ (strcmp(s, "sigmoid")==0) തിരികെ SIG;
    എങ്കിൽ (strcmp(s, "steps")==0) തിരികെ STEPS;
    fprintf(stderr, "Couldn't find policy %s, going with constant\n", s);
    തിരികെ CONSTANT;
}

ശൂന്യം parse_net_options(list *options, network *net)
{
    net->batch = option_find_int(options, "batch",1);
    net->learning_rate = option_find_float(options, "learning_rate", .001);
    net->momentum = option_find_float(options, "momentum", .9);
    net->decay = option_find_float(options, "decay", .0001);
    പൂർണ്ണസംഖ്യ subdivs = option_find_int(options, "subdivisions",1);
    net->time_steps = option_find_int_quiet(options, "time_steps",1);
    net->notruth = option_find_int_quiet(options, "notruth",0);
    net->batch /= subdivs;
    net->batch *= net->time_steps;
    net->subdivisions = subdivs;
    net->random = option_find_int_quiet(options, "random", 0);

    net->adam = option_find_int_quiet(options, "adam", 0);
    എങ്കിൽ(net->adam){
        net->B1 = option_find_float(options, "B1", .9);
        net->B2 = option_find_float(options, "B2", .999);
        net->eps = option_find_float(options, "eps", .0000001);
    }

    net->h = option_find_int_quiet(options, "height",0);
    net->w = option_find_int_quiet(options, "width",0);
    net->c = option_find_int_quiet(options, "channels",0);
    net->inputs = option_find_int_quiet(options, "inputs", net->h * net->w * net->c);
    net->max_crop = option_find_int_quiet(options, "max_crop",net->w*2);
    net->min_crop = option_find_int_quiet(options, "min_crop",net->w);
    net->max_ratio = option_find_float_quiet(options, "max_ratio", (ദശാംശം) net->max_crop / net->w);
    net->min_ratio = option_find_float_quiet(options, "min_ratio", (ദശാംശം) net->min_crop / net->w);
    net->center = option_find_int_quiet(options, "center",0);
    net->clip = option_find_float_quiet(options, "clip", 0);

    net->angle = option_find_float_quiet(options, "angle", 0);
    net->aspect = option_find_float_quiet(options, "aspect", 1);
    net->saturation = option_find_float_quiet(options, "saturation", 1);
    net->exposure = option_find_float_quiet(options, "exposure", 1);
    net->hue = option_find_float_quiet(options, "hue", 0);

    എങ്കിൽ(!net->inputs && !(net->h && net->w && net->c)) error("No input parameters supplied");

    അക്ഷരം *policy_s = option_find_str(options, "policy", "constant");
    net->policy = get_policy(policy_s);
    net->burn_in = option_find_int_quiet(options, "burn_in", 0);
    net->power = option_find_float_quiet(options, "power", 4);
    എങ്കിൽ(net->policy == STEP){
        net->step = option_find_int(options, "step", 1);
        net->scale = option_find_float(options, "scale", 1);
    } അല്ലെങ്കിൽ എങ്കിൽ (net->policy == STEPS){
        അക്ഷരം *l = option_find(options, "steps");
        അക്ഷരം *p = option_find(options, "scales");
        എങ്കിൽ(!l || !p) error("STEPS policy must have steps and scales in cfg file");

        പൂർണ്ണസംഖ്യ len = strlen(l);
        പൂർണ്ണസംഖ്യ n = 1;
        പൂർണ്ണസംഖ്യ i;
        വേണ്ടി(i = 0; i < len; ++i){
            എങ്കിൽ (l[i] == ',') ++n;
        }
        പൂർണ്ണസംഖ്യ *steps = calloc(n, വലുപ്പം(പൂർണ്ണസംഖ്യ));
        ദശാംശം *scales = calloc(n, വലുപ്പം(ദശാംശം));
        വേണ്ടി(i = 0; i < n; ++i){
            പൂർണ്ണസംഖ്യ step    = atoi(l);
            ദശാംശം scale = atof(p);
            l = strchr(l, ',')+1;
            p = strchr(p, ',')+1;
            steps[i] = step;
            scales[i] = scale;
        }
        net->scales = scales;
        net->steps = steps;
        net->അക്കം_steps = n;
    } അല്ലെങ്കിൽ എങ്കിൽ (net->policy == EXP){
        net->gamma = option_find_float(options, "gamma", 1);
    } അല്ലെങ്കിൽ എങ്കിൽ (net->policy == SIG){
        net->gamma = option_find_float(options, "gamma", 1);
        net->step = option_find_int(options, "step", 1);
    } അല്ലെങ്കിൽ എങ്കിൽ (net->policy == POLY || net->policy == RANDOM){
    }
    net->max_batches = option_find_int(options, "max_batches", 0);
}

പൂർണ്ണസംഖ്യ is_network(section *s)
{
    തിരികെ (strcmp(s->type, "[net]")==0
            || strcmp(s->type, "[network]")==0);
}

network *parse_network_cfg(അക്ഷരം *filename)
{
    list *sections = read_cfg(filename);
    കണ്ണി *n = sections->front;
    എങ്കിൽ(!n) error("Config file has no sections");
    network *net = make_network(sections->size - 1);
    net->gpu_index = gpu_index;
    size_params params;

    section *s = (section *)n->val;
    list *options = s->options;
    എങ്കിൽ(!is_network(s)) error("First section must be [net] or [network]");
    parse_net_options(options, net);

    params.h = net->h;
    params.w = net->w;
    params.c = net->c;
    params.inputs = net->inputs;
    params.batch = net->batch;
    params.time_steps = net->time_steps;
    params.net = net;

    size_t workspace_size = 0;
    n = n->next;
    പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    free_section(s);
    fprintf(stderr, "layer     filters    size              input                output\n");
    ആകുമ്പോൾ(n){
        params.index = എണ്ണം;
        fprintf(stderr, "%5d ", എണ്ണം);
        s = (section *)n->val;
        options = s->options;
        layer l = {0};
        LAYER_TYPE lt = string_to_layer_type(s->type);
        എങ്കിൽ(lt == CONVOLUTIONAL){
            l = parse_convolutional(options, params);
        }അല്ലെങ്കിൽ എങ്കിൽ(lt == DECONVOLUTIONAL){
            l = parse_deconvolutional(options, params);
        }അല്ലെങ്കിൽ എങ്കിൽ(lt == LOCAL){
            l = parse_local(options, params);
        }അല്ലെങ്കിൽ എങ്കിൽ(lt == ACTIVE){
            l = parse_activation(options, params);
        }അല്ലെങ്കിൽ എങ്കിൽ(lt == LOGXENT){
            l = parse_logistic(options, params);
        }അല്ലെങ്കിൽ എങ്കിൽ(lt == L2NORM){
            l = parse_l2norm(options, params);
        }അല്ലെങ്കിൽ എങ്കിൽ(lt == RNN){
            l = parse_rnn(options, params);
        }അല്ലെങ്കിൽ എങ്കിൽ(lt == GRU){
            l = parse_gru(options, params);
        }അല്ലെങ്കിൽ എങ്കിൽ (lt == LSTM) {
            l = parse_lstm(options, params);
        }അല്ലെങ്കിൽ എങ്കിൽ(lt == CRNN){
            l = parse_crnn(options, params);
        }അല്ലെങ്കിൽ എങ്കിൽ(lt == CONNECTED){
            l = parse_connected(options, params);
        }അല്ലെങ്കിൽ എങ്കിൽ(lt == CROP){
            l = parse_crop(options, params);
        }അല്ലെങ്കിൽ എങ്കിൽ(lt == COST){
            l = parse_cost(options, params);
        }അല്ലെങ്കിൽ എങ്കിൽ(lt == REGION){
            l = parse_region(options, params);
        }അല്ലെങ്കിൽ എങ്കിൽ(lt == YOLO){
            l = parse_yolo(options, params);
        }അല്ലെങ്കിൽ എങ്കിൽ(lt == ISEG){
            l = parse_iseg(options, params);
        }അല്ലെങ്കിൽ എങ്കിൽ(lt == DETECTION){
            l = parse_detection(options, params);
        }അല്ലെങ്കിൽ എങ്കിൽ(lt == SOFTMAX){
            l = parse_softmax(options, params);
            net->hierarchy = l.softmax_വൃക്ഷം;
        }അല്ലെങ്കിൽ എങ്കിൽ(lt == NORMALIZATION){
            l = parse_normalization(options, params);
        }അല്ലെങ്കിൽ എങ്കിൽ(lt == BATCHNORM){
            l = parse_batchnorm(options, params);
        }അല്ലെങ്കിൽ എങ്കിൽ(lt == MAXPOOL){
            l = parse_maxpool(options, params);
        }അല്ലെങ്കിൽ എങ്കിൽ(lt == REORG){
            l = parse_reorg(options, params);
        }അല്ലെങ്കിൽ എങ്കിൽ(lt == AVGPOOL){
            l = parse_avgpool(options, params);
        }അല്ലെങ്കിൽ എങ്കിൽ(lt == ROUTE){
            l = parse_route(options, params, net);
        }അല്ലെങ്കിൽ എങ്കിൽ(lt == UPSAMPLE){
            l = parse_upsample(options, params, net);
        }അല്ലെങ്കിൽ എങ്കിൽ(lt == SHORTCUT){
            l = parse_shortcut(options, params, net);
        }അല്ലെങ്കിൽ എങ്കിൽ(lt == DROPOUT){
            l = parse_dropout(options, params);
            l.output = net->layers[എണ്ണം-1].output;
            l.delta = net->layers[എണ്ണം-1].delta;
#ifdef GPU
            l.output_gpu = net->layers[എണ്ണം-1].output_gpu;
            l.delta_gpu = net->layers[എണ്ണം-1].delta_gpu;
#endif
        }അല്ലെങ്കിൽ{
            fprintf(stderr, "Type not recognized: %s\n", s->type);
        }
        l.clip = net->clip;
        l.truth = option_find_int_quiet(options, "truth", 0);
        l.onlyforward = option_find_int_quiet(options, "onlyforward", 0);
        l.stopbackward = option_find_int_quiet(options, "stopbackward", 0);
        l.dontsave = option_find_int_quiet(options, "dontsave", 0);
        l.dontload = option_find_int_quiet(options, "dontload", 0);
        l.numload = option_find_int_quiet(options, "numload", 0);
        l.dontloadscales = option_find_int_quiet(options, "dontloadscales", 0);
        l.learning_rate_scale = option_find_float_quiet(options, "learning_rate", 1);
        l.smooth = option_find_float_quiet(options, "smooth", 0);
        option_unused(options);
        net->layers[എണ്ണം] = l;
        എങ്കിൽ (l.workspace_size > workspace_size) workspace_size = l.workspace_size;
        free_section(s);
        n = n->next;
        ++എണ്ണം;
        എങ്കിൽ(n){
            params.h = l.out_h;
            params.w = l.out_w;
            params.c = l.out_c;
            params.inputs = l.outputs;
        }
    }
    free_list(sections);
    layer out = get_network_output_layer(net);
    net->outputs = out.outputs;
    net->truths = out.outputs;
    എങ്കിൽ(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    net->നിവേശം = calloc(net->inputs*net->batch, വലുപ്പം(ദശാംശം));
    net->truth = calloc(net->truths*net->batch, വലുപ്പം(ദശാംശം));
#ifdef GPU
    net->output_gpu = out.output_gpu;
    net->നിവേശം_gpu = cuda_make_array(net->നിവേശം, net->inputs*net->batch);
    net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
#endif
    എങ്കിൽ(workspace_size){
        //printf("%ld\n", workspace_size);
#ifdef GPU
        എങ്കിൽ(gpu_index >= 0){
            net->workspace = cuda_make_array(0, (workspace_size-1)/വലുപ്പം(ദശാംശം)+1);
        }അല്ലെങ്കിൽ {
            net->workspace = calloc(1, workspace_size);
        }
#അല്ലെങ്കിൽ
        net->workspace = calloc(1, workspace_size);
#endif
    }
    തിരികെ net;
}

list *read_cfg(അക്ഷരം *filename)
{
    FILE *file = fopen(filename, "r");
    എങ്കിൽ(file == 0) file_error(filename);
    അക്ഷരം *വരി;
    പൂർണ്ണസംഖ്യ nu = 0;
    list *options = make_list();
    section *current = 0;
    ആകുമ്പോൾ((വരി=fgetl(file)) != 0){
        ++ nu;
        strip(വരി);
        തിരഞ്ഞെടുപ്പ്(വരി[0]){
            അവസ്ഥ '[':
                current = malloc(വലുപ്പം(section));
                list_തിരുകുക(options, current);
                current->options = make_list();
                current->type = വരി;
                നിർത്തുക;
            അവസ്ഥ '\0':
            അവസ്ഥ '#':
            അവസ്ഥ ';':
                സ്വതന്ത്രം(വരി);
                നിർത്തുക;
            സ്ഥിരസ്ഥിതി:
                എങ്കിൽ(!read_option(വരി, current->options)){
                    fprintf(stderr, "Config file error line %d, could parse: %s\n", nu, വരി);
                    സ്വതന്ത്രം(വരി);
                }
                നിർത്തുക;
        }
    }
    fclose(file);
    തിരികെ options;
}

ശൂന്യം save_convolutional_weights_ദ്വിമാന(layer l, FILE *fp)
{
#ifdef GPU
    എങ്കിൽ(gpu_index >= 0){
        pull_convolutional_layer(l);
    }
#endif
    binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.ദ്വിമാന_weights);
    പൂർണ്ണസംഖ്യ size = l.c*l.size*l.size;
    പൂർണ്ണസംഖ്യ i, j, k;
    fwrite(l.biases, വലുപ്പം(ദശാംശം), l.n, fp);
    എങ്കിൽ (l.batch_normalize){
        fwrite(l.scales, വലുപ്പം(ദശാംശം), l.n, fp);
        fwrite(l.rolling_mean, വലുപ്പം(ദശാംശം), l.n, fp);
        fwrite(l.rolling_variance, വലുപ്പം(ദശാംശം), l.n, fp);
    }
    വേണ്ടി(i = 0; i < l.n; ++i){
        ദശാംശം mean = l.ദ്വിമാന_weights[i*size];
        എങ്കിൽ(mean < 0) mean = -mean;
        fwrite(&mean, വലുപ്പം(ദശാംശം), 1, fp);
        വേണ്ടി(j = 0; j < size/8; ++j){
            പൂർണ്ണസംഖ്യ index = i*size + j*8;
            unsigned അക്ഷരം c = 0;
            വേണ്ടി(k = 0; k < 8; ++k){
                എങ്കിൽ (j*8 + k >= size) നിർത്തുക;
                എങ്കിൽ (l.ദ്വിമാന_weights[index + k] > 0) c = (c | 1<<k);
            }
            fwrite(&c, വലുപ്പം(അക്ഷരം), 1, fp);
        }
    }
}

ശൂന്യം save_convolutional_weights(layer l, FILE *fp)
{
    എങ്കിൽ(l.ദ്വിമാന){
        //save_convolutional_weights_binary(l, fp);
        //return;
    }
#ifdef GPU
    എങ്കിൽ(gpu_index >= 0){
        pull_convolutional_layer(l);
    }
#endif
    പൂർണ്ണസംഖ്യ അക്കം = l.nweights;
    fwrite(l.biases, വലുപ്പം(ദശാംശം), l.n, fp);
    എങ്കിൽ (l.batch_normalize){
        fwrite(l.scales, വലുപ്പം(ദശാംശം), l.n, fp);
        fwrite(l.rolling_mean, വലുപ്പം(ദശാംശം), l.n, fp);
        fwrite(l.rolling_variance, വലുപ്പം(ദശാംശം), l.n, fp);
    }
    fwrite(l.weights, വലുപ്പം(ദശാംശം), അക്കം, fp);
}

ശൂന്യം save_batchnorm_weights(layer l, FILE *fp)
{
#ifdef GPU
    എങ്കിൽ(gpu_index >= 0){
        pull_batchnorm_layer(l);
    }
#endif
    fwrite(l.scales, വലുപ്പം(ദശാംശം), l.c, fp);
    fwrite(l.rolling_mean, വലുപ്പം(ദശാംശം), l.c, fp);
    fwrite(l.rolling_variance, വലുപ്പം(ദശാംശം), l.c, fp);
}

ശൂന്യം save_connected_weights(layer l, FILE *fp)
{
#ifdef GPU
    എങ്കിൽ(gpu_index >= 0){
        pull_connected_layer(l);
    }
#endif
    fwrite(l.biases, വലുപ്പം(ദശാംശം), l.outputs, fp);
    fwrite(l.weights, വലുപ്പം(ദശാംശം), l.outputs*l.inputs, fp);
    എങ്കിൽ (l.batch_normalize){
        fwrite(l.scales, വലുപ്പം(ദശാംശം), l.outputs, fp);
        fwrite(l.rolling_mean, വലുപ്പം(ദശാംശം), l.outputs, fp);
        fwrite(l.rolling_variance, വലുപ്പം(ദശാംശം), l.outputs, fp);
    }
}

ശൂന്യം save_weights_upto(network *net, അക്ഷരം *filename, പൂർണ്ണസംഖ്യ cutoff)
{
#ifdef GPU
    എങ്കിൽ(net->gpu_index >= 0){
        cuda_set_device(net->gpu_index);
    }
#endif
    fprintf(stderr, "Saving weights to %s\n", filename);
    FILE *fp = fopen(filename, "wb");
    എങ്കിൽ(!fp) file_error(filename);

    പൂർണ്ണസംഖ്യ major = 0;
    പൂർണ്ണസംഖ്യ minor = 2;
    പൂർണ്ണസംഖ്യ revision = 0;
    fwrite(&major, വലുപ്പം(പൂർണ്ണസംഖ്യ), 1, fp);
    fwrite(&minor, വലുപ്പം(പൂർണ്ണസംഖ്യ), 1, fp);
    fwrite(&revision, വലുപ്പം(പൂർണ്ണസംഖ്യ), 1, fp);
    fwrite(net->seen, വലുപ്പം(size_t), 1, fp);

    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        എങ്കിൽ (l.dontsave) തുടരുക;
        എങ്കിൽ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            save_convolutional_weights(l, fp);
        } എങ്കിൽ(l.type == CONNECTED){
            save_connected_weights(l, fp);
        } എങ്കിൽ(l.type == BATCHNORM){
            save_batchnorm_weights(l, fp);
        } എങ്കിൽ(l.type == RNN){
            save_connected_weights(*(l.നിവേശം_layer), fp);
            save_connected_weights(*(l.self_layer), fp);
            save_connected_weights(*(l.output_layer), fp);
        } എങ്കിൽ (l.type == LSTM) {
            save_connected_weights(*(l.wi), fp);
            save_connected_weights(*(l.wf), fp);
            save_connected_weights(*(l.wo), fp);
            save_connected_weights(*(l.wg), fp);
            save_connected_weights(*(l.ui), fp);
            save_connected_weights(*(l.uf), fp);
            save_connected_weights(*(l.uo), fp);
            save_connected_weights(*(l.ug), fp);
        } എങ്കിൽ (l.type == GRU) {
            എങ്കിൽ(1){
                save_connected_weights(*(l.wz), fp);
                save_connected_weights(*(l.wr), fp);
                save_connected_weights(*(l.wh), fp);
                save_connected_weights(*(l.uz), fp);
                save_connected_weights(*(l.ur), fp);
                save_connected_weights(*(l.uh), fp);
            }അല്ലെങ്കിൽ{
                save_connected_weights(*(l.reset_layer), fp);
                save_connected_weights(*(l.update_layer), fp);
                save_connected_weights(*(l.state_layer), fp);
            }
        }  എങ്കിൽ(l.type == CRNN){
            save_convolutional_weights(*(l.നിവേശം_layer), fp);
            save_convolutional_weights(*(l.self_layer), fp);
            save_convolutional_weights(*(l.output_layer), fp);
        } എങ്കിൽ(l.type == LOCAL){
#ifdef GPU
            എങ്കിൽ(gpu_index >= 0){
                pull_local_layer(l);
            }
#endif
            പൂർണ്ണസംഖ്യ locations = l.out_w*l.out_h;
            പൂർണ്ണസംഖ്യ size = l.size*l.size*l.c*l.n*locations;
            fwrite(l.biases, വലുപ്പം(ദശാംശം), l.outputs, fp);
            fwrite(l.weights, വലുപ്പം(ദശാംശം), size, fp);
        }
    }
    fclose(fp);
}
ശൂന്യം save_weights(network *net, അക്ഷരം *filename)
{
    save_weights_upto(net, filename, net->n);
}

ശൂന്യം transpose_matrix(ദശാംശം *a, പൂർണ്ണസംഖ്യ rows, പൂർണ്ണസംഖ്യ cols)
{
    ദശാംശം *transpose = calloc(rows*cols, വലുപ്പം(ദശാംശം));
    പൂർണ്ണസംഖ്യ x, y;
    വേണ്ടി(x = 0; x < rows; ++x){
        വേണ്ടി(y = 0; y < cols; ++y){
            transpose[y*rows + x] = a[x*cols + y];
        }
    }
    memcpy(a, transpose, rows*cols*വലുപ്പം(ദശാംശം));
    സ്വതന്ത്രം(transpose);
}

ശൂന്യം load_connected_weights(layer l, FILE *fp, പൂർണ്ണസംഖ്യ transpose)
{
    fread(l.biases, വലുപ്പം(ദശാംശം), l.outputs, fp);
    fread(l.weights, വലുപ്പം(ദശാംശം), l.outputs*l.inputs, fp);
    എങ്കിൽ(transpose){
        transpose_matrix(l.weights, l.inputs, l.outputs);
    }
    //printf("Biases: %f mean %f variance\n", mean_array(l.biases, l.outputs), variance_array(l.biases, l.outputs));
    //printf("Weights: %f mean %f variance\n", mean_array(l.weights, l.outputs*l.inputs), variance_array(l.weights, l.outputs*l.inputs));
    എങ്കിൽ (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, വലുപ്പം(ദശാംശം), l.outputs, fp);
        fread(l.rolling_mean, വലുപ്പം(ദശാംശം), l.outputs, fp);
        fread(l.rolling_variance, വലുപ്പം(ദശാംശം), l.outputs, fp);
        //printf("Scales: %f mean %f variance\n", mean_array(l.scales, l.outputs), variance_array(l.scales, l.outputs));
        //printf("rolling_mean: %f mean %f variance\n", mean_array(l.rolling_mean, l.outputs), variance_array(l.rolling_mean, l.outputs));
        //printf("rolling_variance: %f mean %f variance\n", mean_array(l.rolling_variance, l.outputs), variance_array(l.rolling_variance, l.outputs));
    }
#ifdef GPU
    എങ്കിൽ(gpu_index >= 0){
        തള്ളുക_connected_layer(l);
    }
#endif
}

ശൂന്യം load_batchnorm_weights(layer l, FILE *fp)
{
    fread(l.scales, വലുപ്പം(ദശാംശം), l.c, fp);
    fread(l.rolling_mean, വലുപ്പം(ദശാംശം), l.c, fp);
    fread(l.rolling_variance, വലുപ്പം(ദശാംശം), l.c, fp);
#ifdef GPU
    എങ്കിൽ(gpu_index >= 0){
        തള്ളുക_batchnorm_layer(l);
    }
#endif
}

ശൂന്യം load_convolutional_weights_ദ്വിമാന(layer l, FILE *fp)
{
    fread(l.biases, വലുപ്പം(ദശാംശം), l.n, fp);
    എങ്കിൽ (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, വലുപ്പം(ദശാംശം), l.n, fp);
        fread(l.rolling_mean, വലുപ്പം(ദശാംശം), l.n, fp);
        fread(l.rolling_variance, വലുപ്പം(ദശാംശം), l.n, fp);
    }
    പൂർണ്ണസംഖ്യ size = l.c*l.size*l.size;
    പൂർണ്ണസംഖ്യ i, j, k;
    വേണ്ടി(i = 0; i < l.n; ++i){
        ദശാംശം mean = 0;
        fread(&mean, വലുപ്പം(ദശാംശം), 1, fp);
        വേണ്ടി(j = 0; j < size/8; ++j){
            പൂർണ്ണസംഖ്യ index = i*size + j*8;
            unsigned അക്ഷരം c = 0;
            fread(&c, വലുപ്പം(അക്ഷരം), 1, fp);
            വേണ്ടി(k = 0; k < 8; ++k){
                എങ്കിൽ (j*8 + k >= size) നിർത്തുക;
                l.weights[index + k] = (c & 1<<k) ? mean : -mean;
            }
        }
    }
#ifdef GPU
    എങ്കിൽ(gpu_index >= 0){
        തള്ളുക_convolutional_layer(l);
    }
#endif
}

ശൂന്യം load_convolutional_weights(layer l, FILE *fp)
{
    എങ്കിൽ(l.ദ്വിമാന){
        //load_convolutional_weights_binary(l, fp);
        //return;
    }
    എങ്കിൽ(l.numload) l.n = l.numload;
    പൂർണ്ണസംഖ്യ അക്കം = l.c/l.groups*l.n*l.size*l.size;
    fread(l.biases, വലുപ്പം(ദശാംശം), l.n, fp);
    എങ്കിൽ (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, വലുപ്പം(ദശാംശം), l.n, fp);
        fread(l.rolling_mean, വലുപ്പം(ദശാംശം), l.n, fp);
        fread(l.rolling_variance, വലുപ്പം(ദശാംശം), l.n, fp);
        എങ്കിൽ(0){
            പൂർണ്ണസംഖ്യ i;
            വേണ്ടി(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_mean[i]);
            }
            printf("\n");
            വേണ്ടി(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_variance[i]);
            }
            printf("\n");
        }
        എങ്കിൽ(0){
            fill_cpu(l.n, 0, l.rolling_mean, 1);
            fill_cpu(l.n, 0, l.rolling_variance, 1);
        }
        എങ്കിൽ(0){
            പൂർണ്ണസംഖ്യ i;
            വേണ്ടി(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_mean[i]);
            }
            printf("\n");
            വേണ്ടി(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_variance[i]);
            }
            printf("\n");
        }
    }
    fread(l.weights, വലുപ്പം(ദശാംശം), അക്കം, fp);
    //if(l.c == 3) scal_cpu(num, 1./256, l.weights, 1);
    എങ്കിൽ (l.flipped) {
        transpose_matrix(l.weights, l.c*l.size*l.size, l.n);
    }
    //if (l.binary) binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.weights);
#ifdef GPU
    എങ്കിൽ(gpu_index >= 0){
        തള്ളുക_convolutional_layer(l);
    }
#endif
}


ശൂന്യം load_weights_upto(network *net, അക്ഷരം *filename, പൂർണ്ണസംഖ്യ start, പൂർണ്ണസംഖ്യ cutoff)
{
#ifdef GPU
    എങ്കിൽ(net->gpu_index >= 0){
        cuda_set_device(net->gpu_index);
    }
#endif
    fprintf(stderr, "Loading weights from %s...", filename);
    fflush(stdout);
    FILE *fp = fopen(filename, "rb");
    എങ്കിൽ(!fp) file_error(filename);

    പൂർണ്ണസംഖ്യ major;
    പൂർണ്ണസംഖ്യ minor;
    പൂർണ്ണസംഖ്യ revision;
    fread(&major, വലുപ്പം(പൂർണ്ണസംഖ്യ), 1, fp);
    fread(&minor, വലുപ്പം(പൂർണ്ണസംഖ്യ), 1, fp);
    fread(&revision, വലുപ്പം(പൂർണ്ണസംഖ്യ), 1, fp);
    എങ്കിൽ ((major*10 + minor) >= 2 && major < 1000 && minor < 1000){
        fread(net->seen, വലുപ്പം(size_t), 1, fp);
    } അല്ലെങ്കിൽ {
        പൂർണ്ണസംഖ്യ iseen = 0;
        fread(&iseen, വലുപ്പം(പൂർണ്ണസംഖ്യ), 1, fp);
        *net->seen = iseen;
    }
    പൂർണ്ണസംഖ്യ transpose = (major > 1000) || (minor > 1000);

    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = start; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        എങ്കിൽ (l.dontload) തുടരുക;
        എങ്കിൽ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            load_convolutional_weights(l, fp);
        }
        എങ്കിൽ(l.type == CONNECTED){
            load_connected_weights(l, fp, transpose);
        }
        എങ്കിൽ(l.type == BATCHNORM){
            load_batchnorm_weights(l, fp);
        }
        എങ്കിൽ(l.type == CRNN){
            load_convolutional_weights(*(l.നിവേശം_layer), fp);
            load_convolutional_weights(*(l.self_layer), fp);
            load_convolutional_weights(*(l.output_layer), fp);
        }
        എങ്കിൽ(l.type == RNN){
            load_connected_weights(*(l.നിവേശം_layer), fp, transpose);
            load_connected_weights(*(l.self_layer), fp, transpose);
            load_connected_weights(*(l.output_layer), fp, transpose);
        }
        എങ്കിൽ (l.type == LSTM) {
            load_connected_weights(*(l.wi), fp, transpose);
            load_connected_weights(*(l.wf), fp, transpose);
            load_connected_weights(*(l.wo), fp, transpose);
            load_connected_weights(*(l.wg), fp, transpose);
            load_connected_weights(*(l.ui), fp, transpose);
            load_connected_weights(*(l.uf), fp, transpose);
            load_connected_weights(*(l.uo), fp, transpose);
            load_connected_weights(*(l.ug), fp, transpose);
        }
        എങ്കിൽ (l.type == GRU) {
            എങ്കിൽ(1){
                load_connected_weights(*(l.wz), fp, transpose);
                load_connected_weights(*(l.wr), fp, transpose);
                load_connected_weights(*(l.wh), fp, transpose);
                load_connected_weights(*(l.uz), fp, transpose);
                load_connected_weights(*(l.ur), fp, transpose);
                load_connected_weights(*(l.uh), fp, transpose);
            }അല്ലെങ്കിൽ{
                load_connected_weights(*(l.reset_layer), fp, transpose);
                load_connected_weights(*(l.update_layer), fp, transpose);
                load_connected_weights(*(l.state_layer), fp, transpose);
            }
        }
        എങ്കിൽ(l.type == LOCAL){
            പൂർണ്ണസംഖ്യ locations = l.out_w*l.out_h;
            പൂർണ്ണസംഖ്യ size = l.size*l.size*l.c*l.n*locations;
            fread(l.biases, വലുപ്പം(ദശാംശം), l.outputs, fp);
            fread(l.weights, വലുപ്പം(ദശാംശം), size, fp);
#ifdef GPU
            എങ്കിൽ(gpu_index >= 0){
                തള്ളുക_local_layer(l);
            }
#endif
        }
    }
    fprintf(stderr, "Done!\n");
    fclose(fp);
}

ശൂന്യം load_weights(network *net, അക്ഷരം *filename)
{
    load_weights_upto(net, filename, 0, net->n);
}


#समावेश <stdio.h>
#समावेश <string.h>
#समावेश <stdlib.h>
#समावेश <assert.h>

#समावेश "activation_layer.h"
#समावेश "logistic_layer.h"
#समावेश "l2norm_layer.h"
#समावेश "activations.h"
#समावेश "avgpool_layer.h"
#समावेश "batchnorm_layer.h"
#समावेश "blas.h"
#समावेश "connected_layer.h"
#समावेश "deconvolutional_layer.h"
#समावेश "convolutional_layer.h"
#समावेश "cost_layer.h"
#समावेश "crnn_layer.h"
#समावेश "crop_layer.h"
#समावेश "detection_layer.h"
#समावेश "dropout_layer.h"
#समावेश "gru_layer.h"
#समावेश "list.h"
#समावेश "local_layer.h"
#समावेश "maxpool_layer.h"
#समावेश "normalization_layer.h"
#समावेश "option_list.h"
#समावेश "parser.h"
#समावेश "region_layer.h"
#समावेश "yolo_layer.h"
#समावेश "iseg_layer.h"
#समावेश "reorg_layer.h"
#समावेश "rnn_layer.h"
#समावेश "route_layer.h"
#समावेश "upsample_layer.h"
#समावेश "shortcut_layer.h"
#समावेश "softmax_layer.h"
#समावेश "lstm_layer.h"
#समावेश "utils.h"

प्रकार काष्ठा{
    अक्षर *type;
    list *options;
}section;

list *read_cfg(अक्षर *filename);

LAYER_TYPE string_to_layer_type(अक्षर * type)
{

    अगर (म_भेद(type, "[shortcut]")==0) वापस SHORTCUT;
    अगर (म_भेद(type, "[crop]")==0) वापस CROP;
    अगर (म_भेद(type, "[cost]")==0) वापस COST;
    अगर (म_भेद(type, "[detection]")==0) वापस DETECTION;
    अगर (म_भेद(type, "[region]")==0) वापस REGION;
    अगर (म_भेद(type, "[yolo]")==0) वापस YOLO;
    अगर (म_भेद(type, "[iseg]")==0) वापस ISEG;
    अगर (म_भेद(type, "[local]")==0) वापस LOCAL;
    अगर (म_भेद(type, "[conv]")==0
            || म_भेद(type, "[convolutional]")==0) वापस CONVOLUTIONAL;
    अगर (म_भेद(type, "[deconv]")==0
            || म_भेद(type, "[deconvolutional]")==0) वापस DECONVOLUTIONAL;
    अगर (म_भेद(type, "[activation]")==0) वापस ACTIVE;
    अगर (म_भेद(type, "[logistic]")==0) वापस LOGXENT;
    अगर (म_भेद(type, "[l2norm]")==0) वापस L2NORM;
    अगर (म_भेद(type, "[net]")==0
            || म_भेद(type, "[network]")==0) वापस NETWORK;
    अगर (म_भेद(type, "[crnn]")==0) वापस CRNN;
    अगर (म_भेद(type, "[gru]")==0) वापस GRU;
    अगर (म_भेद(type, "[lstm]") == 0) वापस LSTM;
    अगर (म_भेद(type, "[rnn]")==0) वापस RNN;
    अगर (म_भेद(type, "[conn]")==0
            || म_भेद(type, "[connected]")==0) वापस CONNECTED;
    अगर (म_भेद(type, "[max]")==0
            || म_भेद(type, "[maxpool]")==0) वापस MAXPOOL;
    अगर (म_भेद(type, "[reorg]")==0) वापस REORG;
    अगर (म_भेद(type, "[avg]")==0
            || म_भेद(type, "[avgpool]")==0) वापस AVGPOOL;
    अगर (म_भेद(type, "[dropout]")==0) वापस DROPOUT;
    अगर (म_भेद(type, "[lrn]")==0
            || म_भेद(type, "[normalization]")==0) वापस NORMALIZATION;
    अगर (म_भेद(type, "[batchnorm]")==0) वापस BATCHNORM;
    अगर (म_भेद(type, "[soft]")==0
            || म_भेद(type, "[softmax]")==0) वापस SOFTMAX;
    अगर (म_भेद(type, "[route]")==0) वापस ROUTE;
    अगर (म_भेद(type, "[upsample]")==0) वापस UPSAMPLE;
    वापस BLANK;
}

व्योम free_section(section *s)
{
    मुक्त(s->type);
    गाँठ *n = s->options->front;
    जबतक(n){
        kvp *pair = (kvp *)n->val;
        मुक्त(pair->कुंजी);
        मुक्त(pair);
        गाँठ *अग्र = n->अग्र;
        मुक्त(n);
        n = अग्र;
    }
    मुक्त(s->options);
    मुक्त(s);
}

व्योम parse_तथ्य(अक्षर *तथ्य, भग्न *a, पूर्णांक n)
{
    पूर्णांक i;
    अगर(!तथ्य) वापस;
    अक्षर *curr = तथ्य;
    अक्षर *अग्र = तथ्य;
    पूर्णांक done = 0;
    क्रम(i = 0; i < n && !done; ++i){
        जबतक(*++अग्र !='\0' && *अग्र != ',');
        अगर(*अग्र == '\0') done = 1;
        *अग्र = '\0';
        माला_पूछो(curr, "%g", &a[i]);
        curr = अग्र+1;
    }
}

प्रकार काष्ठा आकार_params{
    पूर्णांक batch;
    पूर्णांक inputs;
    पूर्णांक h;
    पूर्णांक w;
    पूर्णांक c;
    पूर्णांक सूचकांक;
    पूर्णांक time_steps;
    network *net;
} आकार_params;

local_layer parse_local(list *options, आकार_params params)
{
    पूर्णांक n = option_find_int(options, "filters",1);
    पूर्णांक आकार = option_find_int(options, "size",1);
    पूर्णांक stride = option_find_int(options, "stride",1);
    पूर्णांक pad = option_find_int(options, "pad",0);
    अक्षर *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    पूर्णांक batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    अगर(!(h && w && c)) error("Layer before local layer must output image.");

    local_layer layer = make_local_layer(batch,h,w,c,n,आकार,stride,pad,activation);

    वापस layer;
}

layer parse_deconvolutional(list *options, आकार_params params)
{
    पूर्णांक n = option_find_int(options, "filters",1);
    पूर्णांक आकार = option_find_int(options, "size",1);
    पूर्णांक stride = option_find_int(options, "stride",1);

    अक्षर *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    पूर्णांक batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    अगर(!(h && w && c)) error("Layer before deconvolutional layer must output image.");
    पूर्णांक batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);
    पूर्णांक pad = option_find_int_quiet(options, "pad",0);
    पूर्णांक padding = option_find_int_quiet(options, "padding",0);
    अगर(pad) padding = आकार/2;

    layer l = make_deconvolutional_layer(batch,h,w,c,n,आकार,stride,padding, activation, batch_normalize, params.net->adam);

    वापस l;
}


convolutional_layer parse_convolutional(list *options, आकार_params params)
{
    पूर्णांक n = option_find_int(options, "filters",1);
    पूर्णांक आकार = option_find_int(options, "size",1);
    पूर्णांक stride = option_find_int(options, "stride",1);
    पूर्णांक pad = option_find_int_quiet(options, "pad",0);
    पूर्णांक padding = option_find_int_quiet(options, "padding",0);
    पूर्णांक groups = option_find_int_quiet(options, "groups", 1);
    अगर(pad) padding = आकार/2;

    अक्षर *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    पूर्णांक batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    अगर(!(h && w && c)) error("Layer before convolutional layer must output image.");
    पूर्णांक batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);
    पूर्णांक द्विआधारी = option_find_int_quiet(options, "binary", 0);
    पूर्णांक xnor = option_find_int_quiet(options, "xnor", 0);

    convolutional_layer layer = make_convolutional_layer(batch,h,w,c,n,groups,आकार,stride,padding,activation, batch_normalize, द्विआधारी, xnor, params.net->adam);
    layer.flipped = option_find_int_quiet(options, "flipped", 0);
    layer.dot = option_find_float_quiet(options, "dot", 0);

    वापस layer;
}

layer parse_crnn(list *options, आकार_params params)
{
    पूर्णांक output_filters = option_find_int(options, "output_filters",1);
    पूर्णांक hidden_filters = option_find_int(options, "hidden_filters",1);
    अक्षर *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    पूर्णांक batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_crnn_layer(params.batch, params.w, params.h, params.c, hidden_filters, output_filters, params.time_steps, activation, batch_normalize);

    l.shortcut = option_find_int_quiet(options, "shortcut", 0);

    वापस l;
}

layer parse_rnn(list *options, आकार_params params)
{
    पूर्णांक output = option_find_int(options, "output",1);
    अक्षर *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    पूर्णांक batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_rnn_layer(params.batch, params.inputs, output, params.time_steps, activation, batch_normalize, params.net->adam);

    l.shortcut = option_find_int_quiet(options, "shortcut", 0);

    वापस l;
}

layer parse_gru(list *options, आकार_params params)
{
    पूर्णांक output = option_find_int(options, "output",1);
    पूर्णांक batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_gru_layer(params.batch, params.inputs, output, params.time_steps, batch_normalize, params.net->adam);
    l.tanh = option_find_int_quiet(options, "tanh", 0);

    वापस l;
}

layer parse_lstm(list *options, आकार_params params)
{
    पूर्णांक output = option_find_int(options, "output", 1);
    पूर्णांक batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_lstm_layer(params.batch, params.inputs, output, params.time_steps, batch_normalize, params.net->adam);

    वापस l;
}

layer parse_connected(list *options, आकार_params params)
{
    पूर्णांक output = option_find_int(options, "output",1);
    अक्षर *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    पूर्णांक batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_connected_layer(params.batch, params.inputs, output, activation, batch_normalize, params.net->adam);
    वापस l;
}

layer parse_softmax(list *options, आकार_params params)
{
    पूर्णांक groups = option_find_int_quiet(options, "groups",1);
    layer l = make_softmax_layer(params.batch, params.inputs, groups);
    l.temperature = option_find_float_quiet(options, "temperature", 1);
    अक्षर *वृक्ष_file = option_find_str(options, "tree", 0);
    अगर (वृक्ष_file) l.softmax_वृक्ष = read_वृक्ष(वृक्ष_file);
    l.w = params.w;
    l.h = params.h;
    l.c = params.c;
    l.spatial = option_find_float_quiet(options, "spatial", 0);
    l.noloss =  option_find_int_quiet(options, "noloss", 0);
    वापस l;
}

पूर्णांक *parse_yolo_mask(अक्षर *a, पूर्णांक *अंक)
{
    पूर्णांक *mask = 0;
    अगर(a){
        पूर्णांक len = म_माप(a);
        पूर्णांक n = 1;
        पूर्णांक i;
        क्रम(i = 0; i < len; ++i){
            अगर (a[i] == ',') ++n;
        }
        mask = सुस्मृति(n, माप(पूर्णांक));
        क्रम(i = 0; i < n; ++i){
            पूर्णांक val = म_से_प(a);
            mask[i] = val;
            a = म_अक्षर(a, ',')+1;
        }
        *अंक = n;
    }
    वापस mask;
}

layer parse_yolo(list *options, आकार_params params)
{
    पूर्णांक classes = option_find_int(options, "classes", 20);
    पूर्णांक योग_2 = option_find_int(options, "num", 1);
    पूर्णांक अंक = योग_2;

    अक्षर *a = option_find_str(options, "mask", 0);
    पूर्णांक *mask = parse_yolo_mask(a, &अंक);
    layer l = make_yolo_layer(params.batch, params.w, params.h, अंक, योग_2, mask, classes);
    निश्चित(l.outputs == params.inputs);

    l.max_boxes = option_find_int_quiet(options, "max",90);
    l.jitter = option_find_float(options, "jitter", .2);

    l.ignore_thresh = option_find_float(options, "ignore_thresh", .5);
    l.truth_thresh = option_find_float(options, "truth_thresh", 1);
    l.random = option_find_int_quiet(options, "random", 0);

    अक्षर *map_file = option_find_str(options, "map", 0);
    अगर (map_file) l.map = read_map(map_file);

    a = option_find_str(options, "anchors", 0);
    अगर(a){
        पूर्णांक len = म_माप(a);
        पूर्णांक n = 1;
        पूर्णांक i;
        क्रम(i = 0; i < len; ++i){
            अगर (a[i] == ',') ++n;
        }
        क्रम(i = 0; i < n; ++i){
            भग्न bias = म_से_भ(a);
            l.biases[i] = bias;
            a = म_अक्षर(a, ',')+1;
        }
    }
    वापस l;
}

layer parse_iseg(list *options, आकार_params params)
{
    पूर्णांक classes = option_find_int(options, "classes", 20);
    पूर्णांक ids = option_find_int(options, "ids", 32);
    layer l = make_iseg_layer(params.batch, params.w, params.h, classes, ids);
    निश्चित(l.outputs == params.inputs);
    वापस l;
}

layer parse_region(list *options, आकार_params params)
{
    पूर्णांक coords = option_find_int(options, "coords", 4);
    पूर्णांक classes = option_find_int(options, "classes", 20);
    पूर्णांक अंक = option_find_int(options, "num", 1);

    layer l = make_region_layer(params.batch, params.w, params.h, अंक, classes, coords);
    निश्चित(l.outputs == params.inputs);

    l.log = option_find_int_quiet(options, "log", 0);
    l.वर्ग_मूल = option_find_int_quiet(options, "sqrt", 0);

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

    अक्षर *वृक्ष_file = option_find_str(options, "tree", 0);
    अगर (वृक्ष_file) l.softmax_वृक्ष = read_वृक्ष(वृक्ष_file);
    अक्षर *map_file = option_find_str(options, "map", 0);
    अगर (map_file) l.map = read_map(map_file);

    अक्षर *a = option_find_str(options, "anchors", 0);
    अगर(a){
        पूर्णांक len = म_माप(a);
        पूर्णांक n = 1;
        पूर्णांक i;
        क्रम(i = 0; i < len; ++i){
            अगर (a[i] == ',') ++n;
        }
        क्रम(i = 0; i < n; ++i){
            भग्न bias = म_से_भ(a);
            l.biases[i] = bias;
            a = म_अक्षर(a, ',')+1;
        }
    }
    वापस l;
}

detection_layer parse_detection(list *options, आकार_params params)
{
    पूर्णांक coords = option_find_int(options, "coords", 1);
    पूर्णांक classes = option_find_int(options, "classes", 1);
    पूर्णांक rescore = option_find_int(options, "rescore", 0);
    पूर्णांक अंक = option_find_int(options, "num", 1);
    पूर्णांक side = option_find_int(options, "side", 7);
    detection_layer layer = make_detection_layer(params.batch, params.inputs, अंक, side, classes, coords, rescore);

    layer.softmax = option_find_int(options, "softmax", 0);
    layer.वर्ग_मूल = option_find_int(options, "sqrt", 0);

    layer.max_boxes = option_find_int_quiet(options, "max",90);
    layer.coord_scale = option_find_float(options, "coord_scale", 1);
    layer.forced = option_find_int(options, "forced", 0);
    layer.object_scale = option_find_float(options, "object_scale", 1);
    layer.noobject_scale = option_find_float(options, "noobject_scale", 1);
    layer.class_scale = option_find_float(options, "class_scale", 1);
    layer.jitter = option_find_float(options, "jitter", .2);
    layer.random = option_find_int_quiet(options, "random", 0);
    layer.reorg = option_find_int_quiet(options, "reorg", 0);
    वापस layer;
}

cost_layer parse_cost(list *options, आकार_params params)
{
    अक्षर *type_s = option_find_str(options, "type", "sse");
    COST_TYPE type = get_cost_type(type_s);
    भग्न scale = option_find_float_quiet(options, "scale",1);
    cost_layer layer = make_cost_layer(params.batch, params.inputs, type, scale);
    layer.ratio =  option_find_float_quiet(options, "ratio",0);
    layer.noobject_scale =  option_find_float_quiet(options, "noobj", 1);
    layer.thresh =  option_find_float_quiet(options, "thresh",0);
    वापस layer;
}

crop_layer parse_crop(list *options, आकार_params params)
{
    पूर्णांक crop_height = option_find_int(options, "crop_height",1);
    पूर्णांक crop_width = option_find_int(options, "crop_width",1);
    पूर्णांक flip = option_find_int(options, "flip",0);
    भग्न angle = option_find_float(options, "angle",0);
    भग्न saturation = option_find_float(options, "saturation",1);
    भग्न exposure = option_find_float(options, "exposure",1);

    पूर्णांक batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    अगर(!(h && w && c)) error("Layer before crop layer must output image.");

    पूर्णांक noadjust = option_find_int_quiet(options, "noadjust",0);

    crop_layer l = make_crop_layer(batch,h,w,c,crop_height,crop_width,flip, angle, saturation, exposure);
    l.shift = option_find_float(options, "shift", 0);
    l.noadjust = noadjust;
    वापस l;
}

layer parse_reorg(list *options, आकार_params params)
{
    पूर्णांक stride = option_find_int(options, "stride",1);
    पूर्णांक reverse = option_find_int_quiet(options, "reverse",0);
    पूर्णांक flatten = option_find_int_quiet(options, "flatten",0);
    पूर्णांक extra = option_find_int_quiet(options, "extra",0);

    पूर्णांक batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    अगर(!(h && w && c)) error("Layer before reorg layer must output image.");

    layer layer = make_reorg_layer(batch,w,h,c,stride,reverse, flatten, extra);
    वापस layer;
}

maxpool_layer parse_maxpool(list *options, आकार_params params)
{
    पूर्णांक stride = option_find_int(options, "stride",1);
    पूर्णांक आकार = option_find_int(options, "size",stride);
    पूर्णांक padding = option_find_int_quiet(options, "padding", आकार-1);

    पूर्णांक batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    अगर(!(h && w && c)) error("Layer before maxpool layer must output image.");

    maxpool_layer layer = make_maxpool_layer(batch,h,w,c,आकार,stride,padding);
    वापस layer;
}

avgpool_layer parse_avgpool(list *options, आकार_params params)
{
    पूर्णांक batch,w,h,c;
    w = params.w;
    h = params.h;
    c = params.c;
    batch=params.batch;
    अगर(!(h && w && c)) error("Layer before avgpool layer must output image.");

    avgpool_layer layer = make_avgpool_layer(batch,w,h,c);
    वापस layer;
}

dropout_layer parse_dropout(list *options, आकार_params params)
{
    भग्न probability = option_find_float(options, "probability", .5);
    dropout_layer layer = make_dropout_layer(params.batch, params.inputs, probability);
    layer.out_w = params.w;
    layer.out_h = params.h;
    layer.out_c = params.c;
    वापस layer;
}

layer parse_normalization(list *options, आकार_params params)
{
    भग्न alpha = option_find_float(options, "alpha", .0001);
    भग्न beta =  option_find_float(options, "beta" , .75);
    भग्न kappa = option_find_float(options, "kappa", 1);
    पूर्णांक आकार = option_find_int(options, "size", 5);
    layer l = make_normalization_layer(params.batch, params.w, params.h, params.c, आकार, alpha, beta, kappa);
    वापस l;
}

layer parse_batchnorm(list *options, आकार_params params)
{
    layer l = make_batchnorm_layer(params.batch, params.w, params.h, params.c);
    वापस l;
}

layer parse_shortcut(list *options, आकार_params params, network *net)
{
    अक्षर *l = option_find(options, "from");
    पूर्णांक सूचकांक = म_से_प(l);
    अगर(सूचकांक < 0) सूचकांक = params.सूचकांक + सूचकांक;

    पूर्णांक batch = params.batch;
    layer from = net->layers[सूचकांक];

    layer s = make_shortcut_layer(batch, सूचकांक, params.w, params.h, params.c, from.out_w, from.out_h, from.out_c);

    अक्षर *activation_s = option_find_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);
    s.activation = activation;
    s.alpha = option_find_float_quiet(options, "alpha", 1);
    s.beta = option_find_float_quiet(options, "beta", 1);
    वापस s;
}


layer parse_l2norm(list *options, आकार_params params)
{
    layer l = make_l2norm_layer(params.batch, params.inputs);
    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;
    वापस l;
}


layer parse_logistic(list *options, आकार_params params)
{
    layer l = make_logistic_layer(params.batch, params.inputs);
    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;
    वापस l;
}

layer parse_activation(list *options, आकार_params params)
{
    अक्षर *activation_s = option_find_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);

    layer l = make_activation_layer(params.batch, params.inputs, activation);

    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;

    वापस l;
}

layer parse_upsample(list *options, आकार_params params, network *net)
{

    पूर्णांक stride = option_find_int(options, "stride",2);
    layer l = make_upsample_layer(params.batch, params.w, params.h, params.c, stride);
    l.scale = option_find_float_quiet(options, "scale", 1);
    वापस l;
}

route_layer parse_route(list *options, आकार_params params, network *net)
{
    अक्षर *l = option_find(options, "layers");
    पूर्णांक len = म_माप(l);
    अगर(!l) error("Route Layer must specify input layers");
    पूर्णांक n = 1;
    पूर्णांक i;
    क्रम(i = 0; i < len; ++i){
        अगर (l[i] == ',') ++n;
    }

    पूर्णांक *layers = सुस्मृति(n, माप(पूर्णांक));
    पूर्णांक *sizes = सुस्मृति(n, माप(पूर्णांक));
    क्रम(i = 0; i < n; ++i){
        पूर्णांक सूचकांक = म_से_प(l);
        l = म_अक्षर(l, ',')+1;
        अगर(सूचकांक < 0) सूचकांक = params.सूचकांक + सूचकांक;
        layers[i] = सूचकांक;
        sizes[i] = net->layers[सूचकांक].outputs;
    }
    पूर्णांक batch = params.batch;

    route_layer layer = make_route_layer(batch, n, layers, sizes);

    convolutional_layer first = net->layers[layers[0]];
    layer.out_w = first.out_w;
    layer.out_h = first.out_h;
    layer.out_c = first.out_c;
    क्रम(i = 1; i < n; ++i){
        पूर्णांक सूचकांक = layers[i];
        convolutional_layer अग्र = net->layers[सूचकांक];
        अगर(अग्र.out_w == first.out_w && अग्र.out_h == first.out_h){
            layer.out_c += अग्र.out_c;
        }अन्यथा{
            layer.out_h = layer.out_w = layer.out_c = 0;
        }
    }

    वापस layer;
}

learning_rate_policy get_policy(अक्षर *s)
{
    अगर (म_भेद(s, "random")==0) वापस RANDOM;
    अगर (म_भेद(s, "poly")==0) वापस POLY;
    अगर (म_भेद(s, "constant")==0) वापस CONSTANT;
    अगर (म_भेद(s, "step")==0) वापस STEP;
    अगर (म_भेद(s, "exp")==0) वापस EXP;
    अगर (म_भेद(s, "sigmoid")==0) वापस SIG;
    अगर (म_भेद(s, "steps")==0) वापस STEPS;
    ख_लिखो(मानक_त्रुटि, "Couldn't find policy %s, going with constant\n", s);
    वापस CONSTANT;
}

व्योम parse_net_options(list *options, network *net)
{
    net->batch = option_find_int(options, "batch",1);
    net->learning_rate = option_find_float(options, "learning_rate", .001);
    net->momentum = option_find_float(options, "momentum", .9);
    net->decay = option_find_float(options, "decay", .0001);
    पूर्णांक subdivs = option_find_int(options, "subdivisions",1);
    net->time_steps = option_find_int_quiet(options, "time_steps",1);
    net->notruth = option_find_int_quiet(options, "notruth",0);
    net->batch /= subdivs;
    net->batch *= net->time_steps;
    net->subdivisions = subdivs;
    net->random = option_find_int_quiet(options, "random", 0);

    net->adam = option_find_int_quiet(options, "adam", 0);
    अगर(net->adam){
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
    net->max_ratio = option_find_float_quiet(options, "max_ratio", (भग्न) net->max_crop / net->w);
    net->min_ratio = option_find_float_quiet(options, "min_ratio", (भग्न) net->min_crop / net->w);
    net->center = option_find_int_quiet(options, "center",0);
    net->clip = option_find_float_quiet(options, "clip", 0);

    net->angle = option_find_float_quiet(options, "angle", 0);
    net->aspect = option_find_float_quiet(options, "aspect", 1);
    net->saturation = option_find_float_quiet(options, "saturation", 1);
    net->exposure = option_find_float_quiet(options, "exposure", 1);
    net->hue = option_find_float_quiet(options, "hue", 0);

    अगर(!net->inputs && !(net->h && net->w && net->c)) error("No input parameters supplied");

    अक्षर *policy_s = option_find_str(options, "policy", "constant");
    net->policy = get_policy(policy_s);
    net->burn_in = option_find_int_quiet(options, "burn_in", 0);
    net->power = option_find_float_quiet(options, "power", 4);
    अगर(net->policy == STEP){
        net->step = option_find_int(options, "step", 1);
        net->scale = option_find_float(options, "scale", 1);
    } अन्यथा अगर (net->policy == STEPS){
        अक्षर *l = option_find(options, "steps");
        अक्षर *p = option_find(options, "scales");
        अगर(!l || !p) error("STEPS policy must have steps and scales in cfg file");

        पूर्णांक len = म_माप(l);
        पूर्णांक n = 1;
        पूर्णांक i;
        क्रम(i = 0; i < len; ++i){
            अगर (l[i] == ',') ++n;
        }
        पूर्णांक *steps = सुस्मृति(n, माप(पूर्णांक));
        भग्न *scales = सुस्मृति(n, माप(भग्न));
        क्रम(i = 0; i < n; ++i){
            पूर्णांक step    = म_से_प(l);
            भग्न scale = म_से_भ(p);
            l = म_अक्षर(l, ',')+1;
            p = म_अक्षर(p, ',')+1;
            steps[i] = step;
            scales[i] = scale;
        }
        net->scales = scales;
        net->steps = steps;
        net->अंक_steps = n;
    } अन्यथा अगर (net->policy == EXP){
        net->gamma = option_find_float(options, "gamma", 1);
    } अन्यथा अगर (net->policy == SIG){
        net->gamma = option_find_float(options, "gamma", 1);
        net->step = option_find_int(options, "step", 1);
    } अन्यथा अगर (net->policy == POLY || net->policy == RANDOM){
    }
    net->max_batches = option_find_int(options, "max_batches", 0);
}

पूर्णांक is_network(section *s)
{
    वापस (म_भेद(s->type, "[net]")==0
            || म_भेद(s->type, "[network]")==0);
}

network *parse_network_cfg(अक्षर *filename)
{
    list *sections = read_cfg(filename);
    गाँठ *n = sections->front;
    अगर(!n) error("Config file has no sections");
    network *net = make_network(sections->आकार - 1);
    net->gpu_सूचकांक = gpu_सूचकांक;
    आकार_params params;

    section *s = (section *)n->val;
    list *options = s->options;
    अगर(!is_network(s)) error("First section must be [net] or [network]");
    parse_net_options(options, net);

    params.h = net->h;
    params.w = net->w;
    params.c = net->c;
    params.inputs = net->inputs;
    params.batch = net->batch;
    params.time_steps = net->time_steps;
    params.net = net;

    माप_प्रकार workspace_आकार = 0;
    n = n->अग्र;
    पूर्णांक गिनती = 0;
    free_section(s);
    ख_लिखो(मानक_त्रुटि, "layer     filters    size              input                output\n");
    जबतक(n){
        params.सूचकांक = गिनती;
        ख_लिखो(मानक_त्रुटि, "%5d ", गिनती);
        s = (section *)n->val;
        options = s->options;
        layer l = {0};
        LAYER_TYPE lt = string_to_layer_type(s->type);
        अगर(lt == CONVOLUTIONAL){
            l = parse_convolutional(options, params);
        }अन्यथा अगर(lt == DECONVOLUTIONAL){
            l = parse_deconvolutional(options, params);
        }अन्यथा अगर(lt == LOCAL){
            l = parse_local(options, params);
        }अन्यथा अगर(lt == ACTIVE){
            l = parse_activation(options, params);
        }अन्यथा अगर(lt == LOGXENT){
            l = parse_logistic(options, params);
        }अन्यथा अगर(lt == L2NORM){
            l = parse_l2norm(options, params);
        }अन्यथा अगर(lt == RNN){
            l = parse_rnn(options, params);
        }अन्यथा अगर(lt == GRU){
            l = parse_gru(options, params);
        }अन्यथा अगर (lt == LSTM) {
            l = parse_lstm(options, params);
        }अन्यथा अगर(lt == CRNN){
            l = parse_crnn(options, params);
        }अन्यथा अगर(lt == CONNECTED){
            l = parse_connected(options, params);
        }अन्यथा अगर(lt == CROP){
            l = parse_crop(options, params);
        }अन्यथा अगर(lt == COST){
            l = parse_cost(options, params);
        }अन्यथा अगर(lt == REGION){
            l = parse_region(options, params);
        }अन्यथा अगर(lt == YOLO){
            l = parse_yolo(options, params);
        }अन्यथा अगर(lt == ISEG){
            l = parse_iseg(options, params);
        }अन्यथा अगर(lt == DETECTION){
            l = parse_detection(options, params);
        }अन्यथा अगर(lt == SOFTMAX){
            l = parse_softmax(options, params);
            net->hierarchy = l.softmax_वृक्ष;
        }अन्यथा अगर(lt == NORMALIZATION){
            l = parse_normalization(options, params);
        }अन्यथा अगर(lt == BATCHNORM){
            l = parse_batchnorm(options, params);
        }अन्यथा अगर(lt == MAXPOOL){
            l = parse_maxpool(options, params);
        }अन्यथा अगर(lt == REORG){
            l = parse_reorg(options, params);
        }अन्यथा अगर(lt == AVGPOOL){
            l = parse_avgpool(options, params);
        }अन्यथा अगर(lt == ROUTE){
            l = parse_route(options, params, net);
        }अन्यथा अगर(lt == UPSAMPLE){
            l = parse_upsample(options, params, net);
        }अन्यथा अगर(lt == SHORTCUT){
            l = parse_shortcut(options, params, net);
        }अन्यथा अगर(lt == DROPOUT){
            l = parse_dropout(options, params);
            l.output = net->layers[गिनती-1].output;
            l.delta = net->layers[गिनती-1].delta;
#अगर_घोषित GPU
            l.output_gpu = net->layers[गिनती-1].output_gpu;
            l.delta_gpu = net->layers[गिनती-1].delta_gpu;
#पूर्ण_अगर
        }अन्यथा{
            ख_लिखो(मानक_त्रुटि, "Type not recognized: %s\n", s->type);
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
        net->layers[गिनती] = l;
        अगर (l.workspace_आकार > workspace_आकार) workspace_आकार = l.workspace_आकार;
        free_section(s);
        n = n->अग्र;
        ++गिनती;
        अगर(n){
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
    अगर(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    net->निवेश = सुस्मृति(net->inputs*net->batch, माप(भग्न));
    net->truth = सुस्मृति(net->truths*net->batch, माप(भग्न));
#अगर_घोषित GPU
    net->output_gpu = out.output_gpu;
    net->निवेश_gpu = cuda_make_array(net->निवेश, net->inputs*net->batch);
    net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
#पूर्ण_अगर
    अगर(workspace_आकार){
        //printf("%ld\n", workspace_size);
#अगर_घोषित GPU
        अगर(gpu_सूचकांक >= 0){
            net->workspace = cuda_make_array(0, (workspace_आकार-1)/माप(भग्न)+1);
        }अन्यथा {
            net->workspace = सुस्मृति(1, workspace_आकार);
        }
#अन्यथा
        net->workspace = सुस्मृति(1, workspace_आकार);
#पूर्ण_अगर
    }
    वापस net;
}

list *read_cfg(अक्षर *filename)
{
    खाता *file = ख_खोलो(filename, "r");
    अगर(file == 0) file_error(filename);
    अक्षर *पंक्ति;
    पूर्णांक nu = 0;
    list *options = make_list();
    section *current = 0;
    जबतक((पंक्ति=fgetl(file)) != 0){
        ++ nu;
        strip(पंक्ति);
        चयन(पंक्ति[0]){
            हाल '[':
                current = दो_स्मृति(माप(section));
                list_जोड़ो(options, current);
                current->options = make_list();
                current->type = पंक्ति;
                अवरोध;
            हाल '\0':
            हाल '#':
            हाल ';':
                मुक्त(पंक्ति);
                अवरोध;
            शेष:
                अगर(!read_option(पंक्ति, current->options)){
                    ख_लिखो(मानक_त्रुटि, "Config file error line %d, could parse: %s\n", nu, पंक्ति);
                    मुक्त(पंक्ति);
                }
                अवरोध;
        }
    }
    ख_बंद(file);
    वापस options;
}

व्योम save_convolutional_weights_द्विआधारी(layer l, खाता *fp)
{
#अगर_घोषित GPU
    अगर(gpu_सूचकांक >= 0){
        pull_convolutional_layer(l);
    }
#पूर्ण_अगर
    binarize_weights(l.weights, l.n, l.c*l.आकार*l.आकार, l.द्विआधारी_weights);
    पूर्णांक आकार = l.c*l.आकार*l.आकार;
    पूर्णांक i, j, k;
    ख_डालो(l.biases, माप(भग्न), l.n, fp);
    अगर (l.batch_normalize){
        ख_डालो(l.scales, माप(भग्न), l.n, fp);
        ख_डालो(l.rolling_mean, माप(भग्न), l.n, fp);
        ख_डालो(l.rolling_variance, माप(भग्न), l.n, fp);
    }
    क्रम(i = 0; i < l.n; ++i){
        भग्न mean = l.द्विआधारी_weights[i*आकार];
        अगर(mean < 0) mean = -mean;
        ख_डालो(&mean, माप(भग्न), 1, fp);
        क्रम(j = 0; j < आकार/8; ++j){
            पूर्णांक सूचकांक = i*आकार + j*8;
            अचिन्हित अक्षर c = 0;
            क्रम(k = 0; k < 8; ++k){
                अगर (j*8 + k >= आकार) अवरोध;
                अगर (l.द्विआधारी_weights[सूचकांक + k] > 0) c = (c | 1<<k);
            }
            ख_डालो(&c, माप(अक्षर), 1, fp);
        }
    }
}

व्योम save_convolutional_weights(layer l, खाता *fp)
{
    अगर(l.द्विआधारी){
        //save_convolutional_weights_binary(l, fp);
        //return;
    }
#अगर_घोषित GPU
    अगर(gpu_सूचकांक >= 0){
        pull_convolutional_layer(l);
    }
#पूर्ण_अगर
    पूर्णांक अंक = l.nweights;
    ख_डालो(l.biases, माप(भग्न), l.n, fp);
    अगर (l.batch_normalize){
        ख_डालो(l.scales, माप(भग्न), l.n, fp);
        ख_डालो(l.rolling_mean, माप(भग्न), l.n, fp);
        ख_डालो(l.rolling_variance, माप(भग्न), l.n, fp);
    }
    ख_डालो(l.weights, माप(भग्न), अंक, fp);
}

व्योम save_batchnorm_weights(layer l, खाता *fp)
{
#अगर_घोषित GPU
    अगर(gpu_सूचकांक >= 0){
        pull_batchnorm_layer(l);
    }
#पूर्ण_अगर
    ख_डालो(l.scales, माप(भग्न), l.c, fp);
    ख_डालो(l.rolling_mean, माप(भग्न), l.c, fp);
    ख_डालो(l.rolling_variance, माप(भग्न), l.c, fp);
}

व्योम save_connected_weights(layer l, खाता *fp)
{
#अगर_घोषित GPU
    अगर(gpu_सूचकांक >= 0){
        pull_connected_layer(l);
    }
#पूर्ण_अगर
    ख_डालो(l.biases, माप(भग्न), l.outputs, fp);
    ख_डालो(l.weights, माप(भग्न), l.outputs*l.inputs, fp);
    अगर (l.batch_normalize){
        ख_डालो(l.scales, माप(भग्न), l.outputs, fp);
        ख_डालो(l.rolling_mean, माप(भग्न), l.outputs, fp);
        ख_डालो(l.rolling_variance, माप(भग्न), l.outputs, fp);
    }
}

व्योम save_weights_upto(network *net, अक्षर *filename, पूर्णांक cutoff)
{
#अगर_घोषित GPU
    अगर(net->gpu_सूचकांक >= 0){
        cuda_set_device(net->gpu_सूचकांक);
    }
#पूर्ण_अगर
    ख_लिखो(मानक_त्रुटि, "Saving weights to %s\n", filename);
    खाता *fp = ख_खोलो(filename, "wb");
    अगर(!fp) file_error(filename);

    पूर्णांक major = 0;
    पूर्णांक minor = 2;
    पूर्णांक revision = 0;
    ख_डालो(&major, माप(पूर्णांक), 1, fp);
    ख_डालो(&minor, माप(पूर्णांक), 1, fp);
    ख_डालो(&revision, माप(पूर्णांक), 1, fp);
    ख_डालो(net->seen, माप(माप_प्रकार), 1, fp);

    पूर्णांक i;
    क्रम(i = 0; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        अगर (l.dontsave) जारी;
        अगर(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            save_convolutional_weights(l, fp);
        } अगर(l.type == CONNECTED){
            save_connected_weights(l, fp);
        } अगर(l.type == BATCHNORM){
            save_batchnorm_weights(l, fp);
        } अगर(l.type == RNN){
            save_connected_weights(*(l.निवेश_layer), fp);
            save_connected_weights(*(l.self_layer), fp);
            save_connected_weights(*(l.output_layer), fp);
        } अगर (l.type == LSTM) {
            save_connected_weights(*(l.wi), fp);
            save_connected_weights(*(l.wf), fp);
            save_connected_weights(*(l.wo), fp);
            save_connected_weights(*(l.wg), fp);
            save_connected_weights(*(l.ui), fp);
            save_connected_weights(*(l.uf), fp);
            save_connected_weights(*(l.uo), fp);
            save_connected_weights(*(l.ug), fp);
        } अगर (l.type == GRU) {
            अगर(1){
                save_connected_weights(*(l.wz), fp);
                save_connected_weights(*(l.wr), fp);
                save_connected_weights(*(l.wh), fp);
                save_connected_weights(*(l.uz), fp);
                save_connected_weights(*(l.ur), fp);
                save_connected_weights(*(l.uh), fp);
            }अन्यथा{
                save_connected_weights(*(l.reset_layer), fp);
                save_connected_weights(*(l.update_layer), fp);
                save_connected_weights(*(l.state_layer), fp);
            }
        }  अगर(l.type == CRNN){
            save_convolutional_weights(*(l.निवेश_layer), fp);
            save_convolutional_weights(*(l.self_layer), fp);
            save_convolutional_weights(*(l.output_layer), fp);
        } अगर(l.type == LOCAL){
#अगर_घोषित GPU
            अगर(gpu_सूचकांक >= 0){
                pull_local_layer(l);
            }
#पूर्ण_अगर
            पूर्णांक locations = l.out_w*l.out_h;
            पूर्णांक आकार = l.आकार*l.आकार*l.c*l.n*locations;
            ख_डालो(l.biases, माप(भग्न), l.outputs, fp);
            ख_डालो(l.weights, माप(भग्न), आकार, fp);
        }
    }
    ख_बंद(fp);
}
व्योम save_weights(network *net, अक्षर *filename)
{
    save_weights_upto(net, filename, net->n);
}

व्योम transpose_matrix(भग्न *a, पूर्णांक rows, पूर्णांक cols)
{
    भग्न *transpose = सुस्मृति(rows*cols, माप(भग्न));
    पूर्णांक x, y;
    क्रम(x = 0; x < rows; ++x){
        क्रम(y = 0; y < cols; ++y){
            transpose[y*rows + x] = a[x*cols + y];
        }
    }
    स_नकल(a, transpose, rows*cols*माप(भग्न));
    मुक्त(transpose);
}

व्योम load_connected_weights(layer l, खाता *fp, पूर्णांक transpose)
{
    ख_पढ़ो(l.biases, माप(भग्न), l.outputs, fp);
    ख_पढ़ो(l.weights, माप(भग्न), l.outputs*l.inputs, fp);
    अगर(transpose){
        transpose_matrix(l.weights, l.inputs, l.outputs);
    }
    //printf("Biases: %f mean %f variance\n", mean_array(l.biases, l.outputs), variance_array(l.biases, l.outputs));
    //printf("Weights: %f mean %f variance\n", mean_array(l.weights, l.outputs*l.inputs), variance_array(l.weights, l.outputs*l.inputs));
    अगर (l.batch_normalize && (!l.dontloadscales)){
        ख_पढ़ो(l.scales, माप(भग्न), l.outputs, fp);
        ख_पढ़ो(l.rolling_mean, माप(भग्न), l.outputs, fp);
        ख_पढ़ो(l.rolling_variance, माप(भग्न), l.outputs, fp);
        //printf("Scales: %f mean %f variance\n", mean_array(l.scales, l.outputs), variance_array(l.scales, l.outputs));
        //printf("rolling_mean: %f mean %f variance\n", mean_array(l.rolling_mean, l.outputs), variance_array(l.rolling_mean, l.outputs));
        //printf("rolling_variance: %f mean %f variance\n", mean_array(l.rolling_variance, l.outputs), variance_array(l.rolling_variance, l.outputs));
    }
#अगर_घोषित GPU
    अगर(gpu_सूचकांक >= 0){
        धकेलो_connected_layer(l);
    }
#पूर्ण_अगर
}

व्योम load_batchnorm_weights(layer l, खाता *fp)
{
    ख_पढ़ो(l.scales, माप(भग्न), l.c, fp);
    ख_पढ़ो(l.rolling_mean, माप(भग्न), l.c, fp);
    ख_पढ़ो(l.rolling_variance, माप(भग्न), l.c, fp);
#अगर_घोषित GPU
    अगर(gpu_सूचकांक >= 0){
        धकेलो_batchnorm_layer(l);
    }
#पूर्ण_अगर
}

व्योम load_convolutional_weights_द्विआधारी(layer l, खाता *fp)
{
    ख_पढ़ो(l.biases, माप(भग्न), l.n, fp);
    अगर (l.batch_normalize && (!l.dontloadscales)){
        ख_पढ़ो(l.scales, माप(भग्न), l.n, fp);
        ख_पढ़ो(l.rolling_mean, माप(भग्न), l.n, fp);
        ख_पढ़ो(l.rolling_variance, माप(भग्न), l.n, fp);
    }
    पूर्णांक आकार = l.c*l.आकार*l.आकार;
    पूर्णांक i, j, k;
    क्रम(i = 0; i < l.n; ++i){
        भग्न mean = 0;
        ख_पढ़ो(&mean, माप(भग्न), 1, fp);
        क्रम(j = 0; j < आकार/8; ++j){
            पूर्णांक सूचकांक = i*आकार + j*8;
            अचिन्हित अक्षर c = 0;
            ख_पढ़ो(&c, माप(अक्षर), 1, fp);
            क्रम(k = 0; k < 8; ++k){
                अगर (j*8 + k >= आकार) अवरोध;
                l.weights[सूचकांक + k] = (c & 1<<k) ? mean : -mean;
            }
        }
    }
#अगर_घोषित GPU
    अगर(gpu_सूचकांक >= 0){
        धकेलो_convolutional_layer(l);
    }
#पूर्ण_अगर
}

व्योम load_convolutional_weights(layer l, खाता *fp)
{
    अगर(l.द्विआधारी){
        //load_convolutional_weights_binary(l, fp);
        //return;
    }
    अगर(l.numload) l.n = l.numload;
    पूर्णांक अंक = l.c/l.groups*l.n*l.आकार*l.आकार;
    ख_पढ़ो(l.biases, माप(भग्न), l.n, fp);
    अगर (l.batch_normalize && (!l.dontloadscales)){
        ख_पढ़ो(l.scales, माप(भग्न), l.n, fp);
        ख_पढ़ो(l.rolling_mean, माप(भग्न), l.n, fp);
        ख_पढ़ो(l.rolling_variance, माप(भग्न), l.n, fp);
        अगर(0){
            पूर्णांक i;
            क्रम(i = 0; i < l.n; ++i){
                म_लिखो("%g, ", l.rolling_mean[i]);
            }
            म_लिखो("\n");
            क्रम(i = 0; i < l.n; ++i){
                म_लिखो("%g, ", l.rolling_variance[i]);
            }
            म_लिखो("\n");
        }
        अगर(0){
            fill_cpu(l.n, 0, l.rolling_mean, 1);
            fill_cpu(l.n, 0, l.rolling_variance, 1);
        }
        अगर(0){
            पूर्णांक i;
            क्रम(i = 0; i < l.n; ++i){
                म_लिखो("%g, ", l.rolling_mean[i]);
            }
            म_लिखो("\n");
            क्रम(i = 0; i < l.n; ++i){
                म_लिखो("%g, ", l.rolling_variance[i]);
            }
            म_लिखो("\n");
        }
    }
    ख_पढ़ो(l.weights, माप(भग्न), अंक, fp);
    //if(l.c == 3) scal_cpu(num, 1./256, l.weights, 1);
    अगर (l.flipped) {
        transpose_matrix(l.weights, l.c*l.आकार*l.आकार, l.n);
    }
    //if (l.binary) binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.weights);
#अगर_घोषित GPU
    अगर(gpu_सूचकांक >= 0){
        धकेलो_convolutional_layer(l);
    }
#पूर्ण_अगर
}


व्योम load_weights_upto(network *net, अक्षर *filename, पूर्णांक start, पूर्णांक cutoff)
{
#अगर_घोषित GPU
    अगर(net->gpu_सूचकांक >= 0){
        cuda_set_device(net->gpu_सूचकांक);
    }
#पूर्ण_अगर
    ख_लिखो(मानक_त्रुटि, "Loading weights from %s...", filename);
    ख_साफ(मानक_निकास);
    खाता *fp = ख_खोलो(filename, "rb");
    अगर(!fp) file_error(filename);

    पूर्णांक major;
    पूर्णांक minor;
    पूर्णांक revision;
    ख_पढ़ो(&major, माप(पूर्णांक), 1, fp);
    ख_पढ़ो(&minor, माप(पूर्णांक), 1, fp);
    ख_पढ़ो(&revision, माप(पूर्णांक), 1, fp);
    अगर ((major*10 + minor) >= 2 && major < 1000 && minor < 1000){
        ख_पढ़ो(net->seen, माप(माप_प्रकार), 1, fp);
    } अन्यथा {
        पूर्णांक iseen = 0;
        ख_पढ़ो(&iseen, माप(पूर्णांक), 1, fp);
        *net->seen = iseen;
    }
    पूर्णांक transpose = (major > 1000) || (minor > 1000);

    पूर्णांक i;
    क्रम(i = start; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        अगर (l.dontload) जारी;
        अगर(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            load_convolutional_weights(l, fp);
        }
        अगर(l.type == CONNECTED){
            load_connected_weights(l, fp, transpose);
        }
        अगर(l.type == BATCHNORM){
            load_batchnorm_weights(l, fp);
        }
        अगर(l.type == CRNN){
            load_convolutional_weights(*(l.निवेश_layer), fp);
            load_convolutional_weights(*(l.self_layer), fp);
            load_convolutional_weights(*(l.output_layer), fp);
        }
        अगर(l.type == RNN){
            load_connected_weights(*(l.निवेश_layer), fp, transpose);
            load_connected_weights(*(l.self_layer), fp, transpose);
            load_connected_weights(*(l.output_layer), fp, transpose);
        }
        अगर (l.type == LSTM) {
            load_connected_weights(*(l.wi), fp, transpose);
            load_connected_weights(*(l.wf), fp, transpose);
            load_connected_weights(*(l.wo), fp, transpose);
            load_connected_weights(*(l.wg), fp, transpose);
            load_connected_weights(*(l.ui), fp, transpose);
            load_connected_weights(*(l.uf), fp, transpose);
            load_connected_weights(*(l.uo), fp, transpose);
            load_connected_weights(*(l.ug), fp, transpose);
        }
        अगर (l.type == GRU) {
            अगर(1){
                load_connected_weights(*(l.wz), fp, transpose);
                load_connected_weights(*(l.wr), fp, transpose);
                load_connected_weights(*(l.wh), fp, transpose);
                load_connected_weights(*(l.uz), fp, transpose);
                load_connected_weights(*(l.ur), fp, transpose);
                load_connected_weights(*(l.uh), fp, transpose);
            }अन्यथा{
                load_connected_weights(*(l.reset_layer), fp, transpose);
                load_connected_weights(*(l.update_layer), fp, transpose);
                load_connected_weights(*(l.state_layer), fp, transpose);
            }
        }
        अगर(l.type == LOCAL){
            पूर्णांक locations = l.out_w*l.out_h;
            पूर्णांक आकार = l.आकार*l.आकार*l.c*l.n*locations;
            ख_पढ़ो(l.biases, माप(भग्न), l.outputs, fp);
            ख_पढ़ो(l.weights, माप(भग्न), आकार, fp);
#अगर_घोषित GPU
            अगर(gpu_सूचकांक >= 0){
                धकेलो_local_layer(l);
            }
#पूर्ण_अगर
        }
    }
    ख_लिखो(मानक_त्रुटि, "Done!\n");
    ख_बंद(fp);
}

व्योम load_weights(network *net, अक्षर *filename)
{
    load_weights_upto(net, filename, 0, net->n);
}


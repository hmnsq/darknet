#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <assert.h>

#অন্তর্ভাব "activation_layer.h"
#অন্তর্ভাব "logistic_layer.h"
#অন্তর্ভাব "l2norm_layer.h"
#অন্তর্ভাব "activations.h"
#অন্তর্ভাব "avgpool_layer.h"
#অন্তর্ভাব "batchnorm_layer.h"
#অন্তর্ভাব "blas.h"
#অন্তর্ভাব "connected_layer.h"
#অন্তর্ভাব "deconvolutional_layer.h"
#অন্তর্ভাব "convolutional_layer.h"
#অন্তর্ভাব "cost_layer.h"
#অন্তর্ভাব "crnn_layer.h"
#অন্তর্ভাব "crop_layer.h"
#অন্তর্ভাব "detection_layer.h"
#অন্তর্ভাব "dropout_layer.h"
#অন্তর্ভাব "gru_layer.h"
#অন্তর্ভাব "list.h"
#অন্তর্ভাব "local_layer.h"
#অন্তর্ভাব "maxpool_layer.h"
#অন্তর্ভাব "normalization_layer.h"
#অন্তর্ভাব "option_list.h"
#অন্তর্ভাব "parser.h"
#অন্তর্ভাব "region_layer.h"
#অন্তর্ভাব "yolo_layer.h"
#অন্তর্ভাব "iseg_layer.h"
#অন্তর্ভাব "reorg_layer.h"
#অন্তর্ভাব "rnn_layer.h"
#অন্তর্ভাব "route_layer.h"
#অন্তর্ভাব "upsample_layer.h"
#অন্তর্ভাব "shortcut_layer.h"
#অন্তর্ভাব "softmax_layer.h"
#অন্তর্ভাব "lstm_layer.h"
#অন্তর্ভাব "utils.h"

প্রকারনির্দেশ সংরচনা{
    বর্ণ *type;
    list *options;
}section;

list *read_cfg(বর্ণ *filename);

LAYER_TYPE string_to_layer_type(বর্ণ * type)
{

    যদি (strcmp(type, "[shortcut]")==0) প্রত্যাগম SHORTCUT;
    যদি (strcmp(type, "[crop]")==0) প্রত্যাগম CROP;
    যদি (strcmp(type, "[cost]")==0) প্রত্যাগম COST;
    যদি (strcmp(type, "[detection]")==0) প্রত্যাগম DETECTION;
    যদি (strcmp(type, "[region]")==0) প্রত্যাগম REGION;
    যদি (strcmp(type, "[yolo]")==0) প্রত্যাগম YOLO;
    যদি (strcmp(type, "[iseg]")==0) প্রত্যাগম ISEG;
    যদি (strcmp(type, "[local]")==0) প্রত্যাগম LOCAL;
    যদি (strcmp(type, "[conv]")==0
            || strcmp(type, "[convolutional]")==0) প্রত্যাগম CONVOLUTIONAL;
    যদি (strcmp(type, "[deconv]")==0
            || strcmp(type, "[deconvolutional]")==0) প্রত্যাগম DECONVOLUTIONAL;
    যদি (strcmp(type, "[activation]")==0) প্রত্যাগম ACTIVE;
    যদি (strcmp(type, "[logistic]")==0) প্রত্যাগম LOGXENT;
    যদি (strcmp(type, "[l2norm]")==0) প্রত্যাগম L2NORM;
    যদি (strcmp(type, "[net]")==0
            || strcmp(type, "[network]")==0) প্রত্যাগম NETWORK;
    যদি (strcmp(type, "[crnn]")==0) প্রত্যাগম CRNN;
    যদি (strcmp(type, "[gru]")==0) প্রত্যাগম GRU;
    যদি (strcmp(type, "[lstm]") == 0) প্রত্যাগম LSTM;
    যদি (strcmp(type, "[rnn]")==0) প্রত্যাগম RNN;
    যদি (strcmp(type, "[conn]")==0
            || strcmp(type, "[connected]")==0) প্রত্যাগম CONNECTED;
    যদি (strcmp(type, "[max]")==0
            || strcmp(type, "[maxpool]")==0) প্রত্যাগম MAXPOOL;
    যদি (strcmp(type, "[reorg]")==0) প্রত্যাগম REORG;
    যদি (strcmp(type, "[avg]")==0
            || strcmp(type, "[avgpool]")==0) প্রত্যাগম AVGPOOL;
    যদি (strcmp(type, "[dropout]")==0) প্রত্যাগম DROPOUT;
    যদি (strcmp(type, "[lrn]")==0
            || strcmp(type, "[normalization]")==0) প্রত্যাগম NORMALIZATION;
    যদি (strcmp(type, "[batchnorm]")==0) প্রত্যাগম BATCHNORM;
    যদি (strcmp(type, "[soft]")==0
            || strcmp(type, "[softmax]")==0) প্রত্যাগম SOFTMAX;
    যদি (strcmp(type, "[route]")==0) প্রত্যাগম ROUTE;
    যদি (strcmp(type, "[upsample]")==0) প্রত্যাগম UPSAMPLE;
    প্রত্যাগম BLANK;
}

শূন্য free_section(section *s)
{
    মুক্ত(s->type);
    গ্রন্থি *n = s->options->front;
    যাবত্(n){
        kvp *pair = (kvp *)n->val;
        মুক্ত(pair->চাবি);
        মুক্ত(pair);
        গ্রন্থি *next = n->next;
        মুক্ত(n);
        n = next;
    }
    মুক্ত(s->options);
    মুক্ত(s);
}

শূন্য parse_data(বর্ণ *data, ভিন্ন *a, পূর্ণাঙ্ক n)
{
    পূর্ণাঙ্ক i;
    যদি(!data) প্রত্যাগম;
    বর্ণ *curr = data;
    বর্ণ *next = data;
    পূর্ণাঙ্ক done = 0;
    প্রতি(i = 0; i < n && !done; ++i){
        যাবত্(*++next !='\0' && *next != ',');
        যদি(*next == '\0') done = 1;
        *next = '\0';
        sscanf(curr, "%g", &a[i]);
        curr = next+1;
    }
}

প্রকারনির্দেশ সংরচনা size_params{
    পূর্ণাঙ্ক batch;
    পূর্ণাঙ্ক inputs;
    পূর্ণাঙ্ক h;
    পূর্ণাঙ্ক w;
    পূর্ণাঙ্ক c;
    পূর্ণাঙ্ক index;
    পূর্ণাঙ্ক time_steps;
    network *net;
} size_params;

local_layer parse_local(list *options, size_params params)
{
    পূর্ণাঙ্ক n = option_find_int(options, "filters",1);
    পূর্ণাঙ্ক size = option_find_int(options, "size",1);
    পূর্ণাঙ্ক stride = option_find_int(options, "stride",1);
    পূর্ণাঙ্ক pad = option_find_int(options, "pad",0);
    বর্ণ *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    পূর্ণাঙ্ক batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    যদি(!(h && w && c)) error("Layer before local layer must output image.");

    local_layer layer = make_local_layer(batch,h,w,c,n,size,stride,pad,activation);

    প্রত্যাগম layer;
}

layer parse_deconvolutional(list *options, size_params params)
{
    পূর্ণাঙ্ক n = option_find_int(options, "filters",1);
    পূর্ণাঙ্ক size = option_find_int(options, "size",1);
    পূর্ণাঙ্ক stride = option_find_int(options, "stride",1);

    বর্ণ *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    পূর্ণাঙ্ক batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    যদি(!(h && w && c)) error("Layer before deconvolutional layer must output image.");
    পূর্ণাঙ্ক batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);
    পূর্ণাঙ্ক pad = option_find_int_quiet(options, "pad",0);
    পূর্ণাঙ্ক padding = option_find_int_quiet(options, "padding",0);
    যদি(pad) padding = size/2;

    layer l = make_deconvolutional_layer(batch,h,w,c,n,size,stride,padding, activation, batch_normalize, params.net->adam);

    প্রত্যাগম l;
}


convolutional_layer parse_convolutional(list *options, size_params params)
{
    পূর্ণাঙ্ক n = option_find_int(options, "filters",1);
    পূর্ণাঙ্ক size = option_find_int(options, "size",1);
    পূর্ণাঙ্ক stride = option_find_int(options, "stride",1);
    পূর্ণাঙ্ক pad = option_find_int_quiet(options, "pad",0);
    পূর্ণাঙ্ক padding = option_find_int_quiet(options, "padding",0);
    পূর্ণাঙ্ক groups = option_find_int_quiet(options, "groups", 1);
    যদি(pad) padding = size/2;

    বর্ণ *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    পূর্ণাঙ্ক batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    যদি(!(h && w && c)) error("Layer before convolutional layer must output image.");
    পূর্ণাঙ্ক batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);
    পূর্ণাঙ্ক দ্বিমিক = option_find_int_quiet(options, "binary", 0);
    পূর্ণাঙ্ক xnor = option_find_int_quiet(options, "xnor", 0);

    convolutional_layer layer = make_convolutional_layer(batch,h,w,c,n,groups,size,stride,padding,activation, batch_normalize, দ্বিমিক, xnor, params.net->adam);
    layer.flipped = option_find_int_quiet(options, "flipped", 0);
    layer.dot = option_find_float_quiet(options, "dot", 0);

    প্রত্যাগম layer;
}

layer parse_crnn(list *options, size_params params)
{
    পূর্ণাঙ্ক output_filters = option_find_int(options, "output_filters",1);
    পূর্ণাঙ্ক hidden_filters = option_find_int(options, "hidden_filters",1);
    বর্ণ *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    পূর্ণাঙ্ক batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_crnn_layer(params.batch, params.w, params.h, params.c, hidden_filters, output_filters, params.time_steps, activation, batch_normalize);

    l.shortcut = option_find_int_quiet(options, "shortcut", 0);

    প্রত্যাগম l;
}

layer parse_rnn(list *options, size_params params)
{
    পূর্ণাঙ্ক output = option_find_int(options, "output",1);
    বর্ণ *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    পূর্ণাঙ্ক batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_rnn_layer(params.batch, params.inputs, output, params.time_steps, activation, batch_normalize, params.net->adam);

    l.shortcut = option_find_int_quiet(options, "shortcut", 0);

    প্রত্যাগম l;
}

layer parse_gru(list *options, size_params params)
{
    পূর্ণাঙ্ক output = option_find_int(options, "output",1);
    পূর্ণাঙ্ক batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_gru_layer(params.batch, params.inputs, output, params.time_steps, batch_normalize, params.net->adam);
    l.tanh = option_find_int_quiet(options, "tanh", 0);

    প্রত্যাগম l;
}

layer parse_lstm(list *options, size_params params)
{
    পূর্ণাঙ্ক output = option_find_int(options, "output", 1);
    পূর্ণাঙ্ক batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_lstm_layer(params.batch, params.inputs, output, params.time_steps, batch_normalize, params.net->adam);

    প্রত্যাগম l;
}

layer parse_connected(list *options, size_params params)
{
    পূর্ণাঙ্ক output = option_find_int(options, "output",1);
    বর্ণ *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    পূর্ণাঙ্ক batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_connected_layer(params.batch, params.inputs, output, activation, batch_normalize, params.net->adam);
    প্রত্যাগম l;
}

layer parse_softmax(list *options, size_params params)
{
    পূর্ণাঙ্ক groups = option_find_int_quiet(options, "groups",1);
    layer l = make_softmax_layer(params.batch, params.inputs, groups);
    l.temperature = option_find_float_quiet(options, "temperature", 1);
    বর্ণ *বৃক্ষ_file = option_find_str(options, "tree", 0);
    যদি (বৃক্ষ_file) l.softmax_বৃক্ষ = read_বৃক্ষ(বৃক্ষ_file);
    l.w = params.w;
    l.h = params.h;
    l.c = params.c;
    l.spatial = option_find_float_quiet(options, "spatial", 0);
    l.noloss =  option_find_int_quiet(options, "noloss", 0);
    প্রত্যাগম l;
}

পূর্ণাঙ্ক *parse_yolo_mask(বর্ণ *a, পূর্ণাঙ্ক *অঙ্ক)
{
    পূর্ণাঙ্ক *mask = 0;
    যদি(a){
        পূর্ণাঙ্ক len = strlen(a);
        পূর্ণাঙ্ক n = 1;
        পূর্ণাঙ্ক i;
        প্রতি(i = 0; i < len; ++i){
            যদি (a[i] == ',') ++n;
        }
        mask = calloc(n, পরিমাণ(পূর্ণাঙ্ক));
        প্রতি(i = 0; i < n; ++i){
            পূর্ণাঙ্ক val = atoi(a);
            mask[i] = val;
            a = strchr(a, ',')+1;
        }
        *অঙ্ক = n;
    }
    প্রত্যাগম mask;
}

layer parse_yolo(list *options, size_params params)
{
    পূর্ণাঙ্ক classes = option_find_int(options, "classes", 20);
    পূর্ণাঙ্ক total = option_find_int(options, "num", 1);
    পূর্ণাঙ্ক অঙ্ক = total;

    বর্ণ *a = option_find_str(options, "mask", 0);
    পূর্ণাঙ্ক *mask = parse_yolo_mask(a, &অঙ্ক);
    layer l = make_yolo_layer(params.batch, params.w, params.h, অঙ্ক, total, mask, classes);
    প্রতিজ্ঞা(l.outputs == params.inputs);

    l.max_boxes = option_find_int_quiet(options, "max",90);
    l.jitter = option_find_float(options, "jitter", .2);

    l.ignore_thresh = option_find_float(options, "ignore_thresh", .5);
    l.truth_thresh = option_find_float(options, "truth_thresh", 1);
    l.random = option_find_int_quiet(options, "random", 0);

    বর্ণ *map_file = option_find_str(options, "map", 0);
    যদি (map_file) l.map = read_map(map_file);

    a = option_find_str(options, "anchors", 0);
    যদি(a){
        পূর্ণাঙ্ক len = strlen(a);
        পূর্ণাঙ্ক n = 1;
        পূর্ণাঙ্ক i;
        প্রতি(i = 0; i < len; ++i){
            যদি (a[i] == ',') ++n;
        }
        প্রতি(i = 0; i < n; ++i){
            ভিন্ন bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    প্রত্যাগম l;
}

layer parse_iseg(list *options, size_params params)
{
    পূর্ণাঙ্ক classes = option_find_int(options, "classes", 20);
    পূর্ণাঙ্ক ids = option_find_int(options, "ids", 32);
    layer l = make_iseg_layer(params.batch, params.w, params.h, classes, ids);
    প্রতিজ্ঞা(l.outputs == params.inputs);
    প্রত্যাগম l;
}

layer parse_region(list *options, size_params params)
{
    পূর্ণাঙ্ক coords = option_find_int(options, "coords", 4);
    পূর্ণাঙ্ক classes = option_find_int(options, "classes", 20);
    পূর্ণাঙ্ক অঙ্ক = option_find_int(options, "num", 1);

    layer l = make_region_layer(params.batch, params.w, params.h, অঙ্ক, classes, coords);
    প্রতিজ্ঞা(l.outputs == params.inputs);

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

    বর্ণ *বৃক্ষ_file = option_find_str(options, "tree", 0);
    যদি (বৃক্ষ_file) l.softmax_বৃক্ষ = read_বৃক্ষ(বৃক্ষ_file);
    বর্ণ *map_file = option_find_str(options, "map", 0);
    যদি (map_file) l.map = read_map(map_file);

    বর্ণ *a = option_find_str(options, "anchors", 0);
    যদি(a){
        পূর্ণাঙ্ক len = strlen(a);
        পূর্ণাঙ্ক n = 1;
        পূর্ণাঙ্ক i;
        প্রতি(i = 0; i < len; ++i){
            যদি (a[i] == ',') ++n;
        }
        প্রতি(i = 0; i < n; ++i){
            ভিন্ন bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    প্রত্যাগম l;
}

detection_layer parse_detection(list *options, size_params params)
{
    পূর্ণাঙ্ক coords = option_find_int(options, "coords", 1);
    পূর্ণাঙ্ক classes = option_find_int(options, "classes", 1);
    পূর্ণাঙ্ক rescore = option_find_int(options, "rescore", 0);
    পূর্ণাঙ্ক অঙ্ক = option_find_int(options, "num", 1);
    পূর্ণাঙ্ক side = option_find_int(options, "side", 7);
    detection_layer layer = make_detection_layer(params.batch, params.inputs, অঙ্ক, side, classes, coords, rescore);

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
    প্রত্যাগম layer;
}

cost_layer parse_cost(list *options, size_params params)
{
    বর্ণ *type_s = option_find_str(options, "type", "sse");
    COST_TYPE type = get_cost_type(type_s);
    ভিন্ন scale = option_find_float_quiet(options, "scale",1);
    cost_layer layer = make_cost_layer(params.batch, params.inputs, type, scale);
    layer.ratio =  option_find_float_quiet(options, "ratio",0);
    layer.noobject_scale =  option_find_float_quiet(options, "noobj", 1);
    layer.thresh =  option_find_float_quiet(options, "thresh",0);
    প্রত্যাগম layer;
}

crop_layer parse_crop(list *options, size_params params)
{
    পূর্ণাঙ্ক crop_height = option_find_int(options, "crop_height",1);
    পূর্ণাঙ্ক crop_width = option_find_int(options, "crop_width",1);
    পূর্ণাঙ্ক flip = option_find_int(options, "flip",0);
    ভিন্ন angle = option_find_float(options, "angle",0);
    ভিন্ন saturation = option_find_float(options, "saturation",1);
    ভিন্ন exposure = option_find_float(options, "exposure",1);

    পূর্ণাঙ্ক batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    যদি(!(h && w && c)) error("Layer before crop layer must output image.");

    পূর্ণাঙ্ক noadjust = option_find_int_quiet(options, "noadjust",0);

    crop_layer l = make_crop_layer(batch,h,w,c,crop_height,crop_width,flip, angle, saturation, exposure);
    l.shift = option_find_float(options, "shift", 0);
    l.noadjust = noadjust;
    প্রত্যাগম l;
}

layer parse_reorg(list *options, size_params params)
{
    পূর্ণাঙ্ক stride = option_find_int(options, "stride",1);
    পূর্ণাঙ্ক reverse = option_find_int_quiet(options, "reverse",0);
    পূর্ণাঙ্ক flatten = option_find_int_quiet(options, "flatten",0);
    পূর্ণাঙ্ক extra = option_find_int_quiet(options, "extra",0);

    পূর্ণাঙ্ক batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    যদি(!(h && w && c)) error("Layer before reorg layer must output image.");

    layer layer = make_reorg_layer(batch,w,h,c,stride,reverse, flatten, extra);
    প্রত্যাগম layer;
}

maxpool_layer parse_maxpool(list *options, size_params params)
{
    পূর্ণাঙ্ক stride = option_find_int(options, "stride",1);
    পূর্ণাঙ্ক size = option_find_int(options, "size",stride);
    পূর্ণাঙ্ক padding = option_find_int_quiet(options, "padding", size-1);

    পূর্ণাঙ্ক batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    যদি(!(h && w && c)) error("Layer before maxpool layer must output image.");

    maxpool_layer layer = make_maxpool_layer(batch,h,w,c,size,stride,padding);
    প্রত্যাগম layer;
}

avgpool_layer parse_avgpool(list *options, size_params params)
{
    পূর্ণাঙ্ক batch,w,h,c;
    w = params.w;
    h = params.h;
    c = params.c;
    batch=params.batch;
    যদি(!(h && w && c)) error("Layer before avgpool layer must output image.");

    avgpool_layer layer = make_avgpool_layer(batch,w,h,c);
    প্রত্যাগম layer;
}

dropout_layer parse_dropout(list *options, size_params params)
{
    ভিন্ন probability = option_find_float(options, "probability", .5);
    dropout_layer layer = make_dropout_layer(params.batch, params.inputs, probability);
    layer.out_w = params.w;
    layer.out_h = params.h;
    layer.out_c = params.c;
    প্রত্যাগম layer;
}

layer parse_normalization(list *options, size_params params)
{
    ভিন্ন alpha = option_find_float(options, "alpha", .0001);
    ভিন্ন beta =  option_find_float(options, "beta" , .75);
    ভিন্ন kappa = option_find_float(options, "kappa", 1);
    পূর্ণাঙ্ক size = option_find_int(options, "size", 5);
    layer l = make_normalization_layer(params.batch, params.w, params.h, params.c, size, alpha, beta, kappa);
    প্রত্যাগম l;
}

layer parse_batchnorm(list *options, size_params params)
{
    layer l = make_batchnorm_layer(params.batch, params.w, params.h, params.c);
    প্রত্যাগম l;
}

layer parse_shortcut(list *options, size_params params, network *net)
{
    বর্ণ *l = option_find(options, "from");
    পূর্ণাঙ্ক index = atoi(l);
    যদি(index < 0) index = params.index + index;

    পূর্ণাঙ্ক batch = params.batch;
    layer from = net->layers[index];

    layer s = make_shortcut_layer(batch, index, params.w, params.h, params.c, from.out_w, from.out_h, from.out_c);

    বর্ণ *activation_s = option_find_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);
    s.activation = activation;
    s.alpha = option_find_float_quiet(options, "alpha", 1);
    s.beta = option_find_float_quiet(options, "beta", 1);
    প্রত্যাগম s;
}


layer parse_l2norm(list *options, size_params params)
{
    layer l = make_l2norm_layer(params.batch, params.inputs);
    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;
    প্রত্যাগম l;
}


layer parse_logistic(list *options, size_params params)
{
    layer l = make_logistic_layer(params.batch, params.inputs);
    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;
    প্রত্যাগম l;
}

layer parse_activation(list *options, size_params params)
{
    বর্ণ *activation_s = option_find_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);

    layer l = make_activation_layer(params.batch, params.inputs, activation);

    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;

    প্রত্যাগম l;
}

layer parse_upsample(list *options, size_params params, network *net)
{

    পূর্ণাঙ্ক stride = option_find_int(options, "stride",2);
    layer l = make_upsample_layer(params.batch, params.w, params.h, params.c, stride);
    l.scale = option_find_float_quiet(options, "scale", 1);
    প্রত্যাগম l;
}

route_layer parse_route(list *options, size_params params, network *net)
{
    বর্ণ *l = option_find(options, "layers");
    পূর্ণাঙ্ক len = strlen(l);
    যদি(!l) error("Route Layer must specify input layers");
    পূর্ণাঙ্ক n = 1;
    পূর্ণাঙ্ক i;
    প্রতি(i = 0; i < len; ++i){
        যদি (l[i] == ',') ++n;
    }

    পূর্ণাঙ্ক *layers = calloc(n, পরিমাণ(পূর্ণাঙ্ক));
    পূর্ণাঙ্ক *sizes = calloc(n, পরিমাণ(পূর্ণাঙ্ক));
    প্রতি(i = 0; i < n; ++i){
        পূর্ণাঙ্ক index = atoi(l);
        l = strchr(l, ',')+1;
        যদি(index < 0) index = params.index + index;
        layers[i] = index;
        sizes[i] = net->layers[index].outputs;
    }
    পূর্ণাঙ্ক batch = params.batch;

    route_layer layer = make_route_layer(batch, n, layers, sizes);

    convolutional_layer first = net->layers[layers[0]];
    layer.out_w = first.out_w;
    layer.out_h = first.out_h;
    layer.out_c = first.out_c;
    প্রতি(i = 1; i < n; ++i){
        পূর্ণাঙ্ক index = layers[i];
        convolutional_layer next = net->layers[index];
        যদি(next.out_w == first.out_w && next.out_h == first.out_h){
            layer.out_c += next.out_c;
        }অন্যথা{
            layer.out_h = layer.out_w = layer.out_c = 0;
        }
    }

    প্রত্যাগম layer;
}

learning_rate_policy get_policy(বর্ণ *s)
{
    যদি (strcmp(s, "random")==0) প্রত্যাগম RANDOM;
    যদি (strcmp(s, "poly")==0) প্রত্যাগম POLY;
    যদি (strcmp(s, "constant")==0) প্রত্যাগম CONSTANT;
    যদি (strcmp(s, "step")==0) প্রত্যাগম STEP;
    যদি (strcmp(s, "exp")==0) প্রত্যাগম EXP;
    যদি (strcmp(s, "sigmoid")==0) প্রত্যাগম SIG;
    যদি (strcmp(s, "steps")==0) প্রত্যাগম STEPS;
    fprintf(stderr, "Couldn't find policy %s, going with constant\n", s);
    প্রত্যাগম CONSTANT;
}

শূন্য parse_net_options(list *options, network *net)
{
    net->batch = option_find_int(options, "batch",1);
    net->learning_rate = option_find_float(options, "learning_rate", .001);
    net->momentum = option_find_float(options, "momentum", .9);
    net->decay = option_find_float(options, "decay", .0001);
    পূর্ণাঙ্ক subdivs = option_find_int(options, "subdivisions",1);
    net->time_steps = option_find_int_quiet(options, "time_steps",1);
    net->notruth = option_find_int_quiet(options, "notruth",0);
    net->batch /= subdivs;
    net->batch *= net->time_steps;
    net->subdivisions = subdivs;
    net->random = option_find_int_quiet(options, "random", 0);

    net->adam = option_find_int_quiet(options, "adam", 0);
    যদি(net->adam){
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
    net->max_ratio = option_find_float_quiet(options, "max_ratio", (ভিন্ন) net->max_crop / net->w);
    net->min_ratio = option_find_float_quiet(options, "min_ratio", (ভিন্ন) net->min_crop / net->w);
    net->center = option_find_int_quiet(options, "center",0);
    net->clip = option_find_float_quiet(options, "clip", 0);

    net->angle = option_find_float_quiet(options, "angle", 0);
    net->aspect = option_find_float_quiet(options, "aspect", 1);
    net->saturation = option_find_float_quiet(options, "saturation", 1);
    net->exposure = option_find_float_quiet(options, "exposure", 1);
    net->hue = option_find_float_quiet(options, "hue", 0);

    যদি(!net->inputs && !(net->h && net->w && net->c)) error("No input parameters supplied");

    বর্ণ *policy_s = option_find_str(options, "policy", "constant");
    net->policy = get_policy(policy_s);
    net->burn_in = option_find_int_quiet(options, "burn_in", 0);
    net->power = option_find_float_quiet(options, "power", 4);
    যদি(net->policy == STEP){
        net->step = option_find_int(options, "step", 1);
        net->scale = option_find_float(options, "scale", 1);
    } অন্যথা যদি (net->policy == STEPS){
        বর্ণ *l = option_find(options, "steps");
        বর্ণ *p = option_find(options, "scales");
        যদি(!l || !p) error("STEPS policy must have steps and scales in cfg file");

        পূর্ণাঙ্ক len = strlen(l);
        পূর্ণাঙ্ক n = 1;
        পূর্ণাঙ্ক i;
        প্রতি(i = 0; i < len; ++i){
            যদি (l[i] == ',') ++n;
        }
        পূর্ণাঙ্ক *steps = calloc(n, পরিমাণ(পূর্ণাঙ্ক));
        ভিন্ন *scales = calloc(n, পরিমাণ(ভিন্ন));
        প্রতি(i = 0; i < n; ++i){
            পূর্ণাঙ্ক step    = atoi(l);
            ভিন্ন scale = atof(p);
            l = strchr(l, ',')+1;
            p = strchr(p, ',')+1;
            steps[i] = step;
            scales[i] = scale;
        }
        net->scales = scales;
        net->steps = steps;
        net->অঙ্ক_steps = n;
    } অন্যথা যদি (net->policy == EXP){
        net->gamma = option_find_float(options, "gamma", 1);
    } অন্যথা যদি (net->policy == SIG){
        net->gamma = option_find_float(options, "gamma", 1);
        net->step = option_find_int(options, "step", 1);
    } অন্যথা যদি (net->policy == POLY || net->policy == RANDOM){
    }
    net->max_batches = option_find_int(options, "max_batches", 0);
}

পূর্ণাঙ্ক is_network(section *s)
{
    প্রত্যাগম (strcmp(s->type, "[net]")==0
            || strcmp(s->type, "[network]")==0);
}

network *parse_network_cfg(বর্ণ *filename)
{
    list *sections = read_cfg(filename);
    গ্রন্থি *n = sections->front;
    যদি(!n) error("Config file has no sections");
    network *net = make_network(sections->size - 1);
    net->gpu_index = gpu_index;
    size_params params;

    section *s = (section *)n->val;
    list *options = s->options;
    যদি(!is_network(s)) error("First section must be [net] or [network]");
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
    পূর্ণাঙ্ক গুনতি = 0;
    free_section(s);
    fprintf(stderr, "layer     filters    size              input                output\n");
    যাবত্(n){
        params.index = গুনতি;
        fprintf(stderr, "%5d ", গুনতি);
        s = (section *)n->val;
        options = s->options;
        layer l = {0};
        LAYER_TYPE lt = string_to_layer_type(s->type);
        যদি(lt == CONVOLUTIONAL){
            l = parse_convolutional(options, params);
        }অন্যথা যদি(lt == DECONVOLUTIONAL){
            l = parse_deconvolutional(options, params);
        }অন্যথা যদি(lt == LOCAL){
            l = parse_local(options, params);
        }অন্যথা যদি(lt == ACTIVE){
            l = parse_activation(options, params);
        }অন্যথা যদি(lt == LOGXENT){
            l = parse_logistic(options, params);
        }অন্যথা যদি(lt == L2NORM){
            l = parse_l2norm(options, params);
        }অন্যথা যদি(lt == RNN){
            l = parse_rnn(options, params);
        }অন্যথা যদি(lt == GRU){
            l = parse_gru(options, params);
        }অন্যথা যদি (lt == LSTM) {
            l = parse_lstm(options, params);
        }অন্যথা যদি(lt == CRNN){
            l = parse_crnn(options, params);
        }অন্যথা যদি(lt == CONNECTED){
            l = parse_connected(options, params);
        }অন্যথা যদি(lt == CROP){
            l = parse_crop(options, params);
        }অন্যথা যদি(lt == COST){
            l = parse_cost(options, params);
        }অন্যথা যদি(lt == REGION){
            l = parse_region(options, params);
        }অন্যথা যদি(lt == YOLO){
            l = parse_yolo(options, params);
        }অন্যথা যদি(lt == ISEG){
            l = parse_iseg(options, params);
        }অন্যথা যদি(lt == DETECTION){
            l = parse_detection(options, params);
        }অন্যথা যদি(lt == SOFTMAX){
            l = parse_softmax(options, params);
            net->hierarchy = l.softmax_বৃক্ষ;
        }অন্যথা যদি(lt == NORMALIZATION){
            l = parse_normalization(options, params);
        }অন্যথা যদি(lt == BATCHNORM){
            l = parse_batchnorm(options, params);
        }অন্যথা যদি(lt == MAXPOOL){
            l = parse_maxpool(options, params);
        }অন্যথা যদি(lt == REORG){
            l = parse_reorg(options, params);
        }অন্যথা যদি(lt == AVGPOOL){
            l = parse_avgpool(options, params);
        }অন্যথা যদি(lt == ROUTE){
            l = parse_route(options, params, net);
        }অন্যথা যদি(lt == UPSAMPLE){
            l = parse_upsample(options, params, net);
        }অন্যথা যদি(lt == SHORTCUT){
            l = parse_shortcut(options, params, net);
        }অন্যথা যদি(lt == DROPOUT){
            l = parse_dropout(options, params);
            l.output = net->layers[গুনতি-1].output;
            l.delta = net->layers[গুনতি-1].delta;
#ifdef GPU
            l.output_gpu = net->layers[গুনতি-1].output_gpu;
            l.delta_gpu = net->layers[গুনতি-1].delta_gpu;
#endif
        }অন্যথা{
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
        net->layers[গুনতি] = l;
        যদি (l.workspace_size > workspace_size) workspace_size = l.workspace_size;
        free_section(s);
        n = n->next;
        ++গুনতি;
        যদি(n){
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
    যদি(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    net->প্রবেশ = calloc(net->inputs*net->batch, পরিমাণ(ভিন্ন));
    net->truth = calloc(net->truths*net->batch, পরিমাণ(ভিন্ন));
#ifdef GPU
    net->output_gpu = out.output_gpu;
    net->প্রবেশ_gpu = cuda_make_array(net->প্রবেশ, net->inputs*net->batch);
    net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
#endif
    যদি(workspace_size){
        //printf("%ld\n", workspace_size);
#ifdef GPU
        যদি(gpu_index >= 0){
            net->workspace = cuda_make_array(0, (workspace_size-1)/পরিমাণ(ভিন্ন)+1);
        }অন্যথা {
            net->workspace = calloc(1, workspace_size);
        }
#অন্যথা
        net->workspace = calloc(1, workspace_size);
#endif
    }
    প্রত্যাগম net;
}

list *read_cfg(বর্ণ *filename)
{
    FILE *file = fopen(filename, "r");
    যদি(file == 0) file_error(filename);
    বর্ণ *সারি;
    পূর্ণাঙ্ক nu = 0;
    list *options = make_list();
    section *current = 0;
    যাবত্((সারি=fgetl(file)) != 0){
        ++ nu;
        strip(সারি);
        বিকল্প(সারি[0]){
            পক্ষ '[':
                current = malloc(পরিমাণ(section));
                list_ঢোকাও(options, current);
                current->options = make_list();
                current->type = সারি;
                ভঙ্গ;
            পক্ষ '\0':
            পক্ষ '#':
            পক্ষ ';':
                মুক্ত(সারি);
                ভঙ্গ;
            স্বভাব:
                যদি(!read_option(সারি, current->options)){
                    fprintf(stderr, "Config file error line %d, could parse: %s\n", nu, সারি);
                    মুক্ত(সারি);
                }
                ভঙ্গ;
        }
    }
    fclose(file);
    প্রত্যাগম options;
}

শূন্য save_convolutional_weights_দ্বিমিক(layer l, FILE *fp)
{
#ifdef GPU
    যদি(gpu_index >= 0){
        pull_convolutional_layer(l);
    }
#endif
    binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.দ্বিমিক_weights);
    পূর্ণাঙ্ক size = l.c*l.size*l.size;
    পূর্ণাঙ্ক i, j, k;
    fwrite(l.biases, পরিমাণ(ভিন্ন), l.n, fp);
    যদি (l.batch_normalize){
        fwrite(l.scales, পরিমাণ(ভিন্ন), l.n, fp);
        fwrite(l.rolling_mean, পরিমাণ(ভিন্ন), l.n, fp);
        fwrite(l.rolling_variance, পরিমাণ(ভিন্ন), l.n, fp);
    }
    প্রতি(i = 0; i < l.n; ++i){
        ভিন্ন mean = l.দ্বিমিক_weights[i*size];
        যদি(mean < 0) mean = -mean;
        fwrite(&mean, পরিমাণ(ভিন্ন), 1, fp);
        প্রতি(j = 0; j < size/8; ++j){
            পূর্ণাঙ্ক index = i*size + j*8;
            unsigned বর্ণ c = 0;
            প্রতি(k = 0; k < 8; ++k){
                যদি (j*8 + k >= size) ভঙ্গ;
                যদি (l.দ্বিমিক_weights[index + k] > 0) c = (c | 1<<k);
            }
            fwrite(&c, পরিমাণ(বর্ণ), 1, fp);
        }
    }
}

শূন্য save_convolutional_weights(layer l, FILE *fp)
{
    যদি(l.দ্বিমিক){
        //save_convolutional_weights_binary(l, fp);
        //return;
    }
#ifdef GPU
    যদি(gpu_index >= 0){
        pull_convolutional_layer(l);
    }
#endif
    পূর্ণাঙ্ক অঙ্ক = l.nweights;
    fwrite(l.biases, পরিমাণ(ভিন্ন), l.n, fp);
    যদি (l.batch_normalize){
        fwrite(l.scales, পরিমাণ(ভিন্ন), l.n, fp);
        fwrite(l.rolling_mean, পরিমাণ(ভিন্ন), l.n, fp);
        fwrite(l.rolling_variance, পরিমাণ(ভিন্ন), l.n, fp);
    }
    fwrite(l.weights, পরিমাণ(ভিন্ন), অঙ্ক, fp);
}

শূন্য save_batchnorm_weights(layer l, FILE *fp)
{
#ifdef GPU
    যদি(gpu_index >= 0){
        pull_batchnorm_layer(l);
    }
#endif
    fwrite(l.scales, পরিমাণ(ভিন্ন), l.c, fp);
    fwrite(l.rolling_mean, পরিমাণ(ভিন্ন), l.c, fp);
    fwrite(l.rolling_variance, পরিমাণ(ভিন্ন), l.c, fp);
}

শূন্য save_connected_weights(layer l, FILE *fp)
{
#ifdef GPU
    যদি(gpu_index >= 0){
        pull_connected_layer(l);
    }
#endif
    fwrite(l.biases, পরিমাণ(ভিন্ন), l.outputs, fp);
    fwrite(l.weights, পরিমাণ(ভিন্ন), l.outputs*l.inputs, fp);
    যদি (l.batch_normalize){
        fwrite(l.scales, পরিমাণ(ভিন্ন), l.outputs, fp);
        fwrite(l.rolling_mean, পরিমাণ(ভিন্ন), l.outputs, fp);
        fwrite(l.rolling_variance, পরিমাণ(ভিন্ন), l.outputs, fp);
    }
}

শূন্য save_weights_upto(network *net, বর্ণ *filename, পূর্ণাঙ্ক cutoff)
{
#ifdef GPU
    যদি(net->gpu_index >= 0){
        cuda_set_device(net->gpu_index);
    }
#endif
    fprintf(stderr, "Saving weights to %s\n", filename);
    FILE *fp = fopen(filename, "wb");
    যদি(!fp) file_error(filename);

    পূর্ণাঙ্ক major = 0;
    পূর্ণাঙ্ক minor = 2;
    পূর্ণাঙ্ক revision = 0;
    fwrite(&major, পরিমাণ(পূর্ণাঙ্ক), 1, fp);
    fwrite(&minor, পরিমাণ(পূর্ণাঙ্ক), 1, fp);
    fwrite(&revision, পরিমাণ(পূর্ণাঙ্ক), 1, fp);
    fwrite(net->seen, পরিমাণ(size_t), 1, fp);

    পূর্ণাঙ্ক i;
    প্রতি(i = 0; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        যদি (l.dontsave) অনুবর্ত;
        যদি(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            save_convolutional_weights(l, fp);
        } যদি(l.type == CONNECTED){
            save_connected_weights(l, fp);
        } যদি(l.type == BATCHNORM){
            save_batchnorm_weights(l, fp);
        } যদি(l.type == RNN){
            save_connected_weights(*(l.প্রবেশ_layer), fp);
            save_connected_weights(*(l.self_layer), fp);
            save_connected_weights(*(l.output_layer), fp);
        } যদি (l.type == LSTM) {
            save_connected_weights(*(l.wi), fp);
            save_connected_weights(*(l.wf), fp);
            save_connected_weights(*(l.wo), fp);
            save_connected_weights(*(l.wg), fp);
            save_connected_weights(*(l.ui), fp);
            save_connected_weights(*(l.uf), fp);
            save_connected_weights(*(l.uo), fp);
            save_connected_weights(*(l.ug), fp);
        } যদি (l.type == GRU) {
            যদি(1){
                save_connected_weights(*(l.wz), fp);
                save_connected_weights(*(l.wr), fp);
                save_connected_weights(*(l.wh), fp);
                save_connected_weights(*(l.uz), fp);
                save_connected_weights(*(l.ur), fp);
                save_connected_weights(*(l.uh), fp);
            }অন্যথা{
                save_connected_weights(*(l.reset_layer), fp);
                save_connected_weights(*(l.update_layer), fp);
                save_connected_weights(*(l.state_layer), fp);
            }
        }  যদি(l.type == CRNN){
            save_convolutional_weights(*(l.প্রবেশ_layer), fp);
            save_convolutional_weights(*(l.self_layer), fp);
            save_convolutional_weights(*(l.output_layer), fp);
        } যদি(l.type == LOCAL){
#ifdef GPU
            যদি(gpu_index >= 0){
                pull_local_layer(l);
            }
#endif
            পূর্ণাঙ্ক locations = l.out_w*l.out_h;
            পূর্ণাঙ্ক size = l.size*l.size*l.c*l.n*locations;
            fwrite(l.biases, পরিমাণ(ভিন্ন), l.outputs, fp);
            fwrite(l.weights, পরিমাণ(ভিন্ন), size, fp);
        }
    }
    fclose(fp);
}
শূন্য save_weights(network *net, বর্ণ *filename)
{
    save_weights_upto(net, filename, net->n);
}

শূন্য transpose_matrix(ভিন্ন *a, পূর্ণাঙ্ক rows, পূর্ণাঙ্ক cols)
{
    ভিন্ন *transpose = calloc(rows*cols, পরিমাণ(ভিন্ন));
    পূর্ণাঙ্ক x, y;
    প্রতি(x = 0; x < rows; ++x){
        প্রতি(y = 0; y < cols; ++y){
            transpose[y*rows + x] = a[x*cols + y];
        }
    }
    memcpy(a, transpose, rows*cols*পরিমাণ(ভিন্ন));
    মুক্ত(transpose);
}

শূন্য load_connected_weights(layer l, FILE *fp, পূর্ণাঙ্ক transpose)
{
    fread(l.biases, পরিমাণ(ভিন্ন), l.outputs, fp);
    fread(l.weights, পরিমাণ(ভিন্ন), l.outputs*l.inputs, fp);
    যদি(transpose){
        transpose_matrix(l.weights, l.inputs, l.outputs);
    }
    //printf("Biases: %f mean %f variance\n", mean_array(l.biases, l.outputs), variance_array(l.biases, l.outputs));
    //printf("Weights: %f mean %f variance\n", mean_array(l.weights, l.outputs*l.inputs), variance_array(l.weights, l.outputs*l.inputs));
    যদি (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, পরিমাণ(ভিন্ন), l.outputs, fp);
        fread(l.rolling_mean, পরিমাণ(ভিন্ন), l.outputs, fp);
        fread(l.rolling_variance, পরিমাণ(ভিন্ন), l.outputs, fp);
        //printf("Scales: %f mean %f variance\n", mean_array(l.scales, l.outputs), variance_array(l.scales, l.outputs));
        //printf("rolling_mean: %f mean %f variance\n", mean_array(l.rolling_mean, l.outputs), variance_array(l.rolling_mean, l.outputs));
        //printf("rolling_variance: %f mean %f variance\n", mean_array(l.rolling_variance, l.outputs), variance_array(l.rolling_variance, l.outputs));
    }
#ifdef GPU
    যদি(gpu_index >= 0){
        ঠেলো_connected_layer(l);
    }
#endif
}

শূন্য load_batchnorm_weights(layer l, FILE *fp)
{
    fread(l.scales, পরিমাণ(ভিন্ন), l.c, fp);
    fread(l.rolling_mean, পরিমাণ(ভিন্ন), l.c, fp);
    fread(l.rolling_variance, পরিমাণ(ভিন্ন), l.c, fp);
#ifdef GPU
    যদি(gpu_index >= 0){
        ঠেলো_batchnorm_layer(l);
    }
#endif
}

শূন্য load_convolutional_weights_দ্বিমিক(layer l, FILE *fp)
{
    fread(l.biases, পরিমাণ(ভিন্ন), l.n, fp);
    যদি (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, পরিমাণ(ভিন্ন), l.n, fp);
        fread(l.rolling_mean, পরিমাণ(ভিন্ন), l.n, fp);
        fread(l.rolling_variance, পরিমাণ(ভিন্ন), l.n, fp);
    }
    পূর্ণাঙ্ক size = l.c*l.size*l.size;
    পূর্ণাঙ্ক i, j, k;
    প্রতি(i = 0; i < l.n; ++i){
        ভিন্ন mean = 0;
        fread(&mean, পরিমাণ(ভিন্ন), 1, fp);
        প্রতি(j = 0; j < size/8; ++j){
            পূর্ণাঙ্ক index = i*size + j*8;
            unsigned বর্ণ c = 0;
            fread(&c, পরিমাণ(বর্ণ), 1, fp);
            প্রতি(k = 0; k < 8; ++k){
                যদি (j*8 + k >= size) ভঙ্গ;
                l.weights[index + k] = (c & 1<<k) ? mean : -mean;
            }
        }
    }
#ifdef GPU
    যদি(gpu_index >= 0){
        ঠেলো_convolutional_layer(l);
    }
#endif
}

শূন্য load_convolutional_weights(layer l, FILE *fp)
{
    যদি(l.দ্বিমিক){
        //load_convolutional_weights_binary(l, fp);
        //return;
    }
    যদি(l.numload) l.n = l.numload;
    পূর্ণাঙ্ক অঙ্ক = l.c/l.groups*l.n*l.size*l.size;
    fread(l.biases, পরিমাণ(ভিন্ন), l.n, fp);
    যদি (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, পরিমাণ(ভিন্ন), l.n, fp);
        fread(l.rolling_mean, পরিমাণ(ভিন্ন), l.n, fp);
        fread(l.rolling_variance, পরিমাণ(ভিন্ন), l.n, fp);
        যদি(0){
            পূর্ণাঙ্ক i;
            প্রতি(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_mean[i]);
            }
            printf("\n");
            প্রতি(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_variance[i]);
            }
            printf("\n");
        }
        যদি(0){
            fill_cpu(l.n, 0, l.rolling_mean, 1);
            fill_cpu(l.n, 0, l.rolling_variance, 1);
        }
        যদি(0){
            পূর্ণাঙ্ক i;
            প্রতি(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_mean[i]);
            }
            printf("\n");
            প্রতি(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_variance[i]);
            }
            printf("\n");
        }
    }
    fread(l.weights, পরিমাণ(ভিন্ন), অঙ্ক, fp);
    //if(l.c == 3) scal_cpu(num, 1./256, l.weights, 1);
    যদি (l.flipped) {
        transpose_matrix(l.weights, l.c*l.size*l.size, l.n);
    }
    //if (l.binary) binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.weights);
#ifdef GPU
    যদি(gpu_index >= 0){
        ঠেলো_convolutional_layer(l);
    }
#endif
}


শূন্য load_weights_upto(network *net, বর্ণ *filename, পূর্ণাঙ্ক start, পূর্ণাঙ্ক cutoff)
{
#ifdef GPU
    যদি(net->gpu_index >= 0){
        cuda_set_device(net->gpu_index);
    }
#endif
    fprintf(stderr, "Loading weights from %s...", filename);
    fflush(stdout);
    FILE *fp = fopen(filename, "rb");
    যদি(!fp) file_error(filename);

    পূর্ণাঙ্ক major;
    পূর্ণাঙ্ক minor;
    পূর্ণাঙ্ক revision;
    fread(&major, পরিমাণ(পূর্ণাঙ্ক), 1, fp);
    fread(&minor, পরিমাণ(পূর্ণাঙ্ক), 1, fp);
    fread(&revision, পরিমাণ(পূর্ণাঙ্ক), 1, fp);
    যদি ((major*10 + minor) >= 2 && major < 1000 && minor < 1000){
        fread(net->seen, পরিমাণ(size_t), 1, fp);
    } অন্যথা {
        পূর্ণাঙ্ক iseen = 0;
        fread(&iseen, পরিমাণ(পূর্ণাঙ্ক), 1, fp);
        *net->seen = iseen;
    }
    পূর্ণাঙ্ক transpose = (major > 1000) || (minor > 1000);

    পূর্ণাঙ্ক i;
    প্রতি(i = start; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        যদি (l.dontload) অনুবর্ত;
        যদি(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            load_convolutional_weights(l, fp);
        }
        যদি(l.type == CONNECTED){
            load_connected_weights(l, fp, transpose);
        }
        যদি(l.type == BATCHNORM){
            load_batchnorm_weights(l, fp);
        }
        যদি(l.type == CRNN){
            load_convolutional_weights(*(l.প্রবেশ_layer), fp);
            load_convolutional_weights(*(l.self_layer), fp);
            load_convolutional_weights(*(l.output_layer), fp);
        }
        যদি(l.type == RNN){
            load_connected_weights(*(l.প্রবেশ_layer), fp, transpose);
            load_connected_weights(*(l.self_layer), fp, transpose);
            load_connected_weights(*(l.output_layer), fp, transpose);
        }
        যদি (l.type == LSTM) {
            load_connected_weights(*(l.wi), fp, transpose);
            load_connected_weights(*(l.wf), fp, transpose);
            load_connected_weights(*(l.wo), fp, transpose);
            load_connected_weights(*(l.wg), fp, transpose);
            load_connected_weights(*(l.ui), fp, transpose);
            load_connected_weights(*(l.uf), fp, transpose);
            load_connected_weights(*(l.uo), fp, transpose);
            load_connected_weights(*(l.ug), fp, transpose);
        }
        যদি (l.type == GRU) {
            যদি(1){
                load_connected_weights(*(l.wz), fp, transpose);
                load_connected_weights(*(l.wr), fp, transpose);
                load_connected_weights(*(l.wh), fp, transpose);
                load_connected_weights(*(l.uz), fp, transpose);
                load_connected_weights(*(l.ur), fp, transpose);
                load_connected_weights(*(l.uh), fp, transpose);
            }অন্যথা{
                load_connected_weights(*(l.reset_layer), fp, transpose);
                load_connected_weights(*(l.update_layer), fp, transpose);
                load_connected_weights(*(l.state_layer), fp, transpose);
            }
        }
        যদি(l.type == LOCAL){
            পূর্ণাঙ্ক locations = l.out_w*l.out_h;
            পূর্ণাঙ্ক size = l.size*l.size*l.c*l.n*locations;
            fread(l.biases, পরিমাণ(ভিন্ন), l.outputs, fp);
            fread(l.weights, পরিমাণ(ভিন্ন), size, fp);
#ifdef GPU
            যদি(gpu_index >= 0){
                ঠেলো_local_layer(l);
            }
#endif
        }
    }
    fprintf(stderr, "Done!\n");
    fclose(fp);
}

শূন্য load_weights(network *net, বর্ণ *filename)
{
    load_weights_upto(net, filename, 0, net->n);
}


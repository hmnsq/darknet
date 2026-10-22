#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <assert.h>

#அந்தர்பாவ "activation_layer.h"
#அந்தர்பாவ "logistic_layer.h"
#அந்தர்பாவ "l2norm_layer.h"
#அந்தர்பாவ "activations.h"
#அந்தர்பாவ "avgpool_layer.h"
#அந்தர்பாவ "batchnorm_layer.h"
#அந்தர்பாவ "blas.h"
#அந்தர்பாவ "connected_layer.h"
#அந்தர்பாவ "deconvolutional_layer.h"
#அந்தர்பாவ "convolutional_layer.h"
#அந்தர்பாவ "cost_layer.h"
#அந்தர்பாவ "crnn_layer.h"
#அந்தர்பாவ "crop_layer.h"
#அந்தர்பாவ "detection_layer.h"
#அந்தர்பாவ "dropout_layer.h"
#அந்தர்பாவ "gru_layer.h"
#அந்தர்பாவ "list.h"
#அந்தர்பாவ "local_layer.h"
#அந்தர்பாவ "maxpool_layer.h"
#அந்தர்பாவ "normalization_layer.h"
#அந்தர்பாவ "option_list.h"
#அந்தர்பாவ "parser.h"
#அந்தர்பாவ "region_layer.h"
#அந்தர்பாவ "yolo_layer.h"
#அந்தர்பாவ "iseg_layer.h"
#அந்தர்பாவ "reorg_layer.h"
#அந்தர்பாவ "rnn_layer.h"
#அந்தர்பாவ "route_layer.h"
#அந்தர்பாவ "upsample_layer.h"
#அந்தர்பாவ "shortcut_layer.h"
#அந்தர்பாவ "softmax_layer.h"
#அந்தர்பாவ "lstm_layer.h"
#அந்தர்பாவ "utils.h"

வகைவரை கட்டமைப்பு{
    எழுத்து *type;
    list *options;
}section;

list *read_cfg(எழுத்து *filename);

LAYER_TYPE string_to_layer_type(எழுத்து * type)
{

    எனில் (strcmp(type, "[shortcut]")==0) திரும்பு SHORTCUT;
    எனில் (strcmp(type, "[crop]")==0) திரும்பு CROP;
    எனில் (strcmp(type, "[cost]")==0) திரும்பு COST;
    எனில் (strcmp(type, "[detection]")==0) திரும்பு DETECTION;
    எனில் (strcmp(type, "[region]")==0) திரும்பு REGION;
    எனில் (strcmp(type, "[yolo]")==0) திரும்பு YOLO;
    எனில் (strcmp(type, "[iseg]")==0) திரும்பு ISEG;
    எனில் (strcmp(type, "[local]")==0) திரும்பு LOCAL;
    எனில் (strcmp(type, "[conv]")==0
            || strcmp(type, "[convolutional]")==0) திரும்பு CONVOLUTIONAL;
    எனில் (strcmp(type, "[deconv]")==0
            || strcmp(type, "[deconvolutional]")==0) திரும்பு DECONVOLUTIONAL;
    எனில் (strcmp(type, "[activation]")==0) திரும்பு ACTIVE;
    எனில் (strcmp(type, "[logistic]")==0) திரும்பு LOGXENT;
    எனில் (strcmp(type, "[l2norm]")==0) திரும்பு L2NORM;
    எனில் (strcmp(type, "[net]")==0
            || strcmp(type, "[network]")==0) திரும்பு NETWORK;
    எனில் (strcmp(type, "[crnn]")==0) திரும்பு CRNN;
    எனில் (strcmp(type, "[gru]")==0) திரும்பு GRU;
    எனில் (strcmp(type, "[lstm]") == 0) திரும்பு LSTM;
    எனில் (strcmp(type, "[rnn]")==0) திரும்பு RNN;
    எனில் (strcmp(type, "[conn]")==0
            || strcmp(type, "[connected]")==0) திரும்பு CONNECTED;
    எனில் (strcmp(type, "[max]")==0
            || strcmp(type, "[maxpool]")==0) திரும்பு MAXPOOL;
    எனில் (strcmp(type, "[reorg]")==0) திரும்பு REORG;
    எனில் (strcmp(type, "[avg]")==0
            || strcmp(type, "[avgpool]")==0) திரும்பு AVGPOOL;
    எனில் (strcmp(type, "[dropout]")==0) திரும்பு DROPOUT;
    எனில் (strcmp(type, "[lrn]")==0
            || strcmp(type, "[normalization]")==0) திரும்பு NORMALIZATION;
    எனில் (strcmp(type, "[batchnorm]")==0) திரும்பு BATCHNORM;
    எனில் (strcmp(type, "[soft]")==0
            || strcmp(type, "[softmax]")==0) திரும்பு SOFTMAX;
    எனில் (strcmp(type, "[route]")==0) திரும்பு ROUTE;
    எனில் (strcmp(type, "[upsample]")==0) திரும்பு UPSAMPLE;
    திரும்பு BLANK;
}

வெறுமை free_section(section *s)
{
    விடுவி(s->type);
    முனை *n = s->options->front;
    போது(n){
        kvp *pair = (kvp *)n->val;
        விடுவி(pair->சாவி);
        விடுவி(pair);
        முனை *next = n->next;
        விடுவி(n);
        n = next;
    }
    விடுவி(s->options);
    விடுவி(s);
}

வெறுமை parse_data(எழுத்து *data, தசம *a, முழு n)
{
    முழு i;
    எனில்(!data) திரும்பு;
    எழுத்து *curr = data;
    எழுத்து *next = data;
    முழு done = 0;
    க்கு(i = 0; i < n && !done; ++i){
        போது(*++next !='\0' && *next != ',');
        எனில்(*next == '\0') done = 1;
        *next = '\0';
        sscanf(curr, "%g", &a[i]);
        curr = next+1;
    }
}

வகைவரை கட்டமைப்பு size_params{
    முழு batch;
    முழு inputs;
    முழு h;
    முழு w;
    முழு c;
    முழு index;
    முழு time_steps;
    network *net;
} size_params;

local_layer parse_local(list *options, size_params params)
{
    முழு n = option_find_int(options, "filters",1);
    முழு size = option_find_int(options, "size",1);
    முழு stride = option_find_int(options, "stride",1);
    முழு pad = option_find_int(options, "pad",0);
    எழுத்து *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    முழு batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    எனில்(!(h && w && c)) error("Layer before local layer must output image.");

    local_layer layer = make_local_layer(batch,h,w,c,n,size,stride,pad,activation);

    திரும்பு layer;
}

layer parse_deconvolutional(list *options, size_params params)
{
    முழு n = option_find_int(options, "filters",1);
    முழு size = option_find_int(options, "size",1);
    முழு stride = option_find_int(options, "stride",1);

    எழுத்து *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    முழு batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    எனில்(!(h && w && c)) error("Layer before deconvolutional layer must output image.");
    முழு batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);
    முழு pad = option_find_int_quiet(options, "pad",0);
    முழு padding = option_find_int_quiet(options, "padding",0);
    எனில்(pad) padding = size/2;

    layer l = make_deconvolutional_layer(batch,h,w,c,n,size,stride,padding, activation, batch_normalize, params.net->adam);

    திரும்பு l;
}


convolutional_layer parse_convolutional(list *options, size_params params)
{
    முழு n = option_find_int(options, "filters",1);
    முழு size = option_find_int(options, "size",1);
    முழு stride = option_find_int(options, "stride",1);
    முழு pad = option_find_int_quiet(options, "pad",0);
    முழு padding = option_find_int_quiet(options, "padding",0);
    முழு groups = option_find_int_quiet(options, "groups", 1);
    எனில்(pad) padding = size/2;

    எழுத்து *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    முழு batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    எனில்(!(h && w && c)) error("Layer before convolutional layer must output image.");
    முழு batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);
    முழு இருமம் = option_find_int_quiet(options, "binary", 0);
    முழு xnor = option_find_int_quiet(options, "xnor", 0);

    convolutional_layer layer = make_convolutional_layer(batch,h,w,c,n,groups,size,stride,padding,activation, batch_normalize, இருமம், xnor, params.net->adam);
    layer.flipped = option_find_int_quiet(options, "flipped", 0);
    layer.dot = option_find_float_quiet(options, "dot", 0);

    திரும்பு layer;
}

layer parse_crnn(list *options, size_params params)
{
    முழு output_filters = option_find_int(options, "output_filters",1);
    முழு hidden_filters = option_find_int(options, "hidden_filters",1);
    எழுத்து *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    முழு batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_crnn_layer(params.batch, params.w, params.h, params.c, hidden_filters, output_filters, params.time_steps, activation, batch_normalize);

    l.shortcut = option_find_int_quiet(options, "shortcut", 0);

    திரும்பு l;
}

layer parse_rnn(list *options, size_params params)
{
    முழு output = option_find_int(options, "output",1);
    எழுத்து *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    முழு batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_rnn_layer(params.batch, params.inputs, output, params.time_steps, activation, batch_normalize, params.net->adam);

    l.shortcut = option_find_int_quiet(options, "shortcut", 0);

    திரும்பு l;
}

layer parse_gru(list *options, size_params params)
{
    முழு output = option_find_int(options, "output",1);
    முழு batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_gru_layer(params.batch, params.inputs, output, params.time_steps, batch_normalize, params.net->adam);
    l.tanh = option_find_int_quiet(options, "tanh", 0);

    திரும்பு l;
}

layer parse_lstm(list *options, size_params params)
{
    முழு output = option_find_int(options, "output", 1);
    முழு batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_lstm_layer(params.batch, params.inputs, output, params.time_steps, batch_normalize, params.net->adam);

    திரும்பு l;
}

layer parse_connected(list *options, size_params params)
{
    முழு output = option_find_int(options, "output",1);
    எழுத்து *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    முழு batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_connected_layer(params.batch, params.inputs, output, activation, batch_normalize, params.net->adam);
    திரும்பு l;
}

layer parse_softmax(list *options, size_params params)
{
    முழு groups = option_find_int_quiet(options, "groups",1);
    layer l = make_softmax_layer(params.batch, params.inputs, groups);
    l.temperature = option_find_float_quiet(options, "temperature", 1);
    எழுத்து *மரம்_file = option_find_str(options, "tree", 0);
    எனில் (மரம்_file) l.softmax_மரம் = read_மரம்(மரம்_file);
    l.w = params.w;
    l.h = params.h;
    l.c = params.c;
    l.spatial = option_find_float_quiet(options, "spatial", 0);
    l.noloss =  option_find_int_quiet(options, "noloss", 0);
    திரும்பு l;
}

முழு *parse_yolo_mask(எழுத்து *a, முழு *இலக்கம்)
{
    முழு *mask = 0;
    எனில்(a){
        முழு len = strlen(a);
        முழு n = 1;
        முழு i;
        க்கு(i = 0; i < len; ++i){
            எனில் (a[i] == ',') ++n;
        }
        mask = calloc(n, அளவு(முழு));
        க்கு(i = 0; i < n; ++i){
            முழு val = atoi(a);
            mask[i] = val;
            a = strchr(a, ',')+1;
        }
        *இலக்கம் = n;
    }
    திரும்பு mask;
}

layer parse_yolo(list *options, size_params params)
{
    முழு classes = option_find_int(options, "classes", 20);
    முழு total = option_find_int(options, "num", 1);
    முழு இலக்கம் = total;

    எழுத்து *a = option_find_str(options, "mask", 0);
    முழு *mask = parse_yolo_mask(a, &இலக்கம்);
    layer l = make_yolo_layer(params.batch, params.w, params.h, இலக்கம், total, mask, classes);
    ப்ரதிஜ்ஞா(l.outputs == params.inputs);

    l.max_boxes = option_find_int_quiet(options, "max",90);
    l.jitter = option_find_float(options, "jitter", .2);

    l.ignore_thresh = option_find_float(options, "ignore_thresh", .5);
    l.truth_thresh = option_find_float(options, "truth_thresh", 1);
    l.random = option_find_int_quiet(options, "random", 0);

    எழுத்து *map_file = option_find_str(options, "map", 0);
    எனில் (map_file) l.map = read_map(map_file);

    a = option_find_str(options, "anchors", 0);
    எனில்(a){
        முழு len = strlen(a);
        முழு n = 1;
        முழு i;
        க்கு(i = 0; i < len; ++i){
            எனில் (a[i] == ',') ++n;
        }
        க்கு(i = 0; i < n; ++i){
            தசம bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    திரும்பு l;
}

layer parse_iseg(list *options, size_params params)
{
    முழு classes = option_find_int(options, "classes", 20);
    முழு ids = option_find_int(options, "ids", 32);
    layer l = make_iseg_layer(params.batch, params.w, params.h, classes, ids);
    ப்ரதிஜ்ஞா(l.outputs == params.inputs);
    திரும்பு l;
}

layer parse_region(list *options, size_params params)
{
    முழு coords = option_find_int(options, "coords", 4);
    முழு classes = option_find_int(options, "classes", 20);
    முழு இலக்கம் = option_find_int(options, "num", 1);

    layer l = make_region_layer(params.batch, params.w, params.h, இலக்கம், classes, coords);
    ப்ரதிஜ்ஞா(l.outputs == params.inputs);

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

    எழுத்து *மரம்_file = option_find_str(options, "tree", 0);
    எனில் (மரம்_file) l.softmax_மரம் = read_மரம்(மரம்_file);
    எழுத்து *map_file = option_find_str(options, "map", 0);
    எனில் (map_file) l.map = read_map(map_file);

    எழுத்து *a = option_find_str(options, "anchors", 0);
    எனில்(a){
        முழு len = strlen(a);
        முழு n = 1;
        முழு i;
        க்கு(i = 0; i < len; ++i){
            எனில் (a[i] == ',') ++n;
        }
        க்கு(i = 0; i < n; ++i){
            தசம bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    திரும்பு l;
}

detection_layer parse_detection(list *options, size_params params)
{
    முழு coords = option_find_int(options, "coords", 1);
    முழு classes = option_find_int(options, "classes", 1);
    முழு rescore = option_find_int(options, "rescore", 0);
    முழு இலக்கம் = option_find_int(options, "num", 1);
    முழு side = option_find_int(options, "side", 7);
    detection_layer layer = make_detection_layer(params.batch, params.inputs, இலக்கம், side, classes, coords, rescore);

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
    திரும்பு layer;
}

cost_layer parse_cost(list *options, size_params params)
{
    எழுத்து *type_s = option_find_str(options, "type", "sse");
    COST_TYPE type = get_cost_type(type_s);
    தசம scale = option_find_float_quiet(options, "scale",1);
    cost_layer layer = make_cost_layer(params.batch, params.inputs, type, scale);
    layer.ratio =  option_find_float_quiet(options, "ratio",0);
    layer.noobject_scale =  option_find_float_quiet(options, "noobj", 1);
    layer.thresh =  option_find_float_quiet(options, "thresh",0);
    திரும்பு layer;
}

crop_layer parse_crop(list *options, size_params params)
{
    முழு crop_height = option_find_int(options, "crop_height",1);
    முழு crop_width = option_find_int(options, "crop_width",1);
    முழு flip = option_find_int(options, "flip",0);
    தசம angle = option_find_float(options, "angle",0);
    தசம saturation = option_find_float(options, "saturation",1);
    தசம exposure = option_find_float(options, "exposure",1);

    முழு batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    எனில்(!(h && w && c)) error("Layer before crop layer must output image.");

    முழு noadjust = option_find_int_quiet(options, "noadjust",0);

    crop_layer l = make_crop_layer(batch,h,w,c,crop_height,crop_width,flip, angle, saturation, exposure);
    l.shift = option_find_float(options, "shift", 0);
    l.noadjust = noadjust;
    திரும்பு l;
}

layer parse_reorg(list *options, size_params params)
{
    முழு stride = option_find_int(options, "stride",1);
    முழு reverse = option_find_int_quiet(options, "reverse",0);
    முழு flatten = option_find_int_quiet(options, "flatten",0);
    முழு extra = option_find_int_quiet(options, "extra",0);

    முழு batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    எனில்(!(h && w && c)) error("Layer before reorg layer must output image.");

    layer layer = make_reorg_layer(batch,w,h,c,stride,reverse, flatten, extra);
    திரும்பு layer;
}

maxpool_layer parse_maxpool(list *options, size_params params)
{
    முழு stride = option_find_int(options, "stride",1);
    முழு size = option_find_int(options, "size",stride);
    முழு padding = option_find_int_quiet(options, "padding", size-1);

    முழு batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    எனில்(!(h && w && c)) error("Layer before maxpool layer must output image.");

    maxpool_layer layer = make_maxpool_layer(batch,h,w,c,size,stride,padding);
    திரும்பு layer;
}

avgpool_layer parse_avgpool(list *options, size_params params)
{
    முழு batch,w,h,c;
    w = params.w;
    h = params.h;
    c = params.c;
    batch=params.batch;
    எனில்(!(h && w && c)) error("Layer before avgpool layer must output image.");

    avgpool_layer layer = make_avgpool_layer(batch,w,h,c);
    திரும்பு layer;
}

dropout_layer parse_dropout(list *options, size_params params)
{
    தசம probability = option_find_float(options, "probability", .5);
    dropout_layer layer = make_dropout_layer(params.batch, params.inputs, probability);
    layer.out_w = params.w;
    layer.out_h = params.h;
    layer.out_c = params.c;
    திரும்பு layer;
}

layer parse_normalization(list *options, size_params params)
{
    தசம alpha = option_find_float(options, "alpha", .0001);
    தசம beta =  option_find_float(options, "beta" , .75);
    தசம kappa = option_find_float(options, "kappa", 1);
    முழு size = option_find_int(options, "size", 5);
    layer l = make_normalization_layer(params.batch, params.w, params.h, params.c, size, alpha, beta, kappa);
    திரும்பு l;
}

layer parse_batchnorm(list *options, size_params params)
{
    layer l = make_batchnorm_layer(params.batch, params.w, params.h, params.c);
    திரும்பு l;
}

layer parse_shortcut(list *options, size_params params, network *net)
{
    எழுத்து *l = option_find(options, "from");
    முழு index = atoi(l);
    எனில்(index < 0) index = params.index + index;

    முழு batch = params.batch;
    layer from = net->layers[index];

    layer s = make_shortcut_layer(batch, index, params.w, params.h, params.c, from.out_w, from.out_h, from.out_c);

    எழுத்து *activation_s = option_find_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);
    s.activation = activation;
    s.alpha = option_find_float_quiet(options, "alpha", 1);
    s.beta = option_find_float_quiet(options, "beta", 1);
    திரும்பு s;
}


layer parse_l2norm(list *options, size_params params)
{
    layer l = make_l2norm_layer(params.batch, params.inputs);
    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;
    திரும்பு l;
}


layer parse_logistic(list *options, size_params params)
{
    layer l = make_logistic_layer(params.batch, params.inputs);
    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;
    திரும்பு l;
}

layer parse_activation(list *options, size_params params)
{
    எழுத்து *activation_s = option_find_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);

    layer l = make_activation_layer(params.batch, params.inputs, activation);

    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;

    திரும்பு l;
}

layer parse_upsample(list *options, size_params params, network *net)
{

    முழு stride = option_find_int(options, "stride",2);
    layer l = make_upsample_layer(params.batch, params.w, params.h, params.c, stride);
    l.scale = option_find_float_quiet(options, "scale", 1);
    திரும்பு l;
}

route_layer parse_route(list *options, size_params params, network *net)
{
    எழுத்து *l = option_find(options, "layers");
    முழு len = strlen(l);
    எனில்(!l) error("Route Layer must specify input layers");
    முழு n = 1;
    முழு i;
    க்கு(i = 0; i < len; ++i){
        எனில் (l[i] == ',') ++n;
    }

    முழு *layers = calloc(n, அளவு(முழு));
    முழு *sizes = calloc(n, அளவு(முழு));
    க்கு(i = 0; i < n; ++i){
        முழு index = atoi(l);
        l = strchr(l, ',')+1;
        எனில்(index < 0) index = params.index + index;
        layers[i] = index;
        sizes[i] = net->layers[index].outputs;
    }
    முழு batch = params.batch;

    route_layer layer = make_route_layer(batch, n, layers, sizes);

    convolutional_layer first = net->layers[layers[0]];
    layer.out_w = first.out_w;
    layer.out_h = first.out_h;
    layer.out_c = first.out_c;
    க்கு(i = 1; i < n; ++i){
        முழு index = layers[i];
        convolutional_layer next = net->layers[index];
        எனில்(next.out_w == first.out_w && next.out_h == first.out_h){
            layer.out_c += next.out_c;
        }இல்லையெனில்{
            layer.out_h = layer.out_w = layer.out_c = 0;
        }
    }

    திரும்பு layer;
}

learning_rate_policy get_policy(எழுத்து *s)
{
    எனில் (strcmp(s, "random")==0) திரும்பு RANDOM;
    எனில் (strcmp(s, "poly")==0) திரும்பு POLY;
    எனில் (strcmp(s, "constant")==0) திரும்பு CONSTANT;
    எனில் (strcmp(s, "step")==0) திரும்பு STEP;
    எனில் (strcmp(s, "exp")==0) திரும்பு EXP;
    எனில் (strcmp(s, "sigmoid")==0) திரும்பு SIG;
    எனில் (strcmp(s, "steps")==0) திரும்பு STEPS;
    fprintf(stderr, "Couldn't find policy %s, going with constant\n", s);
    திரும்பு CONSTANT;
}

வெறுமை parse_net_options(list *options, network *net)
{
    net->batch = option_find_int(options, "batch",1);
    net->learning_rate = option_find_float(options, "learning_rate", .001);
    net->momentum = option_find_float(options, "momentum", .9);
    net->decay = option_find_float(options, "decay", .0001);
    முழு subdivs = option_find_int(options, "subdivisions",1);
    net->time_steps = option_find_int_quiet(options, "time_steps",1);
    net->notruth = option_find_int_quiet(options, "notruth",0);
    net->batch /= subdivs;
    net->batch *= net->time_steps;
    net->subdivisions = subdivs;
    net->random = option_find_int_quiet(options, "random", 0);

    net->adam = option_find_int_quiet(options, "adam", 0);
    எனில்(net->adam){
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
    net->max_ratio = option_find_float_quiet(options, "max_ratio", (தசம) net->max_crop / net->w);
    net->min_ratio = option_find_float_quiet(options, "min_ratio", (தசம) net->min_crop / net->w);
    net->center = option_find_int_quiet(options, "center",0);
    net->clip = option_find_float_quiet(options, "clip", 0);

    net->angle = option_find_float_quiet(options, "angle", 0);
    net->aspect = option_find_float_quiet(options, "aspect", 1);
    net->saturation = option_find_float_quiet(options, "saturation", 1);
    net->exposure = option_find_float_quiet(options, "exposure", 1);
    net->hue = option_find_float_quiet(options, "hue", 0);

    எனில்(!net->inputs && !(net->h && net->w && net->c)) error("No input parameters supplied");

    எழுத்து *policy_s = option_find_str(options, "policy", "constant");
    net->policy = get_policy(policy_s);
    net->burn_in = option_find_int_quiet(options, "burn_in", 0);
    net->power = option_find_float_quiet(options, "power", 4);
    எனில்(net->policy == STEP){
        net->step = option_find_int(options, "step", 1);
        net->scale = option_find_float(options, "scale", 1);
    } இல்லையெனில் எனில் (net->policy == STEPS){
        எழுத்து *l = option_find(options, "steps");
        எழுத்து *p = option_find(options, "scales");
        எனில்(!l || !p) error("STEPS policy must have steps and scales in cfg file");

        முழு len = strlen(l);
        முழு n = 1;
        முழு i;
        க்கு(i = 0; i < len; ++i){
            எனில் (l[i] == ',') ++n;
        }
        முழு *steps = calloc(n, அளவு(முழு));
        தசம *scales = calloc(n, அளவு(தசம));
        க்கு(i = 0; i < n; ++i){
            முழு step    = atoi(l);
            தசம scale = atof(p);
            l = strchr(l, ',')+1;
            p = strchr(p, ',')+1;
            steps[i] = step;
            scales[i] = scale;
        }
        net->scales = scales;
        net->steps = steps;
        net->இலக்கம்_steps = n;
    } இல்லையெனில் எனில் (net->policy == EXP){
        net->gamma = option_find_float(options, "gamma", 1);
    } இல்லையெனில் எனில் (net->policy == SIG){
        net->gamma = option_find_float(options, "gamma", 1);
        net->step = option_find_int(options, "step", 1);
    } இல்லையெனில் எனில் (net->policy == POLY || net->policy == RANDOM){
    }
    net->max_batches = option_find_int(options, "max_batches", 0);
}

முழு is_network(section *s)
{
    திரும்பு (strcmp(s->type, "[net]")==0
            || strcmp(s->type, "[network]")==0);
}

network *parse_network_cfg(எழுத்து *filename)
{
    list *sections = read_cfg(filename);
    முனை *n = sections->front;
    எனில்(!n) error("Config file has no sections");
    network *net = make_network(sections->size - 1);
    net->gpu_index = gpu_index;
    size_params params;

    section *s = (section *)n->val;
    list *options = s->options;
    எனில்(!is_network(s)) error("First section must be [net] or [network]");
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
    முழு மொத்தம் = 0;
    free_section(s);
    fprintf(stderr, "layer     filters    size              input                output\n");
    போது(n){
        params.index = மொத்தம்;
        fprintf(stderr, "%5d ", மொத்தம்);
        s = (section *)n->val;
        options = s->options;
        layer l = {0};
        LAYER_TYPE lt = string_to_layer_type(s->type);
        எனில்(lt == CONVOLUTIONAL){
            l = parse_convolutional(options, params);
        }இல்லையெனில் எனில்(lt == DECONVOLUTIONAL){
            l = parse_deconvolutional(options, params);
        }இல்லையெனில் எனில்(lt == LOCAL){
            l = parse_local(options, params);
        }இல்லையெனில் எனில்(lt == ACTIVE){
            l = parse_activation(options, params);
        }இல்லையெனில் எனில்(lt == LOGXENT){
            l = parse_logistic(options, params);
        }இல்லையெனில் எனில்(lt == L2NORM){
            l = parse_l2norm(options, params);
        }இல்லையெனில் எனில்(lt == RNN){
            l = parse_rnn(options, params);
        }இல்லையெனில் எனில்(lt == GRU){
            l = parse_gru(options, params);
        }இல்லையெனில் எனில் (lt == LSTM) {
            l = parse_lstm(options, params);
        }இல்லையெனில் எனில்(lt == CRNN){
            l = parse_crnn(options, params);
        }இல்லையெனில் எனில்(lt == CONNECTED){
            l = parse_connected(options, params);
        }இல்லையெனில் எனில்(lt == CROP){
            l = parse_crop(options, params);
        }இல்லையெனில் எனில்(lt == COST){
            l = parse_cost(options, params);
        }இல்லையெனில் எனில்(lt == REGION){
            l = parse_region(options, params);
        }இல்லையெனில் எனில்(lt == YOLO){
            l = parse_yolo(options, params);
        }இல்லையெனில் எனில்(lt == ISEG){
            l = parse_iseg(options, params);
        }இல்லையெனில் எனில்(lt == DETECTION){
            l = parse_detection(options, params);
        }இல்லையெனில் எனில்(lt == SOFTMAX){
            l = parse_softmax(options, params);
            net->hierarchy = l.softmax_மரம்;
        }இல்லையெனில் எனில்(lt == NORMALIZATION){
            l = parse_normalization(options, params);
        }இல்லையெனில் எனில்(lt == BATCHNORM){
            l = parse_batchnorm(options, params);
        }இல்லையெனில் எனில்(lt == MAXPOOL){
            l = parse_maxpool(options, params);
        }இல்லையெனில் எனில்(lt == REORG){
            l = parse_reorg(options, params);
        }இல்லையெனில் எனில்(lt == AVGPOOL){
            l = parse_avgpool(options, params);
        }இல்லையெனில் எனில்(lt == ROUTE){
            l = parse_route(options, params, net);
        }இல்லையெனில் எனில்(lt == UPSAMPLE){
            l = parse_upsample(options, params, net);
        }இல்லையெனில் எனில்(lt == SHORTCUT){
            l = parse_shortcut(options, params, net);
        }இல்லையெனில் எனில்(lt == DROPOUT){
            l = parse_dropout(options, params);
            l.output = net->layers[மொத்தம்-1].output;
            l.delta = net->layers[மொத்தம்-1].delta;
#ifdef GPU
            l.output_gpu = net->layers[மொத்தம்-1].output_gpu;
            l.delta_gpu = net->layers[மொத்தம்-1].delta_gpu;
#endif
        }இல்லையெனில்{
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
        net->layers[மொத்தம்] = l;
        எனில் (l.workspace_size > workspace_size) workspace_size = l.workspace_size;
        free_section(s);
        n = n->next;
        ++மொத்தம்;
        எனில்(n){
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
    எனில்(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    net->உள்ளீடு = calloc(net->inputs*net->batch, அளவு(தசம));
    net->truth = calloc(net->truths*net->batch, அளவு(தசம));
#ifdef GPU
    net->output_gpu = out.output_gpu;
    net->உள்ளீடு_gpu = cuda_make_array(net->உள்ளீடு, net->inputs*net->batch);
    net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
#endif
    எனில்(workspace_size){
        //printf("%ld\n", workspace_size);
#ifdef GPU
        எனில்(gpu_index >= 0){
            net->workspace = cuda_make_array(0, (workspace_size-1)/அளவு(தசம)+1);
        }இல்லையெனில் {
            net->workspace = calloc(1, workspace_size);
        }
#இல்லையெனில்
        net->workspace = calloc(1, workspace_size);
#endif
    }
    திரும்பு net;
}

list *read_cfg(எழுத்து *filename)
{
    FILE *file = fopen(filename, "r");
    எனில்(file == 0) file_error(filename);
    எழுத்து *வரி;
    முழு nu = 0;
    list *options = make_list();
    section *current = 0;
    போது((வரி=fgetl(file)) != 0){
        ++ nu;
        strip(வரி);
        தேர்வு(வரி[0]){
            நிலை '[':
                current = malloc(அளவு(section));
                list_செருகு(options, current);
                current->options = make_list();
                current->type = வரி;
                நிறுத்து;
            நிலை '\0':
            நிலை '#':
            நிலை ';':
                விடுவி(வரி);
                நிறுத்து;
            இயல்பு:
                எனில்(!read_option(வரி, current->options)){
                    fprintf(stderr, "Config file error line %d, could parse: %s\n", nu, வரி);
                    விடுவி(வரி);
                }
                நிறுத்து;
        }
    }
    fclose(file);
    திரும்பு options;
}

வெறுமை save_convolutional_weights_இருமம்(layer l, FILE *fp)
{
#ifdef GPU
    எனில்(gpu_index >= 0){
        pull_convolutional_layer(l);
    }
#endif
    binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.இருமம்_weights);
    முழு size = l.c*l.size*l.size;
    முழு i, j, k;
    fwrite(l.biases, அளவு(தசம), l.n, fp);
    எனில் (l.batch_normalize){
        fwrite(l.scales, அளவு(தசம), l.n, fp);
        fwrite(l.rolling_mean, அளவு(தசம), l.n, fp);
        fwrite(l.rolling_variance, அளவு(தசம), l.n, fp);
    }
    க்கு(i = 0; i < l.n; ++i){
        தசம mean = l.இருமம்_weights[i*size];
        எனில்(mean < 0) mean = -mean;
        fwrite(&mean, அளவு(தசம), 1, fp);
        க்கு(j = 0; j < size/8; ++j){
            முழு index = i*size + j*8;
            unsigned எழுத்து c = 0;
            க்கு(k = 0; k < 8; ++k){
                எனில் (j*8 + k >= size) நிறுத்து;
                எனில் (l.இருமம்_weights[index + k] > 0) c = (c | 1<<k);
            }
            fwrite(&c, அளவு(எழுத்து), 1, fp);
        }
    }
}

வெறுமை save_convolutional_weights(layer l, FILE *fp)
{
    எனில்(l.இருமம்){
        //save_convolutional_weights_binary(l, fp);
        //return;
    }
#ifdef GPU
    எனில்(gpu_index >= 0){
        pull_convolutional_layer(l);
    }
#endif
    முழு இலக்கம் = l.nweights;
    fwrite(l.biases, அளவு(தசம), l.n, fp);
    எனில் (l.batch_normalize){
        fwrite(l.scales, அளவு(தசம), l.n, fp);
        fwrite(l.rolling_mean, அளவு(தசம), l.n, fp);
        fwrite(l.rolling_variance, அளவு(தசம), l.n, fp);
    }
    fwrite(l.weights, அளவு(தசம), இலக்கம், fp);
}

வெறுமை save_batchnorm_weights(layer l, FILE *fp)
{
#ifdef GPU
    எனில்(gpu_index >= 0){
        pull_batchnorm_layer(l);
    }
#endif
    fwrite(l.scales, அளவு(தசம), l.c, fp);
    fwrite(l.rolling_mean, அளவு(தசம), l.c, fp);
    fwrite(l.rolling_variance, அளவு(தசம), l.c, fp);
}

வெறுமை save_connected_weights(layer l, FILE *fp)
{
#ifdef GPU
    எனில்(gpu_index >= 0){
        pull_connected_layer(l);
    }
#endif
    fwrite(l.biases, அளவு(தசம), l.outputs, fp);
    fwrite(l.weights, அளவு(தசம), l.outputs*l.inputs, fp);
    எனில் (l.batch_normalize){
        fwrite(l.scales, அளவு(தசம), l.outputs, fp);
        fwrite(l.rolling_mean, அளவு(தசம), l.outputs, fp);
        fwrite(l.rolling_variance, அளவு(தசம), l.outputs, fp);
    }
}

வெறுமை save_weights_upto(network *net, எழுத்து *filename, முழு cutoff)
{
#ifdef GPU
    எனில்(net->gpu_index >= 0){
        cuda_set_device(net->gpu_index);
    }
#endif
    fprintf(stderr, "Saving weights to %s\n", filename);
    FILE *fp = fopen(filename, "wb");
    எனில்(!fp) file_error(filename);

    முழு major = 0;
    முழு minor = 2;
    முழு revision = 0;
    fwrite(&major, அளவு(முழு), 1, fp);
    fwrite(&minor, அளவு(முழு), 1, fp);
    fwrite(&revision, அளவு(முழு), 1, fp);
    fwrite(net->seen, அளவு(size_t), 1, fp);

    முழு i;
    க்கு(i = 0; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        எனில் (l.dontsave) தொடர்;
        எனில்(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            save_convolutional_weights(l, fp);
        } எனில்(l.type == CONNECTED){
            save_connected_weights(l, fp);
        } எனில்(l.type == BATCHNORM){
            save_batchnorm_weights(l, fp);
        } எனில்(l.type == RNN){
            save_connected_weights(*(l.உள்ளீடு_layer), fp);
            save_connected_weights(*(l.self_layer), fp);
            save_connected_weights(*(l.output_layer), fp);
        } எனில் (l.type == LSTM) {
            save_connected_weights(*(l.wi), fp);
            save_connected_weights(*(l.wf), fp);
            save_connected_weights(*(l.wo), fp);
            save_connected_weights(*(l.wg), fp);
            save_connected_weights(*(l.ui), fp);
            save_connected_weights(*(l.uf), fp);
            save_connected_weights(*(l.uo), fp);
            save_connected_weights(*(l.ug), fp);
        } எனில் (l.type == GRU) {
            எனில்(1){
                save_connected_weights(*(l.wz), fp);
                save_connected_weights(*(l.wr), fp);
                save_connected_weights(*(l.wh), fp);
                save_connected_weights(*(l.uz), fp);
                save_connected_weights(*(l.ur), fp);
                save_connected_weights(*(l.uh), fp);
            }இல்லையெனில்{
                save_connected_weights(*(l.reset_layer), fp);
                save_connected_weights(*(l.update_layer), fp);
                save_connected_weights(*(l.state_layer), fp);
            }
        }  எனில்(l.type == CRNN){
            save_convolutional_weights(*(l.உள்ளீடு_layer), fp);
            save_convolutional_weights(*(l.self_layer), fp);
            save_convolutional_weights(*(l.output_layer), fp);
        } எனில்(l.type == LOCAL){
#ifdef GPU
            எனில்(gpu_index >= 0){
                pull_local_layer(l);
            }
#endif
            முழு locations = l.out_w*l.out_h;
            முழு size = l.size*l.size*l.c*l.n*locations;
            fwrite(l.biases, அளவு(தசம), l.outputs, fp);
            fwrite(l.weights, அளவு(தசம), size, fp);
        }
    }
    fclose(fp);
}
வெறுமை save_weights(network *net, எழுத்து *filename)
{
    save_weights_upto(net, filename, net->n);
}

வெறுமை transpose_matrix(தசம *a, முழு rows, முழு cols)
{
    தசம *transpose = calloc(rows*cols, அளவு(தசம));
    முழு x, y;
    க்கு(x = 0; x < rows; ++x){
        க்கு(y = 0; y < cols; ++y){
            transpose[y*rows + x] = a[x*cols + y];
        }
    }
    memcpy(a, transpose, rows*cols*அளவு(தசம));
    விடுவி(transpose);
}

வெறுமை load_connected_weights(layer l, FILE *fp, முழு transpose)
{
    fread(l.biases, அளவு(தசம), l.outputs, fp);
    fread(l.weights, அளவு(தசம), l.outputs*l.inputs, fp);
    எனில்(transpose){
        transpose_matrix(l.weights, l.inputs, l.outputs);
    }
    //printf("Biases: %f mean %f variance\n", mean_array(l.biases, l.outputs), variance_array(l.biases, l.outputs));
    //printf("Weights: %f mean %f variance\n", mean_array(l.weights, l.outputs*l.inputs), variance_array(l.weights, l.outputs*l.inputs));
    எனில் (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, அளவு(தசம), l.outputs, fp);
        fread(l.rolling_mean, அளவு(தசம), l.outputs, fp);
        fread(l.rolling_variance, அளவு(தசம), l.outputs, fp);
        //printf("Scales: %f mean %f variance\n", mean_array(l.scales, l.outputs), variance_array(l.scales, l.outputs));
        //printf("rolling_mean: %f mean %f variance\n", mean_array(l.rolling_mean, l.outputs), variance_array(l.rolling_mean, l.outputs));
        //printf("rolling_variance: %f mean %f variance\n", mean_array(l.rolling_variance, l.outputs), variance_array(l.rolling_variance, l.outputs));
    }
#ifdef GPU
    எனில்(gpu_index >= 0){
        தள்ளு_connected_layer(l);
    }
#endif
}

வெறுமை load_batchnorm_weights(layer l, FILE *fp)
{
    fread(l.scales, அளவு(தசம), l.c, fp);
    fread(l.rolling_mean, அளவு(தசம), l.c, fp);
    fread(l.rolling_variance, அளவு(தசம), l.c, fp);
#ifdef GPU
    எனில்(gpu_index >= 0){
        தள்ளு_batchnorm_layer(l);
    }
#endif
}

வெறுமை load_convolutional_weights_இருமம்(layer l, FILE *fp)
{
    fread(l.biases, அளவு(தசம), l.n, fp);
    எனில் (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, அளவு(தசம), l.n, fp);
        fread(l.rolling_mean, அளவு(தசம), l.n, fp);
        fread(l.rolling_variance, அளவு(தசம), l.n, fp);
    }
    முழு size = l.c*l.size*l.size;
    முழு i, j, k;
    க்கு(i = 0; i < l.n; ++i){
        தசம mean = 0;
        fread(&mean, அளவு(தசம), 1, fp);
        க்கு(j = 0; j < size/8; ++j){
            முழு index = i*size + j*8;
            unsigned எழுத்து c = 0;
            fread(&c, அளவு(எழுத்து), 1, fp);
            க்கு(k = 0; k < 8; ++k){
                எனில் (j*8 + k >= size) நிறுத்து;
                l.weights[index + k] = (c & 1<<k) ? mean : -mean;
            }
        }
    }
#ifdef GPU
    எனில்(gpu_index >= 0){
        தள்ளு_convolutional_layer(l);
    }
#endif
}

வெறுமை load_convolutional_weights(layer l, FILE *fp)
{
    எனில்(l.இருமம்){
        //load_convolutional_weights_binary(l, fp);
        //return;
    }
    எனில்(l.numload) l.n = l.numload;
    முழு இலக்கம் = l.c/l.groups*l.n*l.size*l.size;
    fread(l.biases, அளவு(தசம), l.n, fp);
    எனில் (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, அளவு(தசம), l.n, fp);
        fread(l.rolling_mean, அளவு(தசம), l.n, fp);
        fread(l.rolling_variance, அளவு(தசம), l.n, fp);
        எனில்(0){
            முழு i;
            க்கு(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_mean[i]);
            }
            printf("\n");
            க்கு(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_variance[i]);
            }
            printf("\n");
        }
        எனில்(0){
            fill_cpu(l.n, 0, l.rolling_mean, 1);
            fill_cpu(l.n, 0, l.rolling_variance, 1);
        }
        எனில்(0){
            முழு i;
            க்கு(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_mean[i]);
            }
            printf("\n");
            க்கு(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_variance[i]);
            }
            printf("\n");
        }
    }
    fread(l.weights, அளவு(தசம), இலக்கம், fp);
    //if(l.c == 3) scal_cpu(num, 1./256, l.weights, 1);
    எனில் (l.flipped) {
        transpose_matrix(l.weights, l.c*l.size*l.size, l.n);
    }
    //if (l.binary) binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.weights);
#ifdef GPU
    எனில்(gpu_index >= 0){
        தள்ளு_convolutional_layer(l);
    }
#endif
}


வெறுமை load_weights_upto(network *net, எழுத்து *filename, முழு start, முழு cutoff)
{
#ifdef GPU
    எனில்(net->gpu_index >= 0){
        cuda_set_device(net->gpu_index);
    }
#endif
    fprintf(stderr, "Loading weights from %s...", filename);
    fflush(stdout);
    FILE *fp = fopen(filename, "rb");
    எனில்(!fp) file_error(filename);

    முழு major;
    முழு minor;
    முழு revision;
    fread(&major, அளவு(முழு), 1, fp);
    fread(&minor, அளவு(முழு), 1, fp);
    fread(&revision, அளவு(முழு), 1, fp);
    எனில் ((major*10 + minor) >= 2 && major < 1000 && minor < 1000){
        fread(net->seen, அளவு(size_t), 1, fp);
    } இல்லையெனில் {
        முழு iseen = 0;
        fread(&iseen, அளவு(முழு), 1, fp);
        *net->seen = iseen;
    }
    முழு transpose = (major > 1000) || (minor > 1000);

    முழு i;
    க்கு(i = start; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        எனில் (l.dontload) தொடர்;
        எனில்(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            load_convolutional_weights(l, fp);
        }
        எனில்(l.type == CONNECTED){
            load_connected_weights(l, fp, transpose);
        }
        எனில்(l.type == BATCHNORM){
            load_batchnorm_weights(l, fp);
        }
        எனில்(l.type == CRNN){
            load_convolutional_weights(*(l.உள்ளீடு_layer), fp);
            load_convolutional_weights(*(l.self_layer), fp);
            load_convolutional_weights(*(l.output_layer), fp);
        }
        எனில்(l.type == RNN){
            load_connected_weights(*(l.உள்ளீடு_layer), fp, transpose);
            load_connected_weights(*(l.self_layer), fp, transpose);
            load_connected_weights(*(l.output_layer), fp, transpose);
        }
        எனில் (l.type == LSTM) {
            load_connected_weights(*(l.wi), fp, transpose);
            load_connected_weights(*(l.wf), fp, transpose);
            load_connected_weights(*(l.wo), fp, transpose);
            load_connected_weights(*(l.wg), fp, transpose);
            load_connected_weights(*(l.ui), fp, transpose);
            load_connected_weights(*(l.uf), fp, transpose);
            load_connected_weights(*(l.uo), fp, transpose);
            load_connected_weights(*(l.ug), fp, transpose);
        }
        எனில் (l.type == GRU) {
            எனில்(1){
                load_connected_weights(*(l.wz), fp, transpose);
                load_connected_weights(*(l.wr), fp, transpose);
                load_connected_weights(*(l.wh), fp, transpose);
                load_connected_weights(*(l.uz), fp, transpose);
                load_connected_weights(*(l.ur), fp, transpose);
                load_connected_weights(*(l.uh), fp, transpose);
            }இல்லையெனில்{
                load_connected_weights(*(l.reset_layer), fp, transpose);
                load_connected_weights(*(l.update_layer), fp, transpose);
                load_connected_weights(*(l.state_layer), fp, transpose);
            }
        }
        எனில்(l.type == LOCAL){
            முழு locations = l.out_w*l.out_h;
            முழு size = l.size*l.size*l.c*l.n*locations;
            fread(l.biases, அளவு(தசம), l.outputs, fp);
            fread(l.weights, அளவு(தசம), size, fp);
#ifdef GPU
            எனில்(gpu_index >= 0){
                தள்ளு_local_layer(l);
            }
#endif
        }
    }
    fprintf(stderr, "Done!\n");
    fclose(fp);
}

வெறுமை load_weights(network *net, எழுத்து *filename)
{
    load_weights_upto(net, filename, 0, net->n);
}


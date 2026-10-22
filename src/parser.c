#شامل <stdio.h>
#شامل <string.h>
#شامل <stdlib.h>
#شامل <assert.h>

#شامل "activation_layer.h"
#شامل "logistic_layer.h"
#شامل "l2norm_layer.h"
#شامل "activations.h"
#شامل "avgpool_layer.h"
#شامل "batchnorm_layer.h"
#شامل "blas.h"
#شامل "connected_layer.h"
#شامل "deconvolutional_layer.h"
#شامل "convolutional_layer.h"
#شامل "cost_layer.h"
#شامل "crnn_layer.h"
#شامل "crop_layer.h"
#شامل "detection_layer.h"
#شامل "dropout_layer.h"
#شامل "gru_layer.h"
#شامل "list.h"
#شامل "local_layer.h"
#شامل "maxpool_layer.h"
#شامل "normalization_layer.h"
#شامل "option_list.h"
#شامل "parser.h"
#شامل "region_layer.h"
#شامل "yolo_layer.h"
#شامل "iseg_layer.h"
#شامل "reorg_layer.h"
#شامل "rnn_layer.h"
#شامل "route_layer.h"
#شامل "upsample_layer.h"
#شامل "shortcut_layer.h"
#شامل "softmax_layer.h"
#شامل "lstm_layer.h"
#شامل "utils.h"

قسمتعریف ساخت{
    حرف *type;
    list *options;
}section;

list *read_cfg(حرف *filename);

LAYER_TYPE string_مقصد_layer_type(حرف * type)
{

    اگر (strcmp(type, "[shortcut]")==0) واپس SHORTCUT;
    اگر (strcmp(type, "[crop]")==0) واپس CROP;
    اگر (strcmp(type, "[cost]")==0) واپس لاگت;
    اگر (strcmp(type, "[detection]")==0) واپس DETECTION;
    اگر (strcmp(type, "[region]")==0) واپس REGION;
    اگر (strcmp(type, "[yolo]")==0) واپس YOLO;
    اگر (strcmp(type, "[iseg]")==0) واپس ISEG;
    اگر (strcmp(type, "[local]")==0) واپس LOCAL;
    اگر (strcmp(type, "[conv]")==0
            || strcmp(type, "[convolutional]")==0) واپس CONVOLUTIONAL;
    اگر (strcmp(type, "[deconv]")==0
            || strcmp(type, "[deconvolutional]")==0) واپس DECONVOLUTIONAL;
    اگر (strcmp(type, "[activation]")==0) واپس فعال;
    اگر (strcmp(type, "[logistic]")==0) واپس LOGXENT;
    اگر (strcmp(type, "[l2norm]")==0) واپس L2NORM;
    اگر (strcmp(type, "[net]")==0
            || strcmp(type, "[network]")==0) واپس جال_2;
    اگر (strcmp(type, "[crnn]")==0) واپس CRNN;
    اگر (strcmp(type, "[gru]")==0) واپس GRU;
    اگر (strcmp(type, "[lstm]") == 0) واپس LSTM;
    اگر (strcmp(type, "[rnn]")==0) واپس RNN;
    اگر (strcmp(type, "[conn]")==0
            || strcmp(type, "[connected]")==0) واپس CONNECTED;
    اگر (strcmp(type, "[max]")==0
            || strcmp(type, "[maxpool]")==0) واپس MAXPOOL;
    اگر (strcmp(type, "[reorg]")==0) واپس REORG;
    اگر (strcmp(type, "[avg]")==0
            || strcmp(type, "[avgpool]")==0) واپس AVGPOOL;
    اگر (strcmp(type, "[dropout]")==0) واپس DROPOUT;
    اگر (strcmp(type, "[lrn]")==0
            || strcmp(type, "[normalization]")==0) واپس NORMALIZATION;
    اگر (strcmp(type, "[batchnorm]")==0) واپس BATCHNORM;
    اگر (strcmp(type, "[soft]")==0
            || strcmp(type, "[softmax]")==0) واپس SOFTMAX;
    اگر (strcmp(type, "[route]")==0) واپس ROUTE;
    اگر (strcmp(type, "[upsample]")==0) واپس UPSAMPLE;
    واپس BLANK;
}

خالی free_section(section *s)
{
    آزاد(s->type);
    گرہ *n = s->options->front;
    جب(n){
        kvp *pair = (kvp *)n->val;
        آزاد(pair->کلید);
        آزاد(pair);
        گرہ *اگلا = n->اگلا;
        آزاد(n);
        n = اگلا;
    }
    آزاد(s->options);
    آزاد(s);
}

خالی parse_ڈیٹا(حرف *ڈیٹا, اعشاری *a, صحیح n)
{
    صحیح i;
    اگر(!ڈیٹا) واپس;
    حرف *curr = ڈیٹا;
    حرف *اگلا = ڈیٹا;
    صحیح مکمل = 0;
    برائے(i = 0; i < n && !مکمل; ++i){
        جب(*++اگلا !='\0' && *اگلا != ',');
        اگر(*اگلا == '\0') مکمل = 1;
        *اگلا = '\0';
        لڑی_سے_پڑھو(curr, "%g", &a[i]);
        curr = اگلا+1;
    }
}

قسمتعریف ساخت سائز_params{
    صحیح batch;
    صحیح inputs;
    صحیح h;
    صحیح w;
    صحیح c;
    صحیح اشاریہ;
    صحیح وقت_اقدام;
    جال *net;
} سائز_params;

local_layer parse_local(list *options, سائز_params params)
{
    صحیح n = option_ڈھونڈو_int(options, "filters",1);
    صحیح سائز = option_ڈھونڈو_int(options, "size",1);
    صحیح stride = option_ڈھونڈو_int(options, "stride",1);
    صحیح pad = option_ڈھونڈو_int(options, "pad",0);
    حرف *activation_s = option_ڈھونڈو_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    صحیح batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    اگر(!(h && w && c)) error("Layer before local layer must output image.");

    local_layer layer = make_local_layer(batch,h,w,c,n,سائز,stride,pad,activation);

    واپس layer;
}

layer parse_deconvolutional(list *options, سائز_params params)
{
    صحیح n = option_ڈھونڈو_int(options, "filters",1);
    صحیح سائز = option_ڈھونڈو_int(options, "size",1);
    صحیح stride = option_ڈھونڈو_int(options, "stride",1);

    حرف *activation_s = option_ڈھونڈو_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    صحیح batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    اگر(!(h && w && c)) error("Layer before deconvolutional layer must output image.");
    صحیح batch_normalize = option_ڈھونڈو_int_quiet(options, "batch_normalize", 0);
    صحیح pad = option_ڈھونڈو_int_quiet(options, "pad",0);
    صحیح padding = option_ڈھونڈو_int_quiet(options, "padding",0);
    اگر(pad) padding = سائز/2;

    layer l = make_deconvolutional_layer(batch,h,w,c,n,سائز,stride,padding, activation, batch_normalize, params.net->adam);

    واپس l;
}


convolutional_layer parse_convolutional(list *options, سائز_params params)
{
    صحیح n = option_ڈھونڈو_int(options, "filters",1);
    صحیح سائز = option_ڈھونڈو_int(options, "size",1);
    صحیح stride = option_ڈھونڈو_int(options, "stride",1);
    صحیح pad = option_ڈھونڈو_int_quiet(options, "pad",0);
    صحیح padding = option_ڈھونڈو_int_quiet(options, "padding",0);
    صحیح groups = option_ڈھونڈو_int_quiet(options, "groups", 1);
    اگر(pad) padding = سائز/2;

    حرف *activation_s = option_ڈھونڈو_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    صحیح batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    اگر(!(h && w && c)) error("Layer before convolutional layer must output image.");
    صحیح batch_normalize = option_ڈھونڈو_int_quiet(options, "batch_normalize", 0);
    صحیح ثنائی = option_ڈھونڈو_int_quiet(options, "binary", 0);
    صحیح xnor = option_ڈھونڈو_int_quiet(options, "xnor", 0);

    convolutional_layer layer = make_convolutional_layer(batch,h,w,c,n,groups,سائز,stride,padding,activation, batch_normalize, ثنائی, xnor, params.net->adam);
    layer.flipped = option_ڈھونڈو_int_quiet(options, "flipped", 0);
    layer.dot = option_ڈھونڈو_float_quiet(options, "dot", 0);

    واپس layer;
}

layer parse_crnn(list *options, سائز_params params)
{
    صحیح output_filters = option_ڈھونڈو_int(options, "output_filters",1);
    صحیح hidden_filters = option_ڈھونڈو_int(options, "hidden_filters",1);
    حرف *activation_s = option_ڈھونڈو_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    صحیح batch_normalize = option_ڈھونڈو_int_quiet(options, "batch_normalize", 0);

    layer l = make_crnn_layer(params.batch, params.w, params.h, params.c, hidden_filters, output_filters, params.وقت_اقدام, activation, batch_normalize);

    l.shortcut = option_ڈھونڈو_int_quiet(options, "shortcut", 0);

    واپس l;
}

layer parse_rnn(list *options, سائز_params params)
{
    صحیح output = option_ڈھونڈو_int(options, "output",1);
    حرف *activation_s = option_ڈھونڈو_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    صحیح batch_normalize = option_ڈھونڈو_int_quiet(options, "batch_normalize", 0);

    layer l = make_rnn_layer(params.batch, params.inputs, output, params.وقت_اقدام, activation, batch_normalize, params.net->adam);

    l.shortcut = option_ڈھونڈو_int_quiet(options, "shortcut", 0);

    واپس l;
}

layer parse_gru(list *options, سائز_params params)
{
    صحیح output = option_ڈھونڈو_int(options, "output",1);
    صحیح batch_normalize = option_ڈھونڈو_int_quiet(options, "batch_normalize", 0);

    layer l = make_gru_layer(params.batch, params.inputs, output, params.وقت_اقدام, batch_normalize, params.net->adam);
    l.tanh = option_ڈھونڈو_int_quiet(options, "tanh", 0);

    واپس l;
}

layer parse_lstm(list *options, سائز_params params)
{
    صحیح output = option_ڈھونڈو_int(options, "output", 1);
    صحیح batch_normalize = option_ڈھونڈو_int_quiet(options, "batch_normalize", 0);

    layer l = make_lstm_layer(params.batch, params.inputs, output, params.وقت_اقدام, batch_normalize, params.net->adam);

    واپس l;
}

layer parse_connected(list *options, سائز_params params)
{
    صحیح output = option_ڈھونڈو_int(options, "output",1);
    حرف *activation_s = option_ڈھونڈو_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    صحیح batch_normalize = option_ڈھونڈو_int_quiet(options, "batch_normalize", 0);

    layer l = make_connected_layer(params.batch, params.inputs, output, activation, batch_normalize, params.net->adam);
    واپس l;
}

layer parse_softmax(list *options, سائز_params params)
{
    صحیح groups = option_ڈھونڈو_int_quiet(options, "groups",1);
    layer l = make_softmax_layer(params.batch, params.inputs, groups);
    l.temperature = option_ڈھونڈو_float_quiet(options, "temperature", 1);
    حرف *درخت_file = option_ڈھونڈو_str(options, "tree", 0);
    اگر (درخت_file) l.softmax_درخت = read_درخت(درخت_file);
    l.w = params.w;
    l.h = params.h;
    l.c = params.c;
    l.spatial = option_ڈھونڈو_float_quiet(options, "spatial", 0);
    l.noloss =  option_ڈھونڈو_int_quiet(options, "noloss", 0);
    واپس l;
}

صحیح *parse_yolo_نقاب(حرف *a, صحیح *عدد)
{
    صحیح *نقاب = 0;
    اگر(a){
        صحیح len = لڑی_لمبائی(a);
        صحیح n = 1;
        صحیح i;
        برائے(i = 0; i < len; ++i){
            اگر (a[i] == ',') ++n;
        }
        نقاب = صفر_حافظہ_دو(n, حجم(صحیح));
        برائے(i = 0; i < n; ++i){
            صحیح val = atoi(a);
            نقاب[i] = val;
            a = strchr(a, ',')+1;
        }
        *عدد = n;
    }
    واپس نقاب;
}

layer parse_yolo(list *options, سائز_params params)
{
    صحیح classes = option_ڈھونڈو_int(options, "classes", 20);
    صحیح کل = option_ڈھونڈو_int(options, "num", 1);
    صحیح عدد = کل;

    حرف *a = option_ڈھونڈو_str(options, "mask", 0);
    صحیح *نقاب = parse_yolo_نقاب(a, &عدد);
    layer l = make_yolo_layer(params.batch, params.w, params.h, عدد, کل, نقاب, classes);
    أكد(l.outputs == params.inputs);

    l.max_boxes = option_ڈھونڈو_int_quiet(options, "max",90);
    l.jitter = option_ڈھونڈو_float(options, "jitter", .2);

    l.ignore_thresh = option_ڈھونڈو_float(options, "ignore_thresh", .5);
    l.truth_thresh = option_ڈھونڈو_float(options, "truth_thresh", 1);
    l.random = option_ڈھونڈو_int_quiet(options, "random", 0);

    حرف *map_file = option_ڈھونڈو_str(options, "map", 0);
    اگر (map_file) l.map = read_map(map_file);

    a = option_ڈھونڈو_str(options, "anchors", 0);
    اگر(a){
        صحیح len = لڑی_لمبائی(a);
        صحیح n = 1;
        صحیح i;
        برائے(i = 0; i < len; ++i){
            اگر (a[i] == ',') ++n;
        }
        برائے(i = 0; i < n; ++i){
            اعشاری bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    واپس l;
}

layer parse_iseg(list *options, سائز_params params)
{
    صحیح classes = option_ڈھونڈو_int(options, "classes", 20);
    صحیح ids = option_ڈھونڈو_int(options, "ids", 32);
    layer l = make_iseg_layer(params.batch, params.w, params.h, classes, ids);
    أكد(l.outputs == params.inputs);
    واپس l;
}

layer parse_region(list *options, سائز_params params)
{
    صحیح coords = option_ڈھونڈو_int(options, "coords", 4);
    صحیح classes = option_ڈھونڈو_int(options, "classes", 20);
    صحیح عدد = option_ڈھونڈو_int(options, "num", 1);

    layer l = make_region_layer(params.batch, params.w, params.h, عدد, classes, coords);
    أكد(l.outputs == params.inputs);

    l.لوگارتھم = option_ڈھونڈو_int_quiet(options, "log", 0);
    l.جذر = option_ڈھونڈو_int_quiet(options, "sqrt", 0);

    l.softmax = option_ڈھونڈو_int(options, "softmax", 0);
    l.background = option_ڈھونڈو_int_quiet(options, "background", 0);
    l.max_boxes = option_ڈھونڈو_int_quiet(options, "max",30);
    l.jitter = option_ڈھونڈو_float(options, "jitter", .2);
    l.rescore = option_ڈھونڈو_int_quiet(options, "rescore",0);

    l.thresh = option_ڈھونڈو_float(options, "thresh", .5);
    l.classfix = option_ڈھونڈو_int_quiet(options, "classfix", 0);
    l.absolute = option_ڈھونڈو_int_quiet(options, "absolute", 0);
    l.random = option_ڈھونڈو_int_quiet(options, "random", 0);

    l.coord_scale = option_ڈھونڈو_float(options, "coord_scale", 1);
    l.object_scale = option_ڈھونڈو_float(options, "object_scale", 1);
    l.noobject_scale = option_ڈھونڈو_float(options, "noobject_scale", 1);
    l.نقاب_scale = option_ڈھونڈو_float(options, "mask_scale", 1);
    l.class_scale = option_ڈھونڈو_float(options, "class_scale", 1);
    l.bias_مطابقت = option_ڈھونڈو_int_quiet(options, "bias_match",0);

    حرف *درخت_file = option_ڈھونڈو_str(options, "tree", 0);
    اگر (درخت_file) l.softmax_درخت = read_درخت(درخت_file);
    حرف *map_file = option_ڈھونڈو_str(options, "map", 0);
    اگر (map_file) l.map = read_map(map_file);

    حرف *a = option_ڈھونڈو_str(options, "anchors", 0);
    اگر(a){
        صحیح len = لڑی_لمبائی(a);
        صحیح n = 1;
        صحیح i;
        برائے(i = 0; i < len; ++i){
            اگر (a[i] == ',') ++n;
        }
        برائے(i = 0; i < n; ++i){
            اعشاری bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    واپس l;
}

detection_layer parse_detection(list *options, سائز_params params)
{
    صحیح coords = option_ڈھونڈو_int(options, "coords", 1);
    صحیح classes = option_ڈھونڈو_int(options, "classes", 1);
    صحیح rescore = option_ڈھونڈو_int(options, "rescore", 0);
    صحیح عدد = option_ڈھونڈو_int(options, "num", 1);
    صحیح side = option_ڈھونڈو_int(options, "side", 7);
    detection_layer layer = make_detection_layer(params.batch, params.inputs, عدد, side, classes, coords, rescore);

    layer.softmax = option_ڈھونڈو_int(options, "softmax", 0);
    layer.جذر = option_ڈھونڈو_int(options, "sqrt", 0);

    layer.max_boxes = option_ڈھونڈو_int_quiet(options, "max",90);
    layer.coord_scale = option_ڈھونڈو_float(options, "coord_scale", 1);
    layer.forced = option_ڈھونڈو_int(options, "forced", 0);
    layer.object_scale = option_ڈھونڈو_float(options, "object_scale", 1);
    layer.noobject_scale = option_ڈھونڈو_float(options, "noobject_scale", 1);
    layer.class_scale = option_ڈھونڈو_float(options, "class_scale", 1);
    layer.jitter = option_ڈھونڈو_float(options, "jitter", .2);
    layer.random = option_ڈھونڈو_int_quiet(options, "random", 0);
    layer.reorg = option_ڈھونڈو_int_quiet(options, "reorg", 0);
    واپس layer;
}

لاگت_layer parse_لاگت(list *options, سائز_params params)
{
    حرف *type_s = option_ڈھونڈو_str(options, "type", "sse");
    لاگت_TYPE type = get_لاگت_type(type_s);
    اعشاری scale = option_ڈھونڈو_float_quiet(options, "scale",1);
    لاگت_layer layer = make_لاگت_layer(params.batch, params.inputs, type, scale);
    layer.تناسب =  option_ڈھونڈو_float_quiet(options, "ratio",0);
    layer.noobject_scale =  option_ڈھونڈو_float_quiet(options, "noobj", 1);
    layer.thresh =  option_ڈھونڈو_float_quiet(options, "thresh",0);
    واپس layer;
}

crop_layer parse_crop(list *options, سائز_params params)
{
    صحیح crop_اونچائی = option_ڈھونڈو_int(options, "crop_height",1);
    صحیح crop_چوڑائی = option_ڈھونڈو_int(options, "crop_width",1);
    صحیح flip = option_ڈھونڈو_int(options, "flip",0);
    اعشاری angle = option_ڈھونڈو_float(options, "angle",0);
    اعشاری saturation = option_ڈھونڈو_float(options, "saturation",1);
    اعشاری exposure = option_ڈھونڈو_float(options, "exposure",1);

    صحیح batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    اگر(!(h && w && c)) error("Layer before crop layer must output image.");

    صحیح noadjust = option_ڈھونڈو_int_quiet(options, "noadjust",0);

    crop_layer l = make_crop_layer(batch,h,w,c,crop_اونچائی,crop_چوڑائی,flip, angle, saturation, exposure);
    l.سرکاؤ = option_ڈھونڈو_float(options, "shift", 0);
    l.noadjust = noadjust;
    واپس l;
}

layer parse_reorg(list *options, سائز_params params)
{
    صحیح stride = option_ڈھونڈو_int(options, "stride",1);
    صحیح الٹ = option_ڈھونڈو_int_quiet(options, "reverse",0);
    صحیح flatten = option_ڈھونڈو_int_quiet(options, "flatten",0);
    صحیح extra = option_ڈھونڈو_int_quiet(options, "extra",0);

    صحیح batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    اگر(!(h && w && c)) error("Layer before reorg layer must output image.");

    layer layer = make_reorg_layer(batch,w,h,c,stride,الٹ, flatten, extra);
    واپس layer;
}

maxpool_layer parse_maxpool(list *options, سائز_params params)
{
    صحیح stride = option_ڈھونڈو_int(options, "stride",1);
    صحیح سائز = option_ڈھونڈو_int(options, "size",stride);
    صحیح padding = option_ڈھونڈو_int_quiet(options, "padding", سائز-1);

    صحیح batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    اگر(!(h && w && c)) error("Layer before maxpool layer must output image.");

    maxpool_layer layer = make_maxpool_layer(batch,h,w,c,سائز,stride,padding);
    واپس layer;
}

avgpool_layer parse_avgpool(list *options, سائز_params params)
{
    صحیح batch,w,h,c;
    w = params.w;
    h = params.h;
    c = params.c;
    batch=params.batch;
    اگر(!(h && w && c)) error("Layer before avgpool layer must output image.");

    avgpool_layer layer = make_avgpool_layer(batch,w,h,c);
    واپس layer;
}

dropout_layer parse_dropout(list *options, سائز_params params)
{
    اعشاری probability = option_ڈھونڈو_float(options, "probability", .5);
    dropout_layer layer = make_dropout_layer(params.batch, params.inputs, probability);
    layer.برآمد_w = params.w;
    layer.برآمد_h = params.h;
    layer.برآمد_c = params.c;
    واپس layer;
}

layer parse_normalization(list *options, سائز_params params)
{
    اعشاری alpha = option_ڈھونڈو_float(options, "alpha", .0001);
    اعشاری beta =  option_ڈھونڈو_float(options, "beta" , .75);
    اعشاری kappa = option_ڈھونڈو_float(options, "kappa", 1);
    صحیح سائز = option_ڈھونڈو_int(options, "size", 5);
    layer l = make_normalization_layer(params.batch, params.w, params.h, params.c, سائز, alpha, beta, kappa);
    واپس l;
}

layer parse_batchnorm(list *options, سائز_params params)
{
    layer l = make_batchnorm_layer(params.batch, params.w, params.h, params.c);
    واپس l;
}

layer parse_shortcut(list *options, سائز_params params, جال *net)
{
    حرف *l = option_ڈھونڈو(options, "from");
    صحیح اشاریہ = atoi(l);
    اگر(اشاریہ < 0) اشاریہ = params.اشاریہ + اشاریہ;

    صحیح batch = params.batch;
    layer from = net->layers[اشاریہ];

    layer s = make_shortcut_layer(batch, اشاریہ, params.w, params.h, params.c, from.برآمد_w, from.برآمد_h, from.برآمد_c);

    حرف *activation_s = option_ڈھونڈو_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);
    s.activation = activation;
    s.alpha = option_ڈھونڈو_float_quiet(options, "alpha", 1);
    s.beta = option_ڈھونڈو_float_quiet(options, "beta", 1);
    واپس s;
}


layer parse_l2norm(list *options, سائز_params params)
{
    layer l = make_l2norm_layer(params.batch, params.inputs);
    l.h = l.برآمد_h = params.h;
    l.w = l.برآمد_w = params.w;
    l.c = l.برآمد_c = params.c;
    واپس l;
}


layer parse_logistic(list *options, سائز_params params)
{
    layer l = make_logistic_layer(params.batch, params.inputs);
    l.h = l.برآمد_h = params.h;
    l.w = l.برآمد_w = params.w;
    l.c = l.برآمد_c = params.c;
    واپس l;
}

layer parse_activation(list *options, سائز_params params)
{
    حرف *activation_s = option_ڈھونڈو_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);

    layer l = make_activation_layer(params.batch, params.inputs, activation);

    l.h = l.برآمد_h = params.h;
    l.w = l.برآمد_w = params.w;
    l.c = l.برآمد_c = params.c;

    واپس l;
}

layer parse_upsample(list *options, سائز_params params, جال *net)
{

    صحیح stride = option_ڈھونڈو_int(options, "stride",2);
    layer l = make_upsample_layer(params.batch, params.w, params.h, params.c, stride);
    l.scale = option_ڈھونڈو_float_quiet(options, "scale", 1);
    واپس l;
}

route_layer parse_route(list *options, سائز_params params, جال *net)
{
    حرف *l = option_ڈھونڈو(options, "layers");
    صحیح len = لڑی_لمبائی(l);
    اگر(!l) error("Route Layer must specify input layers");
    صحیح n = 1;
    صحیح i;
    برائے(i = 0; i < len; ++i){
        اگر (l[i] == ',') ++n;
    }

    صحیح *layers = صفر_حافظہ_دو(n, حجم(صحیح));
    صحیح *sizes = صفر_حافظہ_دو(n, حجم(صحیح));
    برائے(i = 0; i < n; ++i){
        صحیح اشاریہ = atoi(l);
        l = strchr(l, ',')+1;
        اگر(اشاریہ < 0) اشاریہ = params.اشاریہ + اشاریہ;
        layers[i] = اشاریہ;
        sizes[i] = net->layers[اشاریہ].outputs;
    }
    صحیح batch = params.batch;

    route_layer layer = make_route_layer(batch, n, layers, sizes);

    convolutional_layer پہلا = net->layers[layers[0]];
    layer.برآمد_w = پہلا.برآمد_w;
    layer.برآمد_h = پہلا.برآمد_h;
    layer.برآمد_c = پہلا.برآمد_c;
    برائے(i = 1; i < n; ++i){
        صحیح اشاریہ = layers[i];
        convolutional_layer اگلا = net->layers[اشاریہ];
        اگر(اگلا.برآمد_w == پہلا.برآمد_w && اگلا.برآمد_h == پہلا.برآمد_h){
            layer.برآمد_c += اگلا.برآمد_c;
        }ورنہ{
            layer.برآمد_h = layer.برآمد_w = layer.برآمد_c = 0;
        }
    }

    واپس layer;
}

learning_شرح_policy get_policy(حرف *s)
{
    اگر (strcmp(s, "random")==0) واپس RANDOM;
    اگر (strcmp(s, "poly")==0) واپس POLY;
    اگر (strcmp(s, "constant")==0) واپس CONSTANT;
    اگر (strcmp(s, "step")==0) واپس قدم_2;
    اگر (strcmp(s, "exp")==0) واپس EXP;
    اگر (strcmp(s, "sigmoid")==0) واپس SIG;
    اگر (strcmp(s, "steps")==0) واپس اقدام_2;
    fprintf(stderr, "Couldn't find policy %s, going with constant\n", s);
    واپس CONSTANT;
}

خالی parse_net_options(list *options, جال *net)
{
    net->batch = option_ڈھونڈو_int(options, "batch",1);
    net->learning_شرح = option_ڈھونڈو_float(options, "learning_rate", .001);
    net->momentum = option_ڈھونڈو_float(options, "momentum", .9);
    net->decay = option_ڈھونڈو_float(options, "decay", .0001);
    صحیح subdivs = option_ڈھونڈو_int(options, "subdivisions",1);
    net->وقت_اقدام = option_ڈھونڈو_int_quiet(options, "time_steps",1);
    net->notruth = option_ڈھونڈو_int_quiet(options, "notruth",0);
    net->batch /= subdivs;
    net->batch *= net->وقت_اقدام;
    net->subdivisions = subdivs;
    net->random = option_ڈھونڈو_int_quiet(options, "random", 0);

    net->adam = option_ڈھونڈو_int_quiet(options, "adam", 0);
    اگر(net->adam){
        net->B1 = option_ڈھونڈو_float(options, "B1", .9);
        net->B2 = option_ڈھونڈو_float(options, "B2", .999);
        net->eps = option_ڈھونڈو_float(options, "eps", .0000001);
    }

    net->h = option_ڈھونڈو_int_quiet(options, "height",0);
    net->w = option_ڈھونڈو_int_quiet(options, "width",0);
    net->c = option_ڈھونڈو_int_quiet(options, "channels",0);
    net->inputs = option_ڈھونڈو_int_quiet(options, "inputs", net->h * net->w * net->c);
    net->max_crop = option_ڈھونڈو_int_quiet(options, "max_crop",net->w*2);
    net->min_crop = option_ڈھونڈو_int_quiet(options, "min_crop",net->w);
    net->max_تناسب = option_ڈھونڈو_float_quiet(options, "max_ratio", (اعشاری) net->max_crop / net->w);
    net->min_تناسب = option_ڈھونڈو_float_quiet(options, "min_ratio", (اعشاری) net->min_crop / net->w);
    net->center = option_ڈھونڈو_int_quiet(options, "center",0);
    net->clip = option_ڈھونڈو_float_quiet(options, "clip", 0);

    net->angle = option_ڈھونڈو_float_quiet(options, "angle", 0);
    net->aspect = option_ڈھونڈو_float_quiet(options, "aspect", 1);
    net->saturation = option_ڈھونڈو_float_quiet(options, "saturation", 1);
    net->exposure = option_ڈھونڈو_float_quiet(options, "exposure", 1);
    net->hue = option_ڈھونڈو_float_quiet(options, "hue", 0);

    اگر(!net->inputs && !(net->h && net->w && net->c)) error("No input parameters supplied");

    حرف *policy_s = option_ڈھونڈو_str(options, "policy", "constant");
    net->policy = get_policy(policy_s);
    net->burn_اندر = option_ڈھونڈو_int_quiet(options, "burn_in", 0);
    net->power = option_ڈھونڈو_float_quiet(options, "power", 4);
    اگر(net->policy == قدم_2){
        net->قدم = option_ڈھونڈو_int(options, "step", 1);
        net->scale = option_ڈھونڈو_float(options, "scale", 1);
    } ورنہ اگر (net->policy == اقدام_2){
        حرف *l = option_ڈھونڈو(options, "steps");
        حرف *p = option_ڈھونڈو(options, "scales");
        اگر(!l || !p) error("STEPS policy must have steps and scales in cfg file");

        صحیح len = لڑی_لمبائی(l);
        صحیح n = 1;
        صحیح i;
        برائے(i = 0; i < len; ++i){
            اگر (l[i] == ',') ++n;
        }
        صحیح *اقدام = صفر_حافظہ_دو(n, حجم(صحیح));
        اعشاری *scales = صفر_حافظہ_دو(n, حجم(اعشاری));
        برائے(i = 0; i < n; ++i){
            صحیح قدم    = atoi(l);
            اعشاری scale = atof(p);
            l = strchr(l, ',')+1;
            p = strchr(p, ',')+1;
            اقدام[i] = قدم;
            scales[i] = scale;
        }
        net->scales = scales;
        net->اقدام = اقدام;
        net->عدد_اقدام = n;
    } ورنہ اگر (net->policy == EXP){
        net->gamma = option_ڈھونڈو_float(options, "gamma", 1);
    } ورنہ اگر (net->policy == SIG){
        net->gamma = option_ڈھونڈو_float(options, "gamma", 1);
        net->قدم = option_ڈھونڈو_int(options, "step", 1);
    } ورنہ اگر (net->policy == POLY || net->policy == RANDOM){
    }
    net->max_batches = option_ڈھونڈو_int(options, "max_batches", 0);
}

صحیح is_جال(section *s)
{
    واپس (strcmp(s->type, "[net]")==0
            || strcmp(s->type, "[network]")==0);
}

جال *parse_جال_cfg(حرف *filename)
{
    list *sections = read_cfg(filename);
    گرہ *n = sections->front;
    اگر(!n) error("Config file has no sections");
    جال *net = make_جال(sections->سائز - 1);
    net->gpu_اشاریہ = gpu_اشاریہ;
    سائز_params params;

    section *s = (section *)n->val;
    list *options = s->options;
    اگر(!is_جال(s)) error("First section must be [net] or [network]");
    parse_net_options(options, net);

    params.h = net->h;
    params.w = net->w;
    params.c = net->c;
    params.inputs = net->inputs;
    params.batch = net->batch;
    params.وقت_اقدام = net->وقت_اقدام;
    params.net = net;

    size_t workspace_سائز = 0;
    n = n->اگلا;
    صحیح گنتی = 0;
    free_section(s);
    fprintf(stderr, "layer     filters    size              input                output\n");
    جب(n){
        params.اشاریہ = گنتی;
        fprintf(stderr, "%5d ", گنتی);
        s = (section *)n->val;
        options = s->options;
        layer l = {0};
        LAYER_TYPE lt = string_مقصد_layer_type(s->type);
        اگر(lt == CONVOLUTIONAL){
            l = parse_convolutional(options, params);
        }ورنہ اگر(lt == DECONVOLUTIONAL){
            l = parse_deconvolutional(options, params);
        }ورنہ اگر(lt == LOCAL){
            l = parse_local(options, params);
        }ورنہ اگر(lt == فعال){
            l = parse_activation(options, params);
        }ورنہ اگر(lt == LOGXENT){
            l = parse_logistic(options, params);
        }ورنہ اگر(lt == L2NORM){
            l = parse_l2norm(options, params);
        }ورنہ اگر(lt == RNN){
            l = parse_rnn(options, params);
        }ورنہ اگر(lt == GRU){
            l = parse_gru(options, params);
        }ورنہ اگر (lt == LSTM) {
            l = parse_lstm(options, params);
        }ورنہ اگر(lt == CRNN){
            l = parse_crnn(options, params);
        }ورنہ اگر(lt == CONNECTED){
            l = parse_connected(options, params);
        }ورنہ اگر(lt == CROP){
            l = parse_crop(options, params);
        }ورنہ اگر(lt == لاگت){
            l = parse_لاگت(options, params);
        }ورنہ اگر(lt == REGION){
            l = parse_region(options, params);
        }ورنہ اگر(lt == YOLO){
            l = parse_yolo(options, params);
        }ورنہ اگر(lt == ISEG){
            l = parse_iseg(options, params);
        }ورنہ اگر(lt == DETECTION){
            l = parse_detection(options, params);
        }ورنہ اگر(lt == SOFTMAX){
            l = parse_softmax(options, params);
            net->hierarchy = l.softmax_درخت;
        }ورنہ اگر(lt == NORMALIZATION){
            l = parse_normalization(options, params);
        }ورنہ اگر(lt == BATCHNORM){
            l = parse_batchnorm(options, params);
        }ورنہ اگر(lt == MAXPOOL){
            l = parse_maxpool(options, params);
        }ورنہ اگر(lt == REORG){
            l = parse_reorg(options, params);
        }ورنہ اگر(lt == AVGPOOL){
            l = parse_avgpool(options, params);
        }ورنہ اگر(lt == ROUTE){
            l = parse_route(options, params, net);
        }ورنہ اگر(lt == UPSAMPLE){
            l = parse_upsample(options, params, net);
        }ورنہ اگر(lt == SHORTCUT){
            l = parse_shortcut(options, params, net);
        }ورنہ اگر(lt == DROPOUT){
            l = parse_dropout(options, params);
            l.output = net->layers[گنتی-1].output;
            l.delta = net->layers[گنتی-1].delta;
#اگر_تعریف_شدہ GPU
            l.output_gpu = net->layers[گنتی-1].output_gpu;
            l.delta_gpu = net->layers[گنتی-1].delta_gpu;
#اگر_ختم
        }ورنہ{
            fprintf(stderr, "Type not recognized: %s\n", s->type);
        }
        l.clip = net->clip;
        l.truth = option_ڈھونڈو_int_quiet(options, "truth", 0);
        l.onlyforward = option_ڈھونڈو_int_quiet(options, "onlyforward", 0);
        l.stopbackward = option_ڈھونڈو_int_quiet(options, "stopbackward", 0);
        l.dontsave = option_ڈھونڈو_int_quiet(options, "dontsave", 0);
        l.dontload = option_ڈھونڈو_int_quiet(options, "dontload", 0);
        l.numload = option_ڈھونڈو_int_quiet(options, "numload", 0);
        l.dontloadscales = option_ڈھونڈو_int_quiet(options, "dontloadscales", 0);
        l.learning_شرح_scale = option_ڈھونڈو_float_quiet(options, "learning_rate", 1);
        l.smooth = option_ڈھونڈو_float_quiet(options, "smooth", 0);
        option_غیر_مستعمل(options);
        net->layers[گنتی] = l;
        اگر (l.workspace_سائز > workspace_سائز) workspace_سائز = l.workspace_سائز;
        free_section(s);
        n = n->اگلا;
        ++گنتی;
        اگر(n){
            params.h = l.برآمد_h;
            params.w = l.برآمد_w;
            params.c = l.برآمد_c;
            params.inputs = l.outputs;
        }
    }
    free_list(sections);
    layer برآمد = get_جال_output_layer(net);
    net->outputs = برآمد.outputs;
    net->truths = برآمد.outputs;
    اگر(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = برآمد.output;
    net->داخلہ = صفر_حافظہ_دو(net->inputs*net->batch, حجم(اعشاری));
    net->truth = صفر_حافظہ_دو(net->truths*net->batch, حجم(اعشاری));
#اگر_تعریف_شدہ GPU
    net->output_gpu = برآمد.output_gpu;
    net->داخلہ_gpu = cuda_make_array(net->داخلہ, net->inputs*net->batch);
    net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
#اگر_ختم
    اگر(workspace_سائز){
        //printf("%ld\n", workspace_size);
#اگر_تعریف_شدہ GPU
        اگر(gpu_اشاریہ >= 0){
            net->workspace = cuda_make_array(0, (workspace_سائز-1)/حجم(اعشاری)+1);
        }ورنہ {
            net->workspace = صفر_حافظہ_دو(1, workspace_سائز);
        }
#ورنہ
        net->workspace = صفر_حافظہ_دو(1, workspace_سائز);
#اگر_ختم
    }
    واپس net;
}

list *read_cfg(حرف *filename)
{
    FILE *file = fopen(filename, "r");
    اگر(file == 0) file_error(filename);
    حرف *سطر;
    صحیح nu = 0;
    list *options = make_list();
    section *current = 0;
    جب((سطر=fgetl(file)) != 0){
        ++ nu;
        strip(سطر);
        انتخاب(سطر[0]){
            حالت '[':
                current = حافظہ_دو(حجم(section));
                list_داخل_کرو(options, current);
                current->options = make_list();
                current->type = سطر;
                توڑو;
            حالت '\0':
            حالت '#':
            حالت ';':
                آزاد(سطر);
                توڑو;
            طےشدہ:
                اگر(!read_option(سطر, current->options)){
                    fprintf(stderr, "Config file error line %d, could parse: %s\n", nu, سطر);
                    آزاد(سطر);
                }
                توڑو;
        }
    }
    fclose(file);
    واپس options;
}

خالی save_convolutional_weights_ثنائی(layer l, FILE *fp)
{
#اگر_تعریف_شدہ GPU
    اگر(gpu_اشاریہ >= 0){
        pull_convolutional_layer(l);
    }
#اگر_ختم
    binarize_weights(l.weights, l.n, l.c*l.سائز*l.سائز, l.ثنائی_weights);
    صحیح سائز = l.c*l.سائز*l.سائز;
    صحیح i, j, k;
    fwrite(l.biases, حجم(اعشاری), l.n, fp);
    اگر (l.batch_normalize){
        fwrite(l.scales, حجم(اعشاری), l.n, fp);
        fwrite(l.rolling_اوسط, حجم(اعشاری), l.n, fp);
        fwrite(l.rolling_variance, حجم(اعشاری), l.n, fp);
    }
    برائے(i = 0; i < l.n; ++i){
        اعشاری اوسط = l.ثنائی_weights[i*سائز];
        اگر(اوسط < 0) اوسط = -اوسط;
        fwrite(&اوسط, حجم(اعشاری), 1, fp);
        برائے(j = 0; j < سائز/8; ++j){
            صحیح اشاریہ = i*سائز + j*8;
            غیرعلامتی حرف c = 0;
            برائے(k = 0; k < 8; ++k){
                اگر (j*8 + k >= سائز) توڑو;
                اگر (l.ثنائی_weights[اشاریہ + k] > 0) c = (c | 1<<k);
            }
            fwrite(&c, حجم(حرف), 1, fp);
        }
    }
}

خالی save_convolutional_weights(layer l, FILE *fp)
{
    اگر(l.ثنائی){
        //save_convolutional_weights_binary(l, fp);
        //return;
    }
#اگر_تعریف_شدہ GPU
    اگر(gpu_اشاریہ >= 0){
        pull_convolutional_layer(l);
    }
#اگر_ختم
    صحیح عدد = l.nweights;
    fwrite(l.biases, حجم(اعشاری), l.n, fp);
    اگر (l.batch_normalize){
        fwrite(l.scales, حجم(اعشاری), l.n, fp);
        fwrite(l.rolling_اوسط, حجم(اعشاری), l.n, fp);
        fwrite(l.rolling_variance, حجم(اعشاری), l.n, fp);
    }
    fwrite(l.weights, حجم(اعشاری), عدد, fp);
}

خالی save_batchnorm_weights(layer l, FILE *fp)
{
#اگر_تعریف_شدہ GPU
    اگر(gpu_اشاریہ >= 0){
        pull_batchnorm_layer(l);
    }
#اگر_ختم
    fwrite(l.scales, حجم(اعشاری), l.c, fp);
    fwrite(l.rolling_اوسط, حجم(اعشاری), l.c, fp);
    fwrite(l.rolling_variance, حجم(اعشاری), l.c, fp);
}

خالی save_connected_weights(layer l, FILE *fp)
{
#اگر_تعریف_شدہ GPU
    اگر(gpu_اشاریہ >= 0){
        pull_connected_layer(l);
    }
#اگر_ختم
    fwrite(l.biases, حجم(اعشاری), l.outputs, fp);
    fwrite(l.weights, حجم(اعشاری), l.outputs*l.inputs, fp);
    اگر (l.batch_normalize){
        fwrite(l.scales, حجم(اعشاری), l.outputs, fp);
        fwrite(l.rolling_اوسط, حجم(اعشاری), l.outputs, fp);
        fwrite(l.rolling_variance, حجم(اعشاری), l.outputs, fp);
    }
}

خالی save_weights_upto(جال *net, حرف *filename, صحیح cutoff)
{
#اگر_تعریف_شدہ GPU
    اگر(net->gpu_اشاریہ >= 0){
        cuda_سیٹ_device(net->gpu_اشاریہ);
    }
#اگر_ختم
    fprintf(stderr, "Saving weights to %s\n", filename);
    FILE *fp = fopen(filename, "wb");
    اگر(!fp) file_error(filename);

    صحیح major = 0;
    صحیح minor = 2;
    صحیح revision = 0;
    fwrite(&major, حجم(صحیح), 1, fp);
    fwrite(&minor, حجم(صحیح), 1, fp);
    fwrite(&revision, حجم(صحیح), 1, fp);
    fwrite(net->نظر_آیا, حجم(size_t), 1, fp);

    صحیح i;
    برائے(i = 0; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        اگر (l.dontsave) جاری;
        اگر(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            save_convolutional_weights(l, fp);
        } اگر(l.type == CONNECTED){
            save_connected_weights(l, fp);
        } اگر(l.type == BATCHNORM){
            save_batchnorm_weights(l, fp);
        } اگر(l.type == RNN){
            save_connected_weights(*(l.داخلہ_layer), fp);
            save_connected_weights(*(l.self_layer), fp);
            save_connected_weights(*(l.output_layer), fp);
        } اگر (l.type == LSTM) {
            save_connected_weights(*(l.wi), fp);
            save_connected_weights(*(l.wf), fp);
            save_connected_weights(*(l.wo), fp);
            save_connected_weights(*(l.wg), fp);
            save_connected_weights(*(l.ui), fp);
            save_connected_weights(*(l.uf), fp);
            save_connected_weights(*(l.uo), fp);
            save_connected_weights(*(l.ug), fp);
        } اگر (l.type == GRU) {
            اگر(1){
                save_connected_weights(*(l.wz), fp);
                save_connected_weights(*(l.wr), fp);
                save_connected_weights(*(l.wh), fp);
                save_connected_weights(*(l.uz), fp);
                save_connected_weights(*(l.ur), fp);
                save_connected_weights(*(l.uh), fp);
            }ورنہ{
                save_connected_weights(*(l.reset_layer), fp);
                save_connected_weights(*(l.update_layer), fp);
                save_connected_weights(*(l.state_layer), fp);
            }
        }  اگر(l.type == CRNN){
            save_convolutional_weights(*(l.داخلہ_layer), fp);
            save_convolutional_weights(*(l.self_layer), fp);
            save_convolutional_weights(*(l.output_layer), fp);
        } اگر(l.type == LOCAL){
#اگر_تعریف_شدہ GPU
            اگر(gpu_اشاریہ >= 0){
                pull_local_layer(l);
            }
#اگر_ختم
            صحیح locations = l.برآمد_w*l.برآمد_h;
            صحیح سائز = l.سائز*l.سائز*l.c*l.n*locations;
            fwrite(l.biases, حجم(اعشاری), l.outputs, fp);
            fwrite(l.weights, حجم(اعشاری), سائز, fp);
        }
    }
    fclose(fp);
}
خالی save_weights(جال *net, حرف *filename)
{
    save_weights_upto(net, filename, net->n);
}

خالی transpose_میٹرکس(اعشاری *a, صحیح صفیں, صحیح ستون_تعداد)
{
    اعشاری *transpose = صفر_حافظہ_دو(صفیں*ستون_تعداد, حجم(اعشاری));
    صحیح x, y;
    برائے(x = 0; x < صفیں; ++x){
        برائے(y = 0; y < ستون_تعداد; ++y){
            transpose[y*صفیں + x] = a[x*ستون_تعداد + y];
        }
    }
    حافظہ_نقل(a, transpose, صفیں*ستون_تعداد*حجم(اعشاری));
    آزاد(transpose);
}

خالی بھرو_connected_weights(layer l, FILE *fp, صحیح transpose)
{
    fread(l.biases, حجم(اعشاری), l.outputs, fp);
    fread(l.weights, حجم(اعشاری), l.outputs*l.inputs, fp);
    اگر(transpose){
        transpose_میٹرکس(l.weights, l.inputs, l.outputs);
    }
    //printf("Biases: %f mean %f variance\n", mean_array(l.biases, l.outputs), variance_array(l.biases, l.outputs));
    //printf("Weights: %f mean %f variance\n", mean_array(l.weights, l.outputs*l.inputs), variance_array(l.weights, l.outputs*l.inputs));
    اگر (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, حجم(اعشاری), l.outputs, fp);
        fread(l.rolling_اوسط, حجم(اعشاری), l.outputs, fp);
        fread(l.rolling_variance, حجم(اعشاری), l.outputs, fp);
        //printf("Scales: %f mean %f variance\n", mean_array(l.scales, l.outputs), variance_array(l.scales, l.outputs));
        //printf("rolling_mean: %f mean %f variance\n", mean_array(l.rolling_mean, l.outputs), variance_array(l.rolling_mean, l.outputs));
        //printf("rolling_variance: %f mean %f variance\n", mean_array(l.rolling_variance, l.outputs), variance_array(l.rolling_variance, l.outputs));
    }
#اگر_تعریف_شدہ GPU
    اگر(gpu_اشاریہ >= 0){
        ڈالو_connected_layer(l);
    }
#اگر_ختم
}

خالی بھرو_batchnorm_weights(layer l, FILE *fp)
{
    fread(l.scales, حجم(اعشاری), l.c, fp);
    fread(l.rolling_اوسط, حجم(اعشاری), l.c, fp);
    fread(l.rolling_variance, حجم(اعشاری), l.c, fp);
#اگر_تعریف_شدہ GPU
    اگر(gpu_اشاریہ >= 0){
        ڈالو_batchnorm_layer(l);
    }
#اگر_ختم
}

خالی بھرو_convolutional_weights_ثنائی(layer l, FILE *fp)
{
    fread(l.biases, حجم(اعشاری), l.n, fp);
    اگر (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, حجم(اعشاری), l.n, fp);
        fread(l.rolling_اوسط, حجم(اعشاری), l.n, fp);
        fread(l.rolling_variance, حجم(اعشاری), l.n, fp);
    }
    صحیح سائز = l.c*l.سائز*l.سائز;
    صحیح i, j, k;
    برائے(i = 0; i < l.n; ++i){
        اعشاری اوسط = 0;
        fread(&اوسط, حجم(اعشاری), 1, fp);
        برائے(j = 0; j < سائز/8; ++j){
            صحیح اشاریہ = i*سائز + j*8;
            غیرعلامتی حرف c = 0;
            fread(&c, حجم(حرف), 1, fp);
            برائے(k = 0; k < 8; ++k){
                اگر (j*8 + k >= سائز) توڑو;
                l.weights[اشاریہ + k] = (c & 1<<k) ? اوسط : -اوسط;
            }
        }
    }
#اگر_تعریف_شدہ GPU
    اگر(gpu_اشاریہ >= 0){
        ڈالو_convolutional_layer(l);
    }
#اگر_ختم
}

خالی بھرو_convolutional_weights(layer l, FILE *fp)
{
    اگر(l.ثنائی){
        //load_convolutional_weights_binary(l, fp);
        //return;
    }
    اگر(l.numload) l.n = l.numload;
    صحیح عدد = l.c/l.groups*l.n*l.سائز*l.سائز;
    fread(l.biases, حجم(اعشاری), l.n, fp);
    اگر (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, حجم(اعشاری), l.n, fp);
        fread(l.rolling_اوسط, حجم(اعشاری), l.n, fp);
        fread(l.rolling_variance, حجم(اعشاری), l.n, fp);
        اگر(0){
            صحیح i;
            برائے(i = 0; i < l.n; ++i){
                لکھو("%g, ", l.rolling_اوسط[i]);
            }
            لکھو("\n");
            برائے(i = 0; i < l.n; ++i){
                لکھو("%g, ", l.rolling_variance[i]);
            }
            لکھو("\n");
        }
        اگر(0){
            fill_cpu(l.n, 0, l.rolling_اوسط, 1);
            fill_cpu(l.n, 0, l.rolling_variance, 1);
        }
        اگر(0){
            صحیح i;
            برائے(i = 0; i < l.n; ++i){
                لکھو("%g, ", l.rolling_اوسط[i]);
            }
            لکھو("\n");
            برائے(i = 0; i < l.n; ++i){
                لکھو("%g, ", l.rolling_variance[i]);
            }
            لکھو("\n");
        }
    }
    fread(l.weights, حجم(اعشاری), عدد, fp);
    //if(l.c == 3) scal_cpu(num, 1./256, l.weights, 1);
    اگر (l.flipped) {
        transpose_میٹرکس(l.weights, l.c*l.سائز*l.سائز, l.n);
    }
    //if (l.binary) binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.weights);
#اگر_تعریف_شدہ GPU
    اگر(gpu_اشاریہ >= 0){
        ڈالو_convolutional_layer(l);
    }
#اگر_ختم
}


خالی بھرو_weights_upto(جال *net, حرف *filename, صحیح آغاز, صحیح cutoff)
{
#اگر_تعریف_شدہ GPU
    اگر(net->gpu_اشاریہ >= 0){
        cuda_سیٹ_device(net->gpu_اشاریہ);
    }
#اگر_ختم
    fprintf(stderr, "Loading weights from %s...", filename);
    fflush(stdout);
    FILE *fp = fopen(filename, "rb");
    اگر(!fp) file_error(filename);

    صحیح major;
    صحیح minor;
    صحیح revision;
    fread(&major, حجم(صحیح), 1, fp);
    fread(&minor, حجم(صحیح), 1, fp);
    fread(&revision, حجم(صحیح), 1, fp);
    اگر ((major*10 + minor) >= 2 && major < 1000 && minor < 1000){
        fread(net->نظر_آیا, حجم(size_t), 1, fp);
    } ورنہ {
        صحیح iseen = 0;
        fread(&iseen, حجم(صحیح), 1, fp);
        *net->نظر_آیا = iseen;
    }
    صحیح transpose = (major > 1000) || (minor > 1000);

    صحیح i;
    برائے(i = آغاز; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        اگر (l.dontload) جاری;
        اگر(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            بھرو_convolutional_weights(l, fp);
        }
        اگر(l.type == CONNECTED){
            بھرو_connected_weights(l, fp, transpose);
        }
        اگر(l.type == BATCHNORM){
            بھرو_batchnorm_weights(l, fp);
        }
        اگر(l.type == CRNN){
            بھرو_convolutional_weights(*(l.داخلہ_layer), fp);
            بھرو_convolutional_weights(*(l.self_layer), fp);
            بھرو_convolutional_weights(*(l.output_layer), fp);
        }
        اگر(l.type == RNN){
            بھرو_connected_weights(*(l.داخلہ_layer), fp, transpose);
            بھرو_connected_weights(*(l.self_layer), fp, transpose);
            بھرو_connected_weights(*(l.output_layer), fp, transpose);
        }
        اگر (l.type == LSTM) {
            بھرو_connected_weights(*(l.wi), fp, transpose);
            بھرو_connected_weights(*(l.wf), fp, transpose);
            بھرو_connected_weights(*(l.wo), fp, transpose);
            بھرو_connected_weights(*(l.wg), fp, transpose);
            بھرو_connected_weights(*(l.ui), fp, transpose);
            بھرو_connected_weights(*(l.uf), fp, transpose);
            بھرو_connected_weights(*(l.uo), fp, transpose);
            بھرو_connected_weights(*(l.ug), fp, transpose);
        }
        اگر (l.type == GRU) {
            اگر(1){
                بھرو_connected_weights(*(l.wz), fp, transpose);
                بھرو_connected_weights(*(l.wr), fp, transpose);
                بھرو_connected_weights(*(l.wh), fp, transpose);
                بھرو_connected_weights(*(l.uz), fp, transpose);
                بھرو_connected_weights(*(l.ur), fp, transpose);
                بھرو_connected_weights(*(l.uh), fp, transpose);
            }ورنہ{
                بھرو_connected_weights(*(l.reset_layer), fp, transpose);
                بھرو_connected_weights(*(l.update_layer), fp, transpose);
                بھرو_connected_weights(*(l.state_layer), fp, transpose);
            }
        }
        اگر(l.type == LOCAL){
            صحیح locations = l.برآمد_w*l.برآمد_h;
            صحیح سائز = l.سائز*l.سائز*l.c*l.n*locations;
            fread(l.biases, حجم(اعشاری), l.outputs, fp);
            fread(l.weights, حجم(اعشاری), سائز, fp);
#اگر_تعریف_شدہ GPU
            اگر(gpu_اشاریہ >= 0){
                ڈالو_local_layer(l);
            }
#اگر_ختم
        }
    }
    fprintf(stderr, "Done!\n");
    fclose(fp);
}

خالی بھرو_weights(جال *net, حرف *filename)
{
    بھرو_weights_upto(net, filename, 0, net->n);
}


#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "activation_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "logistic_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "l2norm_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "activations.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "avgpool_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "batchnorm_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "connected_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "deconvolutional_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "convolutional_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cost_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "crnn_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "crop_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "detection_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dropout_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gru_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "local_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "maxpool_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "normalization_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "option_list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parser.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "region_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "yolo_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "iseg_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "reorg_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "rnn_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "route_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "upsample_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "shortcut_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "softmax_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lstm_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ{
    ਅੱਖਰ *type;
    list *options;
}section;

list *read_cfg(ਅੱਖਰ *filename);

LAYER_TYPE string_ਮੰਜ਼ਿਲ_layer_type(ਅੱਖਰ * type)
{

    ਜੇ (strcmp(type, "[shortcut]")==0) ਵਾਪਸ SHORTCUT;
    ਜੇ (strcmp(type, "[crop]")==0) ਵਾਪਸ CROP;
    ਜੇ (strcmp(type, "[cost]")==0) ਵਾਪਸ ਲਾਗਤ;
    ਜੇ (strcmp(type, "[detection]")==0) ਵਾਪਸ DETECTION;
    ਜੇ (strcmp(type, "[region]")==0) ਵਾਪਸ REGION;
    ਜੇ (strcmp(type, "[yolo]")==0) ਵਾਪਸ YOLO;
    ਜੇ (strcmp(type, "[iseg]")==0) ਵਾਪਸ ISEG;
    ਜੇ (strcmp(type, "[local]")==0) ਵਾਪਸ LOCAL;
    ਜੇ (strcmp(type, "[conv]")==0
            || strcmp(type, "[convolutional]")==0) ਵਾਪਸ CONVOLUTIONAL;
    ਜੇ (strcmp(type, "[deconv]")==0
            || strcmp(type, "[deconvolutional]")==0) ਵਾਪਸ DECONVOLUTIONAL;
    ਜੇ (strcmp(type, "[activation]")==0) ਵਾਪਸ ਸਰਗਰਮ;
    ਜੇ (strcmp(type, "[logistic]")==0) ਵਾਪਸ LOGXENT;
    ਜੇ (strcmp(type, "[l2norm]")==0) ਵਾਪਸ L2NORM;
    ਜੇ (strcmp(type, "[net]")==0
            || strcmp(type, "[network]")==0) ਵਾਪਸ ਜਾਲ_2;
    ਜੇ (strcmp(type, "[crnn]")==0) ਵਾਪਸ CRNN;
    ਜੇ (strcmp(type, "[gru]")==0) ਵਾਪਸ GRU;
    ਜੇ (strcmp(type, "[lstm]") == 0) ਵਾਪਸ LSTM;
    ਜੇ (strcmp(type, "[rnn]")==0) ਵਾਪਸ RNN;
    ਜੇ (strcmp(type, "[conn]")==0
            || strcmp(type, "[connected]")==0) ਵਾਪਸ CONNECTED;
    ਜੇ (strcmp(type, "[max]")==0
            || strcmp(type, "[maxpool]")==0) ਵਾਪਸ MAXPOOL;
    ਜੇ (strcmp(type, "[reorg]")==0) ਵਾਪਸ REORG;
    ਜੇ (strcmp(type, "[avg]")==0
            || strcmp(type, "[avgpool]")==0) ਵਾਪਸ AVGPOOL;
    ਜੇ (strcmp(type, "[dropout]")==0) ਵਾਪਸ DROPOUT;
    ਜੇ (strcmp(type, "[lrn]")==0
            || strcmp(type, "[normalization]")==0) ਵਾਪਸ NORMALIZATION;
    ਜੇ (strcmp(type, "[batchnorm]")==0) ਵਾਪਸ BATCHNORM;
    ਜੇ (strcmp(type, "[soft]")==0
            || strcmp(type, "[softmax]")==0) ਵਾਪਸ SOFTMAX;
    ਜੇ (strcmp(type, "[route]")==0) ਵਾਪਸ ROUTE;
    ਜੇ (strcmp(type, "[upsample]")==0) ਵਾਪਸ UPSAMPLE;
    ਵਾਪਸ BLANK;
}

ਖਾਲੀ free_section(section *s)
{
    ਮੁਕਤ(s->type);
    ਗੰਢ *n = s->options->front;
    ਜਦੋਂ(n){
        kvp *pair = (kvp *)n->val;
        ਮੁਕਤ(pair->ਕੁੰਜੀ);
        ਮੁਕਤ(pair);
        ਗੰਢ *ਅਗਲਾ = n->ਅਗਲਾ;
        ਮੁਕਤ(n);
        n = ਅਗਲਾ;
    }
    ਮੁਕਤ(s->options);
    ਮੁਕਤ(s);
}

ਖਾਲੀ parse_ਅੰਕੜੇ(ਅੱਖਰ *ਅੰਕੜੇ, ਦਸ਼ਮਲਵ *a, ਪੂਰਨ n)
{
    ਪੂਰਨ i;
    ਜੇ(!ਅੰਕੜੇ) ਵਾਪਸ;
    ਅੱਖਰ *curr = ਅੰਕੜੇ;
    ਅੱਖਰ *ਅਗਲਾ = ਅੰਕੜੇ;
    ਪੂਰਨ ਪੂਰਾ = 0;
    ਲਈ(i = 0; i < n && !ਪੂਰਾ; ++i){
        ਜਦੋਂ(*++ਅਗਲਾ !='\0' && *ਅਗਲਾ != ',');
        ਜੇ(*ਅਗਲਾ == '\0') ਪੂਰਾ = 1;
        *ਅਗਲਾ = '\0';
        ਲੜੀ_ਤੋਂ_ਪੜ੍ਹੋ(curr, "%g", &a[i]);
        curr = ਅਗਲਾ+1;
    }
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ਮਾਪ_params{
    ਪੂਰਨ batch;
    ਪੂਰਨ inputs;
    ਪੂਰਨ h;
    ਪੂਰਨ w;
    ਪੂਰਨ c;
    ਪੂਰਨ ਸੂਚਕਾਂਕ;
    ਪੂਰਨ ਸਮਾਂ_ਕਦਮਾਂ;
    ਜਾਲ *net;
} ਮਾਪ_params;

local_layer parse_local(list *options, ਮਾਪ_params params)
{
    ਪੂਰਨ n = option_ਲੱਭੋ_int(options, "filters",1);
    ਪੂਰਨ ਮਾਪ = option_ਲੱਭੋ_int(options, "size",1);
    ਪੂਰਨ stride = option_ਲੱਭੋ_int(options, "stride",1);
    ਪੂਰਨ pad = option_ਲੱਭੋ_int(options, "pad",0);
    ਅੱਖਰ *activation_s = option_ਲੱਭੋ_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    ਪੂਰਨ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ਜੇ(!(h && w && c)) error("Layer before local layer must output image.");

    local_layer layer = make_local_layer(batch,h,w,c,n,ਮਾਪ,stride,pad,activation);

    ਵਾਪਸ layer;
}

layer parse_deconvolutional(list *options, ਮਾਪ_params params)
{
    ਪੂਰਨ n = option_ਲੱਭੋ_int(options, "filters",1);
    ਪੂਰਨ ਮਾਪ = option_ਲੱਭੋ_int(options, "size",1);
    ਪੂਰਨ stride = option_ਲੱਭੋ_int(options, "stride",1);

    ਅੱਖਰ *activation_s = option_ਲੱਭੋ_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    ਪੂਰਨ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ਜੇ(!(h && w && c)) error("Layer before deconvolutional layer must output image.");
    ਪੂਰਨ batch_normalize = option_ਲੱਭੋ_int_quiet(options, "batch_normalize", 0);
    ਪੂਰਨ pad = option_ਲੱਭੋ_int_quiet(options, "pad",0);
    ਪੂਰਨ padding = option_ਲੱਭੋ_int_quiet(options, "padding",0);
    ਜੇ(pad) padding = ਮਾਪ/2;

    layer l = make_deconvolutional_layer(batch,h,w,c,n,ਮਾਪ,stride,padding, activation, batch_normalize, params.net->adam);

    ਵਾਪਸ l;
}


convolutional_layer parse_convolutional(list *options, ਮਾਪ_params params)
{
    ਪੂਰਨ n = option_ਲੱਭੋ_int(options, "filters",1);
    ਪੂਰਨ ਮਾਪ = option_ਲੱਭੋ_int(options, "size",1);
    ਪੂਰਨ stride = option_ਲੱਭੋ_int(options, "stride",1);
    ਪੂਰਨ pad = option_ਲੱਭੋ_int_quiet(options, "pad",0);
    ਪੂਰਨ padding = option_ਲੱਭੋ_int_quiet(options, "padding",0);
    ਪੂਰਨ groups = option_ਲੱਭੋ_int_quiet(options, "groups", 1);
    ਜੇ(pad) padding = ਮਾਪ/2;

    ਅੱਖਰ *activation_s = option_ਲੱਭੋ_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    ਪੂਰਨ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ਜੇ(!(h && w && c)) error("Layer before convolutional layer must output image.");
    ਪੂਰਨ batch_normalize = option_ਲੱਭੋ_int_quiet(options, "batch_normalize", 0);
    ਪੂਰਨ ਦੁਆਧਾਰੀ = option_ਲੱਭੋ_int_quiet(options, "binary", 0);
    ਪੂਰਨ xnor = option_ਲੱਭੋ_int_quiet(options, "xnor", 0);

    convolutional_layer layer = make_convolutional_layer(batch,h,w,c,n,groups,ਮਾਪ,stride,padding,activation, batch_normalize, ਦੁਆਧਾਰੀ, xnor, params.net->adam);
    layer.flipped = option_ਲੱਭੋ_int_quiet(options, "flipped", 0);
    layer.dot = option_ਲੱਭੋ_float_quiet(options, "dot", 0);

    ਵਾਪਸ layer;
}

layer parse_crnn(list *options, ਮਾਪ_params params)
{
    ਪੂਰਨ output_filters = option_ਲੱਭੋ_int(options, "output_filters",1);
    ਪੂਰਨ hidden_filters = option_ਲੱਭੋ_int(options, "hidden_filters",1);
    ਅੱਖਰ *activation_s = option_ਲੱਭੋ_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    ਪੂਰਨ batch_normalize = option_ਲੱਭੋ_int_quiet(options, "batch_normalize", 0);

    layer l = make_crnn_layer(params.batch, params.w, params.h, params.c, hidden_filters, output_filters, params.ਸਮਾਂ_ਕਦਮਾਂ, activation, batch_normalize);

    l.shortcut = option_ਲੱਭੋ_int_quiet(options, "shortcut", 0);

    ਵਾਪਸ l;
}

layer parse_rnn(list *options, ਮਾਪ_params params)
{
    ਪੂਰਨ output = option_ਲੱਭੋ_int(options, "output",1);
    ਅੱਖਰ *activation_s = option_ਲੱਭੋ_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    ਪੂਰਨ batch_normalize = option_ਲੱਭੋ_int_quiet(options, "batch_normalize", 0);

    layer l = make_rnn_layer(params.batch, params.inputs, output, params.ਸਮਾਂ_ਕਦਮਾਂ, activation, batch_normalize, params.net->adam);

    l.shortcut = option_ਲੱਭੋ_int_quiet(options, "shortcut", 0);

    ਵਾਪਸ l;
}

layer parse_gru(list *options, ਮਾਪ_params params)
{
    ਪੂਰਨ output = option_ਲੱਭੋ_int(options, "output",1);
    ਪੂਰਨ batch_normalize = option_ਲੱਭੋ_int_quiet(options, "batch_normalize", 0);

    layer l = make_gru_layer(params.batch, params.inputs, output, params.ਸਮਾਂ_ਕਦਮਾਂ, batch_normalize, params.net->adam);
    l.tanh = option_ਲੱਭੋ_int_quiet(options, "tanh", 0);

    ਵਾਪਸ l;
}

layer parse_lstm(list *options, ਮਾਪ_params params)
{
    ਪੂਰਨ output = option_ਲੱਭੋ_int(options, "output", 1);
    ਪੂਰਨ batch_normalize = option_ਲੱਭੋ_int_quiet(options, "batch_normalize", 0);

    layer l = make_lstm_layer(params.batch, params.inputs, output, params.ਸਮਾਂ_ਕਦਮਾਂ, batch_normalize, params.net->adam);

    ਵਾਪਸ l;
}

layer parse_connected(list *options, ਮਾਪ_params params)
{
    ਪੂਰਨ output = option_ਲੱਭੋ_int(options, "output",1);
    ਅੱਖਰ *activation_s = option_ਲੱਭੋ_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    ਪੂਰਨ batch_normalize = option_ਲੱਭੋ_int_quiet(options, "batch_normalize", 0);

    layer l = make_connected_layer(params.batch, params.inputs, output, activation, batch_normalize, params.net->adam);
    ਵਾਪਸ l;
}

layer parse_softmax(list *options, ਮਾਪ_params params)
{
    ਪੂਰਨ groups = option_ਲੱਭੋ_int_quiet(options, "groups",1);
    layer l = make_softmax_layer(params.batch, params.inputs, groups);
    l.temperature = option_ਲੱਭੋ_float_quiet(options, "temperature", 1);
    ਅੱਖਰ *ਰੁੱਖ_file = option_ਲੱਭੋ_str(options, "tree", 0);
    ਜੇ (ਰੁੱਖ_file) l.softmax_ਰੁੱਖ = read_ਰੁੱਖ(ਰੁੱਖ_file);
    l.w = params.w;
    l.h = params.h;
    l.c = params.c;
    l.spatial = option_ਲੱਭੋ_float_quiet(options, "spatial", 0);
    l.noloss =  option_ਲੱਭੋ_int_quiet(options, "noloss", 0);
    ਵਾਪਸ l;
}

ਪੂਰਨ *parse_yolo_ਨਕਾਬ(ਅੱਖਰ *a, ਪੂਰਨ *ਅੰਕ)
{
    ਪੂਰਨ *ਨਕਾਬ = 0;
    ਜੇ(a){
        ਪੂਰਨ len = ਲੜੀ_ਲੰਬਾਈ(a);
        ਪੂਰਨ n = 1;
        ਪੂਰਨ i;
        ਲਈ(i = 0; i < len; ++i){
            ਜੇ (a[i] == ',') ++n;
        }
        ਨਕਾਬ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਪੂਰਨ));
        ਲਈ(i = 0; i < n; ++i){
            ਪੂਰਨ val = atoi(a);
            ਨਕਾਬ[i] = val;
            a = strchr(a, ',')+1;
        }
        *ਅੰਕ = n;
    }
    ਵਾਪਸ ਨਕਾਬ;
}

layer parse_yolo(list *options, ਮਾਪ_params params)
{
    ਪੂਰਨ classes = option_ਲੱਭੋ_int(options, "classes", 20);
    ਪੂਰਨ ਕੁੱਲ = option_ਲੱਭੋ_int(options, "num", 1);
    ਪੂਰਨ ਅੰਕ = ਕੁੱਲ;

    ਅੱਖਰ *a = option_ਲੱਭੋ_str(options, "mask", 0);
    ਪੂਰਨ *ਨਕਾਬ = parse_yolo_ਨਕਾਬ(a, &ਅੰਕ);
    layer l = make_yolo_layer(params.batch, params.w, params.h, ਅੰਕ, ਕੁੱਲ, ਨਕਾਬ, classes);
    ਪ੍ਰਤਿਜ੍ਞਾ(l.outputs == params.inputs);

    l.max_boxes = option_ਲੱਭੋ_int_quiet(options, "max",90);
    l.jitter = option_ਲੱਭੋ_float(options, "jitter", .2);

    l.ignore_thresh = option_ਲੱਭੋ_float(options, "ignore_thresh", .5);
    l.truth_thresh = option_ਲੱਭੋ_float(options, "truth_thresh", 1);
    l.random = option_ਲੱਭੋ_int_quiet(options, "random", 0);

    ਅੱਖਰ *map_file = option_ਲੱਭੋ_str(options, "map", 0);
    ਜੇ (map_file) l.map = read_map(map_file);

    a = option_ਲੱਭੋ_str(options, "anchors", 0);
    ਜੇ(a){
        ਪੂਰਨ len = ਲੜੀ_ਲੰਬਾਈ(a);
        ਪੂਰਨ n = 1;
        ਪੂਰਨ i;
        ਲਈ(i = 0; i < len; ++i){
            ਜੇ (a[i] == ',') ++n;
        }
        ਲਈ(i = 0; i < n; ++i){
            ਦਸ਼ਮਲਵ bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    ਵਾਪਸ l;
}

layer parse_iseg(list *options, ਮਾਪ_params params)
{
    ਪੂਰਨ classes = option_ਲੱਭੋ_int(options, "classes", 20);
    ਪੂਰਨ ids = option_ਲੱਭੋ_int(options, "ids", 32);
    layer l = make_iseg_layer(params.batch, params.w, params.h, classes, ids);
    ਪ੍ਰਤਿਜ੍ਞਾ(l.outputs == params.inputs);
    ਵਾਪਸ l;
}

layer parse_region(list *options, ਮਾਪ_params params)
{
    ਪੂਰਨ coords = option_ਲੱਭੋ_int(options, "coords", 4);
    ਪੂਰਨ classes = option_ਲੱਭੋ_int(options, "classes", 20);
    ਪੂਰਨ ਅੰਕ = option_ਲੱਭੋ_int(options, "num", 1);

    layer l = make_region_layer(params.batch, params.w, params.h, ਅੰਕ, classes, coords);
    ਪ੍ਰਤਿਜ੍ਞਾ(l.outputs == params.inputs);

    l.ਲਘੂਗਣਕ = option_ਲੱਭੋ_int_quiet(options, "log", 0);
    l.ਵਰਗਮੂਲ = option_ਲੱਭੋ_int_quiet(options, "sqrt", 0);

    l.softmax = option_ਲੱਭੋ_int(options, "softmax", 0);
    l.background = option_ਲੱਭੋ_int_quiet(options, "background", 0);
    l.max_boxes = option_ਲੱਭੋ_int_quiet(options, "max",30);
    l.jitter = option_ਲੱਭੋ_float(options, "jitter", .2);
    l.rescore = option_ਲੱਭੋ_int_quiet(options, "rescore",0);

    l.thresh = option_ਲੱਭੋ_float(options, "thresh", .5);
    l.classfix = option_ਲੱਭੋ_int_quiet(options, "classfix", 0);
    l.absolute = option_ਲੱਭੋ_int_quiet(options, "absolute", 0);
    l.random = option_ਲੱਭੋ_int_quiet(options, "random", 0);

    l.coord_scale = option_ਲੱਭੋ_float(options, "coord_scale", 1);
    l.object_scale = option_ਲੱਭੋ_float(options, "object_scale", 1);
    l.noobject_scale = option_ਲੱਭੋ_float(options, "noobject_scale", 1);
    l.ਨਕਾਬ_scale = option_ਲੱਭੋ_float(options, "mask_scale", 1);
    l.class_scale = option_ਲੱਭੋ_float(options, "class_scale", 1);
    l.bias_ਮੇਲ = option_ਲੱਭੋ_int_quiet(options, "bias_match",0);

    ਅੱਖਰ *ਰੁੱਖ_file = option_ਲੱਭੋ_str(options, "tree", 0);
    ਜੇ (ਰੁੱਖ_file) l.softmax_ਰੁੱਖ = read_ਰੁੱਖ(ਰੁੱਖ_file);
    ਅੱਖਰ *map_file = option_ਲੱਭੋ_str(options, "map", 0);
    ਜੇ (map_file) l.map = read_map(map_file);

    ਅੱਖਰ *a = option_ਲੱਭੋ_str(options, "anchors", 0);
    ਜੇ(a){
        ਪੂਰਨ len = ਲੜੀ_ਲੰਬਾਈ(a);
        ਪੂਰਨ n = 1;
        ਪੂਰਨ i;
        ਲਈ(i = 0; i < len; ++i){
            ਜੇ (a[i] == ',') ++n;
        }
        ਲਈ(i = 0; i < n; ++i){
            ਦਸ਼ਮਲਵ bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    ਵਾਪਸ l;
}

detection_layer parse_detection(list *options, ਮਾਪ_params params)
{
    ਪੂਰਨ coords = option_ਲੱਭੋ_int(options, "coords", 1);
    ਪੂਰਨ classes = option_ਲੱਭੋ_int(options, "classes", 1);
    ਪੂਰਨ rescore = option_ਲੱਭੋ_int(options, "rescore", 0);
    ਪੂਰਨ ਅੰਕ = option_ਲੱਭੋ_int(options, "num", 1);
    ਪੂਰਨ side = option_ਲੱਭੋ_int(options, "side", 7);
    detection_layer layer = make_detection_layer(params.batch, params.inputs, ਅੰਕ, side, classes, coords, rescore);

    layer.softmax = option_ਲੱਭੋ_int(options, "softmax", 0);
    layer.ਵਰਗਮੂਲ = option_ਲੱਭੋ_int(options, "sqrt", 0);

    layer.max_boxes = option_ਲੱਭੋ_int_quiet(options, "max",90);
    layer.coord_scale = option_ਲੱਭੋ_float(options, "coord_scale", 1);
    layer.forced = option_ਲੱਭੋ_int(options, "forced", 0);
    layer.object_scale = option_ਲੱਭੋ_float(options, "object_scale", 1);
    layer.noobject_scale = option_ਲੱਭੋ_float(options, "noobject_scale", 1);
    layer.class_scale = option_ਲੱਭੋ_float(options, "class_scale", 1);
    layer.jitter = option_ਲੱਭੋ_float(options, "jitter", .2);
    layer.random = option_ਲੱਭੋ_int_quiet(options, "random", 0);
    layer.reorg = option_ਲੱਭੋ_int_quiet(options, "reorg", 0);
    ਵਾਪਸ layer;
}

ਲਾਗਤ_layer parse_ਲਾਗਤ(list *options, ਮਾਪ_params params)
{
    ਅੱਖਰ *type_s = option_ਲੱਭੋ_str(options, "type", "sse");
    ਲਾਗਤ_TYPE type = get_ਲਾਗਤ_type(type_s);
    ਦਸ਼ਮਲਵ scale = option_ਲੱਭੋ_float_quiet(options, "scale",1);
    ਲਾਗਤ_layer layer = make_ਲਾਗਤ_layer(params.batch, params.inputs, type, scale);
    layer.ਅਨੁਪਾਤ =  option_ਲੱਭੋ_float_quiet(options, "ratio",0);
    layer.noobject_scale =  option_ਲੱਭੋ_float_quiet(options, "noobj", 1);
    layer.thresh =  option_ਲੱਭੋ_float_quiet(options, "thresh",0);
    ਵਾਪਸ layer;
}

crop_layer parse_crop(list *options, ਮਾਪ_params params)
{
    ਪੂਰਨ crop_ਉਚਾਈ = option_ਲੱਭੋ_int(options, "crop_height",1);
    ਪੂਰਨ crop_ਚੌੜਾਈ = option_ਲੱਭੋ_int(options, "crop_width",1);
    ਪੂਰਨ flip = option_ਲੱਭੋ_int(options, "flip",0);
    ਦਸ਼ਮਲਵ angle = option_ਲੱਭੋ_float(options, "angle",0);
    ਦਸ਼ਮਲਵ saturation = option_ਲੱਭੋ_float(options, "saturation",1);
    ਦਸ਼ਮਲਵ exposure = option_ਲੱਭੋ_float(options, "exposure",1);

    ਪੂਰਨ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ਜੇ(!(h && w && c)) error("Layer before crop layer must output image.");

    ਪੂਰਨ noadjust = option_ਲੱਭੋ_int_quiet(options, "noadjust",0);

    crop_layer l = make_crop_layer(batch,h,w,c,crop_ਉਚਾਈ,crop_ਚੌੜਾਈ,flip, angle, saturation, exposure);
    l.ਖਿਸਕਾਓ = option_ਲੱਭੋ_float(options, "shift", 0);
    l.noadjust = noadjust;
    ਵਾਪਸ l;
}

layer parse_reorg(list *options, ਮਾਪ_params params)
{
    ਪੂਰਨ stride = option_ਲੱਭੋ_int(options, "stride",1);
    ਪੂਰਨ ਉਲਟ = option_ਲੱਭੋ_int_quiet(options, "reverse",0);
    ਪੂਰਨ flatten = option_ਲੱਭੋ_int_quiet(options, "flatten",0);
    ਪੂਰਨ extra = option_ਲੱਭੋ_int_quiet(options, "extra",0);

    ਪੂਰਨ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ਜੇ(!(h && w && c)) error("Layer before reorg layer must output image.");

    layer layer = make_reorg_layer(batch,w,h,c,stride,ਉਲਟ, flatten, extra);
    ਵਾਪਸ layer;
}

maxpool_layer parse_maxpool(list *options, ਮਾਪ_params params)
{
    ਪੂਰਨ stride = option_ਲੱਭੋ_int(options, "stride",1);
    ਪੂਰਨ ਮਾਪ = option_ਲੱਭੋ_int(options, "size",stride);
    ਪੂਰਨ padding = option_ਲੱਭੋ_int_quiet(options, "padding", ਮਾਪ-1);

    ਪੂਰਨ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ਜੇ(!(h && w && c)) error("Layer before maxpool layer must output image.");

    maxpool_layer layer = make_maxpool_layer(batch,h,w,c,ਮਾਪ,stride,padding);
    ਵਾਪਸ layer;
}

avgpool_layer parse_avgpool(list *options, ਮਾਪ_params params)
{
    ਪੂਰਨ batch,w,h,c;
    w = params.w;
    h = params.h;
    c = params.c;
    batch=params.batch;
    ਜੇ(!(h && w && c)) error("Layer before avgpool layer must output image.");

    avgpool_layer layer = make_avgpool_layer(batch,w,h,c);
    ਵਾਪਸ layer;
}

dropout_layer parse_dropout(list *options, ਮਾਪ_params params)
{
    ਦਸ਼ਮਲਵ probability = option_ਲੱਭੋ_float(options, "probability", .5);
    dropout_layer layer = make_dropout_layer(params.batch, params.inputs, probability);
    layer.ਬਾਹਰਲਾ_w = params.w;
    layer.ਬਾਹਰਲਾ_h = params.h;
    layer.ਬਾਹਰਲਾ_c = params.c;
    ਵਾਪਸ layer;
}

layer parse_normalization(list *options, ਮਾਪ_params params)
{
    ਦਸ਼ਮਲਵ alpha = option_ਲੱਭੋ_float(options, "alpha", .0001);
    ਦਸ਼ਮਲਵ beta =  option_ਲੱਭੋ_float(options, "beta" , .75);
    ਦਸ਼ਮਲਵ kappa = option_ਲੱਭੋ_float(options, "kappa", 1);
    ਪੂਰਨ ਮਾਪ = option_ਲੱਭੋ_int(options, "size", 5);
    layer l = make_normalization_layer(params.batch, params.w, params.h, params.c, ਮਾਪ, alpha, beta, kappa);
    ਵਾਪਸ l;
}

layer parse_batchnorm(list *options, ਮਾਪ_params params)
{
    layer l = make_batchnorm_layer(params.batch, params.w, params.h, params.c);
    ਵਾਪਸ l;
}

layer parse_shortcut(list *options, ਮਾਪ_params params, ਜਾਲ *net)
{
    ਅੱਖਰ *l = option_ਲੱਭੋ(options, "from");
    ਪੂਰਨ ਸੂਚਕਾਂਕ = atoi(l);
    ਜੇ(ਸੂਚਕਾਂਕ < 0) ਸੂਚਕਾਂਕ = params.ਸੂਚਕਾਂਕ + ਸੂਚਕਾਂਕ;

    ਪੂਰਨ batch = params.batch;
    layer from = net->layers[ਸੂਚਕਾਂਕ];

    layer s = make_shortcut_layer(batch, ਸੂਚਕਾਂਕ, params.w, params.h, params.c, from.ਬਾਹਰਲਾ_w, from.ਬਾਹਰਲਾ_h, from.ਬਾਹਰਲਾ_c);

    ਅੱਖਰ *activation_s = option_ਲੱਭੋ_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);
    s.activation = activation;
    s.alpha = option_ਲੱਭੋ_float_quiet(options, "alpha", 1);
    s.beta = option_ਲੱਭੋ_float_quiet(options, "beta", 1);
    ਵਾਪਸ s;
}


layer parse_l2norm(list *options, ਮਾਪ_params params)
{
    layer l = make_l2norm_layer(params.batch, params.inputs);
    l.h = l.ਬਾਹਰਲਾ_h = params.h;
    l.w = l.ਬਾਹਰਲਾ_w = params.w;
    l.c = l.ਬਾਹਰਲਾ_c = params.c;
    ਵਾਪਸ l;
}


layer parse_logistic(list *options, ਮਾਪ_params params)
{
    layer l = make_logistic_layer(params.batch, params.inputs);
    l.h = l.ਬਾਹਰਲਾ_h = params.h;
    l.w = l.ਬਾਹਰਲਾ_w = params.w;
    l.c = l.ਬਾਹਰਲਾ_c = params.c;
    ਵਾਪਸ l;
}

layer parse_activation(list *options, ਮਾਪ_params params)
{
    ਅੱਖਰ *activation_s = option_ਲੱਭੋ_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);

    layer l = make_activation_layer(params.batch, params.inputs, activation);

    l.h = l.ਬਾਹਰਲਾ_h = params.h;
    l.w = l.ਬਾਹਰਲਾ_w = params.w;
    l.c = l.ਬਾਹਰਲਾ_c = params.c;

    ਵਾਪਸ l;
}

layer parse_upsample(list *options, ਮਾਪ_params params, ਜਾਲ *net)
{

    ਪੂਰਨ stride = option_ਲੱਭੋ_int(options, "stride",2);
    layer l = make_upsample_layer(params.batch, params.w, params.h, params.c, stride);
    l.scale = option_ਲੱਭੋ_float_quiet(options, "scale", 1);
    ਵਾਪਸ l;
}

route_layer parse_route(list *options, ਮਾਪ_params params, ਜਾਲ *net)
{
    ਅੱਖਰ *l = option_ਲੱਭੋ(options, "layers");
    ਪੂਰਨ len = ਲੜੀ_ਲੰਬਾਈ(l);
    ਜੇ(!l) error("Route Layer must specify input layers");
    ਪੂਰਨ n = 1;
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < len; ++i){
        ਜੇ (l[i] == ',') ++n;
    }

    ਪੂਰਨ *layers = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਪੂਰਨ));
    ਪੂਰਨ *sizes = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਪੂਰਨ));
    ਲਈ(i = 0; i < n; ++i){
        ਪੂਰਨ ਸੂਚਕਾਂਕ = atoi(l);
        l = strchr(l, ',')+1;
        ਜੇ(ਸੂਚਕਾਂਕ < 0) ਸੂਚਕਾਂਕ = params.ਸੂਚਕਾਂਕ + ਸੂਚਕਾਂਕ;
        layers[i] = ਸੂਚਕਾਂਕ;
        sizes[i] = net->layers[ਸੂਚਕਾਂਕ].outputs;
    }
    ਪੂਰਨ batch = params.batch;

    route_layer layer = make_route_layer(batch, n, layers, sizes);

    convolutional_layer ਪਹਿਲਾ = net->layers[layers[0]];
    layer.ਬਾਹਰਲਾ_w = ਪਹਿਲਾ.ਬਾਹਰਲਾ_w;
    layer.ਬਾਹਰਲਾ_h = ਪਹਿਲਾ.ਬਾਹਰਲਾ_h;
    layer.ਬਾਹਰਲਾ_c = ਪਹਿਲਾ.ਬਾਹਰਲਾ_c;
    ਲਈ(i = 1; i < n; ++i){
        ਪੂਰਨ ਸੂਚਕਾਂਕ = layers[i];
        convolutional_layer ਅਗਲਾ = net->layers[ਸੂਚਕਾਂਕ];
        ਜੇ(ਅਗਲਾ.ਬਾਹਰਲਾ_w == ਪਹਿਲਾ.ਬਾਹਰਲਾ_w && ਅਗਲਾ.ਬਾਹਰਲਾ_h == ਪਹਿਲਾ.ਬਾਹਰਲਾ_h){
            layer.ਬਾਹਰਲਾ_c += ਅਗਲਾ.ਬਾਹਰਲਾ_c;
        }ਨਹੀਂਤਾਂ{
            layer.ਬਾਹਰਲਾ_h = layer.ਬਾਹਰਲਾ_w = layer.ਬਾਹਰਲਾ_c = 0;
        }
    }

    ਵਾਪਸ layer;
}

learning_ਦਰ_policy get_policy(ਅੱਖਰ *s)
{
    ਜੇ (strcmp(s, "random")==0) ਵਾਪਸ RANDOM;
    ਜੇ (strcmp(s, "poly")==0) ਵਾਪਸ POLY;
    ਜੇ (strcmp(s, "constant")==0) ਵਾਪਸ CONSTANT;
    ਜੇ (strcmp(s, "step")==0) ਵਾਪਸ ਕਦਮ_2;
    ਜੇ (strcmp(s, "exp")==0) ਵਾਪਸ EXP;
    ਜੇ (strcmp(s, "sigmoid")==0) ਵਾਪਸ SIG;
    ਜੇ (strcmp(s, "steps")==0) ਵਾਪਸ ਕਦਮਾਂ_2;
    fprintf(stderr, "Couldn't find policy %s, going with constant\n", s);
    ਵਾਪਸ CONSTANT;
}

ਖਾਲੀ parse_net_options(list *options, ਜਾਲ *net)
{
    net->batch = option_ਲੱਭੋ_int(options, "batch",1);
    net->learning_ਦਰ = option_ਲੱਭੋ_float(options, "learning_rate", .001);
    net->momentum = option_ਲੱਭੋ_float(options, "momentum", .9);
    net->decay = option_ਲੱਭੋ_float(options, "decay", .0001);
    ਪੂਰਨ subdivs = option_ਲੱਭੋ_int(options, "subdivisions",1);
    net->ਸਮਾਂ_ਕਦਮਾਂ = option_ਲੱਭੋ_int_quiet(options, "time_steps",1);
    net->notruth = option_ਲੱਭੋ_int_quiet(options, "notruth",0);
    net->batch /= subdivs;
    net->batch *= net->ਸਮਾਂ_ਕਦਮਾਂ;
    net->subdivisions = subdivs;
    net->random = option_ਲੱਭੋ_int_quiet(options, "random", 0);

    net->adam = option_ਲੱਭੋ_int_quiet(options, "adam", 0);
    ਜੇ(net->adam){
        net->B1 = option_ਲੱਭੋ_float(options, "B1", .9);
        net->B2 = option_ਲੱਭੋ_float(options, "B2", .999);
        net->eps = option_ਲੱਭੋ_float(options, "eps", .0000001);
    }

    net->h = option_ਲੱਭੋ_int_quiet(options, "height",0);
    net->w = option_ਲੱਭੋ_int_quiet(options, "width",0);
    net->c = option_ਲੱਭੋ_int_quiet(options, "channels",0);
    net->inputs = option_ਲੱਭੋ_int_quiet(options, "inputs", net->h * net->w * net->c);
    net->max_crop = option_ਲੱਭੋ_int_quiet(options, "max_crop",net->w*2);
    net->min_crop = option_ਲੱਭੋ_int_quiet(options, "min_crop",net->w);
    net->max_ਅਨੁਪਾਤ = option_ਲੱਭੋ_float_quiet(options, "max_ratio", (ਦਸ਼ਮਲਵ) net->max_crop / net->w);
    net->min_ਅਨੁਪਾਤ = option_ਲੱਭੋ_float_quiet(options, "min_ratio", (ਦਸ਼ਮਲਵ) net->min_crop / net->w);
    net->center = option_ਲੱਭੋ_int_quiet(options, "center",0);
    net->clip = option_ਲੱਭੋ_float_quiet(options, "clip", 0);

    net->angle = option_ਲੱਭੋ_float_quiet(options, "angle", 0);
    net->aspect = option_ਲੱਭੋ_float_quiet(options, "aspect", 1);
    net->saturation = option_ਲੱਭੋ_float_quiet(options, "saturation", 1);
    net->exposure = option_ਲੱਭੋ_float_quiet(options, "exposure", 1);
    net->hue = option_ਲੱਭੋ_float_quiet(options, "hue", 0);

    ਜੇ(!net->inputs && !(net->h && net->w && net->c)) error("No input parameters supplied");

    ਅੱਖਰ *policy_s = option_ਲੱਭੋ_str(options, "policy", "constant");
    net->policy = get_policy(policy_s);
    net->burn_ਵਿੱਚ = option_ਲੱਭੋ_int_quiet(options, "burn_in", 0);
    net->power = option_ਲੱਭੋ_float_quiet(options, "power", 4);
    ਜੇ(net->policy == ਕਦਮ_2){
        net->ਕਦਮ = option_ਲੱਭੋ_int(options, "step", 1);
        net->scale = option_ਲੱਭੋ_float(options, "scale", 1);
    } ਨਹੀਂਤਾਂ ਜੇ (net->policy == ਕਦਮਾਂ_2){
        ਅੱਖਰ *l = option_ਲੱਭੋ(options, "steps");
        ਅੱਖਰ *p = option_ਲੱਭੋ(options, "scales");
        ਜੇ(!l || !p) error("STEPS policy must have steps and scales in cfg file");

        ਪੂਰਨ len = ਲੜੀ_ਲੰਬਾਈ(l);
        ਪੂਰਨ n = 1;
        ਪੂਰਨ i;
        ਲਈ(i = 0; i < len; ++i){
            ਜੇ (l[i] == ',') ++n;
        }
        ਪੂਰਨ *ਕਦਮਾਂ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਪੂਰਨ));
        ਦਸ਼ਮਲਵ *scales = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        ਲਈ(i = 0; i < n; ++i){
            ਪੂਰਨ ਕਦਮ    = atoi(l);
            ਦਸ਼ਮਲਵ scale = atof(p);
            l = strchr(l, ',')+1;
            p = strchr(p, ',')+1;
            ਕਦਮਾਂ[i] = ਕਦਮ;
            scales[i] = scale;
        }
        net->scales = scales;
        net->ਕਦਮਾਂ = ਕਦਮਾਂ;
        net->ਅੰਕ_ਕਦਮਾਂ = n;
    } ਨਹੀਂਤਾਂ ਜੇ (net->policy == EXP){
        net->gamma = option_ਲੱਭੋ_float(options, "gamma", 1);
    } ਨਹੀਂਤਾਂ ਜੇ (net->policy == SIG){
        net->gamma = option_ਲੱਭੋ_float(options, "gamma", 1);
        net->ਕਦਮ = option_ਲੱਭੋ_int(options, "step", 1);
    } ਨਹੀਂਤਾਂ ਜੇ (net->policy == POLY || net->policy == RANDOM){
    }
    net->max_batches = option_ਲੱਭੋ_int(options, "max_batches", 0);
}

ਪੂਰਨ is_ਜਾਲ(section *s)
{
    ਵਾਪਸ (strcmp(s->type, "[net]")==0
            || strcmp(s->type, "[network]")==0);
}

ਜਾਲ *parse_ਜਾਲ_cfg(ਅੱਖਰ *filename)
{
    list *sections = read_cfg(filename);
    ਗੰਢ *n = sections->front;
    ਜੇ(!n) error("Config file has no sections");
    ਜਾਲ *net = make_ਜਾਲ(sections->ਮਾਪ - 1);
    net->gpu_ਸੂਚਕਾਂਕ = gpu_ਸੂਚਕਾਂਕ;
    ਮਾਪ_params params;

    section *s = (section *)n->val;
    list *options = s->options;
    ਜੇ(!is_ਜਾਲ(s)) error("First section must be [net] or [network]");
    parse_net_options(options, net);

    params.h = net->h;
    params.w = net->w;
    params.c = net->c;
    params.inputs = net->inputs;
    params.batch = net->batch;
    params.ਸਮਾਂ_ਕਦਮਾਂ = net->ਸਮਾਂ_ਕਦਮਾਂ;
    params.net = net;

    size_t workspace_ਮਾਪ = 0;
    n = n->ਅਗਲਾ;
    ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    free_section(s);
    fprintf(stderr, "layer     filters    size              input                output\n");
    ਜਦੋਂ(n){
        params.ਸੂਚਕਾਂਕ = ਗਿਣਤੀ_2;
        fprintf(stderr, "%5d ", ਗਿਣਤੀ_2);
        s = (section *)n->val;
        options = s->options;
        layer l = {0};
        LAYER_TYPE lt = string_ਮੰਜ਼ਿਲ_layer_type(s->type);
        ਜੇ(lt == CONVOLUTIONAL){
            l = parse_convolutional(options, params);
        }ਨਹੀਂਤਾਂ ਜੇ(lt == DECONVOLUTIONAL){
            l = parse_deconvolutional(options, params);
        }ਨਹੀਂਤਾਂ ਜੇ(lt == LOCAL){
            l = parse_local(options, params);
        }ਨਹੀਂਤਾਂ ਜੇ(lt == ਸਰਗਰਮ){
            l = parse_activation(options, params);
        }ਨਹੀਂਤਾਂ ਜੇ(lt == LOGXENT){
            l = parse_logistic(options, params);
        }ਨਹੀਂਤਾਂ ਜੇ(lt == L2NORM){
            l = parse_l2norm(options, params);
        }ਨਹੀਂਤਾਂ ਜੇ(lt == RNN){
            l = parse_rnn(options, params);
        }ਨਹੀਂਤਾਂ ਜੇ(lt == GRU){
            l = parse_gru(options, params);
        }ਨਹੀਂਤਾਂ ਜੇ (lt == LSTM) {
            l = parse_lstm(options, params);
        }ਨਹੀਂਤਾਂ ਜੇ(lt == CRNN){
            l = parse_crnn(options, params);
        }ਨਹੀਂਤਾਂ ਜੇ(lt == CONNECTED){
            l = parse_connected(options, params);
        }ਨਹੀਂਤਾਂ ਜੇ(lt == CROP){
            l = parse_crop(options, params);
        }ਨਹੀਂਤਾਂ ਜੇ(lt == ਲਾਗਤ){
            l = parse_ਲਾਗਤ(options, params);
        }ਨਹੀਂਤਾਂ ਜੇ(lt == REGION){
            l = parse_region(options, params);
        }ਨਹੀਂਤਾਂ ਜੇ(lt == YOLO){
            l = parse_yolo(options, params);
        }ਨਹੀਂਤਾਂ ਜੇ(lt == ISEG){
            l = parse_iseg(options, params);
        }ਨਹੀਂਤਾਂ ਜੇ(lt == DETECTION){
            l = parse_detection(options, params);
        }ਨਹੀਂਤਾਂ ਜੇ(lt == SOFTMAX){
            l = parse_softmax(options, params);
            net->hierarchy = l.softmax_ਰੁੱਖ;
        }ਨਹੀਂਤਾਂ ਜੇ(lt == NORMALIZATION){
            l = parse_normalization(options, params);
        }ਨਹੀਂਤਾਂ ਜੇ(lt == BATCHNORM){
            l = parse_batchnorm(options, params);
        }ਨਹੀਂਤਾਂ ਜੇ(lt == MAXPOOL){
            l = parse_maxpool(options, params);
        }ਨਹੀਂਤਾਂ ਜੇ(lt == REORG){
            l = parse_reorg(options, params);
        }ਨਹੀਂਤਾਂ ਜੇ(lt == AVGPOOL){
            l = parse_avgpool(options, params);
        }ਨਹੀਂਤਾਂ ਜੇ(lt == ROUTE){
            l = parse_route(options, params, net);
        }ਨਹੀਂਤਾਂ ਜੇ(lt == UPSAMPLE){
            l = parse_upsample(options, params, net);
        }ਨਹੀਂਤਾਂ ਜੇ(lt == SHORTCUT){
            l = parse_shortcut(options, params, net);
        }ਨਹੀਂਤਾਂ ਜੇ(lt == DROPOUT){
            l = parse_dropout(options, params);
            l.output = net->layers[ਗਿਣਤੀ_2-1].output;
            l.delta = net->layers[ਗਿਣਤੀ_2-1].delta;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
            l.output_gpu = net->layers[ਗਿਣਤੀ_2-1].output_gpu;
            l.delta_gpu = net->layers[ਗਿਣਤੀ_2-1].delta_gpu;
#ਜੇ_ਅੰਤ
        }ਨਹੀਂਤਾਂ{
            fprintf(stderr, "Type not recognized: %s\n", s->type);
        }
        l.clip = net->clip;
        l.truth = option_ਲੱਭੋ_int_quiet(options, "truth", 0);
        l.onlyforward = option_ਲੱਭੋ_int_quiet(options, "onlyforward", 0);
        l.stopbackward = option_ਲੱਭੋ_int_quiet(options, "stopbackward", 0);
        l.dontsave = option_ਲੱਭੋ_int_quiet(options, "dontsave", 0);
        l.dontload = option_ਲੱਭੋ_int_quiet(options, "dontload", 0);
        l.numload = option_ਲੱਭੋ_int_quiet(options, "numload", 0);
        l.dontloadscales = option_ਲੱਭੋ_int_quiet(options, "dontloadscales", 0);
        l.learning_ਦਰ_scale = option_ਲੱਭੋ_float_quiet(options, "learning_rate", 1);
        l.smooth = option_ਲੱਭੋ_float_quiet(options, "smooth", 0);
        option_ਅਣਵਰਤਿਆ(options);
        net->layers[ਗਿਣਤੀ_2] = l;
        ਜੇ (l.workspace_ਮਾਪ > workspace_ਮਾਪ) workspace_ਮਾਪ = l.workspace_ਮਾਪ;
        free_section(s);
        n = n->ਅਗਲਾ;
        ++ਗਿਣਤੀ_2;
        ਜੇ(n){
            params.h = l.ਬਾਹਰਲਾ_h;
            params.w = l.ਬਾਹਰਲਾ_w;
            params.c = l.ਬਾਹਰਲਾ_c;
            params.inputs = l.outputs;
        }
    }
    free_list(sections);
    layer ਬਾਹਰਲਾ = get_ਜਾਲ_output_layer(net);
    net->outputs = ਬਾਹਰਲਾ.outputs;
    net->truths = ਬਾਹਰਲਾ.outputs;
    ਜੇ(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = ਬਾਹਰਲਾ.output;
    net->ਨਿਵੇਸ਼ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(net->inputs*net->batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    net->truth = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(net->truths*net->batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    net->output_gpu = ਬਾਹਰਲਾ.output_gpu;
    net->ਨਿਵੇਸ਼_gpu = cuda_make_array(net->ਨਿਵੇਸ਼, net->inputs*net->batch);
    net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
#ਜੇ_ਅੰਤ
    ਜੇ(workspace_ਮਾਪ){
        //printf("%ld\n", workspace_size);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
        ਜੇ(gpu_ਸੂਚਕਾਂਕ >= 0){
            net->workspace = cuda_make_array(0, (workspace_ਮਾਪ-1)/ਆਕਾਰ(ਦਸ਼ਮਲਵ)+1);
        }ਨਹੀਂਤਾਂ {
            net->workspace = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, workspace_ਮਾਪ);
        }
#ਨਹੀਂਤਾਂ
        net->workspace = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, workspace_ਮਾਪ);
#ਜੇ_ਅੰਤ
    }
    ਵਾਪਸ net;
}

list *read_cfg(ਅੱਖਰ *filename)
{
    FILE *file = fopen(filename, "r");
    ਜੇ(file == 0) file_error(filename);
    ਅੱਖਰ *ਸਤਰ;
    ਪੂਰਨ nu = 0;
    list *options = make_list();
    section *current = 0;
    ਜਦੋਂ((ਸਤਰ=fgetl(file)) != 0){
        ++ nu;
        strip(ਸਤਰ);
        ਚੋਣ(ਸਤਰ[0]){
            ਹਾਲਤ '[':
                current = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(section));
                list_ਪਾਓ(options, current);
                current->options = make_list();
                current->type = ਸਤਰ;
                ਰੋਕੋ;
            ਹਾਲਤ '\0':
            ਹਾਲਤ '#':
            ਹਾਲਤ ';':
                ਮੁਕਤ(ਸਤਰ);
                ਰੋਕੋ;
            ਮੂਲ:
                ਜੇ(!read_option(ਸਤਰ, current->options)){
                    fprintf(stderr, "Config file error line %d, could parse: %s\n", nu, ਸਤਰ);
                    ਮੁਕਤ(ਸਤਰ);
                }
                ਰੋਕੋ;
        }
    }
    fclose(file);
    ਵਾਪਸ options;
}

ਖਾਲੀ save_convolutional_weights_ਦੁਆਧਾਰੀ(layer l, FILE *fp)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    ਜੇ(gpu_ਸੂਚਕਾਂਕ >= 0){
        pull_convolutional_layer(l);
    }
#ਜੇ_ਅੰਤ
    binarize_weights(l.weights, l.n, l.c*l.ਮਾਪ*l.ਮਾਪ, l.ਦੁਆਧਾਰੀ_weights);
    ਪੂਰਨ ਮਾਪ = l.c*l.ਮਾਪ*l.ਮਾਪ;
    ਪੂਰਨ i, j, k;
    fwrite(l.biases, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.n, fp);
    ਜੇ (l.batch_normalize){
        fwrite(l.scales, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.n, fp);
        fwrite(l.rolling_ਔਸਤ, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.n, fp);
        fwrite(l.rolling_variance, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.n, fp);
    }
    ਲਈ(i = 0; i < l.n; ++i){
        ਦਸ਼ਮਲਵ ਔਸਤ = l.ਦੁਆਧਾਰੀ_weights[i*ਮਾਪ];
        ਜੇ(ਔਸਤ < 0) ਔਸਤ = -ਔਸਤ;
        fwrite(&ਔਸਤ, ਆਕਾਰ(ਦਸ਼ਮਲਵ), 1, fp);
        ਲਈ(j = 0; j < ਮਾਪ/8; ++j){
            ਪੂਰਨ ਸੂਚਕਾਂਕ = i*ਮਾਪ + j*8;
            ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c = 0;
            ਲਈ(k = 0; k < 8; ++k){
                ਜੇ (j*8 + k >= ਮਾਪ) ਰੋਕੋ;
                ਜੇ (l.ਦੁਆਧਾਰੀ_weights[ਸੂਚਕਾਂਕ + k] > 0) c = (c | 1<<k);
            }
            fwrite(&c, ਆਕਾਰ(ਅੱਖਰ), 1, fp);
        }
    }
}

ਖਾਲੀ save_convolutional_weights(layer l, FILE *fp)
{
    ਜੇ(l.ਦੁਆਧਾਰੀ){
        //save_convolutional_weights_binary(l, fp);
        //return;
    }
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    ਜੇ(gpu_ਸੂਚਕਾਂਕ >= 0){
        pull_convolutional_layer(l);
    }
#ਜੇ_ਅੰਤ
    ਪੂਰਨ ਅੰਕ = l.nweights;
    fwrite(l.biases, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.n, fp);
    ਜੇ (l.batch_normalize){
        fwrite(l.scales, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.n, fp);
        fwrite(l.rolling_ਔਸਤ, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.n, fp);
        fwrite(l.rolling_variance, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.n, fp);
    }
    fwrite(l.weights, ਆਕਾਰ(ਦਸ਼ਮਲਵ), ਅੰਕ, fp);
}

ਖਾਲੀ save_batchnorm_weights(layer l, FILE *fp)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    ਜੇ(gpu_ਸੂਚਕਾਂਕ >= 0){
        pull_batchnorm_layer(l);
    }
#ਜੇ_ਅੰਤ
    fwrite(l.scales, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.c, fp);
    fwrite(l.rolling_ਔਸਤ, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.c, fp);
    fwrite(l.rolling_variance, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.c, fp);
}

ਖਾਲੀ save_connected_weights(layer l, FILE *fp)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    ਜੇ(gpu_ਸੂਚਕਾਂਕ >= 0){
        pull_connected_layer(l);
    }
#ਜੇ_ਅੰਤ
    fwrite(l.biases, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.outputs, fp);
    fwrite(l.weights, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.outputs*l.inputs, fp);
    ਜੇ (l.batch_normalize){
        fwrite(l.scales, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.outputs, fp);
        fwrite(l.rolling_ਔਸਤ, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.outputs, fp);
        fwrite(l.rolling_variance, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.outputs, fp);
    }
}

ਖਾਲੀ save_weights_upto(ਜਾਲ *net, ਅੱਖਰ *filename, ਪੂਰਨ cutoff)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    ਜੇ(net->gpu_ਸੂਚਕਾਂਕ >= 0){
        cuda_ਸੈੱਟ_device(net->gpu_ਸੂਚਕਾਂਕ);
    }
#ਜੇ_ਅੰਤ
    fprintf(stderr, "Saving weights to %s\n", filename);
    FILE *fp = fopen(filename, "wb");
    ਜੇ(!fp) file_error(filename);

    ਪੂਰਨ major = 0;
    ਪੂਰਨ minor = 2;
    ਪੂਰਨ revision = 0;
    fwrite(&major, ਆਕਾਰ(ਪੂਰਨ), 1, fp);
    fwrite(&minor, ਆਕਾਰ(ਪੂਰਨ), 1, fp);
    fwrite(&revision, ਆਕਾਰ(ਪੂਰਨ), 1, fp);
    fwrite(net->ਵੇਖਿਆ, ਆਕਾਰ(size_t), 1, fp);

    ਪੂਰਨ i;
    ਲਈ(i = 0; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        ਜੇ (l.dontsave) ਜਾਰੀ;
        ਜੇ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            save_convolutional_weights(l, fp);
        } ਜੇ(l.type == CONNECTED){
            save_connected_weights(l, fp);
        } ਜੇ(l.type == BATCHNORM){
            save_batchnorm_weights(l, fp);
        } ਜੇ(l.type == RNN){
            save_connected_weights(*(l.ਨਿਵੇਸ਼_layer), fp);
            save_connected_weights(*(l.self_layer), fp);
            save_connected_weights(*(l.output_layer), fp);
        } ਜੇ (l.type == LSTM) {
            save_connected_weights(*(l.wi), fp);
            save_connected_weights(*(l.wf), fp);
            save_connected_weights(*(l.wo), fp);
            save_connected_weights(*(l.wg), fp);
            save_connected_weights(*(l.ui), fp);
            save_connected_weights(*(l.uf), fp);
            save_connected_weights(*(l.uo), fp);
            save_connected_weights(*(l.ug), fp);
        } ਜੇ (l.type == GRU) {
            ਜੇ(1){
                save_connected_weights(*(l.wz), fp);
                save_connected_weights(*(l.wr), fp);
                save_connected_weights(*(l.wh), fp);
                save_connected_weights(*(l.uz), fp);
                save_connected_weights(*(l.ur), fp);
                save_connected_weights(*(l.uh), fp);
            }ਨਹੀਂਤਾਂ{
                save_connected_weights(*(l.reset_layer), fp);
                save_connected_weights(*(l.update_layer), fp);
                save_connected_weights(*(l.state_layer), fp);
            }
        }  ਜੇ(l.type == CRNN){
            save_convolutional_weights(*(l.ਨਿਵੇਸ਼_layer), fp);
            save_convolutional_weights(*(l.self_layer), fp);
            save_convolutional_weights(*(l.output_layer), fp);
        } ਜੇ(l.type == LOCAL){
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
            ਜੇ(gpu_ਸੂਚਕਾਂਕ >= 0){
                pull_local_layer(l);
            }
#ਜੇ_ਅੰਤ
            ਪੂਰਨ locations = l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h;
            ਪੂਰਨ ਮਾਪ = l.ਮਾਪ*l.ਮਾਪ*l.c*l.n*locations;
            fwrite(l.biases, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.outputs, fp);
            fwrite(l.weights, ਆਕਾਰ(ਦਸ਼ਮਲਵ), ਮਾਪ, fp);
        }
    }
    fclose(fp);
}
ਖਾਲੀ save_weights(ਜਾਲ *net, ਅੱਖਰ *filename)
{
    save_weights_upto(net, filename, net->n);
}

ਖਾਲੀ transpose_ਮੈਟ੍ਰਿਕਸ(ਦਸ਼ਮਲਵ *a, ਪੂਰਨ ਪੰਕਤੀਆਂ, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ)
{
    ਦਸ਼ਮਲਵ *transpose = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਪੰਕਤੀਆਂ*ਖਾਨੇ_ਗਿਣਤੀ, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਪੂਰਨ x, y;
    ਲਈ(x = 0; x < ਪੰਕਤੀਆਂ; ++x){
        ਲਈ(y = 0; y < ਖਾਨੇ_ਗਿਣਤੀ; ++y){
            transpose[y*ਪੰਕਤੀਆਂ + x] = a[x*ਖਾਨੇ_ਗਿਣਤੀ + y];
        }
    }
    ਸਮ੍ਰਿਤੀ_ਨਕਲ(a, transpose, ਪੰਕਤੀਆਂ*ਖਾਨੇ_ਗਿਣਤੀ*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਮੁਕਤ(transpose);
}

ਖਾਲੀ ਲੱਦੋ_connected_weights(layer l, FILE *fp, ਪੂਰਨ transpose)
{
    fread(l.biases, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.outputs, fp);
    fread(l.weights, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.outputs*l.inputs, fp);
    ਜੇ(transpose){
        transpose_ਮੈਟ੍ਰਿਕਸ(l.weights, l.inputs, l.outputs);
    }
    //printf("Biases: %f mean %f variance\n", mean_array(l.biases, l.outputs), variance_array(l.biases, l.outputs));
    //printf("Weights: %f mean %f variance\n", mean_array(l.weights, l.outputs*l.inputs), variance_array(l.weights, l.outputs*l.inputs));
    ਜੇ (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.outputs, fp);
        fread(l.rolling_ਔਸਤ, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.outputs, fp);
        fread(l.rolling_variance, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.outputs, fp);
        //printf("Scales: %f mean %f variance\n", mean_array(l.scales, l.outputs), variance_array(l.scales, l.outputs));
        //printf("rolling_mean: %f mean %f variance\n", mean_array(l.rolling_mean, l.outputs), variance_array(l.rolling_mean, l.outputs));
        //printf("rolling_variance: %f mean %f variance\n", mean_array(l.rolling_variance, l.outputs), variance_array(l.rolling_variance, l.outputs));
    }
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    ਜੇ(gpu_ਸੂਚਕਾਂਕ >= 0){
        ਧੱਕੋ_connected_layer(l);
    }
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ ਲੱਦੋ_batchnorm_weights(layer l, FILE *fp)
{
    fread(l.scales, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.c, fp);
    fread(l.rolling_ਔਸਤ, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.c, fp);
    fread(l.rolling_variance, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.c, fp);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    ਜੇ(gpu_ਸੂਚਕਾਂਕ >= 0){
        ਧੱਕੋ_batchnorm_layer(l);
    }
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ ਲੱਦੋ_convolutional_weights_ਦੁਆਧਾਰੀ(layer l, FILE *fp)
{
    fread(l.biases, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.n, fp);
    ਜੇ (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.n, fp);
        fread(l.rolling_ਔਸਤ, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.n, fp);
        fread(l.rolling_variance, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.n, fp);
    }
    ਪੂਰਨ ਮਾਪ = l.c*l.ਮਾਪ*l.ਮਾਪ;
    ਪੂਰਨ i, j, k;
    ਲਈ(i = 0; i < l.n; ++i){
        ਦਸ਼ਮਲਵ ਔਸਤ = 0;
        fread(&ਔਸਤ, ਆਕਾਰ(ਦਸ਼ਮਲਵ), 1, fp);
        ਲਈ(j = 0; j < ਮਾਪ/8; ++j){
            ਪੂਰਨ ਸੂਚਕਾਂਕ = i*ਮਾਪ + j*8;
            ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c = 0;
            fread(&c, ਆਕਾਰ(ਅੱਖਰ), 1, fp);
            ਲਈ(k = 0; k < 8; ++k){
                ਜੇ (j*8 + k >= ਮਾਪ) ਰੋਕੋ;
                l.weights[ਸੂਚਕਾਂਕ + k] = (c & 1<<k) ? ਔਸਤ : -ਔਸਤ;
            }
        }
    }
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    ਜੇ(gpu_ਸੂਚਕਾਂਕ >= 0){
        ਧੱਕੋ_convolutional_layer(l);
    }
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ ਲੱਦੋ_convolutional_weights(layer l, FILE *fp)
{
    ਜੇ(l.ਦੁਆਧਾਰੀ){
        //load_convolutional_weights_binary(l, fp);
        //return;
    }
    ਜੇ(l.numload) l.n = l.numload;
    ਪੂਰਨ ਅੰਕ = l.c/l.groups*l.n*l.ਮਾਪ*l.ਮਾਪ;
    fread(l.biases, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.n, fp);
    ਜੇ (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.n, fp);
        fread(l.rolling_ਔਸਤ, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.n, fp);
        fread(l.rolling_variance, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.n, fp);
        ਜੇ(0){
            ਪੂਰਨ i;
            ਲਈ(i = 0; i < l.n; ++i){
                ਲਿਖੋ("%g, ", l.rolling_ਔਸਤ[i]);
            }
            ਲਿਖੋ("\n");
            ਲਈ(i = 0; i < l.n; ++i){
                ਲਿਖੋ("%g, ", l.rolling_variance[i]);
            }
            ਲਿਖੋ("\n");
        }
        ਜੇ(0){
            fill_cpu(l.n, 0, l.rolling_ਔਸਤ, 1);
            fill_cpu(l.n, 0, l.rolling_variance, 1);
        }
        ਜੇ(0){
            ਪੂਰਨ i;
            ਲਈ(i = 0; i < l.n; ++i){
                ਲਿਖੋ("%g, ", l.rolling_ਔਸਤ[i]);
            }
            ਲਿਖੋ("\n");
            ਲਈ(i = 0; i < l.n; ++i){
                ਲਿਖੋ("%g, ", l.rolling_variance[i]);
            }
            ਲਿਖੋ("\n");
        }
    }
    fread(l.weights, ਆਕਾਰ(ਦਸ਼ਮਲਵ), ਅੰਕ, fp);
    //if(l.c == 3) scal_cpu(num, 1./256, l.weights, 1);
    ਜੇ (l.flipped) {
        transpose_ਮੈਟ੍ਰਿਕਸ(l.weights, l.c*l.ਮਾਪ*l.ਮਾਪ, l.n);
    }
    //if (l.binary) binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.weights);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    ਜੇ(gpu_ਸੂਚਕਾਂਕ >= 0){
        ਧੱਕੋ_convolutional_layer(l);
    }
#ਜੇ_ਅੰਤ
}


ਖਾਲੀ ਲੱਦੋ_weights_upto(ਜਾਲ *net, ਅੱਖਰ *filename, ਪੂਰਨ ਸ਼ੁਰੂ, ਪੂਰਨ cutoff)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    ਜੇ(net->gpu_ਸੂਚਕਾਂਕ >= 0){
        cuda_ਸੈੱਟ_device(net->gpu_ਸੂਚਕਾਂਕ);
    }
#ਜੇ_ਅੰਤ
    fprintf(stderr, "Loading weights from %s...", filename);
    fflush(stdout);
    FILE *fp = fopen(filename, "rb");
    ਜੇ(!fp) file_error(filename);

    ਪੂਰਨ major;
    ਪੂਰਨ minor;
    ਪੂਰਨ revision;
    fread(&major, ਆਕਾਰ(ਪੂਰਨ), 1, fp);
    fread(&minor, ਆਕਾਰ(ਪੂਰਨ), 1, fp);
    fread(&revision, ਆਕਾਰ(ਪੂਰਨ), 1, fp);
    ਜੇ ((major*10 + minor) >= 2 && major < 1000 && minor < 1000){
        fread(net->ਵੇਖਿਆ, ਆਕਾਰ(size_t), 1, fp);
    } ਨਹੀਂਤਾਂ {
        ਪੂਰਨ iseen = 0;
        fread(&iseen, ਆਕਾਰ(ਪੂਰਨ), 1, fp);
        *net->ਵੇਖਿਆ = iseen;
    }
    ਪੂਰਨ transpose = (major > 1000) || (minor > 1000);

    ਪੂਰਨ i;
    ਲਈ(i = ਸ਼ੁਰੂ; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        ਜੇ (l.dontload) ਜਾਰੀ;
        ਜੇ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            ਲੱਦੋ_convolutional_weights(l, fp);
        }
        ਜੇ(l.type == CONNECTED){
            ਲੱਦੋ_connected_weights(l, fp, transpose);
        }
        ਜੇ(l.type == BATCHNORM){
            ਲੱਦੋ_batchnorm_weights(l, fp);
        }
        ਜੇ(l.type == CRNN){
            ਲੱਦੋ_convolutional_weights(*(l.ਨਿਵੇਸ਼_layer), fp);
            ਲੱਦੋ_convolutional_weights(*(l.self_layer), fp);
            ਲੱਦੋ_convolutional_weights(*(l.output_layer), fp);
        }
        ਜੇ(l.type == RNN){
            ਲੱਦੋ_connected_weights(*(l.ਨਿਵੇਸ਼_layer), fp, transpose);
            ਲੱਦੋ_connected_weights(*(l.self_layer), fp, transpose);
            ਲੱਦੋ_connected_weights(*(l.output_layer), fp, transpose);
        }
        ਜੇ (l.type == LSTM) {
            ਲੱਦੋ_connected_weights(*(l.wi), fp, transpose);
            ਲੱਦੋ_connected_weights(*(l.wf), fp, transpose);
            ਲੱਦੋ_connected_weights(*(l.wo), fp, transpose);
            ਲੱਦੋ_connected_weights(*(l.wg), fp, transpose);
            ਲੱਦੋ_connected_weights(*(l.ui), fp, transpose);
            ਲੱਦੋ_connected_weights(*(l.uf), fp, transpose);
            ਲੱਦੋ_connected_weights(*(l.uo), fp, transpose);
            ਲੱਦੋ_connected_weights(*(l.ug), fp, transpose);
        }
        ਜੇ (l.type == GRU) {
            ਜੇ(1){
                ਲੱਦੋ_connected_weights(*(l.wz), fp, transpose);
                ਲੱਦੋ_connected_weights(*(l.wr), fp, transpose);
                ਲੱਦੋ_connected_weights(*(l.wh), fp, transpose);
                ਲੱਦੋ_connected_weights(*(l.uz), fp, transpose);
                ਲੱਦੋ_connected_weights(*(l.ur), fp, transpose);
                ਲੱਦੋ_connected_weights(*(l.uh), fp, transpose);
            }ਨਹੀਂਤਾਂ{
                ਲੱਦੋ_connected_weights(*(l.reset_layer), fp, transpose);
                ਲੱਦੋ_connected_weights(*(l.update_layer), fp, transpose);
                ਲੱਦੋ_connected_weights(*(l.state_layer), fp, transpose);
            }
        }
        ਜੇ(l.type == LOCAL){
            ਪੂਰਨ locations = l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h;
            ਪੂਰਨ ਮਾਪ = l.ਮਾਪ*l.ਮਾਪ*l.c*l.n*locations;
            fread(l.biases, ਆਕਾਰ(ਦਸ਼ਮਲਵ), l.outputs, fp);
            fread(l.weights, ਆਕਾਰ(ਦਸ਼ਮਲਵ), ਮਾਪ, fp);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
            ਜੇ(gpu_ਸੂਚਕਾਂਕ >= 0){
                ਧੱਕੋ_local_layer(l);
            }
#ਜੇ_ਅੰਤ
        }
    }
    fprintf(stderr, "Done!\n");
    fclose(fp);
}

ਖਾਲੀ ਲੱਦੋ_weights(ਜਾਲ *net, ਅੱਖਰ *filename)
{
    ਲੱਦੋ_weights_upto(net, filename, 0, net->n);
}


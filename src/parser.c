#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "activation_layer.h"
#include "logistic_layer.h"
#include "l2norm_layer.h"
#include "activations.h"
#include "avgpool_layer.h"
#include "batchnorm_layer.h"
#include "blas.h"
#include "connected_layer.h"
#include "deconvolutional_layer.h"
#include "convolutional_layer.h"
#include "cost_layer.h"
#include "crnn_layer.h"
#include "crop_layer.h"
#include "detection_layer.h"
#include "dropout_layer.h"
#include "gru_layer.h"
#include "list.h"
#include "local_layer.h"
#include "maxpool_layer.h"
#include "normalization_layer.h"
#include "option_list.h"
#include "parser.h"
#include "region_layer.h"
#include "yolo_layer.h"
#include "iseg_layer.h"
#include "reorg_layer.h"
#include "rnn_layer.h"
#include "route_layer.h"
#include "upsample_layer.h"
#include "shortcut_layer.h"
#include "softmax_layer.h"
#include "lstm_layer.h"
#include "utils.h"

typedef sa_nracanaa{
    var_na *type;
    list *options;
}section;

list *read_cfg(var_na *filename);

LAYER_TYPE string_to_layer_type(var_na * type)
{

    ꦪꦼꦤ꧀ (strcmp(type, "[shortcut]")==0) ꦮꦭꦶ SHORTCUT;
    ꦪꦼꦤ꧀ (strcmp(type, "[crop]")==0) ꦮꦭꦶ CROP;
    ꦪꦼꦤ꧀ (strcmp(type, "[cost]")==0) ꦮꦭꦶ COST;
    ꦪꦼꦤ꧀ (strcmp(type, "[detection]")==0) ꦮꦭꦶ DETECTION;
    ꦪꦼꦤ꧀ (strcmp(type, "[region]")==0) ꦮꦭꦶ REGION;
    ꦪꦼꦤ꧀ (strcmp(type, "[yolo]")==0) ꦮꦭꦶ YOLO;
    ꦪꦼꦤ꧀ (strcmp(type, "[iseg]")==0) ꦮꦭꦶ ISEG;
    ꦪꦼꦤ꧀ (strcmp(type, "[local]")==0) ꦮꦭꦶ LOCAL;
    ꦪꦼꦤ꧀ (strcmp(type, "[conv]")==0
            || strcmp(type, "[convolutional]")==0) ꦮꦭꦶ CONVOLUTIONAL;
    ꦪꦼꦤ꧀ (strcmp(type, "[deconv]")==0
            || strcmp(type, "[deconvolutional]")==0) ꦮꦭꦶ DECONVOLUTIONAL;
    ꦪꦼꦤ꧀ (strcmp(type, "[activation]")==0) ꦮꦭꦶ ACTIVE;
    ꦪꦼꦤ꧀ (strcmp(type, "[logistic]")==0) ꦮꦭꦶ LOGXENT;
    ꦪꦼꦤ꧀ (strcmp(type, "[l2norm]")==0) ꦮꦭꦶ L2NORM;
    ꦪꦼꦤ꧀ (strcmp(type, "[net]")==0
            || strcmp(type, "[network]")==0) ꦮꦭꦶ NETWORK;
    ꦪꦼꦤ꧀ (strcmp(type, "[crnn]")==0) ꦮꦭꦶ CRNN;
    ꦪꦼꦤ꧀ (strcmp(type, "[gru]")==0) ꦮꦭꦶ GRU;
    ꦪꦼꦤ꧀ (strcmp(type, "[lstm]") == 0) ꦮꦭꦶ LSTM;
    ꦪꦼꦤ꧀ (strcmp(type, "[rnn]")==0) ꦮꦭꦶ RNN;
    ꦪꦼꦤ꧀ (strcmp(type, "[conn]")==0
            || strcmp(type, "[connected]")==0) ꦮꦭꦶ CONNECTED;
    ꦪꦼꦤ꧀ (strcmp(type, "[max]")==0
            || strcmp(type, "[maxpool]")==0) ꦮꦭꦶ MAXPOOL;
    ꦪꦼꦤ꧀ (strcmp(type, "[reorg]")==0) ꦮꦭꦶ REORG;
    ꦪꦼꦤ꧀ (strcmp(type, "[avg]")==0
            || strcmp(type, "[avgpool]")==0) ꦮꦭꦶ AVGPOOL;
    ꦪꦼꦤ꧀ (strcmp(type, "[dropout]")==0) ꦮꦭꦶ DROPOUT;
    ꦪꦼꦤ꧀ (strcmp(type, "[lrn]")==0
            || strcmp(type, "[normalization]")==0) ꦮꦭꦶ NORMALIZATION;
    ꦪꦼꦤ꧀ (strcmp(type, "[batchnorm]")==0) ꦮꦭꦶ BATCHNORM;
    ꦪꦼꦤ꧀ (strcmp(type, "[soft]")==0
            || strcmp(type, "[softmax]")==0) ꦮꦭꦶ SOFTMAX;
    ꦪꦼꦤ꧀ (strcmp(type, "[route]")==0) ꦮꦭꦶ ROUTE;
    ꦪꦼꦤ꧀ (strcmp(type, "[upsample]")==0) ꦮꦭꦶ UPSAMPLE;
    ꦮꦭꦶ BLANK;
}

void free_section(section *s)
{
    free(s->type);
    node *n = s->options->front;
    ꦱꦼꦢꦼꦭ(n){
        kvp *pair = (kvp *)n->val;
        free(pair->key);
        free(pair);
        node *next = n->next;
        free(n);
        n = next;
    }
    free(s->options);
    free(s);
}

void parse_data(var_na *data, float *a, puur_naa_nka n)
{
    puur_naa_nka i;
    ꦪꦼꦤ꧀(!data) ꦮꦭꦶ;
    var_na *curr = data;
    var_na *next = data;
    puur_naa_nka done = 0;
    ꦏꦁꦒꦺꦴ(i = 0; i < n && !done; ++i){
        ꦱꦼꦢꦼꦭ(*++next !='\0' && *next != ',');
        ꦪꦼꦤ꧀(*next == '\0') done = 1;
        *next = '\0';
        sscanf(curr, "%g", &a[i]);
        curr = next+1;
    }
}

typedef sa_nracanaa size_params{
    puur_naa_nka batch;
    puur_naa_nka inputs;
    puur_naa_nka h;
    puur_naa_nka w;
    puur_naa_nka c;
    puur_naa_nka index;
    puur_naa_nka time_steps;
    network *net;
} size_params;

local_layer parse_local(list *options, size_params params)
{
    puur_naa_nka n = option_find_int(options, "filters",1);
    puur_naa_nka size = option_find_int(options, "size",1);
    puur_naa_nka stride = option_find_int(options, "stride",1);
    puur_naa_nka pad = option_find_int(options, "pad",0);
    var_na *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    puur_naa_nka batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ꦪꦼꦤ꧀(!(h && w && c)) error("Layer before local layer must output image.");

    local_layer layer = make_local_layer(batch,h,w,c,n,size,stride,pad,activation);

    ꦮꦭꦶ layer;
}

layer parse_deconvolutional(list *options, size_params params)
{
    puur_naa_nka n = option_find_int(options, "filters",1);
    puur_naa_nka size = option_find_int(options, "size",1);
    puur_naa_nka stride = option_find_int(options, "stride",1);

    var_na *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    puur_naa_nka batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ꦪꦼꦤ꧀(!(h && w && c)) error("Layer before deconvolutional layer must output image.");
    puur_naa_nka batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);
    puur_naa_nka pad = option_find_int_quiet(options, "pad",0);
    puur_naa_nka padding = option_find_int_quiet(options, "padding",0);
    ꦪꦼꦤ꧀(pad) padding = size/2;

    layer l = make_deconvolutional_layer(batch,h,w,c,n,size,stride,padding, activation, batch_normalize, params.net->adam);

    ꦮꦭꦶ l;
}


convolutional_layer parse_convolutional(list *options, size_params params)
{
    puur_naa_nka n = option_find_int(options, "filters",1);
    puur_naa_nka size = option_find_int(options, "size",1);
    puur_naa_nka stride = option_find_int(options, "stride",1);
    puur_naa_nka pad = option_find_int_quiet(options, "pad",0);
    puur_naa_nka padding = option_find_int_quiet(options, "padding",0);
    puur_naa_nka groups = option_find_int_quiet(options, "groups", 1);
    ꦪꦼꦤ꧀(pad) padding = size/2;

    var_na *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    puur_naa_nka batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ꦪꦼꦤ꧀(!(h && w && c)) error("Layer before convolutional layer must output image.");
    puur_naa_nka batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);
    puur_naa_nka binary = option_find_int_quiet(options, "binary", 0);
    puur_naa_nka xnor = option_find_int_quiet(options, "xnor", 0);

    convolutional_layer layer = make_convolutional_layer(batch,h,w,c,n,groups,size,stride,padding,activation, batch_normalize, binary, xnor, params.net->adam);
    layer.flipped = option_find_int_quiet(options, "flipped", 0);
    layer.dot = option_find_float_quiet(options, "dot", 0);

    ꦮꦭꦶ layer;
}

layer parse_crnn(list *options, size_params params)
{
    puur_naa_nka output_filters = option_find_int(options, "output_filters",1);
    puur_naa_nka hidden_filters = option_find_int(options, "hidden_filters",1);
    var_na *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    puur_naa_nka batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_crnn_layer(params.batch, params.w, params.h, params.c, hidden_filters, output_filters, params.time_steps, activation, batch_normalize);

    l.shortcut = option_find_int_quiet(options, "shortcut", 0);

    ꦮꦭꦶ l;
}

layer parse_rnn(list *options, size_params params)
{
    puur_naa_nka output = option_find_int(options, "output",1);
    var_na *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    puur_naa_nka batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_rnn_layer(params.batch, params.inputs, output, params.time_steps, activation, batch_normalize, params.net->adam);

    l.shortcut = option_find_int_quiet(options, "shortcut", 0);

    ꦮꦭꦶ l;
}

layer parse_gru(list *options, size_params params)
{
    puur_naa_nka output = option_find_int(options, "output",1);
    puur_naa_nka batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_gru_layer(params.batch, params.inputs, output, params.time_steps, batch_normalize, params.net->adam);
    l.tanh = option_find_int_quiet(options, "tanh", 0);

    ꦮꦭꦶ l;
}

layer parse_lstm(list *options, size_params params)
{
    puur_naa_nka output = option_find_int(options, "output", 1);
    puur_naa_nka batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_lstm_layer(params.batch, params.inputs, output, params.time_steps, batch_normalize, params.net->adam);

    ꦮꦭꦶ l;
}

layer parse_connected(list *options, size_params params)
{
    puur_naa_nka output = option_find_int(options, "output",1);
    var_na *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    puur_naa_nka batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_connected_layer(params.batch, params.inputs, output, activation, batch_normalize, params.net->adam);
    ꦮꦭꦶ l;
}

layer parse_softmax(list *options, size_params params)
{
    puur_naa_nka groups = option_find_int_quiet(options, "groups",1);
    layer l = make_softmax_layer(params.batch, params.inputs, groups);
    l.temperature = option_find_float_quiet(options, "temperature", 1);
    var_na *tree_file = option_find_str(options, "tree", 0);
    ꦪꦼꦤ꧀ (tree_file) l.softmax_tree = read_tree(tree_file);
    l.w = params.w;
    l.h = params.h;
    l.c = params.c;
    l.spatial = option_find_float_quiet(options, "spatial", 0);
    l.noloss =  option_find_int_quiet(options, "noloss", 0);
    ꦮꦭꦶ l;
}

puur_naa_nka *parse_yolo_mask(var_na *a, puur_naa_nka *num)
{
    puur_naa_nka *mask = 0;
    ꦪꦼꦤ꧀(a){
        puur_naa_nka len = strlen(a);
        puur_naa_nka n = 1;
        puur_naa_nka i;
        ꦏꦁꦒꦺꦴ(i = 0; i < len; ++i){
            ꦪꦼꦤ꧀ (a[i] == ',') ++n;
        }
        mask = calloc(n, parimaa_na(puur_naa_nka));
        ꦏꦁꦒꦺꦴ(i = 0; i < n; ++i){
            puur_naa_nka val = atoi(a);
            mask[i] = val;
            a = strchr(a, ',')+1;
        }
        *num = n;
    }
    ꦮꦭꦶ mask;
}

layer parse_yolo(list *options, size_params params)
{
    puur_naa_nka classes = option_find_int(options, "classes", 20);
    puur_naa_nka total = option_find_int(options, "num", 1);
    puur_naa_nka num = total;

    var_na *a = option_find_str(options, "mask", 0);
    puur_naa_nka *mask = parse_yolo_mask(a, &num);
    layer l = make_yolo_layer(params.batch, params.w, params.h, num, total, mask, classes);
    assert(l.outputs == params.inputs);

    l.max_boxes = option_find_int_quiet(options, "max",90);
    l.jitter = option_find_float(options, "jitter", .2);

    l.ignore_thresh = option_find_float(options, "ignore_thresh", .5);
    l.truth_thresh = option_find_float(options, "truth_thresh", 1);
    l.random = option_find_int_quiet(options, "random", 0);

    var_na *map_file = option_find_str(options, "map", 0);
    ꦪꦼꦤ꧀ (map_file) l.map = read_map(map_file);

    a = option_find_str(options, "anchors", 0);
    ꦪꦼꦤ꧀(a){
        puur_naa_nka len = strlen(a);
        puur_naa_nka n = 1;
        puur_naa_nka i;
        ꦏꦁꦒꦺꦴ(i = 0; i < len; ++i){
            ꦪꦼꦤ꧀ (a[i] == ',') ++n;
        }
        ꦏꦁꦒꦺꦴ(i = 0; i < n; ++i){
            float bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    ꦮꦭꦶ l;
}

layer parse_iseg(list *options, size_params params)
{
    puur_naa_nka classes = option_find_int(options, "classes", 20);
    puur_naa_nka ids = option_find_int(options, "ids", 32);
    layer l = make_iseg_layer(params.batch, params.w, params.h, classes, ids);
    assert(l.outputs == params.inputs);
    ꦮꦭꦶ l;
}

layer parse_region(list *options, size_params params)
{
    puur_naa_nka coords = option_find_int(options, "coords", 4);
    puur_naa_nka classes = option_find_int(options, "classes", 20);
    puur_naa_nka num = option_find_int(options, "num", 1);

    layer l = make_region_layer(params.batch, params.w, params.h, num, classes, coords);
    assert(l.outputs == params.inputs);

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

    var_na *tree_file = option_find_str(options, "tree", 0);
    ꦪꦼꦤ꧀ (tree_file) l.softmax_tree = read_tree(tree_file);
    var_na *map_file = option_find_str(options, "map", 0);
    ꦪꦼꦤ꧀ (map_file) l.map = read_map(map_file);

    var_na *a = option_find_str(options, "anchors", 0);
    ꦪꦼꦤ꧀(a){
        puur_naa_nka len = strlen(a);
        puur_naa_nka n = 1;
        puur_naa_nka i;
        ꦏꦁꦒꦺꦴ(i = 0; i < len; ++i){
            ꦪꦼꦤ꧀ (a[i] == ',') ++n;
        }
        ꦏꦁꦒꦺꦴ(i = 0; i < n; ++i){
            float bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    ꦮꦭꦶ l;
}

detection_layer parse_detection(list *options, size_params params)
{
    puur_naa_nka coords = option_find_int(options, "coords", 1);
    puur_naa_nka classes = option_find_int(options, "classes", 1);
    puur_naa_nka rescore = option_find_int(options, "rescore", 0);
    puur_naa_nka num = option_find_int(options, "num", 1);
    puur_naa_nka side = option_find_int(options, "side", 7);
    detection_layer layer = make_detection_layer(params.batch, params.inputs, num, side, classes, coords, rescore);

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
    ꦮꦭꦶ layer;
}

cost_layer parse_cost(list *options, size_params params)
{
    var_na *type_s = option_find_str(options, "type", "sse");
    COST_TYPE type = get_cost_type(type_s);
    float scale = option_find_float_quiet(options, "scale",1);
    cost_layer layer = make_cost_layer(params.batch, params.inputs, type, scale);
    layer.ratio =  option_find_float_quiet(options, "ratio",0);
    layer.noobject_scale =  option_find_float_quiet(options, "noobj", 1);
    layer.thresh =  option_find_float_quiet(options, "thresh",0);
    ꦮꦭꦶ layer;
}

crop_layer parse_crop(list *options, size_params params)
{
    puur_naa_nka crop_height = option_find_int(options, "crop_height",1);
    puur_naa_nka crop_width = option_find_int(options, "crop_width",1);
    puur_naa_nka flip = option_find_int(options, "flip",0);
    float angle = option_find_float(options, "angle",0);
    float saturation = option_find_float(options, "saturation",1);
    float exposure = option_find_float(options, "exposure",1);

    puur_naa_nka batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ꦪꦼꦤ꧀(!(h && w && c)) error("Layer before crop layer must output image.");

    puur_naa_nka noadjust = option_find_int_quiet(options, "noadjust",0);

    crop_layer l = make_crop_layer(batch,h,w,c,crop_height,crop_width,flip, angle, saturation, exposure);
    l.shift = option_find_float(options, "shift", 0);
    l.noadjust = noadjust;
    ꦮꦭꦶ l;
}

layer parse_reorg(list *options, size_params params)
{
    puur_naa_nka stride = option_find_int(options, "stride",1);
    puur_naa_nka reverse = option_find_int_quiet(options, "reverse",0);
    puur_naa_nka flatten = option_find_int_quiet(options, "flatten",0);
    puur_naa_nka extra = option_find_int_quiet(options, "extra",0);

    puur_naa_nka batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ꦪꦼꦤ꧀(!(h && w && c)) error("Layer before reorg layer must output image.");

    layer layer = make_reorg_layer(batch,w,h,c,stride,reverse, flatten, extra);
    ꦮꦭꦶ layer;
}

maxpool_layer parse_maxpool(list *options, size_params params)
{
    puur_naa_nka stride = option_find_int(options, "stride",1);
    puur_naa_nka size = option_find_int(options, "size",stride);
    puur_naa_nka padding = option_find_int_quiet(options, "padding", size-1);

    puur_naa_nka batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ꦪꦼꦤ꧀(!(h && w && c)) error("Layer before maxpool layer must output image.");

    maxpool_layer layer = make_maxpool_layer(batch,h,w,c,size,stride,padding);
    ꦮꦭꦶ layer;
}

avgpool_layer parse_avgpool(list *options, size_params params)
{
    puur_naa_nka batch,w,h,c;
    w = params.w;
    h = params.h;
    c = params.c;
    batch=params.batch;
    ꦪꦼꦤ꧀(!(h && w && c)) error("Layer before avgpool layer must output image.");

    avgpool_layer layer = make_avgpool_layer(batch,w,h,c);
    ꦮꦭꦶ layer;
}

dropout_layer parse_dropout(list *options, size_params params)
{
    float probability = option_find_float(options, "probability", .5);
    dropout_layer layer = make_dropout_layer(params.batch, params.inputs, probability);
    layer.out_w = params.w;
    layer.out_h = params.h;
    layer.out_c = params.c;
    ꦮꦭꦶ layer;
}

layer parse_normalization(list *options, size_params params)
{
    float alpha = option_find_float(options, "alpha", .0001);
    float beta =  option_find_float(options, "beta" , .75);
    float kappa = option_find_float(options, "kappa", 1);
    puur_naa_nka size = option_find_int(options, "size", 5);
    layer l = make_normalization_layer(params.batch, params.w, params.h, params.c, size, alpha, beta, kappa);
    ꦮꦭꦶ l;
}

layer parse_batchnorm(list *options, size_params params)
{
    layer l = make_batchnorm_layer(params.batch, params.w, params.h, params.c);
    ꦮꦭꦶ l;
}

layer parse_shortcut(list *options, size_params params, network *net)
{
    var_na *l = option_find(options, "from");
    puur_naa_nka index = atoi(l);
    ꦪꦼꦤ꧀(index < 0) index = params.index + index;

    puur_naa_nka batch = params.batch;
    layer from = net->layers[index];

    layer s = make_shortcut_layer(batch, index, params.w, params.h, params.c, from.out_w, from.out_h, from.out_c);

    var_na *activation_s = option_find_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);
    s.activation = activation;
    s.alpha = option_find_float_quiet(options, "alpha", 1);
    s.beta = option_find_float_quiet(options, "beta", 1);
    ꦮꦭꦶ s;
}


layer parse_l2norm(list *options, size_params params)
{
    layer l = make_l2norm_layer(params.batch, params.inputs);
    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;
    ꦮꦭꦶ l;
}


layer parse_logistic(list *options, size_params params)
{
    layer l = make_logistic_layer(params.batch, params.inputs);
    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;
    ꦮꦭꦶ l;
}

layer parse_activation(list *options, size_params params)
{
    var_na *activation_s = option_find_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);

    layer l = make_activation_layer(params.batch, params.inputs, activation);

    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;

    ꦮꦭꦶ l;
}

layer parse_upsample(list *options, size_params params, network *net)
{

    puur_naa_nka stride = option_find_int(options, "stride",2);
    layer l = make_upsample_layer(params.batch, params.w, params.h, params.c, stride);
    l.scale = option_find_float_quiet(options, "scale", 1);
    ꦮꦭꦶ l;
}

route_layer parse_route(list *options, size_params params, network *net)
{
    var_na *l = option_find(options, "layers");
    puur_naa_nka len = strlen(l);
    ꦪꦼꦤ꧀(!l) error("Route Layer must specify input layers");
    puur_naa_nka n = 1;
    puur_naa_nka i;
    ꦏꦁꦒꦺꦴ(i = 0; i < len; ++i){
        ꦪꦼꦤ꧀ (l[i] == ',') ++n;
    }

    puur_naa_nka *layers = calloc(n, parimaa_na(puur_naa_nka));
    puur_naa_nka *sizes = calloc(n, parimaa_na(puur_naa_nka));
    ꦏꦁꦒꦺꦴ(i = 0; i < n; ++i){
        puur_naa_nka index = atoi(l);
        l = strchr(l, ',')+1;
        ꦪꦼꦤ꧀(index < 0) index = params.index + index;
        layers[i] = index;
        sizes[i] = net->layers[index].outputs;
    }
    puur_naa_nka batch = params.batch;

    route_layer layer = make_route_layer(batch, n, layers, sizes);

    convolutional_layer first = net->layers[layers[0]];
    layer.out_w = first.out_w;
    layer.out_h = first.out_h;
    layer.out_c = first.out_c;
    ꦏꦁꦒꦺꦴ(i = 1; i < n; ++i){
        puur_naa_nka index = layers[i];
        convolutional_layer next = net->layers[index];
        ꦪꦼꦤ꧀(next.out_w == first.out_w && next.out_h == first.out_h){
            layer.out_c += next.out_c;
        }ꦭꦶꦪ{
            layer.out_h = layer.out_w = layer.out_c = 0;
        }
    }

    ꦮꦭꦶ layer;
}

learning_rate_policy get_policy(var_na *s)
{
    ꦪꦼꦤ꧀ (strcmp(s, "random")==0) ꦮꦭꦶ RANDOM;
    ꦪꦼꦤ꧀ (strcmp(s, "poly")==0) ꦮꦭꦶ POLY;
    ꦪꦼꦤ꧀ (strcmp(s, "constant")==0) ꦮꦭꦶ CONSTANT;
    ꦪꦼꦤ꧀ (strcmp(s, "step")==0) ꦮꦭꦶ STEP;
    ꦪꦼꦤ꧀ (strcmp(s, "exp")==0) ꦮꦭꦶ EXP;
    ꦪꦼꦤ꧀ (strcmp(s, "sigmoid")==0) ꦮꦭꦶ SIG;
    ꦪꦼꦤ꧀ (strcmp(s, "steps")==0) ꦮꦭꦶ STEPS;
    fprintf(stderr, "Couldn't find policy %s, going with constant\n", s);
    ꦮꦭꦶ CONSTANT;
}

void parse_net_options(list *options, network *net)
{
    net->batch = option_find_int(options, "batch",1);
    net->learning_rate = option_find_float(options, "learning_rate", .001);
    net->momentum = option_find_float(options, "momentum", .9);
    net->decay = option_find_float(options, "decay", .0001);
    puur_naa_nka subdivs = option_find_int(options, "subdivisions",1);
    net->time_steps = option_find_int_quiet(options, "time_steps",1);
    net->notruth = option_find_int_quiet(options, "notruth",0);
    net->batch /= subdivs;
    net->batch *= net->time_steps;
    net->subdivisions = subdivs;
    net->random = option_find_int_quiet(options, "random", 0);

    net->adam = option_find_int_quiet(options, "adam", 0);
    ꦪꦼꦤ꧀(net->adam){
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
    net->max_ratio = option_find_float_quiet(options, "max_ratio", (float) net->max_crop / net->w);
    net->min_ratio = option_find_float_quiet(options, "min_ratio", (float) net->min_crop / net->w);
    net->center = option_find_int_quiet(options, "center",0);
    net->clip = option_find_float_quiet(options, "clip", 0);

    net->angle = option_find_float_quiet(options, "angle", 0);
    net->aspect = option_find_float_quiet(options, "aspect", 1);
    net->saturation = option_find_float_quiet(options, "saturation", 1);
    net->exposure = option_find_float_quiet(options, "exposure", 1);
    net->hue = option_find_float_quiet(options, "hue", 0);

    ꦪꦼꦤ꧀(!net->inputs && !(net->h && net->w && net->c)) error("No input parameters supplied");

    var_na *policy_s = option_find_str(options, "policy", "constant");
    net->policy = get_policy(policy_s);
    net->burn_in = option_find_int_quiet(options, "burn_in", 0);
    net->power = option_find_float_quiet(options, "power", 4);
    ꦪꦼꦤ꧀(net->policy == STEP){
        net->step = option_find_int(options, "step", 1);
        net->scale = option_find_float(options, "scale", 1);
    } ꦭꦶꦪ ꦪꦼꦤ꧀ (net->policy == STEPS){
        var_na *l = option_find(options, "steps");
        var_na *p = option_find(options, "scales");
        ꦪꦼꦤ꧀(!l || !p) error("STEPS policy must have steps and scales in cfg file");

        puur_naa_nka len = strlen(l);
        puur_naa_nka n = 1;
        puur_naa_nka i;
        ꦏꦁꦒꦺꦴ(i = 0; i < len; ++i){
            ꦪꦼꦤ꧀ (l[i] == ',') ++n;
        }
        puur_naa_nka *steps = calloc(n, parimaa_na(puur_naa_nka));
        float *scales = calloc(n, parimaa_na(float));
        ꦏꦁꦒꦺꦴ(i = 0; i < n; ++i){
            puur_naa_nka step    = atoi(l);
            float scale = atof(p);
            l = strchr(l, ',')+1;
            p = strchr(p, ',')+1;
            steps[i] = step;
            scales[i] = scale;
        }
        net->scales = scales;
        net->steps = steps;
        net->num_steps = n;
    } ꦭꦶꦪ ꦪꦼꦤ꧀ (net->policy == EXP){
        net->gamma = option_find_float(options, "gamma", 1);
    } ꦭꦶꦪ ꦪꦼꦤ꧀ (net->policy == SIG){
        net->gamma = option_find_float(options, "gamma", 1);
        net->step = option_find_int(options, "step", 1);
    } ꦭꦶꦪ ꦪꦼꦤ꧀ (net->policy == POLY || net->policy == RANDOM){
    }
    net->max_batches = option_find_int(options, "max_batches", 0);
}

puur_naa_nka is_network(section *s)
{
    ꦮꦭꦶ (strcmp(s->type, "[net]")==0
            || strcmp(s->type, "[network]")==0);
}

network *parse_network_cfg(var_na *filename)
{
    list *sections = read_cfg(filename);
    node *n = sections->front;
    ꦪꦼꦤ꧀(!n) error("Config file has no sections");
    network *net = make_network(sections->size - 1);
    net->gpu_index = gpu_index;
    size_params params;

    section *s = (section *)n->val;
    list *options = s->options;
    ꦪꦼꦤ꧀(!is_network(s)) error("First section must be [net] or [network]");
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
    puur_naa_nka count = 0;
    free_section(s);
    fprintf(stderr, "layer     filters    size              input                output\n");
    ꦱꦼꦢꦼꦭ(n){
        params.index = count;
        fprintf(stderr, "%5d ", count);
        s = (section *)n->val;
        options = s->options;
        layer l = {0};
        LAYER_TYPE lt = string_to_layer_type(s->type);
        ꦪꦼꦤ꧀(lt == CONVOLUTIONAL){
            l = parse_convolutional(options, params);
        }ꦭꦶꦪ ꦪꦼꦤ꧀(lt == DECONVOLUTIONAL){
            l = parse_deconvolutional(options, params);
        }ꦭꦶꦪ ꦪꦼꦤ꧀(lt == LOCAL){
            l = parse_local(options, params);
        }ꦭꦶꦪ ꦪꦼꦤ꧀(lt == ACTIVE){
            l = parse_activation(options, params);
        }ꦭꦶꦪ ꦪꦼꦤ꧀(lt == LOGXENT){
            l = parse_logistic(options, params);
        }ꦭꦶꦪ ꦪꦼꦤ꧀(lt == L2NORM){
            l = parse_l2norm(options, params);
        }ꦭꦶꦪ ꦪꦼꦤ꧀(lt == RNN){
            l = parse_rnn(options, params);
        }ꦭꦶꦪ ꦪꦼꦤ꧀(lt == GRU){
            l = parse_gru(options, params);
        }ꦭꦶꦪ ꦪꦼꦤ꧀ (lt == LSTM) {
            l = parse_lstm(options, params);
        }ꦭꦶꦪ ꦪꦼꦤ꧀(lt == CRNN){
            l = parse_crnn(options, params);
        }ꦭꦶꦪ ꦪꦼꦤ꧀(lt == CONNECTED){
            l = parse_connected(options, params);
        }ꦭꦶꦪ ꦪꦼꦤ꧀(lt == CROP){
            l = parse_crop(options, params);
        }ꦭꦶꦪ ꦪꦼꦤ꧀(lt == COST){
            l = parse_cost(options, params);
        }ꦭꦶꦪ ꦪꦼꦤ꧀(lt == REGION){
            l = parse_region(options, params);
        }ꦭꦶꦪ ꦪꦼꦤ꧀(lt == YOLO){
            l = parse_yolo(options, params);
        }ꦭꦶꦪ ꦪꦼꦤ꧀(lt == ISEG){
            l = parse_iseg(options, params);
        }ꦭꦶꦪ ꦪꦼꦤ꧀(lt == DETECTION){
            l = parse_detection(options, params);
        }ꦭꦶꦪ ꦪꦼꦤ꧀(lt == SOFTMAX){
            l = parse_softmax(options, params);
            net->hierarchy = l.softmax_tree;
        }ꦭꦶꦪ ꦪꦼꦤ꧀(lt == NORMALIZATION){
            l = parse_normalization(options, params);
        }ꦭꦶꦪ ꦪꦼꦤ꧀(lt == BATCHNORM){
            l = parse_batchnorm(options, params);
        }ꦭꦶꦪ ꦪꦼꦤ꧀(lt == MAXPOOL){
            l = parse_maxpool(options, params);
        }ꦭꦶꦪ ꦪꦼꦤ꧀(lt == REORG){
            l = parse_reorg(options, params);
        }ꦭꦶꦪ ꦪꦼꦤ꧀(lt == AVGPOOL){
            l = parse_avgpool(options, params);
        }ꦭꦶꦪ ꦪꦼꦤ꧀(lt == ROUTE){
            l = parse_route(options, params, net);
        }ꦭꦶꦪ ꦪꦼꦤ꧀(lt == UPSAMPLE){
            l = parse_upsample(options, params, net);
        }ꦭꦶꦪ ꦪꦼꦤ꧀(lt == SHORTCUT){
            l = parse_shortcut(options, params, net);
        }ꦭꦶꦪ ꦪꦼꦤ꧀(lt == DROPOUT){
            l = parse_dropout(options, params);
            l.output = net->layers[count-1].output;
            l.delta = net->layers[count-1].delta;
#ifdef GPU
            l.output_gpu = net->layers[count-1].output_gpu;
            l.delta_gpu = net->layers[count-1].delta_gpu;
#endif
        }ꦭꦶꦪ{
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
        net->layers[count] = l;
        ꦪꦼꦤ꧀ (l.workspace_size > workspace_size) workspace_size = l.workspace_size;
        free_section(s);
        n = n->next;
        ++count;
        ꦪꦼꦤ꧀(n){
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
    ꦪꦼꦤ꧀(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    net->input = calloc(net->inputs*net->batch, parimaa_na(float));
    net->truth = calloc(net->truths*net->batch, parimaa_na(float));
#ifdef GPU
    net->output_gpu = out.output_gpu;
    net->input_gpu = cuda_make_array(net->input, net->inputs*net->batch);
    net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
#endif
    ꦪꦼꦤ꧀(workspace_size){
        //printf("%ld\n", workspace_size);
#ifdef GPU
        ꦪꦼꦤ꧀(gpu_index >= 0){
            net->workspace = cuda_make_array(0, (workspace_size-1)/parimaa_na(float)+1);
        }ꦭꦶꦪ {
            net->workspace = calloc(1, workspace_size);
        }
#ꦭꦶꦪ
        net->workspace = calloc(1, workspace_size);
#endif
    }
    ꦮꦭꦶ net;
}

list *read_cfg(var_na *filename)
{
    FILE *file = fopen(filename, "r");
    ꦪꦼꦤ꧀(file == 0) file_error(filename);
    var_na *line;
    puur_naa_nka nu = 0;
    list *options = make_list();
    section *current = 0;
    ꦱꦼꦢꦼꦭ((line=fgetl(file)) != 0){
        ++ nu;
        strip(line);
        switch(line[0]){
            pak_sa '[':
                current = malloc(parimaa_na(section));
                list_insert(options, current);
                current->options = make_list();
                current->type = line;
                bha_nga;
            pak_sa '\0':
            pak_sa '#':
            pak_sa ';':
                free(line);
                bha_nga;
            default:
                ꦪꦼꦤ꧀(!read_option(line, current->options)){
                    fprintf(stderr, "Config file error line %d, could parse: %s\n", nu, line);
                    free(line);
                }
                bha_nga;
        }
    }
    fclose(file);
    ꦮꦭꦶ options;
}

void save_convolutional_weights_binary(layer l, FILE *fp)
{
#ifdef GPU
    ꦪꦼꦤ꧀(gpu_index >= 0){
        pull_convolutional_layer(l);
    }
#endif
    binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.binary_weights);
    puur_naa_nka size = l.c*l.size*l.size;
    puur_naa_nka i, j, k;
    fwrite(l.biases, parimaa_na(float), l.n, fp);
    ꦪꦼꦤ꧀ (l.batch_normalize){
        fwrite(l.scales, parimaa_na(float), l.n, fp);
        fwrite(l.rolling_mean, parimaa_na(float), l.n, fp);
        fwrite(l.rolling_variance, parimaa_na(float), l.n, fp);
    }
    ꦏꦁꦒꦺꦴ(i = 0; i < l.n; ++i){
        float mean = l.binary_weights[i*size];
        ꦪꦼꦤ꧀(mean < 0) mean = -mean;
        fwrite(&mean, parimaa_na(float), 1, fp);
        ꦏꦁꦒꦺꦴ(j = 0; j < size/8; ++j){
            puur_naa_nka index = i*size + j*8;
            unsigned var_na c = 0;
            ꦏꦁꦒꦺꦴ(k = 0; k < 8; ++k){
                ꦪꦼꦤ꧀ (j*8 + k >= size) bha_nga;
                ꦪꦼꦤ꧀ (l.binary_weights[index + k] > 0) c = (c | 1<<k);
            }
            fwrite(&c, parimaa_na(var_na), 1, fp);
        }
    }
}

void save_convolutional_weights(layer l, FILE *fp)
{
    ꦪꦼꦤ꧀(l.binary){
        //save_convolutional_weights_binary(l, fp);
        //return;
    }
#ifdef GPU
    ꦪꦼꦤ꧀(gpu_index >= 0){
        pull_convolutional_layer(l);
    }
#endif
    puur_naa_nka num = l.nweights;
    fwrite(l.biases, parimaa_na(float), l.n, fp);
    ꦪꦼꦤ꧀ (l.batch_normalize){
        fwrite(l.scales, parimaa_na(float), l.n, fp);
        fwrite(l.rolling_mean, parimaa_na(float), l.n, fp);
        fwrite(l.rolling_variance, parimaa_na(float), l.n, fp);
    }
    fwrite(l.weights, parimaa_na(float), num, fp);
}

void save_batchnorm_weights(layer l, FILE *fp)
{
#ifdef GPU
    ꦪꦼꦤ꧀(gpu_index >= 0){
        pull_batchnorm_layer(l);
    }
#endif
    fwrite(l.scales, parimaa_na(float), l.c, fp);
    fwrite(l.rolling_mean, parimaa_na(float), l.c, fp);
    fwrite(l.rolling_variance, parimaa_na(float), l.c, fp);
}

void save_connected_weights(layer l, FILE *fp)
{
#ifdef GPU
    ꦪꦼꦤ꧀(gpu_index >= 0){
        pull_connected_layer(l);
    }
#endif
    fwrite(l.biases, parimaa_na(float), l.outputs, fp);
    fwrite(l.weights, parimaa_na(float), l.outputs*l.inputs, fp);
    ꦪꦼꦤ꧀ (l.batch_normalize){
        fwrite(l.scales, parimaa_na(float), l.outputs, fp);
        fwrite(l.rolling_mean, parimaa_na(float), l.outputs, fp);
        fwrite(l.rolling_variance, parimaa_na(float), l.outputs, fp);
    }
}

void save_weights_upto(network *net, var_na *filename, puur_naa_nka cutoff)
{
#ifdef GPU
    ꦪꦼꦤ꧀(net->gpu_index >= 0){
        cuda_set_device(net->gpu_index);
    }
#endif
    fprintf(stderr, "Saving weights to %s\n", filename);
    FILE *fp = fopen(filename, "wb");
    ꦪꦼꦤ꧀(!fp) file_error(filename);

    puur_naa_nka major = 0;
    puur_naa_nka minor = 2;
    puur_naa_nka revision = 0;
    fwrite(&major, parimaa_na(puur_naa_nka), 1, fp);
    fwrite(&minor, parimaa_na(puur_naa_nka), 1, fp);
    fwrite(&revision, parimaa_na(puur_naa_nka), 1, fp);
    fwrite(net->seen, parimaa_na(size_t), 1, fp);

    puur_naa_nka i;
    ꦏꦁꦒꦺꦴ(i = 0; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        ꦪꦼꦤ꧀ (l.dontsave) continue;
        ꦪꦼꦤ꧀(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            save_convolutional_weights(l, fp);
        } ꦪꦼꦤ꧀(l.type == CONNECTED){
            save_connected_weights(l, fp);
        } ꦪꦼꦤ꧀(l.type == BATCHNORM){
            save_batchnorm_weights(l, fp);
        } ꦪꦼꦤ꧀(l.type == RNN){
            save_connected_weights(*(l.input_layer), fp);
            save_connected_weights(*(l.self_layer), fp);
            save_connected_weights(*(l.output_layer), fp);
        } ꦪꦼꦤ꧀ (l.type == LSTM) {
            save_connected_weights(*(l.wi), fp);
            save_connected_weights(*(l.wf), fp);
            save_connected_weights(*(l.wo), fp);
            save_connected_weights(*(l.wg), fp);
            save_connected_weights(*(l.ui), fp);
            save_connected_weights(*(l.uf), fp);
            save_connected_weights(*(l.uo), fp);
            save_connected_weights(*(l.ug), fp);
        } ꦪꦼꦤ꧀ (l.type == GRU) {
            ꦪꦼꦤ꧀(1){
                save_connected_weights(*(l.wz), fp);
                save_connected_weights(*(l.wr), fp);
                save_connected_weights(*(l.wh), fp);
                save_connected_weights(*(l.uz), fp);
                save_connected_weights(*(l.ur), fp);
                save_connected_weights(*(l.uh), fp);
            }ꦭꦶꦪ{
                save_connected_weights(*(l.reset_layer), fp);
                save_connected_weights(*(l.update_layer), fp);
                save_connected_weights(*(l.state_layer), fp);
            }
        }  ꦪꦼꦤ꧀(l.type == CRNN){
            save_convolutional_weights(*(l.input_layer), fp);
            save_convolutional_weights(*(l.self_layer), fp);
            save_convolutional_weights(*(l.output_layer), fp);
        } ꦪꦼꦤ꧀(l.type == LOCAL){
#ifdef GPU
            ꦪꦼꦤ꧀(gpu_index >= 0){
                pull_local_layer(l);
            }
#endif
            puur_naa_nka locations = l.out_w*l.out_h;
            puur_naa_nka size = l.size*l.size*l.c*l.n*locations;
            fwrite(l.biases, parimaa_na(float), l.outputs, fp);
            fwrite(l.weights, parimaa_na(float), size, fp);
        }
    }
    fclose(fp);
}
void save_weights(network *net, var_na *filename)
{
    save_weights_upto(net, filename, net->n);
}

void transpose_matrix(float *a, puur_naa_nka rows, puur_naa_nka cols)
{
    float *transpose = calloc(rows*cols, parimaa_na(float));
    puur_naa_nka x, y;
    ꦏꦁꦒꦺꦴ(x = 0; x < rows; ++x){
        ꦏꦁꦒꦺꦴ(y = 0; y < cols; ++y){
            transpose[y*rows + x] = a[x*cols + y];
        }
    }
    memcpy(a, transpose, rows*cols*parimaa_na(float));
    free(transpose);
}

void load_connected_weights(layer l, FILE *fp, puur_naa_nka transpose)
{
    fread(l.biases, parimaa_na(float), l.outputs, fp);
    fread(l.weights, parimaa_na(float), l.outputs*l.inputs, fp);
    ꦪꦼꦤ꧀(transpose){
        transpose_matrix(l.weights, l.inputs, l.outputs);
    }
    //printf("Biases: %f mean %f variance\n", mean_array(l.biases, l.outputs), variance_array(l.biases, l.outputs));
    //printf("Weights: %f mean %f variance\n", mean_array(l.weights, l.outputs*l.inputs), variance_array(l.weights, l.outputs*l.inputs));
    ꦪꦼꦤ꧀ (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, parimaa_na(float), l.outputs, fp);
        fread(l.rolling_mean, parimaa_na(float), l.outputs, fp);
        fread(l.rolling_variance, parimaa_na(float), l.outputs, fp);
        //printf("Scales: %f mean %f variance\n", mean_array(l.scales, l.outputs), variance_array(l.scales, l.outputs));
        //printf("rolling_mean: %f mean %f variance\n", mean_array(l.rolling_mean, l.outputs), variance_array(l.rolling_mean, l.outputs));
        //printf("rolling_variance: %f mean %f variance\n", mean_array(l.rolling_variance, l.outputs), variance_array(l.rolling_variance, l.outputs));
    }
#ifdef GPU
    ꦪꦼꦤ꧀(gpu_index >= 0){
        push_connected_layer(l);
    }
#endif
}

void load_batchnorm_weights(layer l, FILE *fp)
{
    fread(l.scales, parimaa_na(float), l.c, fp);
    fread(l.rolling_mean, parimaa_na(float), l.c, fp);
    fread(l.rolling_variance, parimaa_na(float), l.c, fp);
#ifdef GPU
    ꦪꦼꦤ꧀(gpu_index >= 0){
        push_batchnorm_layer(l);
    }
#endif
}

void load_convolutional_weights_binary(layer l, FILE *fp)
{
    fread(l.biases, parimaa_na(float), l.n, fp);
    ꦪꦼꦤ꧀ (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, parimaa_na(float), l.n, fp);
        fread(l.rolling_mean, parimaa_na(float), l.n, fp);
        fread(l.rolling_variance, parimaa_na(float), l.n, fp);
    }
    puur_naa_nka size = l.c*l.size*l.size;
    puur_naa_nka i, j, k;
    ꦏꦁꦒꦺꦴ(i = 0; i < l.n; ++i){
        float mean = 0;
        fread(&mean, parimaa_na(float), 1, fp);
        ꦏꦁꦒꦺꦴ(j = 0; j < size/8; ++j){
            puur_naa_nka index = i*size + j*8;
            unsigned var_na c = 0;
            fread(&c, parimaa_na(var_na), 1, fp);
            ꦏꦁꦒꦺꦴ(k = 0; k < 8; ++k){
                ꦪꦼꦤ꧀ (j*8 + k >= size) bha_nga;
                l.weights[index + k] = (c & 1<<k) ? mean : -mean;
            }
        }
    }
#ifdef GPU
    ꦪꦼꦤ꧀(gpu_index >= 0){
        push_convolutional_layer(l);
    }
#endif
}

void load_convolutional_weights(layer l, FILE *fp)
{
    ꦪꦼꦤ꧀(l.binary){
        //load_convolutional_weights_binary(l, fp);
        //return;
    }
    ꦪꦼꦤ꧀(l.numload) l.n = l.numload;
    puur_naa_nka num = l.c/l.groups*l.n*l.size*l.size;
    fread(l.biases, parimaa_na(float), l.n, fp);
    ꦪꦼꦤ꧀ (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, parimaa_na(float), l.n, fp);
        fread(l.rolling_mean, parimaa_na(float), l.n, fp);
        fread(l.rolling_variance, parimaa_na(float), l.n, fp);
        ꦪꦼꦤ꧀(0){
            puur_naa_nka i;
            ꦏꦁꦒꦺꦴ(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_mean[i]);
            }
            printf("\n");
            ꦏꦁꦒꦺꦴ(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_variance[i]);
            }
            printf("\n");
        }
        ꦪꦼꦤ꧀(0){
            fill_cpu(l.n, 0, l.rolling_mean, 1);
            fill_cpu(l.n, 0, l.rolling_variance, 1);
        }
        ꦪꦼꦤ꧀(0){
            puur_naa_nka i;
            ꦏꦁꦒꦺꦴ(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_mean[i]);
            }
            printf("\n");
            ꦏꦁꦒꦺꦴ(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_variance[i]);
            }
            printf("\n");
        }
    }
    fread(l.weights, parimaa_na(float), num, fp);
    //if(l.c == 3) scal_cpu(num, 1./256, l.weights, 1);
    ꦪꦼꦤ꧀ (l.flipped) {
        transpose_matrix(l.weights, l.c*l.size*l.size, l.n);
    }
    //if (l.binary) binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.weights);
#ifdef GPU
    ꦪꦼꦤ꧀(gpu_index >= 0){
        push_convolutional_layer(l);
    }
#endif
}


void load_weights_upto(network *net, var_na *filename, puur_naa_nka start, puur_naa_nka cutoff)
{
#ifdef GPU
    ꦪꦼꦤ꧀(net->gpu_index >= 0){
        cuda_set_device(net->gpu_index);
    }
#endif
    fprintf(stderr, "Loading weights from %s...", filename);
    fflush(stdout);
    FILE *fp = fopen(filename, "rb");
    ꦪꦼꦤ꧀(!fp) file_error(filename);

    puur_naa_nka major;
    puur_naa_nka minor;
    puur_naa_nka revision;
    fread(&major, parimaa_na(puur_naa_nka), 1, fp);
    fread(&minor, parimaa_na(puur_naa_nka), 1, fp);
    fread(&revision, parimaa_na(puur_naa_nka), 1, fp);
    ꦪꦼꦤ꧀ ((major*10 + minor) >= 2 && major < 1000 && minor < 1000){
        fread(net->seen, parimaa_na(size_t), 1, fp);
    } ꦭꦶꦪ {
        puur_naa_nka iseen = 0;
        fread(&iseen, parimaa_na(puur_naa_nka), 1, fp);
        *net->seen = iseen;
    }
    puur_naa_nka transpose = (major > 1000) || (minor > 1000);

    puur_naa_nka i;
    ꦏꦁꦒꦺꦴ(i = start; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        ꦪꦼꦤ꧀ (l.dontload) continue;
        ꦪꦼꦤ꧀(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            load_convolutional_weights(l, fp);
        }
        ꦪꦼꦤ꧀(l.type == CONNECTED){
            load_connected_weights(l, fp, transpose);
        }
        ꦪꦼꦤ꧀(l.type == BATCHNORM){
            load_batchnorm_weights(l, fp);
        }
        ꦪꦼꦤ꧀(l.type == CRNN){
            load_convolutional_weights(*(l.input_layer), fp);
            load_convolutional_weights(*(l.self_layer), fp);
            load_convolutional_weights(*(l.output_layer), fp);
        }
        ꦪꦼꦤ꧀(l.type == RNN){
            load_connected_weights(*(l.input_layer), fp, transpose);
            load_connected_weights(*(l.self_layer), fp, transpose);
            load_connected_weights(*(l.output_layer), fp, transpose);
        }
        ꦪꦼꦤ꧀ (l.type == LSTM) {
            load_connected_weights(*(l.wi), fp, transpose);
            load_connected_weights(*(l.wf), fp, transpose);
            load_connected_weights(*(l.wo), fp, transpose);
            load_connected_weights(*(l.wg), fp, transpose);
            load_connected_weights(*(l.ui), fp, transpose);
            load_connected_weights(*(l.uf), fp, transpose);
            load_connected_weights(*(l.uo), fp, transpose);
            load_connected_weights(*(l.ug), fp, transpose);
        }
        ꦪꦼꦤ꧀ (l.type == GRU) {
            ꦪꦼꦤ꧀(1){
                load_connected_weights(*(l.wz), fp, transpose);
                load_connected_weights(*(l.wr), fp, transpose);
                load_connected_weights(*(l.wh), fp, transpose);
                load_connected_weights(*(l.uz), fp, transpose);
                load_connected_weights(*(l.ur), fp, transpose);
                load_connected_weights(*(l.uh), fp, transpose);
            }ꦭꦶꦪ{
                load_connected_weights(*(l.reset_layer), fp, transpose);
                load_connected_weights(*(l.update_layer), fp, transpose);
                load_connected_weights(*(l.state_layer), fp, transpose);
            }
        }
        ꦪꦼꦤ꧀(l.type == LOCAL){
            puur_naa_nka locations = l.out_w*l.out_h;
            puur_naa_nka size = l.size*l.size*l.c*l.n*locations;
            fread(l.biases, parimaa_na(float), l.outputs, fp);
            fread(l.weights, parimaa_na(float), size, fp);
#ifdef GPU
            ꦪꦼꦤ꧀(gpu_index >= 0){
                push_local_layer(l);
            }
#endif
        }
    }
    fprintf(stderr, "Done!\n");
    fclose(fp);
}

void load_weights(network *net, var_na *filename)
{
    load_weights_upto(net, filename, 0, net->n);
}


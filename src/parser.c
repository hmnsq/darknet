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

    གལ་སྲིད (strcmp(type, "[shortcut]")==0) ལོག SHORTCUT;
    གལ་སྲིད (strcmp(type, "[crop]")==0) ལོག CROP;
    གལ་སྲིད (strcmp(type, "[cost]")==0) ལོག COST;
    གལ་སྲིད (strcmp(type, "[detection]")==0) ལོག DETECTION;
    གལ་སྲིད (strcmp(type, "[region]")==0) ལོག REGION;
    གལ་སྲིད (strcmp(type, "[yolo]")==0) ལོག YOLO;
    གལ་སྲིད (strcmp(type, "[iseg]")==0) ལོག ISEG;
    གལ་སྲིད (strcmp(type, "[local]")==0) ལོག LOCAL;
    གལ་སྲིད (strcmp(type, "[conv]")==0
            || strcmp(type, "[convolutional]")==0) ལོག CONVOLUTIONAL;
    གལ་སྲིད (strcmp(type, "[deconv]")==0
            || strcmp(type, "[deconvolutional]")==0) ལོག DECONVOLUTIONAL;
    གལ་སྲིད (strcmp(type, "[activation]")==0) ལོག ACTIVE;
    གལ་སྲིད (strcmp(type, "[logistic]")==0) ལོག LOGXENT;
    གལ་སྲིད (strcmp(type, "[l2norm]")==0) ལོག L2NORM;
    གལ་སྲིད (strcmp(type, "[net]")==0
            || strcmp(type, "[network]")==0) ལོག NETWORK;
    གལ་སྲིད (strcmp(type, "[crnn]")==0) ལོག CRNN;
    གལ་སྲིད (strcmp(type, "[gru]")==0) ལོག GRU;
    གལ་སྲིད (strcmp(type, "[lstm]") == 0) ལོག LSTM;
    གལ་སྲིད (strcmp(type, "[rnn]")==0) ལོག RNN;
    གལ་སྲིད (strcmp(type, "[conn]")==0
            || strcmp(type, "[connected]")==0) ལོག CONNECTED;
    གལ་སྲིད (strcmp(type, "[max]")==0
            || strcmp(type, "[maxpool]")==0) ལོག MAXPOOL;
    གལ་སྲིད (strcmp(type, "[reorg]")==0) ལོག REORG;
    གལ་སྲིད (strcmp(type, "[avg]")==0
            || strcmp(type, "[avgpool]")==0) ལོག AVGPOOL;
    གལ་སྲིད (strcmp(type, "[dropout]")==0) ལོག DROPOUT;
    གལ་སྲིད (strcmp(type, "[lrn]")==0
            || strcmp(type, "[normalization]")==0) ལོག NORMALIZATION;
    གལ་སྲིད (strcmp(type, "[batchnorm]")==0) ལོག BATCHNORM;
    གལ་སྲིད (strcmp(type, "[soft]")==0
            || strcmp(type, "[softmax]")==0) ལོག SOFTMAX;
    གལ་སྲིད (strcmp(type, "[route]")==0) ལོག ROUTE;
    གལ་སྲིད (strcmp(type, "[upsample]")==0) ལོག UPSAMPLE;
    ལོག BLANK;
}

void free_section(section *s)
{
    free(s->type);
    node *n = s->options->front;
    སྐབས(n){
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

void parse_data(var_na *data, float *a, ཧྲིལ n)
{
    ཧྲིལ i;
    གལ་སྲིད(!data) ལོག;
    var_na *curr = data;
    var_na *next = data;
    ཧྲིལ done = 0;
    དོན་ལུ(i = 0; i < n && !done; ++i){
        སྐབས(*++next !='\0' && *next != ',');
        གལ་སྲིད(*next == '\0') done = 1;
        *next = '\0';
        sscanf(curr, "%g", &a[i]);
        curr = next+1;
    }
}

typedef sa_nracanaa size_params{
    ཧྲིལ batch;
    ཧྲིལ inputs;
    ཧྲིལ h;
    ཧྲིལ w;
    ཧྲིལ c;
    ཧྲིལ index;
    ཧྲིལ time_steps;
    network *net;
} size_params;

local_layer parse_local(list *options, size_params params)
{
    ཧྲིལ n = option_find_int(options, "filters",1);
    ཧྲིལ size = option_find_int(options, "size",1);
    ཧྲིལ stride = option_find_int(options, "stride",1);
    ཧྲིལ pad = option_find_int(options, "pad",0);
    var_na *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    ཧྲིལ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    གལ་སྲིད(!(h && w && c)) error("Layer before local layer must output image.");

    local_layer layer = make_local_layer(batch,h,w,c,n,size,stride,pad,activation);

    ལོག layer;
}

layer parse_deconvolutional(list *options, size_params params)
{
    ཧྲིལ n = option_find_int(options, "filters",1);
    ཧྲིལ size = option_find_int(options, "size",1);
    ཧྲིལ stride = option_find_int(options, "stride",1);

    var_na *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    ཧྲིལ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    གལ་སྲིད(!(h && w && c)) error("Layer before deconvolutional layer must output image.");
    ཧྲིལ batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);
    ཧྲིལ pad = option_find_int_quiet(options, "pad",0);
    ཧྲིལ padding = option_find_int_quiet(options, "padding",0);
    གལ་སྲིད(pad) padding = size/2;

    layer l = make_deconvolutional_layer(batch,h,w,c,n,size,stride,padding, activation, batch_normalize, params.net->adam);

    ལོག l;
}


convolutional_layer parse_convolutional(list *options, size_params params)
{
    ཧྲིལ n = option_find_int(options, "filters",1);
    ཧྲིལ size = option_find_int(options, "size",1);
    ཧྲིལ stride = option_find_int(options, "stride",1);
    ཧྲིལ pad = option_find_int_quiet(options, "pad",0);
    ཧྲིལ padding = option_find_int_quiet(options, "padding",0);
    ཧྲིལ groups = option_find_int_quiet(options, "groups", 1);
    གལ་སྲིད(pad) padding = size/2;

    var_na *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    ཧྲིལ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    གལ་སྲིད(!(h && w && c)) error("Layer before convolutional layer must output image.");
    ཧྲིལ batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);
    ཧྲིལ binary = option_find_int_quiet(options, "binary", 0);
    ཧྲིལ xnor = option_find_int_quiet(options, "xnor", 0);

    convolutional_layer layer = make_convolutional_layer(batch,h,w,c,n,groups,size,stride,padding,activation, batch_normalize, binary, xnor, params.net->adam);
    layer.flipped = option_find_int_quiet(options, "flipped", 0);
    layer.dot = option_find_float_quiet(options, "dot", 0);

    ལོག layer;
}

layer parse_crnn(list *options, size_params params)
{
    ཧྲིལ output_filters = option_find_int(options, "output_filters",1);
    ཧྲིལ hidden_filters = option_find_int(options, "hidden_filters",1);
    var_na *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    ཧྲིལ batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_crnn_layer(params.batch, params.w, params.h, params.c, hidden_filters, output_filters, params.time_steps, activation, batch_normalize);

    l.shortcut = option_find_int_quiet(options, "shortcut", 0);

    ལོག l;
}

layer parse_rnn(list *options, size_params params)
{
    ཧྲིལ output = option_find_int(options, "output",1);
    var_na *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    ཧྲིལ batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_rnn_layer(params.batch, params.inputs, output, params.time_steps, activation, batch_normalize, params.net->adam);

    l.shortcut = option_find_int_quiet(options, "shortcut", 0);

    ལོག l;
}

layer parse_gru(list *options, size_params params)
{
    ཧྲིལ output = option_find_int(options, "output",1);
    ཧྲིལ batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_gru_layer(params.batch, params.inputs, output, params.time_steps, batch_normalize, params.net->adam);
    l.tanh = option_find_int_quiet(options, "tanh", 0);

    ལོག l;
}

layer parse_lstm(list *options, size_params params)
{
    ཧྲིལ output = option_find_int(options, "output", 1);
    ཧྲིལ batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_lstm_layer(params.batch, params.inputs, output, params.time_steps, batch_normalize, params.net->adam);

    ལོག l;
}

layer parse_connected(list *options, size_params params)
{
    ཧྲིལ output = option_find_int(options, "output",1);
    var_na *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    ཧྲིལ batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_connected_layer(params.batch, params.inputs, output, activation, batch_normalize, params.net->adam);
    ལོག l;
}

layer parse_softmax(list *options, size_params params)
{
    ཧྲིལ groups = option_find_int_quiet(options, "groups",1);
    layer l = make_softmax_layer(params.batch, params.inputs, groups);
    l.temperature = option_find_float_quiet(options, "temperature", 1);
    var_na *tree_file = option_find_str(options, "tree", 0);
    གལ་སྲིད (tree_file) l.softmax_tree = read_tree(tree_file);
    l.w = params.w;
    l.h = params.h;
    l.c = params.c;
    l.spatial = option_find_float_quiet(options, "spatial", 0);
    l.noloss =  option_find_int_quiet(options, "noloss", 0);
    ལོག l;
}

ཧྲིལ *parse_yolo_mask(var_na *a, ཧྲིལ *num)
{
    ཧྲིལ *mask = 0;
    གལ་སྲིད(a){
        ཧྲིལ len = strlen(a);
        ཧྲིལ n = 1;
        ཧྲིལ i;
        དོན་ལུ(i = 0; i < len; ++i){
            གལ་སྲིད (a[i] == ',') ++n;
        }
        mask = calloc(n, parimaa_na(ཧྲིལ));
        དོན་ལུ(i = 0; i < n; ++i){
            ཧྲིལ val = atoi(a);
            mask[i] = val;
            a = strchr(a, ',')+1;
        }
        *num = n;
    }
    ལོག mask;
}

layer parse_yolo(list *options, size_params params)
{
    ཧྲིལ classes = option_find_int(options, "classes", 20);
    ཧྲིལ total = option_find_int(options, "num", 1);
    ཧྲིལ num = total;

    var_na *a = option_find_str(options, "mask", 0);
    ཧྲིལ *mask = parse_yolo_mask(a, &num);
    layer l = make_yolo_layer(params.batch, params.w, params.h, num, total, mask, classes);
    assert(l.outputs == params.inputs);

    l.max_boxes = option_find_int_quiet(options, "max",90);
    l.jitter = option_find_float(options, "jitter", .2);

    l.ignore_thresh = option_find_float(options, "ignore_thresh", .5);
    l.truth_thresh = option_find_float(options, "truth_thresh", 1);
    l.random = option_find_int_quiet(options, "random", 0);

    var_na *map_file = option_find_str(options, "map", 0);
    གལ་སྲིད (map_file) l.map = read_map(map_file);

    a = option_find_str(options, "anchors", 0);
    གལ་སྲིད(a){
        ཧྲིལ len = strlen(a);
        ཧྲིལ n = 1;
        ཧྲིལ i;
        དོན་ལུ(i = 0; i < len; ++i){
            གལ་སྲིད (a[i] == ',') ++n;
        }
        དོན་ལུ(i = 0; i < n; ++i){
            float bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    ལོག l;
}

layer parse_iseg(list *options, size_params params)
{
    ཧྲིལ classes = option_find_int(options, "classes", 20);
    ཧྲིལ ids = option_find_int(options, "ids", 32);
    layer l = make_iseg_layer(params.batch, params.w, params.h, classes, ids);
    assert(l.outputs == params.inputs);
    ལོག l;
}

layer parse_region(list *options, size_params params)
{
    ཧྲིལ coords = option_find_int(options, "coords", 4);
    ཧྲིལ classes = option_find_int(options, "classes", 20);
    ཧྲིལ num = option_find_int(options, "num", 1);

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
    གལ་སྲིད (tree_file) l.softmax_tree = read_tree(tree_file);
    var_na *map_file = option_find_str(options, "map", 0);
    གལ་སྲིད (map_file) l.map = read_map(map_file);

    var_na *a = option_find_str(options, "anchors", 0);
    གལ་སྲིད(a){
        ཧྲིལ len = strlen(a);
        ཧྲིལ n = 1;
        ཧྲིལ i;
        དོན་ལུ(i = 0; i < len; ++i){
            གལ་སྲིད (a[i] == ',') ++n;
        }
        དོན་ལུ(i = 0; i < n; ++i){
            float bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    ལོག l;
}

detection_layer parse_detection(list *options, size_params params)
{
    ཧྲིལ coords = option_find_int(options, "coords", 1);
    ཧྲིལ classes = option_find_int(options, "classes", 1);
    ཧྲིལ rescore = option_find_int(options, "rescore", 0);
    ཧྲིལ num = option_find_int(options, "num", 1);
    ཧྲིལ side = option_find_int(options, "side", 7);
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
    ལོག layer;
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
    ལོག layer;
}

crop_layer parse_crop(list *options, size_params params)
{
    ཧྲིལ crop_height = option_find_int(options, "crop_height",1);
    ཧྲིལ crop_width = option_find_int(options, "crop_width",1);
    ཧྲིལ flip = option_find_int(options, "flip",0);
    float angle = option_find_float(options, "angle",0);
    float saturation = option_find_float(options, "saturation",1);
    float exposure = option_find_float(options, "exposure",1);

    ཧྲིལ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    གལ་སྲིད(!(h && w && c)) error("Layer before crop layer must output image.");

    ཧྲིལ noadjust = option_find_int_quiet(options, "noadjust",0);

    crop_layer l = make_crop_layer(batch,h,w,c,crop_height,crop_width,flip, angle, saturation, exposure);
    l.shift = option_find_float(options, "shift", 0);
    l.noadjust = noadjust;
    ལོག l;
}

layer parse_reorg(list *options, size_params params)
{
    ཧྲིལ stride = option_find_int(options, "stride",1);
    ཧྲིལ reverse = option_find_int_quiet(options, "reverse",0);
    ཧྲིལ flatten = option_find_int_quiet(options, "flatten",0);
    ཧྲིལ extra = option_find_int_quiet(options, "extra",0);

    ཧྲིལ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    གལ་སྲིད(!(h && w && c)) error("Layer before reorg layer must output image.");

    layer layer = make_reorg_layer(batch,w,h,c,stride,reverse, flatten, extra);
    ལོག layer;
}

maxpool_layer parse_maxpool(list *options, size_params params)
{
    ཧྲིལ stride = option_find_int(options, "stride",1);
    ཧྲིལ size = option_find_int(options, "size",stride);
    ཧྲིལ padding = option_find_int_quiet(options, "padding", size-1);

    ཧྲིལ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    གལ་སྲིད(!(h && w && c)) error("Layer before maxpool layer must output image.");

    maxpool_layer layer = make_maxpool_layer(batch,h,w,c,size,stride,padding);
    ལོག layer;
}

avgpool_layer parse_avgpool(list *options, size_params params)
{
    ཧྲིལ batch,w,h,c;
    w = params.w;
    h = params.h;
    c = params.c;
    batch=params.batch;
    གལ་སྲིད(!(h && w && c)) error("Layer before avgpool layer must output image.");

    avgpool_layer layer = make_avgpool_layer(batch,w,h,c);
    ལོག layer;
}

dropout_layer parse_dropout(list *options, size_params params)
{
    float probability = option_find_float(options, "probability", .5);
    dropout_layer layer = make_dropout_layer(params.batch, params.inputs, probability);
    layer.out_w = params.w;
    layer.out_h = params.h;
    layer.out_c = params.c;
    ལོག layer;
}

layer parse_normalization(list *options, size_params params)
{
    float alpha = option_find_float(options, "alpha", .0001);
    float beta =  option_find_float(options, "beta" , .75);
    float kappa = option_find_float(options, "kappa", 1);
    ཧྲིལ size = option_find_int(options, "size", 5);
    layer l = make_normalization_layer(params.batch, params.w, params.h, params.c, size, alpha, beta, kappa);
    ལོག l;
}

layer parse_batchnorm(list *options, size_params params)
{
    layer l = make_batchnorm_layer(params.batch, params.w, params.h, params.c);
    ལོག l;
}

layer parse_shortcut(list *options, size_params params, network *net)
{
    var_na *l = option_find(options, "from");
    ཧྲིལ index = atoi(l);
    གལ་སྲིད(index < 0) index = params.index + index;

    ཧྲིལ batch = params.batch;
    layer from = net->layers[index];

    layer s = make_shortcut_layer(batch, index, params.w, params.h, params.c, from.out_w, from.out_h, from.out_c);

    var_na *activation_s = option_find_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);
    s.activation = activation;
    s.alpha = option_find_float_quiet(options, "alpha", 1);
    s.beta = option_find_float_quiet(options, "beta", 1);
    ལོག s;
}


layer parse_l2norm(list *options, size_params params)
{
    layer l = make_l2norm_layer(params.batch, params.inputs);
    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;
    ལོག l;
}


layer parse_logistic(list *options, size_params params)
{
    layer l = make_logistic_layer(params.batch, params.inputs);
    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;
    ལོག l;
}

layer parse_activation(list *options, size_params params)
{
    var_na *activation_s = option_find_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);

    layer l = make_activation_layer(params.batch, params.inputs, activation);

    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;

    ལོག l;
}

layer parse_upsample(list *options, size_params params, network *net)
{

    ཧྲིལ stride = option_find_int(options, "stride",2);
    layer l = make_upsample_layer(params.batch, params.w, params.h, params.c, stride);
    l.scale = option_find_float_quiet(options, "scale", 1);
    ལོག l;
}

route_layer parse_route(list *options, size_params params, network *net)
{
    var_na *l = option_find(options, "layers");
    ཧྲིལ len = strlen(l);
    གལ་སྲིད(!l) error("Route Layer must specify input layers");
    ཧྲིལ n = 1;
    ཧྲིལ i;
    དོན་ལུ(i = 0; i < len; ++i){
        གལ་སྲིད (l[i] == ',') ++n;
    }

    ཧྲིལ *layers = calloc(n, parimaa_na(ཧྲིལ));
    ཧྲིལ *sizes = calloc(n, parimaa_na(ཧྲིལ));
    དོན་ལུ(i = 0; i < n; ++i){
        ཧྲིལ index = atoi(l);
        l = strchr(l, ',')+1;
        གལ་སྲིད(index < 0) index = params.index + index;
        layers[i] = index;
        sizes[i] = net->layers[index].outputs;
    }
    ཧྲིལ batch = params.batch;

    route_layer layer = make_route_layer(batch, n, layers, sizes);

    convolutional_layer first = net->layers[layers[0]];
    layer.out_w = first.out_w;
    layer.out_h = first.out_h;
    layer.out_c = first.out_c;
    དོན་ལུ(i = 1; i < n; ++i){
        ཧྲིལ index = layers[i];
        convolutional_layer next = net->layers[index];
        གལ་སྲིད(next.out_w == first.out_w && next.out_h == first.out_h){
            layer.out_c += next.out_c;
        }དེ་མིན{
            layer.out_h = layer.out_w = layer.out_c = 0;
        }
    }

    ལོག layer;
}

learning_rate_policy get_policy(var_na *s)
{
    གལ་སྲིད (strcmp(s, "random")==0) ལོག RANDOM;
    གལ་སྲིད (strcmp(s, "poly")==0) ལོག POLY;
    གལ་སྲིད (strcmp(s, "constant")==0) ལོག CONSTANT;
    གལ་སྲིད (strcmp(s, "step")==0) ལོག STEP;
    གལ་སྲིད (strcmp(s, "exp")==0) ལོག EXP;
    གལ་སྲིད (strcmp(s, "sigmoid")==0) ལོག SIG;
    གལ་སྲིད (strcmp(s, "steps")==0) ལོག STEPS;
    fprintf(stderr, "Couldn't find policy %s, going with constant\n", s);
    ལོག CONSTANT;
}

void parse_net_options(list *options, network *net)
{
    net->batch = option_find_int(options, "batch",1);
    net->learning_rate = option_find_float(options, "learning_rate", .001);
    net->momentum = option_find_float(options, "momentum", .9);
    net->decay = option_find_float(options, "decay", .0001);
    ཧྲིལ subdivs = option_find_int(options, "subdivisions",1);
    net->time_steps = option_find_int_quiet(options, "time_steps",1);
    net->notruth = option_find_int_quiet(options, "notruth",0);
    net->batch /= subdivs;
    net->batch *= net->time_steps;
    net->subdivisions = subdivs;
    net->random = option_find_int_quiet(options, "random", 0);

    net->adam = option_find_int_quiet(options, "adam", 0);
    གལ་སྲིད(net->adam){
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

    གལ་སྲིད(!net->inputs && !(net->h && net->w && net->c)) error("No input parameters supplied");

    var_na *policy_s = option_find_str(options, "policy", "constant");
    net->policy = get_policy(policy_s);
    net->burn_in = option_find_int_quiet(options, "burn_in", 0);
    net->power = option_find_float_quiet(options, "power", 4);
    གལ་སྲིད(net->policy == STEP){
        net->step = option_find_int(options, "step", 1);
        net->scale = option_find_float(options, "scale", 1);
    } དེ་མིན གལ་སྲིད (net->policy == STEPS){
        var_na *l = option_find(options, "steps");
        var_na *p = option_find(options, "scales");
        གལ་སྲིད(!l || !p) error("STEPS policy must have steps and scales in cfg file");

        ཧྲིལ len = strlen(l);
        ཧྲིལ n = 1;
        ཧྲིལ i;
        དོན་ལུ(i = 0; i < len; ++i){
            གལ་སྲིད (l[i] == ',') ++n;
        }
        ཧྲིལ *steps = calloc(n, parimaa_na(ཧྲིལ));
        float *scales = calloc(n, parimaa_na(float));
        དོན་ལུ(i = 0; i < n; ++i){
            ཧྲིལ step    = atoi(l);
            float scale = atof(p);
            l = strchr(l, ',')+1;
            p = strchr(p, ',')+1;
            steps[i] = step;
            scales[i] = scale;
        }
        net->scales = scales;
        net->steps = steps;
        net->num_steps = n;
    } དེ་མིན གལ་སྲིད (net->policy == EXP){
        net->gamma = option_find_float(options, "gamma", 1);
    } དེ་མིན གལ་སྲིད (net->policy == SIG){
        net->gamma = option_find_float(options, "gamma", 1);
        net->step = option_find_int(options, "step", 1);
    } དེ་མིན གལ་སྲིད (net->policy == POLY || net->policy == RANDOM){
    }
    net->max_batches = option_find_int(options, "max_batches", 0);
}

ཧྲིལ is_network(section *s)
{
    ལོག (strcmp(s->type, "[net]")==0
            || strcmp(s->type, "[network]")==0);
}

network *parse_network_cfg(var_na *filename)
{
    list *sections = read_cfg(filename);
    node *n = sections->front;
    གལ་སྲིད(!n) error("Config file has no sections");
    network *net = make_network(sections->size - 1);
    net->gpu_index = gpu_index;
    size_params params;

    section *s = (section *)n->val;
    list *options = s->options;
    གལ་སྲིད(!is_network(s)) error("First section must be [net] or [network]");
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
    ཧྲིལ count = 0;
    free_section(s);
    fprintf(stderr, "layer     filters    size              input                output\n");
    སྐབས(n){
        params.index = count;
        fprintf(stderr, "%5d ", count);
        s = (section *)n->val;
        options = s->options;
        layer l = {0};
        LAYER_TYPE lt = string_to_layer_type(s->type);
        གལ་སྲིད(lt == CONVOLUTIONAL){
            l = parse_convolutional(options, params);
        }དེ་མིན གལ་སྲིད(lt == DECONVOLUTIONAL){
            l = parse_deconvolutional(options, params);
        }དེ་མིན གལ་སྲིད(lt == LOCAL){
            l = parse_local(options, params);
        }དེ་མིན གལ་སྲིད(lt == ACTIVE){
            l = parse_activation(options, params);
        }དེ་མིན གལ་སྲིད(lt == LOGXENT){
            l = parse_logistic(options, params);
        }དེ་མིན གལ་སྲིད(lt == L2NORM){
            l = parse_l2norm(options, params);
        }དེ་མིན གལ་སྲིད(lt == RNN){
            l = parse_rnn(options, params);
        }དེ་མིན གལ་སྲིད(lt == GRU){
            l = parse_gru(options, params);
        }དེ་མིན གལ་སྲིད (lt == LSTM) {
            l = parse_lstm(options, params);
        }དེ་མིན གལ་སྲིད(lt == CRNN){
            l = parse_crnn(options, params);
        }དེ་མིན གལ་སྲིད(lt == CONNECTED){
            l = parse_connected(options, params);
        }དེ་མིན གལ་སྲིད(lt == CROP){
            l = parse_crop(options, params);
        }དེ་མིན གལ་སྲིད(lt == COST){
            l = parse_cost(options, params);
        }དེ་མིན གལ་སྲིད(lt == REGION){
            l = parse_region(options, params);
        }དེ་མིན གལ་སྲིད(lt == YOLO){
            l = parse_yolo(options, params);
        }དེ་མིན གལ་སྲིད(lt == ISEG){
            l = parse_iseg(options, params);
        }དེ་མིན གལ་སྲིད(lt == DETECTION){
            l = parse_detection(options, params);
        }དེ་མིན གལ་སྲིད(lt == SOFTMAX){
            l = parse_softmax(options, params);
            net->hierarchy = l.softmax_tree;
        }དེ་མིན གལ་སྲིད(lt == NORMALIZATION){
            l = parse_normalization(options, params);
        }དེ་མིན གལ་སྲིད(lt == BATCHNORM){
            l = parse_batchnorm(options, params);
        }དེ་མིན གལ་སྲིད(lt == MAXPOOL){
            l = parse_maxpool(options, params);
        }དེ་མིན གལ་སྲིད(lt == REORG){
            l = parse_reorg(options, params);
        }དེ་མིན གལ་སྲིད(lt == AVGPOOL){
            l = parse_avgpool(options, params);
        }དེ་མིན གལ་སྲིད(lt == ROUTE){
            l = parse_route(options, params, net);
        }དེ་མིན གལ་སྲིད(lt == UPSAMPLE){
            l = parse_upsample(options, params, net);
        }དེ་མིན གལ་སྲིད(lt == SHORTCUT){
            l = parse_shortcut(options, params, net);
        }དེ་མིན གལ་སྲིད(lt == DROPOUT){
            l = parse_dropout(options, params);
            l.output = net->layers[count-1].output;
            l.delta = net->layers[count-1].delta;
#ifdef GPU
            l.output_gpu = net->layers[count-1].output_gpu;
            l.delta_gpu = net->layers[count-1].delta_gpu;
#endif
        }དེ་མིན{
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
        གལ་སྲིད (l.workspace_size > workspace_size) workspace_size = l.workspace_size;
        free_section(s);
        n = n->next;
        ++count;
        གལ་སྲིད(n){
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
    གལ་སྲིད(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    net->input = calloc(net->inputs*net->batch, parimaa_na(float));
    net->truth = calloc(net->truths*net->batch, parimaa_na(float));
#ifdef GPU
    net->output_gpu = out.output_gpu;
    net->input_gpu = cuda_make_array(net->input, net->inputs*net->batch);
    net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
#endif
    གལ་སྲིད(workspace_size){
        //printf("%ld\n", workspace_size);
#ifdef GPU
        གལ་སྲིད(gpu_index >= 0){
            net->workspace = cuda_make_array(0, (workspace_size-1)/parimaa_na(float)+1);
        }དེ་མིན {
            net->workspace = calloc(1, workspace_size);
        }
#དེ་མིན
        net->workspace = calloc(1, workspace_size);
#endif
    }
    ལོག net;
}

list *read_cfg(var_na *filename)
{
    FILE *file = fopen(filename, "r");
    གལ་སྲིད(file == 0) file_error(filename);
    var_na *line;
    ཧྲིལ nu = 0;
    list *options = make_list();
    section *current = 0;
    སྐབས((line=fgetl(file)) != 0){
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
                གལ་སྲིད(!read_option(line, current->options)){
                    fprintf(stderr, "Config file error line %d, could parse: %s\n", nu, line);
                    free(line);
                }
                bha_nga;
        }
    }
    fclose(file);
    ལོག options;
}

void save_convolutional_weights_binary(layer l, FILE *fp)
{
#ifdef GPU
    གལ་སྲིད(gpu_index >= 0){
        pull_convolutional_layer(l);
    }
#endif
    binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.binary_weights);
    ཧྲིལ size = l.c*l.size*l.size;
    ཧྲིལ i, j, k;
    fwrite(l.biases, parimaa_na(float), l.n, fp);
    གལ་སྲིད (l.batch_normalize){
        fwrite(l.scales, parimaa_na(float), l.n, fp);
        fwrite(l.rolling_mean, parimaa_na(float), l.n, fp);
        fwrite(l.rolling_variance, parimaa_na(float), l.n, fp);
    }
    དོན་ལུ(i = 0; i < l.n; ++i){
        float mean = l.binary_weights[i*size];
        གལ་སྲིད(mean < 0) mean = -mean;
        fwrite(&mean, parimaa_na(float), 1, fp);
        དོན་ལུ(j = 0; j < size/8; ++j){
            ཧྲིལ index = i*size + j*8;
            unsigned var_na c = 0;
            དོན་ལུ(k = 0; k < 8; ++k){
                གལ་སྲིད (j*8 + k >= size) bha_nga;
                གལ་སྲིད (l.binary_weights[index + k] > 0) c = (c | 1<<k);
            }
            fwrite(&c, parimaa_na(var_na), 1, fp);
        }
    }
}

void save_convolutional_weights(layer l, FILE *fp)
{
    གལ་སྲིད(l.binary){
        //save_convolutional_weights_binary(l, fp);
        //return;
    }
#ifdef GPU
    གལ་སྲིད(gpu_index >= 0){
        pull_convolutional_layer(l);
    }
#endif
    ཧྲིལ num = l.nweights;
    fwrite(l.biases, parimaa_na(float), l.n, fp);
    གལ་སྲིད (l.batch_normalize){
        fwrite(l.scales, parimaa_na(float), l.n, fp);
        fwrite(l.rolling_mean, parimaa_na(float), l.n, fp);
        fwrite(l.rolling_variance, parimaa_na(float), l.n, fp);
    }
    fwrite(l.weights, parimaa_na(float), num, fp);
}

void save_batchnorm_weights(layer l, FILE *fp)
{
#ifdef GPU
    གལ་སྲིད(gpu_index >= 0){
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
    གལ་སྲིད(gpu_index >= 0){
        pull_connected_layer(l);
    }
#endif
    fwrite(l.biases, parimaa_na(float), l.outputs, fp);
    fwrite(l.weights, parimaa_na(float), l.outputs*l.inputs, fp);
    གལ་སྲིད (l.batch_normalize){
        fwrite(l.scales, parimaa_na(float), l.outputs, fp);
        fwrite(l.rolling_mean, parimaa_na(float), l.outputs, fp);
        fwrite(l.rolling_variance, parimaa_na(float), l.outputs, fp);
    }
}

void save_weights_upto(network *net, var_na *filename, ཧྲིལ cutoff)
{
#ifdef GPU
    གལ་སྲིད(net->gpu_index >= 0){
        cuda_set_device(net->gpu_index);
    }
#endif
    fprintf(stderr, "Saving weights to %s\n", filename);
    FILE *fp = fopen(filename, "wb");
    གལ་སྲིད(!fp) file_error(filename);

    ཧྲིལ major = 0;
    ཧྲིལ minor = 2;
    ཧྲིལ revision = 0;
    fwrite(&major, parimaa_na(ཧྲིལ), 1, fp);
    fwrite(&minor, parimaa_na(ཧྲིལ), 1, fp);
    fwrite(&revision, parimaa_na(ཧྲིལ), 1, fp);
    fwrite(net->seen, parimaa_na(size_t), 1, fp);

    ཧྲིལ i;
    དོན་ལུ(i = 0; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        གལ་སྲིད (l.dontsave) continue;
        གལ་སྲིད(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            save_convolutional_weights(l, fp);
        } གལ་སྲིད(l.type == CONNECTED){
            save_connected_weights(l, fp);
        } གལ་སྲིད(l.type == BATCHNORM){
            save_batchnorm_weights(l, fp);
        } གལ་སྲིད(l.type == RNN){
            save_connected_weights(*(l.input_layer), fp);
            save_connected_weights(*(l.self_layer), fp);
            save_connected_weights(*(l.output_layer), fp);
        } གལ་སྲིད (l.type == LSTM) {
            save_connected_weights(*(l.wi), fp);
            save_connected_weights(*(l.wf), fp);
            save_connected_weights(*(l.wo), fp);
            save_connected_weights(*(l.wg), fp);
            save_connected_weights(*(l.ui), fp);
            save_connected_weights(*(l.uf), fp);
            save_connected_weights(*(l.uo), fp);
            save_connected_weights(*(l.ug), fp);
        } གལ་སྲིད (l.type == GRU) {
            གལ་སྲིད(1){
                save_connected_weights(*(l.wz), fp);
                save_connected_weights(*(l.wr), fp);
                save_connected_weights(*(l.wh), fp);
                save_connected_weights(*(l.uz), fp);
                save_connected_weights(*(l.ur), fp);
                save_connected_weights(*(l.uh), fp);
            }དེ་མིན{
                save_connected_weights(*(l.reset_layer), fp);
                save_connected_weights(*(l.update_layer), fp);
                save_connected_weights(*(l.state_layer), fp);
            }
        }  གལ་སྲིད(l.type == CRNN){
            save_convolutional_weights(*(l.input_layer), fp);
            save_convolutional_weights(*(l.self_layer), fp);
            save_convolutional_weights(*(l.output_layer), fp);
        } གལ་སྲིད(l.type == LOCAL){
#ifdef GPU
            གལ་སྲིད(gpu_index >= 0){
                pull_local_layer(l);
            }
#endif
            ཧྲིལ locations = l.out_w*l.out_h;
            ཧྲིལ size = l.size*l.size*l.c*l.n*locations;
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

void transpose_matrix(float *a, ཧྲིལ rows, ཧྲིལ cols)
{
    float *transpose = calloc(rows*cols, parimaa_na(float));
    ཧྲིལ x, y;
    དོན་ལུ(x = 0; x < rows; ++x){
        དོན་ལུ(y = 0; y < cols; ++y){
            transpose[y*rows + x] = a[x*cols + y];
        }
    }
    memcpy(a, transpose, rows*cols*parimaa_na(float));
    free(transpose);
}

void load_connected_weights(layer l, FILE *fp, ཧྲིལ transpose)
{
    fread(l.biases, parimaa_na(float), l.outputs, fp);
    fread(l.weights, parimaa_na(float), l.outputs*l.inputs, fp);
    གལ་སྲིད(transpose){
        transpose_matrix(l.weights, l.inputs, l.outputs);
    }
    //printf("Biases: %f mean %f variance\n", mean_array(l.biases, l.outputs), variance_array(l.biases, l.outputs));
    //printf("Weights: %f mean %f variance\n", mean_array(l.weights, l.outputs*l.inputs), variance_array(l.weights, l.outputs*l.inputs));
    གལ་སྲིད (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, parimaa_na(float), l.outputs, fp);
        fread(l.rolling_mean, parimaa_na(float), l.outputs, fp);
        fread(l.rolling_variance, parimaa_na(float), l.outputs, fp);
        //printf("Scales: %f mean %f variance\n", mean_array(l.scales, l.outputs), variance_array(l.scales, l.outputs));
        //printf("rolling_mean: %f mean %f variance\n", mean_array(l.rolling_mean, l.outputs), variance_array(l.rolling_mean, l.outputs));
        //printf("rolling_variance: %f mean %f variance\n", mean_array(l.rolling_variance, l.outputs), variance_array(l.rolling_variance, l.outputs));
    }
#ifdef GPU
    གལ་སྲིད(gpu_index >= 0){
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
    གལ་སྲིད(gpu_index >= 0){
        push_batchnorm_layer(l);
    }
#endif
}

void load_convolutional_weights_binary(layer l, FILE *fp)
{
    fread(l.biases, parimaa_na(float), l.n, fp);
    གལ་སྲིད (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, parimaa_na(float), l.n, fp);
        fread(l.rolling_mean, parimaa_na(float), l.n, fp);
        fread(l.rolling_variance, parimaa_na(float), l.n, fp);
    }
    ཧྲིལ size = l.c*l.size*l.size;
    ཧྲིལ i, j, k;
    དོན་ལུ(i = 0; i < l.n; ++i){
        float mean = 0;
        fread(&mean, parimaa_na(float), 1, fp);
        དོན་ལུ(j = 0; j < size/8; ++j){
            ཧྲིལ index = i*size + j*8;
            unsigned var_na c = 0;
            fread(&c, parimaa_na(var_na), 1, fp);
            དོན་ལུ(k = 0; k < 8; ++k){
                གལ་སྲིད (j*8 + k >= size) bha_nga;
                l.weights[index + k] = (c & 1<<k) ? mean : -mean;
            }
        }
    }
#ifdef GPU
    གལ་སྲིད(gpu_index >= 0){
        push_convolutional_layer(l);
    }
#endif
}

void load_convolutional_weights(layer l, FILE *fp)
{
    གལ་སྲིད(l.binary){
        //load_convolutional_weights_binary(l, fp);
        //return;
    }
    གལ་སྲིད(l.numload) l.n = l.numload;
    ཧྲིལ num = l.c/l.groups*l.n*l.size*l.size;
    fread(l.biases, parimaa_na(float), l.n, fp);
    གལ་སྲིད (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, parimaa_na(float), l.n, fp);
        fread(l.rolling_mean, parimaa_na(float), l.n, fp);
        fread(l.rolling_variance, parimaa_na(float), l.n, fp);
        གལ་སྲིད(0){
            ཧྲིལ i;
            དོན་ལུ(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_mean[i]);
            }
            printf("\n");
            དོན་ལུ(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_variance[i]);
            }
            printf("\n");
        }
        གལ་སྲིད(0){
            fill_cpu(l.n, 0, l.rolling_mean, 1);
            fill_cpu(l.n, 0, l.rolling_variance, 1);
        }
        གལ་སྲིད(0){
            ཧྲིལ i;
            དོན་ལུ(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_mean[i]);
            }
            printf("\n");
            དོན་ལུ(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_variance[i]);
            }
            printf("\n");
        }
    }
    fread(l.weights, parimaa_na(float), num, fp);
    //if(l.c == 3) scal_cpu(num, 1./256, l.weights, 1);
    གལ་སྲིད (l.flipped) {
        transpose_matrix(l.weights, l.c*l.size*l.size, l.n);
    }
    //if (l.binary) binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.weights);
#ifdef GPU
    གལ་སྲིད(gpu_index >= 0){
        push_convolutional_layer(l);
    }
#endif
}


void load_weights_upto(network *net, var_na *filename, ཧྲིལ start, ཧྲིལ cutoff)
{
#ifdef GPU
    གལ་སྲིད(net->gpu_index >= 0){
        cuda_set_device(net->gpu_index);
    }
#endif
    fprintf(stderr, "Loading weights from %s...", filename);
    fflush(stdout);
    FILE *fp = fopen(filename, "rb");
    གལ་སྲིད(!fp) file_error(filename);

    ཧྲིལ major;
    ཧྲིལ minor;
    ཧྲིལ revision;
    fread(&major, parimaa_na(ཧྲིལ), 1, fp);
    fread(&minor, parimaa_na(ཧྲིལ), 1, fp);
    fread(&revision, parimaa_na(ཧྲིལ), 1, fp);
    གལ་སྲིད ((major*10 + minor) >= 2 && major < 1000 && minor < 1000){
        fread(net->seen, parimaa_na(size_t), 1, fp);
    } དེ་མིན {
        ཧྲིལ iseen = 0;
        fread(&iseen, parimaa_na(ཧྲིལ), 1, fp);
        *net->seen = iseen;
    }
    ཧྲིལ transpose = (major > 1000) || (minor > 1000);

    ཧྲིལ i;
    དོན་ལུ(i = start; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        གལ་སྲིད (l.dontload) continue;
        གལ་སྲིད(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            load_convolutional_weights(l, fp);
        }
        གལ་སྲིད(l.type == CONNECTED){
            load_connected_weights(l, fp, transpose);
        }
        གལ་སྲིད(l.type == BATCHNORM){
            load_batchnorm_weights(l, fp);
        }
        གལ་སྲིད(l.type == CRNN){
            load_convolutional_weights(*(l.input_layer), fp);
            load_convolutional_weights(*(l.self_layer), fp);
            load_convolutional_weights(*(l.output_layer), fp);
        }
        གལ་སྲིད(l.type == RNN){
            load_connected_weights(*(l.input_layer), fp, transpose);
            load_connected_weights(*(l.self_layer), fp, transpose);
            load_connected_weights(*(l.output_layer), fp, transpose);
        }
        གལ་སྲིད (l.type == LSTM) {
            load_connected_weights(*(l.wi), fp, transpose);
            load_connected_weights(*(l.wf), fp, transpose);
            load_connected_weights(*(l.wo), fp, transpose);
            load_connected_weights(*(l.wg), fp, transpose);
            load_connected_weights(*(l.ui), fp, transpose);
            load_connected_weights(*(l.uf), fp, transpose);
            load_connected_weights(*(l.uo), fp, transpose);
            load_connected_weights(*(l.ug), fp, transpose);
        }
        གལ་སྲིད (l.type == GRU) {
            གལ་སྲིད(1){
                load_connected_weights(*(l.wz), fp, transpose);
                load_connected_weights(*(l.wr), fp, transpose);
                load_connected_weights(*(l.wh), fp, transpose);
                load_connected_weights(*(l.uz), fp, transpose);
                load_connected_weights(*(l.ur), fp, transpose);
                load_connected_weights(*(l.uh), fp, transpose);
            }དེ་མིན{
                load_connected_weights(*(l.reset_layer), fp, transpose);
                load_connected_weights(*(l.update_layer), fp, transpose);
                load_connected_weights(*(l.state_layer), fp, transpose);
            }
        }
        གལ་སྲིད(l.type == LOCAL){
            ཧྲིལ locations = l.out_w*l.out_h;
            ཧྲིལ size = l.size*l.size*l.c*l.n*locations;
            fread(l.biases, parimaa_na(float), l.outputs, fp);
            fread(l.weights, parimaa_na(float), size, fp);
#ifdef GPU
            གལ་སྲིད(gpu_index >= 0){
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


#ضمن <stdio.h>
#ضمن <string.h>
#ضمن <stdlib.h>
#ضمن <assert.h>

#ضمن "activation_layer.h"
#ضمن "logistic_layer.h"
#ضمن "l2norm_layer.h"
#ضمن "activations.h"
#ضمن "avgpool_layer.h"
#ضمن "batchnorm_layer.h"
#ضمن "blas.h"
#ضمن "connected_layer.h"
#ضمن "deconvolutional_layer.h"
#ضمن "convolutional_layer.h"
#ضمن "cost_layer.h"
#ضمن "crnn_layer.h"
#ضمن "crop_layer.h"
#ضمن "detection_layer.h"
#ضمن "dropout_layer.h"
#ضمن "gru_layer.h"
#ضمن "list.h"
#ضمن "local_layer.h"
#ضمن "maxpool_layer.h"
#ضمن "normalization_layer.h"
#ضمن "option_list.h"
#ضمن "parser.h"
#ضمن "region_layer.h"
#ضمن "yolo_layer.h"
#ضمن "iseg_layer.h"
#ضمن "reorg_layer.h"
#ضمن "rnn_layer.h"
#ضمن "route_layer.h"
#ضمن "upsample_layer.h"
#ضمن "shortcut_layer.h"
#ضمن "softmax_layer.h"
#ضمن "lstm_layer.h"
#ضمن "utils.h"

تعريفنوع بنية{
    حرف *type;
    list *options;
}section;

list *read_cfg(حرف *filename);

LAYER_TYPE string_to_layer_type(حرف * type)
{

    𐤀𐤌 (strcmp(type, "[shortcut]")==0) أرجع SHORTCUT;
    𐤀𐤌 (strcmp(type, "[crop]")==0) أرجع CROP;
    𐤀𐤌 (strcmp(type, "[cost]")==0) أرجع COST;
    𐤀𐤌 (strcmp(type, "[detection]")==0) أرجع DETECTION;
    𐤀𐤌 (strcmp(type, "[region]")==0) أرجع REGION;
    𐤀𐤌 (strcmp(type, "[yolo]")==0) أرجع YOLO;
    𐤀𐤌 (strcmp(type, "[iseg]")==0) أرجع ISEG;
    𐤀𐤌 (strcmp(type, "[local]")==0) أرجع LOCAL;
    𐤀𐤌 (strcmp(type, "[conv]")==0
            || strcmp(type, "[convolutional]")==0) أرجع CONVOLUTIONAL;
    𐤀𐤌 (strcmp(type, "[deconv]")==0
            || strcmp(type, "[deconvolutional]")==0) أرجع DECONVOLUTIONAL;
    𐤀𐤌 (strcmp(type, "[activation]")==0) أرجع ACTIVE;
    𐤀𐤌 (strcmp(type, "[logistic]")==0) أرجع LOGXENT;
    𐤀𐤌 (strcmp(type, "[l2norm]")==0) أرجع L2NORM;
    𐤀𐤌 (strcmp(type, "[net]")==0
            || strcmp(type, "[network]")==0) أرجع NETWORK;
    𐤀𐤌 (strcmp(type, "[crnn]")==0) أرجع CRNN;
    𐤀𐤌 (strcmp(type, "[gru]")==0) أرجع GRU;
    𐤀𐤌 (strcmp(type, "[lstm]") == 0) أرجع LSTM;
    𐤀𐤌 (strcmp(type, "[rnn]")==0) أرجع RNN;
    𐤀𐤌 (strcmp(type, "[conn]")==0
            || strcmp(type, "[connected]")==0) أرجع CONNECTED;
    𐤀𐤌 (strcmp(type, "[max]")==0
            || strcmp(type, "[maxpool]")==0) أرجع MAXPOOL;
    𐤀𐤌 (strcmp(type, "[reorg]")==0) أرجع REORG;
    𐤀𐤌 (strcmp(type, "[avg]")==0
            || strcmp(type, "[avgpool]")==0) أرجع AVGPOOL;
    𐤀𐤌 (strcmp(type, "[dropout]")==0) أرجع DROPOUT;
    𐤀𐤌 (strcmp(type, "[lrn]")==0
            || strcmp(type, "[normalization]")==0) أرجع NORMALIZATION;
    𐤀𐤌 (strcmp(type, "[batchnorm]")==0) أرجع BATCHNORM;
    𐤀𐤌 (strcmp(type, "[soft]")==0
            || strcmp(type, "[softmax]")==0) أرجع SOFTMAX;
    𐤀𐤌 (strcmp(type, "[route]")==0) أرجع ROUTE;
    𐤀𐤌 (strcmp(type, "[upsample]")==0) أرجع UPSAMPLE;
    أرجع BLANK;
}

فراغ free_section(section *s)
{
    حرر(s->type);
    node *n = s->options->front;
    طالما(n){
        kvp *pair = (kvp *)n->val;
        حرر(pair->key);
        حرر(pair);
        node *next = n->next;
        حرر(n);
        n = next;
    }
    حرر(s->options);
    حرر(s);
}

فراغ parse_data(حرف *data, عائم *a, صحيح n)
{
    صحيح i;
    𐤀𐤌(!data) أرجع;
    حرف *curr = data;
    حرف *next = data;
    صحيح done = 0;
    لكل(i = 0; i < n && !done; ++i){
        طالما(*++next !='\0' && *next != ',');
        𐤀𐤌(*next == '\0') done = 1;
        *next = '\0';
        sscanf(curr, "%g", &a[i]);
        curr = next+1;
    }
}

تعريفنوع بنية size_params{
    صحيح batch;
    صحيح inputs;
    صحيح h;
    صحيح w;
    صحيح c;
    صحيح index;
    صحيح time_steps;
    network *net;
} size_params;

local_layer parse_local(list *options, size_params params)
{
    صحيح n = option_find_int(options, "filters",1);
    صحيح size = option_find_int(options, "size",1);
    صحيح stride = option_find_int(options, "stride",1);
    صحيح pad = option_find_int(options, "pad",0);
    حرف *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    صحيح batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    𐤀𐤌(!(h && w && c)) error("Layer before local layer must output image.");

    local_layer layer = make_local_layer(batch,h,w,c,n,size,stride,pad,activation);

    أرجع layer;
}

layer parse_deconvolutional(list *options, size_params params)
{
    صحيح n = option_find_int(options, "filters",1);
    صحيح size = option_find_int(options, "size",1);
    صحيح stride = option_find_int(options, "stride",1);

    حرف *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    صحيح batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    𐤀𐤌(!(h && w && c)) error("Layer before deconvolutional layer must output image.");
    صحيح batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);
    صحيح pad = option_find_int_quiet(options, "pad",0);
    صحيح padding = option_find_int_quiet(options, "padding",0);
    𐤀𐤌(pad) padding = size/2;

    layer l = make_deconvolutional_layer(batch,h,w,c,n,size,stride,padding, activation, batch_normalize, params.net->adam);

    أرجع l;
}


convolutional_layer parse_convolutional(list *options, size_params params)
{
    صحيح n = option_find_int(options, "filters",1);
    صحيح size = option_find_int(options, "size",1);
    صحيح stride = option_find_int(options, "stride",1);
    صحيح pad = option_find_int_quiet(options, "pad",0);
    صحيح padding = option_find_int_quiet(options, "padding",0);
    صحيح groups = option_find_int_quiet(options, "groups", 1);
    𐤀𐤌(pad) padding = size/2;

    حرف *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    صحيح batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    𐤀𐤌(!(h && w && c)) error("Layer before convolutional layer must output image.");
    صحيح batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);
    صحيح binary = option_find_int_quiet(options, "binary", 0);
    صحيح xnor = option_find_int_quiet(options, "xnor", 0);

    convolutional_layer layer = make_convolutional_layer(batch,h,w,c,n,groups,size,stride,padding,activation, batch_normalize, binary, xnor, params.net->adam);
    layer.flipped = option_find_int_quiet(options, "flipped", 0);
    layer.dot = option_find_float_quiet(options, "dot", 0);

    أرجع layer;
}

layer parse_crnn(list *options, size_params params)
{
    صحيح output_filters = option_find_int(options, "output_filters",1);
    صحيح hidden_filters = option_find_int(options, "hidden_filters",1);
    حرف *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    صحيح batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_crnn_layer(params.batch, params.w, params.h, params.c, hidden_filters, output_filters, params.time_steps, activation, batch_normalize);

    l.shortcut = option_find_int_quiet(options, "shortcut", 0);

    أرجع l;
}

layer parse_rnn(list *options, size_params params)
{
    صحيح output = option_find_int(options, "output",1);
    حرف *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    صحيح batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_rnn_layer(params.batch, params.inputs, output, params.time_steps, activation, batch_normalize, params.net->adam);

    l.shortcut = option_find_int_quiet(options, "shortcut", 0);

    أرجع l;
}

layer parse_gru(list *options, size_params params)
{
    صحيح output = option_find_int(options, "output",1);
    صحيح batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_gru_layer(params.batch, params.inputs, output, params.time_steps, batch_normalize, params.net->adam);
    l.tanh = option_find_int_quiet(options, "tanh", 0);

    أرجع l;
}

layer parse_lstm(list *options, size_params params)
{
    صحيح output = option_find_int(options, "output", 1);
    صحيح batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_lstm_layer(params.batch, params.inputs, output, params.time_steps, batch_normalize, params.net->adam);

    أرجع l;
}

layer parse_connected(list *options, size_params params)
{
    صحيح output = option_find_int(options, "output",1);
    حرف *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    صحيح batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_connected_layer(params.batch, params.inputs, output, activation, batch_normalize, params.net->adam);
    أرجع l;
}

layer parse_softmax(list *options, size_params params)
{
    صحيح groups = option_find_int_quiet(options, "groups",1);
    layer l = make_softmax_layer(params.batch, params.inputs, groups);
    l.temperature = option_find_float_quiet(options, "temperature", 1);
    حرف *tree_file = option_find_str(options, "tree", 0);
    𐤀𐤌 (tree_file) l.softmax_tree = read_tree(tree_file);
    l.w = params.w;
    l.h = params.h;
    l.c = params.c;
    l.spatial = option_find_float_quiet(options, "spatial", 0);
    l.noloss =  option_find_int_quiet(options, "noloss", 0);
    أرجع l;
}

صحيح *parse_yolo_mask(حرف *a, صحيح *num)
{
    صحيح *mask = 0;
    𐤀𐤌(a){
        صحيح len = strlen(a);
        صحيح n = 1;
        صحيح i;
        لكل(i = 0; i < len; ++i){
            𐤀𐤌 (a[i] == ',') ++n;
        }
        mask = calloc(n, حجم(صحيح));
        لكل(i = 0; i < n; ++i){
            صحيح val = atoi(a);
            mask[i] = val;
            a = strchr(a, ',')+1;
        }
        *num = n;
    }
    أرجع mask;
}

layer parse_yolo(list *options, size_params params)
{
    صحيح classes = option_find_int(options, "classes", 20);
    صحيح total = option_find_int(options, "num", 1);
    صحيح num = total;

    حرف *a = option_find_str(options, "mask", 0);
    صحيح *mask = parse_yolo_mask(a, &num);
    layer l = make_yolo_layer(params.batch, params.w, params.h, num, total, mask, classes);
    أكد(l.outputs == params.inputs);

    l.max_boxes = option_find_int_quiet(options, "max",90);
    l.jitter = option_find_float(options, "jitter", .2);

    l.ignore_thresh = option_find_float(options, "ignore_thresh", .5);
    l.truth_thresh = option_find_float(options, "truth_thresh", 1);
    l.random = option_find_int_quiet(options, "random", 0);

    حرف *map_file = option_find_str(options, "map", 0);
    𐤀𐤌 (map_file) l.map = read_map(map_file);

    a = option_find_str(options, "anchors", 0);
    𐤀𐤌(a){
        صحيح len = strlen(a);
        صحيح n = 1;
        صحيح i;
        لكل(i = 0; i < len; ++i){
            𐤀𐤌 (a[i] == ',') ++n;
        }
        لكل(i = 0; i < n; ++i){
            عائم bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    أرجع l;
}

layer parse_iseg(list *options, size_params params)
{
    صحيح classes = option_find_int(options, "classes", 20);
    صحيح ids = option_find_int(options, "ids", 32);
    layer l = make_iseg_layer(params.batch, params.w, params.h, classes, ids);
    أكد(l.outputs == params.inputs);
    أرجع l;
}

layer parse_region(list *options, size_params params)
{
    صحيح coords = option_find_int(options, "coords", 4);
    صحيح classes = option_find_int(options, "classes", 20);
    صحيح num = option_find_int(options, "num", 1);

    layer l = make_region_layer(params.batch, params.w, params.h, num, classes, coords);
    أكد(l.outputs == params.inputs);

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

    حرف *tree_file = option_find_str(options, "tree", 0);
    𐤀𐤌 (tree_file) l.softmax_tree = read_tree(tree_file);
    حرف *map_file = option_find_str(options, "map", 0);
    𐤀𐤌 (map_file) l.map = read_map(map_file);

    حرف *a = option_find_str(options, "anchors", 0);
    𐤀𐤌(a){
        صحيح len = strlen(a);
        صحيح n = 1;
        صحيح i;
        لكل(i = 0; i < len; ++i){
            𐤀𐤌 (a[i] == ',') ++n;
        }
        لكل(i = 0; i < n; ++i){
            عائم bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    أرجع l;
}

detection_layer parse_detection(list *options, size_params params)
{
    صحيح coords = option_find_int(options, "coords", 1);
    صحيح classes = option_find_int(options, "classes", 1);
    صحيح rescore = option_find_int(options, "rescore", 0);
    صحيح num = option_find_int(options, "num", 1);
    صحيح side = option_find_int(options, "side", 7);
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
    أرجع layer;
}

cost_layer parse_cost(list *options, size_params params)
{
    حرف *type_s = option_find_str(options, "type", "sse");
    COST_TYPE type = get_cost_type(type_s);
    عائم scale = option_find_float_quiet(options, "scale",1);
    cost_layer layer = make_cost_layer(params.batch, params.inputs, type, scale);
    layer.ratio =  option_find_float_quiet(options, "ratio",0);
    layer.noobject_scale =  option_find_float_quiet(options, "noobj", 1);
    layer.thresh =  option_find_float_quiet(options, "thresh",0);
    أرجع layer;
}

crop_layer parse_crop(list *options, size_params params)
{
    صحيح crop_height = option_find_int(options, "crop_height",1);
    صحيح crop_width = option_find_int(options, "crop_width",1);
    صحيح flip = option_find_int(options, "flip",0);
    عائم angle = option_find_float(options, "angle",0);
    عائم saturation = option_find_float(options, "saturation",1);
    عائم exposure = option_find_float(options, "exposure",1);

    صحيح batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    𐤀𐤌(!(h && w && c)) error("Layer before crop layer must output image.");

    صحيح noadjust = option_find_int_quiet(options, "noadjust",0);

    crop_layer l = make_crop_layer(batch,h,w,c,crop_height,crop_width,flip, angle, saturation, exposure);
    l.shift = option_find_float(options, "shift", 0);
    l.noadjust = noadjust;
    أرجع l;
}

layer parse_reorg(list *options, size_params params)
{
    صحيح stride = option_find_int(options, "stride",1);
    صحيح reverse = option_find_int_quiet(options, "reverse",0);
    صحيح flatten = option_find_int_quiet(options, "flatten",0);
    صحيح extra = option_find_int_quiet(options, "extra",0);

    صحيح batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    𐤀𐤌(!(h && w && c)) error("Layer before reorg layer must output image.");

    layer layer = make_reorg_layer(batch,w,h,c,stride,reverse, flatten, extra);
    أرجع layer;
}

maxpool_layer parse_maxpool(list *options, size_params params)
{
    صحيح stride = option_find_int(options, "stride",1);
    صحيح size = option_find_int(options, "size",stride);
    صحيح padding = option_find_int_quiet(options, "padding", size-1);

    صحيح batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    𐤀𐤌(!(h && w && c)) error("Layer before maxpool layer must output image.");

    maxpool_layer layer = make_maxpool_layer(batch,h,w,c,size,stride,padding);
    أرجع layer;
}

avgpool_layer parse_avgpool(list *options, size_params params)
{
    صحيح batch,w,h,c;
    w = params.w;
    h = params.h;
    c = params.c;
    batch=params.batch;
    𐤀𐤌(!(h && w && c)) error("Layer before avgpool layer must output image.");

    avgpool_layer layer = make_avgpool_layer(batch,w,h,c);
    أرجع layer;
}

dropout_layer parse_dropout(list *options, size_params params)
{
    عائم probability = option_find_float(options, "probability", .5);
    dropout_layer layer = make_dropout_layer(params.batch, params.inputs, probability);
    layer.out_w = params.w;
    layer.out_h = params.h;
    layer.out_c = params.c;
    أرجع layer;
}

layer parse_normalization(list *options, size_params params)
{
    عائم alpha = option_find_float(options, "alpha", .0001);
    عائم beta =  option_find_float(options, "beta" , .75);
    عائم kappa = option_find_float(options, "kappa", 1);
    صحيح size = option_find_int(options, "size", 5);
    layer l = make_normalization_layer(params.batch, params.w, params.h, params.c, size, alpha, beta, kappa);
    أرجع l;
}

layer parse_batchnorm(list *options, size_params params)
{
    layer l = make_batchnorm_layer(params.batch, params.w, params.h, params.c);
    أرجع l;
}

layer parse_shortcut(list *options, size_params params, network *net)
{
    حرف *l = option_find(options, "from");
    صحيح index = atoi(l);
    𐤀𐤌(index < 0) index = params.index + index;

    صحيح batch = params.batch;
    layer from = net->layers[index];

    layer s = make_shortcut_layer(batch, index, params.w, params.h, params.c, from.out_w, from.out_h, from.out_c);

    حرف *activation_s = option_find_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);
    s.activation = activation;
    s.alpha = option_find_float_quiet(options, "alpha", 1);
    s.beta = option_find_float_quiet(options, "beta", 1);
    أرجع s;
}


layer parse_l2norm(list *options, size_params params)
{
    layer l = make_l2norm_layer(params.batch, params.inputs);
    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;
    أرجع l;
}


layer parse_logistic(list *options, size_params params)
{
    layer l = make_logistic_layer(params.batch, params.inputs);
    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;
    أرجع l;
}

layer parse_activation(list *options, size_params params)
{
    حرف *activation_s = option_find_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);

    layer l = make_activation_layer(params.batch, params.inputs, activation);

    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;

    أرجع l;
}

layer parse_upsample(list *options, size_params params, network *net)
{

    صحيح stride = option_find_int(options, "stride",2);
    layer l = make_upsample_layer(params.batch, params.w, params.h, params.c, stride);
    l.scale = option_find_float_quiet(options, "scale", 1);
    أرجع l;
}

route_layer parse_route(list *options, size_params params, network *net)
{
    حرف *l = option_find(options, "layers");
    صحيح len = strlen(l);
    𐤀𐤌(!l) error("Route Layer must specify input layers");
    صحيح n = 1;
    صحيح i;
    لكل(i = 0; i < len; ++i){
        𐤀𐤌 (l[i] == ',') ++n;
    }

    صحيح *layers = calloc(n, حجم(صحيح));
    صحيح *sizes = calloc(n, حجم(صحيح));
    لكل(i = 0; i < n; ++i){
        صحيح index = atoi(l);
        l = strchr(l, ',')+1;
        𐤀𐤌(index < 0) index = params.index + index;
        layers[i] = index;
        sizes[i] = net->layers[index].outputs;
    }
    صحيح batch = params.batch;

    route_layer layer = make_route_layer(batch, n, layers, sizes);

    convolutional_layer first = net->layers[layers[0]];
    layer.out_w = first.out_w;
    layer.out_h = first.out_h;
    layer.out_c = first.out_c;
    لكل(i = 1; i < n; ++i){
        صحيح index = layers[i];
        convolutional_layer next = net->layers[index];
        𐤀𐤌(next.out_w == first.out_w && next.out_h == first.out_h){
            layer.out_c += next.out_c;
        }وإلا{
            layer.out_h = layer.out_w = layer.out_c = 0;
        }
    }

    أرجع layer;
}

learning_rate_policy get_policy(حرف *s)
{
    𐤀𐤌 (strcmp(s, "random")==0) أرجع RANDOM;
    𐤀𐤌 (strcmp(s, "poly")==0) أرجع POLY;
    𐤀𐤌 (strcmp(s, "constant")==0) أرجع CONSTANT;
    𐤀𐤌 (strcmp(s, "step")==0) أرجع STEP;
    𐤀𐤌 (strcmp(s, "exp")==0) أرجع EXP;
    𐤀𐤌 (strcmp(s, "sigmoid")==0) أرجع SIG;
    𐤀𐤌 (strcmp(s, "steps")==0) أرجع STEPS;
    fprintf(stderr, "Couldn't find policy %s, going with constant\n", s);
    أرجع CONSTANT;
}

فراغ parse_net_options(list *options, network *net)
{
    net->batch = option_find_int(options, "batch",1);
    net->learning_rate = option_find_float(options, "learning_rate", .001);
    net->momentum = option_find_float(options, "momentum", .9);
    net->decay = option_find_float(options, "decay", .0001);
    صحيح subdivs = option_find_int(options, "subdivisions",1);
    net->time_steps = option_find_int_quiet(options, "time_steps",1);
    net->notruth = option_find_int_quiet(options, "notruth",0);
    net->batch /= subdivs;
    net->batch *= net->time_steps;
    net->subdivisions = subdivs;
    net->random = option_find_int_quiet(options, "random", 0);

    net->adam = option_find_int_quiet(options, "adam", 0);
    𐤀𐤌(net->adam){
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
    net->max_ratio = option_find_float_quiet(options, "max_ratio", (عائم) net->max_crop / net->w);
    net->min_ratio = option_find_float_quiet(options, "min_ratio", (عائم) net->min_crop / net->w);
    net->center = option_find_int_quiet(options, "center",0);
    net->clip = option_find_float_quiet(options, "clip", 0);

    net->angle = option_find_float_quiet(options, "angle", 0);
    net->aspect = option_find_float_quiet(options, "aspect", 1);
    net->saturation = option_find_float_quiet(options, "saturation", 1);
    net->exposure = option_find_float_quiet(options, "exposure", 1);
    net->hue = option_find_float_quiet(options, "hue", 0);

    𐤀𐤌(!net->inputs && !(net->h && net->w && net->c)) error("No input parameters supplied");

    حرف *policy_s = option_find_str(options, "policy", "constant");
    net->policy = get_policy(policy_s);
    net->burn_in = option_find_int_quiet(options, "burn_in", 0);
    net->power = option_find_float_quiet(options, "power", 4);
    𐤀𐤌(net->policy == STEP){
        net->step = option_find_int(options, "step", 1);
        net->scale = option_find_float(options, "scale", 1);
    } وإلا 𐤀𐤌 (net->policy == STEPS){
        حرف *l = option_find(options, "steps");
        حرف *p = option_find(options, "scales");
        𐤀𐤌(!l || !p) error("STEPS policy must have steps and scales in cfg file");

        صحيح len = strlen(l);
        صحيح n = 1;
        صحيح i;
        لكل(i = 0; i < len; ++i){
            𐤀𐤌 (l[i] == ',') ++n;
        }
        صحيح *steps = calloc(n, حجم(صحيح));
        عائم *scales = calloc(n, حجم(عائم));
        لكل(i = 0; i < n; ++i){
            صحيح step    = atoi(l);
            عائم scale = atof(p);
            l = strchr(l, ',')+1;
            p = strchr(p, ',')+1;
            steps[i] = step;
            scales[i] = scale;
        }
        net->scales = scales;
        net->steps = steps;
        net->num_steps = n;
    } وإلا 𐤀𐤌 (net->policy == EXP){
        net->gamma = option_find_float(options, "gamma", 1);
    } وإلا 𐤀𐤌 (net->policy == SIG){
        net->gamma = option_find_float(options, "gamma", 1);
        net->step = option_find_int(options, "step", 1);
    } وإلا 𐤀𐤌 (net->policy == POLY || net->policy == RANDOM){
    }
    net->max_batches = option_find_int(options, "max_batches", 0);
}

صحيح is_network(section *s)
{
    أرجع (strcmp(s->type, "[net]")==0
            || strcmp(s->type, "[network]")==0);
}

network *parse_network_cfg(حرف *filename)
{
    list *sections = read_cfg(filename);
    node *n = sections->front;
    𐤀𐤌(!n) error("Config file has no sections");
    network *net = make_network(sections->size - 1);
    net->gpu_index = gpu_index;
    size_params params;

    section *s = (section *)n->val;
    list *options = s->options;
    𐤀𐤌(!is_network(s)) error("First section must be [net] or [network]");
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
    صحيح count = 0;
    free_section(s);
    fprintf(stderr, "layer     filters    size              input                output\n");
    طالما(n){
        params.index = count;
        fprintf(stderr, "%5d ", count);
        s = (section *)n->val;
        options = s->options;
        layer l = {0};
        LAYER_TYPE lt = string_to_layer_type(s->type);
        𐤀𐤌(lt == CONVOLUTIONAL){
            l = parse_convolutional(options, params);
        }وإلا 𐤀𐤌(lt == DECONVOLUTIONAL){
            l = parse_deconvolutional(options, params);
        }وإلا 𐤀𐤌(lt == LOCAL){
            l = parse_local(options, params);
        }وإلا 𐤀𐤌(lt == ACTIVE){
            l = parse_activation(options, params);
        }وإلا 𐤀𐤌(lt == LOGXENT){
            l = parse_logistic(options, params);
        }وإلا 𐤀𐤌(lt == L2NORM){
            l = parse_l2norm(options, params);
        }وإلا 𐤀𐤌(lt == RNN){
            l = parse_rnn(options, params);
        }وإلا 𐤀𐤌(lt == GRU){
            l = parse_gru(options, params);
        }وإلا 𐤀𐤌 (lt == LSTM) {
            l = parse_lstm(options, params);
        }وإلا 𐤀𐤌(lt == CRNN){
            l = parse_crnn(options, params);
        }وإلا 𐤀𐤌(lt == CONNECTED){
            l = parse_connected(options, params);
        }وإلا 𐤀𐤌(lt == CROP){
            l = parse_crop(options, params);
        }وإلا 𐤀𐤌(lt == COST){
            l = parse_cost(options, params);
        }وإلا 𐤀𐤌(lt == REGION){
            l = parse_region(options, params);
        }وإلا 𐤀𐤌(lt == YOLO){
            l = parse_yolo(options, params);
        }وإلا 𐤀𐤌(lt == ISEG){
            l = parse_iseg(options, params);
        }وإلا 𐤀𐤌(lt == DETECTION){
            l = parse_detection(options, params);
        }وإلا 𐤀𐤌(lt == SOFTMAX){
            l = parse_softmax(options, params);
            net->hierarchy = l.softmax_tree;
        }وإلا 𐤀𐤌(lt == NORMALIZATION){
            l = parse_normalization(options, params);
        }وإلا 𐤀𐤌(lt == BATCHNORM){
            l = parse_batchnorm(options, params);
        }وإلا 𐤀𐤌(lt == MAXPOOL){
            l = parse_maxpool(options, params);
        }وإلا 𐤀𐤌(lt == REORG){
            l = parse_reorg(options, params);
        }وإلا 𐤀𐤌(lt == AVGPOOL){
            l = parse_avgpool(options, params);
        }وإلا 𐤀𐤌(lt == ROUTE){
            l = parse_route(options, params, net);
        }وإلا 𐤀𐤌(lt == UPSAMPLE){
            l = parse_upsample(options, params, net);
        }وإلا 𐤀𐤌(lt == SHORTCUT){
            l = parse_shortcut(options, params, net);
        }وإلا 𐤀𐤌(lt == DROPOUT){
            l = parse_dropout(options, params);
            l.output = net->layers[count-1].output;
            l.delta = net->layers[count-1].delta;
#ifdef GPU
            l.output_gpu = net->layers[count-1].output_gpu;
            l.delta_gpu = net->layers[count-1].delta_gpu;
#endif
        }وإلا{
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
        𐤀𐤌 (l.workspace_size > workspace_size) workspace_size = l.workspace_size;
        free_section(s);
        n = n->next;
        ++count;
        𐤀𐤌(n){
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
    𐤀𐤌(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    net->input = calloc(net->inputs*net->batch, حجم(عائم));
    net->truth = calloc(net->truths*net->batch, حجم(عائم));
#ifdef GPU
    net->output_gpu = out.output_gpu;
    net->input_gpu = cuda_make_array(net->input, net->inputs*net->batch);
    net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
#endif
    𐤀𐤌(workspace_size){
        //printf("%ld\n", workspace_size);
#ifdef GPU
        𐤀𐤌(gpu_index >= 0){
            net->workspace = cuda_make_array(0, (workspace_size-1)/حجم(عائم)+1);
        }وإلا {
            net->workspace = calloc(1, workspace_size);
        }
#وإلا
        net->workspace = calloc(1, workspace_size);
#endif
    }
    أرجع net;
}

list *read_cfg(حرف *filename)
{
    FILE *file = fopen(filename, "r");
    𐤀𐤌(file == 0) file_error(filename);
    حرف *line;
    صحيح nu = 0;
    list *options = make_list();
    section *current = 0;
    طالما((line=fgetl(file)) != 0){
        ++ nu;
        strip(line);
        بدل(line[0]){
            حالة '[':
                current = malloc(حجم(section));
                list_insert(options, current);
                current->options = make_list();
                current->type = line;
                اكسر;
            حالة '\0':
            حالة '#':
            حالة ';':
                حرر(line);
                اكسر;
            افتراضي:
                𐤀𐤌(!read_option(line, current->options)){
                    fprintf(stderr, "Config file error line %d, could parse: %s\n", nu, line);
                    حرر(line);
                }
                اكسر;
        }
    }
    fclose(file);
    أرجع options;
}

فراغ save_convolutional_weights_binary(layer l, FILE *fp)
{
#ifdef GPU
    𐤀𐤌(gpu_index >= 0){
        pull_convolutional_layer(l);
    }
#endif
    binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.binary_weights);
    صحيح size = l.c*l.size*l.size;
    صحيح i, j, k;
    fwrite(l.biases, حجم(عائم), l.n, fp);
    𐤀𐤌 (l.batch_normalize){
        fwrite(l.scales, حجم(عائم), l.n, fp);
        fwrite(l.rolling_mean, حجم(عائم), l.n, fp);
        fwrite(l.rolling_variance, حجم(عائم), l.n, fp);
    }
    لكل(i = 0; i < l.n; ++i){
        عائم mean = l.binary_weights[i*size];
        𐤀𐤌(mean < 0) mean = -mean;
        fwrite(&mean, حجم(عائم), 1, fp);
        لكل(j = 0; j < size/8; ++j){
            صحيح index = i*size + j*8;
            unsigned حرف c = 0;
            لكل(k = 0; k < 8; ++k){
                𐤀𐤌 (j*8 + k >= size) اكسر;
                𐤀𐤌 (l.binary_weights[index + k] > 0) c = (c | 1<<k);
            }
            fwrite(&c, حجم(حرف), 1, fp);
        }
    }
}

فراغ save_convolutional_weights(layer l, FILE *fp)
{
    𐤀𐤌(l.binary){
        //save_convolutional_weights_binary(l, fp);
        //return;
    }
#ifdef GPU
    𐤀𐤌(gpu_index >= 0){
        pull_convolutional_layer(l);
    }
#endif
    صحيح num = l.nweights;
    fwrite(l.biases, حجم(عائم), l.n, fp);
    𐤀𐤌 (l.batch_normalize){
        fwrite(l.scales, حجم(عائم), l.n, fp);
        fwrite(l.rolling_mean, حجم(عائم), l.n, fp);
        fwrite(l.rolling_variance, حجم(عائم), l.n, fp);
    }
    fwrite(l.weights, حجم(عائم), num, fp);
}

فراغ save_batchnorm_weights(layer l, FILE *fp)
{
#ifdef GPU
    𐤀𐤌(gpu_index >= 0){
        pull_batchnorm_layer(l);
    }
#endif
    fwrite(l.scales, حجم(عائم), l.c, fp);
    fwrite(l.rolling_mean, حجم(عائم), l.c, fp);
    fwrite(l.rolling_variance, حجم(عائم), l.c, fp);
}

فراغ save_connected_weights(layer l, FILE *fp)
{
#ifdef GPU
    𐤀𐤌(gpu_index >= 0){
        pull_connected_layer(l);
    }
#endif
    fwrite(l.biases, حجم(عائم), l.outputs, fp);
    fwrite(l.weights, حجم(عائم), l.outputs*l.inputs, fp);
    𐤀𐤌 (l.batch_normalize){
        fwrite(l.scales, حجم(عائم), l.outputs, fp);
        fwrite(l.rolling_mean, حجم(عائم), l.outputs, fp);
        fwrite(l.rolling_variance, حجم(عائم), l.outputs, fp);
    }
}

فراغ save_weights_upto(network *net, حرف *filename, صحيح cutoff)
{
#ifdef GPU
    𐤀𐤌(net->gpu_index >= 0){
        cuda_set_device(net->gpu_index);
    }
#endif
    fprintf(stderr, "Saving weights to %s\n", filename);
    FILE *fp = fopen(filename, "wb");
    𐤀𐤌(!fp) file_error(filename);

    صحيح major = 0;
    صحيح minor = 2;
    صحيح revision = 0;
    fwrite(&major, حجم(صحيح), 1, fp);
    fwrite(&minor, حجم(صحيح), 1, fp);
    fwrite(&revision, حجم(صحيح), 1, fp);
    fwrite(net->seen, حجم(size_t), 1, fp);

    صحيح i;
    لكل(i = 0; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        𐤀𐤌 (l.dontsave) استمر;
        𐤀𐤌(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            save_convolutional_weights(l, fp);
        } 𐤀𐤌(l.type == CONNECTED){
            save_connected_weights(l, fp);
        } 𐤀𐤌(l.type == BATCHNORM){
            save_batchnorm_weights(l, fp);
        } 𐤀𐤌(l.type == RNN){
            save_connected_weights(*(l.input_layer), fp);
            save_connected_weights(*(l.self_layer), fp);
            save_connected_weights(*(l.output_layer), fp);
        } 𐤀𐤌 (l.type == LSTM) {
            save_connected_weights(*(l.wi), fp);
            save_connected_weights(*(l.wf), fp);
            save_connected_weights(*(l.wo), fp);
            save_connected_weights(*(l.wg), fp);
            save_connected_weights(*(l.ui), fp);
            save_connected_weights(*(l.uf), fp);
            save_connected_weights(*(l.uo), fp);
            save_connected_weights(*(l.ug), fp);
        } 𐤀𐤌 (l.type == GRU) {
            𐤀𐤌(1){
                save_connected_weights(*(l.wz), fp);
                save_connected_weights(*(l.wr), fp);
                save_connected_weights(*(l.wh), fp);
                save_connected_weights(*(l.uz), fp);
                save_connected_weights(*(l.ur), fp);
                save_connected_weights(*(l.uh), fp);
            }وإلا{
                save_connected_weights(*(l.reset_layer), fp);
                save_connected_weights(*(l.update_layer), fp);
                save_connected_weights(*(l.state_layer), fp);
            }
        }  𐤀𐤌(l.type == CRNN){
            save_convolutional_weights(*(l.input_layer), fp);
            save_convolutional_weights(*(l.self_layer), fp);
            save_convolutional_weights(*(l.output_layer), fp);
        } 𐤀𐤌(l.type == LOCAL){
#ifdef GPU
            𐤀𐤌(gpu_index >= 0){
                pull_local_layer(l);
            }
#endif
            صحيح locations = l.out_w*l.out_h;
            صحيح size = l.size*l.size*l.c*l.n*locations;
            fwrite(l.biases, حجم(عائم), l.outputs, fp);
            fwrite(l.weights, حجم(عائم), size, fp);
        }
    }
    fclose(fp);
}
فراغ save_weights(network *net, حرف *filename)
{
    save_weights_upto(net, filename, net->n);
}

فراغ transpose_matrix(عائم *a, صحيح rows, صحيح cols)
{
    عائم *transpose = calloc(rows*cols, حجم(عائم));
    صحيح x, y;
    لكل(x = 0; x < rows; ++x){
        لكل(y = 0; y < cols; ++y){
            transpose[y*rows + x] = a[x*cols + y];
        }
    }
    memcpy(a, transpose, rows*cols*حجم(عائم));
    حرر(transpose);
}

فراغ load_connected_weights(layer l, FILE *fp, صحيح transpose)
{
    fread(l.biases, حجم(عائم), l.outputs, fp);
    fread(l.weights, حجم(عائم), l.outputs*l.inputs, fp);
    𐤀𐤌(transpose){
        transpose_matrix(l.weights, l.inputs, l.outputs);
    }
    //printf("Biases: %f mean %f variance\n", mean_array(l.biases, l.outputs), variance_array(l.biases, l.outputs));
    //printf("Weights: %f mean %f variance\n", mean_array(l.weights, l.outputs*l.inputs), variance_array(l.weights, l.outputs*l.inputs));
    𐤀𐤌 (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, حجم(عائم), l.outputs, fp);
        fread(l.rolling_mean, حجم(عائم), l.outputs, fp);
        fread(l.rolling_variance, حجم(عائم), l.outputs, fp);
        //printf("Scales: %f mean %f variance\n", mean_array(l.scales, l.outputs), variance_array(l.scales, l.outputs));
        //printf("rolling_mean: %f mean %f variance\n", mean_array(l.rolling_mean, l.outputs), variance_array(l.rolling_mean, l.outputs));
        //printf("rolling_variance: %f mean %f variance\n", mean_array(l.rolling_variance, l.outputs), variance_array(l.rolling_variance, l.outputs));
    }
#ifdef GPU
    𐤀𐤌(gpu_index >= 0){
        push_connected_layer(l);
    }
#endif
}

فراغ load_batchnorm_weights(layer l, FILE *fp)
{
    fread(l.scales, حجم(عائم), l.c, fp);
    fread(l.rolling_mean, حجم(عائم), l.c, fp);
    fread(l.rolling_variance, حجم(عائم), l.c, fp);
#ifdef GPU
    𐤀𐤌(gpu_index >= 0){
        push_batchnorm_layer(l);
    }
#endif
}

فراغ load_convolutional_weights_binary(layer l, FILE *fp)
{
    fread(l.biases, حجم(عائم), l.n, fp);
    𐤀𐤌 (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, حجم(عائم), l.n, fp);
        fread(l.rolling_mean, حجم(عائم), l.n, fp);
        fread(l.rolling_variance, حجم(عائم), l.n, fp);
    }
    صحيح size = l.c*l.size*l.size;
    صحيح i, j, k;
    لكل(i = 0; i < l.n; ++i){
        عائم mean = 0;
        fread(&mean, حجم(عائم), 1, fp);
        لكل(j = 0; j < size/8; ++j){
            صحيح index = i*size + j*8;
            unsigned حرف c = 0;
            fread(&c, حجم(حرف), 1, fp);
            لكل(k = 0; k < 8; ++k){
                𐤀𐤌 (j*8 + k >= size) اكسر;
                l.weights[index + k] = (c & 1<<k) ? mean : -mean;
            }
        }
    }
#ifdef GPU
    𐤀𐤌(gpu_index >= 0){
        push_convolutional_layer(l);
    }
#endif
}

فراغ load_convolutional_weights(layer l, FILE *fp)
{
    𐤀𐤌(l.binary){
        //load_convolutional_weights_binary(l, fp);
        //return;
    }
    𐤀𐤌(l.numload) l.n = l.numload;
    صحيح num = l.c/l.groups*l.n*l.size*l.size;
    fread(l.biases, حجم(عائم), l.n, fp);
    𐤀𐤌 (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, حجم(عائم), l.n, fp);
        fread(l.rolling_mean, حجم(عائم), l.n, fp);
        fread(l.rolling_variance, حجم(عائم), l.n, fp);
        𐤀𐤌(0){
            صحيح i;
            لكل(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_mean[i]);
            }
            printf("\n");
            لكل(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_variance[i]);
            }
            printf("\n");
        }
        𐤀𐤌(0){
            fill_cpu(l.n, 0, l.rolling_mean, 1);
            fill_cpu(l.n, 0, l.rolling_variance, 1);
        }
        𐤀𐤌(0){
            صحيح i;
            لكل(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_mean[i]);
            }
            printf("\n");
            لكل(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_variance[i]);
            }
            printf("\n");
        }
    }
    fread(l.weights, حجم(عائم), num, fp);
    //if(l.c == 3) scal_cpu(num, 1./256, l.weights, 1);
    𐤀𐤌 (l.flipped) {
        transpose_matrix(l.weights, l.c*l.size*l.size, l.n);
    }
    //if (l.binary) binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.weights);
#ifdef GPU
    𐤀𐤌(gpu_index >= 0){
        push_convolutional_layer(l);
    }
#endif
}


فراغ load_weights_upto(network *net, حرف *filename, صحيح start, صحيح cutoff)
{
#ifdef GPU
    𐤀𐤌(net->gpu_index >= 0){
        cuda_set_device(net->gpu_index);
    }
#endif
    fprintf(stderr, "Loading weights from %s...", filename);
    fflush(stdout);
    FILE *fp = fopen(filename, "rb");
    𐤀𐤌(!fp) file_error(filename);

    صحيح major;
    صحيح minor;
    صحيح revision;
    fread(&major, حجم(صحيح), 1, fp);
    fread(&minor, حجم(صحيح), 1, fp);
    fread(&revision, حجم(صحيح), 1, fp);
    𐤀𐤌 ((major*10 + minor) >= 2 && major < 1000 && minor < 1000){
        fread(net->seen, حجم(size_t), 1, fp);
    } وإلا {
        صحيح iseen = 0;
        fread(&iseen, حجم(صحيح), 1, fp);
        *net->seen = iseen;
    }
    صحيح transpose = (major > 1000) || (minor > 1000);

    صحيح i;
    لكل(i = start; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        𐤀𐤌 (l.dontload) استمر;
        𐤀𐤌(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            load_convolutional_weights(l, fp);
        }
        𐤀𐤌(l.type == CONNECTED){
            load_connected_weights(l, fp, transpose);
        }
        𐤀𐤌(l.type == BATCHNORM){
            load_batchnorm_weights(l, fp);
        }
        𐤀𐤌(l.type == CRNN){
            load_convolutional_weights(*(l.input_layer), fp);
            load_convolutional_weights(*(l.self_layer), fp);
            load_convolutional_weights(*(l.output_layer), fp);
        }
        𐤀𐤌(l.type == RNN){
            load_connected_weights(*(l.input_layer), fp, transpose);
            load_connected_weights(*(l.self_layer), fp, transpose);
            load_connected_weights(*(l.output_layer), fp, transpose);
        }
        𐤀𐤌 (l.type == LSTM) {
            load_connected_weights(*(l.wi), fp, transpose);
            load_connected_weights(*(l.wf), fp, transpose);
            load_connected_weights(*(l.wo), fp, transpose);
            load_connected_weights(*(l.wg), fp, transpose);
            load_connected_weights(*(l.ui), fp, transpose);
            load_connected_weights(*(l.uf), fp, transpose);
            load_connected_weights(*(l.uo), fp, transpose);
            load_connected_weights(*(l.ug), fp, transpose);
        }
        𐤀𐤌 (l.type == GRU) {
            𐤀𐤌(1){
                load_connected_weights(*(l.wz), fp, transpose);
                load_connected_weights(*(l.wr), fp, transpose);
                load_connected_weights(*(l.wh), fp, transpose);
                load_connected_weights(*(l.uz), fp, transpose);
                load_connected_weights(*(l.ur), fp, transpose);
                load_connected_weights(*(l.uh), fp, transpose);
            }وإلا{
                load_connected_weights(*(l.reset_layer), fp, transpose);
                load_connected_weights(*(l.update_layer), fp, transpose);
                load_connected_weights(*(l.state_layer), fp, transpose);
            }
        }
        𐤀𐤌(l.type == LOCAL){
            صحيح locations = l.out_w*l.out_h;
            صحيح size = l.size*l.size*l.c*l.n*locations;
            fread(l.biases, حجم(عائم), l.outputs, fp);
            fread(l.weights, حجم(عائم), size, fp);
#ifdef GPU
            𐤀𐤌(gpu_index >= 0){
                push_local_layer(l);
            }
#endif
        }
    }
    fprintf(stderr, "Done!\n");
    fclose(fp);
}

فراغ load_weights(network *net, حرف *filename)
{
    load_weights_upto(net, filename, 0, net->n);
}


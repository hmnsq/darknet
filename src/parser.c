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

تعریفنوع ساختار{
    نویسه *type;
    list *options;
}section;

list *read_cfg(نویسه *filename);

LAYER_TYPE string_to_layer_type(نویسه * type)
{

    اگر (strcmp(type, "[shortcut]")==0) بازگشت SHORTCUT;
    اگر (strcmp(type, "[crop]")==0) بازگشت CROP;
    اگر (strcmp(type, "[cost]")==0) بازگشت COST;
    اگر (strcmp(type, "[detection]")==0) بازگشت DETECTION;
    اگر (strcmp(type, "[region]")==0) بازگشت REGION;
    اگر (strcmp(type, "[yolo]")==0) بازگشت YOLO;
    اگر (strcmp(type, "[iseg]")==0) بازگشت ISEG;
    اگر (strcmp(type, "[local]")==0) بازگشت LOCAL;
    اگر (strcmp(type, "[conv]")==0
            || strcmp(type, "[convolutional]")==0) بازگشت CONVOLUTIONAL;
    اگر (strcmp(type, "[deconv]")==0
            || strcmp(type, "[deconvolutional]")==0) بازگشت DECONVOLUTIONAL;
    اگر (strcmp(type, "[activation]")==0) بازگشت ACTIVE;
    اگر (strcmp(type, "[logistic]")==0) بازگشت LOGXENT;
    اگر (strcmp(type, "[l2norm]")==0) بازگشت L2NORM;
    اگر (strcmp(type, "[net]")==0
            || strcmp(type, "[network]")==0) بازگشت NETWORK;
    اگر (strcmp(type, "[crnn]")==0) بازگشت CRNN;
    اگر (strcmp(type, "[gru]")==0) بازگشت GRU;
    اگر (strcmp(type, "[lstm]") == 0) بازگشت LSTM;
    اگر (strcmp(type, "[rnn]")==0) بازگشت RNN;
    اگر (strcmp(type, "[conn]")==0
            || strcmp(type, "[connected]")==0) بازگشت CONNECTED;
    اگر (strcmp(type, "[max]")==0
            || strcmp(type, "[maxpool]")==0) بازگشت MAXPOOL;
    اگر (strcmp(type, "[reorg]")==0) بازگشت REORG;
    اگر (strcmp(type, "[avg]")==0
            || strcmp(type, "[avgpool]")==0) بازگشت AVGPOOL;
    اگر (strcmp(type, "[dropout]")==0) بازگشت DROPOUT;
    اگر (strcmp(type, "[lrn]")==0
            || strcmp(type, "[normalization]")==0) بازگشت NORMALIZATION;
    اگر (strcmp(type, "[batchnorm]")==0) بازگشت BATCHNORM;
    اگر (strcmp(type, "[soft]")==0
            || strcmp(type, "[softmax]")==0) بازگشت SOFTMAX;
    اگر (strcmp(type, "[route]")==0) بازگشت ROUTE;
    اگر (strcmp(type, "[upsample]")==0) بازگشت UPSAMPLE;
    بازگشت BLANK;
}

تهی free_section(section *s)
{
    آزاد(s->type);
    گره *n = s->options->front;
    وقتی(n){
        kvp *pair = (kvp *)n->val;
        آزاد(pair->کلید);
        آزاد(pair);
        گره *next = n->next;
        آزاد(n);
        n = next;
    }
    آزاد(s->options);
    آزاد(s);
}

تهی parse_data(نویسه *data, اعشاری *a, صحیح n)
{
    صحیح i;
    اگر(!data) بازگشت;
    نویسه *curr = data;
    نویسه *next = data;
    صحیح done = 0;
    برای(i = 0; i < n && !done; ++i){
        وقتی(*++next !='\0' && *next != ',');
        اگر(*next == '\0') done = 1;
        *next = '\0';
        sscanf(curr, "%g", &a[i]);
        curr = next+1;
    }
}

تعریفنوع ساختار size_params{
    صحیح batch;
    صحیح inputs;
    صحیح h;
    صحیح w;
    صحیح c;
    صحیح index;
    صحیح time_steps;
    network *net;
} size_params;

local_layer parse_local(list *options, size_params params)
{
    صحیح n = option_find_int(options, "filters",1);
    صحیح size = option_find_int(options, "size",1);
    صحیح stride = option_find_int(options, "stride",1);
    صحیح pad = option_find_int(options, "pad",0);
    نویسه *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    صحیح batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    اگر(!(h && w && c)) error("Layer before local layer must output image.");

    local_layer layer = make_local_layer(batch,h,w,c,n,size,stride,pad,activation);

    بازگشت layer;
}

layer parse_deconvolutional(list *options, size_params params)
{
    صحیح n = option_find_int(options, "filters",1);
    صحیح size = option_find_int(options, "size",1);
    صحیح stride = option_find_int(options, "stride",1);

    نویسه *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    صحیح batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    اگر(!(h && w && c)) error("Layer before deconvolutional layer must output image.");
    صحیح batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);
    صحیح pad = option_find_int_quiet(options, "pad",0);
    صحیح padding = option_find_int_quiet(options, "padding",0);
    اگر(pad) padding = size/2;

    layer l = make_deconvolutional_layer(batch,h,w,c,n,size,stride,padding, activation, batch_normalize, params.net->adam);

    بازگشت l;
}


convolutional_layer parse_convolutional(list *options, size_params params)
{
    صحیح n = option_find_int(options, "filters",1);
    صحیح size = option_find_int(options, "size",1);
    صحیح stride = option_find_int(options, "stride",1);
    صحیح pad = option_find_int_quiet(options, "pad",0);
    صحیح padding = option_find_int_quiet(options, "padding",0);
    صحیح groups = option_find_int_quiet(options, "groups", 1);
    اگر(pad) padding = size/2;

    نویسه *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    صحیح batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    اگر(!(h && w && c)) error("Layer before convolutional layer must output image.");
    صحیح batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);
    صحیح دودویی = option_find_int_quiet(options, "binary", 0);
    صحیح xnor = option_find_int_quiet(options, "xnor", 0);

    convolutional_layer layer = make_convolutional_layer(batch,h,w,c,n,groups,size,stride,padding,activation, batch_normalize, دودویی, xnor, params.net->adam);
    layer.flipped = option_find_int_quiet(options, "flipped", 0);
    layer.dot = option_find_float_quiet(options, "dot", 0);

    بازگشت layer;
}

layer parse_crnn(list *options, size_params params)
{
    صحیح output_filters = option_find_int(options, "output_filters",1);
    صحیح hidden_filters = option_find_int(options, "hidden_filters",1);
    نویسه *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    صحیح batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_crnn_layer(params.batch, params.w, params.h, params.c, hidden_filters, output_filters, params.time_steps, activation, batch_normalize);

    l.shortcut = option_find_int_quiet(options, "shortcut", 0);

    بازگشت l;
}

layer parse_rnn(list *options, size_params params)
{
    صحیح output = option_find_int(options, "output",1);
    نویسه *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    صحیح batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_rnn_layer(params.batch, params.inputs, output, params.time_steps, activation, batch_normalize, params.net->adam);

    l.shortcut = option_find_int_quiet(options, "shortcut", 0);

    بازگشت l;
}

layer parse_gru(list *options, size_params params)
{
    صحیح output = option_find_int(options, "output",1);
    صحیح batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_gru_layer(params.batch, params.inputs, output, params.time_steps, batch_normalize, params.net->adam);
    l.tanh = option_find_int_quiet(options, "tanh", 0);

    بازگشت l;
}

layer parse_lstm(list *options, size_params params)
{
    صحیح output = option_find_int(options, "output", 1);
    صحیح batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_lstm_layer(params.batch, params.inputs, output, params.time_steps, batch_normalize, params.net->adam);

    بازگشت l;
}

layer parse_connected(list *options, size_params params)
{
    صحیح output = option_find_int(options, "output",1);
    نویسه *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    صحیح batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_connected_layer(params.batch, params.inputs, output, activation, batch_normalize, params.net->adam);
    بازگشت l;
}

layer parse_softmax(list *options, size_params params)
{
    صحیح groups = option_find_int_quiet(options, "groups",1);
    layer l = make_softmax_layer(params.batch, params.inputs, groups);
    l.temperature = option_find_float_quiet(options, "temperature", 1);
    نویسه *درخت_file = option_find_str(options, "tree", 0);
    اگر (درخت_file) l.softmax_درخت = read_درخت(درخت_file);
    l.w = params.w;
    l.h = params.h;
    l.c = params.c;
    l.spatial = option_find_float_quiet(options, "spatial", 0);
    l.noloss =  option_find_int_quiet(options, "noloss", 0);
    بازگشت l;
}

صحیح *parse_yolo_mask(نویسه *a, صحیح *رقم)
{
    صحیح *mask = 0;
    اگر(a){
        صحیح len = strlen(a);
        صحیح n = 1;
        صحیح i;
        برای(i = 0; i < len; ++i){
            اگر (a[i] == ',') ++n;
        }
        mask = calloc(n, اندازه(صحیح));
        برای(i = 0; i < n; ++i){
            صحیح val = atoi(a);
            mask[i] = val;
            a = strchr(a, ',')+1;
        }
        *رقم = n;
    }
    بازگشت mask;
}

layer parse_yolo(list *options, size_params params)
{
    صحیح classes = option_find_int(options, "classes", 20);
    صحیح total = option_find_int(options, "num", 1);
    صحیح رقم = total;

    نویسه *a = option_find_str(options, "mask", 0);
    صحیح *mask = parse_yolo_mask(a, &رقم);
    layer l = make_yolo_layer(params.batch, params.w, params.h, رقم, total, mask, classes);
    أكد(l.outputs == params.inputs);

    l.max_boxes = option_find_int_quiet(options, "max",90);
    l.jitter = option_find_float(options, "jitter", .2);

    l.ignore_thresh = option_find_float(options, "ignore_thresh", .5);
    l.truth_thresh = option_find_float(options, "truth_thresh", 1);
    l.random = option_find_int_quiet(options, "random", 0);

    نویسه *map_file = option_find_str(options, "map", 0);
    اگر (map_file) l.map = read_map(map_file);

    a = option_find_str(options, "anchors", 0);
    اگر(a){
        صحیح len = strlen(a);
        صحیح n = 1;
        صحیح i;
        برای(i = 0; i < len; ++i){
            اگر (a[i] == ',') ++n;
        }
        برای(i = 0; i < n; ++i){
            اعشاری bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    بازگشت l;
}

layer parse_iseg(list *options, size_params params)
{
    صحیح classes = option_find_int(options, "classes", 20);
    صحیح ids = option_find_int(options, "ids", 32);
    layer l = make_iseg_layer(params.batch, params.w, params.h, classes, ids);
    أكد(l.outputs == params.inputs);
    بازگشت l;
}

layer parse_region(list *options, size_params params)
{
    صحیح coords = option_find_int(options, "coords", 4);
    صحیح classes = option_find_int(options, "classes", 20);
    صحیح رقم = option_find_int(options, "num", 1);

    layer l = make_region_layer(params.batch, params.w, params.h, رقم, classes, coords);
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

    نویسه *درخت_file = option_find_str(options, "tree", 0);
    اگر (درخت_file) l.softmax_درخت = read_درخت(درخت_file);
    نویسه *map_file = option_find_str(options, "map", 0);
    اگر (map_file) l.map = read_map(map_file);

    نویسه *a = option_find_str(options, "anchors", 0);
    اگر(a){
        صحیح len = strlen(a);
        صحیح n = 1;
        صحیح i;
        برای(i = 0; i < len; ++i){
            اگر (a[i] == ',') ++n;
        }
        برای(i = 0; i < n; ++i){
            اعشاری bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    بازگشت l;
}

detection_layer parse_detection(list *options, size_params params)
{
    صحیح coords = option_find_int(options, "coords", 1);
    صحیح classes = option_find_int(options, "classes", 1);
    صحیح rescore = option_find_int(options, "rescore", 0);
    صحیح رقم = option_find_int(options, "num", 1);
    صحیح side = option_find_int(options, "side", 7);
    detection_layer layer = make_detection_layer(params.batch, params.inputs, رقم, side, classes, coords, rescore);

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
    بازگشت layer;
}

cost_layer parse_cost(list *options, size_params params)
{
    نویسه *type_s = option_find_str(options, "type", "sse");
    COST_TYPE type = get_cost_type(type_s);
    اعشاری scale = option_find_float_quiet(options, "scale",1);
    cost_layer layer = make_cost_layer(params.batch, params.inputs, type, scale);
    layer.ratio =  option_find_float_quiet(options, "ratio",0);
    layer.noobject_scale =  option_find_float_quiet(options, "noobj", 1);
    layer.thresh =  option_find_float_quiet(options, "thresh",0);
    بازگشت layer;
}

crop_layer parse_crop(list *options, size_params params)
{
    صحیح crop_height = option_find_int(options, "crop_height",1);
    صحیح crop_width = option_find_int(options, "crop_width",1);
    صحیح flip = option_find_int(options, "flip",0);
    اعشاری angle = option_find_float(options, "angle",0);
    اعشاری saturation = option_find_float(options, "saturation",1);
    اعشاری exposure = option_find_float(options, "exposure",1);

    صحیح batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    اگر(!(h && w && c)) error("Layer before crop layer must output image.");

    صحیح noadjust = option_find_int_quiet(options, "noadjust",0);

    crop_layer l = make_crop_layer(batch,h,w,c,crop_height,crop_width,flip, angle, saturation, exposure);
    l.shift = option_find_float(options, "shift", 0);
    l.noadjust = noadjust;
    بازگشت l;
}

layer parse_reorg(list *options, size_params params)
{
    صحیح stride = option_find_int(options, "stride",1);
    صحیح reverse = option_find_int_quiet(options, "reverse",0);
    صحیح flatten = option_find_int_quiet(options, "flatten",0);
    صحیح extra = option_find_int_quiet(options, "extra",0);

    صحیح batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    اگر(!(h && w && c)) error("Layer before reorg layer must output image.");

    layer layer = make_reorg_layer(batch,w,h,c,stride,reverse, flatten, extra);
    بازگشت layer;
}

maxpool_layer parse_maxpool(list *options, size_params params)
{
    صحیح stride = option_find_int(options, "stride",1);
    صحیح size = option_find_int(options, "size",stride);
    صحیح padding = option_find_int_quiet(options, "padding", size-1);

    صحیح batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    اگر(!(h && w && c)) error("Layer before maxpool layer must output image.");

    maxpool_layer layer = make_maxpool_layer(batch,h,w,c,size,stride,padding);
    بازگشت layer;
}

avgpool_layer parse_avgpool(list *options, size_params params)
{
    صحیح batch,w,h,c;
    w = params.w;
    h = params.h;
    c = params.c;
    batch=params.batch;
    اگر(!(h && w && c)) error("Layer before avgpool layer must output image.");

    avgpool_layer layer = make_avgpool_layer(batch,w,h,c);
    بازگشت layer;
}

dropout_layer parse_dropout(list *options, size_params params)
{
    اعشاری probability = option_find_float(options, "probability", .5);
    dropout_layer layer = make_dropout_layer(params.batch, params.inputs, probability);
    layer.out_w = params.w;
    layer.out_h = params.h;
    layer.out_c = params.c;
    بازگشت layer;
}

layer parse_normalization(list *options, size_params params)
{
    اعشاری alpha = option_find_float(options, "alpha", .0001);
    اعشاری beta =  option_find_float(options, "beta" , .75);
    اعشاری kappa = option_find_float(options, "kappa", 1);
    صحیح size = option_find_int(options, "size", 5);
    layer l = make_normalization_layer(params.batch, params.w, params.h, params.c, size, alpha, beta, kappa);
    بازگشت l;
}

layer parse_batchnorm(list *options, size_params params)
{
    layer l = make_batchnorm_layer(params.batch, params.w, params.h, params.c);
    بازگشت l;
}

layer parse_shortcut(list *options, size_params params, network *net)
{
    نویسه *l = option_find(options, "from");
    صحیح index = atoi(l);
    اگر(index < 0) index = params.index + index;

    صحیح batch = params.batch;
    layer from = net->layers[index];

    layer s = make_shortcut_layer(batch, index, params.w, params.h, params.c, from.out_w, from.out_h, from.out_c);

    نویسه *activation_s = option_find_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);
    s.activation = activation;
    s.alpha = option_find_float_quiet(options, "alpha", 1);
    s.beta = option_find_float_quiet(options, "beta", 1);
    بازگشت s;
}


layer parse_l2norm(list *options, size_params params)
{
    layer l = make_l2norm_layer(params.batch, params.inputs);
    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;
    بازگشت l;
}


layer parse_logistic(list *options, size_params params)
{
    layer l = make_logistic_layer(params.batch, params.inputs);
    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;
    بازگشت l;
}

layer parse_activation(list *options, size_params params)
{
    نویسه *activation_s = option_find_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);

    layer l = make_activation_layer(params.batch, params.inputs, activation);

    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;

    بازگشت l;
}

layer parse_upsample(list *options, size_params params, network *net)
{

    صحیح stride = option_find_int(options, "stride",2);
    layer l = make_upsample_layer(params.batch, params.w, params.h, params.c, stride);
    l.scale = option_find_float_quiet(options, "scale", 1);
    بازگشت l;
}

route_layer parse_route(list *options, size_params params, network *net)
{
    نویسه *l = option_find(options, "layers");
    صحیح len = strlen(l);
    اگر(!l) error("Route Layer must specify input layers");
    صحیح n = 1;
    صحیح i;
    برای(i = 0; i < len; ++i){
        اگر (l[i] == ',') ++n;
    }

    صحیح *layers = calloc(n, اندازه(صحیح));
    صحیح *sizes = calloc(n, اندازه(صحیح));
    برای(i = 0; i < n; ++i){
        صحیح index = atoi(l);
        l = strchr(l, ',')+1;
        اگر(index < 0) index = params.index + index;
        layers[i] = index;
        sizes[i] = net->layers[index].outputs;
    }
    صحیح batch = params.batch;

    route_layer layer = make_route_layer(batch, n, layers, sizes);

    convolutional_layer first = net->layers[layers[0]];
    layer.out_w = first.out_w;
    layer.out_h = first.out_h;
    layer.out_c = first.out_c;
    برای(i = 1; i < n; ++i){
        صحیح index = layers[i];
        convolutional_layer next = net->layers[index];
        اگر(next.out_w == first.out_w && next.out_h == first.out_h){
            layer.out_c += next.out_c;
        }وگرنه{
            layer.out_h = layer.out_w = layer.out_c = 0;
        }
    }

    بازگشت layer;
}

learning_rate_policy get_policy(نویسه *s)
{
    اگر (strcmp(s, "random")==0) بازگشت RANDOM;
    اگر (strcmp(s, "poly")==0) بازگشت POLY;
    اگر (strcmp(s, "constant")==0) بازگشت CONSTANT;
    اگر (strcmp(s, "step")==0) بازگشت STEP;
    اگر (strcmp(s, "exp")==0) بازگشت EXP;
    اگر (strcmp(s, "sigmoid")==0) بازگشت SIG;
    اگر (strcmp(s, "steps")==0) بازگشت STEPS;
    fprintf(stderr, "Couldn't find policy %s, going with constant\n", s);
    بازگشت CONSTANT;
}

تهی parse_net_options(list *options, network *net)
{
    net->batch = option_find_int(options, "batch",1);
    net->learning_rate = option_find_float(options, "learning_rate", .001);
    net->momentum = option_find_float(options, "momentum", .9);
    net->decay = option_find_float(options, "decay", .0001);
    صحیح subdivs = option_find_int(options, "subdivisions",1);
    net->time_steps = option_find_int_quiet(options, "time_steps",1);
    net->notruth = option_find_int_quiet(options, "notruth",0);
    net->batch /= subdivs;
    net->batch *= net->time_steps;
    net->subdivisions = subdivs;
    net->random = option_find_int_quiet(options, "random", 0);

    net->adam = option_find_int_quiet(options, "adam", 0);
    اگر(net->adam){
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
    net->max_ratio = option_find_float_quiet(options, "max_ratio", (اعشاری) net->max_crop / net->w);
    net->min_ratio = option_find_float_quiet(options, "min_ratio", (اعشاری) net->min_crop / net->w);
    net->center = option_find_int_quiet(options, "center",0);
    net->clip = option_find_float_quiet(options, "clip", 0);

    net->angle = option_find_float_quiet(options, "angle", 0);
    net->aspect = option_find_float_quiet(options, "aspect", 1);
    net->saturation = option_find_float_quiet(options, "saturation", 1);
    net->exposure = option_find_float_quiet(options, "exposure", 1);
    net->hue = option_find_float_quiet(options, "hue", 0);

    اگر(!net->inputs && !(net->h && net->w && net->c)) error("No input parameters supplied");

    نویسه *policy_s = option_find_str(options, "policy", "constant");
    net->policy = get_policy(policy_s);
    net->burn_in = option_find_int_quiet(options, "burn_in", 0);
    net->power = option_find_float_quiet(options, "power", 4);
    اگر(net->policy == STEP){
        net->step = option_find_int(options, "step", 1);
        net->scale = option_find_float(options, "scale", 1);
    } وگرنه اگر (net->policy == STEPS){
        نویسه *l = option_find(options, "steps");
        نویسه *p = option_find(options, "scales");
        اگر(!l || !p) error("STEPS policy must have steps and scales in cfg file");

        صحیح len = strlen(l);
        صحیح n = 1;
        صحیح i;
        برای(i = 0; i < len; ++i){
            اگر (l[i] == ',') ++n;
        }
        صحیح *steps = calloc(n, اندازه(صحیح));
        اعشاری *scales = calloc(n, اندازه(اعشاری));
        برای(i = 0; i < n; ++i){
            صحیح step    = atoi(l);
            اعشاری scale = atof(p);
            l = strchr(l, ',')+1;
            p = strchr(p, ',')+1;
            steps[i] = step;
            scales[i] = scale;
        }
        net->scales = scales;
        net->steps = steps;
        net->رقم_steps = n;
    } وگرنه اگر (net->policy == EXP){
        net->gamma = option_find_float(options, "gamma", 1);
    } وگرنه اگر (net->policy == SIG){
        net->gamma = option_find_float(options, "gamma", 1);
        net->step = option_find_int(options, "step", 1);
    } وگرنه اگر (net->policy == POLY || net->policy == RANDOM){
    }
    net->max_batches = option_find_int(options, "max_batches", 0);
}

صحیح is_network(section *s)
{
    بازگشت (strcmp(s->type, "[net]")==0
            || strcmp(s->type, "[network]")==0);
}

network *parse_network_cfg(نویسه *filename)
{
    list *sections = read_cfg(filename);
    گره *n = sections->front;
    اگر(!n) error("Config file has no sections");
    network *net = make_network(sections->size - 1);
    net->gpu_index = gpu_index;
    size_params params;

    section *s = (section *)n->val;
    list *options = s->options;
    اگر(!is_network(s)) error("First section must be [net] or [network]");
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
    صحیح شمار = 0;
    free_section(s);
    fprintf(stderr, "layer     filters    size              input                output\n");
    وقتی(n){
        params.index = شمار;
        fprintf(stderr, "%5d ", شمار);
        s = (section *)n->val;
        options = s->options;
        layer l = {0};
        LAYER_TYPE lt = string_to_layer_type(s->type);
        اگر(lt == CONVOLUTIONAL){
            l = parse_convolutional(options, params);
        }وگرنه اگر(lt == DECONVOLUTIONAL){
            l = parse_deconvolutional(options, params);
        }وگرنه اگر(lt == LOCAL){
            l = parse_local(options, params);
        }وگرنه اگر(lt == ACTIVE){
            l = parse_activation(options, params);
        }وگرنه اگر(lt == LOGXENT){
            l = parse_logistic(options, params);
        }وگرنه اگر(lt == L2NORM){
            l = parse_l2norm(options, params);
        }وگرنه اگر(lt == RNN){
            l = parse_rnn(options, params);
        }وگرنه اگر(lt == GRU){
            l = parse_gru(options, params);
        }وگرنه اگر (lt == LSTM) {
            l = parse_lstm(options, params);
        }وگرنه اگر(lt == CRNN){
            l = parse_crnn(options, params);
        }وگرنه اگر(lt == CONNECTED){
            l = parse_connected(options, params);
        }وگرنه اگر(lt == CROP){
            l = parse_crop(options, params);
        }وگرنه اگر(lt == COST){
            l = parse_cost(options, params);
        }وگرنه اگر(lt == REGION){
            l = parse_region(options, params);
        }وگرنه اگر(lt == YOLO){
            l = parse_yolo(options, params);
        }وگرنه اگر(lt == ISEG){
            l = parse_iseg(options, params);
        }وگرنه اگر(lt == DETECTION){
            l = parse_detection(options, params);
        }وگرنه اگر(lt == SOFTMAX){
            l = parse_softmax(options, params);
            net->hierarchy = l.softmax_درخت;
        }وگرنه اگر(lt == NORMALIZATION){
            l = parse_normalization(options, params);
        }وگرنه اگر(lt == BATCHNORM){
            l = parse_batchnorm(options, params);
        }وگرنه اگر(lt == MAXPOOL){
            l = parse_maxpool(options, params);
        }وگرنه اگر(lt == REORG){
            l = parse_reorg(options, params);
        }وگرنه اگر(lt == AVGPOOL){
            l = parse_avgpool(options, params);
        }وگرنه اگر(lt == ROUTE){
            l = parse_route(options, params, net);
        }وگرنه اگر(lt == UPSAMPLE){
            l = parse_upsample(options, params, net);
        }وگرنه اگر(lt == SHORTCUT){
            l = parse_shortcut(options, params, net);
        }وگرنه اگر(lt == DROPOUT){
            l = parse_dropout(options, params);
            l.output = net->layers[شمار-1].output;
            l.delta = net->layers[شمار-1].delta;
#ifdef GPU
            l.output_gpu = net->layers[شمار-1].output_gpu;
            l.delta_gpu = net->layers[شمار-1].delta_gpu;
#endif
        }وگرنه{
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
        net->layers[شمار] = l;
        اگر (l.workspace_size > workspace_size) workspace_size = l.workspace_size;
        free_section(s);
        n = n->next;
        ++شمار;
        اگر(n){
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
    اگر(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    net->درون_داد = calloc(net->inputs*net->batch, اندازه(اعشاری));
    net->truth = calloc(net->truths*net->batch, اندازه(اعشاری));
#ifdef GPU
    net->output_gpu = out.output_gpu;
    net->درون_داد_gpu = cuda_make_array(net->درون_داد, net->inputs*net->batch);
    net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
#endif
    اگر(workspace_size){
        //printf("%ld\n", workspace_size);
#ifdef GPU
        اگر(gpu_index >= 0){
            net->workspace = cuda_make_array(0, (workspace_size-1)/اندازه(اعشاری)+1);
        }وگرنه {
            net->workspace = calloc(1, workspace_size);
        }
#وگرنه
        net->workspace = calloc(1, workspace_size);
#endif
    }
    بازگشت net;
}

list *read_cfg(نویسه *filename)
{
    FILE *file = fopen(filename, "r");
    اگر(file == 0) file_error(filename);
    نویسه *سطر;
    صحیح nu = 0;
    list *options = make_list();
    section *current = 0;
    وقتی((سطر=fgetl(file)) != 0){
        ++ nu;
        strip(سطر);
        انتخاب(سطر[0]){
            حالت '[':
                current = malloc(اندازه(section));
                list_درج(options, current);
                current->options = make_list();
                current->type = سطر;
                بشکن;
            حالت '\0':
            حالت '#':
            حالت ';':
                آزاد(سطر);
                بشکن;
            پیشفرض:
                اگر(!read_option(سطر, current->options)){
                    fprintf(stderr, "Config file error line %d, could parse: %s\n", nu, سطر);
                    آزاد(سطر);
                }
                بشکن;
        }
    }
    fclose(file);
    بازگشت options;
}

تهی save_convolutional_weights_دودویی(layer l, FILE *fp)
{
#ifdef GPU
    اگر(gpu_index >= 0){
        pull_convolutional_layer(l);
    }
#endif
    binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.دودویی_weights);
    صحیح size = l.c*l.size*l.size;
    صحیح i, j, k;
    fwrite(l.biases, اندازه(اعشاری), l.n, fp);
    اگر (l.batch_normalize){
        fwrite(l.scales, اندازه(اعشاری), l.n, fp);
        fwrite(l.rolling_mean, اندازه(اعشاری), l.n, fp);
        fwrite(l.rolling_variance, اندازه(اعشاری), l.n, fp);
    }
    برای(i = 0; i < l.n; ++i){
        اعشاری mean = l.دودویی_weights[i*size];
        اگر(mean < 0) mean = -mean;
        fwrite(&mean, اندازه(اعشاری), 1, fp);
        برای(j = 0; j < size/8; ++j){
            صحیح index = i*size + j*8;
            unsigned نویسه c = 0;
            برای(k = 0; k < 8; ++k){
                اگر (j*8 + k >= size) بشکن;
                اگر (l.دودویی_weights[index + k] > 0) c = (c | 1<<k);
            }
            fwrite(&c, اندازه(نویسه), 1, fp);
        }
    }
}

تهی save_convolutional_weights(layer l, FILE *fp)
{
    اگر(l.دودویی){
        //save_convolutional_weights_binary(l, fp);
        //return;
    }
#ifdef GPU
    اگر(gpu_index >= 0){
        pull_convolutional_layer(l);
    }
#endif
    صحیح رقم = l.nweights;
    fwrite(l.biases, اندازه(اعشاری), l.n, fp);
    اگر (l.batch_normalize){
        fwrite(l.scales, اندازه(اعشاری), l.n, fp);
        fwrite(l.rolling_mean, اندازه(اعشاری), l.n, fp);
        fwrite(l.rolling_variance, اندازه(اعشاری), l.n, fp);
    }
    fwrite(l.weights, اندازه(اعشاری), رقم, fp);
}

تهی save_batchnorm_weights(layer l, FILE *fp)
{
#ifdef GPU
    اگر(gpu_index >= 0){
        pull_batchnorm_layer(l);
    }
#endif
    fwrite(l.scales, اندازه(اعشاری), l.c, fp);
    fwrite(l.rolling_mean, اندازه(اعشاری), l.c, fp);
    fwrite(l.rolling_variance, اندازه(اعشاری), l.c, fp);
}

تهی save_connected_weights(layer l, FILE *fp)
{
#ifdef GPU
    اگر(gpu_index >= 0){
        pull_connected_layer(l);
    }
#endif
    fwrite(l.biases, اندازه(اعشاری), l.outputs, fp);
    fwrite(l.weights, اندازه(اعشاری), l.outputs*l.inputs, fp);
    اگر (l.batch_normalize){
        fwrite(l.scales, اندازه(اعشاری), l.outputs, fp);
        fwrite(l.rolling_mean, اندازه(اعشاری), l.outputs, fp);
        fwrite(l.rolling_variance, اندازه(اعشاری), l.outputs, fp);
    }
}

تهی save_weights_upto(network *net, نویسه *filename, صحیح cutoff)
{
#ifdef GPU
    اگر(net->gpu_index >= 0){
        cuda_set_device(net->gpu_index);
    }
#endif
    fprintf(stderr, "Saving weights to %s\n", filename);
    FILE *fp = fopen(filename, "wb");
    اگر(!fp) file_error(filename);

    صحیح major = 0;
    صحیح minor = 2;
    صحیح revision = 0;
    fwrite(&major, اندازه(صحیح), 1, fp);
    fwrite(&minor, اندازه(صحیح), 1, fp);
    fwrite(&revision, اندازه(صحیح), 1, fp);
    fwrite(net->seen, اندازه(size_t), 1, fp);

    صحیح i;
    برای(i = 0; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        اگر (l.dontsave) ادامه;
        اگر(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            save_convolutional_weights(l, fp);
        } اگر(l.type == CONNECTED){
            save_connected_weights(l, fp);
        } اگر(l.type == BATCHNORM){
            save_batchnorm_weights(l, fp);
        } اگر(l.type == RNN){
            save_connected_weights(*(l.درون_داد_layer), fp);
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
            }وگرنه{
                save_connected_weights(*(l.reset_layer), fp);
                save_connected_weights(*(l.update_layer), fp);
                save_connected_weights(*(l.state_layer), fp);
            }
        }  اگر(l.type == CRNN){
            save_convolutional_weights(*(l.درون_داد_layer), fp);
            save_convolutional_weights(*(l.self_layer), fp);
            save_convolutional_weights(*(l.output_layer), fp);
        } اگر(l.type == LOCAL){
#ifdef GPU
            اگر(gpu_index >= 0){
                pull_local_layer(l);
            }
#endif
            صحیح locations = l.out_w*l.out_h;
            صحیح size = l.size*l.size*l.c*l.n*locations;
            fwrite(l.biases, اندازه(اعشاری), l.outputs, fp);
            fwrite(l.weights, اندازه(اعشاری), size, fp);
        }
    }
    fclose(fp);
}
تهی save_weights(network *net, نویسه *filename)
{
    save_weights_upto(net, filename, net->n);
}

تهی transpose_matrix(اعشاری *a, صحیح rows, صحیح cols)
{
    اعشاری *transpose = calloc(rows*cols, اندازه(اعشاری));
    صحیح x, y;
    برای(x = 0; x < rows; ++x){
        برای(y = 0; y < cols; ++y){
            transpose[y*rows + x] = a[x*cols + y];
        }
    }
    memcpy(a, transpose, rows*cols*اندازه(اعشاری));
    آزاد(transpose);
}

تهی load_connected_weights(layer l, FILE *fp, صحیح transpose)
{
    fread(l.biases, اندازه(اعشاری), l.outputs, fp);
    fread(l.weights, اندازه(اعشاری), l.outputs*l.inputs, fp);
    اگر(transpose){
        transpose_matrix(l.weights, l.inputs, l.outputs);
    }
    //printf("Biases: %f mean %f variance\n", mean_array(l.biases, l.outputs), variance_array(l.biases, l.outputs));
    //printf("Weights: %f mean %f variance\n", mean_array(l.weights, l.outputs*l.inputs), variance_array(l.weights, l.outputs*l.inputs));
    اگر (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, اندازه(اعشاری), l.outputs, fp);
        fread(l.rolling_mean, اندازه(اعشاری), l.outputs, fp);
        fread(l.rolling_variance, اندازه(اعشاری), l.outputs, fp);
        //printf("Scales: %f mean %f variance\n", mean_array(l.scales, l.outputs), variance_array(l.scales, l.outputs));
        //printf("rolling_mean: %f mean %f variance\n", mean_array(l.rolling_mean, l.outputs), variance_array(l.rolling_mean, l.outputs));
        //printf("rolling_variance: %f mean %f variance\n", mean_array(l.rolling_variance, l.outputs), variance_array(l.rolling_variance, l.outputs));
    }
#ifdef GPU
    اگر(gpu_index >= 0){
        افزودن_connected_layer(l);
    }
#endif
}

تهی load_batchnorm_weights(layer l, FILE *fp)
{
    fread(l.scales, اندازه(اعشاری), l.c, fp);
    fread(l.rolling_mean, اندازه(اعشاری), l.c, fp);
    fread(l.rolling_variance, اندازه(اعشاری), l.c, fp);
#ifdef GPU
    اگر(gpu_index >= 0){
        افزودن_batchnorm_layer(l);
    }
#endif
}

تهی load_convolutional_weights_دودویی(layer l, FILE *fp)
{
    fread(l.biases, اندازه(اعشاری), l.n, fp);
    اگر (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, اندازه(اعشاری), l.n, fp);
        fread(l.rolling_mean, اندازه(اعشاری), l.n, fp);
        fread(l.rolling_variance, اندازه(اعشاری), l.n, fp);
    }
    صحیح size = l.c*l.size*l.size;
    صحیح i, j, k;
    برای(i = 0; i < l.n; ++i){
        اعشاری mean = 0;
        fread(&mean, اندازه(اعشاری), 1, fp);
        برای(j = 0; j < size/8; ++j){
            صحیح index = i*size + j*8;
            unsigned نویسه c = 0;
            fread(&c, اندازه(نویسه), 1, fp);
            برای(k = 0; k < 8; ++k){
                اگر (j*8 + k >= size) بشکن;
                l.weights[index + k] = (c & 1<<k) ? mean : -mean;
            }
        }
    }
#ifdef GPU
    اگر(gpu_index >= 0){
        افزودن_convolutional_layer(l);
    }
#endif
}

تهی load_convolutional_weights(layer l, FILE *fp)
{
    اگر(l.دودویی){
        //load_convolutional_weights_binary(l, fp);
        //return;
    }
    اگر(l.numload) l.n = l.numload;
    صحیح رقم = l.c/l.groups*l.n*l.size*l.size;
    fread(l.biases, اندازه(اعشاری), l.n, fp);
    اگر (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, اندازه(اعشاری), l.n, fp);
        fread(l.rolling_mean, اندازه(اعشاری), l.n, fp);
        fread(l.rolling_variance, اندازه(اعشاری), l.n, fp);
        اگر(0){
            صحیح i;
            برای(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_mean[i]);
            }
            printf("\n");
            برای(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_variance[i]);
            }
            printf("\n");
        }
        اگر(0){
            fill_cpu(l.n, 0, l.rolling_mean, 1);
            fill_cpu(l.n, 0, l.rolling_variance, 1);
        }
        اگر(0){
            صحیح i;
            برای(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_mean[i]);
            }
            printf("\n");
            برای(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_variance[i]);
            }
            printf("\n");
        }
    }
    fread(l.weights, اندازه(اعشاری), رقم, fp);
    //if(l.c == 3) scal_cpu(num, 1./256, l.weights, 1);
    اگر (l.flipped) {
        transpose_matrix(l.weights, l.c*l.size*l.size, l.n);
    }
    //if (l.binary) binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.weights);
#ifdef GPU
    اگر(gpu_index >= 0){
        افزودن_convolutional_layer(l);
    }
#endif
}


تهی load_weights_upto(network *net, نویسه *filename, صحیح start, صحیح cutoff)
{
#ifdef GPU
    اگر(net->gpu_index >= 0){
        cuda_set_device(net->gpu_index);
    }
#endif
    fprintf(stderr, "Loading weights from %s...", filename);
    fflush(stdout);
    FILE *fp = fopen(filename, "rb");
    اگر(!fp) file_error(filename);

    صحیح major;
    صحیح minor;
    صحیح revision;
    fread(&major, اندازه(صحیح), 1, fp);
    fread(&minor, اندازه(صحیح), 1, fp);
    fread(&revision, اندازه(صحیح), 1, fp);
    اگر ((major*10 + minor) >= 2 && major < 1000 && minor < 1000){
        fread(net->seen, اندازه(size_t), 1, fp);
    } وگرنه {
        صحیح iseen = 0;
        fread(&iseen, اندازه(صحیح), 1, fp);
        *net->seen = iseen;
    }
    صحیح transpose = (major > 1000) || (minor > 1000);

    صحیح i;
    برای(i = start; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        اگر (l.dontload) ادامه;
        اگر(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            load_convolutional_weights(l, fp);
        }
        اگر(l.type == CONNECTED){
            load_connected_weights(l, fp, transpose);
        }
        اگر(l.type == BATCHNORM){
            load_batchnorm_weights(l, fp);
        }
        اگر(l.type == CRNN){
            load_convolutional_weights(*(l.درون_داد_layer), fp);
            load_convolutional_weights(*(l.self_layer), fp);
            load_convolutional_weights(*(l.output_layer), fp);
        }
        اگر(l.type == RNN){
            load_connected_weights(*(l.درون_داد_layer), fp, transpose);
            load_connected_weights(*(l.self_layer), fp, transpose);
            load_connected_weights(*(l.output_layer), fp, transpose);
        }
        اگر (l.type == LSTM) {
            load_connected_weights(*(l.wi), fp, transpose);
            load_connected_weights(*(l.wf), fp, transpose);
            load_connected_weights(*(l.wo), fp, transpose);
            load_connected_weights(*(l.wg), fp, transpose);
            load_connected_weights(*(l.ui), fp, transpose);
            load_connected_weights(*(l.uf), fp, transpose);
            load_connected_weights(*(l.uo), fp, transpose);
            load_connected_weights(*(l.ug), fp, transpose);
        }
        اگر (l.type == GRU) {
            اگر(1){
                load_connected_weights(*(l.wz), fp, transpose);
                load_connected_weights(*(l.wr), fp, transpose);
                load_connected_weights(*(l.wh), fp, transpose);
                load_connected_weights(*(l.uz), fp, transpose);
                load_connected_weights(*(l.ur), fp, transpose);
                load_connected_weights(*(l.uh), fp, transpose);
            }وگرنه{
                load_connected_weights(*(l.reset_layer), fp, transpose);
                load_connected_weights(*(l.update_layer), fp, transpose);
                load_connected_weights(*(l.state_layer), fp, transpose);
            }
        }
        اگر(l.type == LOCAL){
            صحیح locations = l.out_w*l.out_h;
            صحیح size = l.size*l.size*l.c*l.n*locations;
            fread(l.biases, اندازه(اعشاری), l.outputs, fp);
            fread(l.weights, اندازه(اعشاری), size, fp);
#ifdef GPU
            اگر(gpu_index >= 0){
                افزودن_local_layer(l);
            }
#endif
        }
    }
    fprintf(stderr, "Done!\n");
    fclose(fp);
}

تهی load_weights(network *net, نویسه *filename)
{
    load_weights_upto(net, filename, 0, net->n);
}


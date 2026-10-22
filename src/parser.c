#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <assert.h>

#अन्तर्भाव "activation_layer.h"
#अन्तर्भाव "logistic_layer.h"
#अन्तर्भाव "l2norm_layer.h"
#अन्तर्भाव "activations.h"
#अन्तर्भाव "avgpool_layer.h"
#अन्तर्भाव "batchnorm_layer.h"
#अन्तर्भाव "blas.h"
#अन्तर्भाव "connected_layer.h"
#अन्तर्भाव "deconvolutional_layer.h"
#अन्तर्भाव "convolutional_layer.h"
#अन्तर्भाव "cost_layer.h"
#अन्तर्भाव "crnn_layer.h"
#अन्तर्भाव "crop_layer.h"
#अन्तर्भाव "detection_layer.h"
#अन्तर्भाव "dropout_layer.h"
#अन्तर्भाव "gru_layer.h"
#अन्तर्भाव "list.h"
#अन्तर्भाव "local_layer.h"
#अन्तर्भाव "maxpool_layer.h"
#अन्तर्भाव "normalization_layer.h"
#अन्तर्भाव "option_list.h"
#अन्तर्भाव "parser.h"
#अन्तर्भाव "region_layer.h"
#अन्तर्भाव "yolo_layer.h"
#अन्तर्भाव "iseg_layer.h"
#अन्तर्भाव "reorg_layer.h"
#अन्तर्भाव "rnn_layer.h"
#अन्तर्भाव "route_layer.h"
#अन्तर्भाव "upsample_layer.h"
#अन्तर्भाव "shortcut_layer.h"
#अन्तर्भाव "softmax_layer.h"
#अन्तर्भाव "lstm_layer.h"
#अन्तर्भाव "utils.h"

प्रकारनिर्देश संरचना{
    वर्ण *type;
    list *options;
}section;

list *read_cfg(वर्ण *filename);

LAYER_TYPE string_to_layer_type(वर्ण * type)
{

    यदि (म_भेद(type, "[shortcut]")==0) फर्काऊ SHORTCUT;
    यदि (म_भेद(type, "[crop]")==0) फर्काऊ CROP;
    यदि (म_भेद(type, "[cost]")==0) फर्काऊ COST;
    यदि (म_भेद(type, "[detection]")==0) फर्काऊ DETECTION;
    यदि (म_भेद(type, "[region]")==0) फर्काऊ REGION;
    यदि (म_भेद(type, "[yolo]")==0) फर्काऊ YOLO;
    यदि (म_भेद(type, "[iseg]")==0) फर्काऊ ISEG;
    यदि (म_भेद(type, "[local]")==0) फर्काऊ LOCAL;
    यदि (म_भेद(type, "[conv]")==0
            || म_भेद(type, "[convolutional]")==0) फर्काऊ CONVOLUTIONAL;
    यदि (म_भेद(type, "[deconv]")==0
            || म_भेद(type, "[deconvolutional]")==0) फर्काऊ DECONVOLUTIONAL;
    यदि (म_भेद(type, "[activation]")==0) फर्काऊ ACTIVE;
    यदि (म_भेद(type, "[logistic]")==0) फर्काऊ LOGXENT;
    यदि (म_भेद(type, "[l2norm]")==0) फर्काऊ L2NORM;
    यदि (म_भेद(type, "[net]")==0
            || म_भेद(type, "[network]")==0) फर्काऊ NETWORK;
    यदि (म_भेद(type, "[crnn]")==0) फर्काऊ CRNN;
    यदि (म_भेद(type, "[gru]")==0) फर्काऊ GRU;
    यदि (म_भेद(type, "[lstm]") == 0) फर्काऊ LSTM;
    यदि (म_भेद(type, "[rnn]")==0) फर्काऊ RNN;
    यदि (म_भेद(type, "[conn]")==0
            || म_भेद(type, "[connected]")==0) फर्काऊ CONNECTED;
    यदि (म_भेद(type, "[max]")==0
            || म_भेद(type, "[maxpool]")==0) फर्काऊ MAXPOOL;
    यदि (म_भेद(type, "[reorg]")==0) फर्काऊ REORG;
    यदि (म_भेद(type, "[avg]")==0
            || म_भेद(type, "[avgpool]")==0) फर्काऊ AVGPOOL;
    यदि (म_भेद(type, "[dropout]")==0) फर्काऊ DROPOUT;
    यदि (म_भेद(type, "[lrn]")==0
            || म_भेद(type, "[normalization]")==0) फर्काऊ NORMALIZATION;
    यदि (म_भेद(type, "[batchnorm]")==0) फर्काऊ BATCHNORM;
    यदि (म_भेद(type, "[soft]")==0
            || म_भेद(type, "[softmax]")==0) फर्काऊ SOFTMAX;
    यदि (म_भेद(type, "[route]")==0) फर्काऊ ROUTE;
    यदि (म_भेद(type, "[upsample]")==0) फर्काऊ UPSAMPLE;
    फर्काऊ BLANK;
}

शून्य free_section(section *s)
{
    मुक्त(s->type);
    गाँठो *n = s->options->front;
    जबसम्म(n){
        kvp *pair = (kvp *)n->val;
        मुक्त(pair->साँचो);
        मुक्त(pair);
        गाँठो *next = n->next;
        मुक्त(n);
        n = next;
    }
    मुक्त(s->options);
    मुक्त(s);
}

शून्य parse_data(वर्ण *data, भिन्न *a, पूर्णाङ्क n)
{
    पूर्णाङ्क i;
    यदि(!data) फर्काऊ;
    वर्ण *curr = data;
    वर्ण *next = data;
    पूर्णाङ्क done = 0;
    लागि(i = 0; i < n && !done; ++i){
        जबसम्म(*++next !='\0' && *next != ',');
        यदि(*next == '\0') done = 1;
        *next = '\0';
        माला_पूछो(curr, "%g", &a[i]);
        curr = next+1;
    }
}

प्रकारनिर्देश संरचना size_params{
    पूर्णाङ्क batch;
    पूर्णाङ्क inputs;
    पूर्णाङ्क h;
    पूर्णाङ्क w;
    पूर्णाङ्क c;
    पूर्णाङ्क index;
    पूर्णाङ्क time_steps;
    network *net;
} size_params;

local_layer parse_local(list *options, size_params params)
{
    पूर्णाङ्क n = option_find_int(options, "filters",1);
    पूर्णाङ्क size = option_find_int(options, "size",1);
    पूर्णाङ्क stride = option_find_int(options, "stride",1);
    पूर्णाङ्क pad = option_find_int(options, "pad",0);
    वर्ण *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    पूर्णाङ्क batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    यदि(!(h && w && c)) error("Layer before local layer must output image.");

    local_layer layer = make_local_layer(batch,h,w,c,n,size,stride,pad,activation);

    फर्काऊ layer;
}

layer parse_deconvolutional(list *options, size_params params)
{
    पूर्णाङ्क n = option_find_int(options, "filters",1);
    पूर्णाङ्क size = option_find_int(options, "size",1);
    पूर्णाङ्क stride = option_find_int(options, "stride",1);

    वर्ण *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    पूर्णाङ्क batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    यदि(!(h && w && c)) error("Layer before deconvolutional layer must output image.");
    पूर्णाङ्क batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);
    पूर्णाङ्क pad = option_find_int_quiet(options, "pad",0);
    पूर्णाङ्क padding = option_find_int_quiet(options, "padding",0);
    यदि(pad) padding = size/2;

    layer l = make_deconvolutional_layer(batch,h,w,c,n,size,stride,padding, activation, batch_normalize, params.net->adam);

    फर्काऊ l;
}


convolutional_layer parse_convolutional(list *options, size_params params)
{
    पूर्णाङ्क n = option_find_int(options, "filters",1);
    पूर्णाङ्क size = option_find_int(options, "size",1);
    पूर्णाङ्क stride = option_find_int(options, "stride",1);
    पूर्णाङ्क pad = option_find_int_quiet(options, "pad",0);
    पूर्णाङ्क padding = option_find_int_quiet(options, "padding",0);
    पूर्णाङ्क groups = option_find_int_quiet(options, "groups", 1);
    यदि(pad) padding = size/2;

    वर्ण *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    पूर्णाङ्क batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    यदि(!(h && w && c)) error("Layer before convolutional layer must output image.");
    पूर्णाङ्क batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);
    पूर्णाङ्क द्विआधारी = option_find_int_quiet(options, "binary", 0);
    पूर्णाङ्क xnor = option_find_int_quiet(options, "xnor", 0);

    convolutional_layer layer = make_convolutional_layer(batch,h,w,c,n,groups,size,stride,padding,activation, batch_normalize, द्विआधारी, xnor, params.net->adam);
    layer.flipped = option_find_int_quiet(options, "flipped", 0);
    layer.dot = option_find_float_quiet(options, "dot", 0);

    फर्काऊ layer;
}

layer parse_crnn(list *options, size_params params)
{
    पूर्णाङ्क output_filters = option_find_int(options, "output_filters",1);
    पूर्णाङ्क hidden_filters = option_find_int(options, "hidden_filters",1);
    वर्ण *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    पूर्णाङ्क batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_crnn_layer(params.batch, params.w, params.h, params.c, hidden_filters, output_filters, params.time_steps, activation, batch_normalize);

    l.shortcut = option_find_int_quiet(options, "shortcut", 0);

    फर्काऊ l;
}

layer parse_rnn(list *options, size_params params)
{
    पूर्णाङ्क output = option_find_int(options, "output",1);
    वर्ण *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    पूर्णाङ्क batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_rnn_layer(params.batch, params.inputs, output, params.time_steps, activation, batch_normalize, params.net->adam);

    l.shortcut = option_find_int_quiet(options, "shortcut", 0);

    फर्काऊ l;
}

layer parse_gru(list *options, size_params params)
{
    पूर्णाङ्क output = option_find_int(options, "output",1);
    पूर्णाङ्क batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_gru_layer(params.batch, params.inputs, output, params.time_steps, batch_normalize, params.net->adam);
    l.tanh = option_find_int_quiet(options, "tanh", 0);

    फर्काऊ l;
}

layer parse_lstm(list *options, size_params params)
{
    पूर्णाङ्क output = option_find_int(options, "output", 1);
    पूर्णाङ्क batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_lstm_layer(params.batch, params.inputs, output, params.time_steps, batch_normalize, params.net->adam);

    फर्काऊ l;
}

layer parse_connected(list *options, size_params params)
{
    पूर्णाङ्क output = option_find_int(options, "output",1);
    वर्ण *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    पूर्णाङ्क batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_connected_layer(params.batch, params.inputs, output, activation, batch_normalize, params.net->adam);
    फर्काऊ l;
}

layer parse_softmax(list *options, size_params params)
{
    पूर्णाङ्क groups = option_find_int_quiet(options, "groups",1);
    layer l = make_softmax_layer(params.batch, params.inputs, groups);
    l.temperature = option_find_float_quiet(options, "temperature", 1);
    वर्ण *रूख_file = option_find_str(options, "tree", 0);
    यदि (रूख_file) l.softmax_रूख = read_रूख(रूख_file);
    l.w = params.w;
    l.h = params.h;
    l.c = params.c;
    l.spatial = option_find_float_quiet(options, "spatial", 0);
    l.noloss =  option_find_int_quiet(options, "noloss", 0);
    फर्काऊ l;
}

पूर्णाङ्क *parse_yolo_mask(वर्ण *a, पूर्णाङ्क *अङ्क)
{
    पूर्णाङ्क *mask = 0;
    यदि(a){
        पूर्णाङ्क len = म_माप(a);
        पूर्णाङ्क n = 1;
        पूर्णाङ्क i;
        लागि(i = 0; i < len; ++i){
            यदि (a[i] == ',') ++n;
        }
        mask = सुस्मृति(n, परिमाण(पूर्णाङ्क));
        लागि(i = 0; i < n; ++i){
            पूर्णाङ्क val = म_से_प(a);
            mask[i] = val;
            a = म_अक्षर(a, ',')+1;
        }
        *अङ्क = n;
    }
    फर्काऊ mask;
}

layer parse_yolo(list *options, size_params params)
{
    पूर्णाङ्क classes = option_find_int(options, "classes", 20);
    पूर्णाङ्क total = option_find_int(options, "num", 1);
    पूर्णाङ्क अङ्क = total;

    वर्ण *a = option_find_str(options, "mask", 0);
    पूर्णाङ्क *mask = parse_yolo_mask(a, &अङ्क);
    layer l = make_yolo_layer(params.batch, params.w, params.h, अङ्क, total, mask, classes);
    प्रतिज्ञा(l.outputs == params.inputs);

    l.max_boxes = option_find_int_quiet(options, "max",90);
    l.jitter = option_find_float(options, "jitter", .2);

    l.ignore_thresh = option_find_float(options, "ignore_thresh", .5);
    l.truth_thresh = option_find_float(options, "truth_thresh", 1);
    l.random = option_find_int_quiet(options, "random", 0);

    वर्ण *map_file = option_find_str(options, "map", 0);
    यदि (map_file) l.map = read_map(map_file);

    a = option_find_str(options, "anchors", 0);
    यदि(a){
        पूर्णाङ्क len = म_माप(a);
        पूर्णाङ्क n = 1;
        पूर्णाङ्क i;
        लागि(i = 0; i < len; ++i){
            यदि (a[i] == ',') ++n;
        }
        लागि(i = 0; i < n; ++i){
            भिन्न bias = म_से_भ(a);
            l.biases[i] = bias;
            a = म_अक्षर(a, ',')+1;
        }
    }
    फर्काऊ l;
}

layer parse_iseg(list *options, size_params params)
{
    पूर्णाङ्क classes = option_find_int(options, "classes", 20);
    पूर्णाङ्क ids = option_find_int(options, "ids", 32);
    layer l = make_iseg_layer(params.batch, params.w, params.h, classes, ids);
    प्रतिज्ञा(l.outputs == params.inputs);
    फर्काऊ l;
}

layer parse_region(list *options, size_params params)
{
    पूर्णाङ्क coords = option_find_int(options, "coords", 4);
    पूर्णाङ्क classes = option_find_int(options, "classes", 20);
    पूर्णाङ्क अङ्क = option_find_int(options, "num", 1);

    layer l = make_region_layer(params.batch, params.w, params.h, अङ्क, classes, coords);
    प्रतिज्ञा(l.outputs == params.inputs);

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

    वर्ण *रूख_file = option_find_str(options, "tree", 0);
    यदि (रूख_file) l.softmax_रूख = read_रूख(रूख_file);
    वर्ण *map_file = option_find_str(options, "map", 0);
    यदि (map_file) l.map = read_map(map_file);

    वर्ण *a = option_find_str(options, "anchors", 0);
    यदि(a){
        पूर्णाङ्क len = म_माप(a);
        पूर्णाङ्क n = 1;
        पूर्णाङ्क i;
        लागि(i = 0; i < len; ++i){
            यदि (a[i] == ',') ++n;
        }
        लागि(i = 0; i < n; ++i){
            भिन्न bias = म_से_भ(a);
            l.biases[i] = bias;
            a = म_अक्षर(a, ',')+1;
        }
    }
    फर्काऊ l;
}

detection_layer parse_detection(list *options, size_params params)
{
    पूर्णाङ्क coords = option_find_int(options, "coords", 1);
    पूर्णाङ्क classes = option_find_int(options, "classes", 1);
    पूर्णाङ्क rescore = option_find_int(options, "rescore", 0);
    पूर्णाङ्क अङ्क = option_find_int(options, "num", 1);
    पूर्णाङ्क side = option_find_int(options, "side", 7);
    detection_layer layer = make_detection_layer(params.batch, params.inputs, अङ्क, side, classes, coords, rescore);

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
    फर्काऊ layer;
}

cost_layer parse_cost(list *options, size_params params)
{
    वर्ण *type_s = option_find_str(options, "type", "sse");
    COST_TYPE type = get_cost_type(type_s);
    भिन्न scale = option_find_float_quiet(options, "scale",1);
    cost_layer layer = make_cost_layer(params.batch, params.inputs, type, scale);
    layer.ratio =  option_find_float_quiet(options, "ratio",0);
    layer.noobject_scale =  option_find_float_quiet(options, "noobj", 1);
    layer.thresh =  option_find_float_quiet(options, "thresh",0);
    फर्काऊ layer;
}

crop_layer parse_crop(list *options, size_params params)
{
    पूर्णाङ्क crop_height = option_find_int(options, "crop_height",1);
    पूर्णाङ्क crop_width = option_find_int(options, "crop_width",1);
    पूर्णाङ्क flip = option_find_int(options, "flip",0);
    भिन्न angle = option_find_float(options, "angle",0);
    भिन्न saturation = option_find_float(options, "saturation",1);
    भिन्न exposure = option_find_float(options, "exposure",1);

    पूर्णाङ्क batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    यदि(!(h && w && c)) error("Layer before crop layer must output image.");

    पूर्णाङ्क noadjust = option_find_int_quiet(options, "noadjust",0);

    crop_layer l = make_crop_layer(batch,h,w,c,crop_height,crop_width,flip, angle, saturation, exposure);
    l.shift = option_find_float(options, "shift", 0);
    l.noadjust = noadjust;
    फर्काऊ l;
}

layer parse_reorg(list *options, size_params params)
{
    पूर्णाङ्क stride = option_find_int(options, "stride",1);
    पूर्णाङ्क reverse = option_find_int_quiet(options, "reverse",0);
    पूर्णाङ्क flatten = option_find_int_quiet(options, "flatten",0);
    पूर्णाङ्क extra = option_find_int_quiet(options, "extra",0);

    पूर्णाङ्क batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    यदि(!(h && w && c)) error("Layer before reorg layer must output image.");

    layer layer = make_reorg_layer(batch,w,h,c,stride,reverse, flatten, extra);
    फर्काऊ layer;
}

maxpool_layer parse_maxpool(list *options, size_params params)
{
    पूर्णाङ्क stride = option_find_int(options, "stride",1);
    पूर्णाङ्क size = option_find_int(options, "size",stride);
    पूर्णाङ्क padding = option_find_int_quiet(options, "padding", size-1);

    पूर्णाङ्क batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    यदि(!(h && w && c)) error("Layer before maxpool layer must output image.");

    maxpool_layer layer = make_maxpool_layer(batch,h,w,c,size,stride,padding);
    फर्काऊ layer;
}

avgpool_layer parse_avgpool(list *options, size_params params)
{
    पूर्णाङ्क batch,w,h,c;
    w = params.w;
    h = params.h;
    c = params.c;
    batch=params.batch;
    यदि(!(h && w && c)) error("Layer before avgpool layer must output image.");

    avgpool_layer layer = make_avgpool_layer(batch,w,h,c);
    फर्काऊ layer;
}

dropout_layer parse_dropout(list *options, size_params params)
{
    भिन्न probability = option_find_float(options, "probability", .5);
    dropout_layer layer = make_dropout_layer(params.batch, params.inputs, probability);
    layer.out_w = params.w;
    layer.out_h = params.h;
    layer.out_c = params.c;
    फर्काऊ layer;
}

layer parse_normalization(list *options, size_params params)
{
    भिन्न alpha = option_find_float(options, "alpha", .0001);
    भिन्न beta =  option_find_float(options, "beta" , .75);
    भिन्न kappa = option_find_float(options, "kappa", 1);
    पूर्णाङ्क size = option_find_int(options, "size", 5);
    layer l = make_normalization_layer(params.batch, params.w, params.h, params.c, size, alpha, beta, kappa);
    फर्काऊ l;
}

layer parse_batchnorm(list *options, size_params params)
{
    layer l = make_batchnorm_layer(params.batch, params.w, params.h, params.c);
    फर्काऊ l;
}

layer parse_shortcut(list *options, size_params params, network *net)
{
    वर्ण *l = option_find(options, "from");
    पूर्णाङ्क index = म_से_प(l);
    यदि(index < 0) index = params.index + index;

    पूर्णाङ्क batch = params.batch;
    layer from = net->layers[index];

    layer s = make_shortcut_layer(batch, index, params.w, params.h, params.c, from.out_w, from.out_h, from.out_c);

    वर्ण *activation_s = option_find_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);
    s.activation = activation;
    s.alpha = option_find_float_quiet(options, "alpha", 1);
    s.beta = option_find_float_quiet(options, "beta", 1);
    फर्काऊ s;
}


layer parse_l2norm(list *options, size_params params)
{
    layer l = make_l2norm_layer(params.batch, params.inputs);
    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;
    फर्काऊ l;
}


layer parse_logistic(list *options, size_params params)
{
    layer l = make_logistic_layer(params.batch, params.inputs);
    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;
    फर्काऊ l;
}

layer parse_activation(list *options, size_params params)
{
    वर्ण *activation_s = option_find_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);

    layer l = make_activation_layer(params.batch, params.inputs, activation);

    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;

    फर्काऊ l;
}

layer parse_upsample(list *options, size_params params, network *net)
{

    पूर्णाङ्क stride = option_find_int(options, "stride",2);
    layer l = make_upsample_layer(params.batch, params.w, params.h, params.c, stride);
    l.scale = option_find_float_quiet(options, "scale", 1);
    फर्काऊ l;
}

route_layer parse_route(list *options, size_params params, network *net)
{
    वर्ण *l = option_find(options, "layers");
    पूर्णाङ्क len = म_माप(l);
    यदि(!l) error("Route Layer must specify input layers");
    पूर्णाङ्क n = 1;
    पूर्णाङ्क i;
    लागि(i = 0; i < len; ++i){
        यदि (l[i] == ',') ++n;
    }

    पूर्णाङ्क *layers = सुस्मृति(n, परिमाण(पूर्णाङ्क));
    पूर्णाङ्क *sizes = सुस्मृति(n, परिमाण(पूर्णाङ्क));
    लागि(i = 0; i < n; ++i){
        पूर्णाङ्क index = म_से_प(l);
        l = म_अक्षर(l, ',')+1;
        यदि(index < 0) index = params.index + index;
        layers[i] = index;
        sizes[i] = net->layers[index].outputs;
    }
    पूर्णाङ्क batch = params.batch;

    route_layer layer = make_route_layer(batch, n, layers, sizes);

    convolutional_layer first = net->layers[layers[0]];
    layer.out_w = first.out_w;
    layer.out_h = first.out_h;
    layer.out_c = first.out_c;
    लागि(i = 1; i < n; ++i){
        पूर्णाङ्क index = layers[i];
        convolutional_layer next = net->layers[index];
        यदि(next.out_w == first.out_w && next.out_h == first.out_h){
            layer.out_c += next.out_c;
        }अन्यथा{
            layer.out_h = layer.out_w = layer.out_c = 0;
        }
    }

    फर्काऊ layer;
}

learning_rate_policy get_policy(वर्ण *s)
{
    यदि (म_भेद(s, "random")==0) फर्काऊ RANDOM;
    यदि (म_भेद(s, "poly")==0) फर्काऊ POLY;
    यदि (म_भेद(s, "constant")==0) फर्काऊ CONSTANT;
    यदि (म_भेद(s, "step")==0) फर्काऊ STEP;
    यदि (म_भेद(s, "exp")==0) फर्काऊ EXP;
    यदि (म_भेद(s, "sigmoid")==0) फर्काऊ SIG;
    यदि (म_भेद(s, "steps")==0) फर्काऊ STEPS;
    ख_लिखो(मानक_त्रुटि, "Couldn't find policy %s, going with constant\n", s);
    फर्काऊ CONSTANT;
}

शून्य parse_net_options(list *options, network *net)
{
    net->batch = option_find_int(options, "batch",1);
    net->learning_rate = option_find_float(options, "learning_rate", .001);
    net->momentum = option_find_float(options, "momentum", .9);
    net->decay = option_find_float(options, "decay", .0001);
    पूर्णाङ्क subdivs = option_find_int(options, "subdivisions",1);
    net->time_steps = option_find_int_quiet(options, "time_steps",1);
    net->notruth = option_find_int_quiet(options, "notruth",0);
    net->batch /= subdivs;
    net->batch *= net->time_steps;
    net->subdivisions = subdivs;
    net->random = option_find_int_quiet(options, "random", 0);

    net->adam = option_find_int_quiet(options, "adam", 0);
    यदि(net->adam){
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
    net->max_ratio = option_find_float_quiet(options, "max_ratio", (भिन्न) net->max_crop / net->w);
    net->min_ratio = option_find_float_quiet(options, "min_ratio", (भिन्न) net->min_crop / net->w);
    net->center = option_find_int_quiet(options, "center",0);
    net->clip = option_find_float_quiet(options, "clip", 0);

    net->angle = option_find_float_quiet(options, "angle", 0);
    net->aspect = option_find_float_quiet(options, "aspect", 1);
    net->saturation = option_find_float_quiet(options, "saturation", 1);
    net->exposure = option_find_float_quiet(options, "exposure", 1);
    net->hue = option_find_float_quiet(options, "hue", 0);

    यदि(!net->inputs && !(net->h && net->w && net->c)) error("No input parameters supplied");

    वर्ण *policy_s = option_find_str(options, "policy", "constant");
    net->policy = get_policy(policy_s);
    net->burn_in = option_find_int_quiet(options, "burn_in", 0);
    net->power = option_find_float_quiet(options, "power", 4);
    यदि(net->policy == STEP){
        net->step = option_find_int(options, "step", 1);
        net->scale = option_find_float(options, "scale", 1);
    } अन्यथा यदि (net->policy == STEPS){
        वर्ण *l = option_find(options, "steps");
        वर्ण *p = option_find(options, "scales");
        यदि(!l || !p) error("STEPS policy must have steps and scales in cfg file");

        पूर्णाङ्क len = म_माप(l);
        पूर्णाङ्क n = 1;
        पूर्णाङ्क i;
        लागि(i = 0; i < len; ++i){
            यदि (l[i] == ',') ++n;
        }
        पूर्णाङ्क *steps = सुस्मृति(n, परिमाण(पूर्णाङ्क));
        भिन्न *scales = सुस्मृति(n, परिमाण(भिन्न));
        लागि(i = 0; i < n; ++i){
            पूर्णाङ्क step    = म_से_प(l);
            भिन्न scale = म_से_भ(p);
            l = म_अक्षर(l, ',')+1;
            p = म_अक्षर(p, ',')+1;
            steps[i] = step;
            scales[i] = scale;
        }
        net->scales = scales;
        net->steps = steps;
        net->अङ्क_steps = n;
    } अन्यथा यदि (net->policy == EXP){
        net->gamma = option_find_float(options, "gamma", 1);
    } अन्यथा यदि (net->policy == SIG){
        net->gamma = option_find_float(options, "gamma", 1);
        net->step = option_find_int(options, "step", 1);
    } अन्यथा यदि (net->policy == POLY || net->policy == RANDOM){
    }
    net->max_batches = option_find_int(options, "max_batches", 0);
}

पूर्णाङ्क is_network(section *s)
{
    फर्काऊ (म_भेद(s->type, "[net]")==0
            || म_भेद(s->type, "[network]")==0);
}

network *parse_network_cfg(वर्ण *filename)
{
    list *sections = read_cfg(filename);
    गाँठो *n = sections->front;
    यदि(!n) error("Config file has no sections");
    network *net = make_network(sections->size - 1);
    net->gpu_index = gpu_index;
    size_params params;

    section *s = (section *)n->val;
    list *options = s->options;
    यदि(!is_network(s)) error("First section must be [net] or [network]");
    parse_net_options(options, net);

    params.h = net->h;
    params.w = net->w;
    params.c = net->c;
    params.inputs = net->inputs;
    params.batch = net->batch;
    params.time_steps = net->time_steps;
    params.net = net;

    माप_प्रकार workspace_size = 0;
    n = n->next;
    पूर्णाङ्क गन्ती = 0;
    free_section(s);
    ख_लिखो(मानक_त्रुटि, "layer     filters    size              input                output\n");
    जबसम्म(n){
        params.index = गन्ती;
        ख_लिखो(मानक_त्रुटि, "%5d ", गन्ती);
        s = (section *)n->val;
        options = s->options;
        layer l = {0};
        LAYER_TYPE lt = string_to_layer_type(s->type);
        यदि(lt == CONVOLUTIONAL){
            l = parse_convolutional(options, params);
        }अन्यथा यदि(lt == DECONVOLUTIONAL){
            l = parse_deconvolutional(options, params);
        }अन्यथा यदि(lt == LOCAL){
            l = parse_local(options, params);
        }अन्यथा यदि(lt == ACTIVE){
            l = parse_activation(options, params);
        }अन्यथा यदि(lt == LOGXENT){
            l = parse_logistic(options, params);
        }अन्यथा यदि(lt == L2NORM){
            l = parse_l2norm(options, params);
        }अन्यथा यदि(lt == RNN){
            l = parse_rnn(options, params);
        }अन्यथा यदि(lt == GRU){
            l = parse_gru(options, params);
        }अन्यथा यदि (lt == LSTM) {
            l = parse_lstm(options, params);
        }अन्यथा यदि(lt == CRNN){
            l = parse_crnn(options, params);
        }अन्यथा यदि(lt == CONNECTED){
            l = parse_connected(options, params);
        }अन्यथा यदि(lt == CROP){
            l = parse_crop(options, params);
        }अन्यथा यदि(lt == COST){
            l = parse_cost(options, params);
        }अन्यथा यदि(lt == REGION){
            l = parse_region(options, params);
        }अन्यथा यदि(lt == YOLO){
            l = parse_yolo(options, params);
        }अन्यथा यदि(lt == ISEG){
            l = parse_iseg(options, params);
        }अन्यथा यदि(lt == DETECTION){
            l = parse_detection(options, params);
        }अन्यथा यदि(lt == SOFTMAX){
            l = parse_softmax(options, params);
            net->hierarchy = l.softmax_रूख;
        }अन्यथा यदि(lt == NORMALIZATION){
            l = parse_normalization(options, params);
        }अन्यथा यदि(lt == BATCHNORM){
            l = parse_batchnorm(options, params);
        }अन्यथा यदि(lt == MAXPOOL){
            l = parse_maxpool(options, params);
        }अन्यथा यदि(lt == REORG){
            l = parse_reorg(options, params);
        }अन्यथा यदि(lt == AVGPOOL){
            l = parse_avgpool(options, params);
        }अन्यथा यदि(lt == ROUTE){
            l = parse_route(options, params, net);
        }अन्यथा यदि(lt == UPSAMPLE){
            l = parse_upsample(options, params, net);
        }अन्यथा यदि(lt == SHORTCUT){
            l = parse_shortcut(options, params, net);
        }अन्यथा यदि(lt == DROPOUT){
            l = parse_dropout(options, params);
            l.output = net->layers[गन्ती-1].output;
            l.delta = net->layers[गन्ती-1].delta;
#ifdef GPU
            l.output_gpu = net->layers[गन्ती-1].output_gpu;
            l.delta_gpu = net->layers[गन्ती-1].delta_gpu;
#endif
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
        net->layers[गन्ती] = l;
        यदि (l.workspace_size > workspace_size) workspace_size = l.workspace_size;
        free_section(s);
        n = n->next;
        ++गन्ती;
        यदि(n){
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
    यदि(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    net->आगत = सुस्मृति(net->inputs*net->batch, परिमाण(भिन्न));
    net->truth = सुस्मृति(net->truths*net->batch, परिमाण(भिन्न));
#ifdef GPU
    net->output_gpu = out.output_gpu;
    net->आगत_gpu = cuda_make_array(net->आगत, net->inputs*net->batch);
    net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
#endif
    यदि(workspace_size){
        //printf("%ld\n", workspace_size);
#ifdef GPU
        यदि(gpu_index >= 0){
            net->workspace = cuda_make_array(0, (workspace_size-1)/परिमाण(भिन्न)+1);
        }अन्यथा {
            net->workspace = सुस्मृति(1, workspace_size);
        }
#अन्यथा
        net->workspace = सुस्मृति(1, workspace_size);
#endif
    }
    फर्काऊ net;
}

list *read_cfg(वर्ण *filename)
{
    खाता *file = ख_खोलो(filename, "r");
    यदि(file == 0) file_error(filename);
    वर्ण *पङ्क्ति;
    पूर्णाङ्क nu = 0;
    list *options = make_list();
    section *current = 0;
    जबसम्म((पङ्क्ति=fgetl(file)) != 0){
        ++ nu;
        strip(पङ्क्ति);
        विकल्प(पङ्क्ति[0]){
            पक्ष '[':
                current = दो_स्मृति(परिमाण(section));
                list_घुसाउ(options, current);
                current->options = make_list();
                current->type = पङ्क्ति;
                रोक;
            पक्ष '\0':
            पक्ष '#':
            पक्ष ';':
                मुक्त(पङ्क्ति);
                रोक;
            स्वभाव:
                यदि(!read_option(पङ्क्ति, current->options)){
                    ख_लिखो(मानक_त्रुटि, "Config file error line %d, could parse: %s\n", nu, पङ्क्ति);
                    मुक्त(पङ्क्ति);
                }
                रोक;
        }
    }
    ख_बंद(file);
    फर्काऊ options;
}

शून्य save_convolutional_weights_द्विआधारी(layer l, खाता *fp)
{
#ifdef GPU
    यदि(gpu_index >= 0){
        pull_convolutional_layer(l);
    }
#endif
    binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.द्विआधारी_weights);
    पूर्णाङ्क size = l.c*l.size*l.size;
    पूर्णाङ्क i, j, k;
    ख_डालो(l.biases, परिमाण(भिन्न), l.n, fp);
    यदि (l.batch_normalize){
        ख_डालो(l.scales, परिमाण(भिन्न), l.n, fp);
        ख_डालो(l.rolling_mean, परिमाण(भिन्न), l.n, fp);
        ख_डालो(l.rolling_variance, परिमाण(भिन्न), l.n, fp);
    }
    लागि(i = 0; i < l.n; ++i){
        भिन्न mean = l.द्विआधारी_weights[i*size];
        यदि(mean < 0) mean = -mean;
        ख_डालो(&mean, परिमाण(भिन्न), 1, fp);
        लागि(j = 0; j < size/8; ++j){
            पूर्णाङ्क index = i*size + j*8;
            अचिन्हित वर्ण c = 0;
            लागि(k = 0; k < 8; ++k){
                यदि (j*8 + k >= size) रोक;
                यदि (l.द्विआधारी_weights[index + k] > 0) c = (c | 1<<k);
            }
            ख_डालो(&c, परिमाण(वर्ण), 1, fp);
        }
    }
}

शून्य save_convolutional_weights(layer l, खाता *fp)
{
    यदि(l.द्विआधारी){
        //save_convolutional_weights_binary(l, fp);
        //return;
    }
#ifdef GPU
    यदि(gpu_index >= 0){
        pull_convolutional_layer(l);
    }
#endif
    पूर्णाङ्क अङ्क = l.nweights;
    ख_डालो(l.biases, परिमाण(भिन्न), l.n, fp);
    यदि (l.batch_normalize){
        ख_डालो(l.scales, परिमाण(भिन्न), l.n, fp);
        ख_डालो(l.rolling_mean, परिमाण(भिन्न), l.n, fp);
        ख_डालो(l.rolling_variance, परिमाण(भिन्न), l.n, fp);
    }
    ख_डालो(l.weights, परिमाण(भिन्न), अङ्क, fp);
}

शून्य save_batchnorm_weights(layer l, खाता *fp)
{
#ifdef GPU
    यदि(gpu_index >= 0){
        pull_batchnorm_layer(l);
    }
#endif
    ख_डालो(l.scales, परिमाण(भिन्न), l.c, fp);
    ख_डालो(l.rolling_mean, परिमाण(भिन्न), l.c, fp);
    ख_डालो(l.rolling_variance, परिमाण(भिन्न), l.c, fp);
}

शून्य save_connected_weights(layer l, खाता *fp)
{
#ifdef GPU
    यदि(gpu_index >= 0){
        pull_connected_layer(l);
    }
#endif
    ख_डालो(l.biases, परिमाण(भिन्न), l.outputs, fp);
    ख_डालो(l.weights, परिमाण(भिन्न), l.outputs*l.inputs, fp);
    यदि (l.batch_normalize){
        ख_डालो(l.scales, परिमाण(भिन्न), l.outputs, fp);
        ख_डालो(l.rolling_mean, परिमाण(भिन्न), l.outputs, fp);
        ख_डालो(l.rolling_variance, परिमाण(भिन्न), l.outputs, fp);
    }
}

शून्य save_weights_upto(network *net, वर्ण *filename, पूर्णाङ्क cutoff)
{
#ifdef GPU
    यदि(net->gpu_index >= 0){
        cuda_set_device(net->gpu_index);
    }
#endif
    ख_लिखो(मानक_त्रुटि, "Saving weights to %s\n", filename);
    खाता *fp = ख_खोलो(filename, "wb");
    यदि(!fp) file_error(filename);

    पूर्णाङ्क major = 0;
    पूर्णाङ्क minor = 2;
    पूर्णाङ्क revision = 0;
    ख_डालो(&major, परिमाण(पूर्णाङ्क), 1, fp);
    ख_डालो(&minor, परिमाण(पूर्णाङ्क), 1, fp);
    ख_डालो(&revision, परिमाण(पूर्णाङ्क), 1, fp);
    ख_डालो(net->seen, परिमाण(माप_प्रकार), 1, fp);

    पूर्णाङ्क i;
    लागि(i = 0; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        यदि (l.dontsave) जारी;
        यदि(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            save_convolutional_weights(l, fp);
        } यदि(l.type == CONNECTED){
            save_connected_weights(l, fp);
        } यदि(l.type == BATCHNORM){
            save_batchnorm_weights(l, fp);
        } यदि(l.type == RNN){
            save_connected_weights(*(l.आगत_layer), fp);
            save_connected_weights(*(l.self_layer), fp);
            save_connected_weights(*(l.output_layer), fp);
        } यदि (l.type == LSTM) {
            save_connected_weights(*(l.wi), fp);
            save_connected_weights(*(l.wf), fp);
            save_connected_weights(*(l.wo), fp);
            save_connected_weights(*(l.wg), fp);
            save_connected_weights(*(l.ui), fp);
            save_connected_weights(*(l.uf), fp);
            save_connected_weights(*(l.uo), fp);
            save_connected_weights(*(l.ug), fp);
        } यदि (l.type == GRU) {
            यदि(1){
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
        }  यदि(l.type == CRNN){
            save_convolutional_weights(*(l.आगत_layer), fp);
            save_convolutional_weights(*(l.self_layer), fp);
            save_convolutional_weights(*(l.output_layer), fp);
        } यदि(l.type == LOCAL){
#ifdef GPU
            यदि(gpu_index >= 0){
                pull_local_layer(l);
            }
#endif
            पूर्णाङ्क locations = l.out_w*l.out_h;
            पूर्णाङ्क size = l.size*l.size*l.c*l.n*locations;
            ख_डालो(l.biases, परिमाण(भिन्न), l.outputs, fp);
            ख_डालो(l.weights, परिमाण(भिन्न), size, fp);
        }
    }
    ख_बंद(fp);
}
शून्य save_weights(network *net, वर्ण *filename)
{
    save_weights_upto(net, filename, net->n);
}

शून्य transpose_matrix(भिन्न *a, पूर्णाङ्क rows, पूर्णाङ्क cols)
{
    भिन्न *transpose = सुस्मृति(rows*cols, परिमाण(भिन्न));
    पूर्णाङ्क x, y;
    लागि(x = 0; x < rows; ++x){
        लागि(y = 0; y < cols; ++y){
            transpose[y*rows + x] = a[x*cols + y];
        }
    }
    स_नकल(a, transpose, rows*cols*परिमाण(भिन्न));
    मुक्त(transpose);
}

शून्य load_connected_weights(layer l, खाता *fp, पूर्णाङ्क transpose)
{
    ख_पढ़ो(l.biases, परिमाण(भिन्न), l.outputs, fp);
    ख_पढ़ो(l.weights, परिमाण(भिन्न), l.outputs*l.inputs, fp);
    यदि(transpose){
        transpose_matrix(l.weights, l.inputs, l.outputs);
    }
    //printf("Biases: %f mean %f variance\n", mean_array(l.biases, l.outputs), variance_array(l.biases, l.outputs));
    //printf("Weights: %f mean %f variance\n", mean_array(l.weights, l.outputs*l.inputs), variance_array(l.weights, l.outputs*l.inputs));
    यदि (l.batch_normalize && (!l.dontloadscales)){
        ख_पढ़ो(l.scales, परिमाण(भिन्न), l.outputs, fp);
        ख_पढ़ो(l.rolling_mean, परिमाण(भिन्न), l.outputs, fp);
        ख_पढ़ो(l.rolling_variance, परिमाण(भिन्न), l.outputs, fp);
        //printf("Scales: %f mean %f variance\n", mean_array(l.scales, l.outputs), variance_array(l.scales, l.outputs));
        //printf("rolling_mean: %f mean %f variance\n", mean_array(l.rolling_mean, l.outputs), variance_array(l.rolling_mean, l.outputs));
        //printf("rolling_variance: %f mean %f variance\n", mean_array(l.rolling_variance, l.outputs), variance_array(l.rolling_variance, l.outputs));
    }
#ifdef GPU
    यदि(gpu_index >= 0){
        थप_connected_layer(l);
    }
#endif
}

शून्य load_batchnorm_weights(layer l, खाता *fp)
{
    ख_पढ़ो(l.scales, परिमाण(भिन्न), l.c, fp);
    ख_पढ़ो(l.rolling_mean, परिमाण(भिन्न), l.c, fp);
    ख_पढ़ो(l.rolling_variance, परिमाण(भिन्न), l.c, fp);
#ifdef GPU
    यदि(gpu_index >= 0){
        थप_batchnorm_layer(l);
    }
#endif
}

शून्य load_convolutional_weights_द्विआधारी(layer l, खाता *fp)
{
    ख_पढ़ो(l.biases, परिमाण(भिन्न), l.n, fp);
    यदि (l.batch_normalize && (!l.dontloadscales)){
        ख_पढ़ो(l.scales, परिमाण(भिन्न), l.n, fp);
        ख_पढ़ो(l.rolling_mean, परिमाण(भिन्न), l.n, fp);
        ख_पढ़ो(l.rolling_variance, परिमाण(भिन्न), l.n, fp);
    }
    पूर्णाङ्क size = l.c*l.size*l.size;
    पूर्णाङ्क i, j, k;
    लागि(i = 0; i < l.n; ++i){
        भिन्न mean = 0;
        ख_पढ़ो(&mean, परिमाण(भिन्न), 1, fp);
        लागि(j = 0; j < size/8; ++j){
            पूर्णाङ्क index = i*size + j*8;
            अचिन्हित वर्ण c = 0;
            ख_पढ़ो(&c, परिमाण(वर्ण), 1, fp);
            लागि(k = 0; k < 8; ++k){
                यदि (j*8 + k >= size) रोक;
                l.weights[index + k] = (c & 1<<k) ? mean : -mean;
            }
        }
    }
#ifdef GPU
    यदि(gpu_index >= 0){
        थप_convolutional_layer(l);
    }
#endif
}

शून्य load_convolutional_weights(layer l, खाता *fp)
{
    यदि(l.द्विआधारी){
        //load_convolutional_weights_binary(l, fp);
        //return;
    }
    यदि(l.numload) l.n = l.numload;
    पूर्णाङ्क अङ्क = l.c/l.groups*l.n*l.size*l.size;
    ख_पढ़ो(l.biases, परिमाण(भिन्न), l.n, fp);
    यदि (l.batch_normalize && (!l.dontloadscales)){
        ख_पढ़ो(l.scales, परिमाण(भिन्न), l.n, fp);
        ख_पढ़ो(l.rolling_mean, परिमाण(भिन्न), l.n, fp);
        ख_पढ़ो(l.rolling_variance, परिमाण(भिन्न), l.n, fp);
        यदि(0){
            पूर्णाङ्क i;
            लागि(i = 0; i < l.n; ++i){
                म_लिखो("%g, ", l.rolling_mean[i]);
            }
            म_लिखो("\n");
            लागि(i = 0; i < l.n; ++i){
                म_लिखो("%g, ", l.rolling_variance[i]);
            }
            म_लिखो("\n");
        }
        यदि(0){
            fill_cpu(l.n, 0, l.rolling_mean, 1);
            fill_cpu(l.n, 0, l.rolling_variance, 1);
        }
        यदि(0){
            पूर्णाङ्क i;
            लागि(i = 0; i < l.n; ++i){
                म_लिखो("%g, ", l.rolling_mean[i]);
            }
            म_लिखो("\n");
            लागि(i = 0; i < l.n; ++i){
                म_लिखो("%g, ", l.rolling_variance[i]);
            }
            म_लिखो("\n");
        }
    }
    ख_पढ़ो(l.weights, परिमाण(भिन्न), अङ्क, fp);
    //if(l.c == 3) scal_cpu(num, 1./256, l.weights, 1);
    यदि (l.flipped) {
        transpose_matrix(l.weights, l.c*l.size*l.size, l.n);
    }
    //if (l.binary) binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.weights);
#ifdef GPU
    यदि(gpu_index >= 0){
        थप_convolutional_layer(l);
    }
#endif
}


शून्य load_weights_upto(network *net, वर्ण *filename, पूर्णाङ्क start, पूर्णाङ्क cutoff)
{
#ifdef GPU
    यदि(net->gpu_index >= 0){
        cuda_set_device(net->gpu_index);
    }
#endif
    ख_लिखो(मानक_त्रुटि, "Loading weights from %s...", filename);
    ख_साफ(मानक_निकास);
    खाता *fp = ख_खोलो(filename, "rb");
    यदि(!fp) file_error(filename);

    पूर्णाङ्क major;
    पूर्णाङ्क minor;
    पूर्णाङ्क revision;
    ख_पढ़ो(&major, परिमाण(पूर्णाङ्क), 1, fp);
    ख_पढ़ो(&minor, परिमाण(पूर्णाङ्क), 1, fp);
    ख_पढ़ो(&revision, परिमाण(पूर्णाङ्क), 1, fp);
    यदि ((major*10 + minor) >= 2 && major < 1000 && minor < 1000){
        ख_पढ़ो(net->seen, परिमाण(माप_प्रकार), 1, fp);
    } अन्यथा {
        पूर्णाङ्क iseen = 0;
        ख_पढ़ो(&iseen, परिमाण(पूर्णाङ्क), 1, fp);
        *net->seen = iseen;
    }
    पूर्णाङ्क transpose = (major > 1000) || (minor > 1000);

    पूर्णाङ्क i;
    लागि(i = start; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        यदि (l.dontload) जारी;
        यदि(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            load_convolutional_weights(l, fp);
        }
        यदि(l.type == CONNECTED){
            load_connected_weights(l, fp, transpose);
        }
        यदि(l.type == BATCHNORM){
            load_batchnorm_weights(l, fp);
        }
        यदि(l.type == CRNN){
            load_convolutional_weights(*(l.आगत_layer), fp);
            load_convolutional_weights(*(l.self_layer), fp);
            load_convolutional_weights(*(l.output_layer), fp);
        }
        यदि(l.type == RNN){
            load_connected_weights(*(l.आगत_layer), fp, transpose);
            load_connected_weights(*(l.self_layer), fp, transpose);
            load_connected_weights(*(l.output_layer), fp, transpose);
        }
        यदि (l.type == LSTM) {
            load_connected_weights(*(l.wi), fp, transpose);
            load_connected_weights(*(l.wf), fp, transpose);
            load_connected_weights(*(l.wo), fp, transpose);
            load_connected_weights(*(l.wg), fp, transpose);
            load_connected_weights(*(l.ui), fp, transpose);
            load_connected_weights(*(l.uf), fp, transpose);
            load_connected_weights(*(l.uo), fp, transpose);
            load_connected_weights(*(l.ug), fp, transpose);
        }
        यदि (l.type == GRU) {
            यदि(1){
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
        यदि(l.type == LOCAL){
            पूर्णाङ्क locations = l.out_w*l.out_h;
            पूर्णाङ्क size = l.size*l.size*l.c*l.n*locations;
            ख_पढ़ो(l.biases, परिमाण(भिन्न), l.outputs, fp);
            ख_पढ़ो(l.weights, परिमाण(भिन्न), size, fp);
#ifdef GPU
            यदि(gpu_index >= 0){
                थप_local_layer(l);
            }
#endif
        }
    }
    ख_लिखो(मानक_त्रुटि, "Done!\n");
    ख_बंद(fp);
}

शून्य load_weights(network *net, वर्ण *filename)
{
    load_weights_upto(net, filename, 0, net->n);
}


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

LAYER_TYPE string_गन्तव्यम्_layer_type(वर्ण * type)
{

    यदि (वाक्यं_तुलय(type, "[shortcut]")==0) प्रत्यागम SHORTCUT;
    यदि (वाक्यं_तुलय(type, "[crop]")==0) प्रत्यागम CROP;
    यदि (वाक्यं_तुलय(type, "[cost]")==0) प्रत्यागम COST;
    यदि (वाक्यं_तुलय(type, "[detection]")==0) प्रत्यागम DETECTION;
    यदि (वाक्यं_तुलय(type, "[region]")==0) प्रत्यागम REGION;
    यदि (वाक्यं_तुलय(type, "[yolo]")==0) प्रत्यागम YOLO;
    यदि (वाक्यं_तुलय(type, "[iseg]")==0) प्रत्यागम ISEG;
    यदि (वाक्यं_तुलय(type, "[local]")==0) प्रत्यागम LOCAL;
    यदि (वाक्यं_तुलय(type, "[conv]")==0
            || वाक्यं_तुलय(type, "[convolutional]")==0) प्रत्यागम CONVOLUTIONAL;
    यदि (वाक्यं_तुलय(type, "[deconv]")==0
            || वाक्यं_तुलय(type, "[deconvolutional]")==0) प्रत्यागम DECONVOLUTIONAL;
    यदि (वाक्यं_तुलय(type, "[activation]")==0) प्रत्यागम सक्रिय;
    यदि (वाक्यं_तुलय(type, "[logistic]")==0) प्रत्यागम LOGXENT;
    यदि (वाक्यं_तुलय(type, "[l2norm]")==0) प्रत्यागम L2NORM;
    यदि (वाक्यं_तुलय(type, "[net]")==0
            || वाक्यं_तुलय(type, "[network]")==0) प्रत्यागम जालम्_2;
    यदि (वाक्यं_तुलय(type, "[crnn]")==0) प्रत्यागम CRNN;
    यदि (वाक्यं_तुलय(type, "[gru]")==0) प्रत्यागम GRU;
    यदि (वाक्यं_तुलय(type, "[lstm]") == 0) प्रत्यागम LSTM;
    यदि (वाक्यं_तुलय(type, "[rnn]")==0) प्रत्यागम RNN;
    यदि (वाक्यं_तुलय(type, "[conn]")==0
            || वाक्यं_तुलय(type, "[connected]")==0) प्रत्यागम CONNECTED;
    यदि (वाक्यं_तुलय(type, "[max]")==0
            || वाक्यं_तुलय(type, "[maxpool]")==0) प्रत्यागम MAXPOOL;
    यदि (वाक्यं_तुलय(type, "[reorg]")==0) प्रत्यागम REORG;
    यदि (वाक्यं_तुलय(type, "[avg]")==0
            || वाक्यं_तुलय(type, "[avgpool]")==0) प्रत्यागम AVGPOOL;
    यदि (वाक्यं_तुलय(type, "[dropout]")==0) प्रत्यागम DROPOUT;
    यदि (वाक्यं_तुलय(type, "[lrn]")==0
            || वाक्यं_तुलय(type, "[normalization]")==0) प्रत्यागम NORMALIZATION;
    यदि (वाक्यं_तुलय(type, "[batchnorm]")==0) प्रत्यागम BATCHNORM;
    यदि (वाक्यं_तुलय(type, "[soft]")==0
            || वाक्यं_तुलय(type, "[softmax]")==0) प्रत्यागम SOFTMAX;
    यदि (वाक्यं_तुलय(type, "[route]")==0) प्रत्यागम ROUTE;
    यदि (वाक्यं_तुलय(type, "[upsample]")==0) प्रत्यागम UPSAMPLE;
    प्रत्यागम BLANK;
}

शून्य free_section(section *s)
{
    मुक्त(s->type);
    ग्रन्थिः *n = s->options->front;
    यावत्(n){
        kvp *pair = (kvp *)n->val;
        मुक्त(pair->कुञ्चिका);
        मुक्त(pair);
        ग्रन्थिः *अग्र = n->अग्र;
        मुक्त(n);
        n = अग्र;
    }
    मुक्त(s->options);
    मुक्त(s);
}

शून्य parse_दत्त(वर्ण *दत्त, भिन्न *a, पूर्णाङ्क n)
{
    पूर्णाङ्क i;
    यदि(!दत्त) प्रत्यागम;
    वर्ण *curr = दत्त;
    वर्ण *अग्र = दत्त;
    पूर्णाङ्क समाप्तम् = 0;
    प्रति(i = 0; i < n && !समाप्तम्; ++i){
        यावत्(*++अग्र !='\0' && *अग्र != ',');
        यदि(*अग्र == '\0') समाप्तम् = 1;
        *अग्र = '\0';
        वाक्यात्_पठ(curr, "%g", &a[i]);
        curr = अग्र+1;
    }
}

प्रकारनिर्देश संरचना आकार_params{
    पूर्णाङ्क batch;
    पूर्णाङ्क inputs;
    पूर्णाङ्क h;
    पूर्णाङ्क w;
    पूर्णाङ्क c;
    पूर्णाङ्क सूचकांक;
    पूर्णाङ्क time_चरणाः;
    जालम् *net;
} आकार_params;

local_layer parse_local(list *options, आकार_params params)
{
    पूर्णाङ्क n = option_find_int(options, "filters",1);
    पूर्णाङ्क आकार = option_find_int(options, "size",1);
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

    local_layer layer = make_local_layer(batch,h,w,c,n,आकार,stride,pad,activation);

    प्रत्यागम layer;
}

layer parse_deconvolutional(list *options, आकार_params params)
{
    पूर्णाङ्क n = option_find_int(options, "filters",1);
    पूर्णाङ्क आकार = option_find_int(options, "size",1);
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
    यदि(pad) padding = आकार/2;

    layer l = make_deconvolutional_layer(batch,h,w,c,n,आकार,stride,padding, activation, batch_normalize, params.net->adam);

    प्रत्यागम l;
}


convolutional_layer parse_convolutional(list *options, आकार_params params)
{
    पूर्णाङ्क n = option_find_int(options, "filters",1);
    पूर्णाङ्क आकार = option_find_int(options, "size",1);
    पूर्णाङ्क stride = option_find_int(options, "stride",1);
    पूर्णाङ्क pad = option_find_int_quiet(options, "pad",0);
    पूर्णाङ्क padding = option_find_int_quiet(options, "padding",0);
    पूर्णाङ्क groups = option_find_int_quiet(options, "groups", 1);
    यदि(pad) padding = आकार/2;

    वर्ण *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    पूर्णाङ्क batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    यदि(!(h && w && c)) error("Layer before convolutional layer must output image.");
    पूर्णाङ्क batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);
    पूर्णाङ्क द्विमान = option_find_int_quiet(options, "binary", 0);
    पूर्णाङ्क xnor = option_find_int_quiet(options, "xnor", 0);

    convolutional_layer layer = make_convolutional_layer(batch,h,w,c,n,groups,आकार,stride,padding,activation, batch_normalize, द्विमान, xnor, params.net->adam);
    layer.flipped = option_find_int_quiet(options, "flipped", 0);
    layer.dot = option_find_float_quiet(options, "dot", 0);

    प्रत्यागम layer;
}

layer parse_crnn(list *options, आकार_params params)
{
    पूर्णाङ्क output_filters = option_find_int(options, "output_filters",1);
    पूर्णाङ्क hidden_filters = option_find_int(options, "hidden_filters",1);
    वर्ण *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    पूर्णाङ्क batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_crnn_layer(params.batch, params.w, params.h, params.c, hidden_filters, output_filters, params.time_चरणाः, activation, batch_normalize);

    l.shortcut = option_find_int_quiet(options, "shortcut", 0);

    प्रत्यागम l;
}

layer parse_rnn(list *options, आकार_params params)
{
    पूर्णाङ्क output = option_find_int(options, "output",1);
    वर्ण *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    पूर्णाङ्क batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_rnn_layer(params.batch, params.inputs, output, params.time_चरणाः, activation, batch_normalize, params.net->adam);

    l.shortcut = option_find_int_quiet(options, "shortcut", 0);

    प्रत्यागम l;
}

layer parse_gru(list *options, आकार_params params)
{
    पूर्णाङ्क output = option_find_int(options, "output",1);
    पूर्णाङ्क batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_gru_layer(params.batch, params.inputs, output, params.time_चरणाः, batch_normalize, params.net->adam);
    l.tanh = option_find_int_quiet(options, "tanh", 0);

    प्रत्यागम l;
}

layer parse_lstm(list *options, आकार_params params)
{
    पूर्णाङ्क output = option_find_int(options, "output", 1);
    पूर्णाङ्क batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_lstm_layer(params.batch, params.inputs, output, params.time_चरणाः, batch_normalize, params.net->adam);

    प्रत्यागम l;
}

layer parse_connected(list *options, आकार_params params)
{
    पूर्णाङ्क output = option_find_int(options, "output",1);
    वर्ण *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    पूर्णाङ्क batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_connected_layer(params.batch, params.inputs, output, activation, batch_normalize, params.net->adam);
    प्रत्यागम l;
}

layer parse_softmax(list *options, आकार_params params)
{
    पूर्णाङ्क groups = option_find_int_quiet(options, "groups",1);
    layer l = make_softmax_layer(params.batch, params.inputs, groups);
    l.temperature = option_find_float_quiet(options, "temperature", 1);
    वर्ण *वृक्षः_file = option_find_str(options, "tree", 0);
    यदि (वृक्षः_file) l.softmax_वृक्षः = read_वृक्षः(वृक्षः_file);
    l.w = params.w;
    l.h = params.h;
    l.c = params.c;
    l.spatial = option_find_float_quiet(options, "spatial", 0);
    l.noloss =  option_find_int_quiet(options, "noloss", 0);
    प्रत्यागम l;
}

पूर्णाङ्क *parse_yolo_आवरणम्(वर्ण *a, पूर्णाङ्क *अङ्कः)
{
    पूर्णाङ्क *आवरणम् = 0;
    यदि(a){
        पूर्णाङ्क len = वाक्यदैर्घ्यम्(a);
        पूर्णाङ्क n = 1;
        पूर्णाङ्क i;
        प्रति(i = 0; i < len; ++i){
            यदि (a[i] == ',') ++n;
        }
        आवरणम् = शून्यस्मृतिं_देहि(n, परिमाण(पूर्णाङ्क));
        प्रति(i = 0; i < n; ++i){
            पूर्णाङ्क val = atoi(a);
            आवरणम्[i] = val;
            a = strchr(a, ',')+1;
        }
        *अङ्कः = n;
    }
    प्रत्यागम आवरणम्;
}

layer parse_yolo(list *options, आकार_params params)
{
    पूर्णाङ्क classes = option_find_int(options, "classes", 20);
    पूर्णाङ्क योग_2 = option_find_int(options, "num", 1);
    पूर्णाङ्क अङ्कः = योग_2;

    वर्ण *a = option_find_str(options, "mask", 0);
    पूर्णाङ्क *आवरणम् = parse_yolo_आवरणम्(a, &अङ्कः);
    layer l = make_yolo_layer(params.batch, params.w, params.h, अङ्कः, योग_2, आवरणम्, classes);
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
        पूर्णाङ्क len = वाक्यदैर्घ्यम्(a);
        पूर्णाङ्क n = 1;
        पूर्णाङ्क i;
        प्रति(i = 0; i < len; ++i){
            यदि (a[i] == ',') ++n;
        }
        प्रति(i = 0; i < n; ++i){
            भिन्न bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    प्रत्यागम l;
}

layer parse_iseg(list *options, आकार_params params)
{
    पूर्णाङ्क classes = option_find_int(options, "classes", 20);
    पूर्णाङ्क ids = option_find_int(options, "ids", 32);
    layer l = make_iseg_layer(params.batch, params.w, params.h, classes, ids);
    प्रतिज्ञा(l.outputs == params.inputs);
    प्रत्यागम l;
}

layer parse_region(list *options, आकार_params params)
{
    पूर्णाङ्क coords = option_find_int(options, "coords", 4);
    पूर्णाङ्क classes = option_find_int(options, "classes", 20);
    पूर्णाङ्क अङ्कः = option_find_int(options, "num", 1);

    layer l = make_region_layer(params.batch, params.w, params.h, अङ्कः, classes, coords);
    प्रतिज्ञा(l.outputs == params.inputs);

    l.लघुगणकः = option_find_int_quiet(options, "log", 0);
    l.वर्गमूलम् = option_find_int_quiet(options, "sqrt", 0);

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
    l.आवरणम्_scale = option_find_float(options, "mask_scale", 1);
    l.class_scale = option_find_float(options, "class_scale", 1);
    l.bias_मेलनम् = option_find_int_quiet(options, "bias_match",0);

    वर्ण *वृक्षः_file = option_find_str(options, "tree", 0);
    यदि (वृक्षः_file) l.softmax_वृक्षः = read_वृक्षः(वृक्षः_file);
    वर्ण *map_file = option_find_str(options, "map", 0);
    यदि (map_file) l.map = read_map(map_file);

    वर्ण *a = option_find_str(options, "anchors", 0);
    यदि(a){
        पूर्णाङ्क len = वाक्यदैर्घ्यम्(a);
        पूर्णाङ्क n = 1;
        पूर्णाङ्क i;
        प्रति(i = 0; i < len; ++i){
            यदि (a[i] == ',') ++n;
        }
        प्रति(i = 0; i < n; ++i){
            भिन्न bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    प्रत्यागम l;
}

detection_layer parse_detection(list *options, आकार_params params)
{
    पूर्णाङ्क coords = option_find_int(options, "coords", 1);
    पूर्णाङ्क classes = option_find_int(options, "classes", 1);
    पूर्णाङ्क rescore = option_find_int(options, "rescore", 0);
    पूर्णाङ्क अङ्कः = option_find_int(options, "num", 1);
    पूर्णाङ्क side = option_find_int(options, "side", 7);
    detection_layer layer = make_detection_layer(params.batch, params.inputs, अङ्कः, side, classes, coords, rescore);

    layer.softmax = option_find_int(options, "softmax", 0);
    layer.वर्गमूलम् = option_find_int(options, "sqrt", 0);

    layer.max_boxes = option_find_int_quiet(options, "max",90);
    layer.coord_scale = option_find_float(options, "coord_scale", 1);
    layer.forced = option_find_int(options, "forced", 0);
    layer.object_scale = option_find_float(options, "object_scale", 1);
    layer.noobject_scale = option_find_float(options, "noobject_scale", 1);
    layer.class_scale = option_find_float(options, "class_scale", 1);
    layer.jitter = option_find_float(options, "jitter", .2);
    layer.random = option_find_int_quiet(options, "random", 0);
    layer.reorg = option_find_int_quiet(options, "reorg", 0);
    प्रत्यागम layer;
}

cost_layer parse_cost(list *options, आकार_params params)
{
    वर्ण *type_s = option_find_str(options, "type", "sse");
    COST_TYPE type = get_cost_type(type_s);
    भिन्न scale = option_find_float_quiet(options, "scale",1);
    cost_layer layer = make_cost_layer(params.batch, params.inputs, type, scale);
    layer.अनुपातः =  option_find_float_quiet(options, "ratio",0);
    layer.noobject_scale =  option_find_float_quiet(options, "noobj", 1);
    layer.thresh =  option_find_float_quiet(options, "thresh",0);
    प्रत्यागम layer;
}

crop_layer parse_crop(list *options, आकार_params params)
{
    पूर्णाङ्क crop_height = option_find_int(options, "crop_height",1);
    पूर्णाङ्क crop_विस्तारः = option_find_int(options, "crop_width",1);
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

    crop_layer l = make_crop_layer(batch,h,w,c,crop_height,crop_विस्तारः,flip, angle, saturation, exposure);
    l.सरणम् = option_find_float(options, "shift", 0);
    l.noadjust = noadjust;
    प्रत्यागम l;
}

layer parse_reorg(list *options, आकार_params params)
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
    प्रत्यागम layer;
}

maxpool_layer parse_maxpool(list *options, आकार_params params)
{
    पूर्णाङ्क stride = option_find_int(options, "stride",1);
    पूर्णाङ्क आकार = option_find_int(options, "size",stride);
    पूर्णाङ्क padding = option_find_int_quiet(options, "padding", आकार-1);

    पूर्णाङ्क batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    यदि(!(h && w && c)) error("Layer before maxpool layer must output image.");

    maxpool_layer layer = make_maxpool_layer(batch,h,w,c,आकार,stride,padding);
    प्रत्यागम layer;
}

avgpool_layer parse_avgpool(list *options, आकार_params params)
{
    पूर्णाङ्क batch,w,h,c;
    w = params.w;
    h = params.h;
    c = params.c;
    batch=params.batch;
    यदि(!(h && w && c)) error("Layer before avgpool layer must output image.");

    avgpool_layer layer = make_avgpool_layer(batch,w,h,c);
    प्रत्यागम layer;
}

dropout_layer parse_dropout(list *options, आकार_params params)
{
    भिन्न probability = option_find_float(options, "probability", .5);
    dropout_layer layer = make_dropout_layer(params.batch, params.inputs, probability);
    layer.बहिर्गमनम्_w = params.w;
    layer.बहिर्गमनम्_h = params.h;
    layer.बहिर्गमनम्_c = params.c;
    प्रत्यागम layer;
}

layer parse_normalization(list *options, आकार_params params)
{
    भिन्न alpha = option_find_float(options, "alpha", .0001);
    भिन्न beta =  option_find_float(options, "beta" , .75);
    भिन्न kappa = option_find_float(options, "kappa", 1);
    पूर्णाङ्क आकार = option_find_int(options, "size", 5);
    layer l = make_normalization_layer(params.batch, params.w, params.h, params.c, आकार, alpha, beta, kappa);
    प्रत्यागम l;
}

layer parse_batchnorm(list *options, आकार_params params)
{
    layer l = make_batchnorm_layer(params.batch, params.w, params.h, params.c);
    प्रत्यागम l;
}

layer parse_shortcut(list *options, आकार_params params, जालम् *net)
{
    वर्ण *l = option_find(options, "from");
    पूर्णाङ्क सूचकांक = atoi(l);
    यदि(सूचकांक < 0) सूचकांक = params.सूचकांक + सूचकांक;

    पूर्णाङ्क batch = params.batch;
    layer from = net->layers[सूचकांक];

    layer s = make_shortcut_layer(batch, सूचकांक, params.w, params.h, params.c, from.बहिर्गमनम्_w, from.बहिर्गमनम्_h, from.बहिर्गमनम्_c);

    वर्ण *activation_s = option_find_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);
    s.activation = activation;
    s.alpha = option_find_float_quiet(options, "alpha", 1);
    s.beta = option_find_float_quiet(options, "beta", 1);
    प्रत्यागम s;
}


layer parse_l2norm(list *options, आकार_params params)
{
    layer l = make_l2norm_layer(params.batch, params.inputs);
    l.h = l.बहिर्गमनम्_h = params.h;
    l.w = l.बहिर्गमनम्_w = params.w;
    l.c = l.बहिर्गमनम्_c = params.c;
    प्रत्यागम l;
}


layer parse_logistic(list *options, आकार_params params)
{
    layer l = make_logistic_layer(params.batch, params.inputs);
    l.h = l.बहिर्गमनम्_h = params.h;
    l.w = l.बहिर्गमनम्_w = params.w;
    l.c = l.बहिर्गमनम्_c = params.c;
    प्रत्यागम l;
}

layer parse_activation(list *options, आकार_params params)
{
    वर्ण *activation_s = option_find_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);

    layer l = make_activation_layer(params.batch, params.inputs, activation);

    l.h = l.बहिर्गमनम्_h = params.h;
    l.w = l.बहिर्गमनम्_w = params.w;
    l.c = l.बहिर्गमनम्_c = params.c;

    प्रत्यागम l;
}

layer parse_upsample(list *options, आकार_params params, जालम् *net)
{

    पूर्णाङ्क stride = option_find_int(options, "stride",2);
    layer l = make_upsample_layer(params.batch, params.w, params.h, params.c, stride);
    l.scale = option_find_float_quiet(options, "scale", 1);
    प्रत्यागम l;
}

route_layer parse_route(list *options, आकार_params params, जालम् *net)
{
    वर्ण *l = option_find(options, "layers");
    पूर्णाङ्क len = वाक्यदैर्घ्यम्(l);
    यदि(!l) error("Route Layer must specify input layers");
    पूर्णाङ्क n = 1;
    पूर्णाङ्क i;
    प्रति(i = 0; i < len; ++i){
        यदि (l[i] == ',') ++n;
    }

    पूर्णाङ्क *layers = शून्यस्मृतिं_देहि(n, परिमाण(पूर्णाङ्क));
    पूर्णाङ्क *sizes = शून्यस्मृतिं_देहि(n, परिमाण(पूर्णाङ्क));
    प्रति(i = 0; i < n; ++i){
        पूर्णाङ्क सूचकांक = atoi(l);
        l = strchr(l, ',')+1;
        यदि(सूचकांक < 0) सूचकांक = params.सूचकांक + सूचकांक;
        layers[i] = सूचकांक;
        sizes[i] = net->layers[सूचकांक].outputs;
    }
    पूर्णाङ्क batch = params.batch;

    route_layer layer = make_route_layer(batch, n, layers, sizes);

    convolutional_layer first = net->layers[layers[0]];
    layer.बहिर्गमनम्_w = first.बहिर्गमनम्_w;
    layer.बहिर्गमनम्_h = first.बहिर्गमनम्_h;
    layer.बहिर्गमनम्_c = first.बहिर्गमनम्_c;
    प्रति(i = 1; i < n; ++i){
        पूर्णाङ्क सूचकांक = layers[i];
        convolutional_layer अग्र = net->layers[सूचकांक];
        यदि(अग्र.बहिर्गमनम्_w == first.बहिर्गमनम्_w && अग्र.बहिर्गमनम्_h == first.बहिर्गमनम्_h){
            layer.बहिर्गमनम्_c += अग्र.बहिर्गमनम्_c;
        }अन्यथा{
            layer.बहिर्गमनम्_h = layer.बहिर्गमनम्_w = layer.बहिर्गमनम्_c = 0;
        }
    }

    प्रत्यागम layer;
}

learning_दरः_policy get_policy(वर्ण *s)
{
    यदि (वाक्यं_तुलय(s, "random")==0) प्रत्यागम RANDOM;
    यदि (वाक्यं_तुलय(s, "poly")==0) प्रत्यागम POLY;
    यदि (वाक्यं_तुलय(s, "constant")==0) प्रत्यागम CONSTANT;
    यदि (वाक्यं_तुलय(s, "step")==0) प्रत्यागम चरणः_2;
    यदि (वाक्यं_तुलय(s, "exp")==0) प्रत्यागम EXP;
    यदि (वाक्यं_तुलय(s, "sigmoid")==0) प्रत्यागम SIG;
    यदि (वाक्यं_तुलय(s, "steps")==0) प्रत्यागम चरणाः_2;
    fprintf(stderr, "Couldn't find policy %s, going with constant\n", s);
    प्रत्यागम CONSTANT;
}

शून्य parse_net_options(list *options, जालम् *net)
{
    net->batch = option_find_int(options, "batch",1);
    net->learning_दरः = option_find_float(options, "learning_rate", .001);
    net->momentum = option_find_float(options, "momentum", .9);
    net->decay = option_find_float(options, "decay", .0001);
    पूर्णाङ्क subdivs = option_find_int(options, "subdivisions",1);
    net->time_चरणाः = option_find_int_quiet(options, "time_steps",1);
    net->notruth = option_find_int_quiet(options, "notruth",0);
    net->batch /= subdivs;
    net->batch *= net->time_चरणाः;
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
    net->max_अनुपातः = option_find_float_quiet(options, "max_ratio", (भिन्न) net->max_crop / net->w);
    net->min_अनुपातः = option_find_float_quiet(options, "min_ratio", (भिन्न) net->min_crop / net->w);
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
    net->burn_अन्तर् = option_find_int_quiet(options, "burn_in", 0);
    net->power = option_find_float_quiet(options, "power", 4);
    यदि(net->policy == चरणः_2){
        net->चरणः = option_find_int(options, "step", 1);
        net->scale = option_find_float(options, "scale", 1);
    } अन्यथा यदि (net->policy == चरणाः_2){
        वर्ण *l = option_find(options, "steps");
        वर्ण *p = option_find(options, "scales");
        यदि(!l || !p) error("STEPS policy must have steps and scales in cfg file");

        पूर्णाङ्क len = वाक्यदैर्घ्यम्(l);
        पूर्णाङ्क n = 1;
        पूर्णाङ्क i;
        प्रति(i = 0; i < len; ++i){
            यदि (l[i] == ',') ++n;
        }
        पूर्णाङ्क *चरणाः = शून्यस्मृतिं_देहि(n, परिमाण(पूर्णाङ्क));
        भिन्न *scales = शून्यस्मृतिं_देहि(n, परिमाण(भिन्न));
        प्रति(i = 0; i < n; ++i){
            पूर्णाङ्क चरणः    = atoi(l);
            भिन्न scale = atof(p);
            l = strchr(l, ',')+1;
            p = strchr(p, ',')+1;
            चरणाः[i] = चरणः;
            scales[i] = scale;
        }
        net->scales = scales;
        net->चरणाः = चरणाः;
        net->अङ्कः_चरणाः = n;
    } अन्यथा यदि (net->policy == EXP){
        net->gamma = option_find_float(options, "gamma", 1);
    } अन्यथा यदि (net->policy == SIG){
        net->gamma = option_find_float(options, "gamma", 1);
        net->चरणः = option_find_int(options, "step", 1);
    } अन्यथा यदि (net->policy == POLY || net->policy == RANDOM){
    }
    net->max_batches = option_find_int(options, "max_batches", 0);
}

पूर्णाङ्क is_जालम्(section *s)
{
    प्रत्यागम (वाक्यं_तुलय(s->type, "[net]")==0
            || वाक्यं_तुलय(s->type, "[network]")==0);
}

जालम् *parse_जालम्_cfg(वर्ण *filename)
{
    list *sections = read_cfg(filename);
    ग्रन्थिः *n = sections->front;
    यदि(!n) error("Config file has no sections");
    जालम् *net = make_जालम्(sections->आकार - 1);
    net->gpu_सूचकांक = gpu_सूचकांक;
    आकार_params params;

    section *s = (section *)n->val;
    list *options = s->options;
    यदि(!is_जालम्(s)) error("First section must be [net] or [network]");
    parse_net_options(options, net);

    params.h = net->h;
    params.w = net->w;
    params.c = net->c;
    params.inputs = net->inputs;
    params.batch = net->batch;
    params.time_चरणाः = net->time_चरणाः;
    params.net = net;

    size_t workspace_आकार = 0;
    n = n->अग्र;
    पूर्णाङ्क गणनम् = 0;
    free_section(s);
    fprintf(stderr, "layer     filters    size              input                output\n");
    यावत्(n){
        params.सूचकांक = गणनम्;
        fprintf(stderr, "%5d ", गणनम्);
        s = (section *)n->val;
        options = s->options;
        layer l = {0};
        LAYER_TYPE lt = string_गन्तव्यम्_layer_type(s->type);
        यदि(lt == CONVOLUTIONAL){
            l = parse_convolutional(options, params);
        }अन्यथा यदि(lt == DECONVOLUTIONAL){
            l = parse_deconvolutional(options, params);
        }अन्यथा यदि(lt == LOCAL){
            l = parse_local(options, params);
        }अन्यथा यदि(lt == सक्रिय){
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
            net->hierarchy = l.softmax_वृक्षः;
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
            l.output = net->layers[गणनम्-1].output;
            l.delta = net->layers[गणनम्-1].delta;
#यदि_परिभाषितम् GPU
            l.output_gpu = net->layers[गणनम्-1].output_gpu;
            l.delta_gpu = net->layers[गणनम्-1].delta_gpu;
#यदि_अन्तः
        }अन्यथा{
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
        l.learning_दरः_scale = option_find_float_quiet(options, "learning_rate", 1);
        l.smooth = option_find_float_quiet(options, "smooth", 0);
        option_अप्रयुक्तम्(options);
        net->layers[गणनम्] = l;
        यदि (l.workspace_आकार > workspace_आकार) workspace_आकार = l.workspace_आकार;
        free_section(s);
        n = n->अग्र;
        ++गणनम्;
        यदि(n){
            params.h = l.बहिर्गमनम्_h;
            params.w = l.बहिर्गमनम्_w;
            params.c = l.बहिर्गमनम्_c;
            params.inputs = l.outputs;
        }
    }
    free_list(sections);
    layer बहिर्गमनम् = get_जालम्_output_layer(net);
    net->outputs = बहिर्गमनम्.outputs;
    net->truths = बहिर्गमनम्.outputs;
    यदि(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = बहिर्गमनम्.output;
    net->निवेशः = शून्यस्मृतिं_देहि(net->inputs*net->batch, परिमाण(भिन्न));
    net->truth = शून्यस्मृतिं_देहि(net->truths*net->batch, परिमाण(भिन्न));
#यदि_परिभाषितम् GPU
    net->output_gpu = बहिर्गमनम्.output_gpu;
    net->निवेशः_gpu = cuda_make_array(net->निवेशः, net->inputs*net->batch);
    net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
#यदि_अन्तः
    यदि(workspace_आकार){
        //printf("%ld\n", workspace_size);
#यदि_परिभाषितम् GPU
        यदि(gpu_सूचकांक >= 0){
            net->workspace = cuda_make_array(0, (workspace_आकार-1)/परिमाण(भिन्न)+1);
        }अन्यथा {
            net->workspace = शून्यस्मृतिं_देहि(1, workspace_आकार);
        }
#अन्यथा
        net->workspace = शून्यस्मृतिं_देहि(1, workspace_आकार);
#यदि_अन्तः
    }
    प्रत्यागम net;
}

list *read_cfg(वर्ण *filename)
{
    FILE *file = fopen(filename, "r");
    यदि(file == 0) file_error(filename);
    वर्ण *पङ्क्तिः;
    पूर्णाङ्क nu = 0;
    list *options = make_list();
    section *current = 0;
    यावत्((पङ्क्तिः=fgetl(file)) != 0){
        ++ nu;
        strip(पङ्क्तिः);
        विकल्प(पङ्क्तिः[0]){
            पक्ष '[':
                current = स्मृतिं_देहि(परिमाण(section));
                list_निवेशय(options, current);
                current->options = make_list();
                current->type = पङ्क्तिः;
                भङ्ग;
            पक्ष '\0':
            पक्ष '#':
            पक्ष ';':
                मुक्त(पङ्क्तिः);
                भङ्ग;
            स्वभाव:
                यदि(!read_option(पङ्क्तिः, current->options)){
                    fprintf(stderr, "Config file error line %d, could parse: %s\n", nu, पङ्क्तिः);
                    मुक्त(पङ्क्तिः);
                }
                भङ्ग;
        }
    }
    fclose(file);
    प्रत्यागम options;
}

शून्य save_convolutional_weights_द्विमान(layer l, FILE *fp)
{
#यदि_परिभाषितम् GPU
    यदि(gpu_सूचकांक >= 0){
        pull_convolutional_layer(l);
    }
#यदि_अन्तः
    binarize_weights(l.weights, l.n, l.c*l.आकार*l.आकार, l.द्विमान_weights);
    पूर्णाङ्क आकार = l.c*l.आकार*l.आकार;
    पूर्णाङ्क i, j, k;
    fwrite(l.biases, परिमाण(भिन्न), l.n, fp);
    यदि (l.batch_normalize){
        fwrite(l.scales, परिमाण(भिन्न), l.n, fp);
        fwrite(l.rolling_माध्यम्, परिमाण(भिन्न), l.n, fp);
        fwrite(l.rolling_variance, परिमाण(भिन्न), l.n, fp);
    }
    प्रति(i = 0; i < l.n; ++i){
        भिन्न माध्यम् = l.द्विमान_weights[i*आकार];
        यदि(माध्यम् < 0) माध्यम् = -माध्यम्;
        fwrite(&माध्यम्, परिमाण(भिन्न), 1, fp);
        प्रति(j = 0; j < आकार/8; ++j){
            पूर्णाङ्क सूचकांक = i*आकार + j*8;
            अचिह्नित वर्ण c = 0;
            प्रति(k = 0; k < 8; ++k){
                यदि (j*8 + k >= आकार) भङ्ग;
                यदि (l.द्विमान_weights[सूचकांक + k] > 0) c = (c | 1<<k);
            }
            fwrite(&c, परिमाण(वर्ण), 1, fp);
        }
    }
}

शून्य save_convolutional_weights(layer l, FILE *fp)
{
    यदि(l.द्विमान){
        //save_convolutional_weights_binary(l, fp);
        //return;
    }
#यदि_परिभाषितम् GPU
    यदि(gpu_सूचकांक >= 0){
        pull_convolutional_layer(l);
    }
#यदि_अन्तः
    पूर्णाङ्क अङ्कः = l.nweights;
    fwrite(l.biases, परिमाण(भिन्न), l.n, fp);
    यदि (l.batch_normalize){
        fwrite(l.scales, परिमाण(भिन्न), l.n, fp);
        fwrite(l.rolling_माध्यम्, परिमाण(भिन्न), l.n, fp);
        fwrite(l.rolling_variance, परिमाण(भिन्न), l.n, fp);
    }
    fwrite(l.weights, परिमाण(भिन्न), अङ्कः, fp);
}

शून्य save_batchnorm_weights(layer l, FILE *fp)
{
#यदि_परिभाषितम् GPU
    यदि(gpu_सूचकांक >= 0){
        pull_batchnorm_layer(l);
    }
#यदि_अन्तः
    fwrite(l.scales, परिमाण(भिन्न), l.c, fp);
    fwrite(l.rolling_माध्यम्, परिमाण(भिन्न), l.c, fp);
    fwrite(l.rolling_variance, परिमाण(भिन्न), l.c, fp);
}

शून्य save_connected_weights(layer l, FILE *fp)
{
#यदि_परिभाषितम् GPU
    यदि(gpu_सूचकांक >= 0){
        pull_connected_layer(l);
    }
#यदि_अन्तः
    fwrite(l.biases, परिमाण(भिन्न), l.outputs, fp);
    fwrite(l.weights, परिमाण(भिन्न), l.outputs*l.inputs, fp);
    यदि (l.batch_normalize){
        fwrite(l.scales, परिमाण(भिन्न), l.outputs, fp);
        fwrite(l.rolling_माध्यम्, परिमाण(भिन्न), l.outputs, fp);
        fwrite(l.rolling_variance, परिमाण(भिन्न), l.outputs, fp);
    }
}

शून्य save_weights_upto(जालम् *net, वर्ण *filename, पूर्णाङ्क cutoff)
{
#यदि_परिभाषितम् GPU
    यदि(net->gpu_सूचकांक >= 0){
        cuda_समुच्चयः_device(net->gpu_सूचकांक);
    }
#यदि_अन्तः
    fprintf(stderr, "Saving weights to %s\n", filename);
    FILE *fp = fopen(filename, "wb");
    यदि(!fp) file_error(filename);

    पूर्णाङ्क major = 0;
    पूर्णाङ्क minor = 2;
    पूर्णाङ्क revision = 0;
    fwrite(&major, परिमाण(पूर्णाङ्क), 1, fp);
    fwrite(&minor, परिमाण(पूर्णाङ्क), 1, fp);
    fwrite(&revision, परिमाण(पूर्णाङ्क), 1, fp);
    fwrite(net->आलोकितम्, परिमाण(size_t), 1, fp);

    पूर्णाङ्क i;
    प्रति(i = 0; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        यदि (l.dontsave) अनुवर्त;
        यदि(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            save_convolutional_weights(l, fp);
        } यदि(l.type == CONNECTED){
            save_connected_weights(l, fp);
        } यदि(l.type == BATCHNORM){
            save_batchnorm_weights(l, fp);
        } यदि(l.type == RNN){
            save_connected_weights(*(l.निवेशः_layer), fp);
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
            save_convolutional_weights(*(l.निवेशः_layer), fp);
            save_convolutional_weights(*(l.self_layer), fp);
            save_convolutional_weights(*(l.output_layer), fp);
        } यदि(l.type == LOCAL){
#यदि_परिभाषितम् GPU
            यदि(gpu_सूचकांक >= 0){
                pull_local_layer(l);
            }
#यदि_अन्तः
            पूर्णाङ्क locations = l.बहिर्गमनम्_w*l.बहिर्गमनम्_h;
            पूर्णाङ्क आकार = l.आकार*l.आकार*l.c*l.n*locations;
            fwrite(l.biases, परिमाण(भिन्न), l.outputs, fp);
            fwrite(l.weights, परिमाण(भिन्न), आकार, fp);
        }
    }
    fclose(fp);
}
शून्य save_weights(जालम् *net, वर्ण *filename)
{
    save_weights_upto(net, filename, net->n);
}

शून्य transpose_matrix(भिन्न *a, पूर्णाङ्क पङ्क्तिकाः, पूर्णाङ्क स्तम्भसङ्ख्या)
{
    भिन्न *transpose = शून्यस्मृतिं_देहि(पङ्क्तिकाः*स्तम्भसङ्ख्या, परिमाण(भिन्न));
    पूर्णाङ्क x, y;
    प्रति(x = 0; x < पङ्क्तिकाः; ++x){
        प्रति(y = 0; y < स्तम्भसङ्ख्या; ++y){
            transpose[y*पङ्क्तिकाः + x] = a[x*स्तम्भसङ्ख्या + y];
        }
    }
    स्मृतिं_प्रतिलिख(a, transpose, पङ्क्तिकाः*स्तम्भसङ्ख्या*परिमाण(भिन्न));
    मुक्त(transpose);
}

शून्य आरोपय_connected_weights(layer l, FILE *fp, पूर्णाङ्क transpose)
{
    fread(l.biases, परिमाण(भिन्न), l.outputs, fp);
    fread(l.weights, परिमाण(भिन्न), l.outputs*l.inputs, fp);
    यदि(transpose){
        transpose_matrix(l.weights, l.inputs, l.outputs);
    }
    //printf("Biases: %f mean %f variance\n", mean_array(l.biases, l.outputs), variance_array(l.biases, l.outputs));
    //printf("Weights: %f mean %f variance\n", mean_array(l.weights, l.outputs*l.inputs), variance_array(l.weights, l.outputs*l.inputs));
    यदि (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, परिमाण(भिन्न), l.outputs, fp);
        fread(l.rolling_माध्यम्, परिमाण(भिन्न), l.outputs, fp);
        fread(l.rolling_variance, परिमाण(भिन्न), l.outputs, fp);
        //printf("Scales: %f mean %f variance\n", mean_array(l.scales, l.outputs), variance_array(l.scales, l.outputs));
        //printf("rolling_mean: %f mean %f variance\n", mean_array(l.rolling_mean, l.outputs), variance_array(l.rolling_mean, l.outputs));
        //printf("rolling_variance: %f mean %f variance\n", mean_array(l.rolling_variance, l.outputs), variance_array(l.rolling_variance, l.outputs));
    }
#यदि_परिभाषितम् GPU
    यदि(gpu_सूचकांक >= 0){
        प्रक्षिप_connected_layer(l);
    }
#यदि_अन्तः
}

शून्य आरोपय_batchnorm_weights(layer l, FILE *fp)
{
    fread(l.scales, परिमाण(भिन्न), l.c, fp);
    fread(l.rolling_माध्यम्, परिमाण(भिन्न), l.c, fp);
    fread(l.rolling_variance, परिमाण(भिन्न), l.c, fp);
#यदि_परिभाषितम् GPU
    यदि(gpu_सूचकांक >= 0){
        प्रक्षिप_batchnorm_layer(l);
    }
#यदि_अन्तः
}

शून्य आरोपय_convolutional_weights_द्विमान(layer l, FILE *fp)
{
    fread(l.biases, परिमाण(भिन्न), l.n, fp);
    यदि (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, परिमाण(भिन्न), l.n, fp);
        fread(l.rolling_माध्यम्, परिमाण(भिन्न), l.n, fp);
        fread(l.rolling_variance, परिमाण(भिन्न), l.n, fp);
    }
    पूर्णाङ्क आकार = l.c*l.आकार*l.आकार;
    पूर्णाङ्क i, j, k;
    प्रति(i = 0; i < l.n; ++i){
        भिन्न माध्यम् = 0;
        fread(&माध्यम्, परिमाण(भिन्न), 1, fp);
        प्रति(j = 0; j < आकार/8; ++j){
            पूर्णाङ्क सूचकांक = i*आकार + j*8;
            अचिह्नित वर्ण c = 0;
            fread(&c, परिमाण(वर्ण), 1, fp);
            प्रति(k = 0; k < 8; ++k){
                यदि (j*8 + k >= आकार) भङ्ग;
                l.weights[सूचकांक + k] = (c & 1<<k) ? माध्यम् : -माध्यम्;
            }
        }
    }
#यदि_परिभाषितम् GPU
    यदि(gpu_सूचकांक >= 0){
        प्रक्षिप_convolutional_layer(l);
    }
#यदि_अन्तः
}

शून्य आरोपय_convolutional_weights(layer l, FILE *fp)
{
    यदि(l.द्विमान){
        //load_convolutional_weights_binary(l, fp);
        //return;
    }
    यदि(l.numload) l.n = l.numload;
    पूर्णाङ्क अङ्कः = l.c/l.groups*l.n*l.आकार*l.आकार;
    fread(l.biases, परिमाण(भिन्न), l.n, fp);
    यदि (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, परिमाण(भिन्न), l.n, fp);
        fread(l.rolling_माध्यम्, परिमाण(भिन्न), l.n, fp);
        fread(l.rolling_variance, परिमाण(भिन्न), l.n, fp);
        यदि(0){
            पूर्णाङ्क i;
            प्रति(i = 0; i < l.n; ++i){
                प्रारूप_लिख("%g, ", l.rolling_माध्यम्[i]);
            }
            प्रारूप_लिख("\n");
            प्रति(i = 0; i < l.n; ++i){
                प्रारूप_लिख("%g, ", l.rolling_variance[i]);
            }
            प्रारूप_लिख("\n");
        }
        यदि(0){
            fill_cpu(l.n, 0, l.rolling_माध्यम्, 1);
            fill_cpu(l.n, 0, l.rolling_variance, 1);
        }
        यदि(0){
            पूर्णाङ्क i;
            प्रति(i = 0; i < l.n; ++i){
                प्रारूप_लिख("%g, ", l.rolling_माध्यम्[i]);
            }
            प्रारूप_लिख("\n");
            प्रति(i = 0; i < l.n; ++i){
                प्रारूप_लिख("%g, ", l.rolling_variance[i]);
            }
            प्रारूप_लिख("\n");
        }
    }
    fread(l.weights, परिमाण(भिन्न), अङ्कः, fp);
    //if(l.c == 3) scal_cpu(num, 1./256, l.weights, 1);
    यदि (l.flipped) {
        transpose_matrix(l.weights, l.c*l.आकार*l.आकार, l.n);
    }
    //if (l.binary) binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.weights);
#यदि_परिभाषितम् GPU
    यदि(gpu_सूचकांक >= 0){
        प्रक्षिप_convolutional_layer(l);
    }
#यदि_अन्तः
}


शून्य आरोपय_weights_upto(जालम् *net, वर्ण *filename, पूर्णाङ्क start, पूर्णाङ्क cutoff)
{
#यदि_परिभाषितम् GPU
    यदि(net->gpu_सूचकांक >= 0){
        cuda_समुच्चयः_device(net->gpu_सूचकांक);
    }
#यदि_अन्तः
    fprintf(stderr, "Loading weights from %s...", filename);
    fflush(stdout);
    FILE *fp = fopen(filename, "rb");
    यदि(!fp) file_error(filename);

    पूर्णाङ्क major;
    पूर्णाङ्क minor;
    पूर्णाङ्क revision;
    fread(&major, परिमाण(पूर्णाङ्क), 1, fp);
    fread(&minor, परिमाण(पूर्णाङ्क), 1, fp);
    fread(&revision, परिमाण(पूर्णाङ्क), 1, fp);
    यदि ((major*10 + minor) >= 2 && major < 1000 && minor < 1000){
        fread(net->आलोकितम्, परिमाण(size_t), 1, fp);
    } अन्यथा {
        पूर्णाङ्क iseen = 0;
        fread(&iseen, परिमाण(पूर्णाङ्क), 1, fp);
        *net->आलोकितम् = iseen;
    }
    पूर्णाङ्क transpose = (major > 1000) || (minor > 1000);

    पूर्णाङ्क i;
    प्रति(i = start; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        यदि (l.dontload) अनुवर्त;
        यदि(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            आरोपय_convolutional_weights(l, fp);
        }
        यदि(l.type == CONNECTED){
            आरोपय_connected_weights(l, fp, transpose);
        }
        यदि(l.type == BATCHNORM){
            आरोपय_batchnorm_weights(l, fp);
        }
        यदि(l.type == CRNN){
            आरोपय_convolutional_weights(*(l.निवेशः_layer), fp);
            आरोपय_convolutional_weights(*(l.self_layer), fp);
            आरोपय_convolutional_weights(*(l.output_layer), fp);
        }
        यदि(l.type == RNN){
            आरोपय_connected_weights(*(l.निवेशः_layer), fp, transpose);
            आरोपय_connected_weights(*(l.self_layer), fp, transpose);
            आरोपय_connected_weights(*(l.output_layer), fp, transpose);
        }
        यदि (l.type == LSTM) {
            आरोपय_connected_weights(*(l.wi), fp, transpose);
            आरोपय_connected_weights(*(l.wf), fp, transpose);
            आरोपय_connected_weights(*(l.wo), fp, transpose);
            आरोपय_connected_weights(*(l.wg), fp, transpose);
            आरोपय_connected_weights(*(l.ui), fp, transpose);
            आरोपय_connected_weights(*(l.uf), fp, transpose);
            आरोपय_connected_weights(*(l.uo), fp, transpose);
            आरोपय_connected_weights(*(l.ug), fp, transpose);
        }
        यदि (l.type == GRU) {
            यदि(1){
                आरोपय_connected_weights(*(l.wz), fp, transpose);
                आरोपय_connected_weights(*(l.wr), fp, transpose);
                आरोपय_connected_weights(*(l.wh), fp, transpose);
                आरोपय_connected_weights(*(l.uz), fp, transpose);
                आरोपय_connected_weights(*(l.ur), fp, transpose);
                आरोपय_connected_weights(*(l.uh), fp, transpose);
            }अन्यथा{
                आरोपय_connected_weights(*(l.reset_layer), fp, transpose);
                आरोपय_connected_weights(*(l.update_layer), fp, transpose);
                आरोपय_connected_weights(*(l.state_layer), fp, transpose);
            }
        }
        यदि(l.type == LOCAL){
            पूर्णाङ्क locations = l.बहिर्गमनम्_w*l.बहिर्गमनम्_h;
            पूर्णाङ्क आकार = l.आकार*l.आकार*l.c*l.n*locations;
            fread(l.biases, परिमाण(भिन्न), l.outputs, fp);
            fread(l.weights, परिमाण(भिन्न), आकार, fp);
#यदि_परिभाषितम् GPU
            यदि(gpu_सूचकांक >= 0){
                प्रक्षिप_local_layer(l);
            }
#यदि_अन्तः
        }
    }
    fprintf(stderr, "Done!\n");
    fclose(fp);
}

शून्य आरोपय_weights(जालम् *net, वर्ण *filename)
{
    आरोपय_weights_upto(net, filename, 0, net->n);
}


#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <assert.h>

#ಅನ್ತರ್ಭಾವ "activation_layer.h"
#ಅನ್ತರ್ಭಾವ "logistic_layer.h"
#ಅನ್ತರ್ಭಾವ "l2norm_layer.h"
#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "avgpool_layer.h"
#ಅನ್ತರ್ಭಾವ "batchnorm_layer.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "connected_layer.h"
#ಅನ್ತರ್ಭಾವ "deconvolutional_layer.h"
#ಅನ್ತರ್ಭಾವ "convolutional_layer.h"
#ಅನ್ತರ್ಭಾವ "cost_layer.h"
#ಅನ್ತರ್ಭಾವ "crnn_layer.h"
#ಅನ್ತರ್ಭಾವ "crop_layer.h"
#ಅನ್ತರ್ಭಾವ "detection_layer.h"
#ಅನ್ತರ್ಭಾವ "dropout_layer.h"
#ಅನ್ತರ್ಭಾವ "gru_layer.h"
#ಅನ್ತರ್ಭಾವ "list.h"
#ಅನ್ತರ್ಭಾವ "local_layer.h"
#ಅನ್ತರ್ಭಾವ "maxpool_layer.h"
#ಅನ್ತರ್ಭಾವ "normalization_layer.h"
#ಅನ್ತರ್ಭಾವ "option_list.h"
#ಅನ್ತರ್ಭಾವ "parser.h"
#ಅನ್ತರ್ಭಾವ "region_layer.h"
#ಅನ್ತರ್ಭಾವ "yolo_layer.h"
#ಅನ್ತರ್ಭಾವ "iseg_layer.h"
#ಅನ್ತರ್ಭಾವ "reorg_layer.h"
#ಅನ್ತರ್ಭಾವ "rnn_layer.h"
#ಅನ್ತರ್ಭಾವ "route_layer.h"
#ಅನ್ತರ್ಭಾವ "upsample_layer.h"
#ಅನ್ತರ್ಭಾವ "shortcut_layer.h"
#ಅನ್ತರ್ಭಾವ "softmax_layer.h"
#ಅನ್ತರ್ಭಾವ "lstm_layer.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ{
    ವರ್ಣ *type;
    list *options;
}section;

list *read_cfg(ವರ್ಣ *filename);

LAYER_TYPE string_to_layer_type(ವರ್ಣ * type)
{

    ಯದಿ (strcmp(type, "[shortcut]")==0) ಪ್ರತ್ಯಾಗಮ SHORTCUT;
    ಯದಿ (strcmp(type, "[crop]")==0) ಪ್ರತ್ಯಾಗಮ CROP;
    ಯದಿ (strcmp(type, "[cost]")==0) ಪ್ರತ್ಯಾಗಮ COST;
    ಯದಿ (strcmp(type, "[detection]")==0) ಪ್ರತ್ಯಾಗಮ DETECTION;
    ಯದಿ (strcmp(type, "[region]")==0) ಪ್ರತ್ಯಾಗಮ REGION;
    ಯದಿ (strcmp(type, "[yolo]")==0) ಪ್ರತ್ಯಾಗಮ YOLO;
    ಯದಿ (strcmp(type, "[iseg]")==0) ಪ್ರತ್ಯಾಗಮ ISEG;
    ಯದಿ (strcmp(type, "[local]")==0) ಪ್ರತ್ಯಾಗಮ LOCAL;
    ಯದಿ (strcmp(type, "[conv]")==0
            || strcmp(type, "[convolutional]")==0) ಪ್ರತ್ಯಾಗಮ CONVOLUTIONAL;
    ಯದಿ (strcmp(type, "[deconv]")==0
            || strcmp(type, "[deconvolutional]")==0) ಪ್ರತ್ಯಾಗಮ DECONVOLUTIONAL;
    ಯದಿ (strcmp(type, "[activation]")==0) ಪ್ರತ್ಯಾಗಮ ACTIVE;
    ಯದಿ (strcmp(type, "[logistic]")==0) ಪ್ರತ್ಯಾಗಮ LOGXENT;
    ಯದಿ (strcmp(type, "[l2norm]")==0) ಪ್ರತ್ಯಾಗಮ L2NORM;
    ಯದಿ (strcmp(type, "[net]")==0
            || strcmp(type, "[network]")==0) ಪ್ರತ್ಯಾಗಮ NETWORK;
    ಯದಿ (strcmp(type, "[crnn]")==0) ಪ್ರತ್ಯಾಗಮ CRNN;
    ಯದಿ (strcmp(type, "[gru]")==0) ಪ್ರತ್ಯಾಗಮ GRU;
    ಯದಿ (strcmp(type, "[lstm]") == 0) ಪ್ರತ್ಯಾಗಮ LSTM;
    ಯದಿ (strcmp(type, "[rnn]")==0) ಪ್ರತ್ಯಾಗಮ RNN;
    ಯದಿ (strcmp(type, "[conn]")==0
            || strcmp(type, "[connected]")==0) ಪ್ರತ್ಯಾಗಮ CONNECTED;
    ಯದಿ (strcmp(type, "[max]")==0
            || strcmp(type, "[maxpool]")==0) ಪ್ರತ್ಯಾಗಮ MAXPOOL;
    ಯದಿ (strcmp(type, "[reorg]")==0) ಪ್ರತ್ಯಾಗಮ REORG;
    ಯದಿ (strcmp(type, "[avg]")==0
            || strcmp(type, "[avgpool]")==0) ಪ್ರತ್ಯಾಗಮ AVGPOOL;
    ಯದಿ (strcmp(type, "[dropout]")==0) ಪ್ರತ್ಯಾಗಮ DROPOUT;
    ಯದಿ (strcmp(type, "[lrn]")==0
            || strcmp(type, "[normalization]")==0) ಪ್ರತ್ಯಾಗಮ NORMALIZATION;
    ಯದಿ (strcmp(type, "[batchnorm]")==0) ಪ್ರತ್ಯಾಗಮ BATCHNORM;
    ಯದಿ (strcmp(type, "[soft]")==0
            || strcmp(type, "[softmax]")==0) ಪ್ರತ್ಯಾಗಮ SOFTMAX;
    ಯದಿ (strcmp(type, "[route]")==0) ಪ್ರತ್ಯಾಗಮ ROUTE;
    ಯದಿ (strcmp(type, "[upsample]")==0) ಪ್ರತ್ಯಾಗಮ UPSAMPLE;
    ಪ್ರತ್ಯಾಗಮ BLANK;
}

ಶೂನ್ಯ free_section(section *s)
{
    ಮುಕ್ತ(s->type);
    ಗಂಟು *n = s->options->front;
    ಯಾವತ್(n){
        kvp *pair = (kvp *)n->val;
        ಮುಕ್ತ(pair->ಕೀಲಿ);
        ಮುಕ್ತ(pair);
        ಗಂಟು *next = n->next;
        ಮುಕ್ತ(n);
        n = next;
    }
    ಮುಕ್ತ(s->options);
    ಮುಕ್ತ(s);
}

ಶೂನ್ಯ parse_data(ವರ್ಣ *data, ಭಿನ್ನ *a, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಯದಿ(!data) ಪ್ರತ್ಯಾಗಮ;
    ವರ್ಣ *curr = data;
    ವರ್ಣ *next = data;
    ಪೂರ್ಣಾಙ್ಕ done = 0;
    ಪ್ರತಿ(i = 0; i < n && !done; ++i){
        ಯಾವತ್(*++next !='\0' && *next != ',');
        ಯದಿ(*next == '\0') done = 1;
        *next = '\0';
        sscanf(curr, "%g", &a[i]);
        curr = next+1;
    }
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ size_params{
    ಪೂರ್ಣಾಙ್ಕ batch;
    ಪೂರ್ಣಾಙ್ಕ inputs;
    ಪೂರ್ಣಾಙ್ಕ h;
    ಪೂರ್ಣಾಙ್ಕ w;
    ಪೂರ್ಣಾಙ್ಕ c;
    ಪೂರ್ಣಾಙ್ಕ index;
    ಪೂರ್ಣಾಙ್ಕ time_steps;
    network *net;
} size_params;

local_layer parse_local(list *options, size_params params)
{
    ಪೂರ್ಣಾಙ್ಕ n = option_find_int(options, "filters",1);
    ಪೂರ್ಣಾಙ್ಕ size = option_find_int(options, "size",1);
    ಪೂರ್ಣಾಙ್ಕ stride = option_find_int(options, "stride",1);
    ಪೂರ್ಣಾಙ್ಕ pad = option_find_int(options, "pad",0);
    ವರ್ಣ *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    ಪೂರ್ಣಾಙ್ಕ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ಯದಿ(!(h && w && c)) error("Layer before local layer must output image.");

    local_layer layer = make_local_layer(batch,h,w,c,n,size,stride,pad,activation);

    ಪ್ರತ್ಯಾಗಮ layer;
}

layer parse_deconvolutional(list *options, size_params params)
{
    ಪೂರ್ಣಾಙ್ಕ n = option_find_int(options, "filters",1);
    ಪೂರ್ಣಾಙ್ಕ size = option_find_int(options, "size",1);
    ಪೂರ್ಣಾಙ್ಕ stride = option_find_int(options, "stride",1);

    ವರ್ಣ *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    ಪೂರ್ಣಾಙ್ಕ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ಯದಿ(!(h && w && c)) error("Layer before deconvolutional layer must output image.");
    ಪೂರ್ಣಾಙ್ಕ batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);
    ಪೂರ್ಣಾಙ್ಕ pad = option_find_int_quiet(options, "pad",0);
    ಪೂರ್ಣಾಙ್ಕ padding = option_find_int_quiet(options, "padding",0);
    ಯದಿ(pad) padding = size/2;

    layer l = make_deconvolutional_layer(batch,h,w,c,n,size,stride,padding, activation, batch_normalize, params.net->adam);

    ಪ್ರತ್ಯಾಗಮ l;
}


convolutional_layer parse_convolutional(list *options, size_params params)
{
    ಪೂರ್ಣಾಙ್ಕ n = option_find_int(options, "filters",1);
    ಪೂರ್ಣಾಙ್ಕ size = option_find_int(options, "size",1);
    ಪೂರ್ಣಾಙ್ಕ stride = option_find_int(options, "stride",1);
    ಪೂರ್ಣಾಙ್ಕ pad = option_find_int_quiet(options, "pad",0);
    ಪೂರ್ಣಾಙ್ಕ padding = option_find_int_quiet(options, "padding",0);
    ಪೂರ್ಣಾಙ್ಕ groups = option_find_int_quiet(options, "groups", 1);
    ಯದಿ(pad) padding = size/2;

    ವರ್ಣ *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    ಪೂರ್ಣಾಙ್ಕ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ಯದಿ(!(h && w && c)) error("Layer before convolutional layer must output image.");
    ಪೂರ್ಣಾಙ್ಕ batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);
    ಪೂರ್ಣಾಙ್ಕ ದ್ವಿಮಾನ = option_find_int_quiet(options, "binary", 0);
    ಪೂರ್ಣಾಙ್ಕ xnor = option_find_int_quiet(options, "xnor", 0);

    convolutional_layer layer = make_convolutional_layer(batch,h,w,c,n,groups,size,stride,padding,activation, batch_normalize, ದ್ವಿಮಾನ, xnor, params.net->adam);
    layer.flipped = option_find_int_quiet(options, "flipped", 0);
    layer.dot = option_find_float_quiet(options, "dot", 0);

    ಪ್ರತ್ಯಾಗಮ layer;
}

layer parse_crnn(list *options, size_params params)
{
    ಪೂರ್ಣಾಙ್ಕ output_filters = option_find_int(options, "output_filters",1);
    ಪೂರ್ಣಾಙ್ಕ hidden_filters = option_find_int(options, "hidden_filters",1);
    ವರ್ಣ *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    ಪೂರ್ಣಾಙ್ಕ batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_crnn_layer(params.batch, params.w, params.h, params.c, hidden_filters, output_filters, params.time_steps, activation, batch_normalize);

    l.shortcut = option_find_int_quiet(options, "shortcut", 0);

    ಪ್ರತ್ಯಾಗಮ l;
}

layer parse_rnn(list *options, size_params params)
{
    ಪೂರ್ಣಾಙ್ಕ output = option_find_int(options, "output",1);
    ವರ್ಣ *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    ಪೂರ್ಣಾಙ್ಕ batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_rnn_layer(params.batch, params.inputs, output, params.time_steps, activation, batch_normalize, params.net->adam);

    l.shortcut = option_find_int_quiet(options, "shortcut", 0);

    ಪ್ರತ್ಯಾಗಮ l;
}

layer parse_gru(list *options, size_params params)
{
    ಪೂರ್ಣಾಙ್ಕ output = option_find_int(options, "output",1);
    ಪೂರ್ಣಾಙ್ಕ batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_gru_layer(params.batch, params.inputs, output, params.time_steps, batch_normalize, params.net->adam);
    l.tanh = option_find_int_quiet(options, "tanh", 0);

    ಪ್ರತ್ಯಾಗಮ l;
}

layer parse_lstm(list *options, size_params params)
{
    ಪೂರ್ಣಾಙ್ಕ output = option_find_int(options, "output", 1);
    ಪೂರ್ಣಾಙ್ಕ batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_lstm_layer(params.batch, params.inputs, output, params.time_steps, batch_normalize, params.net->adam);

    ಪ್ರತ್ಯಾಗಮ l;
}

layer parse_connected(list *options, size_params params)
{
    ಪೂರ್ಣಾಙ್ಕ output = option_find_int(options, "output",1);
    ವರ್ಣ *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    ಪೂರ್ಣಾಙ್ಕ batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_connected_layer(params.batch, params.inputs, output, activation, batch_normalize, params.net->adam);
    ಪ್ರತ್ಯಾಗಮ l;
}

layer parse_softmax(list *options, size_params params)
{
    ಪೂರ್ಣಾಙ್ಕ groups = option_find_int_quiet(options, "groups",1);
    layer l = make_softmax_layer(params.batch, params.inputs, groups);
    l.temperature = option_find_float_quiet(options, "temperature", 1);
    ವರ್ಣ *ವೃಕ್ಷ_file = option_find_str(options, "tree", 0);
    ಯದಿ (ವೃಕ್ಷ_file) l.softmax_ವೃಕ್ಷ = read_ವೃಕ್ಷ(ವೃಕ್ಷ_file);
    l.w = params.w;
    l.h = params.h;
    l.c = params.c;
    l.spatial = option_find_float_quiet(options, "spatial", 0);
    l.noloss =  option_find_int_quiet(options, "noloss", 0);
    ಪ್ರತ್ಯಾಗಮ l;
}

ಪೂರ್ಣಾಙ್ಕ *parse_yolo_mask(ವರ್ಣ *a, ಪೂರ್ಣಾಙ್ಕ *ಅಂಕೆ)
{
    ಪೂರ್ಣಾಙ್ಕ *mask = 0;
    ಯದಿ(a){
        ಪೂರ್ಣಾಙ್ಕ len = strlen(a);
        ಪೂರ್ಣಾಙ್ಕ n = 1;
        ಪೂರ್ಣಾಙ್ಕ i;
        ಪ್ರತಿ(i = 0; i < len; ++i){
            ಯದಿ (a[i] == ',') ++n;
        }
        mask = calloc(n, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ));
        ಪ್ರತಿ(i = 0; i < n; ++i){
            ಪೂರ್ಣಾಙ್ಕ val = atoi(a);
            mask[i] = val;
            a = strchr(a, ',')+1;
        }
        *ಅಂಕೆ = n;
    }
    ಪ್ರತ್ಯಾಗಮ mask;
}

layer parse_yolo(list *options, size_params params)
{
    ಪೂರ್ಣಾಙ್ಕ classes = option_find_int(options, "classes", 20);
    ಪೂರ್ಣಾಙ್ಕ total = option_find_int(options, "num", 1);
    ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ = total;

    ವರ್ಣ *a = option_find_str(options, "mask", 0);
    ಪೂರ್ಣಾಙ್ಕ *mask = parse_yolo_mask(a, &ಅಂಕೆ);
    layer l = make_yolo_layer(params.batch, params.w, params.h, ಅಂಕೆ, total, mask, classes);
    ಪ್ರತಿಜ್ಞಾ(l.outputs == params.inputs);

    l.max_boxes = option_find_int_quiet(options, "max",90);
    l.jitter = option_find_float(options, "jitter", .2);

    l.ignore_thresh = option_find_float(options, "ignore_thresh", .5);
    l.truth_thresh = option_find_float(options, "truth_thresh", 1);
    l.random = option_find_int_quiet(options, "random", 0);

    ವರ್ಣ *map_file = option_find_str(options, "map", 0);
    ಯದಿ (map_file) l.map = read_map(map_file);

    a = option_find_str(options, "anchors", 0);
    ಯದಿ(a){
        ಪೂರ್ಣಾಙ್ಕ len = strlen(a);
        ಪೂರ್ಣಾಙ್ಕ n = 1;
        ಪೂರ್ಣಾಙ್ಕ i;
        ಪ್ರತಿ(i = 0; i < len; ++i){
            ಯದಿ (a[i] == ',') ++n;
        }
        ಪ್ರತಿ(i = 0; i < n; ++i){
            ಭಿನ್ನ bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    ಪ್ರತ್ಯಾಗಮ l;
}

layer parse_iseg(list *options, size_params params)
{
    ಪೂರ್ಣಾಙ್ಕ classes = option_find_int(options, "classes", 20);
    ಪೂರ್ಣಾಙ್ಕ ids = option_find_int(options, "ids", 32);
    layer l = make_iseg_layer(params.batch, params.w, params.h, classes, ids);
    ಪ್ರತಿಜ್ಞಾ(l.outputs == params.inputs);
    ಪ್ರತ್ಯಾಗಮ l;
}

layer parse_region(list *options, size_params params)
{
    ಪೂರ್ಣಾಙ್ಕ coords = option_find_int(options, "coords", 4);
    ಪೂರ್ಣಾಙ್ಕ classes = option_find_int(options, "classes", 20);
    ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ = option_find_int(options, "num", 1);

    layer l = make_region_layer(params.batch, params.w, params.h, ಅಂಕೆ, classes, coords);
    ಪ್ರತಿಜ್ಞಾ(l.outputs == params.inputs);

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

    ವರ್ಣ *ವೃಕ್ಷ_file = option_find_str(options, "tree", 0);
    ಯದಿ (ವೃಕ್ಷ_file) l.softmax_ವೃಕ್ಷ = read_ವೃಕ್ಷ(ವೃಕ್ಷ_file);
    ವರ್ಣ *map_file = option_find_str(options, "map", 0);
    ಯದಿ (map_file) l.map = read_map(map_file);

    ವರ್ಣ *a = option_find_str(options, "anchors", 0);
    ಯದಿ(a){
        ಪೂರ್ಣಾಙ್ಕ len = strlen(a);
        ಪೂರ್ಣಾಙ್ಕ n = 1;
        ಪೂರ್ಣಾಙ್ಕ i;
        ಪ್ರತಿ(i = 0; i < len; ++i){
            ಯದಿ (a[i] == ',') ++n;
        }
        ಪ್ರತಿ(i = 0; i < n; ++i){
            ಭಿನ್ನ bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    ಪ್ರತ್ಯಾಗಮ l;
}

detection_layer parse_detection(list *options, size_params params)
{
    ಪೂರ್ಣಾಙ್ಕ coords = option_find_int(options, "coords", 1);
    ಪೂರ್ಣಾಙ್ಕ classes = option_find_int(options, "classes", 1);
    ಪೂರ್ಣಾಙ್ಕ rescore = option_find_int(options, "rescore", 0);
    ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ = option_find_int(options, "num", 1);
    ಪೂರ್ಣಾಙ್ಕ side = option_find_int(options, "side", 7);
    detection_layer layer = make_detection_layer(params.batch, params.inputs, ಅಂಕೆ, side, classes, coords, rescore);

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
    ಪ್ರತ್ಯಾಗಮ layer;
}

cost_layer parse_cost(list *options, size_params params)
{
    ವರ್ಣ *type_s = option_find_str(options, "type", "sse");
    COST_TYPE type = get_cost_type(type_s);
    ಭಿನ್ನ scale = option_find_float_quiet(options, "scale",1);
    cost_layer layer = make_cost_layer(params.batch, params.inputs, type, scale);
    layer.ratio =  option_find_float_quiet(options, "ratio",0);
    layer.noobject_scale =  option_find_float_quiet(options, "noobj", 1);
    layer.thresh =  option_find_float_quiet(options, "thresh",0);
    ಪ್ರತ್ಯಾಗಮ layer;
}

crop_layer parse_crop(list *options, size_params params)
{
    ಪೂರ್ಣಾಙ್ಕ crop_height = option_find_int(options, "crop_height",1);
    ಪೂರ್ಣಾಙ್ಕ crop_width = option_find_int(options, "crop_width",1);
    ಪೂರ್ಣಾಙ್ಕ flip = option_find_int(options, "flip",0);
    ಭಿನ್ನ angle = option_find_float(options, "angle",0);
    ಭಿನ್ನ saturation = option_find_float(options, "saturation",1);
    ಭಿನ್ನ exposure = option_find_float(options, "exposure",1);

    ಪೂರ್ಣಾಙ್ಕ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ಯದಿ(!(h && w && c)) error("Layer before crop layer must output image.");

    ಪೂರ್ಣಾಙ್ಕ noadjust = option_find_int_quiet(options, "noadjust",0);

    crop_layer l = make_crop_layer(batch,h,w,c,crop_height,crop_width,flip, angle, saturation, exposure);
    l.shift = option_find_float(options, "shift", 0);
    l.noadjust = noadjust;
    ಪ್ರತ್ಯಾಗಮ l;
}

layer parse_reorg(list *options, size_params params)
{
    ಪೂರ್ಣಾಙ್ಕ stride = option_find_int(options, "stride",1);
    ಪೂರ್ಣಾಙ್ಕ reverse = option_find_int_quiet(options, "reverse",0);
    ಪೂರ್ಣಾಙ್ಕ flatten = option_find_int_quiet(options, "flatten",0);
    ಪೂರ್ಣಾಙ್ಕ extra = option_find_int_quiet(options, "extra",0);

    ಪೂರ್ಣಾಙ್ಕ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ಯದಿ(!(h && w && c)) error("Layer before reorg layer must output image.");

    layer layer = make_reorg_layer(batch,w,h,c,stride,reverse, flatten, extra);
    ಪ್ರತ್ಯಾಗಮ layer;
}

maxpool_layer parse_maxpool(list *options, size_params params)
{
    ಪೂರ್ಣಾಙ್ಕ stride = option_find_int(options, "stride",1);
    ಪೂರ್ಣಾಙ್ಕ size = option_find_int(options, "size",stride);
    ಪೂರ್ಣಾಙ್ಕ padding = option_find_int_quiet(options, "padding", size-1);

    ಪೂರ್ಣಾಙ್ಕ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ಯದಿ(!(h && w && c)) error("Layer before maxpool layer must output image.");

    maxpool_layer layer = make_maxpool_layer(batch,h,w,c,size,stride,padding);
    ಪ್ರತ್ಯಾಗಮ layer;
}

avgpool_layer parse_avgpool(list *options, size_params params)
{
    ಪೂರ್ಣಾಙ್ಕ batch,w,h,c;
    w = params.w;
    h = params.h;
    c = params.c;
    batch=params.batch;
    ಯದಿ(!(h && w && c)) error("Layer before avgpool layer must output image.");

    avgpool_layer layer = make_avgpool_layer(batch,w,h,c);
    ಪ್ರತ್ಯಾಗಮ layer;
}

dropout_layer parse_dropout(list *options, size_params params)
{
    ಭಿನ್ನ probability = option_find_float(options, "probability", .5);
    dropout_layer layer = make_dropout_layer(params.batch, params.inputs, probability);
    layer.out_w = params.w;
    layer.out_h = params.h;
    layer.out_c = params.c;
    ಪ್ರತ್ಯಾಗಮ layer;
}

layer parse_normalization(list *options, size_params params)
{
    ಭಿನ್ನ alpha = option_find_float(options, "alpha", .0001);
    ಭಿನ್ನ beta =  option_find_float(options, "beta" , .75);
    ಭಿನ್ನ kappa = option_find_float(options, "kappa", 1);
    ಪೂರ್ಣಾಙ್ಕ size = option_find_int(options, "size", 5);
    layer l = make_normalization_layer(params.batch, params.w, params.h, params.c, size, alpha, beta, kappa);
    ಪ್ರತ್ಯಾಗಮ l;
}

layer parse_batchnorm(list *options, size_params params)
{
    layer l = make_batchnorm_layer(params.batch, params.w, params.h, params.c);
    ಪ್ರತ್ಯಾಗಮ l;
}

layer parse_shortcut(list *options, size_params params, network *net)
{
    ವರ್ಣ *l = option_find(options, "from");
    ಪೂರ್ಣಾಙ್ಕ index = atoi(l);
    ಯದಿ(index < 0) index = params.index + index;

    ಪೂರ್ಣಾಙ್ಕ batch = params.batch;
    layer from = net->layers[index];

    layer s = make_shortcut_layer(batch, index, params.w, params.h, params.c, from.out_w, from.out_h, from.out_c);

    ವರ್ಣ *activation_s = option_find_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);
    s.activation = activation;
    s.alpha = option_find_float_quiet(options, "alpha", 1);
    s.beta = option_find_float_quiet(options, "beta", 1);
    ಪ್ರತ್ಯಾಗಮ s;
}


layer parse_l2norm(list *options, size_params params)
{
    layer l = make_l2norm_layer(params.batch, params.inputs);
    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;
    ಪ್ರತ್ಯಾಗಮ l;
}


layer parse_logistic(list *options, size_params params)
{
    layer l = make_logistic_layer(params.batch, params.inputs);
    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;
    ಪ್ರತ್ಯಾಗಮ l;
}

layer parse_activation(list *options, size_params params)
{
    ವರ್ಣ *activation_s = option_find_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);

    layer l = make_activation_layer(params.batch, params.inputs, activation);

    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;

    ಪ್ರತ್ಯಾಗಮ l;
}

layer parse_upsample(list *options, size_params params, network *net)
{

    ಪೂರ್ಣಾಙ್ಕ stride = option_find_int(options, "stride",2);
    layer l = make_upsample_layer(params.batch, params.w, params.h, params.c, stride);
    l.scale = option_find_float_quiet(options, "scale", 1);
    ಪ್ರತ್ಯಾಗಮ l;
}

route_layer parse_route(list *options, size_params params, network *net)
{
    ವರ್ಣ *l = option_find(options, "layers");
    ಪೂರ್ಣಾಙ್ಕ len = strlen(l);
    ಯದಿ(!l) error("Route Layer must specify input layers");
    ಪೂರ್ಣಾಙ್ಕ n = 1;
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < len; ++i){
        ಯದಿ (l[i] == ',') ++n;
    }

    ಪೂರ್ಣಾಙ್ಕ *layers = calloc(n, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ));
    ಪೂರ್ಣಾಙ್ಕ *sizes = calloc(n, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ));
    ಪ್ರತಿ(i = 0; i < n; ++i){
        ಪೂರ್ಣಾಙ್ಕ index = atoi(l);
        l = strchr(l, ',')+1;
        ಯದಿ(index < 0) index = params.index + index;
        layers[i] = index;
        sizes[i] = net->layers[index].outputs;
    }
    ಪೂರ್ಣಾಙ್ಕ batch = params.batch;

    route_layer layer = make_route_layer(batch, n, layers, sizes);

    convolutional_layer first = net->layers[layers[0]];
    layer.out_w = first.out_w;
    layer.out_h = first.out_h;
    layer.out_c = first.out_c;
    ಪ್ರತಿ(i = 1; i < n; ++i){
        ಪೂರ್ಣಾಙ್ಕ index = layers[i];
        convolutional_layer next = net->layers[index];
        ಯದಿ(next.out_w == first.out_w && next.out_h == first.out_h){
            layer.out_c += next.out_c;
        }ಅನ್ಯಥಾ{
            layer.out_h = layer.out_w = layer.out_c = 0;
        }
    }

    ಪ್ರತ್ಯಾಗಮ layer;
}

learning_rate_policy get_policy(ವರ್ಣ *s)
{
    ಯದಿ (strcmp(s, "random")==0) ಪ್ರತ್ಯಾಗಮ RANDOM;
    ಯದಿ (strcmp(s, "poly")==0) ಪ್ರತ್ಯಾಗಮ POLY;
    ಯದಿ (strcmp(s, "constant")==0) ಪ್ರತ್ಯಾಗಮ CONSTANT;
    ಯದಿ (strcmp(s, "step")==0) ಪ್ರತ್ಯಾಗಮ STEP;
    ಯದಿ (strcmp(s, "exp")==0) ಪ್ರತ್ಯಾಗಮ EXP;
    ಯದಿ (strcmp(s, "sigmoid")==0) ಪ್ರತ್ಯಾಗಮ SIG;
    ಯದಿ (strcmp(s, "steps")==0) ಪ್ರತ್ಯಾಗಮ STEPS;
    fprintf(stderr, "Couldn't find policy %s, going with constant\n", s);
    ಪ್ರತ್ಯಾಗಮ CONSTANT;
}

ಶೂನ್ಯ parse_net_options(list *options, network *net)
{
    net->batch = option_find_int(options, "batch",1);
    net->learning_rate = option_find_float(options, "learning_rate", .001);
    net->momentum = option_find_float(options, "momentum", .9);
    net->decay = option_find_float(options, "decay", .0001);
    ಪೂರ್ಣಾಙ್ಕ subdivs = option_find_int(options, "subdivisions",1);
    net->time_steps = option_find_int_quiet(options, "time_steps",1);
    net->notruth = option_find_int_quiet(options, "notruth",0);
    net->batch /= subdivs;
    net->batch *= net->time_steps;
    net->subdivisions = subdivs;
    net->random = option_find_int_quiet(options, "random", 0);

    net->adam = option_find_int_quiet(options, "adam", 0);
    ಯದಿ(net->adam){
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
    net->max_ratio = option_find_float_quiet(options, "max_ratio", (ಭಿನ್ನ) net->max_crop / net->w);
    net->min_ratio = option_find_float_quiet(options, "min_ratio", (ಭಿನ್ನ) net->min_crop / net->w);
    net->center = option_find_int_quiet(options, "center",0);
    net->clip = option_find_float_quiet(options, "clip", 0);

    net->angle = option_find_float_quiet(options, "angle", 0);
    net->aspect = option_find_float_quiet(options, "aspect", 1);
    net->saturation = option_find_float_quiet(options, "saturation", 1);
    net->exposure = option_find_float_quiet(options, "exposure", 1);
    net->hue = option_find_float_quiet(options, "hue", 0);

    ಯದಿ(!net->inputs && !(net->h && net->w && net->c)) error("No input parameters supplied");

    ವರ್ಣ *policy_s = option_find_str(options, "policy", "constant");
    net->policy = get_policy(policy_s);
    net->burn_in = option_find_int_quiet(options, "burn_in", 0);
    net->power = option_find_float_quiet(options, "power", 4);
    ಯದಿ(net->policy == STEP){
        net->step = option_find_int(options, "step", 1);
        net->scale = option_find_float(options, "scale", 1);
    } ಅನ್ಯಥಾ ಯದಿ (net->policy == STEPS){
        ವರ್ಣ *l = option_find(options, "steps");
        ವರ್ಣ *p = option_find(options, "scales");
        ಯದಿ(!l || !p) error("STEPS policy must have steps and scales in cfg file");

        ಪೂರ್ಣಾಙ್ಕ len = strlen(l);
        ಪೂರ್ಣಾಙ್ಕ n = 1;
        ಪೂರ್ಣಾಙ್ಕ i;
        ಪ್ರತಿ(i = 0; i < len; ++i){
            ಯದಿ (l[i] == ',') ++n;
        }
        ಪೂರ್ಣಾಙ್ಕ *steps = calloc(n, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ));
        ಭಿನ್ನ *scales = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ));
        ಪ್ರತಿ(i = 0; i < n; ++i){
            ಪೂರ್ಣಾಙ್ಕ step    = atoi(l);
            ಭಿನ್ನ scale = atof(p);
            l = strchr(l, ',')+1;
            p = strchr(p, ',')+1;
            steps[i] = step;
            scales[i] = scale;
        }
        net->scales = scales;
        net->steps = steps;
        net->ಅಂಕೆ_steps = n;
    } ಅನ್ಯಥಾ ಯದಿ (net->policy == EXP){
        net->gamma = option_find_float(options, "gamma", 1);
    } ಅನ್ಯಥಾ ಯದಿ (net->policy == SIG){
        net->gamma = option_find_float(options, "gamma", 1);
        net->step = option_find_int(options, "step", 1);
    } ಅನ್ಯಥಾ ಯದಿ (net->policy == POLY || net->policy == RANDOM){
    }
    net->max_batches = option_find_int(options, "max_batches", 0);
}

ಪೂರ್ಣಾಙ್ಕ is_network(section *s)
{
    ಪ್ರತ್ಯಾಗಮ (strcmp(s->type, "[net]")==0
            || strcmp(s->type, "[network]")==0);
}

network *parse_network_cfg(ವರ್ಣ *filename)
{
    list *sections = read_cfg(filename);
    ಗಂಟು *n = sections->front;
    ಯದಿ(!n) error("Config file has no sections");
    network *net = make_network(sections->size - 1);
    net->gpu_index = gpu_index;
    size_params params;

    section *s = (section *)n->val;
    list *options = s->options;
    ಯದಿ(!is_network(s)) error("First section must be [net] or [network]");
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
    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
    free_section(s);
    fprintf(stderr, "layer     filters    size              input                output\n");
    ಯಾವತ್(n){
        params.index = ಎಣಿಕೆ;
        fprintf(stderr, "%5d ", ಎಣಿಕೆ);
        s = (section *)n->val;
        options = s->options;
        layer l = {0};
        LAYER_TYPE lt = string_to_layer_type(s->type);
        ಯದಿ(lt == CONVOLUTIONAL){
            l = parse_convolutional(options, params);
        }ಅನ್ಯಥಾ ಯದಿ(lt == DECONVOLUTIONAL){
            l = parse_deconvolutional(options, params);
        }ಅನ್ಯಥಾ ಯದಿ(lt == LOCAL){
            l = parse_local(options, params);
        }ಅನ್ಯಥಾ ಯದಿ(lt == ACTIVE){
            l = parse_activation(options, params);
        }ಅನ್ಯಥಾ ಯದಿ(lt == LOGXENT){
            l = parse_logistic(options, params);
        }ಅನ್ಯಥಾ ಯದಿ(lt == L2NORM){
            l = parse_l2norm(options, params);
        }ಅನ್ಯಥಾ ಯದಿ(lt == RNN){
            l = parse_rnn(options, params);
        }ಅನ್ಯಥಾ ಯದಿ(lt == GRU){
            l = parse_gru(options, params);
        }ಅನ್ಯಥಾ ಯದಿ (lt == LSTM) {
            l = parse_lstm(options, params);
        }ಅನ್ಯಥಾ ಯದಿ(lt == CRNN){
            l = parse_crnn(options, params);
        }ಅನ್ಯಥಾ ಯದಿ(lt == CONNECTED){
            l = parse_connected(options, params);
        }ಅನ್ಯಥಾ ಯದಿ(lt == CROP){
            l = parse_crop(options, params);
        }ಅನ್ಯಥಾ ಯದಿ(lt == COST){
            l = parse_cost(options, params);
        }ಅನ್ಯಥಾ ಯದಿ(lt == REGION){
            l = parse_region(options, params);
        }ಅನ್ಯಥಾ ಯದಿ(lt == YOLO){
            l = parse_yolo(options, params);
        }ಅನ್ಯಥಾ ಯದಿ(lt == ISEG){
            l = parse_iseg(options, params);
        }ಅನ್ಯಥಾ ಯದಿ(lt == DETECTION){
            l = parse_detection(options, params);
        }ಅನ್ಯಥಾ ಯದಿ(lt == SOFTMAX){
            l = parse_softmax(options, params);
            net->hierarchy = l.softmax_ವೃಕ್ಷ;
        }ಅನ್ಯಥಾ ಯದಿ(lt == NORMALIZATION){
            l = parse_normalization(options, params);
        }ಅನ್ಯಥಾ ಯದಿ(lt == BATCHNORM){
            l = parse_batchnorm(options, params);
        }ಅನ್ಯಥಾ ಯದಿ(lt == MAXPOOL){
            l = parse_maxpool(options, params);
        }ಅನ್ಯಥಾ ಯದಿ(lt == REORG){
            l = parse_reorg(options, params);
        }ಅನ್ಯಥಾ ಯದಿ(lt == AVGPOOL){
            l = parse_avgpool(options, params);
        }ಅನ್ಯಥಾ ಯದಿ(lt == ROUTE){
            l = parse_route(options, params, net);
        }ಅನ್ಯಥಾ ಯದಿ(lt == UPSAMPLE){
            l = parse_upsample(options, params, net);
        }ಅನ್ಯಥಾ ಯದಿ(lt == SHORTCUT){
            l = parse_shortcut(options, params, net);
        }ಅನ್ಯಥಾ ಯದಿ(lt == DROPOUT){
            l = parse_dropout(options, params);
            l.output = net->layers[ಎಣಿಕೆ-1].output;
            l.delta = net->layers[ಎಣಿಕೆ-1].delta;
#ifdef GPU
            l.output_gpu = net->layers[ಎಣಿಕೆ-1].output_gpu;
            l.delta_gpu = net->layers[ಎಣಿಕೆ-1].delta_gpu;
#endif
        }ಅನ್ಯಥಾ{
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
        net->layers[ಎಣಿಕೆ] = l;
        ಯದಿ (l.workspace_size > workspace_size) workspace_size = l.workspace_size;
        free_section(s);
        n = n->next;
        ++ಎಣಿಕೆ;
        ಯದಿ(n){
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
    ಯದಿ(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    net->ಒಳಹರಿವು = calloc(net->inputs*net->batch, ಪರಿಮಾಣ(ಭಿನ್ನ));
    net->truth = calloc(net->truths*net->batch, ಪರಿಮಾಣ(ಭಿನ್ನ));
#ifdef GPU
    net->output_gpu = out.output_gpu;
    net->ಒಳಹರಿವು_gpu = cuda_make_array(net->ಒಳಹರಿವು, net->inputs*net->batch);
    net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
#endif
    ಯದಿ(workspace_size){
        //printf("%ld\n", workspace_size);
#ifdef GPU
        ಯದಿ(gpu_index >= 0){
            net->workspace = cuda_make_array(0, (workspace_size-1)/ಪರಿಮಾಣ(ಭಿನ್ನ)+1);
        }ಅನ್ಯಥಾ {
            net->workspace = calloc(1, workspace_size);
        }
#ಅನ್ಯಥಾ
        net->workspace = calloc(1, workspace_size);
#endif
    }
    ಪ್ರತ್ಯಾಗಮ net;
}

list *read_cfg(ವರ್ಣ *filename)
{
    FILE *file = fopen(filename, "r");
    ಯದಿ(file == 0) file_error(filename);
    ವರ್ಣ *ಸಾಲು;
    ಪೂರ್ಣಾಙ್ಕ nu = 0;
    list *options = make_list();
    section *current = 0;
    ಯಾವತ್((ಸಾಲು=fgetl(file)) != 0){
        ++ nu;
        strip(ಸಾಲು);
        ವಿಕಲ್ಪ(ಸಾಲು[0]){
            ಪಕ್ಷ '[':
                current = malloc(ಪರಿಮಾಣ(section));
                list_ಸೇರಿಸು(options, current);
                current->options = make_list();
                current->type = ಸಾಲು;
                ಭಙ್ಗ;
            ಪಕ್ಷ '\0':
            ಪಕ್ಷ '#':
            ಪಕ್ಷ ';':
                ಮುಕ್ತ(ಸಾಲು);
                ಭಙ್ಗ;
            ಸ್ವಭಾವ:
                ಯದಿ(!read_option(ಸಾಲು, current->options)){
                    fprintf(stderr, "Config file error line %d, could parse: %s\n", nu, ಸಾಲು);
                    ಮುಕ್ತ(ಸಾಲು);
                }
                ಭಙ್ಗ;
        }
    }
    fclose(file);
    ಪ್ರತ್ಯಾಗಮ options;
}

ಶೂನ್ಯ save_convolutional_weights_ದ್ವಿಮಾನ(layer l, FILE *fp)
{
#ifdef GPU
    ಯದಿ(gpu_index >= 0){
        pull_convolutional_layer(l);
    }
#endif
    binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.ದ್ವಿಮಾನ_weights);
    ಪೂರ್ಣಾಙ್ಕ size = l.c*l.size*l.size;
    ಪೂರ್ಣಾಙ್ಕ i, j, k;
    fwrite(l.biases, ಪರಿಮಾಣ(ಭಿನ್ನ), l.n, fp);
    ಯದಿ (l.batch_normalize){
        fwrite(l.scales, ಪರಿಮಾಣ(ಭಿನ್ನ), l.n, fp);
        fwrite(l.rolling_mean, ಪರಿಮಾಣ(ಭಿನ್ನ), l.n, fp);
        fwrite(l.rolling_variance, ಪರಿಮಾಣ(ಭಿನ್ನ), l.n, fp);
    }
    ಪ್ರತಿ(i = 0; i < l.n; ++i){
        ಭಿನ್ನ mean = l.ದ್ವಿಮಾನ_weights[i*size];
        ಯದಿ(mean < 0) mean = -mean;
        fwrite(&mean, ಪರಿಮಾಣ(ಭಿನ್ನ), 1, fp);
        ಪ್ರತಿ(j = 0; j < size/8; ++j){
            ಪೂರ್ಣಾಙ್ಕ index = i*size + j*8;
            unsigned ವರ್ಣ c = 0;
            ಪ್ರತಿ(k = 0; k < 8; ++k){
                ಯದಿ (j*8 + k >= size) ಭಙ್ಗ;
                ಯದಿ (l.ದ್ವಿಮಾನ_weights[index + k] > 0) c = (c | 1<<k);
            }
            fwrite(&c, ಪರಿಮಾಣ(ವರ್ಣ), 1, fp);
        }
    }
}

ಶೂನ್ಯ save_convolutional_weights(layer l, FILE *fp)
{
    ಯದಿ(l.ದ್ವಿಮಾನ){
        //save_convolutional_weights_binary(l, fp);
        //return;
    }
#ifdef GPU
    ಯದಿ(gpu_index >= 0){
        pull_convolutional_layer(l);
    }
#endif
    ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ = l.nweights;
    fwrite(l.biases, ಪರಿಮಾಣ(ಭಿನ್ನ), l.n, fp);
    ಯದಿ (l.batch_normalize){
        fwrite(l.scales, ಪರಿಮಾಣ(ಭಿನ್ನ), l.n, fp);
        fwrite(l.rolling_mean, ಪರಿಮಾಣ(ಭಿನ್ನ), l.n, fp);
        fwrite(l.rolling_variance, ಪರಿಮಾಣ(ಭಿನ್ನ), l.n, fp);
    }
    fwrite(l.weights, ಪರಿಮಾಣ(ಭಿನ್ನ), ಅಂಕೆ, fp);
}

ಶೂನ್ಯ save_batchnorm_weights(layer l, FILE *fp)
{
#ifdef GPU
    ಯದಿ(gpu_index >= 0){
        pull_batchnorm_layer(l);
    }
#endif
    fwrite(l.scales, ಪರಿಮಾಣ(ಭಿನ್ನ), l.c, fp);
    fwrite(l.rolling_mean, ಪರಿಮಾಣ(ಭಿನ್ನ), l.c, fp);
    fwrite(l.rolling_variance, ಪರಿಮಾಣ(ಭಿನ್ನ), l.c, fp);
}

ಶೂನ್ಯ save_connected_weights(layer l, FILE *fp)
{
#ifdef GPU
    ಯದಿ(gpu_index >= 0){
        pull_connected_layer(l);
    }
#endif
    fwrite(l.biases, ಪರಿಮಾಣ(ಭಿನ್ನ), l.outputs, fp);
    fwrite(l.weights, ಪರಿಮಾಣ(ಭಿನ್ನ), l.outputs*l.inputs, fp);
    ಯದಿ (l.batch_normalize){
        fwrite(l.scales, ಪರಿಮಾಣ(ಭಿನ್ನ), l.outputs, fp);
        fwrite(l.rolling_mean, ಪರಿಮಾಣ(ಭಿನ್ನ), l.outputs, fp);
        fwrite(l.rolling_variance, ಪರಿಮಾಣ(ಭಿನ್ನ), l.outputs, fp);
    }
}

ಶೂನ್ಯ save_weights_upto(network *net, ವರ್ಣ *filename, ಪೂರ್ಣಾಙ್ಕ cutoff)
{
#ifdef GPU
    ಯದಿ(net->gpu_index >= 0){
        cuda_set_device(net->gpu_index);
    }
#endif
    fprintf(stderr, "Saving weights to %s\n", filename);
    FILE *fp = fopen(filename, "wb");
    ಯದಿ(!fp) file_error(filename);

    ಪೂರ್ಣಾಙ್ಕ major = 0;
    ಪೂರ್ಣಾಙ್ಕ minor = 2;
    ಪೂರ್ಣಾಙ್ಕ revision = 0;
    fwrite(&major, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ), 1, fp);
    fwrite(&minor, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ), 1, fp);
    fwrite(&revision, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ), 1, fp);
    fwrite(net->seen, ಪರಿಮಾಣ(size_t), 1, fp);

    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        ಯದಿ (l.dontsave) ಅನುವರ್ತ;
        ಯದಿ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            save_convolutional_weights(l, fp);
        } ಯದಿ(l.type == CONNECTED){
            save_connected_weights(l, fp);
        } ಯದಿ(l.type == BATCHNORM){
            save_batchnorm_weights(l, fp);
        } ಯದಿ(l.type == RNN){
            save_connected_weights(*(l.ಒಳಹರಿವು_layer), fp);
            save_connected_weights(*(l.self_layer), fp);
            save_connected_weights(*(l.output_layer), fp);
        } ಯದಿ (l.type == LSTM) {
            save_connected_weights(*(l.wi), fp);
            save_connected_weights(*(l.wf), fp);
            save_connected_weights(*(l.wo), fp);
            save_connected_weights(*(l.wg), fp);
            save_connected_weights(*(l.ui), fp);
            save_connected_weights(*(l.uf), fp);
            save_connected_weights(*(l.uo), fp);
            save_connected_weights(*(l.ug), fp);
        } ಯದಿ (l.type == GRU) {
            ಯದಿ(1){
                save_connected_weights(*(l.wz), fp);
                save_connected_weights(*(l.wr), fp);
                save_connected_weights(*(l.wh), fp);
                save_connected_weights(*(l.uz), fp);
                save_connected_weights(*(l.ur), fp);
                save_connected_weights(*(l.uh), fp);
            }ಅನ್ಯಥಾ{
                save_connected_weights(*(l.reset_layer), fp);
                save_connected_weights(*(l.update_layer), fp);
                save_connected_weights(*(l.state_layer), fp);
            }
        }  ಯದಿ(l.type == CRNN){
            save_convolutional_weights(*(l.ಒಳಹರಿವು_layer), fp);
            save_convolutional_weights(*(l.self_layer), fp);
            save_convolutional_weights(*(l.output_layer), fp);
        } ಯದಿ(l.type == LOCAL){
#ifdef GPU
            ಯದಿ(gpu_index >= 0){
                pull_local_layer(l);
            }
#endif
            ಪೂರ್ಣಾಙ್ಕ locations = l.out_w*l.out_h;
            ಪೂರ್ಣಾಙ್ಕ size = l.size*l.size*l.c*l.n*locations;
            fwrite(l.biases, ಪರಿಮಾಣ(ಭಿನ್ನ), l.outputs, fp);
            fwrite(l.weights, ಪರಿಮಾಣ(ಭಿನ್ನ), size, fp);
        }
    }
    fclose(fp);
}
ಶೂನ್ಯ save_weights(network *net, ವರ್ಣ *filename)
{
    save_weights_upto(net, filename, net->n);
}

ಶೂನ್ಯ transpose_matrix(ಭಿನ್ನ *a, ಪೂರ್ಣಾಙ್ಕ rows, ಪೂರ್ಣಾಙ್ಕ cols)
{
    ಭಿನ್ನ *transpose = calloc(rows*cols, ಪರಿಮಾಣ(ಭಿನ್ನ));
    ಪೂರ್ಣಾಙ್ಕ x, y;
    ಪ್ರತಿ(x = 0; x < rows; ++x){
        ಪ್ರತಿ(y = 0; y < cols; ++y){
            transpose[y*rows + x] = a[x*cols + y];
        }
    }
    memcpy(a, transpose, rows*cols*ಪರಿಮಾಣ(ಭಿನ್ನ));
    ಮುಕ್ತ(transpose);
}

ಶೂನ್ಯ load_connected_weights(layer l, FILE *fp, ಪೂರ್ಣಾಙ್ಕ transpose)
{
    fread(l.biases, ಪರಿಮಾಣ(ಭಿನ್ನ), l.outputs, fp);
    fread(l.weights, ಪರಿಮಾಣ(ಭಿನ್ನ), l.outputs*l.inputs, fp);
    ಯದಿ(transpose){
        transpose_matrix(l.weights, l.inputs, l.outputs);
    }
    //printf("Biases: %f mean %f variance\n", mean_array(l.biases, l.outputs), variance_array(l.biases, l.outputs));
    //printf("Weights: %f mean %f variance\n", mean_array(l.weights, l.outputs*l.inputs), variance_array(l.weights, l.outputs*l.inputs));
    ಯದಿ (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, ಪರಿಮಾಣ(ಭಿನ್ನ), l.outputs, fp);
        fread(l.rolling_mean, ಪರಿಮಾಣ(ಭಿನ್ನ), l.outputs, fp);
        fread(l.rolling_variance, ಪರಿಮಾಣ(ಭಿನ್ನ), l.outputs, fp);
        //printf("Scales: %f mean %f variance\n", mean_array(l.scales, l.outputs), variance_array(l.scales, l.outputs));
        //printf("rolling_mean: %f mean %f variance\n", mean_array(l.rolling_mean, l.outputs), variance_array(l.rolling_mean, l.outputs));
        //printf("rolling_variance: %f mean %f variance\n", mean_array(l.rolling_variance, l.outputs), variance_array(l.rolling_variance, l.outputs));
    }
#ifdef GPU
    ಯದಿ(gpu_index >= 0){
        ತಳ್ಳು_connected_layer(l);
    }
#endif
}

ಶೂನ್ಯ load_batchnorm_weights(layer l, FILE *fp)
{
    fread(l.scales, ಪರಿಮಾಣ(ಭಿನ್ನ), l.c, fp);
    fread(l.rolling_mean, ಪರಿಮಾಣ(ಭಿನ್ನ), l.c, fp);
    fread(l.rolling_variance, ಪರಿಮಾಣ(ಭಿನ್ನ), l.c, fp);
#ifdef GPU
    ಯದಿ(gpu_index >= 0){
        ತಳ್ಳು_batchnorm_layer(l);
    }
#endif
}

ಶೂನ್ಯ load_convolutional_weights_ದ್ವಿಮಾನ(layer l, FILE *fp)
{
    fread(l.biases, ಪರಿಮಾಣ(ಭಿನ್ನ), l.n, fp);
    ಯದಿ (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, ಪರಿಮಾಣ(ಭಿನ್ನ), l.n, fp);
        fread(l.rolling_mean, ಪರಿಮಾಣ(ಭಿನ್ನ), l.n, fp);
        fread(l.rolling_variance, ಪರಿಮಾಣ(ಭಿನ್ನ), l.n, fp);
    }
    ಪೂರ್ಣಾಙ್ಕ size = l.c*l.size*l.size;
    ಪೂರ್ಣಾಙ್ಕ i, j, k;
    ಪ್ರತಿ(i = 0; i < l.n; ++i){
        ಭಿನ್ನ mean = 0;
        fread(&mean, ಪರಿಮಾಣ(ಭಿನ್ನ), 1, fp);
        ಪ್ರತಿ(j = 0; j < size/8; ++j){
            ಪೂರ್ಣಾಙ್ಕ index = i*size + j*8;
            unsigned ವರ್ಣ c = 0;
            fread(&c, ಪರಿಮಾಣ(ವರ್ಣ), 1, fp);
            ಪ್ರತಿ(k = 0; k < 8; ++k){
                ಯದಿ (j*8 + k >= size) ಭಙ್ಗ;
                l.weights[index + k] = (c & 1<<k) ? mean : -mean;
            }
        }
    }
#ifdef GPU
    ಯದಿ(gpu_index >= 0){
        ತಳ್ಳು_convolutional_layer(l);
    }
#endif
}

ಶೂನ್ಯ load_convolutional_weights(layer l, FILE *fp)
{
    ಯದಿ(l.ದ್ವಿಮಾನ){
        //load_convolutional_weights_binary(l, fp);
        //return;
    }
    ಯದಿ(l.numload) l.n = l.numload;
    ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ = l.c/l.groups*l.n*l.size*l.size;
    fread(l.biases, ಪರಿಮಾಣ(ಭಿನ್ನ), l.n, fp);
    ಯದಿ (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, ಪರಿಮಾಣ(ಭಿನ್ನ), l.n, fp);
        fread(l.rolling_mean, ಪರಿಮಾಣ(ಭಿನ್ನ), l.n, fp);
        fread(l.rolling_variance, ಪರಿಮಾಣ(ಭಿನ್ನ), l.n, fp);
        ಯದಿ(0){
            ಪೂರ್ಣಾಙ್ಕ i;
            ಪ್ರತಿ(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_mean[i]);
            }
            printf("\n");
            ಪ್ರತಿ(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_variance[i]);
            }
            printf("\n");
        }
        ಯದಿ(0){
            fill_cpu(l.n, 0, l.rolling_mean, 1);
            fill_cpu(l.n, 0, l.rolling_variance, 1);
        }
        ಯದಿ(0){
            ಪೂರ್ಣಾಙ್ಕ i;
            ಪ್ರತಿ(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_mean[i]);
            }
            printf("\n");
            ಪ್ರತಿ(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_variance[i]);
            }
            printf("\n");
        }
    }
    fread(l.weights, ಪರಿಮಾಣ(ಭಿನ್ನ), ಅಂಕೆ, fp);
    //if(l.c == 3) scal_cpu(num, 1./256, l.weights, 1);
    ಯದಿ (l.flipped) {
        transpose_matrix(l.weights, l.c*l.size*l.size, l.n);
    }
    //if (l.binary) binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.weights);
#ifdef GPU
    ಯದಿ(gpu_index >= 0){
        ತಳ್ಳು_convolutional_layer(l);
    }
#endif
}


ಶೂನ್ಯ load_weights_upto(network *net, ವರ್ಣ *filename, ಪೂರ್ಣಾಙ್ಕ start, ಪೂರ್ಣಾಙ್ಕ cutoff)
{
#ifdef GPU
    ಯದಿ(net->gpu_index >= 0){
        cuda_set_device(net->gpu_index);
    }
#endif
    fprintf(stderr, "Loading weights from %s...", filename);
    fflush(stdout);
    FILE *fp = fopen(filename, "rb");
    ಯದಿ(!fp) file_error(filename);

    ಪೂರ್ಣಾಙ್ಕ major;
    ಪೂರ್ಣಾಙ್ಕ minor;
    ಪೂರ್ಣಾಙ್ಕ revision;
    fread(&major, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ), 1, fp);
    fread(&minor, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ), 1, fp);
    fread(&revision, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ), 1, fp);
    ಯದಿ ((major*10 + minor) >= 2 && major < 1000 && minor < 1000){
        fread(net->seen, ಪರಿಮಾಣ(size_t), 1, fp);
    } ಅನ್ಯಥಾ {
        ಪೂರ್ಣಾಙ್ಕ iseen = 0;
        fread(&iseen, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ), 1, fp);
        *net->seen = iseen;
    }
    ಪೂರ್ಣಾಙ್ಕ transpose = (major > 1000) || (minor > 1000);

    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = start; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        ಯದಿ (l.dontload) ಅನುವರ್ತ;
        ಯದಿ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            load_convolutional_weights(l, fp);
        }
        ಯದಿ(l.type == CONNECTED){
            load_connected_weights(l, fp, transpose);
        }
        ಯದಿ(l.type == BATCHNORM){
            load_batchnorm_weights(l, fp);
        }
        ಯದಿ(l.type == CRNN){
            load_convolutional_weights(*(l.ಒಳಹರಿವು_layer), fp);
            load_convolutional_weights(*(l.self_layer), fp);
            load_convolutional_weights(*(l.output_layer), fp);
        }
        ಯದಿ(l.type == RNN){
            load_connected_weights(*(l.ಒಳಹರಿವು_layer), fp, transpose);
            load_connected_weights(*(l.self_layer), fp, transpose);
            load_connected_weights(*(l.output_layer), fp, transpose);
        }
        ಯದಿ (l.type == LSTM) {
            load_connected_weights(*(l.wi), fp, transpose);
            load_connected_weights(*(l.wf), fp, transpose);
            load_connected_weights(*(l.wo), fp, transpose);
            load_connected_weights(*(l.wg), fp, transpose);
            load_connected_weights(*(l.ui), fp, transpose);
            load_connected_weights(*(l.uf), fp, transpose);
            load_connected_weights(*(l.uo), fp, transpose);
            load_connected_weights(*(l.ug), fp, transpose);
        }
        ಯದಿ (l.type == GRU) {
            ಯದಿ(1){
                load_connected_weights(*(l.wz), fp, transpose);
                load_connected_weights(*(l.wr), fp, transpose);
                load_connected_weights(*(l.wh), fp, transpose);
                load_connected_weights(*(l.uz), fp, transpose);
                load_connected_weights(*(l.ur), fp, transpose);
                load_connected_weights(*(l.uh), fp, transpose);
            }ಅನ್ಯಥಾ{
                load_connected_weights(*(l.reset_layer), fp, transpose);
                load_connected_weights(*(l.update_layer), fp, transpose);
                load_connected_weights(*(l.state_layer), fp, transpose);
            }
        }
        ಯದಿ(l.type == LOCAL){
            ಪೂರ್ಣಾಙ್ಕ locations = l.out_w*l.out_h;
            ಪೂರ್ಣಾಙ್ಕ size = l.size*l.size*l.c*l.n*locations;
            fread(l.biases, ಪರಿಮಾಣ(ಭಿನ್ನ), l.outputs, fp);
            fread(l.weights, ಪರಿಮಾಣ(ಭಿನ್ನ), size, fp);
#ifdef GPU
            ಯದಿ(gpu_index >= 0){
                ತಳ್ಳು_local_layer(l);
            }
#endif
        }
    }
    fprintf(stderr, "Done!\n");
    fclose(fp);
}

ಶೂನ್ಯ load_weights(network *net, ವರ್ಣ *filename)
{
    load_weights_upto(net, filename, 0, net->n);
}


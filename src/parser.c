#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <assert.h>

#ଅନ୍ତର୍ଭାଵ "activation_layer.h"
#ଅନ୍ତର୍ଭାଵ "logistic_layer.h"
#ଅନ୍ତର୍ଭାଵ "l2norm_layer.h"
#ଅନ୍ତର୍ଭାଵ "activations.h"
#ଅନ୍ତର୍ଭାଵ "avgpool_layer.h"
#ଅନ୍ତର୍ଭାଵ "batchnorm_layer.h"
#ଅନ୍ତର୍ଭାଵ "blas.h"
#ଅନ୍ତର୍ଭାଵ "connected_layer.h"
#ଅନ୍ତର୍ଭାଵ "deconvolutional_layer.h"
#ଅନ୍ତର୍ଭାଵ "convolutional_layer.h"
#ଅନ୍ତର୍ଭାଵ "cost_layer.h"
#ଅନ୍ତର୍ଭାଵ "crnn_layer.h"
#ଅନ୍ତର୍ଭାଵ "crop_layer.h"
#ଅନ୍ତର୍ଭାଵ "detection_layer.h"
#ଅନ୍ତର୍ଭାଵ "dropout_layer.h"
#ଅନ୍ତର୍ଭାଵ "gru_layer.h"
#ଅନ୍ତର୍ଭାଵ "list.h"
#ଅନ୍ତର୍ଭାଵ "local_layer.h"
#ଅନ୍ତର୍ଭାଵ "maxpool_layer.h"
#ଅନ୍ତର୍ଭାଵ "normalization_layer.h"
#ଅନ୍ତର୍ଭାଵ "option_list.h"
#ଅନ୍ତର୍ଭାଵ "parser.h"
#ଅନ୍ତର୍ଭାଵ "region_layer.h"
#ଅନ୍ତର୍ଭାଵ "yolo_layer.h"
#ଅନ୍ତର୍ଭାଵ "iseg_layer.h"
#ଅନ୍ତର୍ଭାଵ "reorg_layer.h"
#ଅନ୍ତର୍ଭାଵ "rnn_layer.h"
#ଅନ୍ତର୍ଭାଵ "route_layer.h"
#ଅନ୍ତର୍ଭାଵ "upsample_layer.h"
#ଅନ୍ତର୍ଭାଵ "shortcut_layer.h"
#ଅନ୍ତର୍ଭାଵ "softmax_layer.h"
#ଅନ୍ତର୍ଭାଵ "lstm_layer.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା{
    ଅକ୍ଷର *type;
    list *options;
}section;

list *read_cfg(ଅକ୍ଷର *filename);

LAYER_TYPE string_to_layer_type(ଅକ୍ଷର * type)
{

    ଯଦି (strcmp(type, "[shortcut]")==0) ଫେରନ୍ତୁ SHORTCUT;
    ଯଦି (strcmp(type, "[crop]")==0) ଫେରନ୍ତୁ CROP;
    ଯଦି (strcmp(type, "[cost]")==0) ଫେରନ୍ତୁ COST;
    ଯଦି (strcmp(type, "[detection]")==0) ଫେରନ୍ତୁ DETECTION;
    ଯଦି (strcmp(type, "[region]")==0) ଫେରନ୍ତୁ REGION;
    ଯଦି (strcmp(type, "[yolo]")==0) ଫେରନ୍ତୁ YOLO;
    ଯଦି (strcmp(type, "[iseg]")==0) ଫେରନ୍ତୁ ISEG;
    ଯଦି (strcmp(type, "[local]")==0) ଫେରନ୍ତୁ LOCAL;
    ଯଦି (strcmp(type, "[conv]")==0
            || strcmp(type, "[convolutional]")==0) ଫେରନ୍ତୁ CONVOLUTIONAL;
    ଯଦି (strcmp(type, "[deconv]")==0
            || strcmp(type, "[deconvolutional]")==0) ଫେରନ୍ତୁ DECONVOLUTIONAL;
    ଯଦି (strcmp(type, "[activation]")==0) ଫେରନ୍ତୁ ACTIVE;
    ଯଦି (strcmp(type, "[logistic]")==0) ଫେରନ୍ତୁ LOGXENT;
    ଯଦି (strcmp(type, "[l2norm]")==0) ଫେରନ୍ତୁ L2NORM;
    ଯଦି (strcmp(type, "[net]")==0
            || strcmp(type, "[network]")==0) ଫେରନ୍ତୁ NETWORK;
    ଯଦି (strcmp(type, "[crnn]")==0) ଫେରନ୍ତୁ CRNN;
    ଯଦି (strcmp(type, "[gru]")==0) ଫେରନ୍ତୁ GRU;
    ଯଦି (strcmp(type, "[lstm]") == 0) ଫେରନ୍ତୁ LSTM;
    ଯଦି (strcmp(type, "[rnn]")==0) ଫେରନ୍ତୁ RNN;
    ଯଦି (strcmp(type, "[conn]")==0
            || strcmp(type, "[connected]")==0) ଫେରନ୍ତୁ CONNECTED;
    ଯଦି (strcmp(type, "[max]")==0
            || strcmp(type, "[maxpool]")==0) ଫେରନ୍ତୁ MAXPOOL;
    ଯଦି (strcmp(type, "[reorg]")==0) ଫେରନ୍ତୁ REORG;
    ଯଦି (strcmp(type, "[avg]")==0
            || strcmp(type, "[avgpool]")==0) ଫେରନ୍ତୁ AVGPOOL;
    ଯଦି (strcmp(type, "[dropout]")==0) ଫେରନ୍ତୁ DROPOUT;
    ଯଦି (strcmp(type, "[lrn]")==0
            || strcmp(type, "[normalization]")==0) ଫେରନ୍ତୁ NORMALIZATION;
    ଯଦି (strcmp(type, "[batchnorm]")==0) ଫେରନ୍ତୁ BATCHNORM;
    ଯଦି (strcmp(type, "[soft]")==0
            || strcmp(type, "[softmax]")==0) ଫେରନ୍ତୁ SOFTMAX;
    ଯଦି (strcmp(type, "[route]")==0) ଫେରନ୍ତୁ ROUTE;
    ଯଦି (strcmp(type, "[upsample]")==0) ଫେରନ୍ତୁ UPSAMPLE;
    ଫେରନ୍ତୁ BLANK;
}

ଶୂନ୍ୟ free_section(section *s)
{
    ମୁକ୍ତ(s->type);
    ଗଣ୍ଠି *n = s->options->front;
    ଯେତେବେଳେ(n){
        kvp *pair = (kvp *)n->val;
        ମୁକ୍ତ(pair->ଚାବି);
        ମୁକ୍ତ(pair);
        ଗଣ୍ଠି *next = n->next;
        ମୁକ୍ତ(n);
        n = next;
    }
    ମୁକ୍ତ(s->options);
    ମୁକ୍ତ(s);
}

ଶୂନ୍ୟ parse_data(ଅକ୍ଷର *data, ଦଶମିକ *a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଯଦି(!data) ଫେରନ୍ତୁ;
    ଅକ୍ଷର *curr = data;
    ଅକ୍ଷର *next = data;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା done = 0;
    ପାଇଁ(i = 0; i < n && !done; ++i){
        ଯେତେବେଳେ(*++next !='\0' && *next != ',');
        ଯଦି(*next == '\0') done = 1;
        *next = '\0';
        sscanf(curr, "%g", &a[i]);
        curr = next+1;
    }
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା size_params{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା inputs;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା h;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା w;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା index;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା time_steps;
    network *net;
} size_params;

local_layer parse_local(list *options, size_params params)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = option_find_int(options, "filters",1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = option_find_int(options, "size",1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride = option_find_int(options, "stride",1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା pad = option_find_int(options, "pad",0);
    ଅକ୍ଷର *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ଯଦି(!(h && w && c)) error("Layer before local layer must output image.");

    local_layer layer = make_local_layer(batch,h,w,c,n,size,stride,pad,activation);

    ଫେରନ୍ତୁ layer;
}

layer parse_deconvolutional(list *options, size_params params)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = option_find_int(options, "filters",1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = option_find_int(options, "size",1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride = option_find_int(options, "stride",1);

    ଅକ୍ଷର *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ଯଦି(!(h && w && c)) error("Layer before deconvolutional layer must output image.");
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା pad = option_find_int_quiet(options, "pad",0);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା padding = option_find_int_quiet(options, "padding",0);
    ଯଦି(pad) padding = size/2;

    layer l = make_deconvolutional_layer(batch,h,w,c,n,size,stride,padding, activation, batch_normalize, params.net->adam);

    ଫେରନ୍ତୁ l;
}


convolutional_layer parse_convolutional(list *options, size_params params)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = option_find_int(options, "filters",1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = option_find_int(options, "size",1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride = option_find_int(options, "stride",1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା pad = option_find_int_quiet(options, "pad",0);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା padding = option_find_int_quiet(options, "padding",0);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା groups = option_find_int_quiet(options, "groups", 1);
    ଯଦି(pad) padding = size/2;

    ଅକ୍ଷର *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ଯଦି(!(h && w && c)) error("Layer before convolutional layer must output image.");
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଦ୍ୱିମିକ = option_find_int_quiet(options, "binary", 0);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା xnor = option_find_int_quiet(options, "xnor", 0);

    convolutional_layer layer = make_convolutional_layer(batch,h,w,c,n,groups,size,stride,padding,activation, batch_normalize, ଦ୍ୱିମିକ, xnor, params.net->adam);
    layer.flipped = option_find_int_quiet(options, "flipped", 0);
    layer.dot = option_find_float_quiet(options, "dot", 0);

    ଫେରନ୍ତୁ layer;
}

layer parse_crnn(list *options, size_params params)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା output_filters = option_find_int(options, "output_filters",1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା hidden_filters = option_find_int(options, "hidden_filters",1);
    ଅକ୍ଷର *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_crnn_layer(params.batch, params.w, params.h, params.c, hidden_filters, output_filters, params.time_steps, activation, batch_normalize);

    l.shortcut = option_find_int_quiet(options, "shortcut", 0);

    ଫେରନ୍ତୁ l;
}

layer parse_rnn(list *options, size_params params)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା output = option_find_int(options, "output",1);
    ଅକ୍ଷର *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_rnn_layer(params.batch, params.inputs, output, params.time_steps, activation, batch_normalize, params.net->adam);

    l.shortcut = option_find_int_quiet(options, "shortcut", 0);

    ଫେରନ୍ତୁ l;
}

layer parse_gru(list *options, size_params params)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା output = option_find_int(options, "output",1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_gru_layer(params.batch, params.inputs, output, params.time_steps, batch_normalize, params.net->adam);
    l.tanh = option_find_int_quiet(options, "tanh", 0);

    ଫେରନ୍ତୁ l;
}

layer parse_lstm(list *options, size_params params)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା output = option_find_int(options, "output", 1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_lstm_layer(params.batch, params.inputs, output, params.time_steps, batch_normalize, params.net->adam);

    ଫେରନ୍ତୁ l;
}

layer parse_connected(list *options, size_params params)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା output = option_find_int(options, "output",1);
    ଅକ୍ଷର *activation_s = option_find_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch_normalize = option_find_int_quiet(options, "batch_normalize", 0);

    layer l = make_connected_layer(params.batch, params.inputs, output, activation, batch_normalize, params.net->adam);
    ଫେରନ୍ତୁ l;
}

layer parse_softmax(list *options, size_params params)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା groups = option_find_int_quiet(options, "groups",1);
    layer l = make_softmax_layer(params.batch, params.inputs, groups);
    l.temperature = option_find_float_quiet(options, "temperature", 1);
    ଅକ୍ଷର *ବୃକ୍ଷ_file = option_find_str(options, "tree", 0);
    ଯଦି (ବୃକ୍ଷ_file) l.softmax_ବୃକ୍ଷ = read_ବୃକ୍ଷ(ବୃକ୍ଷ_file);
    l.w = params.w;
    l.h = params.h;
    l.c = params.c;
    l.spatial = option_find_float_quiet(options, "spatial", 0);
    l.noloss =  option_find_int_quiet(options, "noloss", 0);
    ଫେରନ୍ତୁ l;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା *parse_yolo_mask(ଅକ୍ଷର *a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ଅଙ୍କ)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା *mask = 0;
    ଯଦି(a){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = strlen(a);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 1;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
        ପାଇଁ(i = 0; i < len; ++i){
            ଯଦି (a[i] == ',') ++n;
        }
        mask = calloc(n, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
        ପାଇଁ(i = 0; i < n; ++i){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା val = atoi(a);
            mask[i] = val;
            a = strchr(a, ',')+1;
        }
        *ଅଙ୍କ = n;
    }
    ଫେରନ୍ତୁ mask;
}

layer parse_yolo(list *options, size_params params)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes = option_find_int(options, "classes", 20);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା total = option_find_int(options, "num", 1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ = total;

    ଅକ୍ଷର *a = option_find_str(options, "mask", 0);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା *mask = parse_yolo_mask(a, &ଅଙ୍କ);
    layer l = make_yolo_layer(params.batch, params.w, params.h, ଅଙ୍କ, total, mask, classes);
    ପ୍ରତିଜ୍ଞା(l.outputs == params.inputs);

    l.max_boxes = option_find_int_quiet(options, "max",90);
    l.jitter = option_find_float(options, "jitter", .2);

    l.ignore_thresh = option_find_float(options, "ignore_thresh", .5);
    l.truth_thresh = option_find_float(options, "truth_thresh", 1);
    l.random = option_find_int_quiet(options, "random", 0);

    ଅକ୍ଷର *map_file = option_find_str(options, "map", 0);
    ଯଦି (map_file) l.map = read_map(map_file);

    a = option_find_str(options, "anchors", 0);
    ଯଦି(a){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = strlen(a);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 1;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
        ପାଇଁ(i = 0; i < len; ++i){
            ଯଦି (a[i] == ',') ++n;
        }
        ପାଇଁ(i = 0; i < n; ++i){
            ଦଶମିକ bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    ଫେରନ୍ତୁ l;
}

layer parse_iseg(list *options, size_params params)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes = option_find_int(options, "classes", 20);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ids = option_find_int(options, "ids", 32);
    layer l = make_iseg_layer(params.batch, params.w, params.h, classes, ids);
    ପ୍ରତିଜ୍ଞା(l.outputs == params.inputs);
    ଫେରନ୍ତୁ l;
}

layer parse_region(list *options, size_params params)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା coords = option_find_int(options, "coords", 4);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes = option_find_int(options, "classes", 20);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ = option_find_int(options, "num", 1);

    layer l = make_region_layer(params.batch, params.w, params.h, ଅଙ୍କ, classes, coords);
    ପ୍ରତିଜ୍ଞା(l.outputs == params.inputs);

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

    ଅକ୍ଷର *ବୃକ୍ଷ_file = option_find_str(options, "tree", 0);
    ଯଦି (ବୃକ୍ଷ_file) l.softmax_ବୃକ୍ଷ = read_ବୃକ୍ଷ(ବୃକ୍ଷ_file);
    ଅକ୍ଷର *map_file = option_find_str(options, "map", 0);
    ଯଦି (map_file) l.map = read_map(map_file);

    ଅକ୍ଷର *a = option_find_str(options, "anchors", 0);
    ଯଦି(a){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = strlen(a);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 1;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
        ପାଇଁ(i = 0; i < len; ++i){
            ଯଦି (a[i] == ',') ++n;
        }
        ପାଇଁ(i = 0; i < n; ++i){
            ଦଶମିକ bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    ଫେରନ୍ତୁ l;
}

detection_layer parse_detection(list *options, size_params params)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା coords = option_find_int(options, "coords", 1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes = option_find_int(options, "classes", 1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା rescore = option_find_int(options, "rescore", 0);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ = option_find_int(options, "num", 1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା side = option_find_int(options, "side", 7);
    detection_layer layer = make_detection_layer(params.batch, params.inputs, ଅଙ୍କ, side, classes, coords, rescore);

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
    ଫେରନ୍ତୁ layer;
}

cost_layer parse_cost(list *options, size_params params)
{
    ଅକ୍ଷର *type_s = option_find_str(options, "type", "sse");
    COST_TYPE type = get_cost_type(type_s);
    ଦଶମିକ scale = option_find_float_quiet(options, "scale",1);
    cost_layer layer = make_cost_layer(params.batch, params.inputs, type, scale);
    layer.ratio =  option_find_float_quiet(options, "ratio",0);
    layer.noobject_scale =  option_find_float_quiet(options, "noobj", 1);
    layer.thresh =  option_find_float_quiet(options, "thresh",0);
    ଫେରନ୍ତୁ layer;
}

crop_layer parse_crop(list *options, size_params params)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା crop_height = option_find_int(options, "crop_height",1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା crop_width = option_find_int(options, "crop_width",1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା flip = option_find_int(options, "flip",0);
    ଦଶମିକ angle = option_find_float(options, "angle",0);
    ଦଶମିକ saturation = option_find_float(options, "saturation",1);
    ଦଶମିକ exposure = option_find_float(options, "exposure",1);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ଯଦି(!(h && w && c)) error("Layer before crop layer must output image.");

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା noadjust = option_find_int_quiet(options, "noadjust",0);

    crop_layer l = make_crop_layer(batch,h,w,c,crop_height,crop_width,flip, angle, saturation, exposure);
    l.shift = option_find_float(options, "shift", 0);
    l.noadjust = noadjust;
    ଫେରନ୍ତୁ l;
}

layer parse_reorg(list *options, size_params params)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride = option_find_int(options, "stride",1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା reverse = option_find_int_quiet(options, "reverse",0);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା flatten = option_find_int_quiet(options, "flatten",0);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା extra = option_find_int_quiet(options, "extra",0);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ଯଦି(!(h && w && c)) error("Layer before reorg layer must output image.");

    layer layer = make_reorg_layer(batch,w,h,c,stride,reverse, flatten, extra);
    ଫେରନ୍ତୁ layer;
}

maxpool_layer parse_maxpool(list *options, size_params params)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride = option_find_int(options, "stride",1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = option_find_int(options, "size",stride);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା padding = option_find_int_quiet(options, "padding", size-1);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ଯଦି(!(h && w && c)) error("Layer before maxpool layer must output image.");

    maxpool_layer layer = make_maxpool_layer(batch,h,w,c,size,stride,padding);
    ଫେରନ୍ତୁ layer;
}

avgpool_layer parse_avgpool(list *options, size_params params)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch,w,h,c;
    w = params.w;
    h = params.h;
    c = params.c;
    batch=params.batch;
    ଯଦି(!(h && w && c)) error("Layer before avgpool layer must output image.");

    avgpool_layer layer = make_avgpool_layer(batch,w,h,c);
    ଫେରନ୍ତୁ layer;
}

dropout_layer parse_dropout(list *options, size_params params)
{
    ଦଶମିକ probability = option_find_float(options, "probability", .5);
    dropout_layer layer = make_dropout_layer(params.batch, params.inputs, probability);
    layer.out_w = params.w;
    layer.out_h = params.h;
    layer.out_c = params.c;
    ଫେରନ୍ତୁ layer;
}

layer parse_normalization(list *options, size_params params)
{
    ଦଶମିକ alpha = option_find_float(options, "alpha", .0001);
    ଦଶମିକ beta =  option_find_float(options, "beta" , .75);
    ଦଶମିକ kappa = option_find_float(options, "kappa", 1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = option_find_int(options, "size", 5);
    layer l = make_normalization_layer(params.batch, params.w, params.h, params.c, size, alpha, beta, kappa);
    ଫେରନ୍ତୁ l;
}

layer parse_batchnorm(list *options, size_params params)
{
    layer l = make_batchnorm_layer(params.batch, params.w, params.h, params.c);
    ଫେରନ୍ତୁ l;
}

layer parse_shortcut(list *options, size_params params, network *net)
{
    ଅକ୍ଷର *l = option_find(options, "from");
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = atoi(l);
    ଯଦି(index < 0) index = params.index + index;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch = params.batch;
    layer from = net->layers[index];

    layer s = make_shortcut_layer(batch, index, params.w, params.h, params.c, from.out_w, from.out_h, from.out_c);

    ଅକ୍ଷର *activation_s = option_find_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);
    s.activation = activation;
    s.alpha = option_find_float_quiet(options, "alpha", 1);
    s.beta = option_find_float_quiet(options, "beta", 1);
    ଫେରନ୍ତୁ s;
}


layer parse_l2norm(list *options, size_params params)
{
    layer l = make_l2norm_layer(params.batch, params.inputs);
    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;
    ଫେରନ୍ତୁ l;
}


layer parse_logistic(list *options, size_params params)
{
    layer l = make_logistic_layer(params.batch, params.inputs);
    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;
    ଫେରନ୍ତୁ l;
}

layer parse_activation(list *options, size_params params)
{
    ଅକ୍ଷର *activation_s = option_find_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);

    layer l = make_activation_layer(params.batch, params.inputs, activation);

    l.h = l.out_h = params.h;
    l.w = l.out_w = params.w;
    l.c = l.out_c = params.c;

    ଫେରନ୍ତୁ l;
}

layer parse_upsample(list *options, size_params params, network *net)
{

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride = option_find_int(options, "stride",2);
    layer l = make_upsample_layer(params.batch, params.w, params.h, params.c, stride);
    l.scale = option_find_float_quiet(options, "scale", 1);
    ଫେରନ୍ତୁ l;
}

route_layer parse_route(list *options, size_params params, network *net)
{
    ଅକ୍ଷର *l = option_find(options, "layers");
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = strlen(l);
    ଯଦି(!l) error("Route Layer must specify input layers");
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 1;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < len; ++i){
        ଯଦି (l[i] == ',') ++n;
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା *layers = calloc(n, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା *sizes = calloc(n, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
    ପାଇଁ(i = 0; i < n; ++i){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = atoi(l);
        l = strchr(l, ',')+1;
        ଯଦି(index < 0) index = params.index + index;
        layers[i] = index;
        sizes[i] = net->layers[index].outputs;
    }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch = params.batch;

    route_layer layer = make_route_layer(batch, n, layers, sizes);

    convolutional_layer first = net->layers[layers[0]];
    layer.out_w = first.out_w;
    layer.out_h = first.out_h;
    layer.out_c = first.out_c;
    ପାଇଁ(i = 1; i < n; ++i){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = layers[i];
        convolutional_layer next = net->layers[index];
        ଯଦି(next.out_w == first.out_w && next.out_h == first.out_h){
            layer.out_c += next.out_c;
        }ଅନ୍ୟଥା{
            layer.out_h = layer.out_w = layer.out_c = 0;
        }
    }

    ଫେରନ୍ତୁ layer;
}

learning_rate_policy get_policy(ଅକ୍ଷର *s)
{
    ଯଦି (strcmp(s, "random")==0) ଫେରନ୍ତୁ RANDOM;
    ଯଦି (strcmp(s, "poly")==0) ଫେରନ୍ତୁ POLY;
    ଯଦି (strcmp(s, "constant")==0) ଫେରନ୍ତୁ CONSTANT;
    ଯଦି (strcmp(s, "step")==0) ଫେରନ୍ତୁ STEP;
    ଯଦି (strcmp(s, "exp")==0) ଫେରନ୍ତୁ EXP;
    ଯଦି (strcmp(s, "sigmoid")==0) ଫେରନ୍ତୁ SIG;
    ଯଦି (strcmp(s, "steps")==0) ଫେରନ୍ତୁ STEPS;
    fprintf(stderr, "Couldn't find policy %s, going with constant\n", s);
    ଫେରନ୍ତୁ CONSTANT;
}

ଶୂନ୍ୟ parse_net_options(list *options, network *net)
{
    net->batch = option_find_int(options, "batch",1);
    net->learning_rate = option_find_float(options, "learning_rate", .001);
    net->momentum = option_find_float(options, "momentum", .9);
    net->decay = option_find_float(options, "decay", .0001);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା subdivs = option_find_int(options, "subdivisions",1);
    net->time_steps = option_find_int_quiet(options, "time_steps",1);
    net->notruth = option_find_int_quiet(options, "notruth",0);
    net->batch /= subdivs;
    net->batch *= net->time_steps;
    net->subdivisions = subdivs;
    net->random = option_find_int_quiet(options, "random", 0);

    net->adam = option_find_int_quiet(options, "adam", 0);
    ଯଦି(net->adam){
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
    net->max_ratio = option_find_float_quiet(options, "max_ratio", (ଦଶମିକ) net->max_crop / net->w);
    net->min_ratio = option_find_float_quiet(options, "min_ratio", (ଦଶମିକ) net->min_crop / net->w);
    net->center = option_find_int_quiet(options, "center",0);
    net->clip = option_find_float_quiet(options, "clip", 0);

    net->angle = option_find_float_quiet(options, "angle", 0);
    net->aspect = option_find_float_quiet(options, "aspect", 1);
    net->saturation = option_find_float_quiet(options, "saturation", 1);
    net->exposure = option_find_float_quiet(options, "exposure", 1);
    net->hue = option_find_float_quiet(options, "hue", 0);

    ଯଦି(!net->inputs && !(net->h && net->w && net->c)) error("No input parameters supplied");

    ଅକ୍ଷର *policy_s = option_find_str(options, "policy", "constant");
    net->policy = get_policy(policy_s);
    net->burn_in = option_find_int_quiet(options, "burn_in", 0);
    net->power = option_find_float_quiet(options, "power", 4);
    ଯଦି(net->policy == STEP){
        net->step = option_find_int(options, "step", 1);
        net->scale = option_find_float(options, "scale", 1);
    } ଅନ୍ୟଥା ଯଦି (net->policy == STEPS){
        ଅକ୍ଷର *l = option_find(options, "steps");
        ଅକ୍ଷର *p = option_find(options, "scales");
        ଯଦି(!l || !p) error("STEPS policy must have steps and scales in cfg file");

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = strlen(l);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 1;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
        ପାଇଁ(i = 0; i < len; ++i){
            ଯଦି (l[i] == ',') ++n;
        }
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା *steps = calloc(n, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
        ଦଶମିକ *scales = calloc(n, ଆକାର(ଦଶମିକ));
        ପାଇଁ(i = 0; i < n; ++i){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା step    = atoi(l);
            ଦଶମିକ scale = atof(p);
            l = strchr(l, ',')+1;
            p = strchr(p, ',')+1;
            steps[i] = step;
            scales[i] = scale;
        }
        net->scales = scales;
        net->steps = steps;
        net->ଅଙ୍କ_steps = n;
    } ଅନ୍ୟଥା ଯଦି (net->policy == EXP){
        net->gamma = option_find_float(options, "gamma", 1);
    } ଅନ୍ୟଥା ଯଦି (net->policy == SIG){
        net->gamma = option_find_float(options, "gamma", 1);
        net->step = option_find_int(options, "step", 1);
    } ଅନ୍ୟଥା ଯଦି (net->policy == POLY || net->policy == RANDOM){
    }
    net->max_batches = option_find_int(options, "max_batches", 0);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_network(section *s)
{
    ଫେରନ୍ତୁ (strcmp(s->type, "[net]")==0
            || strcmp(s->type, "[network]")==0);
}

network *parse_network_cfg(ଅକ୍ଷର *filename)
{
    list *sections = read_cfg(filename);
    ଗଣ୍ଠି *n = sections->front;
    ଯଦି(!n) error("Config file has no sections");
    network *net = make_network(sections->size - 1);
    net->gpu_index = gpu_index;
    size_params params;

    section *s = (section *)n->val;
    list *options = s->options;
    ଯଦି(!is_network(s)) error("First section must be [net] or [network]");
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
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    free_section(s);
    fprintf(stderr, "layer     filters    size              input                output\n");
    ଯେତେବେଳେ(n){
        params.index = ଗଣନା_2;
        fprintf(stderr, "%5d ", ଗଣନା_2);
        s = (section *)n->val;
        options = s->options;
        layer l = {0};
        LAYER_TYPE lt = string_to_layer_type(s->type);
        ଯଦି(lt == CONVOLUTIONAL){
            l = parse_convolutional(options, params);
        }ଅନ୍ୟଥା ଯଦି(lt == DECONVOLUTIONAL){
            l = parse_deconvolutional(options, params);
        }ଅନ୍ୟଥା ଯଦି(lt == LOCAL){
            l = parse_local(options, params);
        }ଅନ୍ୟଥା ଯଦି(lt == ACTIVE){
            l = parse_activation(options, params);
        }ଅନ୍ୟଥା ଯଦି(lt == LOGXENT){
            l = parse_logistic(options, params);
        }ଅନ୍ୟଥା ଯଦି(lt == L2NORM){
            l = parse_l2norm(options, params);
        }ଅନ୍ୟଥା ଯଦି(lt == RNN){
            l = parse_rnn(options, params);
        }ଅନ୍ୟଥା ଯଦି(lt == GRU){
            l = parse_gru(options, params);
        }ଅନ୍ୟଥା ଯଦି (lt == LSTM) {
            l = parse_lstm(options, params);
        }ଅନ୍ୟଥା ଯଦି(lt == CRNN){
            l = parse_crnn(options, params);
        }ଅନ୍ୟଥା ଯଦି(lt == CONNECTED){
            l = parse_connected(options, params);
        }ଅନ୍ୟଥା ଯଦି(lt == CROP){
            l = parse_crop(options, params);
        }ଅନ୍ୟଥା ଯଦି(lt == COST){
            l = parse_cost(options, params);
        }ଅନ୍ୟଥା ଯଦି(lt == REGION){
            l = parse_region(options, params);
        }ଅନ୍ୟଥା ଯଦି(lt == YOLO){
            l = parse_yolo(options, params);
        }ଅନ୍ୟଥା ଯଦି(lt == ISEG){
            l = parse_iseg(options, params);
        }ଅନ୍ୟଥା ଯଦି(lt == DETECTION){
            l = parse_detection(options, params);
        }ଅନ୍ୟଥା ଯଦି(lt == SOFTMAX){
            l = parse_softmax(options, params);
            net->hierarchy = l.softmax_ବୃକ୍ଷ;
        }ଅନ୍ୟଥା ଯଦି(lt == NORMALIZATION){
            l = parse_normalization(options, params);
        }ଅନ୍ୟଥା ଯଦି(lt == BATCHNORM){
            l = parse_batchnorm(options, params);
        }ଅନ୍ୟଥା ଯଦି(lt == MAXPOOL){
            l = parse_maxpool(options, params);
        }ଅନ୍ୟଥା ଯଦି(lt == REORG){
            l = parse_reorg(options, params);
        }ଅନ୍ୟଥା ଯଦି(lt == AVGPOOL){
            l = parse_avgpool(options, params);
        }ଅନ୍ୟଥା ଯଦି(lt == ROUTE){
            l = parse_route(options, params, net);
        }ଅନ୍ୟଥା ଯଦି(lt == UPSAMPLE){
            l = parse_upsample(options, params, net);
        }ଅନ୍ୟଥା ଯଦି(lt == SHORTCUT){
            l = parse_shortcut(options, params, net);
        }ଅନ୍ୟଥା ଯଦି(lt == DROPOUT){
            l = parse_dropout(options, params);
            l.output = net->layers[ଗଣନା_2-1].output;
            l.delta = net->layers[ଗଣନା_2-1].delta;
#ifdef GPU
            l.output_gpu = net->layers[ଗଣନା_2-1].output_gpu;
            l.delta_gpu = net->layers[ଗଣନା_2-1].delta_gpu;
#endif
        }ଅନ୍ୟଥା{
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
        net->layers[ଗଣନା_2] = l;
        ଯଦି (l.workspace_size > workspace_size) workspace_size = l.workspace_size;
        free_section(s);
        n = n->next;
        ++ଗଣନା_2;
        ଯଦି(n){
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
    ଯଦି(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = out.output;
    net->ନିବେଶ = calloc(net->inputs*net->batch, ଆକାର(ଦଶମିକ));
    net->truth = calloc(net->truths*net->batch, ଆକାର(ଦଶମିକ));
#ifdef GPU
    net->output_gpu = out.output_gpu;
    net->ନିବେଶ_gpu = cuda_make_array(net->ନିବେଶ, net->inputs*net->batch);
    net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
#endif
    ଯଦି(workspace_size){
        //printf("%ld\n", workspace_size);
#ifdef GPU
        ଯଦି(gpu_index >= 0){
            net->workspace = cuda_make_array(0, (workspace_size-1)/ଆକାର(ଦଶମିକ)+1);
        }ଅନ୍ୟଥା {
            net->workspace = calloc(1, workspace_size);
        }
#ଅନ୍ୟଥା
        net->workspace = calloc(1, workspace_size);
#endif
    }
    ଫେରନ୍ତୁ net;
}

list *read_cfg(ଅକ୍ଷର *filename)
{
    FILE *file = fopen(filename, "r");
    ଯଦି(file == 0) file_error(filename);
    ଅକ୍ଷର *ଧାଡ଼ି;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nu = 0;
    list *options = make_list();
    section *current = 0;
    ଯେତେବେଳେ((ଧାଡ଼ି=fgetl(file)) != 0){
        ++ nu;
        strip(ଧାଡ଼ି);
        ବାଛ(ଧାଡ଼ି[0]){
            ସ୍ଥିତି '[':
                current = malloc(ଆକାର(section));
                list_ଭର୍ତ୍ତି(options, current);
                current->options = make_list();
                current->type = ଧାଡ଼ି;
                ଭାଙ୍ଗ;
            ସ୍ଥିତି '\0':
            ସ୍ଥିତି '#':
            ସ୍ଥିତି ';':
                ମୁକ୍ତ(ଧାଡ଼ି);
                ଭାଙ୍ଗ;
            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                ଯଦି(!read_option(ଧାଡ଼ି, current->options)){
                    fprintf(stderr, "Config file error line %d, could parse: %s\n", nu, ଧାଡ଼ି);
                    ମୁକ୍ତ(ଧାଡ଼ି);
                }
                ଭାଙ୍ଗ;
        }
    }
    fclose(file);
    ଫେରନ୍ତୁ options;
}

ଶୂନ୍ୟ save_convolutional_weights_ଦ୍ୱିମିକ(layer l, FILE *fp)
{
#ifdef GPU
    ଯଦି(gpu_index >= 0){
        pull_convolutional_layer(l);
    }
#endif
    binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.ଦ୍ୱିମିକ_weights);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = l.c*l.size*l.size;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, k;
    fwrite(l.biases, ଆକାର(ଦଶମିକ), l.n, fp);
    ଯଦି (l.batch_normalize){
        fwrite(l.scales, ଆକାର(ଦଶମିକ), l.n, fp);
        fwrite(l.rolling_mean, ଆକାର(ଦଶମିକ), l.n, fp);
        fwrite(l.rolling_variance, ଆକାର(ଦଶମିକ), l.n, fp);
    }
    ପାଇଁ(i = 0; i < l.n; ++i){
        ଦଶମିକ mean = l.ଦ୍ୱିମିକ_weights[i*size];
        ଯଦି(mean < 0) mean = -mean;
        fwrite(&mean, ଆକାର(ଦଶମିକ), 1, fp);
        ପାଇଁ(j = 0; j < size/8; ++j){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = i*size + j*8;
            unsigned ଅକ୍ଷର c = 0;
            ପାଇଁ(k = 0; k < 8; ++k){
                ଯଦି (j*8 + k >= size) ଭାଙ୍ଗ;
                ଯଦି (l.ଦ୍ୱିମିକ_weights[index + k] > 0) c = (c | 1<<k);
            }
            fwrite(&c, ଆକାର(ଅକ୍ଷର), 1, fp);
        }
    }
}

ଶୂନ୍ୟ save_convolutional_weights(layer l, FILE *fp)
{
    ଯଦି(l.ଦ୍ୱିମିକ){
        //save_convolutional_weights_binary(l, fp);
        //return;
    }
#ifdef GPU
    ଯଦି(gpu_index >= 0){
        pull_convolutional_layer(l);
    }
#endif
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ = l.nweights;
    fwrite(l.biases, ଆକାର(ଦଶମିକ), l.n, fp);
    ଯଦି (l.batch_normalize){
        fwrite(l.scales, ଆକାର(ଦଶମିକ), l.n, fp);
        fwrite(l.rolling_mean, ଆକାର(ଦଶମିକ), l.n, fp);
        fwrite(l.rolling_variance, ଆକାର(ଦଶମିକ), l.n, fp);
    }
    fwrite(l.weights, ଆକାର(ଦଶମିକ), ଅଙ୍କ, fp);
}

ଶୂନ୍ୟ save_batchnorm_weights(layer l, FILE *fp)
{
#ifdef GPU
    ଯଦି(gpu_index >= 0){
        pull_batchnorm_layer(l);
    }
#endif
    fwrite(l.scales, ଆକାର(ଦଶମିକ), l.c, fp);
    fwrite(l.rolling_mean, ଆକାର(ଦଶମିକ), l.c, fp);
    fwrite(l.rolling_variance, ଆକାର(ଦଶମିକ), l.c, fp);
}

ଶୂନ୍ୟ save_connected_weights(layer l, FILE *fp)
{
#ifdef GPU
    ଯଦି(gpu_index >= 0){
        pull_connected_layer(l);
    }
#endif
    fwrite(l.biases, ଆକାର(ଦଶମିକ), l.outputs, fp);
    fwrite(l.weights, ଆକାର(ଦଶମିକ), l.outputs*l.inputs, fp);
    ଯଦି (l.batch_normalize){
        fwrite(l.scales, ଆକାର(ଦଶମିକ), l.outputs, fp);
        fwrite(l.rolling_mean, ଆକାର(ଦଶମିକ), l.outputs, fp);
        fwrite(l.rolling_variance, ଆକାର(ଦଶମିକ), l.outputs, fp);
    }
}

ଶୂନ୍ୟ save_weights_upto(network *net, ଅକ୍ଷର *filename, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cutoff)
{
#ifdef GPU
    ଯଦି(net->gpu_index >= 0){
        cuda_set_device(net->gpu_index);
    }
#endif
    fprintf(stderr, "Saving weights to %s\n", filename);
    FILE *fp = fopen(filename, "wb");
    ଯଦି(!fp) file_error(filename);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା major = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା minor = 2;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା revision = 0;
    fwrite(&major, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା), 1, fp);
    fwrite(&minor, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା), 1, fp);
    fwrite(&revision, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା), 1, fp);
    fwrite(net->seen, ଆକାର(size_t), 1, fp);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        ଯଦି (l.dontsave) ଜାରି;
        ଯଦି(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            save_convolutional_weights(l, fp);
        } ଯଦି(l.type == CONNECTED){
            save_connected_weights(l, fp);
        } ଯଦି(l.type == BATCHNORM){
            save_batchnorm_weights(l, fp);
        } ଯଦି(l.type == RNN){
            save_connected_weights(*(l.ନିବେଶ_layer), fp);
            save_connected_weights(*(l.self_layer), fp);
            save_connected_weights(*(l.output_layer), fp);
        } ଯଦି (l.type == LSTM) {
            save_connected_weights(*(l.wi), fp);
            save_connected_weights(*(l.wf), fp);
            save_connected_weights(*(l.wo), fp);
            save_connected_weights(*(l.wg), fp);
            save_connected_weights(*(l.ui), fp);
            save_connected_weights(*(l.uf), fp);
            save_connected_weights(*(l.uo), fp);
            save_connected_weights(*(l.ug), fp);
        } ଯଦି (l.type == GRU) {
            ଯଦି(1){
                save_connected_weights(*(l.wz), fp);
                save_connected_weights(*(l.wr), fp);
                save_connected_weights(*(l.wh), fp);
                save_connected_weights(*(l.uz), fp);
                save_connected_weights(*(l.ur), fp);
                save_connected_weights(*(l.uh), fp);
            }ଅନ୍ୟଥା{
                save_connected_weights(*(l.reset_layer), fp);
                save_connected_weights(*(l.update_layer), fp);
                save_connected_weights(*(l.state_layer), fp);
            }
        }  ଯଦି(l.type == CRNN){
            save_convolutional_weights(*(l.ନିବେଶ_layer), fp);
            save_convolutional_weights(*(l.self_layer), fp);
            save_convolutional_weights(*(l.output_layer), fp);
        } ଯଦି(l.type == LOCAL){
#ifdef GPU
            ଯଦି(gpu_index >= 0){
                pull_local_layer(l);
            }
#endif
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା locations = l.out_w*l.out_h;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = l.size*l.size*l.c*l.n*locations;
            fwrite(l.biases, ଆକାର(ଦଶମିକ), l.outputs, fp);
            fwrite(l.weights, ଆକାର(ଦଶମିକ), size, fp);
        }
    }
    fclose(fp);
}
ଶୂନ୍ୟ save_weights(network *net, ଅକ୍ଷର *filename)
{
    save_weights_upto(net, filename, net->n);
}

ଶୂନ୍ୟ transpose_matrix(ଦଶମିକ *a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols)
{
    ଦଶମିକ *transpose = calloc(rows*cols, ଆକାର(ଦଶମିକ));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା x, y;
    ପାଇଁ(x = 0; x < rows; ++x){
        ପାଇଁ(y = 0; y < cols; ++y){
            transpose[y*rows + x] = a[x*cols + y];
        }
    }
    memcpy(a, transpose, rows*cols*ଆକାର(ଦଶମିକ));
    ମୁକ୍ତ(transpose);
}

ଶୂନ୍ୟ load_connected_weights(layer l, FILE *fp, ପୂର୍ଣ୍ଣସଂଖ୍ୟା transpose)
{
    fread(l.biases, ଆକାର(ଦଶମିକ), l.outputs, fp);
    fread(l.weights, ଆକାର(ଦଶମିକ), l.outputs*l.inputs, fp);
    ଯଦି(transpose){
        transpose_matrix(l.weights, l.inputs, l.outputs);
    }
    //printf("Biases: %f mean %f variance\n", mean_array(l.biases, l.outputs), variance_array(l.biases, l.outputs));
    //printf("Weights: %f mean %f variance\n", mean_array(l.weights, l.outputs*l.inputs), variance_array(l.weights, l.outputs*l.inputs));
    ଯଦି (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, ଆକାର(ଦଶମିକ), l.outputs, fp);
        fread(l.rolling_mean, ଆକାର(ଦଶମିକ), l.outputs, fp);
        fread(l.rolling_variance, ଆକାର(ଦଶମିକ), l.outputs, fp);
        //printf("Scales: %f mean %f variance\n", mean_array(l.scales, l.outputs), variance_array(l.scales, l.outputs));
        //printf("rolling_mean: %f mean %f variance\n", mean_array(l.rolling_mean, l.outputs), variance_array(l.rolling_mean, l.outputs));
        //printf("rolling_variance: %f mean %f variance\n", mean_array(l.rolling_variance, l.outputs), variance_array(l.rolling_variance, l.outputs));
    }
#ifdef GPU
    ଯଦି(gpu_index >= 0){
        ଠେଲ_connected_layer(l);
    }
#endif
}

ଶୂନ୍ୟ load_batchnorm_weights(layer l, FILE *fp)
{
    fread(l.scales, ଆକାର(ଦଶମିକ), l.c, fp);
    fread(l.rolling_mean, ଆକାର(ଦଶମିକ), l.c, fp);
    fread(l.rolling_variance, ଆକାର(ଦଶମିକ), l.c, fp);
#ifdef GPU
    ଯଦି(gpu_index >= 0){
        ଠେଲ_batchnorm_layer(l);
    }
#endif
}

ଶୂନ୍ୟ load_convolutional_weights_ଦ୍ୱିମିକ(layer l, FILE *fp)
{
    fread(l.biases, ଆକାର(ଦଶମିକ), l.n, fp);
    ଯଦି (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, ଆକାର(ଦଶମିକ), l.n, fp);
        fread(l.rolling_mean, ଆକାର(ଦଶମିକ), l.n, fp);
        fread(l.rolling_variance, ଆକାର(ଦଶମିକ), l.n, fp);
    }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = l.c*l.size*l.size;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, k;
    ପାଇଁ(i = 0; i < l.n; ++i){
        ଦଶମିକ mean = 0;
        fread(&mean, ଆକାର(ଦଶମିକ), 1, fp);
        ପାଇଁ(j = 0; j < size/8; ++j){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = i*size + j*8;
            unsigned ଅକ୍ଷର c = 0;
            fread(&c, ଆକାର(ଅକ୍ଷର), 1, fp);
            ପାଇଁ(k = 0; k < 8; ++k){
                ଯଦି (j*8 + k >= size) ଭାଙ୍ଗ;
                l.weights[index + k] = (c & 1<<k) ? mean : -mean;
            }
        }
    }
#ifdef GPU
    ଯଦି(gpu_index >= 0){
        ଠେଲ_convolutional_layer(l);
    }
#endif
}

ଶୂନ୍ୟ load_convolutional_weights(layer l, FILE *fp)
{
    ଯଦି(l.ଦ୍ୱିମିକ){
        //load_convolutional_weights_binary(l, fp);
        //return;
    }
    ଯଦି(l.numload) l.n = l.numload;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ = l.c/l.groups*l.n*l.size*l.size;
    fread(l.biases, ଆକାର(ଦଶମିକ), l.n, fp);
    ଯଦି (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, ଆକାର(ଦଶମିକ), l.n, fp);
        fread(l.rolling_mean, ଆକାର(ଦଶମିକ), l.n, fp);
        fread(l.rolling_variance, ଆକାର(ଦଶମିକ), l.n, fp);
        ଯଦି(0){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
            ପାଇଁ(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_mean[i]);
            }
            printf("\n");
            ପାଇଁ(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_variance[i]);
            }
            printf("\n");
        }
        ଯଦି(0){
            fill_cpu(l.n, 0, l.rolling_mean, 1);
            fill_cpu(l.n, 0, l.rolling_variance, 1);
        }
        ଯଦି(0){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
            ପାଇଁ(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_mean[i]);
            }
            printf("\n");
            ପାଇଁ(i = 0; i < l.n; ++i){
                printf("%g, ", l.rolling_variance[i]);
            }
            printf("\n");
        }
    }
    fread(l.weights, ଆକାର(ଦଶମିକ), ଅଙ୍କ, fp);
    //if(l.c == 3) scal_cpu(num, 1./256, l.weights, 1);
    ଯଦି (l.flipped) {
        transpose_matrix(l.weights, l.c*l.size*l.size, l.n);
    }
    //if (l.binary) binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.weights);
#ifdef GPU
    ଯଦି(gpu_index >= 0){
        ଠେଲ_convolutional_layer(l);
    }
#endif
}


ଶୂନ୍ୟ load_weights_upto(network *net, ଅକ୍ଷର *filename, ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cutoff)
{
#ifdef GPU
    ଯଦି(net->gpu_index >= 0){
        cuda_set_device(net->gpu_index);
    }
#endif
    fprintf(stderr, "Loading weights from %s...", filename);
    fflush(stdout);
    FILE *fp = fopen(filename, "rb");
    ଯଦି(!fp) file_error(filename);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା major;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା minor;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା revision;
    fread(&major, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା), 1, fp);
    fread(&minor, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା), 1, fp);
    fread(&revision, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା), 1, fp);
    ଯଦି ((major*10 + minor) >= 2 && major < 1000 && minor < 1000){
        fread(net->seen, ଆକାର(size_t), 1, fp);
    } ଅନ୍ୟଥା {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା iseen = 0;
        fread(&iseen, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା), 1, fp);
        *net->seen = iseen;
    }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା transpose = (major > 1000) || (minor > 1000);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = start; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        ଯଦି (l.dontload) ଜାରି;
        ଯଦି(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            load_convolutional_weights(l, fp);
        }
        ଯଦି(l.type == CONNECTED){
            load_connected_weights(l, fp, transpose);
        }
        ଯଦି(l.type == BATCHNORM){
            load_batchnorm_weights(l, fp);
        }
        ଯଦି(l.type == CRNN){
            load_convolutional_weights(*(l.ନିବେଶ_layer), fp);
            load_convolutional_weights(*(l.self_layer), fp);
            load_convolutional_weights(*(l.output_layer), fp);
        }
        ଯଦି(l.type == RNN){
            load_connected_weights(*(l.ନିବେଶ_layer), fp, transpose);
            load_connected_weights(*(l.self_layer), fp, transpose);
            load_connected_weights(*(l.output_layer), fp, transpose);
        }
        ଯଦି (l.type == LSTM) {
            load_connected_weights(*(l.wi), fp, transpose);
            load_connected_weights(*(l.wf), fp, transpose);
            load_connected_weights(*(l.wo), fp, transpose);
            load_connected_weights(*(l.wg), fp, transpose);
            load_connected_weights(*(l.ui), fp, transpose);
            load_connected_weights(*(l.uf), fp, transpose);
            load_connected_weights(*(l.uo), fp, transpose);
            load_connected_weights(*(l.ug), fp, transpose);
        }
        ଯଦି (l.type == GRU) {
            ଯଦି(1){
                load_connected_weights(*(l.wz), fp, transpose);
                load_connected_weights(*(l.wr), fp, transpose);
                load_connected_weights(*(l.wh), fp, transpose);
                load_connected_weights(*(l.uz), fp, transpose);
                load_connected_weights(*(l.ur), fp, transpose);
                load_connected_weights(*(l.uh), fp, transpose);
            }ଅନ୍ୟଥା{
                load_connected_weights(*(l.reset_layer), fp, transpose);
                load_connected_weights(*(l.update_layer), fp, transpose);
                load_connected_weights(*(l.state_layer), fp, transpose);
            }
        }
        ଯଦି(l.type == LOCAL){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା locations = l.out_w*l.out_h;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = l.size*l.size*l.c*l.n*locations;
            fread(l.biases, ଆକାର(ଦଶମିକ), l.outputs, fp);
            fread(l.weights, ଆକାର(ଦଶମିକ), size, fp);
#ifdef GPU
            ଯଦି(gpu_index >= 0){
                ଠେଲ_local_layer(l);
            }
#endif
        }
    }
    fprintf(stderr, "Done!\n");
    fclose(fp);
}

ଶୂନ୍ୟ load_weights(network *net, ଅକ୍ଷର *filename)
{
    load_weights_upto(net, filename, 0, net->n);
}


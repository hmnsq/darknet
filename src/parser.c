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

LAYER_TYPE string_منزل_layer_type(حرف * type)
{

    إذا (strcmp(type, "[shortcut]")==0) أرجع SHORTCUT;
    إذا (strcmp(type, "[crop]")==0) أرجع CROP;
    إذا (strcmp(type, "[cost]")==0) أرجع لاگت;
    إذا (strcmp(type, "[detection]")==0) أرجع DETECTION;
    إذا (strcmp(type, "[region]")==0) أرجع REGION;
    إذا (strcmp(type, "[yolo]")==0) أرجع YOLO;
    إذا (strcmp(type, "[iseg]")==0) أرجع ISEG;
    إذا (strcmp(type, "[local]")==0) أرجع LOCAL;
    إذا (strcmp(type, "[conv]")==0
            || strcmp(type, "[convolutional]")==0) أرجع CONVOLUTIONAL;
    إذا (strcmp(type, "[deconv]")==0
            || strcmp(type, "[deconvolutional]")==0) أرجع DECONVOLUTIONAL;
    إذا (strcmp(type, "[activation]")==0) أرجع سرگرم;
    إذا (strcmp(type, "[logistic]")==0) أرجع LOGXENT;
    إذا (strcmp(type, "[l2norm]")==0) أرجع L2NORM;
    إذا (strcmp(type, "[net]")==0
            || strcmp(type, "[network]")==0) أرجع جال_2;
    إذا (strcmp(type, "[crnn]")==0) أرجع CRNN;
    إذا (strcmp(type, "[gru]")==0) أرجع GRU;
    إذا (strcmp(type, "[lstm]") == 0) أرجع LSTM;
    إذا (strcmp(type, "[rnn]")==0) أرجع RNN;
    إذا (strcmp(type, "[conn]")==0
            || strcmp(type, "[connected]")==0) أرجع CONNECTED;
    إذا (strcmp(type, "[max]")==0
            || strcmp(type, "[maxpool]")==0) أرجع MAXPOOL;
    إذا (strcmp(type, "[reorg]")==0) أرجع REORG;
    إذا (strcmp(type, "[avg]")==0
            || strcmp(type, "[avgpool]")==0) أرجع AVGPOOL;
    إذا (strcmp(type, "[dropout]")==0) أرجع DROPOUT;
    إذا (strcmp(type, "[lrn]")==0
            || strcmp(type, "[normalization]")==0) أرجع NORMALIZATION;
    إذا (strcmp(type, "[batchnorm]")==0) أرجع BATCHNORM;
    إذا (strcmp(type, "[soft]")==0
            || strcmp(type, "[softmax]")==0) أرجع SOFTMAX;
    إذا (strcmp(type, "[route]")==0) أرجع ROUTE;
    إذا (strcmp(type, "[upsample]")==0) أرجع UPSAMPLE;
    أرجع BLANK;
}

فراغ free_section(section *s)
{
    حرر(s->type);
    گرہ *n = s->options->front;
    طالما(n){
        kvp *pair = (kvp *)n->val;
        حرر(pair->کلید);
        حرر(pair);
        گرہ *اگلا = n->اگلا;
        حرر(n);
        n = اگلا;
    }
    حرر(s->options);
    حرر(s);
}

فراغ parse_انکڑے(حرف *انکڑے, عائم *a, صحيح n)
{
    صحيح i;
    إذا(!انکڑے) أرجع;
    حرف *curr = انکڑے;
    حرف *اگلا = انکڑے;
    صحيح پورا = 0;
    لكل(i = 0; i < n && !پورا; ++i){
        طالما(*++اگلا !='\0' && *اگلا != ',');
        إذا(*اگلا == '\0') پورا = 1;
        *اگلا = '\0';
        لڑی_توں_پڑھو(curr, "%g", &a[i]);
        curr = اگلا+1;
    }
}

تعريفنوع بنية ناپ_params{
    صحيح batch;
    صحيح inputs;
    صحيح h;
    صحيح w;
    صحيح c;
    صحيح اشاریہ;
    صحيح ویلا_قدماں;
    جال *net;
} ناپ_params;

local_layer parse_local(list *options, ناپ_params params)
{
    صحيح n = option_لبھو_int(options, "filters",1);
    صحيح ناپ = option_لبھو_int(options, "size",1);
    صحيح stride = option_لبھو_int(options, "stride",1);
    صحيح pad = option_لبھو_int(options, "pad",0);
    حرف *activation_s = option_لبھو_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    صحيح batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    إذا(!(h && w && c)) error("Layer before local layer must output image.");

    local_layer layer = make_local_layer(batch,h,w,c,n,ناپ,stride,pad,activation);

    أرجع layer;
}

layer parse_deconvolutional(list *options, ناپ_params params)
{
    صحيح n = option_لبھو_int(options, "filters",1);
    صحيح ناپ = option_لبھو_int(options, "size",1);
    صحيح stride = option_لبھو_int(options, "stride",1);

    حرف *activation_s = option_لبھو_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    صحيح batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    إذا(!(h && w && c)) error("Layer before deconvolutional layer must output image.");
    صحيح batch_normalize = option_لبھو_int_quiet(options, "batch_normalize", 0);
    صحيح pad = option_لبھو_int_quiet(options, "pad",0);
    صحيح padding = option_لبھو_int_quiet(options, "padding",0);
    إذا(pad) padding = ناپ/2;

    layer l = make_deconvolutional_layer(batch,h,w,c,n,ناپ,stride,padding, activation, batch_normalize, params.net->adam);

    أرجع l;
}


convolutional_layer parse_convolutional(list *options, ناپ_params params)
{
    صحيح n = option_لبھو_int(options, "filters",1);
    صحيح ناپ = option_لبھو_int(options, "size",1);
    صحيح stride = option_لبھو_int(options, "stride",1);
    صحيح pad = option_لبھو_int_quiet(options, "pad",0);
    صحيح padding = option_لبھو_int_quiet(options, "padding",0);
    صحيح groups = option_لبھو_int_quiet(options, "groups", 1);
    إذا(pad) padding = ناپ/2;

    حرف *activation_s = option_لبھو_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    صحيح batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    إذا(!(h && w && c)) error("Layer before convolutional layer must output image.");
    صحيح batch_normalize = option_لبھو_int_quiet(options, "batch_normalize", 0);
    صحيح ثنائی = option_لبھو_int_quiet(options, "binary", 0);
    صحيح xnor = option_لبھو_int_quiet(options, "xnor", 0);

    convolutional_layer layer = make_convolutional_layer(batch,h,w,c,n,groups,ناپ,stride,padding,activation, batch_normalize, ثنائی, xnor, params.net->adam);
    layer.flipped = option_لبھو_int_quiet(options, "flipped", 0);
    layer.dot = option_لبھو_float_quiet(options, "dot", 0);

    أرجع layer;
}

layer parse_crnn(list *options, ناپ_params params)
{
    صحيح output_filters = option_لبھو_int(options, "output_filters",1);
    صحيح hidden_filters = option_لبھو_int(options, "hidden_filters",1);
    حرف *activation_s = option_لبھو_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    صحيح batch_normalize = option_لبھو_int_quiet(options, "batch_normalize", 0);

    layer l = make_crnn_layer(params.batch, params.w, params.h, params.c, hidden_filters, output_filters, params.ویلا_قدماں, activation, batch_normalize);

    l.shortcut = option_لبھو_int_quiet(options, "shortcut", 0);

    أرجع l;
}

layer parse_rnn(list *options, ناپ_params params)
{
    صحيح output = option_لبھو_int(options, "output",1);
    حرف *activation_s = option_لبھو_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    صحيح batch_normalize = option_لبھو_int_quiet(options, "batch_normalize", 0);

    layer l = make_rnn_layer(params.batch, params.inputs, output, params.ویلا_قدماں, activation, batch_normalize, params.net->adam);

    l.shortcut = option_لبھو_int_quiet(options, "shortcut", 0);

    أرجع l;
}

layer parse_gru(list *options, ناپ_params params)
{
    صحيح output = option_لبھو_int(options, "output",1);
    صحيح batch_normalize = option_لبھو_int_quiet(options, "batch_normalize", 0);

    layer l = make_gru_layer(params.batch, params.inputs, output, params.ویلا_قدماں, batch_normalize, params.net->adam);
    l.tanh = option_لبھو_int_quiet(options, "tanh", 0);

    أرجع l;
}

layer parse_lstm(list *options, ناپ_params params)
{
    صحيح output = option_لبھو_int(options, "output", 1);
    صحيح batch_normalize = option_لبھو_int_quiet(options, "batch_normalize", 0);

    layer l = make_lstm_layer(params.batch, params.inputs, output, params.ویلا_قدماں, batch_normalize, params.net->adam);

    أرجع l;
}

layer parse_connected(list *options, ناپ_params params)
{
    صحيح output = option_لبھو_int(options, "output",1);
    حرف *activation_s = option_لبھو_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    صحيح batch_normalize = option_لبھو_int_quiet(options, "batch_normalize", 0);

    layer l = make_connected_layer(params.batch, params.inputs, output, activation, batch_normalize, params.net->adam);
    أرجع l;
}

layer parse_softmax(list *options, ناپ_params params)
{
    صحيح groups = option_لبھو_int_quiet(options, "groups",1);
    layer l = make_softmax_layer(params.batch, params.inputs, groups);
    l.temperature = option_لبھو_float_quiet(options, "temperature", 1);
    حرف *درخت_file = option_لبھو_str(options, "tree", 0);
    إذا (درخت_file) l.softmax_درخت = read_درخت(درخت_file);
    l.w = params.w;
    l.h = params.h;
    l.c = params.c;
    l.spatial = option_لبھو_float_quiet(options, "spatial", 0);
    l.noloss =  option_لبھو_int_quiet(options, "noloss", 0);
    أرجع l;
}

صحيح *parse_yolo_نقاب(حرف *a, صحيح *عدد)
{
    صحيح *نقاب = 0;
    إذا(a){
        صحيح len = لڑی_لمبائی(a);
        صحيح n = 1;
        صحيح i;
        لكل(i = 0; i < len; ++i){
            إذا (a[i] == ',') ++n;
        }
        نقاب = صفر_حافظہ_دیو(n, حجم(صحيح));
        لكل(i = 0; i < n; ++i){
            صحيح val = atoi(a);
            نقاب[i] = val;
            a = strchr(a, ',')+1;
        }
        *عدد = n;
    }
    أرجع نقاب;
}

layer parse_yolo(list *options, ناپ_params params)
{
    صحيح classes = option_لبھو_int(options, "classes", 20);
    صحيح کل = option_لبھو_int(options, "num", 1);
    صحيح عدد = کل;

    حرف *a = option_لبھو_str(options, "mask", 0);
    صحيح *نقاب = parse_yolo_نقاب(a, &عدد);
    layer l = make_yolo_layer(params.batch, params.w, params.h, عدد, کل, نقاب, classes);
    أكد(l.outputs == params.inputs);

    l.max_boxes = option_لبھو_int_quiet(options, "max",90);
    l.jitter = option_لبھو_float(options, "jitter", .2);

    l.ignore_thresh = option_لبھو_float(options, "ignore_thresh", .5);
    l.truth_thresh = option_لبھو_float(options, "truth_thresh", 1);
    l.random = option_لبھو_int_quiet(options, "random", 0);

    حرف *map_file = option_لبھو_str(options, "map", 0);
    إذا (map_file) l.map = read_map(map_file);

    a = option_لبھو_str(options, "anchors", 0);
    إذا(a){
        صحيح len = لڑی_لمبائی(a);
        صحيح n = 1;
        صحيح i;
        لكل(i = 0; i < len; ++i){
            إذا (a[i] == ',') ++n;
        }
        لكل(i = 0; i < n; ++i){
            عائم bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    أرجع l;
}

layer parse_iseg(list *options, ناپ_params params)
{
    صحيح classes = option_لبھو_int(options, "classes", 20);
    صحيح ids = option_لبھو_int(options, "ids", 32);
    layer l = make_iseg_layer(params.batch, params.w, params.h, classes, ids);
    أكد(l.outputs == params.inputs);
    أرجع l;
}

layer parse_region(list *options, ناپ_params params)
{
    صحيح coords = option_لبھو_int(options, "coords", 4);
    صحيح classes = option_لبھو_int(options, "classes", 20);
    صحيح عدد = option_لبھو_int(options, "num", 1);

    layer l = make_region_layer(params.batch, params.w, params.h, عدد, classes, coords);
    أكد(l.outputs == params.inputs);

    l.لوگارتھم = option_لبھو_int_quiet(options, "log", 0);
    l.جذر = option_لبھو_int_quiet(options, "sqrt", 0);

    l.softmax = option_لبھو_int(options, "softmax", 0);
    l.background = option_لبھو_int_quiet(options, "background", 0);
    l.max_boxes = option_لبھو_int_quiet(options, "max",30);
    l.jitter = option_لبھو_float(options, "jitter", .2);
    l.rescore = option_لبھو_int_quiet(options, "rescore",0);

    l.thresh = option_لبھو_float(options, "thresh", .5);
    l.classfix = option_لبھو_int_quiet(options, "classfix", 0);
    l.absolute = option_لبھو_int_quiet(options, "absolute", 0);
    l.random = option_لبھو_int_quiet(options, "random", 0);

    l.coord_scale = option_لبھو_float(options, "coord_scale", 1);
    l.object_scale = option_لبھو_float(options, "object_scale", 1);
    l.noobject_scale = option_لبھو_float(options, "noobject_scale", 1);
    l.نقاب_scale = option_لبھو_float(options, "mask_scale", 1);
    l.class_scale = option_لبھو_float(options, "class_scale", 1);
    l.bias_میل = option_لبھو_int_quiet(options, "bias_match",0);

    حرف *درخت_file = option_لبھو_str(options, "tree", 0);
    إذا (درخت_file) l.softmax_درخت = read_درخت(درخت_file);
    حرف *map_file = option_لبھو_str(options, "map", 0);
    إذا (map_file) l.map = read_map(map_file);

    حرف *a = option_لبھو_str(options, "anchors", 0);
    إذا(a){
        صحيح len = لڑی_لمبائی(a);
        صحيح n = 1;
        صحيح i;
        لكل(i = 0; i < len; ++i){
            إذا (a[i] == ',') ++n;
        }
        لكل(i = 0; i < n; ++i){
            عائم bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    أرجع l;
}

detection_layer parse_detection(list *options, ناپ_params params)
{
    صحيح coords = option_لبھو_int(options, "coords", 1);
    صحيح classes = option_لبھو_int(options, "classes", 1);
    صحيح rescore = option_لبھو_int(options, "rescore", 0);
    صحيح عدد = option_لبھو_int(options, "num", 1);
    صحيح side = option_لبھو_int(options, "side", 7);
    detection_layer layer = make_detection_layer(params.batch, params.inputs, عدد, side, classes, coords, rescore);

    layer.softmax = option_لبھو_int(options, "softmax", 0);
    layer.جذر = option_لبھو_int(options, "sqrt", 0);

    layer.max_boxes = option_لبھو_int_quiet(options, "max",90);
    layer.coord_scale = option_لبھو_float(options, "coord_scale", 1);
    layer.forced = option_لبھو_int(options, "forced", 0);
    layer.object_scale = option_لبھو_float(options, "object_scale", 1);
    layer.noobject_scale = option_لبھو_float(options, "noobject_scale", 1);
    layer.class_scale = option_لبھو_float(options, "class_scale", 1);
    layer.jitter = option_لبھو_float(options, "jitter", .2);
    layer.random = option_لبھو_int_quiet(options, "random", 0);
    layer.reorg = option_لبھو_int_quiet(options, "reorg", 0);
    أرجع layer;
}

لاگت_layer parse_لاگت(list *options, ناپ_params params)
{
    حرف *type_s = option_لبھو_str(options, "type", "sse");
    لاگت_TYPE type = get_لاگت_type(type_s);
    عائم scale = option_لبھو_float_quiet(options, "scale",1);
    لاگت_layer layer = make_لاگت_layer(params.batch, params.inputs, type, scale);
    layer.تناسب =  option_لبھو_float_quiet(options, "ratio",0);
    layer.noobject_scale =  option_لبھو_float_quiet(options, "noobj", 1);
    layer.thresh =  option_لبھو_float_quiet(options, "thresh",0);
    أرجع layer;
}

crop_layer parse_crop(list *options, ناپ_params params)
{
    صحيح crop_اچائی = option_لبھو_int(options, "crop_height",1);
    صحيح crop_چوڑائی = option_لبھو_int(options, "crop_width",1);
    صحيح flip = option_لبھو_int(options, "flip",0);
    عائم angle = option_لبھو_float(options, "angle",0);
    عائم saturation = option_لبھو_float(options, "saturation",1);
    عائم exposure = option_لبھو_float(options, "exposure",1);

    صحيح batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    إذا(!(h && w && c)) error("Layer before crop layer must output image.");

    صحيح noadjust = option_لبھو_int_quiet(options, "noadjust",0);

    crop_layer l = make_crop_layer(batch,h,w,c,crop_اچائی,crop_چوڑائی,flip, angle, saturation, exposure);
    l.کھسکاؤ = option_لبھو_float(options, "shift", 0);
    l.noadjust = noadjust;
    أرجع l;
}

layer parse_reorg(list *options, ناپ_params params)
{
    صحيح stride = option_لبھو_int(options, "stride",1);
    صحيح الٹ = option_لبھو_int_quiet(options, "reverse",0);
    صحيح flatten = option_لبھو_int_quiet(options, "flatten",0);
    صحيح extra = option_لبھو_int_quiet(options, "extra",0);

    صحيح batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    إذا(!(h && w && c)) error("Layer before reorg layer must output image.");

    layer layer = make_reorg_layer(batch,w,h,c,stride,الٹ, flatten, extra);
    أرجع layer;
}

maxpool_layer parse_maxpool(list *options, ناپ_params params)
{
    صحيح stride = option_لبھو_int(options, "stride",1);
    صحيح ناپ = option_لبھو_int(options, "size",stride);
    صحيح padding = option_لبھو_int_quiet(options, "padding", ناپ-1);

    صحيح batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    إذا(!(h && w && c)) error("Layer before maxpool layer must output image.");

    maxpool_layer layer = make_maxpool_layer(batch,h,w,c,ناپ,stride,padding);
    أرجع layer;
}

avgpool_layer parse_avgpool(list *options, ناپ_params params)
{
    صحيح batch,w,h,c;
    w = params.w;
    h = params.h;
    c = params.c;
    batch=params.batch;
    إذا(!(h && w && c)) error("Layer before avgpool layer must output image.");

    avgpool_layer layer = make_avgpool_layer(batch,w,h,c);
    أرجع layer;
}

dropout_layer parse_dropout(list *options, ناپ_params params)
{
    عائم probability = option_لبھو_float(options, "probability", .5);
    dropout_layer layer = make_dropout_layer(params.batch, params.inputs, probability);
    layer.باہرلا_w = params.w;
    layer.باہرلا_h = params.h;
    layer.باہرلا_c = params.c;
    أرجع layer;
}

layer parse_normalization(list *options, ناپ_params params)
{
    عائم alpha = option_لبھو_float(options, "alpha", .0001);
    عائم beta =  option_لبھو_float(options, "beta" , .75);
    عائم kappa = option_لبھو_float(options, "kappa", 1);
    صحيح ناپ = option_لبھو_int(options, "size", 5);
    layer l = make_normalization_layer(params.batch, params.w, params.h, params.c, ناپ, alpha, beta, kappa);
    أرجع l;
}

layer parse_batchnorm(list *options, ناپ_params params)
{
    layer l = make_batchnorm_layer(params.batch, params.w, params.h, params.c);
    أرجع l;
}

layer parse_shortcut(list *options, ناپ_params params, جال *net)
{
    حرف *l = option_لبھو(options, "from");
    صحيح اشاریہ = atoi(l);
    إذا(اشاریہ < 0) اشاریہ = params.اشاریہ + اشاریہ;

    صحيح batch = params.batch;
    layer from = net->layers[اشاریہ];

    layer s = make_shortcut_layer(batch, اشاریہ, params.w, params.h, params.c, from.باہرلا_w, from.باہرلا_h, from.باہرلا_c);

    حرف *activation_s = option_لبھو_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);
    s.activation = activation;
    s.alpha = option_لبھو_float_quiet(options, "alpha", 1);
    s.beta = option_لبھو_float_quiet(options, "beta", 1);
    أرجع s;
}


layer parse_l2norm(list *options, ناپ_params params)
{
    layer l = make_l2norm_layer(params.batch, params.inputs);
    l.h = l.باہرلا_h = params.h;
    l.w = l.باہرلا_w = params.w;
    l.c = l.باہرلا_c = params.c;
    أرجع l;
}


layer parse_logistic(list *options, ناپ_params params)
{
    layer l = make_logistic_layer(params.batch, params.inputs);
    l.h = l.باہرلا_h = params.h;
    l.w = l.باہرلا_w = params.w;
    l.c = l.باہرلا_c = params.c;
    أرجع l;
}

layer parse_activation(list *options, ناپ_params params)
{
    حرف *activation_s = option_لبھو_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);

    layer l = make_activation_layer(params.batch, params.inputs, activation);

    l.h = l.باہرلا_h = params.h;
    l.w = l.باہرلا_w = params.w;
    l.c = l.باہرلا_c = params.c;

    أرجع l;
}

layer parse_upsample(list *options, ناپ_params params, جال *net)
{

    صحيح stride = option_لبھو_int(options, "stride",2);
    layer l = make_upsample_layer(params.batch, params.w, params.h, params.c, stride);
    l.scale = option_لبھو_float_quiet(options, "scale", 1);
    أرجع l;
}

route_layer parse_route(list *options, ناپ_params params, جال *net)
{
    حرف *l = option_لبھو(options, "layers");
    صحيح len = لڑی_لمبائی(l);
    إذا(!l) error("Route Layer must specify input layers");
    صحيح n = 1;
    صحيح i;
    لكل(i = 0; i < len; ++i){
        إذا (l[i] == ',') ++n;
    }

    صحيح *layers = صفر_حافظہ_دیو(n, حجم(صحيح));
    صحيح *sizes = صفر_حافظہ_دیو(n, حجم(صحيح));
    لكل(i = 0; i < n; ++i){
        صحيح اشاریہ = atoi(l);
        l = strchr(l, ',')+1;
        إذا(اشاریہ < 0) اشاریہ = params.اشاریہ + اشاریہ;
        layers[i] = اشاریہ;
        sizes[i] = net->layers[اشاریہ].outputs;
    }
    صحيح batch = params.batch;

    route_layer layer = make_route_layer(batch, n, layers, sizes);

    convolutional_layer پہلا = net->layers[layers[0]];
    layer.باہرلا_w = پہلا.باہرلا_w;
    layer.باہرلا_h = پہلا.باہرلا_h;
    layer.باہرلا_c = پہلا.باہرلا_c;
    لكل(i = 1; i < n; ++i){
        صحيح اشاریہ = layers[i];
        convolutional_layer اگلا = net->layers[اشاریہ];
        إذا(اگلا.باہرلا_w == پہلا.باہرلا_w && اگلا.باہرلا_h == پہلا.باہرلا_h){
            layer.باہرلا_c += اگلا.باہرلا_c;
        }وإلا{
            layer.باہرلا_h = layer.باہرلا_w = layer.باہرلا_c = 0;
        }
    }

    أرجع layer;
}

learning_شرح_policy get_policy(حرف *s)
{
    إذا (strcmp(s, "random")==0) أرجع RANDOM;
    إذا (strcmp(s, "poly")==0) أرجع POLY;
    إذا (strcmp(s, "constant")==0) أرجع CONSTANT;
    إذا (strcmp(s, "step")==0) أرجع قدم_2;
    إذا (strcmp(s, "exp")==0) أرجع EXP;
    إذا (strcmp(s, "sigmoid")==0) أرجع SIG;
    إذا (strcmp(s, "steps")==0) أرجع قدماں_2;
    fprintf(stderr, "Couldn't find policy %s, going with constant\n", s);
    أرجع CONSTANT;
}

فراغ parse_net_options(list *options, جال *net)
{
    net->batch = option_لبھو_int(options, "batch",1);
    net->learning_شرح = option_لبھو_float(options, "learning_rate", .001);
    net->momentum = option_لبھو_float(options, "momentum", .9);
    net->decay = option_لبھو_float(options, "decay", .0001);
    صحيح subdivs = option_لبھو_int(options, "subdivisions",1);
    net->ویلا_قدماں = option_لبھو_int_quiet(options, "time_steps",1);
    net->notruth = option_لبھو_int_quiet(options, "notruth",0);
    net->batch /= subdivs;
    net->batch *= net->ویلا_قدماں;
    net->subdivisions = subdivs;
    net->random = option_لبھو_int_quiet(options, "random", 0);

    net->adam = option_لبھو_int_quiet(options, "adam", 0);
    إذا(net->adam){
        net->B1 = option_لبھو_float(options, "B1", .9);
        net->B2 = option_لبھو_float(options, "B2", .999);
        net->eps = option_لبھو_float(options, "eps", .0000001);
    }

    net->h = option_لبھو_int_quiet(options, "height",0);
    net->w = option_لبھو_int_quiet(options, "width",0);
    net->c = option_لبھو_int_quiet(options, "channels",0);
    net->inputs = option_لبھو_int_quiet(options, "inputs", net->h * net->w * net->c);
    net->max_crop = option_لبھو_int_quiet(options, "max_crop",net->w*2);
    net->min_crop = option_لبھو_int_quiet(options, "min_crop",net->w);
    net->max_تناسب = option_لبھو_float_quiet(options, "max_ratio", (عائم) net->max_crop / net->w);
    net->min_تناسب = option_لبھو_float_quiet(options, "min_ratio", (عائم) net->min_crop / net->w);
    net->center = option_لبھو_int_quiet(options, "center",0);
    net->clip = option_لبھو_float_quiet(options, "clip", 0);

    net->angle = option_لبھو_float_quiet(options, "angle", 0);
    net->aspect = option_لبھو_float_quiet(options, "aspect", 1);
    net->saturation = option_لبھو_float_quiet(options, "saturation", 1);
    net->exposure = option_لبھو_float_quiet(options, "exposure", 1);
    net->hue = option_لبھو_float_quiet(options, "hue", 0);

    إذا(!net->inputs && !(net->h && net->w && net->c)) error("No input parameters supplied");

    حرف *policy_s = option_لبھو_str(options, "policy", "constant");
    net->policy = get_policy(policy_s);
    net->burn_وچ = option_لبھو_int_quiet(options, "burn_in", 0);
    net->power = option_لبھو_float_quiet(options, "power", 4);
    إذا(net->policy == قدم_2){
        net->قدم = option_لبھو_int(options, "step", 1);
        net->scale = option_لبھو_float(options, "scale", 1);
    } وإلا إذا (net->policy == قدماں_2){
        حرف *l = option_لبھو(options, "steps");
        حرف *p = option_لبھو(options, "scales");
        إذا(!l || !p) error("STEPS policy must have steps and scales in cfg file");

        صحيح len = لڑی_لمبائی(l);
        صحيح n = 1;
        صحيح i;
        لكل(i = 0; i < len; ++i){
            إذا (l[i] == ',') ++n;
        }
        صحيح *قدماں = صفر_حافظہ_دیو(n, حجم(صحيح));
        عائم *scales = صفر_حافظہ_دیو(n, حجم(عائم));
        لكل(i = 0; i < n; ++i){
            صحيح قدم    = atoi(l);
            عائم scale = atof(p);
            l = strchr(l, ',')+1;
            p = strchr(p, ',')+1;
            قدماں[i] = قدم;
            scales[i] = scale;
        }
        net->scales = scales;
        net->قدماں = قدماں;
        net->عدد_قدماں = n;
    } وإلا إذا (net->policy == EXP){
        net->gamma = option_لبھو_float(options, "gamma", 1);
    } وإلا إذا (net->policy == SIG){
        net->gamma = option_لبھو_float(options, "gamma", 1);
        net->قدم = option_لبھو_int(options, "step", 1);
    } وإلا إذا (net->policy == POLY || net->policy == RANDOM){
    }
    net->max_batches = option_لبھو_int(options, "max_batches", 0);
}

صحيح is_جال(section *s)
{
    أرجع (strcmp(s->type, "[net]")==0
            || strcmp(s->type, "[network]")==0);
}

جال *parse_جال_cfg(حرف *filename)
{
    list *sections = read_cfg(filename);
    گرہ *n = sections->front;
    إذا(!n) error("Config file has no sections");
    جال *net = make_جال(sections->ناپ - 1);
    net->gpu_اشاریہ = gpu_اشاریہ;
    ناپ_params params;

    section *s = (section *)n->val;
    list *options = s->options;
    إذا(!is_جال(s)) error("First section must be [net] or [network]");
    parse_net_options(options, net);

    params.h = net->h;
    params.w = net->w;
    params.c = net->c;
    params.inputs = net->inputs;
    params.batch = net->batch;
    params.ویلا_قدماں = net->ویلا_قدماں;
    params.net = net;

    size_t workspace_ناپ = 0;
    n = n->اگلا;
    صحيح گنتی = 0;
    free_section(s);
    fprintf(stderr, "layer     filters    size              input                output\n");
    طالما(n){
        params.اشاریہ = گنتی;
        fprintf(stderr, "%5d ", گنتی);
        s = (section *)n->val;
        options = s->options;
        layer l = {0};
        LAYER_TYPE lt = string_منزل_layer_type(s->type);
        إذا(lt == CONVOLUTIONAL){
            l = parse_convolutional(options, params);
        }وإلا إذا(lt == DECONVOLUTIONAL){
            l = parse_deconvolutional(options, params);
        }وإلا إذا(lt == LOCAL){
            l = parse_local(options, params);
        }وإلا إذا(lt == سرگرم){
            l = parse_activation(options, params);
        }وإلا إذا(lt == LOGXENT){
            l = parse_logistic(options, params);
        }وإلا إذا(lt == L2NORM){
            l = parse_l2norm(options, params);
        }وإلا إذا(lt == RNN){
            l = parse_rnn(options, params);
        }وإلا إذا(lt == GRU){
            l = parse_gru(options, params);
        }وإلا إذا (lt == LSTM) {
            l = parse_lstm(options, params);
        }وإلا إذا(lt == CRNN){
            l = parse_crnn(options, params);
        }وإلا إذا(lt == CONNECTED){
            l = parse_connected(options, params);
        }وإلا إذا(lt == CROP){
            l = parse_crop(options, params);
        }وإلا إذا(lt == لاگت){
            l = parse_لاگت(options, params);
        }وإلا إذا(lt == REGION){
            l = parse_region(options, params);
        }وإلا إذا(lt == YOLO){
            l = parse_yolo(options, params);
        }وإلا إذا(lt == ISEG){
            l = parse_iseg(options, params);
        }وإلا إذا(lt == DETECTION){
            l = parse_detection(options, params);
        }وإلا إذا(lt == SOFTMAX){
            l = parse_softmax(options, params);
            net->hierarchy = l.softmax_درخت;
        }وإلا إذا(lt == NORMALIZATION){
            l = parse_normalization(options, params);
        }وإلا إذا(lt == BATCHNORM){
            l = parse_batchnorm(options, params);
        }وإلا إذا(lt == MAXPOOL){
            l = parse_maxpool(options, params);
        }وإلا إذا(lt == REORG){
            l = parse_reorg(options, params);
        }وإلا إذا(lt == AVGPOOL){
            l = parse_avgpool(options, params);
        }وإلا إذا(lt == ROUTE){
            l = parse_route(options, params, net);
        }وإلا إذا(lt == UPSAMPLE){
            l = parse_upsample(options, params, net);
        }وإلا إذا(lt == SHORTCUT){
            l = parse_shortcut(options, params, net);
        }وإلا إذا(lt == DROPOUT){
            l = parse_dropout(options, params);
            l.output = net->layers[گنتی-1].output;
            l.delta = net->layers[گنتی-1].delta;
#جے_تعریف_شدہ GPU
            l.output_gpu = net->layers[گنتی-1].output_gpu;
            l.delta_gpu = net->layers[گنتی-1].delta_gpu;
#جے_انت
        }وإلا{
            fprintf(stderr, "Type not recognized: %s\n", s->type);
        }
        l.clip = net->clip;
        l.truth = option_لبھو_int_quiet(options, "truth", 0);
        l.onlyforward = option_لبھو_int_quiet(options, "onlyforward", 0);
        l.stopbackward = option_لبھو_int_quiet(options, "stopbackward", 0);
        l.dontsave = option_لبھو_int_quiet(options, "dontsave", 0);
        l.dontload = option_لبھو_int_quiet(options, "dontload", 0);
        l.numload = option_لبھو_int_quiet(options, "numload", 0);
        l.dontloadscales = option_لبھو_int_quiet(options, "dontloadscales", 0);
        l.learning_شرح_scale = option_لبھو_float_quiet(options, "learning_rate", 1);
        l.smooth = option_لبھو_float_quiet(options, "smooth", 0);
        option_بن_ورتیا(options);
        net->layers[گنتی] = l;
        إذا (l.workspace_ناپ > workspace_ناپ) workspace_ناپ = l.workspace_ناپ;
        free_section(s);
        n = n->اگلا;
        ++گنتی;
        إذا(n){
            params.h = l.باہرلا_h;
            params.w = l.باہرلا_w;
            params.c = l.باہرلا_c;
            params.inputs = l.outputs;
        }
    }
    free_list(sections);
    layer باہرلا = get_جال_output_layer(net);
    net->outputs = باہرلا.outputs;
    net->truths = باہرلا.outputs;
    إذا(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = باہرلا.output;
    net->داخلہ = صفر_حافظہ_دیو(net->inputs*net->batch, حجم(عائم));
    net->truth = صفر_حافظہ_دیو(net->truths*net->batch, حجم(عائم));
#جے_تعریف_شدہ GPU
    net->output_gpu = باہرلا.output_gpu;
    net->داخلہ_gpu = cuda_make_array(net->داخلہ, net->inputs*net->batch);
    net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
#جے_انت
    إذا(workspace_ناپ){
        //printf("%ld\n", workspace_size);
#جے_تعریف_شدہ GPU
        إذا(gpu_اشاریہ >= 0){
            net->workspace = cuda_make_array(0, (workspace_ناپ-1)/حجم(عائم)+1);
        }وإلا {
            net->workspace = صفر_حافظہ_دیو(1, workspace_ناپ);
        }
#وإلا
        net->workspace = صفر_حافظہ_دیو(1, workspace_ناپ);
#جے_انت
    }
    أرجع net;
}

list *read_cfg(حرف *filename)
{
    FILE *file = fopen(filename, "r");
    إذا(file == 0) file_error(filename);
    حرف *سطر;
    صحيح nu = 0;
    list *options = make_list();
    section *current = 0;
    طالما((سطر=fgetl(file)) != 0){
        ++ nu;
        strip(سطر);
        بدل(سطر[0]){
            حالة '[':
                current = حافظہ_دیو(حجم(section));
                list_داخل_کرو(options, current);
                current->options = make_list();
                current->type = سطر;
                اكسر;
            حالة '\0':
            حالة '#':
            حالة ';':
                حرر(سطر);
                اكسر;
            افتراضي:
                إذا(!read_option(سطر, current->options)){
                    fprintf(stderr, "Config file error line %d, could parse: %s\n", nu, سطر);
                    حرر(سطر);
                }
                اكسر;
        }
    }
    fclose(file);
    أرجع options;
}

فراغ save_convolutional_weights_ثنائی(layer l, FILE *fp)
{
#جے_تعریف_شدہ GPU
    إذا(gpu_اشاریہ >= 0){
        pull_convolutional_layer(l);
    }
#جے_انت
    binarize_weights(l.weights, l.n, l.c*l.ناپ*l.ناپ, l.ثنائی_weights);
    صحيح ناپ = l.c*l.ناپ*l.ناپ;
    صحيح i, j, k;
    fwrite(l.biases, حجم(عائم), l.n, fp);
    إذا (l.batch_normalize){
        fwrite(l.scales, حجم(عائم), l.n, fp);
        fwrite(l.rolling_اوسط, حجم(عائم), l.n, fp);
        fwrite(l.rolling_variance, حجم(عائم), l.n, fp);
    }
    لكل(i = 0; i < l.n; ++i){
        عائم اوسط = l.ثنائی_weights[i*ناپ];
        إذا(اوسط < 0) اوسط = -اوسط;
        fwrite(&اوسط, حجم(عائم), 1, fp);
        لكل(j = 0; j < ناپ/8; ++j){
            صحيح اشاریہ = i*ناپ + j*8;
            بنانشانی حرف c = 0;
            لكل(k = 0; k < 8; ++k){
                إذا (j*8 + k >= ناپ) اكسر;
                إذا (l.ثنائی_weights[اشاریہ + k] > 0) c = (c | 1<<k);
            }
            fwrite(&c, حجم(حرف), 1, fp);
        }
    }
}

فراغ save_convolutional_weights(layer l, FILE *fp)
{
    إذا(l.ثنائی){
        //save_convolutional_weights_binary(l, fp);
        //return;
    }
#جے_تعریف_شدہ GPU
    إذا(gpu_اشاریہ >= 0){
        pull_convolutional_layer(l);
    }
#جے_انت
    صحيح عدد = l.nweights;
    fwrite(l.biases, حجم(عائم), l.n, fp);
    إذا (l.batch_normalize){
        fwrite(l.scales, حجم(عائم), l.n, fp);
        fwrite(l.rolling_اوسط, حجم(عائم), l.n, fp);
        fwrite(l.rolling_variance, حجم(عائم), l.n, fp);
    }
    fwrite(l.weights, حجم(عائم), عدد, fp);
}

فراغ save_batchnorm_weights(layer l, FILE *fp)
{
#جے_تعریف_شدہ GPU
    إذا(gpu_اشاریہ >= 0){
        pull_batchnorm_layer(l);
    }
#جے_انت
    fwrite(l.scales, حجم(عائم), l.c, fp);
    fwrite(l.rolling_اوسط, حجم(عائم), l.c, fp);
    fwrite(l.rolling_variance, حجم(عائم), l.c, fp);
}

فراغ save_connected_weights(layer l, FILE *fp)
{
#جے_تعریف_شدہ GPU
    إذا(gpu_اشاریہ >= 0){
        pull_connected_layer(l);
    }
#جے_انت
    fwrite(l.biases, حجم(عائم), l.outputs, fp);
    fwrite(l.weights, حجم(عائم), l.outputs*l.inputs, fp);
    إذا (l.batch_normalize){
        fwrite(l.scales, حجم(عائم), l.outputs, fp);
        fwrite(l.rolling_اوسط, حجم(عائم), l.outputs, fp);
        fwrite(l.rolling_variance, حجم(عائم), l.outputs, fp);
    }
}

فراغ save_weights_upto(جال *net, حرف *filename, صحيح cutoff)
{
#جے_تعریف_شدہ GPU
    إذا(net->gpu_اشاریہ >= 0){
        cuda_سیٹ_device(net->gpu_اشاریہ);
    }
#جے_انت
    fprintf(stderr, "Saving weights to %s\n", filename);
    FILE *fp = fopen(filename, "wb");
    إذا(!fp) file_error(filename);

    صحيح major = 0;
    صحيح minor = 2;
    صحيح revision = 0;
    fwrite(&major, حجم(صحيح), 1, fp);
    fwrite(&minor, حجم(صحيح), 1, fp);
    fwrite(&revision, حجم(صحيح), 1, fp);
    fwrite(net->تکیا, حجم(size_t), 1, fp);

    صحيح i;
    لكل(i = 0; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        إذا (l.dontsave) استمر;
        إذا(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            save_convolutional_weights(l, fp);
        } إذا(l.type == CONNECTED){
            save_connected_weights(l, fp);
        } إذا(l.type == BATCHNORM){
            save_batchnorm_weights(l, fp);
        } إذا(l.type == RNN){
            save_connected_weights(*(l.داخلہ_layer), fp);
            save_connected_weights(*(l.self_layer), fp);
            save_connected_weights(*(l.output_layer), fp);
        } إذا (l.type == LSTM) {
            save_connected_weights(*(l.wi), fp);
            save_connected_weights(*(l.wf), fp);
            save_connected_weights(*(l.wo), fp);
            save_connected_weights(*(l.wg), fp);
            save_connected_weights(*(l.ui), fp);
            save_connected_weights(*(l.uf), fp);
            save_connected_weights(*(l.uo), fp);
            save_connected_weights(*(l.ug), fp);
        } إذا (l.type == GRU) {
            إذا(1){
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
        }  إذا(l.type == CRNN){
            save_convolutional_weights(*(l.داخلہ_layer), fp);
            save_convolutional_weights(*(l.self_layer), fp);
            save_convolutional_weights(*(l.output_layer), fp);
        } إذا(l.type == LOCAL){
#جے_تعریف_شدہ GPU
            إذا(gpu_اشاریہ >= 0){
                pull_local_layer(l);
            }
#جے_انت
            صحيح locations = l.باہرلا_w*l.باہرلا_h;
            صحيح ناپ = l.ناپ*l.ناپ*l.c*l.n*locations;
            fwrite(l.biases, حجم(عائم), l.outputs, fp);
            fwrite(l.weights, حجم(عائم), ناپ, fp);
        }
    }
    fclose(fp);
}
فراغ save_weights(جال *net, حرف *filename)
{
    save_weights_upto(net, filename, net->n);
}

فراغ transpose_میٹرکس(عائم *a, صحيح صفاں, صحيح خانے_گنتی)
{
    عائم *transpose = صفر_حافظہ_دیو(صفاں*خانے_گنتی, حجم(عائم));
    صحيح x, y;
    لكل(x = 0; x < صفاں; ++x){
        لكل(y = 0; y < خانے_گنتی; ++y){
            transpose[y*صفاں + x] = a[x*خانے_گنتی + y];
        }
    }
    حافظہ_نقل(a, transpose, صفاں*خانے_گنتی*حجم(عائم));
    حرر(transpose);
}

فراغ لدو_connected_weights(layer l, FILE *fp, صحيح transpose)
{
    fread(l.biases, حجم(عائم), l.outputs, fp);
    fread(l.weights, حجم(عائم), l.outputs*l.inputs, fp);
    إذا(transpose){
        transpose_میٹرکس(l.weights, l.inputs, l.outputs);
    }
    //printf("Biases: %f mean %f variance\n", mean_array(l.biases, l.outputs), variance_array(l.biases, l.outputs));
    //printf("Weights: %f mean %f variance\n", mean_array(l.weights, l.outputs*l.inputs), variance_array(l.weights, l.outputs*l.inputs));
    إذا (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, حجم(عائم), l.outputs, fp);
        fread(l.rolling_اوسط, حجم(عائم), l.outputs, fp);
        fread(l.rolling_variance, حجم(عائم), l.outputs, fp);
        //printf("Scales: %f mean %f variance\n", mean_array(l.scales, l.outputs), variance_array(l.scales, l.outputs));
        //printf("rolling_mean: %f mean %f variance\n", mean_array(l.rolling_mean, l.outputs), variance_array(l.rolling_mean, l.outputs));
        //printf("rolling_variance: %f mean %f variance\n", mean_array(l.rolling_variance, l.outputs), variance_array(l.rolling_variance, l.outputs));
    }
#جے_تعریف_شدہ GPU
    إذا(gpu_اشاریہ >= 0){
        ڈالو_connected_layer(l);
    }
#جے_انت
}

فراغ لدو_batchnorm_weights(layer l, FILE *fp)
{
    fread(l.scales, حجم(عائم), l.c, fp);
    fread(l.rolling_اوسط, حجم(عائم), l.c, fp);
    fread(l.rolling_variance, حجم(عائم), l.c, fp);
#جے_تعریف_شدہ GPU
    إذا(gpu_اشاریہ >= 0){
        ڈالو_batchnorm_layer(l);
    }
#جے_انت
}

فراغ لدو_convolutional_weights_ثنائی(layer l, FILE *fp)
{
    fread(l.biases, حجم(عائم), l.n, fp);
    إذا (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, حجم(عائم), l.n, fp);
        fread(l.rolling_اوسط, حجم(عائم), l.n, fp);
        fread(l.rolling_variance, حجم(عائم), l.n, fp);
    }
    صحيح ناپ = l.c*l.ناپ*l.ناپ;
    صحيح i, j, k;
    لكل(i = 0; i < l.n; ++i){
        عائم اوسط = 0;
        fread(&اوسط, حجم(عائم), 1, fp);
        لكل(j = 0; j < ناپ/8; ++j){
            صحيح اشاریہ = i*ناپ + j*8;
            بنانشانی حرف c = 0;
            fread(&c, حجم(حرف), 1, fp);
            لكل(k = 0; k < 8; ++k){
                إذا (j*8 + k >= ناپ) اكسر;
                l.weights[اشاریہ + k] = (c & 1<<k) ? اوسط : -اوسط;
            }
        }
    }
#جے_تعریف_شدہ GPU
    إذا(gpu_اشاریہ >= 0){
        ڈالو_convolutional_layer(l);
    }
#جے_انت
}

فراغ لدو_convolutional_weights(layer l, FILE *fp)
{
    إذا(l.ثنائی){
        //load_convolutional_weights_binary(l, fp);
        //return;
    }
    إذا(l.numload) l.n = l.numload;
    صحيح عدد = l.c/l.groups*l.n*l.ناپ*l.ناپ;
    fread(l.biases, حجم(عائم), l.n, fp);
    إذا (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, حجم(عائم), l.n, fp);
        fread(l.rolling_اوسط, حجم(عائم), l.n, fp);
        fread(l.rolling_variance, حجم(عائم), l.n, fp);
        إذا(0){
            صحيح i;
            لكل(i = 0; i < l.n; ++i){
                لکھو("%g, ", l.rolling_اوسط[i]);
            }
            لکھو("\n");
            لكل(i = 0; i < l.n; ++i){
                لکھو("%g, ", l.rolling_variance[i]);
            }
            لکھو("\n");
        }
        إذا(0){
            fill_cpu(l.n, 0, l.rolling_اوسط, 1);
            fill_cpu(l.n, 0, l.rolling_variance, 1);
        }
        إذا(0){
            صحيح i;
            لكل(i = 0; i < l.n; ++i){
                لکھو("%g, ", l.rolling_اوسط[i]);
            }
            لکھو("\n");
            لكل(i = 0; i < l.n; ++i){
                لکھو("%g, ", l.rolling_variance[i]);
            }
            لکھو("\n");
        }
    }
    fread(l.weights, حجم(عائم), عدد, fp);
    //if(l.c == 3) scal_cpu(num, 1./256, l.weights, 1);
    إذا (l.flipped) {
        transpose_میٹرکس(l.weights, l.c*l.ناپ*l.ناپ, l.n);
    }
    //if (l.binary) binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.weights);
#جے_تعریف_شدہ GPU
    إذا(gpu_اشاریہ >= 0){
        ڈالو_convolutional_layer(l);
    }
#جے_انت
}


فراغ لدو_weights_upto(جال *net, حرف *filename, صحيح شروع, صحيح cutoff)
{
#جے_تعریف_شدہ GPU
    إذا(net->gpu_اشاریہ >= 0){
        cuda_سیٹ_device(net->gpu_اشاریہ);
    }
#جے_انت
    fprintf(stderr, "Loading weights from %s...", filename);
    fflush(stdout);
    FILE *fp = fopen(filename, "rb");
    إذا(!fp) file_error(filename);

    صحيح major;
    صحيح minor;
    صحيح revision;
    fread(&major, حجم(صحيح), 1, fp);
    fread(&minor, حجم(صحيح), 1, fp);
    fread(&revision, حجم(صحيح), 1, fp);
    إذا ((major*10 + minor) >= 2 && major < 1000 && minor < 1000){
        fread(net->تکیا, حجم(size_t), 1, fp);
    } وإلا {
        صحيح iseen = 0;
        fread(&iseen, حجم(صحيح), 1, fp);
        *net->تکیا = iseen;
    }
    صحيح transpose = (major > 1000) || (minor > 1000);

    صحيح i;
    لكل(i = شروع; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        إذا (l.dontload) استمر;
        إذا(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            لدو_convolutional_weights(l, fp);
        }
        إذا(l.type == CONNECTED){
            لدو_connected_weights(l, fp, transpose);
        }
        إذا(l.type == BATCHNORM){
            لدو_batchnorm_weights(l, fp);
        }
        إذا(l.type == CRNN){
            لدو_convolutional_weights(*(l.داخلہ_layer), fp);
            لدو_convolutional_weights(*(l.self_layer), fp);
            لدو_convolutional_weights(*(l.output_layer), fp);
        }
        إذا(l.type == RNN){
            لدو_connected_weights(*(l.داخلہ_layer), fp, transpose);
            لدو_connected_weights(*(l.self_layer), fp, transpose);
            لدو_connected_weights(*(l.output_layer), fp, transpose);
        }
        إذا (l.type == LSTM) {
            لدو_connected_weights(*(l.wi), fp, transpose);
            لدو_connected_weights(*(l.wf), fp, transpose);
            لدو_connected_weights(*(l.wo), fp, transpose);
            لدو_connected_weights(*(l.wg), fp, transpose);
            لدو_connected_weights(*(l.ui), fp, transpose);
            لدو_connected_weights(*(l.uf), fp, transpose);
            لدو_connected_weights(*(l.uo), fp, transpose);
            لدو_connected_weights(*(l.ug), fp, transpose);
        }
        إذا (l.type == GRU) {
            إذا(1){
                لدو_connected_weights(*(l.wz), fp, transpose);
                لدو_connected_weights(*(l.wr), fp, transpose);
                لدو_connected_weights(*(l.wh), fp, transpose);
                لدو_connected_weights(*(l.uz), fp, transpose);
                لدو_connected_weights(*(l.ur), fp, transpose);
                لدو_connected_weights(*(l.uh), fp, transpose);
            }وإلا{
                لدو_connected_weights(*(l.reset_layer), fp, transpose);
                لدو_connected_weights(*(l.update_layer), fp, transpose);
                لدو_connected_weights(*(l.state_layer), fp, transpose);
            }
        }
        إذا(l.type == LOCAL){
            صحيح locations = l.باہرلا_w*l.باہرلا_h;
            صحيح ناپ = l.ناپ*l.ناپ*l.c*l.n*locations;
            fread(l.biases, حجم(عائم), l.outputs, fp);
            fread(l.weights, حجم(عائم), ناپ, fp);
#جے_تعریف_شدہ GPU
            إذا(gpu_اشاریہ >= 0){
                ڈالو_local_layer(l);
            }
#جے_انت
        }
    }
    fprintf(stderr, "Done!\n");
    fclose(fp);
}

فراغ لدو_weights(جال *net, حرف *filename)
{
    لدو_weights_upto(net, filename, 0, net->n);
}


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

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ{
    ಅಕ್ಷರ *type;
    list *options;
}section;

list *read_cfg(ಅಕ್ಷರ *filename);

LAYER_TYPE string_ಗಮ್ಯ_layer_type(ಅಕ್ಷರ * type)
{

    ಆದರೆ (strcmp(type, "[shortcut]")==0) ಹಿಂತಿರುಗಿ SHORTCUT;
    ಆದರೆ (strcmp(type, "[crop]")==0) ಹಿಂತಿರುಗಿ CROP;
    ಆದರೆ (strcmp(type, "[cost]")==0) ಹಿಂತಿರುಗಿ ವೆಚ್ಚ;
    ಆದರೆ (strcmp(type, "[detection]")==0) ಹಿಂತಿರುಗಿ DETECTION;
    ಆದರೆ (strcmp(type, "[region]")==0) ಹಿಂತಿರುಗಿ REGION;
    ಆದರೆ (strcmp(type, "[yolo]")==0) ಹಿಂತಿರುಗಿ YOLO;
    ಆದರೆ (strcmp(type, "[iseg]")==0) ಹಿಂತಿರುಗಿ ISEG;
    ಆದರೆ (strcmp(type, "[local]")==0) ಹಿಂತಿರುಗಿ LOCAL;
    ಆದರೆ (strcmp(type, "[conv]")==0
            || strcmp(type, "[convolutional]")==0) ಹಿಂತಿರುಗಿ CONVOLUTIONAL;
    ಆದರೆ (strcmp(type, "[deconv]")==0
            || strcmp(type, "[deconvolutional]")==0) ಹಿಂತಿರುಗಿ DECONVOLUTIONAL;
    ಆದರೆ (strcmp(type, "[activation]")==0) ಹಿಂತಿರುಗಿ ಸಕ್ರಿಯ;
    ಆದರೆ (strcmp(type, "[logistic]")==0) ಹಿಂತಿರುಗಿ LOGXENT;
    ಆದರೆ (strcmp(type, "[l2norm]")==0) ಹಿಂತಿರುಗಿ L2NORM;
    ಆದರೆ (strcmp(type, "[net]")==0
            || strcmp(type, "[network]")==0) ಹಿಂತಿರುಗಿ ಜಾಲ_2;
    ಆದರೆ (strcmp(type, "[crnn]")==0) ಹಿಂತಿರುಗಿ CRNN;
    ಆದರೆ (strcmp(type, "[gru]")==0) ಹಿಂತಿರುಗಿ GRU;
    ಆದರೆ (strcmp(type, "[lstm]") == 0) ಹಿಂತಿರುಗಿ LSTM;
    ಆದರೆ (strcmp(type, "[rnn]")==0) ಹಿಂತಿರುಗಿ RNN;
    ಆದರೆ (strcmp(type, "[conn]")==0
            || strcmp(type, "[connected]")==0) ಹಿಂತಿರುಗಿ CONNECTED;
    ಆದರೆ (strcmp(type, "[max]")==0
            || strcmp(type, "[maxpool]")==0) ಹಿಂತಿರುಗಿ MAXPOOL;
    ಆದರೆ (strcmp(type, "[reorg]")==0) ಹಿಂತಿರುಗಿ REORG;
    ಆದರೆ (strcmp(type, "[avg]")==0
            || strcmp(type, "[avgpool]")==0) ಹಿಂತಿರುಗಿ AVGPOOL;
    ಆದರೆ (strcmp(type, "[dropout]")==0) ಹಿಂತಿರುಗಿ DROPOUT;
    ಆದರೆ (strcmp(type, "[lrn]")==0
            || strcmp(type, "[normalization]")==0) ಹಿಂತಿರುಗಿ NORMALIZATION;
    ಆದರೆ (strcmp(type, "[batchnorm]")==0) ಹಿಂತಿರುಗಿ BATCHNORM;
    ಆದರೆ (strcmp(type, "[soft]")==0
            || strcmp(type, "[softmax]")==0) ಹಿಂತಿರುಗಿ SOFTMAX;
    ಆದರೆ (strcmp(type, "[route]")==0) ಹಿಂತಿರುಗಿ ROUTE;
    ಆದರೆ (strcmp(type, "[upsample]")==0) ಹಿಂತಿರುಗಿ UPSAMPLE;
    ಹಿಂತಿರುಗಿ BLANK;
}

ಶೂನ್ಯ free_section(section *s)
{
    ಬಿಡುಗಡೆ(s->type);
    ಗಂಟು *n = s->options->front;
    ಆಗ(n){
        kvp *pair = (kvp *)n->val;
        ಬಿಡುಗಡೆ(pair->ಕೀಲಿ);
        ಬಿಡುಗಡೆ(pair);
        ಗಂಟು *ಮುಂದಿನ = n->ಮುಂದಿನ;
        ಬಿಡುಗಡೆ(n);
        n = ಮುಂದಿನ;
    }
    ಬಿಡುಗಡೆ(s->options);
    ಬಿಡುಗಡೆ(s);
}

ಶೂನ್ಯ parse_ದತ್ತಾಂಶ(ಅಕ್ಷರ *ದತ್ತಾಂಶ, ದಶಮಾಂಶ *a, ಪೂರ್ಣಾಂಕ n)
{
    ಪೂರ್ಣಾಂಕ i;
    ಆದರೆ(!ದತ್ತಾಂಶ) ಹಿಂತಿರುಗಿ;
    ಅಕ್ಷರ *curr = ದತ್ತಾಂಶ;
    ಅಕ್ಷರ *ಮುಂದಿನ = ದತ್ತಾಂಶ;
    ಪೂರ್ಣಾಂಕ ಮುಗಿದ = 0;
    ಗಾಗಿ(i = 0; i < n && !ಮುಗಿದ; ++i){
        ಆಗ(*++ಮುಂದಿನ !='\0' && *ಮುಂದಿನ != ',');
        ಆದರೆ(*ಮುಂದಿನ == '\0') ಮುಗಿದ = 1;
        *ಮುಂದಿನ = '\0';
        ಅಕ್ಷರಸರಣಿ_ಓದು(curr, "%g", &a[i]);
        curr = ಮುಂದಿನ+1;
    }
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ಗಾತ್ರ_params{
    ಪೂರ್ಣಾಂಕ batch;
    ಪೂರ್ಣಾಂಕ inputs;
    ಪೂರ್ಣಾಂಕ h;
    ಪೂರ್ಣಾಂಕ w;
    ಪೂರ್ಣಾಂಕ c;
    ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ;
    ಪೂರ್ಣಾಂಕ ಸಮಯ_ಹೆಜ್ಜೆಗಳು;
    ಜಾಲ *net;
} ಗಾತ್ರ_params;

local_layer parse_local(list *options, ಗಾತ್ರ_params params)
{
    ಪೂರ್ಣಾಂಕ n = option_ಕಂಡುಹಿಡಿ_int(options, "filters",1);
    ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2 = option_ಕಂಡುಹಿಡಿ_int(options, "size",1);
    ಪೂರ್ಣಾಂಕ stride = option_ಕಂಡುಹಿಡಿ_int(options, "stride",1);
    ಪೂರ್ಣಾಂಕ pad = option_ಕಂಡುಹಿಡಿ_int(options, "pad",0);
    ಅಕ್ಷರ *activation_s = option_ಕಂಡುಹಿಡಿ_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    ಪೂರ್ಣಾಂಕ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ಆದರೆ(!(h && w && c)) error("Layer before local layer must output image.");

    local_layer layer = make_local_layer(batch,h,w,c,n,ಗಾತ್ರ_2,stride,pad,activation);

    ಹಿಂತಿರುಗಿ layer;
}

layer parse_deconvolutional(list *options, ಗಾತ್ರ_params params)
{
    ಪೂರ್ಣಾಂಕ n = option_ಕಂಡುಹಿಡಿ_int(options, "filters",1);
    ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2 = option_ಕಂಡುಹಿಡಿ_int(options, "size",1);
    ಪೂರ್ಣಾಂಕ stride = option_ಕಂಡುಹಿಡಿ_int(options, "stride",1);

    ಅಕ್ಷರ *activation_s = option_ಕಂಡುಹಿಡಿ_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    ಪೂರ್ಣಾಂಕ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ಆದರೆ(!(h && w && c)) error("Layer before deconvolutional layer must output image.");
    ಪೂರ್ಣಾಂಕ batch_normalize = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "batch_normalize", 0);
    ಪೂರ್ಣಾಂಕ pad = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "pad",0);
    ಪೂರ್ಣಾಂಕ padding = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "padding",0);
    ಆದರೆ(pad) padding = ಗಾತ್ರ_2/2;

    layer l = make_deconvolutional_layer(batch,h,w,c,n,ಗಾತ್ರ_2,stride,padding, activation, batch_normalize, params.net->adam);

    ಹಿಂತಿರುಗಿ l;
}


convolutional_layer parse_convolutional(list *options, ಗಾತ್ರ_params params)
{
    ಪೂರ್ಣಾಂಕ n = option_ಕಂಡುಹಿಡಿ_int(options, "filters",1);
    ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2 = option_ಕಂಡುಹಿಡಿ_int(options, "size",1);
    ಪೂರ್ಣಾಂಕ stride = option_ಕಂಡುಹಿಡಿ_int(options, "stride",1);
    ಪೂರ್ಣಾಂಕ pad = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "pad",0);
    ಪೂರ್ಣಾಂಕ padding = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "padding",0);
    ಪೂರ್ಣಾಂಕ groups = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "groups", 1);
    ಆದರೆ(pad) padding = ಗಾತ್ರ_2/2;

    ಅಕ್ಷರ *activation_s = option_ಕಂಡುಹಿಡಿ_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);

    ಪೂರ್ಣಾಂಕ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ಆದರೆ(!(h && w && c)) error("Layer before convolutional layer must output image.");
    ಪೂರ್ಣಾಂಕ batch_normalize = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "batch_normalize", 0);
    ಪೂರ್ಣಾಂಕ ದ್ವಿಮಾನ = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "binary", 0);
    ಪೂರ್ಣಾಂಕ xnor = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "xnor", 0);

    convolutional_layer layer = make_convolutional_layer(batch,h,w,c,n,groups,ಗಾತ್ರ_2,stride,padding,activation, batch_normalize, ದ್ವಿಮಾನ, xnor, params.net->adam);
    layer.flipped = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "flipped", 0);
    layer.dot = option_ಕಂಡುಹಿಡಿ_float_quiet(options, "dot", 0);

    ಹಿಂತಿರುಗಿ layer;
}

layer parse_crnn(list *options, ಗಾತ್ರ_params params)
{
    ಪೂರ್ಣಾಂಕ output_filters = option_ಕಂಡುಹಿಡಿ_int(options, "output_filters",1);
    ಪೂರ್ಣಾಂಕ hidden_filters = option_ಕಂಡುಹಿಡಿ_int(options, "hidden_filters",1);
    ಅಕ್ಷರ *activation_s = option_ಕಂಡುಹಿಡಿ_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    ಪೂರ್ಣಾಂಕ batch_normalize = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "batch_normalize", 0);

    layer l = make_crnn_layer(params.batch, params.w, params.h, params.c, hidden_filters, output_filters, params.ಸಮಯ_ಹೆಜ್ಜೆಗಳು, activation, batch_normalize);

    l.shortcut = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "shortcut", 0);

    ಹಿಂತಿರುಗಿ l;
}

layer parse_rnn(list *options, ಗಾತ್ರ_params params)
{
    ಪೂರ್ಣಾಂಕ output = option_ಕಂಡುಹಿಡಿ_int(options, "output",1);
    ಅಕ್ಷರ *activation_s = option_ಕಂಡುಹಿಡಿ_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    ಪೂರ್ಣಾಂಕ batch_normalize = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "batch_normalize", 0);

    layer l = make_rnn_layer(params.batch, params.inputs, output, params.ಸಮಯ_ಹೆಜ್ಜೆಗಳು, activation, batch_normalize, params.net->adam);

    l.shortcut = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "shortcut", 0);

    ಹಿಂತಿರುಗಿ l;
}

layer parse_gru(list *options, ಗಾತ್ರ_params params)
{
    ಪೂರ್ಣಾಂಕ output = option_ಕಂಡುಹಿಡಿ_int(options, "output",1);
    ಪೂರ್ಣಾಂಕ batch_normalize = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "batch_normalize", 0);

    layer l = make_gru_layer(params.batch, params.inputs, output, params.ಸಮಯ_ಹೆಜ್ಜೆಗಳು, batch_normalize, params.net->adam);
    l.tanh = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "tanh", 0);

    ಹಿಂತಿರುಗಿ l;
}

layer parse_lstm(list *options, ಗಾತ್ರ_params params)
{
    ಪೂರ್ಣಾಂಕ output = option_ಕಂಡುಹಿಡಿ_int(options, "output", 1);
    ಪೂರ್ಣಾಂಕ batch_normalize = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "batch_normalize", 0);

    layer l = make_lstm_layer(params.batch, params.inputs, output, params.ಸಮಯ_ಹೆಜ್ಜೆಗಳು, batch_normalize, params.net->adam);

    ಹಿಂತಿರುಗಿ l;
}

layer parse_connected(list *options, ಗಾತ್ರ_params params)
{
    ಪೂರ್ಣಾಂಕ output = option_ಕಂಡುಹಿಡಿ_int(options, "output",1);
    ಅಕ್ಷರ *activation_s = option_ಕಂಡುಹಿಡಿ_str(options, "activation", "logistic");
    ACTIVATION activation = get_activation(activation_s);
    ಪೂರ್ಣಾಂಕ batch_normalize = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "batch_normalize", 0);

    layer l = make_connected_layer(params.batch, params.inputs, output, activation, batch_normalize, params.net->adam);
    ಹಿಂತಿರುಗಿ l;
}

layer parse_softmax(list *options, ಗಾತ್ರ_params params)
{
    ಪೂರ್ಣಾಂಕ groups = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "groups",1);
    layer l = make_softmax_layer(params.batch, params.inputs, groups);
    l.temperature = option_ಕಂಡುಹಿಡಿ_float_quiet(options, "temperature", 1);
    ಅಕ್ಷರ *ವೃಕ್ಷ_file = option_ಕಂಡುಹಿಡಿ_str(options, "tree", 0);
    ಆದರೆ (ವೃಕ್ಷ_file) l.softmax_ವೃಕ್ಷ = read_ವೃಕ್ಷ(ವೃಕ್ಷ_file);
    l.w = params.w;
    l.h = params.h;
    l.c = params.c;
    l.spatial = option_ಕಂಡುಹಿಡಿ_float_quiet(options, "spatial", 0);
    l.noloss =  option_ಕಂಡುಹಿಡಿ_int_quiet(options, "noloss", 0);
    ಹಿಂತಿರುಗಿ l;
}

ಪೂರ್ಣಾಂಕ *parse_yolo_ಮುಖವಾಡ(ಅಕ್ಷರ *a, ಪೂರ್ಣಾಂಕ *ಅಂಕೆ)
{
    ಪೂರ್ಣಾಂಕ *ಮುಖವಾಡ = 0;
    ಆದರೆ(a){
        ಪೂರ್ಣಾಂಕ len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(a);
        ಪೂರ್ಣಾಂಕ n = 1;
        ಪೂರ್ಣಾಂಕ i;
        ಗಾಗಿ(i = 0; i < len; ++i){
            ಆದರೆ (a[i] == ',') ++n;
        }
        ಮುಖವಾಡ = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ));
        ಗಾಗಿ(i = 0; i < n; ++i){
            ಪೂರ್ಣಾಂಕ val = atoi(a);
            ಮುಖವಾಡ[i] = val;
            a = strchr(a, ',')+1;
        }
        *ಅಂಕೆ = n;
    }
    ಹಿಂತಿರುಗಿ ಮುಖವಾಡ;
}

layer parse_yolo(list *options, ಗಾತ್ರ_params params)
{
    ಪೂರ್ಣಾಂಕ classes = option_ಕಂಡುಹಿಡಿ_int(options, "classes", 20);
    ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = option_ಕಂಡುಹಿಡಿ_int(options, "num", 1);
    ಪೂರ್ಣಾಂಕ ಅಂಕೆ = ಒಟ್ಟು;

    ಅಕ್ಷರ *a = option_ಕಂಡುಹಿಡಿ_str(options, "mask", 0);
    ಪೂರ್ಣಾಂಕ *ಮುಖವಾಡ = parse_yolo_ಮುಖವಾಡ(a, &ಅಂಕೆ);
    layer l = make_yolo_layer(params.batch, params.w, params.h, ಅಂಕೆ, ಒಟ್ಟು, ಮುಖವಾಡ, classes);
    ಪ್ರತಿಜ್ಞಾ(l.outputs == params.inputs);

    l.max_boxes = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "max",90);
    l.jitter = option_ಕಂಡುಹಿಡಿ_float(options, "jitter", .2);

    l.ignore_thresh = option_ಕಂಡುಹಿಡಿ_float(options, "ignore_thresh", .5);
    l.truth_thresh = option_ಕಂಡುಹಿಡಿ_float(options, "truth_thresh", 1);
    l.random = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "random", 0);

    ಅಕ್ಷರ *map_file = option_ಕಂಡುಹಿಡಿ_str(options, "map", 0);
    ಆದರೆ (map_file) l.map = read_map(map_file);

    a = option_ಕಂಡುಹಿಡಿ_str(options, "anchors", 0);
    ಆದರೆ(a){
        ಪೂರ್ಣಾಂಕ len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(a);
        ಪೂರ್ಣಾಂಕ n = 1;
        ಪೂರ್ಣಾಂಕ i;
        ಗಾಗಿ(i = 0; i < len; ++i){
            ಆದರೆ (a[i] == ',') ++n;
        }
        ಗಾಗಿ(i = 0; i < n; ++i){
            ದಶಮಾಂಶ bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    ಹಿಂತಿರುಗಿ l;
}

layer parse_iseg(list *options, ಗಾತ್ರ_params params)
{
    ಪೂರ್ಣಾಂಕ classes = option_ಕಂಡುಹಿಡಿ_int(options, "classes", 20);
    ಪೂರ್ಣಾಂಕ ids = option_ಕಂಡುಹಿಡಿ_int(options, "ids", 32);
    layer l = make_iseg_layer(params.batch, params.w, params.h, classes, ids);
    ಪ್ರತಿಜ್ಞಾ(l.outputs == params.inputs);
    ಹಿಂತಿರುಗಿ l;
}

layer parse_region(list *options, ಗಾತ್ರ_params params)
{
    ಪೂರ್ಣಾಂಕ coords = option_ಕಂಡುಹಿಡಿ_int(options, "coords", 4);
    ಪೂರ್ಣಾಂಕ classes = option_ಕಂಡುಹಿಡಿ_int(options, "classes", 20);
    ಪೂರ್ಣಾಂಕ ಅಂಕೆ = option_ಕಂಡುಹಿಡಿ_int(options, "num", 1);

    layer l = make_region_layer(params.batch, params.w, params.h, ಅಂಕೆ, classes, coords);
    ಪ್ರತಿಜ್ಞಾ(l.outputs == params.inputs);

    l.ಲಘುಗಣಕ = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "log", 0);
    l.ವರ್ಗಮೂಲ = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "sqrt", 0);

    l.softmax = option_ಕಂಡುಹಿಡಿ_int(options, "softmax", 0);
    l.background = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "background", 0);
    l.max_boxes = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "max",30);
    l.jitter = option_ಕಂಡುಹಿಡಿ_float(options, "jitter", .2);
    l.rescore = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "rescore",0);

    l.thresh = option_ಕಂಡುಹಿಡಿ_float(options, "thresh", .5);
    l.classfix = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "classfix", 0);
    l.absolute = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "absolute", 0);
    l.random = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "random", 0);

    l.coord_scale = option_ಕಂಡುಹಿಡಿ_float(options, "coord_scale", 1);
    l.object_scale = option_ಕಂಡುಹಿಡಿ_float(options, "object_scale", 1);
    l.noobject_scale = option_ಕಂಡುಹಿಡಿ_float(options, "noobject_scale", 1);
    l.ಮುಖವಾಡ_scale = option_ಕಂಡುಹಿಡಿ_float(options, "mask_scale", 1);
    l.class_scale = option_ಕಂಡುಹಿಡಿ_float(options, "class_scale", 1);
    l.bias_ಹೊಂದಿಕೆ = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "bias_match",0);

    ಅಕ್ಷರ *ವೃಕ್ಷ_file = option_ಕಂಡುಹಿಡಿ_str(options, "tree", 0);
    ಆದರೆ (ವೃಕ್ಷ_file) l.softmax_ವೃಕ್ಷ = read_ವೃಕ್ಷ(ವೃಕ್ಷ_file);
    ಅಕ್ಷರ *map_file = option_ಕಂಡುಹಿಡಿ_str(options, "map", 0);
    ಆದರೆ (map_file) l.map = read_map(map_file);

    ಅಕ್ಷರ *a = option_ಕಂಡುಹಿಡಿ_str(options, "anchors", 0);
    ಆದರೆ(a){
        ಪೂರ್ಣಾಂಕ len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(a);
        ಪೂರ್ಣಾಂಕ n = 1;
        ಪೂರ್ಣಾಂಕ i;
        ಗಾಗಿ(i = 0; i < len; ++i){
            ಆದರೆ (a[i] == ',') ++n;
        }
        ಗಾಗಿ(i = 0; i < n; ++i){
            ದಶಮಾಂಶ bias = atof(a);
            l.biases[i] = bias;
            a = strchr(a, ',')+1;
        }
    }
    ಹಿಂತಿರುಗಿ l;
}

detection_layer parse_detection(list *options, ಗಾತ್ರ_params params)
{
    ಪೂರ್ಣಾಂಕ coords = option_ಕಂಡುಹಿಡಿ_int(options, "coords", 1);
    ಪೂರ್ಣಾಂಕ classes = option_ಕಂಡುಹಿಡಿ_int(options, "classes", 1);
    ಪೂರ್ಣಾಂಕ rescore = option_ಕಂಡುಹಿಡಿ_int(options, "rescore", 0);
    ಪೂರ್ಣಾಂಕ ಅಂಕೆ = option_ಕಂಡುಹಿಡಿ_int(options, "num", 1);
    ಪೂರ್ಣಾಂಕ side = option_ಕಂಡುಹಿಡಿ_int(options, "side", 7);
    detection_layer layer = make_detection_layer(params.batch, params.inputs, ಅಂಕೆ, side, classes, coords, rescore);

    layer.softmax = option_ಕಂಡುಹಿಡಿ_int(options, "softmax", 0);
    layer.ವರ್ಗಮೂಲ = option_ಕಂಡುಹಿಡಿ_int(options, "sqrt", 0);

    layer.max_boxes = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "max",90);
    layer.coord_scale = option_ಕಂಡುಹಿಡಿ_float(options, "coord_scale", 1);
    layer.forced = option_ಕಂಡುಹಿಡಿ_int(options, "forced", 0);
    layer.object_scale = option_ಕಂಡುಹಿಡಿ_float(options, "object_scale", 1);
    layer.noobject_scale = option_ಕಂಡುಹಿಡಿ_float(options, "noobject_scale", 1);
    layer.class_scale = option_ಕಂಡುಹಿಡಿ_float(options, "class_scale", 1);
    layer.jitter = option_ಕಂಡುಹಿಡಿ_float(options, "jitter", .2);
    layer.random = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "random", 0);
    layer.reorg = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "reorg", 0);
    ಹಿಂತಿರುಗಿ layer;
}

ವೆಚ್ಚ_layer parse_ವೆಚ್ಚ(list *options, ಗಾತ್ರ_params params)
{
    ಅಕ್ಷರ *type_s = option_ಕಂಡುಹಿಡಿ_str(options, "type", "sse");
    ವೆಚ್ಚ_TYPE type = get_ವೆಚ್ಚ_type(type_s);
    ದಶಮಾಂಶ scale = option_ಕಂಡುಹಿಡಿ_float_quiet(options, "scale",1);
    ವೆಚ್ಚ_layer layer = make_ವೆಚ್ಚ_layer(params.batch, params.inputs, type, scale);
    layer.ಅನುಪಾತ =  option_ಕಂಡುಹಿಡಿ_float_quiet(options, "ratio",0);
    layer.noobject_scale =  option_ಕಂಡುಹಿಡಿ_float_quiet(options, "noobj", 1);
    layer.thresh =  option_ಕಂಡುಹಿಡಿ_float_quiet(options, "thresh",0);
    ಹಿಂತಿರುಗಿ layer;
}

crop_layer parse_crop(list *options, ಗಾತ್ರ_params params)
{
    ಪೂರ್ಣಾಂಕ crop_ಎತ್ತರ = option_ಕಂಡುಹಿಡಿ_int(options, "crop_height",1);
    ಪೂರ್ಣಾಂಕ crop_ಅಗಲ = option_ಕಂಡುಹಿಡಿ_int(options, "crop_width",1);
    ಪೂರ್ಣಾಂಕ flip = option_ಕಂಡುಹಿಡಿ_int(options, "flip",0);
    ದಶಮಾಂಶ angle = option_ಕಂಡುಹಿಡಿ_float(options, "angle",0);
    ದಶಮಾಂಶ saturation = option_ಕಂಡುಹಿಡಿ_float(options, "saturation",1);
    ದಶಮಾಂಶ exposure = option_ಕಂಡುಹಿಡಿ_float(options, "exposure",1);

    ಪೂರ್ಣಾಂಕ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ಆದರೆ(!(h && w && c)) error("Layer before crop layer must output image.");

    ಪೂರ್ಣಾಂಕ noadjust = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "noadjust",0);

    crop_layer l = make_crop_layer(batch,h,w,c,crop_ಎತ್ತರ,crop_ಅಗಲ,flip, angle, saturation, exposure);
    l.ಸರಿಸು = option_ಕಂಡುಹಿಡಿ_float(options, "shift", 0);
    l.noadjust = noadjust;
    ಹಿಂತಿರುಗಿ l;
}

layer parse_reorg(list *options, ಗಾತ್ರ_params params)
{
    ಪೂರ್ಣಾಂಕ stride = option_ಕಂಡುಹಿಡಿ_int(options, "stride",1);
    ಪೂರ್ಣಾಂಕ ಹಿಮ್ಮುಖ = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "reverse",0);
    ಪೂರ್ಣಾಂಕ flatten = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "flatten",0);
    ಪೂರ್ಣಾಂಕ extra = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "extra",0);

    ಪೂರ್ಣಾಂಕ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ಆದರೆ(!(h && w && c)) error("Layer before reorg layer must output image.");

    layer layer = make_reorg_layer(batch,w,h,c,stride,ಹಿಮ್ಮುಖ, flatten, extra);
    ಹಿಂತಿರುಗಿ layer;
}

maxpool_layer parse_maxpool(list *options, ಗಾತ್ರ_params params)
{
    ಪೂರ್ಣಾಂಕ stride = option_ಕಂಡುಹಿಡಿ_int(options, "stride",1);
    ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2 = option_ಕಂಡುಹಿಡಿ_int(options, "size",stride);
    ಪೂರ್ಣಾಂಕ padding = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "padding", ಗಾತ್ರ_2-1);

    ಪೂರ್ಣಾಂಕ batch,h,w,c;
    h = params.h;
    w = params.w;
    c = params.c;
    batch=params.batch;
    ಆದರೆ(!(h && w && c)) error("Layer before maxpool layer must output image.");

    maxpool_layer layer = make_maxpool_layer(batch,h,w,c,ಗಾತ್ರ_2,stride,padding);
    ಹಿಂತಿರುಗಿ layer;
}

avgpool_layer parse_avgpool(list *options, ಗಾತ್ರ_params params)
{
    ಪೂರ್ಣಾಂಕ batch,w,h,c;
    w = params.w;
    h = params.h;
    c = params.c;
    batch=params.batch;
    ಆದರೆ(!(h && w && c)) error("Layer before avgpool layer must output image.");

    avgpool_layer layer = make_avgpool_layer(batch,w,h,c);
    ಹಿಂತಿರುಗಿ layer;
}

dropout_layer parse_dropout(list *options, ಗಾತ್ರ_params params)
{
    ದಶಮಾಂಶ probability = option_ಕಂಡುಹಿಡಿ_float(options, "probability", .5);
    dropout_layer layer = make_dropout_layer(params.batch, params.inputs, probability);
    layer.ಹೊರಹೋಗು_w = params.w;
    layer.ಹೊರಹೋಗು_h = params.h;
    layer.ಹೊರಹೋಗು_c = params.c;
    ಹಿಂತಿರುಗಿ layer;
}

layer parse_normalization(list *options, ಗಾತ್ರ_params params)
{
    ದಶಮಾಂಶ alpha = option_ಕಂಡುಹಿಡಿ_float(options, "alpha", .0001);
    ದಶಮಾಂಶ beta =  option_ಕಂಡುಹಿಡಿ_float(options, "beta" , .75);
    ದಶಮಾಂಶ kappa = option_ಕಂಡುಹಿಡಿ_float(options, "kappa", 1);
    ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2 = option_ಕಂಡುಹಿಡಿ_int(options, "size", 5);
    layer l = make_normalization_layer(params.batch, params.w, params.h, params.c, ಗಾತ್ರ_2, alpha, beta, kappa);
    ಹಿಂತಿರುಗಿ l;
}

layer parse_batchnorm(list *options, ಗಾತ್ರ_params params)
{
    layer l = make_batchnorm_layer(params.batch, params.w, params.h, params.c);
    ಹಿಂತಿರುಗಿ l;
}

layer parse_shortcut(list *options, ಗಾತ್ರ_params params, ಜಾಲ *net)
{
    ಅಕ್ಷರ *l = option_ಕಂಡುಹಿಡಿ(options, "from");
    ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = atoi(l);
    ಆದರೆ(ಸೂಚ್ಯಂಕ < 0) ಸೂಚ್ಯಂಕ = params.ಸೂಚ್ಯಂಕ + ಸೂಚ್ಯಂಕ;

    ಪೂರ್ಣಾಂಕ batch = params.batch;
    layer from = net->layers[ಸೂಚ್ಯಂಕ];

    layer s = make_shortcut_layer(batch, ಸೂಚ್ಯಂಕ, params.w, params.h, params.c, from.ಹೊರಹೋಗು_w, from.ಹೊರಹೋಗು_h, from.ಹೊರಹೋಗು_c);

    ಅಕ್ಷರ *activation_s = option_ಕಂಡುಹಿಡಿ_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);
    s.activation = activation;
    s.alpha = option_ಕಂಡುಹಿಡಿ_float_quiet(options, "alpha", 1);
    s.beta = option_ಕಂಡುಹಿಡಿ_float_quiet(options, "beta", 1);
    ಹಿಂತಿರುಗಿ s;
}


layer parse_l2norm(list *options, ಗಾತ್ರ_params params)
{
    layer l = make_l2norm_layer(params.batch, params.inputs);
    l.h = l.ಹೊರಹೋಗು_h = params.h;
    l.w = l.ಹೊರಹೋಗು_w = params.w;
    l.c = l.ಹೊರಹೋಗು_c = params.c;
    ಹಿಂತಿರುಗಿ l;
}


layer parse_logistic(list *options, ಗಾತ್ರ_params params)
{
    layer l = make_logistic_layer(params.batch, params.inputs);
    l.h = l.ಹೊರಹೋಗು_h = params.h;
    l.w = l.ಹೊರಹೋಗು_w = params.w;
    l.c = l.ಹೊರಹೋಗು_c = params.c;
    ಹಿಂತಿರುಗಿ l;
}

layer parse_activation(list *options, ಗಾತ್ರ_params params)
{
    ಅಕ್ಷರ *activation_s = option_ಕಂಡುಹಿಡಿ_str(options, "activation", "linear");
    ACTIVATION activation = get_activation(activation_s);

    layer l = make_activation_layer(params.batch, params.inputs, activation);

    l.h = l.ಹೊರಹೋಗು_h = params.h;
    l.w = l.ಹೊರಹೋಗು_w = params.w;
    l.c = l.ಹೊರಹೋಗು_c = params.c;

    ಹಿಂತಿರುಗಿ l;
}

layer parse_upsample(list *options, ಗಾತ್ರ_params params, ಜಾಲ *net)
{

    ಪೂರ್ಣಾಂಕ stride = option_ಕಂಡುಹಿಡಿ_int(options, "stride",2);
    layer l = make_upsample_layer(params.batch, params.w, params.h, params.c, stride);
    l.scale = option_ಕಂಡುಹಿಡಿ_float_quiet(options, "scale", 1);
    ಹಿಂತಿರುಗಿ l;
}

route_layer parse_route(list *options, ಗಾತ್ರ_params params, ಜಾಲ *net)
{
    ಅಕ್ಷರ *l = option_ಕಂಡುಹಿಡಿ(options, "layers");
    ಪೂರ್ಣಾಂಕ len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(l);
    ಆದರೆ(!l) error("Route Layer must specify input layers");
    ಪೂರ್ಣಾಂಕ n = 1;
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < len; ++i){
        ಆದರೆ (l[i] == ',') ++n;
    }

    ಪೂರ್ಣಾಂಕ *layers = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ));
    ಪೂರ್ಣಾಂಕ *sizes = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ));
    ಗಾಗಿ(i = 0; i < n; ++i){
        ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = atoi(l);
        l = strchr(l, ',')+1;
        ಆದರೆ(ಸೂಚ್ಯಂಕ < 0) ಸೂಚ್ಯಂಕ = params.ಸೂಚ್ಯಂಕ + ಸೂಚ್ಯಂಕ;
        layers[i] = ಸೂಚ್ಯಂಕ;
        sizes[i] = net->layers[ಸೂಚ್ಯಂಕ].outputs;
    }
    ಪೂರ್ಣಾಂಕ batch = params.batch;

    route_layer layer = make_route_layer(batch, n, layers, sizes);

    convolutional_layer ಮೊದಲ = net->layers[layers[0]];
    layer.ಹೊರಹೋಗು_w = ಮೊದಲ.ಹೊರಹೋಗು_w;
    layer.ಹೊರಹೋಗು_h = ಮೊದಲ.ಹೊರಹೋಗು_h;
    layer.ಹೊರಹೋಗು_c = ಮೊದಲ.ಹೊರಹೋಗು_c;
    ಗಾಗಿ(i = 1; i < n; ++i){
        ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = layers[i];
        convolutional_layer ಮುಂದಿನ = net->layers[ಸೂಚ್ಯಂಕ];
        ಆದರೆ(ಮುಂದಿನ.ಹೊರಹೋಗು_w == ಮೊದಲ.ಹೊರಹೋಗು_w && ಮುಂದಿನ.ಹೊರಹೋಗು_h == ಮೊದಲ.ಹೊರಹೋಗು_h){
            layer.ಹೊರಹೋಗು_c += ಮುಂದಿನ.ಹೊರಹೋಗು_c;
        }ಇಲ್ಲದಿದ್ದರೆ{
            layer.ಹೊರಹೋಗು_h = layer.ಹೊರಹೋಗು_w = layer.ಹೊರಹೋಗು_c = 0;
        }
    }

    ಹಿಂತಿರುಗಿ layer;
}

learning_ದರ_policy get_policy(ಅಕ್ಷರ *s)
{
    ಆದರೆ (strcmp(s, "random")==0) ಹಿಂತಿರುಗಿ RANDOM;
    ಆದರೆ (strcmp(s, "poly")==0) ಹಿಂತಿರುಗಿ POLY;
    ಆದರೆ (strcmp(s, "constant")==0) ಹಿಂತಿರುಗಿ CONSTANT;
    ಆದರೆ (strcmp(s, "step")==0) ಹಿಂತಿರುಗಿ ಹೆಜ್ಜೆ_2;
    ಆದರೆ (strcmp(s, "exp")==0) ಹಿಂತಿರುಗಿ EXP;
    ಆದರೆ (strcmp(s, "sigmoid")==0) ಹಿಂತಿರುಗಿ SIG;
    ಆದರೆ (strcmp(s, "steps")==0) ಹಿಂತಿರುಗಿ ಹೆಜ್ಜೆಗಳು_2;
    fprintf(stderr, "Couldn't find policy %s, going with constant\n", s);
    ಹಿಂತಿರುಗಿ CONSTANT;
}

ಶೂನ್ಯ parse_net_options(list *options, ಜಾಲ *net)
{
    net->batch = option_ಕಂಡುಹಿಡಿ_int(options, "batch",1);
    net->learning_ದರ = option_ಕಂಡುಹಿಡಿ_float(options, "learning_rate", .001);
    net->momentum = option_ಕಂಡುಹಿಡಿ_float(options, "momentum", .9);
    net->decay = option_ಕಂಡುಹಿಡಿ_float(options, "decay", .0001);
    ಪೂರ್ಣಾಂಕ subdivs = option_ಕಂಡುಹಿಡಿ_int(options, "subdivisions",1);
    net->ಸಮಯ_ಹೆಜ್ಜೆಗಳು = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "time_steps",1);
    net->notruth = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "notruth",0);
    net->batch /= subdivs;
    net->batch *= net->ಸಮಯ_ಹೆಜ್ಜೆಗಳು;
    net->subdivisions = subdivs;
    net->random = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "random", 0);

    net->adam = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "adam", 0);
    ಆದರೆ(net->adam){
        net->B1 = option_ಕಂಡುಹಿಡಿ_float(options, "B1", .9);
        net->B2 = option_ಕಂಡುಹಿಡಿ_float(options, "B2", .999);
        net->eps = option_ಕಂಡುಹಿಡಿ_float(options, "eps", .0000001);
    }

    net->h = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "height",0);
    net->w = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "width",0);
    net->c = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "channels",0);
    net->inputs = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "inputs", net->h * net->w * net->c);
    net->max_crop = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "max_crop",net->w*2);
    net->min_crop = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "min_crop",net->w);
    net->max_ಅನುಪಾತ = option_ಕಂಡುಹಿಡಿ_float_quiet(options, "max_ratio", (ದಶಮಾಂಶ) net->max_crop / net->w);
    net->min_ಅನುಪಾತ = option_ಕಂಡುಹಿಡಿ_float_quiet(options, "min_ratio", (ದಶಮಾಂಶ) net->min_crop / net->w);
    net->center = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "center",0);
    net->clip = option_ಕಂಡುಹಿಡಿ_float_quiet(options, "clip", 0);

    net->angle = option_ಕಂಡುಹಿಡಿ_float_quiet(options, "angle", 0);
    net->aspect = option_ಕಂಡುಹಿಡಿ_float_quiet(options, "aspect", 1);
    net->saturation = option_ಕಂಡುಹಿಡಿ_float_quiet(options, "saturation", 1);
    net->exposure = option_ಕಂಡುಹಿಡಿ_float_quiet(options, "exposure", 1);
    net->hue = option_ಕಂಡುಹಿಡಿ_float_quiet(options, "hue", 0);

    ಆದರೆ(!net->inputs && !(net->h && net->w && net->c)) error("No input parameters supplied");

    ಅಕ್ಷರ *policy_s = option_ಕಂಡುಹಿಡಿ_str(options, "policy", "constant");
    net->policy = get_policy(policy_s);
    net->burn_ಒಳ = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "burn_in", 0);
    net->power = option_ಕಂಡುಹಿಡಿ_float_quiet(options, "power", 4);
    ಆದರೆ(net->policy == ಹೆಜ್ಜೆ_2){
        net->ಹೆಜ್ಜೆ = option_ಕಂಡುಹಿಡಿ_int(options, "step", 1);
        net->scale = option_ಕಂಡುಹಿಡಿ_float(options, "scale", 1);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (net->policy == ಹೆಜ್ಜೆಗಳು_2){
        ಅಕ್ಷರ *l = option_ಕಂಡುಹಿಡಿ(options, "steps");
        ಅಕ್ಷರ *p = option_ಕಂಡುಹಿಡಿ(options, "scales");
        ಆದರೆ(!l || !p) error("STEPS policy must have steps and scales in cfg file");

        ಪೂರ್ಣಾಂಕ len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(l);
        ಪೂರ್ಣಾಂಕ n = 1;
        ಪೂರ್ಣಾಂಕ i;
        ಗಾಗಿ(i = 0; i < len; ++i){
            ಆದರೆ (l[i] == ',') ++n;
        }
        ಪೂರ್ಣಾಂಕ *ಹೆಜ್ಜೆಗಳು = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ));
        ದಶಮಾಂಶ *scales = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ದಶಮಾಂಶ));
        ಗಾಗಿ(i = 0; i < n; ++i){
            ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆ    = atoi(l);
            ದಶಮಾಂಶ scale = atof(p);
            l = strchr(l, ',')+1;
            p = strchr(p, ',')+1;
            ಹೆಜ್ಜೆಗಳು[i] = ಹೆಜ್ಜೆ;
            scales[i] = scale;
        }
        net->scales = scales;
        net->ಹೆಜ್ಜೆಗಳು = ಹೆಜ್ಜೆಗಳು;
        net->ಅಂಕೆ_ಹೆಜ್ಜೆಗಳು = n;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (net->policy == EXP){
        net->gamma = option_ಕಂಡುಹಿಡಿ_float(options, "gamma", 1);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (net->policy == SIG){
        net->gamma = option_ಕಂಡುಹಿಡಿ_float(options, "gamma", 1);
        net->ಹೆಜ್ಜೆ = option_ಕಂಡುಹಿಡಿ_int(options, "step", 1);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (net->policy == POLY || net->policy == RANDOM){
    }
    net->max_batches = option_ಕಂಡುಹಿಡಿ_int(options, "max_batches", 0);
}

ಪೂರ್ಣಾಂಕ is_ಜಾಲ(section *s)
{
    ಹಿಂತಿರುಗಿ (strcmp(s->type, "[net]")==0
            || strcmp(s->type, "[network]")==0);
}

ಜಾಲ *parse_ಜಾಲ_cfg(ಅಕ್ಷರ *filename)
{
    list *sections = read_cfg(filename);
    ಗಂಟು *n = sections->front;
    ಆದರೆ(!n) error("Config file has no sections");
    ಜಾಲ *net = make_ಜಾಲ(sections->ಗಾತ್ರ_2 - 1);
    net->gpu_ಸೂಚ್ಯಂಕ = gpu_ಸೂಚ್ಯಂಕ;
    ಗಾತ್ರ_params params;

    section *s = (section *)n->val;
    list *options = s->options;
    ಆದರೆ(!is_ಜಾಲ(s)) error("First section must be [net] or [network]");
    parse_net_options(options, net);

    params.h = net->h;
    params.w = net->w;
    params.c = net->c;
    params.inputs = net->inputs;
    params.batch = net->batch;
    params.ಸಮಯ_ಹೆಜ್ಜೆಗಳು = net->ಸಮಯ_ಹೆಜ್ಜೆಗಳು;
    params.net = net;

    size_t workspace_ಗಾತ್ರ = 0;
    n = n->ಮುಂದಿನ;
    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
    free_section(s);
    fprintf(stderr, "layer     filters    size              input                output\n");
    ಆಗ(n){
        params.ಸೂಚ್ಯಂಕ = ಎಣಿಕೆ_2;
        fprintf(stderr, "%5d ", ಎಣಿಕೆ_2);
        s = (section *)n->val;
        options = s->options;
        layer l = {0};
        LAYER_TYPE lt = string_ಗಮ್ಯ_layer_type(s->type);
        ಆದರೆ(lt == CONVOLUTIONAL){
            l = parse_convolutional(options, params);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lt == DECONVOLUTIONAL){
            l = parse_deconvolutional(options, params);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lt == LOCAL){
            l = parse_local(options, params);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lt == ಸಕ್ರಿಯ){
            l = parse_activation(options, params);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lt == LOGXENT){
            l = parse_logistic(options, params);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lt == L2NORM){
            l = parse_l2norm(options, params);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lt == RNN){
            l = parse_rnn(options, params);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lt == GRU){
            l = parse_gru(options, params);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (lt == LSTM) {
            l = parse_lstm(options, params);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lt == CRNN){
            l = parse_crnn(options, params);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lt == CONNECTED){
            l = parse_connected(options, params);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lt == CROP){
            l = parse_crop(options, params);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lt == ವೆಚ್ಚ){
            l = parse_ವೆಚ್ಚ(options, params);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lt == REGION){
            l = parse_region(options, params);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lt == YOLO){
            l = parse_yolo(options, params);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lt == ISEG){
            l = parse_iseg(options, params);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lt == DETECTION){
            l = parse_detection(options, params);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lt == SOFTMAX){
            l = parse_softmax(options, params);
            net->hierarchy = l.softmax_ವೃಕ್ಷ;
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lt == NORMALIZATION){
            l = parse_normalization(options, params);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lt == BATCHNORM){
            l = parse_batchnorm(options, params);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lt == MAXPOOL){
            l = parse_maxpool(options, params);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lt == REORG){
            l = parse_reorg(options, params);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lt == AVGPOOL){
            l = parse_avgpool(options, params);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lt == ROUTE){
            l = parse_route(options, params, net);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lt == UPSAMPLE){
            l = parse_upsample(options, params, net);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lt == SHORTCUT){
            l = parse_shortcut(options, params, net);
        }ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lt == DROPOUT){
            l = parse_dropout(options, params);
            l.output = net->layers[ಎಣಿಕೆ_2-1].output;
            l.delta = net->layers[ಎಣಿಕೆ_2-1].delta;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
            l.output_gpu = net->layers[ಎಣಿಕೆ_2-1].output_gpu;
            l.delta_gpu = net->layers[ಎಣಿಕೆ_2-1].delta_gpu;
#ಆದರೆ_ಅಂತ್ಯ
        }ಇಲ್ಲದಿದ್ದರೆ{
            fprintf(stderr, "Type not recognized: %s\n", s->type);
        }
        l.clip = net->clip;
        l.truth = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "truth", 0);
        l.onlyforward = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "onlyforward", 0);
        l.stopbackward = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "stopbackward", 0);
        l.dontsave = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "dontsave", 0);
        l.dontload = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "dontload", 0);
        l.numload = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "numload", 0);
        l.dontloadscales = option_ಕಂಡುಹಿಡಿ_int_quiet(options, "dontloadscales", 0);
        l.learning_ದರ_scale = option_ಕಂಡುಹಿಡಿ_float_quiet(options, "learning_rate", 1);
        l.smooth = option_ಕಂಡುಹಿಡಿ_float_quiet(options, "smooth", 0);
        option_ಬಳಸದ(options);
        net->layers[ಎಣಿಕೆ_2] = l;
        ಆದರೆ (l.workspace_ಗಾತ್ರ > workspace_ಗಾತ್ರ) workspace_ಗಾತ್ರ = l.workspace_ಗಾತ್ರ;
        free_section(s);
        n = n->ಮುಂದಿನ;
        ++ಎಣಿಕೆ_2;
        ಆದರೆ(n){
            params.h = l.ಹೊರಹೋಗು_h;
            params.w = l.ಹೊರಹೋಗು_w;
            params.c = l.ಹೊರಹೋಗು_c;
            params.inputs = l.outputs;
        }
    }
    free_list(sections);
    layer ಹೊರಹೋಗು = get_ಜಾಲ_output_layer(net);
    net->outputs = ಹೊರಹೋಗು.outputs;
    net->truths = ಹೊರಹೋಗು.outputs;
    ಆದರೆ(net->layers[net->n-1].truths) net->truths = net->layers[net->n-1].truths;
    net->output = ಹೊರಹೋಗು.output;
    net->ಒಳಹರಿವು = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(net->inputs*net->batch, ಗಾತ್ರ(ದಶಮಾಂಶ));
    net->truth = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(net->truths*net->batch, ಗಾತ್ರ(ದಶಮಾಂಶ));
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    net->output_gpu = ಹೊರಹೋಗು.output_gpu;
    net->ಒಳಹರಿವು_gpu = cuda_make_array(net->ಒಳಹರಿವು, net->inputs*net->batch);
    net->truth_gpu = cuda_make_array(net->truth, net->truths*net->batch);
#ಆದರೆ_ಅಂತ್ಯ
    ಆದರೆ(workspace_ಗಾತ್ರ){
        //printf("%ld\n", workspace_size);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
        ಆದರೆ(gpu_ಸೂಚ್ಯಂಕ >= 0){
            net->workspace = cuda_make_array(0, (workspace_ಗಾತ್ರ-1)/ಗಾತ್ರ(ದಶಮಾಂಶ)+1);
        }ಇಲ್ಲದಿದ್ದರೆ {
            net->workspace = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, workspace_ಗಾತ್ರ);
        }
#ಇಲ್ಲದಿದ್ದರೆ
        net->workspace = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, workspace_ಗಾತ್ರ);
#ಆದರೆ_ಅಂತ್ಯ
    }
    ಹಿಂತಿರುಗಿ net;
}

list *read_cfg(ಅಕ್ಷರ *filename)
{
    FILE *file = fopen(filename, "r");
    ಆದರೆ(file == 0) file_error(filename);
    ಅಕ್ಷರ *ಸಾಲು;
    ಪೂರ್ಣಾಂಕ nu = 0;
    list *options = make_list();
    section *current = 0;
    ಆಗ((ಸಾಲು=fgetl(file)) != 0){
        ++ nu;
        strip(ಸಾಲು);
        ಆಯ್ಕೆ(ಸಾಲು[0]){
            ಸ್ಥಿತಿ '[':
                current = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(section));
                list_ಸೇರಿಸು(options, current);
                current->options = make_list();
                current->type = ಸಾಲು;
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ '\0':
            ಸ್ಥಿತಿ '#':
            ಸ್ಥಿತಿ ';':
                ಬಿಡುಗಡೆ(ಸಾಲು);
                ನಿಲ್ಲಿಸು;
            ಪೂರ್ವನಿಯೋಜಿತ:
                ಆದರೆ(!read_option(ಸಾಲು, current->options)){
                    fprintf(stderr, "Config file error line %d, could parse: %s\n", nu, ಸಾಲು);
                    ಬಿಡುಗಡೆ(ಸಾಲು);
                }
                ನಿಲ್ಲಿಸು;
        }
    }
    fclose(file);
    ಹಿಂತಿರುಗಿ options;
}

ಶೂನ್ಯ save_convolutional_weights_ದ್ವಿಮಾನ(layer l, FILE *fp)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    ಆದರೆ(gpu_ಸೂಚ್ಯಂಕ >= 0){
        pull_convolutional_layer(l);
    }
#ಆದರೆ_ಅಂತ್ಯ
    binarize_weights(l.weights, l.n, l.c*l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2, l.ದ್ವಿಮಾನ_weights);
    ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2 = l.c*l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2;
    ಪೂರ್ಣಾಂಕ i, j, k;
    fwrite(l.biases, ಗಾತ್ರ(ದಶಮಾಂಶ), l.n, fp);
    ಆದರೆ (l.batch_normalize){
        fwrite(l.scales, ಗಾತ್ರ(ದಶಮಾಂಶ), l.n, fp);
        fwrite(l.rolling_ಸರಾಸರಿ, ಗಾತ್ರ(ದಶಮಾಂಶ), l.n, fp);
        fwrite(l.rolling_variance, ಗಾತ್ರ(ದಶಮಾಂಶ), l.n, fp);
    }
    ಗಾಗಿ(i = 0; i < l.n; ++i){
        ದಶಮಾಂಶ ಸರಾಸರಿ = l.ದ್ವಿಮಾನ_weights[i*ಗಾತ್ರ_2];
        ಆದರೆ(ಸರಾಸರಿ < 0) ಸರಾಸರಿ = -ಸರಾಸರಿ;
        fwrite(&ಸರಾಸರಿ, ಗಾತ್ರ(ದಶಮಾಂಶ), 1, fp);
        ಗಾಗಿ(j = 0; j < ಗಾತ್ರ_2/8; ++j){
            ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = i*ಗಾತ್ರ_2 + j*8;
            ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c = 0;
            ಗಾಗಿ(k = 0; k < 8; ++k){
                ಆದರೆ (j*8 + k >= ಗಾತ್ರ_2) ನಿಲ್ಲಿಸು;
                ಆದರೆ (l.ದ್ವಿಮಾನ_weights[ಸೂಚ್ಯಂಕ + k] > 0) c = (c | 1<<k);
            }
            fwrite(&c, ಗಾತ್ರ(ಅಕ್ಷರ), 1, fp);
        }
    }
}

ಶೂನ್ಯ save_convolutional_weights(layer l, FILE *fp)
{
    ಆದರೆ(l.ದ್ವಿಮಾನ){
        //save_convolutional_weights_binary(l, fp);
        //return;
    }
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    ಆದರೆ(gpu_ಸೂಚ್ಯಂಕ >= 0){
        pull_convolutional_layer(l);
    }
#ಆದರೆ_ಅಂತ್ಯ
    ಪೂರ್ಣಾಂಕ ಅಂಕೆ = l.nweights;
    fwrite(l.biases, ಗಾತ್ರ(ದಶಮಾಂಶ), l.n, fp);
    ಆದರೆ (l.batch_normalize){
        fwrite(l.scales, ಗಾತ್ರ(ದಶಮಾಂಶ), l.n, fp);
        fwrite(l.rolling_ಸರಾಸರಿ, ಗಾತ್ರ(ದಶಮಾಂಶ), l.n, fp);
        fwrite(l.rolling_variance, ಗಾತ್ರ(ದಶಮಾಂಶ), l.n, fp);
    }
    fwrite(l.weights, ಗಾತ್ರ(ದಶಮಾಂಶ), ಅಂಕೆ, fp);
}

ಶೂನ್ಯ save_batchnorm_weights(layer l, FILE *fp)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    ಆದರೆ(gpu_ಸೂಚ್ಯಂಕ >= 0){
        pull_batchnorm_layer(l);
    }
#ಆದರೆ_ಅಂತ್ಯ
    fwrite(l.scales, ಗಾತ್ರ(ದಶಮಾಂಶ), l.c, fp);
    fwrite(l.rolling_ಸರಾಸರಿ, ಗಾತ್ರ(ದಶಮಾಂಶ), l.c, fp);
    fwrite(l.rolling_variance, ಗಾತ್ರ(ದಶಮಾಂಶ), l.c, fp);
}

ಶೂನ್ಯ save_connected_weights(layer l, FILE *fp)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    ಆದರೆ(gpu_ಸೂಚ್ಯಂಕ >= 0){
        pull_connected_layer(l);
    }
#ಆದರೆ_ಅಂತ್ಯ
    fwrite(l.biases, ಗಾತ್ರ(ದಶಮಾಂಶ), l.outputs, fp);
    fwrite(l.weights, ಗಾತ್ರ(ದಶಮಾಂಶ), l.outputs*l.inputs, fp);
    ಆದರೆ (l.batch_normalize){
        fwrite(l.scales, ಗಾತ್ರ(ದಶಮಾಂಶ), l.outputs, fp);
        fwrite(l.rolling_ಸರಾಸರಿ, ಗಾತ್ರ(ದಶಮಾಂಶ), l.outputs, fp);
        fwrite(l.rolling_variance, ಗಾತ್ರ(ದಶಮಾಂಶ), l.outputs, fp);
    }
}

ಶೂನ್ಯ save_weights_upto(ಜಾಲ *net, ಅಕ್ಷರ *filename, ಪೂರ್ಣಾಂಕ cutoff)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    ಆದರೆ(net->gpu_ಸೂಚ್ಯಂಕ >= 0){
        cuda_ಗಣ_device(net->gpu_ಸೂಚ್ಯಂಕ);
    }
#ಆದರೆ_ಅಂತ್ಯ
    fprintf(stderr, "Saving weights to %s\n", filename);
    FILE *fp = fopen(filename, "wb");
    ಆದರೆ(!fp) file_error(filename);

    ಪೂರ್ಣಾಂಕ major = 0;
    ಪೂರ್ಣಾಂಕ minor = 2;
    ಪೂರ್ಣಾಂಕ revision = 0;
    fwrite(&major, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ), 1, fp);
    fwrite(&minor, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ), 1, fp);
    fwrite(&revision, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ), 1, fp);
    fwrite(net->ನೋಡಿದ, ಗಾತ್ರ(size_t), 1, fp);

    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        ಆದರೆ (l.dontsave) ಮುಂದುವರಿ;
        ಆದರೆ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            save_convolutional_weights(l, fp);
        } ಆದರೆ(l.type == CONNECTED){
            save_connected_weights(l, fp);
        } ಆದರೆ(l.type == BATCHNORM){
            save_batchnorm_weights(l, fp);
        } ಆದರೆ(l.type == RNN){
            save_connected_weights(*(l.ಒಳಹರಿವು_layer), fp);
            save_connected_weights(*(l.self_layer), fp);
            save_connected_weights(*(l.output_layer), fp);
        } ಆದರೆ (l.type == LSTM) {
            save_connected_weights(*(l.wi), fp);
            save_connected_weights(*(l.wf), fp);
            save_connected_weights(*(l.wo), fp);
            save_connected_weights(*(l.wg), fp);
            save_connected_weights(*(l.ui), fp);
            save_connected_weights(*(l.uf), fp);
            save_connected_weights(*(l.uo), fp);
            save_connected_weights(*(l.ug), fp);
        } ಆದರೆ (l.type == GRU) {
            ಆದರೆ(1){
                save_connected_weights(*(l.wz), fp);
                save_connected_weights(*(l.wr), fp);
                save_connected_weights(*(l.wh), fp);
                save_connected_weights(*(l.uz), fp);
                save_connected_weights(*(l.ur), fp);
                save_connected_weights(*(l.uh), fp);
            }ಇಲ್ಲದಿದ್ದರೆ{
                save_connected_weights(*(l.reset_layer), fp);
                save_connected_weights(*(l.update_layer), fp);
                save_connected_weights(*(l.state_layer), fp);
            }
        }  ಆದರೆ(l.type == CRNN){
            save_convolutional_weights(*(l.ಒಳಹರಿವು_layer), fp);
            save_convolutional_weights(*(l.self_layer), fp);
            save_convolutional_weights(*(l.output_layer), fp);
        } ಆದರೆ(l.type == LOCAL){
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
            ಆದರೆ(gpu_ಸೂಚ್ಯಂಕ >= 0){
                pull_local_layer(l);
            }
#ಆದರೆ_ಅಂತ್ಯ
            ಪೂರ್ಣಾಂಕ locations = l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h;
            ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2 = l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.c*l.n*locations;
            fwrite(l.biases, ಗಾತ್ರ(ದಶಮಾಂಶ), l.outputs, fp);
            fwrite(l.weights, ಗಾತ್ರ(ದಶಮಾಂಶ), ಗಾತ್ರ_2, fp);
        }
    }
    fclose(fp);
}
ಶೂನ್ಯ save_weights(ಜಾಲ *net, ಅಕ್ಷರ *filename)
{
    save_weights_upto(net, filename, net->n);
}

ಶೂನ್ಯ transpose_ಮಾತೃಕೆ(ದಶಮಾಂಶ *a, ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ)
{
    ದಶಮಾಂಶ *transpose = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(ಅಡ್ಡಸಾಲುಗಳು*ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಪೂರ್ಣಾಂಕ x, y;
    ಗಾಗಿ(x = 0; x < ಅಡ್ಡಸಾಲುಗಳು; ++x){
        ಗಾಗಿ(y = 0; y < ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; ++y){
            transpose[y*ಅಡ್ಡಸಾಲುಗಳು + x] = a[x*ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ + y];
        }
    }
    ಸ್ಮರಣೆ_ನಕಲು(a, transpose, ಅಡ್ಡಸಾಲುಗಳು*ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ*ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಬಿಡುಗಡೆ(transpose);
}

ಶೂನ್ಯ ತುಂಬು_connected_weights(layer l, FILE *fp, ಪೂರ್ಣಾಂಕ transpose)
{
    fread(l.biases, ಗಾತ್ರ(ದಶಮಾಂಶ), l.outputs, fp);
    fread(l.weights, ಗಾತ್ರ(ದಶಮಾಂಶ), l.outputs*l.inputs, fp);
    ಆದರೆ(transpose){
        transpose_ಮಾತೃಕೆ(l.weights, l.inputs, l.outputs);
    }
    //printf("Biases: %f mean %f variance\n", mean_array(l.biases, l.outputs), variance_array(l.biases, l.outputs));
    //printf("Weights: %f mean %f variance\n", mean_array(l.weights, l.outputs*l.inputs), variance_array(l.weights, l.outputs*l.inputs));
    ಆದರೆ (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, ಗಾತ್ರ(ದಶಮಾಂಶ), l.outputs, fp);
        fread(l.rolling_ಸರಾಸರಿ, ಗಾತ್ರ(ದಶಮಾಂಶ), l.outputs, fp);
        fread(l.rolling_variance, ಗಾತ್ರ(ದಶಮಾಂಶ), l.outputs, fp);
        //printf("Scales: %f mean %f variance\n", mean_array(l.scales, l.outputs), variance_array(l.scales, l.outputs));
        //printf("rolling_mean: %f mean %f variance\n", mean_array(l.rolling_mean, l.outputs), variance_array(l.rolling_mean, l.outputs));
        //printf("rolling_variance: %f mean %f variance\n", mean_array(l.rolling_variance, l.outputs), variance_array(l.rolling_variance, l.outputs));
    }
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    ಆದರೆ(gpu_ಸೂಚ್ಯಂಕ >= 0){
        ತಳ್ಳು_connected_layer(l);
    }
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ ತುಂಬು_batchnorm_weights(layer l, FILE *fp)
{
    fread(l.scales, ಗಾತ್ರ(ದಶಮಾಂಶ), l.c, fp);
    fread(l.rolling_ಸರಾಸರಿ, ಗಾತ್ರ(ದಶಮಾಂಶ), l.c, fp);
    fread(l.rolling_variance, ಗಾತ್ರ(ದಶಮಾಂಶ), l.c, fp);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    ಆದರೆ(gpu_ಸೂಚ್ಯಂಕ >= 0){
        ತಳ್ಳು_batchnorm_layer(l);
    }
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ ತುಂಬು_convolutional_weights_ದ್ವಿಮಾನ(layer l, FILE *fp)
{
    fread(l.biases, ಗಾತ್ರ(ದಶಮಾಂಶ), l.n, fp);
    ಆದರೆ (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, ಗಾತ್ರ(ದಶಮಾಂಶ), l.n, fp);
        fread(l.rolling_ಸರಾಸರಿ, ಗಾತ್ರ(ದಶಮಾಂಶ), l.n, fp);
        fread(l.rolling_variance, ಗಾತ್ರ(ದಶಮಾಂಶ), l.n, fp);
    }
    ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2 = l.c*l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2;
    ಪೂರ್ಣಾಂಕ i, j, k;
    ಗಾಗಿ(i = 0; i < l.n; ++i){
        ದಶಮಾಂಶ ಸರಾಸರಿ = 0;
        fread(&ಸರಾಸರಿ, ಗಾತ್ರ(ದಶಮಾಂಶ), 1, fp);
        ಗಾಗಿ(j = 0; j < ಗಾತ್ರ_2/8; ++j){
            ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = i*ಗಾತ್ರ_2 + j*8;
            ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c = 0;
            fread(&c, ಗಾತ್ರ(ಅಕ್ಷರ), 1, fp);
            ಗಾಗಿ(k = 0; k < 8; ++k){
                ಆದರೆ (j*8 + k >= ಗಾತ್ರ_2) ನಿಲ್ಲಿಸು;
                l.weights[ಸೂಚ್ಯಂಕ + k] = (c & 1<<k) ? ಸರಾಸರಿ : -ಸರಾಸರಿ;
            }
        }
    }
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    ಆದರೆ(gpu_ಸೂಚ್ಯಂಕ >= 0){
        ತಳ್ಳು_convolutional_layer(l);
    }
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ ತುಂಬು_convolutional_weights(layer l, FILE *fp)
{
    ಆದರೆ(l.ದ್ವಿಮಾನ){
        //load_convolutional_weights_binary(l, fp);
        //return;
    }
    ಆದರೆ(l.numload) l.n = l.numload;
    ಪೂರ್ಣಾಂಕ ಅಂಕೆ = l.c/l.groups*l.n*l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2;
    fread(l.biases, ಗಾತ್ರ(ದಶಮಾಂಶ), l.n, fp);
    ಆದರೆ (l.batch_normalize && (!l.dontloadscales)){
        fread(l.scales, ಗಾತ್ರ(ದಶಮಾಂಶ), l.n, fp);
        fread(l.rolling_ಸರಾಸರಿ, ಗಾತ್ರ(ದಶಮಾಂಶ), l.n, fp);
        fread(l.rolling_variance, ಗಾತ್ರ(ದಶಮಾಂಶ), l.n, fp);
        ಆದರೆ(0){
            ಪೂರ್ಣಾಂಕ i;
            ಗಾಗಿ(i = 0; i < l.n; ++i){
                ಬರೆ("%g, ", l.rolling_ಸರಾಸರಿ[i]);
            }
            ಬರೆ("\n");
            ಗಾಗಿ(i = 0; i < l.n; ++i){
                ಬರೆ("%g, ", l.rolling_variance[i]);
            }
            ಬರೆ("\n");
        }
        ಆದರೆ(0){
            fill_cpu(l.n, 0, l.rolling_ಸರಾಸರಿ, 1);
            fill_cpu(l.n, 0, l.rolling_variance, 1);
        }
        ಆದರೆ(0){
            ಪೂರ್ಣಾಂಕ i;
            ಗಾಗಿ(i = 0; i < l.n; ++i){
                ಬರೆ("%g, ", l.rolling_ಸರಾಸರಿ[i]);
            }
            ಬರೆ("\n");
            ಗಾಗಿ(i = 0; i < l.n; ++i){
                ಬರೆ("%g, ", l.rolling_variance[i]);
            }
            ಬರೆ("\n");
        }
    }
    fread(l.weights, ಗಾತ್ರ(ದಶಮಾಂಶ), ಅಂಕೆ, fp);
    //if(l.c == 3) scal_cpu(num, 1./256, l.weights, 1);
    ಆದರೆ (l.flipped) {
        transpose_ಮಾತೃಕೆ(l.weights, l.c*l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2, l.n);
    }
    //if (l.binary) binarize_weights(l.weights, l.n, l.c*l.size*l.size, l.weights);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    ಆದರೆ(gpu_ಸೂಚ್ಯಂಕ >= 0){
        ತಳ್ಳು_convolutional_layer(l);
    }
#ಆದರೆ_ಅಂತ್ಯ
}


ಶೂನ್ಯ ತುಂಬು_weights_upto(ಜಾಲ *net, ಅಕ್ಷರ *filename, ಪೂರ್ಣಾಂಕ ಆರಂಭ, ಪೂರ್ಣಾಂಕ cutoff)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    ಆದರೆ(net->gpu_ಸೂಚ್ಯಂಕ >= 0){
        cuda_ಗಣ_device(net->gpu_ಸೂಚ್ಯಂಕ);
    }
#ಆದರೆ_ಅಂತ್ಯ
    fprintf(stderr, "Loading weights from %s...", filename);
    fflush(stdout);
    FILE *fp = fopen(filename, "rb");
    ಆದರೆ(!fp) file_error(filename);

    ಪೂರ್ಣಾಂಕ major;
    ಪೂರ್ಣಾಂಕ minor;
    ಪೂರ್ಣಾಂಕ revision;
    fread(&major, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ), 1, fp);
    fread(&minor, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ), 1, fp);
    fread(&revision, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ), 1, fp);
    ಆದರೆ ((major*10 + minor) >= 2 && major < 1000 && minor < 1000){
        fread(net->ನೋಡಿದ, ಗಾತ್ರ(size_t), 1, fp);
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಪೂರ್ಣಾಂಕ iseen = 0;
        fread(&iseen, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ), 1, fp);
        *net->ನೋಡಿದ = iseen;
    }
    ಪೂರ್ಣಾಂಕ transpose = (major > 1000) || (minor > 1000);

    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = ಆರಂಭ; i < net->n && i < cutoff; ++i){
        layer l = net->layers[i];
        ಆದರೆ (l.dontload) ಮುಂದುವರಿ;
        ಆದರೆ(l.type == CONVOLUTIONAL || l.type == DECONVOLUTIONAL){
            ತುಂಬು_convolutional_weights(l, fp);
        }
        ಆದರೆ(l.type == CONNECTED){
            ತುಂಬು_connected_weights(l, fp, transpose);
        }
        ಆದರೆ(l.type == BATCHNORM){
            ತುಂಬು_batchnorm_weights(l, fp);
        }
        ಆದರೆ(l.type == CRNN){
            ತುಂಬು_convolutional_weights(*(l.ಒಳಹರಿವು_layer), fp);
            ತುಂಬು_convolutional_weights(*(l.self_layer), fp);
            ತುಂಬು_convolutional_weights(*(l.output_layer), fp);
        }
        ಆದರೆ(l.type == RNN){
            ತುಂಬು_connected_weights(*(l.ಒಳಹರಿವು_layer), fp, transpose);
            ತುಂಬು_connected_weights(*(l.self_layer), fp, transpose);
            ತುಂಬು_connected_weights(*(l.output_layer), fp, transpose);
        }
        ಆದರೆ (l.type == LSTM) {
            ತುಂಬು_connected_weights(*(l.wi), fp, transpose);
            ತುಂಬು_connected_weights(*(l.wf), fp, transpose);
            ತುಂಬು_connected_weights(*(l.wo), fp, transpose);
            ತುಂಬು_connected_weights(*(l.wg), fp, transpose);
            ತುಂಬು_connected_weights(*(l.ui), fp, transpose);
            ತುಂಬು_connected_weights(*(l.uf), fp, transpose);
            ತುಂಬು_connected_weights(*(l.uo), fp, transpose);
            ತುಂಬು_connected_weights(*(l.ug), fp, transpose);
        }
        ಆದರೆ (l.type == GRU) {
            ಆದರೆ(1){
                ತುಂಬು_connected_weights(*(l.wz), fp, transpose);
                ತುಂಬು_connected_weights(*(l.wr), fp, transpose);
                ತುಂಬು_connected_weights(*(l.wh), fp, transpose);
                ತುಂಬು_connected_weights(*(l.uz), fp, transpose);
                ತುಂಬು_connected_weights(*(l.ur), fp, transpose);
                ತುಂಬು_connected_weights(*(l.uh), fp, transpose);
            }ಇಲ್ಲದಿದ್ದರೆ{
                ತುಂಬು_connected_weights(*(l.reset_layer), fp, transpose);
                ತುಂಬು_connected_weights(*(l.update_layer), fp, transpose);
                ತುಂಬು_connected_weights(*(l.state_layer), fp, transpose);
            }
        }
        ಆದರೆ(l.type == LOCAL){
            ಪೂರ್ಣಾಂಕ locations = l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h;
            ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2 = l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.c*l.n*locations;
            fread(l.biases, ಗಾತ್ರ(ದಶಮಾಂಶ), l.outputs, fp);
            fread(l.weights, ಗಾತ್ರ(ದಶಮಾಂಶ), ಗಾತ್ರ_2, fp);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
            ಆದರೆ(gpu_ಸೂಚ್ಯಂಕ >= 0){
                ತಳ್ಳು_local_layer(l);
            }
#ಆದರೆ_ಅಂತ್ಯ
        }
    }
    fprintf(stderr, "Done!\n");
    fclose(fp);
}

ಶೂನ್ಯ ತುಂಬು_weights(ಜಾಲ *net, ಅಕ್ಷರ *filename)
{
    ತುಂಬು_weights_upto(net, filename, 0, net->n);
}


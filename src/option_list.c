#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ "option_list.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"

list *read_data_cfg(ଅକ୍ଷର *filename)
{
    FILE *file = fopen(filename, "r");
    ଯଦି(file == 0) file_error(filename);
    ଅକ୍ଷର *ଧାଡ଼ି;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nu = 0;
    list *options = make_list();
    ଯେତେବେଳେ((ଧାଡ଼ି=fgetl(file)) != 0){
        ++ nu;
        strip(ଧାଡ଼ି);
        ବାଛ(ଧାଡ଼ି[0]){
            ସ୍ଥିତି '\0':
            ସ୍ଥିତି '#':
            ସ୍ଥିତି ';':
                ମୁକ୍ତ(ଧାଡ଼ି);
                ଭାଙ୍ଗ;
            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                ଯଦି(!read_option(ଧାଡ଼ି, options)){
                    fprintf(stderr, "Config file error line %d, could parse: %s\n", nu, ଧାଡ଼ି);
                    ମୁକ୍ତ(ଧାଡ଼ି);
                }
                ଭାଙ୍ଗ;
        }
    }
    fclose(file);
    ଫେରନ୍ତୁ options;
}

metadata get_metadata(ଅକ୍ଷର *file)
{
    metadata m = {0};
    list *options = read_data_cfg(file);

    ଅକ୍ଷର *name_list = option_find_str(options, "names", 0);
    ଯଦି(!name_list) name_list = option_find_str(options, "labels", 0);
    ଯଦି(!name_list) {
        fprintf(stderr, "No names or labels found\n");
    } ଅନ୍ୟଥା {
        m.names = get_labels(name_list);
    }
    m.classes = option_find_int(options, "classes", 2);
    free_list(options);
    ଫେରନ୍ତୁ m;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_option(ଅକ୍ଷର *s, list *options)
{
    size_t i;
    size_t len = strlen(s);
    ଅକ୍ଷର *val = 0;
    ପାଇଁ(i = 0; i < len; ++i){
        ଯଦି(s[i] == '='){
            s[i] = '\0';
            val = s+i+1;
            ଭାଙ୍ଗ;
        }
    }
    ଯଦି(i == len-1) ଫେରନ୍ତୁ 0;
    ଅକ୍ଷର *ଚାବି = s;
    option_ଭର୍ତ୍ତି(options, ଚାବି, val);
    ଫେରନ୍ତୁ 1;
}

ଶୂନ୍ୟ option_ଭର୍ତ୍ତି(list *l, ଅକ୍ଷର *ଚାବି, ଅକ୍ଷର *val)
{
    kvp *p = malloc(ଆକାର(kvp));
    p->ଚାବି = ଚାବି;
    p->val = val;
    p->used = 0;
    list_ଭର୍ତ୍ତି(l, p);
}

ଶୂନ୍ୟ option_unused(list *l)
{
    ଗଣ୍ଠି *n = l->front;
    ଯେତେବେଳେ(n){
        kvp *p = (kvp *)n->val;
        ଯଦି(!p->used){
            fprintf(stderr, "Unused field: '%s = %s'\n", p->ଚାବି, p->val);
        }
        n = n->next;
    }
}

ଅକ୍ଷର *option_find(list *l, ଅକ୍ଷର *ଚାବି)
{
    ଗଣ୍ଠି *n = l->front;
    ଯେତେବେଳେ(n){
        kvp *p = (kvp *)n->val;
        ଯଦି(strcmp(p->ଚାବି, ଚାବି) == 0){
            p->used = 1;
            ଫେରନ୍ତୁ p->val;
        }
        n = n->next;
    }
    ଫେରନ୍ତୁ 0;
}
ଅକ୍ଷର *option_find_str(list *l, ଅକ୍ଷର *ଚାବି, ଅକ୍ଷର *def)
{
    ଅକ୍ଷର *v = option_find(l, ଚାବି);
    ଯଦି(v) ଫେରନ୍ତୁ v;
    ଯଦି(def) fprintf(stderr, "%s: Using default '%s'\n", ଚାବି, def);
    ଫେରନ୍ତୁ def;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_find_int(list *l, ଅକ୍ଷର *ଚାବି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା def)
{
    ଅକ୍ଷର *v = option_find(l, ଚାବି);
    ଯଦି(v) ଫେରନ୍ତୁ atoi(v);
    fprintf(stderr, "%s: Using default '%d'\n", ଚାବି, def);
    ଫେରନ୍ତୁ def;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_find_int_quiet(list *l, ଅକ୍ଷର *ଚାବି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା def)
{
    ଅକ୍ଷର *v = option_find(l, ଚାବି);
    ଯଦି(v) ଫେରନ୍ତୁ atoi(v);
    ଫେରନ୍ତୁ def;
}

ଦଶମିକ option_find_float_quiet(list *l, ଅକ୍ଷର *ଚାବି, ଦଶମିକ def)
{
    ଅକ୍ଷର *v = option_find(l, ଚାବି);
    ଯଦି(v) ଫେରନ୍ତୁ atof(v);
    ଫେରନ୍ତୁ def;
}

ଦଶମିକ option_find_float(list *l, ଅକ୍ଷର *ଚାବି, ଦଶମିକ def)
{
    ଅକ୍ଷର *v = option_find(l, ଚାବି);
    ଯଦି(v) ଫେରନ୍ତୁ atof(v);
    fprintf(stderr, "%s: Using default '%lf'\n", ଚାବି, def);
    ଫେରନ୍ତୁ def;
}

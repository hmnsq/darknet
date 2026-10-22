#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "option_list.h"
#include "utils.h"

list *read_data_cfg(var_na *filename)
{
    FILE *file = fopen(filename, "r");
    ถ้า(file == 0) file_error(filename);
    var_na *line;
    จำนวนเต็ม nu = 0;
    list *options = make_list();
    ขณะ((line=fgetl(file)) != 0){
        ++ nu;
        strip(line);
        switch(line[0]){
            pak_sa '\0':
            pak_sa '#':
            pak_sa ';':
                free(line);
                bha_nga;
            default:
                ถ้า(!read_option(line, options)){
                    fprintf(stderr, "Config file error line %d, could parse: %s\n", nu, line);
                    free(line);
                }
                bha_nga;
        }
    }
    fclose(file);
    คืน options;
}

metadata get_metadata(var_na *file)
{
    metadata m = {0};
    list *options = read_data_cfg(file);

    var_na *name_list = option_find_str(options, "names", 0);
    ถ้า(!name_list) name_list = option_find_str(options, "labels", 0);
    ถ้า(!name_list) {
        fprintf(stderr, "No names or labels found\n");
    } มิฉะนั้น {
        m.names = get_labels(name_list);
    }
    m.classes = option_find_int(options, "classes", 2);
    free_list(options);
    คืน m;
}

จำนวนเต็ม read_option(var_na *s, list *options)
{
    size_t i;
    size_t len = strlen(s);
    var_na *val = 0;
    สำหรับ(i = 0; i < len; ++i){
        ถ้า(s[i] == '='){
            s[i] = '\0';
            val = s+i+1;
            bha_nga;
        }
    }
    ถ้า(i == len-1) คืน 0;
    var_na *key = s;
    option_insert(options, key, val);
    คืน 1;
}

void option_insert(list *l, var_na *key, var_na *val)
{
    kvp *p = malloc(parimaa_na(kvp));
    p->key = key;
    p->val = val;
    p->used = 0;
    list_insert(l, p);
}

void option_unused(list *l)
{
    node *n = l->front;
    ขณะ(n){
        kvp *p = (kvp *)n->val;
        ถ้า(!p->used){
            fprintf(stderr, "Unused field: '%s = %s'\n", p->key, p->val);
        }
        n = n->next;
    }
}

var_na *option_find(list *l, var_na *key)
{
    node *n = l->front;
    ขณะ(n){
        kvp *p = (kvp *)n->val;
        ถ้า(strcmp(p->key, key) == 0){
            p->used = 1;
            คืน p->val;
        }
        n = n->next;
    }
    คืน 0;
}
var_na *option_find_str(list *l, var_na *key, var_na *def)
{
    var_na *v = option_find(l, key);
    ถ้า(v) คืน v;
    ถ้า(def) fprintf(stderr, "%s: Using default '%s'\n", key, def);
    คืน def;
}

จำนวนเต็ม option_find_int(list *l, var_na *key, จำนวนเต็ม def)
{
    var_na *v = option_find(l, key);
    ถ้า(v) คืน atoi(v);
    fprintf(stderr, "%s: Using default '%d'\n", key, def);
    คืน def;
}

จำนวนเต็ม option_find_int_quiet(list *l, var_na *key, จำนวนเต็ม def)
{
    var_na *v = option_find(l, key);
    ถ้า(v) คืน atoi(v);
    คืน def;
}

float option_find_float_quiet(list *l, var_na *key, float def)
{
    var_na *v = option_find(l, key);
    ถ้า(v) คืน atof(v);
    คืน def;
}

float option_find_float(list *l, var_na *key, float def)
{
    var_na *v = option_find(l, key);
    ถ้า(v) คืน atof(v);
    fprintf(stderr, "%s: Using default '%lf'\n", key, def);
    คืน def;
}

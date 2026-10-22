#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "option_list.h"
#include "utils.h"

list *read_data_cfg(char *filename)
{
    FILE *file = fopen(filename, "r");
    nếu(file == 0) file_error(filename);
    char *line;
    số_nguyên nu = 0;
    list *options = make_list();
    trong_khi((line=fgetl(file)) != 0){
        ++ nu;
        strip(line);
        switch(line[0]){
            case '\0':
            case '#':
            case ';':
                free(line);
                break;
            default:
                nếu(!read_option(line, options)){
                    fprintf(stderr, "Config file error line %d, could parse: %s\n", nu, line);
                    free(line);
                }
                break;
        }
    }
    fclose(file);
    trả_về options;
}

metadata get_metadata(char *file)
{
    metadata m = {0};
    list *options = read_data_cfg(file);

    char *name_list = option_find_str(options, "names", 0);
    nếu(!name_list) name_list = option_find_str(options, "labels", 0);
    nếu(!name_list) {
        fprintf(stderr, "No names or labels found\n");
    } không_thì {
        m.names = get_labels(name_list);
    }
    m.classes = option_find_int(options, "classes", 2);
    free_list(options);
    trả_về m;
}

số_nguyên read_option(char *s, list *options)
{
    size_t i;
    size_t len = strlen(s);
    char *val = 0;
    cho(i = 0; i < len; ++i){
        nếu(s[i] == '='){
            s[i] = '\0';
            val = s+i+1;
            break;
        }
    }
    nếu(i == len-1) trả_về 0;
    char *key = s;
    option_insert(options, key, val);
    trả_về 1;
}

void option_insert(list *l, char *key, char *val)
{
    kvp *p = malloc(sizeof(kvp));
    p->key = key;
    p->val = val;
    p->used = 0;
    list_insert(l, p);
}

void option_unused(list *l)
{
    node *n = l->front;
    trong_khi(n){
        kvp *p = (kvp *)n->val;
        nếu(!p->used){
            fprintf(stderr, "Unused field: '%s = %s'\n", p->key, p->val);
        }
        n = n->next;
    }
}

char *option_find(list *l, char *key)
{
    node *n = l->front;
    trong_khi(n){
        kvp *p = (kvp *)n->val;
        nếu(strcmp(p->key, key) == 0){
            p->used = 1;
            trả_về p->val;
        }
        n = n->next;
    }
    trả_về 0;
}
char *option_find_str(list *l, char *key, char *def)
{
    char *v = option_find(l, key);
    nếu(v) trả_về v;
    nếu(def) fprintf(stderr, "%s: Using default '%s'\n", key, def);
    trả_về def;
}

số_nguyên option_find_int(list *l, char *key, số_nguyên def)
{
    char *v = option_find(l, key);
    nếu(v) trả_về atoi(v);
    fprintf(stderr, "%s: Using default '%d'\n", key, def);
    trả_về def;
}

số_nguyên option_find_int_quiet(list *l, char *key, số_nguyên def)
{
    char *v = option_find(l, key);
    nếu(v) trả_về atoi(v);
    trả_về def;
}

float option_find_float_quiet(list *l, char *key, float def)
{
    char *v = option_find(l, key);
    nếu(v) trả_về atof(v);
    trả_về def;
}

float option_find_float(list *l, char *key, float def)
{
    char *v = option_find(l, key);
    nếu(v) trả_về atof(v);
    fprintf(stderr, "%s: Using default '%lf'\n", key, def);
    trả_về def;
}

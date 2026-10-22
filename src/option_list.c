#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "option_list.h"
#include "utils.h"

list *read_data_cfg(char *filename)
{
    FILE *file = fopen(filename, "r");
    якщо(file == 0) file_error(filename);
    char *line;
    ціле nu = 0;
    list *options = make_list();
    поки((line=fgetl(file)) != 0){
        ++ nu;
        strip(line);
        switch(line[0]){
            case '\0':
            case '#':
            case ';':
                free(line);
                break;
            default:
                якщо(!read_option(line, options)){
                    fprintf(stderr, "Config file error line %d, could parse: %s\n", nu, line);
                    free(line);
                }
                break;
        }
    }
    fclose(file);
    повернути options;
}

metadata get_metadata(char *file)
{
    metadata m = {0};
    list *options = read_data_cfg(file);

    char *name_list = option_find_str(options, "names", 0);
    якщо(!name_list) name_list = option_find_str(options, "labels", 0);
    якщо(!name_list) {
        fprintf(stderr, "No names or labels found\n");
    } інакше {
        m.names = get_labels(name_list);
    }
    m.classes = option_find_int(options, "classes", 2);
    free_list(options);
    повернути m;
}

ціле read_option(char *s, list *options)
{
    size_t i;
    size_t len = strlen(s);
    char *val = 0;
    для(i = 0; i < len; ++i){
        якщо(s[i] == '='){
            s[i] = '\0';
            val = s+i+1;
            break;
        }
    }
    якщо(i == len-1) повернути 0;
    char *key = s;
    option_insert(options, key, val);
    повернути 1;
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
    поки(n){
        kvp *p = (kvp *)n->val;
        якщо(!p->used){
            fprintf(stderr, "Unused field: '%s = %s'\n", p->key, p->val);
        }
        n = n->next;
    }
}

char *option_find(list *l, char *key)
{
    node *n = l->front;
    поки(n){
        kvp *p = (kvp *)n->val;
        якщо(strcmp(p->key, key) == 0){
            p->used = 1;
            повернути p->val;
        }
        n = n->next;
    }
    повернути 0;
}
char *option_find_str(list *l, char *key, char *def)
{
    char *v = option_find(l, key);
    якщо(v) повернути v;
    якщо(def) fprintf(stderr, "%s: Using default '%s'\n", key, def);
    повернути def;
}

ціле option_find_int(list *l, char *key, ціле def)
{
    char *v = option_find(l, key);
    якщо(v) повернути atoi(v);
    fprintf(stderr, "%s: Using default '%d'\n", key, def);
    повернути def;
}

ціле option_find_int_quiet(list *l, char *key, ціле def)
{
    char *v = option_find(l, key);
    якщо(v) повернути atoi(v);
    повернути def;
}

float option_find_float_quiet(list *l, char *key, float def)
{
    char *v = option_find(l, key);
    якщо(v) повернути atof(v);
    повернути def;
}

float option_find_float(list *l, char *key, float def)
{
    char *v = option_find(l, key);
    якщо(v) повернути atof(v);
    fprintf(stderr, "%s: Using default '%lf'\n", key, def);
    повернути def;
}

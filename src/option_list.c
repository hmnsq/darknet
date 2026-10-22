#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "option_list.h"
#include "utils.h"

list *read_data_cfg(char *filename)
{
    FILE *file = fopen(filename, "r");
    もし(file == 0) file_error(filename);
    char *line;
    整数 nu = 0;
    list *options = make_list();
    ながら((line=fgetl(file)) != 0){
        ++ nu;
        strip(line);
        switch(line[0]){
            case '\0':
            case '#':
            case ';':
                free(line);
                break;
            default:
                もし(!read_option(line, options)){
                    fprintf(stderr, "Config file error line %d, could parse: %s\n", nu, line);
                    free(line);
                }
                break;
        }
    }
    fclose(file);
    戻る options;
}

metadata get_metadata(char *file)
{
    metadata m = {0};
    list *options = read_data_cfg(file);

    char *name_list = option_find_str(options, "names", 0);
    もし(!name_list) name_list = option_find_str(options, "labels", 0);
    もし(!name_list) {
        fprintf(stderr, "No names or labels found\n");
    } 否则 {
        m.names = get_labels(name_list);
    }
    m.classes = option_find_int(options, "classes", 2);
    free_list(options);
    戻る m;
}

整数 read_option(char *s, list *options)
{
    size_t i;
    size_t len = strlen(s);
    char *val = 0;
    ために(i = 0; i < len; ++i){
        もし(s[i] == '='){
            s[i] = '\0';
            val = s+i+1;
            break;
        }
    }
    もし(i == len-1) 戻る 0;
    char *key = s;
    option_insert(options, key, val);
    戻る 1;
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
    ながら(n){
        kvp *p = (kvp *)n->val;
        もし(!p->used){
            fprintf(stderr, "Unused field: '%s = %s'\n", p->key, p->val);
        }
        n = n->next;
    }
}

char *option_find(list *l, char *key)
{
    node *n = l->front;
    ながら(n){
        kvp *p = (kvp *)n->val;
        もし(strcmp(p->key, key) == 0){
            p->used = 1;
            戻る p->val;
        }
        n = n->next;
    }
    戻る 0;
}
char *option_find_str(list *l, char *key, char *def)
{
    char *v = option_find(l, key);
    もし(v) 戻る v;
    もし(def) fprintf(stderr, "%s: Using default '%s'\n", key, def);
    戻る def;
}

整数 option_find_int(list *l, char *key, 整数 def)
{
    char *v = option_find(l, key);
    もし(v) 戻る atoi(v);
    fprintf(stderr, "%s: Using default '%d'\n", key, def);
    戻る def;
}

整数 option_find_int_quiet(list *l, char *key, 整数 def)
{
    char *v = option_find(l, key);
    もし(v) 戻る atoi(v);
    戻る def;
}

float option_find_float_quiet(list *l, char *key, float def)
{
    char *v = option_find(l, key);
    もし(v) 戻る atof(v);
    戻る def;
}

float option_find_float(list *l, char *key, float def)
{
    char *v = option_find(l, key);
    もし(v) 戻る atof(v);
    fprintf(stderr, "%s: Using default '%lf'\n", key, def);
    戻る def;
}

#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "utils.h"
#include "data.h"

void change_leaves(tree *t, var_na *leaf_list)
{
    list *llist = get_paths(leaf_list);
    var_na **leaves = (var_na **)list_to_array(llist);
    ཧྲིལ n = llist->size;
    ཧྲིལ i,j;
    ཧྲིལ found = 0;
    ཆེད་དུ(i = 0; i < t->n; ++i){
        t->leaf[i] = 0;
        ཆེད་དུ(j = 0; j < n; ++j){
            གལ་ཏེ (0==strcmp(t->name[i], leaves[j])){
                t->leaf[i] = 1;
                ++found;
                bha_nga;
            }
        }
    }
    fprintf(stderr, "Found %d leaves.\n", found);
}

float get_hierarchy_probability(float *x, tree *hier, ཧྲིལ c, ཧྲིལ stride)
{
    float p = 1;
    སྐབས(c >= 0){
        p = p * x[c*stride];
        c = hier->parent[c];
    }
    ཕྱིར p;
}

void hierarchy_predictions(float *predictions, ཧྲིལ n, tree *hier, ཧྲིལ only_leaves, ཧྲིལ stride)
{
    ཧྲིལ j;
    ཆེད་དུ(j = 0; j < n; ++j){
        ཧྲིལ parent = hier->parent[j];
        གལ་ཏེ(parent >= 0){
            predictions[j*stride] *= predictions[parent*stride]; 
        }
    }
    གལ་ཏེ(only_leaves){
        ཆེད་དུ(j = 0; j < n; ++j){
            གལ་ཏེ(!hier->leaf[j]) predictions[j*stride] = 0;
        }
    }
}

ཧྲིལ hierarchy_top_prediction(float *predictions, tree *hier, float thresh, ཧྲིལ stride)
{
    float p = 1;
    ཧྲིལ group = 0;
    ཧྲིལ i;
    སྐབས(1){
        float max = 0;
        ཧྲིལ max_i = 0;

        ཆེད་དུ(i = 0; i < hier->group_size[group]; ++i){
            ཧྲིལ index = i + hier->group_offset[group];
            float val = predictions[(i + hier->group_offset[group])*stride];
            གལ་ཏེ(val > max){
                max_i = index;
                max = val;
            }
        }
        གལ་ཏེ(p*max > thresh){
            p = p*max;
            group = hier->child[max_i];
            གལ་ཏེ(hier->child[max_i] < 0) ཕྱིར max_i;
        } དེ་མིན གལ་ཏེ (group == 0){
            ཕྱིར max_i;
        } དེ་མིན {
            ཕྱིར hier->parent[hier->group_offset[group]];
        }
    }
    ཕྱིར 0;
}

tree *read_tree(var_na *filename)
{
    tree t = {0};
    FILE *fp = fopen(filename, "r");

    var_na *line;
    ཧྲིལ last_parent = -1;
    ཧྲིལ group_size = 0;
    ཧྲིལ groups = 0;
    ཧྲིལ n = 0;
    སྐབས((line=fgetl(fp)) != 0){
        var_na *id = calloc(256, parimaa_na(var_na));
        ཧྲིལ parent = -1;
        sscanf(line, "%s %d", id, &parent);
        t.parent = realloc(t.parent, (n+1)*parimaa_na(ཧྲིལ));
        t.parent[n] = parent;

        t.child = realloc(t.child, (n+1)*parimaa_na(ཧྲིལ));
        t.child[n] = -1;

        t.name = realloc(t.name, (n+1)*parimaa_na(var_na *));
        t.name[n] = id;
        གལ་ཏེ(parent != last_parent){
            ++groups;
            t.group_offset = realloc(t.group_offset, groups * parimaa_na(ཧྲིལ));
            t.group_offset[groups - 1] = n - group_size;
            t.group_size = realloc(t.group_size, groups * parimaa_na(ཧྲིལ));
            t.group_size[groups - 1] = group_size;
            group_size = 0;
            last_parent = parent;
        }
        t.group = realloc(t.group, (n+1)*parimaa_na(ཧྲིལ));
        t.group[n] = groups;
        གལ་ཏེ (parent >= 0) {
            t.child[parent] = groups;
        }
        ++n;
        ++group_size;
    }
    ++groups;
    t.group_offset = realloc(t.group_offset, groups * parimaa_na(ཧྲིལ));
    t.group_offset[groups - 1] = n - group_size;
    t.group_size = realloc(t.group_size, groups * parimaa_na(ཧྲིལ));
    t.group_size[groups - 1] = group_size;
    t.n = n;
    t.groups = groups;
    t.leaf = calloc(n, parimaa_na(ཧྲིལ));
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < n; ++i) t.leaf[i] = 1;
    ཆེད་དུ(i = 0; i < n; ++i) གལ་ཏེ(t.parent[i] >= 0) t.leaf[t.parent[i]] = 0;

    fclose(fp);
    tree *tree_ptr = calloc(1, parimaa_na(tree));
    *tree_ptr = t;
    //error(0);
    ཕྱིར tree_ptr;
}

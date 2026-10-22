#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "utils.h"
#include "data.h"

void change_leaves(tree *t, char *leaf_list)
{
    list *llist = get_paths(leaf_list);
    char **leaves = (char **)list_to_array(llist);
    ціле n = llist->size;
    ціле i,j;
    ціле found = 0;
    для(i = 0; i < t->n; ++i){
        t->leaf[i] = 0;
        для(j = 0; j < n; ++j){
            якщо (0==strcmp(t->name[i], leaves[j])){
                t->leaf[i] = 1;
                ++found;
                break;
            }
        }
    }
    fprintf(stderr, "Found %d leaves.\n", found);
}

float get_hierarchy_probability(float *x, tree *hier, ціле c, ціле stride)
{
    float p = 1;
    поки(c >= 0){
        p = p * x[c*stride];
        c = hier->parent[c];
    }
    повернути p;
}

void hierarchy_predictions(float *predictions, ціле n, tree *hier, ціле only_leaves, ціле stride)
{
    ціле j;
    для(j = 0; j < n; ++j){
        ціле parent = hier->parent[j];
        якщо(parent >= 0){
            predictions[j*stride] *= predictions[parent*stride]; 
        }
    }
    якщо(only_leaves){
        для(j = 0; j < n; ++j){
            якщо(!hier->leaf[j]) predictions[j*stride] = 0;
        }
    }
}

ціле hierarchy_top_prediction(float *predictions, tree *hier, float thresh, ціле stride)
{
    float p = 1;
    ціле group = 0;
    ціле i;
    поки(1){
        float max = 0;
        ціле max_i = 0;

        для(i = 0; i < hier->group_size[group]; ++i){
            ціле index = i + hier->group_offset[group];
            float val = predictions[(i + hier->group_offset[group])*stride];
            якщо(val > max){
                max_i = index;
                max = val;
            }
        }
        якщо(p*max > thresh){
            p = p*max;
            group = hier->child[max_i];
            якщо(hier->child[max_i] < 0) повернути max_i;
        } інакше якщо (group == 0){
            повернути max_i;
        } інакше {
            повернути hier->parent[hier->group_offset[group]];
        }
    }
    повернути 0;
}

tree *read_tree(char *filename)
{
    tree t = {0};
    FILE *fp = fopen(filename, "r");

    char *line;
    ціле last_parent = -1;
    ціле group_size = 0;
    ціле groups = 0;
    ціле n = 0;
    поки((line=fgetl(fp)) != 0){
        char *id = calloc(256, sizeof(char));
        ціле parent = -1;
        sscanf(line, "%s %d", id, &parent);
        t.parent = realloc(t.parent, (n+1)*sizeof(ціле));
        t.parent[n] = parent;

        t.child = realloc(t.child, (n+1)*sizeof(ціле));
        t.child[n] = -1;

        t.name = realloc(t.name, (n+1)*sizeof(char *));
        t.name[n] = id;
        якщо(parent != last_parent){
            ++groups;
            t.group_offset = realloc(t.group_offset, groups * sizeof(ціле));
            t.group_offset[groups - 1] = n - group_size;
            t.group_size = realloc(t.group_size, groups * sizeof(ціле));
            t.group_size[groups - 1] = group_size;
            group_size = 0;
            last_parent = parent;
        }
        t.group = realloc(t.group, (n+1)*sizeof(ціле));
        t.group[n] = groups;
        якщо (parent >= 0) {
            t.child[parent] = groups;
        }
        ++n;
        ++group_size;
    }
    ++groups;
    t.group_offset = realloc(t.group_offset, groups * sizeof(ціле));
    t.group_offset[groups - 1] = n - group_size;
    t.group_size = realloc(t.group_size, groups * sizeof(ціле));
    t.group_size[groups - 1] = group_size;
    t.n = n;
    t.groups = groups;
    t.leaf = calloc(n, sizeof(ціле));
    ціле i;
    для(i = 0; i < n; ++i) t.leaf[i] = 1;
    для(i = 0; i < n; ++i) якщо(t.parent[i] >= 0) t.leaf[t.parent[i]] = 0;

    fclose(fp);
    tree *tree_ptr = calloc(1, sizeof(tree));
    *tree_ptr = t;
    //error(0);
    повернути tree_ptr;
}

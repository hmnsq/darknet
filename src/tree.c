#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "utils.h"
#include "data.h"

void change_leaves(tree *t, char *leaf_list)
{
    list *llist = get_paths(leaf_list);
    char **leaves = (char **)list_to_array(llist);
    целое n = llist->size;
    целое i,j;
    целое found = 0;
    для(i = 0; i < t->n; ++i){
        t->leaf[i] = 0;
        для(j = 0; j < n; ++j){
            если (0==strcmp(t->name[i], leaves[j])){
                t->leaf[i] = 1;
                ++found;
                break;
            }
        }
    }
    fprintf(stderr, "Found %d leaves.\n", found);
}

float get_hierarchy_probability(float *x, tree *hier, целое c, целое stride)
{
    float p = 1;
    пока(c >= 0){
        p = p * x[c*stride];
        c = hier->parent[c];
    }
    вернуть p;
}

void hierarchy_predictions(float *predictions, целое n, tree *hier, целое only_leaves, целое stride)
{
    целое j;
    для(j = 0; j < n; ++j){
        целое parent = hier->parent[j];
        если(parent >= 0){
            predictions[j*stride] *= predictions[parent*stride]; 
        }
    }
    если(only_leaves){
        для(j = 0; j < n; ++j){
            если(!hier->leaf[j]) predictions[j*stride] = 0;
        }
    }
}

целое hierarchy_top_prediction(float *predictions, tree *hier, float thresh, целое stride)
{
    float p = 1;
    целое group = 0;
    целое i;
    пока(1){
        float max = 0;
        целое max_i = 0;

        для(i = 0; i < hier->group_size[group]; ++i){
            целое index = i + hier->group_offset[group];
            float val = predictions[(i + hier->group_offset[group])*stride];
            если(val > max){
                max_i = index;
                max = val;
            }
        }
        если(p*max > thresh){
            p = p*max;
            group = hier->child[max_i];
            если(hier->child[max_i] < 0) вернуть max_i;
        } иначе если (group == 0){
            вернуть max_i;
        } иначе {
            вернуть hier->parent[hier->group_offset[group]];
        }
    }
    вернуть 0;
}

tree *read_tree(char *filename)
{
    tree t = {0};
    FILE *fp = fopen(filename, "r");

    char *line;
    целое last_parent = -1;
    целое group_size = 0;
    целое groups = 0;
    целое n = 0;
    пока((line=fgetl(fp)) != 0){
        char *id = calloc(256, sizeof(char));
        целое parent = -1;
        sscanf(line, "%s %d", id, &parent);
        t.parent = realloc(t.parent, (n+1)*sizeof(целое));
        t.parent[n] = parent;

        t.child = realloc(t.child, (n+1)*sizeof(целое));
        t.child[n] = -1;

        t.name = realloc(t.name, (n+1)*sizeof(char *));
        t.name[n] = id;
        если(parent != last_parent){
            ++groups;
            t.group_offset = realloc(t.group_offset, groups * sizeof(целое));
            t.group_offset[groups - 1] = n - group_size;
            t.group_size = realloc(t.group_size, groups * sizeof(целое));
            t.group_size[groups - 1] = group_size;
            group_size = 0;
            last_parent = parent;
        }
        t.group = realloc(t.group, (n+1)*sizeof(целое));
        t.group[n] = groups;
        если (parent >= 0) {
            t.child[parent] = groups;
        }
        ++n;
        ++group_size;
    }
    ++groups;
    t.group_offset = realloc(t.group_offset, groups * sizeof(целое));
    t.group_offset[groups - 1] = n - group_size;
    t.group_size = realloc(t.group_size, groups * sizeof(целое));
    t.group_size[groups - 1] = group_size;
    t.n = n;
    t.groups = groups;
    t.leaf = calloc(n, sizeof(целое));
    целое i;
    для(i = 0; i < n; ++i) t.leaf[i] = 1;
    для(i = 0; i < n; ++i) если(t.parent[i] >= 0) t.leaf[t.parent[i]] = 0;

    fclose(fp);
    tree *tree_ptr = calloc(1, sizeof(tree));
    *tree_ptr = t;
    //error(0);
    вернуть tree_ptr;
}

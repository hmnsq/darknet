#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "utils.h"
#include "data.h"

void change_leaves(tree *t, char *leaf_list)
{
    list *llist = get_paths(leaf_list);
    char **leaves = (char **)list_to_array(llist);
    цяло n = llist->size;
    цяло i,j;
    цяло found = 0;
    за(i = 0; i < t->n; ++i){
        t->leaf[i] = 0;
        за(j = 0; j < n; ++j){
            ако (0==strcmp(t->name[i], leaves[j])){
                t->leaf[i] = 1;
                ++found;
                break;
            }
        }
    }
    fprintf(stderr, "Found %d leaves.\n", found);
}

float get_hierarchy_probability(float *x, tree *hier, цяло c, цяло stride)
{
    float p = 1;
    докато(c >= 0){
        p = p * x[c*stride];
        c = hier->parent[c];
    }
    върни p;
}

void hierarchy_predictions(float *predictions, цяло n, tree *hier, цяло only_leaves, цяло stride)
{
    цяло j;
    за(j = 0; j < n; ++j){
        цяло parent = hier->parent[j];
        ако(parent >= 0){
            predictions[j*stride] *= predictions[parent*stride]; 
        }
    }
    ако(only_leaves){
        за(j = 0; j < n; ++j){
            ако(!hier->leaf[j]) predictions[j*stride] = 0;
        }
    }
}

цяло hierarchy_top_prediction(float *predictions, tree *hier, float thresh, цяло stride)
{
    float p = 1;
    цяло group = 0;
    цяло i;
    докато(1){
        float max = 0;
        цяло max_i = 0;

        за(i = 0; i < hier->group_size[group]; ++i){
            цяло index = i + hier->group_offset[group];
            float val = predictions[(i + hier->group_offset[group])*stride];
            ако(val > max){
                max_i = index;
                max = val;
            }
        }
        ако(p*max > thresh){
            p = p*max;
            group = hier->child[max_i];
            ако(hier->child[max_i] < 0) върни max_i;
        } иначе ако (group == 0){
            върни max_i;
        } иначе {
            върни hier->parent[hier->group_offset[group]];
        }
    }
    върни 0;
}

tree *read_tree(char *filename)
{
    tree t = {0};
    FILE *fp = fopen(filename, "r");

    char *line;
    цяло last_parent = -1;
    цяло group_size = 0;
    цяло groups = 0;
    цяло n = 0;
    докато((line=fgetl(fp)) != 0){
        char *id = calloc(256, sizeof(char));
        цяло parent = -1;
        sscanf(line, "%s %d", id, &parent);
        t.parent = realloc(t.parent, (n+1)*sizeof(цяло));
        t.parent[n] = parent;

        t.child = realloc(t.child, (n+1)*sizeof(цяло));
        t.child[n] = -1;

        t.name = realloc(t.name, (n+1)*sizeof(char *));
        t.name[n] = id;
        ако(parent != last_parent){
            ++groups;
            t.group_offset = realloc(t.group_offset, groups * sizeof(цяло));
            t.group_offset[groups - 1] = n - group_size;
            t.group_size = realloc(t.group_size, groups * sizeof(цяло));
            t.group_size[groups - 1] = group_size;
            group_size = 0;
            last_parent = parent;
        }
        t.group = realloc(t.group, (n+1)*sizeof(цяло));
        t.group[n] = groups;
        ако (parent >= 0) {
            t.child[parent] = groups;
        }
        ++n;
        ++group_size;
    }
    ++groups;
    t.group_offset = realloc(t.group_offset, groups * sizeof(цяло));
    t.group_offset[groups - 1] = n - group_size;
    t.group_size = realloc(t.group_size, groups * sizeof(цяло));
    t.group_size[groups - 1] = group_size;
    t.n = n;
    t.groups = groups;
    t.leaf = calloc(n, sizeof(цяло));
    цяло i;
    за(i = 0; i < n; ++i) t.leaf[i] = 1;
    за(i = 0; i < n; ++i) ако(t.parent[i] >= 0) t.leaf[t.parent[i]] = 0;

    fclose(fp);
    tree *tree_ptr = calloc(1, sizeof(tree));
    *tree_ptr = t;
    //error(0);
    върни tree_ptr;
}

#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "utils.h"
#include "data.h"

void change_leaves(tree *t, char *leaf_list)
{
    list *llist = get_paths(leaf_list);
    char **leaves = (char **)list_to_array(llist);
    цео n = llist->size;
    цео i,j;
    цео found = 0;
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

float get_hierarchy_probability(float *x, tree *hier, цео c, цео stride)
{
    float p = 1;
    док(c >= 0){
        p = p * x[c*stride];
        c = hier->parent[c];
    }
    врати p;
}

void hierarchy_predictions(float *predictions, цео n, tree *hier, цео only_leaves, цео stride)
{
    цео j;
    за(j = 0; j < n; ++j){
        цео parent = hier->parent[j];
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

цео hierarchy_top_prediction(float *predictions, tree *hier, float thresh, цео stride)
{
    float p = 1;
    цео group = 0;
    цео i;
    док(1){
        float max = 0;
        цео max_i = 0;

        за(i = 0; i < hier->group_size[group]; ++i){
            цео index = i + hier->group_offset[group];
            float val = predictions[(i + hier->group_offset[group])*stride];
            ако(val > max){
                max_i = index;
                max = val;
            }
        }
        ако(p*max > thresh){
            p = p*max;
            group = hier->child[max_i];
            ако(hier->child[max_i] < 0) врати max_i;
        } инако ако (group == 0){
            врати max_i;
        } инако {
            врати hier->parent[hier->group_offset[group]];
        }
    }
    врати 0;
}

tree *read_tree(char *filename)
{
    tree t = {0};
    FILE *fp = fopen(filename, "r");

    char *line;
    цео last_parent = -1;
    цео group_size = 0;
    цео groups = 0;
    цео n = 0;
    док((line=fgetl(fp)) != 0){
        char *id = calloc(256, sizeof(char));
        цео parent = -1;
        sscanf(line, "%s %d", id, &parent);
        t.parent = realloc(t.parent, (n+1)*sizeof(цео));
        t.parent[n] = parent;

        t.child = realloc(t.child, (n+1)*sizeof(цео));
        t.child[n] = -1;

        t.name = realloc(t.name, (n+1)*sizeof(char *));
        t.name[n] = id;
        ако(parent != last_parent){
            ++groups;
            t.group_offset = realloc(t.group_offset, groups * sizeof(цео));
            t.group_offset[groups - 1] = n - group_size;
            t.group_size = realloc(t.group_size, groups * sizeof(цео));
            t.group_size[groups - 1] = group_size;
            group_size = 0;
            last_parent = parent;
        }
        t.group = realloc(t.group, (n+1)*sizeof(цео));
        t.group[n] = groups;
        ако (parent >= 0) {
            t.child[parent] = groups;
        }
        ++n;
        ++group_size;
    }
    ++groups;
    t.group_offset = realloc(t.group_offset, groups * sizeof(цео));
    t.group_offset[groups - 1] = n - group_size;
    t.group_size = realloc(t.group_size, groups * sizeof(цео));
    t.group_size[groups - 1] = group_size;
    t.n = n;
    t.groups = groups;
    t.leaf = calloc(n, sizeof(цео));
    цео i;
    за(i = 0; i < n; ++i) t.leaf[i] = 1;
    за(i = 0; i < n; ++i) ако(t.parent[i] >= 0) t.leaf[t.parent[i]] = 0;

    fclose(fp);
    tree *tree_ptr = calloc(1, sizeof(tree));
    *tree_ptr = t;
    //error(0);
    врати tree_ptr;
}

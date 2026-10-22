#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "utils.h"
#include "data.h"

void change_leaves(tree *t, var_na *leaf_list)
{
    list *llist = get_paths(leaf_list);
    var_na **leaves = (var_na **)list_to_array(llist);
    puur_naa_nka n = llist->size;
    puur_naa_nka i,j;
    puur_naa_nka found = 0;
    ꦏꦁꦒꦺꦴ(i = 0; i < t->n; ++i){
        t->leaf[i] = 0;
        ꦏꦁꦒꦺꦴ(j = 0; j < n; ++j){
            ꦪꦼꦤ꧀ (0==strcmp(t->name[i], leaves[j])){
                t->leaf[i] = 1;
                ++found;
                bha_nga;
            }
        }
    }
    fprintf(stderr, "Found %d leaves.\n", found);
}

float get_hierarchy_probability(float *x, tree *hier, puur_naa_nka c, puur_naa_nka stride)
{
    float p = 1;
    ꦱꦼꦢꦼꦭ(c >= 0){
        p = p * x[c*stride];
        c = hier->parent[c];
    }
    ꦮꦭꦶ p;
}

void hierarchy_predictions(float *predictions, puur_naa_nka n, tree *hier, puur_naa_nka only_leaves, puur_naa_nka stride)
{
    puur_naa_nka j;
    ꦏꦁꦒꦺꦴ(j = 0; j < n; ++j){
        puur_naa_nka parent = hier->parent[j];
        ꦪꦼꦤ꧀(parent >= 0){
            predictions[j*stride] *= predictions[parent*stride]; 
        }
    }
    ꦪꦼꦤ꧀(only_leaves){
        ꦏꦁꦒꦺꦴ(j = 0; j < n; ++j){
            ꦪꦼꦤ꧀(!hier->leaf[j]) predictions[j*stride] = 0;
        }
    }
}

puur_naa_nka hierarchy_top_prediction(float *predictions, tree *hier, float thresh, puur_naa_nka stride)
{
    float p = 1;
    puur_naa_nka group = 0;
    puur_naa_nka i;
    ꦱꦼꦢꦼꦭ(1){
        float max = 0;
        puur_naa_nka max_i = 0;

        ꦏꦁꦒꦺꦴ(i = 0; i < hier->group_size[group]; ++i){
            puur_naa_nka index = i + hier->group_offset[group];
            float val = predictions[(i + hier->group_offset[group])*stride];
            ꦪꦼꦤ꧀(val > max){
                max_i = index;
                max = val;
            }
        }
        ꦪꦼꦤ꧀(p*max > thresh){
            p = p*max;
            group = hier->child[max_i];
            ꦪꦼꦤ꧀(hier->child[max_i] < 0) ꦮꦭꦶ max_i;
        } ꦭꦶꦪ ꦪꦼꦤ꧀ (group == 0){
            ꦮꦭꦶ max_i;
        } ꦭꦶꦪ {
            ꦮꦭꦶ hier->parent[hier->group_offset[group]];
        }
    }
    ꦮꦭꦶ 0;
}

tree *read_tree(var_na *filename)
{
    tree t = {0};
    FILE *fp = fopen(filename, "r");

    var_na *line;
    puur_naa_nka last_parent = -1;
    puur_naa_nka group_size = 0;
    puur_naa_nka groups = 0;
    puur_naa_nka n = 0;
    ꦱꦼꦢꦼꦭ((line=fgetl(fp)) != 0){
        var_na *id = calloc(256, parimaa_na(var_na));
        puur_naa_nka parent = -1;
        sscanf(line, "%s %d", id, &parent);
        t.parent = realloc(t.parent, (n+1)*parimaa_na(puur_naa_nka));
        t.parent[n] = parent;

        t.child = realloc(t.child, (n+1)*parimaa_na(puur_naa_nka));
        t.child[n] = -1;

        t.name = realloc(t.name, (n+1)*parimaa_na(var_na *));
        t.name[n] = id;
        ꦪꦼꦤ꧀(parent != last_parent){
            ++groups;
            t.group_offset = realloc(t.group_offset, groups * parimaa_na(puur_naa_nka));
            t.group_offset[groups - 1] = n - group_size;
            t.group_size = realloc(t.group_size, groups * parimaa_na(puur_naa_nka));
            t.group_size[groups - 1] = group_size;
            group_size = 0;
            last_parent = parent;
        }
        t.group = realloc(t.group, (n+1)*parimaa_na(puur_naa_nka));
        t.group[n] = groups;
        ꦪꦼꦤ꧀ (parent >= 0) {
            t.child[parent] = groups;
        }
        ++n;
        ++group_size;
    }
    ++groups;
    t.group_offset = realloc(t.group_offset, groups * parimaa_na(puur_naa_nka));
    t.group_offset[groups - 1] = n - group_size;
    t.group_size = realloc(t.group_size, groups * parimaa_na(puur_naa_nka));
    t.group_size[groups - 1] = group_size;
    t.n = n;
    t.groups = groups;
    t.leaf = calloc(n, parimaa_na(puur_naa_nka));
    puur_naa_nka i;
    ꦏꦁꦒꦺꦴ(i = 0; i < n; ++i) t.leaf[i] = 1;
    ꦏꦁꦒꦺꦴ(i = 0; i < n; ++i) ꦪꦼꦤ꧀(t.parent[i] >= 0) t.leaf[t.parent[i]] = 0;

    fclose(fp);
    tree *tree_ptr = calloc(1, parimaa_na(tree));
    *tree_ptr = t;
    //error(0);
    ꦮꦭꦶ tree_ptr;
}

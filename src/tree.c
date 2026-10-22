#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ "tree.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "data.h"

ଶୂନ୍ୟ change_leaves(ବୃକ୍ଷ *t, ଅକ୍ଷର *leaf_list)
{
    list *llist = get_paths(leaf_list);
    ଅକ୍ଷର **leaves = (ଅକ୍ଷର **)list_to_array(llist);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = llist->size;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା found = 0;
    ପାଇଁ(i = 0; i < t->n; ++i){
        t->leaf[i] = 0;
        ପାଇଁ(j = 0; j < n; ++j){
            ଯଦି (0==strcmp(t->name[i], leaves[j])){
                t->leaf[i] = 1;
                ++found;
                ଭାଙ୍ଗ;
            }
        }
    }
    fprintf(stderr, "Found %d leaves.\n", found);
}

ଦଶମିକ get_hierarchy_probability(ଦଶମିକ *x, ବୃକ୍ଷ *hier, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride)
{
    ଦଶମିକ p = 1;
    ଯେତେବେଳେ(c >= 0){
        p = p * x[c*stride];
        c = hier->parent[c];
    }
    ଫେରନ୍ତୁ p;
}

ଶୂନ୍ୟ hierarchy_predictions(ଦଶମିକ *predictions, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ବୃକ୍ଷ *hier, ପୂର୍ଣ୍ଣସଂଖ୍ୟା only_leaves, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;
    ପାଇଁ(j = 0; j < n; ++j){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା parent = hier->parent[j];
        ଯଦି(parent >= 0){
            predictions[j*stride] *= predictions[parent*stride]; 
        }
    }
    ଯଦି(only_leaves){
        ପାଇଁ(j = 0; j < n; ++j){
            ଯଦି(!hier->leaf[j]) predictions[j*stride] = 0;
        }
    }
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା hierarchy_ଶୀର୍ଷ_prediction(ଦଶମିକ *predictions, ବୃକ୍ଷ *hier, ଦଶମିକ thresh, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride)
{
    ଦଶମିକ p = 1;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା group = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଯେତେବେଳେ(1){
        ଦଶମିକ max = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_i = 0;

        ପାଇଁ(i = 0; i < hier->group_size[group]; ++i){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = i + hier->group_offset[group];
            ଦଶମିକ val = predictions[(i + hier->group_offset[group])*stride];
            ଯଦି(val > max){
                max_i = index;
                max = val;
            }
        }
        ଯଦି(p*max > thresh){
            p = p*max;
            group = hier->child[max_i];
            ଯଦି(hier->child[max_i] < 0) ଫେରନ୍ତୁ max_i;
        } ଅନ୍ୟଥା ଯଦି (group == 0){
            ଫେରନ୍ତୁ max_i;
        } ଅନ୍ୟଥା {
            ଫେରନ୍ତୁ hier->parent[hier->group_offset[group]];
        }
    }
    ଫେରନ୍ତୁ 0;
}

ବୃକ୍ଷ *read_ବୃକ୍ଷ(ଅକ୍ଷର *filename)
{
    ବୃକ୍ଷ t = {0};
    FILE *fp = fopen(filename, "r");

    ଅକ୍ଷର *ଧାଡ଼ି;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା last_parent = -1;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା group_size = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା groups = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 0;
    ଯେତେବେଳେ((ଧାଡ଼ି=fgetl(fp)) != 0){
        ଅକ୍ଷର *id = calloc(256, ଆକାର(ଅକ୍ଷର));
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା parent = -1;
        sscanf(ଧାଡ଼ି, "%s %d", id, &parent);
        t.parent = realloc(t.parent, (n+1)*ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
        t.parent[n] = parent;

        t.child = realloc(t.child, (n+1)*ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
        t.child[n] = -1;

        t.name = realloc(t.name, (n+1)*ଆକାର(ଅକ୍ଷର *));
        t.name[n] = id;
        ଯଦି(parent != last_parent){
            ++groups;
            t.group_offset = realloc(t.group_offset, groups * ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
            t.group_offset[groups - 1] = n - group_size;
            t.group_size = realloc(t.group_size, groups * ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
            t.group_size[groups - 1] = group_size;
            group_size = 0;
            last_parent = parent;
        }
        t.group = realloc(t.group, (n+1)*ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
        t.group[n] = groups;
        ଯଦି (parent >= 0) {
            t.child[parent] = groups;
        }
        ++n;
        ++group_size;
    }
    ++groups;
    t.group_offset = realloc(t.group_offset, groups * ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
    t.group_offset[groups - 1] = n - group_size;
    t.group_size = realloc(t.group_size, groups * ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
    t.group_size[groups - 1] = group_size;
    t.n = n;
    t.groups = groups;
    t.leaf = calloc(n, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n; ++i) t.leaf[i] = 1;
    ପାଇଁ(i = 0; i < n; ++i) ଯଦି(t.parent[i] >= 0) t.leaf[t.parent[i]] = 0;

    fclose(fp);
    ବୃକ୍ଷ *ବୃକ୍ଷ_ptr = calloc(1, ଆକାର(ବୃକ୍ଷ));
    *ବୃକ୍ଷ_ptr = t;
    //error(0);
    ଫେରନ୍ତୁ ବୃକ୍ଷ_ptr;
}

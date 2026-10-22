#شامل <stdio.h>
#شامل <stdlib.h>
#شامل "tree.h"
#شامل "utils.h"
#شامل "data.h"

تهی change_leaves(درخت *t, نویسه *leaf_list)
{
    list *llist = get_paths(leaf_list);
    نویسه **leaves = (نویسه **)list_to_array(llist);
    صحیح n = llist->size;
    صحیح i,j;
    صحیح found = 0;
    برای(i = 0; i < t->n; ++i){
        t->leaf[i] = 0;
        برای(j = 0; j < n; ++j){
            اگر (0==strcmp(t->name[i], leaves[j])){
                t->leaf[i] = 1;
                ++found;
                بشکن;
            }
        }
    }
    fprintf(stderr, "Found %d leaves.\n", found);
}

اعشاری get_hierarchy_probability(اعشاری *x, درخت *hier, صحیح c, صحیح stride)
{
    اعشاری p = 1;
    وقتی(c >= 0){
        p = p * x[c*stride];
        c = hier->parent[c];
    }
    بازگشت p;
}

تهی hierarchy_predictions(اعشاری *predictions, صحیح n, درخت *hier, صحیح only_leaves, صحیح stride)
{
    صحیح j;
    برای(j = 0; j < n; ++j){
        صحیح parent = hier->parent[j];
        اگر(parent >= 0){
            predictions[j*stride] *= predictions[parent*stride]; 
        }
    }
    اگر(only_leaves){
        برای(j = 0; j < n; ++j){
            اگر(!hier->leaf[j]) predictions[j*stride] = 0;
        }
    }
}

صحیح hierarchy_بالاترین_prediction(اعشاری *predictions, درخت *hier, اعشاری thresh, صحیح stride)
{
    اعشاری p = 1;
    صحیح group = 0;
    صحیح i;
    وقتی(1){
        اعشاری max = 0;
        صحیح max_i = 0;

        برای(i = 0; i < hier->group_size[group]; ++i){
            صحیح index = i + hier->group_offset[group];
            اعشاری val = predictions[(i + hier->group_offset[group])*stride];
            اگر(val > max){
                max_i = index;
                max = val;
            }
        }
        اگر(p*max > thresh){
            p = p*max;
            group = hier->child[max_i];
            اگر(hier->child[max_i] < 0) بازگشت max_i;
        } وگرنه اگر (group == 0){
            بازگشت max_i;
        } وگرنه {
            بازگشت hier->parent[hier->group_offset[group]];
        }
    }
    بازگشت 0;
}

درخت *read_درخت(نویسه *filename)
{
    درخت t = {0};
    FILE *fp = fopen(filename, "r");

    نویسه *سطر;
    صحیح last_parent = -1;
    صحیح group_size = 0;
    صحیح groups = 0;
    صحیح n = 0;
    وقتی((سطر=fgetl(fp)) != 0){
        نویسه *id = calloc(256, اندازه(نویسه));
        صحیح parent = -1;
        sscanf(سطر, "%s %d", id, &parent);
        t.parent = realloc(t.parent, (n+1)*اندازه(صحیح));
        t.parent[n] = parent;

        t.child = realloc(t.child, (n+1)*اندازه(صحیح));
        t.child[n] = -1;

        t.name = realloc(t.name, (n+1)*اندازه(نویسه *));
        t.name[n] = id;
        اگر(parent != last_parent){
            ++groups;
            t.group_offset = realloc(t.group_offset, groups * اندازه(صحیح));
            t.group_offset[groups - 1] = n - group_size;
            t.group_size = realloc(t.group_size, groups * اندازه(صحیح));
            t.group_size[groups - 1] = group_size;
            group_size = 0;
            last_parent = parent;
        }
        t.group = realloc(t.group, (n+1)*اندازه(صحیح));
        t.group[n] = groups;
        اگر (parent >= 0) {
            t.child[parent] = groups;
        }
        ++n;
        ++group_size;
    }
    ++groups;
    t.group_offset = realloc(t.group_offset, groups * اندازه(صحیح));
    t.group_offset[groups - 1] = n - group_size;
    t.group_size = realloc(t.group_size, groups * اندازه(صحیح));
    t.group_size[groups - 1] = group_size;
    t.n = n;
    t.groups = groups;
    t.leaf = calloc(n, اندازه(صحیح));
    صحیح i;
    برای(i = 0; i < n; ++i) t.leaf[i] = 1;
    برای(i = 0; i < n; ++i) اگر(t.parent[i] >= 0) t.leaf[t.parent[i]] = 0;

    fclose(fp);
    درخت *درخت_ptr = calloc(1, اندازه(درخت));
    *درخت_ptr = t;
    //error(0);
    بازگشت درخت_ptr;
}

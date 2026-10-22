#અન્તર્ભાવ <stdio.h>

#અન્તર્ભાવ "network.h"
#અન્તર્ભાવ "detection_layer.h"
#અન્તર્ભાવ "cost_layer.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "parser.h"
#અન્તર્ભાવ "box.h"

શૂન્ય train_compare(અક્ષર *cfgfile, અક્ષર *weightfile)
{
    srand(time(0));
    દશાંશ avg_loss = -1;
    અક્ષર *base = basecfg(cfgfile);
    અક્ષર *backup_directory = "/home/pjreddie/backup/";
    printf("%s\n", base);
    network net = parse_network_cfg(cfgfile);
    જો(weightfile){
        load_weights(&net, weightfile);
    }
    printf("Learning Rate: %g, Momentum: %g, Decay: %g\n", net.learning_rate, net.momentum, net.decay);
    પૂર્ણાંક imgs = 1024;
    list *plist = get_paths("data/compare.train.list");
    અક્ષર **paths = (અક્ષર **)list_to_array(plist);
    પૂર્ણાંક N = plist->size;
    printf("%d\n", N);
    clock_t time;
    pthread_t load_thread;
    data train;
    data buffer;

    load_args args = {0};
    args.w = net.w;
    args.h = net.h;
    args.paths = paths;
    args.classes = 20;
    args.n = imgs;
    args.m = N;
    args.d = &buffer;
    args.type = COMPARE_DATA;

    load_thread = load_data_in_thread(args);
    પૂર્ણાંક epoch = *net.seen/N;
    પૂર્ણાંક i = 0;
    જ્યારે(1){
        ++i;
        time=clock();
        pthread_join(load_thread, 0);
        train = buffer;

        load_thread = load_data_in_thread(args);
        printf("Loaded: %lf seconds\n", sec(clock()-time));
        time=clock();
        દશાંશ loss = train_network(net, train);
        જો(avg_loss == -1) avg_loss = loss;
        avg_loss = avg_loss*.9 + loss*.1;
        printf("%.3f: %f, %f avg, %lf seconds, %ld images\n", (દશાંશ)*net.seen/N, loss, avg_loss, sec(clock()-time), *net.seen);
        free_data(train);
        જો(i%100 == 0){
            અક્ષર buff[256];
            sprintf(buff, "%s/%s_%d_minor_%d.weights",backup_directory,base, epoch, i);
            save_weights(net, buff);
        }
        જો(*net.seen/N > epoch){
            epoch = *net.seen/N;
            i = 0;
            અક્ષર buff[256];
            sprintf(buff, "%s/%s_%d.weights",backup_directory,base, epoch);
            save_weights(net, buff);
            જો(epoch%22 == 0) net.learning_rate *= .1;
        }
    }
    pthread_join(load_thread, 0);
    free_data(buffer);
    free_network(net);
    free_ptrs((શૂન્ય**)paths, plist->size);
    free_list(plist);
    મુક્ત(base);
}

શૂન્ય validate_compare(અક્ષર *filename, અક્ષર *weightfile)
{
    પૂર્ણાંક i = 0;
    network net = parse_network_cfg(filename);
    જો(weightfile){
        load_weights(&net, weightfile);
    }
    srand(time(0));

    list *plist = get_paths("data/compare.val.list");
    //list *plist = get_paths("data/compare.val.old");
    અક્ષર **paths = (અક્ષર **)list_to_array(plist);
    પૂર્ણાંક N = plist->size/2;
    free_list(plist);

    clock_t time;
    પૂર્ણાંક correct = 0;
    પૂર્ણાંક total = 0;
    પૂર્ણાંક splits = 10;
    પૂર્ણાંક અંક = (i+1)*N/splits - i*N/splits;

    data val, buffer;

    load_args args = {0};
    args.w = net.w;
    args.h = net.h;
    args.paths = paths;
    args.classes = 20;
    args.n = અંક;
    args.m = 0;
    args.d = &buffer;
    args.type = COMPARE_DATA;

    pthread_t load_thread = load_data_in_thread(args);
    માટે(i = 1; i <= splits; ++i){
        time=clock();

        pthread_join(load_thread, 0);
        val = buffer;

        અંક = (i+1)*N/splits - i*N/splits;
        અક્ષર **part = paths+(i*N/splits);
        જો(i != splits){
            args.paths = part;
            load_thread = load_data_in_thread(args);
        }
        printf("Loaded: %d images in %lf seconds\n", val.X.rows, sec(clock()-time));

        time=clock();
        matrix pred = network_predict_data(net, val);
        પૂર્ણાંક j,k;
        માટે(j = 0; j < val.y.rows; ++j){
            માટે(k = 0; k < 20; ++k){
                જો(val.y.vals[j][k*2] != val.y.vals[j][k*2+1]){
                    ++total;
                    જો((val.y.vals[j][k*2] < val.y.vals[j][k*2+1]) == (pred.vals[j][k*2] < pred.vals[j][k*2+1])){
                        ++correct;
                    }
                }
            }
        }
        free_matrix(pred);
        printf("%d: Acc: %f, %lf seconds, %d images\n", i, (દશાંશ)correct/total, sec(clock()-time), val.X.rows);
        free_data(val);
    }
}

પ્રકારવ્યાખ્યા સંરચના {
    network net;
    અક્ષર *filename;
    પૂર્ણાંક class;
    પૂર્ણાંક classes;
    દશાંશ elo;
    દશાંશ *elos;
} sortable_bbox;

પૂર્ણાંક total_compares = 0;
પૂર્ણાંક current_class = 0;

પૂર્ણાંક elo_comparator(અચલ શૂન્ય*a, અચલ શૂન્ય *b)
{
    sortable_bbox box1 = *(sortable_bbox*)a;
    sortable_bbox box2 = *(sortable_bbox*)b;
    જો(box1.elos[current_class] == box2.elos[current_class]) પાછા 0;
    જો(box1.elos[current_class] >  box2.elos[current_class]) પાછા -1;
    પાછા 1;
}

પૂર્ણાંક bbox_comparator(અચલ શૂન્ય *a, અચલ શૂન્ય *b)
{
    ++total_compares;
    sortable_bbox box1 = *(sortable_bbox*)a;
    sortable_bbox box2 = *(sortable_bbox*)b;
    network net = box1.net;
    પૂર્ણાંક class   = box1.class;

    image im1 = load_image_color(box1.filename, net.w, net.h);
    image im2 = load_image_color(box2.filename, net.w, net.h);
    દશાંશ *X  = calloc(net.w*net.h*net.c, કદ(દશાંશ));
    memcpy(X,                   im1.data, im1.w*im1.h*im1.c*કદ(દશાંશ));
    memcpy(X+im1.w*im1.h*im1.c, im2.data, im2.w*im2.h*im2.c*કદ(દશાંશ));
    દશાંશ *predictions = network_predict(net, X);
    
    free_image(im1);
    free_image(im2);
    મુક્ત(X);
    જો (predictions[class*2] > predictions[class*2+1]){
        પાછા 1;
    }
    પાછા -1;
}

શૂન્ય bbox_update(sortable_bbox *a, sortable_bbox *b, પૂર્ણાંક class, પૂર્ણાંક result)
{
    પૂર્ણાંક k = 32;
    દશાંશ EA = 1./(1+pow(10, (b->elos[class] - a->elos[class])/400.));
    દશાંશ EB = 1./(1+pow(10, (a->elos[class] - b->elos[class])/400.));
    દશાંશ SA = result ? 1 : 0;
    દશાંશ SB = result ? 0 : 1;
    a->elos[class] += k*(SA - EA);
    b->elos[class] += k*(SB - EB);
}

શૂન્ય bbox_fight(network net, sortable_bbox *a, sortable_bbox *b, પૂર્ણાંક classes, પૂર્ણાંક class)
{
    image im1 = load_image_color(a->filename, net.w, net.h);
    image im2 = load_image_color(b->filename, net.w, net.h);
    દશાંશ *X  = calloc(net.w*net.h*net.c, કદ(દશાંશ));
    memcpy(X,                   im1.data, im1.w*im1.h*im1.c*કદ(દશાંશ));
    memcpy(X+im1.w*im1.h*im1.c, im2.data, im2.w*im2.h*im2.c*કદ(દશાંશ));
    દશાંશ *predictions = network_predict(net, X);
    ++total_compares;

    પૂર્ણાંક i;
    માટે(i = 0; i < classes; ++i){
        જો(class < 0 || class == i){
            પૂર્ણાંક result = predictions[i*2] > predictions[i*2+1];
            bbox_update(a, b, i, result);
        }
    }
    
    free_image(im1);
    free_image(im2);
    મુક્ત(X);
}

શૂન્ય SortMaster3000(અક્ષર *filename, અક્ષર *weightfile)
{
    પૂર્ણાંક i = 0;
    network net = parse_network_cfg(filename);
    જો(weightfile){
        load_weights(&net, weightfile);
    }
    srand(time(0));
    set_batch_network(&net, 1);

    list *plist = get_paths("data/compare.sort.list");
    //list *plist = get_paths("data/compare.val.old");
    અક્ષર **paths = (અક્ષર **)list_to_array(plist);
    પૂર્ણાંક N = plist->size;
    free_list(plist);
    sortable_bbox *boxes = calloc(N, કદ(sortable_bbox));
    printf("Sorting %d boxes...\n", N);
    માટે(i = 0; i < N; ++i){
        boxes[i].filename = paths[i];
        boxes[i].net = net;
        boxes[i].class = 7;
        boxes[i].elo = 1500;
    }
    clock_t time=clock();
    qsort(boxes, N, કદ(sortable_bbox), bbox_comparator);
    માટે(i = 0; i < N; ++i){
        printf("%s\n", boxes[i].filename);
    }
    printf("Sorted in %d compares, %f secs\n", total_compares, sec(clock()-time));
}

શૂન્ય BattleRoyaleWithCheese(અક્ષર *filename, અક્ષર *weightfile)
{
    પૂર્ણાંક classes = 20;
    પૂર્ણાંક i,j;
    network net = parse_network_cfg(filename);
    જો(weightfile){
        load_weights(&net, weightfile);
    }
    srand(time(0));
    set_batch_network(&net, 1);

    list *plist = get_paths("data/compare.sort.list");
    //list *plist = get_paths("data/compare.small.list");
    //list *plist = get_paths("data/compare.cat.list");
    //list *plist = get_paths("data/compare.val.old");
    અક્ષર **paths = (અક્ષર **)list_to_array(plist);
    પૂર્ણાંક N = plist->size;
    પૂર્ણાંક total = N;
    free_list(plist);
    sortable_bbox *boxes = calloc(N, કદ(sortable_bbox));
    printf("Battling %d boxes...\n", N);
    માટે(i = 0; i < N; ++i){
        boxes[i].filename = paths[i];
        boxes[i].net = net;
        boxes[i].classes = classes;
        boxes[i].elos = calloc(classes, કદ(દશાંશ));;
        માટે(j = 0; j < classes; ++j){
            boxes[i].elos[j] = 1500;
        }
    }
    પૂર્ણાંક round;
    clock_t time=clock();
    માટે(round = 1; round <= 4; ++round){
        clock_t round_time=clock();
        printf("Round: %d\n", round);
        shuffle(boxes, N, કદ(sortable_bbox));
        માટે(i = 0; i < N/2; ++i){
            bbox_fight(net, boxes+i*2, boxes+i*2+1, classes, -1);
        }
        printf("Round: %f secs, %d remaining\n", sec(clock()-round_time), N);
    }

    પૂર્ણાંક class;

    માટે (class = 0; class < classes; ++class){

        N = total;
        current_class = class;
        qsort(boxes, N, કદ(sortable_bbox), elo_comparator);
        N /= 2;

        માટે(round = 1; round <= 100; ++round){
            clock_t round_time=clock();
            printf("Round: %d\n", round);

            sorta_shuffle(boxes, N, કદ(sortable_bbox), 10);
            માટે(i = 0; i < N/2; ++i){
                bbox_fight(net, boxes+i*2, boxes+i*2+1, classes, class);
            }
            qsort(boxes, N, કદ(sortable_bbox), elo_comparator);
            જો(round <= 20) N = (N*9/10)/2*2;

            printf("Round: %f secs, %d remaining\n", sec(clock()-round_time), N);
        }
        અક્ષર buff[256];
        sprintf(buff, "results/battle_%d.log", class);
        FILE *outfp = fopen(buff, "w");
        માટે(i = 0; i < N; ++i){
            fprintf(outfp, "%s %f\n", boxes[i].filename, boxes[i].elos[class]);
        }
        fclose(outfp);
    }
    printf("Tournament in %d compares, %f secs\n", total_compares, sec(clock()-time));
}

શૂન્ય run_compare(પૂર્ણાંક argc, અક્ષર **argv)
{
    જો(argc < 4){
        fprintf(stderr, "usage: %s %s [train/test/valid] [cfg] [weights (optional)]\n", argv[0], argv[1]);
        પાછા;
    }

    અક્ષર *cfg = argv[3];
    અક્ષર *weights = (argc > 4) ? argv[4] : 0;
    //char *filename = (argc > 5) ? argv[5]: 0;
    જો(0==strcmp(argv[2], "train")) train_compare(cfg, weights);
    નહીંતર જો(0==strcmp(argv[2], "valid")) validate_compare(cfg, weights);
    નહીંતર જો(0==strcmp(argv[2], "sort")) SortMaster3000(cfg, weights);
    નહીંતર જો(0==strcmp(argv[2], "battle")) BattleRoyaleWithCheese(cfg, weights);
    /*
       else if(0==strcmp(argv[2], "train")) train_coco(cfg, weights);
       else if(0==strcmp(argv[2], "extract")) extract_boxes(cfg, weights);
       else if(0==strcmp(argv[2], "valid")) validate_recall(cfg, weights);
     */
}

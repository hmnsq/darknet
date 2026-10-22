#ضمن <stdio.h>

#ضمن "network.h"
#ضمن "detection_layer.h"
#ضمن "cost_layer.h"
#ضمن "utils.h"
#ضمن "parser.h"
#ضمن "box.h"

فراغ train_compare(حرف *cfgfile, حرف *weightfile)
{
    srand(وقت(0));
    عائم avg_loss = -1;
    حرف *بنياد = basecfg(cfgfile);
    حرف *backup_directory = "/home/pjreddie/backup/";
    لکو("%s\n", بنياد);
    ڄار net = parse_ڄار_cfg(cfgfile);
    إذا(weightfile){
        ڀريو_weights(&net, weightfile);
    }
    لکو("Learning Rate: %g, Momentum: %g, Decay: %g\n", net.learning_شرح, net.momentum, net.decay);
    صحيح imgs = 1024;
    list *plist = get_paths("data/compare.train.list");
    حرف **paths = (حرف **)list_منزل_array(plist);
    صحيح N = plist->سائيز;
    لکو("%d\n", N);
    clock_t وقت;
    pthread_t ڀريو_thread;
    معلومات train;
    معلومات بفر;

    ڀريو_دليلون دليلون = {0};
    دليلون.w = net.w;
    دليلون.h = net.h;
    دليلون.paths = paths;
    دليلون.classes = 20;
    دليلون.n = imgs;
    دليلون.m = N;
    دليلون.d = &بفر;
    دليلون.type = COMPARE_معلومات;

    ڀريو_thread = ڀريو_معلومات_اندريون_thread(دليلون);
    صحيح epoch = *net.ڏٺل/N;
    صحيح i = 0;
    طالما(1){
        ++i;
        وقت=clock();
        pthread_join(ڀريو_thread, 0);
        train = بفر;

        ڀريو_thread = ڀريو_معلومات_اندريون_thread(دليلون);
        لکو("Loaded: %lf seconds\n", sec(clock()-وقت));
        وقت=clock();
        عائم loss = train_ڄار(net, train);
        إذا(avg_loss == -1) avg_loss = loss;
        avg_loss = avg_loss*.9 + loss*.1;
        لکو("%.3f: %f, %f avg, %lf seconds, %ld images\n", (عائم)*net.ڏٺل/N, loss, avg_loss, sec(clock()-وقت), *net.ڏٺل);
        free_معلومات(train);
        إذا(i%100 == 0){
            حرف buff[256];
            sprintf(buff, "%s/%s_%d_minor_%d.weights",backup_directory,بنياد, epoch, i);
            save_weights(net, buff);
        }
        إذا(*net.ڏٺل/N > epoch){
            epoch = *net.ڏٺل/N;
            i = 0;
            حرف buff[256];
            sprintf(buff, "%s/%s_%d.weights",backup_directory,بنياد, epoch);
            save_weights(net, buff);
            إذا(epoch%22 == 0) net.learning_شرح *= .1;
        }
    }
    pthread_join(ڀريو_thread, 0);
    free_معلومات(بفر);
    free_ڄار(net);
    free_ptrs((فراغ**)paths, plist->سائيز);
    free_list(plist);
    حرر(بنياد);
}

فراغ validate_compare(حرف *filename, حرف *weightfile)
{
    صحيح i = 0;
    ڄار net = parse_ڄار_cfg(filename);
    إذا(weightfile){
        ڀريو_weights(&net, weightfile);
    }
    srand(وقت(0));

    list *plist = get_paths("data/compare.val.list");
    //list *plist = get_paths("data/compare.val.old");
    حرف **paths = (حرف **)list_منزل_array(plist);
    صحيح N = plist->سائيز/2;
    free_list(plist);

    clock_t وقت;
    صحيح correct = 0;
    صحيح ڪل = 0;
    صحيح splits = 10;
    صحيح انگ = (i+1)*N/splits - i*N/splits;

    معلومات val, بفر;

    ڀريو_دليلون دليلون = {0};
    دليلون.w = net.w;
    دليلون.h = net.h;
    دليلون.paths = paths;
    دليلون.classes = 20;
    دليلون.n = انگ;
    دليلون.m = 0;
    دليلون.d = &بفر;
    دليلون.type = COMPARE_معلومات;

    pthread_t ڀريو_thread = ڀريو_معلومات_اندريون_thread(دليلون);
    لكل(i = 1; i <= splits; ++i){
        وقت=clock();

        pthread_join(ڀريو_thread, 0);
        val = بفر;

        انگ = (i+1)*N/splits - i*N/splits;
        حرف **part = paths+(i*N/splits);
        إذا(i != splits){
            دليلون.paths = part;
            ڀريو_thread = ڀريو_معلومات_اندريون_thread(دليلون);
        }
        لکو("Loaded: %d images in %lf seconds\n", val.X.صفون, sec(clock()-وقت));

        وقت=clock();
        ميٽرڪس pred = ڄار_predict_معلومات(net, val);
        صحيح j,k;
        لكل(j = 0; j < val.y.صفون; ++j){
            لكل(k = 0; k < 20; ++k){
                إذا(val.y.vals[j][k*2] != val.y.vals[j][k*2+1]){
                    ++ڪل;
                    إذا((val.y.vals[j][k*2] < val.y.vals[j][k*2+1]) == (pred.vals[j][k*2] < pred.vals[j][k*2+1])){
                        ++correct;
                    }
                }
            }
        }
        free_ميٽرڪس(pred);
        لکو("%d: Acc: %f, %lf seconds, %d images\n", i, (عائم)correct/ڪل, sec(clock()-وقت), val.X.صفون);
        free_معلومات(val);
    }
}

تعريفنوع بنية {
    ڄار net;
    حرف *filename;
    صحيح class;
    صحيح classes;
    عائم elo;
    عائم *elos;
} sortable_bbox;

صحيح ڪل_compares = 0;
صحيح current_class = 0;

صحيح elo_comparator(ثابت فراغ*a, ثابت فراغ *b)
{
    sortable_bbox box1 = *(sortable_bbox*)a;
    sortable_bbox box2 = *(sortable_bbox*)b;
    إذا(box1.elos[current_class] == box2.elos[current_class]) أرجع 0;
    إذا(box1.elos[current_class] >  box2.elos[current_class]) أرجع -1;
    أرجع 1;
}

صحيح bbox_comparator(ثابت فراغ *a, ثابت فراغ *b)
{
    ++ڪل_compares;
    sortable_bbox box1 = *(sortable_bbox*)a;
    sortable_bbox box2 = *(sortable_bbox*)b;
    ڄار net = box1.net;
    صحيح class   = box1.class;

    image im1 = ڀريو_image_color(box1.filename, net.w, net.h);
    image im2 = ڀريو_image_color(box2.filename, net.w, net.h);
    عائم *X  = صفر_يادگيري_ڏيو(net.w*net.h*net.c, حجم(عائم));
    يادگيري_نقل(X,                   im1.معلومات, im1.w*im1.h*im1.c*حجم(عائم));
    يادگيري_نقل(X+im1.w*im1.h*im1.c, im2.معلومات, im2.w*im2.h*im2.c*حجم(عائم));
    عائم *predictions = ڄار_predict(net, X);
    
    free_image(im1);
    free_image(im2);
    حرر(X);
    إذا (predictions[class*2] > predictions[class*2+1]){
        أرجع 1;
    }
    أرجع -1;
}

فراغ bbox_update(sortable_bbox *a, sortable_bbox *b, صحيح class, صحيح نتيجو)
{
    صحيح k = 32;
    عائم EA = 1./(1+قوت(10, (b->elos[class] - a->elos[class])/400.));
    عائم EB = 1./(1+قوت(10, (a->elos[class] - b->elos[class])/400.));
    عائم SA = نتيجو ? 1 : 0;
    عائم SB = نتيجو ? 0 : 1;
    a->elos[class] += k*(SA - EA);
    b->elos[class] += k*(SB - EB);
}

فراغ bbox_fight(ڄار net, sortable_bbox *a, sortable_bbox *b, صحيح classes, صحيح class)
{
    image im1 = ڀريو_image_color(a->filename, net.w, net.h);
    image im2 = ڀريو_image_color(b->filename, net.w, net.h);
    عائم *X  = صفر_يادگيري_ڏيو(net.w*net.h*net.c, حجم(عائم));
    يادگيري_نقل(X,                   im1.معلومات, im1.w*im1.h*im1.c*حجم(عائم));
    يادگيري_نقل(X+im1.w*im1.h*im1.c, im2.معلومات, im2.w*im2.h*im2.c*حجم(عائم));
    عائم *predictions = ڄار_predict(net, X);
    ++ڪل_compares;

    صحيح i;
    لكل(i = 0; i < classes; ++i){
        إذا(class < 0 || class == i){
            صحيح نتيجو = predictions[i*2] > predictions[i*2+1];
            bbox_update(a, b, i, نتيجو);
        }
    }
    
    free_image(im1);
    free_image(im2);
    حرر(X);
}

فراغ ترتيب_ڏيو_Master3000(حرف *filename, حرف *weightfile)
{
    صحيح i = 0;
    ڄار net = parse_ڄار_cfg(filename);
    إذا(weightfile){
        ڀريو_weights(&net, weightfile);
    }
    srand(وقت(0));
    سيٽ_batch_ڄار(&net, 1);

    list *plist = get_paths("data/compare.sort.list");
    //list *plist = get_paths("data/compare.val.old");
    حرف **paths = (حرف **)list_منزل_array(plist);
    صحيح N = plist->سائيز;
    free_list(plist);
    sortable_bbox *boxes = صفر_يادگيري_ڏيو(N, حجم(sortable_bbox));
    لکو("Sorting %d boxes...\n", N);
    لكل(i = 0; i < N; ++i){
        boxes[i].filename = paths[i];
        boxes[i].net = net;
        boxes[i].class = 7;
        boxes[i].elo = 1500;
    }
    clock_t وقت=clock();
    qsort(boxes, N, حجم(sortable_bbox), bbox_comparator);
    لكل(i = 0; i < N; ++i){
        لکو("%s\n", boxes[i].filename);
    }
    لکو("Sorted in %d compares, %f secs\n", ڪل_compares, sec(clock()-وقت));
}

فراغ BattleRoyaleWithCheese(حرف *filename, حرف *weightfile)
{
    صحيح classes = 20;
    صحيح i,j;
    ڄار net = parse_ڄار_cfg(filename);
    إذا(weightfile){
        ڀريو_weights(&net, weightfile);
    }
    srand(وقت(0));
    سيٽ_batch_ڄار(&net, 1);

    list *plist = get_paths("data/compare.sort.list");
    //list *plist = get_paths("data/compare.small.list");
    //list *plist = get_paths("data/compare.cat.list");
    //list *plist = get_paths("data/compare.val.old");
    حرف **paths = (حرف **)list_منزل_array(plist);
    صحيح N = plist->سائيز;
    صحيح ڪل = N;
    free_list(plist);
    sortable_bbox *boxes = صفر_يادگيري_ڏيو(N, حجم(sortable_bbox));
    لکو("Battling %d boxes...\n", N);
    لكل(i = 0; i < N; ++i){
        boxes[i].filename = paths[i];
        boxes[i].net = net;
        boxes[i].classes = classes;
        boxes[i].elos = صفر_يادگيري_ڏيو(classes, حجم(عائم));;
        لكل(j = 0; j < classes; ++j){
            boxes[i].elos[j] = 1500;
        }
    }
    صحيح ڦيرو;
    clock_t وقت=clock();
    لكل(ڦيرو = 1; ڦيرو <= 4; ++ڦيرو){
        clock_t ڦيرو_وقت=clock();
        لکو("Round: %d\n", ڦيرو);
        shuffle(boxes, N, حجم(sortable_bbox));
        لكل(i = 0; i < N/2; ++i){
            bbox_fight(net, boxes+i*2, boxes+i*2+1, classes, -1);
        }
        لکو("Round: %f secs, %d remaining\n", sec(clock()-ڦيرو_وقت), N);
    }

    صحيح class;

    لكل (class = 0; class < classes; ++class){

        N = ڪل;
        current_class = class;
        qsort(boxes, N, حجم(sortable_bbox), elo_comparator);
        N /= 2;

        لكل(ڦيرو = 1; ڦيرو <= 100; ++ڦيرو){
            clock_t ڦيرو_وقت=clock();
            لکو("Round: %d\n", ڦيرو);

            sorta_shuffle(boxes, N, حجم(sortable_bbox), 10);
            لكل(i = 0; i < N/2; ++i){
                bbox_fight(net, boxes+i*2, boxes+i*2+1, classes, class);
            }
            qsort(boxes, N, حجم(sortable_bbox), elo_comparator);
            إذا(ڦيرو <= 20) N = (N*9/10)/2*2;

            لکو("Round: %f secs, %d remaining\n", sec(clock()-ڦيرو_وقت), N);
        }
        حرف buff[256];
        sprintf(buff, "results/battle_%d.log", class);
        FILE *outfp = fopen(buff, "w");
        لكل(i = 0; i < N; ++i){
            fprintf(outfp, "%s %f\n", boxes[i].filename, boxes[i].elos[class]);
        }
        fclose(outfp);
    }
    لکو("Tournament in %d compares, %f secs\n", ڪل_compares, sec(clock()-وقت));
}

فراغ لاڳيتو_compare(صحيح argc, حرف **argv)
{
    إذا(argc < 4){
        fprintf(stderr, "usage: %s %s [train/test/valid] [cfg] [weights (optional)]\n", argv[0], argv[1]);
        أرجع;
    }

    حرف *cfg = argv[3];
    حرف *weights = (argc > 4) ? argv[4] : 0;
    //char *filename = (argc > 5) ? argv[5]: 0;
    إذا(0==strcmp(argv[2], "train")) train_compare(cfg, weights);
    وإلا إذا(0==strcmp(argv[2], "valid")) validate_compare(cfg, weights);
    وإلا إذا(0==strcmp(argv[2], "sort")) ترتيب_ڏيو_Master3000(cfg, weights);
    وإلا إذا(0==strcmp(argv[2], "battle")) BattleRoyaleWithCheese(cfg, weights);
    /*
       else if(0==strcmp(argv[2], "train")) train_coco(cfg, weights);
       else if(0==strcmp(argv[2], "extract")) extract_boxes(cfg, weights);
       else if(0==strcmp(argv[2], "valid")) validate_recall(cfg, weights);
     */
}

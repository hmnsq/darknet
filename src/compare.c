#ضمن <stdio.h>

#ضمن "network.h"
#ضمن "detection_layer.h"
#ضمن "cost_layer.h"
#ضمن "utils.h"
#ضمن "parser.h"
#ضمن "box.h"

فراغ train_compare(حرف *cfgfile, حرف *weightfile)
{
    srand(وخت(0));
    عائم avg_loss = -1;
    حرف *بنسټ = basecfg(cfgfile);
    حرف *backup_directory = "/home/pjreddie/backup/";
    چاپ_کړه("%s\n", بنسټ);
    شبکه net = parse_شبکه_cfg(cfgfile);
    إذا(weightfile){
        بارول_weights(&net, weightfile);
    }
    چاپ_کړه("Learning Rate: %g, Momentum: %g, Decay: %g\n", net.learning_شرح, net.momentum, net.decay);
    صحيح imgs = 1024;
    list *plist = get_paths("data/compare.train.list");
    حرف **paths = (حرف **)list_منزل_array(plist);
    صحيح N = plist->اندازه;
    چاپ_کړه("%d\n", N);
    clock_t وخت;
    pthread_t بارول_thread;
    معلومات train;
    معلومات بفر;

    بارول_دلیلونه دلیلونه = {0};
    دلیلونه.w = net.w;
    دلیلونه.h = net.h;
    دلیلونه.paths = paths;
    دلیلونه.classes = 20;
    دلیلونه.n = imgs;
    دلیلونه.m = N;
    دلیلونه.d = &بفر;
    دلیلونه.type = COMPARE_معلومات;

    بارول_thread = بارول_معلومات_دننه_thread(دلیلونه);
    صحيح epoch = *net.کتل_شوی/N;
    صحيح i = 0;
    طالما(1){
        ++i;
        وخت=clock();
        pthread_join(بارول_thread, 0);
        train = بفر;

        بارول_thread = بارول_معلومات_دننه_thread(دلیلونه);
        چاپ_کړه("Loaded: %lf seconds\n", sec(clock()-وخت));
        وخت=clock();
        عائم loss = train_شبکه(net, train);
        إذا(avg_loss == -1) avg_loss = loss;
        avg_loss = avg_loss*.9 + loss*.1;
        چاپ_کړه("%.3f: %f, %f avg, %lf seconds, %ld images\n", (عائم)*net.کتل_شوی/N, loss, avg_loss, sec(clock()-وخت), *net.کتل_شوی);
        free_معلومات(train);
        إذا(i%100 == 0){
            حرف buff[256];
            sprintf(buff, "%s/%s_%d_minor_%d.weights",backup_directory,بنسټ, epoch, i);
            save_weights(net, buff);
        }
        إذا(*net.کتل_شوی/N > epoch){
            epoch = *net.کتل_شوی/N;
            i = 0;
            حرف buff[256];
            sprintf(buff, "%s/%s_%d.weights",backup_directory,بنسټ, epoch);
            save_weights(net, buff);
            إذا(epoch%22 == 0) net.learning_شرح *= .1;
        }
    }
    pthread_join(بارول_thread, 0);
    free_معلومات(بفر);
    free_شبکه(net);
    free_ptrs((فراغ**)paths, plist->اندازه);
    free_list(plist);
    حرر(بنسټ);
}

فراغ validate_compare(حرف *filename, حرف *weightfile)
{
    صحيح i = 0;
    شبکه net = parse_شبکه_cfg(filename);
    إذا(weightfile){
        بارول_weights(&net, weightfile);
    }
    srand(وخت(0));

    list *plist = get_paths("data/compare.val.list");
    //list *plist = get_paths("data/compare.val.old");
    حرف **paths = (حرف **)list_منزل_array(plist);
    صحيح N = plist->اندازه/2;
    free_list(plist);

    clock_t وخت;
    صحيح correct = 0;
    صحيح ټول = 0;
    صحيح splits = 10;
    صحيح عدد = (i+1)*N/splits - i*N/splits;

    معلومات val, بفر;

    بارول_دلیلونه دلیلونه = {0};
    دلیلونه.w = net.w;
    دلیلونه.h = net.h;
    دلیلونه.paths = paths;
    دلیلونه.classes = 20;
    دلیلونه.n = عدد;
    دلیلونه.m = 0;
    دلیلونه.d = &بفر;
    دلیلونه.type = COMPARE_معلومات;

    pthread_t بارول_thread = بارول_معلومات_دننه_thread(دلیلونه);
    لكل(i = 1; i <= splits; ++i){
        وخت=clock();

        pthread_join(بارول_thread, 0);
        val = بفر;

        عدد = (i+1)*N/splits - i*N/splits;
        حرف **part = paths+(i*N/splits);
        إذا(i != splits){
            دلیلونه.paths = part;
            بارول_thread = بارول_معلومات_دننه_thread(دلیلونه);
        }
        چاپ_کړه("Loaded: %d images in %lf seconds\n", val.X.لیکې, sec(clock()-وخت));

        وخت=clock();
        ماتریکس pred = شبکه_predict_معلومات(net, val);
        صحيح j,k;
        لكل(j = 0; j < val.y.لیکې; ++j){
            لكل(k = 0; k < 20; ++k){
                إذا(val.y.vals[j][k*2] != val.y.vals[j][k*2+1]){
                    ++ټول;
                    إذا((val.y.vals[j][k*2] < val.y.vals[j][k*2+1]) == (pred.vals[j][k*2] < pred.vals[j][k*2+1])){
                        ++correct;
                    }
                }
            }
        }
        free_ماتریکس(pred);
        چاپ_کړه("%d: Acc: %f, %lf seconds, %d images\n", i, (عائم)correct/ټول, sec(clock()-وخت), val.X.لیکې);
        free_معلومات(val);
    }
}

تعريفنوع بنية {
    شبکه net;
    حرف *filename;
    صحيح class;
    صحيح classes;
    عائم elo;
    عائم *elos;
} sortable_bbox;

صحيح ټول_compares = 0;
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
    ++ټول_compares;
    sortable_bbox box1 = *(sortable_bbox*)a;
    sortable_bbox box2 = *(sortable_bbox*)b;
    شبکه net = box1.net;
    صحيح class   = box1.class;

    image im1 = بارول_image_color(box1.filename, net.w, net.h);
    image im2 = بارول_image_color(box2.filename, net.w, net.h);
    عائم *X  = حافظه_صفر_ورکړه(net.w*net.h*net.c, حجم(عائم));
    حافظه_نقل(X,                   im1.معلومات, im1.w*im1.h*im1.c*حجم(عائم));
    حافظه_نقل(X+im1.w*im1.h*im1.c, im2.معلومات, im2.w*im2.h*im2.c*حجم(عائم));
    عائم *predictions = شبکه_predict(net, X);
    
    free_image(im1);
    free_image(im2);
    حرر(X);
    إذا (predictions[class*2] > predictions[class*2+1]){
        أرجع 1;
    }
    أرجع -1;
}

فراغ bbox_update(sortable_bbox *a, sortable_bbox *b, صحيح class, صحيح پایله)
{
    صحيح k = 32;
    عائم EA = 1./(1+توان(10, (b->elos[class] - a->elos[class])/400.));
    عائم EB = 1./(1+توان(10, (a->elos[class] - b->elos[class])/400.));
    عائم SA = پایله ? 1 : 0;
    عائم SB = پایله ? 0 : 1;
    a->elos[class] += k*(SA - EA);
    b->elos[class] += k*(SB - EB);
}

فراغ bbox_fight(شبکه net, sortable_bbox *a, sortable_bbox *b, صحيح classes, صحيح class)
{
    image im1 = بارول_image_color(a->filename, net.w, net.h);
    image im2 = بارول_image_color(b->filename, net.w, net.h);
    عائم *X  = حافظه_صفر_ورکړه(net.w*net.h*net.c, حجم(عائم));
    حافظه_نقل(X,                   im1.معلومات, im1.w*im1.h*im1.c*حجم(عائم));
    حافظه_نقل(X+im1.w*im1.h*im1.c, im2.معلومات, im2.w*im2.h*im2.c*حجم(عائم));
    عائم *predictions = شبکه_predict(net, X);
    ++ټول_compares;

    صحيح i;
    لكل(i = 0; i < classes; ++i){
        إذا(class < 0 || class == i){
            صحيح پایله = predictions[i*2] > predictions[i*2+1];
            bbox_update(a, b, i, پایله);
        }
    }
    
    free_image(im1);
    free_image(im2);
    حرر(X);
}

فراغ ترتیبول_Master3000(حرف *filename, حرف *weightfile)
{
    صحيح i = 0;
    شبکه net = parse_شبکه_cfg(filename);
    إذا(weightfile){
        بارول_weights(&net, weightfile);
    }
    srand(وخت(0));
    سېټ_batch_شبکه(&net, 1);

    list *plist = get_paths("data/compare.sort.list");
    //list *plist = get_paths("data/compare.val.old");
    حرف **paths = (حرف **)list_منزل_array(plist);
    صحيح N = plist->اندازه;
    free_list(plist);
    sortable_bbox *boxes = حافظه_صفر_ورکړه(N, حجم(sortable_bbox));
    چاپ_کړه("Sorting %d boxes...\n", N);
    لكل(i = 0; i < N; ++i){
        boxes[i].filename = paths[i];
        boxes[i].net = net;
        boxes[i].class = 7;
        boxes[i].elo = 1500;
    }
    clock_t وخت=clock();
    qsort(boxes, N, حجم(sortable_bbox), bbox_comparator);
    لكل(i = 0; i < N; ++i){
        چاپ_کړه("%s\n", boxes[i].filename);
    }
    چاپ_کړه("Sorted in %d compares, %f secs\n", ټول_compares, sec(clock()-وخت));
}

فراغ BattleRoyaleWithCheese(حرف *filename, حرف *weightfile)
{
    صحيح classes = 20;
    صحيح i,j;
    شبکه net = parse_شبکه_cfg(filename);
    إذا(weightfile){
        بارول_weights(&net, weightfile);
    }
    srand(وخت(0));
    سېټ_batch_شبکه(&net, 1);

    list *plist = get_paths("data/compare.sort.list");
    //list *plist = get_paths("data/compare.small.list");
    //list *plist = get_paths("data/compare.cat.list");
    //list *plist = get_paths("data/compare.val.old");
    حرف **paths = (حرف **)list_منزل_array(plist);
    صحيح N = plist->اندازه;
    صحيح ټول = N;
    free_list(plist);
    sortable_bbox *boxes = حافظه_صفر_ورکړه(N, حجم(sortable_bbox));
    چاپ_کړه("Battling %d boxes...\n", N);
    لكل(i = 0; i < N; ++i){
        boxes[i].filename = paths[i];
        boxes[i].net = net;
        boxes[i].classes = classes;
        boxes[i].elos = حافظه_صفر_ورکړه(classes, حجم(عائم));;
        لكل(j = 0; j < classes; ++j){
            boxes[i].elos[j] = 1500;
        }
    }
    صحيح پړاو;
    clock_t وخت=clock();
    لكل(پړاو = 1; پړاو <= 4; ++پړاو){
        clock_t پړاو_وخت=clock();
        چاپ_کړه("Round: %d\n", پړاو);
        shuffle(boxes, N, حجم(sortable_bbox));
        لكل(i = 0; i < N/2; ++i){
            bbox_fight(net, boxes+i*2, boxes+i*2+1, classes, -1);
        }
        چاپ_کړه("Round: %f secs, %d remaining\n", sec(clock()-پړاو_وخت), N);
    }

    صحيح class;

    لكل (class = 0; class < classes; ++class){

        N = ټول;
        current_class = class;
        qsort(boxes, N, حجم(sortable_bbox), elo_comparator);
        N /= 2;

        لكل(پړاو = 1; پړاو <= 100; ++پړاو){
            clock_t پړاو_وخت=clock();
            چاپ_کړه("Round: %d\n", پړاو);

            sorta_shuffle(boxes, N, حجم(sortable_bbox), 10);
            لكل(i = 0; i < N/2; ++i){
                bbox_fight(net, boxes+i*2, boxes+i*2+1, classes, class);
            }
            qsort(boxes, N, حجم(sortable_bbox), elo_comparator);
            إذا(پړاو <= 20) N = (N*9/10)/2*2;

            چاپ_کړه("Round: %f secs, %d remaining\n", sec(clock()-پړاو_وخت), N);
        }
        حرف buff[256];
        sprintf(buff, "results/battle_%d.log", class);
        FILE *outfp = fopen(buff, "w");
        لكل(i = 0; i < N; ++i){
            fprintf(outfp, "%s %f\n", boxes[i].filename, boxes[i].elos[class]);
        }
        fclose(outfp);
    }
    چاپ_کړه("Tournament in %d compares, %f secs\n", ټول_compares, sec(clock()-وخت));
}

فراغ پرلپسې_compare(صحيح argc, حرف **argv)
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
    وإلا إذا(0==strcmp(argv[2], "sort")) ترتیبول_Master3000(cfg, weights);
    وإلا إذا(0==strcmp(argv[2], "battle")) BattleRoyaleWithCheese(cfg, weights);
    /*
       else if(0==strcmp(argv[2], "train")) train_coco(cfg, weights);
       else if(0==strcmp(argv[2], "extract")) extract_boxes(cfg, weights);
       else if(0==strcmp(argv[2], "valid")) validate_recall(cfg, weights);
     */
}

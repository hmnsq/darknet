#ضمن <stdio.h>

#ضمن "network.h"
#ضمن "detection_layer.h"
#ضمن "cost_layer.h"
#ضمن "utils.h"
#ضمن "parser.h"
#ضمن "box.h"

فراغ train_compare(حرف *cfgfile, حرف *weightfile)
{
    srand(زمان(0));
    عائم avg_loss = -1;
    حرف *پایه = basecfg(cfgfile);
    حرف *backup_directory = "/home/pjreddie/backup/";
    چاپ_کن("%s\n", پایه);
    شبکه net = parse_شبکه_cfg(cfgfile);
    إذا(weightfile){
        بارگیری_weights(&net, weightfile);
    }
    چاپ_کن("Learning Rate: %g, Momentum: %g, Decay: %g\n", net.learning_نرخ, net.momentum, net.decay);
    صحيح imgs = 1024;
    list *plist = get_paths("data/compare.train.list");
    حرف **paths = (حرف **)list_مقصد_array(plist);
    صحيح N = plist->اندازه;
    چاپ_کن("%d\n", N);
    clock_t زمان;
    pthread_t بارگیری_thread;
    داده train;
    داده میانگیر;

    بارگیری_آرگومانها آرگومانها = {0};
    آرگومانها.w = net.w;
    آرگومانها.h = net.h;
    آرگومانها.paths = paths;
    آرگومانها.classes = 20;
    آرگومانها.n = imgs;
    آرگومانها.m = N;
    آرگومانها.d = &میانگیر;
    آرگومانها.type = COMPARE_داده;

    بارگیری_thread = بارگیری_داده_درون_thread(آرگومانها);
    صحيح epoch = *net.دیده/N;
    صحيح i = 0;
    طالما(1){
        ++i;
        زمان=clock();
        pthread_join(بارگیری_thread, 0);
        train = میانگیر;

        بارگیری_thread = بارگیری_داده_درون_thread(آرگومانها);
        چاپ_کن("Loaded: %lf seconds\n", sec(clock()-زمان));
        زمان=clock();
        عائم loss = train_شبکه(net, train);
        إذا(avg_loss == -1) avg_loss = loss;
        avg_loss = avg_loss*.9 + loss*.1;
        چاپ_کن("%.3f: %f, %f avg, %lf seconds, %ld images\n", (عائم)*net.دیده/N, loss, avg_loss, sec(clock()-زمان), *net.دیده);
        free_داده(train);
        إذا(i%100 == 0){
            حرف buff[256];
            sprintf(buff, "%s/%s_%d_minor_%d.weights",backup_directory,پایه, epoch, i);
            save_weights(net, buff);
        }
        إذا(*net.دیده/N > epoch){
            epoch = *net.دیده/N;
            i = 0;
            حرف buff[256];
            sprintf(buff, "%s/%s_%d.weights",backup_directory,پایه, epoch);
            save_weights(net, buff);
            إذا(epoch%22 == 0) net.learning_نرخ *= .1;
        }
    }
    pthread_join(بارگیری_thread, 0);
    free_داده(میانگیر);
    free_شبکه(net);
    free_ptrs((فراغ**)paths, plist->اندازه);
    free_list(plist);
    حرر(پایه);
}

فراغ validate_compare(حرف *filename, حرف *weightfile)
{
    صحيح i = 0;
    شبکه net = parse_شبکه_cfg(filename);
    إذا(weightfile){
        بارگیری_weights(&net, weightfile);
    }
    srand(زمان(0));

    list *plist = get_paths("data/compare.val.list");
    //list *plist = get_paths("data/compare.val.old");
    حرف **paths = (حرف **)list_مقصد_array(plist);
    صحيح N = plist->اندازه/2;
    free_list(plist);

    clock_t زمان;
    صحيح correct = 0;
    صحيح مجموع = 0;
    صحيح splits = 10;
    صحيح رقم = (i+1)*N/splits - i*N/splits;

    داده val, میانگیر;

    بارگیری_آرگومانها آرگومانها = {0};
    آرگومانها.w = net.w;
    آرگومانها.h = net.h;
    آرگومانها.paths = paths;
    آرگومانها.classes = 20;
    آرگومانها.n = رقم;
    آرگومانها.m = 0;
    آرگومانها.d = &میانگیر;
    آرگومانها.type = COMPARE_داده;

    pthread_t بارگیری_thread = بارگیری_داده_درون_thread(آرگومانها);
    لكل(i = 1; i <= splits; ++i){
        زمان=clock();

        pthread_join(بارگیری_thread, 0);
        val = میانگیر;

        رقم = (i+1)*N/splits - i*N/splits;
        حرف **part = paths+(i*N/splits);
        إذا(i != splits){
            آرگومانها.paths = part;
            بارگیری_thread = بارگیری_داده_درون_thread(آرگومانها);
        }
        چاپ_کن("Loaded: %d images in %lf seconds\n", val.X.ردیفها, sec(clock()-زمان));

        زمان=clock();
        ماتریس pred = شبکه_predict_داده(net, val);
        صحيح j,k;
        لكل(j = 0; j < val.y.ردیفها; ++j){
            لكل(k = 0; k < 20; ++k){
                إذا(val.y.vals[j][k*2] != val.y.vals[j][k*2+1]){
                    ++مجموع;
                    إذا((val.y.vals[j][k*2] < val.y.vals[j][k*2+1]) == (pred.vals[j][k*2] < pred.vals[j][k*2+1])){
                        ++correct;
                    }
                }
            }
        }
        free_ماتریس(pred);
        چاپ_کن("%d: Acc: %f, %lf seconds, %d images\n", i, (عائم)correct/مجموع, sec(clock()-زمان), val.X.ردیفها);
        free_داده(val);
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

صحيح مجموع_compares = 0;
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
    ++مجموع_compares;
    sortable_bbox box1 = *(sortable_bbox*)a;
    sortable_bbox box2 = *(sortable_bbox*)b;
    شبکه net = box1.net;
    صحيح class   = box1.class;

    image im1 = بارگیری_image_color(box1.filename, net.w, net.h);
    image im2 = بارگیری_image_color(box2.filename, net.w, net.h);
    عائم *X  = حافظه_صفر_بده(net.w*net.h*net.c, حجم(عائم));
    کپی_حافظه(X,                   im1.داده, im1.w*im1.h*im1.c*حجم(عائم));
    کپی_حافظه(X+im1.w*im1.h*im1.c, im2.داده, im2.w*im2.h*im2.c*حجم(عائم));
    عائم *predictions = شبکه_predict(net, X);
    
    free_image(im1);
    free_image(im2);
    حرر(X);
    إذا (predictions[class*2] > predictions[class*2+1]){
        أرجع 1;
    }
    أرجع -1;
}

فراغ bbox_update(sortable_bbox *a, sortable_bbox *b, صحيح class, صحيح نتیجه)
{
    صحيح k = 32;
    عائم EA = 1./(1+توان(10, (b->elos[class] - a->elos[class])/400.));
    عائم EB = 1./(1+توان(10, (a->elos[class] - b->elos[class])/400.));
    عائم SA = نتیجه ? 1 : 0;
    عائم SB = نتیجه ? 0 : 1;
    a->elos[class] += k*(SA - EA);
    b->elos[class] += k*(SB - EB);
}

فراغ bbox_fight(شبکه net, sortable_bbox *a, sortable_bbox *b, صحيح classes, صحيح class)
{
    image im1 = بارگیری_image_color(a->filename, net.w, net.h);
    image im2 = بارگیری_image_color(b->filename, net.w, net.h);
    عائم *X  = حافظه_صفر_بده(net.w*net.h*net.c, حجم(عائم));
    کپی_حافظه(X,                   im1.داده, im1.w*im1.h*im1.c*حجم(عائم));
    کپی_حافظه(X+im1.w*im1.h*im1.c, im2.داده, im2.w*im2.h*im2.c*حجم(عائم));
    عائم *predictions = شبکه_predict(net, X);
    ++مجموع_compares;

    صحيح i;
    لكل(i = 0; i < classes; ++i){
        إذا(class < 0 || class == i){
            صحيح نتیجه = predictions[i*2] > predictions[i*2+1];
            bbox_update(a, b, i, نتیجه);
        }
    }
    
    free_image(im1);
    free_image(im2);
    حرر(X);
}

فراغ مرتب_سازی_Master3000(حرف *filename, حرف *weightfile)
{
    صحيح i = 0;
    شبکه net = parse_شبکه_cfg(filename);
    إذا(weightfile){
        بارگیری_weights(&net, weightfile);
    }
    srand(زمان(0));
    مجموعه_batch_شبکه(&net, 1);

    list *plist = get_paths("data/compare.sort.list");
    //list *plist = get_paths("data/compare.val.old");
    حرف **paths = (حرف **)list_مقصد_array(plist);
    صحيح N = plist->اندازه;
    free_list(plist);
    sortable_bbox *boxes = حافظه_صفر_بده(N, حجم(sortable_bbox));
    چاپ_کن("Sorting %d boxes...\n", N);
    لكل(i = 0; i < N; ++i){
        boxes[i].filename = paths[i];
        boxes[i].net = net;
        boxes[i].class = 7;
        boxes[i].elo = 1500;
    }
    clock_t زمان=clock();
    qsort(boxes, N, حجم(sortable_bbox), bbox_comparator);
    لكل(i = 0; i < N; ++i){
        چاپ_کن("%s\n", boxes[i].filename);
    }
    چاپ_کن("Sorted in %d compares, %f secs\n", مجموع_compares, sec(clock()-زمان));
}

فراغ BattleRoyaleWithCheese(حرف *filename, حرف *weightfile)
{
    صحيح classes = 20;
    صحيح i,j;
    شبکه net = parse_شبکه_cfg(filename);
    إذا(weightfile){
        بارگیری_weights(&net, weightfile);
    }
    srand(زمان(0));
    مجموعه_batch_شبکه(&net, 1);

    list *plist = get_paths("data/compare.sort.list");
    //list *plist = get_paths("data/compare.small.list");
    //list *plist = get_paths("data/compare.cat.list");
    //list *plist = get_paths("data/compare.val.old");
    حرف **paths = (حرف **)list_مقصد_array(plist);
    صحيح N = plist->اندازه;
    صحيح مجموع = N;
    free_list(plist);
    sortable_bbox *boxes = حافظه_صفر_بده(N, حجم(sortable_bbox));
    چاپ_کن("Battling %d boxes...\n", N);
    لكل(i = 0; i < N; ++i){
        boxes[i].filename = paths[i];
        boxes[i].net = net;
        boxes[i].classes = classes;
        boxes[i].elos = حافظه_صفر_بده(classes, حجم(عائم));;
        لكل(j = 0; j < classes; ++j){
            boxes[i].elos[j] = 1500;
        }
    }
    صحيح دور;
    clock_t زمان=clock();
    لكل(دور = 1; دور <= 4; ++دور){
        clock_t دور_زمان=clock();
        چاپ_کن("Round: %d\n", دور);
        shuffle(boxes, N, حجم(sortable_bbox));
        لكل(i = 0; i < N/2; ++i){
            bbox_fight(net, boxes+i*2, boxes+i*2+1, classes, -1);
        }
        چاپ_کن("Round: %f secs, %d remaining\n", sec(clock()-دور_زمان), N);
    }

    صحيح class;

    لكل (class = 0; class < classes; ++class){

        N = مجموع;
        current_class = class;
        qsort(boxes, N, حجم(sortable_bbox), elo_comparator);
        N /= 2;

        لكل(دور = 1; دور <= 100; ++دور){
            clock_t دور_زمان=clock();
            چاپ_کن("Round: %d\n", دور);

            sorta_shuffle(boxes, N, حجم(sortable_bbox), 10);
            لكل(i = 0; i < N/2; ++i){
                bbox_fight(net, boxes+i*2, boxes+i*2+1, classes, class);
            }
            qsort(boxes, N, حجم(sortable_bbox), elo_comparator);
            إذا(دور <= 20) N = (N*9/10)/2*2;

            چاپ_کن("Round: %f secs, %d remaining\n", sec(clock()-دور_زمان), N);
        }
        حرف buff[256];
        sprintf(buff, "results/battle_%d.log", class);
        FILE *outfp = fopen(buff, "w");
        لكل(i = 0; i < N; ++i){
            fprintf(outfp, "%s %f\n", boxes[i].filename, boxes[i].elos[class]);
        }
        fclose(outfp);
    }
    چاپ_کن("Tournament in %d compares, %f secs\n", مجموع_compares, sec(clock()-زمان));
}

فراغ پیاپی_compare(صحيح argc, حرف **argv)
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
    وإلا إذا(0==strcmp(argv[2], "sort")) مرتب_سازی_Master3000(cfg, weights);
    وإلا إذا(0==strcmp(argv[2], "battle")) BattleRoyaleWithCheese(cfg, weights);
    /*
       else if(0==strcmp(argv[2], "train")) train_coco(cfg, weights);
       else if(0==strcmp(argv[2], "extract")) extract_boxes(cfg, weights);
       else if(0==strcmp(argv[2], "valid")) validate_recall(cfg, weights);
     */
}

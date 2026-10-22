#समावेश <stdio.h>

#समावेश "network.h"
#समावेश "detection_layer.h"
#समावेश "cost_layer.h"
#समावेश "utils.h"
#समावेश "parser.h"
#समावेश "box.h"

व्योम train_compare(अक्षर *cfgfile, अक्षर *weightfile)
{
    बेक्रम(समय(0));
    भग्न avg_loss = -1;
    अक्षर *base = basecfg(cfgfile);
    अक्षर *backup_directory = "/home/pjreddie/backup/";
    म_लिखो("%s\n", base);
    network net = parse_network_cfg(cfgfile);
    अगर(weightfile){
        load_weights(&net, weightfile);
    }
    म_लिखो("Learning Rate: %g, Momentum: %g, Decay: %g\n", net.learning_rate, net.momentum, net.decay);
    पूर्णांक imgs = 1024;
    list *plist = get_paths("data/compare.train.list");
    अक्षर **paths = (अक्षर **)list_to_array(plist);
    पूर्णांक N = plist->आकार;
    म_लिखो("%d\n", N);
    घड़ी_प्रकार समय;
    pthread_t load_thread;
    तथ्य train;
    तथ्य बफर;

    load_args args = {0};
    args.w = net.w;
    args.h = net.h;
    args.paths = paths;
    args.classes = 20;
    args.n = imgs;
    args.m = N;
    args.d = &बफर;
    args.type = COMPARE_तथ्य;

    load_thread = load_तथ्य_in_thread(args);
    पूर्णांक epoch = *net.seen/N;
    पूर्णांक i = 0;
    जबतक(1){
        ++i;
        समय=घड़ी();
        pthread_join(load_thread, 0);
        train = बफर;

        load_thread = load_तथ्य_in_thread(args);
        म_लिखो("Loaded: %lf seconds\n", sec(घड़ी()-समय));
        समय=घड़ी();
        भग्न loss = train_network(net, train);
        अगर(avg_loss == -1) avg_loss = loss;
        avg_loss = avg_loss*.9 + loss*.1;
        म_लिखो("%.3f: %f, %f avg, %lf seconds, %ld images\n", (भग्न)*net.seen/N, loss, avg_loss, sec(घड़ी()-समय), *net.seen);
        free_तथ्य(train);
        अगर(i%100 == 0){
            अक्षर buff[256];
            प्र_लिखो(buff, "%s/%s_%d_minor_%d.weights",backup_directory,base, epoch, i);
            save_weights(net, buff);
        }
        अगर(*net.seen/N > epoch){
            epoch = *net.seen/N;
            i = 0;
            अक्षर buff[256];
            प्र_लिखो(buff, "%s/%s_%d.weights",backup_directory,base, epoch);
            save_weights(net, buff);
            अगर(epoch%22 == 0) net.learning_rate *= .1;
        }
    }
    pthread_join(load_thread, 0);
    free_तथ्य(बफर);
    free_network(net);
    free_ptrs((व्योम**)paths, plist->आकार);
    free_list(plist);
    मुक्त(base);
}

व्योम validate_compare(अक्षर *filename, अक्षर *weightfile)
{
    पूर्णांक i = 0;
    network net = parse_network_cfg(filename);
    अगर(weightfile){
        load_weights(&net, weightfile);
    }
    बेक्रम(समय(0));

    list *plist = get_paths("data/compare.val.list");
    //list *plist = get_paths("data/compare.val.old");
    अक्षर **paths = (अक्षर **)list_to_array(plist);
    पूर्णांक N = plist->आकार/2;
    free_list(plist);

    घड़ी_प्रकार समय;
    पूर्णांक correct = 0;
    पूर्णांक योग_2 = 0;
    पूर्णांक splits = 10;
    पूर्णांक अंक = (i+1)*N/splits - i*N/splits;

    तथ्य val, बफर;

    load_args args = {0};
    args.w = net.w;
    args.h = net.h;
    args.paths = paths;
    args.classes = 20;
    args.n = अंक;
    args.m = 0;
    args.d = &बफर;
    args.type = COMPARE_तथ्य;

    pthread_t load_thread = load_तथ्य_in_thread(args);
    क्रम(i = 1; i <= splits; ++i){
        समय=घड़ी();

        pthread_join(load_thread, 0);
        val = बफर;

        अंक = (i+1)*N/splits - i*N/splits;
        अक्षर **part = paths+(i*N/splits);
        अगर(i != splits){
            args.paths = part;
            load_thread = load_तथ्य_in_thread(args);
        }
        म_लिखो("Loaded: %d images in %lf seconds\n", val.X.rows, sec(घड़ी()-समय));

        समय=घड़ी();
        matrix pred = network_predict_तथ्य(net, val);
        पूर्णांक j,k;
        क्रम(j = 0; j < val.y.rows; ++j){
            क्रम(k = 0; k < 20; ++k){
                अगर(val.y.vals[j][k*2] != val.y.vals[j][k*2+1]){
                    ++योग_2;
                    अगर((val.y.vals[j][k*2] < val.y.vals[j][k*2+1]) == (pred.vals[j][k*2] < pred.vals[j][k*2+1])){
                        ++correct;
                    }
                }
            }
        }
        free_matrix(pred);
        म_लिखो("%d: Acc: %f, %lf seconds, %d images\n", i, (भग्न)correct/योग_2, sec(घड़ी()-समय), val.X.rows);
        free_तथ्य(val);
    }
}

प्रकार काष्ठा {
    network net;
    अक्षर *filename;
    पूर्णांक class;
    पूर्णांक classes;
    भग्न elo;
    भग्न *elos;
} sortable_bbox;

पूर्णांक योग_compares = 0;
पूर्णांक current_class = 0;

पूर्णांक elo_comparator(स्थिर व्योम*a, स्थिर व्योम *b)
{
    sortable_bbox box1 = *(sortable_bbox*)a;
    sortable_bbox box2 = *(sortable_bbox*)b;
    अगर(box1.elos[current_class] == box2.elos[current_class]) वापस 0;
    अगर(box1.elos[current_class] >  box2.elos[current_class]) वापस -1;
    वापस 1;
}

पूर्णांक bbox_comparator(स्थिर व्योम *a, स्थिर व्योम *b)
{
    ++योग_compares;
    sortable_bbox box1 = *(sortable_bbox*)a;
    sortable_bbox box2 = *(sortable_bbox*)b;
    network net = box1.net;
    पूर्णांक class   = box1.class;

    image im1 = load_image_color(box1.filename, net.w, net.h);
    image im2 = load_image_color(box2.filename, net.w, net.h);
    भग्न *X  = सुस्मृति(net.w*net.h*net.c, माप(भग्न));
    स_नकल(X,                   im1.तथ्य, im1.w*im1.h*im1.c*माप(भग्न));
    स_नकल(X+im1.w*im1.h*im1.c, im2.तथ्य, im2.w*im2.h*im2.c*माप(भग्न));
    भग्न *predictions = network_predict(net, X);
    
    free_image(im1);
    free_image(im2);
    मुक्त(X);
    अगर (predictions[class*2] > predictions[class*2+1]){
        वापस 1;
    }
    वापस -1;
}

व्योम bbox_update(sortable_bbox *a, sortable_bbox *b, पूर्णांक class, पूर्णांक परिणाम)
{
    पूर्णांक k = 32;
    भग्न EA = 1./(1+घात(10, (b->elos[class] - a->elos[class])/400.));
    भग्न EB = 1./(1+घात(10, (a->elos[class] - b->elos[class])/400.));
    भग्न SA = परिणाम ? 1 : 0;
    भग्न SB = परिणाम ? 0 : 1;
    a->elos[class] += k*(SA - EA);
    b->elos[class] += k*(SB - EB);
}

व्योम bbox_fight(network net, sortable_bbox *a, sortable_bbox *b, पूर्णांक classes, पूर्णांक class)
{
    image im1 = load_image_color(a->filename, net.w, net.h);
    image im2 = load_image_color(b->filename, net.w, net.h);
    भग्न *X  = सुस्मृति(net.w*net.h*net.c, माप(भग्न));
    स_नकल(X,                   im1.तथ्य, im1.w*im1.h*im1.c*माप(भग्न));
    स_नकल(X+im1.w*im1.h*im1.c, im2.तथ्य, im2.w*im2.h*im2.c*माप(भग्न));
    भग्न *predictions = network_predict(net, X);
    ++योग_compares;

    पूर्णांक i;
    क्रम(i = 0; i < classes; ++i){
        अगर(class < 0 || class == i){
            पूर्णांक परिणाम = predictions[i*2] > predictions[i*2+1];
            bbox_update(a, b, i, परिणाम);
        }
    }
    
    free_image(im1);
    free_image(im2);
    मुक्त(X);
}

व्योम SortMaster3000(अक्षर *filename, अक्षर *weightfile)
{
    पूर्णांक i = 0;
    network net = parse_network_cfg(filename);
    अगर(weightfile){
        load_weights(&net, weightfile);
    }
    बेक्रम(समय(0));
    set_batch_network(&net, 1);

    list *plist = get_paths("data/compare.sort.list");
    //list *plist = get_paths("data/compare.val.old");
    अक्षर **paths = (अक्षर **)list_to_array(plist);
    पूर्णांक N = plist->आकार;
    free_list(plist);
    sortable_bbox *boxes = सुस्मृति(N, माप(sortable_bbox));
    म_लिखो("Sorting %d boxes...\n", N);
    क्रम(i = 0; i < N; ++i){
        boxes[i].filename = paths[i];
        boxes[i].net = net;
        boxes[i].class = 7;
        boxes[i].elo = 1500;
    }
    घड़ी_प्रकार समय=घड़ी();
    क्विक(boxes, N, माप(sortable_bbox), bbox_comparator);
    क्रम(i = 0; i < N; ++i){
        म_लिखो("%s\n", boxes[i].filename);
    }
    म_लिखो("Sorted in %d compares, %f secs\n", योग_compares, sec(घड़ी()-समय));
}

व्योम BattleRoyaleWithCheese(अक्षर *filename, अक्षर *weightfile)
{
    पूर्णांक classes = 20;
    पूर्णांक i,j;
    network net = parse_network_cfg(filename);
    अगर(weightfile){
        load_weights(&net, weightfile);
    }
    बेक्रम(समय(0));
    set_batch_network(&net, 1);

    list *plist = get_paths("data/compare.sort.list");
    //list *plist = get_paths("data/compare.small.list");
    //list *plist = get_paths("data/compare.cat.list");
    //list *plist = get_paths("data/compare.val.old");
    अक्षर **paths = (अक्षर **)list_to_array(plist);
    पूर्णांक N = plist->आकार;
    पूर्णांक योग_2 = N;
    free_list(plist);
    sortable_bbox *boxes = सुस्मृति(N, माप(sortable_bbox));
    म_लिखो("Battling %d boxes...\n", N);
    क्रम(i = 0; i < N; ++i){
        boxes[i].filename = paths[i];
        boxes[i].net = net;
        boxes[i].classes = classes;
        boxes[i].elos = सुस्मृति(classes, माप(भग्न));;
        क्रम(j = 0; j < classes; ++j){
            boxes[i].elos[j] = 1500;
        }
    }
    पूर्णांक round;
    घड़ी_प्रकार समय=घड़ी();
    क्रम(round = 1; round <= 4; ++round){
        घड़ी_प्रकार round_time=घड़ी();
        म_लिखो("Round: %d\n", round);
        shuffle(boxes, N, माप(sortable_bbox));
        क्रम(i = 0; i < N/2; ++i){
            bbox_fight(net, boxes+i*2, boxes+i*2+1, classes, -1);
        }
        म_लिखो("Round: %f secs, %d remaining\n", sec(घड़ी()-round_time), N);
    }

    पूर्णांक class;

    क्रम (class = 0; class < classes; ++class){

        N = योग_2;
        current_class = class;
        क्विक(boxes, N, माप(sortable_bbox), elo_comparator);
        N /= 2;

        क्रम(round = 1; round <= 100; ++round){
            घड़ी_प्रकार round_time=घड़ी();
            म_लिखो("Round: %d\n", round);

            sorta_shuffle(boxes, N, माप(sortable_bbox), 10);
            क्रम(i = 0; i < N/2; ++i){
                bbox_fight(net, boxes+i*2, boxes+i*2+1, classes, class);
            }
            क्विक(boxes, N, माप(sortable_bbox), elo_comparator);
            अगर(round <= 20) N = (N*9/10)/2*2;

            म_लिखो("Round: %f secs, %d remaining\n", sec(घड़ी()-round_time), N);
        }
        अक्षर buff[256];
        प्र_लिखो(buff, "results/battle_%d.log", class);
        खाता *outfp = ख_खोलो(buff, "w");
        क्रम(i = 0; i < N; ++i){
            ख_लिखो(outfp, "%s %f\n", boxes[i].filename, boxes[i].elos[class]);
        }
        ख_बंद(outfp);
    }
    म_लिखो("Tournament in %d compares, %f secs\n", योग_compares, sec(घड़ी()-समय));
}

व्योम run_compare(पूर्णांक argc, अक्षर **argv)
{
    अगर(argc < 4){
        ख_लिखो(मानक_त्रुटि, "usage: %s %s [train/test/valid] [cfg] [weights (optional)]\n", argv[0], argv[1]);
        वापस;
    }

    अक्षर *cfg = argv[3];
    अक्षर *weights = (argc > 4) ? argv[4] : 0;
    //char *filename = (argc > 5) ? argv[5]: 0;
    अगर(0==म_भेद(argv[2], "train")) train_compare(cfg, weights);
    अन्यथा अगर(0==म_भेद(argv[2], "valid")) validate_compare(cfg, weights);
    अन्यथा अगर(0==म_भेद(argv[2], "sort")) SortMaster3000(cfg, weights);
    अन्यथा अगर(0==म_भेद(argv[2], "battle")) BattleRoyaleWithCheese(cfg, weights);
    /*
       else if(0==strcmp(argv[2], "train")) train_coco(cfg, weights);
       else if(0==strcmp(argv[2], "extract")) extract_boxes(cfg, weights);
       else if(0==strcmp(argv[2], "valid")) validate_recall(cfg, weights);
     */
}

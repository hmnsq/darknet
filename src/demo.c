#شامل "network.h"
#شامل "detection_layer.h"
#شامل "region_layer.h"
#شامل "cost_layer.h"
#شامل "utils.h"
#شامل "parser.h"
#شامل "box.h"
#شامل "image.h"
#شامل "demo.h"
#شامل <sys/time.h>

#define DEMO 1

#ifdef OPENCV

ایستا نویسه **demo_names;
ایستا image **demo_alphabet;
ایستا صحیح demo_classes;

ایستا network *net;
ایستا image buff [3];
ایستا image buff_letter[3];
ایستا صحیح buff_index = 0;
ایستا تهی * cap;
ایستا اعشاری fps = 0;
ایستا اعشاری demo_thresh = 0;
ایستا اعشاری demo_hier = .5;
ایستا صحیح running = 0;

ایستا صحیح demo_frame = 3;
ایستا صحیح demo_index = 0;
ایستا اعشاری **predictions;
ایستا اعشاری *avg;
ایستا صحیح demo_done = 0;
ایستا صحیح demo_total = 0;
دوگانه demo_time;

detection *get_network_boxes(network *net, صحیح w, صحیح h, اعشاری thresh, اعشاری hier, صحیح *map, صحیح relative, صحیح *رقم);

صحیح size_network(network *net)
{
    صحیح i;
    صحیح شمار = 0;
    برای(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        اگر(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            شمار += l.outputs;
        }
    }
    بازگشت شمار;
}

تهی remember_network(network *net)
{
    صحیح i;
    صحیح شمار = 0;
    برای(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        اگر(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            memcpy(predictions[demo_index] + شمار, net->layers[i].output, اندازه(اعشاری) * l.outputs);
            شمار += l.outputs;
        }
    }
}

detection *avg_predictions(network *net, صحیح *nboxes)
{
    صحیح i, j;
    صحیح شمار = 0;
    fill_cpu(demo_total, 0, avg, 1);
    برای(j = 0; j < demo_frame; ++j){
        axpy_cpu(demo_total, 1./demo_frame, predictions[j], 1, avg, 1);
    }
    برای(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        اگر(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            memcpy(l.output, avg + شمار, اندازه(اعشاری) * l.outputs);
            شمار += l.outputs;
        }
    }
    detection *dets = get_network_boxes(net, buff[0].w, buff[0].h, demo_thresh, demo_hier, 0, 1, nboxes);
    بازگشت dets;
}

تهی *detect_in_thread(تهی *ptr)
{
    running = 1;
    اعشاری nms = .4;

    layer l = net->layers[net->n-1];
    اعشاری *X = buff_letter[(buff_index+2)%3].data;
    network_predict(net, X);

    /*
       if(l.type == DETECTION){
       get_detection_boxes(l, 1, 1, demo_thresh, probs, boxes, 0);
       } else */
    remember_network(net);
    detection *dets = 0;
    صحیح nboxes = 0;
    dets = avg_predictions(net, &nboxes);


    /*
       int i,j;
       box zero = {0};
       int classes = l.classes;
       for(i = 0; i < demo_detections; ++i){
       avg[i].objectness = 0;
       avg[i].bbox = zero;
       memset(avg[i].prob, 0, classes*sizeof(float));
       for(j = 0; j < demo_frame; ++j){
       axpy_cpu(classes, 1./demo_frame, dets[j][i].prob, 1, avg[i].prob, 1);
       avg[i].objectness += dets[j][i].objectness * 1./demo_frame;
       avg[i].bbox.x += dets[j][i].bbox.x * 1./demo_frame;
       avg[i].bbox.y += dets[j][i].bbox.y * 1./demo_frame;
       avg[i].bbox.w += dets[j][i].bbox.w * 1./demo_frame;
       avg[i].bbox.h += dets[j][i].bbox.h * 1./demo_frame;
       }
    //copy_cpu(classes, dets[0][i].prob, 1, avg[i].prob, 1);
    //avg[i].objectness = dets[0][i].objectness;
    }
     */

    اگر (nms > 0) do_nms_obj(dets, nboxes, l.classes, nms);

    printf("\033[2J");
    printf("\033[1;1H");
    printf("\nFPS:%.1f\n",fps);
    printf("Objects:\n\n");
    image display = buff[(buff_index+2) % 3];
    draw_detections(display, dets, nboxes, demo_thresh, demo_names, demo_alphabet, demo_classes);
    free_detections(dets, nboxes);

    demo_index = (demo_index + 1)%demo_frame;
    running = 0;
    بازگشت 0;
}

تهی *fetch_in_thread(تهی *ptr)
{
    free_image(buff[buff_index]);
    buff[buff_index] = get_image_from_stream(cap);
    اگر(buff[buff_index].data == 0) {
        demo_done = 1;
        بازگشت 0;
    }
    letterbox_image_into(buff[buff_index], net->w, net->h, buff_letter[buff_index]);
    بازگشت 0;
}

تهی *display_in_thread(تهی *ptr)
{
    صحیح c = show_image(buff[(buff_index + 1)%3], "Demo", 1);
    اگر (c != -1) c = c%256;
    اگر (c == 27) {
        demo_done = 1;
        بازگشت 0;
    } وگرنه اگر (c == 82) {
        demo_thresh += .02;
    } وگرنه اگر (c == 84) {
        demo_thresh -= .02;
        اگر(demo_thresh <= .02) demo_thresh = .02;
    } وگرنه اگر (c == 83) {
        demo_hier += .02;
    } وگرنه اگر (c == 81) {
        demo_hier -= .02;
        اگر(demo_hier <= .0) demo_hier = .0;
    }
    بازگشت 0;
}

تهی *display_loop(تهی *ptr)
{
    وقتی(1){
        display_in_thread(0);
    }
}

تهی *detect_loop(تهی *ptr)
{
    وقتی(1){
        detect_in_thread(0);
    }
}

تهی demo(نویسه *cfgfile, نویسه *weightfile, اعشاری thresh, صحیح cam_index, ثابت نویسه *filename, نویسه **names, صحیح classes, صحیح delay, نویسه *prefix, صحیح avg_frames, اعشاری hier, صحیح w, صحیح h, صحیح frames, صحیح fullscreen)
{
    //demo_frame = avg_frames;
    image **alphabet = load_alphabet();
    demo_names = names;
    demo_alphabet = alphabet;
    demo_classes = classes;
    demo_thresh = thresh;
    demo_hier = hier;
    printf("Demo\n");
    net = load_network(cfgfile, weightfile, 0);
    set_batch_network(net, 1);
    pthread_t detect_thread;
    pthread_t fetch_thread;

    srand(2222222);

    صحیح i;
    demo_total = size_network(net);
    predictions = calloc(demo_frame, اندازه(اعشاری*));
    برای (i = 0; i < demo_frame; ++i){
        predictions[i] = calloc(demo_total, اندازه(اعشاری));
    }
    avg = calloc(demo_total, اندازه(اعشاری));

    اگر(filename){
        printf("video file: %s\n", filename);
        cap = open_video_stream(filename, 0, 0, 0, 0);
    }وگرنه{
        cap = open_video_stream(0, cam_index, w, h, frames);
    }

    اگر(!cap) error("Couldn't connect to webcam.\n");

    buff[0] = get_image_from_stream(cap);
    buff[1] = copy_image(buff[0]);
    buff[2] = copy_image(buff[0]);
    buff_letter[0] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[1] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[2] = letterbox_image(buff[0], net->w, net->h);

    صحیح شمار = 0;
    اگر(!prefix){
        make_window("Demo", 1352, 1013, fullscreen);
    }

    demo_time = what_time_is_it_now();

    وقتی(!demo_done){
        buff_index = (buff_index + 1) %3;
        اگر(pthread_create(&fetch_thread, 0, fetch_in_thread, 0)) error("Thread creation failed");
        اگر(pthread_create(&detect_thread, 0, detect_in_thread, 0)) error("Thread creation failed");
        اگر(!prefix){
            fps = 1./(what_time_is_it_now() - demo_time);
            demo_time = what_time_is_it_now();
            display_in_thread(0);
        }وگرنه{
            نویسه name[256];
            sprintf(name, "%s_%08d", prefix, شمار);
            save_image(buff[(buff_index + 1)%3], name);
        }
        pthread_join(fetch_thread, 0);
        pthread_join(detect_thread, 0);
        ++شمار;
    }
}

/*
   void demo_compare(char *cfg1, char *weight1, char *cfg2, char *weight2, float thresh, int cam_index, const char *filename, char **names, int classes, int delay, char *prefix, int avg_frames, float hier, int w, int h, int frames, int fullscreen)
   {
   demo_frame = avg_frames;
   predictions = calloc(demo_frame, sizeof(float*));
   image **alphabet = load_alphabet();
   demo_names = names;
   demo_alphabet = alphabet;
   demo_classes = classes;
   demo_thresh = thresh;
   demo_hier = hier;
   printf("Demo\n");
   net = load_network(cfg1, weight1, 0);
   set_batch_network(net, 1);
   pthread_t detect_thread;
   pthread_t fetch_thread;

   srand(2222222);

   if(filename){
   printf("video file: %s\n", filename);
   cap = cvCaptureFromFile(filename);
   }else{
   cap = cvCaptureFromCAM(cam_index);

   if(w){
   cvSetCaptureProperty(cap, CV_CAP_PROP_FRAME_WIDTH, w);
   }
   if(h){
   cvSetCaptureProperty(cap, CV_CAP_PROP_FRAME_HEIGHT, h);
   }
   if(frames){
   cvSetCaptureProperty(cap, CV_CAP_PROP_FPS, frames);
   }
   }

   if(!cap) error("Couldn't connect to webcam.\n");

   layer l = net->layers[net->n-1];
   demo_detections = l.n*l.w*l.h;
   int j;

   avg = (float *) calloc(l.outputs, sizeof(float));
   for(j = 0; j < demo_frame; ++j) predictions[j] = (float *) calloc(l.outputs, sizeof(float));

   boxes = (box *)calloc(l.w*l.h*l.n, sizeof(box));
   probs = (float **)calloc(l.w*l.h*l.n, sizeof(float *));
   for(j = 0; j < l.w*l.h*l.n; ++j) probs[j] = (float *)calloc(l.classes+1, sizeof(float));

   buff[0] = get_image_from_stream(cap);
   buff[1] = copy_image(buff[0]);
   buff[2] = copy_image(buff[0]);
   buff_letter[0] = letterbox_image(buff[0], net->w, net->h);
   buff_letter[1] = letterbox_image(buff[0], net->w, net->h);
   buff_letter[2] = letterbox_image(buff[0], net->w, net->h);
   ipl = cvCreateImage(cvSize(buff[0].w,buff[0].h), IPL_DEPTH_8U, buff[0].c);

   int count = 0;
   if(!prefix){
   cvNamedWindow("Demo", CV_WINDOW_NORMAL); 
   if(fullscreen){
   cvSetWindowProperty("Demo", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
   } else {
   cvMoveWindow("Demo", 0, 0);
   cvResizeWindow("Demo", 1352, 1013);
   }
   }

   demo_time = what_time_is_it_now();

   while(!demo_done){
buff_index = (buff_index + 1) %3;
if(pthread_create(&fetch_thread, 0, fetch_in_thread, 0)) error("Thread creation failed");
if(pthread_create(&detect_thread, 0, detect_in_thread, 0)) error("Thread creation failed");
if(!prefix){
    fps = 1./(what_time_is_it_now() - demo_time);
    demo_time = what_time_is_it_now();
    display_in_thread(0);
}else{
    char name[256];
    sprintf(name, "%s_%08d", prefix, count);
    save_image(buff[(buff_index + 1)%3], name);
}
pthread_join(fetch_thread, 0);
pthread_join(detect_thread, 0);
++count;
}
}
*/
#وگرنه
تهی demo(نویسه *cfgfile, نویسه *weightfile, اعشاری thresh, صحیح cam_index, ثابت نویسه *filename, نویسه **names, صحیح classes, صحیح delay, نویسه *prefix, صحیح avg, اعشاری hier, صحیح w, صحیح h, صحیح frames, صحیح fullscreen)
{
    fprintf(stderr, "Demo needs OpenCV for webcam images.\n");
}
#endif


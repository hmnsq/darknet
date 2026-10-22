#অন্তর্ভাৱ "network.h"
#অন্তর্ভাৱ "detection_layer.h"
#অন্তর্ভাৱ "region_layer.h"
#অন্তর্ভাৱ "cost_layer.h"
#অন্তর্ভাৱ "utils.h"
#অন্তর্ভাৱ "parser.h"
#অন্তর্ভাৱ "box.h"
#অন্তর্ভাৱ "image.h"
#অন্তর্ভাৱ "demo.h"
#অন্তর্ভাৱ <sys/time.h>

#define DEMO 1

#ifdef OPENCV

স্থির ৱর্ণ **demo_names;
স্থির image **demo_alphabet;
স্থির পূর্ণাঙ্ক demo_classes;

স্থির network *net;
স্থির image buff [3];
স্থির image buff_letter[3];
স্থির পূর্ণাঙ্ক buff_index = 0;
স্থির শূন্য * cap;
স্থির ভিন্ন fps = 0;
স্থির ভিন্ন demo_thresh = 0;
স্থির ভিন্ন demo_hier = .5;
স্থির পূর্ণাঙ্ক running = 0;

স্থির পূর্ণাঙ্ক demo_frame = 3;
স্থির পূর্ণাঙ্ক demo_index = 0;
স্থির ভিন্ন **predictions;
স্থির ভিন্ন *avg;
স্থির পূর্ণাঙ্ক demo_done = 0;
স্থির পূর্ণাঙ্ক demo_total = 0;
দ্ৱিগুণ demo_time;

detection *get_network_boxes(network *net, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, ভিন্ন thresh, ভিন্ন hier, পূর্ণাঙ্ক *map, পূর্ণাঙ্ক relative, পূর্ণাঙ্ক *অঙ্ক);

পূর্ণাঙ্ক size_network(network *net)
{
    পূর্ণাঙ্ক i;
    পূর্ণাঙ্ক গুনতি = 0;
    বাবে(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        যদি(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            গুনতি += l.outputs;
        }
    }
    ঘূৰাই গুনতি;
}

শূন্য remember_network(network *net)
{
    পূর্ণাঙ্ক i;
    পূর্ণাঙ্ক গুনতি = 0;
    বাবে(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        যদি(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            memcpy(predictions[demo_index] + গুনতি, net->layers[i].output, পরিমাণ(ভিন্ন) * l.outputs);
            গুনতি += l.outputs;
        }
    }
}

detection *avg_predictions(network *net, পূর্ণাঙ্ক *nboxes)
{
    পূর্ণাঙ্ক i, j;
    পূর্ণাঙ্ক গুনতি = 0;
    fill_cpu(demo_total, 0, avg, 1);
    বাবে(j = 0; j < demo_frame; ++j){
        axpy_cpu(demo_total, 1./demo_frame, predictions[j], 1, avg, 1);
    }
    বাবে(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        যদি(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            memcpy(l.output, avg + গুনতি, পরিমাণ(ভিন্ন) * l.outputs);
            গুনতি += l.outputs;
        }
    }
    detection *dets = get_network_boxes(net, buff[0].w, buff[0].h, demo_thresh, demo_hier, 0, 1, nboxes);
    ঘূৰাই dets;
}

শূন্য *detect_in_thread(শূন্য *ptr)
{
    running = 1;
    ভিন্ন nms = .4;

    layer l = net->layers[net->n-1];
    ভিন্ন *X = buff_letter[(buff_index+2)%3].data;
    network_predict(net, X);

    /*
       if(l.type == DETECTION){
       get_detection_boxes(l, 1, 1, demo_thresh, probs, boxes, 0);
       } else */
    remember_network(net);
    detection *dets = 0;
    পূর্ণাঙ্ক nboxes = 0;
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

    যদি (nms > 0) do_nms_obj(dets, nboxes, l.classes, nms);

    printf("\033[2J");
    printf("\033[1;1H");
    printf("\nFPS:%.1f\n",fps);
    printf("Objects:\n\n");
    image display = buff[(buff_index+2) % 3];
    draw_detections(display, dets, nboxes, demo_thresh, demo_names, demo_alphabet, demo_classes);
    free_detections(dets, nboxes);

    demo_index = (demo_index + 1)%demo_frame;
    running = 0;
    ঘূৰাই 0;
}

শূন্য *fetch_in_thread(শূন্য *ptr)
{
    free_image(buff[buff_index]);
    buff[buff_index] = get_image_from_stream(cap);
    যদি(buff[buff_index].data == 0) {
        demo_done = 1;
        ঘূৰাই 0;
    }
    letterbox_image_into(buff[buff_index], net->w, net->h, buff_letter[buff_index]);
    ঘূৰাই 0;
}

শূন্য *display_in_thread(শূন্য *ptr)
{
    পূর্ণাঙ্ক c = show_image(buff[(buff_index + 1)%3], "Demo", 1);
    যদি (c != -1) c = c%256;
    যদি (c == 27) {
        demo_done = 1;
        ঘূৰাই 0;
    } অন্যথা যদি (c == 82) {
        demo_thresh += .02;
    } অন্যথা যদি (c == 84) {
        demo_thresh -= .02;
        যদি(demo_thresh <= .02) demo_thresh = .02;
    } অন্যথা যদি (c == 83) {
        demo_hier += .02;
    } অন্যথা যদি (c == 81) {
        demo_hier -= .02;
        যদি(demo_hier <= .0) demo_hier = .0;
    }
    ঘূৰাই 0;
}

শূন্য *display_loop(শূন্য *ptr)
{
    যেতিয়ালৈ(1){
        display_in_thread(0);
    }
}

শূন্য *detect_loop(শূন্য *ptr)
{
    যেতিয়ালৈ(1){
        detect_in_thread(0);
    }
}

শূন্য demo(ৱর্ণ *cfgfile, ৱর্ণ *weightfile, ভিন্ন thresh, পূর্ণাঙ্ক cam_index, ধ্রুৱ ৱর্ণ *filename, ৱর্ণ **names, পূর্ণাঙ্ক classes, পূর্ণাঙ্ক delay, ৱর্ণ *prefix, পূর্ণাঙ্ক avg_frames, ভিন্ন hier, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক frames, পূর্ণাঙ্ক fullscreen)
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

    পূর্ণাঙ্ক i;
    demo_total = size_network(net);
    predictions = calloc(demo_frame, পরিমাণ(ভিন্ন*));
    বাবে (i = 0; i < demo_frame; ++i){
        predictions[i] = calloc(demo_total, পরিমাণ(ভিন্ন));
    }
    avg = calloc(demo_total, পরিমাণ(ভিন্ন));

    যদি(filename){
        printf("video file: %s\n", filename);
        cap = open_video_stream(filename, 0, 0, 0, 0);
    }অন্যথা{
        cap = open_video_stream(0, cam_index, w, h, frames);
    }

    যদি(!cap) error("Couldn't connect to webcam.\n");

    buff[0] = get_image_from_stream(cap);
    buff[1] = copy_image(buff[0]);
    buff[2] = copy_image(buff[0]);
    buff_letter[0] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[1] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[2] = letterbox_image(buff[0], net->w, net->h);

    পূর্ণাঙ্ক গুনতি = 0;
    যদি(!prefix){
        make_window("Demo", 1352, 1013, fullscreen);
    }

    demo_time = what_time_is_it_now();

    যেতিয়ালৈ(!demo_done){
        buff_index = (buff_index + 1) %3;
        যদি(pthread_create(&fetch_thread, 0, fetch_in_thread, 0)) error("Thread creation failed");
        যদি(pthread_create(&detect_thread, 0, detect_in_thread, 0)) error("Thread creation failed");
        যদি(!prefix){
            fps = 1./(what_time_is_it_now() - demo_time);
            demo_time = what_time_is_it_now();
            display_in_thread(0);
        }অন্যথা{
            ৱর্ণ name[256];
            sprintf(name, "%s_%08d", prefix, গুনতি);
            save_image(buff[(buff_index + 1)%3], name);
        }
        pthread_join(fetch_thread, 0);
        pthread_join(detect_thread, 0);
        ++গুনতি;
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
#অন্যথা
শূন্য demo(ৱর্ণ *cfgfile, ৱর্ণ *weightfile, ভিন্ন thresh, পূর্ণাঙ্ক cam_index, ধ্রুৱ ৱর্ণ *filename, ৱর্ণ **names, পূর্ণাঙ্ক classes, পূর্ণাঙ্ক delay, ৱর্ণ *prefix, পূর্ণাঙ্ক avg, ভিন্ন hier, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক frames, পূর্ণাঙ্ক fullscreen)
{
    fprintf(stderr, "Demo needs OpenCV for webcam images.\n");
}
#endif


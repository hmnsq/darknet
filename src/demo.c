#ଅନ୍ତର୍ଭାଵ "network.h"
#ଅନ୍ତର୍ଭାଵ "detection_layer.h"
#ଅନ୍ତର୍ଭାଵ "region_layer.h"
#ଅନ୍ତର୍ଭାଵ "cost_layer.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "parser.h"
#ଅନ୍ତର୍ଭାଵ "box.h"
#ଅନ୍ତର୍ଭାଵ "image.h"
#ଅନ୍ତର୍ଭାଵ "demo.h"
#ଅନ୍ତର୍ଭାଵ <sys/time.h>

#define DEMO 1

#ifdef OPENCV

ସ୍ଥିର ଅକ୍ଷର **demo_names;
ସ୍ଥିର image **demo_alphabet;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା demo_classes;

ସ୍ଥିର network *net;
ସ୍ଥିର image buff [3];
ସ୍ଥିର image buff_letter[3];
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା buff_index = 0;
ସ୍ଥିର ଶୂନ୍ୟ * cap;
ସ୍ଥିର ଦଶମିକ fps = 0;
ସ୍ଥିର ଦଶମିକ demo_thresh = 0;
ସ୍ଥିର ଦଶମିକ demo_hier = .5;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା running = 0;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା demo_frame = 3;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା demo_index = 0;
ସ୍ଥିର ଦଶମିକ **predictions;
ସ୍ଥିର ଦଶମିକ *avg;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା demo_done = 0;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା demo_total = 0;
ଦ୍ୱିଗୁଣ demo_time;

detection *get_network_boxes(network *net, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ଦଶମିକ thresh, ଦଶମିକ hier, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *map, ପୂର୍ଣ୍ଣସଂଖ୍ୟା relative, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ଅଙ୍କ);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା size_network(network *net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    ପାଇଁ(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ଯଦି(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            ଗଣନା_2 += l.outputs;
        }
    }
    ଫେରନ୍ତୁ ଗଣନା_2;
}

ଶୂନ୍ୟ remember_network(network *net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    ପାଇଁ(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ଯଦି(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            memcpy(predictions[demo_index] + ଗଣନା_2, net->layers[i].output, ଆକାର(ଦଶମିକ) * l.outputs);
            ଗଣନା_2 += l.outputs;
        }
    }
}

detection *avg_predictions(network *net, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *nboxes)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    fill_cpu(demo_total, 0, avg, 1);
    ପାଇଁ(j = 0; j < demo_frame; ++j){
        axpy_cpu(demo_total, 1./demo_frame, predictions[j], 1, avg, 1);
    }
    ପାଇଁ(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        ଯଦି(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            memcpy(l.output, avg + ଗଣନା_2, ଆକାର(ଦଶମିକ) * l.outputs);
            ଗଣନା_2 += l.outputs;
        }
    }
    detection *dets = get_network_boxes(net, buff[0].w, buff[0].h, demo_thresh, demo_hier, 0, 1, nboxes);
    ଫେରନ୍ତୁ dets;
}

ଶୂନ୍ୟ *detect_in_thread(ଶୂନ୍ୟ *ptr)
{
    running = 1;
    ଦଶମିକ nms = .4;

    layer l = net->layers[net->n-1];
    ଦଶମିକ *X = buff_letter[(buff_index+2)%3].data;
    network_predict(net, X);

    /*
       if(l.type == DETECTION){
       get_detection_boxes(l, 1, 1, demo_thresh, probs, boxes, 0);
       } else */
    remember_network(net);
    detection *dets = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nboxes = 0;
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

    ଯଦି (nms > 0) do_nms_obj(dets, nboxes, l.classes, nms);

    printf("\033[2J");
    printf("\033[1;1H");
    printf("\nFPS:%.1f\n",fps);
    printf("Objects:\n\n");
    image display = buff[(buff_index+2) % 3];
    draw_detections(display, dets, nboxes, demo_thresh, demo_names, demo_alphabet, demo_classes);
    free_detections(dets, nboxes);

    demo_index = (demo_index + 1)%demo_frame;
    running = 0;
    ଫେରନ୍ତୁ 0;
}

ଶୂନ୍ୟ *fetch_in_thread(ଶୂନ୍ୟ *ptr)
{
    free_image(buff[buff_index]);
    buff[buff_index] = get_image_from_stream(cap);
    ଯଦି(buff[buff_index].data == 0) {
        demo_done = 1;
        ଫେରନ୍ତୁ 0;
    }
    letterbox_image_into(buff[buff_index], net->w, net->h, buff_letter[buff_index]);
    ଫେରନ୍ତୁ 0;
}

ଶୂନ୍ୟ *display_in_thread(ଶୂନ୍ୟ *ptr)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = show_image(buff[(buff_index + 1)%3], "Demo", 1);
    ଯଦି (c != -1) c = c%256;
    ଯଦି (c == 27) {
        demo_done = 1;
        ଫେରନ୍ତୁ 0;
    } ଅନ୍ୟଥା ଯଦି (c == 82) {
        demo_thresh += .02;
    } ଅନ୍ୟଥା ଯଦି (c == 84) {
        demo_thresh -= .02;
        ଯଦି(demo_thresh <= .02) demo_thresh = .02;
    } ଅନ୍ୟଥା ଯଦି (c == 83) {
        demo_hier += .02;
    } ଅନ୍ୟଥା ଯଦି (c == 81) {
        demo_hier -= .02;
        ଯଦି(demo_hier <= .0) demo_hier = .0;
    }
    ଫେରନ୍ତୁ 0;
}

ଶୂନ୍ୟ *display_loop(ଶୂନ୍ୟ *ptr)
{
    ଯେତେବେଳେ(1){
        display_in_thread(0);
    }
}

ଶୂନ୍ୟ *detect_loop(ଶୂନ୍ୟ *ptr)
{
    ଯେତେବେଳେ(1){
        detect_in_thread(0);
    }
}

ଶୂନ୍ୟ demo(ଅକ୍ଷର *cfgfile, ଅକ୍ଷର *weightfile, ଦଶମିକ thresh, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cam_index, ଅଚଳ ଅକ୍ଷର *filename, ଅକ୍ଷର **names, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା delay, ଅକ୍ଷର *prefix, ପୂର୍ଣ୍ଣସଂଖ୍ୟା avg_frames, ଦଶମିକ hier, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା frames, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fullscreen)
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

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    demo_total = size_network(net);
    predictions = calloc(demo_frame, ଆକାର(ଦଶମିକ*));
    ପାଇଁ (i = 0; i < demo_frame; ++i){
        predictions[i] = calloc(demo_total, ଆକାର(ଦଶମିକ));
    }
    avg = calloc(demo_total, ଆକାର(ଦଶମିକ));

    ଯଦି(filename){
        printf("video file: %s\n", filename);
        cap = open_video_stream(filename, 0, 0, 0, 0);
    }ଅନ୍ୟଥା{
        cap = open_video_stream(0, cam_index, w, h, frames);
    }

    ଯଦି(!cap) error("Couldn't connect to webcam.\n");

    buff[0] = get_image_from_stream(cap);
    buff[1] = copy_image(buff[0]);
    buff[2] = copy_image(buff[0]);
    buff_letter[0] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[1] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[2] = letterbox_image(buff[0], net->w, net->h);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    ଯଦି(!prefix){
        make_window("Demo", 1352, 1013, fullscreen);
    }

    demo_time = what_time_is_it_now();

    ଯେତେବେଳେ(!demo_done){
        buff_index = (buff_index + 1) %3;
        ଯଦି(pthread_create(&fetch_thread, 0, fetch_in_thread, 0)) error("Thread creation failed");
        ଯଦି(pthread_create(&detect_thread, 0, detect_in_thread, 0)) error("Thread creation failed");
        ଯଦି(!prefix){
            fps = 1./(what_time_is_it_now() - demo_time);
            demo_time = what_time_is_it_now();
            display_in_thread(0);
        }ଅନ୍ୟଥା{
            ଅକ୍ଷର name[256];
            sprintf(name, "%s_%08d", prefix, ଗଣନା_2);
            save_image(buff[(buff_index + 1)%3], name);
        }
        pthread_join(fetch_thread, 0);
        pthread_join(detect_thread, 0);
        ++ଗଣନା_2;
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
#ଅନ୍ୟଥା
ଶୂନ୍ୟ demo(ଅକ୍ଷର *cfgfile, ଅକ୍ଷର *weightfile, ଦଶମିକ thresh, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cam_index, ଅଚଳ ଅକ୍ଷର *filename, ଅକ୍ଷର **names, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା delay, ଅକ୍ଷର *prefix, ପୂର୍ଣ୍ଣସଂଖ୍ୟା avg, ଦଶମିକ hier, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା frames, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fullscreen)
{
    fprintf(stderr, "Demo needs OpenCV for webcam images.\n");
}
#endif


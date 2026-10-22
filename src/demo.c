#அந்தர்பாவ "network.h"
#அந்தர்பாவ "detection_layer.h"
#அந்தர்பாவ "region_layer.h"
#அந்தர்பாவ "cost_layer.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "parser.h"
#அந்தர்பாவ "box.h"
#அந்தர்பாவ "image.h"
#அந்தர்பாவ "demo.h"
#அந்தர்பாவ <sys/time.h>

#define DEMO 1

#ifdef OPENCV

நிலையான எழுத்து **demo_names;
நிலையான image **demo_alphabet;
நிலையான முழு demo_classes;

நிலையான network *net;
நிலையான image buff [3];
நிலையான image buff_letter[3];
நிலையான முழு buff_index = 0;
நிலையான வெறுமை * cap;
நிலையான தசம fps = 0;
நிலையான தசம demo_thresh = 0;
நிலையான தசம demo_hier = .5;
நிலையான முழு running = 0;

நிலையான முழு demo_frame = 3;
நிலையான முழு demo_index = 0;
நிலையான தசம **predictions;
நிலையான தசம *avg;
நிலையான முழு demo_done = 0;
நிலையான முழு demo_total = 0;
இரட்டை demo_time;

detection *get_network_boxes(network *net, முழு w, முழு h, தசம thresh, தசம hier, முழு *map, முழு relative, முழு *இலக்கம்);

முழு size_network(network *net)
{
    முழு i;
    முழு மொத்தம் = 0;
    க்கு(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        எனில்(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            மொத்தம் += l.outputs;
        }
    }
    திரும்பு மொத்தம்;
}

வெறுமை remember_network(network *net)
{
    முழு i;
    முழு மொத்தம் = 0;
    க்கு(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        எனில்(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            memcpy(predictions[demo_index] + மொத்தம், net->layers[i].output, அளவு(தசம) * l.outputs);
            மொத்தம் += l.outputs;
        }
    }
}

detection *avg_predictions(network *net, முழு *nboxes)
{
    முழு i, j;
    முழு மொத்தம் = 0;
    fill_cpu(demo_total, 0, avg, 1);
    க்கு(j = 0; j < demo_frame; ++j){
        axpy_cpu(demo_total, 1./demo_frame, predictions[j], 1, avg, 1);
    }
    க்கு(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        எனில்(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            memcpy(l.output, avg + மொத்தம், அளவு(தசம) * l.outputs);
            மொத்தம் += l.outputs;
        }
    }
    detection *dets = get_network_boxes(net, buff[0].w, buff[0].h, demo_thresh, demo_hier, 0, 1, nboxes);
    திரும்பு dets;
}

வெறுமை *detect_in_thread(வெறுமை *ptr)
{
    running = 1;
    தசம nms = .4;

    layer l = net->layers[net->n-1];
    தசம *X = buff_letter[(buff_index+2)%3].data;
    network_predict(net, X);

    /*
       if(l.type == DETECTION){
       get_detection_boxes(l, 1, 1, demo_thresh, probs, boxes, 0);
       } else */
    remember_network(net);
    detection *dets = 0;
    முழு nboxes = 0;
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

    எனில் (nms > 0) do_nms_obj(dets, nboxes, l.classes, nms);

    printf("\033[2J");
    printf("\033[1;1H");
    printf("\nFPS:%.1f\n",fps);
    printf("Objects:\n\n");
    image display = buff[(buff_index+2) % 3];
    draw_detections(display, dets, nboxes, demo_thresh, demo_names, demo_alphabet, demo_classes);
    free_detections(dets, nboxes);

    demo_index = (demo_index + 1)%demo_frame;
    running = 0;
    திரும்பு 0;
}

வெறுமை *fetch_in_thread(வெறுமை *ptr)
{
    free_image(buff[buff_index]);
    buff[buff_index] = get_image_from_stream(cap);
    எனில்(buff[buff_index].data == 0) {
        demo_done = 1;
        திரும்பு 0;
    }
    letterbox_image_into(buff[buff_index], net->w, net->h, buff_letter[buff_index]);
    திரும்பு 0;
}

வெறுமை *display_in_thread(வெறுமை *ptr)
{
    முழு c = show_image(buff[(buff_index + 1)%3], "Demo", 1);
    எனில் (c != -1) c = c%256;
    எனில் (c == 27) {
        demo_done = 1;
        திரும்பு 0;
    } இல்லையெனில் எனில் (c == 82) {
        demo_thresh += .02;
    } இல்லையெனில் எனில் (c == 84) {
        demo_thresh -= .02;
        எனில்(demo_thresh <= .02) demo_thresh = .02;
    } இல்லையெனில் எனில் (c == 83) {
        demo_hier += .02;
    } இல்லையெனில் எனில் (c == 81) {
        demo_hier -= .02;
        எனில்(demo_hier <= .0) demo_hier = .0;
    }
    திரும்பு 0;
}

வெறுமை *display_loop(வெறுமை *ptr)
{
    போது(1){
        display_in_thread(0);
    }
}

வெறுமை *detect_loop(வெறுமை *ptr)
{
    போது(1){
        detect_in_thread(0);
    }
}

வெறுமை demo(எழுத்து *cfgfile, எழுத்து *weightfile, தசம thresh, முழு cam_index, மாறிலி எழுத்து *filename, எழுத்து **names, முழு classes, முழு delay, எழுத்து *prefix, முழு avg_frames, தசம hier, முழு w, முழு h, முழு frames, முழு fullscreen)
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

    முழு i;
    demo_total = size_network(net);
    predictions = calloc(demo_frame, அளவு(தசம*));
    க்கு (i = 0; i < demo_frame; ++i){
        predictions[i] = calloc(demo_total, அளவு(தசம));
    }
    avg = calloc(demo_total, அளவு(தசம));

    எனில்(filename){
        printf("video file: %s\n", filename);
        cap = open_video_stream(filename, 0, 0, 0, 0);
    }இல்லையெனில்{
        cap = open_video_stream(0, cam_index, w, h, frames);
    }

    எனில்(!cap) error("Couldn't connect to webcam.\n");

    buff[0] = get_image_from_stream(cap);
    buff[1] = copy_image(buff[0]);
    buff[2] = copy_image(buff[0]);
    buff_letter[0] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[1] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[2] = letterbox_image(buff[0], net->w, net->h);

    முழு மொத்தம் = 0;
    எனில்(!prefix){
        make_window("Demo", 1352, 1013, fullscreen);
    }

    demo_time = what_time_is_it_now();

    போது(!demo_done){
        buff_index = (buff_index + 1) %3;
        எனில்(pthread_create(&fetch_thread, 0, fetch_in_thread, 0)) error("Thread creation failed");
        எனில்(pthread_create(&detect_thread, 0, detect_in_thread, 0)) error("Thread creation failed");
        எனில்(!prefix){
            fps = 1./(what_time_is_it_now() - demo_time);
            demo_time = what_time_is_it_now();
            display_in_thread(0);
        }இல்லையெனில்{
            எழுத்து name[256];
            sprintf(name, "%s_%08d", prefix, மொத்தம்);
            save_image(buff[(buff_index + 1)%3], name);
        }
        pthread_join(fetch_thread, 0);
        pthread_join(detect_thread, 0);
        ++மொத்தம்;
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
#இல்லையெனில்
வெறுமை demo(எழுத்து *cfgfile, எழுத்து *weightfile, தசம thresh, முழு cam_index, மாறிலி எழுத்து *filename, எழுத்து **names, முழு classes, முழு delay, எழுத்து *prefix, முழு avg, தசம hier, முழு w, முழு h, முழு frames, முழு fullscreen)
{
    fprintf(stderr, "Demo needs OpenCV for webcam images.\n");
}
#endif


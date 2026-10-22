#अन्तर्भाव "network.h"
#अन्तर्भाव "detection_layer.h"
#अन्तर्भाव "region_layer.h"
#अन्तर्भाव "cost_layer.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "parser.h"
#अन्तर्भाव "box.h"
#अन्तर्भाव "image.h"
#अन्तर्भाव "demo.h"
#अन्तर्भाव <sys/time.h>

#define DEMO 1

#ifdef OPENCV

स्थिर वर्ण **demo_names;
स्थिर image **demo_alphabet;
स्थिर पूर्णाङ्क demo_classes;

स्थिर network *net;
स्थिर image buff [3];
स्थिर image buff_letter[3];
स्थिर पूर्णाङ्क buff_index = 0;
स्थिर शून्य * cap;
स्थिर भिन्न fps = 0;
स्थिर भिन्न demo_thresh = 0;
स्थिर भिन्न demo_hier = .5;
स्थिर पूर्णाङ्क running = 0;

स्थिर पूर्णाङ्क demo_frame = 3;
स्थिर पूर्णाङ्क demo_index = 0;
स्थिर भिन्न **predictions;
स्थिर भिन्न *avg;
स्थिर पूर्णाङ्क demo_done = 0;
स्थिर पूर्णाङ्क demo_total = 0;
द्विगुण demo_time;

detection *get_network_boxes(network *net, पूर्णाङ्क w, पूर्णाङ्क h, भिन्न thresh, भिन्न hier, पूर्णाङ्क *map, पूर्णाङ्क relative, पूर्णाङ्क *अंक);

पूर्णाङ्क size_network(network *net)
{
    पूर्णाङ्क i;
    पूर्णाङ्क गिनती = 0;
    प्रति(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        यदि(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            गिनती += l.outputs;
        }
    }
    प्रत्यागम गिनती;
}

शून्य remember_network(network *net)
{
    पूर्णाङ्क i;
    पूर्णाङ्क गिनती = 0;
    प्रति(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        यदि(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            स_नकल(predictions[demo_index] + गिनती, net->layers[i].output, परिमाण(भिन्न) * l.outputs);
            गिनती += l.outputs;
        }
    }
}

detection *avg_predictions(network *net, पूर्णाङ्क *nboxes)
{
    पूर्णाङ्क i, j;
    पूर्णाङ्क गिनती = 0;
    fill_cpu(demo_total, 0, avg, 1);
    प्रति(j = 0; j < demo_frame; ++j){
        axpy_cpu(demo_total, 1./demo_frame, predictions[j], 1, avg, 1);
    }
    प्रति(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        यदि(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            स_नकल(l.output, avg + गिनती, परिमाण(भिन्न) * l.outputs);
            गिनती += l.outputs;
        }
    }
    detection *dets = get_network_boxes(net, buff[0].w, buff[0].h, demo_thresh, demo_hier, 0, 1, nboxes);
    प्रत्यागम dets;
}

शून्य *detect_in_thread(शून्य *ptr)
{
    running = 1;
    भिन्न nms = .4;

    layer l = net->layers[net->n-1];
    भिन्न *X = buff_letter[(buff_index+2)%3].data;
    network_predict(net, X);

    /*
       if(l.type == DETECTION){
       get_detection_boxes(l, 1, 1, demo_thresh, probs, boxes, 0);
       } else */
    remember_network(net);
    detection *dets = 0;
    पूर्णाङ्क nboxes = 0;
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

    यदि (nms > 0) do_nms_obj(dets, nboxes, l.classes, nms);

    म_लिखो("\033[2J");
    म_लिखो("\033[1;1H");
    म_लिखो("\nFPS:%.1f\n",fps);
    म_लिखो("Objects:\n\n");
    image display = buff[(buff_index+2) % 3];
    draw_detections(display, dets, nboxes, demo_thresh, demo_names, demo_alphabet, demo_classes);
    free_detections(dets, nboxes);

    demo_index = (demo_index + 1)%demo_frame;
    running = 0;
    प्रत्यागम 0;
}

शून्य *fetch_in_thread(शून्य *ptr)
{
    free_image(buff[buff_index]);
    buff[buff_index] = get_image_from_stream(cap);
    यदि(buff[buff_index].data == 0) {
        demo_done = 1;
        प्रत्यागम 0;
    }
    letterbox_image_into(buff[buff_index], net->w, net->h, buff_letter[buff_index]);
    प्रत्यागम 0;
}

शून्य *display_in_thread(शून्य *ptr)
{
    पूर्णाङ्क c = show_image(buff[(buff_index + 1)%3], "Demo", 1);
    यदि (c != -1) c = c%256;
    यदि (c == 27) {
        demo_done = 1;
        प्रत्यागम 0;
    } अन्यथा यदि (c == 82) {
        demo_thresh += .02;
    } अन्यथा यदि (c == 84) {
        demo_thresh -= .02;
        यदि(demo_thresh <= .02) demo_thresh = .02;
    } अन्यथा यदि (c == 83) {
        demo_hier += .02;
    } अन्यथा यदि (c == 81) {
        demo_hier -= .02;
        यदि(demo_hier <= .0) demo_hier = .0;
    }
    प्रत्यागम 0;
}

शून्य *display_loop(शून्य *ptr)
{
    यावत्(1){
        display_in_thread(0);
    }
}

शून्य *detect_loop(शून्य *ptr)
{
    यावत्(1){
        detect_in_thread(0);
    }
}

शून्य demo(वर्ण *cfgfile, वर्ण *weightfile, भिन्न thresh, पूर्णाङ्क cam_index, ध्रुव वर्ण *filename, वर्ण **names, पूर्णाङ्क classes, पूर्णाङ्क delay, वर्ण *prefix, पूर्णाङ्क avg_frames, भिन्न hier, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क frames, पूर्णाङ्क fullscreen)
{
    //demo_frame = avg_frames;
    image **alphabet = load_alphabet();
    demo_names = names;
    demo_alphabet = alphabet;
    demo_classes = classes;
    demo_thresh = thresh;
    demo_hier = hier;
    म_लिखो("Demo\n");
    net = load_network(cfgfile, weightfile, 0);
    set_batch_network(net, 1);
    pthread_t detect_thread;
    pthread_t fetch_thread;

    बेक्रम(2222222);

    पूर्णाङ्क i;
    demo_total = size_network(net);
    predictions = सुस्मृति(demo_frame, परिमाण(भिन्न*));
    प्रति (i = 0; i < demo_frame; ++i){
        predictions[i] = सुस्मृति(demo_total, परिमाण(भिन्न));
    }
    avg = सुस्मृति(demo_total, परिमाण(भिन्न));

    यदि(filename){
        म_लिखो("video file: %s\n", filename);
        cap = open_video_stream(filename, 0, 0, 0, 0);
    }अन्यथा{
        cap = open_video_stream(0, cam_index, w, h, frames);
    }

    यदि(!cap) error("Couldn't connect to webcam.\n");

    buff[0] = get_image_from_stream(cap);
    buff[1] = copy_image(buff[0]);
    buff[2] = copy_image(buff[0]);
    buff_letter[0] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[1] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[2] = letterbox_image(buff[0], net->w, net->h);

    पूर्णाङ्क गिनती = 0;
    यदि(!prefix){
        make_window("Demo", 1352, 1013, fullscreen);
    }

    demo_time = what_time_is_it_now();

    यावत्(!demo_done){
        buff_index = (buff_index + 1) %3;
        यदि(pthread_create(&fetch_thread, 0, fetch_in_thread, 0)) error("Thread creation failed");
        यदि(pthread_create(&detect_thread, 0, detect_in_thread, 0)) error("Thread creation failed");
        यदि(!prefix){
            fps = 1./(what_time_is_it_now() - demo_time);
            demo_time = what_time_is_it_now();
            display_in_thread(0);
        }अन्यथा{
            वर्ण name[256];
            प्र_लिखो(name, "%s_%08d", prefix, गिनती);
            save_image(buff[(buff_index + 1)%3], name);
        }
        pthread_join(fetch_thread, 0);
        pthread_join(detect_thread, 0);
        ++गिनती;
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
#अन्यथा
शून्य demo(वर्ण *cfgfile, वर्ण *weightfile, भिन्न thresh, पूर्णाङ्क cam_index, ध्रुव वर्ण *filename, वर्ण **names, पूर्णाङ्क classes, पूर्णाङ्क delay, वर्ण *prefix, पूर्णाङ्क avg, भिन्न hier, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क frames, पूर्णाङ्क fullscreen)
{
    ख_लिखो(मानक_त्रुटि, "Demo needs OpenCV for webcam images.\n");
}
#endif


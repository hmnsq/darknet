#समावेश "network.h"
#समावेश "detection_layer.h"
#समावेश "region_layer.h"
#समावेश "cost_layer.h"
#समावेश "utils.h"
#समावेश "parser.h"
#समावेश "box.h"
#समावेश "image.h"
#समावेश "demo.h"
#समावेश <sys/time.h>

#घोषणा DEMO 1

#अगर_घोषित OPENCV

अटल अक्षर **demo_names;
अटल image **demo_alphabet;
अटल पूर्णांक demo_classes;

अटल network *net;
अटल image buff [3];
अटल image buff_letter[3];
अटल पूर्णांक buff_सूचकांक = 0;
अटल व्योम * cap;
अटल भग्न fps = 0;
अटल भग्न demo_thresh = 0;
अटल भग्न demo_hier = .5;
अटल पूर्णांक running = 0;

अटल पूर्णांक demo_frame = 3;
अटल पूर्णांक demo_सूचकांक = 0;
अटल भग्न **predictions;
अटल भग्न *avg;
अटल पूर्णांक demo_done = 0;
अटल पूर्णांक demo_योग = 0;
द्विगुन demo_time;

detection *get_network_boxes(network *net, पूर्णांक w, पूर्णांक h, भग्न thresh, भग्न hier, पूर्णांक *map, पूर्णांक relative, पूर्णांक *अंक);

पूर्णांक आकार_network(network *net)
{
    पूर्णांक i;
    पूर्णांक गिनती = 0;
    क्रम(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        अगर(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            गिनती += l.outputs;
        }
    }
    वापस गिनती;
}

व्योम remember_network(network *net)
{
    पूर्णांक i;
    पूर्णांक गिनती = 0;
    क्रम(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        अगर(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            स_नकल(predictions[demo_सूचकांक] + गिनती, net->layers[i].output, माप(भग्न) * l.outputs);
            गिनती += l.outputs;
        }
    }
}

detection *avg_predictions(network *net, पूर्णांक *nboxes)
{
    पूर्णांक i, j;
    पूर्णांक गिनती = 0;
    fill_cpu(demo_योग, 0, avg, 1);
    क्रम(j = 0; j < demo_frame; ++j){
        axpy_cpu(demo_योग, 1./demo_frame, predictions[j], 1, avg, 1);
    }
    क्रम(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        अगर(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            स_नकल(l.output, avg + गिनती, माप(भग्न) * l.outputs);
            गिनती += l.outputs;
        }
    }
    detection *dets = get_network_boxes(net, buff[0].w, buff[0].h, demo_thresh, demo_hier, 0, 1, nboxes);
    वापस dets;
}

व्योम *detect_in_thread(व्योम *ptr)
{
    running = 1;
    भग्न nms = .4;

    layer l = net->layers[net->n-1];
    भग्न *X = buff_letter[(buff_सूचकांक+2)%3].तथ्य;
    network_predict(net, X);

    /*
       if(l.type == DETECTION){
       get_detection_boxes(l, 1, 1, demo_thresh, probs, boxes, 0);
       } else */
    remember_network(net);
    detection *dets = 0;
    पूर्णांक nboxes = 0;
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

    अगर (nms > 0) do_nms_obj(dets, nboxes, l.classes, nms);

    म_लिखो("\033[2J");
    म_लिखो("\033[1;1H");
    म_लिखो("\nFPS:%.1f\n",fps);
    म_लिखो("Objects:\n\n");
    image display = buff[(buff_सूचकांक+2) % 3];
    draw_detections(display, dets, nboxes, demo_thresh, demo_names, demo_alphabet, demo_classes);
    free_detections(dets, nboxes);

    demo_सूचकांक = (demo_सूचकांक + 1)%demo_frame;
    running = 0;
    वापस 0;
}

व्योम *fetch_in_thread(व्योम *ptr)
{
    free_image(buff[buff_सूचकांक]);
    buff[buff_सूचकांक] = get_image_from_stream(cap);
    अगर(buff[buff_सूचकांक].तथ्य == 0) {
        demo_done = 1;
        वापस 0;
    }
    letterbox_image_into(buff[buff_सूचकांक], net->w, net->h, buff_letter[buff_सूचकांक]);
    वापस 0;
}

व्योम *display_in_thread(व्योम *ptr)
{
    पूर्णांक c = show_image(buff[(buff_सूचकांक + 1)%3], "Demo", 1);
    अगर (c != -1) c = c%256;
    अगर (c == 27) {
        demo_done = 1;
        वापस 0;
    } अन्यथा अगर (c == 82) {
        demo_thresh += .02;
    } अन्यथा अगर (c == 84) {
        demo_thresh -= .02;
        अगर(demo_thresh <= .02) demo_thresh = .02;
    } अन्यथा अगर (c == 83) {
        demo_hier += .02;
    } अन्यथा अगर (c == 81) {
        demo_hier -= .02;
        अगर(demo_hier <= .0) demo_hier = .0;
    }
    वापस 0;
}

व्योम *display_loop(व्योम *ptr)
{
    जबतक(1){
        display_in_thread(0);
    }
}

व्योम *detect_loop(व्योम *ptr)
{
    जबतक(1){
        detect_in_thread(0);
    }
}

व्योम demo(अक्षर *cfgfile, अक्षर *weightfile, भग्न thresh, पूर्णांक cam_सूचकांक, स्थिर अक्षर *filename, अक्षर **names, पूर्णांक classes, पूर्णांक delay, अक्षर *prefix, पूर्णांक avg_frames, भग्न hier, पूर्णांक w, पूर्णांक h, पूर्णांक frames, पूर्णांक fullscreen)
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

    पूर्णांक i;
    demo_योग = आकार_network(net);
    predictions = सुस्मृति(demo_frame, माप(भग्न*));
    क्रम (i = 0; i < demo_frame; ++i){
        predictions[i] = सुस्मृति(demo_योग, माप(भग्न));
    }
    avg = सुस्मृति(demo_योग, माप(भग्न));

    अगर(filename){
        म_लिखो("video file: %s\n", filename);
        cap = open_video_stream(filename, 0, 0, 0, 0);
    }अन्यथा{
        cap = open_video_stream(0, cam_सूचकांक, w, h, frames);
    }

    अगर(!cap) error("Couldn't connect to webcam.\n");

    buff[0] = get_image_from_stream(cap);
    buff[1] = copy_image(buff[0]);
    buff[2] = copy_image(buff[0]);
    buff_letter[0] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[1] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[2] = letterbox_image(buff[0], net->w, net->h);

    पूर्णांक गिनती = 0;
    अगर(!prefix){
        make_window("Demo", 1352, 1013, fullscreen);
    }

    demo_time = what_time_is_it_now();

    जबतक(!demo_done){
        buff_सूचकांक = (buff_सूचकांक + 1) %3;
        अगर(pthread_create(&fetch_thread, 0, fetch_in_thread, 0)) error("Thread creation failed");
        अगर(pthread_create(&detect_thread, 0, detect_in_thread, 0)) error("Thread creation failed");
        अगर(!prefix){
            fps = 1./(what_time_is_it_now() - demo_time);
            demo_time = what_time_is_it_now();
            display_in_thread(0);
        }अन्यथा{
            अक्षर name[256];
            प्र_लिखो(name, "%s_%08d", prefix, गिनती);
            save_image(buff[(buff_सूचकांक + 1)%3], name);
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
व्योम demo(अक्षर *cfgfile, अक्षर *weightfile, भग्न thresh, पूर्णांक cam_सूचकांक, स्थिर अक्षर *filename, अक्षर **names, पूर्णांक classes, पूर्णांक delay, अक्षर *prefix, पूर्णांक avg, भग्न hier, पूर्णांक w, पूर्णांक h, पूर्णांक frames, पूर्णांक fullscreen)
{
    ख_लिखो(मानक_त्रुटि, "Demo needs OpenCV for webcam images.\n");
}
#पूर्ण_अगर


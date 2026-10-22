#ifdef OPENCV

#ଅନ୍ତର୍ଭାଵ "stdio.h"
#ଅନ୍ତର୍ଭାଵ "stdlib.h"
#ଅନ୍ତର୍ଭାଵ "opencv2/opencv.hpp"
#ଅନ୍ତର୍ଭାଵ "image.h"

using ନାମାକାଶ cv;

ବାହ୍ୟ "C" {

IplImage *image_to_ipl(image im)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା x,y,c;
    IplImage *disp = cvCreateImage(cvSize(im.w,im.h), IPL_DEPTH_8U, im.c);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା step = disp->widthStep;
    ପାଇଁ(y = 0; y < im.h; ++y){
        ପାଇଁ(x = 0; x < im.w; ++x){
            ପାଇଁ(c= 0; c < im.c; ++c){
                ଦଶମିକ val = im.data[c*im.h*im.w + y*im.w + x];
                disp->imageData[y*step + x*im.c + c] = (unsigned ଅକ୍ଷର)(val*255);
            }
        }
    }
    ଫେରନ୍ତୁ disp;
}

image ipl_to_image(IplImage* src)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା h = src->height;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା w = src->width;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = src->nChannels;
    image im = make_image(w, h, c);
    unsigned ଅକ୍ଷର *data = (unsigned ଅକ୍ଷର *)src->imageData;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା step = src->widthStep;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, k;

    ପାଇଁ(i = 0; i < h; ++i){
        ପାଇଁ(k= 0; k < c; ++k){
            ପାଇଁ(j = 0; j < w; ++j){
                im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
            }
        }
    }
    ଫେରନ୍ତୁ im;
}

Mat image_to_mat(image im)
{
    image copy = copy_image(im);
    constrain_image(copy);
    ଯଦି(im.c == 3) rgbgr_image(copy);

    IplImage *ipl = image_to_ipl(copy);
    Mat m = cvarrToMat(ipl, ସତ୍ୟ);
    cvReleaseImage(&ipl);
    free_image(copy);
    ଫେରନ୍ତୁ m;
}

image mat_to_image(Mat m)
{
    IplImage ipl = m;
    image im = ipl_to_image(&ipl);
    rgbgr_image(im);
    ଫେରନ୍ତୁ im;
}

ଶୂନ୍ୟ *open_video_stream(ଅଚଳ ଅକ୍ଷର *f, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fps)
{
    VideoCapture *cap;
    ଯଦି(f) cap = ନୂତନ VideoCapture(f);
    ଅନ୍ୟଥା cap = ନୂତନ VideoCapture(c);
    ଯଦି(!cap->isOpened()) ଫେରନ୍ତୁ 0;
    ଯଦି(w) cap->set(CV_CAP_PROP_FRAME_WIDTH, w);
    ଯଦି(h) cap->set(CV_CAP_PROP_FRAME_HEIGHT, w);
    ଯଦି(fps) cap->set(CV_CAP_PROP_FPS, w);
    ଫେରନ୍ତୁ (ଶୂନ୍ୟ *) cap;
}

image get_image_from_stream(ଶୂନ୍ୟ *p)
{
    VideoCapture *cap = (VideoCapture *)p;
    Mat m;
    *cap >> m;
    ଯଦି(m.empty()) ଫେରନ୍ତୁ make_empty_image(0,0,0);
    ଫେରନ୍ତୁ mat_to_image(m);
}

image load_image_cv(ଅକ୍ଷର *filename, ପୂର୍ଣ୍ଣସଂଖ୍ୟା channels)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା flag = -1;
    ଯଦି (channels == 0) flag = -1;
    ଅନ୍ୟଥା ଯଦି (channels == 1) flag = 0;
    ଅନ୍ୟଥା ଯଦି (channels == 3) flag = 1;
    ଅନ୍ୟଥା {
        fprintf(stderr, "OpenCV can't force load with %d channels\n", channels);
    }
    Mat m;
    m = imread(filename, flag);
    ଯଦି(!m.data){
        fprintf(stderr, "Cannot load image \"%s\"\n", filename);
        ଅକ୍ଷର buff[256];
        sprintf(buff, "echo %s >> bad.list", filename);
        system(buff);
        ଫେରନ୍ତୁ make_image(10,10,3);
        //exit(0);
    }
    image im = mat_to_image(m);
    ଫେରନ୍ତୁ im;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_image_cv(image im, ଅଚଳ ଅକ୍ଷର* name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ms)
{
    Mat m = image_to_mat(im);
    imshow(name, m);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = wait_ଚାବି(ms);
    ଯଦି (c != -1) c = c%256;
    ଫେରନ୍ତୁ c;
}

ଶୂନ୍ୟ make_window(ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fullscreen)
{
    namedWindow(name, WINDOW_NORMAL); 
    ଯଦି (fullscreen) {
        setWindowProperty(name, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    } ଅନ୍ୟଥା {
        resizeWindow(name, w, h);
        ଯଦି(strcmp(name, "Demo") == 0) moveWindow(name, 0, 0);
    }
}

}

#endif

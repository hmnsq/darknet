#ifdef OPENCV

#include "stdio.h"
#include "stdlib.h"
#include "opencv2/opencv.hpp"
#include "image.h"

using namespace cv;

extern "C" {

IplImage *image_to_ipl(image im)
{
    整数 x,y,c;
    IplImage *disp = cvCreateImage(cvSize(im.w,im.h), IPL_DEPTH_8U, im.c);
    整数 step = disp->widthStep;
    ために(y = 0; y < im.h; ++y){
        ために(x = 0; x < im.w; ++x){
            ために(c= 0; c < im.c; ++c){
                float val = im.data[c*im.h*im.w + y*im.w + x];
                disp->imageData[y*step + x*im.c + c] = (unsigned char)(val*255);
            }
        }
    }
    戻る disp;
}

image ipl_to_image(IplImage* src)
{
    整数 h = src->height;
    整数 w = src->width;
    整数 c = src->nChannels;
    image im = make_image(w, h, c);
    unsigned char *data = (unsigned char *)src->imageData;
    整数 step = src->widthStep;
    整数 i, j, k;

    ために(i = 0; i < h; ++i){
        ために(k= 0; k < c; ++k){
            ために(j = 0; j < w; ++j){
                im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
            }
        }
    }
    戻る im;
}

Mat image_to_mat(image im)
{
    image copy = copy_image(im);
    constrain_image(copy);
    もし(im.c == 3) rgbgr_image(copy);

    IplImage *ipl = image_to_ipl(copy);
    Mat m = cvarrToMat(ipl, true);
    cvReleaseImage(&ipl);
    free_image(copy);
    戻る m;
}

image mat_to_image(Mat m)
{
    IplImage ipl = m;
    image im = ipl_to_image(&ipl);
    rgbgr_image(im);
    戻る im;
}

void *open_video_stream(const char *f, 整数 c, 整数 w, 整数 h, 整数 fps)
{
    VideoCapture *cap;
    もし(f) cap = new VideoCapture(f);
    否则 cap = new VideoCapture(c);
    もし(!cap->isOpened()) 戻る 0;
    もし(w) cap->set(CV_CAP_PROP_FRAME_WIDTH, w);
    もし(h) cap->set(CV_CAP_PROP_FRAME_HEIGHT, w);
    もし(fps) cap->set(CV_CAP_PROP_FPS, w);
    戻る (void *) cap;
}

image get_image_from_stream(void *p)
{
    VideoCapture *cap = (VideoCapture *)p;
    Mat m;
    *cap >> m;
    もし(m.empty()) 戻る make_empty_image(0,0,0);
    戻る mat_to_image(m);
}

image load_image_cv(char *filename, 整数 channels)
{
    整数 flag = -1;
    もし (channels == 0) flag = -1;
    否则 もし (channels == 1) flag = 0;
    否则 もし (channels == 3) flag = 1;
    否则 {
        fprintf(stderr, "OpenCV can't force load with %d channels\n", channels);
    }
    Mat m;
    m = imread(filename, flag);
    もし(!m.data){
        fprintf(stderr, "Cannot load image \"%s\"\n", filename);
        char buff[256];
        sprintf(buff, "echo %s >> bad.list", filename);
        system(buff);
        戻る make_image(10,10,3);
        //exit(0);
    }
    image im = mat_to_image(m);
    戻る im;
}

整数 show_image_cv(image im, const char* name, 整数 ms)
{
    Mat m = image_to_mat(im);
    imshow(name, m);
    整数 c = waitKey(ms);
    もし (c != -1) c = c%256;
    戻る c;
}

void make_window(char *name, 整数 w, 整数 h, 整数 fullscreen)
{
    namedWindow(name, WINDOW_NORMAL); 
    もし (fullscreen) {
        setWindowProperty(name, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    } 否则 {
        resizeWindow(name, w, h);
        もし(strcmp(name, "Demo") == 0) moveWindow(name, 0, 0);
    }
}

}

#endif

#ifdef OPENCV

#شامل "stdio.h"
#شامل "stdlib.h"
#شامل "opencv2/opencv.hpp"
#شامل "image.h"

using فضاینام cv;

بیرونی "C" {

IplImage *image_to_ipl(image im)
{
    صحیح x,y,c;
    IplImage *disp = cvCreateImage(cvSize(im.w,im.h), IPL_DEPTH_8U, im.c);
    صحیح step = disp->widthStep;
    برای(y = 0; y < im.h; ++y){
        برای(x = 0; x < im.w; ++x){
            برای(c= 0; c < im.c; ++c){
                اعشاری val = im.data[c*im.h*im.w + y*im.w + x];
                disp->imageData[y*step + x*im.c + c] = (unsigned نویسه)(val*255);
            }
        }
    }
    بازگشت disp;
}

image ipl_to_image(IplImage* src)
{
    صحیح h = src->height;
    صحیح w = src->width;
    صحیح c = src->nChannels;
    image im = make_image(w, h, c);
    unsigned نویسه *data = (unsigned نویسه *)src->imageData;
    صحیح step = src->widthStep;
    صحیح i, j, k;

    برای(i = 0; i < h; ++i){
        برای(k= 0; k < c; ++k){
            برای(j = 0; j < w; ++j){
                im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
            }
        }
    }
    بازگشت im;
}

Mat image_to_mat(image im)
{
    image copy = copy_image(im);
    constrain_image(copy);
    اگر(im.c == 3) rgbgr_image(copy);

    IplImage *ipl = image_to_ipl(copy);
    Mat m = cvarrToMat(ipl, درست);
    cvReleaseImage(&ipl);
    free_image(copy);
    بازگشت m;
}

image mat_to_image(Mat m)
{
    IplImage ipl = m;
    image im = ipl_to_image(&ipl);
    rgbgr_image(im);
    بازگشت im;
}

تهی *open_video_stream(ثابت نویسه *f, صحیح c, صحیح w, صحیح h, صحیح fps)
{
    VideoCapture *cap;
    اگر(f) cap = جدید VideoCapture(f);
    وگرنه cap = جدید VideoCapture(c);
    اگر(!cap->isOpened()) بازگشت 0;
    اگر(w) cap->set(CV_CAP_PROP_FRAME_WIDTH, w);
    اگر(h) cap->set(CV_CAP_PROP_FRAME_HEIGHT, w);
    اگر(fps) cap->set(CV_CAP_PROP_FPS, w);
    بازگشت (تهی *) cap;
}

image get_image_from_stream(تهی *p)
{
    VideoCapture *cap = (VideoCapture *)p;
    Mat m;
    *cap >> m;
    اگر(m.empty()) بازگشت make_empty_image(0,0,0);
    بازگشت mat_to_image(m);
}

image load_image_cv(نویسه *filename, صحیح channels)
{
    صحیح flag = -1;
    اگر (channels == 0) flag = -1;
    وگرنه اگر (channels == 1) flag = 0;
    وگرنه اگر (channels == 3) flag = 1;
    وگرنه {
        fprintf(stderr, "OpenCV can't force load with %d channels\n", channels);
    }
    Mat m;
    m = imread(filename, flag);
    اگر(!m.data){
        fprintf(stderr, "Cannot load image \"%s\"\n", filename);
        نویسه buff[256];
        sprintf(buff, "echo %s >> bad.list", filename);
        system(buff);
        بازگشت make_image(10,10,3);
        //exit(0);
    }
    image im = mat_to_image(m);
    بازگشت im;
}

صحیح show_image_cv(image im, ثابت نویسه* name, صحیح ms)
{
    Mat m = image_to_mat(im);
    imshow(name, m);
    صحیح c = wait_کلید(ms);
    اگر (c != -1) c = c%256;
    بازگشت c;
}

تهی make_window(نویسه *name, صحیح w, صحیح h, صحیح fullscreen)
{
    namedWindow(name, WINDOW_NORMAL); 
    اگر (fullscreen) {
        setWindowProperty(name, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    } وگرنه {
        resizeWindow(name, w, h);
        اگر(strcmp(name, "Demo") == 0) moveWindow(name, 0, 0);
    }
}

}

#endif

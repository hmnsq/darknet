#অন্তর্ভাব "box.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <math.h>
#অন্তর্ভাব <stdlib.h>

পূর্ণাঙ্ক nms_comparator(ধ্রুব শূন্য *pa, ধ্রুব শূন্য *pb)
{
    detection a = *(detection *)pa;
    detection b = *(detection *)pb;
    ভিন্ন diff = 0;
    যদি(b.sort_class >= 0){
        diff = a.prob[b.sort_class] - b.prob[b.sort_class];
    } অন্যথা {
        diff = a.objectness - b.objectness;
    }
    যদি(diff < 0) প্রত্যাগম 1;
    অন্যথা যদি(diff > 0) প্রত্যাগম -1;
    প্রত্যাগম 0;
}

শূন্য do_nms_obj(detection *dets, পূর্ণাঙ্ক total, পূর্ণাঙ্ক classes, ভিন্ন thresh)
{
    পূর্ণাঙ্ক i, j, k;
    k = total-1;
    প্রতি(i = 0; i <= k; ++i){
        যদি(dets[i].objectness == 0){
            detection swap = dets[i];
            dets[i] = dets[k];
            dets[k] = swap;
            --k;
            --i;
        }
    }
    total = k+1;

    প্রতি(i = 0; i < total; ++i){
        dets[i].sort_class = -1;
    }

    qsort(dets, total, পরিমাণ(detection), nms_comparator);
    প্রতি(i = 0; i < total; ++i){
        যদি(dets[i].objectness == 0) অনুবর্ত;
        box a = dets[i].bbox;
        প্রতি(j = i+1; j < total; ++j){
            যদি(dets[j].objectness == 0) অনুবর্ত;
            box b = dets[j].bbox;
            যদি (box_iou(a, b) > thresh){
                dets[j].objectness = 0;
                প্রতি(k = 0; k < classes; ++k){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}


শূন্য do_nms_sort(detection *dets, পূর্ণাঙ্ক total, পূর্ণাঙ্ক classes, ভিন্ন thresh)
{
    পূর্ণাঙ্ক i, j, k;
    k = total-1;
    প্রতি(i = 0; i <= k; ++i){
        যদি(dets[i].objectness == 0){
            detection swap = dets[i];
            dets[i] = dets[k];
            dets[k] = swap;
            --k;
            --i;
        }
    }
    total = k+1;

    প্রতি(k = 0; k < classes; ++k){
        প্রতি(i = 0; i < total; ++i){
            dets[i].sort_class = k;
        }
        qsort(dets, total, পরিমাণ(detection), nms_comparator);
        প্রতি(i = 0; i < total; ++i){
            যদি(dets[i].prob[k] == 0) অনুবর্ত;
            box a = dets[i].bbox;
            প্রতি(j = i+1; j < total; ++j){
                box b = dets[j].bbox;
                যদি (box_iou(a, b) > thresh){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}

box float_to_box(ভিন্ন *f, পূর্ণাঙ্ক stride)
{
    box b = {0};
    b.x = f[0];
    b.y = f[1*stride];
    b.w = f[2*stride];
    b.h = f[3*stride];
    প্রত্যাগম b;
}

dbox derivative(box a, box b)
{
    dbox d;
    d.dx = 0;
    d.dw = 0;
    ভিন্ন l1 = a.x - a.w/2;
    ভিন্ন l2 = b.x - b.w/2;
    যদি (l1 > l2){
        d.dx -= 1;
        d.dw += .5;
    }
    ভিন্ন r1 = a.x + a.w/2;
    ভিন্ন r2 = b.x + b.w/2;
    যদি(r1 < r2){
        d.dx += 1;
        d.dw += .5;
    }
    যদি (l1 > r2) {
        d.dx = -1;
        d.dw = 0;
    }
    যদি (r1 < l2){
        d.dx = 1;
        d.dw = 0;
    }

    d.dy = 0;
    d.dh = 0;
    ভিন্ন t1 = a.y - a.h/2;
    ভিন্ন t2 = b.y - b.h/2;
    যদি (t1 > t2){
        d.dy -= 1;
        d.dh += .5;
    }
    ভিন্ন b1 = a.y + a.h/2;
    ভিন্ন b2 = b.y + b.h/2;
    যদি(b1 < b2){
        d.dy += 1;
        d.dh += .5;
    }
    যদি (t1 > b2) {
        d.dy = -1;
        d.dh = 0;
    }
    যদি (b1 < t2){
        d.dy = 1;
        d.dh = 0;
    }
    প্রত্যাগম d;
}

ভিন্ন overlap(ভিন্ন x1, ভিন্ন w1, ভিন্ন x2, ভিন্ন w2)
{
    ভিন্ন l1 = x1 - w1/2;
    ভিন্ন l2 = x2 - w2/2;
    ভিন্ন বাম = l1 > l2 ? l1 : l2;
    ভিন্ন r1 = x1 + w1/2;
    ভিন্ন r2 = x2 + w2/2;
    ভিন্ন ডান = r1 < r2 ? r1 : r2;
    প্রত্যাগম ডান - বাম;
}

ভিন্ন box_intersection(box a, box b)
{
    ভিন্ন w = overlap(a.x, a.w, b.x, b.w);
    ভিন্ন h = overlap(a.y, a.h, b.y, b.h);
    যদি(w < 0 || h < 0) প্রত্যাগম 0;
    ভিন্ন area = w*h;
    প্রত্যাগম area;
}

ভিন্ন box_union(box a, box b)
{
    ভিন্ন i = box_intersection(a, b);
    ভিন্ন u = a.w*a.h + b.w*b.h - i;
    প্রত্যাগম u;
}

ভিন্ন box_iou(box a, box b)
{
    প্রত্যাগম box_intersection(a, b)/box_union(a, b);
}

ভিন্ন box_rmse(box a, box b)
{
    প্রত্যাগম sqrt(pow(a.x-b.x, 2) + 
                pow(a.y-b.y, 2) + 
                pow(a.w-b.w, 2) + 
                pow(a.h-b.h, 2));
}

dbox dintersect(box a, box b)
{
    ভিন্ন w = overlap(a.x, a.w, b.x, b.w);
    ভিন্ন h = overlap(a.y, a.h, b.y, b.h);
    dbox dover = derivative(a, b);
    dbox di;

    di.dw = dover.dw*h;
    di.dx = dover.dx*h;
    di.dh = dover.dh*w;
    di.dy = dover.dy*w;

    প্রত্যাগম di;
}

dbox dunion(box a, box b)
{
    dbox du;

    dbox di = dintersect(a, b);
    du.dw = a.h - di.dw;
    du.dh = a.w - di.dh;
    du.dx = -di.dx;
    du.dy = -di.dy;

    প্রত্যাগম du;
}


শূন্য test_dunion()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dunion(a,b);
    printf("Union: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    ভিন্ন inter =  box_union(a, b);
    ভিন্ন xinter = box_union(dxa, b);
    ভিন্ন yinter = box_union(dya, b);
    ভিন্ন winter = box_union(dwa, b);
    ভিন্ন hinter = box_union(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    printf("Union Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}
শূন্য test_dintersect()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dintersect(a,b);
    printf("Inter: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    ভিন্ন inter =  box_intersection(a, b);
    ভিন্ন xinter = box_intersection(dxa, b);
    ভিন্ন yinter = box_intersection(dya, b);
    ভিন্ন winter = box_intersection(dwa, b);
    ভিন্ন hinter = box_intersection(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    printf("Inter Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}

শূন্য test_box()
{
    test_dintersect();
    test_dunion();
    box a = {0, 0, 1, 1};
    box dxa= {0+.00001, 0, 1, 1};
    box dya= {0, 0+.00001, 1, 1};
    box dwa= {0, 0, 1+.00001, 1};
    box dha= {0, 0, 1, 1+.00001};

    box b = {.5, 0, .2, .2};

    ভিন্ন iou = box_iou(a,b);
    iou = (1-iou)*(1-iou);
    printf("%f\n", iou);
    dbox d = diou(a, b);
    printf("%f %f %f %f\n", d.dx, d.dy, d.dw, d.dh);

    ভিন্ন xiou = box_iou(dxa, b);
    ভিন্ন yiou = box_iou(dya, b);
    ভিন্ন wiou = box_iou(dwa, b);
    ভিন্ন hiou = box_iou(dha, b);
    xiou = ((1-xiou)*(1-xiou) - iou)/(.00001);
    yiou = ((1-yiou)*(1-yiou) - iou)/(.00001);
    wiou = ((1-wiou)*(1-wiou) - iou)/(.00001);
    hiou = ((1-hiou)*(1-hiou) - iou)/(.00001);
    printf("manual %f %f %f %f\n", xiou, yiou, wiou, hiou);
}

dbox diou(box a, box b)
{
    ভিন্ন u = box_union(a,b);
    ভিন্ন i = box_intersection(a,b);
    dbox di = dintersect(a,b);
    dbox du = dunion(a,b);
    dbox dd = {0,0,0,0};

    যদি(i <= 0 || 1) {
        dd.dx = b.x - a.x;
        dd.dy = b.y - a.y;
        dd.dw = b.w - a.w;
        dd.dh = b.h - a.h;
        প্রত্যাগম dd;
    }

    dd.dx = 2*pow((1-(i/u)),1)*(di.dx*u - du.dx*i)/(u*u);
    dd.dy = 2*pow((1-(i/u)),1)*(di.dy*u - du.dy*i)/(u*u);
    dd.dw = 2*pow((1-(i/u)),1)*(di.dw*u - du.dw*i)/(u*u);
    dd.dh = 2*pow((1-(i/u)),1)*(di.dh*u - du.dh*i)/(u*u);
    প্রত্যাগম dd;
}


শূন্য do_nms(box *boxes, ভিন্ন **probs, পূর্ণাঙ্ক total, পূর্ণাঙ্ক classes, ভিন্ন thresh)
{
    পূর্ণাঙ্ক i, j, k;
    প্রতি(i = 0; i < total; ++i){
        পূর্ণাঙ্ক any = 0;
        প্রতি(k = 0; k < classes; ++k) any = any || (probs[i][k] > 0);
        যদি(!any) {
            অনুবর্ত;
        }
        প্রতি(j = i+1; j < total; ++j){
            যদি (box_iou(boxes[i], boxes[j]) > thresh){
                প্রতি(k = 0; k < classes; ++k){
                    যদি (probs[i][k] < probs[j][k]) probs[i][k] = 0;
                    অন্যথা probs[j][k] = 0;
                }
            }
        }
    }
}

box encode_box(box b, box anchor)
{
    box encode;
    encode.x = (b.x - anchor.x) / anchor.w;
    encode.y = (b.y - anchor.y) / anchor.h;
    encode.w = log2(b.w / anchor.w);
    encode.h = log2(b.h / anchor.h);
    প্রত্যাগম encode;
}

box decode_box(box b, box anchor)
{
    box decode;
    decode.x = b.x * anchor.w + anchor.x;
    decode.y = b.y * anchor.h + anchor.y;
    decode.w = pow(2., b.w) * anchor.w;
    decode.h = pow(2., b.h) * anchor.h;
    প্রত্যাগম decode;
}

#ضمن "box.h"
#ضمن <stdio.h>
#ضمن <math.h>
#ضمن <stdlib.h>

صحيح nms_comparator(ثابت فراغ *pa, ثابت فراغ *pb)
{
    detection a = *(detection *)pa;
    detection b = *(detection *)pb;
    عائم diff = 0;
    𐡄𐡍(b.sort_class >= 0){
        diff = a.prob[b.sort_class] - b.prob[b.sort_class];
    } وإلا {
        diff = a.objectness - b.objectness;
    }
    𐡄𐡍(diff < 0) أرجع 1;
    وإلا 𐡄𐡍(diff > 0) أرجع -1;
    أرجع 0;
}

فراغ do_nms_obj(detection *dets, صحيح total, صحيح classes, عائم thresh)
{
    صحيح i, j, k;
    k = total-1;
    لكل(i = 0; i <= k; ++i){
        𐡄𐡍(dets[i].objectness == 0){
            detection swap = dets[i];
            dets[i] = dets[k];
            dets[k] = swap;
            --k;
            --i;
        }
    }
    total = k+1;

    لكل(i = 0; i < total; ++i){
        dets[i].sort_class = -1;
    }

    qsort(dets, total, حجم(detection), nms_comparator);
    لكل(i = 0; i < total; ++i){
        𐡄𐡍(dets[i].objectness == 0) استمر;
        box a = dets[i].bbox;
        لكل(j = i+1; j < total; ++j){
            𐡄𐡍(dets[j].objectness == 0) استمر;
            box b = dets[j].bbox;
            𐡄𐡍 (box_iou(a, b) > thresh){
                dets[j].objectness = 0;
                لكل(k = 0; k < classes; ++k){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}


فراغ do_nms_sort(detection *dets, صحيح total, صحيح classes, عائم thresh)
{
    صحيح i, j, k;
    k = total-1;
    لكل(i = 0; i <= k; ++i){
        𐡄𐡍(dets[i].objectness == 0){
            detection swap = dets[i];
            dets[i] = dets[k];
            dets[k] = swap;
            --k;
            --i;
        }
    }
    total = k+1;

    لكل(k = 0; k < classes; ++k){
        لكل(i = 0; i < total; ++i){
            dets[i].sort_class = k;
        }
        qsort(dets, total, حجم(detection), nms_comparator);
        لكل(i = 0; i < total; ++i){
            𐡄𐡍(dets[i].prob[k] == 0) استمر;
            box a = dets[i].bbox;
            لكل(j = i+1; j < total; ++j){
                box b = dets[j].bbox;
                𐡄𐡍 (box_iou(a, b) > thresh){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}

box float_to_box(عائم *f, صحيح stride)
{
    box b = {0};
    b.x = f[0];
    b.y = f[1*stride];
    b.w = f[2*stride];
    b.h = f[3*stride];
    أرجع b;
}

dbox derivative(box a, box b)
{
    dbox d;
    d.dx = 0;
    d.dw = 0;
    عائم l1 = a.x - a.w/2;
    عائم l2 = b.x - b.w/2;
    𐡄𐡍 (l1 > l2){
        d.dx -= 1;
        d.dw += .5;
    }
    عائم r1 = a.x + a.w/2;
    عائم r2 = b.x + b.w/2;
    𐡄𐡍(r1 < r2){
        d.dx += 1;
        d.dw += .5;
    }
    𐡄𐡍 (l1 > r2) {
        d.dx = -1;
        d.dw = 0;
    }
    𐡄𐡍 (r1 < l2){
        d.dx = 1;
        d.dw = 0;
    }

    d.dy = 0;
    d.dh = 0;
    عائم t1 = a.y - a.h/2;
    عائم t2 = b.y - b.h/2;
    𐡄𐡍 (t1 > t2){
        d.dy -= 1;
        d.dh += .5;
    }
    عائم b1 = a.y + a.h/2;
    عائم b2 = b.y + b.h/2;
    𐡄𐡍(b1 < b2){
        d.dy += 1;
        d.dh += .5;
    }
    𐡄𐡍 (t1 > b2) {
        d.dy = -1;
        d.dh = 0;
    }
    𐡄𐡍 (b1 < t2){
        d.dy = 1;
        d.dh = 0;
    }
    أرجع d;
}

عائم overlap(عائم x1, عائم w1, عائم x2, عائم w2)
{
    عائم l1 = x1 - w1/2;
    عائم l2 = x2 - w2/2;
    عائم left = l1 > l2 ? l1 : l2;
    عائم r1 = x1 + w1/2;
    عائم r2 = x2 + w2/2;
    عائم right = r1 < r2 ? r1 : r2;
    أرجع right - left;
}

عائم box_intersection(box a, box b)
{
    عائم w = overlap(a.x, a.w, b.x, b.w);
    عائم h = overlap(a.y, a.h, b.y, b.h);
    𐡄𐡍(w < 0 || h < 0) أرجع 0;
    عائم area = w*h;
    أرجع area;
}

عائم box_union(box a, box b)
{
    عائم i = box_intersection(a, b);
    عائم u = a.w*a.h + b.w*b.h - i;
    أرجع u;
}

عائم box_iou(box a, box b)
{
    أرجع box_intersection(a, b)/box_union(a, b);
}

عائم box_rmse(box a, box b)
{
    أرجع sqrt(pow(a.x-b.x, 2) + 
                pow(a.y-b.y, 2) + 
                pow(a.w-b.w, 2) + 
                pow(a.h-b.h, 2));
}

dbox dintersect(box a, box b)
{
    عائم w = overlap(a.x, a.w, b.x, b.w);
    عائم h = overlap(a.y, a.h, b.y, b.h);
    dbox dover = derivative(a, b);
    dbox di;

    di.dw = dover.dw*h;
    di.dx = dover.dx*h;
    di.dh = dover.dh*w;
    di.dy = dover.dy*w;

    أرجع di;
}

dbox dunion(box a, box b)
{
    dbox du;

    dbox di = dintersect(a, b);
    du.dw = a.h - di.dw;
    du.dh = a.w - di.dh;
    du.dx = -di.dx;
    du.dy = -di.dy;

    أرجع du;
}


فراغ test_dunion()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dunion(a,b);
    printf("Union: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    عائم inter =  box_union(a, b);
    عائم xinter = box_union(dxa, b);
    عائم yinter = box_union(dya, b);
    عائم winter = box_union(dwa, b);
    عائم hinter = box_union(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    printf("Union Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}
فراغ test_dintersect()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dintersect(a,b);
    printf("Inter: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    عائم inter =  box_intersection(a, b);
    عائم xinter = box_intersection(dxa, b);
    عائم yinter = box_intersection(dya, b);
    عائم winter = box_intersection(dwa, b);
    عائم hinter = box_intersection(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    printf("Inter Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}

فراغ test_box()
{
    test_dintersect();
    test_dunion();
    box a = {0, 0, 1, 1};
    box dxa= {0+.00001, 0, 1, 1};
    box dya= {0, 0+.00001, 1, 1};
    box dwa= {0, 0, 1+.00001, 1};
    box dha= {0, 0, 1, 1+.00001};

    box b = {.5, 0, .2, .2};

    عائم iou = box_iou(a,b);
    iou = (1-iou)*(1-iou);
    printf("%f\n", iou);
    dbox d = diou(a, b);
    printf("%f %f %f %f\n", d.dx, d.dy, d.dw, d.dh);

    عائم xiou = box_iou(dxa, b);
    عائم yiou = box_iou(dya, b);
    عائم wiou = box_iou(dwa, b);
    عائم hiou = box_iou(dha, b);
    xiou = ((1-xiou)*(1-xiou) - iou)/(.00001);
    yiou = ((1-yiou)*(1-yiou) - iou)/(.00001);
    wiou = ((1-wiou)*(1-wiou) - iou)/(.00001);
    hiou = ((1-hiou)*(1-hiou) - iou)/(.00001);
    printf("manual %f %f %f %f\n", xiou, yiou, wiou, hiou);
}

dbox diou(box a, box b)
{
    عائم u = box_union(a,b);
    عائم i = box_intersection(a,b);
    dbox di = dintersect(a,b);
    dbox du = dunion(a,b);
    dbox dd = {0,0,0,0};

    𐡄𐡍(i <= 0 || 1) {
        dd.dx = b.x - a.x;
        dd.dy = b.y - a.y;
        dd.dw = b.w - a.w;
        dd.dh = b.h - a.h;
        أرجع dd;
    }

    dd.dx = 2*pow((1-(i/u)),1)*(di.dx*u - du.dx*i)/(u*u);
    dd.dy = 2*pow((1-(i/u)),1)*(di.dy*u - du.dy*i)/(u*u);
    dd.dw = 2*pow((1-(i/u)),1)*(di.dw*u - du.dw*i)/(u*u);
    dd.dh = 2*pow((1-(i/u)),1)*(di.dh*u - du.dh*i)/(u*u);
    أرجع dd;
}


فراغ do_nms(box *boxes, عائم **probs, صحيح total, صحيح classes, عائم thresh)
{
    صحيح i, j, k;
    لكل(i = 0; i < total; ++i){
        صحيح any = 0;
        لكل(k = 0; k < classes; ++k) any = any || (probs[i][k] > 0);
        𐡄𐡍(!any) {
            استمر;
        }
        لكل(j = i+1; j < total; ++j){
            𐡄𐡍 (box_iou(boxes[i], boxes[j]) > thresh){
                لكل(k = 0; k < classes; ++k){
                    𐡄𐡍 (probs[i][k] < probs[j][k]) probs[i][k] = 0;
                    وإلا probs[j][k] = 0;
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
    أرجع encode;
}

box decode_box(box b, box anchor)
{
    box decode;
    decode.x = b.x * anchor.w + anchor.x;
    decode.y = b.y * anchor.h + anchor.y;
    decode.w = pow(2., b.w) * anchor.w;
    decode.h = pow(2., b.h) * anchor.h;
    أرجع decode;
}

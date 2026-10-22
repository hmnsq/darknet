#شامل "box.h"
#شامل <stdio.h>
#شامل <math.h>
#شامل <stdlib.h>

صحیح nms_comparator(ثابت خالی *pa, ثابت خالی *pb)
{
    detection a = *(detection *)pa;
    detection b = *(detection *)pb;
    اعشاری diff = 0;
    اگر(b.ترتیب_دو_class >= 0){
        diff = a.prob[b.ترتیب_دو_class] - b.prob[b.ترتیب_دو_class];
    } ورنہ {
        diff = a.objectness - b.objectness;
    }
    اگر(diff < 0) واپس 1;
    ورنہ اگر(diff > 0) واپس -1;
    واپس 0;
}

خالی do_nms_obj(detection *dets, صحیح کل, صحیح classes, اعشاری thresh)
{
    صحیح i, j, k;
    k = کل-1;
    برائے(i = 0; i <= k; ++i){
        اگر(dets[i].objectness == 0){
            detection تبادلہ = dets[i];
            dets[i] = dets[k];
            dets[k] = تبادلہ;
            --k;
            --i;
        }
    }
    کل = k+1;

    برائے(i = 0; i < کل; ++i){
        dets[i].ترتیب_دو_class = -1;
    }

    qsort(dets, کل, حجم(detection), nms_comparator);
    برائے(i = 0; i < کل; ++i){
        اگر(dets[i].objectness == 0) جاری;
        box a = dets[i].bbox;
        برائے(j = i+1; j < کل; ++j){
            اگر(dets[j].objectness == 0) جاری;
            box b = dets[j].bbox;
            اگر (box_iou(a, b) > thresh){
                dets[j].objectness = 0;
                برائے(k = 0; k < classes; ++k){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}


خالی do_nms_ترتیب_دو(detection *dets, صحیح کل, صحیح classes, اعشاری thresh)
{
    صحیح i, j, k;
    k = کل-1;
    برائے(i = 0; i <= k; ++i){
        اگر(dets[i].objectness == 0){
            detection تبادلہ = dets[i];
            dets[i] = dets[k];
            dets[k] = تبادلہ;
            --k;
            --i;
        }
    }
    کل = k+1;

    برائے(k = 0; k < classes; ++k){
        برائے(i = 0; i < کل; ++i){
            dets[i].ترتیب_دو_class = k;
        }
        qsort(dets, کل, حجم(detection), nms_comparator);
        برائے(i = 0; i < کل; ++i){
            اگر(dets[i].prob[k] == 0) جاری;
            box a = dets[i].bbox;
            برائے(j = i+1; j < کل; ++j){
                box b = dets[j].bbox;
                اگر (box_iou(a, b) > thresh){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}

box float_مقصد_box(اعشاری *f, صحیح stride)
{
    box b = {0};
    b.x = f[0];
    b.y = f[1*stride];
    b.w = f[2*stride];
    b.h = f[3*stride];
    واپس b;
}

dbox derivative(box a, box b)
{
    dbox d;
    d.dx = 0;
    d.dw = 0;
    اعشاری l1 = a.x - a.w/2;
    اعشاری l2 = b.x - b.w/2;
    اگر (l1 > l2){
        d.dx -= 1;
        d.dw += .5;
    }
    اعشاری r1 = a.x + a.w/2;
    اعشاری r2 = b.x + b.w/2;
    اگر(r1 < r2){
        d.dx += 1;
        d.dw += .5;
    }
    اگر (l1 > r2) {
        d.dx = -1;
        d.dw = 0;
    }
    اگر (r1 < l2){
        d.dx = 1;
        d.dw = 0;
    }

    d.dy = 0;
    d.dh = 0;
    اعشاری t1 = a.y - a.h/2;
    اعشاری t2 = b.y - b.h/2;
    اگر (t1 > t2){
        d.dy -= 1;
        d.dh += .5;
    }
    اعشاری b1 = a.y + a.h/2;
    اعشاری b2 = b.y + b.h/2;
    اگر(b1 < b2){
        d.dy += 1;
        d.dh += .5;
    }
    اگر (t1 > b2) {
        d.dy = -1;
        d.dh = 0;
    }
    اگر (b1 < t2){
        d.dy = 1;
        d.dh = 0;
    }
    واپس d;
}

اعشاری overlap(اعشاری x1, اعشاری w1, اعشاری x2, اعشاری w2)
{
    اعشاری l1 = x1 - w1/2;
    اعشاری l2 = x2 - w2/2;
    اعشاری بایاں = l1 > l2 ? l1 : l2;
    اعشاری r1 = x1 + w1/2;
    اعشاری r2 = x2 + w2/2;
    اعشاری دایاں = r1 < r2 ? r1 : r2;
    واپس دایاں - بایاں;
}

اعشاری box_intersection(box a, box b)
{
    اعشاری w = overlap(a.x, a.w, b.x, b.w);
    اعشاری h = overlap(a.y, a.h, b.y, b.h);
    اگر(w < 0 || h < 0) واپس 0;
    اعشاری area = w*h;
    واپس area;
}

اعشاری box_union(box a, box b)
{
    اعشاری i = box_intersection(a, b);
    اعشاری u = a.w*a.h + b.w*b.h - i;
    واپس u;
}

اعشاری box_iou(box a, box b)
{
    واپس box_intersection(a, b)/box_union(a, b);
}

اعشاری box_rmse(box a, box b)
{
    واپس جذر(قوت(a.x-b.x, 2) + 
                قوت(a.y-b.y, 2) + 
                قوت(a.w-b.w, 2) + 
                قوت(a.h-b.h, 2));
}

dbox dintersect(box a, box b)
{
    اعشاری w = overlap(a.x, a.w, b.x, b.w);
    اعشاری h = overlap(a.y, a.h, b.y, b.h);
    dbox dover = derivative(a, b);
    dbox di;

    di.dw = dover.dw*h;
    di.dx = dover.dx*h;
    di.dh = dover.dh*w;
    di.dy = dover.dy*w;

    واپس di;
}

dbox dunion(box a, box b)
{
    dbox du;

    dbox di = dintersect(a, b);
    du.dw = a.h - di.dw;
    du.dh = a.w - di.dh;
    du.dx = -di.dx;
    du.dy = -di.dy;

    واپس du;
}


خالی test_dunion()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dunion(a,b);
    لکھو("Union: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    اعشاری inter =  box_union(a, b);
    اعشاری xinter = box_union(dxa, b);
    اعشاری yinter = box_union(dya, b);
    اعشاری winter = box_union(dwa, b);
    اعشاری hinter = box_union(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    لکھو("Union Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}
خالی test_dintersect()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dintersect(a,b);
    لکھو("Inter: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    اعشاری inter =  box_intersection(a, b);
    اعشاری xinter = box_intersection(dxa, b);
    اعشاری yinter = box_intersection(dya, b);
    اعشاری winter = box_intersection(dwa, b);
    اعشاری hinter = box_intersection(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    لکھو("Inter Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}

خالی test_box()
{
    test_dintersect();
    test_dunion();
    box a = {0, 0, 1, 1};
    box dxa= {0+.00001, 0, 1, 1};
    box dya= {0, 0+.00001, 1, 1};
    box dwa= {0, 0, 1+.00001, 1};
    box dha= {0, 0, 1, 1+.00001};

    box b = {.5, 0, .2, .2};

    اعشاری iou = box_iou(a,b);
    iou = (1-iou)*(1-iou);
    لکھو("%f\n", iou);
    dbox d = diou(a, b);
    لکھو("%f %f %f %f\n", d.dx, d.dy, d.dw, d.dh);

    اعشاری xiou = box_iou(dxa, b);
    اعشاری yiou = box_iou(dya, b);
    اعشاری wiou = box_iou(dwa, b);
    اعشاری hiou = box_iou(dha, b);
    xiou = ((1-xiou)*(1-xiou) - iou)/(.00001);
    yiou = ((1-yiou)*(1-yiou) - iou)/(.00001);
    wiou = ((1-wiou)*(1-wiou) - iou)/(.00001);
    hiou = ((1-hiou)*(1-hiou) - iou)/(.00001);
    لکھو("manual %f %f %f %f\n", xiou, yiou, wiou, hiou);
}

dbox diou(box a, box b)
{
    اعشاری u = box_union(a,b);
    اعشاری i = box_intersection(a,b);
    dbox di = dintersect(a,b);
    dbox du = dunion(a,b);
    dbox dd = {0,0,0,0};

    اگر(i <= 0 || 1) {
        dd.dx = b.x - a.x;
        dd.dy = b.y - a.y;
        dd.dw = b.w - a.w;
        dd.dh = b.h - a.h;
        واپس dd;
    }

    dd.dx = 2*قوت((1-(i/u)),1)*(di.dx*u - du.dx*i)/(u*u);
    dd.dy = 2*قوت((1-(i/u)),1)*(di.dy*u - du.dy*i)/(u*u);
    dd.dw = 2*قوت((1-(i/u)),1)*(di.dw*u - du.dw*i)/(u*u);
    dd.dh = 2*قوت((1-(i/u)),1)*(di.dh*u - du.dh*i)/(u*u);
    واپس dd;
}


خالی do_nms(box *boxes, اعشاری **probs, صحیح کل, صحیح classes, اعشاری thresh)
{
    صحیح i, j, k;
    برائے(i = 0; i < کل; ++i){
        صحیح any = 0;
        برائے(k = 0; k < classes; ++k) any = any || (probs[i][k] > 0);
        اگر(!any) {
            جاری;
        }
        برائے(j = i+1; j < کل; ++j){
            اگر (box_iou(boxes[i], boxes[j]) > thresh){
                برائے(k = 0; k < classes; ++k){
                    اگر (probs[i][k] < probs[j][k]) probs[i][k] = 0;
                    ورنہ probs[j][k] = 0;
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
    واپس encode;
}

box decode_box(box b, box anchor)
{
    box decode;
    decode.x = b.x * anchor.w + anchor.x;
    decode.y = b.y * anchor.h + anchor.y;
    decode.w = قوت(2., b.w) * anchor.w;
    decode.h = قوت(2., b.h) * anchor.h;
    واپس decode;
}

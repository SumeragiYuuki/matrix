
#define _MAT33_ANIMATION_WALK
#define _VECMAT33

#ifdef _VECMAT33
#include"libOne.h"
//２次元ベクトル（２行１列の行列として考えることもできる）
class VEC {
public:
    float x, y;
    VEC(float x = 0, float y = 0) {
        this->x = x;
        this->y = y;
    }
    VEC operator-(const VEC& v) {
        return VEC(x - v.x, y - v.y);
    }
    float mag() {
        return sqrt(x * x + y * y);
    }
};

//３行３列の行列
class MAT33 {
public:
    float _11, _12, _13;
    float _21, _22, _23;
    float _31, _32, _33;
    MAT33(float _11 = 1, float _12 = 0, float _13 = 0,
        float _21 = 0, float _22 = 1, float _23 = 0,
        float _31 = 0, float _32 = 0, float _33 = 1) {
        this->_11 = _11; this->_12 = _12; this->_13 = _13;
        this->_21 = _21; this->_22 = _22; this->_23 = _23;
        this->_31 = _31; this->_32 = _32; this->_33 = _33;
    }
    //単位行列をつくる
    void identity() {
        _11 = 1; _12 = 0; _13 = 0;
        _21 = 0; _22 = 1; _23 = 0;
        _31 = 0; _32 = 0; _33 = 1;
    }
    //拡大縮小行列をつくる
    void scaling(float sx, float sy) {
        _11 = sx; _12 = 0; _13 = 0;
        _21 = 0; _22 = sy; _23 = 0;
        _31 = 0; _32 = 0; _33 = 1;
    }
    //回転行列をつくる
    void rotate(float angle) {
        float c = cos(angle);
        float s = sin(angle);
        _11 = c; _12 = -s; _13 = 0;
        _21 = s; _22 = c; _23 = 0;
        _31 = 0; _32 = 0; _33 = 1;
    }
    //平行移動行列をつくる
    void translate(float x, float y) {
        _11 = 1; _12 = 0; _13 = x;
        _21 = 0; _22 = 1; _23 = y;
        _31 = 0; _32 = 0; _33 = 1;
    }
    //「３行３列行列」と「２行１列行列（２次元ベクトル）＋１行」の掛け算
    VEC operator*(const VEC& v) const {
        return VEC(
            _11 * v.x + _12 * v.y + _13,
            _21 * v.x + _22 * v.y + _23
        );
    }
    //３行３列行列どうしの掛け算
    MAT33 operator*(const MAT33& m) {
        return MAT33(
            //１行目
            _11 * m._11 + _12 * m._21 + _13 * m._31,
            _11 * m._12 + _12 * m._22 + _13 * m._32,
            _11 * m._13 + _12 * m._23 + _13 * m._33,
            //２行目
            _21 * m._11 + _22 * m._21 + _23 * m._31,
            _21 * m._12 + _22 * m._22 + _23 * m._32,
            _21 * m._13 + _22 * m._23 + _23 * m._33,
            //３行目
            _31 + m._11 + _32 * m._21 + _33 * m._31,
            _31 + m._12 + _32 * m._22 + _33 * m._32,
            _31 + m._13 + _32 * m._23 + _33 * m._33
        );
    }
    //--------------------------------------------------------
    //最適化されたMAT33同士の掛け算
    //--------------------------------------------------------
    void mulTranslate(float x, float y) {
        // |_11 _12 _13|    |1  0  x| 　 |_11  _12  _11x+_12y+_13| 
        // |_21 _22 _23| × |0  1  y| ＝ |_21  _22  _21x+_22y+_23|
        // |  0   0   1|    |0  0  1|    |  0    0              1|
        _13 += _11 * x + _12 * y;
        _23 += _21 * x + _22 * y;
    }
    void mulScaling(float x, float y) {
        // |_11 _12 _13|    |x  0  0| 　 |_11x _12y _13|
        // |_21 _22 _23| × |0  y  0| ＝ |_21x _22y _23|
        // |  0   0   1|    |0  0  1|    |  0    0    1|
        _11 *= x;    _12 *= y;
        _21 *= x;    _22 *= y;
    }
    void mulRotate(float angle) {
        // |_11 _12 _13|    |c -s  0| 　 |_11c+_12s   _11(-s)+_12c   _13|
        // |_21 _22 _23| × |s  c  0| ＝ |_21c+_22s   _21(-s)+_22c   _23|
        // |  0   0   1|    |0  0  1|    |        0             0      1|
        float c = cos(angle);
        float s = sin(angle);
        float tmp;
        //1行目
        tmp = _11 * c + _12 * s;
        _12 = _11 * -s + _12 * c;
        _11 = tmp;
        //2行目
        tmp = _21 * c + _22 * s;
        _22 = _21 * -s + _22 * c;
        _21 = tmp;
    }

};
#endif

#ifdef _MAT33_ANIMATION_WALK
class SEG {
    VEC o_sp, o_ep;
    VEC sp, ep;
    COLOR c[12];
public:
    SEG() {
        o_sp = VEC(0, 0);
        o_ep = VEC(0, -1);
        angleMode(DEGREES);
        colorMode(HSV);
        float theta = 360.0f / 12;
        for (int i = 0; i < 12; i++) {
            c[i] = COLOR(theta * i, 128, 255);
        }
    }
    void draw(const MAT33& mat, int colorNo, float sw) {
        sp = mat * o_sp;
        ep = mat * o_ep;
        strokeWeight(sw);
        stroke(c[colorNo]);
        mathLine(sp.x, sp.y, ep.x, ep.y);
        strokeWeight(sw / 3);
        stroke(0, 0, 128);
        mathPoint(sp.x, sp.y);
    }
};

void gmain() {
    window(1000, 1000, 1);
    SEG seg;
    //線分の長さを決める行列
    MAT33 mWaistLen, mLegLen, mSpineLen, mNeckLen, mArmLen;
    mWaistLen.scaling(1, 0.2f);
    mLegLen.scaling(1, 0.5f);
    mSpineLen.scaling(1, -0.4f);
    mNeckLen.scaling(1, -0.4f);
    mArmLen.scaling(1, 0.4f);

    //回転移動させる階層行列
    MAT33 mMaster,
        mSpine,
        mNeck,
        mArm1R,
        mArm2R,
        mArm1L,
        mArm2L,
        mWaist,
        mLeg1R,
        mLeg2R,
        mLeg1L,
        mLeg2L;

    float size = 1;
    float sw = 45 * size;
    angleMode(DEGREES);
    float theta = 0;
    int RL = 1;//停止状態での左右変換
    float Mangle = 0;
    int Sangle = 0;//背骨アングル
    int Nangle = 0;//首アングル
    float MtsX = 0;
    float MtsY = 0;
    int stop = 1;
    while (notQuit) {
        clear();
        mathAxis(2.3f, 255);
        //コマンド
        
        //傾き
        //if (isPress(KEY_RIGHT) && Mangle >= -45) Mangle += -0.25f;
        //if (isPress(KEY_LEFT) && Mangle <= 45) Mangle += 0.25f;
        ////拡大縮小
        //if (isPress(KEY_UP) && size <= 2) size += 0.0025f;
        //if (isPress(KEY_DOWN) && size >= 0.5f) size += -0.0025f;
         //上下左右移動
         if (isPress(KEY_D) && MtsX <= 2) {
             MtsX += 0.025f;
             RL = 1;
             theta += 4 * stop;
         }
         else if (isPress(KEY_A) && MtsX >= -2) {
             MtsX += -0.025f;
             RL = -1;
             theta += 4 * stop;
         }
         else {
             if (theta) {
             }
         }
        if (isPress(KEY_W) && MtsY <= 2) MtsY += 0.025f;
        if (isPress(KEY_S) && MtsY >= -2) MtsY += -0.025f;
        if (!isPress(KEY_W) && !isPress(KEY_S)) {
            Sangle = 0;
            Nangle = 0;
        }
        else if (isPress(KEY_W) && RL == 1 || isPress(KEY_S) && RL == -1) {
            Sangle = -5;
            Nangle = -20;
        }
        else if (isPress(KEY_S) && RL == 1 || isPress(KEY_W) && RL == -1) {
            Sangle = 20;
            Nangle = 5;
        }
        //歩きON/OFF
        if (isTrigger(KEY_SPACE)) {
            int tmp = stop + 1;
            stop = tmp % 2;
        }
        //リセット
        if (isTrigger(KEY_ENTER)) {
            size = 1;
            RL = 1;
            Mangle = 0;
            MtsX = 0;
            MtsY = 0;
            stop = 1;
        }

        //回転係数
        float s = sin(theta);
        float legs = s < 0 ? s / 2 : s; //太ももの動き
        float s_ntoz = s < 0 ? 0 : s;  //負の値は0にする
        float ns = -s;
        float legns = ns < 0 ? ns / 2 : ns; //太ももの動き
        float ns_ntoz = ns < 0 ? 0 : ns;  //負の値は0にする
        

        //基準座標
        mMaster.translate(MtsX, MtsY);
        mMaster.mulRotate(Mangle);
        mMaster.mulScaling(size, size);
        //背骨
        mSpine.translate(0, 1.2f);
        mSpine.mulRotate(Sangle);
        //首＆頭
        mNeck.translate(0, 0.4f);
        mNeck.mulRotate(Nangle);
        //右手
        mArm1R.translate(0, 0.4f);
        mArm1R.mulRotate(RL * 30 * ns);
        mArm2R.translate(0, -0.4f);
        mArm2R.mulRotate(RL * 30 * ns_ntoz);
        //左手
        mArm1L.translate(0, 0.4f);
        mArm1L.mulRotate(RL * 30 * s);
        mArm2L.translate(0, -0.4f);
        mArm2L.mulRotate(RL * 30 * s_ntoz);
        //腰
        mWaist.translate(0, 1.2f);
        mWaist.mulRotate(0);
        //右足
        mLeg1R.translate(0, -0.2f);
        mLeg1R.mulRotate(RL * 30 * legs);
        mLeg2R.translate(0, -0.5f);
        mLeg2R.mulRotate(RL * -40 * ns_ntoz);
        //左足
        mLeg1L.translate(0, -0.2f);
        mLeg1L.mulRotate(RL * 30 * legns);
        mLeg2L.translate(0, -0.5f);
        mLeg2L.mulRotate(RL * -40 * s_ntoz);

        if (RL == 1) {
            seg.draw(mMaster * mSpine * mArm1L * mArmLen, 9, sw);          //左上腕
            seg.draw(mMaster * mSpine * mArm1L * mArm2L * mArmLen, 10, sw);//左下腕
            seg.draw(mMaster * mWaist * mWaistLen, 0, sw);                 //腰
            seg.draw(mMaster * mWaist * mLeg1L * mLegLen, 3, sw);          //左太もも
            seg.draw(mMaster * mWaist * mLeg1L * mLeg2L * mLegLen, 5, sw); //左足
            seg.draw(mMaster * mWaist * mLeg1R * mLegLen, 1, sw);          //右太もも
            seg.draw(mMaster * mWaist * mLeg1R * mLeg2R * mLegLen, 2, sw); //右足
            seg.draw(mMaster * mSpine * mSpineLen, 5, sw);                 //背骨
            seg.draw(mMaster * mSpine * mNeck * mNeckLen, 6, sw);          //首＆頭
            seg.draw(mMaster * mSpine * mArm1R * mArmLen, 7, sw);          //右上腕
            seg.draw(mMaster * mSpine * mArm1R * mArm2R * mArmLen, 8, sw); //右下腕
        }
        else if (RL==-1) {
            seg.draw(mMaster * mSpine * mArm1R * mArmLen, 7, sw);          //右上腕
            seg.draw(mMaster * mSpine * mArm1R * mArm2R * mArmLen, 8, sw); //右下腕
            seg.draw(mMaster * mWaist * mWaistLen, 0, sw);                 //腰
            seg.draw(mMaster * mWaist * mLeg1R * mLegLen, 1, sw);          //右太もも
            seg.draw(mMaster * mWaist * mLeg1R * mLeg2R * mLegLen, 2, sw); //右足
            seg.draw(mMaster * mWaist * mLeg1L * mLegLen, 3, sw);          //左太もも
            seg.draw(mMaster * mWaist * mLeg1L * mLeg2L * mLegLen, 5, sw); //左足
            seg.draw(mMaster * mSpine * mSpineLen, 5, sw);                 //背骨
            seg.draw(mMaster * mSpine * mNeck * mNeckLen, 6, sw);          //首＆頭
            seg.draw(mMaster * mSpine * mArm1L * mArmLen, 9, sw);          //左上腕
            seg.draw(mMaster * mSpine * mArm1L * mArm2L * mArmLen, 10, sw);//左下腕
        }
    }
}
#endif

#ifdef _MAT33_ANIMATION
class SEG {
    VEC o_sp, o_ep;
    VEC sp, ep;
    COLOR c[12];
public:
    SEG() {
        o_sp = VEC(0, 0);
        o_ep = VEC(0, -1);
        angleMode(DEGREES);
        colorMode(HSV);
        float theta = 360.0f / 12;
        for (int i = 0; i < 12; i++) {
            c[i] = COLOR(theta * i, 128, 255);
        }
    }
    void draw(const MAT33& mat, int colorNo, float sw) {
        sp = mat * o_sp;
        ep = mat * o_ep;
        strokeWeight(sw);
        stroke(c[colorNo]);
        mathLine(sp.x, sp.y, ep.x, ep.y);
        strokeWeight(sw / 3);
        stroke(0, 0,128);
        mathPoint(sp.x, sp.y);
    }
};

void gmain() {
    window(1000, 1000, 1);
    SEG seg;
    //線分の長さを決める行列
    MAT33 mWaistLen, mLegLen, mSpineLen, mNeckLen, mArmLen;
    mWaistLen.scaling(1, 0.2f);
    mLegLen.scaling(1, 0.5f);
    mSpineLen.scaling(1, -0.4f);
    mNeckLen.scaling(1, -0.4f);
    mArmLen.scaling(1, 0.4f);

    //回転移動させる階層行列
    MAT33 mMaster,
            mSpine,
                mNeck,
                mArm1R,
                    mArm2R,
                mArm1L,
                    mArm2L,
            mWaist,
                mLeg1R,
                    mLeg2R,
                mLeg1L,
                    mLeg2L;

    float size = 1;
    float sw = 45 * size;
    float theta = 0;
    int RL = 1;
    float Mangle = 0;
    int Sangle = 0;
    int Nangle = 0;
    float MtsX = 0;
    float MtsY = 0;
    int stop = 1;
    while (notQuit) {
        clear();
        mathAxis(2.3f, 255);
        //コマンド
        //左右の向き
        if (isTrigger(KEY_R)) RL = -1;
        if (isTrigger(KEY_L)) RL = 1;
        //傾き
        if (isPress(KEY_RIGHT) && Mangle >= -45) Mangle += -0.25f;
        if (isPress(KEY_LEFT) && Mangle <= 45) Mangle += 0.25f;
        //拡大縮小
        if (isPress(KEY_UP) && size <= 2) size += 0.0025f;
        if (isPress(KEY_DOWN) && size >= 0.5f) size += -0.0025f;
        //上下左右移動
        if (isPress(KEY_D) && MtsX <= 2) {
            MtsX += 0.025f;
            RL = -1;
        }
        if (isPress(KEY_A) && MtsX >= -2) {
            MtsX += -0.025f;
            RL = 1;
        }
        if (isPress(KEY_W) && MtsY <= 2) MtsY += 0.025f;
        if (isPress(KEY_S) && MtsY >= -2) MtsY += -0.025f;
        if (!isPress(KEY_W) && !isPress(KEY_S)) {
            Sangle = 0;
            Nangle = 0;
        }
        else if (isPress(KEY_W) && RL == 1 || isPress(KEY_S) && RL == -1) {
            Sangle = -5;
            Nangle = -20;
        }
        else if (isPress(KEY_S) && RL == 1 || isPress(KEY_W) && RL == -1) {
            Sangle = 20;
            Nangle = 5;
        }
        //歩きON/OFF
        if (isTrigger(KEY_SPACE)) {
            int tmp = stop + 1;
            stop = tmp % 2;
        }
        //リセット
        if (isTrigger(KEY_ENTER)) {
            size = 1;
            RL = 1;
            Mangle = 0;
            MtsX = 0;
            MtsY = 0;
            stop = 1;
        }

        //回転係数
        float s = sin(theta);
        float s_ntoz = s < 0 ? 0 : s;  //負の値は0にする
        float ns = -s;
        float ns_ntoz = ns < 0 ? 0 : ns;  //負の値は0にする
        theta += 4 * stop;

        mMaster.translate(MtsX, MtsY);
        mMaster.mulRotate(Mangle);
        mMaster.mulScaling(size, size);

        mSpine.translate(0, 1.2f);
        mSpine.mulRotate(Sangle);
        mNeck.translate(0, 0.4f);
        mNeck.mulRotate(Nangle);
        mArm1R.translate(0, 0.4f);
        mArm1R.mulRotate(RL * -30 * ns);
        mArm2R.translate(0, -0.4f);
        mArm2R.mulRotate(RL * -30 * ns_ntoz);
        mArm1L.translate(0, 0.4f);
        mArm1L.mulRotate(RL * -30 * s);
        mArm2L.translate(0, -0.4f);
        mArm2L.mulRotate(RL * -30 * s_ntoz);


        mWaist.translate(0, 1.2f);
        mWaist.mulRotate(0);
        mLeg1R.translate(0, -0.2f);
        mLeg1R.mulRotate(RL * -30 * s_ntoz);
        mLeg2R.translate(0, -0.5f);
        mLeg2R.mulRotate(RL * 40 * s_ntoz);
        mLeg1L.translate(0, -0.2f);
        mLeg1L.mulRotate(RL * -30 * ns_ntoz);
        mLeg2L.translate(0, -0.5f);
        mLeg2L.mulRotate(RL * 40 * ns_ntoz);

        seg.draw(mMaster * mSpine * mArm1R * mArmLen, 7, sw);
        seg.draw(mMaster * mSpine * mArm1R * mArm2R * mArmLen, 8, sw);
        seg.draw(mMaster * mWaist * mWaistLen, 0, sw);
        seg.draw(mMaster * mWaist * mLeg1R * mLegLen, 1, sw);
        seg.draw(mMaster * mWaist * mLeg1R * mLeg2R * mLegLen, 2, sw);
        seg.draw(mMaster * mWaist * mLeg1L * mLegLen, 3, sw);
        seg.draw(mMaster * mWaist * mLeg1L * mLeg2L * mLegLen, 5, sw);
        seg.draw(mMaster * mSpine * mSpineLen, 5, sw);
        seg.draw(mMaster * mSpine * mNeck * mNeckLen, 6, sw);
        seg.draw(mMaster * mSpine * mArm1L * mArmLen, 9, sw);
        seg.draw(mMaster * mSpine * mArm1L * mArm2L * mArmLen, 10, sw);
    }
}
#endif

#ifdef _MAT33_TEST
void gmain() {
    window(1000, 1000);
    VEC op(0, 1);
    VEC p, p2;
    MAT33 mat;
    angleMode(DEGREES);
    float angle = 0;

    while (notQuit) {
        //clear();
        mathAxis(3.1f, 255);

        mat.identity();
        mat.mulTranslate(1.5, 0);
        mat.mulScaling(0.5, 0.5);
        mat.mulRotate(angle);
        mat.mulTranslate(1, 0);
        mat.mulRotate(angle * 16);
        mat.mulScaling(1, 0.5);
        p = mat * op;

        mat.identity();
        mat.mulTranslate(-1.5, 0);
        mat.mulScaling(0.5, 0.5);
        mat.mulRotate(angle);
        mat.mulTranslate(1, 0);
        mat.mulScaling(1, 0.5);
        mat.mulRotate(angle * 8);
        p2 = mat * op;

        angle += 1;

        strokeWeight(10);
        mathPoint(op.x, op.y);
        stroke(255, 0, 0);
        mathPoint(p.x, p.y);
        stroke(0, 0, 255);
        mathPoint(p2.x, p2.y);
    }
}

#endif

#ifdef _MAT33_POINTS
void gmain() {
    window(1920, 1080, 1);
    colorMode(HSV);
    //頂点を用意する
    const int num = 10000;
    COLOR c[num];
    VEC op[num];
    float theta = M_2PI / num;
    for (int i = 0; i < num; i++) {
        float radius = 1.0 - 0.6 * (i % 2);
        op[i].x = sin(theta * i) * radius;
        op[i].y = cos(theta * i) * radius;
        c[i] = COLOR(theta * i, 127, 255);
    }
    VEC p[num];
    MAT33 mat;
    float angle = 0;
    axisMode(NODRAW);

    while (notQuit) {
        clear();
        mathAxis(6.0f, 255);

        mat.identity();
        mat.mulRotate(angle);
        mat.mulTranslate(2.0, 0);
        mat.mulRotate(angle * 4);
        mat.mulScaling(2.5, 2.5);
        angle += 0.02;

        for (int i = 0; i < num; i++) {
            p[i] = mat * op[i];
        }

        strokeWeight(0.03);
        for (int i = 0; i < num; i++) {
            stroke(c[i]);
            mathPoint(op[i].x, op[i].y);
            mathPoint(p[i].x, p[i].y);
            int j = (i + 1) % num;
            mathLine(op[i].x, op[i].y, op[j].x, op[j].y);
            mathLine(p[i].x, p[i].y, p[j].x, p[j].y);

        }
    }
}

#endif

#ifdef _MAT33_DRAW
void gmain() {
    window(1920, 1080, 1);
    VEC o_sp, o_ep;
    axisMode(NODRAW);
    int R = 0, G = 0, B = 0;
    float hue = 0;
    while (notQuit) {
        mathAxis(1000, 255);
        //クリア
        if (isTrigger(KEY_SPACE)) clear();
        //RGBクリア
        if (isTrigger(KEY_ENTER)) R = 0, G = 0, B = 0;
        //色設定
        if (isTrigger(KEY_R) && isPress(KEY_S))R = 255;
        if (isTrigger(KEY_R) && isPress(KEY_L))R = 127;
        if (isTrigger(KEY_R) && isPress(KEY_Z))R = 0;
        if (isTrigger(KEY_G) && isPress(KEY_S))G = 255;
        if (isTrigger(KEY_G) && isPress(KEY_L))G = 127;
        if (isTrigger(KEY_G) && isPress(KEY_Z))G = 0;
        if (isTrigger(KEY_B) && isPress(KEY_S))B = 255;
        if (isTrigger(KEY_B) && isPress(KEY_L))B = 127;
        if (isTrigger(KEY_B) && isPress(KEY_Z))B = 0;
        if (isTrigger(KEY_Y)) { R = 255; G = 255; B = 0; }
        if (isTrigger(KEY_M)) { R = 255; G = 0; B = 255; }
        if (isTrigger(KEY_C)) { R = 0; G = 255; B = 255; }
        if (isTrigger(KEY_W)) { R = 255; G = 255; B = 255; }


        if (!isPress(MOUSE_LBUTTON) && !isPress(MOUSE_RBUTTON)) {
            o_sp = VEC(mathMouseX, mathMouseY);
            continue;
        }
        //虹ペン
        if (isPress(MOUSE_RBUTTON)) {
            colorMode(HSV);
            hue += 0.05;
            stroke(hue, 255, 255);
            o_ep = VEC(mathMouseX, mathMouseY);
        }
        else {
            colorMode(RGB);
            stroke(R, G, B);
            o_ep = VEC(mathMouseX, mathMouseY);
        }
        //消しゴム
        if (isPress(KEY_D)) {
            R = 0, G = 0, B = 0;
            hue = 0;
            strokeWeight(100);
        }
        else {
            float len = (o_ep - o_sp).mag();
            float sw = map(len, 0, 60, 3, 1);
            //strokeWeight(sw);
            strokeWeight(3);
        }
        mathLine(o_sp.x, o_sp.y, o_ep.x, o_ep.y);

        o_sp = o_ep;
    }
}

#endif

#ifdef _MAT33_SNOW
void gmain() {
    window(1920, 1080, 1);
    VEC o_sp, o_ep;
    VEC sp, ep;
    int num = 6;
    float angle = M_2PI / num;
    MAT33 mat;
    axisMode(NODRAW);
    while (notQuit) {
        mathAxis(1000, 255);
        if (isTrigger(KEY_SPACE)) clear();
        if (!isPress(MOUSE_LBUTTON)) {
            o_sp = VEC(mathMouseX, mathMouseY);
            continue;
        }
        o_ep = VEC(mathMouseX, mathMouseY);

        float len = (o_ep - o_sp).mag();
        float sw = map(len, 0, 60, 3, 1);
        //strokeWeight(sw);

        stroke(255, 255, 255, 127);
        for (int i = 0; i < num; i++) {
            mat.identity();
            mat.mulRotate(angle * i);
            sp = mat * o_sp;
            ep = mat * o_ep;
            mathLine(sp.x, sp.y, ep.x, ep.y);
            sp.y *= -1;
            ep.y *= -1;
            mathLine(sp.x, sp.y, ep.x, ep.y);
        }

        o_sp = o_ep;
    }
}

#endif

#ifdef _MAT33_KALEIDOSCOPE
void gmain() {
    window(1920, 1080, 1);
    VEC o_sp, o_ep;
    VEC sp, ep;
    int num = 6;
    float angle = M_2PI / num;
    MAT33 mat;
    axisMode(NODRAW);
    colorMode(HSV);
    float hue = 0;

    while (notQuit) {
        if (isTrigger(KEY_SPACE)) clear();
        fill(0, 0, 0, 10); noStroke(); rect(0, 0, width, height);
        mathAxis(1000, 255);

        if (!isPress(MOUSE_LBUTTON)) {
            o_sp = VEC(mathMouseX, mathMouseY);
            continue;
        }
        o_ep = VEC(mathMouseX, mathMouseY);

        float len = (o_ep - o_sp).mag();
        float sw = map(len, 0, 60, 3, 1);
        //strokeWeight(sw);
        strokeWeight(1);

        hue += 0.05;
        stroke(hue, 255, 255);

        for (int i = 0; i < num; i++) {
            mat.identity();
            mat.mulRotate(angle * i);
            sp = mat * o_sp;
            ep = mat * o_ep;
            mathLine(sp.x, sp.y, ep.x, ep.y);
            sp.y *= -1;
            ep.y *= -1;
            mathLine(sp.x, sp.y, ep.x, ep.y);
        }

        o_sp = o_ep;
        }
}
#endif

#ifdef _MAT
#include"libOne.h"


//２次元ベクトル（２行１列の行列として考えることもできる）
class VEC {
public:
    float x, y;
    VEC(float x = 0, float y = 0) {
        this->x = x;
        this->y = y;
    }
};

//３行３列の行列
class MAT33 {
public:
    float _11, _12, _13;
    float _21, _22, _23;
    float _31, _32, _33;
    MAT33(float _11 = 1, float _12 = 0, float _13 = 0,
        float _21 = 0, float _22 = 1, float _23 = 0,
        float _31 = 0, float _32 = 0, float _33 = 1) {
        this->_11 = _11; this->_12 = _12; this->_13 = _13;
        this->_21 = _21; this->_22 = _22; this->_23 = _23;
        this->_31 = _31; this->_32 = _32; this->_33 = _33;
    }
    //単位行列をつくる
    void identity() {
        _11 = 1; _12 = 0; _13 = 0;
        _21 = 0; _22 = 1; _23 = 0;
        _31 = 0; _32 = 0; _33 = 1;
    }
    //拡大縮小行列をつくる
    void scaling(float sx, float sy) {
        _11 = sx;
        _22 = sy;
    }
    //回転行列をつくる
    void rotate(float angle) {
        float c = cos(angle);
        float s = sin(angle);
        _11 = c; _12 = -s;
        _21 = s; _22 = c;
    }
    //平行移動行列をつくる
    void translate(float x, float y) {
        _13 = x;
        _23 = y;
    }
    //「３行３列行列」と「２行１列行列（２次元ベクトル）＋１行」の掛け算
    VEC operator*(const VEC& v) {
        return VEC(
            _11 * v.x + _12 * v.y + _13,
            _21 * v.x + _22 * v.y + _23
        );
    }
    //３行３列行列どうしの掛け算
    MAT33 operator*(const MAT33& m) {
        return MAT33(
            //１行目
            _11 * m._11 + _12 * m._21 + _13 * m._31,
            _11 * m._12 + _12 * m._22 + _13 * m._32,
            _11 * m._13 + _12 * m._23 + _13 * m._33,
            //２行目
            _21 * m._11 + _22 * m._21 + _23 * m._31,
            _21 * m._12 + _22 * m._22 + _23 * m._32,
            _21 * m._13 + _22 * m._23 + _23 * m._33,
            //３行目
            _31 + m._11 + _32 * m._21 + _33 * m._31,
            _31 + m._12 + _32 * m._22 + _33 * m._32,
            _31 + m._13 + _32 * m._23 + _33 * m._33
        );
    }
};

void gmain() {
    window(1000, 1000);
    VEC op(0, 1);
    VEC p;
    MAT33 mat, tm, rm, sm, rm2;
    angleMode(DEGREES);
    float angle = 0;

    while (notQuit) {
        //clear();
        mathAxis(3.1f, 255);

        rm2.rotate(angle);
        tm.translate(1, 0);
        rm.rotate(angle  * 5);
        sm.scaling(1, 0.3);
        angle += 2;
        mat = rm2 * tm * sm * rm;
        p = mat * op;

        strokeWeight(10);
        stroke(255, 0, 0);
        mathPoint(op.x, op.y);
        mathPoint(p.x, p.y);
    }
}

#endif

#ifdef _MAT22
#include"libOne.h"

//２次元ベクトル（２行１列の行列として考えることもできる）
class VEC {
public:
    //行列の要素
    float x;
    float y;
    //コンストラクタ
    VEC(float x=0, float y=0) {
        this->x = x;
        this->y = y;
    }
};

//２行２列の行列
class MAT22 {
public:
    //行列の要素
    float _11, _12;
    float _21, _22;
    //コンストラクタ
    MAT22(float _11=1, float _12=0,
          float _21=0, float _22=1 ) {
        this->_11 = _11; this->_12 = _12;
        this->_21 = _21; this->_22 = _22;
    }
    //単位行列をつくる
    void identity() {
        _11 = 1; _12 = 0;
        _21 = 0; _22 = 1;
    }
    //拡大縮小行列をつくる
    void scaling(float sx, float sy) {
        _11 = sx;
        _22 = sy;
    }
    //回転行列をつくる
    void rotate(float angle) {
        float c = cos(angle);
        float s = sin(angle);
        _11 = c; _12 = -s;
        _21 = s; _22 = c;
    }
    //「２行２列行列」と「２行１列行列（２次元ベクトル）」の掛け算
    VEC operator*(const VEC& v) {
        return VEC(
            _11 * v.x + _12 * v.y,
            _21 * v.x + _22 * v.y
        );
    }
    //２行２列行列どうしの掛け算
    MAT22 operator*(const MAT22& m) {
        return MAT22(
            //１行目
            _11 * m._11 + _12 * m._21,//１行１列の要素
            _11 * m._12 + _12 * m._22,//１行２列の要素
            //２行目
            _21 * m._11 + _22 * m._21,//２行１列の要素
            _21 * m._12 + _22 * m._22 //２行２列の要素
        );
    }
};

//背景クリア関数
void background() {
    fill(0, 0, 60); noStroke(); rect(0, 0, width, height);
}

//「１つの座標」を「２行２列の行列で回転、拡大縮小する」
void gmain() {
    window(1080, 1080, full);
    hideCursor();

    //点の色
    angleMode(DEGREES);
    colorMode(HSV);
    COLOR c(60, 128, 255);
    //元の座標 original position
    VEC op(1, 0.5f);
    //行列計算後の座標 position
    VEC p;
    //行列
    MAT22 sm;//scaling matrix
    MAT22 rm;//rotate matrix
    float angle = 0;
    MAT22 mat;//合成した行列

    while (notQuit) {
        background();
        mathAxis(2.1f,255);
        //各行列をつくる
        sm.scaling(0.5f,2.0f);
        angle += 1;
        rm.rotate(angle);
        //行列を合成する
        mat = sm * rm;
        //座標変換する
        p = mat * op;
        //描画
        strokeWeight(20);
        stroke(c);
        //元の座標
        mathPoint(op.x, op.y);
        //変換後の座標
        mathPoint(p.x, p.y);
    }
}
#endif

#ifdef _MAT33
#include"libOne.h"
//２次元ベクトル（２行１列の行列として考えることもできる）
class VEC {
public:
    float x, y;
    VEC(float x = 0, float y = 0) {
        this->x = x;
        this->y = y;
    }
};
//３行３列の行列
class MAT33 {
public:
    float _11, _12, _13;
    float _21, _22, _23;
    float _31, _32, _33;
    MAT33(float _11=1, float _12=0, float _13=0,
          float _21=0, float _22=1, float _23=0,
          float _31=0, float _32=0, float _33=1) {
        this->_11 = _11; this->_12 = _12; this->_13 = _13;
        this->_21 = _21; this->_22 = _22; this->_23 = _23;
        this->_31 = _31; this->_32 = _32; this->_33 = _33;
    }
    //単位行列をつくる
    void identity() {
        _11 = 1; _12 = 0; _13 = 0;
        _21 = 0; _22 = 1; _23 = 0;
        _31 = 0; _32 = 0; _33 = 1;
    }
    //拡大縮小行列をつくる
    void scaling(float sx, float sy) {
        _11 = sx;
        _22 = sy;
    }
    //回転行列をつくる
    void rotate(float angle) {
        float c = cos(angle);
        float s = sin(angle);
        _11 = c; _12 = -s;
        _21 = s; _22 = c;
    }
    //平行移動行列をつくる
    void translate(float x, float y) {
        _13 = x;
        _23 = y;
    }
    //「３行３列行列」と「２行１列行列（２次元ベクトル）＋１行」の掛け算
    VEC operator*(const VEC& v) {
        return VEC(
            _11 * v.x + _12 * v.y + _13 * 1,
            _21 * v.x + _22 * v.y + _23 * 1
        );
    }
    //３行３列行列どうしの掛け算
    MAT33 operator*(const MAT33& m) {
        return MAT33(
            //１行目
            _11 * m._11 + _12 * m._21 + _13 * m._31,
            _11 * m._12 + _12 * m._22 + _13 * m._32,
            _11 * m._13 + _12 * m._23 + _13 * m._33,
            //２行目
            _21 * m._11 + _22 * m._21 + _23 * m._31,
            _21 * m._12 + _22 * m._22 + _23 * m._32,
            _21 * m._13 + _22 * m._23 + _23 * m._33,
            //３行目
            _31 + m._11 + _32 * m._21 + _33 * m._31,
            _31 + m._12 + _32 * m._22 + _33 * m._32,
            _31 + m._13 + _32 * m._23 + _33 * m._33
        );
    }
};

#include<vector>
void background() {
    fill(0, 0, 60); noStroke(); rect(0, 0, width, height);
}
void gmain() {
    window(1080, 1080, full);
    hideCursor();
    angleMode(DEGREES);
    colorMode(HSV);
    COLOR c(60,128,255);
    VEC op(1, 0.5f);
    VEC p;
    MAT33 mat,sm,rm,tm,rm2;
    float angle = 0;
    while (notQuit) {
        background();
        mathAxis(3, 128);
        //各行列をつくる
        rm2.rotate(angle);
        tm.translate(1, 0);
        rm.rotate(angle*4);
        sm.scaling(0.4f, 0.4f);
        angle += 1;
        //行列を合成する
        mat = rm2 * tm * rm * sm;
        //座標変換
        p = mat * op;
        //描画
        strokeWeight(20);
        stroke(c);
        mathPoint(op.x, op.y);//元の座標
        mathPoint(p.x, p.y);//変換後の座標
    }
}
#endif

#ifdef _MAT33_OPT
//---------------------------------------------------------------------
//「行列クラスの最適化」と「星形アニメーション」
//---------------------------------------------------------------------

#include"libOne.h"
//VECは変更なし
class VEC {
public:
    float x, y;
    VEC(float x = 0, float y = 0) {
        this->x = x;
        this->y = y;
    }
};

//---------------------------------------------------------------------
//行列の要素_31,_32,_33は0,0,1のまま変化しないため省略することができる
//---------------------------------------------------------------------
class MAT33 {
public:
    float _11, _12, _13;
    float _21, _22, _23;
    //float _31, _32, _33;
    MAT33(float _11=1, float _12=0, float _13=0,
          float _21=0, float _22=1, float _23=0//,
          //float _31=0, float _32=0, float _33=1
    ) {
        this->_11 = _11; this->_12 = _12; this->_13 = _13;
        this->_21 = _21; this->_22 = _22; this->_23 = _23;
        //this->_31 = _31; this->_32 = _32; this->_33 = _33;
    }
    void identity() {
        _11 = 1; _12 = 0; _13 = 0;
        _21 = 0; _22 = 1; _23 = 0;
        //_31 = 0; _32 = 0; _33 = 1;
    }
    void scaling(float sx, float sy) {
        _11 = sx;
        _22 = sy;
    }
    void rotate(float angle) {
        float c = cos(angle);
        float s = sin(angle);
        _11 = c; _12 = -s;
        _21 = s; _22 = c;
    }
    void translate(float x, float y) {
        _13 = x;
        _23 = y;
    }
    VEC operator*(const VEC& v) {
        return VEC(
            _11 * v.x + _12 * v.y + _13,// * 1,←1は掛ける必要がない
            _21 * v.x + _22 * v.y + _23// * 1
        );
    }
    MAT33 operator*(const MAT33& m) {
        return MAT33(
            _11 * m._11 + _12 * m._21,// + _13 * m._31,
            _11 * m._12 + _12 * m._22,// + _13 * m._32,
            _11 * m._13 + _12 * m._23 + _13,// * m._33,
            _21 * m._11 + _22 * m._21,// +_23 * m._31,
            _21 * m._12 + _22 * m._22,// + _23 * m._32,
            _21 * m._13 + _22 * m._23 + _23// * m._33,
            //_31 + m._11 + _32 * m._21 + _33 * m._31,
            //_31 + m._12 + _32 * m._22 + _33 * m._32,
            //_31 + m._13 + _32 * m._23 + _33 * m._33
        );
    }
};

//------------------------------------------------------------------------
//星形に用意した座標を行列で動かす
//------------------------------------------------------------------------
void background() {
    fill(0, 0, 20); noStroke(); rect(0, 0, width, height);
}
void gmain() {
    window(1080, 1080, full);
    hideCursor();
    angleMode(DEGREES);
    colorMode(HSV);
    //座標を配列で複数用意する
    const int num = 10;
    VEC op[num];
    VEC p[num];
    COLOR c[num];
    //星形になる座標を設定
    float theta = 360.0f / num;
    for (int i = 0; i < num; i++) {
        float r = 1.0f - 0.5f * (i % 2);//radius
        op[i] = VEC(sin(theta * i) * r, cos(theta * i) * r);
        c[i] = COLOR(theta * i, 128, 255);
    }
    //全行列
    MAT33 mat, tm, rm, sm, rm2;
    float angle = 0;
    while (notQuit) {
        background();
        axisMode(NODRAW);
        mathAxis(2, 128);
        //行列をつくる
        rm2.rotate(angle);
        tm.translate(1, 0);
        sm.scaling(0.25f, 0.5f);
        rm.rotate(angle * 4);
        angle += 0.5f;
        //行列を合成する
        mat = rm2 * tm * sm * rm;
        //座標変換する
        for (int i = 0; i < num; i++) {
            p[i] = mat * op[i];
        }
        //描画する
        for (int i = 0; i < num; i++) {
            strokeWeight(10);
            stroke(c[i]);
            mathPoint(op[i].x, op[i].y);
            mathPoint(p[i].x, p[i].y);
            int j = (i + 1) % num;//iが最後の座標を指すとき、ｊを0とする
            mathLine(p[i].x, p[i].y, p[j].x, p[j].y);
        }
    }
}
#endif

#ifdef _MAT33_SAMPLES
#include"libOne.h"
class VEC {
public:
    float x, y;
    VEC(float x = 0, float y = 0) {
        this->x = x;
        this->y = y;
    }
};
class MAT33 {
public:
    float _11, _12, _13;
    float _21, _22, _23;
    //float _31, _32, _33;
    MAT33(float _11=1, float _12=0, float _13=0,
          float _21=0, float _22=1, float _23=0
          //,float _31=0, float _32=0, float _33=1
    ) {
        this->_11 = _11; this->_12 = _12; this->_13 = _13;
        this->_21 = _21; this->_22 = _22; this->_23 = _23;
        //this->_31 = _31; this->_32 = _32; this->_33 = _33;
    }
    void identity() {
        _11 = 1; _12 = 0; _13 = 0;
        _21 = 0; _22 = 1; _23 = 0;
        //_31 = 0; _32 = 0; _33 = 1;
    }
    void scaling(float sx, float sy) {
        _11 = sx;
        _22 = sy;
    }
    void rotate(float angle) {
        float c = cos(angle);
        float s = sin(angle);
        _11 = c; _12 = -s;
        _21 = s; _22 = c;
    }
    void translate(float x, float y) {
        _13 = x;
        _23 = y;
    }
    VEC operator*(const VEC& v) {
        return VEC(
            _11 * v.x + _12 * v.y + _13 * 1,
            _21 * v.x + _22 * v.y + _23 * 1
        );
    }
    MAT33 operator*(const MAT33& m) {
        return MAT33(
            _11 * m._11 + _12 * m._21,// + _13 * m._31,
            _11 * m._12 + _12 * m._22,// + _13 * m._32,
            _11 * m._13 + _12 * m._23 + _13,// * m._33,

            _21 * m._11 + _22 * m._21,// +_23 * m._31,
            _21 * m._12 + _22 * m._22,// + _23 * m._32,
            _21 * m._13 + _22 * m._23 + _23// * m._33
            //,_31 + m._11 + _32 * m._21 + _33 * m._31,
            //_31 + m._12 + _32 * m._22 + _33 * m._32,
            //_31 + m._13 + _32 * m._23 + _33 * m._33
        );
    }
};

#include<vector>
void background() {
    fill(0, 0, 20); noStroke(); rect(0, 0, width, height);
}
void gmain() {
    window(1080, 1080, full);
    hideCursor();
    angleMode(DEGREES);
    colorMode(HSV);
    
    std::vector<COLOR> c;
    std::vector<VEC> op;
    int num = 10;
    float angle = 360.0f / num;
    for (int i = 0; i < num; i++) {
        float r = 1.0f - 0.5f * (i % 2);
        op.emplace_back(sin(angle * i) * r, cos(angle * i) * r);
        c.emplace_back(angle * i, 128, 255);
    }
    std::vector<VEC> p = op;
    
    MAT33 mat, tm, rm, sm, rm2;
    angle = 0;

    int numSamples = 8;
    int no = 0;
    while (notQuit) {
        background();
        mathAxis(2, 128);
        
        //print表示設定
        fill(0, 0, 255);
        printSize(30);

        //サンプルナンバー切り替え
        if (isTrigger(KEY_SPACE)) {
            ++no %= numSamples;
            angle = 0;
        }

        switch (no) {
        case 0:
            print("op[2]を回転");

            rm.rotate(angle);
            angle += 2.5f;
            mat = rm;

            p[2] = mat * op[2];

            strokeWeight(20);
            stroke(c[2]);
            mathPoint(op[2].x, op[2].y);
            mathPoint(p[2].x, p[2].y);
            break;
        case 1:
            print("op[2]を回転して、縦長にスケーリング");

            rm.rotate(angle);
            sm.scaling(0.2f, 0.7f);
            angle += 2.5f;
            mat = sm * rm;

            p[2] = mat * op[2];

            strokeWeight(20);
            stroke(c[2]);
            mathPoint(op[2].x, op[2].y);
            mathPoint(p[2].x, p[2].y);
            break;
        case 2:
            print("op[2]を回転して、0.5にスケーリング");

            rm.rotate(angle);
            sm.scaling(0.5f, 0.5f);
            angle += 10;
            mat = sm * rm;

            p[2] = mat * op[2];

            strokeWeight(20);
            stroke(c[2]);
            mathPoint(op[2].x, op[2].y);
            mathPoint(p[2].x, p[2].y);
            break;
        case 3:
            print("op[2]を回転して、0.5にスケーリング、右に１平行移動");

            tm.translate(1, 0);
            rm.rotate(angle);
            sm.scaling(0.5f, 0.5f);
            angle += 10;
            mat = tm * sm * rm;

            p[2] = mat * op[2];

            strokeWeight(20);
            stroke(c[2]);
            mathPoint(op[2].x, op[2].y);
            mathPoint(p[2].x, p[2].y);
            break;
        case 4:
            print("op[2]を回転して、0.5にスケーリング、右に１平行移動、さらに回転");

            rm2.rotate(angle/4);
            tm.translate(1, 0);
            rm.rotate(angle);
            sm.scaling(0.5f, 0.5f);
            angle += 10;
            mat = rm2 * tm * sm * rm;

            p[2] = mat * op[2];

            strokeWeight(20);
            stroke(c[2]);
            mathPoint(op[2].x, op[2].y);
            mathPoint(p[2].x, p[2].y);
            break;
        case 5:
            print("すべての op を回転して、0.5にスケーリング、右に１平行移動、さらに回転");

            rm2.rotate(angle/2);
            tm.translate(1, 0);
            rm.rotate(angle);
            sm.scaling(0.25f, 0.5f);
            angle += 2;
            mat = rm2 * tm * sm * rm;

            for (int i = 0; i < p.size(); i++) {
                p[i] = mat * op[i];
            }

            for (int i = 0; i < p.size(); i++) {
                strokeWeight(10);
                stroke(c[i]);
                mathPoint(op[i].x, op[i].y);
                mathPoint(p[i].x, p[i].y);
            }
            break;
        case 6:
            print("op 非表示");

            rm2.rotate(angle / 2);
            tm.translate(1, 0);
            rm.rotate(angle);
            sm.scaling(0.25f, 0.5f);
            angle += 2;
            mat = rm2 * tm * sm * rm;

            for (int i = 0; i < p.size(); i++) {
                p[i] = mat * op[i];
            }

            for (int i = 0; i < p.size(); i++) {
                strokeWeight(10);
                stroke(c[i]);
                mathPoint(p[i].x, p[i].y);
            }
            break;
        case 7:
            print("点と点を線で結ぶ");

            rm2.rotate(angle/2);
            tm.translate(1, 0);
            rm.rotate(angle);
            sm.scaling(0.25f, 0.5f);
            angle += 2;
            mat = rm2 * tm * sm * rm;

            for (int i = 0; i < p.size(); i++) {
                p[i] = mat * op[i];
            }

            for (int i = 0; i < p.size(); i++) {
                strokeWeight(10);
                stroke(c[i]);
                int j = (i + 1) % p.size();
                mathLine(p[i].x, p[i].y, p[j].x, p[j].y);
            }
            break;
        }//end of switch
    }
}
#endif

/*
//--------------------------------------------------------
//最適化されたMAT33同士の掛け算
//--------------------------------------------------------
void mulTranslate(float x, float y) {
    // |_11 _12 _13|    |1  0  x| 　 |_11  _12  _11x+_12y+_13| 
    // |_21 _22 _23| × |0  1  y| ＝ |_21  _22  _21x+_22y+_23|
    // |  0   0   1|    |0  0  1|    |  0    0              1|
    _13 += _11 * x + _12 * y;
    _23 += _21 * x + _22 * y;
}
void mulScaling(float x, float y) {
    // |_11 _12 _13|    |x  0  0| 　 |_11x _12y _13|
    // |_21 _22 _23| × |0  y  0| ＝ |_21x _22y _23|
    // |  0   0   1|    |0  0  1|    |  0    0    1|
    _11 *= x;    _12 *= y;
    _21 *= x;    _22 *= y;
}
void mulRotate(float angle) {
    // |_11 _12 _13|    |c -s  0| 　 |_11c+_12s   _11(-s)+_12c   _13|
    // |_21 _22 _23| × |s  c  0| ＝ |_21c+_22s   _21(-s)+_22c   _23|
    // |  0   0   1|    |0  0  1|    |        0             0      1|
    float c = cos(angle);
    float s = sin(angle);
    float tmp;
    //1行目
    tmp = _11 * c + _12 * s;
    _12 = _11 * -s + _12 * c;
    _11 = tmp;
    //2行目
    tmp = _21 * c + _22 * s;
    _22 = _21 * -s + _22 * c;
    _21 = tmp;
}
*/
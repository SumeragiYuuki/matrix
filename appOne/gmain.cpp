
#define _MAT33_ANIMATION_WALK
#define _VECMAT33

#ifdef _VECMAT33
#include"libOne.h"
//�Q�����x�N�g���i�Q�s�P��̍s��Ƃ��čl���邱�Ƃ��ł���j
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

//�R�s�R��̍s��
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
    //�P�ʍs�������
    void identity() {
        _11 = 1; _12 = 0; _13 = 0;
        _21 = 0; _22 = 1; _23 = 0;
        _31 = 0; _32 = 0; _33 = 1;
    }
    //�g��k���s�������
    void scaling(float sx, float sy) {
        _11 = sx; _12 = 0; _13 = 0;
        _21 = 0; _22 = sy; _23 = 0;
        _31 = 0; _32 = 0; _33 = 1;
    }
    //��]�s�������
    void rotate(float angle) {
        float c = cos(angle);
        float s = sin(angle);
        _11 = c; _12 = -s; _13 = 0;
        _21 = s; _22 = c; _23 = 0;
        _31 = 0; _32 = 0; _33 = 1;
    }
    //���s�ړ��s�������
    void translate(float x, float y) {
        _11 = 1; _12 = 0; _13 = x;
        _21 = 0; _22 = 1; _23 = y;
        _31 = 0; _32 = 0; _33 = 1;
    }
    //�u�R�s�R��s��v�Ɓu�Q�s�P��s��i�Q�����x�N�g���j�{�P�s�v�̊|���Z
    VEC operator*(const VEC& v) const {
        return VEC(
            _11 * v.x + _12 * v.y + _13,
            _21 * v.x + _22 * v.y + _23
        );
    }
    //�R�s�R��s��ǂ����̊|���Z
    MAT33 operator*(const MAT33& m) {
        return MAT33(
            //�P�s��
            _11 * m._11 + _12 * m._21 + _13 * m._31,
            _11 * m._12 + _12 * m._22 + _13 * m._32,
            _11 * m._13 + _12 * m._23 + _13 * m._33,
            //�Q�s��
            _21 * m._11 + _22 * m._21 + _23 * m._31,
            _21 * m._12 + _22 * m._22 + _23 * m._32,
            _21 * m._13 + _22 * m._23 + _23 * m._33,
            //�R�s��
            _31 + m._11 + _32 * m._21 + _33 * m._31,
            _31 + m._12 + _32 * m._22 + _33 * m._32,
            _31 + m._13 + _32 * m._23 + _33 * m._33
        );
    }
    //--------------------------------------------------------
    //�œK�����ꂽMAT33���m�̊|���Z
    //--------------------------------------------------------
    void mulTranslate(float x, float y) {
        // |_11 _12 _13|    |1  0  x| �@ |_11  _12  _11x+_12y+_13| 
        // |_21 _22 _23| �~ |0  1  y| �� |_21  _22  _21x+_22y+_23|
        // |  0   0   1|    |0  0  1|    |  0    0              1|
        _13 += _11 * x + _12 * y;
        _23 += _21 * x + _22 * y;
    }
    void mulScaling(float x, float y) {
        // |_11 _12 _13|    |x  0  0| �@ |_11x _12y _13|
        // |_21 _22 _23| �~ |0  y  0| �� |_21x _22y _23|
        // |  0   0   1|    |0  0  1|    |  0    0    1|
        _11 *= x;    _12 *= y;
        _21 *= x;    _22 *= y;
    }
    void mulRotate(float angle) {
        // |_11 _12 _13|    |c -s  0| �@ |_11c+_12s   _11(-s)+_12c   _13|
        // |_21 _22 _23| �~ |s  c  0| �� |_21c+_22s   _21(-s)+_22c   _23|
        // |  0   0   1|    |0  0  1|    |        0             0      1|
        float c = cos(angle);
        float s = sin(angle);
        float tmp;
        //1�s��
        tmp = _11 * c + _12 * s;
        _12 = _11 * -s + _12 * c;
        _11 = tmp;
        //2�s��
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
    //�����̒��������߂�s��
    MAT33 mWaistLen, mLegLen, mSpineLen, mNeckLen, mArmLen;
    mWaistLen.scaling(1, 0.2f);
    mLegLen.scaling(1, 0.5f);
    mSpineLen.scaling(1, -0.4f);
    mNeckLen.scaling(1, -0.4f);
    mArmLen.scaling(1, 0.4f);

    //��]�ړ�������K�w�s��
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
    int RL = 1;//��~��Ԃł̍��E�ϊ�
    float Mangle = 0;
    int Sangle = 0;//�w���A���O��
    int Nangle = 0;//��A���O��
    float MtsX = 0;
    float MtsY = 0;
    int stop = 1;
    while (notQuit) {
        clear();
        mathAxis(2.3f, 255);
        //�R�}���h
        
        //�X��
        //if (isPress(KEY_RIGHT) && Mangle >= -45) Mangle += -0.25f;
        //if (isPress(KEY_LEFT) && Mangle <= 45) Mangle += 0.25f;
        ////�g��k��
        //if (isPress(KEY_UP) && size <= 2) size += 0.0025f;
        //if (isPress(KEY_DOWN) && size >= 0.5f) size += -0.0025f;
         //�㉺���E�ړ�
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
        //����ON/OFF
        if (isTrigger(KEY_SPACE)) {
            int tmp = stop + 1;
            stop = tmp % 2;
        }
        //���Z�b�g
        if (isTrigger(KEY_ENTER)) {
            size = 1;
            RL = 1;
            Mangle = 0;
            MtsX = 0;
            MtsY = 0;
            stop = 1;
        }

        //��]�W��
        float s = sin(theta);
        float legs = s < 0 ? s / 2 : s; //�������̓���
        float s_ntoz = s < 0 ? 0 : s;  //���̒l��0�ɂ���
        float ns = -s;
        float legns = ns < 0 ? ns / 2 : ns; //�������̓���
        float ns_ntoz = ns < 0 ? 0 : ns;  //���̒l��0�ɂ���
        

        //����W
        mMaster.translate(MtsX, MtsY);
        mMaster.mulRotate(Mangle);
        mMaster.mulScaling(size, size);
        //�w��
        mSpine.translate(0, 1.2f);
        mSpine.mulRotate(Sangle);
        //�񁕓�
        mNeck.translate(0, 0.4f);
        mNeck.mulRotate(Nangle);
        //�E��
        mArm1R.translate(0, 0.4f);
        mArm1R.mulRotate(RL * 30 * ns);
        mArm2R.translate(0, -0.4f);
        mArm2R.mulRotate(RL * 30 * ns_ntoz);
        //����
        mArm1L.translate(0, 0.4f);
        mArm1L.mulRotate(RL * 30 * s);
        mArm2L.translate(0, -0.4f);
        mArm2L.mulRotate(RL * 30 * s_ntoz);
        //��
        mWaist.translate(0, 1.2f);
        mWaist.mulRotate(0);
        //�E��
        mLeg1R.translate(0, -0.2f);
        mLeg1R.mulRotate(RL * 30 * legs);
        mLeg2R.translate(0, -0.5f);
        mLeg2R.mulRotate(RL * -40 * ns_ntoz);
        //����
        mLeg1L.translate(0, -0.2f);
        mLeg1L.mulRotate(RL * 30 * legns);
        mLeg2L.translate(0, -0.5f);
        mLeg2L.mulRotate(RL * -40 * s_ntoz);

        if (RL == 1) {
            seg.draw(mMaster * mSpine * mArm1L * mArmLen, 9, sw);          //����r
            seg.draw(mMaster * mSpine * mArm1L * mArm2L * mArmLen, 10, sw);//�����r
            seg.draw(mMaster * mWaist * mWaistLen, 0, sw);                 //��
            seg.draw(mMaster * mWaist * mLeg1L * mLegLen, 3, sw);          //��������
            seg.draw(mMaster * mWaist * mLeg1L * mLeg2L * mLegLen, 5, sw); //����
            seg.draw(mMaster * mWaist * mLeg1R * mLegLen, 1, sw);          //�E������
            seg.draw(mMaster * mWaist * mLeg1R * mLeg2R * mLegLen, 2, sw); //�E��
            seg.draw(mMaster * mSpine * mSpineLen, 5, sw);                 //�w��
            seg.draw(mMaster * mSpine * mNeck * mNeckLen, 6, sw);          //�񁕓�
            seg.draw(mMaster * mSpine * mArm1R * mArmLen, 7, sw);          //�E��r
            seg.draw(mMaster * mSpine * mArm1R * mArm2R * mArmLen, 8, sw); //�E���r
        }
        else if (RL==-1) {
            seg.draw(mMaster * mSpine * mArm1R * mArmLen, 7, sw);          //�E��r
            seg.draw(mMaster * mSpine * mArm1R * mArm2R * mArmLen, 8, sw); //�E���r
            seg.draw(mMaster * mWaist * mWaistLen, 0, sw);                 //��
            seg.draw(mMaster * mWaist * mLeg1R * mLegLen, 1, sw);          //�E������
            seg.draw(mMaster * mWaist * mLeg1R * mLeg2R * mLegLen, 2, sw); //�E��
            seg.draw(mMaster * mWaist * mLeg1L * mLegLen, 3, sw);          //��������
            seg.draw(mMaster * mWaist * mLeg1L * mLeg2L * mLegLen, 5, sw); //����
            seg.draw(mMaster * mSpine * mSpineLen, 5, sw);                 //�w��
            seg.draw(mMaster * mSpine * mNeck * mNeckLen, 6, sw);          //�񁕓�
            seg.draw(mMaster * mSpine * mArm1L * mArmLen, 9, sw);          //����r
            seg.draw(mMaster * mSpine * mArm1L * mArm2L * mArmLen, 10, sw);//�����r
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
    //�����̒��������߂�s��
    MAT33 mWaistLen, mLegLen, mSpineLen, mNeckLen, mArmLen;
    mWaistLen.scaling(1, 0.2f);
    mLegLen.scaling(1, 0.5f);
    mSpineLen.scaling(1, -0.4f);
    mNeckLen.scaling(1, -0.4f);
    mArmLen.scaling(1, 0.4f);

    //��]�ړ�������K�w�s��
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
        //�R�}���h
        //���E�̌���
        if (isTrigger(KEY_R)) RL = -1;
        if (isTrigger(KEY_L)) RL = 1;
        //�X��
        if (isPress(KEY_RIGHT) && Mangle >= -45) Mangle += -0.25f;
        if (isPress(KEY_LEFT) && Mangle <= 45) Mangle += 0.25f;
        //�g��k��
        if (isPress(KEY_UP) && size <= 2) size += 0.0025f;
        if (isPress(KEY_DOWN) && size >= 0.5f) size += -0.0025f;
        //�㉺���E�ړ�
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
        //����ON/OFF
        if (isTrigger(KEY_SPACE)) {
            int tmp = stop + 1;
            stop = tmp % 2;
        }
        //���Z�b�g
        if (isTrigger(KEY_ENTER)) {
            size = 1;
            RL = 1;
            Mangle = 0;
            MtsX = 0;
            MtsY = 0;
            stop = 1;
        }

        //��]�W��
        float s = sin(theta);
        float s_ntoz = s < 0 ? 0 : s;  //���̒l��0�ɂ���
        float ns = -s;
        float ns_ntoz = ns < 0 ? 0 : ns;  //���̒l��0�ɂ���
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
    //���_��p�ӂ���
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
        //�N���A
        if (isTrigger(KEY_SPACE)) clear();
        //RGB�N���A
        if (isTrigger(KEY_ENTER)) R = 0, G = 0, B = 0;
        //�F�ݒ�
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
        //���y��
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
        //�����S��
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


//�Q�����x�N�g���i�Q�s�P��̍s��Ƃ��čl���邱�Ƃ��ł���j
class VEC {
public:
    float x, y;
    VEC(float x = 0, float y = 0) {
        this->x = x;
        this->y = y;
    }
};

//�R�s�R��̍s��
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
    //�P�ʍs�������
    void identity() {
        _11 = 1; _12 = 0; _13 = 0;
        _21 = 0; _22 = 1; _23 = 0;
        _31 = 0; _32 = 0; _33 = 1;
    }
    //�g��k���s�������
    void scaling(float sx, float sy) {
        _11 = sx;
        _22 = sy;
    }
    //��]�s�������
    void rotate(float angle) {
        float c = cos(angle);
        float s = sin(angle);
        _11 = c; _12 = -s;
        _21 = s; _22 = c;
    }
    //���s�ړ��s�������
    void translate(float x, float y) {
        _13 = x;
        _23 = y;
    }
    //�u�R�s�R��s��v�Ɓu�Q�s�P��s��i�Q�����x�N�g���j�{�P�s�v�̊|���Z
    VEC operator*(const VEC& v) {
        return VEC(
            _11 * v.x + _12 * v.y + _13,
            _21 * v.x + _22 * v.y + _23
        );
    }
    //�R�s�R��s��ǂ����̊|���Z
    MAT33 operator*(const MAT33& m) {
        return MAT33(
            //�P�s��
            _11 * m._11 + _12 * m._21 + _13 * m._31,
            _11 * m._12 + _12 * m._22 + _13 * m._32,
            _11 * m._13 + _12 * m._23 + _13 * m._33,
            //�Q�s��
            _21 * m._11 + _22 * m._21 + _23 * m._31,
            _21 * m._12 + _22 * m._22 + _23 * m._32,
            _21 * m._13 + _22 * m._23 + _23 * m._33,
            //�R�s��
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

//�Q�����x�N�g���i�Q�s�P��̍s��Ƃ��čl���邱�Ƃ��ł���j
class VEC {
public:
    //�s��̗v�f
    float x;
    float y;
    //�R���X�g���N�^
    VEC(float x=0, float y=0) {
        this->x = x;
        this->y = y;
    }
};

//�Q�s�Q��̍s��
class MAT22 {
public:
    //�s��̗v�f
    float _11, _12;
    float _21, _22;
    //�R���X�g���N�^
    MAT22(float _11=1, float _12=0,
          float _21=0, float _22=1 ) {
        this->_11 = _11; this->_12 = _12;
        this->_21 = _21; this->_22 = _22;
    }
    //�P�ʍs�������
    void identity() {
        _11 = 1; _12 = 0;
        _21 = 0; _22 = 1;
    }
    //�g��k���s�������
    void scaling(float sx, float sy) {
        _11 = sx;
        _22 = sy;
    }
    //��]�s�������
    void rotate(float angle) {
        float c = cos(angle);
        float s = sin(angle);
        _11 = c; _12 = -s;
        _21 = s; _22 = c;
    }
    //�u�Q�s�Q��s��v�Ɓu�Q�s�P��s��i�Q�����x�N�g���j�v�̊|���Z
    VEC operator*(const VEC& v) {
        return VEC(
            _11 * v.x + _12 * v.y,
            _21 * v.x + _22 * v.y
        );
    }
    //�Q�s�Q��s��ǂ����̊|���Z
    MAT22 operator*(const MAT22& m) {
        return MAT22(
            //�P�s��
            _11 * m._11 + _12 * m._21,//�P�s�P��̗v�f
            _11 * m._12 + _12 * m._22,//�P�s�Q��̗v�f
            //�Q�s��
            _21 * m._11 + _22 * m._21,//�Q�s�P��̗v�f
            _21 * m._12 + _22 * m._22 //�Q�s�Q��̗v�f
        );
    }
};

//�w�i�N���A�֐�
void background() {
    fill(0, 0, 60); noStroke(); rect(0, 0, width, height);
}

//�u�P�̍��W�v���u�Q�s�Q��̍s��ŉ�]�A�g��k������v
void gmain() {
    window(1080, 1080, full);
    hideCursor();

    //�_�̐F
    angleMode(DEGREES);
    colorMode(HSV);
    COLOR c(60, 128, 255);
    //���̍��W original position
    VEC op(1, 0.5f);
    //�s��v�Z��̍��W position
    VEC p;
    //�s��
    MAT22 sm;//scaling matrix
    MAT22 rm;//rotate matrix
    float angle = 0;
    MAT22 mat;//���������s��

    while (notQuit) {
        background();
        mathAxis(2.1f,255);
        //�e�s�������
        sm.scaling(0.5f,2.0f);
        angle += 1;
        rm.rotate(angle);
        //�s�����������
        mat = sm * rm;
        //���W�ϊ�����
        p = mat * op;
        //�`��
        strokeWeight(20);
        stroke(c);
        //���̍��W
        mathPoint(op.x, op.y);
        //�ϊ���̍��W
        mathPoint(p.x, p.y);
    }
}
#endif

#ifdef _MAT33
#include"libOne.h"
//�Q�����x�N�g���i�Q�s�P��̍s��Ƃ��čl���邱�Ƃ��ł���j
class VEC {
public:
    float x, y;
    VEC(float x = 0, float y = 0) {
        this->x = x;
        this->y = y;
    }
};
//�R�s�R��̍s��
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
    //�P�ʍs�������
    void identity() {
        _11 = 1; _12 = 0; _13 = 0;
        _21 = 0; _22 = 1; _23 = 0;
        _31 = 0; _32 = 0; _33 = 1;
    }
    //�g��k���s�������
    void scaling(float sx, float sy) {
        _11 = sx;
        _22 = sy;
    }
    //��]�s�������
    void rotate(float angle) {
        float c = cos(angle);
        float s = sin(angle);
        _11 = c; _12 = -s;
        _21 = s; _22 = c;
    }
    //���s�ړ��s�������
    void translate(float x, float y) {
        _13 = x;
        _23 = y;
    }
    //�u�R�s�R��s��v�Ɓu�Q�s�P��s��i�Q�����x�N�g���j�{�P�s�v�̊|���Z
    VEC operator*(const VEC& v) {
        return VEC(
            _11 * v.x + _12 * v.y + _13 * 1,
            _21 * v.x + _22 * v.y + _23 * 1
        );
    }
    //�R�s�R��s��ǂ����̊|���Z
    MAT33 operator*(const MAT33& m) {
        return MAT33(
            //�P�s��
            _11 * m._11 + _12 * m._21 + _13 * m._31,
            _11 * m._12 + _12 * m._22 + _13 * m._32,
            _11 * m._13 + _12 * m._23 + _13 * m._33,
            //�Q�s��
            _21 * m._11 + _22 * m._21 + _23 * m._31,
            _21 * m._12 + _22 * m._22 + _23 * m._32,
            _21 * m._13 + _22 * m._23 + _23 * m._33,
            //�R�s��
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
        //�e�s�������
        rm2.rotate(angle);
        tm.translate(1, 0);
        rm.rotate(angle*4);
        sm.scaling(0.4f, 0.4f);
        angle += 1;
        //�s�����������
        mat = rm2 * tm * rm * sm;
        //���W�ϊ�
        p = mat * op;
        //�`��
        strokeWeight(20);
        stroke(c);
        mathPoint(op.x, op.y);//���̍��W
        mathPoint(p.x, p.y);//�ϊ���̍��W
    }
}
#endif

#ifdef _MAT33_OPT
//---------------------------------------------------------------------
//�u�s��N���X�̍œK���v�Ɓu���`�A�j���[�V�����v
//---------------------------------------------------------------------

#include"libOne.h"
//VEC�͕ύX�Ȃ�
class VEC {
public:
    float x, y;
    VEC(float x = 0, float y = 0) {
        this->x = x;
        this->y = y;
    }
};

//---------------------------------------------------------------------
//�s��̗v�f_31,_32,_33��0,0,1�̂܂ܕω����Ȃ����ߏȗ����邱�Ƃ��ł���
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
            _11 * v.x + _12 * v.y + _13,// * 1,��1�͊|����K�v���Ȃ�
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
//���`�ɗp�ӂ������W���s��œ�����
//------------------------------------------------------------------------
void background() {
    fill(0, 0, 20); noStroke(); rect(0, 0, width, height);
}
void gmain() {
    window(1080, 1080, full);
    hideCursor();
    angleMode(DEGREES);
    colorMode(HSV);
    //���W��z��ŕ����p�ӂ���
    const int num = 10;
    VEC op[num];
    VEC p[num];
    COLOR c[num];
    //���`�ɂȂ���W��ݒ�
    float theta = 360.0f / num;
    for (int i = 0; i < num; i++) {
        float r = 1.0f - 0.5f * (i % 2);//radius
        op[i] = VEC(sin(theta * i) * r, cos(theta * i) * r);
        c[i] = COLOR(theta * i, 128, 255);
    }
    //�S�s��
    MAT33 mat, tm, rm, sm, rm2;
    float angle = 0;
    while (notQuit) {
        background();
        axisMode(NODRAW);
        mathAxis(2, 128);
        //�s�������
        rm2.rotate(angle);
        tm.translate(1, 0);
        sm.scaling(0.25f, 0.5f);
        rm.rotate(angle * 4);
        angle += 0.5f;
        //�s�����������
        mat = rm2 * tm * sm * rm;
        //���W�ϊ�����
        for (int i = 0; i < num; i++) {
            p[i] = mat * op[i];
        }
        //�`�悷��
        for (int i = 0; i < num; i++) {
            strokeWeight(10);
            stroke(c[i]);
            mathPoint(op[i].x, op[i].y);
            mathPoint(p[i].x, p[i].y);
            int j = (i + 1) % num;//i���Ō�̍��W���w���Ƃ��A����0�Ƃ���
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
        
        //print�\���ݒ�
        fill(0, 0, 255);
        printSize(30);

        //�T���v���i���o�[�؂�ւ�
        if (isTrigger(KEY_SPACE)) {
            ++no %= numSamples;
            angle = 0;
        }

        switch (no) {
        case 0:
            print("op[2]����]");

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
            print("op[2]����]���āA�c���ɃX�P�[�����O");

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
            print("op[2]����]���āA0.5�ɃX�P�[�����O");

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
            print("op[2]����]���āA0.5�ɃX�P�[�����O�A�E�ɂP���s�ړ�");

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
            print("op[2]����]���āA0.5�ɃX�P�[�����O�A�E�ɂP���s�ړ��A����ɉ�]");

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
            print("���ׂĂ� op ����]���āA0.5�ɃX�P�[�����O�A�E�ɂP���s�ړ��A����ɉ�]");

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
            print("op ��\��");

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
            print("�_�Ɠ_����Ō���");

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
//�œK�����ꂽMAT33���m�̊|���Z
//--------------------------------------------------------
void mulTranslate(float x, float y) {
    // |_11 _12 _13|    |1  0  x| �@ |_11  _12  _11x+_12y+_13| 
    // |_21 _22 _23| �~ |0  1  y| �� |_21  _22  _21x+_22y+_23|
    // |  0   0   1|    |0  0  1|    |  0    0              1|
    _13 += _11 * x + _12 * y;
    _23 += _21 * x + _22 * y;
}
void mulScaling(float x, float y) {
    // |_11 _12 _13|    |x  0  0| �@ |_11x _12y _13|
    // |_21 _22 _23| �~ |0  y  0| �� |_21x _22y _23|
    // |  0   0   1|    |0  0  1|    |  0    0    1|
    _11 *= x;    _12 *= y;
    _21 *= x;    _22 *= y;
}
void mulRotate(float angle) {
    // |_11 _12 _13|    |c -s  0| �@ |_11c+_12s   _11(-s)+_12c   _13|
    // |_21 _22 _23| �~ |s  c  0| �� |_21c+_22s   _21(-s)+_22c   _23|
    // |  0   0   1|    |0  0  1|    |        0             0      1|
    float c = cos(angle);
    float s = sin(angle);
    float tmp;
    //1�s��
    tmp = _11 * c + _12 * s;
    _12 = _11 * -s + _12 * c;
    _11 = tmp;
    //2�s��
    tmp = _21 * c + _22 * s;
    _22 = _21 * -s + _22 * c;
    _21 = tmp;
}
*/
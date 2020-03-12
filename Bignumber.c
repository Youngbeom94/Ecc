#include "Bignumber.h"

long long cpucycles()
{
    return __rdtsc();
}

void set_bigint(bigint_st *bi_X, word *Input) 
{
    int cnt_i;
    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        bi_X->a[cnt_i] = Input[WORD_LEN - (cnt_i + 1)]; //? Input된 배열의 값을 실질적으로 대입해줌 단 메모리 저장순서는 역순으로 -> 계산의 편리성을 위하여
    }
    bi_X->e = 0;
}

void reset_bigint(bigint_st *bi_X) 
{
    int cnt_i;
    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        bi_X->a[cnt_i] = 0x00; //? Biginteger을 0으로 만들어주는 함수
    }
    bi_X->e = 0;
}
void copy_bigint(bigint_st *dst, bigint_st *src)
{
    int cnt_i;
    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        dst->a[cnt_i] = src->a[cnt_i]; 
    }
    dst->e = src->e;
}

void show(bigint_st *bi_X) //? 숫자를 보여주는 함수
{

    int cnt_i;

    printf("0x");
    for (cnt_i = WORD_LEN - 1; cnt_i >= 0; cnt_i--)
    {
#if WORD_BITLEN == 32
        printf("%08X", bi_X->a[cnt_i]); // Wordbitlen = 32일 경우 즉 4byte가 1워드인경우
#else
        printf("%016llX", bi_X->a[cnt_i]); // Wordbitlen = 64일 경우 즉 8byte가 1워드인경우
#endif
    }
    printf("\n");
}

int Compare(bigint_st *bi_X, bigint_st *bi_Y) //? bi_X,bi_Y의 크기를  비교해주는 함수
{

    int cnt_i;
    for (cnt_i = WORD_LEN - 1; cnt_i >= 0; cnt_i--)
    {
        if (bi_X->a[cnt_i] > bi_Y->a[cnt_i]) // 배열의 각 word를 비교하기
            return FST_IS_BIG;               // First is big (bi_X>bi_Y)
        if (bi_X->a[cnt_i] < bi_Y->a[cnt_i])
            return SCD_IS_BIG; // Second is big
    }
    return BOTH_ARE_SAME; // 만약 해당하는것이 아무것도 없는경우 두수는 같다고 표현
    printf("ERROR");
    return ERROR;
}
void setone(bigint_st *bi_X)
{
    int cnt_i = 0;
    for (cnt_i = 1; cnt_i < WORD_LEN; cnt_i++)
    {
        bi_X->a[cnt_i] = 0x00;
    }
    bi_X->a[0] = 0x01;
}
int isEven(bigint_st *bi_X)
{
    if ((bi_X->a[0] & 0x01) == 1)
        return FALSE;
    else
    {
        return TRUE;
    }
}
int isOne(bigint_st *bi_X)
{
    int cnt_i = 0;

    for (cnt_i = 1; cnt_i < WORD_LEN; cnt_i++)
    {
        if (bi_X->a[cnt_i] != 0x00)
            return FALSE;
    }
    if (bi_X->a[0] == 0x01)
        return TRUE;
    else
    {
        return FALSE;
    }
}
int iszero(bigint_st *bi_X)
{
    int cnt_i = 0;
    for (cnt_i = 1; cnt_i < WORD_LEN; cnt_i++)
    {
        if (bi_X->a[cnt_i] != 0x00)
            return FALSE;
    }
    return TRUE;
}
void RightShift(bigint_st *bi_X)
{
    int temp = 0;
    int cnt_i = 0;
    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        bi_X->a[cnt_i] = bi_X->a[cnt_i] >> 1;
        if (cnt_i < 7)
        {
            temp = (bi_X->a[cnt_i + 1] & 1) << 31;
            bi_X->a[cnt_i] = bi_X->a[cnt_i] ^ temp;
        }
    }
}
void Addition(bigint_st *bi_X, bigint_st *bi_Y, bigint_st *bi_Z, bigint_st *Prime)
{
    int cnt_i;     //for loop counting variable
    int carry = 0; // Carry

    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++) //둘의 WORD_LEN이 같으므로 한번에 계산 가능하다
    {
        bi_Z->a[cnt_i] = bi_X->a[cnt_i] + bi_Y->a[cnt_i] + carry; // 단순 덧셈. modulo는 자동적으로 작동 (word = unsigned int)
        if (bi_X->a[cnt_i] == bi_Z->a[cnt_i])
            continue;
        carry = bi_X->a[cnt_i] > bi_Z->a[cnt_i] ? 1 : 0;
    }
    bi_Z->e = carry; // 구조체의 e값은 carry값이므로 그대로 대입해준다.
    Reduction(bi_Z, Prime);
}
void Addition_NR(bigint_st *bi_X, bigint_st *bi_Y, bigint_st *bi_Z, bigint_st *Prime)
{
    int cnt_i;     //for loop counting variable
    int carry = 0; // Carry

    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++) //둘의 WORD_LEN이 같으므로 한번에 계산 가능하다
    {
        bi_Z->a[cnt_i] = bi_X->a[cnt_i] + bi_Y->a[cnt_i] + carry; // 단순 덧셈. modulo는 자동적으로 작동
        if (bi_X->a[cnt_i] == bi_Z->a[cnt_i])
            continue;
        carry = bi_X->a[cnt_i] > bi_Z->a[cnt_i] ? 1 : 0;
    }
    bi_Z->e = carry; // 구조체의 e값은 carry값이므로 그대로 대입해준다.
}
int sub_Core_borrow(word a, word b) //? Addition에서는 carry, Subtraction에서는 borrow.  borrow를 생각해주는 함수이다.
{
    if (a >= b)
        return 0;

    return 1;
}

void Subtraction(bigint_st *bi_X, bigint_st *bi_Y, bigint_st *bi_Z, bigint_st *Prime)
{
    int borrow = 0;
    int cnt_i = 0;
    int carry = 0;
    unsigned int temp[WORD_LEN] = {0x00};

    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++) //WORD_LEN의 길이만큼 뺼셈을 해준다. 루프안의 각각경우마다 두번의 뺄셈이들어간다.
    {                                          //1번째 뺄셈은 bi_X와 borrow값, 2번쨰 뺄셈은 1번쨰 뺼셈의 결과와 bi_Y.
        bi_Z->a[cnt_i] = bi_X->a[cnt_i] - borrow;
        borrow = sub_Core_borrow(bi_X->a[cnt_i], borrow); //borrow는 연속적으로 계산을 해줘야한다.
        borrow += sub_Core_borrow(bi_Z->a[cnt_i], bi_Y->a[cnt_i]);
        bi_Z->a[cnt_i] = bi_Z->a[cnt_i] - bi_Y->a[cnt_i];
    }
    bi_Z->e = borrow;

    if (bi_Z->e == 1) // borrow가 1인경우 감산을 해줘야하는데 뺄셈은, P값을 더해주어야한다.알고리즘은 덧셈과 동일
    {
        for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
        {
            temp[cnt_i] = bi_Z->a[cnt_i] + Prime->a[cnt_i] + carry;
            if (temp[cnt_i] == bi_Z->a[cnt_i])
                continue;
            carry = temp[cnt_i] < bi_Z->a[cnt_i] ? 1 : 0;
            bi_Z->a[cnt_i] = temp[cnt_i];
        }
        bi_Z->e = 0;
    }
}

void Reduction(bigint_st *bi_X, bigint_st *Prime) //? 감산 함수.
{
    if (bi_X->e == 1) //P값보다 크거나 carry값이 1일때
    {
        unsigned int temp[WORD_LEN] = {0x00};
        int borrow = 0;
        int cnt_i = 0;
        for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++) //감산해주는 함수는 뺄셈과 동일한 알고리즘을 가진다.
        {
            temp[cnt_i] = bi_X->a[cnt_i] - borrow;
            borrow = sub_Core_borrow(bi_X->a[cnt_i], borrow);
            borrow += sub_Core_borrow(temp[cnt_i], Prime->a[cnt_i]);
            temp[cnt_i] = temp[cnt_i] - Prime->a[cnt_i];
        }

        for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
        {
            bi_X->a[cnt_i] = temp[cnt_i];
        }
    }
}

void OS64MUL_256(bigint_st *bi_X, bigint_st *bi_Y, bigint_st *bi_Z, bigint_st *Prime)
{ //OS 64 Mul은 32bit 두개를 곱셈한 값을 저장시킬수있는 64bit 변수를 선언함으로써 가능하다.
    int cnt_i, cnt_j; 
    unsigned long long UV = 0x00LL;
    unsigned int U, V = 0x00;
    word result[WORD_LEN * 2] = {0x00};

    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        UV &= 0x00000000ffffffff;
        for (cnt_j = 0; cnt_j < WORD_LEN; cnt_j++)
        {
            U = UV >> 32;
            UV = result[cnt_i + cnt_j] + ((unsigned long long)bi_X->a[cnt_i] * (unsigned long long)bi_Y->a[cnt_j]) + U;
            V = UV & 0x00000000ffffffff;
            result[cnt_i + cnt_j] = V;
        }
        U = UV >> 32;
        result[cnt_i + WORD_LEN] = U;
    }
    Reduction_256(result, bi_Z, Prime);
}
void SPlit_Word_Mul(word* A, word* B, word* C)
{
     //! 곱셈:4  ,shift :8, 조건문 :2, 덧셈 12
    word temp[2] = {0x00};
    word temp0, temp1, temp2, temp3, temp4;
    temp0 = ((*A) & 0x0000ffff) * ((*B) & 0x0000ffff);
    temp1 = ((*A) >> 16) * ((*B) & 0x0000ffff);
    temp2 = ((*B) >> 16) * ((*A) & 0x0000ffff);
    temp3 = temp1 & 0x0000ffff;
    temp4 = temp2 & 0x0000ffff;
    temp3 = temp3 << 16;
    temp4 = temp2 << 16;

    temp[0] = temp0 + temp3;
    if (temp[0] < temp0)
        temp[1]++;
    temp[0] += temp4;
    if (temp[0] < temp4)
        temp[1]++;

    temp3 = temp1 >> 16;
    temp4 = temp2 >> 16;
    temp0 = ((*A) >> 16) * ((*B) >> 16);
    temp[1] += temp3 + temp4 + temp0;

    C[0] = temp[0];
    C[1] = temp[1];

}
void PS_Split_MUL_256(bigint_st *bi_X,bigint_st *bi_Y,bigint_st *bi_Z,bigint_st *Prime)//? PS_Split version of Multiplication of Two Biginteger
{
    int cnt_i = 0, cnt_j = 0; //for loop counting variable
    word result[WORD_LEN * WORD_LEN] = {0x00};
    word AH = 0x00;
    word AL = 0x00;
    word BH = 0x00;
    word BL = 0x00;
    word tmp = 0x00;
    word temp1[16] = {0x00};
    word temp2[16] = {0x00};
    word temp3[16] = {0x00};
    int carry = 0;

    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        for (cnt_j = 0; cnt_j < WORD_LEN; cnt_j++)
        {

            AH = bi_X->a[cnt_i] >> 16;
            AL = bi_X->a[cnt_i] & 0x0000ffff;
            BH = bi_Y->a[cnt_j] >> 16;
            BL = bi_Y->a[cnt_j] & 0x0000ffff;

            tmp = AL * BL;
            temp1[cnt_i + cnt_j] += tmp;
            carry = temp1[cnt_i + cnt_j] < (tmp) ? 1 : 0;
            temp1[cnt_i + cnt_j + 1] += carry;

            tmp = AH * BH;
            temp1[cnt_i + cnt_j + 1] += tmp;
            carry = temp1[cnt_i + cnt_j + 1] < (tmp) ? 1 : 0;
            temp1[cnt_i + cnt_j + 2] += carry;
        }
    }
    //!
    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        for (cnt_j = 0; cnt_j < WORD_LEN; cnt_j++)
        {
            AH = bi_X->a[cnt_i] >> 16;
            BH = bi_Y->a[cnt_j] >> 16;
            AL = bi_X->a[cnt_i] & 0x0000ffff;
            BL = bi_Y->a[cnt_j] & 0x0000ffff;

            tmp = AL * BH;
            temp2[cnt_i + cnt_j] += tmp;
            carry = temp2[cnt_i + cnt_j] < tmp ? 1 : 0;
            temp2[cnt_i + cnt_j + 1] += carry;

            tmp = AH * BL;
            temp2[cnt_i + cnt_j] += tmp;
            carry = temp2[cnt_i + cnt_j] < tmp ? 1 : 0;
            temp2[cnt_i + cnt_j + 1] += carry;
        }
    }
    //!

    for (cnt_i = 0; cnt_i < 2 * WORD_LEN; cnt_i++)
    {
        if (cnt_i == 0)
        {
            AH = temp2[cnt_i] >> 16;
            temp2[cnt_i] = temp2[cnt_i] << 16;
            continue;
        }
        AL = temp2[cnt_i] >> 16;
        temp2[cnt_i] = temp2[cnt_i] << 16;
        temp2[cnt_i] &= 0xffff0000;
        temp2[cnt_i] ^= AH;
        AH = AL;
    }
    carry = 0;
    for (cnt_i = 0; cnt_i < WORD_LEN * 2; cnt_i++) //둘의 WORD_LEN이 같으므로 한번에 계산 가능하다
    {
        temp3[cnt_i] = temp1[cnt_i] + temp2[cnt_i] + carry; // 단순 덧셈. modulo는 자동적으로 작동
        carry = temp3[cnt_i] < temp1[cnt_i] ? 1 : 0;
        result[cnt_i] = temp3[cnt_i];
    }
    Reduction_256(result,bi_Z, Prime);

}

void Reduction_256(word *bi_X, bigint_st *bi_Z, bigint_st *Prime)//? Using Fast reduction 256 method
{
    int cnt_i = 0;
    bigint_st Temp1 = {{0x00}, 0x00};
    bigint_st Temp2 = {{0x00}, 0x00};

    bigint_st S1 = {{0x00}, 0x00};
    bigint_st S2 = {{0x00}, 0x00};
    bigint_st S3 = {{0x00}, 0x00};
    bigint_st S4 = {{0x00}, 0x00};
    bigint_st S5 = {{0x00}, 0x00};
    bigint_st S6 = {{0x00}, 0x00};
    bigint_st S7 = {{0x00}, 0x00};
    bigint_st S8 = {{0x00}, 0x00};
    bigint_st S9 = {{0x00}, 0x00};

    word Input1[8] = {bi_X[7], bi_X[6], bi_X[5], bi_X[4], bi_X[3], bi_X[2], bi_X[1], bi_X[0]};
    word Input2[8] = {bi_X[15], bi_X[14], bi_X[13], bi_X[12], bi_X[11], 0x00, 0x00, 0x00};
    word Input3[8] = {0x00, bi_X[15], bi_X[14], bi_X[13], bi_X[12], 0x00, 0x00, 0x00};
    word Input4[8] = {bi_X[15], bi_X[14], 0x00, 0x00, 0x00, bi_X[10], bi_X[9], bi_X[8]};
    word Input5[8] = {bi_X[8], bi_X[13], bi_X[15], bi_X[14], bi_X[13], bi_X[11], bi_X[10], bi_X[9]};
    word Input6[8] = {bi_X[10], bi_X[8], 0x00, 0x00, 0x00, bi_X[13], bi_X[12], bi_X[11]};
    word Input7[8] = {bi_X[11], bi_X[9], 0x00, 0x00, bi_X[15], bi_X[14], bi_X[13], bi_X[12]};
    word Input8[8] = {bi_X[12], 0x00, bi_X[10], bi_X[9], bi_X[8], bi_X[15], bi_X[14], bi_X[13]};
    word Input9[9] = {bi_X[13], 0x00, bi_X[11], bi_X[10], bi_X[9], 0x00, bi_X[15], bi_X[14]};

    set_bigint(&S1, Input1);
    set_bigint(&S2, Input2);
    set_bigint(&S3, Input3);
    set_bigint(&S4, Input4);
    set_bigint(&S5, Input5);
    set_bigint(&S6, Input6);
    set_bigint(&S7, Input7);
    set_bigint(&S8, Input8);
    set_bigint(&S9, Input9);

    Addition(&S1, &S2, &Temp1, Prime);
    Subtraction(&Temp1, &S6, &Temp2, Prime);
    Addition(&Temp2, &S2, &Temp1, Prime);
    Subtraction(&Temp1, &S7, &Temp2, Prime);
    Addition(&Temp2, &S3, &Temp1, Prime);
    Subtraction(&Temp1, &S8, &Temp2, Prime);
    Addition(&Temp2, &S3, &Temp1, Prime);
    Subtraction(&Temp1, &S9, &Temp2, Prime);
    Addition(&Temp2, &S4, &Temp1, Prime);
    Addition(&Temp1, &S5, &Temp2, Prime);

    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        bi_Z->a[cnt_i] = Temp2.a[cnt_i];
    }
}

void Inverse_FLT(bigint_st *bi_X, bigint_st *bi_Z, bigint_st *Prime) //? Inv 함수 하지만 거의 사용하지 않을것이고, 주로 EEA방법을 사용할 것입니다.
{
    int cnt_i = 0;
    bigint_st z3 = {{0x00}, 0x00};
    bigint_st z15 = {{0x00}, 0x00};
    bigint_st temp1 = {{0x00}, 0x00};
    bigint_st temp2 = {{0x00}, 0x00};
    bigint_st temp3 = {{0x00}, 0x00};

    OS64MUL_256(bi_X, bi_X, &temp1, Prime);
    OS64MUL_256(&temp1, bi_X, &z3, Prime);

    OS64MUL_256(&z3, &z3, &temp1, Prime);
    OS64MUL_256(&temp1, &temp1, &temp2, Prime);
    OS64MUL_256(&temp2, &z3, &z15, Prime);

    bigint_st t0 = {{0x00}, 0x00};
    OS64MUL_256(&z15, &z15, &temp1, Prime);
    OS64MUL_256(&temp1, &temp1, &temp2, Prime);
    OS64MUL_256(&temp2, &z3, &t0, Prime);

    bigint_st t1 = {{0x00}, 0x00};
    OS64MUL_256(&t0, &t0, &temp1, Prime);
    for (cnt_i = 0; cnt_i < 2; cnt_i++) //* (n -2) /2
    {
        OS64MUL_256(&temp1, &temp1, &temp2, Prime);
        OS64MUL_256(&temp2, &temp2, &temp1, Prime);
    }
    OS64MUL_256(&temp1, &temp1, &temp3, Prime);
    OS64MUL_256(&temp3, &t0, &t1, Prime);

    bigint_st t2 = {{0x00}, 0x00};
    OS64MUL_256(&t1, &t1, &temp1, Prime);
    for (cnt_i = 0; cnt_i < 5; cnt_i++) //* (n -2) /2
    {
        OS64MUL_256(&temp1, &temp1, &temp2, Prime);
        OS64MUL_256(&temp2, &temp2, &temp1, Prime);
    }
    OS64MUL_256(&temp1, &temp1, &temp3, Prime);
    OS64MUL_256(&temp3, &t1, &t2, Prime);

    OS64MUL_256(&t2, &t2, &temp1, Prime);
    for (cnt_i = 0; cnt_i < 2; cnt_i++) //* (n -2) /2
    {
        OS64MUL_256(&temp1, &temp1, &temp2, Prime);
        OS64MUL_256(&temp2, &temp2, &temp1, Prime);
    }
    OS64MUL_256(&temp1, &temp1, &temp3, Prime);
    OS64MUL_256(&temp3, &t0, &t2, Prime);

    bigint_st t3 = {{0x00}, 0x00};
    OS64MUL_256(&t2, &t2, &temp1, Prime);
    OS64MUL_256(&temp1, &temp1, &temp2, Prime);
    OS64MUL_256(&temp2, &z3, &t3, Prime);

    bigint_st t4 = {{0x00}, 0x00};
    OS64MUL_256(&t3, &t3, &temp1, Prime);
    for (cnt_i = 0; cnt_i < 15; cnt_i++) //* (n -2) /2
    {
        OS64MUL_256(&temp1, &temp1, &temp2, Prime);
        OS64MUL_256(&temp2, &temp2, &temp1, Prime);
    }
    OS64MUL_256(&temp1, &temp1, &temp3, Prime);
    OS64MUL_256(&temp3, bi_X, &t4, Prime);

    OS64MUL_256(&t4, &t4, &temp1, Prime);
    for (cnt_i = 0; cnt_i < 47; cnt_i++) //* (n -2) /2
    {
        OS64MUL_256(&temp1, &temp1, &temp2, Prime);
        OS64MUL_256(&temp2, &temp2, &temp1, Prime);
    }
    OS64MUL_256(&temp1, &temp1, &t4, Prime);

    bigint_st t5 = {{0x00}, 0x00};
    OS64MUL_256(&t4, &t4, &temp1, Prime);
    for (cnt_i = 0; cnt_i < 15; cnt_i++) //* (n -2) /2
    {
        OS64MUL_256(&temp1, &temp1, &temp2, Prime);
        OS64MUL_256(&temp2, &temp2, &temp1, Prime);
    }
    OS64MUL_256(&temp1, &temp1, &temp3, Prime);
    OS64MUL_256(&temp3, &t3, &t5, Prime);

    OS64MUL_256(&t5, &t5, &temp1, Prime);
    for (cnt_i = 0; cnt_i < 15; cnt_i++) //* (n -2) /2
    {
        OS64MUL_256(&temp1, &temp1, &temp2, Prime);
        OS64MUL_256(&temp2, &temp2, &temp1, Prime);
    }
    OS64MUL_256(&temp1, &temp1, &temp3, Prime);
    OS64MUL_256(&temp3, &t3, &t5, Prime);

    OS64MUL_256(&t5, &t5, &temp1, Prime);
    for (cnt_i = 0; cnt_i < 14; cnt_i++) //* (n -2) /2
    {
        OS64MUL_256(&temp1, &temp1, &temp2, Prime);
        OS64MUL_256(&temp2, &temp2, &temp1, Prime);
    }
    OS64MUL_256(&temp1, &temp1, &temp3, Prime);
    OS64MUL_256(&temp3, &t2, bi_Z, Prime);

    OS64MUL_256(bi_Z, bi_Z, &temp1, Prime);
    OS64MUL_256(&temp1, &temp1, &temp2, Prime);
    OS64MUL_256(&temp2, bi_X, bi_Z, Prime);
}
void Inverse_EEA(bigint_st *bi_X, bigint_st *bi_Z, bigint_st *Prime) // Inverse
{
    bigint_st u = {{0x00}, 0x00};
    bigint_st v = {{0x00}, 0x00};
    bigint_st x1 = {{0x00}, 0x00};
    bigint_st x2 = {{0x00}, 0x00};
    bigint_st temp = {{0x00}, 0x00};
    x1.a[0] = 0x01;
    copy_bigint(&u, bi_X);
    copy_bigint(&v, Prime);
    int cnt_i = 0;
    while ((isOne(&u) == FALSE) && (isOne(&v) == FALSE))
    {

        while (isEven(&u) == TRUE)
        {
            RightShift(&u);
            if (isEven(&x1) == TRUE)
                RightShift(&x1);
            else
            {
                Addition_NR(&x1, Prime, &temp, Prime);
                RightShift(&temp);
                if (temp.e == 1)
                    temp.a[WORD_LEN - 1] ^= 0x80000000;
                copy_bigint(&x1, &temp);
            }
        }
        while (isEven(&v) == TRUE)
        {
            RightShift(&v);
            if (isEven(&x2) == TRUE)
                RightShift(&x2);
            else
            {
                Addition_NR(&x2, Prime, &temp, Prime);
                RightShift(&temp);
                if (temp.e == 1)
                    temp.a[WORD_LEN - 1] ^= 0x80000000;
                copy_bigint(&x2, &temp);
            }
        }
        if (Compare(&u, &v) != SCD_IS_BIG)
        {
            Subtraction(&u, &v, &temp, Prime);
            copy_bigint(&u, &temp);

            Subtraction(&x1, &x2, &temp, Prime);
            copy_bigint(&x1, &temp);
        }
        else
        {
            Subtraction(&v, &u, &temp, Prime);
            copy_bigint(&v, &temp);

            Subtraction(&x2, &x1, &temp, Prime);
            copy_bigint(&x2, &temp);
        }
    }
    if (isOne(&u) == TRUE)
    {
        copy_bigint(bi_Z, &x1);
    }
    else
    {
        copy_bigint(bi_Z, &x2);
    }
}

void show_EN(Ecc_pt *EN_X)
{
    if (EN_X->isinfinity == TRUE)
    {
        printf("무한원점입니다.\n");
        return; //* L_to_R//* L_to_R//* L_to_R
    }
    int cnt_i = 0;
    printf("X = ");
    for (cnt_i = WORD_LEN - 1; cnt_i >= 0; cnt_i--)
    {
        printf("%08X", EN_X->x.a[cnt_i]);
    }
    printf("\nY = ");
    for (cnt_i = WORD_LEN - 1; cnt_i >= 0; cnt_i--)
    {
        printf("%08X", EN_X->y.a[cnt_i]);
    }

    printf("\n\n");
}
void show_EN_J(Ecc_Jpt *EN_X)
{
    int cnt_i = 0;
    if (EN_X->isinfinity == TRUE)
    {
        printf("무한원점입니다.\n");
        return;
    }
    printf("X = ");
    for (cnt_i = WORD_LEN - 1; cnt_i >= 0; cnt_i--)
    {
        printf("%08X", EN_X->x.a[cnt_i]);
    }
    printf("\nY = ");
    for (cnt_i = WORD_LEN - 1; cnt_i >= 0; cnt_i--)
    {
        printf("%08X", EN_X->y.a[cnt_i]);
    }
    printf("\nZ = ");
    for (cnt_i = WORD_LEN - 1; cnt_i >= 0; cnt_i--)
    {
        printf("%08X", EN_X->z.a[cnt_i]);
    }
    printf("\n\n");
}

void set_EN(Ecc_pt *EN_P, word *input_X, word *input_Y)
{
    int cnt_i = 0;
    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        EN_P->x.a[cnt_i] = input_X[WORD_LEN - cnt_i - 1];
        EN_P->y.a[cnt_i] = input_Y[WORD_LEN - cnt_i - 1];
    }
}
void set_EN_J(Ecc_Jpt *EN_P, word *input_X, word *input_Y, word *input_Z)
{
    int cnt_i = 0;
    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        EN_P->x.a[cnt_i] = input_X[WORD_LEN - cnt_i - 1];
        EN_P->y.a[cnt_i] = input_Y[WORD_LEN - cnt_i - 1];
        EN_P->z.a[cnt_i] = input_Z[WORD_LEN - cnt_i - 1];
    }
}
void set_EN_J_reverse(Ecc_Jpt *EN_P, word *input_X, word *input_Y, word *input_Z)
{
    int cnt_i = 0;
    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        EN_P->x.a[cnt_i] = input_X[cnt_i];
        EN_P->y.a[cnt_i] = input_Y[cnt_i];
        EN_P->z.a[cnt_i] = input_Z[cnt_i];
    }
}
void set_EN_J_reverse_nonZ(Ecc_Jpt *EN_P, word *input_X, word *input_Y, word *input_Z)
{
    int cnt_i = 0;
    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        EN_P->x.a[cnt_i] = input_X[cnt_i];
        EN_P->y.a[cnt_i] = input_Y[cnt_i];
        EN_P->z.a[cnt_i] = input_Z[WORD_LEN - cnt_i - 1];
    }
}
void set_EN_reset(Ecc_pt *EN_P)
{
    int cnt_i = 0;
    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        EN_P->x.a[cnt_i] = 0x00;
        EN_P->y.a[cnt_i] = 0x00;
    }
    EN_P->x.e = 0;
    EN_P->y.e = 0;
}
void set_EN_J_reset(Ecc_Jpt *EN_P)
{
    int cnt_i = 0;
    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        EN_P->x.a[cnt_i] = 0x00;
        EN_P->y.a[cnt_i] = 0x00;
        EN_P->z.a[cnt_i] = 0x00;
    }
    EN_P->x.e = 0;
    EN_P->y.e = 0;
    EN_P->z.e = 0;
}
void EN_copy(Ecc_pt *EN_dst, Ecc_pt *EN_src)
{
    int cnt_i = 0;
    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        EN_dst->x.a[cnt_i] = EN_src->x.a[cnt_i];
        EN_dst->y.a[cnt_i] = EN_src->y.a[cnt_i];
    }
}
void EN_J_copy(Ecc_Jpt *EN_dst, Ecc_Jpt *EN_src)
{
    int cnt_i = 0;
    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        EN_dst->x.a[cnt_i] = EN_src->x.a[cnt_i];
        EN_dst->y.a[cnt_i] = EN_src->y.a[cnt_i];
        EN_dst->z.a[cnt_i] = EN_src->z.a[cnt_i];
    }
}
void Trns_A_to_J(Ecc_Jpt *dst, Ecc_pt *src, bigint_st *Prime)
{
    bigint_st Z1 = {{0x00}, 0x00};
    word Z1_array[8] = {0x00, 0x00, 0x00, 0x00, 0x000, 0x00, 0x00, 0x01};
    set_bigint(&Z1, Z1_array);
    set_EN_J_reverse(dst, src->x.a, src->y.a, Z1.a);
}
void Trns_J_to_A(Ecc_pt *dst, Ecc_Jpt *src, bigint_st *Prime)
{
    bigint_st Z1 = {{0x00}, 0x00};
    copy_bigint(&Z1, &src->z);
    bigint_st Z2 = {{0x00}, 0x00};
    bigint_st Z2inv = {{0x00}, 0x00};
    bigint_st Z3 = {{0x00}, 0x00};
    bigint_st Z3inv = {{0x00}, 0x00};

    OS64MUL_256(&Z1, &Z1, &Z2, Prime);
    OS64MUL_256(&Z1, &Z2, &Z3, Prime);
    Inverse_EEA(&Z3, &Z3inv, Prime);
    OS64MUL_256(&Z3inv, &src->y, &dst->y, Prime);
    OS64MUL_256(&Z3inv, &Z1, &Z2inv, Prime);
    OS64MUL_256(&Z2inv, &src->x, &dst->x, Prime);
}

void ECADD(Ecc_pt *EN_P, Ecc_pt *EN_Q, Ecc_pt *EN_R, bigint_st *Prime)
{
    bigint_st temp1 = {{0x00}, 0x00};
    bigint_st temp2 = {{0x00}, 0x00};
    bigint_st temp3 = {{0x00}, 0x00};
    bigint_st X = {{0x00}, 0x00};
    bigint_st Y = {{0x00}, 0x00};

    Subtraction(&(EN_Q->y), &(EN_P->y), &temp1, Prime);
    Subtraction(&(EN_Q->x), &(EN_P->x), &temp2, Prime);
    Inverse_FLT(&temp2, &temp3, Prime);
    OS64MUL_256(&temp1, &temp3, &temp2, Prime);
    OS64MUL_256(&temp2, &temp2, &temp1, Prime);

    Subtraction(&temp1, &(EN_P)->x, &temp2, Prime);
    Subtraction(&temp2, &(EN_Q)->x, &(EN_R)->x, Prime);

    //!
    Subtraction(&(EN_Q->y), &(EN_P->y), &temp1, Prime);
    Subtraction(&(EN_Q->x), &(EN_P->x), &temp2, Prime);
    Inverse_FLT(&temp2, &temp3, Prime);
    OS64MUL_256(&temp1, &temp3, &temp2, Prime);
    Subtraction(&(EN_P->x), &(EN_R)->x, &temp1, Prime);

    OS64MUL_256(&temp2, &temp1, &temp3, Prime);
    Subtraction(&temp3, &(EN_P->y), &(EN_R)->y, Prime);
}

void ECDBL(Ecc_pt *EN_P, Ecc_pt *EN_R, bigint_st *Prime, bigint_st *a)
{
    bigint_st temp1 = {{0x00}, 0x00};
    bigint_st temp2 = {{0x00}, 0x00};
    bigint_st temp3 = {{0x00}, 0x00};

    //! X
    OS64MUL_256(&(EN_P->x), &(EN_P->x), &temp1, Prime);
    Addition(&temp1, &temp1, &temp2, Prime);
    Addition(&temp2, &temp1, &temp3, Prime);
    // show(&temp3);
    Addition(&temp3, a, &temp1, Prime);

    Addition(&(EN_P->y), &(EN_P->y), &temp2, Prime);
    Inverse_FLT(&temp2, &temp3, Prime);
    OS64MUL_256(&temp1, &temp3, &temp2, Prime);
    OS64MUL_256(&temp2, &temp2, &temp1, Prime);

    Subtraction(&temp1, &(EN_P->x), &temp2, Prime);
    Subtraction(&temp2, &(EN_P->x), &(EN_R->x), Prime);

    //! Y
    OS64MUL_256(&(EN_P->x), &(EN_P->x), &temp1, Prime);
    Addition(&temp1, &temp1, &temp2, Prime);
    Addition(&temp2, &temp1, &temp3, Prime);
    Addition(&temp3, a, &temp1, Prime);

    Addition(&(EN_P->y), &(EN_P->y), &temp2, Prime);
    Inverse_FLT(&temp2, &temp3, Prime);
    OS64MUL_256(&temp1, &temp3, &temp2, Prime);

    Subtraction(&(EN_P->x), &(EN_R->x), &temp1, Prime);
    OS64MUL_256(&temp1, &temp2, &temp3, Prime);
    Subtraction(&temp3, &(EN_P->y), &(EN_R->y), Prime);
}
void ECDBL_J(Ecc_Jpt *EN_P, Ecc_Jpt *EN_R, bigint_st *Prime)
{
    bigint_st T1 = {{0x00}, 0x00};
    bigint_st T2 = {{0x00}, 0x00};
    bigint_st T3 = {{0x00}, 0x00};
    bigint_st X3 = {{0x00}, 0x00};
    bigint_st Y3 = {{0x00}, 0x00};
    bigint_st Z3 = {{0x00}, 0x00};
    bigint_st temp = {{0x00}, 0x00};
    bigint_st temp2 = {{0x00}, 0x00};

    OS64MUL_256(&EN_P->z, &EN_P->z, &T1, Prime); //2
    Subtraction(&EN_P->x, &T1, &T2, Prime);      //3
    Addition(&EN_P->x, &T1, &temp, Prime);       //4
    copy_bigint(&T1, &temp);
    OS64MUL_256(&T2, &T1, &T2, Prime); //5

    Addition(&T2, &T2, &temp, Prime);    //6
    Addition(&temp, &T2, &temp2, Prime); //6
    copy_bigint(&T2, &temp2);

    Addition(&EN_P->y, &EN_P->y, &Y3, Prime); //7

    OS64MUL_256(&Y3, &EN_P->z, &Z3, Prime); //8

    OS64MUL_256(&Y3, &Y3, &temp, Prime); //9
    copy_bigint(&Y3, &temp);

    OS64MUL_256(&Y3, &EN_P->x, &T3, Prime); //10

    OS64MUL_256(&Y3, &Y3, &Y3, Prime); //11

    if (isEven(&Y3) == TRUE) //12
        RightShift(&Y3);
    else
    {
        Addition_NR(&Y3, Prime, &temp, Prime);
        RightShift(&temp);
        if (temp.e == 1)
            temp.a[WORD_LEN - 1] ^= 0x80000000;
        copy_bigint(&Y3, &temp);
    }

    OS64MUL_256(&T2, &T2, &X3, Prime); //13

    Addition(&T3, &T3, &T1, Prime); //14

    Subtraction(&X3, &T1, &temp, Prime); //15
    copy_bigint(&X3, &temp);

    Subtraction(&T3, &X3, &T1, Prime); //16

    OS64MUL_256(&T1, &T2, &temp, Prime);
    copy_bigint(&T1, &temp); //17

    reset_bigint(&temp);
    Subtraction(&T1, &Y3, &temp, Prime);
    copy_bigint(&Y3, &temp); //18

    set_EN_J_reverse(EN_R, X3.a, Y3.a, Z3.a);
}

void ECADD_J(Ecc_Jpt *EN_P, Ecc_pt *EN_Q, Ecc_Jpt *EN_R, bigint_st *Prime)
{
    bigint_st T1 = {{0x00}, 0x00};
    bigint_st T2 = {{0x00}, 0x00};
    bigint_st T3 = {{0x00}, 0x00};
    bigint_st T4 = {{0x00}, 0x00};
    bigint_st X3 = {{0x00}, 0x00};
    bigint_st Y3 = {{0x00}, 0x00};
    bigint_st Z3 = {{0x00}, 0x00};
    bigint_st temp = {{0x00}, 0x00};
    bigint_st temp2 = {{0x00}, 0x00};
    if (EN_Q->isinfinity == TRUE)
    {
        EN_R = EN_P;
    }
    if (EN_P->isinfinity == TRUE)
    {
        int cnt_i = 0;
        EN_R->x = EN_Q->x;
        EN_R->y = EN_Q->y;
        word Z1_array[8] = {0xFFFFFFFF, 0x00000001, 0x00000000, 0x00000000, 0x00000001, 0x00, 0x00, 0x00};
        for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
        {
            EN_R->z.a[cnt_i] = Z1_array[WORD_LEN - cnt_i - 1];
        }
    }

    OS64MUL_256(&EN_P->z, &EN_P->z, &T1, Prime); //3
    OS64MUL_256(&T1, &EN_P->z, &T2, Prime);      //4
    OS64MUL_256(&T1, &EN_Q->x, &T1, Prime);      //5
    OS64MUL_256(&T2, &EN_Q->y, &T2, Prime);      //6

    Subtraction(&T1, &EN_P->x, &temp, Prime); //7
    copy_bigint(&T1, &temp);

    Subtraction(&T2, &EN_P->y, &temp, Prime); //8
    copy_bigint(&T2, &temp);

    if (iszero(&T1) == TRUE) //! //9
    {
        printf(" T1 is zero\n");
        if (iszero(&T2) == TRUE)
        {
            Ecc_Jpt temp = {{0x00}, 0x00};
            Ecc_Jpt temp2 = {{0x00}, 0x00};
            Trns_A_to_J(&temp, EN_Q, Prime);
            ECDBL_J(&temp, &temp2, Prime);
            EN_R = &temp2;
            printf(" T2 is zero\n");
            return;
        }
        else
        {
            printf("infinity\n");
            EN_R->isinfinity = TRUE;
            return;
        }
    }

    OS64MUL_256(&EN_P->z, &T1, &Z3, Prime); //10
    OS64MUL_256(&T1, &T1, &T3, Prime);      //11
    OS64MUL_256(&T3, &T1, &T4, Prime);      //12
    OS64MUL_256(&T3, &EN_P->x, &T3, Prime); //13
    Addition(&T3, &T3, &T1, Prime);         //14
    OS64MUL_256(&T2, &T2, &X3, Prime);      //15
    Subtraction(&X3, &T1, &temp, Prime);    //16
    copy_bigint(&X3, &temp);

    Subtraction(&X3, &T4, &temp, Prime); //17
    copy_bigint(&X3, &temp);

    Subtraction(&T3, &X3, &temp, Prime); //18
    copy_bigint(&T3, &temp);

    OS64MUL_256(&T3, &T2, &T3, Prime);      //19
    OS64MUL_256(&T4, &EN_P->y, &T4, Prime); //20

    Subtraction(&T3, &T4, &Y3, Prime); //21

    set_EN_J_reverse(EN_R, X3.a, Y3.a, Z3.a);
}
void ECLtoR(Ecc_pt *EN_P, bigint_st *K, Ecc_pt *EN_R, bigint_st *Prime, bigint_st *a)
{
    int cnt_i = 0, cnt_j = 0;
    int DC = 0x00;
    Ecc_pt temp1 = {{0x00}, 0x00};
    Ecc_pt temp2 = {{0x00}, 0x00};
    Ecc_pt temp3 = {{0x00}, 0x00};
    int find_first_bit = 0;

    int count = 0; //?
    //!reset?
    for (cnt_i = WORD_LEN - 1; cnt_i >= 0; cnt_i--)
    {
        for (cnt_j = 31; cnt_j >= 0; cnt_j--)
        {
            if (find_first_bit == 1)
            {
                ECDBL(&temp1, &temp2, Prime, a);
                DC = ((K->a[cnt_i]) >> cnt_j) & 0x01;
                if (DC == 1)
                {
                    ECADD(&temp2, EN_P, &temp3, Prime);
                    EN_copy(&temp1, &temp3);
                    continue;
                }
                EN_copy(&temp1, &temp2);
                continue;
            }

            DC = ((K->a[cnt_i]) >> cnt_j) & 0x01;
            if (DC == 0)
                continue;
            if (DC == 1)
            {
                EN_copy(&temp1, EN_P);
                find_first_bit = 1;
            }
            continue;
        }
    }
    EN_copy(EN_R, &temp1);
}
void ECLtoR_J(Ecc_Jpt *EN_P, bigint_st *K, Ecc_Jpt *EN_R, bigint_st *Prime)
{
    int cnt_i = 0, cnt_j = 0;
    int DC = 0x00;
    Ecc_Jpt temp1 = {{0x00}, 0x00};
    Ecc_Jpt temp2 = {{0x00}, 0x00};
    Ecc_Jpt temp3 = {{0x00}, 0x00};
    Ecc_pt ttemp = {{0x00}, 0x00};
    int find_first_bit = 0;

    int count = 0; //?
    //!reset?
    int abc = 0;
    for (cnt_i = WORD_LEN - 1; cnt_i >= 0; cnt_i--)
    {
        for (cnt_j = 31; cnt_j >= 0; cnt_j--)
        {
            if (find_first_bit == 1)
            {
                ECDBL_J(&temp1, &temp2, Prime);
                DC = ((K->a[cnt_i]) >> cnt_j) & 0x01;
                if (DC == 1)
                {
                    Trns_J_to_A(&ttemp, EN_P, Prime);
                    ECADD_J(&temp2, &ttemp, &temp3, Prime);
                    EN_J_copy(&temp1, &temp3);
                    continue;
                }
                EN_J_copy(&temp1, &temp2);
                continue;
            }
            DC = ((K->a[cnt_i]) >> cnt_j) & 0x01;
            if (DC == 0)
                continue;
            if (DC == 1)
            {
                EN_J_copy(&temp1, EN_P);
                find_first_bit = 1;
            }
            continue;
        }
    }
    EN_J_copy(EN_R, &temp1);
}

void ECRtoL(Ecc_pt *EN_P, bigint_st *K, Ecc_pt *EN_R, bigint_st *Prime, bigint_st *a)
{
    int cnt_i = 0, cnt_j = 0;
    int DC = 0x00;
    Ecc_pt temp1 = {{0x00}, 0x00};
    Ecc_pt temp2 = {{0x00}, 0x00};
    Ecc_pt PA = {{0x00}, 0x00};
    Ecc_pt A = {{0x00}, 0x00};
    int count = 0x00;
    int first_bit = 0;

    EN_copy(&PA, EN_P);

    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        for (cnt_j = 0; cnt_j < 32; cnt_j++)
        {
            DC = ((K->a[cnt_i]) >> cnt_j) & 0x01;
            if (DC == 1)
            {
                first_bit += 1;
                if (first_bit == 1)
                {
                    EN_copy(&A, &PA);
                    ECDBL(&PA, &temp1, Prime, a);
                    EN_copy(&PA, &temp1);
                }
                else
                {
                    ECADD(&A, &PA, &temp1, Prime);
                    EN_copy(&A, &temp1);
                    ECDBL(&PA, &temp1, Prime, a);
                    EN_copy(&PA, &temp1);
                }
            }
            else
            {
                ECDBL(&PA, &temp1, Prime, a);
                EN_copy(&PA, &temp1);
            }
        }
        EN_copy(EN_R, &A);
    }
    return;
}

void NAF_recoding(bigint_st *Scalar, char *NAF, bigint_st *Prime)
{
    int cnt_i = 0;
    bigint_st one = {{0x00}, 0x00};
    bigint_st K = {{0x00}, 0x00};
    bigint_st temp = {{0x00}, 0x00};
    bigint_st temp2 = {{0x00}, 0x00};
    bigint_st temp3 = {{0x00}, 0x00};
    char modtemp = 0x00;
    copy_bigint(&K, Scalar);
    one.a[0] = 0x01;

    while (Compare(&K, &one) != SCD_IS_BIG)
    {
        if (isEven(&K) != TRUE)
        {
            NAF[cnt_i] = K.a[0] & 0x0000000f;
            if (NAF[cnt_i] > 8)
            {
                NAF[cnt_i] = NAF[cnt_i] - 16;
                modtemp = NAF[cnt_i];
                modtemp = ~(modtemp) + 1;
                temp.a[0] = modtemp;
                Addition(&K, &temp, &temp2, Prime);
                copy_bigint(&K, &temp2);
            }
            else
            {
                temp.a[0] = NAF[cnt_i];
                Subtraction(&K, &temp, &temp2, Prime);
                copy_bigint(&K, &temp2);
            }
        }
        else
        {
            NAF[cnt_i] = 0x00;
        }
        RightShift(&K);
        cnt_i++;
    }
}

void ECLtoR_J_wNAF(Ecc_Jpt *EN_P, char *NAF, Ecc_Jpt *EN_R, bigint_st *Prime)
{
    int cnt_i = 0, cnt_j = 0;
    int DC = 0x00;
    Ecc_Jpt temp1 = {{0x00}, 0x00};
    Ecc_Jpt temp2 = {{0x00}, 0x00};
    Ecc_Jpt temp3 = {{0x00}, 0x00};
    Ecc_Jpt Pi[4] = {{0x00}, 0x00};
    Ecc_Jpt Pi2[4] = {{0x00}, 0x00};
    Ecc_pt ttemp = {{0x00}, 0x00};
    char temp = 0;

    EN_J_copy(Pi, EN_P); //* 1

    ECDBL_J(EN_P, &temp1, Prime);
    Trns_J_to_A(&ttemp, &temp1, Prime);
    ECADD_J(EN_P, &ttemp, &temp2, Prime);
    EN_J_copy(Pi + 1, &temp2); //* 3
    ECADD_J(Pi + 1, &ttemp, &temp3, Prime);
    EN_J_copy(Pi + 2, &temp3); //* 5
    ECADD_J(Pi + 2, &ttemp, &temp3, Prime);
    EN_J_copy(Pi + 3, &temp3); //* 7

    EN_J_copy(Pi2, Pi); //* 1
    Subtraction(Prime, &(Pi[0].y), &(Pi2[0].y), Prime);
    EN_J_copy(Pi2 + 1, Pi + 1); //* 3
    Subtraction(Prime, &(Pi[1].y), &(Pi2[1].y), Prime);
    EN_J_copy(Pi2 + 2, Pi + 2); //* 5
    Subtraction(Prime, &(Pi[2].y), &(Pi2[2].y), Prime);
    EN_J_copy(Pi2 + 3, Pi + 3); //* 7
    Subtraction(Prime, &(Pi[3].y), &(Pi2[3].y), Prime);

    int find_first_bit = 0;
    //!reset?
    for (cnt_i = WORD_LEN * WORD_BITLEN + 1; cnt_i >= 0; cnt_i--)
    {
        if (find_first_bit == 1)
        {
            ECDBL_J(&temp1, &temp2, Prime);
           
            if (NAF[cnt_i] != 0x00)
            {
                if (NAF[cnt_i] > 0x00)
                {
                    Trns_J_to_A(&ttemp, &Pi[((NAF[cnt_i] - 1) / 2)], Prime);
                    ECADD_J(&temp2, &ttemp, &temp3, Prime);
                    EN_J_copy(&temp1, &temp3);
                    continue;
                }
                else
                {
                    Trns_J_to_A(&ttemp, &Pi2[((-NAF[cnt_i] - 1) / 2)], Prime);
                    ECADD_J(&temp2, &ttemp, &temp3, Prime);
                    EN_J_copy(&temp1, &temp3);
                    continue;
                }
            }
            EN_J_copy(&temp1, &temp2);
            continue;
        }
        if (NAF[cnt_i] == 0)
            continue;
        if (NAF[cnt_i] != 0)
        {
            EN_J_copy(&temp1, &Pi[((NAF[cnt_i] - 1) / 2)]);
            find_first_bit = 1;
        }
        continue;
    }
    EN_J_copy(EN_R, &temp1);
}

void comb_Table(char table[WORD_LEN][WORD_BITLEN], Ecc_pt *J_table, Ecc_pt *EN_P, bigint_st *K, bigint_st *Prime)
{
    int cnt_i, cnt_j;
    Ecc_Jpt temp = {0x00};
    Ecc_Jpt temp2 = {0x00};
    Ecc_pt ttemp = {0x00};
    EN_copy(J_table, EN_P);   
    EN_copy(J_table + 1, EN_P); 
    for (cnt_i = 1; cnt_i < 8; cnt_i++)
    {
        for (cnt_j = 0; cnt_j < 32; cnt_j++)
        {
            Trns_A_to_J(&temp, J_table + cnt_i, Prime);
            ECDBL_J(&temp, &temp2, Prime);
            Trns_J_to_A(&ttemp, &temp2, Prime);
            EN_copy(J_table + cnt_i, &ttemp);
        }
        EN_copy(&J_table[cnt_i + 1], &J_table[cnt_i]); 
    }

    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        for (cnt_j = 0; cnt_j < WORD_BITLEN; cnt_j++)
        {
            table[cnt_i][cnt_j] = (K->a[cnt_i] >> (WORD_BITLEN - cnt_j - 1)) & 0x01;
        }
    }

}
void ECLtoR_J_comb(Ecc_Jpt *EN_P, char table[WORD_LEN][WORD_BITLEN], Ecc_pt *J_Table, Ecc_Jpt *EN_R, bigint_st *Prime)
{
    int cnt_i = 0, cnt_j = 0;
    int bit_check = 0;
    Ecc_Jpt jtemp = {{0x00}, 0x00};
    Ecc_Jpt jtemp2 = {{0x00}, 0x00};
    Ecc_pt Q = {0x00,};

    for (cnt_i = 0; cnt_i < WORD_BITLEN; cnt_i++)
    {
        for (cnt_j = 0; cnt_j < WORD_LEN; cnt_j++)
        {
            if (table[cnt_j][cnt_i] == 1)
            {
                bit_check += 1;
                if (bit_check == 1)
                {
                    Trns_A_to_J(&jtemp, &J_Table[cnt_j], Prime);
                    EN_J_copy(&jtemp2, &jtemp);
                }
                else
                {
                    ECADD_J(&jtemp2, J_Table + cnt_j, &jtemp, Prime);
                    EN_J_copy(&jtemp2, &jtemp);
                }
            }
        }
        ECDBL_J(&jtemp2, &jtemp, Prime);
        EN_J_copy(EN_R, &jtemp2);
        EN_J_copy(&jtemp2, &jtemp);
    }
}

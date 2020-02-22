#include "Bignumber.h"

long long cpucycles()
{
    return __rdtsc();
}

void set_bigint(bigint_st *bi_X, word *Input) //? Big_int 구조체에 값 설정해주는 함수
{
    int cnt_i;
    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        bi_X->a[cnt_i] = Input[WORD_LEN - (cnt_i + 1)]; //Input된 배열의 값을 실질적으로 대입해줌
    }
    bi_X->e = 0;
}

void reset_bigint(bigint_st *bi_X) //?Big_int 구조체 0으로 초기화 해주는 함수
{
    int cnt_i;
    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        bi_X->a[cnt_i] = 0x00; //Input된 배열의 값을 실질적으로 대입해줌
    }
    bi_X->e = 0;
}
void copy_bigint(bigint_st *dst, bigint_st *src)
{
    int cnt_i;
    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        dst->a[cnt_i] = src->a[cnt_i]; //Input된 배열의 값을 실질적으로 대입해줌
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
        bi_Z->a[cnt_i] = bi_X->a[cnt_i] + bi_Y->a[cnt_i] + carry; // 단순 덧셈. modulo는 자동적으로 작동
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
{
    int cnt_i, cnt_j; //for loop counting variable
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

void Reduction_256(word *bi_X, bigint_st *bi_Z, bigint_st *Prime)
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

void Inverse_FLT(bigint_st *bi_X, bigint_st *bi_Z, bigint_st *Prime) //? Inv 함수
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

    //todo t
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
void Inverse_EEA(bigint_st *bi_X, bigint_st *bi_Z, bigint_st *Prime)
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
            if (cnt_i == (WORD_LEN - 1))
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
            ECDBL(&temp1, &temp2, Prime, a);
            DC = ((K->a[cnt_i]) >> cnt_j) & 0x01;
            if (DC == 1)
            {
                ECADD(&temp2, EN_P, &temp3, Prime);
                EN_copy(&temp1, &temp3);
                continue;
            }
            EN_copy(&temp1, &temp2);
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
            // printf("%d\n",abc);//!
            // abc ++;//!
            if (cnt_i == (WORD_LEN - 1))
            {
                if (find_first_bit == 1)
                {
                    ECDBL_J(&temp1, &temp2, Prime);
                    // show_EN_J(&temp2); //!
                    DC = ((K->a[cnt_i]) >> cnt_j) & 0x01;
                    if (DC == 1)
                    {
                        Trns_J_to_A(&ttemp, EN_P, Prime);
                        ECADD_J(&temp2, &ttemp, &temp3, Prime);
                        // show_EN_J(&temp3);//!
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
            ECDBL_J(&temp1, &temp2, Prime);
            // show_EN_J(&temp2); //!
            DC = ((K->a[cnt_i]) >> cnt_j) & 0x01;
            if (DC == 1)
            {
                Trns_J_to_A(&ttemp, EN_P, Prime);
                ECADD_J(&temp2, &ttemp, &temp3, Prime); //!
                // show_EN_J(&temp3);//!
                EN_J_copy(&temp1, &temp3);
                continue;
            }
            EN_J_copy(&temp1, &temp2);
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
            NAF[cnt_i] = K.a[0] & 0x0f;
            temp.a[0] = NAF[cnt_i];
            Subtraction(&K, &temp, &temp2, Prime);
            copy_bigint(&K, &temp2);
            if(NAF[cnt_i] > 8)
                NAF[cnt_i] = NAF[cnt_i] - 16;   
        }
        else
        {
            NAF[cnt_i] = 0x00;
        }
        RightShift(&K);
        cnt_i++;
    }
    // for(cnt_i = 255 ; cnt_i>=0; cnt_i --)
    // {
    //     printf("%d ",NAF[cnt_i]);
    // }
   
}
void Find_Pi(Ecc_pt *Pi, char *NAF, int *cnt_i, Ecc_pt *EN_R, bigint_st *Prime, bigint_st *a)
{
    char k = NAF[*cnt_i];
    Ecc_pt Prime_pt = {{0x00}, 0x00};
    Ecc_pt temp = {{0x00}, 0x00};
    copy_bigint(&(Prime_pt.x), Prime);
    copy_bigint(&(Prime_pt.y), Prime);

    if (k >= 0)
    {
        EN_R = &Pi[((NAF[*cnt_i] - 1) / 2)];
    }

    else
    {
        EN_R = &Pi[((-NAF[*cnt_i] - 1) / 2)];
        ECADD(EN_R, &Prime_pt, &temp, Prime);
        EN_copy(EN_R, &temp);
    }
}

void ECLtoR_wNAF(Ecc_pt *EN_P, char *NAF, Ecc_pt *EN_R, bigint_st *Prime, bigint_st *a)
{
    int cnt_i = 0, cnt_j = 0;
    int DC = 0x00;
    Ecc_pt temp1 = {{0x00}, 0x00};
    Ecc_pt temp2 = {{0x00}, 0x00};
    Ecc_pt temp3 = {{0x00}, 0x00};
    Ecc_pt temp_P = {{0x00}, 0x00};
    Ecc_pt Pi[4] = {{0x00}, 0x00};

    Pi[0] = *EN_P; //* 1
    ECDBL(EN_P, &temp1, Prime, a);
    ECADD(EN_P, &temp1, &temp2, Prime);
    Pi[1] = temp2; //* 3
    ECADD(&temp1, &temp2, &temp3, Prime);
    Pi[2] = temp3; //* 5
    ECADD(&temp1, &temp3, &temp2, Prime);
    Pi[3] = temp2; //* 7

    int find_first_bit = 0;
    int count = 0; //?
    //!reset?
    for (cnt_i = WORD_LEN * WORD_BITLEN - 1; cnt_i >= 0; cnt_i--)
    {

        if (cnt_i == WORD_LEN * WORD_BITLEN - 1)
        {
            if (find_first_bit == 1)
            {
                ECDBL(&temp1, &temp2, Prime, a);
                if (NAF[cnt_i] != 0x00)
                {
                    if (NAF[cnt_i] > 0x00)
                    {
                        Find_Pi(Pi, NAF, &cnt_i, &temp_P, Prime, a);
                        ECADD(&temp2, &temp_P, &temp3, Prime);
                        EN_copy(&temp1, &temp3);
                        continue;
                    }
                    else
                    {
                        Find_Pi(Pi, NAF, &cnt_i, &temp_P, Prime, a);
                        ECADD(&temp2, &temp_P, &temp3, Prime);
                        EN_copy(&temp1, &temp3); //!
                        continue;
                    }
                }
                EN_copy(&temp1, &temp2);
                continue;
            }

            if (NAF[cnt_i] == 0)
                continue;
            if (NAF[cnt_i] != 0)
            {
                EN_copy(&temp1, EN_P);
                find_first_bit = 1;
            }
            continue;
        }
        ECDBL(&temp1, &temp2, Prime, a);
        if (NAF[cnt_i] != 0x00)
        {
            if (NAF[cnt_i] > 0x00)
            {
                Find_Pi(Pi, NAF, &cnt_i, &temp_P, Prime, a);
                ECADD(&temp2, &temp_P, &temp3, Prime);
                EN_copy(&temp1, &temp3);
                continue;
            }
            else
            {
                Find_Pi(Pi, NAF, &cnt_i, &temp_P, Prime, a);
                ECADD(&temp2, &temp_P, &temp3, Prime);
                EN_copy(&temp1, &temp3); //!
                continue;
            }
        }
        EN_copy(&temp1, &temp2);
        continue;
    }
    EN_copy(EN_R, &temp1);
    
}
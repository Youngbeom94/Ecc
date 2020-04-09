#include "Bignumber.h"

long long cpucycles()
{
    return __rdtsc();
}
void swap(word *A, word *B)
{
    word temp;
    temp = *A;
    *A = *B;
    *B = temp;
}

void delete_bigint(bigint_st_ptr *bi_X) //? 구조체에 할당된 메모리를 지우는 작업과 동시에, 초기화를 시켜줌.
{
    int cnt_i;
    if (*bi_X == NULL)
        return;
    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        (*bi_X)->a[cnt_i] = 0x0; //기존  구조체 메모리가 점유하는 부분의 값을 초기화
    }
    free((*bi_X)->a); //Big_int 구조체 멤버변수 a에 할당된 메모리 free
    free(*bi_X);      //Big_ing 구조체 포인터에 할당된 메모리 free
    *bi_X = NULL;
}

void set_bigint(bigint_st_ptr *bi_X, word *Input) //? Big_int 구조체에 값 설정해주는 함수
{
    if (*bi_X != NULL) //기존의 포인터값이 할당되어있을경우 해제
        delete_bigint(bi_X);

    *bi_X = (bigint_st *)calloc(1, sizeof(bigint_st));
    (*bi_X)->a = (word *)calloc(WORD_LEN, sizeof(word));

    int cnt_i;
    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        (*bi_X)->a[cnt_i] = Input[WORD_LEN - (cnt_i + 1)]; //Input된 배열의 값을 실질적으로 대입해줌
    }
    (*bi_X)->e = 0;
}
void set_bigint_reset(bigint_st_ptr *bi_X) //? Big_int 구조체 0으로 초기화 해주는 함수
{
    if (*bi_X != NULL) //기존의 포인터값이 할당되어있을경우 해제
        delete_bigint(bi_X);

    *bi_X = (bigint_st *)calloc(1, sizeof(bigint_st));
    (*bi_X)->a = (word *)calloc(WORD_LEN, sizeof(word));

    int cnt_i;
    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        (*bi_X)->a[cnt_i] = 0x00; //Input된 배열의 값을 실질적으로 대입해줌
    }
    (*bi_X)->e = 0;
}
void set_bigint_reset_mul(bigint_st_ptr *bi_X) //? Big_int 구조체  할당 및 0으로 초기화 해주는 함수
{
    if (*bi_X != NULL) //기존의 포인터값이 할당되어있을경우 해제
        delete_bigint(bi_X);

    *bi_X = (bigint_st *)calloc(1, sizeof(bigint_st));
    (*bi_X)->a = (word *)calloc(WORD_LEN * 2, sizeof(word));

    int cnt_i;
    for (cnt_i = 0; cnt_i < WORD_LEN * 2; cnt_i++)
    {
        (*bi_X)->a[cnt_i] = 0x00; //Input된 배열의 값을 실질적으로 대입해줌
    }
    (*bi_X)->e = 0;
}
void reset_bigint(bigint_st_ptr *bi_X) //?Big_int 구조체 0으로 초기화 해주는 함수
{
    int cnt_i;
    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        (*bi_X)->a[cnt_i] = 0x00; //Input된 배열의 값을 실질적으로 대입해줌
    }
    (*bi_X)->e = 0;
}

void show(bigint_st_ptr *bi_X) //? 숫자를 보여주는 함수
{
    int cnt_i;

    printf("0x");
    for (cnt_i = WORD_LEN - 1; cnt_i >= 0; cnt_i--)
    {
#if WORD_BITLEN == 32
        printf("%08X", (*bi_X)->a[cnt_i]); // Wordbitlen = 32일 경우 즉 4byte가 1워드인경우
#else
        printf("%016llX", (*bi_X)->a[cnt_i]); // Wordbitlen = 64일 경우 즉 8byte가 1워드인경우
#endif
    }
    printf("\n");
}

void integer_copy(bigint_st_ptr *bi_dst, bigint_st_ptr *bi_src) //? bigint_src를 bigint_dst에 복사하는 함수
{
    if (*bi_dst != NULL)
        delete_bigint(bi_dst);

    (*bi_dst) = (bigint_st *)calloc(1, sizeof(bigint_st)); //bigint_dst 포인터에 구조체 할당
    (*bi_dst)->a = (word *)calloc(WORD_LEN, sizeof(word)); //구조체 할당 후 구조체 멤버변수에 워드 할당
    for (int cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        (*bi_dst)->a[cnt_i] = (*bi_src)->a[cnt_i]; // 할당된 워드에 복사할 값을 대입
    }
    (*bi_dst)->e = (*bi_src)->e; //src의 wordlen 맞춰주기
}

unsigned int get_rand_32bits() //? 32bit의 랜덤 수 생성
{
    unsigned int temp = (rand() * rand()) & 0xffffffff;
    return temp; //return 값은 총 32비트의 랜덤 수 이다
}

unsigned long long get_rand_64bits() //64bit도 32bit와 동일하게 만들어준다.
{
    unsigned long long temp = get_rand_32bits(); // 32bit의 랜덤 수 생성
    temp = temp << 32;                           //32bit left shifting
    temp ^= get_rand_32bits();                   //총 64bit 랜덤 수 생성
    return temp;
}

#if WORD_BITLEN == 32
word get_random_word()
{
    return (word)get_rand_32bits();
}
#else
word get_random_word()
{
    return (word)get_rand_64bits();
}
#endif

void get_random_array(word dst[]) //? Word의 배열과 , Wordlen(size)를 받을떄 랜덤값을 대입해 주는 함수
{
    int cnt_j = 0;
    for (cnt_j = 0; cnt_j < WORD_LEN; cnt_j++)
    {
        dst[cnt_j] = get_random_word(); //각각 배열의 원소에 랜덤 값 대입
    }
}

void generate_random_integer(bigint_st_ptr *bi_X, bigint_st_ptr *Prime) //? 랜덤한 숫자를 갖는 bigint 구조체 만들어 주는 함수
{
    if (*bi_X != NULL)
        delete_bigint(bi_X);

    word *Input;                                    // Word의 배열을 동적할당으로 만들어주고 배열에 랜덤값을 대입, 최종적으로 Set_bigint 함수의 input값을 맞춰줌
    Input = (word *)calloc(WORD_LEN, sizeof(word)); // 배열 동적할당
    get_random_array(Input);                        // 기존에 만들어 놓았던 랜덤 함수를 이용 값을 대입시켜 줌.

    set_bigint(bi_X, Input);
    free(Input); // 할당된 메모리를 Free
    Reduction(bi_X, Prime);
}

void swap_integer(bigint_st_ptr *bi_X, bigint_st_ptr *bi_Y) //? Swap_integer
{
    bigint_st_ptr temp = NULL; //Swap함수도 기존 정수 Swap함수와 마찬가지로 포인터를 만들어줘서 Swap 해주면 된다.
    temp = (bigint_st_ptr)calloc(1, sizeof(bigint_st));

    temp = *bi_X;
    *bi_X = *bi_Y;
    *bi_Y = temp;
}

int Compare(bigint_st_ptr *bi_X, bigint_st_ptr *bi_Y) //? bi_X,bi_Y의 크기를  비교해주는 함수
{

    int cnt_i;
    for (cnt_i = WORD_LEN - 1; cnt_i >= 0; cnt_i--)
    {
        if ((*bi_X)->a[cnt_i] > (*bi_Y)->a[cnt_i]) // 배열의 각 word를 비교하기
            return FST_IS_BIG;                     // First is big (bi_X>bi_Y)
        if ((*bi_X)->a[cnt_i] < (*bi_Y)->a[cnt_i])
            return SCD_IS_BIG; // Second is big
    }
    return BOTH_ARE_SAME; // 만약 해당하는것이 아무것도 없는경우 두수는 같다고 표현

    return ERROR;
}

int Compare_mul(bigint_st_ptr *bi_X, bigint_st_ptr *bi_Y) //? bi_X,bi_Y의 크기를  비교해주는 함수
{

    int cnt_i;
    for (cnt_i = WORD_LEN * 2 - 1; cnt_i >= 0; cnt_i--)
    {
        if ((*bi_X)->a[cnt_i] > (*bi_Y)->a[cnt_i]) // 배열의 각 word를 비교하기
            return FST_IS_BIG;                     // First is big (bi_X>bi_Y)
        if ((*bi_X)->a[cnt_i] < (*bi_Y)->a[cnt_i])
            return SCD_IS_BIG; // Second is big
    }
    return BOTH_ARE_SAME; // 만약 해당하는것이 아무것도 없는경우 두수는 같다고 표현

    return ERROR;
}

void Addition(bigint_st_ptr *bi_X, bigint_st_ptr *bi_Y, bigint_st_ptr *bi_Z, bigint_st_ptr *Prime)
{
    int cnt_i;     //for loop counting variable
    int carry = 0; // Carry

    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++) //둘의 WORD_LEN이 같으므로 한번에 계산 가능하다
    {
        (*bi_Z)->a[cnt_i] = (*bi_X)->a[cnt_i] + (*bi_Y)->a[cnt_i] + carry; // 단순 덧셈. modulo는 자동적으로 작동
        if ((*bi_X)->a[cnt_i] == (*bi_Z)->a[cnt_i])
            continue;
        carry = (*bi_X)->a[cnt_i] > (*bi_Z)->a[cnt_i] ? 1 : 0;
    }
    (*bi_Z)->e = carry; // 구조체의 e값은 carry값이므로 그대로 대입해준다.
    Reduction(bi_Z, Prime);
}

int sub_Core_borrow(word a, word b) //? Addition에서는 carry, Subtraction에서는 borrow.  borrow를 생각해주는 함수이다.
{
    if (a >= b)
        return 0;

    return 1;
}

void Subtraction(bigint_st_ptr *bi_X, bigint_st_ptr *bi_Y, bigint_st_ptr *bi_Z, bigint_st_ptr *Prime)
{
    int borrow = 0;
    int cnt_i = 0;
    int carry = 0;
    unsigned int temp[WORD_LEN] = {0x00};

    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++) //WORD_LEN의 길이만큼 뺼셈을 해준다. 루프안의 각각경우마다 두번의 뺄셈이들어간다.
    {                                          //1번째 뺄셈은 bi_X와 borrow값, 2번쨰 뺄셈은 1번쨰 뺼셈의 결과와 bi_Y.
        (*bi_Z)->a[cnt_i] = (*bi_X)->a[cnt_i] - borrow;
        borrow = sub_Core_borrow((*bi_X)->a[cnt_i], borrow); //borrow는 연속적으로 계산을 해줘야한다.
        borrow += sub_Core_borrow((*bi_Z)->a[cnt_i], (*bi_Y)->a[cnt_i]);
        (*bi_Z)->a[cnt_i] = (*bi_Z)->a[cnt_i] - (*bi_Y)->a[cnt_i];
    }
    (*bi_Z)->e = borrow;

    if ((*bi_Z)->e == 1) // borrow가 1인경우 감산을 해줘야하는데 뺄셈은, P값을 더해주어야한다.알고리즘은 덧셈과 동일
    {
        for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
        {
            temp[cnt_i] = (*bi_Z)->a[cnt_i] + (*Prime)->a[cnt_i] + carry;
            if (temp[cnt_i] == (*bi_Z)->a[cnt_i])
                continue;
            carry = temp[cnt_i] < (*bi_Z)->a[cnt_i] ? 1 : 0;
            (*bi_Z)->a[cnt_i] = temp[cnt_i];
        }
        (*bi_Z)->e = 0;
    }
}

void Reduction(bigint_st_ptr *bi_X, bigint_st_ptr *Prime) //? 감산 함수.
{
    if ((*bi_X)->e == 1) //P값보다 크거나 carry값이 1일때
    {
        unsigned int temp[WORD_LEN] = {0x00};
        int borrow = 0;
        int cnt_i = 0;
        for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++) //감산해주는 함수는 뺄셈과 동일한 알고리즘을 가진다.
        {
            temp[cnt_i] = (*bi_X)->a[cnt_i] - borrow;
            borrow = sub_Core_borrow((*bi_X)->a[cnt_i], borrow);
            borrow += sub_Core_borrow(temp[cnt_i], (*Prime)->a[cnt_i]);
            temp[cnt_i] = temp[cnt_i] - (*Prime)->a[cnt_i];
        }

        for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
        {
            (*bi_X)->a[cnt_i] = temp[cnt_i];
        }
    }
}

void OS_64_Multiplication(bigint_st_ptr *bi_X, bigint_st_ptr *bi_Y, bigint_st_ptr *bi_Z, bigint_st_ptr *Prime)
{
    int cnt_i, cnt_j; //for loop counting variable
    unsigned long long UV = 0x00LL;
    unsigned int U, V = 0x00;
    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        (*bi_Z)->a[cnt_i] = 0x00;
    }

    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        UV &= 0x00000000ffffffff;
        for (cnt_j = 0; cnt_j < WORD_LEN; cnt_j++)
        {
            U = UV >> 32;
            UV = (*bi_Z)->a[cnt_i + cnt_j] + ((unsigned long long)(*bi_X)->a[cnt_i] * (unsigned long long)(*bi_Y)->a[cnt_j]) + U;
            V = UV & 0x00000000ffffffff;
            (*bi_Z)->a[cnt_i + cnt_j] = V;
        }
        U = UV >> 32;
        (*bi_Z)->a[cnt_i + WORD_LEN] = U;
    }
    Reduction_256(bi_Z, Prime);
}

void SPlit_Word_Mul(word *A, word *B, word *C)
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
void OS_SPLIT_Multiplication(bigint_st_ptr *bi_X, bigint_st_ptr *bi_Y, bigint_st_ptr *bi_Z, bigint_st_ptr *Prime)
{
    int cnt_i, cnt_j; //for loop counting variable
    word UV[2] = {0x00};
    unsigned int U;

    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        UV[1] = 0x00;
        for (cnt_j = 0; cnt_j < WORD_LEN; cnt_j++)
        {
            U = UV[1];
            SPlit_Word_Mul(&(*bi_X)->a[cnt_i], &(*bi_Y)->a[cnt_j], UV);
            UV[0] += U;
            if (UV[0] < U)
                UV[1]++;
            UV[0] += (*bi_Z)->a[cnt_i + cnt_j];
            if (UV[0] < (*bi_Z)->a[cnt_i + cnt_j])
                UV[1]++;
            (*bi_Z)->a[cnt_i + cnt_j] = UV[0];
        }
        (*bi_Z)->a[cnt_i + WORD_LEN] = UV[1];
    }
    Reduction_256(bi_Z, Prime);
}

void PS_64_Multiplication(bigint_st_ptr *bi_X, bigint_st_ptr *bi_Y, bigint_st_ptr *bi_Z, bigint_st_ptr *Prime)
{

    int cnt_i, cnt_j; //for loop counting variable
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

            AH = (*bi_X)->a[cnt_i] >> 16;
            AL = (*bi_X)->a[cnt_i] & 0x0000ffff;
            BH = (*bi_Y)->a[cnt_j] >> 16;
            BL = (*bi_Y)->a[cnt_j] & 0x0000ffff;

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
            AH = (*bi_X)->a[cnt_i] >> 16;
            BH = (*bi_Y)->a[cnt_j] >> 16;
            AL = (*bi_X)->a[cnt_i] & 0x0000ffff;
            BL = (*bi_Y)->a[cnt_j] & 0x0000ffff;

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
        (*bi_Z)->a[cnt_i] = temp3[cnt_i];
    }
    Reduction_256(bi_Z, Prime);
}

//V =  UV[0]     U = UV[1] V = UV[0]
void PS_SPLIT_Multiplication(bigint_st_ptr *bi_X, bigint_st_ptr *bi_Y, bigint_st_ptr *bi_Z, bigint_st_ptr *Prime)
{

    int cnt_i = 0, cnt_j = 0; //for loop counting variable
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

            AH = (*bi_X)->a[cnt_i] >> 16;
            AL = (*bi_X)->a[cnt_i] & 0x0000ffff;
            BH = (*bi_Y)->a[cnt_j] >> 16;
            BL = (*bi_Y)->a[cnt_j] & 0x0000ffff;

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
            AH = (*bi_X)->a[cnt_i] >> 16;
            BH = (*bi_Y)->a[cnt_j] >> 16;
            AL = (*bi_X)->a[cnt_i] & 0x0000ffff;
            BL = (*bi_Y)->a[cnt_j] & 0x0000ffff;

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
        (*bi_Z)->a[cnt_i] = temp3[cnt_i];
    }
    Reduction_256(bi_Z, Prime);
}

void Square_64_Multiplication(bigint_st_ptr *bi_X, bigint_st_ptr *bi_Z, bigint_st_ptr *Prime)
{
    int cnt_i, cnt_j, cnt_k; //for loop counting variable
    unsigned int R0 = 0x00;
    unsigned int R1 = 0x00;
    unsigned int R2 = 0x00;
    unsigned long long UV = 0x00LL;
    unsigned long long UV2 = 0x00LL;
    unsigned int U, V = 0x00;
    int carry = 0;
    int carry2 = 0;
    int carry3 = 0;

    for (cnt_k = 0; cnt_k < 2 * WORD_LEN - 1; cnt_k++)
    {
        for (cnt_i = cnt_k >> 1, cnt_j = cnt_i + (cnt_k & 1); (cnt_i >= 0) && (cnt_j < WORD_LEN); cnt_j++, cnt_i--)
        {
            UV = ((unsigned long long)(*bi_X)->a[cnt_i] * (unsigned long long)(*bi_X)->a[cnt_j]);
            if (cnt_i < cnt_j)
            {
                UV2 = UV >> 60;
                carry = UV2 * 2 > 0xf ? 1 : 0;
                UV = UV << 1LL;
                R2 += carry;
            }

            V = UV & 0x00000000ffffffff;
            U = UV >> 32;

            R0 += V;
            carry = R0 < V ? 1 : 0;

            R1 += U;
            carry2 = R1 < U ? 1 : 0;
            R1 += carry;
            carry3 = R1 < carry ? 1 : 0;

            R2 += carry3 + carry2;
        }

        (*bi_Z)->a[cnt_k] = R0;
        R0 = R1;
        R1 = R2;
        R2 = 0x00;
    }
    (*bi_Z)->a[2 * WORD_LEN - 1] = R0;
    Reduction_256(bi_Z, Prime);
}

void Square_SPLIT_Multiplication(bigint_st_ptr *bi_X, bigint_st_ptr *bi_Z, bigint_st_ptr *Prime)
{
    int cnt_i, cnt_j, cnt_k; //for loop counting variable
    unsigned int R0 = 0x00;
    unsigned int R1 = 0x00;
    unsigned int R2 = 0x00;
    unsigned int UV2 = 0x00;
    unsigned int UV[2] = {0x00};
    int carry = 0;
    int carry2 = 0;
    int carry3 = 0;

    for (cnt_k = 0; cnt_k < 2 * WORD_LEN - 1; cnt_k++)
    {
        for (cnt_i = cnt_k >> 1, cnt_j = cnt_i + (cnt_k & 1); (cnt_i >= 0) && (cnt_j < WORD_LEN); cnt_j++, cnt_i--)
        {
            SPlit_Word_Mul(&(*bi_X)->a[cnt_i], &(*bi_X)->a[cnt_j], UV);

            if (cnt_i < cnt_j)
            {
                UV2 = UV[1] >> 28;
                carry = UV2 * 2 > 0xf ? 1 : 0;
                UV2 = UV[0] >> 28;
                carry2 = UV2 * 2 > 0xf ? 1 : 0;
                UV[0] = UV[0] << 1;
                UV[1] = UV[1] << 1;
                UV[1] += carry2;

                R2 += carry;
            }

            R0 = R0 + UV[0];
            carry = R0 < UV[0] ? 1 : 0;

            R1 += UV[1];
            carry2 = R1 < UV[1] ? 1 : 0;
            R1 += carry;
            carry3 = R1 < carry ? 1 : 0;

            R2 += carry3 + carry2;
        }
        (*bi_Z)->a[cnt_k] = R0;
        R0 = R1;
        R1 = R2;
        R2 = 0x00;
    }
    (*bi_Z)->a[2 * WORD_LEN - 1] = R0;
    Reduction_256(bi_Z, Prime);
}

void Reduction_256(bigint_st_ptr *bi_X, bigint_st_ptr *Prime)
{
    int cnt_i = 0;
    bigint_st_ptr Temp1 = NULL;
    bigint_st_ptr Temp2 = NULL;

    bigint_st_ptr S1 = NULL;
    bigint_st_ptr S2 = NULL;
    bigint_st_ptr S3 = NULL;
    bigint_st_ptr S4 = NULL;
    bigint_st_ptr S5 = NULL;
    bigint_st_ptr S6 = NULL;
    bigint_st_ptr S7 = NULL;
    bigint_st_ptr S8 = NULL;
    bigint_st_ptr S9 = NULL;

    word Input1[8] = {(*bi_X)->a[7], (*bi_X)->a[6], (*bi_X)->a[5], (*bi_X)->a[4], (*bi_X)->a[3], (*bi_X)->a[2], (*bi_X)->a[1], (*bi_X)->a[0]};
    word Input2[8] = {(*bi_X)->a[15], (*bi_X)->a[14], (*bi_X)->a[13], (*bi_X)->a[12], (*bi_X)->a[11], 0x00, 0x00, 0x00};
    word Input3[8] = {0x00, (*bi_X)->a[15], (*bi_X)->a[14], (*bi_X)->a[13], (*bi_X)->a[12], 0x00, 0x00, 0x00};
    word Input4[8] = {(*bi_X)->a[15], (*bi_X)->a[14], 0x00, 0x00, 0x00, (*bi_X)->a[10], (*bi_X)->a[9], (*bi_X)->a[8]};
    word Input5[8] = {(*bi_X)->a[8], (*bi_X)->a[13], (*bi_X)->a[15], (*bi_X)->a[14], (*bi_X)->a[13], (*bi_X)->a[11], (*bi_X)->a[10], (*bi_X)->a[9]};
    word Input6[8] = {(*bi_X)->a[10], (*bi_X)->a[8], 0x00, 0x00, 0x00, (*bi_X)->a[13], (*bi_X)->a[12], (*bi_X)->a[11]};
    word Input7[8] = {(*bi_X)->a[11], (*bi_X)->a[9], 0x00, 0x00, (*bi_X)->a[15], (*bi_X)->a[14], (*bi_X)->a[13], (*bi_X)->a[12]};
    word Input8[8] = {(*bi_X)->a[12], 0x00, (*bi_X)->a[10], (*bi_X)->a[9], (*bi_X)->a[8], (*bi_X)->a[15], (*bi_X)->a[14], (*bi_X)->a[13]};
    word Input9[9] = {(*bi_X)->a[13], 0x00, (*bi_X)->a[11], (*bi_X)->a[10], (*bi_X)->a[9], 0x00, (*bi_X)->a[15], (*bi_X)->a[14]};

    set_bigint(&S1, Input1);
    set_bigint(&S2, Input2);
    set_bigint(&S3, Input3);
    set_bigint(&S4, Input4);
    set_bigint(&S5, Input5);
    set_bigint(&S6, Input6);
    set_bigint(&S7, Input7);
    set_bigint(&S8, Input8);
    set_bigint(&S9, Input9);

    set_bigint_reset(&Temp1);
    Addition(&S1, &S2, &Temp1, Prime);

    set_bigint_reset(&Temp2);
    Subtraction(&Temp1, &S6, &Temp2, Prime);

    reset_bigint(&Temp1);
    Addition(&Temp2, &S2, &Temp1, Prime);

    reset_bigint(&Temp2);
    Subtraction(&Temp1, &S7, &Temp2, Prime);

    reset_bigint(&Temp1);
    Addition(&Temp2, &S3, &Temp1, Prime);

    reset_bigint(&Temp2);
    Subtraction(&Temp1, &S8, &Temp2, Prime);

    reset_bigint(&Temp1);
    Addition(&Temp2, &S3, &Temp1, Prime);

    reset_bigint(&Temp2);
    Subtraction(&Temp1, &S9, &Temp2, Prime);

    reset_bigint(&Temp1);
    Addition(&Temp2, &S4, &Temp1, Prime);

    reset_bigint(&Temp2);
    Addition(&Temp1, &S5, &Temp2, Prime);

    (*bi_X)->a = (word *)realloc((*bi_X)->a, WORD_LEN * sizeof(word));
    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        (*bi_X)->a[cnt_i] = Temp2->a[cnt_i];
    }

    // delete_bigint(&S1);
    // delete_bigint(&S2);
    // delete_bigint(&S3);
    // delete_bigint(&S4);
    // delete_bigint(&S5);
    // delete_bigint(&S6);
    // delete_bigint(&S7);
    // delete_bigint(&S8);
    // delete_bigint(&S9);
}

void Inverse_FLT(bigint_st_ptr *bi_X, bigint_st_ptr *bi_Z, bigint_st_ptr *Prime) //? Inv 함수
{
    int cnt_i = 0;
    bigint_st_ptr z3 = NULL;
    bigint_st_ptr z15 = NULL;
    bigint_st_ptr temp1 = NULL;
    bigint_st_ptr temp2 = NULL;
    bigint_st_ptr temp3 = NULL;

    set_bigint_reset(&z3);
    set_bigint_reset(&z15);

    set_bigint_reset_mul(&temp1); //!
    OS_64_Multiplication(bi_X, bi_X, &temp1, Prime);
    set_bigint_reset_mul(&z3); //!
    OS_64_Multiplication(&temp1, bi_X, &z3, Prime);

    set_bigint_reset_mul(&temp1); //!
    OS_64_Multiplication(&z3, &z3, &temp1, Prime);
    set_bigint_reset_mul(&temp2); //!
    OS_64_Multiplication(&temp1, &temp1, &temp2, Prime);
    set_bigint_reset_mul(&z15); //!
    OS_64_Multiplication(&temp2, &z3, &z15, Prime);

    bigint_st_ptr t0 = NULL;
    set_bigint_reset_mul(&t0);
    set_bigint_reset_mul(&temp1); //!
    OS_64_Multiplication(&z15, &z15, &temp1, Prime);
    set_bigint_reset_mul(&temp2); //!
    OS_64_Multiplication(&temp1, &temp1, &temp2, Prime);
    OS_64_Multiplication(&temp2, &z3, &t0, Prime);

    bigint_st_ptr t1 = NULL;
    set_bigint_reset_mul(&t1);
    set_bigint_reset_mul(&temp1); //!
    OS_64_Multiplication(&t0, &t0, &temp1, Prime);
    for (cnt_i = 0; cnt_i < 2; cnt_i++) //* (n -2) /2
    {
        set_bigint_reset_mul(&temp2); //!
        OS_64_Multiplication(&temp1, &temp1, &temp2, Prime);
        set_bigint_reset_mul(&temp1); //!
        OS_64_Multiplication(&temp2, &temp2, &temp1, Prime);
    }
    set_bigint_reset_mul(&temp3); //!
    OS_64_Multiplication(&temp1, &temp1, &temp3, Prime);
    OS_64_Multiplication(&temp3, &t0, &t1, Prime);

    bigint_st_ptr t2 = NULL;
    set_bigint_reset_mul(&t2);
    set_bigint_reset_mul(&temp1); //!
    OS_64_Multiplication(&t1, &t1, &temp1, Prime);
    for (cnt_i = 0; cnt_i < 5; cnt_i++) //* (n -2) /2
    {
        set_bigint_reset_mul(&temp2); //!
        OS_64_Multiplication(&temp1, &temp1, &temp2, Prime);
        set_bigint_reset_mul(&temp1); //!
        OS_64_Multiplication(&temp2, &temp2, &temp1, Prime);
    }
    set_bigint_reset_mul(&temp3); //!
    OS_64_Multiplication(&temp1, &temp1, &temp3, Prime);
    set_bigint_reset_mul(&temp2); //!
    OS_64_Multiplication(&temp3, &t1, &t2, Prime);

    set_bigint_reset_mul(&temp1); //!
    OS_64_Multiplication(&t2, &t2, &temp1, Prime);
    for (cnt_i = 0; cnt_i < 2; cnt_i++) //* (n -2) /2
    {
        set_bigint_reset_mul(&temp2); //!
        OS_64_Multiplication(&temp1, &temp1, &temp2, Prime);
        set_bigint_reset_mul(&temp1); //!
        OS_64_Multiplication(&temp2, &temp2, &temp1, Prime);
    }
    set_bigint_reset_mul(&temp3); //!
    OS_64_Multiplication(&temp1, &temp1, &temp3, Prime);
    set_bigint_reset_mul(&t2); //!
    OS_64_Multiplication(&temp3, &t0, &t2, Prime);

    bigint_st_ptr t3 = NULL;
    set_bigint_reset_mul(&t3);
    set_bigint_reset_mul(&temp1); //!
    OS_64_Multiplication(&t2, &t2, &temp1, Prime);
    set_bigint_reset_mul(&temp2); //!
    OS_64_Multiplication(&temp1, &temp1, &temp2, Prime);
    OS_64_Multiplication(&temp2, &z3, &t3, Prime);

    bigint_st_ptr t4 = NULL;
    set_bigint_reset_mul(&t4);
    set_bigint_reset_mul(&temp1); //!
    OS_64_Multiplication(&t3, &t3, &temp1, Prime);
    for (cnt_i = 0; cnt_i < 15; cnt_i++) //* (n -2) /2
    {
        set_bigint_reset_mul(&temp2); //!
        OS_64_Multiplication(&temp1, &temp1, &temp2, Prime);
        set_bigint_reset_mul(&temp1); //!
        OS_64_Multiplication(&temp2, &temp2, &temp1, Prime);
    }
    set_bigint_reset_mul(&temp3); //!
    OS_64_Multiplication(&temp1, &temp1, &temp3, Prime);
    OS_64_Multiplication(&temp3, bi_X, &t4, Prime);

    set_bigint_reset_mul(&temp1); //!
    OS_64_Multiplication(&t4, &t4, &temp1, Prime);
    for (cnt_i = 0; cnt_i < 47; cnt_i++) //* (n -2) /2
    {
        set_bigint_reset_mul(&temp2); //!
        OS_64_Multiplication(&temp1, &temp1, &temp2, Prime);
        set_bigint_reset_mul(&temp1); //!
        OS_64_Multiplication(&temp2, &temp2, &temp1, Prime);
    }
    set_bigint_reset_mul(&t4); //!
    OS_64_Multiplication(&temp1, &temp1, &t4, Prime);

    bigint_st_ptr t5 = NULL;
    set_bigint_reset_mul(&t5);
    set_bigint_reset_mul(&temp1); //!
    OS_64_Multiplication(&t4, &t4, &temp1, Prime);
    for (cnt_i = 0; cnt_i < 15; cnt_i++) //* (n -2) /2
    {
        set_bigint_reset_mul(&temp2); //!
        OS_64_Multiplication(&temp1, &temp1, &temp2, Prime);
        set_bigint_reset_mul(&temp1); //!
        OS_64_Multiplication(&temp2, &temp2, &temp1, Prime);
    }
    set_bigint_reset_mul(&temp3); //!
    OS_64_Multiplication(&temp1, &temp1, &temp3, Prime);
    OS_64_Multiplication(&temp3, &t3, &t5, Prime);

    set_bigint_reset_mul(&temp1); //!
    OS_64_Multiplication(&t5, &t5, &temp1, Prime);
    for (cnt_i = 0; cnt_i < 15; cnt_i++) //* (n -2) /2
    {
        set_bigint_reset_mul(&temp2); //!
        OS_64_Multiplication(&temp1, &temp1, &temp2, Prime);
        set_bigint_reset_mul(&temp1); //!
        OS_64_Multiplication(&temp2, &temp2, &temp1, Prime);
    }
    set_bigint_reset_mul(&temp3); //!
    OS_64_Multiplication(&temp1, &temp1, &temp3, Prime);
    set_bigint_reset_mul(&t5); //!
    OS_64_Multiplication(&temp3, &t3, &t5, Prime);

    //todo t
    set_bigint_reset_mul(&temp1); //!
    OS_64_Multiplication(&t5, &t5, &temp1, Prime);
    for (cnt_i = 0; cnt_i < 14; cnt_i++) //* (n -2) /2
    {
        set_bigint_reset_mul(&temp2); //!
        OS_64_Multiplication(&temp1, &temp1, &temp2, Prime);
        set_bigint_reset_mul(&temp1); //!
        OS_64_Multiplication(&temp2, &temp2, &temp1, Prime);
    }
    set_bigint_reset_mul(&temp3); //!
    OS_64_Multiplication(&temp1, &temp1, &temp3, Prime);
    set_bigint_reset_mul(bi_Z); //!
    OS_64_Multiplication(&temp3, &t2, bi_Z, Prime);

    set_bigint_reset_mul(&temp1); //!
    OS_64_Multiplication(bi_Z, bi_Z, &temp1, Prime);
    set_bigint_reset_mul(&temp2); //!
    OS_64_Multiplication(&temp1, &temp1, &temp2, Prime);
    set_bigint_reset_mul(bi_Z); //!
    OS_64_Multiplication(&temp2, bi_X, bi_Z, Prime);

    //!hh
}

void show_EN(Ecc_st_ptr *EN_X)
{
    int cnt_i = 0;
    printf("\nX = ");
    for (cnt_i = WORD_LEN - 1; cnt_i >= 0 ; cnt_i--)
    {
        printf("%08X", (*EN_X)->x->a[cnt_i]);
    }
    printf("\nY = ");
    for (cnt_i = WORD_LEN - 1; cnt_i >= 0 ; cnt_i--)
    {
        printf("%08X", (*EN_X)->y->a[cnt_i]);
    }
    printf("\n");
    // for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    // {
    //     printf("%08X", (*EN_X)->x->a[cnt_i]);
    // }
    // printf("\nY = ");
    // for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    // {
    //     printf("%08X", (*EN_X)->y->a[cnt_i]);
    // }
    // printf("\n");
}

void set_EN(Ecc_st_ptr *EN_P, word *input_X, word *input_Y)
{
    int cnt_i = 0;
    (*EN_P) = (Ecc_pt *)calloc(1, sizeof(Ecc_pt));
    (*EN_P)->x = (bigint_st *)calloc(1, sizeof(bigint_st));
    (*EN_P)->y = (bigint_st *)calloc(1, sizeof(bigint_st));
    (*EN_P)->x->a = (word *)calloc(WORD_LEN, sizeof(word));
    (*EN_P)->y->a = (word *)calloc(WORD_LEN, sizeof(word));

    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        (*EN_P)->x->a[cnt_i] = input_X[WORD_LEN - cnt_i - 1];
        (*EN_P)->y->a[cnt_i] = input_Y[WORD_LEN - cnt_i - 1];
    }
}
void EN_copy(Ecc_st_ptr *EN_dst,Ecc_st_ptr *EN_src)
{
    int cnt_i = 0;
    for(cnt_i = 0 ; cnt_i <WORD_LEN ; cnt_i ++)
    {
        (*EN_dst)->x->a[cnt_i] = (*EN_src)->x->a[cnt_i];
        (*EN_dst)->y->a[cnt_i] = (*EN_src)->y->a[cnt_i];
    }

}
void delete_EN(Ecc_st_ptr *EN_P)
{
    int cnt_i;
    if (*EN_P == NULL)
        return;
    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        (*EN_P)->x->a[cnt_i] = 0x0; //기존  구조체 메모리가 점유하는 부분의 값을 초기화
    }
    free((*EN_P)->x->a); //Big_int 구조체 멤버변수 a에 할당된 메모리 free
    free((*EN_P)->y->a); //Big_int 구조체 멤버변수 a에 할당된 메모리 free
    free((*EN_P)->x);    //Big_int 구조체 멤버변수 a에 할당된 메모리 free
    free((*EN_P)->y);    //Big_int 구조체 멤버변수 a에 할당된 메모리 free
    free(*EN_P);          //Big_ing 구조체 포인터에 할당된 메모리 free
    *EN_P = NULL;
}
void set_EN_reset(Ecc_st_ptr *EN_P)
{
    if (EN_P != NULL)
        delete_EN(EN_P);

    int cnt_i = 0;
    (*EN_P) = (Ecc_pt *)calloc(1, sizeof(Ecc_pt));
    (*EN_P)->x = (bigint_st *)calloc(1, sizeof(bigint_st));
    (*EN_P)->y = (bigint_st *)calloc(1, sizeof(bigint_st));
    (*EN_P)->x->a = (word *)calloc(WORD_LEN, sizeof(word));
    (*EN_P)->y->a = (word *)calloc(WORD_LEN, sizeof(word));

    for (cnt_i = 0; cnt_i < WORD_LEN; cnt_i++)
    {
        (*EN_P)->x->a[cnt_i] = 0x00;
        (*EN_P)->y->a[cnt_i] = 0x00;
    }
}
void ECADD(Ecc_st_ptr *EN_P, Ecc_st_ptr *EN_Q, Ecc_st_ptr *EN_R, bigint_st_ptr *Prime)
{
    bigint_st_ptr temp1 = NULL;
    bigint_st_ptr temp2 = NULL;
    bigint_st_ptr temp3 = NULL;
    bigint_st_ptr X = NULL;
    bigint_st_ptr Y = NULL;

    set_bigint_reset(&temp1);
    set_bigint_reset(&temp2);
    set_bigint_reset(&temp3);
    set_bigint_reset(&X);
    set_bigint_reset(&Y);

    Subtraction(&((*EN_Q)->y), &((*EN_P)->y), &temp1, Prime);
    Subtraction(&((*EN_Q)->x), &((*EN_P)->x), &temp2, Prime);
    Inverse_FLT(&temp2, &temp3, Prime);
    set_bigint_reset_mul(&temp2);
    OS_64_Multiplication(&temp1, &temp3, &temp2, Prime);

    set_bigint_reset_mul(&temp1);
    OS_64_Multiplication(&temp2, &temp2, &temp1, Prime);

    Subtraction(&temp1, &((*EN_P)->x), &temp2, Prime);

    Subtraction(&temp2, &((*EN_Q)->x), &X, Prime);

    //!
    Subtraction(&((*EN_Q)->y), &((*EN_P)->y), &temp1, Prime);
    Subtraction(&((*EN_Q)->x), &((*EN_P)->x), &temp2, Prime);
    Inverse_FLT(&temp2, &temp3, Prime);
    set_bigint_reset_mul(&temp2);
    OS_64_Multiplication(&temp1, &temp3, &temp2, Prime);
    Subtraction(&((*EN_P)->x), &X, &temp1, Prime);

    set_bigint_reset_mul(&temp3);
    OS_64_Multiplication(&temp2, &temp1, &temp3, Prime);

    Subtraction(&temp3, &((*EN_P)->y), &Y, Prime);

    set_EN(EN_R, X->a, Y->a);

    delete_bigint(&temp1);
    delete_bigint(&temp2);
    delete_bigint(&temp3);
    delete_bigint(&X);
    delete_bigint(&Y);
}

void ECDBL(Ecc_st_ptr *EN_P, Ecc_st_ptr *EN_R, bigint_st_ptr *Prime, bigint_st_ptr *a)
{
    bigint_st_ptr temp1 = NULL;
    bigint_st_ptr temp2 = NULL;
    bigint_st_ptr temp3 = NULL;
    bigint_st_ptr X = NULL;
    bigint_st_ptr Y = NULL;

    set_bigint_reset(&temp1);
    set_bigint_reset(&temp2);
    set_bigint_reset(&temp3);
    set_bigint_reset(&X);
    set_bigint_reset(&Y);

    //! X
    set_bigint_reset_mul(&temp1);
    OS_64_Multiplication(&((*EN_P)->x), &((*EN_P)->x), &temp1, Prime);
    Addition(&temp1, &temp1, &temp2, Prime);
    Addition(&temp2, &temp1, &temp3, Prime);
    Addition(&temp3, a, &temp1, Prime);

    Addition(&((*EN_P)->y), &((*EN_P)->y), &temp2, Prime);
    Inverse_FLT(&temp2, &temp3, Prime);
    set_bigint_reset_mul(&temp2);
    OS_64_Multiplication(&temp1, &temp3, &temp2, Prime);
    set_bigint_reset_mul(&temp1);
    OS_64_Multiplication(&temp2, &temp2, &temp1, Prime);

    Subtraction(&temp1, &((*EN_P)->x), &temp2, Prime);
    Subtraction(&temp2, &((*EN_P)->x), &((*EN_R)->x), Prime);

    //! Y
    set_bigint_reset_mul(&temp1);
    OS_64_Multiplication(&((*EN_P)->x), &((*EN_P)->x), &temp1, Prime);
    Addition(&temp1, &temp1, &temp2, Prime);
    Addition(&temp2, &temp1, &temp3, Prime);
    Addition(&temp3, a, &temp1, Prime);

    Addition(&((*EN_P)->y), &((*EN_P)->y), &temp2, Prime);
    Inverse_FLT(&temp2, &temp3, Prime);
    set_bigint_reset_mul(&temp2);
    OS_64_Multiplication(&temp1, &temp3, &temp2, Prime);

    Subtraction(&((*EN_P)->x), &((*EN_R)->x), &temp1, Prime);
    set_bigint_reset_mul(&temp3);
    OS_64_Multiplication(&temp1, &temp2, &temp3, Prime);
    Subtraction(&temp3, &((*EN_P)->y), &((*EN_R)->y), Prime);

    delete_bigint(&temp1);
    delete_bigint(&temp2);
    delete_bigint(&temp3);
    delete_bigint(&X);
    delete_bigint(&Y);
}

void ECC_MUL_LtoR(Ecc_st_ptr *EN_P, bigint_st_ptr *K,Ecc_st_ptr *EN_R, bigint_st_ptr *Prime, bigint_st_ptr *a)
{
    int cnt_i = 0, cnt_j = 0;
    int DC;
    Ecc_st_ptr temp1 = NULL;
    Ecc_st_ptr temp2 = NULL;
    Ecc_st_ptr temp3 = NULL;

    set_EN_reset(&temp1);
    set_EN_reset(&temp2);
    set_EN_reset(&temp3);
    
    EN_copy(&temp1,EN_P);

    for (cnt_i = WORD_LEN - 1; cnt_i >= 0; cnt_i--)
    {
        for (cnt_j = 31; cnt_j >= 0; cnt_j--)
        {
            ECDBL(&temp1, &temp2, Prime, a);
            DC = (((*K)->a[cnt_i]) >> cnt_j) & 0x00000001;
            if (DC == 1)
            {
                ECADD(&temp2, EN_P, &temp3, Prime);
                EN_copy(&temp1,&temp3);
                continue;
            }
            EN_copy(&temp1,&temp2);
        }
    }
    EN_copy(EN_R,&temp1);
    delete_EN(&temp1);
    delete_EN(&temp2);
    delete_EN(&temp3);

}
//* 암호최적화 및 응용연구실 20175204 김영범
#ifndef __PLUS__
#define __PLUS__

#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define NEGATIVE 1
#define NON_NEGATIVE 0
#define TRUE 1
#define FALSE 0
#define FST_IS_BIG 1 //!first is big for Compare function
#define BOTH_ARE_SAME 0//! both are same for Compare function
#define SCD_IS_BIG -1//! Second is big for Compare function
#define ERROR -0xeeeee //! Error

#define WORD_BITLEN 32

#if WORD_BITLEN == 32
typedef unsigned int word; 
#define WORD_LEN 8
#else
typedef unsigned long long word;
#define WORDLEN 9
#endif


typedef struct{ //! Big number struct
    word *a;
    int e;

} bigint_st;

typedef bigint_st* bigint_st_ptr; //! typedef for bigint struct pointerduction

typedef struct { //! ECC Number struct
    bigint_st_ptr x;
    bigint_st_ptr y;
}Ecc_pt; 
typedef Ecc_pt* Ecc_st_ptr; //! typedef for bigint struct pointerduction


//! Test speed
long long cpucycles();

//! basic function
void delete_bigint(bigint_st_ptr *bi_X);//? Big_int 구조체 포인터 값 NULL로 만들어주는 해주는 함수
void set_bigint(bigint_st_ptr *bi_X, word *Input); //? Big_int 구조체에 값 설정해주는 함수
void set_bigint_reset(bigint_st_ptr *bi_X); //? Big_int 구조체  할당 및 0으로 초기화 해주는 함수
void set_bigint_reset_mul(bigint_st_ptr *bi_X); //? Big_int 구조체  할당 및 0으로 초기화 해주는 함수
void reset_bigint(bigint_st_ptr *bi_X); //?Big_int 구조체 0으로 초기화 해주는 함수
void show(bigint_st_ptr *bi_X); //? 숫자를 보여주는 함수
void integer_copy(bigint_st_ptr *bi_dst,bigint_st_ptr *bi_src);//? Big_integer 함수 복사해주는 함수
unsigned int get_rand_32bits();//? 랜덤 32bit 값 생성
unsigned long long get_rand_64bits();//?랜덤 64bit 값 생성
word get_random_word();//? 위 두함수를 이용해서 랜덤한 word값 생성
void get_random_array(word dst[]);//? 마찬가지로 배열 생성함수
void generate_random_integer(bigint_st_ptr *bi_X,bigint_st_ptr *Prime);//? 랜덤한 integer 생성함수 (연산 확인용 만드는 함수)
void swap_integer(bigint_st_ptr *bi_X,bigint_st_ptr *bi_Y);//? Swap 함수
void Reduction(bigint_st_ptr *bi_X,bigint_st_ptr *Prime); //? 감산 함수.
int Compare(bigint_st_ptr *bi_X, bigint_st_ptr *bi_Y);//? 비교함수
int Compare_mul(bigint_st_ptr *bi_X, bigint_st_ptr *bi_Y);//? 비교함수
void swap(word *A, word *B);

//! Addition
void Addition(bigint_st_ptr *bi_X,bigint_st_ptr *bi_Y,bigint_st_ptr *bi_Z,bigint_st_ptr *Prime);

//! Subtraction
int sub_Core_borrow(word a, word b);//? Borrow 계산 함수
void Subtraction(bigint_st_ptr *bi_X,bigint_st_ptr *bi_Y,bigint_st_ptr *bi_Z,bigint_st_ptr *Prime);

//! Multiplication
void OS_64_Multiplication(bigint_st_ptr *bi_X,bigint_st_ptr *bi_Y,bigint_st_ptr *bi_Z,bigint_st_ptr *Prime);//*
void OS_SPLIT_Multiplication(bigint_st_ptr *bi_X,bigint_st_ptr *bi_Y,bigint_st_ptr *bi_Z,bigint_st_ptr *Prime);
void SPlit_Word_Mul(word* A, word* B, word* C);
void PS_64_Multiplication(bigint_st_ptr *bi_X,bigint_st_ptr *bi_Y,bigint_st_ptr *bi_Z,bigint_st_ptr *Prime);
void PS_SPLIT_Multiplication(bigint_st_ptr *bi_X,bigint_st_ptr *bi_Y,bigint_st_ptr *bi_Z,bigint_st_ptr *Prime);

//! Squaring
void Square_64_Multiplication(bigint_st_ptr *bi_X,bigint_st_ptr *bi_Z,bigint_st_ptr *Prime);
void Square_SPLIT_Multiplication(bigint_st_ptr *bi_X,bigint_st_ptr *bi_Z,bigint_st_ptr *Prime);

//! Reduction_256
void Reduction_256(bigint_st_ptr *bi_X,bigint_st_ptr *Prime); //? 감산 함수.

//! INV
void Inverse_FLT(bigint_st_ptr *bi_X , bigint_st_ptr * bi_Z, bigint_st_ptr *Prime);//? Inv 함수

//! ECC coordinate
void set_EN(Ecc_st_ptr *EN_P, word *input_X, word *input_Y);
void set_EN_reset(Ecc_st_ptr *EN_P);
void EN_copy(Ecc_st_ptr *EN_dst,Ecc_st_ptr *EN_src);
void delete_EN(Ecc_st_ptr *EN_P);
void show_EN(Ecc_st_ptr *EN_P);
void ECADD(Ecc_st_ptr *EN_P, Ecc_st_ptr *EN_Q, Ecc_st_ptr *EN_R, bigint_st_ptr *Prime);
void ECDBL(Ecc_st_ptr *EN_P,Ecc_st_ptr *EN_R, bigint_st_ptr *Prime, bigint_st_ptr *a);
void ECC_MUL_LtoR(Ecc_st_ptr *EN_P, bigint_st_ptr *K ,Ecc_st_ptr *EN_R, bigint_st_ptr *Prime, bigint_st_ptr *a);


#endif
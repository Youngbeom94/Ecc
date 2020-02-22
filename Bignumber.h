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
    word a[WORD_LEN];
    int e;
} bigint_st;

typedef struct { //! ECC Number struct
    bigint_st x;
    bigint_st y;
    char isinfinity;
} Ecc_pt; 

typedef struct{
    bigint_st x;
    bigint_st y;
    bigint_st z;
    char isinfinity;
} Ecc_Jpt;


//! Test speed
long long cpucycles();

//! basic function
void set_bigint(bigint_st *bi_X, word *Input); //? Big_int 구조체에 값 설정해주는 함수
void set_bigint_array(bigint_st *bi_X, word *Input); //? Big_int 구조체에 값 설정해주는 함수
void copy_bigint(bigint_st *dst, bigint_st *src); //? Big_int 구조체에 값 설정해주는 함수
void reset_bigint(bigint_st *bi_X); //?Big_int 구조체 0으로 초기화 해주는 함수
int Compare(bigint_st *bi_X, bigint_st *bi_Y);//? 비교함수
void Reduction(bigint_st *bi_X,bigint_st *Prime); //? 감산 함수.
void show(bigint_st *bi_X);
void setone(bigint_st *bi_X);
int isEven(bigint_st *bi_X);
int isOne(bigint_st *bi_X);
int iszero(bigint_st *bi_X);
void RightShift(bigint_st *bi_X); 

//! Addition
void Addition(bigint_st *bi_X,bigint_st *bi_Y, bigint_st *bi_Z, bigint_st *Prime);
void Addition_NR(bigint_st *bi_X,bigint_st *bi_Y, bigint_st *bi_Z, bigint_st *Prime);

//! Subtraction
int sub_Core_borrow(word a, word b);//? Borrow 계산 함수
void Subtraction(bigint_st *bi_X,bigint_st *bi_Y,bigint_st *bi_Z,bigint_st *Prime);

// //! Multiplication
void OS64MUL_256(bigint_st *bi_X,bigint_st *bi_Y,bigint_st *bi_Z,bigint_st *Prime);//*
void Reduction_256(word *bi_X,bigint_st *bi_Z,bigint_st *Prime); //? 감산 함수.

// //! INV
void Inverse_FLT(bigint_st *bi_X , bigint_st * bi_Z, bigint_st *Prime);//? Inv 함수
void Inverse_EEA(bigint_st *bi_X , bigint_st * bi_Z, bigint_st *Prime);//? Inv 함수

// //! ECC coordinate
void set_EN(Ecc_pt*EN_P, word *input_X, word *input_Y);
void set_EN_J(Ecc_Jpt*EN_P, word *input_X, word *input_Y,word *input_Z);
void set_EN_J_reverse(Ecc_Jpt*EN_P, word *input_X, word *input_Y,word *input_Z);
void set_EN_J_reverse_nonZ(Ecc_Jpt*EN_P, word *input_X, word *input_Y,word *input_Z);
void set_EN_reset(Ecc_pt *EN_P);
void set_EN_J_reset(Ecc_Jpt *EN_P);
void Trns_A_to_J(Ecc_Jpt *dst, Ecc_pt *src,bigint_st *Prime);
void Trns_J_to_A(Ecc_pt *dst, Ecc_Jpt *src,bigint_st *Prime);
void show_EN(Ecc_pt *EN_P);
void show_EN_J(Ecc_Jpt *EN_X);
void EN_copy(Ecc_pt *EN_dst,Ecc_pt *EN_src);
void EN_J_copy(Ecc_Jpt *EN_dst,Ecc_Jpt *EN_src);
void ECADD(Ecc_pt *EN_P, Ecc_pt *EN_Q, Ecc_pt *EN_R, bigint_st *Prime);
void ECDBL(Ecc_pt *EN_P,Ecc_pt *EN_R, bigint_st *Prime, bigint_st *a);
void ECDBL_J(Ecc_Jpt *EN_P,Ecc_Jpt *EN_R, bigint_st *Prime);
void ECADD_J(Ecc_Jpt *EN_P,Ecc_pt *EN_Q,Ecc_Jpt *EN_R, bigint_st *Prime);
void ECLtoR(Ecc_pt *EN_P, bigint_st *K ,Ecc_pt *EN_R, bigint_st *Prime, bigint_st *a);
void ECLtoR_J(Ecc_Jpt *EN_P, bigint_st *K ,Ecc_Jpt *EN_R, bigint_st *Prime);
void ECRtoL(Ecc_pt *EN_P, bigint_st *K ,Ecc_pt *EN_R, bigint_st *Prime, bigint_st *a);


#endif
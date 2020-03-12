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

#define W 4 //! w = 4 need for wNAF method 
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
    int e;//? flag carry or borrow
} bigint_st;

typedef struct { //! ECC Number struct
    bigint_st x;
    bigint_st y;
    char isinfinity; //? flag for infinity point
} Ecc_pt; //! Ecc point by Affin coordinate

typedef struct{
    bigint_st x;
    bigint_st y;
    bigint_st z;
    char isinfinity;//? flag for infinity point
} Ecc_Jpt; //! Ecc point by Jacobian coordinate


//! Test speed
long long cpucycles();//? 성능측정용 함수 (cpu cycle calculate)

//! basic function
void set_bigint(bigint_st *bi_X, word *Input); //? Set Biginteger Using Array.
void copy_bigint(bigint_st *dst, bigint_st *src); //? Copy biginteger.
void reset_bigint(bigint_st *bi_X); //? Reset Biginteger .
int Compare(bigint_st *bi_X, bigint_st *bi_Y);//? Compare Two Biginteger.
void Reduction(bigint_st *bi_X,bigint_st *Prime); //? Reduction on P256 field.
void show(bigint_st *bi_X); //? Show Biginteger on Affin coordinate.
void setone(bigint_st *bi_X);//? Make Biginteger One
int isEven(bigint_st *bi_X);//? is Biginteger Even?
int isOne(bigint_st *bi_X);//? is Biginteger One?
int iszero(bigint_st *bi_X);//? is Biginteger Zero?
void RightShift(bigint_st *bi_X);//? == biginteger >> 1

//! Addition
void Addition(bigint_st *bi_X,bigint_st *bi_Y, bigint_st *bi_Z, bigint_st *Prime);//? Addition Two Biginteger and Reduction256
void Addition_NR(bigint_st *bi_X,bigint_st *bi_Y, bigint_st *bi_Z, bigint_st *Prime);//? Addition Two Biginteger and  Not Reduction256

//! Subtraction
int sub_Core_borrow(word a, word b);//? set borrow
void Subtraction(bigint_st *bi_X,bigint_st *bi_Y,bigint_st *bi_Z,bigint_st *Prime);//? Subtraction Two Biginteger and Reduction256

// //! Multiplication
void OS64MUL_256(bigint_st *bi_X,bigint_st *bi_Y,bigint_st *bi_Z,bigint_st *Prime);//? OS64 version of Multiplication of Two Biginteger
void PS_Split_MUL_256(bigint_st *bi_X,bigint_st *bi_Y,bigint_st *bi_Z,bigint_st *Prime);//? PS_Split version of Multiplication of Two Biginteger
void SPlit_Word_Mul(word* A, word* B, word* C);
void Reduction_256(word *bi_X,bigint_st *bi_Z,bigint_st *Prime); //? Reduction_256 for Biginteger which have 2 * WORD_LEN wordlen

// //! INV
void Inverse_FLT(bigint_st *bi_X , bigint_st * bi_Z, bigint_st *Prime);//? find inverse of biginteger in Prime field ,using Fermet Little Thm 
void Inverse_EEA(bigint_st *bi_X , bigint_st * bi_Z, bigint_st *Prime);//? find inverse of biginteger in Prime field ,using Extended U - A

// //! ECC coordinate
void set_EN(Ecc_pt*EN_P, word *input_X, word *input_Y);//? set Ecc Point x,y,z on Affin Coordinate
void set_EN_J(Ecc_Jpt*EN_P, word *input_X, word *input_Y,word *input_Z);//? set Ecc Point x,y,z using Array on Jacobian Coordinate
void set_EN_J_reverse(Ecc_Jpt*EN_P, word *input_X, word *input_Y,word *input_Z);//? set Ecc Point x,y,z on Jacobian Coordinate, Not use memory reverse
void set_EN_J_reverse_nonZ(Ecc_Jpt*EN_P, word *input_X, word *input_Y,word *input_Z);//? set Ecc Point x,y,z on Jacobian Coordinate, Not use memory reverse of Z
void set_EN_reset(Ecc_pt *EN_P); //? reset array of Ecc Point on Affin coordinate
void set_EN_J_reset(Ecc_Jpt *EN_P);//? reset array of Ecc Point on Jacobian coordinate
void Trns_A_to_J(Ecc_Jpt *dst, Ecc_pt *src,bigint_st *Prime);//? transform Affin To Jacobian
void Trns_J_to_A(Ecc_pt *dst, Ecc_Jpt *src,bigint_st *Prime);//? transform Jacobian To Affin
void show_EN(Ecc_pt *EN_P);//? Show Ecc Point on Affin coordinate
void show_EN_J(Ecc_Jpt *EN_X);//? Show Ecc Point on Jacobian coordinate
void EN_copy(Ecc_pt *EN_dst,Ecc_pt *EN_src);//? Ecc point of Affin coordinate
void EN_J_copy(Ecc_Jpt *EN_dst,Ecc_Jpt *EN_src);//? Ecc point of Jacobian coordinate

// //! ECC ADD DBL two methods by Affin and Jacobian
void ECADD(Ecc_pt *EN_P, Ecc_pt *EN_Q, Ecc_pt *EN_R, bigint_st *Prime); //? ECADD two Ecc point
void ECDBL(Ecc_pt *EN_P,Ecc_pt *EN_R, bigint_st *Prime, bigint_st *a);//? ECDBl one Ecc point
void ECDBL_J(Ecc_Jpt *EN_P,Ecc_Jpt *EN_R, bigint_st *Prime);//? ECDBL one Ecc point on Jacobian coordinate
void ECADD_J(Ecc_Jpt *EN_P,Ecc_pt *EN_Q,Ecc_Jpt *EN_R, bigint_st *Prime);//? ECADD two Ecc point on Jacobian coordinate

// //! ECC Scalar Mul by Affin
void ECLtoR(Ecc_pt *EN_P, bigint_st *K ,Ecc_pt *EN_R, bigint_st *Prime, bigint_st *a);//? Scalar Multiplication Using L to R method; Pixed Point is Base Ecc Point on Affin
void ECRtoL(Ecc_pt *EN_P, bigint_st *K ,Ecc_pt *EN_R, bigint_st *Prime, bigint_st *a);//? Scalar Multiplication Using R to L method; Pixed Point is Base Ecc Point on Affin

// //! ECC Scalar Mul by Jacobian
void ECLtoR_J(Ecc_Jpt *EN_P, bigint_st *K ,Ecc_Jpt *EN_R, bigint_st *Prime);//? Scalar Multiplication Using L to R method; Pixed Point is Base Ecc Point on Jacobian coordinate

// //! ECC Windows method of Scalar Mul by Jacobian
void NAF_recoding(bigint_st *Scalar, char *NAF, bigint_st *Prime);//? Make char array for wNAF method using Scalar
void ECLtoR_J_wNAF(Ecc_Jpt *EN_P, char *NAF ,Ecc_Jpt *EN_R, bigint_st *Prime);//? Scalar Mul Using wNAF L to R method; Pixed Point is Base Ecc Point on Jacobian coordinate

// //! ECC comb_table method of Scalar Mul by Jacobian
void comb_Table(char Table[WORD_LEN][WORD_BITLEN],Ecc_pt *J_Table,Ecc_pt *EN_P, bigint_st *K ,bigint_st *Prime);//? Make comb Table for Comb method using Scalar
void ECLtoR_J_comb(Ecc_Jpt *EN_P,char Table[WORD_LEN][WORD_BITLEN], Ecc_pt *J_Table ,Ecc_Jpt *EN_R, bigint_st *Prime);//? Scalar Mul Using Comb L to R method; Pixed Point is Base Ecc Point on Jacobian coordinate

#endif
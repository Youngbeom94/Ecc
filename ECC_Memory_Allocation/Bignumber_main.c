#include "Bignumber.h"

#if 1
int main()
{
    //todo
    Ecc_st_ptr Based_Point = NULL;
    word bt_x[8] = {0x6b17d1f2,0xe12c4247,0xf8bce6e5,0x63a440f2,0x77037d81,0x2deb33a0,0xf4a13945,0xd898c296};
    word bt_y[8] = {0x4fe342e2,0xfe1a7f9b,0x8ee7eb4a,0x7c0f9e16,0x2bce3357,0x6b315ece,0xcbb64068,0x37bf51f5};
    set_EN(&Based_Point, bt_x,bt_y);

    bigint_st_ptr a = NULL;
    word inputdata_a[8] = {0xFFFFFFFF, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFC};
    bigint_st_ptr Prime = NULL;
    word Prime_array[8] = {0xFFFFFFFF, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
    set_bigint(&Prime, Prime_array);
    set_bigint(&a,inputdata_a);
    //todo
    
    unsigned long long  cycles1 = 0 ,cycles2 = 0;

    Ecc_st_ptr A = NULL;
    Ecc_st_ptr B = NULL;
    Ecc_st_ptr D = NULL;
    word px[8] = {0x10BEA102, 0x1DD0C663, 0x95F34467, 0xB169D121, 0xD85B11D5, 0xEDD66162, 0x51FCD78A, 0x5302133F};
    word py[8] = {0x99CEF2FE, 0xCB8A6A9F, 0x9E503EDD, 0xC368CA73, 0x67F9D69B, 0x5E58C04C, 0x4F248E0A, 0xC94454E7};
    word qx[8] = {0xBB472AF9, 0xDF7DE38A, 0x01A45F7A, 0x8A78922C, 0xDA42F065, 0x8EE82993, 0xC313E0BA, 0x2D1466C9};
    word qy[8] = {0xCFC3747F, 0xEFE8AEB8, 0xB27CE035, 0xEEB04F23, 0x4D012E53, 0x3FF41EA7, 0x380229E2, 0x5D1AC6DD};

    set_EN(&A , px ,py);
    set_EN(&B , qx ,qy);
    set_EN_reset(&D);

    printf(" P \n");
    show_EN(&A);
    printf(" Q \n");
    show_EN(&B);
    cycles1 = cpucycles(); 
    ECADD(&A,&B,&D,&Prime);
    // ECDBL(&A,&D,&Prime,&a);
    cycles2 = cpucycles(); 

    // printf("cycles time = %10lld\n",cycles2 - cycles1);          //*3
   
    show_EN(&D);

    return 0 ;
}

#endif
#if 0 //파일 입출력 용
int main()
{
    int cnt_i, cnt_j = 0;
    int res = 0;
    FILE *R_opA, *R_Scalar, *R_inv, *R_SM, *O_Inv, *O_SM ;
    R_opA = fopen("TV_opA.txt", "r");      // Read할 파일 개방
    R_Scalar = fopen("TV_Scalar.txt", "r");      // Read할 파일 개방
    R_inv = fopen("TV_PFINV_TV.txt", "r");      // Read할 파일 개방
    R_SM = fopen("TV_SM.txt", "r");      // Read할 파일 개방
    O_Inv = fopen("test INV.txt", "w");    //Write할 파일 개방
    O_SM = fopen("test_SM.txt", "w");    //Write할 파일 개방

    assert(R_opA != NULL);
    assert(R_Scalar != NULL);
    assert(R_SM != NULL);
    assert(R_inv != NULL);

   //todo
    Ecc_st_ptr Based_Point = NULL;
    word bt_x[8] = {0x6b17d1f2,0xe12c4247,0xf8bce6e5,0x63a440f2,0x77037d81,0x2deb33a0,0xf4a13945,0xd898c296};
    word bt_y[8] = {0x4fe342e2,0xfe1a7f9b,0x8ee7eb4a,0x7c0f9e16,0x2bce3357,0x6b315ece,0xcbb64068,0x37bf51f5};
    set_EN(&Based_Point, bt_x,bt_y);

    bigint_st_ptr a = NULL;
    word inputdata_a[8] = {0xFFFFFFFF, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFC};
    bigint_st_ptr Prime = NULL;
    word Prime_array[8] = {0xFFFFFFFF, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
    set_bigint(&Prime, Prime_array);
    set_bigint(&a,inputdata_a);
    //todo

    bigint_st_ptr A = NULL; 
    bigint_st_ptr B = NULL;
    bigint_st_ptr C = NULL;
    bigint_st_ptr D = NULL;
    bigint_st_ptr E = NULL;

    word Input[8] = {0x00};
    word Input2[8] = {0x00};
    word Input3[8] = {0x00};
    word Input4[8] = {0x00};
    word Input5[8] = {0x00};

   bigint_st_ptr Inv_out = NULL;
   bigint_st_ptr SM_out_X = NULL;
   bigint_st_ptr SM_out_Y = NULL;
   Ecc_st_ptr EN_R = NULL;

    unsigned long long  cycles1 = 0 ,cycles2 = 0,totalcycles = 0;
    int uu = 1 ;
    //cycles1 = cpucycles
    for (cnt_j = 0 ; cnt_j < uu ; cnt_j++)
    {

        for (cnt_i = 0; cnt_i < 8; cnt_i++)
        {
            // res = fscanf(R_opA, "%08X", &Input[cnt_i]);
            // res = fscanf(R_inv, "%08X", &Input2[cnt_i]);
            res = fscanf(R_Scalar, "%08X", &Input3[cnt_i]);
            res = fscanf(R_SM, "%08X", &Input4[cnt_i]);
            res = fscanf(R_SM, "%08X", &Input5[cnt_i]);
        }
        
        // set_bigint(&A, Input);//* R opA
        // set_bigint(&B, Input2);//* R inv
        set_bigint(&C, Input3);//* R Scalar
        set_bigint(&D, Input4);//* R SM_X
        set_bigint(&E, Input5);//* R SM_Y

        // set_bigint_reset(&Inv_out);
        set_EN_reset(&EN_R);
        // Inverse_FLT(&A,&Inv_out,&Prime);
        cycles1 = cpucycles();
        ECC_MUL_LtoR(&Based_Point,&C,&EN_R,&Prime,&a);
        cycles2 = cpucycles();
        totalcycles += cycles2 - cycles1;
        // printf("%10lld\n",totalcycles/uu);
       
        // if (Compare(&B, &Inv_out) != BOTH_ARE_SAME)//주어진 답지와 계산한 값이 맞지 않은경우 Not_True
        //     printf("INV NOT true\n");//!add

        // if (Compare(&D, &SM_out) != BOTH_ARE_SAME)//주어진 답지와 계산한 값이 맞지 않은경우 Not_True
        //     printf("Mul pr Not_True\n");//!mul_pf
       

        for (cnt_i = 0; cnt_i < 8; cnt_i++)
        {
            // res = fprintf(O_Inv, "%08X", Inv_out->a[8-cnt_i-1]);//값 입력하기
            res = fprintf(O_SM,  "%08X", (EN_R)->x->a[8 - cnt_i - 1]);
        }

        fprintf(O_SM, "\n");
        for (cnt_i = 0; cnt_i < 8; cnt_i++)
        {

            res = fprintf(O_SM, "%08X", (EN_R)->y->a[8 - cnt_i - 1]);
        }

        // fprintf(O_Inv, "\n\n");
        fprintf(O_SM, "\n\n");

    delete_bigint(&A); //! Free function
    delete_bigint(&B);
    delete_bigint(&C);
    delete_bigint(&D);
   
    delete_bigint(&Inv_out);
    delete_bigint(&SM_out_X);
    delete_bigint(&SM_out_Y);
    }
    printf("%10lld\n",totalcycles/uu);
    fclose(R_inv); //개방한 파일들 닫아주기
    fclose(R_opA); //개방한 파일들 닫아주기
    fclose(R_Scalar); //개방한 파일들 닫아주기
    fclose(R_SM); //개방한 파일들 닫아주기
    fclose(O_Inv);
    fclose(O_SM);

    return 0;
}
#endif


#if 0 //!  성능 확인용 함수.
int main()
{
  int cnt_i, cnt_j = 0;
    int res = 0;
    FILE *R_opA, *R_Scalar, *R_inv, *R_SM, *O_Inv, *O_SM ;
    R_opA = fopen("TV_opA.txt", "r");      // Read할 파일 개방
    R_Scalar = fopen("TV_Scalar.txt", "r");      // Read할 파일 개방
    R_inv = fopen("TV_PFINV_TV.txt", "r");      // Read할 파일 개방
    R_SM = fopen("TV_SM.txt", "r");      // Read할 파일 개방
    O_Inv = fopen("test INV.txt", "w");    //Write할 파일 개방
    O_SM = fopen("test_SM.txt", "w");    //Write할 파일 개방

    assert(R_opA != NULL);
    assert(R_Scalar != NULL);
    assert(R_SM != NULL);
    assert(R_inv != NULL);


    word Input[8] = {0x00};
    word Input2[8] = {0x00};
    word Input3[8] = {0x00};
    word Input4[8] = {0x00};
    word Prime_array[8] = {0xFFFFFFFF, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
    
    bigint_st_ptr Prime = NULL;
    set_bigint(&Prime, Prime_array);

    bigint_st_ptr A = NULL; 
    bigint_st_ptr B = NULL;
    bigint_st_ptr C = NULL;
    bigint_st_ptr D = NULL;
   
   bigint_st_ptr Inv_out = NULL;
   bigint_st_ptr SM_out = NULL;

    unsigned long long cycles1, cycles2;
    unsigned long long total_cpucycles = 0x00;


    for (cnt_j = 0 ; cnt_j < 10000 ; cnt_j++)
    {
        for (cnt_i = 0; cnt_i < 8; cnt_i++)
        {
            res = fscanf(R_opA, "%08X", &Input[cnt_i]);
            res = fscanf(R_inv, "%08X", &Input2[cnt_i]);
            res = fscanf(R_Scalar, "%08X", &Input3[cnt_i]);
            res = fscanf(R_SM, "%08X", &Input4[cnt_i]);
        }
        
        set_bigint(&A, Input);//* R opA
        set_bigint(&B, Input2);//* R inv
        set_bigint(&C, Input3);//* R Scalar
        set_bigint(&D, Input4);//* R SM


        set_bigint_reset(&Inv_out);
        
        cycles1 = cpucycles(); //! 성능테스트 시작 단일 기준

        Inverse_FLT(&A,&Inv_out,&Prime);
       

        cycles2 = cpucycles(); //! 성능테스트 끝 단일 기준

        total_cpucycles += cycles2 - cycles1; //! 각 루프마다 total_cpucycles 더해주기
        // printf("%10lld\n",total_cpucycles);
    }
    
    printf("Inverse _FLT cycles for 10000 time = %10lld\n",total_cpucycles/ 10000);          //*3
  
   delete_bigint(&A); //! Free function
    delete_bigint(&B);
    delete_bigint(&C);
    delete_bigint(&D);
   
    delete_bigint(&Inv_out);
    delete_bigint(&SM_out);
    fclose(R_inv); //개방한 파일들 닫아주기
    fclose(R_opA); //개방한 파일들 닫아주기
    fclose(R_Scalar); //개방한 파일들 닫아주기
    fclose(R_SM); //개방한 파일들 닫아주기
    fclose(O_Inv);
    fclose(O_SM);

    return 0;
}
#endif

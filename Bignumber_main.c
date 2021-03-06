#include "Bignumber.h"

#define LOOP_COUNTER 100000
//need to test function for ECC
#if 1 //! file test for verification of Operation
int main()
{
    int cnt_i, cnt_j = 0;
    int res = 0;
    char NAF[257] = {0x00};
    char Table[WORD_LEN][WORD_BITLEN] = {0x00};
    Ecc_pt J_Table[8] = {0x00};
    FILE *R_opA, *R_opB, *R_opC, *R_opD, *R_Scalar, *R_ADD, *R_SQR, *R_SUB, *R_INV, *R_SM, *O_ADD, *O_SUB, *O_SQR, *O_INV, *O_SM;
    R_opA = fopen("TV_opA.txt", "r");       // Read할 파일 개방
    R_opB = fopen("TV_opB.txt", "r");       // Read할 파일 개방
    R_opC = fopen("TV_opC.txt", "r");       // Read할 파일 개방
    R_opD = fopen("TV_opD.txt", "r");       // Read할 파일 개방
    //R_Scalar = fopen("TV_Scalar.txt", "r"); // Read할 파일 개방
    R_Scalar = fopen("TV_Scalar_P256.txt", "r"); // Read할 파일 개방
    R_ADD = fopen("TV_PFADD_TV.txt", "r");  // Read할 파일 개방
    R_SUB = fopen("TV_PFSUB_TV.txt", "r");  // Read할 파일 개방
    R_SQR = fopen("TV_PFSQR_TV.txt", "r");  // Read할 파일 개방
    R_INV = fopen("TV_PFINV_TV.txt", "r");  // Read할 파일 개방
    //R_SM = fopen("TV_PFSM_TV.txt", "r");    // Read할 파일 개방
    R_SM = fopen("TV_SM_P256.txt", "r");    // Read할 파일 개방
    O_ADD = fopen("test add.txt", "w");     //Write할 파일 개방
    O_SUB = fopen("test_sub.txt", "w");     //Write할 파일 개방
    O_SQR = fopen("test_sqr.txt", "w");     //Write할 파일 개방
    O_INV = fopen("test_inv.txt", "w");     //Write할 파일 개방
    O_SM = fopen("test_sm.txt", "w");       //Write할 파일 개방
    assert(R_opA != NULL);
    assert(R_opB != NULL);
    assert(R_opC != NULL);
    assert(R_opD != NULL);
    assert(R_Scalar != NULL);
    assert(R_ADD != NULL);
    assert(R_SUB != NULL);
    assert(R_SQR != NULL);
    assert(R_INV != NULL);
    assert(R_SM != NULL);

    //? fix array
    bigint_st a = {{0x00}, 0x00};
    bigint_st Prime = {{0x00}, 0x00};
    Ecc_pt Based_Pt = {{0x00}, 0x00};
    word bt_x[8] = {0x6b17d1f2, 0xe12c4247, 0xf8bce6e5, 0x63a440f2, 0x77037d81, 0x2deb33a0, 0xf4a13945, 0xd898c296};
    word bt_y[8] = {0x4fe342e2, 0xfe1a7f9b, 0x8ee7eb4a, 0x7c0f9e16, 0x2bce3357, 0x6b315ece, 0xcbb64068, 0x37bf51f5};
    word inputdata_a[8] = {0xFFFFFFFF, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFC};
    word Prime_array[8] = {0xFFFFFFFF, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
    set_bigint(&Prime, Prime_array);
    set_bigint(&a, inputdata_a);
    set_EN(&Based_Pt, bt_x, bt_y);
    //? fix array

     word bt_i[8] = {0xABBEBAB0,0xA1E8F75B, 0x4292273, 0x2F26D3FC, 0x93152045, 0x48256BC1, 0x7DBA59F8 ,0x4387DB22};

    bigint_st opA = {{0x00}, 0x00};
    bigint_st opB = {{0x00}, 0x00};
    bigint_st opC = {{0x00}, 0x00};
    bigint_st opD = {{0x00}, 0x00};
    bigint_st Scalar = {{0x00}, 0x00};
    bigint_st radd = {{0x00}, 0x00};
    bigint_st rsub = {{0x00}, 0x00};
    bigint_st rsqr = {{0x00}, 0x00};
    bigint_st rinv = {{0x00}, 0x00};
    bigint_st rsm_x = {{0x00}, 0x00};
    bigint_st rsm_y = {{0x00}, 0x00};
    bigint_st oadd = {{0x00}, 0x00};
    bigint_st osub = {{0x00}, 0x00};
    bigint_st osqr = {{0x00}, 0x00};
    bigint_st oinv = {{0x00}, 0x00};
    Ecc_Jpt in = {{0x00}, 0x00};
    Ecc_Jpt out = {{0x00}, 0x00};
    Ecc_pt osm = {{0x00}, 0x00};

    word Input_opa[8] = {0x00};
    word Input_opb[8] = {0x00};
    word Input_opc[8] = {0x00};
    word Input_opd[8] = {0x00};
    word Input_scalar[8] = {0x00};
    word Input_radd[8] = {0x00};
    word Input_rsub[8] = {0x00};
    word Input_rsqr[8] = {0x00};
    word Input_rinv[8] = {0x00};
    word Input_rsm_x[8] = {0x00};
    word Input_rsm_y[8] = {0x00};

    unsigned long long cycles1 = 0, cycles2 = 0, totalcycles = 0;

    for (cnt_j = 0; cnt_j < LOOP_COUNTER; cnt_j++)
    {
        for (cnt_i = 0; cnt_i < 257; cnt_i++)
        {
            NAF[cnt_i] = 0x00; //? NAF 초기화
        }
        for (cnt_i = 0; cnt_i < 8; cnt_i++)
        {
            res = fscanf(R_opA, "%08X", &Input_opa[cnt_i]);
            res = fscanf(R_opB, "%08X", &Input_opb[cnt_i]);
            res = fscanf(R_opC, "%08X", &Input_opc[cnt_i]);
            res = fscanf(R_opD, "%08X", &Input_opd[cnt_i]);
            res = fscanf(R_ADD, "%08X", &Input_radd[cnt_i]);
            res = fscanf(R_SUB, "%08X", &Input_rsub[cnt_i]);
            res = fscanf(R_SQR, "%08X", &Input_rsqr[cnt_i]);
            res = fscanf(R_INV, "%08X", &Input_rinv[cnt_i]);
            res = fscanf(R_Scalar, "%08X", &Input_scalar[cnt_i]);
            res = fscanf(R_SM, "%08X", &Input_rsm_x[cnt_i]);
        }
        for (cnt_i = 0; cnt_i < 8; cnt_i++)
        {
            res = fscanf(R_SM, "%08X", &Input_rsm_y[cnt_i]);
        }

        set_bigint(&opA, Input_opa);
        set_bigint(&opB, Input_opb);
        set_bigint(&opC, Input_opc);
        set_bigint(&opD, Input_opd);
        set_bigint(&Scalar, Input_scalar);
        set_bigint(&radd, Input_radd);
        set_bigint(&rsub, Input_rsub);
        set_bigint(&rsqr, Input_rsqr);
        set_bigint(&rinv, Input_rinv);
        set_bigint(&rsm_x, Input_rsm_x);
        set_bigint(&rsm_y, Input_rsm_y);
        set_EN_reset(&osm);


        //!-----------------------------------------------------------------------------------Test
        //TODO //////////////////////////////////////////////////////////////////////////////////////////
        //* Addition verification test
        Addition(&opA, &opB, &oadd, &Prime);  

        //TODO //////////////////////////////////////////////////////////////////////////////////////////
        //* Subtraction verification test
        Subtraction(&opA, &opB, &osub, &Prime); 

        //TODO //////////////////////////////////////////////////////////////////////////////////////////
        //* Multiplication_OS verification test
        // OS64MUL_256(&opC, &opC, &osqr, &Prime);

        //* Multiplication_OS_Split verification test
        OS_SPlit_MUL_256(&opC,&opC,&osqr, &Prime);

        //* Multiplication_PS_Split verification test
        // PS_Split_MUL_256(&opC,&opC,&osqr, &Prime);

        //TODO //////////////////////////////////////////////////////////////////////////////////////////
        //* Inverse_Flt verification test
        // Inverse_FLT(&opD, &oinv, &Prime); 

        //* Inverse_EEA verification test
        Inverse_EEA(&opD, &oinv, &Prime);

        //TODO //////////////////////////////////////////////////////////////////////////////////////////
        //* Scalar_LtoR verification test
        //ECLtoR(&Based_Pt, &Scalar, &osm, &Prime, &a); 

        //* Scalar_RtoL verification test
        //ECRtoL(&Based_Pt, &Scalar, &osm, &Prime, &a); 

        //* L_to_R , Scalar_LtoR_Jacobian verification test
        // Trns_A_to_J(&in,&Based_Pt, &Prime); 
        // ECLtoR_J(&in,&Scalar,&out, &Prime); 
        // Trns_J_to_A(&osm,&out,&Prime);      

        //*_wNAF Scalar_LtoR_Affine verification test
        //NAF_recoding(&Scalar, NAF, &Prime);   
        //ECLtoR_wNAF(&Based_Pt, NAF, &osm, &Prime, &a);

        //*_wNAF_J Scalar_LtoR_Jacobian verification test
        // NAF_recoding(&Scalar, NAF, &Prime);   
        // Trns_A_to_J(&in, &Based_Pt, &Prime);  
        // ECLtoR_J_wNAF(&in, NAF, &out, &Prime); 
        // Trns_J_to_A(&osm, &out, &Prime);      
        
        //* L_to_R _Comb Scalar_LtoR_Jacobian verification test
        // Trns_A_to_J(&in, &Based_Pt, &Prime);  
        // comb_Table(Table, J_Table, &Based_Pt, &Scalar, &Prime);
        // ECLtoR_J_comb(&in, Table, J_Table, &out, &Prime);
        // Trns_J_to_A(&osm, &out, &Prime); 

        

        cycles1 = cpucycles(); //todo   ---------사이클을 재고싶은 함수 앞에 두기
        //*_wNAF_J Scalar_LtoR_Jacobian verification test
        NAF_recoding(&Scalar, NAF, &Prime);   
        Trns_A_to_J(&in, &Based_Pt, &Prime);  
        ECLtoR_J_wNAF(&in, NAF, &out, &Prime); 
        Trns_J_to_A(&osm, &out, &Prime);    
        cycles2 = cpucycles();//todo    ----------사이클을 재고싶은 함수 뒤에 두기

        totalcycles += cycles2 -  cycles1;//todo ---------- 고정

        //!-----------------------------------------------------------------------------------------구현 정확성 테스트
        if (Compare(&radd, &oadd) != BOTH_ARE_SAME)     //주어진 답지와 계산한 값이 맞지 않은경우 Not_True
            printf("add NOT true\n");                   //!add
        if (Compare(&rsub, &osub) != BOTH_ARE_SAME)     //주어진 답지와 계산한 값이 맞지 않은경우 Not_True
            printf("sub NOT true\n");                   //!sub
        if (Compare(&rsqr, &osqr) != BOTH_ARE_SAME)     //주어진 답지와 계산한 값이 맞지 않은경우 Not_True
            printf("sqr NOT true\n");                   //!sqr
        if (Compare(&rinv, &oinv) != BOTH_ARE_SAME)     //주어진 답지와 계산한 값이 맞지 않은경우 Not_True
            printf("inv NOT true\n");                   //!inv
        if (Compare(&rsm_x, &(osm.x)) != BOTH_ARE_SAME) //주어진 답지와 계산한 값이 맞지 않은경우 Not_True
            printf("SM_X NOT true\n");                  //!SM_X
        if (Compare(&rsm_y, &(osm.y)) != BOTH_ARE_SAME) //주어진 답지와 계산한 값이 맞지 않은경우 Not_True
            printf("SM_Y NOT true\n");                  //!SM_Y


        //!-----------------------------------------------------------------------------------------다룰 영역

        for (cnt_i = 0; cnt_i < 8; cnt_i++)
        {

            res = fprintf(O_SUB, "%08X", osub.a[8 - cnt_i - 1]);
            res = fprintf(O_ADD, "%08X", oadd.a[8 - cnt_i - 1]);
            res = fprintf(O_SQR, "%08X", osqr.a[8 - cnt_i - 1]);
            res = fprintf(O_INV, "%08X", oinv.a[8 - cnt_i - 1]);
            res = fprintf(O_SM, "%08X", osm.x.a[8 - cnt_i - 1]);
        }
        res = fprintf(O_SM, "\n");
        for (cnt_i = 0; cnt_i < 8; cnt_i++)
        {
            res = fprintf(O_SM, "%08X", osm.y.a[8 - cnt_i - 1]);
        }
        fprintf(O_SUB, "\n\n");
        fprintf(O_ADD, "\n\n");
        fprintf(O_SQR, "\n\n");
        fprintf(O_INV, "\n\n");
        fprintf(O_SM, "\n\n");
    }
    printf("cycles %d번 계산한 총 사이클/ %d  = %10lld\n", LOOP_COUNTER,LOOP_COUNTER,totalcycles / LOOP_COUNTER); //todo
    fclose(R_opB);    //개방한 파일들 닫아주기
    fclose(R_opA);    //개방한 파일들 닫아주기
    fclose(R_opC);    //개방한 파일들 닫아주기
    fclose(R_opD);    //개방한 파일들 닫아주기
    fclose(R_Scalar); //개방한 파일들 닫아주기
    fclose(R_ADD);    //개방한 파일들 닫아주기
    fclose(R_SUB);    //개방한 파일들 닫아주기
    fclose(R_SQR);    //개방한 파일들 닫아주기
    fclose(R_INV);    //개방한 파일들 닫아주기
    fclose(R_SM);     //개방한 파일들 닫아주기

    return 0;
}

#endif
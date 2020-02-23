#include "Bignumber.h"

#if 1 //file test for verification of Operation
int main()
{
    int cnt_i, cnt_j = 0;
    int res = 0;
    FILE *R_opA, *R_opB, *R_opC, *R_opD, *R_Scalar, *R_ADD, *R_SQR, *R_SUB, *R_INV, *R_SM, *O_ADD, *O_SUB, *O_SQR, *O_INV, *O_SM;
    R_opA = fopen("TV_opA.txt", "r");       // Read할 파일 개방
    R_opB = fopen("TV_opB.txt", "r");       // Read할 파일 개방
    R_opC = fopen("TV_opC.txt", "r");       // Read할 파일 개방
    R_opD = fopen("TV_opD.txt", "r");       // Read할 파일 개방
    R_Scalar = fopen("TV_Scalar.txt", "r"); // Read할 파일 개방
    R_ADD = fopen("TV_PFADD_TV.txt", "r");  // Read할 파일 개방
    R_SUB = fopen("TV_PFSUB_TV.txt", "r");  // Read할 파일 개방
    R_SQR = fopen("TV_PFSQR_TV.txt", "r");  // Read할 파일 개방
    R_INV = fopen("TV_PFINV_TV.txt", "r");  // Read할 파일 개방
    R_SM = fopen("TV_PFSM_TV.txt", "r");    // Read할 파일 개방
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
    char NAF[WORD_LEN *WORD_BITLEN + 1] = {0x00};
    set_bigint(&Prime, Prime_array);
    set_bigint(&a, inputdata_a);
    set_EN(&Based_Pt, bt_x, bt_y);
    //? fix array

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
    int time = 1; //!----------------------------------

    for (cnt_j = 0; cnt_j < time; cnt_j++)
    {

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


        // cycles1 = cpucycles(); //todo
        // Addition(&opA, &opB, &oadd, &Prime);
        // Subtraction(&opA, &opB, &osub, &Prime);
        // OS64MUL_256(&opC, &opC, &osqr, &Prime);
        // show_EN(&Based_Pt);
        // Inverse_FLT(&opD, &oinv, &Prime);
        // Inverse_EEA(&opD, &oinv, &Prime);
        // ECLtoR(&Based_Pt, &Scalar, &osm, &Prime, &a);
        // ECRtoL(&Based_Pt, &Scalar, &osm, &Prime, &a);
        NAF_recoding(&Scalar,NAF,&Prime);//* NAF_recording
        ECLtoR_wNAF(&Based_Pt,NAF,&osm,&Prime,&a);//* NaF_recording
        // Trns_A_to_J(&in,&Based_Pt, &Prime); //* L_to_R
        // ECLtoR_J(&in,&Scalar,&out, &Prime); //* L_to_R
        // Trns_J_to_A(&osm,&out,&Prime);      //* L_to_R
        // cycles2 = cpucycles();//todo
        // totalcycles += cycles2 -  cycles1;//todo

        // if (Compare(&radd, &oadd) != BOTH_ARE_SAME)     //주어진 답지와 계산한 값이 맞지 않은경우 Not_True
        //     printf("add NOT true\n");                   //!add
        // if (Compare(&rsub, &osub) != BOTH_ARE_SAME)     //주어진 답지와 계산한 값이 맞지 않은경우 Not_True
        //     printf("sub NOT true\n");                   //!sub
        // if (Compare(&rsqr, &osqr) != BOTH_ARE_SAME)     //주어진 답지와 계산한 값이 맞지 않은경우 Not_True
        //     printf("sqr NOT true\n");                   //!sqr
        // if (Compare(&rinv, &oinv) != BOTH_ARE_SAME)     //주어진 답지와 계산한 값이 맞지 않은경우 Not_True
        //     printf("inv NOT true\n");                   //!inv
        if (Compare(&rsm_x, &(osm.x)) != BOTH_ARE_SAME) //주어진 답지와 계산한 값이 맞지 않은경우 Not_True
            printf("SM_X NOT true\n");                  //!SM_X
        // if (Compare(&rsm_y, &(osm.y)) != BOTH_ARE_SAME) //주어진 답지와 계산한 값이 맞지 않은경우 Not_True
        // printf("SM_Y NOT true\n");                  //!SM_Y

        for (cnt_i = 0; cnt_i < 8; cnt_i++)
        {

            // res = fprintf(O_SUB, "%08X", osub.a[8 - cnt_i - 1]);
            // res = fprintf(O_ADD, "%08X", oadd.a[8 - cnt_i - 1]);
            // res = fprintf(O_SQR, "%08X", osqr.a[8 - cnt_i - 1]);
            // res = fprintf(O_INV, "%08X", oinv.a[8 - cnt_i - 1]);
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
    // printf("cycles 10000 time = %10lld\n", totalcycles / time); //todo
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

#include <stdio.h>
#include <openssl/err.h>
#include <openssl/rand.h>

int main(int argc, char *argv[])
{
    int i;
    unsigned int retVal = 1;
    // 랜덤 수의 길이는 64로 한다.

    unsigned int length = 16;
    // PRNG에 공급할 seed 생성
    // 생성할 랜덤 수 길이만큼의 버퍼 생성
    unsigned char *buffer = (unsigned char *)malloc(sizeof(unsigned char) * length);
    RAND_seed(buffer,length);

    // run PRNG
    retVal = RAND_bytes(buffer, length);
    if (retVal <= 0)
    {
        printf("error encount !n");
        return 0;
    }

    // print random number
    printf("Random number = ");

    for (i = 0; i < length; i++)
        printf("%02x", buffer[i]);
    

    return 1;
}
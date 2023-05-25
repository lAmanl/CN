#include <stdio.h>
#include <math.h>

int main(){
    // m is the number of data bits
    // r is the number of redundant bits or parity bits
    int m, r;
    printf("Enter the number of bits in the dataword: ");
    scanf("%d", &m);

    // dataword[] stores the dataword
    int inp, dataword[m];
    printf("Enter the dataword: ");
    scanf("%d", &inp);

    int temp = inp, res = 2;
    int i, j;
    for (i = 0; i < m; ++i){
        dataword[i] = inp % 10;
        inp = inp / 10;
    }
    for (r = 1; res < m + r + 1; ++r)
        res = 2 * res;

    // bits[] calculates and stores the equivalent Hamming code of the dataword
    res = 1;
    int k = 0;
    int bits[m + r];
    for (i = 1; i <= m + r; ++i){
        if (i != res)
            bits[i - 1] = dataword[k++];
        else{
            bits[i - 1] = 0;
            res = res * 2;
        }
    }

    // Calculating the parity bits of bits[]
    k = 1;
    for (i = 1; i <= m + r; i = 2 * i){
        for (j = i - 1; j < m + r;){
            if (k > i){
                j = j + i;
                k = 1;
            }
            else{
                bits[i - 1] += bits[j];
                ++j, ++k;
            }
        }
        bits[i - 1] = bits[i - 1] % 2;
        k = 1;
    }

    printf("The codeword (with parity bits at 2^i index) is ");
    for (i = m + r - 1; i >= 0; --i)
        printf("%d", bits[i]);
    printf("\n");
    j = 1;
    int codeword[m + r];
    for (i = 0; i < m; ++i)
        codeword[i + r] = dataword[i];
    for (i = 0; i < r; ++i){
        codeword[i] = bits[j - 1];
        j = j * 2;
    }
    printf("The codeword (with parity bits at the end) is ");
    for (i = m + r - 1; i >= 0; --i)
        printf("%d", codeword[i]);
    printf("\n");
    int pos;
    printf("Enter the bit position of the codeword which is to be changed: ");
    scanf("%d", &pos);

    // Complementing the bit at position (pos - 1)
    if (pos > m + r || pos < 1)
        return 0;
    else if (bits[pos - 1] == 0)
        bits[pos - 1] = 1;
    else
        bits[pos - 1] = 0;
    printf("The incorrect codeword (with parity bits at 2^i index) is ");
    for (i = m + r - 1; i >= 0; --i)
        printf("%d", bits[i]);
    // Calculating the error syndrome bits in err_syndrome[]
    k = 1;
    int c = 0;
    int err_syndrome[r];
    for (i = 1; i <= m + r; i = i * 2){
        for (j = i - 1; j < m + r;){
            if (k > i){
                j = j + i;
                k = 1;
            }
            else{
                err_syndrome[c] += bits[j];
                ++j, ++k;
            }
        }
        err_syndrome[c] %= 2;
        ++c, k = 1;
    }

    // Calculating the decimal equivalent of the bits in err_syndrome[]
    res = 1;
    int dec_eq = 0;
    for (i = 0; i < r; ++i){
        dec_eq += res * err_syndrome[i];
        res = res * 2;
    }

    // Correcting the error at position (dec_eq - 1), if there is an error
    if (dec_eq > 0 && dec_eq < m + r)
        if (bits[dec_eq - 1] == 0)
            bits[dec_eq - 1] = 1;
        else
            bits[dec_eq - 1] = 0;
    else if (dec_eq == 0)
        printf("No change in bits detected\n");
    printf("\n");

    printf("The codeword (with parity bits at 2^i index) is ");
    for (i = m + r - 1; i >= 0; --i)
        printf("%d", bits[i]);
    printf("\n");
    
    printf("The codeword (with parity bits at the end) is ");
    for (i = m + r - 1; i >= 0; --i)
        printf("%d", codeword[i]);
    printf("\n");
    return 0;
}
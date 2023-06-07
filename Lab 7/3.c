#include <stdio.h>
#include <string.h>
#include <math.h>

int main(){
    char s[19];
    printf("Enter the IP address: ");
    scanf("%s", s);
    int len = strlen(s);
    int n = (s[len-2]-'0')*10 + s[len-1]-'0';
    int noa = (int)pow(2, 32-n);
    printf("Number of addresses: %d\n", (int)pow(2, 32-n));
    int l, j = 0;
    for(int i = 0; i<4; i++){
    	l = 0;
    	while(s[j] != '.' && s[j] != '/'){
    	    l = l*10 + (int)(s[j] - '0');
    	    j++;
    	}
    	j++;
    }    
    int k = 7;
    int b[9] = {2};
    for(int i = 0; i<8; i++){
    	int a = l%2;
    	l = l/2;
    	b[k--] = a; //01010010
    }
    k = 7;
    for(int i = 0; i<32-n; i++){
    	b[k--] = 0;
    }
    int p = 7, dec = 0;
    for(int i = 0; i<8; i++){
    	dec += b[i]*pow(2,p--);
    }
    j = 0;
    printf("First Address: ");
    for(int i = 0; i<3; i++){
    	while(s[j] != '.'){
    	    printf("%c", s[j++]);
    	}
    	printf(".");
    	j++;
    }
    printf("%d/%d\n", dec, n);
    j = 0;
    printf("Last Address: ");
    for(int i = 0; i<3; i++){
    	while(s[j] != '.'){
    	    printf("%c", s[j++]);
    	}
    	printf(".");
    	j++;
    }
    printf("%d/%d\n", dec+noa-1, n);
    return 0;
}

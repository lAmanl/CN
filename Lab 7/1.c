#include <stdio.h>

int main(){
    char s[16];
    printf("Enter the IP address: ");
    scanf("%s", s);
    char class;
    int a = 0, i = 0;
    while(s[i] != '.'){
    	a = a*10 + (int)(s[i]-'0');
    	i++;
    }
    printf("%s/", s);
    if(a >= 0 && a<= 127)
    	printf("8\n");
    else if(a >= 128 && a <= 191)
    	printf("16\n");
    else if(a >= 192 && a <= 223)
    	printf("24\n");
    else
        printf("Invalid");
    
    return 0;
}

#include <stdio.h>
#include <string.h>

int main(){
    char s[16] = {'\0'};
    printf("Enter the IP address: ");
    scanf("%s", s);
    int j = 0;
    for(int i = 0; i<4; i++){
    	char b[9] = {'\0'};
    	int num = 0;
    	int k = 7;
    	while(s[j] != '.' && s[j] != '\0'){
    	    num = num*10 + (int)(s[j] - '0');
    	    j++;
    	}
    	for(int l = 0; l<8; l++){
    	    int a = num%2;
    	    num = num/2;
    	    b[k--] = a+'0';
    	}
    	printf("%s ", b);
    	j++;
    }    
    printf("\n");
    return 0;
}

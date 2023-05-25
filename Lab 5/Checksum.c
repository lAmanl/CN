        #include <stdio.h>

        void wrap_sum(int temp, int *code){
            for (int i = 0; temp != 0; ++i){
                *(code + i) = *(code + i) + temp % 2;
                temp = temp / 2;
            }
        }

        void dec_bin(int num, int *code){
            int temp = num, i;
            for (i = 0; i <= 10; ++i){
                *(code + i) = temp % 2;
                temp = temp / 2;
            }
            if (temp != 0)
                wrap_sum(temp, code);
            if (num < 0){
                for (i = 0; i <= 10; ++i)
                    if (code[i] == 0)
                        code[i] = 1;
                    else
                        code[i] = 0;
            }
        }

        int main(){
            int n, i, j;
            int checksum = 0, sum = 0;
            printf("Enter the number of data items: ");
            scanf("%d", &n);
            int arr[n + 1];
            for (i = 0; i < n; ++i){
                printf("Enter a number: ");
                scanf("%d", &arr[i]);
                sum += arr[i];
            }
            // Sender's Side
            arr[n] = -sum;
            checksum = -sum;
            int code[n][11];
            for (i = 0; i <= n; ++i)
                dec_bin(arr[i], &code[i][0]);
            printf("Sender's side:\n");
            printf("The checksum in decimal is : %d\n", checksum);
            printf("The checksum in binary is : ");
            for (i = 10; i >= 0; --i)
                printf("%d", code[n][i]);
            printf("\n");
            // Receiver's Side
            int add = 0, carry = 0;
            int check_sum[15];
            for (i = 0; i <= 14; ++i)
                check_sum[i] = 0;
            for (i = 0; i <= n; ++i){
                for (j = 0; j <= 10; ++j){
                    add = check_sum[j] + code[i][j] + carry;
                    if (add == 3){
                        check_sum[j] = 1;
                        carry = 1;
                    }
                    else if (add == 2){
                        check_sum[j] = 0;
                        carry = 1;
                    }
                    else if (add == 1){
                        check_sum[j] = 1;
                        carry = 0;
                    }
                    else{
                        check_sum[j] = 0;
                        carry = 0;
                    }
                }
                for (j = 11; j <= 14 && carry == 1; ++j){
                    add = check_sum[j] + carry;

                    if (add == 2){
                        check_sum[j] = 0;
                        carry = 1;
                    }
                    else if (add == 1){
                        check_sum[j] = 1;
                        carry = 0;
                        break;
                    }
                    else{
                        check_sum[j] = 0;
                        carry = 0;
                        break;
                    }
                }
            }
            //  Wraaping the check_sum
            for (i = 11; i <= 14; ++i){
                if (check_sum[i] == 1){
                    add = 0, carry = check_sum[i];
                    for (j = i % 11; j <= 10 && carry == 1; ++j){
                        add = check_sum[j] + carry;
                        if (add == 3){
                            check_sum[j] = 1;
                            carry = 1;
                        }
                        else if (add == 2){
                            check_sum[j] = 0;
                            carry = 1;
                        }
                        else if (add == 1){
                            check_sum[j] = 1;
                            carry = 0;
                        }
                        else{
                            check_sum[j] = 0;
                            carry = 0;
                        }
                    }
                    for (j = 0; j <= 10 && carry == 1; ++j){
                        add = check_sum[j] + carry;
                        if (add == 2){
                            check_sum[j] = 0;
                            carry = 1;
                        }
                        else if (add == 1){
                            check_sum[j] = 1;
                            carry = 0;
                            break;
                        }
                        else{
                            check_sum[j] = 0;
                            carry = 0;
                            break;
                        }
                    }
                }
            }
            printf("Receiver's side:\n");
            printf("The checksum in binary is: ");
            for (i = 10; i >= 0; --i){
                if (check_sum[i] == 1)
                    check_sum[i] = 0;
                else
                    check_sum[i] = 1;
                printf("%d", check_sum[i]);
            }
            printf("\n");
            int flag = 1;
            for (i = 0; i <= 10; ++i){
                if (check_sum[i] != 0)
                    flag = 0;
            }
            if (flag == 1)
                printf("No errors detected\n");
            else
                printf("Error detected\n");
        }
#include <stdio.h>
#include <head.h>
int main() {
    // 调用 add 函数
    int result_add = add(5, 3);
    printf("Addition result: %d\n", result_add);

    // 调用 sub 函数
    int result_sub = sub(5, 3);
    printf("Subtraction result: %d\n", result_sub);


    // 调用 mul 函数
    int result_mul = mul(5, 3);
    printf("Multiplication result: %d\n", result_mul);
    return 0;
}

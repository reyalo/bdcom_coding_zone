#include<stdio.h>

int main(void){
    // int a, b;
    // a = 3;
    // b = (a++) + (a++) + (a++);
    // printf("%d, %d\n", a, b);

    // a = 3;
    // b = (++a) + (++a) + (++a);
    // printf("%d, %d\n", a, b);

    // return 0;

    //_______________________code2_________________________
    //section a:
    // int a[3][2] = {{1, 2}, {3, 4}, {5, 6}};
    // int a[3][2] = {(1, 2), (3, 4), (5, 6)};
    // for(int i=0; i<3; i++){
    //     for(int j=0; j<2; j++){
    //         printf("%d %d %d\n",i , j, a[i][j]);
    //     }
    // }
    // int a[3][2] = {(1, 2), (3, 4), (5, 6)};
    // printf("%d\n", a[0][1]);

    // section b:
    // int x = 3, y = 0, z = 0;
    // if(x = y + z)
    //     printf("111\n");
    // else
    //     printf("222\n");

    //section c:
    // int a = 0, b = 0, c = 0, d = 0;
    // d = (c = ( a = 11, b = 22)) == 11;

    // printf("%d %d", c, d);

    //section d
    int x = 3, y = 2, z = 3;
    z = x < y ? !x : !((x = 2) || (x = 3)) ? (x = 1) : (y = 2);
    printf("%d %d %d\n", x, y, z);


}
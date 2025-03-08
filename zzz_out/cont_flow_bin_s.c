#include<stdio.h>
int binsearch(int x, int v[], int n)
{
    int low, high, mid;
        low = -1;
        high = n;

    while (low + 1 < high) {
            mid = (low + high)/2;
            if (v[mid] <= x)
                low = mid;
            else
                high = mid;

    }

    // if(v[low] == x)return low;

    // return -1; /* no match */





}

int main(){
    int n = 4;
    int v[] = {1, 3, 5, 7};
    int x = 4;
    int indx = binsearch(x, v, n);

    if(indx < 0){
        printf("Not found\n");
    }
    else printf("found\n");

}

#include<stdio.h>
int main ()
{   int p,c,m,avg;
    printf("Enter Physics Marks:");
    scanf("%d",&p);
    printf("Enter chemistry marks:");
    scanf("%d",&c);
    printf("Enter maths marks:");
    scanf("%d&,&m");
    avg=(p+c+m)/3;
    printf("\nPCM Average =%d",avg);
    avg>=50?printf("\nEligible for Admission"): printf("\nNot Eligible for admission");


}

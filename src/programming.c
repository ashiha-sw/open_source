#inlude <stdio.h> 

int main(void)
{
  int i,j;
  for(i = 0; i<=9; i++)
    for(j = 0; j<=9; j++)
      printf("%d 곱하기 %d은 %d\n",i,j,i*j);
   return 0;
}

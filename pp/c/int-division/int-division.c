#include <stdio.h>
int main() {
  int i,j;
  float ratio;
  double dratio;

  printf("Program demonstrating integer division\n");
  printf("Input two integer numbers i,j:");
  scanf("%i %i", &i,&j);
  ratio = i/j; // integer division
  printf("Integer division i/j= %f\n",ratio);

  ratio = i/(float) j; // float division
  printf("Float division i/j= %f\n",ratio);

  dratio = (double)i/ j; // double division
  printf("Double division i/j= %lf\n",dratio);

  return 0;
} // end of MAIN

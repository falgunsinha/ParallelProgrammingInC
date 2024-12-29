#include <stdio.h>
int main() {
  int i=7,j=4;
  float ratio;
  double dratio;

  printf("Program demonstrating integer division\n");
//	printf("Input two integer numbers i,j:");
//	scanf("%i %i", &i,&j);
  printf("i=%i, j=%i\n",i,j);
  ratio = i/j; // integer division
  printf("Integer division i/j= %f\n",ratio);
  printf("    This result is 1, because the result of integer division is an integer number\n");
  printf("    i.e. the integer part of the division. Only then the result is assigned to float variable ratio\n\n");
  ratio = i/(float) j; // float division

  printf("Float division i/j= %f\n",ratio);
  printf("    Correct result 1.75, because j is first converted to float. The division is then calculated in float\n");
  printf("    The conversion of type function (float) has top priority\n\n");
  dratio = (double)i/ j; // double division

  printf("Double division i/j= %lf\n",dratio);
  printf("    The same holds for coversion to type double\n\n");

  printf("Of course it is also posible to change the float type to integer\n");
  i=ratio;
  dratio=(int) ratio;
  ratio=(int)ratio;
  printf("i= %i, ratio=%f, dratio=%lf\n",i,ratio,dratio);
  return 0;
} // end of MAIN

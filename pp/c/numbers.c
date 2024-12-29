#include <stdio.h>
#include <math.h> // this include is not needed when using icc
// shows how declare and print numbers of different types
int main() {
/* BTW you can make a comment accross several lines by hash+star
and star+hash */
  int j; float x; double y;

  j=acos(-1.);x=acos(-1.);y=acos(-1.);
  printf("j=%i\n",j);
  printf("x=%f\n",x);
  printf("y=%f\n",y);

  printf("1000*x=%f\n",1000*x);
  printf("1000*y=%f\n",1000*y);

  printf("x=%16.12f\n",x);
  printf("y=%16.12f\n",y);

  return 0;
} // end of MAIN

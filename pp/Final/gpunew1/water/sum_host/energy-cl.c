// linear array - optimization of variables - copy a molecule (9 elements) from global memory to private memory
// nthreads = number of GPU threads
// nmol = # molecules =  3 of atoms/3
// L = box size
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
// #pragma CL_USE_DEPRECATED_OPENCL_1_0_APIS 
__kernel void energy(uint nthreads, uint nmol,
       __global double *atomArray,__global double *result, double L )
//       double L, double total_energy )
{
int i;
// variables and constants used to calculate energy of water
double r6,distsq,dist;
double energy_me=0;
const double sig=0.3166,eps=0.65,eps0=8.85e-12,e=1.602e-19,Na=6.022e23;
double q[3]={-0.8476,0.4238,0.4238};
double rcutsq=1.44; elst,sig6;
elst=e*e/(4*3.141593*eps0*1e-9)*Na/1e3,sig6=pow(sig,6);

int me = get_global_id(0); // obtaining the index of a thread
// calculation of energy starting from ID of own thread till the end
for (i=me;i<nmol; i = i + nthreads) { // GPU parallelization of outer loop
  int j,atomi,atomj,xyz,u;
  double ene=0;
  double shift[3];
  double dr[3];
  double mol1[9];
// copy 9 coordinates to private memory
  for(u=0;u<9; u++) mol1[u]= atomArray[i*9+u];
  
  for(j=i+1;j<nmol;j++) { //inner loop
    for(xyz=0;xyz<3;xyz++) {
      dr[xyz] = mol1[xyz] - atomArray[j*9+xyz];
      shift[xyz]=-L* floor(dr[xyz]/L+ .5);
      dr[xyz] = dr[xyz] + shift[xyz];
    }
    distsq= dr[0]*dr[0] + dr[1]*dr[1] + dr[2]*dr[2];
    ene = 0;
    if(distsq<rcutsq){ //calculate energy if two molecules are within 1.2 nm
      r6=sig6/(distsq*distsq*distsq);
      ene=4*eps*r6*(r6-1.);
      for(atomi=0;atomi<3;atomi++) {
        for(atomj=0;atomj<3;atomj++) {
          for(xyz=0;xyz<3;xyz++)
            dr[xyz]= mol1[atomi*3 + xyz] - atomArray[j*9+atomj*3 + xyz] + shift[xyz];
            dist = sqrt(dr[0]*dr[0]+dr[1]*dr[1]+dr[2]*dr[2]);
            ene = ene+elst*q[atomi]*q[atomj]/dist;
        }
      }
    }
    energy_me += ene;
  }
}
result[me] = energy_me;
// synchronization (arguments are OpenCL synchronization switches)
barrier(CLK_GLOBAL_MEM_FENCE);
//barrier(CLK_GLOBAL_MEM_FENCE | CLK_LOCAL_MEM_FENCE);
}

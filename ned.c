// g++ -o ned ned.c -ltommath
//Kn=474691839095945264362538101109863618607781504175903737338048001803579690420469780840102816434989852155531340614154235948888598367327156357656036314078956738794466187552277676187125861899523727983747813563026882295545983767085137449951288857716715461806197601766591161117
//Ke=787749893733024415799810030312901002456339631953553117598884308014898773341793474638998024611913918487347424691376850637787296236392231
//Kd=226605413020760117229482412708690940872808244114077106923306951667650732890383783293912172374762874655303015567668106168160579436281834265004358715899961010751622374837274982350137310621926597360172298570993735525408971719912578457733619922989521604053292405378262717271
//6dcedb8d2d2f21eded095e4b19aa599d9f9090ca38cd13b0c15c87844f95af75f3e3a465a42dfdd7f497a027cc1e7cb960732bb0d11600efa42d522279d3f99b8e986458ae4a4c48ad31c694373a13c9649a5699d96a39fc4dbb2b49ff86f87180df5a9011cc6e87d7b4c440c7f75357
#include "tommath.h"
#include <ctime>
#pragma comment(lib,"tommath.lib")
#include <iostream>
#include <fstream>
using namespace std;
int main()
{
  mp_int p,q,ao_la,p1,q1,gcd;
  mp_int n,e,d;
  mp_init(&p);
  mp_init(&q);
  mp_init(&n);
  mp_init(&ao_la);
  mp_init(&p1);
  mp_init(&q1);
  mp_init(&gcd);
  mp_init(&e);
  mp_init(&d);
 
  int result1,i=600;//随机选择素数p
  do{
    srand((int)time(0)+i);
    i+=300;
    mp_rand(&p,8);
    mp_prime_is_prime(&p,256,&result1);
  }while(result1==0);
 
  int result2,j=700;//随机选择素数q
  do{
    srand((int)time(0)+j);
    j+=300;
    mp_rand(&q,8);
    mp_prime_is_prime(&q,256,&result2);
  }while(result2==0);
 
 mp_mul(&p,&q,&n);
 mp_sub_d(&p,1,&p1);
 mp_sub_d(&q,1,&q1);
 mp_mul(&p1,&q1,&ao_la);
 
  int k=800;//随机选择整数e，使得gcd(e,ao_la)=1
  do
  {
    srand((int)time(0)+k);
    mp_rand(&e,8);
    mp_gcd(&e,&ao_la,&gcd);
  }while(mp_cmp_d(&gcd,1)!=MP_EQ);//MP_GT||mp_cmp_d(&gcd,1)==MP_LT);常量MP_GT:大于；MP_LT:小于
 
  mp_invmod(&e,&ao_la,&d);//求d,使ed=1(mod ao_la)
 
  char *Kn=new char[600];
  char *Ke=new char[600];
  char *Kd=new char[600];

 
  mp_toradix(&n,Kn,16);//store n in "Kn" as a base-10 string of ASCII chars
  mp_toradix(&e,Ke,16);
  mp_toradix(&d,Kd,16);

 

  ofstream fout;
  fout.open("RSAkey.txt");
  fout<<"Kn="<<Kn<<endl;
  fout<<"Ke="<<Ke<<endl;
  fout<<"Kd="<<Kd<<endl;

   
 
  delete[] Kn;
  delete[] Ke;
  delete[] Kd;
  mp_clear(&p);
  mp_clear(&q);
  mp_clear(&p1);
  mp_clear(&q1);
  mp_clear(&ao_la);
  mp_clear(&gcd);
  mp_clear(&n);
  mp_clear(&e);
  mp_clear(&d);
}

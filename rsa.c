// gcc rsa.c -lssl -lcrypto

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/rsa.h>
void testRSAGen(){
   RSA    *r;
   int   bits=512,ret;
    unsigned long e=RSA_3;
   BIGNUM   *bne;
   r=RSA_generate_key(bits,e,NULL,NULL);
   RSA_print_fp(stdout,r,11);
   printf("--------------------------------\n");
   RSA_free(r);
   bne=BN_new();
   ret=BN_set_word(bne,e);
   r=RSA_new();
   ret=RSA_generate_key_ex(r,bits,bne,NULL);
   if(ret!=1)
    {
   printf("RSA_generate_key_ex err!\n");
    return ;
    }
   //RSA_print_fp(stdout,r,11);
   RSA_free(r);

}

RSA * get_new_cert_rsa(int key_length)
{
#if OPENSSL_VERSION_NUMBER >= 0x0090801fL
	RSA *rsa = RSA_new();
	BIGNUM *bn = BN_new();
    
	if (BN_set_word(bn, 0x10001) <= 0 ||
			RSA_generate_key_ex(rsa, key_length, bn, NULL) <= 0)
	{
		BN_free(bn);
		printf("+++ get_new_cert_rsa failed--------------------\n");
		RSA_free(rsa);
		return NULL;
	}
    
/*
        BIGNUM *n;              // public modulus
        BIGNUM *e;              // public exponent
        BIGNUM *d;              // private exponent
        BIGNUM *p;              // secret prime factor
        BIGNUM *q;              // secret prime factor
        BIGNUM *dmp1;           // d mod (p-1)
        BIGNUM *dmq1;           // d mod (q-1)
        BIGNUM *iqmp;           // q^-1 mod p
*/

    char *n, *e, *d, *p, *q, *dmp1, *dmq1, *iqmp;
    
    e = BN_bn2hex(rsa->e);
    d = BN_bn2hex(rsa->d);
    n = BN_bn2hex(rsa->n);
    p = BN_bn2hex(rsa->p);
    q = BN_bn2hex(rsa->q);
    dmp1 = BN_bn2hex(rsa->dmp1);
    dmq1 = BN_bn2hex(rsa->dmq1);
    iqmp = BN_bn2hex(rsa->iqmp);
   
    printf("size=%d\n", strlen(e));
    
	BN_free(bn);
#else
	RSA *rsa = RSA_generate_key(key_length, 0x10001, NULL, NULL);
	if(rsa == NULL)
	{
		return NULL;
	}
#endif
	return rsa;
}

void testRSA(){
    RSA *r;
    BIGNUM *bne,*bnn,*bnd;

    int bits = 1024, ret, len, flen, padding, i;

    unsigned char *key, *p;
    BIO *b;
    //要加密的明文
    unsigned  char *in = "abcef";
    unsigned  char *encData,*decData,*tmpData;//加密后的数据/解密后的数据/临时指针

    //使用的密匙数据
    unsigned long e = 0x010001;
    const char *MODULUS="C8B92CFCD0381A5395210EE2FE1F21235DA22C7064320B1624811B292717CE32091FB6F98D752C9437EE7C45A3A908E562C5707B410162723A3411C8D1FEA24E7033394772A81AB833A03A9D01619B35474219B26072928CC206A1F1F6F6196AEA858C466A2CE7DA6EB8D7581616888F8C071603381E7AD226765631C75D082C6ED6D33445B4EAA6830FB3E788827749BBDB56701373D3C4A84F1F0A73F824567B96653E24A6DEBFE0CFFF282A1B715D8E42B61751E953707D3321F990CEBFD895F54795D32EB167B9B6C1FE3952F4CE71F56C7C2DEAC5D3DF9B47D25044F2B00F1D57DDE285A2A1B33958C6EFC2E0A9FB5EBFE6434564186466B6E06FADE20F";
    const char *PRIVATE="990D39D47C208B52784000CDECDF67B143D048FA5A2C1E6FFFDDC20C1B16F7CF074DFB8345EDB6D43DE1C73882967D860B4E697A5212F03211576AC829F7736344028B0683D2D2ACEA109F20E0D0C03C5BDD7722A0903C56C00BB6A87D9B21BE789CE45D3CD088E9F1CB57C87EEF27A9ED27EA0D02599B656A96891AE3DD7840AEE664585CAFF15BF6AEB2D756C215B6ED1505A17851C1DF1A9B23CC539A2523083BEEB0823247978079423EBB8D5576CC949331812F58DDFEAE61614DC91F6DAC5CEF080E3BE21EC3A95F409FEC84AB41D72B1357319E2560324EF6807EC68DFED9CA0D3AE38AF5F30BAF688CCF41ED30870FF8398B7C58B929B7F4663B7641";

   //构建RSA数据结构
    bne = BN_new();
    bnd = BN_new();
    bnn = BN_new();
    ret = BN_set_word(bne, e);
    BN_hex2bn(&bnd, PRIVATE);
    BN_hex2bn(&bnn, MODULUS);

    r = RSA_new();
   r->e=bne;
   r->d=bnd;
   r->n=bnn;
   RSA_print_fp(stdout, r, 5);

   //准备输出的加密数据结构
    flen =  RSA_size(r);// - 11;
    encData =  (unsigned char *)malloc(flen);
   bzero(encData, flen);//memset(encData, 0, flen);

   printf("Begin RSA_private_encrypt ...\n");
    ret =  RSA_private_encrypt(flen, in, encData, r,  RSA_NO_PADDING);
    if(ret < 0){
      printf("Encrypt failed!\n");
      return;
    }
   printf("Size:%d\n", ret);
   printf("ClearText:%s\n", in);
   printf("CipherText(Hex):\n");
   tmpData=encData;
   for  (i=0; i<ret; i++){
       printf("0x%x, ", *tmpData);
      tmpData++;
    }
    printf("end private encrypt \n");
   printf("------------------------\n");


   //准备输出的解密数据结构
    flen =  RSA_size(r);// - 11;
    decData =  (unsigned char *)malloc(flen);
   bzero(decData, flen);//memset(encData, 0, flen);

   printf("Begin RSA_public_decrypt ...\n");
    ret =  RSA_public_decrypt(flen, encData, decData, r,  RSA_NO_PADDING);
    if(ret < 0){
         printf("RSA_public_decrypt failed!\n");
         return;
    }
   printf("Size:%d\n", ret);
   printf("ClearText:%s\n", decData);

   free(encData);
   free(decData);
   RSA_free(r);

}
void testSHA256(){
    unsigned char in[]="asdfwerqrewrasfaser";
    unsigned char out[32];

    size_t n;
    int i;

   n=strlen((const char*)in);

   SHA256(in,n,out);
   printf("\n\nSHA256 digest result:\n");

   printf("%d\n",sizeof(out));

   for(i=0;i<32;i++)
      printf("%d",out[i]);
   printf("\n");

}
int main(void) {
   //testSHA256();
   get_new_cert_rsa(2048);
   //testRSA();
   return EXIT_SUCCESS;
}

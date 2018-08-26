#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <iomanip>
#include <stdio.h>
#include <stdexcept>
#include <exception>
#include "BlowFish.h"

using namespace std;
unsigned char bundleVer[]= {"****************************************************************"};
void BlowFishDecrypt(unsigned char *szDataIn,unsigned char *szDataOut, int len) {
    CBlowFish oBlowFish((unsigned char*)bundleVer, 118);
    oBlowFish.ResetChain();
    int num = ((len % 8) != 0) ? ((len + 8) - (len % 8)) : len;
    if(num==len)
        oBlowFish.Decrypt((unsigned char*)szDataIn,
                          (unsigned char*)szDataOut, num, CBlowFish::CBC);
    else {
        memcpy(szDataOut,szDataIn,len);
        oBlowFish.Decrypt((unsigned char*)szDataOut,
                          (unsigned char*)szDataOut, num, CBlowFish::CBC);
    }
}

//void BlowFishEncrypt(unsigned char *szDataIn,unsigned char *szDataOut, int len) {
//    CBlowFish oBlowFish((unsigned char*)bundleVer, 118);
//    oBlowFish.ResetChain();
//    int num = ((len % 8) != 0) ? ((len + 8) - (len % 8)) : len;
//    if(num==len)
//        oBlowFish.Encrypt((unsigned char*)szDataIn,
//                          (unsigned char*)szDataOut, num, CBlowFish::CBC);
//    else {
//        memcpy(szDataOut,szDataIn,len);
//        oBlowFish.Encrypt((unsigned char*)szDataOut,
//                          (unsigned char*)szDataOut, num, CBlowFish::CBC);
//    }
//    cout<<"Encrypted"<<endl;
//}
int DecryptFile(char *filename) {
    char *FileIn=NULL;
    FileIn=filename;
    char FileOut[100]=("");
    memset(FileOut, 0, 100);
    strcat(FileOut,filename);
    strcat(FileOut,".bundle");
    FILE *fp=NULL;
    fp=fopen(FileIn,"rb");
    if(fp==NULL)
        throw std::logic_error("Read File Error.\n");
    char *buf;
    fseek(fp,0,SEEK_END);
    int len=ftell(fp);
    buf= (char *)malloc(len);
    rewind(fp);
    fread(buf,1,len,fp);
    if(NULL==fp) return -1;
    fclose(fp);
    int num=len;
//    cout<<"num="<<num<<endl;
    int num2 = 8;
//    cout<<"num2="<<num2<<endl;
    int num3 = buf[num-9];
//    cout<<"num3="<<num3<<endl;
    num2 += 1 + num3;
//    cout<<"num2="<<num2<<endl;
    num -= num2;
//    cout<<"num="<<num<<endl;
//Test CBC
    char *szDataOut;
    szDataOut= (char *)malloc(len);
//          if((strcmp(argv[1],"-d")==0))
    BlowFishDecrypt((unsigned char*)buf,(unsigned char*)szDataOut, num);
    cout<<filename<<" Decrypted"<<endl;
//        if((strcmp(argv[1],"-e")==0))
//            BlowFishEncrypt((unsigned char*)buf,(unsigned char*)szDataOut, num);
    FILE *pFile = fopen(FileOut,"wb");
    if(pFile==NULL)
        throw std::logic_error("Write File Error.\n");
    fwrite(szDataOut,1,len,pFile);
    fclose(pFile);
    free(szDataOut);
    free(buf);
    return 0;
}
int main(int argc, char *argv[]) {
    try {
//        if(argc!=4||(strcmp(argv[1],"-d")||strcmp(argv[1],"-e")==0))
//           throw std::logic_error("Error argument supplied.\n");
        for(int i=1; i<argc; i++) {
            DecryptFile(argv[i]);
        }

    } catch(exception& roException) {
        cout << "Exception: "
             << roException.what() << endl;
    }
}

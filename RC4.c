#include "RC4.h"

table256 S;
table256 S_de;
tableNormal K;
table256 T;

void K_table_init(){
    printf("The length of K:");
    scanf("%d",&K.len);
    K.table=(uint8_t *)malloc(K.len*sizeof(uint8_t));
    printf("Please input the K:");
    for(uint32_t i=0;i<K.len;i++){
        char c;
        scanf("%c",&c);
        if(c=='\n'){
            i--;
            continue;
        }
        K.table[i]=c;
    }
}
void K_table_init_by_file(){
    FILE *seed_file;
    seed_file=fopen("seed","rb");
    if(!seed_file){
        perror("Error opening seed file");
        exit(1);
    }
    // read seed file's first line as K.len
    fscanf(seed_file,"%d",&K.len);
    K.table=(uint8_t *)malloc(K.len*sizeof(uint8_t));
    // read seed file's second line as K.table
    for(uint32_t i=0;i<K.len;i++){
        char c;
        fscanf(seed_file,"%c",&c);
        if(c=='\n'){
            i--;
            continue;
        }
        K.table[i]=c;
    }
}
void copy_table(table256 *a,table256 *b){
    memcpy(a->table,b->table,tb_length);
}

void S_table_init(){
    for(int16_t i=0;i<tb_length;i++){
        S.table[i]=i;
    }
}
void T_table_init(){
    for(int16_t i=0;i<tb_length;i++){
        T.table[i]=K.table[i%K.len];
    }
}
void swap(uint8_t *a,uint8_t *b){
    uint8_t temp=*a;
    *a=*b;
    *b=temp;
}
void KSA(){
    int16_t j=0;
    for(int16_t i=0;i<tb_length;i++){
        j=(j+S.table[i]+T.table[i])%tb_length;
        swap(&S.table[i],&S.table[j]);
    }
}
void PRGA(uint8_t *pi,uint8_t *pj,uint8_t *pk,uint16_t plaintext_length,uint8_t *plaintext,uint8_t *ciphertext){
    uint8_t i=*pi,j=*pj,k=*pk;
    for(uint16_t m=0;m<plaintext_length;m++){
        i=(i+1)%tb_length;
        j=(j+S.table[i])%tb_length;
        swap(&S.table[i],&S.table[j]);
        uint8_t t=(S.table[i]+S.table[j])%tb_length;
        // printf("%02hhx %02hhx\n",plaintext[m],S.table[t]);
        ciphertext[m]=plaintext[m]^S.table[t];
    }
    *pi=i,*pj=j,*pk=k;
}
// decode
void PRGA_decode(uint8_t *pi,uint8_t *pj,uint8_t *pk,uint16_t plaintext_length,uint8_t *ciphertext,uint8_t *plaintext){
    uint8_t i=*pi,j=*pj,k=*pk;
    for(uint16_t m=0;m<plaintext_length;m++){
        i=(i+1)%tb_length;
        j=(j+S_de.table[i])%tb_length;
        swap(&S_de.table[i],&S_de.table[j]);
        uint8_t t=(S_de.table[i]+S_de.table[j])%tb_length;
        // printf("%02hhx %02hhx\n",plaintext[m],S_de.table[t]);
        plaintext[m]=ciphertext[m]^S_de.table[t];
    }
    *pi=i,*pj=j,*pk=k;
}
void init_all_in_one(){
    puts("S initing...");
    S_table_init();
    puts("S inited");
    puts("S table:");
    printBytes((uint8_t *)&S,sizeof(S));

    puts("K initing...");
    K_table_init_by_file();
    puts("K inited");
    printf("K.length: %d\n",K.len);
    puts("K table:");
    printBytes((uint8_t *)K.table,K.len);

    puts("T initing...");
    T_table_init();
    puts("T inited");
    puts("T table:");
    printBytes((uint8_t *)&T,sizeof(T));

    puts("KSAing...");
    KSA();
    puts("S table after KSA:");
    printBytes((uint8_t *)&S,sizeof(S));

    copy_table(&S_de,&S);
}
int main(){
    init_all_in_one();
    puts("--------------------");

    uint8_t i=0,j=0,k=0;
    uint8_t *pi=&i,*pj=&j,*pk=&k;

    FILE *sourceFile, *encodedFile, *decodeFile;
    const size_t bufferSize = 256;  // e.g., read in chunks of 256 Bytes
    uint8_t buffer[bufferSize];
    uint8_t buffer2[bufferSize];
    size_t bytesRead;

    // clean the encoded file
    encodedFile = fopen("encode", "wb");
    if (!encodedFile) {
        perror("Error opening encoded file");
        fclose(sourceFile);
        return 1;
    }
    fclose(encodedFile);
    // clean the decoded file
    decodeFile = fopen("decode", "wb");
    if (!decodeFile) {
        perror("Error opening decode file");
        return 1;
    }
    fclose(decodeFile);

    sourceFile = fopen("plaintext", "rb");
    if (!sourceFile) {
        perror("Error opening source file");
        return 1;
    }


    encodedFile = fopen("encode", "ab");
    if (!encodedFile) {
        perror("Error opening encoded file");
        fclose(sourceFile);
        return 1;
    }
    // encode
    puts("begin encoding");
    while ((bytesRead = fread(buffer, 1, bufferSize, sourceFile)) > 0) {
        PRGA(pi,pj,pk,bytesRead,buffer,buffer2);
        fwrite(buffer2, 1, bytesRead, encodedFile);
    }

    fclose(sourceFile);
    fclose(encodedFile);

    puts("encoded finished");
    puts("--------------------");



    encodedFile = fopen("encode", "rb");
    decodeFile = fopen("decode", "wb");
    if (!sourceFile) {
        perror("Error opening source file");
        return 1;
    }
    if (!decodeFile) {
        perror("Error opening decode file");
        fclose(encodedFile);
        return 1;
    }
    // decode
    puts("begin decoding");
    *pi=0,*pj=0,*pk=0;
    while ((bytesRead = fread(buffer, 1, bufferSize, encodedFile)) > 0) {
        PRGA_decode(pi,pj,pk,bytesRead,buffer,buffer2);
        fwrite(buffer2, 1, bytesRead, decodeFile);
    }

    puts("decode finished");
    puts("--------------------");
    return 0;
}
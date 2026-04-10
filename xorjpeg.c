#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void xor_cipher(unsigned char *data, int data_len, const char *key) {
    int key_len = strlen(key);

    for (int i = 0; i < data_len; i++) {
        // XOR the data byte with a key byte
        // The % operator ensures we cycle through the key characters
        data[i] = data[i] ^ key[i % key_len];
    }
}

int main() {
    // 1. Our raw data
    // Raw data is the binary of JPEG
    FILE* fin = fopen("pizza.jpg", "rb");
    fseek(fin, 0, SEEK_END); //End of file
    int buff = ftell(fin); //Gets position at end of file for buffer size
    rewind(fin); //Back to beginning of file

    char *imageData = (unsigned char *)malloc(buff);

    fread(imageData, 1, buff, fin);

    fclose(fin);
   
    // 2. Our ASCII key
    const char my_key[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x21}; // "Hello!"

    printf("Original: %s\n", imageData);

    // 3. Encrypt
    xor_cipher(imageData, buff, my_key);
    printf("Encrypted/Obfuscated data created.\n");

    FILE *fout = fopen("encrypted.jpg", "wb"); //Save encrypted image to encrypted.jpg for testing

    fwrite(imageData, 1, buff, fout);
    fclose(fout);

    

    // 4. Decrypt (Running the same function again restores the data)
    xor_cipher(imageData, buff, my_key);
    printf("Decrypted: %s\n", imageData);

    FILE *fout1 = fopen("decrypted.jpg", "wb"); //Save decrypted image to decrypted.jpg to ensure same result as beginning

    fwrite(imageData, 1, buff, fout1);
    fclose(fout1);

    free(imageData);

    return 0;
}
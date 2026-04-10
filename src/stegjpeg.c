#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STBI_NO_LINEAR
#define STBI_NO_HDR
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <Python.h>


void encrypt(char jpg[], char msg[], char png[]){
    int width, height, channels;
    
    // stbi_load takes the filename, pointers to variables to store the image info, 
    // and the number of channels you want (3 = RGB).
    unsigned char *image = stbi_load(jpg, &width, &height, &channels, 3);

    if (image == NULL) {
        printf("Failed to load image.\n");
        exit(0);
    }

    printf("Image loaded! Width: %d, Height: %d\n", width, height);


    unsigned char* byte_msg = (unsigned char*)msg;
    
    int image_index = 0;

    int msglen = strlen(msg);
    // Iterate through each character in the message
    for (int i = 0; i <= msglen; i++) {
        
        unsigned char current_char = byte_msg[i];
        
        // Iterate 8 times to grab the 8 bits of the current character
        for (int bit_index = 0; bit_index < 8; bit_index++) {
            
            // Extract the current bit from the character using shift and mask
            int bit = (current_char >> bit_index) & 1;
            
            // Clear the LSB to 0
            image[image_index] = image[image_index] & 0xFE;
            
            // Insert the message bit into the cleared image byte
            image[image_index] = image[image_index] | bit;
            
            // Move our image tracker to the next pixel byte for the next loop
            image_index++;
        }
    }
    
    printf("Saving encrypted image to %s...\n", png);
    int stride = width * 3; // 3 channels (RGB) per pixel
    stbi_write_png(png, width, height, 3, image, stride);

    // free the memory
    stbi_image_free(image);

}

char* decrypt(char png[]){
    int width, height, channels;
    
    // stbi_load takes the filename, pointers to variables to store the image info, 
    // and the number of channels you want (3 = RGB).
    unsigned char *image = stbi_load(png, &width, &height, &channels, 3);

    if (image == NULL) {
        printf("Failed to load image.\n");
        exit(0);
    }


    int total_bytes = width * height * 3;
    int max_chars = total_bytes / 8;

    char *extracted = (char *)malloc(max_chars + 1);
    int msg_index = 0;

    unsigned char current_char = 0; //empty byte of 8 zeros

    int bit_index = 0;

    for (int i = 0; i < total_bytes; i++) {
    
        // Extract the LSB from the current pixel
        int bit = image[i] & 1;
        
        // Shift the bit to its original position and merge it into current character
        current_char = current_char | (bit << bit_index);
        
        // Move to the next bit position
        bit_index++;

        if (bit_index == 8) {

            if (current_char == '\0') {
                extracted[msg_index] = current_char;
                break; 
            }
            

            extracted[msg_index] = current_char;
            msg_index++;
            
            current_char = 0;
            bit_index = 0;
        }
    }

    // free the memory
    stbi_image_free(image);

    return extracted;

}

// Python wrapping
static PyObject* py_encrypt(PyObject* self, PyObject* args) {
    char *jpg_file;
    char *message;
    char *png_out;
 
    // "sss" means three Strings
    if (!PyArg_ParseTuple(args, "sss", &jpg_file, &message, &png_out)) {
        return NULL;
    }

    encrypt(jpg_file, message, png_out);

    Py_RETURN_NONE;
}

static PyObject* py_decrypt(PyObject* self, PyObject* args) {
    char *png;

    if (!PyArg_ParseTuple(args, "s", &png)) {
        return NULL;
    }

    char *result = decrypt(png);

    if (result == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to load image for decryption.");
        return NULL;
    }

    // Translate into a Python string
    PyObject *py_result = PyUnicode_FromString(result);

    free(result);

    return py_result;
}

static PyMethodDef StegMethods[] = {

    {"encrypt", py_encrypt, METH_VARARGS, "Encrypt a message into a png given a jpeg file"},
    {"decrypt", py_decrypt, METH_VARARGS, "Decrypt a message from a png"},
    
    {NULL, NULL, 0, NULL} // This empty row tells Python the list is finished
};

static struct PyModuleDef stegmodule = {
    // The name of the module in Python
    PyModuleDef_HEAD_INIT, "stegjpeg", NULL, -1, StegMethods
};

PyMODINIT_FUNC PyInit_stegjpeg(void) {
    return PyModule_Create(&stegmodule);
}

int main() {

    char msg[] = "HelloWorld";
    char fin[] = "pizza.jpg";
    char fout[] = "steg.png";

    encrypt(fin, msg, fout);
    decrypt(fout);

    return 0;
}
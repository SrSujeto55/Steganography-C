#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>


/*
    Function to get the number of bytes left in the string
    ptr: pointer to the string
    index: index of the string pointer
    returns: number of bytes left
*/
int bytesLeft(unsigned char *ptr, int index) {
    int remainingBytes = strlen((char *)ptr) - index;
    return remainingBytes;
}

/*
    Function to convert 4 bytes to an integer by shifting and ORing
    ptr: pointer to the string
    index: index of the current string pointer
    returns: integer representation of the 4 bytes
*/
int bytesToInt(unsigned char *ptr, int index) {
    int num = 0;
    int leftBytes = 4; // default 4 bytes

    // If the number of bytes left is less than 4, then we need to shift and OR the bytes accordingly
    // We left the leftmost bytes as 0x00
    if(bytesLeft(ptr, index) < 4)
        leftBytes = bytesLeft(ptr, index);
    
    for(int i = 0; i < leftBytes; i++) {
        num = num << 8;
        num = num | ptr[index + i];
    }

    return num;
}

/*
    Function to convert an integer to 4 bytes
    num: integer to convert
    bytesToPrint: number of bytes to print
    returns: array of bytes (pointer to the firts byte)
*/
unsigned char *intToBytes(int num, int *bytesToPrint) {
    unsigned char *bytes = (unsigned char *)malloc(4 * sizeof(unsigned char)); // 4 bytes
    
    for(int i = 3; i >= 0; i--) {
        bytes[i] = num & 0xFF;

        // If the byte is 0, then this means we're in the final 4 byte and can be any number of bytes from 1 to 4
        // we invert the order of the bytes and return the correct number of bytes
        if(bytes[i] == 0) {
            unsigned char *newBytes = (unsigned char *)malloc((*bytesToPrint) * sizeof(unsigned char));
            i++;
            for(int j = 0; j < *bytesToPrint; j++) {
                newBytes[j] = bytes[i];
                i++;
            }
            return newBytes;
        }
        *bytesToPrint = *bytesToPrint + 1;
        num = num >> 8;
    }
    return bytes;
}

// Main function
int main() {
    char str[100];
    printf("Enter a string: ");
    fflush(stdout);
    scanf("%[^\n]s", str); // Read the string with spaces until a newline character is found
    unsigned char *ptr = (unsigned char *)str; // Pointer to the string (bytes representation)

    int length = (int)ceil(strlen(str) / 4.0); // Rounded up the number of bytes needed to represent the string

    int *numberRep = (int *)malloc(length * sizeof(int));
    
    printf("Byte Representation: ");
    for(int i = 0; i < strlen(str); i++) {
        printf("%02X ", ptr[i]);
    }

    printf("\nInteger Representation: ");
    // Get the integer representation of the string
    for(int i = 0; i < length; i++) {
        numberRep[i] = bytesToInt(ptr, i*4);
        printf("%d ", numberRep[i]);
    }

    printf("\nString Representation (Rebuild): ");
    int bytesToPrint;
    for(int i = 0; i < length; i++) {
        bytesToPrint = 0;
        unsigned char *bytes = intToBytes(numberRep[i], &bytesToPrint);
        for(int j = 0; j < bytesToPrint; j++) {
            printf("%c", bytes[j], bytes[j]);
        }
        free(bytes);
    }
    free(numberRep);
    return 0;
}
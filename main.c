#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <uchar.h>
#include <wchar.h>

char username[800];
char password[800];


void getinputs () {
    printf("\nPlease enter the username: \n");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("\nPlease enter the password: \n");
    fgets(password, sizeof(password), stdin);
    password[strcspn(username, "\n")] = '\0';


    printf(username);
    printf(password);
    printf("-----------------\n");
}


int key[] = {5, -14, 31, -9, 3};
int key_length = 5;
char decoded_data[1600];

int decode_file(char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error: File not found\n");
        return 1;
    }
        int byte;
        int i = 0;
        int j = 0;
    while ((byte = fgetc(file)) != EOF) {
        if (byte != 0x0A) {
            byte -= key[i];
            i = (i + 1) % key_length;
            wchar_t asd;
asd = (char) byte;
            printf("BYTE: %d\n", byte);
            printf("CHAR: %c\n", (char) byte);
        } else {
            i = 0;
        }
        decoded_data[j++] = byte;

    }
    fclose(file);
    return 1;
}
#define MAX_LINES 100
#define MAX_LINE_LENGTH 800
char lines[MAX_LINES][MAX_LINE_LENGTH];

void split_username_password() {
    int line_count = 0;
    char *line = strtok(decoded_data, "\n");

    while (line != NULL && line_count < MAX_LINES) {
        strncpy(lines[line_count], line, MAX_LINE_LENGTH - 1);
        lines[line_count][MAX_LINE_LENGTH - 1] = '\0';
        line = strtok(NULL, "\n");
        line_count++;
    }

    for (int i = 0; i <= line_count; i++) {
        char *test = strpbrk(lines[i], "*@");
        if (test != NULL) {
            *test = '\0';
            char *user = lines[i];
            char *pass = test + 1;
            if ((strcoll(password, pass) == 0) && (strcoll(username, user) == 0)) {
                printf("Login successful\n");
                break;
            }
        }
    }

}


void username_to_bytes(const char *username, unsigned char *byte_array) {
    size_t length = strlen(username);
    for (size_t i = 0; i < length; i++) {
        byte_array[i] = (unsigned char)username[i];
    }
    byte_array[length] = '\0';  // Null-terminate the byte array
}



int main() {
    setlocale(LC_ALL, "hu_HU.UTF-8");
    decode_file("D:/Work/untitled/jelszo.bin");
    //getinputs();
    unsigned char byte_array[800];
    username_to_bytes(username, byte_array);
    printf("----------------------\n");
    for (size_t i = 0; i < strlen((char *)byte_array); i++) {
        printf("%d ", byte_array[i]);
        printf("CHAR: %c\n", (char) byte_array[i]);
    }
    printf("\n");

    //split_username_password();
    return 0;
}
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


#define MAX_LINES 100
#define MAX_LINE_LENGTH 800
#define MAX_ENTRIES 1600
#define PAGE_SIZE 10

char lines[MAX_LINES][MAX_LINE_LENGTH];
char username[800];
char password[800];
int key[] = {5, -14, 31, -9, 3};
int key_length = 5;
char users[1600];

typedef struct {
    char name[50];
    char phone[50];
} PhonebookEntry;

char phonebook_data[MAX_ENTRIES];

PhonebookEntry phonebook[800];


void getinputs () {
    printf("\nPlease enter the username: \n");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("\nPlease enter the password: \n");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';
}

void decode_file(char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error: File not found\n");
        return;
    }
        int isuser = (strcmp(filename, "D:/Work/untitled/telefonkonyv.bin") == 0) ? 0 : 1;
        int byte;
        int i = 0;
        int j = 0;
    while ((byte = fgetc(file)) != EOF) {
        if (byte != 0x0A) {
            byte -= key[i];
            i = (i + 1) % key_length;
            isuser == 1 ? (users[j++] = (char)byte) : (phonebook_data[j++] = (char)byte);
        } else {
            i = 0;
            isuser == 1 ? (users[j++] = '\n') : (phonebook_data[j++] = '\n');
        }
    }
    isuser == 1 ? (users[j++] = '\0') : (phonebook_data[j++] = '\0');
    fclose(file);
}

bool split_username_password() {
    int line_count = 0;
    char *line = strtok(users, "\n");

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
            if ((strcmp(password, pass) == 0) && (strcmp(username, user) == 0)) {
                printf("Login successful\n");
                return true;
            }
        }
    }
    printf("Login failed\n");
    return false;
}

void split_phonebook() {
    char *line = strtok(phonebook_data, "\n");
    int entry_count = 0;
    while (line != NULL) {
        char *separator = strchr(line, ';');
        if ((separator != NULL) && (entry_count < MAX_ENTRIES)) {
            *separator = '\0';
            strncpy(phonebook[entry_count].name, line, sizeof(phonebook[entry_count].name) - 1);
            strncpy(phonebook[entry_count].phone, separator + 1, sizeof(phonebook[entry_count].phone) - 1);
            phonebook[entry_count].name[sizeof(phonebook[entry_count].name) - 1] = '\0';
            phonebook[entry_count].phone[sizeof(phonebook[entry_count].phone) - 1] = '\0';
            entry_count++;
        }
        line = strtok(NULL, "\n");
    }
}

void draw_rectangle(int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                printf("#");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void display_phonebook_page(int page) {
    int start_index = page * PAGE_SIZE;
    int end_index = start_index + PAGE_SIZE;
    if (end_index > MAX_ENTRIES) {
        end_index = MAX_ENTRIES;
    }

    draw_rectangle(60, 15);
    printf("\nTelefonkönyv - %d. oldal\n", page + 1);
    for (int i = start_index; i < end_index && phonebook[i].name[0] != '\0'; i++) {
        printf("%d. %s - %s\n", i + 1, phonebook[i].name, phonebook[i].phone);
    }
    printf("\nNavigálj a következő oldalakhoz: [n] Következő, [p] Előző, [q] Kilépés\n");
}


int main() {
    setlocale(LC_ALL, "hu_HU.UTF-8");
    getinputs();
    decode_file("D:/Work/untitled/jelszo_javitott.bin");
    if(split_username_password()) {
        decode_file("D:/Work/untitled/telefonkonyv.bin");
        split_phonebook();
        display_phonebook_page(2);
        sleep(120);
    }


    /*
    for (int i = 0; i < 800 && phonebook[i].name[0] != '\0'; i++) {
        printf("%s - %s\n", phonebook[i].name, phonebook[i].phone);
    } */
    return 0;
}
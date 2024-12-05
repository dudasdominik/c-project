#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <conio.h>


#define MAX_LINES 100
#define MAX_LINE_LENGTH 800
#define MAX_ENTRIES 1600
#define PAGE_SIZE 10

char lines[MAX_LINES][MAX_LINE_LENGTH];
char username[25];
char password[25];
int key[] = {5, -14, 31, -9, 3};
int key_length = 5;

typedef struct {
    char name[50];
    char data[50];
} Entry;
char users_data[1600];
Entry users[800];
char phonebook_data[MAX_ENTRIES];
Entry phonebook[800];


void get_inputs () {
    printf("\nPlease enter the username: \n");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("\nPlease enter the password: \n");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';
}

void decode_file(char *filename, char *decoded) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error: File not found\n");
        return;
    }
        int byte;
        int i = 0;
        int j = 0;
    while ((byte = fgetc(file)) != EOF) {
        if (byte != 0x0A) {
            byte -= key[i];
            i = (i + 1) % key_length;
            decoded[j++] = byte;
        } else {
            i = 0;
            decoded[j++] = '\n';
        }
    }
    decoded[j] = '\0';
    fclose(file);
}


void split_data(char *data, Entry *entries, int max_entries) {
    char *line = strtok(data, "\n");
    int entry_count = 0;
    while (line != NULL) {
        char *separator = strpbrk(line, ";*");
        if ((separator != NULL) && (entry_count < max_entries)) {
            *separator = '\0';
            strncpy(entries[entry_count].name, line, sizeof(entries[entry_count].name) - 1);
            strncpy(entries[entry_count].data, separator + 1, sizeof(entries[entry_count].data) - 1);
            entries[entry_count].name[sizeof(entries[entry_count].name) - 1] = '\0';
            entries[entry_count].data[sizeof(entries[entry_count].data) - 1] = '\0';
            entry_count++;
        }
        line = strtok(NULL, "\n");
    }
}


bool isLoggedIn() {
    split_data(users_data, users, 800);
    for (int i = 0; i < 800; i++) {
        if (strcmp(users[i].name, username) == 0 && strcmp(users[i].data, password) == 0) {
            return true;
        }
    }
    return false;
}

void draw_rectangle_with_content(int width, int height, int page) {
    int start_index = page * PAGE_SIZE;
    int end_index = start_index + PAGE_SIZE;
    if (end_index > MAX_ENTRIES) {
        end_index = MAX_ENTRIES;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                printf("#");
            } else if (i > 1 && i < height - 1 && j == 2 && (i - 2) < PAGE_SIZE && (start_index + (i - 2)) < end_index) {
                int entry_index = start_index + (i - 2);
                if (phonebook[entry_index].name[0] != '\0') {
                    printf("%s - %s", phonebook[entry_index].name, phonebook[entry_index].data);
                    j += strlen(phonebook[entry_index].name) + strlen(phonebook[entry_index].data) + 2;
                }
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void display_phonebook_page_with_selection(int page, int selected_index) {
    int start_index = page * PAGE_SIZE;
    int end_index = start_index + PAGE_SIZE;
    if (end_index > MAX_ENTRIES) {
        end_index = MAX_ENTRIES;
    }

    printf("Telefonkönyv - %d. oldal\n", page + 1);
    for (int i = start_index; i < end_index && phonebook[i].name[0] != '\0'; i++) {
        if (i == selected_index) {
            printf("> %s - %s <\n", phonebook[i].name, phonebook[i].data);
        } else {
            printf("  %s - %s\n", phonebook[i].name, phonebook[i].data);
        }
    }
    printf("\nNavigálj a talalatok kozott: w (Fel), s (Le), v (Menu Valtas)\n");
}



void display_phonebook_page(int page) {
    draw_rectangle_with_content(45, 13, page);
    printf("\nJelenlegi oldal: %d\n", page + 1);
    printf("\nNavigacio: [n] Kovetkezo, [p] Elozo, [s] Kereses, [m] Menu Valtas , [q] Kilepes\n");
}

void search_phonebook() {
    char input[50];
    printf("Add meg a keresett nevet vagy telefonszamot: ");
    scanf("%49s", input);
    getchar();

    bool found = false;
    for (int i = 0; i < MAX_ENTRIES && phonebook[i].name[0] != '\0'; i++) {
        if ((strcmp(phonebook[i].name, input) == 0) || (strcmp(phonebook[i].data, input) == 0)) {
            printf("Talalat: %s - %s\n", phonebook[i].name, phonebook[i].data);
            printf("\n Navigalj vissza a telefonkonyvhez: [b] Back\n");
            found = true;
            }
    }
    if (!found) {
        printf("Nincs talalat.\n");
    }
}

void display_page(int page, bool is_selection, int selected_index) {
    system("cls");
    if (is_selection) {
        display_phonebook_page_with_selection(page, selected_index);
    } else {
        display_phonebook_page(page);
    }
}

void navigation_menu() {
    split_data(phonebook_data, phonebook, MAX_ENTRIES);
    int current_page = 0;
    int selected_index = 0;
    char choice;
    bool is_Selection = false;
    system("cls");
    display_phonebook_page(current_page);
    while (true) {
        if(is_Selection) {
            switch (getch()) {
                case 'w':
                    if (selected_index > current_page * PAGE_SIZE) {
                        system("cls");
                        selected_index--;
                        display_phonebook_page_with_selection(current_page, selected_index);
                    } else if (current_page > 0) {
                        system("cls");
                        current_page--;
                        display_phonebook_page_with_selection(current_page, selected_index);
                        selected_index = (current_page + 1) * PAGE_SIZE - 1;
                    }
                break;
                case 's':
                    if (selected_index < (current_page + 1) * PAGE_SIZE - 1 && phonebook[selected_index + 1].name[0] != '\0') {
                        system("cls");
                        selected_index++;
                        display_phonebook_page_with_selection(current_page, selected_index);
                    } else if ((current_page + 1) * PAGE_SIZE < MAX_ENTRIES && phonebook[(current_page + 1) * PAGE_SIZE].name[0] != '\0') {
                        system("cls");
                        current_page++;
                        display_phonebook_page_with_selection(current_page, selected_index);
                        selected_index = current_page * PAGE_SIZE;
                    }
                break;
                case 'v':
                    system("cls");
                current_page = 0;
                display_phonebook_page(current_page);
                is_Selection = false;
                break;
            }
        }
        else {
            choice = getchar();
            getchar();
            switch (choice) {
                case 'n':
                    if (current_page < 5) {
                        current_page++;
                        display_page(current_page, is_Selection, selected_index);
                    }
                break;
                case 'p':
                    if (current_page > 0) {
                        current_page--;
                        display_page(current_page, is_Selection, selected_index);
                    }
                break;
                case 'q':
                    return;
                case 's':
                    search_phonebook();
                break;
                case 'm':
                    is_Selection = true;
                    current_page = 0;
                    display_page(current_page, is_Selection, selected_index);
                break;
                case 'b':
                    is_Selection = false;
                display_page(current_page, is_Selection, selected_index);
                break;
                default:
                    printf("Nem valid, probald meg ujra\n");
                break;
            }
        }

    }
}


int main() {
    get_inputs();
    decode_file("D:/Work/untitled/jelszo_javitott.bin",  users_data);
    if(isLoggedIn()) {
        decode_file("D:/Work/untitled/telefonkonyv.bin", phonebook_data);
        navigation_menu();
    }
    return 0;
}
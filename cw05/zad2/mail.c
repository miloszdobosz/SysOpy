#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void print_mail(int sort_by) {
    FILE* mail;

    if (sort_by == 0) {
        mail = popen("mail | sort -k3d -", "w");
    } else {
        mail = popen("mail |", "w");
    }

    pclose(mail);
}

void write_mail(const char* email, const char* subject, const char* message) {
    FILE* mail;
    char command_buffer[256];

    sprintf(command_buffer, "mail -s %s %s", subject, email);
    mail = popen(command_buffer, "w");

    fputs(message, mail);
    pclose(mail);
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("Zbyt malo argumentow!\n");
        return 1;
    }

    if (argc < 4) {
        int sorting_key;

        if (strcmp(argv[1], "nadawca") == 0) {
            sorting_key = 0;
        } else if (strcmp(argv[1], "data") == 0) {
            sorting_key = 1;
        } else {
            printf("Nieprawidlowy klucz!\n");
            return 2;
        }

        print_mail(sorting_key);

    } else {
        char* email = argv[1];
        char* subject = argv[2];
        char* message = argv[3];

        write_mail(email, subject, message);
    }

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100
#define MAX_LENGTH 50

typedef struct
{
    char platform[MAX_LENGTH];
    char userID[MAX_LENGTH];
    char password[MAX_LENGTH];
    char notes[MAX_LENGTH];
} Account;

void encryptFile(FILE *file, const char *password)
{
    char ch;
    int i = 0;
    while ((ch = fgetc(file)) != EOF)
    {
        ch = ch ^ password[i % strlen(password)];
        fputc(ch, file);
        i++;
    }
}

void addAccount(Account accounts[], int *count)
{
    if (*count >= MAX_ACCOUNTS)
    {
        printf("Maximum number of accounts reached.\n");
        return;
    }

    Account newAccount;

    printf("Enter platform name: ");
    scanf("%s", newAccount.platform);
    printf("Enter user ID: ");
    scanf("%s", newAccount.userID);
    printf("Enter password: ");
    scanf("%s", newAccount.password);
    printf("Enter notes: ");
    scanf("%s", newAccount.notes);

    accounts[*count] = newAccount;
    (*count)++;
}

void saveAccounts(Account accounts[], int count, const char *filename, const char *adminPassword)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    fprintf(file, "%s\n", adminPassword);
    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%s %s %s %s\n", accounts[i].platform, accounts[i].userID, accounts[i].password, accounts[i].notes);
    }

    encryptFile(file, adminPassword);
    fclose(file);

    printf("Accounts saved successfully.\n");
}

void showAccounts(const char *filename, const char *adminPassword)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    char password[MAX_LENGTH];
    fscanf(file, "%s", password);

    if (strcmp(password, adminPassword) != 0)
    {
        printf("Incorrect admin password.\n");
        fclose(file);
        return;
    }

    Account accounts[MAX_ACCOUNTS];
    int count = 0;

    while (fscanf(file, "%s %s %s %s", accounts[count].platform, accounts[count].userID, accounts[count].password, accounts[count].notes) != EOF)
    {
        count++;
    }

    fclose(file);

    printf("Index\tPlatform\tUser ID\tPassword\tNotes\n");

    for (int i = 0; i < count; i++)
    {
        printf("%d\t%s\t%s\t%s\t%s\n", i + 1, accounts[i].platform, accounts[i].userID, accounts[i].password, accounts[i].notes);
    }
}

void createEncryptedFile(const char *filename, const char *adminPassword)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error creating file.\n");
        return;
    }

    fprintf(file, "%s", adminPassword);
    encryptFile(file, adminPassword);
    fclose(file);

    printf("Encrypted file created successfully.\n");
}

int main()
{
    Account accounts[MAX_ACCOUNTS];
    int count = 0;
    char filename[MAX_LENGTH] = "pass.txt";
    char adminPassword[MAX_LENGTH];

    // Check if pass.txt exists, if not, create and encrypt it
    FILE *fileCheck = fopen(filename, "r");
    if (fileCheck == NULL)
    {
        fclose(fileCheck); // Close the file if opened
        printf("pass.txt not found. Creating...\n");
        printf("Enter admin password for encryption: ");
        scanf("%s", adminPassword);
        createEncryptedFile(filename, adminPassword);
    }
    else
    {
        fclose(fileCheck);
        printf("Enter admin password: ");
        scanf("%s", adminPassword);
    }

    while (1)
    {
        printf("\n1. Add account\n2. Save accounts\n3. Show saved accounts\n4. Exit\n");
        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addAccount(accounts, &count);
            break;
        case 2:
            saveAccounts(accounts, count, filename, adminPassword);
            break;
        case 3:
            showAccounts(filename, adminPassword);
            break;
        case 4:
            printf("Exiting program.\n");
            exit(0);
        default:
            printf("Invalid choice.\n");
        }
    }

    return 0;
}

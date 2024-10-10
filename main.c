#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

void generate_fake_isbn(int type);
void generate_real_isbn(int type);
int validate_isbn10(const char isbn[]);
int validate_isbn13(const char isbn[]);
int calculate_isbn10_checksum(const char isbn[]);
int calculate_isbn13_checksum(const char isbn[]);

int main() {
    int choice, type;

    // Seed the random number generator
    srand(time(NULL));

    do {
        // Menu for user options
        printf("\nSelect an option:\n");
        printf("1. Validate ISBN\n");
        printf("2. Generate Fake ISBN\n");
        printf("3. Generate Real ISBN\n");
        printf("4. Exit\n");
        printf("Enter your choice (1, 2, 3, or 4): ");

        // Check for valid input for menu choice
        while (scanf("%d", &choice) != 1 || choice < 1 || choice > 4) {
            printf("Invalid choice! Please enter a number between 1 and 4: ");
            while (getchar() != '\n'); // Clear the input buffer
        }

        switch (choice) {
            case 1: {
                char isbn[20];
                printf("Enter the ISBN to validate (10 or 13 digits, with or without hyphens): ");
                scanf("%s", isbn);

                // Clean ISBN input by removing hyphens
                char cleaned_isbn[20];
                int j = 0;
                for (int i = 0; isbn[i] != '\0'; i++) {
                    if (isbn[i] != '-') {
                        cleaned_isbn[j++] = isbn[i];
                    }
                }
                cleaned_isbn[j] = '\0'; // Null-terminate the cleaned ISBN
                int length = strlen(cleaned_isbn);

                if (length == 10) {
                    if (validate_isbn10(cleaned_isbn)) {
                        printf("The ISBN-10 code is valid.\n");
                    } else {
                        printf("The ISBN-10 code is invalid.\n");
                    }
                } else if (length == 13) {
                    if (validate_isbn13(cleaned_isbn)) {
                        printf("The ISBN-13 code is valid.\n");
                    } else {
                        printf("The ISBN-13 code is invalid.\n");
                    }
                } else {
                    printf("Invalid input. ISBN should be 10 or 13 digits long (excluding hyphens).\n");
                }
                break;
            }
            case 2: {
                printf("Choose ISBN type:\n1. ISBN-10\n2. ISBN-13\nEnter your choice (1 or 2): ");
                scanf("%d", &type);
                if (type == 1 || type == 2) {
                    generate_fake_isbn(type);
                } else {
                    printf("Invalid choice. Please enter 1 for ISBN-10 or 2 for ISBN-13.\n");
                }
                break;
            }
            case 3: {
                printf("Choose ISBN type:\n1. ISBN-10\n2. ISBN-13\nEnter your choice (1 or 2): ");
                scanf("%d", &type);
                if (type == 1 || type == 2) {
                    generate_real_isbn(type);
                } else {
                    printf("Invalid choice. Please enter 1 for ISBN-10 or 2 for ISBN-13.\n");
                }
                break;
            }
            case 4:
                printf("Exiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice! Please select either 1, 2, 3, or 4.\n");
                break;
        }
    } while (choice != 4);

    return 0;
}

// Function to generate a fake ISBN
void generate_fake_isbn(int type) {
    char isbn[14]; // Buffer for ISBN-13, ISBN-10 is handled by the first 10 characters
    int i;

    if (type == 1) { // Generate ISBN-10
        // Generate the first 9 digits randomly
        for (i = 0; i < 9; i++) {
            isbn[i] = '0' + (rand() % 10); // Generate digits 0-9
        }
        isbn[9] = '\0'; // Null-terminate the string

        // Generate a fake checksum that cannot be valid
        int fake_checksum;
        do {
            fake_checksum = rand() % 11; // Generate a new checksum (0-10)
        } while (fake_checksum == calculate_isbn10_checksum(isbn));

        // Assign the last character for ISBN-10
        isbn[9] = (fake_checksum == 10) ? 'X' : '0' + fake_checksum; // Set the last character

        printf("Generated Fake ISBN-10: %s\n", isbn);
    } else { // Generate ISBN-13
        // Generate first 12 digits randomly
        for (i = 0; i < 12; i++) {
            isbn[i] = '0' + (rand() % 10); // Generate digits 0-9
        }
        isbn[12] = '\0'; // Null-terminate the string

        // Calculate a fake checksum that cannot be valid
        int checksum;
        do {
            checksum = rand() % 10; // Generate a new checksum (0-9)
        } while (checksum == calculate_isbn13_checksum(isbn));

        // Assign the fake checksum digit
        isbn[12] = '0' + checksum; // Set the last character

        printf("Generated Fake ISBN-13: %s\n", isbn);
    }
}

// Function to generate a real ISBN
void generate_real_isbn(int type) {
    char isbn[14]; // Buffer for ISBN-13, ISBN-10 is handled by the first 10 characters
    int i;

    if (type == 1) { // Generate ISBN-10
        // Generate the first 9 digits randomly
        for (i = 0; i < 9; i++) {
            isbn[i] = '0' + (rand() % 10); // Generate digits 0-9
        }
        isbn[9] = '\0'; // Null-terminate the string

        // Calculate checksum for ISBN-10
        int checksum = calculate_isbn10_checksum(isbn);
        isbn[9] = (checksum == 10) ? 'X' : '0' + checksum; // Set the last character

        printf("Generated Real ISBN-10: %s\n", isbn);
    } else { // Generate ISBN-13
        // Generate first 12 digits randomly
        for (i = 0; i < 12; i++) {
            isbn[i] = '0' + (rand() % 10); // Generate digits 0-9
        }
        isbn[12] = '\0'; // Null-terminate the string

        // Calculate checksum for ISBN-13
        int checksum = calculate_isbn13_checksum(isbn);
        isbn[12] = '0' + checksum; // Assign checksum digit

        printf("Generated Real ISBN-13: %s\n", isbn);
    }
}

// Function to validate ISBN-10
int validate_isbn10(const char isbn[]) {
    int sum = 0;

    // Check for proper format (numbers + 'X' allowed at the end)
    for (int i = 0; i < 9; i++) {
        if (!isdigit(isbn[i])) {
            return 0;
        }
    }
    if (!isdigit(isbn[9]) && isbn[9] != 'X') {
        return 0;
    }

    // Calculate checksum for ISBN-10
    for (int i = 0; i < 9; i++) {
        sum += (isbn[i] - '0') * (10 - i);
    }
    if (isbn[9] == 'X') {
        sum += 10;
    } else {
        sum += isbn[9] - '0';
    }

    return (sum % 11 == 0);
}

// Function to validate ISBN-13
int validate_isbn13(const char isbn[]) {
    int sum = 0;

    // Check for proper format (must all be digits)
    for (int i = 0; i < 13; i++) {
        if (!isdigit(isbn[i])) {
            return 0;
        }
    }

    // Calculate checksum for ISBN-13
    for (int i = 0; i < 12; i++) {
        if (i % 2 == 0) {
            sum += (isbn[i] - '0');
        } else {
            sum += 3 * (isbn[i] - '0');
        }
    }

    int checksum = (10 - (sum % 10)) % 10; // Calculate expected checksum
    return (checksum == (isbn[12] - '0'));
}

// Function to calculate the checksum for ISBN-10
int calculate_isbn10_checksum(const char isbn[]) {
    int sum = 0;

    for (int i = 0; i < 9; i++) {
        sum += (isbn[i] - '0') * (10 - i);
    }

    return sum % 11; // Return the check digit
}

// Function to calculate the checksum for ISBN-13
int calculate_isbn13_checksum(const char isbn[]) {
    int sum = 0;

    for (int i = 0; i < 12; i++) {
        if (i % 2 == 0) {
            sum += (isbn[i] - '0');
        } else {
            sum += 3 * (isbn[i] - '0');
        }
    }

    return (10 - (sum % 10)) % 10; // Return the check digit
}

//danahAbuRayya_1210195_sec5
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    char word[100];
    char meaning1[100];
    char meaning2[100];
    char meaning3[100];
} Dictionary;

// Hash function for quadratic probing
int hash(char *key, int TableSize, int i)
{
    long long hValue = 0;
    while (*key != '\0')
    {
        hValue = ((hValue * 32) + *key++) % TableSize;
    }
    return (hValue + i * i) % TableSize;
}

// Insert a new word into the hash table using quadratic probing
void insertNewWord(Dictionary htable[], char *word, char *meanings_list[], int size)
{
    int i = 0;
    while (i < size)
    {
        int index = hash(word, size, i);
        if (htable[index].word[0] == '\0')
        {
            // If the slot is empty, insert the word and meanings
            strcpy(htable[index].word, word);
            strcpy(htable[index].meaning1, meanings_list[0]);
            strcpy(htable[index].meaning2, meanings_list[1]);
            strcpy(htable[index].meaning3, meanings_list[2]);
            return;
        }
        else
        {
            i++;
        }
    }
    // Handle the case where the table is full (optional)
    printf("Error: Hash table is full. Unable to insert %s.\n", word);
}

// Hash function for double hashing
int hashDouble(int tableSize, char *key, int i)
{
    int x = hash(key, tableSize, i);
    int h1 = x % tableSize;
    int h2 = 1 + (x % (tableSize - 1));
    return (h1 + i * h2) % tableSize;
}

// Hash function for quadratic probing
int hashQuadratic(int table_size, char *key, int i)
{
    int x = hash(key, table_size, i);
    return (x + i * i) % table_size;
}

// Read words and meanings from a file and insert them into the hash table
void readFile(const char *filename, Dictionary htable[], int tableSize)
{
    FILE *ptr = fopen(filename, "r");
    if (ptr == NULL)
    {
        printf("File doesn't exist.\n");
        return;
    }

    char buffer[1000];

    while (fgets(buffer, sizeof(buffer), ptr) != NULL)
    {
        // Remove newline
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
        {
            buffer[len - 1] = '\0';
        }

        char *word = strtok(buffer, ":");
        char *meanings = strtok(NULL, ":");

        // Ensure that the three meanings exist
        if (meanings != NULL && strlen(meanings) > 0)
        {
            // Remove trailing newline character from meanings
            meanings[strlen(meanings) - 1] = '\0';

            char *meaning = strtok(meanings, "#");
            char *meaningsList[3] = {meaning, NULL, NULL};
            meaning = strtok(NULL, "#");
            if (meaning != NULL)
            {
                meaningsList[1] = meaning;
                meaning = strtok(NULL, "#");
                if (meaning != NULL)
                {
                    meaningsList[2] = meaning;
                }
            }

            insertNewWord(htable, word, meaningsList, tableSize);
        }
    }

    fclose(ptr);
}

// Check if a number is prime
int isPrime(int num)
{
    if (num <= 1)
    {
        return 0; // 0 represents false
    }

    for (int i = 2; i * i <= num; i++)
    {
        if (num % i == 0)
        {
            return 0; // 0 represents false
        }
    }

    return 1; // 1 represents true
}

// Calculate an appropriate table size based on the number of words in the file
int calculateTableSize(const char *filename)
{
    FILE *ptr = fopen(filename, "r");
    if (ptr == NULL)
    {
        printf("File doesn't exist.\n");
        return 0;
    }

    int counter = 0;
    char buffer[1000];

    while (fgets(buffer, sizeof(buffer), ptr) != NULL)
    {
        counter++;
    }

    fclose(ptr);

    int TSize = 2 * counter + 20;
    while (!isPrime(TSize))
    {
        TSize++;
    }

    return TSize;
}

// Search for a word using quadratic probing
int searchQuadratic(Dictionary htable[], char *key, int size)
{
    int i = 0;
    while (i < size)
    {
        int index = hashQuadratic(size, key, i);
        if (htable[index].word[0] != '\0' && strcmp(htable[index].word, key) == 0)
        {
            return index;
        }
        else if (htable[index].word[0] == '\0')
        {
            return -1;
        }
        else
        {
            i++;
        }
    }
    return -1;
}

// Search for a word using double hashing
int searchDouble(Dictionary htable[], char *key, int size)
{
    int i = 0;
    while (i < size)
    {
        int index = hashDouble(size, key, i);
        if (htable[index].word[0] != '\0' && strcmp(htable[index].word, key) == 0)
        {
            return index;
        }
        else if (htable[index].word[0] == '\0')
        {
            return -1;
        }
        else
        {
            i++;
        }
    }
    return -1;
}

// Delete a word from the hash table
void deletation(Dictionary htable[], char *key, int type, int tableSize)
{
    int index;
    if (type == 1)
    {
        index = searchQuadratic(htable, key, tableSize);
    }
    else if (type == 2)
    {
        index = searchDouble(htable, key, tableSize);
    }

    if (index != -1)
    {
        htable[index].word[0] = '\0';
    }
}

// Display the contents of the hash table
void displayTable(Dictionary hashtable[], int size)
{
    printf("Hash Table:\n");
    for (int i = 0; i < size; i++)
    {
        if (hashtable[i].word[0] != '\0')
        {
            printf("Index %d: %s: ", i, hashtable[i].word);
            printf("%s,%s,%s\n", hashtable[i].meaning1, hashtable[i].meaning2, hashtable[i].meaning3);
        }
        else
        {
            printf("Index %d:\n", i);
        }
    }
}

// Calculate the number of occupied slots in the hash table
int calculateOccupiedSlots(Dictionary hash_table[], int table_size)
{
    int count = 0;
    for (int i = 0; i < table_size; i++)
    {
        if (hash_table[i].word[0] != '\0')
        {
            count++;
        }
    }
    return count;
}

// Calculate the load factor of the hash table
float calculateLoadFactor(Dictionary hashTable[], int size)
{
    int occupiedSlots = calculateOccupiedSlots(hashTable, size);
    return (float)occupiedSlots / size;
}

// Write the contents of the hash table to a file
void writefile(Dictionary htable[], const char *filename, int size)
{
    FILE *ptr = fopen(filename, "a");
    if (ptr == NULL)
    {
        printf("Error opening file: %s\n", filename);
        return;
    }

    for (int i = 0; i < size; i++)
    {
        if (htable[i].word[0] != '\0')
        {
            fprintf(ptr, "%s:", htable[i].word);
            fprintf(ptr, "%s#", htable[i].meaning1);
            fprintf(ptr, "%s#", htable[i].meaning2);
            fprintf(ptr, "%s\n", htable[i].meaning3);
        }
    }

    fclose(ptr);
}

// Display the number of collisions for both open addressing methods
void collisions(int collisionsQuadratic, int collisionsDouble)
{
    printf("Collisions (Quadratic): %d\n", collisionsQuadratic);
    printf("Collisions (Double): %d\n", collisionsDouble);
}

// Display the main menu of options
void displayMainMenu()
{
    printf("\nOptions:\n");
    printf("1. Print hashed tables\n");
    printf("2. Print table size and load factor\n");
    printf("3. Print used hash functions\n");
    printf("4. Insert a new word to hash table\n");
    printf("5. Search for a specific word\n");
    printf("6. Delete a specific word\n");
    printf("7. Compare the number of collisions\n");
    printf("8. Save hash table to file (double hashing)\n");
    printf("9. Exit\n");
}

int main()
{
    // Calculate an appropriate table size based on the number of words in the file
    int tableSize = calculateTableSize("word.txt");
    Dictionary hQ[tableSize];
    Dictionary hD[tableSize];

    Dictionary *hashTableOpenaddressing = hQ;
    Dictionary *hashTableDoubleHashing = hD;

    // Initialize hash tables
    for (int i = 0; i < tableSize; i++)
    {
        hashTableOpenaddressing[i].word[0] = '\0';
        hashTableDoubleHashing[i].word[0] = '\0';
    }

    int collisionsQ = 0;
    int collisionsD = 0;

    // Read words and meanings from a file and insert them into the hash tables
    readFile("word.txt", hashTableOpenaddressing, tableSize);
    readFile("word.txt", hashTableDoubleHashing, tableSize);

    while (1)
    {
        // Display the main menu
        displayMainMenu();

        int key;
        printf("Enter option: ");
        scanf("%d", &key);

        switch (key)
        {
            case 1:
                // Print the contents of both hash tables
                printf(" Quadratic Hash Table:\n");
                displayTable(hashTableOpenaddressing, tableSize);
                printf("\nDouble Hashing Hash Table:\n");
                displayTable(hashTableDoubleHashing, tableSize);
                break;
            case 2:
                // Print table size and load factor for both hash tables
                printf("Table Size: %d\n", tableSize);
                printf("Open Addressing Load Factor: %f\n", calculateLoadFactor(hashTableOpenaddressing, tableSize));
                printf("Double Hashing Load Factor: %f\n", calculateLoadFactor(hashTableDoubleHashing, tableSize));
                break;
            case 3:
                // Print the hash functions used by both hash tables
                printf("Open Addressing Hash Function: hashQuadratic = (word + i * i) %% tableSize\n");
                printf("Double Hashing Hash Function: hashDouble = (word %% tableSize )+1 + (word %% (tableSize - 1)) \n");
                break;
            case 4:
                // Insert a new record into both hash tables
                char in[100];
                printf("Enter word and meanings as this format (Word: #Meaning1, #Meaning2, #Meaning3): ");
                scanf(" %[^\n]", in);

                char *word = strtok(in, ":");
                char *meanings = strtok(NULL, ":");

                char *meaning = strtok(meanings, "#");
                char *meaningsList[3] = {meaning, NULL, NULL};
                meaning = strtok(NULL, "#");
                if (meaning != NULL)
                {
                    meaningsList[1] = meaning;
                    meaning = strtok(NULL, "#");
                    if (meaning != NULL)
                    {
                        meaningsList[2] = meaning;
                    }
                }

                insertNewWord(hashTableOpenaddressing, word, meaningsList, tableSize);
                insertNewWord(hashTableDoubleHashing, word, meaningsList, tableSize);

                printf("Inserted into Open Addressing Hash Table\n");
                printf("Inserted into Double Hashing Hash Table\n");

                collisionsQ += 1;
                collisionsD += 1;
                break;
            case 5:
                // Search for a specific word in both hash tables
                char search[100];
                printf("Enter word to search: ");
                scanf(" %[^\n]", search);

                int indexOpenAddressing = searchQuadratic(hashTableOpenaddressing, search, tableSize);
                int indexDoubleHashing = searchDouble(hashTableDoubleHashing, search, tableSize);

                if (indexOpenAddressing != -1)
                {
                    printf("Open Addressing: Found at index %d\n", indexOpenAddressing);
                }
                else
                {
                    printf("Open Addressing: Not found\n");
                }

                if (indexDoubleHashing != -1)
                {
                    printf("Double Hashing: Found at index %d\n", indexDoubleHashing);
                }
                else
                {
                    printf("Double Hashing: Not found\n");
                }

                break;
            case 6:
                // Delete a specific word from both hash tables
                char delete[100];
                printf("Enter word to delete: ");
                scanf(" %[^\n]", delete);

                deletation(hashTableOpenaddressing, delete, 1, tableSize);
                deletation(hashTableDoubleHashing, delete, 2, tableSize);

                printf("Open Addressing: Deleted successfully\n");
                printf("Double Hashing: Deleted successfully\n");
                break;
            case 7:
                // Compare the number of collisions in both hash tables
                collisions(collisionsQ, collisionsD);
                break;
            case 8:
                // Save the hash table that used double hashing to a file
                writefile(hashTableDoubleHashing, "output.txt", tableSize);
                printf("Hash table saved to output.txt\n");
                break;
            case 9:

                exit(0);
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    }

    return 0;
}

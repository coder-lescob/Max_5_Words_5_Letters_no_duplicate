#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>

typedef struct ArrayList {
    int* arrayPtr;
    int size;
    int maxcapacity;
} ArrayList;


typedef struct Word {
    char* word;
    int length;

    int duplicates;
    int lettersMask;
    ArrayList groups;
} Word;

typedef struct Group {
    Word words[5];
} Group;

ArrayList NewArrayList(int* arrayInitilizer, int size, int capacity) {
    assert(size <= capacity);
    ArrayList array = {.arrayPtr = (int*)malloc(capacity * sizeof(int)), .size = size, .maxcapacity = capacity};
    if (arrayInitilizer) memcpy(array.arrayPtr, arrayInitilizer, size);
    return array;
}

void AddToArrayList(ArrayList* array, int item) {
    if (array->size < array->maxcapacity) {
        // just add it to the end
        array->arrayPtr[array->size++] = item;
    }
    else {
        int* oldArray = array->arrayPtr;
        *array = NewArrayList(oldArray, array->size, (array->size + 1) * 2);
        free(oldArray);

        // just add it to the end
        array->arrayPtr[array->size++] = item;
    }
}

int GetItem(ArrayList array, int index) {
    assert(index < array.size && index >= 0);
    return array.arrayPtr[index];
}

void SetItem(ArrayList* array, int index, int value) {
    assert(index < array->size && index >= 0);
    array->arrayPtr[index] = value;
}

int main() {
    FILE *fptr;
    fptr = fopen("words.txt", "r");
    if (fptr == NULL) {
        printf("Not able to open file.\n");
        return 0;
    }

    fseek(fptr, 0, SEEK_END); // seek to end of file
    int size = ftell(fptr); // get current file pointer
    fseek(fptr, 0, SEEK_SET); // seek back to beginning of file

    // convert from bytes to words
    size /= 7;

    ArrayList groupMask  = NewArrayList(NULL, 0, 100);
    ArrayList groupWords = NewArrayList(NULL, 0, 100);

    ArrayList completedGroups = NewArrayList(NULL, 0, 100);
    Word words[size];

    char chunk[8];
    int j = 0;
    while (fgets(chunk, 8, fptr) && j < size) {
        assert(groupMask.size == groupWords.size);
        chunk[7] = 0;
        // process word !
        Word word;
        word.word = (char*)malloc(8 * sizeof(char));
        memcpy(word.word, chunk, 8);
        word.length = 5; // ignore \n and 0
        word.lettersMask = 0x00;
        word.duplicates  = 0x00;

        // create the letters mask
        for (int i = 0; i < word.length; i++) {
            int letterMask = 1 << (word.word[i] - 'a');
            if (word.lettersMask & letterMask) {
                // this word does not interrest us
                word.duplicates = 0x01;
                break;
            }
            word.lettersMask |= letterMask;
        }
        if (word.duplicates == 0) {
            // valid word
            printf("%.5s 0x%x ", word.word, word.lettersMask);
            int foundGroup = 0;

            for (int i = 0; i < groupMask.size; i++) {
                if ((GetItem(groupMask, i) & word.lettersMask) == 0) {
                    if (GetItem(groupWords, i) < 5) {
                        // hourra there is place on that group
                        AddToArrayList(&word.groups, i);
                        foundGroup = 1;
                        SetItem(&groupWords, i, GetItem(groupWords, i) + 1);
                        SetItem(&groupMask, i, GetItem(groupMask, i) | word.lettersMask);
                        printf("group: %d ", i + 1);
                        if (GetItem(groupWords, i) == 5) {
                            // whoa the group is found !
                            AddToArrayList(&completedGroups, i);
                            printf("group %d completed", i + 1);
                        }
                        break;
                    }
                }
            }

            if (groupMask.size == 0 || word.groups.size == 0 || foundGroup == 0) {
                AddToArrayList(&groupMask, word.lettersMask);
                AddToArrayList(&groupWords, 1);
                printf(" new group: %d", groupMask.size);
            }

            printf("\n");
        }

        words[j] = word;
        j++;
    }

    Group groups[completedGroups.size];

    for (int i = 0; i < groupMask.size; i++) {
        printf("group size %d\n", GetItem(groupWords, i));
    }

    for (int i = 0; i < size; i++) {
        // free word
        free(words[i].word);
    }

    printf("number of solutions: 0x%x\n", completedGroups.size);

    fclose(fptr);
}
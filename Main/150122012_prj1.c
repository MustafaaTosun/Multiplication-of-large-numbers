#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/*This code performs multiplication of large integer and float numbers*/


struct multiple {
    int exp;
    int digit;
    struct multiple *next;
    struct multiple *prev;
};

typedef struct multiple mult;

void handle(mult **head);

void linkedlist(mult **head);

void addDigit(mult **head, int digit, int exp);

mult *process(mult *head1, mult *head2);

int nodeCounter(mult *head);

void printList(mult *head, FILE *outputFile, int dot);

void process2(mult **head);

void addDigit(mult **head, int digit, int exp) {
    mult *newNode = (mult *) malloc(sizeof(mult));
    newNode->digit = digit;
    newNode->next = NULL;
    newNode->exp = exp;
    newNode->prev = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        mult *curr = *head;

        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = newNode;
        newNode->prev = curr;
    }
}

mult *process(mult *head1, mult *head2) {
    mult *result = NULL;
    mult *tail = NULL;
    mult *curr1 = head1;
    mult *curr2;

    while (curr1 != NULL) {
        curr2 = head2;
        while (curr2 != NULL) {
            mult *newNode = (mult *) malloc(sizeof(mult));
            newNode->digit = curr1->digit * curr2->digit;
            newNode->exp = curr1->exp + curr2->exp;
            newNode->next = NULL;

            if (result == NULL) {
                result = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }

            curr2 = curr2->next;
        }
        curr1 = curr1->next;
    }
    return result;
}


//sum the digit have the same exponent
void process2(mult **head) {
    mult *curr1 = *head;
    mult *prev, *curr2;

    while (curr1 != NULL) {
        prev = curr1;
        curr2 = curr1->next;

        while (curr2 != NULL) {
            if (curr1->exp == curr2->exp) {
                curr1->digit += curr2->digit;
                prev->next = curr2->next;
                if (curr2->next != NULL) {
                    curr2->next->prev = prev;
                }
                free(curr2);
                curr2 = prev->next;
            } else {
                prev = curr2;
                curr2 = curr2->next;
            }
        }
        curr1 = curr1->next;
    }
}

//be arranged the node's prev and next
void linkedlist(mult **head) {
    mult *curr = *head;
    curr->prev = NULL;
    while (curr->next != NULL) {
        curr->next->prev = curr;
        curr = curr->next;
    }
}

void handle(mult **head) {
    // arrange the node's prev and next
    linkedlist(head);
    mult *curr = *head;


    while (curr->next != NULL) {
        curr = curr->next;
    }

    while (curr != NULL) {
        if (curr->digit > 10) {
            int carry = (curr->digit) / 10;
            curr->digit = curr->digit % 10;

            if (curr->prev != NULL) {
                curr->prev->digit += carry;
            } else {
                mult *newNode = (mult *) malloc(sizeof(mult));
                newNode->digit = carry;
                newNode->next = *head;
                newNode->prev = NULL;

                if (*head != NULL) {
                    (*head)->prev = newNode;
                }
                *head = newNode;
            }
        }
        curr = curr->prev;
    }
}

int nodeCounter(mult *head) {
    mult *curr = head;
    int count = 0;
    while (curr->next != NULL) {
        count++;
        curr = curr->next;
    }
    return count;
}

void printList(mult *head, FILE *outputFile, int dot) {
    mult *curr = head;
    int count = nodeCounter(head);

    while (curr != NULL) {
        if (curr->exp + 1 == dot - count) {
            fprintf(outputFile, ".");
        }
        fprintf(outputFile, "%d", curr->digit);
        curr = curr->next;
    }
}

int main(void) {
    clock_t start, end;
    double time_spent;
    start = clock();

    mult *head1 = NULL;
    mult *head2 = NULL;
    FILE *inputFile;
    FILE *outputFile;
    int ch;
    int currentList = 1;
    int bool = 0;
    int a = 0;
    int b = 0;
    int exp1 = 0, exp2 = 0;

    inputFile = fopen("150122012_prj1_input.txt", "r");
    if (inputFile == NULL) {
        printf("Dosya açılamadı!\n");
        return 1;
    }

    while ((ch = fgetc(inputFile)) != EOF) {
        if (ch >= '0' && ch <= '9') {
            if (currentList == 1) {
                addDigit(&head1, ch - '0', exp1);
                exp1--;
                if (bool) {
                    a++;
                }
            } else {
                addDigit(&head2, ch - '0', exp2);
                exp2--;
                if (bool) {
                    b++;
                }
            }
        } else if (ch == '\n') {
            currentList = 2;
            bool = 0;
        } else {
            bool = 1;
        }
    }
    fclose(inputFile);

    mult *result = process(head1, head2);
    process2(&result);
    handle(&result);

    outputFile = fopen("150122012_prj1_output.txt", "w");
    if (outputFile == NULL) {
        printf("Sonuç dosyası açılamadı!\n");
        return 1;
    }
    fprintf(outputFile, "Multiplicand: ");
    printList(head1, outputFile, a);
    fprintf(outputFile, "\nMultiplier: ");
    printList(head2, outputFile, b);
    fprintf(outputFile, "\nResult: ");
    printList(result, outputFile, a + b);

    end = clock();
    time_spent = (double) (end - start) / CLOCKS_PER_SEC;
    fprintf(outputFile, "\nExecution Time: %.8f", time_spent);
    fclose(outputFile);

    return 0;
}

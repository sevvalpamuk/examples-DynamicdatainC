#include <stdio.h>
#include <stdlib.h>

// Linked-List
typedef struct Node {
    int data;
    struct Node* next;
} node;

void write_linklist(node* n);
void write_linklist_to_file(FILE* fptr, node* n);
void Create_linklist(node** n, int num); // pointerin adresini yolluyoruz kalıcı değişikliklik yapabilmek için
void add_node_to_start(node** linkedlist, int num);
void add_node_to_end(node** linkedlist, int num);
int delete_node(node** linkedlist, int num);
int isEmpty(node* n);
void orderlist(node** n, int item);
void information();

int main() {
    node* linkedlist = NULL;  // Başlangıçta boş bir bağlı liste
    int item;
    int choice;
    information();

    printf("? ");
    scanf("%d", &choice);
    
    FILE* fptr = fopen("linkedlist.txt", "w"); // Dosyayı aç
    if (fptr == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    while (choice != 3) {
        switch (choice) {
            case 1:
                printf("Enter an integer: ");
                scanf("%d", &item);
                orderlist(&linkedlist, item); /* insert item in list */
                write_linklist(linkedlist);
                write_linklist_to_file(fptr, linkedlist); // Dosyaya yaz
                break;

            case 2:
                if (!isEmpty(linkedlist)) {
                    printf("Enter integer to be deleted: ");
                    scanf("%d", &item);
                    /* if integer is found, remove it */
                    if (delete_node(&linkedlist, item)) { /* remove item */
                        printf("%d deleted.\n", item);
                        write_linklist(linkedlist);
                        write_linklist_to_file(fptr, linkedlist); // Dosyaya yaz
                    } else {
                        printf("%d not found.\n\n", item);
                    }
                } else {
                    printf("List is empty.\n\n");
                }
                break;

            default:
                printf("Invalid choice.\n\n");
                break;
        }
        information();
        printf("? ");
        scanf("%d", &choice);
    }
    printf("End of program.\n");

    fclose(fptr); // Dosyayı kapat

    // Bellek temizleme
    node* temp;
    while (linkedlist != NULL) {
        temp = linkedlist; // ikisi de aynı düğümü işaret eder
        linkedlist = linkedlist->next;
        free(temp);
    }

    return 0;
}

void write_linklist(node* n) {
    node* iter = n;

    while (iter != NULL) {
        printf("%d- ", iter->data);
        iter = iter->next;
    }
    printf("\n");
}

void write_linklist_to_file(FILE* fptr, node* n) {
    node* iter = n;
    
    rewind(fptr); // Dosya başına git
    // Dosyayı temizle

    while (iter != NULL) {
        fprintf(fptr, "%d-", iter->data);
        iter = iter->next;
    }
    fprintf(fptr, "\n");
    fflush(fptr); // Dosya yazma işlemlerini hemen diske yaz
}

void Create_linklist(node** n, int num) {
    *n = malloc(sizeof(node));
    if (*n == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    (*n)->data = num;
    (*n)->next = NULL;
}

void add_node_to_start(node** linkedlist, int num) {
    node* newlinkedlist; // Yeni liste oluşturdum
    Create_linklist(&newlinkedlist, num); // Yeni listeme veri atadım
    newlinkedlist->next = *linkedlist; // yeni listemden sonraki sıraya kendi listemi koydum, en başa yeni değeri koydum
    *linkedlist = newlinkedlist; // yeni listemin tamamını asıl listeme atadım
}

void add_node_to_end(node** linkedlist, int num) {
    if (*linkedlist == NULL) { // Eğer bağlı liste boşsa, listenin başına ekle
        add_node_to_start(linkedlist, num);
        return;
    }
    node* iter = *linkedlist;
    while (iter->next != NULL) {
        iter = iter->next;
    }
    node* new_node;
    Create_linklist(&new_node, num); // yeni nodu oluştur
    iter->next = new_node; // itere ekle aynı zamanda linked list sonuna
}

int delete_node(node** linkedlist, int num) {
    node* temp;
    node* current = (*linkedlist)->next;
    if (*linkedlist == NULL) {
        printf("List is empty\n");
        return 0;
    } else if ((*linkedlist)->data == num) {
        temp = (*linkedlist);
        *linkedlist = (*linkedlist)->next;
        free(temp);
        return 1;
    } else {
        temp = *linkedlist;
        while (current != NULL && current->data != num) { // verilen num bulunamazsa iter sürekli nextine gidecek, bunun olmaması için NULL koşulu ekledik
            temp = current;
            current = current->next;
        }
        if (current == NULL) {
            printf("Value not found\n");
        } else {
            temp->next = current->next;
            free(current);
            return 1;
        }
    }
    return 0;
}

int isEmpty(node* n) {
    return n == NULL;
}

void orderlist(node** n, int item) {
    node* newPtr = NULL; // alınan değer için Yeni düğüm işaretçi
    node* previousPtr = NULL; // Önceki düğüm için işaretçi
    node* currentPtr = NULL; // Mevcut düğüm için işaretçi
    Create_linklist(&newPtr, item);
    previousPtr = NULL; // liste başındayız
    currentPtr = *n;

    while (currentPtr != NULL && currentPtr->data < item) { // doğru karşılaştırma
        previousPtr = currentPtr; // 1. öncekini tutuyor, bunun sebebi prev ile current arasına newptr koyacağımız, ikisi de lazım
        currentPtr = currentPtr->next; // İlerliyoruz     
    }

    if (previousPtr == NULL) { // eğer hala prev == NULL ise liste başındayızdır
        newPtr->next = *n;
        *n = newPtr; // *n benim başlangıcımı tutar
    } else {
        previousPtr->next = newPtr;
        newPtr->next = currentPtr; // listeleri birbirine atıoyruz
    }
}

void information() {
    printf("Enter your choice:\n 1 to insert an element into the list.\n 2 to delete an element from the list.\n 3 to end.\n");
}

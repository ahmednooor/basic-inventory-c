#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "lib/cs50.h"
#include "lib/cs50.c"

typedef struct _item {
    char ID[9];
    char name[201];
    float qty;
    float avgUnitCost;
} _item;

int loadInventory(void);
int showInventoryList(void);
int writeInventoryToFile(void);
int addNewInventoryItem(void);
int deleteInventoryItem(void);
int addQuantity(void);
int removeQuantity(void);
int searchInventoryItem(void);

_item inventory[100];
int counter = 0;

int main(void) {
    loadInventory();

    printf("\n-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n  B a s i c   I n v e n t o r y   S y s t e m  |\n-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");

    while (1) {
        printf("\n# MENU\n#  0 - EXIT\n#  1 - Add new inventory item\n#  2 - Delete inventory item\n#  3 - Add quantity to an inventory item\n#  4 - Subtract quantity from an inventory item\n#  5 - Show complete inventory list\n#  6 - Search for an inventory item\n\n");
        int menuIndex;
        menuIndex = get_int();
        if (menuIndex == 0) {
            return 0;
        } else if (menuIndex == 1) {
            addNewInventoryItem();
        } else if (menuIndex == 2) {
            deleteInventoryItem();
        } else if (menuIndex == 3) {
            addQuantity();
        } else if (menuIndex == 4) {
            removeQuantity();
        } else if (menuIndex == 5) {
            showInventoryList();
        } else if (menuIndex == 6) {
            searchInventoryItem();
        } else {
            continue;
        }
    }
}

int loadInventory() {
    FILE* database = fopen("database/inventory.txt", "r");
    if (database == NULL) {
        printf("Could not open database.\n");
        return 1;
    }
    char c = '0';
    while (c != EOF) {
        if (c != EOF) {
            _item item;
            inventory[counter] = item;
            inventory[counter].qty = 0.0;
            inventory[counter].avgUnitCost = 0.0;
        }
        int i = 0;
        while (c != EOF) {
            c = fgetc(database);
            if (c == '\n' || c == EOF) {
                if (c == '\n') {
                    inventory[counter].ID[i] = '\0';
                }
                break;
            } else {
                inventory[counter].ID[i] = c;
                i++;
            }
        }
        int j = 0;
        while (c != EOF) {
            c = fgetc(database);
            if (c == '\n' || c == EOF) {
                if (c == '\n') {
                    inventory[counter].name[j] = '\0';
                }
                break;
            } else {
                inventory[counter].name[j] = c;
                j++;
            }
        }
        int k = 0;
        int floatSwitch = 0;
        int decimalCounter = 0;
        while (c != EOF) {
            c = fgetc(database);
            if (c == '\n' || c == EOF) {
                break;
            } else {
                if (c == '.') {
                    floatSwitch = 1;
                    decimalCounter = 1;
                } else {
                    if (floatSwitch == 0) {
                        inventory[counter].qty = (inventory[counter].qty * 10.0) + (1.0 * (c - '0'));
                    } else {
                        inventory[counter].qty = (inventory[counter].qty) + 1.0 * ((1.0 * (c - '0')) / powf(10.0, decimalCounter));
                        decimalCounter++;
                    }
                }
                k++;
            }
        }
        int l = 0;
        floatSwitch = 0;
        decimalCounter = 0;
        while (c != EOF) {
            c = fgetc(database);
            if (c == '\n' || c == EOF) {
                break;
            } else {
                if (c == '.') {
                    floatSwitch = 1;
                    decimalCounter = 1;
                } else {
                    if (floatSwitch == 0) {
                        inventory[counter].avgUnitCost = (inventory[counter].avgUnitCost * 10.0) + (1.0 * (c - '0'));
                    } else {
                        inventory[counter].avgUnitCost = (inventory[counter].avgUnitCost) + 1.0 * ((1.0 * (c - '0')) / powf(10.0, decimalCounter));
                        decimalCounter++;
                    }
                }
                l++;
            }
        }
        if (c != EOF) {
            counter++;
        }
    }
    fclose(database);
    return 0;
}

int showInventoryList(void) {
    int columnWidth = 0;
    for (int i = 0; i < counter; i++) {
        for (int j = 0; j < counter; j++) {
            if (strlen(inventory[i].name) >= strlen(inventory[j].name)) {
                columnWidth = strlen(inventory[i].name);
            } else {
                break;
            }
        }
    }
    printf("\n### Inventory List ###\n\n");
    printf("%-10s\t%-*s\t%s\t%s\n", "ID", (columnWidth + 4), "NAME", "QTY", "AvgUnitCost");
    for (int i = 0; i < counter; i++) {
        printf("%-10s\t%-*s\t%.2f\t%.2f\n", inventory[i].ID, (columnWidth + 4), inventory[i].name, inventory[i].qty, inventory[i].avgUnitCost);
    }
    printf("\nPress Enter to return to MENU.\n");
    getchar();
    return 0;
}

int writeInventoryToFile(void) {
    FILE* database = fopen("database/inventory.txt", "w");
    if (database == NULL) {
        printf("Could not open database.\n");
        return 1;
    }
    for (int i = 0; i < counter; i++) {
        for (int j = 0; j < strlen(inventory[i].ID); j++) {
            fputc(inventory[i].ID[j], database);
        }
        fputc('\n', database);
        for (int j = 0; j < strlen(inventory[i].name); j++) {
            fputc(inventory[i].name[j], database);
        }
        fputc('\n', database);
        fprintf(database, "%f\n", inventory[i].qty);
        fprintf(database, "%f\n", inventory[i].avgUnitCost);
    }
    fclose(database);
    return 0;
}

int addNewInventoryItem(void) {
    printf("\n### New Inventory Item Addition ###\n\n");
    char IDsearch[500];
    printf("Enter the ID of Product. (Maximum 8 characters): ");
    fgets(IDsearch, sizeof(IDsearch), stdin);
    IDsearch[strcspn(IDsearch, "\r\n")] = 0;
    if (strlen(IDsearch) != 0) {
        while (strlen(IDsearch) > 8) {
            printf("Exceeded 8 characters. Please try again: ");
            fgets(IDsearch, sizeof(IDsearch), stdin);
            IDsearch[strcspn(IDsearch, "\r\n")] = 0;
        }
        int i = 0;
        for (i = 0; i < counter; i++) {
            if (strcmp(IDsearch, inventory[i].ID) == 0) {
                break;
            }
        }
        if (strcmp(IDsearch, inventory[i].ID) != 0) {
            _item item;
            inventory[counter] = item;
            strcpy(inventory[counter].ID, IDsearch);
            printf("Enter the Name of Product. (Maximum 200 characters): ");
            fgets(inventory[counter].name, sizeof(inventory[counter].name), stdin);
            inventory[counter].name[strcspn(inventory[counter].name, "\r\n")] = 0;
            while (strlen(inventory[counter].name) > 200) {
                printf("Exceeded 200 characters. Please try again: ");
                fgets(inventory[counter].name, sizeof(inventory[counter].name), stdin);
                inventory[counter].name[strcspn(inventory[counter].name, "\r\n")] = 0;
            }
            printf("Enter the Quantity of Product in numbers. (e.g. 10.00): ");
            inventory[counter].qty = get_double();
            printf("Enter the Total Cost of above entered Quantity in numbers. (e.g. 10.00): ");
            float totalCost = get_double();
            inventory[counter].avgUnitCost = inventory[counter].qty != 0.0 ? totalCost / inventory[counter].qty : 0.0;
            counter++;
            writeInventoryToFile();
            printf("\nNew inventory item added.\n");
            printf("%-10s\t%-*s\t%s\t%s\n", "ID", (int) (strlen(inventory[counter - 1].name) + 4), "NAME", "QTY", "AvgUnitCost");
            printf("%-10s\t%-*s\t%.2f\t%.2f\n", inventory[counter - 1].ID, (int) (strlen(inventory[counter - 1].ID) + 4), inventory[counter - 1].name, inventory[counter - 1].qty, inventory[counter - 1].avgUnitCost);
            printf("\nPress Enter to return to MENU.\n");
            getchar();
            return 0;
        } else {
            printf("\nAnother inventory item of the same ID exists already. Please use a different ID.\n");
            printf("Press Enter to return to MENU.\n");
            getchar();
            return 1;
        }
    } else {
        return 1;
    }
}

int deleteInventoryItem(void) {
    printf("\n### Inventory Item Deletion ###\n\n");
    char IDsearch[500];
    printf("Enter the ID of Product to Delete. (Maximum 8 characters): ");
    fgets(IDsearch, sizeof(IDsearch), stdin);
    IDsearch[strcspn(IDsearch, "\r\n")] = 0;
    if (strlen(IDsearch) != 0) {
        while (strlen(IDsearch) > 8) {
            printf("Exceeded 8 characters. Please try again: ");
            fgets(IDsearch, sizeof(IDsearch), stdin);
            IDsearch[strcspn(IDsearch, "\r\n")] = 0;
        }
        int i = 0;
        for (i = 0; i < counter; i++) {
            if (strcmp(IDsearch, inventory[i].ID) == 0) {
                break;
            }
        }
        if (strcmp(IDsearch, inventory[i].ID) == 0) {
            printf("\nThe following inventory item will be deleted.\n");
            printf("%-10s\t%-*s\t%s\t%s\n", "ID", (int) (strlen(inventory[i].name) + 4), "NAME", "QTY", "AvgUnitCost");
            printf("%-10s\t%-*s\t%.2f\t%.2f\n", inventory[i].ID, (int) (strlen(inventory[i].name) + 4), inventory[i].name, inventory[i].qty, inventory[i].avgUnitCost);
            printf("\nDo you want to proceed? y/n: ");
            char yn;
            yn = get_char();
            if (yn == 'y' || yn == 'Y') {
                inventory[i] = inventory[counter - 1];
                _item item;
                inventory[counter - 1] = item;
                counter--;
                writeInventoryToFile();
                printf("\nIventory item deleted. Press Enter to return to MENU.\n");
                getchar();
                return 0;
            } else {
                return 0;
            }
        } else {
            printf("\nThe ID did not match with any inventory item.\n");
            printf("Press Enter to return to MENU.\n");
            getchar();
            return 1;
        }
    } else {
        return 1;
    }
}

int addQuantity(void) {
    printf("\n### Inventory Item Quantity Addition ###\n\n");
    char IDsearch[500];
    printf("Enter the ID of Product to be increased in quantity. (Maximum 8 characters): ");
    fgets(IDsearch, sizeof(IDsearch), stdin);
    IDsearch[strcspn(IDsearch, "\r\n")] = 0;
    if (strlen(IDsearch) != 0) {
        while (strlen(IDsearch) > 8) {
            printf("Exceeded 8 characters. Please try again: ");
            fgets(IDsearch, sizeof(IDsearch), stdin);
            IDsearch[strcspn(IDsearch, "\r\n")] = 0;
        }
        int i = 0;
        for (i = 0; i < counter; i++) {
            if (strcmp(IDsearch, inventory[i].ID) == 0) {
                break;
            }
        }
        if (strcmp(IDsearch, inventory[i].ID) == 0) {
            printf("Enter the Quantity to add. (e.g. 10.00): ");
            float newQty = get_double();
            printf("Enter the Total Cost of added Quantity in numbers. (e.g. 10.00): ");
            float newTotalCost = get_double();
            inventory[i].avgUnitCost = inventory[i].qty + newQty != 0.0 ? ((inventory[i].avgUnitCost * inventory[i].qty) + newTotalCost) / (inventory[i].qty + newQty) : 0.0;
            inventory[i].qty = inventory[i].qty + newQty;
            writeInventoryToFile();
            printf("\nInventory item updated.\n");
            printf("%-10s\t%-*s\t%s\t%s\n", "ID", (int) (strlen(inventory[i].name) + 4), "NAME", "QTY", "AvgUnitCost");
            printf("%-10s\t%-*s\t%.2f\t%.2f\n", inventory[i].ID, (int) (strlen(inventory[i].name) + 4), inventory[i].name, inventory[i].qty, inventory[i].avgUnitCost);
            printf("\nPress Enter to return to MENU.\n");
            getchar();
            return 0;
        } else {
            printf("\nThe ID did not match with any inventory item.\n");
            printf("Press Enter to return to MENU.\n");
            getchar();
            return 1;
        }
    } else {
        return 1;
    }
}

int removeQuantity(void) {
    printf("\n### Inventory Item Quantity Subtraction ###\n\n");
    char IDsearch[500];
    printf("Enter the ID of Product to be decreased in quantity. (Maximum 8 characters): ");
    fgets(IDsearch, sizeof(IDsearch), stdin);
    IDsearch[strcspn(IDsearch, "\r\n")] = 0;
    if (strlen(IDsearch) != 0) {
        while (strlen(IDsearch) > 8) {
            printf("Exceeded 8 characters. Please try again: ");
            fgets(IDsearch, sizeof(IDsearch), stdin);
            IDsearch[strcspn(IDsearch, "\r\n")] = 0;
        }
        int i = 0;
        for (i = 0; i < counter; i++) {
            if (strcmp(IDsearch, inventory[i].ID) == 0) {
                break;
            }
        }
        if (strcmp(IDsearch, inventory[i].ID) == 0) {
            printf("Enter the Quantity to subtract. (e.g. 10.00): ");
            float newQty = get_double();
            printf("Enter the Total Cost of subtracted Quantity in numbers. (e.g. 10.00): ");
            float newTotalCost = get_double();
            inventory[i].avgUnitCost = inventory[i].qty - newQty != 0.0 ? ((inventory[i].avgUnitCost * inventory[i].qty) - newTotalCost) / (inventory[i].qty - newQty) : 0.0;
            inventory[i].qty = inventory[i].qty - newQty;
            writeInventoryToFile();
            printf("\nInventory item updated.\n");
            printf("%-10s\t%-*s\t%s\t%s\n", "ID", (int) (strlen(inventory[i].name) + 4), "NAME", "QTY", "AvgUnitCost");
            printf("%-10s\t%-*s\t%.2f\t%.2f\n", inventory[i].ID, (int) (strlen(inventory[i].name) + 4), inventory[i].name, inventory[i].qty, inventory[i].avgUnitCost);
            printf("\nPress Enter to return to MENU.\n");
            getchar();
            return 0;
        } else {
            printf("\nThe ID did not match with any inventory item.\n");
            printf("Press Enter to return to MENU.\n");
            getchar();
            return 1;
        }
    } else {
        return 1;
    }
}

int searchInventoryItem(void) {
    printf("\n### Inventory Item Search ###\n\n");
    char IDsearch[500];
    printf("Enter the ID of Product to Search. (Maximum 8 characters): ");
    fgets(IDsearch, sizeof(IDsearch), stdin);
    IDsearch[strcspn(IDsearch, "\r\n")] = 0;
    if (strlen(IDsearch) != 0) {
        while (strlen(IDsearch) > 8) {
            printf("Exceeded 8 characters. Please try again: ");
            fgets(IDsearch, sizeof(IDsearch), stdin);
            IDsearch[strcspn(IDsearch, "\r\n")] = 0;
        }
        int i = 0;
        for (i = 0; i < counter; i++) {
            if (strcmp(IDsearch, inventory[i].ID) == 0) {
                break;
            }
        }
        if (strcmp(IDsearch, inventory[i].ID) == 0) {
            printf("\nSearched inventory item.\n");
            printf("%-10s\t%-*s\t%s\t%s\n", "ID", (int) (strlen(inventory[i].name) + 4), "NAME", "QTY", "AvgUnitCost");
            printf("%-10s\t%-*s\t%.2f\t%.2f\n", inventory[i].ID, (int) (strlen(inventory[i].name) + 4), inventory[i].name, inventory[i].qty, inventory[i].avgUnitCost);
            printf("\nPress Enter to return to MENU.\n");
            getchar();
            return 0;
        } else {
            printf("\nThe ID did not match with any inventory item.\n");
            printf("Press Enter to return to MENU.\n");
            getchar();
            return 1;
        }
    } else {
        return 1;
    }
}

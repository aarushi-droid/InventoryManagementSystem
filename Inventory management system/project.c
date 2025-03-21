#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

struct date {
    int dd, mm, yy;
};

struct item {
    int itemno;
    char name[25];
    struct date d;
    float price, qty, tax, dis, netamt;
};

// Function Prototypes
void login();
void addNewUser();
void addItem();
void displayItems();
void editItem();
void deleteItem();
void generateBill();
void loadPredefinedItems();
void calculate(struct item *it);

// Function for calculating net amount
void calculate(struct item *it) {
    float gross = it->price + (it->price * (it->tax / 100));
    it->netamt = it->qty * (gross - (gross * (it->dis / 100)));
}

int main() {
    int choice;

    login(); // User must log in first
    loadPredefinedItems(); // Load predefined items

    do {
        printf("\nInventory Management System");
        printf("\n1. Add Item");
        printf("\n2. Display Items");
        printf("\n3. Edit Item");
        printf("\n4. Delete Item");
        printf("\n5. Generate Bill");
        printf("\n6. Add New User");
        printf("\n7. Exit");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addItem(); break;
            case 2: displayItems(); break;
            case 3: editItem(); break;
            case 4: deleteItem(); break;
            case 5: generateBill(); break;
            case 6: addNewUser(); break;
            case 7: exit(0);
            default: printf("\nInvalid choice! Try again.\n");
        }
    } while (choice != 7);

    return 0;
}

void login() {
    char username[20], password[20];
    int authenticated = 0;

    printf("\n--- Login ---\n");

    while (!authenticated) {
        printf("Enter Username: ");
        scanf("%s", username);
        printf("Enter Password: ");
        scanf("%s", password);

        if (strcmp(username, "admin") == 0 && strcmp(password, "1234") == 0) {
            printf("\nLogin Successful!\n");
            authenticated = 1;
        } else {
            printf("\nInvalid credentials. Try again.\n");
        }
    }
}

void addNewUser() {
    char newUser[20], newPass[20];

    printf("\n----- Add New User -----\n");
    printf("Enter New Username: ");
    scanf("%s", newUser);
    printf("Enter New Password: ");
    scanf("%s", newPass);

    FILE *file = fopen("users.txt", "a");
    if (file == NULL) {
        printf("\nError: Cannot open users file!\n");
        return;
    }

    // Save new user credentials to file
    fprintf(file, "%s %s\n", newUser, newPass);
    fclose(file);

    printf("\nNew User Added Successfully!\n");
}

void addItem() {
    struct item it;
    FILE *file;

    file = fopen("items.txt", "a");
    if (!file) {
        printf("\nError opening items file!\n");
        return;
    }

    printf("\nEnter Item Number: ");
    scanf("%d", &it.itemno);
    printf("Enter Item Name: ");
    scanf("%s", it.name);
    printf("Enter Date (dd mm yyyy): ");
    scanf("%d %d %d", &it.d.dd, &it.d.mm, &it.d.yy);
    printf("Enter Price: ");
    scanf("%f", &it.price);
    printf("Enter Quantity: ");
    scanf("%f", &it.qty);
    printf("Enter Tax Percentage: ");
    scanf("%f", &it.tax);
    printf("Enter Discount Percentage: ");
    scanf("%f", &it.dis);

    calculate(&it);

    fprintf(file, "%d %s %d %d %d %.2f %.2f %.2f %.2f %.2f\n", it.itemno, it.name, it.d.dd, it.d.mm, it.d.yy, it.price, it.qty, it.tax, it.dis, it.netamt);
    fclose(file);
    printf("\nItem added successfully!\n");
}

void displayItems() {
    FILE *file;
    struct item it;

    file = fopen("items.txt", "r");
    if (!file) {
        printf("\nNo items found in inventory!\n");
        return;
    }

    printf("\n--- Inventory Items ---\n");
    printf("%-8s %-20s %-12s %-8s %-10s %-8s %-8s\n", "ItemNo", "Name", "Date", "Price", "Quantity", "Tax", "Net Amount");

    while (fscanf(file, "%d %s %d %d %d %f %f %f %f %f", &it.itemno, it.name, &it.d.dd, &it.d.mm, &it.d.yy, &it.price, &it.qty, &it.tax, &it.dis, &it.netamt) != EOF) {
        printf("%-8d %-20s %02d/%02d/%4d %-8.2f %-10.2f %-8.2f %.2f\n", it.itemno, it.name, it.d.dd, it.d.mm, it.d.yy, it.price, it.qty, it.tax, it.netamt);
    }

    fclose(file);
}

void editItem() {
    FILE *file, *temp;
    int itemNo, found = 0;
    struct item it;

    printf("\nEnter the Item Number to Edit: ");
    scanf("%d", &itemNo);

    file = fopen("items.txt", "r");
    temp = fopen("temp.txt", "w");

    if (!file || !temp) {
        printf("\nError opening file!\n");
        return;
    }

    while (fscanf(file, "%d %s %d %d %d %f %f %f %f %f", &it.itemno, it.name, &it.d.dd, &it.d.mm, &it.d.yy, &it.price, &it.qty, &it.tax, &it.dis, &it.netamt) != EOF) {
        if (it.itemno == itemNo) {
            printf("\nEnter new details for the item:\n");
            printf("Enter Item Name: ");
            scanf("%s", it.name);
            printf("Enter Price: ");
            scanf("%f", &it.price);
            printf("Enter Quantity: ");
            scanf("%f", &it.qty);
            printf("Enter Tax Percentage: ");
            scanf("%f", &it.tax);
            printf("Enter Discount Percentage: ");
            scanf("%f", &it.dis);

            calculate(&it);
            found = 1;
        }
        fprintf(temp, "%d %s %d %d %d %.2f %.2f %.2f %.2f %.2f\n", it.itemno, it.name, it.d.dd, it.d.mm, it.d.yy, it.price, it.qty, it.tax, it.dis, it.netamt);
    }

    fclose(file);
    fclose(temp);

    remove("items.txt");
    rename("temp.txt", "items.txt");

    if (found) {
        printf("\nItem edited successfully!\n");
    } else {
        printf("\nItem not found!\n");
    }
}

void deleteItem() {
    FILE *file, *temp;
    int itemNo, found = 0;
    struct item it;

    printf("\nEnter the Item Number to Delete: ");
    scanf("%d", &itemNo);

    file = fopen("items.txt", "r");
    temp = fopen("temp.txt", "w");

    while (fscanf(file, "%d %s %d %d %d %f %f %f %f %f", &it.itemno, it.name, &it.d.dd, &it.d.mm, &it.d.yy, &it.price, &it.qty, &it.tax, &it.dis, &it.netamt) != EOF) {
        if (it.itemno != itemNo) {
            fprintf(temp, "%d %s %d %d %d %.2f %.2f %.2f %.2f %.2f\n", it.itemno, it.name, it.d.dd, it.d.mm, it.d.yy, it.price, it.qty, it.tax, it.dis, it.netamt);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(temp);

    remove("items.txt");
    rename("temp.txt", "items.txt");

    if (found) {
        printf("\nItem deleted successfully!\n");
    } else {
        printf("\nItem not found!\n");
    }
}

void generateBill() {
    FILE *file;
    int itemNo, found;
    float total = 0;
    struct item it;

    file = fopen("items.txt", "r");

    printf("\n--- Bill Generation ---\n");

    while (1) {
        printf("\nEnter Item Number (or -1 to finish): ");
        scanf("%d", &itemNo);
        if (itemNo == -1) break;

        rewind(file);
        found = 0;

        while (fscanf(file, "%d %s %d %d %d %f %f %f %f %f", &it.itemno, it.name, &it.d.dd, &it.d.mm, &it.d.yy, &it.price, &it.qty, &it.tax, &it.dis, &it.netamt) != EOF) {
            if (it.itemno == itemNo) {
                printf("\nItem: %s | Price: %.2f | Quantity: %.2f | Net Amount: %.2f\n", it.name, it.price, it.qty, it.netamt);
                total += it.netamt;
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("\nItem not found!\n");
        }
    }

    printf("\nTotal Bill Amount: %.2f\n", total);
    fclose(file);
}

void loadPredefinedItems() {
    FILE *file;
    file = fopen("items.txt", "a");
    struct item predefinedItems[] = {
        {101, "Milk", {1, 1, 2025}, 50.0, 10.0, 5.0, 10.0, 0.0},
        {102, "Bread", {1, 1, 2025}, 30.0, 15.0, 0.0, 5.0, 0.0},
        {103, "Eggs", {1, 1, 2025}, 6.0, 12.0, 0.0, 2.0, 0.0}
    };
    for (int i = 0; i < 3; i++) {
        calculate(&predefinedItems[i]);
        fprintf(file, "%d %s %d %d %d %.2f %.2f %.2f %.2f %.2f\n", predefinedItems[i].itemno, predefinedItems[i].name, predefinedItems[i].d.dd, predefinedItems[i].d.mm, predefinedItems[i].d.yy, predefinedItems[i].price, predefinedItems[i].qty, predefinedItems[i].tax, predefinedItems[i].dis, predefinedItems[i].netamt);
    }
    fclose(file);
}

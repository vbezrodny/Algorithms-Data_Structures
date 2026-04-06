#include "subj.h"

void PrintMenu() {
    cout << "\nMenu:\n";
    cout << "1. Add item\n";
    cout << "2. Print list\n";
    cout << "3. Get item\n";
    cout << "4. Remove item\n";
    cout << "5. Delete item\n";
    cout << "6. Insert item\n";
    cout << "7. Count items\n";
    cout << "8. Clear list\n";
    cout << "9. Get index of item\n";
    cout << "10. Sort list\n";
    cout << "11. Find item\n";
    cout << "0. Exit\n";
    cout << "\nEnter your choice: ";
}

void PrintFindMenu() {
    cout << "\nFind Menu:\n";
    cout << "1. Find by type\n";
    cout << "2. Find by performer\n";
    cout << "3. Find by director\n";
    cout << "4. Find by free space\n";
    cout << "\nEnter your choice: ";
}

int main() {
    SubjList *l = new SubjList;
	Base *item;

    int choice;
    int numb;
    ItemType type;
    string performer;
    string director;
    int requiredSpace;

    while (true) {
        PrintMenu();
        while (!(cin >> choice) || choice < 0) {
            cout << "Error! Wrong choice input!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1:
                type = SelectType();
                item = Base::Create(type);
                if (item) {
                    item->Input();
                    l->Add((Item *)item);
                }
                break;
            case 2:
                l->Print();
                break;
            case 3:
                cout << "\nEnter index: ";
                cin >> numb;
                item = (Base *)l->GetItem(numb);
                if (item) {
                    item->Print();
                } else {
                    cout << "Item at index " << numb << " not found\n";
                }
                break;
            case 4:
                cout << "Enter index: ";
                cin >> numb;
                item = (Base *)l->Remove(numb);
                if (item) {
                    cout << "Removed item:\n";
                    item->Print();
                    delete item;
                } else {
                    cout << "Item at index " << numb << " not found\n";
                }
                break;
            case 5:
                cout << "\nEnter index: ";
                cin >> numb;
                l->Delete(numb);
                break;
            case 6:
                cout << "\nEnter index: ";
                cin >> numb;
                type = SelectType();
                item = Base::Create(type);
                if (item) {
                    item->Input();
                    l->Insert((Item *)item, numb);
                }
                break;
            case 7:
                cout << "\nCount: " << l->Count() << "\n";
                break;
            case 8:
                l->Clear();
                break;
            case 9:
                cout << "\nEnter index: ";
                cin >> numb;
                item = (Base *)l->GetItem(numb);
                if (item) {
                    cout << "Index of item " << item << ": " << l->GetIndex((Item *)item) << "\n";
                } else {
                    cout << "Item at index " << numb << " not found\n";
                }
                break;
            case 10:
                l->Sort();
                break;
            case 11:
                l->Find();
                break;
            case 0:
                l->Clear();
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}

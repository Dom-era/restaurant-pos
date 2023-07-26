#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct MenuItem {
    int itemId;
    string itemName; // Change the type of itemName to string
    double price;
    int stock;

   MenuItem() : itemId(0), itemName(""), price(0.0), stock(0) {}
    MenuItem(int id, const string& name, double p, int s) : itemId(id), itemName(name), price(p), stock(s) {}
};

// Function to display the menu
void displayMenu(const map<int, MenuItem>& menu) {
    cout << "Menu:\n";
    for (map<int, MenuItem>::const_iterator it = menu.begin(); it != menu.end(); ++it) {
        const MenuItem& item = it->second;
        cout << item.itemName << " - KES" << fixed << setprecision(2) << item.price;
        cout << " - Stock: " << item.stock << endl;
    }
    cout << endl;
}

// Function to save the menu
void saveMenu(const map<int, MenuItem>& menu, const string& fileName) {
    ofstream file(fileName.c_str());
    if (!file.is_open()) {
        cout << "Error: Could not open file for saving the menu.\n";
        return;
    }

    for (map<int, MenuItem>::const_iterator it = menu.begin(); it != menu.end(); ++it) {
        const MenuItem& item = it->second;
        file << item.itemName << "," << fixed << setprecision(2) << item.price << "," << item.stock << endl;
    }

    file.close();
    cout << "Menu saved successfully.\n";
}


// Function to calculate the total cost of the order
double calculateTotal(const map<int, MenuItem>& menu, const map<string, int>& order) {
    double total = 0.0;
    map<string, int>::const_iterator orderIt = order.begin();
    while (orderIt != order.end()) {
        const string& itemName = orderIt->first;
        int quantity = orderIt->second;
        map<int, MenuItem>::const_iterator menuIt = menu.begin();
        while (menuIt != menu.end()) {
            if (menuIt->second.itemName == itemName) {
                total += menuIt->second.price * quantity;
                break;
            }
            ++menuIt;
        }
        ++orderIt;
    }
    return total;
}
// Function to generate a receipt
void generateReceipt(const map<int, MenuItem>& menu, const map<string, int>& order, double total,
                     const string& customerName, const string& customerAddress, const string& paymentDetails) {
    cout << "Receipt:\n";
    cout << "Restaurant: Dome Village" << endl;
    cout << "Customer Name: " << customerName << endl;
    cout << "Customer Address: " << customerAddress << endl;
    cout << "Payment Details: " << paymentDetails << endl;
    cout << "-----------------------------------" << endl;

    // Use iterators to access elements from the 'order' map.
    map<string, int>::const_iterator it;
    for (it = order.begin(); it != order.end(); ++it) {
        const string& itemName = it->first;
        int quantity = it->second;

        // Find the corresponding item in the 'menu' map using a traditional loop.
        map<int, MenuItem>::const_iterator menuIt;
        for (menuIt = menu.begin(); menuIt != menu.end(); ++menuIt) {
            if (menuIt->second.itemName == itemName) {
                cout << itemName << " x" << quantity << " - $" << fixed << setprecision(2) << menuIt->second.price * quantity << endl;
                break;
            }
        }
    }
    cout << "-----------------------------------" << endl;
    cout << "Total: $" << fixed << setprecision(2) << total << endl;
    cout << "Payment: " << paymentDetails << endl;
    cout << "-----------------------------------" << endl;
    cout << "Thank you for dining with us at Dome Village!" << endl;
    cout << "We hope to serve you again soon.\n";
}

// Function to save daily sales
void saveDailySales(const string& fileName, const string& data) {
    ofstream file(fileName.c_str(), ios::app); // Use c_str() to convert fileName to C-style string
    if (file.is_open()) {
        file << data << endl;
        file.close();
        cout << "Daily sales saved successfully.\n";
    } else {
        cout << "Error: Could not open file for saving daily sales.\n";
    }
}

int main() {
    // Define the menu as a dictionary (map) with integer keys
    map<int, MenuItem> menu;
    menu[1] = MenuItem(1, "Burger", 500.00, 10);
    menu[2] = MenuItem(2, "Pizza", 1000.00, 15);
    menu[3] = MenuItem(3, "Salad", 300.00, 20);
    menu[4] = MenuItem(4, "Samosas", 60.00, 20);
    menu[5] = MenuItem(5, "fried-plantain", 400.00, 20);
    menu[6] = MenuItem(6, "meat-curry", 600.00, 20);
    menu[7] = MenuItem(7, "nyama-choma", 1200.00, 20);
    menu[8] = MenuItem(8, "chicken-wings", 300.00, 20);
    menu[9] = MenuItem(9, "fries", 200.00, 20);
    menu[10] = MenuItem(10, "loaded-fries", 500.00, 20);
    menu[11] = MenuItem(11, "chapati", 20.00, 20);

    // Display menu
    displayMenu(menu);

    // Save the menu to a file
    saveMenu(menu, "menu.txt");

    // Initialize order and customer details
    map<string, int> order;
    string customerName, customerAddress, paymentDetails;

 
// Order process
while (true) {
    int itemId; // Use integer to accept the itemId
    int quantity;
    cout << "Enter item ID (or 0 to finish the order): ";
    cin >> itemId;

    if (itemId == 0) {
        break;
    }

    bool itemFound = false;
    map<int, MenuItem>::iterator it; // Use iterator to access 'menu' elements.
    for (it = menu.begin(); it != menu.end(); ++it) {
        const MenuItem& item = it->second;
        if (item.itemId == itemId) {
            itemFound = true;
            cout << "Enter quantity: ";
            cin >> quantity;

            if (quantity <= 0) {
                cout << "Invalid quantity. Please try again.\n";
            } else if (quantity > item.stock) {
                cout << "Insufficient stock. Available stock: " << item.stock << endl;
            } else {
                // Use itemName instead of itemId in the order
                order[item.itemName] += quantity;
                it->second.stock -= quantity; // Modify stock using the iterator.
            }
            break;
        }
    }

    if (!itemFound) {
        cout << "Invalid item ID. Please try again.\n";
    }
}




    // Calculate total
    double total = calculateTotal(menu, order);

// Capture customer details
cin.ignore(); // Clear any newline characters left in the buffer
cout << "Enter customer name: ";
getline(cin, customerName);
cout << "Enter customer address: ";
getline(cin, customerAddress);

    // Generate receipt
    generateReceipt(menu, order, total, customerName, customerAddress, paymentDetails);

    // Save daily sales
    ostringstream oss;
    oss << fixed << setprecision(2) << total;
    string salesData = customerName + "," + customerAddress + "," + oss.str();
    saveDailySales("daily_sales.txt", salesData);

    return 0;
}
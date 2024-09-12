#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>
using namespace std;

// Function to get validated integer input from the user
int getValidatedInput(const string& prompt) {
    int input;
    cout << prompt;
    while (!(cin >> input)) { // Loop until valid input is received
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Invalid input. Input a number. \n\n" << prompt;
    }
    cin.ignore(); 
    return input;
}

class Product {
private:
    string prodID, prodName;
    double prodPrice;
    int cartQty = 0; 

public:  
    Product(string i, string n, double p) {
    	prodID = i;
		prodName = n;
		prodPrice = p;
		}

    // Getters for product details
    string getID() const { return prodID; }
    string getName() const { return prodName; }
    double getPrice() const { return prodPrice; }
    int getQty() const { return cartQty; }

    // Display product information
    void displayProduct() const {
        cout << setw(15) << prodID 
             << setw(15) << prodName 
             << setw(10) << fixed << setprecision(1) << prodPrice 
             << endl;
    }

    // Display product information in the cart
    void displayCartItem() const {
        cout << setw(15) << prodID 
             << setw(15) << prodName 
             << setw(10) << fixed << setprecision(1) << prodPrice 
             << setw(13) << cartQty 
             << endl;
    }

    // Calculate total price for this product in the cart
    double getTotalPrice() const {
        return prodPrice * cartQty;
    }

    // Set quantity of product in the cart
    void setCartQty(int qty) { cartQty = qty; }
};

class Inventory {
private:
    vector<Product> products = {
        {"ABC", "Apple", 250.0},
        {"DEF", "Banana", 150.0},
        {"GHI", "Strawberry", 330.0},
        {"JKL", "Grapes", 200.0},
        {"MNO", "Mango", 399.0},
        {"PQR", "Orange", 350.0},
        {"STU", "Watermelon", 240.0},
        {"VWX", "Melon", 190.0},
        {"YZ", "Pineapple", 299.0},
        
    };

public:
    // Display all products in the inventory
    void displayAllProducts() const {
        cout << setw(15) << "Product ID" 
             << setw(15) << "Products" 
             << setw(10) << "Price" 
             << endl << endl;
        for (const auto& p : products) {
            p.displayProduct();
        }
    }

    // Find a product by ID
    Product* findProduct(const string& id) {
        for (auto& p : products) {
            if (p.getID() == id) return &p;
        }
        return nullptr;
    }
};

class ShoppingCart {
private:
    vector<Product> items; 

public:
    // Add a product to the cart
    void addProduct(const Product& product) {
        for (auto& item : items) {
            if (item.getID() == product.getID()) {
                item.setCartQty(item.getQty() + product.getQty());
                return;
            }
        }
        items.push_back(product); // Add new product if not already in cart
    }

    // Display all items in the cart
    void displayCart() const {
        cout << setw(15) << "Product ID" 
             << setw(15) << "Product" 
             << setw(10) << "Price" 
             << setw(13) << "Quantity" 
             << endl;
        for (const Product& item : items) {
            item.displayCartItem();
        }
    }

    // Calculate the total price of all items in the cart
    double calculateTotal() const {
        double total = 0.0;
        for (const Product& item : items) {
            total += item.getTotalPrice();
        }
        return total;
    }

    // Get all items in the cart
    vector<Product> getItems() const { return items; }
};

class Order {
private:
    vector<Product> items; 
    int orderID; 
    static int orderCount; 

public:
    Order(const vector<Product>& cartItems) : items(cartItems) {
    	static int orderCount = 1;
    	orderID = orderCount++;
    }

    // Display the order details
    void displayOrder() const {
        cout << "\tOrder ID: " << orderID << endl;
        cout << "\tTotal Amount: " << fixed << setprecision(2) << calculateTotal() << endl;
        cout << setw(15) << "ID" 
             << setw(15) << "Product" 
             << setw(10) << "Price" 
             << setw(13) << "Quantity" 
             << endl;
        for (const auto& item : items) {
            item.displayCartItem();
        }
    }

    // Calculate the total price of the order
    double calculateTotal() const {
        double total = 0.0;
        for (const auto& item : items) {
            total += item.getTotalPrice();
        }
        return total;
    }
};

int main() {
    Inventory inv; 
    ShoppingCart cart; 
    vector<Order> orders; 
    int choice; 
    string id;
    char checkout, addP; 

    do {
        system("CLS"); 

        // Display the menu
        cout << "\t   SHOP\n\n";
        cout << "  1. View Products\n";
        cout << "  2. View Shopping Cart\n";
        cout << "  3. View Orders\n";
        cout << "  4. Exit\n";
        choice = getValidatedInput("  Enter choice: ");
        cout << endl;
        
        switch (choice) {
            case 1: {
                // Display all products and allow adding to cart
                inv.displayAllProducts();
                do {
                    cout << "\n  Enter the ID of the product you want to add to the shopping cart: ";
                    cin >> id;
                    for (char& c : id) c = toupper(c); // Convert ID to uppercase

                    Product* product = inv.findProduct(id);
                    if (product) {
                        int qty = getValidatedInput("  Enter quantity: ");
                        if (qty < 1) {
                            cout << "  Input 1 or more.\n";
                        } else {
                            product->setCartQty(qty);
                            cart.addProduct(*product);
                            cout << "  Product added successfully!" << endl;
                        }
                    } else {
                        cout << "  Product not found." << endl;
                    }

                    cout << "\n  Add more? (Y/N): ";
                    cin >> addP;
                } while (addP == 'Y' || addP == 'y');
                break;
            }

            case 2: {
                // Display cart and handle checkout
                cart.displayCart();
                cout << "\n  Do you want to checkout all the products? (Y/N): ";
                cin >> checkout;
                
                if (checkout == 'Y' || checkout == 'y') {
                    Order newOrder(cart.getItems());
                    orders.push_back(newOrder);
                    cout << "\n       ================== ORDER ==================\n\n";
                    newOrder.displayOrder();
                    cout << "\n  You have successfully checked out the products!\n";
                    cart = ShoppingCart(); // Clear cart after checkout
                }
                break;
            }

            case 3: {
                cout << "\n       ================== ORDERS ==================\n\n";
                for (const auto& order : orders) {
                    order.displayOrder();
                    cout << endl;
                }
                break;
            }

            case 4: {
                cout << "\nExiting..." << endl;
                return 0;
            }

            default: {
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
        }
        cout << endl;
        system("PAUSE");
    } while (choice != 4); 

    return 0;
}

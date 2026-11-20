 #include <iostream>
#include <string>
#include <cctype>
using namespace std;

// Week 1: references, pointers and string operations
void swapRef(double &a, double &b) { double t = a; a = b; b = t; }
void swapPtr(double *a, double *b) { double t = *a; *a = *b; *b = t; }
void showTextInfo(const string &s) {
    cout << "Name length: " << s.length() << "\n";
    cout << "Uppercase: ";
    for (char ch : s) cout << static_cast<char>(toupper(static_cast<unsigned char>(ch)));
    cout << "\nContains 'an': " << (s.find("an") != string::npos ? "Yes" : "No") << "\n";
    cout << "First part: " << s.substr(0, s.size() < 4 ? s.size() : 4) << "\n";
    cout << "Comparison with 'bank': " << s.compare("bank") << "\n";
    int frequency[26] = {0};
    for (char ch : s) if (isalpha(static_cast<unsigned char>(ch))) ++frequency[tolower(static_cast<unsigned char>(ch)) - 'a'];
    cout << "Letter frequencies: ";
    for (int i = 0; i < 26; ++i) if (frequency[i]) cout << char('a' + i) << ":" << frequency[i] << " ";
    cout << "\n";
    bool palindrome = true;
    for (size_t i = 0; i < s.size() / 2; ++i)
        if (s[i] != s[s.size() - 1 - i]) { palindrome = false; break; }
    cout << "Palindrome: " << (palindrome ? "Yes" : "No") << "\n";
}

// Week 2: inline function, overloading, defaults and min/max by reference/pointer
inline double serviceCharge(double amount) { return amount * 0.01; }
double calculateInterest(double principal, double rate = 4.0, int years = 1) {
    return principal * rate * years / 100.0;
}
int fee(int transactions) { return transactions * 2; } 
  
  double fee(double amount) { return amount * 0.01; }
void minMax(const double values[], int n, double &mn, double &mx) {
    mn = mx = values[0];
    for (int i = 1; i < n; ++i) { if (values[i] < mn) mn = values[i]; if (values[i] > mx) mx = values[i]; }
}
void minMaxPtr(const double values[], int n, double *mn, double *mx) {
    *mn = *mx = values[0];
    for (int i = 1; i < n; ++i) { if (values[i] < *mn) *mn = values[i]; if (values[i] > *mx) *mx = values[i]; }
}

// Week 3: a small class to demonstrate an array of objects, object passing,
// returning an object and method chaining through the this pointer.
class BranchPoint {
    int x, y;
public:
    BranchPoint(int a = 0, int b = 0) : x(a), y(b) {}
    BranchPoint add(const BranchPoint &p) const { return BranchPoint(x + p.x, y + p.y); }
    BranchPoint& setX(int value) { this->x = value; return *this; }
    void show() const { cout << "(" << x << ", " << y << ")\n"; }
};

class Dashboard;
class BankAccount {
private:
    string owner;
    int accountNumber;
    double balance;
    double *transactions;
    int transactionCount;
    int capacity;
    static int liveAccounts;

    void record(double amount) {
        if (transactionCount == capacity) {
            int newCapacity = capacity * 2;
            double *temp = new double[newCapacity];
            for (int i = 0; i < transactionCount; ++i) temp[i] = transactions[i];
            delete[] transactions;
            transactions = temp;
            capacity = newCapacity;
        }
        transactions[transactionCount++] = amount;
    }
public:
    // Week 4: default, parameterized and delegating constructors
    BankAccount() : BankAccount("Guest", 0, 0.0) {}
    BankAccount(string name, int number, double openingBalance = 0.0)
        : owner(name), accountNumber(number), balance(openingBalance > 0 ? openingBalance : 0),
          transactions(new double[2]), transactionCount(0), capacity(2) {
        ++liveAccounts;
    }
    // Deep-copy constructor
    BankAccount(const BankAccount &other)
        : owner(other.owner), accountNumber(other.accountNumber), balance(other.balance),
          transactions(new double[other.capacity]), transactionCount(other.transactionCount), capacity(other.capacity) {
        for (int i = 0; i < transactionCount; ++i) transactions[i] = other.transactions[i];
        ++liveAccounts;
    }
    // Deep-copy assignment operator
    BankAccount& operator=(const BankAccount &other) {
        if (this != &other) {
            double *temp = new double[other.capacity];
            for (int i = 0; i < other.transactionCount; ++i) temp[i] = other.transactions[i];
            delete[] transactions;
            transactions = temp;
            capacity = other.capacity;
            transactionCount = other.transactionCount;
            owner = other.owner;
            accountNumber = other.accountNumber;
            balance = other.balance;
        }
        return *this;
    }
    ~BankAccount() { delete[] transactions; --liveAccounts; }

    void deposit(double amount) {
        if (amount > 0) { balance += amount; record(amount); cout << "Deposit successful.\n"; }
        else cout << "Enter a positive amount.\n";
    }
    bool withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount; record(-amount); cout << "Withdrawal successful.\n"; return true;
        }
        cout << "Withdrawal denied. Check amount/balance.\n"; return false;
    }
    double getBalance() const { return balance; }
    string getOwner() const { return owner; }
    int getAccountNumber() const { return accountNumber; }
    int getTransactionCount() const { return transactionCount; }

    // Week 6: arithmetic, comparison, increment and subscript operators
    BankAccount operator+(const BankAccount &other) const {
        BankAccount result("Combined", 0, balance + other.balance);
        return result;
    }
    bool operator==(const BankAccount &other) const { return accountNumber == other.accountNumber; } 
   
   BankAccount& operator++() { ++balance; return *this; } // prefix
    BankAccount operator++(int) { BankAccount old(*this); ++balance; return old; } // postfix
    double& operator[](int index) {
        if (index < 0 || index >= transactionCount) {
            cout << "Transaction index out of range.\n";
            static double zero = 0;
            return zero;
        }
        return transactions[index];
    }

    // Week 5: friend function and friend class
    friend ostream& operator<<(ostream &out, const BankAccount &account);
    friend void transfer(BankAccount &from, BankAccount &to, double amount);
    friend class Dashboard;
    static int getLiveAccounts() { return liveAccounts; }
};
int BankAccount::liveAccounts = 0;

ostream& operator<<(ostream &out, const BankAccount &account) {
    out << "Account holder: " << account.owner << "\nAccount number: " << account.accountNumber
        << "\nBalance: Rs. " << account.balance;
    return out;
}
void transfer(BankAccount &from, BankAccount &to, double amount) {
    if (amount > 0 && amount <= from.balance) {
        from.balance -= amount; from.record(-amount);
        to.balance += amount; to.record(amount);
        cout << "Transfer successful.\n";
    } else cout << "Transfer failed.\n";
}
class Dashboard {
public:
    void showPrivateBalance(const BankAccount &account) const {
        cout << "[Dashboard] Verified balance: Rs. " << account.balance << "\n";
    }
};

int main() {
    cout << "===== BANKING AND ACCOUNT MANAGEMENT SYSTEM =====\n";
    string name;
    int number;
    cout << "Enter account holder name: ";
    getline(cin, name);
    cout << "Enter account number: ";
    cin >> number;
    BankAccount account(name, number, 1000.0);
    BankAccount savings("Savings", number + 1, 500.0);

    showTextInfo(account.getOwner());
    double x = 10, y = 20;
    swapRef(x, y); // reference example
    cout << "Reference swap demonstration: " << x << ", " << y << "\n";
    swapPtr(&x, &y); // pointer example
    cout << "Pointer swap demonstration: " << x << ", " << y << "\n";
    BranchPoint branches[2] = {BranchPoint(1, 2), BranchPoint(3, 4)}; // array of objects
    BranchPoint total = branches[0].add(branches[1]); // pass and return object
    cout << "Combined branch point: "; total.show();
    branches[0].setX(9); // this pointer and method call
    cout << "Updated first branch point: "; branches[0].show();

    int choice;
    do {
        cout << "\n--- MENU ---\n1. Deposit\n2. Withdraw\n3. Display account\n4. Calculate interest\n"
             << "5. Transfer Rs. 100 to sample savings account\n6. Show transaction history\n"
             << "7. Show dashboard and account count\n8. Demonstrate operators\n0. Exit\nChoice: ";
        cin >> choice;
        switch (choice) {
            case 1: { double amount; cout << "Amount: "; cin >> amount; account.deposit(amount); break; }
            case 2: { double amount; cout << "Amount: "; cin >> amount; account.withdraw(amount); break; }
            case 3: cout << account << "\n"; break;
            case 4: {
                cout << "Interest at default rate (4%, 1 year): Rs. " << calculateInterest(account.getBalance()) << "\n";
                cout << "Interest at custom rate (6%, 2 years): Rs. " << calculateInterest(account.getBalance(), 6.0, 2) << "\n";
                cout << "Overloaded fee examples: " << fee(3) << " and Rs. " << fee(500.0) << "\n";
                cout << "Inline service charge on Rs. 500: Rs. " << serviceCharge(500) << "\n";
                break;
            }
            case 5: transfer(account, savings, 100.0); break;
            case 6: {
                if (account.getTransactionCount() == 0) cout << "No transactions yet.\n";
                for (int i = 0; i < account.getTransactionCount(); ++i)
                    cout << i + 1 << ". " << (account[i] >= 0 ? "Credit: Rs. " : "Debit: Rs. ") << (account[i] >= 0 ? account[i] : -account[i]) << "\n";
                double lo, hi;
                if (account.getTransactionCount() > 0) {
                    minMax(&account[0], account.getTransactionCount(), lo, hi);
                    cout << "Transaction range using references: " << lo << " to " << hi << "\n";
                    minMaxPtr(&account[0], account.getTransactionCount(), &lo, &hi);
                    cout << "Transaction range using pointers: " << lo << " to " << hi << "\n";
                }
                break;
            }
            case 7: Dashboard().showPrivateBalance(account); cout << "Live accounts: " << BankAccount::getLiveAccounts() << "\n"; break;
            case 8: {
                BankAccount combined = account + savings;
                cout << "Combined balance: Rs. " << combined.getBalance() << "\n";
                cout << "Same account number? " << (account == savings ? "Yes" : "No") << "\n";
                ++account;
                cout << "Balance after prefix ++ (adds Rs. 1): Rs. " << account.getBalance() << "\n";
                account++;
                cout << "Balance after postfix ++ (adds Rs. 1): Rs. " << account.getBalance() << "\n";
                BankAccount copy(account); // copy constructor demonstration
                cout << "Copied account balance: Rs. " << copy.getBalance() << "\n";
                BankAccount assigned;
                assigned = account; // overloaded assignment operator
                cout << "Assigned account balance: Rs. " << assigned.getBalance() << "\n";
                break;
            }
            case 0: cout << "Thank you for using the system.\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

class VendingMachine; // forward declaration

// ---------- Item ----------
class Item {
    int price;
    string name;
public:
    Item(int p, string n) : price(p), name(n) {}
    int getPrice() { return price; }
    string getName() { return name; }
};

// ---------- Coin ----------
class Coin {
    int value;
public:
    Coin(int v) : value(v) {}
    int getValue() { return value; }
};

class Cent : public Coin { public: Cent() : Coin(1) {} };
class Nickel : public Coin { public: Nickel() : Coin(5) {} };
class Dime : public Coin { public: Dime() : Coin(10) {} };

// ---------- State Interface ----------
class MachineState {
public:
    virtual void insertCoin(VendingMachine&, Coin*) = 0;
    virtual void selectProduct(VendingMachine&, Item*) = 0;
    virtual void dispenseProduct(VendingMachine&) = 0;
    virtual void refund(VendingMachine&) = 0;
    virtual ~MachineState() {}
};

// ---------- Concrete States ----------
class IdleState : public MachineState {
public:
    void insertCoin(VendingMachine&, Coin*)  override;
    void selectProduct(VendingMachine&, Item*) override;
    void dispenseProduct(VendingMachine&) override;
    void refund(VendingMachine&) override;
};

class CoinInsertedState : public MachineState {
public:
    void insertCoin(VendingMachine&, Coin*)  override;
    void selectProduct(VendingMachine&, Item*)  override;
    void dispenseProduct(VendingMachine&) override;
    void refund(VendingMachine&) override;
};

class SelectProductState : public MachineState {
public:
    void insertCoin(VendingMachine&, Coin*) override;
    void selectProduct(VendingMachine&, Item*)  override;
    void dispenseProduct(VendingMachine&)  override;
    void refund(VendingMachine&) override;
};

class DispenseProductState : public MachineState {
public:
    void insertCoin(VendingMachine&, Coin*)  override;
    void selectProduct(VendingMachine&, Item*)  override;
    void dispenseProduct(VendingMachine&) override;
    void refund(VendingMachine&) override;
};

// ---------- VendingMachine ----------
class VendingMachine {
    int balance;
    Item* item;
    MachineState* currentState;
    IdleState idleState;
    CoinInsertedState coinInsertedState;
    SelectProductState selectProductState;
    DispenseProductState dispenseProductState;
    unordered_map<string, int> itemMap;

    VendingMachine() {
        balance = 0;
        item = nullptr;
        currentState = &idleState;
    }
    static VendingMachine* instance;

public:
    static VendingMachine* getInstance() {
        if (!instance) instance = new VendingMachine();
        return instance;
    }

    int getQuantityOfItem(string itemName) {
        return itemMap[itemName];
    }

    void refilItems(Item* item, int quantity) {
        itemMap[item->getName()] += quantity;
    }
    void removeItems(Item* item, int quantity) {
        itemMap[item->getName()] -= quantity;
        if (itemMap[item->getName()] <= 0) {
            itemMap.erase(item->getName());
        }
    }
    void insertCoin(Coin* coin) { currentState->insertCoin(*this, coin); }
    void selectProduct(Item* productName) { currentState->selectProduct(*this, productName); }
    void dispenseProduct() { currentState->dispenseProduct(*this); }
    void refund() { currentState->refund(*this); }

    int getBalance() const { return balance; }
    void setBalance(int b) { balance = b; }
    Item getItem() const { return *item; }
    void setItem(Item& i) { item = &i; }
    void setState(MachineState* state) { currentState = state; }
    IdleState* getIdleState() { return &idleState; }
    CoinInsertedState* getCoinInsertedState() { return &coinInsertedState; }
    SelectProductState* getSelectProductState() { return &selectProductState; }
    DispenseProductState* getDispenseProductState() { return &dispenseProductState; }
};

VendingMachine* VendingMachine::instance = nullptr;

// ---------- State Implementations ----------
void IdleState::insertCoin(VendingMachine& machine, Coin* coin) {
    int currBalance = machine.getBalance() + coin->getValue();
    machine.setBalance(currBalance);
    cout << "Coin inserted. Balance: " << currBalance << endl;
    machine.setState(machine.getCoinInsertedState());
}

void IdleState::selectProduct(VendingMachine& machine, Item* productName) {
    cout << "Insert coin first." << endl;
}

void IdleState::dispenseProduct(VendingMachine& machine) {
    cout << "Insert coin first." << endl;
}

void IdleState::refund(VendingMachine& machine) {
    cout << "Insert coin first." << endl;
}

void CoinInsertedState::insertCoin(VendingMachine& machine, Coin* coin) {
    int currBalance = machine.getBalance() + coin->getValue();
    machine.setBalance(currBalance);
    cout << "Coin inserted. Balance: " << currBalance << endl;
}

void CoinInsertedState::selectProduct(VendingMachine& machine, Item* productName) {
    if (machine.getBalance() >= productName->getPrice()) {
        if(machine.getQuantityOfItem(productName->getName()) <= 0) {
            cout << "Selected product is out of stock." << endl;
            return;
        }
        machine.setItem(*productName);
        machine.setState(machine.getSelectProductState());
        machine.removeItems(productName, 1);
        cout << "Product selected: " << productName->getName() << endl;
    } else {
        cout << "Insufficient balance." << endl;
    }
}

void CoinInsertedState::dispenseProduct(VendingMachine& machine) {
    cout << "Select a product first." << endl;
}

void CoinInsertedState::refund(VendingMachine& machine) {
    cout << "Refunding " << machine.getBalance() << endl;
    machine.setBalance(0);
    machine.setState(machine.getIdleState());
}

void SelectProductState::insertCoin(VendingMachine& machine, Coin* coin) {
    cout << "Coin cannot be added in this state." << endl;
}

void SelectProductState::selectProduct(VendingMachine& machine, Item* productName) {
    machine.setItem(*productName);
    machine.setState(machine.getSelectProductState());
    cout << "Product selected: " << productName->getName() << endl;
}

void SelectProductState::dispenseProduct(VendingMachine& machine) {
    int balance = machine.getBalance();
    balance -= machine.getItem().getPrice();
    cout << "Dispensing " << machine.getItem().getName() << endl;
    machine.setBalance(balance);
    machine.setState(machine.getDispenseProductState());
    cout << "Remaining balance: " << balance << endl;
}

void SelectProductState::refund(VendingMachine& machine) {
    cout << "Refunding " << machine.getBalance() << endl;
    machine.setBalance(0);
    machine.setState(machine.getIdleState());
}

void DispenseProductState::insertCoin(VendingMachine& machine, Coin* coin) {
    cout << "Coin cannot be added in this state." << endl;
}

void DispenseProductState::selectProduct(VendingMachine& machine, Item* productName) {
    cout << "Product cannot be selected." << endl;
}

void DispenseProductState::dispenseProduct(VendingMachine& machine) {
    cout << "Item already being dispensed." << endl;
}

void DispenseProductState::refund(VendingMachine& machine) {
    if(machine.getBalance() <= 0) {
        cout << "No balance to refund." << endl;
    } else {
        cout << "Refunding " << machine.getBalance() << endl;
        machine.setBalance(0);
    }
    machine.setState(machine.getIdleState());
}

// ---------- Main ----------
int main() {
    VendingMachine* vendingMachine = VendingMachine::getInstance();

    vendingMachine->insertCoin(new Coin(25));
    Item* item1 = new Item(20, "Soda");
    vendingMachine->refilItems(item1, 5);
    vendingMachine->selectProduct(item1);
    vendingMachine->dispenseProduct();
    vendingMachine->refund();

    return 0;
}

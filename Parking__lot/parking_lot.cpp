#include<bits/stdc++.h>
using namespace std;

enum VehicleType { MOTORCYCLE, CAR, BUS };
int getRandomNumber() {
    return rand() % 1000 + 1;
}

class Vehicle {
    int licenceplate;
    VehicleType type;
public:
    Vehicle(int lp, VehicleType t) {
        licenceplate = lp;
        type = t;
    }
    int getLicencePlate() const { return licenceplate; }
    VehicleType getType() const { return type; }
};

class Car : public Vehicle {
public:
    Car(int lp) : Vehicle(lp, CAR) {}
};

class Bus : public Vehicle {
public:
    Bus(int lp) : Vehicle(lp, BUS) {}
};

class Motorcycle : public Vehicle {
public:
    Motorcycle(int lp) : Vehicle(lp, MOTORCYCLE) {}
};

class ParkingSlot {
    int slotNumber;
    bool occupied;
    VehicleType type;
    Vehicle* vehicle;

    public:
    ParkingSlot(int num, VehicleType t) {
        slotNumber = num;
        type = t;
        occupied = false;
        vehicle = nullptr;
    }

    VehicleType getType() const {
        return type;
    }

    int getSlotNumber() {
        return slotNumber;
    }

    bool isOccupied() const {
        return occupied;
    }

    Vehicle* getVehicle() {
        return vehicle;
    }

    void parkVehicle(Vehicle* v) {
        vehicle = v;
        occupied = true;
    }

    void removeVehicle() {
        vehicle = nullptr;
        occupied = false;
    }
};

class ParkingFloor {
    vector<ParkingSlot*> slots;
    unordered_map<int, ParkingSlot*> slotMap;
    public:

    void addParkingSlot(VehicleType t, int slotNumber) {
        ParkingSlot* slot = new ParkingSlot(slotNumber, t);
        slotMap[slotNumber] = slot;
        slots.push_back(slot);
    }

    ParkingSlot* getSlot(int slotNumber) {
        if(slotMap.find(slotNumber) != slotMap.end()) {
            return slotMap[slotNumber];
        }
        return nullptr;
    }

    int availableSlot(Vehicle* v) {
        for(auto slot : slots) {
            if(!slot->isOccupied() && slot->getType() == v->getType()) {
                cout << "Available slot for vehicle " << v->getLicencePlate() << " is " << slot->getSlotNumber() << endl;
                return slot->getSlotNumber();
            }
        }
        return -1;
    }

    void parkVehicle(Vehicle* v, int slotNumber) {
        if(slotMap.count(slotNumber) && !slotMap[slotNumber]->isOccupied()) {
            slotMap[slotNumber]->parkVehicle(v);
            cout << "Vehicle " << v->getLicencePlate() << " parked in slot " << slotNumber << endl;
        } else {
            cout << "Could not park vehicle " << v->getLicencePlate() << " in slot " << slotNumber << endl;
        }
    }

    void removeVehicle(int slotNumber) {
        for(auto slot : slots) {
            if(slot->getSlotNumber() == slotNumber && slot->isOccupied()) {
                cout << "Vehicle " << slot->getVehicle()->getLicencePlate() << " removed from slot " << slotNumber << endl;
                slot->removeVehicle();
                return;
            }
        }
        cout << "No vehicle found in slot " << slotNumber << endl;
    }
};

class Payment {
    public:
    virtual void paymentMethod() = 0;
};

class CreditCardPayment : public Payment {
    void paymentMethod() override {
        cout << "Done payment with Credit Card" << endl;
    }
};

class PayPalPayment : public Payment {
    void paymentMethod() override {
        cout << "Done payment with PayPal" << endl;
    }
};

class PaymentStrategy {
    public:
    virtual void calculateFare(int duration) = 0;
};

class FixedRatePaymentStrategy : public PaymentStrategy {
    void calculateFare(int duration) override {
        int rate = 10;
        cout << "Total fare: " << rate * duration << endl;
    }
};

class HourlyPaymentStrategy : public PaymentStrategy {
    void calculateFare(int duration) override {
        int rate = 5; 
        cout << "Total fare: " << rate * duration << endl;
    }
};

class Ticket {
    int ticketId;
    Vehicle* v;
    ParkingFloor* floor;
    int duration;
    ParkingSlot* slot;
    PaymentStrategy* paymentStrategy;

    public: 
    Vehicle* getVehicle() {
        return v;
    }

    ParkingFloor* getFloor() const {
        return floor;
    }

    int getDuration() const {
        return duration;
    }

    ParkingSlot* getSlot() const {
        return slot;
    }

    int getTicketId() const {
        return ticketId;
    }

    PaymentStrategy* getPaymentStrategy() const {
        return paymentStrategy;
    }

    Ticket (int tid, Vehicle* vehicle, int dur, ParkingSlot* s, PaymentStrategy* strategy, ParkingFloor* f) {
        v = vehicle;
        ticketId = tid;
        duration = dur;
        slot = s;
        paymentStrategy = strategy;
        floor = f;
    }

    void displayTicket() {
        cout << "Ticket Info: Vehicle " << v->getLicencePlate() << ", Duration " << duration << " hours, Slot " << slot->getSlotNumber() << endl;
    }
};

class TicketManager {
    unordered_map<int, Ticket*> tickets;

public:
    void addTicket(Ticket* ticket) {
        tickets[ticket->getTicketId()] = ticket;
    }

    Ticket* getTicket(int id) {
        if (tickets.find(id) != tickets.end()) {
            return tickets[id];
        }
        return nullptr;
    }

    void removeTicket(int id) {
        tickets.erase(id);
    }
};

class ParkingLotManagement {
    static ParkingLotManagement* instance;
    vector<ParkingFloor*> floors;
    TicketManager* ticketManager;

    ParkingLotManagement() {
        ticketManager = new TicketManager();
    }
public:

    static ParkingLotManagement* getInstance() {
        if(instance==nullptr) {
            instance = new ParkingLotManagement();
        }
        return instance;
    }

    Ticket* getTicket(int id) {
        return ticketManager->getTicket(id);
    }

    void addParkingFloor() {
        ParkingFloor* floor = new ParkingFloor();
        floors.push_back(floor);
    }

        void removeVehicle(Ticket* t, Payment* payment) {
            if (t == nullptr) {
                cout << "Invalid ticket for removal." << endl;
                return;
            }
            t->getPaymentStrategy()->calculateFare(t->getDuration());
            cout << "Processing payment for ticket " << t->getTicketId() << endl;

            payment->paymentMethod();
            t->getFloor()->removeVehicle(t->getSlot()->getSlotNumber());
            ticketManager->removeTicket(t->getTicketId());
        }

        void addVehicle(Vehicle* v, int duration) {
            for(auto floor: floors) {
                int slotNumber = floor->availableSlot(v);
                if(slotNumber!=-1) {
                    ParkingSlot* slot = floor->getSlot(slotNumber);
                    floor->parkVehicle(v, slotNumber); 
                    Ticket* ticket = new Ticket(getRandomNumber(), v, duration, slot, nullptr, floor);
                    ticketManager->addTicket(ticket);
                    ticket->displayTicket();
                    return;
                }
            }
            cout << "Sorry, no available parking for vehicle " << v->getLicencePlate() << endl;
        }

        void addParkingSlotFloor(VehicleType type, int slotNumber, int floorNumber) {
            if (floorNumber < floors.size()) {
                floors[floorNumber]->addParkingSlot(type, slotNumber);
            } else {
                cout << "Invalid floor number." << endl;
            }
        }
};

ParkingLotManagement* ParkingLotManagement::instance = nullptr;

int main() {
    srand(time(0));  

    ParkingLotManagement* lot = ParkingLotManagement::getInstance();

    // Add 2 floors
    lot->addParkingFloor();
    lot->addParkingSlotFloor(CAR, 1, 0);
    lot->addParkingSlotFloor(MOTORCYCLE, 2, 0);
    lot->addParkingSlotFloor(BUS, 3, 0);

    lot->addParkingFloor();
    // Add slots to floor 1
    lot->addParkingSlotFloor(CAR, 4, 1);
    lot->addParkingSlotFloor(MOTORCYCLE, 5, 1);

    // Create vehicles
    Vehicle* car1 = new Car(101);
    Vehicle* bike1 = new Motorcycle(202);
    Vehicle* bus1 = new Bus(303);

    // Add vehicles to lot
    cout << "\n--- Vehicle entry process ---\n";
    lot->addVehicle(car1, 2);   // 2 hours
    lot->addVehicle(bike1, 3);  // 3 hours
    lot->addVehicle(bus1, 1);   // 1 hour

    // Pick one ticket (in real system you'd fetch by ID)
    cout << "\n--- Vehicle exit process ---\n";
    Ticket* exitTicket = lot->getTicket(5);

    // Choose payment methods
    Payment* paymentMethod = new CreditCardPayment();

    // Remove vehicle
    lot->removeVehicle(exitTicket, paymentMethod);

    return 0;
}

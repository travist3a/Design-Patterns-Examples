#include <iostream>
#include <string>
#include <vector>
#include <memory>

//
// ===========================
// Factory Method Pattern
// ===========================
// Purpose: Define an interface for creating an object,
// but let subclasses decide which class to instantiate.
// Used here to return different types of Transport (Truck, Ship)
// without modifying client code.
//

// Product Interface
class Transport {
public:
    virtual std::string deliver() const = 0;
    virtual ~Transport() = default;
};

// Concrete Products
class Truck : public Transport {
public:
    std::string deliver() const override { return "Delivery by Truck"; }
};

class Ship : public Transport {
public:
    std::string deliver() const override { return "Delivery by Ship"; }
};

// Creator (defines factory method)
class Logistics {
public:
    virtual Transport* createTransport() const = 0; // Factory Method
    virtual ~Logistics() = default;

    // This is the operation that uses the product
    void planDelivery() const {
        std::unique_ptr<Transport> t(createTransport());
        std::cout << "[Factory Method] " << t->deliver() << "\n";
    }
};

// Concrete Creators (override factory method)
class RoadLogistics : public Logistics {
public:
    Transport* createTransport() const override { return new Truck(); }
};

class SeaLogistics : public Logistics {
public:
    Transport* createTransport() const override { return new Ship(); }
};


//
// ===========================
// Abstract Factory Pattern
// ===========================
// Purpose: Provide an interface for creating families of related
// or dependent objects without specifying their concrete classes.
// Used here to create different styles of furniture (Victorian, Modern)
//

class Chair {
public:
    virtual std::string type() const = 0;
    virtual ~Chair() = default;
};

// Concrete Products
class VictorianChair : public Chair {
public:
    std::string type() const override { return "Victorian Chair"; }
};

class ModernChair : public Chair {
public:
    std::string type() const override { return "Modern Chair"; }
};

// Abstract Factory Interface
class FurnitureFactory {
public:
    virtual Chair* createChair() const = 0;
    virtual ~FurnitureFactory() = default;
};

// Concrete Factories (return related products)
class VictorianFactory : public FurnitureFactory {
public:
    Chair* createChair() const override { return new VictorianChair(); }
};

class ModernFactory : public FurnitureFactory {
public:
    Chair* createChair() const override { return new ModernChair(); }
};

// Client uses abstract factory, not knowing which concrete product is created
void showFurniture(const FurnitureFactory& factory) {
    std::unique_ptr<Chair> c(factory.createChair());
    std::cout << "[Abstract Factory] Created: " << c->type() << "\n";
}


//
// ===========================
// Builder Pattern
// ===========================
// Purpose: Separate the construction of a complex object from its representation
// so the same construction process can create different representations.
// Used here to build different configurations of a House object.
//

// Product to be built
class House {
public:
    void addPart(const std::string& part) { parts.push_back(part); }

    void show() const {
        std::cout << "[Builder] House with: ";
        for (const auto& p : parts)
            std::cout << p << " ";
        std::cout << "\n";
    }

private:
    std::vector<std::string> parts;
};

// Builder Interface (defines steps)
class HouseBuilder {
public:
    virtual void buildWalls() = 0;
    virtual void buildDoors() = 0;
    virtual void buildWindows() = 0;
    virtual House* getResult() = 0;
    virtual ~HouseBuilder() = default;
};

// Concrete Builder (implements steps)
class SimpleHouseBuilder : public HouseBuilder {
    House* house;
public:
    SimpleHouseBuilder() { house = new House(); }
    ~SimpleHouseBuilder() { delete house; }

    void buildWalls() override { house->addPart("Walls"); }
    void buildDoors() override { house->addPart("Doors"); }
    void buildWindows() override { house->addPart("Windows"); }

    // Return built product and reset internal pointer
    House* getResult() override {
        House* result = house;
        house = new House(); // allow reuse
        return result;
    }
};

// Director (orchestrates steps in a specific order)
class Director {
    HouseBuilder* builder;
public:
    void setBuilder(HouseBuilder* b) { builder = b; }

    void buildMinimalHouse() {
        builder->buildWalls();
        builder->buildDoors();
    }

    void buildFullHouse() {
        builder->buildWalls();
        builder->buildDoors();
        builder->buildWindows();
    }
};


//
// ===========================
// Main: Demonstrate all Patterns
// ===========================

int main() {
    // ---- Factory Method Demo ----
    RoadLogistics road;
    SeaLogistics sea;
    road.planDelivery(); // uses Truck
    sea.planDelivery();  // uses Ship

    // ---- Abstract Factory Demo ----
    VictorianFactory vf;
    ModernFactory mf;
    showFurniture(vf);  // creates Victorian Chair
    showFurniture(mf);  // creates Modern Chair

    // ---- Builder Demo ----
    SimpleHouseBuilder builder;
    Director director;
    director.setBuilder(&builder);

    director.buildMinimalHouse();
    std::unique_ptr<House> h1(builder.getResult());
    h1->show(); // House with walls and doors

    director.buildFullHouse();
    std::unique_ptr<House> h2(builder.getResult());
    h2->show(); // House with walls, doors, windows

    return 0;
}

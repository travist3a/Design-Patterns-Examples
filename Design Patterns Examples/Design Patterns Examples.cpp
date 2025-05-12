#include <iostream>
#include <string>
#include <vector>
#include <memory>

//
// ===========================
// Factory Method Pattern
// ===========================
//
// Intent: Define an interface for creating an object, but let subclasses
// decide which class to instantiate. Factory Method lets a class defer
// instantiation to subclasses.
//
// Key Roles:
// - Product: common interface for all possible products
// - ConcreteProduct: specific classes implementing the product
// - Creator: declares the factory method (can have default implementations)
// - ConcreteCreator: overrides the factory method to return a ConcreteProduct
//

// ---------- Product Interface ----------
class Transport {
public:
    virtual std::string deliver() const = 0;
    virtual ~Transport() = default;
};

// ---------- Concrete Products ----------
class Truck : public Transport {
public:
    std::string deliver() const override { return "Delivery by Truck"; }
};

class Ship : public Transport {
public:
    std::string deliver() const override { return "Delivery by Ship"; }
};

// ---------- Creator Interface ----------
class Logistics {
public:
    virtual Transport* createTransport() const = 0; // Factory Method
    virtual ~Logistics() = default;

    // Template method using the product created by factory method
    void planDelivery() const {
        std::unique_ptr<Transport> t(createTransport()); // Decouples creation
        std::cout << "[Factory Method] " << t->deliver() << "\n";
    }
};

// ---------- Concrete Creators ----------
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
//
// Intent: Provide an interface for creating families of related or dependent
// objects without specifying their concrete classes.
//
// Key Roles:
// - AbstractFactory: declares interfaces for each product type
// - ConcreteFactory: implements creation for a product family
// - AbstractProduct: declares product interfaces
// - ConcreteProduct: product implementations that belong to a family
//

// ---------- Abstract Product ----------
class Chair {
public:
    virtual std::string type() const = 0;
    virtual ~Chair() = default;
};

// ---------- Concrete Products ----------
class VictorianChair : public Chair {
public:
    std::string type() const override { return "Victorian Chair"; }
};

class ModernChair : public Chair {
public:
    std::string type() const override { return "Modern Chair"; }
};

// ---------- Abstract Factory Interface ----------
class FurnitureFactory {
public:
    virtual Chair* createChair() const = 0;
    virtual ~FurnitureFactory() = default;
};

// ---------- Concrete Factories ----------
class VictorianFactory : public FurnitureFactory {
public:
    Chair* createChair() const override { return new VictorianChair(); }
};

class ModernFactory : public FurnitureFactory {
public:
    Chair* createChair() const override { return new ModernChair(); }
};

// ---------- Client Code ----------
void showFurniture(const FurnitureFactory& factory) {
    // Client only depends on abstract interfaces
    std::unique_ptr<Chair> c(factory.createChair());
    std::cout << "[Abstract Factory] Created: " << c->type() << "\n";
}


//
// ===========================
// Builder Pattern
// ===========================
//
// Intent: Separate the construction of a complex object from its representation
// so the same construction process can create different representations.
//
// Key Roles:
// - Product: complex object being built
// - Builder: abstract interface defining build steps
// - ConcreteBuilder: provides step-by-step implementation
// - Director: controls construction sequence (optional)
//

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

// ---------- Builder Interface ----------
class HouseBuilder {
public:
    virtual void buildWalls() = 0;
    virtual void buildDoors() = 0;
    virtual void buildWindows() = 0;
    virtual House* getResult() = 0;
    virtual ~HouseBuilder() = default;
};

// ---------- Concrete Builder ----------
class SimpleHouseBuilder : public HouseBuilder {
    House* house;
public:
    SimpleHouseBuilder() { house = new House(); }
    ~SimpleHouseBuilder() { delete house; }

    void buildWalls() override { house->addPart("Walls"); }
    void buildDoors() override { house->addPart("Doors"); }
    void buildWindows() override { house->addPart("Windows"); }

    House* getResult() override {
        House* result = house;
        house = new House(); // prepare for next build
        return result;
    }
};

// ---------- Director (optional) ----------
class Director {
    HouseBuilder* builder;
public:
    void setBuilder(HouseBuilder* b) { builder = b; }

    // Build only essential parts
    void buildMinimalHouse() {
        builder->buildWalls();
        builder->buildDoors();
    }

    // Build everything
    void buildFullHouse() {
        builder->buildWalls();
        builder->buildDoors();
        builder->buildWindows();
    }
};


//
// ===========================
// Main: Demonstrate All Three Patterns
// ===========================

int main() {
    // ==== Factory Method Demo ====
    RoadLogistics road;
    SeaLogistics sea;
    road.planDelivery(); // Client uses Logistics interface
    sea.planDelivery();  // Actual creation handled by subclasses

    // ==== Abstract Factory Demo ====
    VictorianFactory vf;
    ModernFactory mf;
    showFurniture(vf); // Produces a Victorian Chair
    showFurniture(mf); // Produces a Modern Chair

    // ==== Builder Demo ====
    SimpleHouseBuilder builder;
    Director director;
    director.setBuilder(&builder);

    director.buildMinimalHouse();
    std::unique_ptr<House> h1(builder.getResult());
    h1->show(); // House with Walls, Doors

    director.buildFullHouse();
    std::unique_ptr<House> h2(builder.getResult());
    h2->show(); // House with Walls, Doors, Windows

    return 0;
}

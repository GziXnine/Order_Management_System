# Smart Order Management System (OMS)

A learning project demonstrating **OOP** and **SOLID** principles in C++17.

---

## Table of Contents

- [Business Model](#business-model)
- [Project Structure](#project-structure)
- [Domain Concepts](#domain-concepts)
- [System Flow](#system-flow)
- [Diagrams](#diagrams)

---

## Business Model

### Problem

An online store needs to:
- Track product catalog and inventory levels
- Process customer orders reliably (validate, pay, ship)
- Handle failures gracefully (payment declined, out of stock)
- Notify customers at every step

The OMS is the **central brain** coordinating Products, Inventory, Orders, Payments, and Shipments.

### Actors

| Actor | Role |
|---|---|
| **Customer** | Places orders, receives notifications |
| **Admin** | Manages products, monitors orders |
| **System (OMS)** | Validates, coordinates, enforces rules |
| **Payment Provider** | Charges/refunds money (external) |
| **Shipping Carrier** | Delivers packages (external) |

### Order Lifecycle

```
PLACED → VALIDATED → INVENTORY_RESERVED → PAYMENT_PROCESSED → SHIPPED → DELIVERED
```

Failure paths:
- Validation fails → **REJECTED**
- Out of stock → **CANCELLED**
- Payment fails → **PAYMENT_FAILED** (inventory released)
- Customer cancels (before shipped) → **CANCELLED** (refund + release)

---

**Dependency Rule**: Upper layers depend on lower layers through **abstractions** (interfaces), never on concrete implementations.

---

## Project Structure

```
Order Management System/
├── main.cpp                          # Entry point — Manual DI wiring
├── CMakeLists.txt                    # Build configuration
├── README.md
│
├── include/
│   ├── domain/
│   │   ├── enum/
│   │   │   ├── EOrderStatus.h
│   │   │   ├── EPaymentStatus.h
│   │   │   └── EShipmentStatus.h
│   │   ├── Product.h
│   │   ├── OrderItem.h
│   │   ├── Order.h
│   │   ├── Inventory.h
│   │   ├── Payment.h
│   │   └── Shipment.h
│   │
│   ├── application/
│   │   ├── Interface/
│   │   │   ├── IOrderRepository.h
│   │   │   ├── IProductRepository.h
│   │   │   ├── IPaymentProvider.h
│   │   │   ├── IShippingService.h
│   │   │   └── INotificationService.h
│   │   ├── CreateOrderService.h
│   │   ├── CancelOrderService.h
│   │   └── ProcessPaymentService.h
│   │
│   ├── infrastructure/
│   │   ├── InMemoryOrderRepository.h
│   │   ├── InMemoryProductRepository.h
│   │   ├── FakePaymentProvider.h
│   │   ├── FakeShippingService.h
│   │   └── ConsoleNotificationService.h
│   │
│   └── presentation/
│       └── OrderController.h
│
└── src/
    ├── domain/
    │   ├── Product.cpp
    │   ├── OrderItem.cpp
    │   ├── Order.cpp
    │   ├── Inventory.cpp
    │   ├── Payment.cpp
    │   └── Shipment.cpp
    │
    ├── application/
    │   ├── CreateOrderService.cpp
    │   ├── CancelOrderService.cpp
    │   └── ProcessPaymentService.cpp
    │
    ├── infrastructure/
    │   ├── InMemoryOrderRepository.cpp
    │   ├── InMemoryProductRepository.cpp
    │   ├── FakePaymentProvider.cpp
    │   ├── FakeShippingService.cpp
    │   └── ConsoleNotificationService.cpp
    │
    └── presentation/
        └── OrderController.cpp
```

---

## Domain Concepts

### Product
- Catalog entry: id, name, price, active flag
- Rules: price > 0, name not empty, inactive products can't be sold

### Inventory
- Tracks `total` and `reserved` quantities per product
- Available = total − reserved
- Operations: `reserve`, `release`, `deduct`, `restock`
- Rule: available can never go negative

### Order
- Central document: orderId, list of OrderItems, totalAmount, status, timestamp
- Rules: at least one item, valid state transitions only, terminal states are final
- State transitions enforced by `changeStatus()` guard

### OrderItem
- Holds a Product reference + quantity + computed lineTotal
- Rule of Five implemented (raw pointer requires copy/move semantics)
- Rule: quantity >= 1

### Payment
- Status enum: Pending → Completed | Failed | Refunded
- Transitions: `markComplete()`, `markFailed()`, `refund()`

### Shipment
- Status enum: Pending → Shipped → Delivered
- Tracks address and carrier tracking number

---

## System Flow

### Place Order (Happy Path)

1. **Customer** submits order (product IDs + quantities)
2. **System** fetches Product details from ProductRepository
3. **System** validates product is active
4. **Inventory** checks availability and reserves stock
5. **PaymentProvider** charges the total amount
6. **Inventory** permanently deducts reserved stock
7. **ShippingService** creates shipment with tracking number
8. **OrderRepository** persists the order
9. **NotificationService** notifies customer

### Cancel Order

1. **System** fetches order from repository
2. **System** checks `canBeCancelled()` (only Placed / Validated / InventoryReserved)
3. **Inventory** releases reserved stock
4. **PaymentProvider** refunds the amount
5. **Order** status → Cancelled
6. **NotificationService** notifies customer

### Failure Handling

- **Product inactive** → Exception thrown, order not created
- **Out of stock** → Exception thrown, no reservation made
- **Payment fails** → Inventory released, status → PaymentFailed, order saved

---

## Diagrams

### High-Level System Overview

```mermaid
graph TD
    Customer([Customer])
    Admin([Admin])

    Customer -->|Places Order| OMS[Order Management System]
    Admin -->|Manages Products| OMS

    OMS --> ProductCatalog[Product Catalog]
    OMS --> InventoryMgr[Inventory Manager]
    OMS --> OrderProcessor[Order Processor]
    OMS --> PaymentHandler[Payment Handler]
    OMS --> ShipmentHandler[Shipment Handler]
    OMS --> NotificationSender[Notification Sender]

    PaymentHandler -->|Charges| PaymentProvider([Payment Provider])
    ShipmentHandler -->|Dispatches| ShippingCarrier([Shipping Carrier])
    NotificationSender -->|Sends Updates| Customer

    style Customer fill:#4CAF50,color:#fff
    style Admin fill:#2196F3,color:#fff
    style PaymentProvider fill:#FF9800,color:#fff
    style ShippingCarrier fill:#FF9800,color:#fff
    style OMS fill:#9C27B0,color:#fff
```

### Order Lifecycle — State Diagram

```mermaid
stateDiagram-v2
    [*] --> Placed : Customer submits order

    Placed --> Validated : All items valid
    Placed --> Rejected : Validation failed

    Validated --> InventoryReserved : Stock available
    Validated --> Cancelled : Out of stock

    InventoryReserved --> PaymentProcessed : Payment succeeds
    InventoryReserved --> PaymentFailed : Payment fails

    PaymentFailed --> Cancelled : Inventory released

    PaymentProcessed --> Shipped : Carrier picks up
    PaymentProcessed --> Cancelled : Customer cancels before ship

    Shipped --> Delivered : Carrier delivers

    Rejected --> [*]
    Cancelled --> [*]
    Delivered --> [*]
```

### Sequence Diagram — Place Order

```mermaid
sequenceDiagram
    actor C as Customer
    participant Ctrl as OrderController
    participant Svc as CreateOrderService
    participant PR as ProductRepository
    participant I as Inventory
    participant Pay as PaymentProvider
    participant Ship as ShippingService
    participant Repo as OrderRepository
    participant N as NotificationService

    C->>Ctrl: placeOrder()
    Ctrl->>Svc: createOrder(customerId, items)

    loop For each item
        Svc->>PR: findById(productId)
        PR-->>Svc: Product
        Svc->>Svc: Validate product is active
        Svc->>I: isAvailable(productId, qty)
        I-->>Svc: true
        Svc->>I: reserve(productId, qty)
    end

    Svc->>Pay: charge(totalAmount)

    alt Payment succeeds
        Pay-->>Svc: true
        loop For each item
            Svc->>I: deduct(productId, qty)
        end
        Svc->>Ship: createShipment(order)
        Ship-->>Svc: Shipment
        Svc->>Repo: save(order)
        Svc->>N: notify(success message)
        Svc-->>Ctrl: Order
        Ctrl-->>C: Display success
    else Payment fails
        Pay-->>Svc: false
        loop For each item
            Svc->>I: release(productId, qty)
        end
        Svc->>Repo: save(order as PaymentFailed)
        Svc->>N: notify(failure message)
        Svc-->>Ctrl: Order
        Ctrl-->>C: Display failure
    end
```

### Workflow Diagram — Decision Flow

```mermaid
graph LR
    A[Customer Places Order] --> B{Products Active?}

    B -->|No| R[Exception: Inactive Product]
    B -->|Yes| C{Stock Available?}

    C -->|No| X1[Exception: Out of Stock]
    C -->|Yes| D[Reserve Inventory]

    D --> E{Payment Succeeds?}

    E -->|No| F[Release Inventory]
    F --> X2[Status: PaymentFailed]

    E -->|Yes| G[Deduct Inventory]
    G --> H[Create Shipment]
    H --> I[Save Order]
    I --> J[Notify Customer]

    style A fill:#4CAF50,color:#fff
    style R fill:#f44336,color:#fff
    style X1 fill:#f44336,color:#fff
    style X2 fill:#f44336,color:#fff
    style J fill:#2196F3,color:#fff
```

### Dependency Injection Wiring

```mermaid
graph TD
    Main[main.cpp] -->|creates| Inv[Inventory]
    Main -->|creates| PRepo[InMemoryProductRepository]
    Main -->|creates| ORepo[InMemoryOrderRepository]
    Main -->|creates| Pay[FakePaymentProvider]
    Main -->|creates| Ship[FakeShippingService]
    Main -->|creates| Notif[ConsoleNotificationService]

    Main -->|injects into| CS[CreateOrderService]
    Main -->|injects into| CAS[CancelOrderService]

    CS -.->|depends on| IOR([IOrderRepository])
    CS -.->|depends on| IPR([IProductRepository])
    CS -.->|depends on| IPP([IPaymentProvider])
    CS -.->|depends on| ISS([IShippingService])
    CS -.->|depends on| INS([INotificationService])

    ORepo -->|implements| IOR
    PRepo -->|implements| IPR
    Pay -->|implements| IPP
    Ship -->|implements| ISS
    Notif -->|implements| INS

    Main -->|injects into| Ctrl[OrderController]

    style Main fill:#9C27B0,color:#fff
    style IOR fill:#FF9800,color:#fff
    style IPR fill:#FF9800,color:#fff
    style IPP fill:#FF9800,color:#fff
    style ISS fill:#FF9800,color:#fff
    style INS fill:#FF9800,color:#fff
```

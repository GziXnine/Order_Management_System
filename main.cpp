#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

#include "include/domain/Product.h"
#include "include/domain/Order.h"
#include "include/domain/OrderItem.h"
#include "include/domain/Inventory.h"
#include "include/domain/Payment.h"
#include "include/domain/Shipment.h"
#include "include/domain/Customer.h"

#include "include/infrastructure/InMemoryOrderRepository.h"
#include "include/infrastructure/InMemoryProductRepository.h"
#include "include/infrastructure/InMemoryCustomerRepository.h"
#include "include/infrastructure/FakePaymentProvider.h"
#include "include/infrastructure/FakeShippingService.h"
#include "include/infrastructure/ConsoleNotificationService.h"

#include "include/application/CreateOrderService.h"
#include "include/application/CancelOrderService.h"

#include "include/presentation/OrderController.h"

using std::cout;
using std::endl;
using std::fixed;
using std::left;
using std::setprecision;
using std::setw;
using std::string;

// ============================================================
//  CP437 Box-Drawing Characters (single-byte, native Windows)
// ============================================================
static const char TL = '\xC9'; // top-left
static const char TR = '\xBB'; // top-right
static const char BL = '\xC8'; // bottom-left
static const char BR = '\xBC'; // bottom-right
static const char HH = '\xCD'; // double horizontal
static const char VV = '\xBA'; // double vertical
static const char ML = '\xCC'; // middle-left
static const char MR = '\xB9'; // middle-right
static const char MT = '\xCB'; // middle-top
static const char MB = '\xCA'; // middle-bottom
static const char XX = '\xCE'; // cross

static string rep(char c, int n) { return string(n, c); }

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(437);
    SetConsoleCP(437);
#endif

    // ──────────────────────────────────────────────
    //  Repositories
    // ──────────────────────────────────────────────
    InMemoryOrderRepository orderRepo;
    InMemoryProductRepository productRepo;
    InMemoryCustomerRepository customerRepo;

    // ──────────────────────────────────────────────
    //  Seed Customers
    // ──────────────────────────────────────────────
    customerRepo.addCustomer(Customer(1, "Ahmed Ali", 500.00));
    customerRepo.addCustomer(Customer(2, "Sara Mohamed", 1200.00));
    customerRepo.addCustomer(Customer(3, "Omar Hassan", 250.00));

    // ──────────────────────────────────────────────
    //  Seed Products
    // ──────────────────────────────────────────────
    productRepo.addProduct(Product(1, "Wireless Mouse", 29.99, true));
    productRepo.addProduct(Product(2, "Mechanical KB", 89.99, true));
    productRepo.addProduct(Product(3, "USB-C Hub", 49.99, true));
    productRepo.addProduct(Product(4, "Old Trackball", 19.99, false));

    // ──────────────────────────────────────────────
    //  Inventory
    // ──────────────────────────────────────────────
    Inventory inventory;
    inventory.restock(1, 50);
    inventory.restock(2, 30);
    inventory.restock(3, 20);
    inventory.restock(4, 5);

    // ──────────────────────────────────────────────
    //  External Services (Fakes)
    // ──────────────────────────────────────────────
    FakePaymentProvider paymentProvider;
    FakeShippingService shippingService;
    ConsoleNotificationService notifier;

    // ──────────────────────────────────────────────
    //  Application Services
    // ──────────────────────────────────────────────
    CreateOrderService createService(orderRepo, inventory, productRepo,
                                     customerRepo, paymentProvider,
                                     shippingService, notifier);
    CancelOrderService cancelService(orderRepo, inventory,
                                     paymentProvider, notifier);

    // ══════════════════════════════════════════════
    //  Banner
    // ══════════════════════════════════════════════
    cout << "\n";
    cout << TL << rep(HH, 58) << TR << endl;
    cout << VV << "           SEED DATA LOADED SUCCESSFULLY                   " << VV << endl;
    cout << BL << rep(HH, 58) << BR << endl;

    // ── Customers Table ──────────────────────────
    cout << "\n  Customers:\n";
    cout << "  " << TL << rep(HH, 8) << MT << rep(HH, 22) << MT << rep(HH, 15) << TR << endl;
    cout << "  " << VV << " " << setw(6) << left << "ID"
         << " " << VV << " " << setw(20) << left << "Name"
         << " " << VV << " " << setw(13) << left << "Balance"
         << " " << VV << endl;
    cout << "  " << ML << rep(HH, 8) << XX << rep(HH, 22) << XX << rep(HH, 15) << MR << endl;

    std::vector<Customer> customers = customerRepo.getAll();
    for (const auto &c : customers)
    {
        std::ostringstream balStr;
        balStr << "$" << fixed << setprecision(2) << c.getBalance();
        cout << "  " << VV << " " << setw(6) << left << c.getId()
             << " " << VV << " " << setw(20) << left << c.getName()
             << " " << VV << " " << setw(13) << left << balStr.str()
             << " " << VV << endl;
    }
    cout << "  " << BL << rep(HH, 8) << MB << rep(HH, 22) << MB << rep(HH, 15) << BR << endl;

    // ── Products Table ───────────────────────────
    cout << "\n  Products:\n";
    cout << "  " << TL << rep(HH, 8) << MT << rep(HH, 22) << MT << rep(HH, 10) << MT << rep(HH, 12) << MT << rep(HH, 10) << TR << endl;
    cout << "  " << VV << " " << setw(6) << left << "ID"
         << " " << VV << " " << setw(20) << left << "Product Name"
         << " " << VV << " " << setw(8) << left << "Price"
         << " " << VV << " " << setw(10) << left << "Stock"
         << " " << VV << " " << setw(8) << left << "Status"
         << " " << VV << endl;
    cout << "  " << ML << rep(HH, 8) << XX << rep(HH, 22) << XX << rep(HH, 10) << XX << rep(HH, 12) << XX << rep(HH, 10) << MR << endl;

    std::vector<Product> products = productRepo.getAll();
    for (const auto &p : products)
    {
        std::ostringstream priceStr;
        priceStr << "$" << fixed << setprecision(2) << p.getPrice();
        string statusStr = p.isActive() ? "Active" : "INACTIVE";

        cout << "  " << VV << " " << setw(6) << left << p.getId()
             << " " << VV << " " << setw(20) << left << p.getName()
             << " " << VV << " " << setw(8) << left << priceStr.str()
             << " " << VV << " " << setw(10) << left << std::to_string(inventory.getAvailableQuantity(p.getId()))
             << " " << VV << " " << setw(8) << left << statusStr
             << " " << VV << endl;
    }
    cout << "  " << BL << rep(HH, 8) << MB << rep(HH, 22) << MB << rep(HH, 10) << MB << rep(HH, 12) << MB << rep(HH, 10) << BR << endl;

    // ──────────────────────────────────────────────
    //  Controller
    // ──────────────────────────────────────────────
    OrderController controller(createService, cancelService,
                               orderRepo, productRepo,
                               customerRepo, inventory);
    controller.showMenu();

    return 0;
}

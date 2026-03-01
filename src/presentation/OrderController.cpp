#include "../../include/presentation/OrderController.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <utility>
#include <string>
#include <limits>
#include <algorithm>
#include <sstream>

using std::cout;
using std::cin;
using std::endl;
using std::left;
using std::right;
using std::setw;
using std::fixed;
using std::setprecision;
using std::string;

// ============================================================
//  CP437 Box-Drawing Characters (single-byte, native Windows)
// ============================================================
static const char TL  = '\xC9'; // ╔  top-left
static const char TR  = '\xBB'; // ╗  top-right
static const char BL  = '\xC8'; // ╚  bottom-left
static const char BR  = '\xBC'; // ╝  bottom-right
static const char HH  = '\xCD'; // ═  double horizontal
static const char VV  = '\xBA'; // ║  double vertical
static const char ML  = '\xCC'; // ╠  middle-left
static const char MR  = '\xB9'; // ╣  middle-right
static const char MT  = '\xCB'; // ╦  middle-top
static const char MB  = '\xCA'; // ╩  middle-bottom
static const char XX  = '\xCE'; // ╬  cross
static const char DL  = '\xC7'; // ╟  divider-left  (dbl vert / single horiz)
static const char DR  = '\xB6'; // ╢  divider-right (dbl vert / single horiz)
static const char SH  = '\xC4'; // ─  single horizontal

// Helper: repeat a char N times
static string rep(char c, int n) { return string(n, c); }

// ============================================================
//  Constructor
// ============================================================
OrderController::OrderController(CreateOrderService &createSvc, CancelOrderService &cancelSvc,
                                 IOrderRepository &orderRepo, IProductRepository &productRepo,
                                 ICustomerRepository &customerRepo, Inventory &inventory)
    : createService(createSvc), cancelService(cancelSvc), orderRepo(orderRepo),
      productRepo(productRepo), customerRepo(customerRepo), inventory(inventory) {}

// ============================================================
//  UI Helpers
// ============================================================
void OrderController::printHeader(const string &title) const
{
    int width = 50;
    int pad = (width - 2 - (int)title.size()) / 2;
    if (pad < 1) pad = 1;

    cout << "\n";
    cout << TL << rep(HH, width - 2) << TR << endl;
    cout << VV;
    for (int i = 0; i < pad; i++) cout << " ";
    cout << title;
    for (int i = 0; i < width - 2 - pad - (int)title.size(); i++) cout << " ";
    cout << VV << endl;
    cout << BL << rep(HH, width - 2) << BR << endl;
}

void OrderController::printSuccess(const string &msg) const
{
    cout << "\n  [+] " << msg << "\n" << endl;
}

void OrderController::printError(const string &msg) const
{
    cout << "\n  [!] Error: " << msg << "\n" << endl;
}

void OrderController::printDivider() const
{
    cout << DL << rep(SH, 48) << DR << endl;
}

std::string OrderController::statusToString(EOrderStatus status) const
{
    switch (status)
    {
    case EOrderStatus::Placed:            return "Placed";
    case EOrderStatus::Validated:         return "Validated";
    case EOrderStatus::InventoryReserved: return "Inventory Reserved";
    case EOrderStatus::PaymentProcessed:  return "Payment Processed";
    case EOrderStatus::Shipped:           return "Shipped";
    case EOrderStatus::Delivered:         return "Delivered";
    case EOrderStatus::Cancelled:         return "Cancelled";
    case EOrderStatus::Rejected:          return "Rejected";
    case EOrderStatus::PaymentFailed:     return "Payment Failed";
    default:                              return "Unknown";
    }
}

// ============================================================
//  Dynamic Tables
// ============================================================
void OrderController::showCustomerTable(int customerId) const
{
    try
    {
        Customer customer = customerRepo.findById(customerId);

        std::ostringstream balStr;
        balStr << "$" << fixed << setprecision(2) << customer.getBalance();

        //  ╔════════╦══════════════════════╦═══════════════╗
        cout << "\n";
        cout << "  " << TL << rep(HH,8) << MT << rep(HH,22) << MT << rep(HH,15) << TR << endl;
        cout << "  " << VV << " " << setw(6) << left << "ID"
             << " " << VV << " " << setw(20) << left << "Name"
             << " " << VV << " " << setw(13) << left << "Balance"
             << " " << VV << endl;
        cout << "  " << ML << rep(HH,8) << XX << rep(HH,22) << XX << rep(HH,15) << MR << endl;
        cout << "  " << VV << " " << setw(6) << left << customer.getId()
             << " " << VV << " " << setw(20) << left << customer.getName()
             << " " << VV << " " << setw(13) << left << balStr.str()
             << " " << VV << endl;
        cout << "  " << BL << rep(HH,8) << MB << rep(HH,22) << MB << rep(HH,15) << BR << endl;
        cout << endl;
    }
    catch (const std::exception &e)
    {
        printError(e.what());
    }
}

void OrderController::showProductCatalog() const
{
    std::vector<Product> products = productRepo.getAll();

    cout << "\n";
    //  ╔════════╦══════════════════════╦══════════╦════════════╦══════════╗
    cout << "  " << TL << rep(HH,8) << MT << rep(HH,22) << MT << rep(HH,10) << MT << rep(HH,12) << MT << rep(HH,10) << TR << endl;
    cout << "  " << VV << " " << setw(6)  << left << "ID"
         << " " << VV << " " << setw(20) << left << "Product Name"
         << " " << VV << " " << setw(8)  << left << "Price"
         << " " << VV << " " << setw(10) << left << "Stock"
         << " " << VV << " " << setw(8)  << left << "Status"
         << " " << VV << endl;
    cout << "  " << ML << rep(HH,8) << XX << rep(HH,22) << XX << rep(HH,10) << XX << rep(HH,12) << XX << rep(HH,10) << MR << endl;

    for (const auto &p : products)
    {
        std::ostringstream priceStr;
        priceStr << "$" << fixed << setprecision(2) << p.getPrice();
        string stockStr  = std::to_string(inventory.getAvailableQuantity(p.getId()));
        string statusStr = p.isActive() ? "Active" : "INACTIVE";

        cout << "  " << VV << " " << setw(6)  << left << p.getId()
             << " " << VV << " " << setw(20) << left << p.getName()
             << " " << VV << " " << setw(8)  << left << priceStr.str()
             << " " << VV << " " << setw(10) << left << stockStr
             << " " << VV << " " << setw(8)  << left << statusStr
             << " " << VV << endl;
    }

    cout << "  " << BL << rep(HH,8) << MB << rep(HH,22) << MB << rep(HH,10) << MB << rep(HH,12) << MB << rep(HH,10) << BR << endl;
    cout << endl;
}

void OrderController::showOrderTable(int orderId) const
{
    Order order = orderRepo.findById(orderId);

    string customerName = "Unknown";
    try
    {
        Customer c = customerRepo.findById(order.getCustomerId());
        customerName = c.getName();
    }
    catch (...) {}

    cout << "\n";
    cout << "  " << TL << rep(HH, 50) << TR << endl;
    cout << "  " << VV << "  Order #" << setw(41) << left << order.getOrderId() << VV << endl;
    cout << "  " << DL << rep(SH, 50) << DR << endl;

    std::ostringstream totalStr;
    totalStr << "$" << fixed << setprecision(2) << order.getTotalAmount();

    cout << "  " << VV << "  Customer : " << setw(37) << left << customerName        << VV << endl;
    cout << "  " << VV << "  Status   : " << setw(37) << left << statusToString(order.getStatus()) << VV << endl;
    cout << "  " << VV << "  Total    : " << setw(37) << left << totalStr.str()       << VV << endl;
    cout << "  " << VV << "  Items    : " << setw(37) << left << order.getItems().size() << VV << endl;

    cout << "  " << DL << rep(SH, 50) << DR << endl;

    cout << "  " << VV << "  " << setw(3)  << left  << "#"
                               << setw(20) << left  << "Product"
                               << setw(6)  << right << "Qty"
         << "   " << setw(10) << right << "Line Total"
         << "    " << VV << endl;

    cout << "  " << DL << rep(SH, 50) << DR << endl;

    for (size_t i = 0; i < order.getItems().size(); i++)
    {
        const OrderItem &item = order.getItems()[i];
        std::ostringstream lineStr;
        lineStr << "$" << fixed << setprecision(2) << item.getLineTotal();

        cout << "  " << VV << "  " << setw(3)  << left  << (i + 1)
                                    << setw(20) << left  << item.getProduct().getName()
                                    << setw(6)  << right << item.getQuantity()
             << "   " << setw(10) << right << lineStr.str()
             << "    " << VV << endl;
    }

    cout << "  " << BL << rep(HH, 50) << BR << endl;
    cout << endl;
}

void OrderController::showAllOrders() const
{
    std::vector<Order> orders = orderRepo.getAll();

    if (orders.empty())
    {
        cout << "\n  No orders found.\n" << endl;
        return;
    }

    cout << "\n";
    //  ╔════════╦══════════════╦══════════════════╦═══════════════╗
    cout << "  " << TL << rep(HH,8) << MT << rep(HH,14) << MT << rep(HH,18) << MT << rep(HH,15) << TR << endl;
    cout << "  " << VV << " " << setw(6)  << left << "ID"
         << " " << VV << " " << setw(12) << left << "Customer"
         << " " << VV << " " << setw(16) << left << "Status"
         << " " << VV << " " << setw(13) << left << "Total"
         << " " << VV << endl;
    cout << "  " << ML << rep(HH,8) << XX << rep(HH,14) << XX << rep(HH,18) << XX << rep(HH,15) << MR << endl;

    for (const auto &order : orders)
    {
        string custName = "N/A";
        try
        {
            Customer c = customerRepo.findById(order.getCustomerId());
            custName = c.getName();
            if (custName.size() > 12) custName = custName.substr(0, 11) + ".";
        }
        catch (...) {}

        std::ostringstream totalStr;
        totalStr << "$" << fixed << setprecision(2) << order.getTotalAmount();

        cout << "  " << VV << " " << setw(6)  << left << order.getOrderId()
             << " " << VV << " " << setw(12) << left << custName
             << " " << VV << " " << setw(16) << left << statusToString(order.getStatus())
             << " " << VV << " " << setw(13) << left << totalStr.str()
             << " " << VV << endl;
    }

    cout << "  " << BL << rep(HH,8) << MB << rep(HH,14) << MB << rep(HH,18) << MB << rep(HH,15) << BR << endl;
    cout << endl;
}

// ============================================================
//  Actions
// ============================================================
void OrderController::placeOrder()
{
    printHeader("Place New Order");

    int customerId;
    cout << "  Enter Customer ID: ";
    cin >> customerId;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        printError("Invalid Customer ID");
        return;
    }

    try { customerRepo.findById(customerId); }
    catch (const std::exception &e) { printError(e.what()); return; }

    showCustomerTable(customerId);
    showProductCatalog();

    std::vector<std::pair<int, int>> items;
    string more = "y";

    while (more == "y" || more == "Y")
    {
        int productId, qty;

        cout << "  Enter Product ID: ";
        cin >> productId;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            printError("Invalid Product ID");
            continue;
        }

        cout << "  Enter Quantity  : ";
        cin >> qty;
        if (cin.fail() || qty <= 0)
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            printError("Invalid Quantity (must be > 0)");
            continue;
        }

        items.push_back({productId, qty});
        cout << "  Add another item? (y/n): ";
        cin >> more;
    }

    if (items.empty())
    {
        printError("No items selected");
        return;
    }

    try
    {
        Order order = createService.createOrder(customerId, items);

        std::ostringstream msg;
        msg << "Order #" << order.getOrderId() << " created! Total: $"
            << fixed << setprecision(2) << order.getTotalAmount()
            << " | Status: " << statusToString(order.getStatus());
        printSuccess(msg.str());

        showCustomerTable(customerId);
    }
    catch (const std::exception &e)
    {
        printError(e.what());
    }
}

void OrderController::cancelOrder()
{
    printHeader("Cancel Order");
    showAllOrders();

    int orderId;
    cout << "  Enter Order ID to cancel: ";
    cin >> orderId;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        printError("Invalid Order ID");
        return;
    }

    try
    {
        cancelService.cancelOrder(orderId);
        printSuccess("Order #" + std::to_string(orderId) + " cancelled successfully");
    }
    catch (const std::exception &e)
    {
        printError(e.what());
    }
}

void OrderController::showOrder()
{
    printHeader("Show Order Details");
    showAllOrders();

    int orderId;
    cout << "  Enter Order ID: ";
    cin >> orderId;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        printError("Invalid Order ID");
        return;
    }

    try { showOrderTable(orderId); }
    catch (const std::exception &e) { printError(e.what()); }
}

// ============================================================
//  Main Menu
// ============================================================
void OrderController::showMenu()
{
    int choice = 0;

    while (true)
    {
        cout << "\n";
        cout << TL << rep(HH, 50) << TR << endl;
        cout << VV << "     Order Management System (OMS)              " << VV << endl;
        cout << ML << rep(HH, 50) << MR << endl;
        cout << VV << "                                                  " << VV << endl;
        cout << VV << "    [1]  Place Order                              " << VV << endl;
        cout << VV << "    [2]  Cancel Order                             " << VV << endl;
        cout << VV << "    [3]  Show Order Details                       " << VV << endl;
        cout << VV << "    [4]  Show All Orders                          " << VV << endl;
        cout << VV << "    [5]  Show Product Catalog                     " << VV << endl;
        cout << VV << "    [0]  Exit                                     " << VV << endl;
        cout << VV << "                                                  " << VV << endl;
        cout << BL << rep(HH, 50) << BR << endl;

        cout << "  Choose: ";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            printError("Invalid input. Try again.");
            continue;
        }

        switch (choice)
        {
        case 1: placeOrder();       break;
        case 2: cancelOrder();      break;
        case 3: showOrder();        break;
        case 4: showAllOrders();    break;
        case 5: showProductCatalog(); break;
        case 0: printHeader("Goodbye!"); return;
        default: printError("Invalid choice. Try again.");
        }
    }
}

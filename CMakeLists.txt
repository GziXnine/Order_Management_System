cmake_minimum_required(VERSION 3.16)
project(OrderManagementSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Source files ──────────────────────────────────────────────

set(DOMAIN_SOURCES
  src/domain/Product.cpp
  src/domain/OrderItem.cpp
  src/domain/Order.cpp
  src/domain/Inventory.cpp
  src/domain/Payment.cpp
  src/domain/Shipment.cpp
)

set(APPLICATION_SOURCES
  src/application/CreateOrderService.cpp
  src/application/CancelOrderService.cpp
  src/application/ProcessPaymentService.cpp
)

set(INFRASTRUCTURE_SOURCES
  src/infrastructure/InMemoryOrderRepository.cpp
  src/infrastructure/InMemoryProductRepository.cpp
  src/infrastructure/FakePaymentProvider.cpp
  src/infrastructure/FakeShippingService.cpp
  src/infrastructure/ConsoleNotificationService.cpp
)

set(PRESENTATION_SOURCES
  src/presentation/OrderController.cpp
)

# ── Executable ────────────────────────────────────────────────

add_executable(oms
  main.cpp
  ${DOMAIN_SOURCES}
  ${APPLICATION_SOURCES}
  ${INFRASTRUCTURE_SOURCES}
  ${PRESENTATION_SOURCES}
)

target_include_directories(oms PRIVATE ${CMAKE_SOURCE_DIR}/include)

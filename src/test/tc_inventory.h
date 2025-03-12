#include <iostream>
#include "app/inventory.h" 

using namespace std;

void tc_inventory1001() {
    int arr[] = {1, 2, 3, 4, 5};
    List1D<int> list1D(arr, 5);
    
    cout << "tc_inventory1001: " << list1D.toString() << endl;
}

void tc_inventory1002() {
    int row0[] = {1, 2, 3};
    int row1[] = {4, 5, 6};
    int row2[] = {7, 8, 9};
    List1D<int> rows[3] = { List1D<int>(row0, 3), List1D<int>(row1, 3), List1D<int>(row2, 3) };
    List2D<int> matrix(rows, 3);
    
    cout << "tc_inventory1002: " << matrix.toString() << endl;
}

void tc_inventory1003() {
    // Tạo ma trận thuộc tính với InventoryAttribute
    InventoryAttribute attrRow0[] = { {"weight", 10}, {"height", 156} };
    InventoryAttribute attrRow1[] = { {"weight", 20}, {"height", 100} };
    List1D<InventoryAttribute> rows[2] = { List1D<InventoryAttribute>(attrRow0, 2),
                                            List1D<InventoryAttribute>(attrRow1, 2) };
    List2D<InventoryAttribute> attributesMatrix(rows, 2);
    
    string namesArr[] = {"ProductA", "ProductB"};
    List1D<string> productNames(namesArr, 2);
    
    int qtyArr[] = {5, 10};
    List1D<int> quantities(qtyArr, 2);
    
    InventoryManager inv(attributesMatrix, productNames, quantities);
    
    cout << "tc_inventory1003: " << inv.toString() << endl;
}

void tc_inventory1004() {
    // Tạo InventoryManager với 2 sản phẩm
    InventoryAttribute attrRow0[] = { {"weight", 10}, {"height", 156} };
    InventoryAttribute attrRow1[] = { {"weight", 20}, {"height", 100} };
    List1D<InventoryAttribute> rows[2] = { List1D<InventoryAttribute>(attrRow0, 2),
                                            List1D<InventoryAttribute>(attrRow1, 2) };
    List2D<InventoryAttribute> matrix(rows, 2);
    
    string namesArr[] = {"ProductA", "ProductB"};
    List1D<string> names(namesArr, 2);
    
    int qtyArr[] = {50, 30};
    List1D<int> quantities(qtyArr, 2);
    
    InventoryManager inv1(matrix, names, quantities);
    InventoryManager invCopy(inv1);
    invCopy.updateQuantity(0, 20);
    
    cout << "tc_inventory1004:\n";
    cout << "Original quantity: " << inv1.getProductQuantity(0) << endl;
    cout << "Copied quantity: " << invCopy.getProductQuantity(0) << endl;
}

void tc_inventory1005() {
    // Test query với InventoryManager
    InventoryAttribute attrRow0[] = { {"weight", 10}, {"height", 156} };  
    InventoryAttribute attrRow1[] = { {"weight", 20}, {"height", 100} }; 
    InventoryAttribute attrRow2[] = { {"weight", 30}, {"height", 200} }; 
    List1D<InventoryAttribute> rows[3] = { List1D<InventoryAttribute>(attrRow0, 2),
                                            List1D<InventoryAttribute>(attrRow1, 2),
                                            List1D<InventoryAttribute>(attrRow2, 2) };
    List2D<InventoryAttribute> matrix(rows, 3);
    
    string namesArr[] = {"ProductA", "ProductB", "ProductC"};
    List1D<string> names(namesArr, 3);
    
    int qtyArr[] = { 5, 10, 15 };
    List1D<int> quantities(qtyArr, 3);
    
    InventoryManager inv(matrix, names, quantities);
    
    // Query: lấy các sản phẩm có thuộc tính ở chỉ số 0 (weight) trong khoảng [10, 30] và số lượng >= 5.
    // Với ascending == true, sắp xếp theo giá trị tăng dần.
    List1D<string> result = inv.query(0, 10, 30, 5, true);
    
    cout << "tc_inventory1005: " << result.toString() << endl;
}
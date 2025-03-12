#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include "list/XArrayList.h"
#include "list/DLinkedList.h"
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

// ======================= InventoryAttribute =======================
struct InventoryAttribute {
    string name;
    double value;
    // Constructor mặc định
    InventoryAttribute() : name(""), value(0.0) {}
    InventoryAttribute(const string &name, double value) : name(name), value(value) {}
    bool operator==(const InventoryAttribute &other) const {
        return name == other.name && fabs(value - other.value) < 1E-8;
    }
    string toString() const {
        ostringstream oss;
        oss << name << ": " << fixed << setprecision(6) << value;
        return oss.str();
    }
};

inline ostream &operator<<(ostream &os, const InventoryAttribute &attr) {
    os << attr.toString();
    return os;
}

// ======================= List1D =======================
template <typename T>
class List1D {
private:
    IList<T>* pList;
public:
    List1D() {
        pList = new XArrayList<T>(nullptr, nullptr, 10);
    }
    List1D(int num_elements) {
        int cap = (num_elements > 10 ? num_elements : 10);
        pList = new XArrayList<T>(nullptr, nullptr, cap);
        for (int i = 0; i < num_elements; i++) {
            pList->add(T());
        }
    }
    List1D(const T* array, int num_elements) {
        int cap = (num_elements > 10 ? num_elements : 10);
        pList = new XArrayList<T>(nullptr, nullptr, cap);
        for (int i = 0; i < num_elements; i++) {
            pList->add(array[i]);
        }
    }
    // Copy constructor
    List1D(const List1D<T>& other) {
        XArrayList<T>* pOther = dynamic_cast<XArrayList<T>*>(other.pList);
        if(pOther)
            pList = new XArrayList<T>(*pOther);
        else
            throw logic_error("List1D copy: underlying list not of type XArrayList");
    }

    List1D<T>& operator=(const List1D<T>& other) {
        if(this == &other)
            return *this;
        delete pList;
        XArrayList<T>* pOther = dynamic_cast<XArrayList<T>*>(other.pList);
        if(pOther)
            pList = new XArrayList<T>(*pOther);
        else
            throw logic_error("List1D operator=: underlying list not of type XArrayList");
        return *this;
    }
    virtual ~List1D() {
        delete pList;
    }
    int size() const {
        return pList->size();
    }
    T get(int index) const {
        if(index < 0 || index >= size())
            throw out_of_range("Index is invalid!");
        return pList->get(index);
    }
    void set(int index, T value) {
        if(index < 0 || index >= size())
            throw out_of_range("Index is invalid!");
        pList->get(index) = value;
    }
    void add(const T &value) {
        pList->add(value);
    }

    void removeAt(int index) {
        if(index < 0 || index >= size())
            throw out_of_range("Index is invalid!");
        pList->removeAt(index);
    }
    string toString() const {
        return pList->toString();
    }
    friend ostream &operator<<(ostream &os, const List1D<T>& list) {
        os << list.toString();
        return os;
    }
};

// ======================= List2D =======================
template <typename T>
class List2D {
private:
    IList< List1D<T>* >* pMatrix;
public:
    List2D() {
        pMatrix = new XArrayList< List1D<T>* >(nullptr, nullptr, 10);
    }

    List2D(List1D<T>* array, int num_rows) {
        int cap = (num_rows > 10 ? num_rows : 10);
        pMatrix = new XArrayList< List1D<T>* >(nullptr, nullptr, cap);
        for (int i = 0; i < num_rows; i++) {
            pMatrix->add(new List1D<T>(array[i]));
        }
    }

    List2D(const List2D<T>& other) {
        int r = other.rows();
        pMatrix = new XArrayList< List1D<T>* >(nullptr, nullptr, (r > 10 ? r : 10));
        for (int i = 0; i < r; i++) {
            pMatrix->add(new List1D<T>(*(other.pMatrix->get(i))));
        }
    }

    List2D<T>& operator=(const List2D<T>& other) {
        if(this == &other)
            return *this;

        int r = rows();
        for (int i = 0; i < r; i++) {
            delete pMatrix->get(i);
        }
        delete pMatrix;
        int r2 = other.rows();
        pMatrix = new XArrayList< List1D<T>* >(nullptr, nullptr, (r2 > 10 ? r2 : 10));
        for (int i = 0; i < r2; i++) {
            pMatrix->add(new List1D<T>(*(other.pMatrix->get(i))));
        }
        return *this;
    }
    virtual ~List2D() {
        int r = rows();
        for (int i = 0; i < r; i++) {
            delete pMatrix->get(i);
        }
        delete pMatrix;
    }
    int rows() const {
        return pMatrix->size();
    }

    void addRow(List1D<T>* row) {
        pMatrix->add(row);
    }

    List1D<T>* removeRow(int rowIndex) {
        if(rowIndex < 0 || rowIndex >= rows())
            throw out_of_range("Index is invalid!");
        return pMatrix->removeAt(rowIndex);
    }

    void setRow(int rowIndex, const List1D<T>& row) {
        if(rowIndex < 0 || rowIndex >= rows())
            throw out_of_range("Index is invalid!");
        delete pMatrix->removeAt(rowIndex);
        pMatrix->add(rowIndex, new List1D<T>(row));
    }

    T get(int rowIndex, int colIndex) const {
        if(rowIndex < 0 || rowIndex >= rows())
            throw out_of_range("Index is invalid!");
        List1D<T>* row = pMatrix->get(rowIndex);
        if(colIndex < 0 || colIndex >= row->size())
            throw out_of_range("Index is invalid!");
        return row->get(colIndex);
    }

    List1D<T> getRow(int rowIndex) const {
        if(rowIndex < 0 || rowIndex >= rows())
            throw out_of_range("Index is invalid!");
        return *(pMatrix->get(rowIndex));
    }
    string toString() const {
        ostringstream oss;
        oss << "[";
        int r = rows();
        for (int i = 0; i < r; i++) {
            if(i > 0)
                oss << ", ";
            oss << pMatrix->get(i)->toString();
        }
        oss << "]";
        return oss.str();
    }
    friend ostream &operator<<(ostream &os, const List2D<T>& matrix) {
        os << matrix.toString();
        return os;
    }
};

// ======================= InventoryManager =======================
class InventoryManager {
private:
    List2D<InventoryAttribute> attributesMatrix;
    List1D<string> productNames;
    List1D<int> quantities;
public:
    InventoryManager() {

    }
    InventoryManager(const List2D<InventoryAttribute>& matrix,
                     const List1D<string>& names,
                     const List1D<int>& quantities)
      : attributesMatrix(matrix), productNames(names), quantities(quantities) {}
    InventoryManager(const InventoryManager& other)
      : attributesMatrix(other.attributesMatrix),
        productNames(other.productNames),
        quantities(other.quantities) {}
    int size() const {
        return productNames.size();
    }

    List1D<InventoryAttribute> getProductAttributes(int index) const {
        if(index < 0 || index >= size())
            throw out_of_range("Index is invalid!");
        return attributesMatrix.getRow(index);
    }
    string getProductName(int index) const {
        if(index < 0 || index >= size())
            throw out_of_range("Index is invalid!");
        return productNames.get(index);
    }
    int getProductQuantity(int index) const {
        if(index < 0 || index >= size())
            throw out_of_range("Index is invalid!");
        return quantities.get(index);
    }
    void updateQuantity(int index, int newQuantity) {
        if(index < 0 || index >= size())
            throw out_of_range("Index is invalid!");
        quantities.set(index, newQuantity);
    }

    void addProduct(const List1D<InventoryAttribute>& attributes, const string& name, int quantity) {
        List1D<InventoryAttribute>* newRow = new List1D<InventoryAttribute>(attributes);
        attributesMatrix.addRow(newRow);
        productNames.add(name);
        quantities.add(quantity);
    }
    void removeProduct(int index) {
        if(index < 0 || index >= size())
            throw out_of_range("Index is invalid!");
        List1D<InventoryAttribute>* removedRow = attributesMatrix.removeRow(index);
        delete removedRow;
        productNames.removeAt(index);
        quantities.removeAt(index);
    }

    List1D<string> query(int attributeIndex, const double &minValue,
                         const double &maxValue, int minQuantity, bool ascending) const {
        List1D<string> result;
        int n = size();
        vector<pair<double, string>> temp;
        for (int i = 0; i < n; i++) {
            if(getProductQuantity(i) < minQuantity)
                continue;
            List1D<InventoryAttribute> attrs = getProductAttributes(i);
            if(attributeIndex < 0 || attributeIndex >= attrs.size())
                continue;
            InventoryAttribute attr = attrs.get(attributeIndex);
            if(attr.value >= minValue && attr.value <= maxValue)
                temp.push_back(make_pair(attr.value, getProductName(i)));
        }
        if(ascending)
            sort(temp.begin(), temp.end(), [](const pair<double,string>& a, const pair<double,string>& b){
                return a.first < b.first;
            });
        for(auto &p : temp)
            result.add(p.second);
        return result;
    }

    void removeDuplicates() {
        int n = size();
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; ) {
                if (getProductName(i) == getProductName(j)) {
                    List1D<InventoryAttribute> attrs_i = getProductAttributes(i);
                    List1D<InventoryAttribute> attrs_j = getProductAttributes(j);
                    if (attrs_i.size() == attrs_j.size()) {
                        bool same = true;
                        for (int k = 0; k < attrs_i.size(); k++) {
                            if (!(attrs_i.get(k) == attrs_j.get(k))) {
                                same = false;
                                break;
                            }
                        }
                        if (same) {
                            int newQty = getProductQuantity(i) + getProductQuantity(j);
                            updateQuantity(i, newQty);
                            removeProduct(j);
                            n--;
                            continue;
                        }
                    }
                }
                j++;
            }
        }
    }

    static InventoryManager merge(const InventoryManager &inv1,
                                  const InventoryManager &inv2) {
        InventoryManager merged(inv1);
        int n2 = inv2.size();
        for (int i = 0; i < n2; i++) {
            merged.addProduct(inv2.getProductAttributes(i),
                              inv2.getProductName(i),
                              inv2.getProductQuantity(i));
        }
        return merged;
    }

    void split(InventoryManager &section1,
               InventoryManager &section2,
               double ratio) const {
        int n = size();
        int n1 = (int)ceil(n * ratio);
        section1 = InventoryManager();
        section2 = InventoryManager();
        for (int i = 0; i < n; i++) {
            if (i < n1)
                section1.addProduct(getProductAttributes(i), getProductName(i), getProductQuantity(i));
            else
                section2.addProduct(getProductAttributes(i), getProductName(i), getProductQuantity(i));
        }
    }
    List2D<InventoryAttribute> getAttributesMatrix() const {
        return attributesMatrix;
    }
    List1D<string> getProductNames() const {
        return productNames;
    }
    List1D<int> getQuantities() const {
        return quantities;
    }

    string toString() const {
        ostringstream oss;
        oss << "InventoryManager[\n";
        oss << "AttributesMatrix: " << attributesMatrix.toString() << ",\n";
        oss << "ProductNames: " << productNames.toString() << ",\n";
        oss << "Quantities: " << quantities.toString() << "\n]";
        return oss.str();
    }
};

#endif /* INVENTORY_MANAGER_H */

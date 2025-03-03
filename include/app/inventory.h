#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include "list/XArrayList.h"
#include "list/DLinkedList.h"
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace std;

// -------------------- List1D --------------------
template <typename T>
class List1D
{
private:
    IList<T> *pList;

public:
    List1D();
    List1D(int num_elements);
    List1D(const T *array, int num_elements);
    List1D(const List1D<T> &other);
    virtual ~List1D();

    int size() const;
    T get(int index) const;
    void set(int index, T value);
    void add(const T &value);
    string toString() const;

    friend ostream &operator<<(ostream &os, const List1D<T> &list);
};

// -------------------- List2D --------------------
template <typename T>
class List2D
{
private:
    IList<IList<T> *> *pMatrix;

public:
    List2D();
    List2D(List1D<T> *array, int num_rows);
    List2D(const List2D<T> &other);
    virtual ~List2D();

    int rows() const;
    void setRow(int rowIndex, const List1D<T> &row);
    T get(int rowIndex, int colIndex) const;
    List1D<T> getRow(int rowIndex) const;
    string toString() const;

    friend ostream &operator<<(ostream &os, const List2D<T> &matrix);
};

struct InventoryAttribute
{
    string name;
    double value;
    InventoryAttribute(const string &name, double value) : name(name), value(value) {}
    string toString() const { return name + ": " + to_string(value); }
};

// -------------------- InventoryManager --------------------
class InventoryManager
{
private:
    List2D<InventoryAttribute> attributesMatrix;
    List1D<string> productNames;
    List1D<int> quantities;

public:
    InventoryManager();
    InventoryManager(const List2D<InventoryAttribute> &matrix,
                     const List1D<string> &names,
                     const List1D<int> &quantities);
    InventoryManager(const InventoryManager &other);

    int size() const;
    List1D<InventoryAttribute> getProductAttributes(int index) const;
    string getProductName(int index) const;
    int getProductQuantity(int index) const;
    void updateQuantity(int index, int newQuantity);
    void addProduct(const List1D<InventoryAttribute> &attributes, const string &name, int quantity);
    void removeProduct(int index);

    List1D<string> query(int attributeName, const double &minValue,
                         const double &maxValue, int minQuantity, bool ascending) const;

    void removeDuplicates();

    static InventoryManager merge(const InventoryManager &inv1,
                                  const InventoryManager &inv2);

    void split(InventoryManager &section1,
               InventoryManager &section2,
               double ratio) const;

    List2D<InventoryAttribute> getAttributesMatrix() const;
    List1D<string> getProductNames() const;
    List1D<int> getQuantities() const;
    string toString() const;
};

// -------------------- List1D Method Definitions --------------------
template <typename T>
List1D<T>::List1D()
{
    // TODO
    pList = new XArrayList<T>();
}

template <typename T>
List1D<T>::List1D(int num_elements)
{
    // TODO
    pList = new XArrayList<T>();

    for(int i = 0; i<num_elements; i++)
        pList->add(T());
}

template <typename T>
List1D<T>::List1D(const T *array, int num_elements)
{
    // TODO
    pList = new XArrayList<T>();

    for(int i = 0; i<num_elements; i++)
        pList->add(array[i]);
}

template <typename T>
List1D<T>::List1D(const List1D<T> &other)
{
    // TODO
    pList = new XArrayList<T>(*static_cast<XArrayList<T>*>(other.pList));
}

template <typename T>
List1D<T>::~List1D()
{
    // TODO
    delete pList;
}

template <typename T>
int List1D<T>::size() const
{
    // TODO
    return pList->size();
}

template <typename T>
T List1D<T>::get(int index) const
{
    // TODO
    return pList->get(index);
}

template <typename T>
void List1D<T>::set(int index, T value)
{
    // TODO
    pList->get(index)=value;
}

template <typename T>
void List1D<T>::add(const T &value)
{
    // TODO
    pList->add(value);
}

template <typename T>
string List1D<T>::toString() const
{
    // TODO
    return pList->toString();
}

template <typename T>
ostream &operator<<(ostream &os, const List1D<T> &list)
{
    // TODO
    os << list.toString();
    return os;
}

// -------------------- List2D Method Definitions --------------------
template <typename T>
List2D<T>::List2D()
{
    // TODO
    pMatrix = new XArrayList<IList<T>*>();
}

template <typename T>
List2D<T>::List2D(List1D<T> *array, int num_rows)
{
    // TODO
    pMatrix = new XArrayList<IList<T>*>();

    for (int i = 0; i < num_rows; i++)
        pMatrix->add(static_cast<IList<T>*>(&array[i]));
}

template <typename T>
List2D<T>::List2D(const List2D<T> &other)
{
    // TODO
    pMatrix = new XArrayList<IList<T>*>();

    int rows = other.rows();

    for (int i = 0; i < rows; i++) {
        IList<T>* originalRow = other.pMatrix->get(i);
        
        List1D<T>* newRow = new List1D<T>( *static_cast<List1D<T>*>(originalRow) );
        
        pMatrix->add(newRow);
    }
}

template <typename T>
List2D<T>::~List2D()
{
    // TODO
    if (pMatrix != nullptr)
    {
        int numRows = pMatrix->size();
        for (int i = 0; i < numRows; i++)
        {
            delete pMatrix->get(i);
        }
        delete pMatrix;
        pMatrix = nullptr;
    }
}

template <typename T>
int List2D<T>::rows() const
{
    // TODO
    return pMatrix->size();
}

template <typename T>
void List2D<T>::setRow(int rowIndex, const List1D<T> &row)
{
    // TODO
    if (rowIndex < 0 || rowIndex >= pMatrix->size())
        throw std::out_of_range("Row index is out of range!");
    
    IList<T>* oldRow = pMatrix->removeAt(rowIndex);
    delete static_cast<List1D<T>*>(oldRow);

    List1D<T>* newRow = new List1D<T>(row);

    pMatrix->add(rowIndex, newRow);
}

template <typename T>
T List2D<T>::get(int rowIndex, int colIndex) const
{
    // TODO
    if (rowIndex < 0 || rowIndex >= pMatrix->size())
        throw std::out_of_range("Row index is out of range!");
    
    IList<T>* row = pMatrix->get(rowIndex);
    
    List1D<T>* listRow = static_cast<List1D<T>*>(row);
    
    return listRow->get(colIndex);
}

template <typename T>
List1D<T> List2D<T>::getRow(int rowIndex) const
{
    // TODO
    if (rowIndex < 0 || rowIndex >= pMatrix->size())
        throw std::out_of_range("Row index is out of range!");
    
    IList<T>* rowPtr = pMatrix->get(rowIndex);
    
    List1D<T>* listRow = static_cast<List1D<T>*>(rowPtr);
    
    return *listRow;
}

template <typename T>
string List2D<T>::toString() const
{
    // TODO
    ostringstream oss;
    oss << "[";
    int numRows = pMatrix->size();
    for (int i = 0; i < numRows; i++) {
        IList<T>* row = pMatrix->get(i);
        List1D<T>* listRow = static_cast<List1D<T>*>(row);
        oss << listRow->toString();
        if (i != numRows - 1)
            oss << ", ";
    }
    oss << "]";
    return oss.str();
}

template <typename T>
ostream &operator<<(ostream &os, const List2D<T> &matrix)
{
    // TODO
    os << matrix.toString();
    return os;
}

// -------------------- InventoryManager Method Definitions --------------------
InventoryManager::InventoryManager()
{
    // TODO
}

InventoryManager::InventoryManager(const List2D<InventoryAttribute> &matrix,
                                   const List1D<string> &names,
                                   const List1D<int> &quantities)
{
    // TODO
}

InventoryManager::InventoryManager(const InventoryManager &other)
{
    // TODO
}

int InventoryManager::size() const
{
    // TODO
}

List1D<InventoryAttribute> InventoryManager::getProductAttributes(int index) const
{
    // TODO
}

string InventoryManager::getProductName(int index) const
{
    // TODO
}

int InventoryManager::getProductQuantity(int index) const
{
    // TODO
}

void InventoryManager::updateQuantity(int index, int newQuantity)
{
    // TODO
}

void InventoryManager::addProduct(const List1D<InventoryAttribute> &attributes, const string &name, int quantity)
{
    // TODO
}

void InventoryManager::removeProduct(int index)
{
    // TODO
}

List1D<string> InventoryManager::query(int attributeIndex, const double &minValue,
                                       const double &maxValue, int minQuantity, bool ascending) const
{
    // TODO
}

void InventoryManager::removeDuplicates()
{
    // TODO
}

InventoryManager InventoryManager::merge(const InventoryManager &inv1,
                                         const InventoryManager &inv2)
{
    // TODO
}

void InventoryManager::split(InventoryManager &section1,
                             InventoryManager &section2,
                             double ratio) const
{
    // TODO
}

List2D<InventoryAttribute> InventoryManager::getAttributesMatrix() const
{
    // TODO
}

List1D<string> InventoryManager::getProductNames() const
{
    // TODO
}

List1D<int> InventoryManager::getQuantities() const
{
    // TODO
}

string InventoryManager::toString() const
{
    // TODO
}

#endif /* INVENTORY_MANAGER_H */

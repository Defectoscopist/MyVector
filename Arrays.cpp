// Arrays.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <memory>
//#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <array>
//#include <algorithm>

using namespace std;

template <typename T>
class MyVector
{
private:
    T* data_;           // pointer to array
    size_t size_;       // current size
    size_t capacity_;    // current capacity

public:

    MyVector() : data_(nullptr), size_(0), capacity_(0)
    {
        cout << "Created MyVector: data_ = nullptr, size_ = " << size_ << " capacity_ = " << capacity_ << '\n';
    };
    MyVector(size_t count) : data_(new T[count]()), size_(count), capacity_(count)
    {
        cout << "Created MyVector: data_ = " << *data_ << " size_ = " << size_ << " capacity_ = " << capacity_ << '\n';
    };

    ~MyVector() { delete[] data_; cout << "vector deleted"; }

    // Return size of array
    __forceinline size_t size() const { return size_; }

    // Return capacity of array
    __forceinline size_t capacity() const { return capacity_; }

    // Return if array is empty
    __forceinline bool empty() const { return size() == 0; }

    // Check is operator[] in range of array
    __forceinline bool assert(size_t index) { return index < size_; }

    // Return data of array
    __forceinline T* data() const { return data_; }

    // Get element of array by index
    T& operator[](size_t index) const
    {
        if (assert(index))
        {
            return data()[index];
        }
        else
        {
            cout << "error: out of range";
            return T();
        }
    }

    // Get first element of array
    T& front() const
    {
        if (!empty())
        {
            return data()[0];
        }
        else
        {
            cout << "error: out of range";
            return T();
        }
    }

    // Get last element of array
    T& back() const
    {
        if (!empty())
        {
            return data()[size_ - 1];
        }
        else
        {
            cout << "error: out of range";
            return T();
        }
    }
};

int main()
{
    MyVector<int>  myVector1;
    MyVector<int> myVector2(5);
    for (size_t i = 0; i < myVector2.size(); i++)
    {
        cout << "myVector2[" << i << "] = " << myVector2.data()[i] << '\n';
    }

    cout << "myVector2 opertor[] " << myVector2[0] << '\n';
    cout << "myVector2.front() " << myVector2.front() << '\n';
    cout << "myVector2.back() " << myVector2.back() << '\n';
}

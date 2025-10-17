// Arrays.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <memory>
#include <string>

using namespace std;


class Person
{
public:
	Person() : name(""), age(0) {};
	Person(const string& n, int a) : name(n), age(a) {};

	~Person() {};

	__forceinline string get_name() const { return name; }
	__forceinline int get_age() const { return age; }

private:

	string name;
	int age;
};

template<typename...Args>
Person makePerson(Args&&...args)
{
	return Person(forward<Args>(args)...);
}

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

	MyVector(const MyVector& other_vector) : data_(new T[other_vector.capacity_]), size_(other_vector.size_), capacity_(other_vector.capacity_)
	{
		for (size_t i = 0; i < size_; i++)
		{
			data_[i] = other_vector.data_[i];
		}
		cout << "Created MyVector from other MyVector: data_ = " << *data_ << " size_ = " << size_ << " capacity_ = " << capacity_ << '\n';
	};

	MyVector(MyVector&& other_vector) noexcept : data_(other_vector.data_), size_(other_vector.size_), capacity_(other_vector.capacity_)
	{
		other_vector.data_ = nullptr;
		other_vector.size_ = 0;
		other_vector.capacity_ = 0;
		cout << "other MyVector transfered to other vector: data_ = " << *data_ << " size_ = " << size_ << " capacity_ = " << capacity_ << '\n';
	};

	~MyVector() { delete[] data_; cout << "vector deleted\n"; }

	// Return size of array
	__forceinline size_t size() const { return size_; }

	// Return capacity of array
	__forceinline size_t capacity() const { return capacity_; }

	// Return if array is empty
	__forceinline bool empty() const { return size_ == 0; }

	// Check is operator[] in range of array
	__forceinline bool is_valid_index(size_t index) const { return index < size_; }

	// Return data of array
	__forceinline T* data() const { return data_; }

	// Get element of array by index
	T& operator[](size_t index)
	{
		if (!is_valid_index(index))
		{
			throw out_of_range("Index " + to_string(index) + " is out of range");
		}
		return data_[index];
	}

	MyVector& operator=(const MyVector& other_vector)
	{
		if (&other_vector != this)
		{
			T* new_data = new T[other_vector.capacity_];
			for (size_t i = 0; i < other_vector.size_; i++)
			{
				new_data[i] = other_vector.data_[i];
			}
			delete[] data_;
			capacity_ = other_vector.capacity_;
			size_ = other_vector.size_;
			data_ = new_data;
		}
		return *this;
	}

	MyVector& operator=(MyVector&& other_vector) noexcept
	{
		if (&other_vector != this)
		{
			delete[] data_;
			capacity_ = other_vector.capacity_;
			size_ = other_vector.size_;
			data_ = other_vector.data_;

			other_vector.data_ = nullptr;
			other_vector.size_ = 0;
			other_vector.capacity_ = 0;
		}
		return *this;
	}

	// Get element of array by index
	const T& operator[](size_t index) const
	{
		if (!is_valid_index(index))
		{
			throw out_of_range("Index " + to_string(index) + " is out of range");
		}
		return data_[index];
	}

	// Get first element of array
	T& front()
	{
		if (empty())
		{
			throw out_of_range("Can't return front element - array is empty");
		}
		return data_[0];
	}

	// Get first element of array
	const T& front() const
	{
		if (empty())
		{
			throw out_of_range("Can't return front element - array is empty");
		}
		return data_[0];
	}

	// Get last element of array
	T& back()
	{
		if (empty())
		{
			throw out_of_range("Can't return last element - array is empty");
		}
		return data_[size_ - 1];
	}

	// Get last element of array
	const T& back() const
	{
		if (empty())
		{
			throw out_of_range("Can't return last element - array is empty");
		}
		return data_[size_ - 1];
	}

	void reserve(size_t new_capacity)
	{
		if (new_capacity > capacity_)
		{
			T* new_data = new T[new_capacity]();
			if (data_ != nullptr)
			{
				for (size_t i = 0; i < size_; i++)
				{
					new_data[i] = data_[i];
				}
				delete[] data_;
			}
			data_ = new_data;
			capacity_ = new_capacity;
		}
	}

	void resize(size_t new_size)
	{
		if (new_size > capacity_)
		{
			reserve(new_size);
		}
		if (new_size > size_)
		{
			for (size_t i = size_; i < new_size; i++)
			{
				data_[i] = T();
			}
		}
		size_ = new_size;
	}

	void push_back(const T& value)
	{
		if ((size_ + 1) > capacity_)
		{
			reserve(capacity_ == 0 ? 1 : 2 * capacity_);
		}
		data_[size_] = value;
		size_++;
	}

	void push_back(T&& value)
	{
		if ((size_ + 1) > capacity_)
		{
			reserve(capacity_ == 0 ? 1 : 2 * capacity_);
		}
		data_[size_] = move(value);
		size_++;
	}

	void pop_back()
	{
		if (!empty())
		{
			size_--;
		}
		else
		{
			throw out_of_range("Can't delete last element - array is empty");
		}
	}

	template <typename... Args>
	void emplace_back(Args&&... args)
	{
		if ((size_ + 1) > capacity_)
		{
			reserve(capacity_ == 0 ? 1 : 2 * capacity_);
		}

		new (&data_[size_]) T(forward<Args>(args)...);
		size_++;
	}
};

	/*
	// some lambdas
	template<typename Func>
	void for_each_element(Func func)
	//for_each_element(const MyVector<T> vector, void (*func)(T&))
	{
		for (size_t i = 0; i < size_; i++)
		{
			func(data_[i]);
			cout << "for_each_element: data_[i] = " << data_[i] << '\n';
		}
	}


	for_each_element (some_vector, [](int& x)
	{
		x +=5  // *func
	});


	template<typename Func>
	int find_if_element(Func predicate) const
	//int find_if_element(const MyVector<T> vector, bool(*predicate)(const T&))
	{
		for (size_t i = 0; i < size_; i++)
		{
			if (predicate(data_[i]))
			{
				cout << "find_if_element: data_[i] = " << data_[i] << '\n';
				return data_[i];
			}
			else
			{
				cout << "find_if_element: false\n";
			}
		}
		return -1;
	}


	int need_index = find_if_element (some_vector, [](const int& x)
	{
		return x > 25; // *predicate
	});
	*/
//};

int main()
{
	auto safe_call = [](const string& op_name, auto&& func)
		{
			try { func(); }
			catch (const exception& e)
			{
				cout << "Error: " << e.what() << '\n';
			}
		};

	MyVector<int>  myVector1;
	MyVector<int> myVector2(5);
	for (size_t i = 0; i < myVector2.size(); i++)
	{
		cout << "myVector2[" << i << "] = " << myVector2.data()[i] << '\n';
	}

	cout << "myVector2 opertor[] " << myVector2[0] << '\n';
	cout << "myVector2.front() " << myVector2.front() << '\n';
	cout << "myVector2.back() " << myVector2.back() << '\n';

	/*
	myVector2.for_each_element([](int& x) {
		x += 5;
	});

	myVector2.find_if_element([](int& x)
		{
		 return x < 25;
		});
	*/

	MyVector<int> reserveVector(5);
	cout << "Before reserve: capacity_ = " << reserveVector.capacity() << '\n';
	reserveVector.reserve(10);
	cout << "After reserve: capacity_ = " << reserveVector.capacity() << '\n';
	reserveVector.reserve(5);
	cout << "After reserve: capacity_ = " << reserveVector.capacity() << '\n';
	reserveVector.resize(20);
	cout << "After resize: size_ = " << reserveVector.size() << '\n';
	reserveVector.resize(5);
	cout << "After resize: size_ = " << reserveVector.size() << '\n';

	void* memory = operator new(sizeof(Person) * 2);
	Person* p1 = new (memory) Person("Dima", 26);
	cout << p1->get_name() << '\n';
	//Person* p2 = new(static_cast<char*>(memory) + sizeof(Person)) Person("Bob", 30);
	Person* p2 = new(reinterpret_cast<Person*>(memory) + 1) Person("Bob", 30);
	cout << p2->get_age() << '\n';

	operator delete(memory);

	Person p3 = makePerson("Peter", 33);
	cout << p3.get_name() << " is " << p3.get_age() << " years old";

}

#pragma once

#include <type_traits>

#include <iostream>
#include <vector>

#include <cassert>

template<typename T, typename Enable = void>
class NDArray;

template <class T>
class NDArray <T, typename std::enable_if<std::is_same<T, int>::value || std::is_same<T, float>::value || std::is_same<T, double>::value>::type>
{
public:

	NDArray(int size, T fill = 0)	 // Создание пустого массива заданного размера and Создание массива заполненного нулями
	{
		if (size)
		{
			this->ptr = new T[size];
		}
		else
		{
			this->ptr = nullptr;
		}

		this->size = size;

		this->offset = nullptr;

		this->ndim = 1;
		this->shape.first = 1;
		this->shape.second = size;

		for (int i = 0; i < this->size; i++)
		{
			this->ptr[i] = fill;
		}
		
		return;
	}

	NDArray(int size_rows, int size_cols, T fill) : NDArray(size_rows * size_cols, fill)	 // Создание пустого двумерного массива заданного размера and Создание двумерного массива заполненного нулями
	{
		this->ndim = size_rows > 1 ? 2 : 1;
		this->shape.first = size_rows;
		this->shape.second = size_cols;

		return;
	}

	NDArray(const NDArray<T>& other)
	{
		this->size = other.size;

		this->ndim = other.ndim;
		this->shape.first = other.shape.first;
		this->shape.second = other.shape.second;

		if (other.offset == nullptr)
		{
			this->ptr = new T[other.size];
			this->offset = nullptr;
			for (int i = 0; i < this->size; i++)
			{
				this->ptr[i] = other.ptr[i];
			}
		}
		else
		{
			this->ptr = other.ptr;
			this->offset = new std::vector<int>;
			for (int i = 0; i < other.offset->size(); i++)
			{
				this->offset->push_back(other.offset->at(i));
			}
		}

		return;
	}

	static NDArray ones(int size_rows, int size_cols)	 // Создание массива заполненного единицами
	{
		return NDArray(size_rows, size_cols, 1);
	}

	static NDArray random_values(int size_rows, int size_cols)	 // Создание массива заполненного случайными значениям
	{
		NDArray res(size_rows, size_cols, 0);

		for (int i = 0; i < res.get_size_rows(); i++)
		{
			for (int j = 0; j < res.get_size_cols(); j++)
			{
				res[std::pair<int, int>(i, j)] = rand();
			}
		}

		return res;
	}

	static NDArray random_values(int size_cols)
	{
		return NDArray::random_values(1, size_cols);
	}

	NDArray operator+(NDArray& other)	// Оператор поэлементного сложения
	{
		return element_wise_operator(other, [](T a, T b) {return a + b; });
	}

	NDArray operator-(NDArray& other)	// Оператор поэлементного вычитания
	{
		return element_wise_operator(other, [](T a, T b) {return a - b; });
	}

	NDArray operator*(NDArray other)	// Оператор поэлементного умножения и матричного умножения
	{
		if (this->ndim == 2)
		{
			assert(this->shape.second == other.shape.first);

			NDArray res(this->shape.first, other.shape.second, 0);

			for (int i = 0; i < this->shape.first; i++)
			{
				for (int j = 0; j < other.shape.second; j++)
				{
					res.get_row(i)[j] = (this->get_row(i) * other.get_col(j))[0];
				}
			}

			return res;
		}
		else if (this->ndim == 1)
		{
			NDArray res = element_wise_operator(other, [](T a, T b) {return a * b; });

			return NDArray(1, res.summ());
		}
		else
		{
			throw "Current NDIM not supported in operator*";
		}

		return NDArray(1, 0);
	}

	NDArray operator/(NDArray& other)	// Оператор поэлементного деления
	{
		return element_wise_operator(other, [](T a, T b) {return a / b; });
	}

	T summ(T start_init = 0)
	{
		T res(start_init);

		for (int i = 0; i < this->shape.first; i++)
		{
			for (int j = 0; j < this->shape.second; j++)
			{
				res += (*this)[std::make_pair(i, j)];
			}
		}

		return res;
	}

	T max()	// Операция сокращения: максимум для определенной размерности
	{
		T  res = (*this)[std::make_pair(0, 0)];

		for (int i = 0; i < this->shape.first; i++)
		{
			for (int j = 0; j < this->shape.second; j++)
			{
				res = (*this)[std::make_pair(i, j)] > res ? (*this)[std::make_pair(i, j)] : res;
			}
		}

		return res;
	}

	T min()	// Операция сокращения: минимум для определенной размерности
	{
		T  res = (*this)[std::make_pair(0, 0)];

		for (int i = 0; i < this->shape.first; i++)
		{
			for (int j = 0; j < this->shape.second; j++)
			{
				res = (*this)[std::make_pair(i, j)] < res ? (*this)[std::make_pair(i, j)] : res;
			}
		}

		return res;
	}

	T avg()	// Операция сокращения: среднее для определенной размерности
	{
		return this->summ() / this->size;
	}

	T& operator[](std::pair<int, int> indexes) const
	{
		if (this->ndim == 1)
		{
			assert((indexes.first + indexes.second) < this->size);	// Так можно передать индекс и в первом и во втором элементе пары
			return this->operator[](indexes.first + indexes.second);
		}
		else if (this->ndim == 2)
		{
			assert((indexes.first * this->shape.second + indexes.second) < this->size);	// Так можно передать индекс и в первом и во втором элементе пары
			return this->ptr[indexes.first * this->shape.second + indexes.second];
		}
		else
		{
			throw "Current NDIM not supported in operator []";
		}
	}

	T& operator[](int index) const
	{
		if (this->ndim == 1)
		{
			assert(index < this->size);

			if (this->offset == nullptr)
			{
				return this->ptr[index];
			}
			else
			{
				return this->ptr[this->offset->at(index)];
			}
		}
		else
		{
			throw "Current NDIM not supported in operator [] with one parameter";
		}
	}

	NDArray transpose()	// Метод для транспонирования
	{
		NDArray res(this->shape.second, this->shape.first, 0);

		for (int i = 0; i < this->shape.second; i++)
		{
			res.get_row(i) = this->get_col(i);
		}

		return res;
	}

	NDArray& operator=(const NDArray& other)
	{
		assert(this->shape == other.shape);

		for (int i = 0; i < this->size; i++)
		{
			(*this)[i] = other[i];
		}

		return *this;
	}

	NDArray get_row(int n) const	// Взятие среза без выделения новой памяти, т.е. изменения в срезе должны менять исходный массив
	{
		assert(n < this->shape.first);

		NDArray res(0);
		res.ptr = this->ptr;
		res.size = this->shape.second;
		res.shape.second = res.size;
		res.offset = new std::vector<int>;

		for (int i = 0; i < this->size; i++)
		{
			res.offset->push_back(n * this->shape.second + i);
		}

		return res;
	}

	NDArray get_col(int n) const	// Взятие среза без выделения новой памяти, т.е. изменения в срезе должны менять исходный массив
	{
		assert(n < this->shape.second);

		NDArray res(0);
		res.ptr = this->ptr;
		res.size = this->shape.first;
		res.shape.second = res.size;
		res.offset = new std::vector<int>;

		for (int i = 0; i < this->size; i++)
		{
			res.offset->push_back(i * this->shape.second + n);
		}

		return res;
	}

	int get_size()
	{
		return this->size;
	}

	int get_size_rows() const
	{
		return this->shape.first;
	}

	int get_size_cols() const
	{
		return this->shape.second;
	}

	friend std::ostream& operator<<(std::ostream& out, const NDArray<T>& arr)
	{
		for (int i = 0; i < arr.get_size_rows(); i++)
		{
			if (i)
				out << "\n";
			out << "[ " << arr[std::make_pair(i, 0)];
			for (int j = 1; j < arr.get_size_cols(); j++)
			{
				out << ", " << arr[std::make_pair(i, j)];
			}
			out << " ]";
		}

		return out;
	}

	~NDArray()
	{
		if (this->offset == nullptr)
		{
			delete[] this->ptr;
		}

		if (this->offset)
		{
			delete this->offset;
		}

		return;
	}

private:
	T* ptr;
	int size;

	std::vector<int>* offset;

	int ndim;
	std::pair<int, int> shape;	// first == quantity rows and second == quantity columns || vector

	NDArray element_wise_operator(NDArray& other, T(*func)(T, T))
	{
		assert(this->shape == other.shape && "Shapes are not equivalent");

		NDArray res(this->size);

		for (int i = 0; i < this->size; i++)
		{
			res[i] = func((*this)[i], other[i]);
		}

		return res;
	}
};
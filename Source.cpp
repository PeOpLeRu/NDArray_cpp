#include <iostream>

#include "NDArray.hpp"

using namespace std;

int main()
{
	NDArray<int> arr(3, 3, 2);

	arr[pair<int, int>(2, 2)] = 8;

	NDArray<int> arr_1(3);
	NDArray<int> arr_2(3);
	for (int i = 0; i < arr_1.get_size(); i++)
	{
		arr_1[i] = i;
		arr_2[i] = i + 1;
	}
	
	std::cout << "Arr 1 == " << arr_1 << std::endl << std::endl;
	std::cout << "Arr 2 == " << arr_2 << std::endl << std::endl;
	auto res = arr_1.element_multiplication(arr_2);
	std::cout << "Res multiply === " << res << std::endl << std::endl;

	NDArray<int> copy_arr = arr;
	std::cout << "SAVED Copy_arr\n";

	std::cout << "Arr == \n" << arr << std::endl << std::endl;
	auto row = arr.get_row(2);
	std::cout << "3 row of arr == " << row << std::endl << std::endl;
	arr[std::make_pair(2, 0)] = 1;
	std::cout << "3 row of arr == " << row << std::endl << std::endl;

	std::cout << "Arr == \n" << arr << std::endl << std::endl;
	auto col = arr.get_col(0);
	std::cout << "\n1 col of arr == " << col << std::endl << std::endl;

	std::cout << "Copy_arr == \n" << copy_arr << std::endl << std::endl;

	arr[pair<int, int>(0, 0)] = 0;
	arr[pair<int, int>(0, 1)] = 1;
	arr[pair<int, int>(0, 2)] = 2;
	arr[pair<int, int>(1, 0)] = 3;
	arr[pair<int, int>(1, 1)] = 4;
	arr[pair<int, int>(1, 2)] = 5;
	arr[pair<int, int>(2, 0)] = 6;
	arr[pair<int, int>(2, 1)] = 7;
	arr[pair<int, int>(2, 2)] = 8;
	std::cout << "Arr == \n" << arr << std::endl << std::endl;
	std::cout << "Transposed arr == \n" << arr.transpose() << std::endl << std::endl;

	std::cout << "arr * transposed arr == \n" << arr * arr.transpose() << std::endl << std::endl;

	std::cout << "----------------- Info about arr * arr.transpose() -----------------" << std::endl;
	auto for_test = arr * arr.transpose();
	std::cout << "max == " << for_test.max() << std::endl;
	std::cout << "min == " << for_test.min() << std::endl;
	std::cout << "avg == " << for_test.avg() << std::endl;
	std::cout << "summ == " << for_test.summ() << std::endl;

	std::cout << "summ of " << for_test.get_row(1) << " == " << for_test.get_row(1).summ() << std::endl;
	std::cout << "min of " << for_test.get_row(1) << " == " << for_test.get_row(1).min() << std::endl;
	std::cout << "min of " << for_test.get_row(2) << " == " << for_test.get_row(2).min() << std::endl << std::endl;


	std::cout << "max(0) == " << for_test.max(0) << std::endl;	// Здесь задаем оси для сокращения (столбец)
	std::cout << "min(0) == " << for_test.min(0) << std::endl;
	std::cout << "avg(0) == " << for_test.avg(0) << std::endl << std::endl;
	
	std::cout << "max(1) == " << for_test.max(1) << std::endl;	// Здесь задаем оси для сокращения (строка)
	std::cout << "min(1) == " << for_test.min(1) << std::endl;
	std::cout << "avg(1) == " << for_test.avg(1) << std::endl << std::endl;
	//std::cout << arr[1];
	//NDArray<int>::random_values(1);

	std::cout << "Check element_multiplication: arr * transposed arr == \n" << arr.element_multiplication(arr.transpose()) << std::endl << std::endl;

	std::cout << "Check + operator: arr + transposed arr == \n" << arr + arr.transpose() << std::endl << std::endl;

	std::cout << "Check method: arr.get_slice(0, arr.get_size_rows(), 2)  == \n" << arr.get_slice(0, arr.get_size_rows(), 2) << std::endl << std::endl;

	std::cout << "Check method: arr.get_slice(0, arr.get_size_rows(), 2).get_slice(0, 6, 2)  == \n" << arr.get_slice(0, arr.get_size_rows(), 2).get_slice(0, 6, 2) << std::endl << std::endl;

	std::cout << "--------- End ---------";

	return 0;
}
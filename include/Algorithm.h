//
// Created by Jacob Lim on 2023/04/08.
//

#ifndef ENGINE_ALGORITHM_H
#define ENGINE_ALGORITHM_H

template <typename T>
static void Swap(T array[], int lhs, int rhs)
{
	const auto temp = array[lhs];
	array[lhs] = array[rhs];
	array[rhs] = temp;
}

template <typename T>
static void SelectionSort(T array[], int size)
{
	for (int i = size - 1; i >= 0; --i)
	{
		int k = 0;
		for (int j = 1; j <= i; ++j)
		{
			if (array[j] > array[k])
			{
				k = j;
			}
		}
		if (k != i)
		{
			Swap(array, k, i);
		}
	}
}

template <typename T>
static void BubbleSort(T array[], int size)
{
	for (int i = 0; i < size; ++i)
	{
		for (int j = 1; j < size - i; ++j)
		{
			if (array[j - 1] > array[j])
			{
				Swap(array, j - 1, j);
			}
		}
	}
}

template <typename T>
static void InsertionSort(T array[], int size)
{
	for (int i = 1; i < size; ++i)
	{
		int location = i - 1;
		int item = array[i];
		while (location >= 0)
		{
			if (item < array[location])
			{
				array[location + 1] = array[location];
				location -= 1;
			}
			else
			{
				break;
			}
		}
		array[location + 1] = item;
	}
}

template <typename T>
static void MergeRecursive(T array[], int lhs, int pivot, int rhs)
{
	int i = lhs;
	int j = pivot + 1;
	int t = 0;
	int temp[rhs - lhs + 1];

	while (i <= pivot && j <= rhs)
	{
		if (array[i] < array[j])
		{
			temp[t++] = array[i++];
		}
		else
		{
			temp[t++] = array[j++];
		}
	}
	while (i <= pivot)
	{
		temp[t++] = array[i++];
	}
	while (j <= rhs)
	{
		temp[t++] = array[j++];
	}
	i = lhs;
	t = 0;
	while (i <= rhs)
	{
		array[i++] = temp[t++];
	}
}

template <typename T>
static void MergeSort(T array[], int lhs, int rhs)
{
	if (lhs >= rhs)
		return;

	int pivot = (lhs + rhs) / 2;
	MergeSort(array, lhs, pivot);
	MergeSort(array, pivot + 1, rhs);
	MergeRecursive(array, lhs, pivot, rhs);
}

template <typename T>
static int Partition(T array[], int lhs, int rhs)
{
	int pivot = array[rhs];
	int i = lhs;

	for (int j = lhs; j < rhs; ++j)
	{
		if (array[j] <= pivot)
		{
			Swap(array, i, j);
			++i;
		}
	}
	Swap(array, i, rhs);
	return i;
}

template <typename T>
static void QuickSortRecursive(T array[], int lhs, int rhs)
{
	if (lhs >= rhs)
		return;
	int pivot = Partition<T>(array, lhs, rhs);
	QuickSortRecursive(array, lhs, pivot - 1);
	QuickSortRecursive(array, pivot + 1, rhs);
}

template <typename T>
static void QuickSort(T array[], int size)
{
	QuickSortRecursive(array, 0, size - 1);
}

template <typename T>
static int QuickSelectionRecursive(T array[], int lhs, int rhs, int index)
{
	if (lhs == rhs)
		return array[lhs];

	int pivot = Partition<T>(array, lhs, rhs);
	if (index < pivot)
		return QuickSelectionRecursive(array, lhs, pivot - 1, index);
	else if (index == pivot)
		return array[pivot];
	else
		return QuickSelectionRecursive(array, pivot + 1, rhs, index);
}

template <typename T>
static int LinearSeachRecursive(T array[], int lhs, int rhs, int index){
	// TODO: 원소의 총수가 5개 이하면 index번째 원소를 찾고 알고리듬을 끝낸다.

	// TODO: 전체 원소를 5개씩의 원소를 가진 n/5개의 그룹으로 나눈다.

	// TODO: 각 그룹에서 중앙값을 찾는다.

	// TODO: 중앙값들의 중앙값 M을 재귀적으로 구한다.

	// TODO: M을 기준원소 삼아 전체 원소를 분할한다.

	// TODO: 분할된 두 그룹 중 적합한 쪽을 선택해 단계 1 ~ 6 반복
}

template <typename T>
static int GetIndex(T array[], int size, int index)
{
	return QuickSelectionRecursive(array, 0, size - 1, index);
}

template <typename T>
static void HeapSort(T array[], int n);

template <typename T>
static void DownHeap(T array[], int i, int n)
{
	int lhs = i * 2 + 1;
	int rhs = i * 2 + 2;
	int largest = i;

	if (lhs < n)
	{
		if (array[lhs] > array[largest])
			largest = lhs;
	}


	if (rhs < n)
	{
		if (array[rhs] > array[largest])
			largest = rhs;
	}

	if (largest != i)
	{
		Swap(array, i, largest);
		DownHeap(array, largest, n);
	}
}


#endif // ENGINE_ALGORITHM_H

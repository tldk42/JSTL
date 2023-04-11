#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

/**@ pair<cost, total-cost> */
using Path = std::pair<int, int>;

template <typename T>
static void Swap(T array[], int lhs, int rhs)
{
	const auto temp = array[lhs];
	array[lhs] = array[rhs];
	array[rhs] = temp;
}

namespace DP
{
	int GetMin(int left, int bottom, int bottomLeft)
	{
		return std::min((std::min(left, bottom)), (std::min(left, bottomLeft)));
	}

	/**
	 * Current WayPoint (r, c)     \n
	 * ---- Previous WayPoint ---- \n
	 * left        (r, c + 1)	   \n
	 * bottom      (r - 1, c)	   \n
	 * left-bottom (r - 1, c + 1)  \n
	 * @param map 2 dimension array
	 * @param n len
	 * @return Min path on adjacent array
	 */
	int FindMinPath(Path** map, int n)
	{

		for (int r = 0; r < n; ++r)
		{
			for (int c = n - 1; c >= 0; --c)
			{
				if (c == n - 1 && r == 0)
					continue;
				if (c == n - 1 && r != 0)
					map[r][c].second = map[r][c].first + map[r - 1][c].second;

				else if (r == 0 && c != n - 1)
					map[r][c].second = map[r][c].first + map[r][c + 1].second;

				else
					map[r][c].second =
					    map[r][c].first + GetMin(map[r][c + 1].second, map[r - 1][c].second, map[r - 1][c + 1].second);
			}
		}
		return map[n - 1][0].second;
	}
} // namespace DP

namespace Selection
{

	template <typename T>
	int Partition(T array[], int lhs, int rhs)
	{
		int pivot = array[rhs];
		int i = lhs;

		for (int j = lhs; j < rhs; ++j)
		{
			if (array[j] <= pivot)
				Swap(array, i++, j);
		}
		Swap(array, i, rhs);
		return i;
	}

	template <typename T>
	T QuickSelect(T array[], int lhs, int rhs, int index)
	{
		if (lhs == rhs)
			return array[lhs];

		int pivot = Partition(array, lhs, rhs);
		int k = pivot - lhs ;

		if (index < k)
			return QuickSelect(array, lhs, pivot - 1, index);
		else if (index == k)
			return array[pivot];
		else
			return QuickSelect(array, pivot + 1, rhs, index - k);
	}

	template <typename T>
	T MOMPartition(T array[], int lhs, int rhs, int middle)
	{
		int index = middle;
		// TODO: find the index of middle in array
		for (int i = lhs; i < rhs; ++i)
			if (array[i] == array[middle])
				index = i;
		// TODO: swap array[index] and array[rhs]
		Swap(array, index, rhs);
		// TODO: return partition(array, lhs, rhs)
		return Partition(array, lhs, rhs);
	}

	template <typename T>
	T MOMSelect(T array[], int lhs, int rhs, int index)
	{
		int size = rhs - lhs;
		if (size <= 5)
			return QuickSelect(array, lhs, rhs, index);

		int groupSize = ceil(size / 5.0);

		T group[groupSize];

		for (int i = lhs; i < rhs; i += 5)
		{
			int mid = QuickSelect(array, i, std::min(i + 4, rhs), 2);
			group[i / 5] = mid;
		}

		int mid = MOMSelect(group, 0, groupSize - 1, ceil(groupSize / 2.0));

		int pivot = MOMPartition(array, lhs, rhs, mid);

		int k = pivot - lhs;

		if (index < k)
			return MOMSelect(array, lhs, pivot - 1, index);
		else if (index == k)
			return array[pivot];
		else
			return MOMSelect(array, pivot + 1, rhs, index);
	}

} // namespace Selection

int main()
{

	int data[2000];
	for (int i = 0; i < 2000; i++)
	{
		data[i] = i + 1;
	}

	std::cout << "중간값 : " << Selection::MOMSelect(data, 0, 1999, 1000) << std::endl;

//	for (int i = 0; i < 10; i++)
//	{
//		// 데이터를 랜덤하게 섞는다.
//		std::shuffle(data, data + 9999, g);
//
//		// 매번 5000 이 나와야 한다.
//	}
}

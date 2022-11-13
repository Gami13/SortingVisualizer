namespace Sorts
{

	static void quicksortB(std::vector<long long int> &arr, int left, int right)
	{
		int i = left, j = right;
		int tmp;
		int pivot = arr[(left + right) / 2];

		/* partition */
		while (i <= j)
		{
			while (arr[i] < pivot)
				i++;
			while (arr[j] > pivot)
				j--;
			if (i <= j)
			{
				Step s;
				s.i = i;
				s.j = j;
				steps.push_back(s); // save step for drawing
				tmp = arr[i];
				arr[i] = arr[j];

				arr[j] = tmp;
				i++;
				j--;
			}
		};

		/* recursion */
		if (left < j)
			quicksortB(arr, left, j);
		if (i < right)
			quicksortB(arr, i, right);
	}

	static void quicksort(std::vector<long long int> &arr)
	{
		Sorts::quicksortB(arr, 0, arr.size() - 1);
	}
}
namespace Sorts
{

	static void quicksortB(std::vector<unsigned int> &arr, int left, int right)
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

	static void quicksort(std::vector<unsigned int> &arr)
	{
		Sorts::quicksortB(arr, 0, arr.size() - 1);
	}
	void sort(std::vector<unsigned int> arr, int passes, void (*function)(std::vector<unsigned int> &))
	{
		int i = 0;
		while (i < passes)
		{

			auto start = std::chrono::steady_clock::now();
			function(arr);

			auto finish = std::chrono::steady_clock::now();

			std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() << "ms" << std::endl;
			i++;
		}
	}
}
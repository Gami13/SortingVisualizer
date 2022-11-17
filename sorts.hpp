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
			amountOfIterations++;
			while (arr[i] < pivot)
				i++;
			while (arr[j] > pivot)
				j--;
			if (i <= j)
			{
				amountOfComparisons++;

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
		{
			amountOfComparisons++;
			quicksortB(arr, left, j);
		}
		if (i < right)
		{
			amountOfComparisons++;
			quicksortB(arr, i, right);
		}
	}

	static void quicksort(std::vector<unsigned int> &arr)
	{
		Sorts::quicksortB(arr, 0, arr.size() - 1);
	}
	static void bubbleSort(std::vector<unsigned int> &arr)
	{
		int n = arr.size();
		for (int i = 0; i < n - 1; i++)
		{
			for (int j = 0; j < n - i - 1; j++)
			{
				amountOfIterations++;
				if (arr[j] > arr[j + 1])
				{
					amountOfComparisons++;
					Step s;
					s.i = j;
					s.j = j + 1;
					steps.push_back(s); // save step for drawing
					std::swap(arr[j], arr[j + 1]);
				}
			}
		}
	}
	static void insertionSort(std::vector<unsigned int> &arr)
	{
		int n = arr.size();
		for (int i = 1; i < n; ++i)
		{
			int key = arr[i];
			int j = i - 1;

			while (j >= 0 && arr[j] > key)
			{
				amountOfIterations++;
				amountOfComparisons++;
				Step s;
				s.i = j;
				s.j = j + 1;
				steps.push_back(s); // save step for drawing
				arr[j + 1] = arr[j];
				--j;
			}
			arr[j + 1] = key;
		}
	}
	void sort(std::vector<unsigned int> arr, int passes, void (*function)(std::vector<unsigned int> &))
	{
		steps.clear();

		std::vector<float> times;
		int i = 0;
		while (i < passes)
		{
			steps.clear();
			amountOfIterations = 0;
			auto start = std::chrono::steady_clock::now();
			function(arr);

			auto finish = std::chrono::steady_clock::now();
			float time = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() / 1000000.0;
			times.push_back(time);
			std::cout << time << "ms" << std::endl;
			i++;
		}
		fastestSort = *std::min_element(times.begin(), times.end());
		slowestSort = *std::max_element(times.begin(), times.end());

		averageSort = std::reduce(times.begin(), times.end()) / times.size();
	}
}
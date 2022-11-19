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
	static void selectionSort(std::vector<unsigned int> &arr)
	{
		int n = arr.size();
		for (int i = 0; i < n - 1; i++)
		{
			int min_idx = i;
			for (int j = i + 1; j < n; j++)
			{
				amountOfIterations++;
				if (arr[j] < arr[min_idx])
				{
					amountOfComparisons++;
					min_idx = j;
				}
			}
			Step s;
			s.i = i;
			s.j = min_idx;
			steps.push_back(s); // save step for drawing
			std::swap(arr[min_idx], arr[i]);
		}
	}
	static void cocktailSort(std::vector<unsigned int> &arr)
	{
		bool swapped = true;
		int start = 0;
		int end = arr.size();

		while (swapped)
		{
			swapped = false;

			for (int i = start; i < end - 1; ++i)
			{
				amountOfIterations++;
				if (arr[i] > arr[i + 1])
				{
					amountOfComparisons++;
					Step s;
					s.i = i;
					s.j = i + 1;
					steps.push_back(s); // save step for drawing
					std::swap(arr[i], arr[i + 1]);
					swapped = true;
				}
			}

			if (!swapped)
				break;

			swapped = false;

			--end;

			for (int i = end - 1; i >= start; --i)
			{
				amountOfIterations++;
				if (arr[i] > arr[i + 1])
				{
					amountOfComparisons++;
					Step s;
					s.i = i;
					s.j = i + 1;
					steps.push_back(s); // save step for drawing
					std::swap(arr[i], arr[i + 1]);
					swapped = true;
				}
			}

			++start;
		}
	}
	static void heapify(std::vector<unsigned int> &arr, int n, int i)
	{
		int largest = i;
		int l = 2 * i + 1;
		int r = 2 * i + 2;

		if (l < n && arr[l] > arr[largest])
		{
			amountOfComparisons++;
			largest = l;
		}

		if (r < n && arr[r] > arr[largest])
		{
			amountOfComparisons++;
			largest = r;
		}

		if (largest != i)
		{
			Step s;
			s.i = i;
			s.j = largest;
			steps.push_back(s); // save step for drawing
			std::swap(arr[i], arr[largest]);

			heapify(arr, n, largest);
		}
	}
	static void heapSort(std::vector<unsigned int> &arr)
	{
		int n = arr.size();

		for (int i = n / 2 - 1; i >= 0; i--)
		{
			amountOfIterations++;
			heapify(arr, n, i);
		}

		for (int i = n - 1; i >= 0; i--)
		{
			amountOfIterations++;
			Step s;
			s.i = 0;
			s.j = i;
			steps.push_back(s); // save step for drawing
			std::swap(arr[0], arr[i]);

			heapify(arr, i, 0);
		}
	}
	/* getNextGap */
	static int getNextGap(int gap)
	{
		gap = (gap * 10) / 13;
		if (gap < 1)
			return 1;
		return gap;
	}
	static void combSort(std::vector<unsigned int> &arr)
	{
		int n = arr.size();

		int gap = n;

		bool swapped = true;

		while (gap != 1 || swapped == true)
		{
			gap = getNextGap(gap);

			swapped = false;

			for (int i = 0; i < n - gap; i++)
			{
				amountOfIterations++;
				if (arr[i] > arr[i + gap])
				{
					amountOfComparisons++;
					Step s;
					s.i = i;
					s.j = i + gap;
					steps.push_back(s); // save step for drawing
					std::swap(arr[i], arr[i + gap]);
					swapped = true;
				}
			}
		}
	}
	static void shellSort(std::vector<unsigned int> &arr)
	{
		int n = arr.size();

		for (int gap = n / 2; gap > 0; gap /= 2)
		{
			for (int i = gap; i < n; i += 1)
			{
				int temp = arr[i];

				int j;
				for (j = i; j >= gap && arr[j - gap] > temp; j -= gap)
				{
					amountOfIterations++;
					amountOfComparisons++;
					Step s;
					s.i = j;
					s.j = j - gap;
					steps.push_back(s); // save step for drawing
					arr[j] = arr[j - gap];
				}

				arr[j] = temp;
			}
		}
	}
	static void cycleSort(std::vector<unsigned int> &arr)
	{
		int writes = 0;

		for (int cycle_start = 0; cycle_start <= arr.size() - 2; cycle_start++)
		{
			unsigned int item = arr[cycle_start];

			int pos = cycle_start;
			for (int i = cycle_start + 1; i < arr.size(); i++)
			{
				amountOfIterations++;
				if (arr[i] < item)
				{
					amountOfComparisons++;
					pos++;
				}
			}

			if (pos == cycle_start)
				continue;

			while (item == arr[pos])
			{
				amountOfIterations++;
				pos += 1;
			}

			if (pos != cycle_start)
			{
				Step s;
				s.i = cycle_start;
				s.j = pos;
				steps.push_back(s); // save step for drawing
				std::swap(item, arr[pos]);
				writes++;
			}

			while (pos != cycle_start)
			{
				pos = cycle_start;

				for (int i = cycle_start + 1; i < arr.size(); i++)
				{
					amountOfIterations++;
					if (arr[i] < item)
					{
						amountOfComparisons++;
						pos += 1;
					}
				}

				while (item == arr[pos])
				{
					amountOfIterations++;
					pos += 1;
				}

				if (item != arr[pos])
				{
					Step s;
					s.i = cycle_start;
					s.j = pos;
					steps.push_back(s); // save step for drawing
					std::swap(item, arr[pos]);
					writes++;
				}
			}
		}
	}
	static void gnomeSort(std::vector<unsigned int> &arr)
	{
		int index = 0;

		while (index < arr.size())
		{
			amountOfIterations++;
			if (index == 0)
				index++;
			if (arr[index] >= arr[index - 1])
				index++;
			else
			{
				amountOfComparisons++;
				Step s;
				s.i = index;
				s.j = index - 1;
				steps.push_back(s); // save step for drawing
				std::swap(arr[index], arr[index - 1]);
				index--;
			}
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
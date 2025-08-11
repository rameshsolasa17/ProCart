/* ---------------------------------------------------------------------------
Purpose:
This algorithm efficiently sorts a large dataset using external sort and the 
PolyPhase Merge technique.
----------------------------------------------------------------------------*/
 #ifndef _MY_EXTERNAL_SORT_
 #define _MY_EXTERNAL_SORT_
 #include <vector>
 #include <iostream>
 #include <cstdlib>
 #include <ctime>
 #include <queue>
 #include <assert.h>
 #include <string>
 #include <fstream>
 #include <limits.h>
 #include <math.h>
 #include <chrono>
 using namespace std;
 // total number of tapes you have,
 // note that you should leave one tape for output during merging
 const int TOTAL_TAPES = 16;
 // the number of elements that can be loaded in to the main memory each time
 const int BATCH_SIZE = 32;
 // a list of kth order Fibonacci numbers
 static std::vector<size_t> fibo_series;
 // writes sorted block to the tape in inital pass
 void writeBlockToTape(std::string tape, std::vector<long long int> block)
 {
  std::ofstream of(tape, ios::app);
  if (of.is_open())
  {
    for (int i = 0; i < BATCH_SIZE; ++i)
    {
      of << block[i] << '\n';
    }
    of.close();
  }
  else
  {
    std::cerr << "Error opening " << tape << " for writing." << '\n';
  }
  return;
}
 void AllElementsToOutputFile(std::string inputFileName, std::string outputFileName,
 long long int numberOfBlocks, long long int numberOfElements, bool reverse = false)
 {
  std::ifstream inputFile(inputFileName);
  std::ofstream outputFile(outputFileName, std::ios::trunc);
  if (!inputFile.is_open() || !outputFile.is_open())
  {
    std::cerr << "Error opening files." << '\n';
    return;
  }
  std::string line;
  if (reverse)
  {
    long long int temp = (numberOfBlocks * BATCH_SIZE) - numberOfElements;
    while (temp--)
    {
    }
  }
      getline(inputFile, line);
  for (long long int i = 0; i < numberOfElements; i++)
  {
    if (std::getline(inputFile, line))
      outputFile << line << '\n';
  }
  inputFile.close();
  outputFile.close();
  return;
 }
 void RemoveElementsFromTape(std::string file_name)
 {
  std::ofstream file(file_name, std::ios::trunc);
  if (file.is_open())
  {
    file.close();
  }
  else
  {
    std::cout << "Error opening the file." << '\n';
  }
  return;
 }
 /*-----------------------------------------------------------------------------
  Function: ComputeKthFibonacci
    Compute K-th order Fibonacci number for computing number of blocks should store
 in each tape.
  Note: you may call this function as many times as needed.------------------------------------------------------------------------------*/
 void ComputeKthFibonacci(void)
 {
  /* formula:
    F(k)(0, 1, ..., k-1) = 0, 0, ..., 1
    F(k)(n) = F(k)(n − 1) + F(k)(n − 2) + ··· + F(k)(n − k), n >= k
    when n == k:
      F(k)(k) = F(k)(k − 1) + F(k)(k − 2) + ··· + F(k)(k − k)
              = 1           + 0           + ... + 0 = 1
    when n > k:
      F(k)(n)     = F(k)(n − 1) + F(k)(n − 2) + ··· + F(k)(n − k)
      F(k)(n)     = F(k)(n − 1) + [F(k)(n − 2) + ··· + F(k)(n − k) + F(k)(n − k-1)]- F(k)(n − k-1)
      F(k)(n − 1) = F(k)(n − 2) + F(k)(n − 3) + ··· + F(k)(n − k) + F(k)(n − k-1)
      F(k)(n)     = 2*F(k)(n − 1) - F(k)(n − k-1)
  */
  int k = TOTAL_TAPES - 1;
  if (fibo_series.empty()) // initial fibo_series
  {
    for (int i = 0; i < k - 1; i++)
      fibo_series.push_back(0);
    fibo_series.push_back(1);
    fibo_series.push_back(1); // this is fibo_series[k]
  }
  for (int i = 0; i < 100; i++) // compute next 100 items for usage
  {
    int n = fibo_series.size();
    size_t new_item = 2 * fibo_series[n - 1] - fibo_series[n - k - 1];
    fibo_series.push_back(new_item);
  }
 }
 /*-----------------------------------------------------------------------------
  Function: GetBlocksAt
    get the number of blocks you should write to at current pass.
  Vairables:
    pass      - current pass of PolyPhaseMergePass.
    blocks_at - where you want to store the block counts of eath tape to.
  Note:
  For the t-th input tape at pass n, it should hold blocks_at[t] blocks:
    blocks_at[t] = F(k)(n+k-2) + F(k)(n+k-3) + ... + F(k)(n+t-2), t = 1, 2, ..., k------------------------------------------------------------------------------*/
 void GetBlocksAt(long long int pass, std::vector<long long int> &blocks_at)
 {
  if (pass < 1)
    return;
  int k = TOTAL_TAPES - 1;
  blocks_at = std::vector<long long int>(k, 0);
  for (int t = 0; t < k; t++)
  {
    for (long long int tmp_i = pass + t - 1; tmp_i <= pass + k - 2; tmp_i++)
    {
      if (tmp_i >= fibo_series.size())
        ComputeKthFibonacci(); // compute more fibo_series
      blocks_at[t] += fibo_series[tmp_i];
    }
  }
 }
 /*-----------------------------------------------------------------------------
  Function: IsSorted
    Checks if the content in input_file is sorted
  Vairables:
    input_file - the file name that you want to check if it's sorted
    reverse    -  if set true, sort in descending order; otherwise in ascending 
order
  Note: you can use this to check if your result is correctly sorted.------------------------------------------------------------------------------*/
 bool IsSorted(const std::string &input_file, bool reverse = false)
 {
  bool sorted = true;
  std::ifstream in(input_file.c_str());
  if (!in.is_open())
  {
    std::cout << input_file << " doesn't exist!\n";
    return false;
  }
  else
  {
    std::string buffer;
    long long int prev = LLONG_MIN, curr;
    if (reverse)
      prev = LLONG_MAX;
    while (!in.eof())
    {
      in >> curr;
      if ((curr < prev && !reverse) || (curr > prev && reverse))
      {
        sorted = false;
        std::cout << "Out of order: " << prev << ", " << curr << std::endl;
        break;
      }
      prev = curr;
    }
  }
  in.close();
  return sorted;
 }
 /*-----------------------------------------------------------------------------
  The insertion sort algorithm.
  Vairables:
   a              - the input array
   left and right - the left and end indexes of the range of the elements to be 
sorted, inclusive
   reverse        - if set true, sort in descending order. Default: false------------------------------------------------------------------------------*/
 template <typename Comparable>
 void InsertionSort(std::vector<Comparable> &a, long long int left, long long int 
right, bool reverse = false)
 {
  // CODE BEGINS
  for (long long int i = left + 1; i <= right; ++i)
  {
    Comparable temp = a[i];
    int j = i;
    if (!reverse)
    {
      for (j = i; j > left && temp < a[j - 1]; j--)
      {
        a[j] = a[j - 1];
      }
    }
    else
    {
      for (j = i; j > left && temp > a[j - 1]; j--)
      {
        a[j] = a[j - 1];
      }
    }
    a[j] = temp;
  }
 }
 vector<long long int> LoadSortInputBlocks(
    const std::string &input_file,
    const std::vector<std::string> &ext_arrays,
    bool reverse = false)
 {
  long long int cnt_pass = 1;
  long long int numberofElements = 0;
  long long int numberofBlocks = 0;
  int tapeNumber = 0;
  std::ifstream input(input_file);
  std::string line;
  std::vector<long long int> currBlocks(TOTAL_TAPES - 1, 0);
  std::vector<long long int> blockInCurrPass(TOTAL_TAPES - 1, 0);
  std::vector<long long int> newBlocks(TOTAL_TAPES - 1, 0);
  while (!input.eof())
  {
    GetBlocksAt(cnt_pass, blockInCurrPass);
    long long int totalBlockToBeAdded = 0;
    for (int i = 0; i < TOTAL_TAPES - 1; i++)
    {
      newBlocks[i] = blockInCurrPass[i] - currBlocks[i];
      totalBlockToBeAdded += newBlocks[i];
      currBlocks[i] = blockInCurrPass[i];
    }
    numberofBlocks += totalBlockToBeAdded;
    while (totalBlockToBeAdded > 0)
    {
      std::vector<long long int> block;
      for (int i = 0; i < BATCH_SIZE; i++)
      {
        if (std::getline(input, line))
        {
          block.push_back(std::stoll(line));
          numberofElements++;
        }
        else
        {
          block.push_back(LLONG_MAX);
        }
      }
      InsertionSort(block, 0, BATCH_SIZE - 1, reverse);
      long long int tapeCounter = 0;
      // Write the block to the corresponding tape
      while (true && tapeCounter < 15)
      {
        if (newBlocks[(tapeNumber % (TOTAL_TAPES - 1))])
        {
          writeBlockToTape(ext_arrays[(tapeNumber % (TOTAL_TAPES - 1))], block);
          // Reduce the number of blocks that needs to be inserted by 1
          totalBlockToBeAdded--;
          // Reduce the number of block to be inserted in the particular tape by 1.
          newBlocks[(tapeNumber % (TOTAL_TAPES - 1))]--;
          break;
        }
        else
        {
          tapeCounter++;
          tapeNumber++;
        }
      }
      tapeNumber++;
    }
    cnt_pass++;
  }
  return {cnt_pass - 1, numberofBlocks, numberofElements};
 }
 /*-----------------------------------------------------------------------------
   Function: PolyPhaseMerge
   You should
     (1) load data from k external tapes
     (2) repeat (3) until you have merged all blocks in any one of the tapes:
     (3) merge one block from each tape into one block, and write the merged block 
into an external tape.
     (4) clean up intermediate files.
   Vairables:
     cnt_pass    - total number of passes you need to perform PolyPhaseMerges,
                   this is computed from LoadSortInputBlocks
     ext_arrays  - the names of the list of files that serve as the external arrays
     reverse     - if set true, sort in descending order; otherwise in ascending 
order------------------------------------------------------------------------------*/
 void sortPass(std::ifstream in[], int outputIndex, long long int passNumber, 
std::vector<long long int> &numberOfElements, std::vector<long long int> 
&blocksAtIthPass, std::vector<std::string> &ext_arrays, bool reverse)
 {
  std::string line;
  std::vector<long long int> leftElements = numberOfElements;
  long long int firstmin, secondmin;
  firstmin = secondmin = LLONG_MAX;
  for (int i : blocksAtIthPass)
  {
    if (i < firstmin)
    {
      secondmin = firstmin;
      firstmin = i;
    }
    else if (i < secondmin && i != firstmin)
      secondmin = i;
  }
  int temp = secondmin;
  std::ofstream of;
  of.open(ext_arrays[outputIndex % TOTAL_TAPES], ios::app);
  if (!reverse)
  {
    while (temp--)
    {
      leftElements = numberOfElements;
      std::priority_queue<std::pair<long long int, int>, std::vector<std::pair<long
 long int, int>>, std::greater<std::pair<long long int, int>>> pq;
      for (int k = 0; k < TOTAL_TAPES; k++)
      {
        if ((outputIndex % TOTAL_TAPES) != k)
        {
          if (getline(in[k], line))
          {
            pq.push({std::stoll(line), k});
            leftElements[k]--;
          }
        }
      }
      while (!pq.empty())
      {
        pair<long long int, int> it = pq.top();
        pq.pop();
        long long int value = it.first;
        int tapeNumber = it.second;
        of << value << '\n';
        if (leftElements[tapeNumber])
        {
          if (getline(in[tapeNumber], line))
          {
            pq.push({std::stoll(line), tapeNumber});
            leftElements[tapeNumber]--;
          }
        }
      }
    }
    for (int i = 0; i < TOTAL_TAPES; i++)
    {
      if (blocksAtIthPass[i] == secondmin)
      {
        RemoveElementsFromTape(ext_arrays[i]);
      }
    }
    of.close();
    in[outputIndex % TOTAL_TAPES].close();
    in[outputIndex % TOTAL_TAPES].open(ext_arrays[outputIndex % TOTAL_TAPES]);
    for (int i = 0; i < TOTAL_TAPES; i++)
    {
      blocksAtIthPass[i] = abs(blocksAtIthPass[i] - secondmin);
    }
  }
  else
  {
    while (temp--)
    {
      leftElements = numberOfElements;
      std::priority_queue<std::pair<long long int, int>> pq;
      for (int k = 0; k < TOTAL_TAPES; k++)
      {
        if ((outputIndex % TOTAL_TAPES) != k)
        {
          if (getline(in[k], line))
          {
            pq.push({std::stoll(line), k});
            leftElements[k]--;
          }
        }
      }
      while (!pq.empty())
      {
        pair<long long int, int> it = pq.top();
        pq.pop();
        long long int value = it.first;
        int tapeNumber = it.second;
        of << value << '\n';
        if (leftElements[tapeNumber])
        {
          if (getline(in[tapeNumber], line))
          {
            pq.push({std::stoll(line), tapeNumber});
            leftElements[tapeNumber]--;
          }
        }
      }
    }
    for (int i = 0; i < TOTAL_TAPES; i++)
    {
      if (blocksAtIthPass[i] == secondmin)
      {
        RemoveElementsFromTape(ext_arrays[i]);
      }
    }
    of.close();
    in[outputIndex % TOTAL_TAPES].close();
    in[outputIndex % TOTAL_TAPES].open(ext_arrays[outputIndex % TOTAL_TAPES]);
    for (int i = 0; i < TOTAL_TAPES; i++)
    {
      blocksAtIthPass[i] = abs(blocksAtIthPass[i] - secondmin);
    }
  }
 }
 int PolyPhaseMerge(const long long int cnt_pass, std::vector<std::string> 
&ext_arrays, bool reverse)
 {
  int outputIndex = TOTAL_TAPES - 1;
  std::vector<long long int> numberOfElements(TOTAL_TAPES - 1, BATCH_SIZE);
  numberOfElements.push_back(0);
  long long int pass = cnt_pass;
  std::ifstream in[TOTAL_TAPES];
  for (int k = 0; k < TOTAL_TAPES; k++)
  {
    in[k].open(ext_arrays[k].c_str());
  }
  std::vector<long long int> blocksAtIthPass(TOTAL_TAPES - 1, 0);
  GetBlocksAt(cnt_pass, blocksAtIthPass);
  blocksAtIthPass.push_back(0);
  while (pass)
  {
    sortPass(in, outputIndex, pass, numberOfElements, blocksAtIthPass, ext_arrays, 
reverse);
    long long int temp = 0;
    for (int i = 0; i < TOTAL_TAPES; i++)
    {
      temp += numberOfElements[i];
    }
    numberOfElements[(outputIndex % TOTAL_TAPES)] = temp;
    outputIndex--;
    outputIndex = (outputIndex + TOTAL_TAPES) % TOTAL_TAPES;
    numberOfElements[(outputIndex % TOTAL_TAPES)] = 0;
    pass--;
  }
  for (int k = 0; k < TOTAL_TAPES; k++)
  {
    if (in[k].is_open())
      in[k].close();
  }
  return (outputIndex + 1) % TOTAL_TAPES;
 }
 /*-----------------------------------------------------------------------------
 The driver external sort function function
   input_file   - the file name that contains the inputs
   output_file  - the file name that contains the final sorted elements
   reverse      - if set true, sort in descending order; otherwise in ascending 
order------------------------------------------------------------------------------*/
 void ExternalSort(const std::string &input_file, const std::string &output_file, 
bool reverse = false)
 {
  // Create Tapes for sorting
  std::vector<string> ext_arrays;
  for (int i = 0; i < TOTAL_TAPES; ++i)
  {
    std::string file_name = "./tapes" + std::to_string(i) + ".txt";
    ext_arrays.push_back(file_name);
    std::ofstream file(file_name);
    if (file.is_open())
    {
    }
      file.close();
    else
    {
      std::cerr << "Error creating file ";
    }
  }
  // Load Initial Blocks To Tapes
  std::vector<long long int> res = LoadSortInputBlocks(input_file, ext_arrays, 
reverse);
  long long int cnt_pass = res[0];
  long long int numberofBlocks = res[1];
  long long int numberofElements = res[2];
  // PolyPhase Merge
  int finalOutputIndex = PolyPhaseMerge(cnt_pass, ext_arrays, reverse);
  // Adds all elements of the output tape to output file
  AllElementsToOutputFile(ext_arrays[finalOutputIndex], output_file, 
numberofBlocks, numberofElements, reverse);
 }
 #endif
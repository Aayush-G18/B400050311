
/*
  Windows does not support user defined reductions.
  This program may not run on MVSC++ compilers for Windows.
  Please use Linux Environment.[You can try using "windows subsystem for linux"]
*/

#include<iostream>
#include<omp.h>
#include<numeric>
#include<algorithm>
using namespace std;
int minval(long int arr[], int n){
  int minval = arr[0];
  #pragma omp parallel for reduction(min : minval)
    for(int i = 0; i < n; i++){
      if(arr[i] < minval) minval = arr[i];
    }
  return minval;
}

long int maxval(long int arr[], int n){
  long int maxval = arr[0];
  #pragma omp parallel for reduction(max : maxval)
    for(int i = 0; i < n; i++){
      if(arr[i] > maxval) maxval = arr[i];
    }
  return maxval;
}

long int sum(long int arr[], int n){
  long int sum = 0;
  #pragma omp parallel for reduction(+ : sum)
    for(int i = 0; i < n; i++){
      sum += arr[i];
    }
  return sum;
}

long int average(long int arr[], int n){
  return (double)sum(arr, n) / n;
}
//Sequential

int seq_minval(long int arr[], int n) {
  return *min_element(arr, arr + n);
}

long int seq_maxval(long int arr[], int n) {
  return *max_element(arr, arr + n);
}

long int seq_sum(long int arr[], int n) {
  return accumulate(arr, arr + n, 0);
}

double seq_average(long int arr[], int n) {
  return (double)seq_sum(arr, n) / n;
}

int main(){
  int n = 1e8;
  long int* arr=new long int[n];
  for(int i = 0, j = n; i < n; i++, j--) arr[i] = j;
  double start_time,end_time;
  start_time = omp_get_wtime();
  cout << "Sequential Min: " << seq_minval(arr, n) << '\n';
  cout << "Sequential Max: " << seq_maxval(arr, n) << '\n';
  cout << "Sequential Sum: " << seq_sum(arr, n) << '\n';
  cout << "Sequential Avg: " << seq_average(arr, n) << '\n';
  end_time = omp_get_wtime();
  cout << "Time (Sequential): " << end_time - start_time << " seconds\n\n";
  start_time = omp_get_wtime();
  cout << "Parallel Min: " << minval(arr, n) << '\n';
  cout << "Parallel Max: " << maxval(arr, n) << '\n';
  cout << "Parallel Sum: " << sum(arr, n) << '\n';
  cout << "Parallel Avg: " << average(arr, n) << '\n';
  end_time = omp_get_wtime();
  cout << "Time (Parallel): " << end_time - start_time << " seconds\n";
  delete [] arr;
  return 0;
}
// Compile: g++ path/to/file/file_name.cpp -fopenmp

// Execute: ./a.out [Linux] or ./a.exe [Windows]

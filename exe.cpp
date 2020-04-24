#include <vector>
#include <stdio.h>
using std::vector;

class Solution {
public:
    bool Find(int target, vector<vector<int> > array) {
        if(array.size() == 0) return false;
        for(unsigned i=0; i<array.size();i++){
            if(bs(target, array[i])){
                printf("true\n");
                return true;
            }
        }
        printf("false\n");
        return false;
    }
    
    bool bs(int target, vector<int>& arr){
        if(arr.size()==0) return false;
        int left = 0, right = arr.size() - 1;
        while (left + 1 < right){
            int mid = left + (right-left)/2;
            if(target < arr[mid]){
                right = mid;
            }else if(target > arr[mid]){
                left = mid;
            }else{
                return true;
            }
        }
        printf("arr[left]==%d arr[right]==%d\n", arr[left], arr[right]);
        return (arr[left] == target) || (arr[right] == target);
    }
};

int main(){
    Solution s;
    vector<vector<int>> array;
    vector<int> arr{1,2,8,9},arr1{2,4,9,12},arr2{4,7,10,13},arr3{6,8,11,15};
    array.push_back(arr); array.push_back(arr1); array.push_back(arr2);array.push_back(arr3);
    s.Find(5, array);
    return 0;
}
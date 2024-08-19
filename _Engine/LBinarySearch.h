#pragma once

#include <iostream>
#include <vector>

template <typename T>
inline int binarySearch(const std::vector<T>& arr, T target) {

    //Checks for when array is of size 0 and 1
    if (arr.size() == 0)
        return -1;
    if (arr.size() == 1)
        return arr[0] == target ? 0 : -1;

    int left = 0, right = arr.size() - 1;

    // Determine if the array is sorted in ascending or descending order
    bool isAscending = arr[0] < arr[1];

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            return mid; // Target found
        }

        if (isAscending) {
            // Ascending order logic
            if (arr[mid] < target) {
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }
        else {
            // Descending order logic
            if (arr[mid] > target) {
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }
    }

    return -1; // Target not found
}

template <typename T>
inline int linearSearch(const std::vector<T>& arr, T target)
{
    for (size_t i = 0; i < arr.size(); i++)
    {
        if (arr[i] == target)
            return i;
    }

    return -1;
}

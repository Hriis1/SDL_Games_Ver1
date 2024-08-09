#pragma once

#include <iostream>
#include <vector>

template <typename T>
inline int binarySearch(const std::vector<T>& arr, T target) {
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

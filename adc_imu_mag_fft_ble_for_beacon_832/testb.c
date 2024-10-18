#include <stdio.h>

// 交换两个整数的值
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 选择前 8 大的元素及其位置
void find_top_8(int values[], int positions[], int size) {
    // 遍历数组，找到前 8 大的元素
    for (int i = 0; i < 8; i++) {
        int max_idx = i;  // 假设当前元素是最大的
        for (int j = i + 1; j < size; j++) {
            if (values[j] > values[max_idx]) {
                max_idx = j;
            }
        }
        // 交换当前最大值到第 i 个位置
        if (max_idx != i) {
            swap(&values[i], &values[max_idx]);
            swap(&positions[i], &positions[max_idx]);
        }
    }
}

int main() {
    // 数值数组和位置数组
    int values[] = {12, 45, 2, 89, 33, 75, 22, 67, 55, 99, 18, 44};
    int positions[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};  // 位置数组
    int size = sizeof(values) / sizeof(values[0]);

    // 查找前 8 大的元素
    find_top_8(values, positions, size);

    // 输出前 8 大的数值及其位置
    printf("前 8 大的数值及其位置：\n");
    for (int i = 0; i < 8; i++) {
        printf("值: %d, 位置: %d\n", values[i], positions[i]);
    }

    return 0;
}
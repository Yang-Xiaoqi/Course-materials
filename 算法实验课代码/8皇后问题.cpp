#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

const int N = 8;
int maxSum = 0; 
vector<vector<int>> board(N, vector<int>(N)); 

bool isSafe(vector<int> queens, int row, int col) {
    for (int i = 0; i < row; i++) { // ֻ����ѷ��õ���
        if (queens[i] == col || abs(row - i) == abs(col - queens[i])) 
            return false;
    }
    return true;
}

void solve(int row, vector<int> queens, int currentSum) {
    if (row == N) {
        maxSum = max(maxSum, currentSum);
        return;
    }
    for (int col = 0; col < N; col++) {
        if (isSafe(queens, row, col)) {
            queens[row] = col; // ���ûʺ�
            solve(row + 1, queens, currentSum + board[row][col]);
//            queens[row] = -1; // ����
        }
    }
}

int main() {
    int k;
    cin >> k;
    while (k--) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                cin >> board[i][j];
        
        maxSum = 0;
        vector<int> queens(N, -1); // ��ʼ���ʺ�λ��
        solve(0, queens, 0);
        cout << maxSum << endl;
    }
    return 0;
}


#include <bits/stdc++.h>
#include <chrono> 

using namespace std;
using namespace std::chrono;

const int BITWISE_MASK = (1 << 16) - 1; 

// Function to compute hash for a given K*K submatrix
uint64_t computeHash(const vector<vector<char>> &matrix, int startRow, int startCol, int K) {
    uint64_t hashValue = 0;
    for(int i = 0; i < K; i++){
        for(int j = 0; j < K; j++){
            hashValue = ((hashValue << 5) ^ matrix[startRow + i][startCol + j]) & BITWISE_MASK;
        }
    }

    return hashValue;
}

// Rolling hash update when moving down
uint64_t rollDown(uint64_t prevHash, const vector<vector<char>> &matrix, int row, int col, int K) {
    for(int j = 0; j < K; j++){
        prevHash = ((prevHash << 5) ^ matrix[row + K - 1][col + j]) & BITWISE_MASK;
    }

    return prevHash;
}

// Function to check if the top-right K*K corner is duplicated in the matrix
bool isDuplicated(const vector<vector<char>> &picture, int M, int N, int K) {
    if(K > M || K > N) {
        return false; 
    }

    if(K == 1){
        char targetChar = picture[0][N - 1];
        for(int i = 0; i < M; i++){
            for(int j = 0; j < N; j++){
                if((i != 0 || j != (N - 1)) && picture[i][j] == targetChar) { 
                    return true;
                }
            }
        }
        return false;
    }

    auto startTotal = high_resolution_clock::now();

    auto startHash = high_resolution_clock::now();

    uint64_t targetHash = computeHash(picture, 0, N - K, K);

    auto endHash = high_resolution_clock::now();

    auto startRowHashes = high_resolution_clock::now();

    vector<uint64_t> rowHashes(N - K + 1);
    for(int j = 0; j <= N - K; j++){
        rowHashes[j] = computeHash(picture, 0, j, K);

        if(rowHashes[j] == targetHash && j != N - K){
            return true;
        }
    }

    auto endRowHashes = high_resolution_clock::now();

    auto startRolling = high_resolution_clock::now();

    for(int i = 1; i <= M - K; i++){
        for(int j = 0; j <= N - K; j++){
            rowHashes[j] = rollDown(rowHashes[j], picture, i, j, K);

            if(rowHashes[j] == targetHash){
                auto endTotal = high_resolution_clock::now();
                cout << "Total Time: " << duration_cast<microseconds>(endTotal - startTotal).count() << "µs\n";
                cout << "Top-Right KxK Hash Computation: " << duration_cast<microseconds>(endHash - startHash).count() << "µs\n";
                cout << "Initial Row Hashing: " << duration_cast<microseconds>(endRowHashes - startRowHashes).count() << "µs\n";
                cout << "Rolling Hash Updates: " << duration_cast<microseconds>(high_resolution_clock::now() - startRolling).count() << "µs\n";
                return true;
            }
        }
    }

    auto endRolling = high_resolution_clock::now();
    auto endTotal = high_resolution_clock::now();

    cout << "Total Time: " << duration_cast<microseconds>(endTotal - startTotal).count() << "µs\n";
    cout << "Top-Right KxK Hash Computation: " << duration_cast<microseconds>(endHash - startHash).count() << "µs\n";
    cout << "Initial Row Hashing: " << duration_cast<microseconds>(endRowHashes - startRowHashes).count() << "µs\n";
    cout << "Rolling Hash Updates: " << duration_cast<microseconds>(endRolling - startRolling).count() << "µs\n";

    return false;
}


int main() {
    vector<vector<char>> picture = {
        {'D', 'E', 'B', 'C', 'D', 'E'},
        {'I', 'J', 'G', 'H', 'I', 'J'},
        {'I', 'J', 'D', 'D', 'N', 'O'},
        {'B', 'D', 'D', 'J', 'S', 'T'},
        {'B', 'I', 'J', 'W', 'X', 'Y'},
        {'B', 'I', 'J', 'W', 'X', 'Y'},
        {'B', 'I', 'J', 'W', 'X', 'Y'}
    };

    int M = picture.size(), N = picture[0].size();
    int K = 1;
    
    cin >> K;

    if(isDuplicated(picture, M, N, K)){
        cout << "Duplicate found!\n";
    }else{
        cout << "No duplicate found.\n";
    }

    return 0;
}


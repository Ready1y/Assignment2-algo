#include <bits/stdc++.h>

using namespace std;

// Function to find a ^ n & mod 
int findPower(int a, int n, int mod) {
    if(n == 0){
        return 1;
    }

    if(n == 1){
        return a & mod; 
    }

    int pow = findPower(a, n / 2, mod);
    if(n & 1){
        return ((a & mod) * (pow & mod) * (pow & mod)) & mod;
    }else{
        return ((pow & mod) * (pow & mod)) & mod;
    }
}

// Function to find the first hash of first k rows
vector<int> findHash(vector<vector<char>> &picture, int row, int mod, int radix) {
    vector<int> hash;
    int col = picture[0].size();
    for(int i = 0; i < col; i++) {
        int h = 0;
        for(int j = 0; j < row; j++) {
            h = (((h * radix) & mod) + (picture[j][i] & mod)) & mod; 
        }
        hash.push_back(h);
    }

    return hash;
}

// Function to check if all values of pattern matches with the text
bool check(vector<vector<char>> &picture, vector<vector<char>> &pattern, int r, int c) {
    for(int i = 0; i < pattern.size(); i++) {
        for(int j = 0; j < pattern[0].size(); j++) {
            if(pattern[i][j] != picture[i + r][j + c]) {
                return false;
            }
        }
    }

    return true;
}

// Function to calculate rolling hash for columns
void colRollingHash(vector<vector<char>> &picture, vector<int> &pictureHash, int row, int n2, int rowPow, int mod, int radix) {
        
    for(int i = 0; i < pictureHash.size(); i++) {
        pictureHash[i] = (pictureHash[i] & mod) - ((picture[row][i] & mod) * (rowPow & mod)) & mod;
        pictureHash[i] = ((pictureHash[i] & mod) * (radix & mod)) & mod;
        pictureHash[i] = ((pictureHash[i] & mod) + (picture[row + n2][i] & mod)) & mod;
    }
}

// Function to find the indices of picture[][] where pattern[][] is present
vector<vector<int>> searchPattern(vector<vector<char>> &picture, vector<vector<char>> &pattern) {
    int n1 = picture.size(), m1 = picture[0].size();
    int n2 = pattern.size(), m2 = pattern[0].size();

    int mod = 255;
    int radix = 256;

    vector<vector<int>> res;

    int rowPow = findPower(radix, n2 - 1, mod);

    int colPow = findPower(radix, m2 - 1, mod);

    vector<int> pictureHash = findHash(picture, n2, mod, radix);

    vector<int> patHash = findHash(pattern, n2, mod, radix);

    int patternHashVal = 0;
    for(int i = 0; i < m2; i++) {
        patternHashVal = (patternHashVal * radix + patHash[i]) & mod; 
    }

    for(int i = 0; i <= n1 - n2; i++) {

        int pictureHashVal = 0;
        for(int j = 0; j < m2; j++){
            pictureHashVal = (pictureHashVal * radix + pictureHash[j]) & mod; 
        }

        for(int j = 0; j <= m1 - m2; j++){
            if(pictureHashVal == patternHashVal){
                if(check(picture, pattern, i, j)){
                    res.push_back({i, j});
                }
            }

            pictureHashVal = (pictureHashVal & mod) - ((pictureHash[j] & mod) * (colPow & mod)) & mod;
            pictureHashVal = ((pictureHashVal & mod) * (radix & mod)) & mod;
          	if(j + m2 < m1){
            	pictureHashVal = ((pictureHashVal & mod) + (pictureHash[j + m2] & mod)) & mod;
            }
        }

        if(i < n1 - n2){
            colRollingHash(picture, pictureHash, i, n2, rowPow, mod, radix);
        }
    }

    return res;
}

vector<vector<char>> extractTopRightCorner(const vector<vector<char>> &matrix, int K) {
    int M = matrix.size();       
    int N = matrix[0].size();    

    if(K > M || K > N){
        cout << "K is too large for the given matrix dimensions.\n";
        return {};
    }

    vector<vector<char>> submatrix(K, vector<char>(K));

    for(int i = 0; i < K; ++i){
        for(int j = 0; j < K; ++j){
            submatrix[i][j] = matrix[i][N - K + j]; 
        }
    }

    return submatrix;
}

int main() {
    vector<vector<char>> picture = {
        {'D', 'F', 'B', 'C', 'D', 'E'},
        {'I', 'J', 'G', 'H', 'I', 'J'},
        {'I', 'J', 'D', 'D', 'N', 'O'},
        {'B', 'D', 'D', 'J', 'S', 'T'},
        {'B', 'I', 'J', 'W', 'X', 'Y'},
        {'B', 'I', 'J', 'W', 'X', 'Y'},
        {'B', 'I', 'J', 'W', 'X', 'Y'}
    };

    int K; 

    cin >> K;

    vector<vector<char>> pat = extractTopRightCorner(picture, K);

    vector<vector<int>> res = searchPattern(picture, pat);
    
    if(res.size() > 1){
        cout << "Duplicate found!\n";
    }else{
        cout << "No duplicate found.\n";
    }

    return 0;
}

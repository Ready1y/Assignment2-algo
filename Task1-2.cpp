#include <iostream>
#include <vector>
#include <queue>

using namespace std;

pair<bool, int> Brute_match(string patt, string data) {
    pair<bool, int> result;
    result.first = false;
    result.second = -1;

    if(patt.length() > data.length()){
        return result;
    }

    for (int i = 0; i <= data.length() - patt.length(); i++) {  // Limit i to data.length() - patt.length()
        bool same = true;
        for (int j = 0; j < patt.length(); j++) {  // Loop through the pattern length
            if (patt[j] != data[i + j]) {  // Compare the substring of data with the pattern
                same = false;
                break;  // No need to continue once a mismatch is found
            }
        }
        if (same) {
            result.first = same;
            result.second = i + patt.length() - 1;
            return result;  // Found a match
        }
    }
    return result;  // No match found
}


//shift by the smallest possible amount
pair<bool, int> Sunday_match(string patt, string data) {
    pair<bool, int> result;
    result.first = false;
    result.second = -1;

    int patt_length = patt.size();
    int data_length = data.size();

    if (patt_length == 0) return result;

    vector<int> shift_table(256, patt_length);

    for (int i = 0; i < patt_length; ++i) {
        shift_table[patt[i]] = patt_length - i;
    }

    int i = 0;
    while (i <= data_length - patt_length) {
        // Check for match
        if (data.substr(i, patt_length) == patt) {
            //cout<<"match at i=="<<i<<endl;
            result.first = true;
            result.second = i + patt_length - 1;
            return result;
        }

        // Shift based on next character in patt
        int next_index = i + patt_length;
        int shift = shift_table[data[next_index]];

        i += shift;
    }
    return result; // No match found
}


queue<string> DivideString(string pattern){
    queue<string> strings;

    int patternL = pattern.length();
    string currentPattern = pattern;
    for(int i = 0; i < patternL; i++){
        if(pattern[i] == '*'){
            if(i == 0){
                currentPattern = pattern.substr(1, patternL - 1);

                continue;
            }
            
            if(i + 1 == patternL){
                strings.push(currentPattern.substr(0, currentPattern.length() - 1));

                continue;
            }

            if(i > 0 && pattern[i - 1] == '\\'){
                continue;
            }
            
            strings.push(pattern.substr(patternL - currentPattern.length(), i - (patternL - currentPattern.length())));
            currentPattern = pattern.substr(i + 1, patternL - (i + 1));

            continue;
        } else if(pattern[i] == '?'){
                if(i == 0){
                    strings.push("?");
                    
                    currentPattern = pattern.substr(1, patternL - 1);
    
                    continue;
                }
                
                if(i + 1 == patternL){
                    strings.push(currentPattern.substr(0, currentPattern.length() - 1));
                    
                    strings.push("?");
    
                    continue;
                }
    
                if(i > 0 && pattern[i - 1] == '\\'){
                    continue;
                }
                
                strings.push(pattern.substr(patternL - currentPattern.length(), i - (patternL - currentPattern.length())));
                
                strings.push("?");
    
                currentPattern = pattern.substr(i + 1, patternL - (i + 1));
    
                continue;
        }else   {
            if(i == patternL - 1){
                strings.push(currentPattern);
            }
        }
    }

    queue<string> finalStrings;
    while(strings.size() > 0){
        string s = strings.front();

        string finalString = "", currentString = s;
        for(int i = 0; i <= s.length(); i++){
            if(s[i] == '\\' && i + 1 != s.length() && ( s[i + 1] == '*' || s[i + 1] == '?')){
                finalString += s.substr(s.length() - currentString.length(), i - (s.length() - currentString.length()));
                currentString = s.substr(i + 1, patternL - (i + 1));
            }
        }

        finalString += currentString;
        finalStrings.push(finalString);
        strings.pop();
    }
    
    return finalStrings;
}

pair<bool, int> Implement(string s, queue<string> dividedStrings, int option = 1){
    pair<bool, int> lastResult;
    lastResult.first = true;
    lastResult.second = -1;

    int lastPointer = 0;
    while(dividedStrings.size() > 0){
        string frontElement = dividedStrings.front();

        if(!lastResult.first){
            break;
        }

        lastPointer = lastResult.second + 1;
        if(lastPointer >= s.length()){
            lastResult.first = false;

            break;
        }

        if(frontElement == "?"){
            dividedStrings.pop();

            string tempS = dividedStrings.front();

            lastPointer++;

            if(s.substr(lastPointer, tempS.length()) == tempS){
                lastResult.first = true;
                lastResult.second = lastPointer + tempS.length() - 1;
            }else{
                lastResult.first = false;
            }

            dividedStrings.pop();

            continue;
        }

        if(option == 1){
            lastResult = Brute_match(frontElement, s.substr(lastPointer, s.length() - lastPointer));
        }else if(option == 2){
            lastResult = Sunday_match(frontElement, s.substr(lastPointer, s.length() - lastPointer));
        }
        
        dividedStrings.pop();
    }

    return lastResult;
}

int main(){
    string s, patt;
    cin >> s >> patt;

    queue<string> dividedStrings;
    dividedStrings = DivideString(patt);

    pair<bool, int> result1 = Implement(s, dividedStrings,  1);

    if(result1.first){
        cout<<" True"<<endl;
    }else{
        cout<<"False"<<endl;
    }

    cout << s << " " << patt<< endl;

    pair<bool, int> result2 = Implement(s, dividedStrings, 2);

    if(result2.first){
        cout<<" True"<<endl;
    }else{
        cout<<"False"<<endl;
    }

    return 0;
}
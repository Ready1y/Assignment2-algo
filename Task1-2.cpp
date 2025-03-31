#include <iostream>
#include <vector>
#include <queue>

using namespace std;

pair<bool, int> Brute_match(string pattern, string inputString) {
    pair<bool, int> result;
    result.first = false;
    result.second = -1;

    if(pattern.length() > inputString.length()){
        return result;
    }

    for (int i = 0; i <= inputString.length() - pattern.length(); i++) {  // Limit i to inputString.length() - pattern.length()
        bool same = true;
        for (int j = 0; j < pattern.length(); j++) {  // Loop through the patternern length
            if (pattern[j] != inputString[i + j]) {  // Compare the substring of inputString with the patternern
                same = false;
                break;  // No need to continue once a mismatch is found
            }
        }
        if (same) {
            result.first = same;
            result.second = i + pattern.length() - 1;
            return result;  // Found a match
        }
    }
    return result;  // No match found
}


//shift by the smallest possible amount
pair<bool, int> Sunday_match(string pattern, string inputString) {
    pair<bool, int> result;
    result.first = false;
    result.second = -1;

    int pattern_length = pattern.size();
    int data_length = inputString.size();

    if (pattern_length == 0) return result;

    vector<int> shift_table(256, pattern_length);

    for (int i = 0; i < pattern_length; i++) {
        shift_table[pattern[i]] = pattern_length - i;
    }

    int i = 0;
    while (i <= data_length - pattern_length) {
        // Check for match
        if (inputString.substr(i, pattern_length) == pattern) {
            //cout<<"match at i=="<<i<<endl;
            result.first = true;
            result.second = i + pattern_length - 1;
            return result;
        }

        // Shift based on next character in pattern
        int next_index = i + pattern_length;
        int shift = shift_table[inputString[next_index]];

        i += shift;
    }

    return result; // No match found
}

//A function that divides lines into those that do not contain wildcards
queue<string> DivideString(string patternern){
    queue<string> strings;

    int patternernL = patternern.length();
    string currentpatternern = patternern;
    for(int i = 0; i < patternernL; i++){
        if(patternern[i] == '*'){
            if(i == 0){
                currentpatternern = patternern.substr(1, patternernL - 1);

                continue;
            }
            
            if(i + 1 == patternernL){
                strings.push(currentpatternern.substr(0, currentpatternern.length() - 1));

                continue;
            }

            if(i > 0 && patternern[i - 1] == '\\'){
                continue;
            }
            
            strings.push(patternern.substr(patternernL - currentpatternern.length(), i - (patternernL - currentpatternern.length())));
            currentpatternern = patternern.substr(i + 1, patternernL - (i + 1));

            continue;
        } else if(patternern[i] == '?'){
                if(i == 0){
                    strings.push("?");
                    
                    currentpatternern = patternern.substr(1, patternernL - 1);
    
                    continue;
                }
                
                if(i + 1 == patternernL){
                    strings.push(currentpatternern.substr(0, currentpatternern.length() - 1));
                    
                    strings.push("?");
    
                    continue;
                }
    
                if(i > 0 && patternern[i - 1] == '\\'){
                    continue;
                }
                
                strings.push(patternern.substr(patternernL - currentpatternern.length(), i - (patternernL - currentpatternern.length())));
                
                strings.push("?");
    
                currentpatternern = patternern.substr(i + 1, patternernL - (i + 1));
    
                continue;
        }else   {
            if(i == patternernL - 1){
                strings.push(currentpatternern);
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
                currentString = s.substr(i + 1, patternernL - (i + 1));
            }
        }

        finalString += currentString;
        finalStrings.push(finalString);
        strings.pop();
    }
    
    return finalStrings;
}

//Function that implement our algorithms on the divided strings
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

            if(dividedStrings.size() <= 0){
                break;
            }

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
    pair<string, string> inputValues[10];
    inputValues[0].first = "abccba"; inputValues[0].second = "cba";
    inputValues[1].first = "abccba"; inputValues[1].second = "b*b";
    inputValues[2].first = "abccba"; inputValues[2].second = "c*c";
    inputValues[3].first = "abccba"; inputValues[3].second = "*ab";
    inputValues[4].first = "abccba"; inputValues[4].second = "ba*";
    inputValues[5].first = "abccba"; inputValues[5].second = "a?c";
    inputValues[6].first = "abccba"; inputValues[6].second = "?bc";
    inputValues[7].first = "abccba"; inputValues[7].second = "ab?";
    inputValues[8].first = "abccba"; inputValues[8].second = "a?b";
    inputValues[9].first = "abccba"; inputValues[9].second = "bbb";

    for(int i = 0; i <= 9; i++){
        queue<string> dividedStrings;
        dividedStrings = DivideString(inputValues[i].second);

        pair<bool, int> result1 = Implement(inputValues[i].first, dividedStrings,  1);

        cout << "Input string is " << inputValues[i].first << ", pattern is " << inputValues[i].second << endl;

        if(result1.first){
            cout<<"Brude force result is TRUE"<<endl;
        }else{
            cout<<"Brude force result is FALSE"<<endl;
        }

        pair<bool, int> result2 = Implement(inputValues[i].first, dividedStrings, 2);

        if(result2.first){
            cout<<"Sunday algorithm result is TRUE"<<endl;
        }else{
            cout<<"Sunday algorithm result is FALSE"<<endl;
        }
    }

    return 0;
}
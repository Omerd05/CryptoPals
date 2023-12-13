#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>;

using namespace std;
typedef long long ll;
const int INF = 1e9;

int freq[256]; //frequency of english characters

void init() {
    for (int i = 0; i < 123; i++)freq[i] = 0;
    //oof
    freq['e'] = 11.1607;
    freq['a'] = 8.4966;
    freq['r'] = 7.5809;
    freq['i'] = 7.5448;
    freq['o'] = 7.1635;
    freq['t'] = 6.9509;
    freq['n'] = 6.6544;
    freq['s'] = 5.7351;
    freq['l'] = 5.4893;
    freq['c'] = 4.5388;
    freq['u'] = 3.6308;
    freq['d'] = 3.3844;
    freq['p'] = 3.1671;
    freq['m'] = 3.0129;
    freq['h'] = 3.0034;
    freq['g'] = 2.4705;
    freq['b'] = 2.0720;
    freq['f'] = 1.8121;
    freq['y'] = 1.7779;
    freq['w'] = 1.2899;
    freq['k'] = 1.1016;
    freq['v'] = 1.0074;
    freq['x'] = 0.2902;
    freq['z'] = 0.2722;
    freq['j'] = 0.1965;
    freq['q'] = 0.1962;
}

inline string byteToHex(int x) {
    vector<char> temp;
    for (int j = 0; j < 2; j++) {
        int cj = x & ((1 << 4) - 1);
        if (cj <= 9) {
            temp.push_back((char)('0' + cj));
        }
        else {
            cj -= 10;
            temp.push_back((char)('a' + cj));
        }
        x >>= 4;
    }
    reverse(temp.begin(), temp.end());
    string result = string(temp.begin(), temp.end());
    return result;
}

string hexaXOR(string& s1, string& s2) {
    string result;
    vector<char> hex1,hex2;
    int turn = 1;
    for (int i = 0; i < s1.size(); i++) {
        hex1.push_back(s1[i]);
        hex2.push_back(s2[i]);
        if (turn == 0) {
            int x1 = stoi(string(hex1.begin(), hex1.end()),nullptr,16);
            int x2 = stoi(string(hex2.begin(), hex2.end()),nullptr,16);
            int xx = x1 ^ x2;
            result.append(byteToHex(xx));
            hex1.clear();
            hex2.clear();
        }
        turn ^= 1;
    }
    return result;
}

inline char byteTob64(int num) {
    if (num < 0 || num > 63)return '?';

    if (num <= 25) {
        return (char)(num + 65);
    }
    if (num <= 51) {
        return (char)(num - 26 + 97);
    }
    if (num <= 61) {
        return (char)(num - 52 + 48);
    }
    if (num == 62) {
        return '+';
    }
    return '/';
}

string hexaTob64(string& hexa) {
    vector<int> bytes;
    vector<char> curr;
    for (int i = 0;; i++) {
        if (curr.size() == 2) {
            bytes.push_back(stoi(string(curr.begin(), curr.end()), nullptr, 16));
            curr.clear();
        }
        if (i == hexa.size()) {
            break;
        }
        curr.push_back(hexa[i]);
    }
    vector<char> b64;

    int idx = 0;
    int num = 0;
    for (int i = bytes.size() - 1;; i--) {
        if (idx == 3) {
            for (int j = 0; j < 4; j++) {
                int temp = num & ((1 << 6)-1);
                b64.push_back(byteTob64(temp));
                num >>= 6;
            }
            idx = 0;
            num = 0;
        }
        if (i == -1) {
            break;
        }
        num += bytes[i] << 8*idx;
        idx++;
    }
    reverse(b64.begin(), b64.end());
    return string(b64.begin(), b64.end());
}

string hexaToASCII(string hexa) {
    vector<char> res;
    vector<char> curr;
    for (int i = 0; i <= hexa.size(); i++) {
        if (curr.size() == 2) {
            res.push_back((char)stoi(string(curr.begin(), curr.end()), nullptr, 16));
            curr.clear();
        }
        if (i == hexa.size()) {
            break;
        }
        curr.push_back(hexa[i]);
    }
    return string(res.begin(), res.end());
}

inline int penaltySystem(string& text) { //Penalties to weird ascii characters.
    int n = text.size();
    int penalties = 0;
    for (int i = 0; i < n; i++) {
        if (text[i] >= 'A' && text[i] <= 'Z')continue;
        if (text[i] >= 'a' && text[i] <= 'z')continue;
        if (text[i] == ' ') continue;
        penalties++;
        //if (text[i] >= 127 || text[i] < ' ') {
        //    penalties += 1e4;
        //}
    }
    return penalties;
}

int b64ToBytes(string encoded) {
    while (encoded.size() % 4 != 0) {
        encoded.push_back('=');
    }
    int val = 0;
    int p = 0;
    for (int j = 3; j >= 0; j--) {
        if (encoded[j] >= 'A' && encoded[j] <= 'Z') {
            val += (encoded[j] - 'A') << 6 * p;
        }
        if (encoded[j] >= 'a' && encoded[j] <= 'z') {
            val += (encoded[j] - 'a' + 26) << 6 * p;
        }
        if (encoded[j] >= '0' && encoded[j] <= '9') {
            val += (encoded[j] - '0' + 52) << 6 * p;
        }
        if (encoded[j] == '+') {
            val += 62 << 6 * p;
        }
        if (encoded[j] == '/') {
            val += 63 << 6 * p;
        }
        p++;
    }
    return val;
}

string ASCIIToHexa(string text) {
    string result;
    for (int i = 0; i < text.size(); i++) {
        string temp = byteToHex((int)text[i]);
        result.push_back(temp[0]); result.push_back(temp[1]);
    }
    return result;
}

//Hamming dist of hexas
double hammingDist(string& s1, string& s2) { //god I forgot about the existence off substr :(
    if (s1.size() % 2 == 1) { //Asserting sizes are of even length.
        s1.push_back((char)0);
    }
    if (s2.size() % 2 == 1) {
        s2.push_back((char)0);
    }
    int delta = 0;
    for (int i = 0; i < s1.size(); i+=2) {
        int x1 = stoi(s1.substr(i,2), nullptr, 16);
        int x2 = stoi(s2.substr(i,2), nullptr, 16);

        for (int p = 0; p < 8; p++) { //checking whether the p'th bit is the same for x1 and x2.
            if (((1 << p) & x1) ^ ((1 << p) & x2)) {
                delta++;
            }
        }
    }
    return delta;
}

pair<string,int> challenge3(string& encoded) {
    pair<int, string> win = { INF,"" };
    int key = 0;

    for (int i = 0; i < 256; i++) {

        vector<char> chrs;
        for (int j = 0; j < encoded.size()/2; j++) {
            string temp = byteToHex(i);
            chrs.push_back(temp.front());
            chrs.push_back(temp.back());
        }
        
        string repeat(chrs.begin(), chrs.end());
        chrs.clear();

        string xored = hexaXOR(encoded, repeat);

        string cand = hexaToASCII(xored);

        pair<int, string> opt = {penaltySystem(cand),cand};
        if (opt.first < win.first) { 
            win = opt;
            key = i;
        }
        //cout << i << '\n';
    }
    return { win.second,key };
    //Cooking MC's like a pound of bacon
}

string challenge4() {
    ifstream input_file("input.txt");
    string cand;
    pair<int, string> win = { INF,"" };
    int i = 0;
    while (getline(input_file, cand, '\n')) {
        string opt = challenge3(cand).first;
        pair<int,string> rival = { penaltySystem(opt),opt};

        if (win.first > rival.first) {
            win = rival;
        }
        cout << i++ << '\n';
    }
    return win.second;

    //Now that the party is jumping
}

string challenge5(string& text, string& key) {
    vector<char> chrs;
    for (int i = 0; i < text.size(); i++) {
        string hexa = byteToHex((int)text[i]);
        chrs.push_back(hexa[0]);
        chrs.push_back(hexa[1]);
    }
    string hText(chrs.begin(), chrs.end()); //Encoding text as hex

    chrs.clear();
    for (int i = 0; i < key.size(); i++) { //Encoding key as hex
        string hexa = byteToHex((int)key[i]);
        chrs.push_back(hexa[0]);
        chrs.push_back(hexa[1]);
    }


    int turn = 0;
    int idx = chrs.size();
    while (idx < hText.size()) { //Multiplying the key (text.size()/key.size()) times, a pretty neat implementation :)
        chrs.push_back(chrs[turn++]);
        idx++;
    }
    string hKey(chrs.begin(), chrs.end());
    chrs.clear();
    
    return hexaXOR(hKey, hText);
}

string challenge6() {
    ifstream input_file("challenge6.txt");
    string sob;
    string b64Encoded;
    string answer = "";
    while (getline(input_file, sob, '\n')) { //Getting input, working
        b64Encoded.append(sob);
    }

    string encoded = "";
    for (int i = 0; i < b64Encoded.size(); i += 4) { //Decoding b64 to hex, working
        string temp = b64Encoded.substr(i, 4);
        int val = b64ToBytes(temp);
        for (int p = 3; p >= 1; p--) {
            int goodbits = (1 << 8 * p) - 1;
            goodbits ^= (1 << (8 * (p - 1))) - 1;
            encoded.append(byteToHex((val & goodbits) >> 8 * (p - 1)));
        }
    }

    pair<double, int> mini = { INF,1 };
    for (int keysize = 30; keysize <= 30; keysize++) {
        //string byte[6] = { encoded.substr(0,2 * keysize),encoded.substr(2 * keysize,2 * keysize),encoded.substr(4 * keysize,2 * keysize)
        //    ,encoded.substr(6 * keysize,2 * keysize),encoded.substr(8 * keysize,2 * keysize),encoded.substr(10 * keysize,2 * keysize) };
        string byte[2] = { encoded.substr(0,2 * keysize),encoded.substr(2 * keysize,2 * keysize) };
        //Normalized hamming score
        double res1 = hammingDist(byte[0], byte[1]) / keysize;
        //double res2 = hammingDist(byte[2], byte[3]) / keysize;
        //double res3 = hammingDist(byte[4], byte[5]) / keysize;
        double hammingscore = res1;//(res1 + res2 + res3) / 3;

        if (mini.first > hammingscore) {
            mini = { hammingscore,keysize };
        }
    }
    //Now we've find the keysize. Maybe working ?
    int keysize = mini.second;
    string key = "";
    vector<string> blocks(keysize);

    for (int i = 0; i < encoded.size(); i++) {
        blocks[i % keysize].append(encoded.substr(i, 2));
    }
    //Now we've organized blocks
    for (auto block : blocks) {
        key.append(byteToHex(challenge3(block).second));
        cout << "ay" << '\n';
    }
    //Calculated the key
    int idx = 0;
    while (key.size() < encoded.size()) { //Working
        key.push_back(key[idx++]);
    }
    answer.append(hexaToASCII(hexaXOR(encoded, key)));
    return answer;
    
}

int main()
{
    ios::sync_with_stdio(0); cout.tie(0);
    //init();
    cout << challenge6() << '\n';
    //string oof1 = ASCIIToHexa("this is a test");
    //string oof2 = ASCIIToHexa("wokka wokka!!!");
    //cout << hammingDist(oof1, oof2);
}

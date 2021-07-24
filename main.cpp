#include<bits/stdc++.h>

using namespace std;
class KeyGenerator {
    array<int, 4> armstrongNumbers = { 153, 370, 371, 407 };
    array<int, 24> baseTable = { 1234, 1243, 1324, 1342, 1423, 1432, 2134, 2143, 2314, 2341, 2413, 2431, 3124,
            3142, 3214, 3241, 3412, 3421, 4123, 4132, 4213, 4231, 4312, 4321 };

    string key, numericKey;

public:
    KeyGenerator(string k) {
        
        key = k;
        numericKey = "";
    }
    string getNumericKey() {
        if (numericKey == "")
            generateNumericKey();

        return numericKey;
    }

    void generateNumericKey() {// key: How old is my computer?
        int tot = 0;
        int l, i;
        l = key.length();
        for (i = 0; i < l; i++) {// add the ASCII of characters of key

            tot += key[i];
        }
        // example : tot = 2141

        int permutation = baseTable[tot % (baseTable.size())];
        // example : permutation = baseTable[5] = 1432

        string temp = "";
        cout << "*" << permutation << '\n';
        while (permutation > 0) {
            temp = to_string(armstrongNumbers[permutation % 10 - 1]) + temp;
            permutation /= 10;
        }
        // numericKey = partA+partB
        numericKey = temp + to_string(tot);
    }

    void display() {
        cout << "key : " << key << "\n";
        cout << "numeric key : " << numericKey << "\n";
    }
};

class ArmstrongManager {
    string numericKey;
    int enc_index, dec_index;
public:
    ArmstrongManager(string nK) {
        numericKey = nK;
        enc_index = -1;
        dec_index = -1;
    }

    int encrypt(int data) {
        // cout<<data<<"$$4"<<"\n";
        try {
            return data ^ numericKey[++enc_index];
        }
        catch (int ex) {
            enc_index = -1;
            return data ^ numericKey[++enc_index];
        }
    }


    int decrypt(int data)
    {
        try
        {
            return data ^ numericKey[++dec_index];
        }
        catch (int ex)
        {
            dec_index = -1;
            return data ^ numericKey[++dec_index];
        }
    }
};

class ByteManager {
public:
    static void byteToNibble(int x, int arr[]) {
        // ‭104‬ ---> ‭[0110,1000‬]
        arr[0] = (x & 255) >> 4;// higher nibble
        arr[1] = x & 15;// lower nibble
    }

    static int nibblesToByte(int nibbles[]) {
        // ‭[0110,1000‬] ---> 104
        return ((nibbles[0] & 15) << 4) | (nibbles[1] & 15);
    }
};



class ColorManager {
    int* rgb;
    int enc_index, dec_index;
public:
    ColorManager(string nK) {
        rgb = new int[3];
        enc_index = dec_index = -1;

        int partB = stoi(nK.substr(12));               // int(nK[12:])
        rgb[0] = (stoi(nK.substr(0, 4)) + partB) % 256; // int(nK[0:4])
        rgb[1] = (stoi(nK.substr(4, 8)) + partB) % 256; // int(nK[4:8])
        rgb[2] = (stoi(nK.substr(8, 12)) + partB) % 256; // int(nK[8:12])
    }

    int encrypt(int data) {
        int nibbles[2];
        ByteManager b;
        b.byteToNibble(data, nibbles);
        enc_index = (enc_index + 1) % 3; //3 is size of rgb array
        return (rgb[enc_index] + nibbles[0] * 16 + nibbles[1]) % 256;
    }

    int decrypt(int data) {
        int temp;
        int nibbles[2];
        dec_index = (dec_index + 1) % 3;
        temp = (data - rgb[dec_index] + 256) % 256;

        nibbles[0] = temp / 16;
        nibbles[1] = temp % 16;
        ByteManager b;
        return b.nibblesToByte(nibbles);

    }
};

int main() {
    string data = "This is an very important statement.";
    string key = "How old is my computer?";
    KeyGenerator kGen(key);
    string nK = kGen.getNumericKey();
    //DEBUG
    kGen.display();
    ArmstrongManager aMgr(nK);
    ColorManager cMgr(nK);

    string encData = "";
    int temp;
    
    for (int i = 0; i < data.length(); i++) {
        temp = aMgr.encrypt(data[i]);
        temp = cMgr.encrypt(temp);
        encData = encData + (char)temp;
    }

    string decData = "";
    for (int i = 0; i < encData.length(); i++) {
        temp = cMgr.decrypt(encData[i]);
        temp = aMgr.decrypt(temp);

        decData = decData + (char)temp;
    }

    cout << "data: " << data << "  " << data.length() << "\n";
    cout << "enc data: " << encData << "  " << encData.length() << "\n";
    cout << "dec data: " << decData << "  " << decData.length() << "\n";


}
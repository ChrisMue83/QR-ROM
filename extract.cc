#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

int min, max;
vector<vector< int> > infor;


//This program brings back the scrambled result from zbarimg in the correct order and rearranges it



void checkboundaries(string *strr) { //function writes into infor, the index of the QR-Code, the beginning of the string "QR-Code", the position of return (= \n)
    vector<int> help;

    help.resize(3);

    int startpoint = 0, endpoint = 0, start = 0;
    string anumber;

    do {
        anumber = "";
        startpoint = strr->find("QR-Code:", start);

        if (startpoint == string::npos) break;

        for (int i = 0; i < 20; i++) {
            endpoint = i;

            if (strr->at(startpoint + 8 + i) == '\n') break;

            anumber += strr->at(startpoint + 8 + i);
        }

        help[0] = stoi(anumber);
        help[1] = startpoint;
        help[2] = startpoint + 9 + endpoint;

        infor.push_back(help);


        start = startpoint + 8 + endpoint;
    } while (startpoint != string::npos);

    if (infor.size() == 0) exit(0); //if no QR-Code was found the program exits
}

void rearrange(string *strr) { // rearranges the results of the QR-Codes in the correct order and concats them
    int mini = 1E9, savi; //any big number
    vector <int> order;


    for (int i = 0; i < infor.size(); i++) { //finds the minimum of the QR-Code index
        if (infor[i][0] < mini) {
            mini = infor[i][0];
            savi = i;
        }
    }

    order.push_back(savi);

    for (int j = 0; j < infor.size(); j++) { //calculates the correct order of indices and saves them in the variable "order"
        for (int i = 0; i < infor.size(); i++) {
            if (infor[i][0] == mini + 1) {
                mini = mini + 1;
                order.push_back(i);
            }
        }

        if (order.size() == infor.size()) break;
    }

    vector<int> help; // needed to read the last existing QR-Code
    help.resize(3);
    help[1] = strr->size();
    infor.push_back(help);

    int paddingcounter = 0; //the sign "==" represents the end of a base64 file

    for (int j = 0; j < order.size(); j++) { //concats all QR encoded information and checks that only base64 characterset is used
        for (int i = 0; i < infor[order[j] + 1][1] - infor[order[j]][2] - 1; i++) {
            if ((unsigned int)strr->at(infor[order[j]][2] + i) >= 47 && (unsigned int)strr->at(infor[order[j]][2] + i) <= 57 ||
                (unsigned int)strr->at(infor[order[j]][2] + i) >= 65 && (unsigned int)strr->at(infor[order[j]][2] + i) <= 90 ||
                (unsigned int)strr->at(infor[order[j]][2] + i) >= 97 && (unsigned int)strr->at(infor[order[j]][2] + i) <= 122 ||
                (unsigned int)strr->at(infor[order[j]][2] + i) == 43 ||
                (unsigned int)strr->at(infor[order[j]][2] + i) == 61) {
                if ((unsigned int)strr->at(infor[order[j]][2] + i) == 61) {
                    paddingcounter++;
                }

                cout << strr->at(infor[order[j]][2] + i);

                if (paddingcounter == 2) break;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    std::ifstream is(argv[1], std::ifstream::binary);

    if (argc != 2) exit(0); //exact two input variables are needed

    if (is) {
        is.seekg(0, is.end);
        int length = is.tellg();
        is.seekg(0, is.beg);

        char *buffer = new char [length];


        is.read(buffer, length);

        if (is) {
            string str(buffer);

            checkboundaries(&str);

            rearrange(&str);
        } else std::cout << "error: only " << is.gcount() << " could be read";

        is.close();


        delete[] buffer;
    }

    return 0;
}

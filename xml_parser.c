#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Dictionary {
    string word;
    int count;
};

int main() {
    ifstream in("1984.txt", ios::binary);
    ofstream out("result.xml", ios::binary);

    if (!in.is_open()) {
        cout << "Cannot open file" << endl;
        return 1;
    }

    int size = 100;
    int word_cnt = 0;
    Dictionary* words = new Dictionary[size];
    int total = 0;
    string line;

    while (getline(in, line)) {
        string word;
        for (size_t i = 0; i < line.length(); i++) {
            unsigned char c = line[i];

            if (c == ' ' || c == '\t' || c == '\n' || c == '\r' ||
                c == '.' || c == ',' || c == '!' || c == '?' ||
                c == ':' || c == ';' || c == '(' || c == ')' ||
                c == '\"' || c == '\'' || c == '-' || c == '–' || c == '—') {

                if (!word.empty()) {
                    total++;
                    bool found = false;

                    for (int i = 0; i < word_cnt; i++) {
                        if (words[i].word == word) {
                            words[i].count++;
                            found = true;
                            break;
                        }
                    }

                    if (!found) {

                        if (word_cnt >= size) {
                            size *= 2;
                            Dictionary* temp_words = new Dictionary[size];

                            for (int i = 0; i < word_cnt; i++) {
                                temp_words[i] = words[i];
                            }

                            delete[] words;
                            words = temp_words;
                        }

                        words[word_cnt].word = word;
                        words[word_cnt].count = 1;
                        word_cnt++;
                    }
                    word.clear();
                }
            }
            else {
                word += c;
            }
        }

        //last word processing
        if (!word.empty()) {
            total++;
            bool found = false;

            for (int i = 0; i < word_cnt; i++) {
                if (words[i].word == word) {
                    words[i].count++;
                    found = true;
                    break;
                }
            }

            if (!found) {
                if (word_cnt >= size) {
                    size *= 2;
                    Dictionary* temp_words = new Dictionary[size];

                    for (int i = 0; i < word_cnt; i++) {
                        temp_words[i] = words[i];
                    }

                    delete[] words;
                    words = temp_words;
                }

                words[word_cnt].word = word;
                words[word_cnt].count = 1;
                word_cnt++;
            }
        }
    }

    for (int i = 0; i < word_cnt; i++) {
        for (int j = i + 1; j < word_cnt; j++) {
            if (words[i].word > words[j].word) {
                Dictionary temp = words[i];
                words[i] = words[j];
                words[j] = temp;
            }
        }
    }

    // write xml
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    out << "<text_statistics>\n";
    out << "<total_words>" << total << "</total_words>\n";
    out << "<unique_words>" << word_cnt << "</unique_words>\n";

    for (int i = 0; i < word_cnt; i++) {
        out << "<word>\n";
        out << "  <name>" << words[i].word << "</name>\n";
        out << "  <count>" << words[i].count << "</count>\n";
        out << "</word>\n";
    }

    out << "</text_statistics>";

    delete[] words;

    cout << "Analysis complete!" << endl;
    cout << "Total words: " << total << endl;
    cout << "Unique words: " << word_cnt << endl;

    return 0;
}

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm> 
using namespace std;

string program = "";
vector<vector<string>> program_new;
string filename = "";
vector<string> keyword = {"if", "else", "while", "do", "main", "int", "float", "double", "return", "const", 
                          "void", "continue", "break", "char", "unsigned", "enum", "long", "switch", "case", "auto", "static"};
int program_char = 0, line = 1, line_word = 1;
bool is_string = false;
string token = "";
int symbol = 0;

void Read() {
    cout << "�������ļ���: ";
    cin >> filename;
    ifstream f1(filename + ".txt");
    int read_current_state = 0; // ���浱ǰ״̬
    char read_char;
    
    while (f1.get(read_char)) {
        if (read_current_state == 0) {
            if (read_char == '/') {
                read_current_state = 1;
                continue;
            } else {
                program += read_char;
            }
        }
        if (read_current_state == 1) {
            if (read_char == '*') {
                read_current_state = 2;
                continue;
            } else if (read_char == '/') {
                read_current_state = 4;
                continue;
            } else {
                read_current_state = 0;
            }
        }
        if (read_current_state == 2) {
            if (read_char == '*') {
                read_current_state = 3;
                continue;
            }
        }
        if (read_current_state == 3) {
            if (read_char == '/') {
                read_current_state = 0;
            } else if (read_char != '*') {
                read_current_state = 2;
            }
        }
        if (read_current_state == 4 && read_char == '\n') {
            read_current_state = 0;
            program += read_char;
        }
    }
    f1.close();
    
    // ����������ǰ�Ŀ���
    while (program[0] == '\n') {
        program = program.substr(1);
    }

    // �����������еĿ������ǰ�ո�
    size_t index_of_blank = 1;
    while (index_of_blank < program.size()) {
        if (program[index_of_blank - 1] == '\n' && (program[index_of_blank] == ' ' || program[index_of_blank] == '\n')) {
            program = program.substr(0, index_of_blank) + program.substr(index_of_blank + 1);
        } else {
            index_of_blank++;
        }
    }

    // ����޸ĺ��������
    int row = 1;
    for (char c : program) {
        cout << c;
        if (c == '\n') {
            row++;
            cout << row << '\t';
        }
    }
    cout << endl;
}

void GetToken() {
    if (is_string == false && program[program_char - 1] == '\"') {
        is_string = true;
        symbol = 48;
        while (program[program_char] != '\"') {
            token += program[program_char++];
        }
        return;
    }

    if (is_string == true && program[program_char - 1] == '\"') {
        is_string = false;
    }

    while (program[program_char] == ' ' || program[program_char] == '\t') {
        program_char++;
        line_word++;
    }

    if (program[program_char] == '\n') {
        line++;
        line_word = 1;
        program_char++;
    }

    // ʶ��ؼ������ʶ��
    if (isalpha(program[program_char]) || program[program_char] == '_') {
        symbol = 42;  // id
        while (isalnum(program[program_char]) || program[program_char] == '_') {
            token += program[program_char++];
        }
        if (find(keyword.begin(), keyword.end(), token) != keyword.end()) {
            symbol = find(keyword.begin(), keyword.end(), token) - keyword.begin() + 1;
            return;
        }
        return;
    }

    // ʶ����ֵ����
    if (program[program_char] == '0') {
        token += program[program_char++];
        if (program[program_char] == 'b' || program[program_char] == 'B') {
            while (program[program_char] == '0' || program[program_char] == '1') {
                token += program[program_char++];
            }
            symbol = 45; // ������
        } else if (program[program_char] == 'o' || program[program_char] == 'O') {
            while (program[program_char] >= '0' && program[program_char] <= '7') {
                token += program[program_char++];
            }
            symbol = 46; // �˽���
        } else if (program[program_char] == 'x' || program[program_char] == 'X') {
            while (isxdigit(program[program_char])) {
                token += program[program_char++];
            }
            symbol = 47; // ʮ������
        } else if (program[program_char] == '.') {
            while (isdigit(program[program_char])) {
                token += program[program_char++];
            }
            symbol = 44; // ������
        } else {
            symbol = 43; // ����
        }
    } else if (isdigit(program[program_char])) {
        while (isdigit(program[program_char])) {
            token += program[program_char++];
        }
        if (program[program_char] == '.') {
            symbol = 44; // ������
            while (isdigit(program[program_char])) {
                token += program[program_char++];
            }
        } else {
            symbol = 43; // ����
        }
    } else {
        switch (program[program_char]) {
            case '+': symbol = 22; token = "+"; break;
            case '-': symbol = 23; token = "-"; break;
            case '*': symbol = 24; token = "*"; break;
            case '/': symbol = 25; token = "/"; break;
            case '=': symbol = 26; token = "="; program_char++; 
                      if (program[program_char] == '=') { token = "=="; symbol = 35; } break;
            case '<': symbol = 27; token = "<"; program_char++; 
                      if (program[program_char] == '=') { token = "<="; symbol = 41; } break;
            case '>': symbol = 39; token = ">"; program_char++; 
                      if (program[program_char] == '=') { token = ">="; symbol = 40; } break;
            case '{': symbol = 28; token = "{"; break;
            case '}': symbol = 29; token = "}"; break;
            case ';': symbol = 30; token = ";"; break;
            case '(': symbol = 31; token = "("; break;
            case ')': symbol = 32; token = ")"; break;
            case '\'': symbol = 33; token = "'"; break;
            case '\"': symbol = 34; token = "\""; break;
            case '!': program_char++; if (program[program_char] == '=') { token = "!="; symbol = 36; } break;
            case '&': program_char++; if (program[program_char] == '&') { token = "&&"; symbol = 37; } break;
            case '|': program_char++; if (program[program_char] == '|') { token = "||"; symbol = 38; } break;
            default: symbol = -2; break;
        }
        program_char++;
    }
}

int main() {
    cout << "-----------Step 1 ����ע��--------------" << endl;
    Read();

    cout << "-----------Step 2 �ʷ�����--------------" << endl;

    while (program_char < program.size()) {
        token = "";
        symbol = 0;
        GetToken();
        cout << "(" << symbol << "," << token << ")  line:" << line << ",row:" << line_word << endl;
        program_new.push_back({to_string(symbol), token, to_string(line), to_string(line_word)});
        line_word += token.length();
    }

    // �����ļ�
    ofstream f2(filename + "-out.txt");
    for (const auto& item : program_new) {
        for (const auto& field : item) {
            f2 << field << ',';
        }
        f2 << endl;
    }
    f2.close();

    cout << "�ʷ���������������" << filename + "_result.txt"<<"�ļ�" << endl;
    return 0;
}


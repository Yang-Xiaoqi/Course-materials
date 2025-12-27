#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <string>
#include <sstream>

using namespace std;
void program();
void block();
void stmts();
void stmt();
void boolean();
void expr();
void expr1();
void term();
void term1();
void factor();

string filename = "";
vector<vector<string>> vt_table;
int read_vt_index = 0;
int error_flag = 0;
int step = 1;
stack<string> stk;
string method = "";

void info() {
    if (error_flag) return;
    cout << "-----Step: " << step << "-----" << endl;
    cout << "识别串：=> ";
    stack<string> temp = stk;
    while (!temp.empty()) {
        cout << temp.top() << " ";
        temp.pop();
    }
    cout << endl;
    cout << "动作：" << method << endl << endl;
    step++;
}

void Read() {
    cout << "请输入文件名: ";
    cin >> filename;
    ifstream file(filename + ".txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string item;
        vector<string> vt;
        while (getline(ss, item, ',')) {
            vt.push_back(item);
        }
        vt_table.push_back(vt);
    }
}

void match(const string& vt) {
    if (error_flag) return;
    if (vt != vt_table[read_vt_index][1]) {
        cerr << "出错！" << endl;
        error_flag = 1;
        cout << "当前出现的" << vt_table[read_vt_index][1] << "与需要的" << vt << "不匹配" << endl;
        return;
    }
    read_vt_index++;
}

void program() {
    if (error_flag) return;
    method = "program\t-->\tblock";
    info();
    string top = stk.top();
    stk.pop();
    stk.push("block");
    block();
}

void block() {
    if (error_flag) return;
    method = "block\t-->\t{stmts}";
    info();
    string top = stk.top();
    stk.pop();
    stk.push("{");
    stk.push("stmts");
    stk.push("}");
    match("{");
    stmts();
    match("}");
}

void stmts() {
    if (error_flag) return;
    if (vt_table[read_vt_index][1] == "}") {
        method = "stmts\t-->\tnull";
        info();
        stk.pop();
        return;
    }
    method = "stmts\t-->\tstmt stmts";
    info();
    string top = stk.top();
    stk.pop();
    stk.push("stmt");
    stk.push("stmts");
    stmt();
    stmts();
}

void stmt() {
    if (error_flag) return;
    if (vt_table[read_vt_index][0] == "42") { // id
        method = "stmt\t-->\tid = expr;";
        info();
        stk.push("id");
        stk.push("=");
        stk.push("expr");
        stk.push(";");
        read_vt_index++;
        match("=");
        expr();
        match(";");
    } else if (vt_table[read_vt_index][1] == "if") {
        read_vt_index++;
        match("(");
        boolean();
        match(")");
        stmt();
        if (vt_table[read_vt_index][1] == "else") {
            method = "stmt\t-->\tif (boolean) stmt else stmt";
            info();
            stk.push("if");
            stk.push("(");
            stk.push("boolean");
            stk.push(")");
            stk.push("stmt");
            stk.push("else");
            stk.push("stmt");
            match("else");
            stmt();
            return;
        }
        method = "stmt\t-->\tif (boolean) stmt";
        info();
        stk.push("if");
        stk.push("(");
        stk.push("boolean");
        stk.push(")");
        stk.push("stmt");
    } else if (vt_table[read_vt_index][1] == "while") {
        method = "stmt\t-->\twhile (boolean) stmt";
        info();
        stk.push("while");
        stk.push("(");
        stk.push("boolean");
        stk.push(")");
        stk.push("stmt");
        read_vt_index++;
        match("(");
        boolean();
        match(")");
        stmt();
    } else if (vt_table[read_vt_index][1] == "do") {
        method = "stmt\t-->\tdo stmt while (boolean)";
        info();
        stk.push("do");
        stk.push("stmt");
        stk.push("while");
        stk.push("(");
        stk.push("boolean");
        stk.push(")");
        read_vt_index++;
        stmt();
        match("while");
        match("(");
        boolean();
        match(")");
    } else if (vt_table[read_vt_index][1] == "break") {
        method = "stmt\t-->\tbreak";
        info();
        stk.push("break");
        read_vt_index++;
    } else {
        method = "stmt\t-->\tblock";
        info();
        stk.push("block");
        block();
    }
}

void boolean() {
    if (error_flag) return;
    if (vt_table[read_vt_index + 1][1] == "<") {
        method = "boolean\t-->\texpr < expr";
        info();
        stk.push("expr");
        stk.push("<");
        stk.push("expr");
        expr();
        read_vt_index++;
    } else if (vt_table[read_vt_index + 1][1] == "<=") {
        method = "boolean\t-->\texpr <= expr";
        info();
        stk.push("expr");
        stk.push("<=");
        stk.push("expr");
        expr();
        read_vt_index++;
    } else if (vt_table[read_vt_index + 1][1] == ">") {
        method = "boolean\t-->\texpr > expr";
        info();
        stk.push("expr");
        stk.push(">");
        stk.push("expr");
        expr();
        read_vt_index++;
    } else if (vt_table[read_vt_index + 1][1] == ">=") {
        method = "boolean\t-->\texpr >= expr";
        info();
        stk.push("expr");
        stk.push(">=");
        stk.push("expr");
        expr();
        read_vt_index++;
    } else {
        method = "boolean\t-->\texpr";
        info();
        stk.push("expr");
    }
    expr();
}

void expr() {
    if (error_flag) return;
    method = "expr\t-->\tterm expr1";
    info();
    stk.push("term");
    stk.push("expr1");
    term();
    expr1();
}

void expr1() {
    if (error_flag) return;
    if (vt_table[read_vt_index][1] == "+") {
        method = "expr1\t-->\t + term expr1";
        info();
        stk.push("+");
        stk.push("term");
        stk.push("expr1");
        read_vt_index++;
        term();
        expr1();
    } else if (vt_table[read_vt_index][1] == "-") {
        method = "expr1\t-->\t - term expr1";
        info();
        stk.push("-");
        stk.push("term");
        stk.push("expr1");
        read_vt_index++;
        term();
        expr1();
    } else {
        method = "expr1\t-->\tnull";
        info();
        stk.pop();
    }
}

void term() {
    if (error_flag) return;
    method = "term\t-->\tfactor term1";
    info();
    stk.push("factor");
    stk.push("term1");
    factor();
    term1();
}

void term1() {
    if (error_flag) return;
    if (vt_table[read_vt_index][1] == "*") {
        method = "term1\t-->\t * factor term1";
        info();
        stk.push("*");
        stk.push("factor");
        stk.push("term1");
        read_vt_index++;
        factor();
        term1();
    } else if (vt_table[read_vt_index][1] == "/") {
        method = "term1\t-->\t / factor term1";
        info();
        stk.push("/");
        stk.push("factor");
        stk.push("term1");
        read_vt_index++;
        factor();
        term1();
    } else {
        method = "term1\t-->\tnull";
        info();
        stk.pop();
    }
}

void factor() {
    if (error_flag) return;
    if (vt_table[read_vt_index][1] == "(") {
        method = "factor\t-->\t(expr)";
        info();
        stk.push("(");
        stk.push("expr");
        stk.push(")");
        read_vt_index++;
        match("(");
        expr();
        match(")");
    } else {
        method = "factor\t-->\tnumber";
        info();
        stk.push("number");
        read_vt_index++;
    }
}

int main() {
    Read();
    stk.push("program");
    program();
    return 0;
}


#include<iostream>
#include<fstream>
using namespace std;
int main()
{
    int num1[20] = { 0 };
    ofstream f1("f1.dat", ios::out);
    ofstream f2("f2.dat", ios::out);
    for (int i = 0; i < 20; i++)
    {
        cin >> num1[i];
        if (i < 10)
            f1 << num1[i] << ' ';
        else
            f2 << num1[i] << ' ';
    }
    f1.close();
    f2.close();
    ifstream infile("f1.dat");
    ofstream outfile("f2.dat", ios::out | ios::app);
    for (int i = 0; i < 10; i++)
    {
        infile >> num1[i];
        outfile << num1[i] << ' ';
    }
    infile.close();
    outfile.close();
    ifstream f3("f2.dat");
    for (int i = 0; i < 20; i++)
    {
        f3 >> num1[i];
    }
    f3.close();
    ofstream f4("f2.dat", ios::out | ios::trunc);
    for (int i = 0; i < 19; i++)
    {
        for (int j = 0; j < 19 - i; j++)
        {
            if (num1[j] > num1[j + 1])
            {
                int t = num1[j];
                num1[j] = num1[j + 1];
                num1[j + 1] = t;
            }
        }
    }
    for (int i = 0; i < 20; i++)
        f4 << num1[i] << ' ';
    f4.close();
    return 0;

}



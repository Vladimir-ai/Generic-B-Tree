// Task3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Controller.h"


using namespace std;

int main()
{
    //MyBTree<int> tree(3);
    /*tree.add(7);
    tree.add(6);
    tree.add(5);
    tree.add(4);
    tree.add(3);
    tree.add(2);
    tree.add(1);
    //*/
    //MyBTree<int> tree(7);
    //for (int i = 31; i >= 1; i--) {
    //    tree.add(2*i);
    //}

    //tree.add(21);
    //tree.add(23);

    //cout << tree;

    //tree.remove(16 * 2);

    //cout << tree;

    //tree.remove(28 * 2);
    //
    //cout << 28 << endl<< tree;
    ////tree.add(15);
    //tree.remove(24 * 2);
    //cout << 24 << endl << tree;
    //tree.remove(25 * 2);
    //cout << tree;
    //tree.remove(26 * 2);
    //cout << tree;
    //tree.remove(15 * 2);
    //cout << tree;
    //tree.remove(54);

    //int a = 0;
    //cout << tree;
  
    //cout << stoi(string("024"), NULL, 0);

    Controller controller;

    cout << controller.printHelp();

    while (1) {
        cout << endl << ">>";
        string input;
        
        getline(cin, input);

        cout << controller.processInput(input);

    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

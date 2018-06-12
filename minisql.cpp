#include <iostream>
#include "minisql.h"
#include "Interpreter.h"

using namespace std;

bool online = 1;

int main()
{
	string SQL;
	//打印软件信息
    cout << endl;
	cout << "\t\t***********************************************" << endl;
	cout << "\t\t             欢迎使用 MiniSQL !" << endl;
	cout << "\t\t               Version (1.0)  " << endl;
	cout << "\t\t               请输入SQL语句 " << endl;
	cout << "\t\t***********************************************" << endl;
	cout << endl << endl;
	while (online)
	{
		cout << "MiniSQL-->> ";
		SQL="";//清空SQL语句
		SQL=Interpreter(SQL);
		cout << SQL << endl;
	}
    cout << "\t\t              谢谢使用，再见！ " << endl;
}

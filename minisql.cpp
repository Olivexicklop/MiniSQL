#include <iostream>
#include "minisql.h"
#include "Interpreter.h"

using namespace std;

bool online = 1;

int main()
{
	string SQL;
	//��ӡ�����Ϣ
    cout << endl;
	cout << "\t\t***********************************************" << endl;
	cout << "\t\t             ��ӭʹ�� MiniSQL !" << endl;
	cout << "\t\t               Version (1.0)  " << endl;
	cout << "\t\t               ������SQL��� " << endl;
	cout << "\t\t***********************************************" << endl;
	cout << endl << endl;
	while (online)
	{
		cout << "MiniSQL-->> ";
		SQL="";//���SQL���
		SQL=Interpreter(SQL);
		cout << SQL << endl;
	}
    cout << "\t\t              ллʹ�ã��ټ��� " << endl;
}

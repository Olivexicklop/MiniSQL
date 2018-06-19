#include <iostream>
#include "minisql.h"
using namespace std;

bool online = 1;

BufferManager* minisql::bufferManager = nullptr;

RecordManager* minisql::recordManager = nullptr;

CatalogManager* minisql::catalogManager = nullptr;

int main()
{
	string SQL;
	//��ӡ�����Ϣ
    cout << endl;
	// cout << "\t\t***********************************************" << endl;
	// cout << "\t\t             ��ӭʹ�� MiniSQL !" << endl;
	// cout << "\t\t               Version (1.0)  " << endl;
	// cout << "\t\t               ������SQL��� " << endl;
	// cout << "\t\t***********************************************" << endl;
	cout << endl << endl;

	minisql::bufferManager = new BufferManager();
	minisql::recordManager = new RecordManager();
	while (online)
	{
		cout << "MiniSQL-->> ";
		SQL="";//���SQL���
		SQL=Interpreter(SQL);
		cout << SQL << endl;
	}
   // cout << "\t\t              ллʹ�ã��ټ��� " << endl;
}

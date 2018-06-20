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
    minisql::catalogManager = new CatalogManager();
	//while (online)
	{
		cout << "MiniSQL-->> ";
		SQL="";//���SQL���
		//SQL=Interpreter(SQL);
        vector<string>* colname = new vector<string>;
        string s = "name1";
       // colname->push_back(s);
        vector<short>* colType = new vector<short>;
        colType->push_back(TYPE_INT);
        vector<char>* colUnique = new vector<char>;
        colUnique->push_back(UNIQUE);
        minisql::catalogManager->createTable("qwe", "primary", colname, colType, colUnique);
		cout << SQL << endl;
	}
   // cout << "\t\t              ллʹ�ã��ټ��� " << endl;
}

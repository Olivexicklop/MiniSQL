#include "API.h"

/*
大部分代码都是把interpreter翻译好的SQL语句里的内容 传到相应的class里
可以先忽视.. 主要是各种接口可以看一下 很多我都还没想好...
 */
void API::createTable(const string &SQL)
{
	Table tbl;
	Index index;
	string info = SQL.substr(2, SQL.size() - 2);
	vector<string> attrs = split(info, ",");
	stringstream ss;
	string attrName, attrType;
	int i, PKflag = 0;
	
	tbl.table_name = attrs[0];
	for (i = 1; i < attrs.size(); i++) {
		ss.clear();
		ss << attrs[i];
		ss >> attrName >> attrType;
		if (attrType == "#") { // primary key
			PKflag = 1;
			break;//最后一个属性
		}
		else {
			if (attrType == "+") { // INT
				tbl.attrs[i - 1].attr_name = attrName;
				tbl.attrs[i - 1].attr_type = INT;
				tbl.attrs[i - 1].attr_len = 4;
				tbl.attrs[i - 1].attr_id = i - 1;
				ss >> tbl.attrs[i - 1].attr_key_type;
			}
			else if (attrType == "-") { // FLOAT
				tbl.attrs[i - 1].attr_name = attrName;
				tbl.attrs[i - 1].attr_type = FLOAT;
				tbl.attrs[i - 1].attr_len = 4;
				tbl.attrs[i - 1].attr_id = i - 1;
				ss >> tbl.attrs[i - 1].attr_key_type;
			}
			else { // CHAR
				ss >> tbl.attrs[i - 1].attr_key_type;
				tbl.attrs[i - 1].attr_name = attrName;
				tbl.attrs[i - 1].attr_type = CHAR;
				tbl.attrs[i - 1].attr_id = i - 1;
				ss.clear();
				ss << attrType;
				ss >> tbl.attrs[i - 1].attr_len;
			}
		}
	}
	tbl.attr_count = i - 1;
	if (PKflag) {//如果有primary key 生成Index
		for (i = 0; i < tbl.attr_count; i++) {
			if (tbl.attrs[i].attr_name == attrName) {
				tbl.attrs[i].attr_key_type = PRIMARY;
				//cout<<tbl.attrs[i].attr_name<<endl;
				index.attr_name = attrName;
				index.index_name = attrName + "_autoindex";
				index.table_name = tbl.table_name;
			}
		}
	}
	//调用catalogManager的create table（传入table

	minisql::recordManager->createTable(tbl.table_name.c_str());
	//调用catalogManager的create index（传入index
	//调用indexManager的create index（传入？？
}

void API::createIndex(const string &SQL)
{
	Index index;
	string info = SQL.substr(2, SQL.size() - 2);
	stringstream ss;
	ss << info;
	ss >> index.index_name >> index.table_name >> index.attr_name;
	Table table;
	table.table_name = index.table_name;
	//调用catalogManager的相关函数 获取此index所在的表的信息（传入table_name 返回table
	int id = table.searchAttrId(index.attr_name);
	if(table.attrs[id].attr_key_type == UNIQUE || table.attrs[id].attr_key_type == PRIMARY)
		//调用catalogManager的create index（传入index
		//调用indexManager的create index（传入？？
		cout << "0 row(s) affected." << endl;
	else
		cout << "Error: invalid key type." << endl;

}

void API::dropTable(string &tableName)
{
	Table table;
	table.table_name = tableName;
	//调用catalogManager的相关函数 获取此table的信息（传入table_name 返回table
	Index index;

	index.table_name = tableName;
	if (table.attr_count != 0) {
		for (int i = 0; i < table.attr_count; i++) {
			index.attr_name = table.attrs[i].attr_name;
			//调用catalogManager的相关函数 判断此index是否存在
			//存在的话,补充一下index_name
			dropIndex(index.index_name);
		}
	}
}

void API::dropIndex(string &indexName)
{
	//调用catalogManager的drop index（传入index_name
	//调用indexManager的drop index（传入？？
	cout << "0 row(s) affected." << endl;

}

void API::insertTuple(const string &SQL)
{
	string info = SQL.substr(2, SQL.size() - 2);
	vector<string> values = split(info, ",");
	string key;
	Table table;
	table.table_name = values[0];
	//调用catalogManager的相关函数 获取此table的信息（传入table_name 返回table
	Index index;
	index.table_name = table.table_name;
	for (int i = 0; i < table.attr_count; i++) {
		index.attr_name = table.attrs[i].attr_name;
		//调用catalogManager的相关函数 判断此index是否存在
		//若存在
		key = values[i+1];
		//调用indexManager的相关函数 查找此index对应的属性值是否与key重复（传入key，index.attr_name,table_name?
		//若重复
		cout << "Error: Duplicate entry on index" << endl;
		return;
		//没有重复
		//调用recordManager的insert函数（传入table，values
		//调用indexManager的insert key函数（传入？？
		cout << "1 row(s) affected." << endl;
		return;
	}
	//若没有index
	//调用recordManager的insert函数（传入table，values
	cout << "1 row(s) affected." << endl;

}

void API::selectTuple_nowhere(string &tableName)
{
	Table table;
	table.table_name = tableName;
	//调用catalogManager的相关函数 获取此table的信息（传入table_name 返回table
	
	vector<string>select_values;
	//调用recordManager的select函数（传入table_name 返回vector<string>
	if (!select_values.empty()) {
		drawResult(table, select_values);
		int select_num = select_values.size() / table.attr_count;
		cout << select_num << " row(s) returned." << endl;
	}
	else {
		cout << "0 row(s) returned." << endl;
	}
}

void API::selectTuple(string &tableName, const string &SQL)
{
	condList list;
	Condition cond;
	string info = SQL.substr(2, SQL.size() - 2) + ",";
	vector<string> conditions = split(info, ",");
	vector<string>::iterator it;
	stringstream ss;
	it = conditions.begin();
	for (++it; it != conditions.end(); ++it) {
		ss.clear();
		ss << *it;
		while (ss >> cond.attr_name) {
			ss >> cond.op_type;
			ss >> cond.cmp_value;
			list.push_back(cond);
		}
	}

	Table table;
	table.table_name = tableName;
	//调用catalogManager的相关函数 获取此table的信息（传入table_name 返回table

	// 分为有索引的条件和无索引的条件
	condList noIndexList;
	condList::iterator cit;
	Index index;
	index.table_name = tableName;
	int flag = 0;
	
	for (cit = list.begin(); cit != list.end(); ++it) {
		index.attr_name = cit -> attr_name;
		//调用catalogManager的相关函数 判断此index是否存在
		//若存在，且此条件的操作符不是<>
		if (cit->op_type != "<>" ) 
			//疑问：如果查找条件中有多个索引 要根据哪个索引去b+树搜索？
			//调用indexManager的select record函数（传入？？返回一个偏移量？
			//如果偏移量不为0
			flag = 1;
		//若不存在index
		noIndexList.push_back(*cit);

	}
	
	vector<string>select_values;

	if(flag)
		//调用recordManager的select with index函数（传入table，noIndexList,偏移量？返回vector<string>
	//else
	//如果没有索引条件
	//调用recordManager的select no index函数（传入table，list？返回vector<string>
	
	if (!select_values.empty()) {
		drawResult(table, select_values);
		int select_num = select_values.size() / table.attr_count;
		cout << select_num << " row(s) returned." << endl;
	}
	else {
		cout << "0 row(s) returned." << endl;
	}

}

void API::deleteTuple_nowhere(string &tableName)
{
	Table table;
	table.table_name = tableName;
	//调用catalogManager的相关函数 获取此table的信息（传入table_name 返回table
	
	//调用recordManager的delete函数（传入table 返回偏移量？
	//如果offset=0
	cout << "0 row(s) returned." << endl;
	//else
	int delete_num;
	cout << delete_num << " row(s) affected." << endl;

	//调用indexManager的delete key函数（传入table？
}

void API::deleteTuple(string &tableName, const string &SQL)
{
	condList list;
	Condition cond;
	string info = SQL.substr(2, SQL.size() - 2) + ",";
	vector<string> conditions = split(info, ",");
	vector<string>::iterator it;
	stringstream ss;
	it = conditions.begin();
	for (++it; it != conditions.end(); ++it) {
		ss.clear();
		ss << *it;
		while (ss >> cond.attr_name) {
			ss >> cond.op_type;
			ss >> cond.cmp_value;
			list.push_back(cond);
		}
	}

	Table table;
	table.table_name = tableName;
	//调用catalogManager的相关函数 获取此table的信息（传入table_name 返回table

	// 分为有索引的条件和无索引的条件
	condList noIndexList;
	condList::iterator cit;
	Index index;
	index.table_name = tableName;
	int flag = 0;
	
	for (cit = list.begin(); cit != list.end(); ++it) {
		index.attr_name = cit->attr_name;
		//调用catalogManager的相关函数 判断此index是否存在
		//若存在，且此条件的操作符不是<>
		if (cit->op_type != "<>" ) 
			//疑问：如果查找条件中有多个索引 要根据哪个索引去b+树搜索？
			//调用indexManager的select record函数（传入？？返回一个偏移量？
			//如果偏移量不为0
			flag = 1;
		//若不存在index
		noIndexList.push_back(*cit);
	}

	if(flag)
		//调用recordManager的delete with index函数（传入table,noIndexList,偏移量?返回偏移量？
	//else
		//调用recordManager的delete no index函数（传入table,noIndexList？返回偏移量？
	
	//如果偏移量=0
	cout << "0 row(s) returned." << endl;
	//else
	int delete_num;
	cout << delete_num << " row(s) affected." << endl;

	//疑问：这里还要删去indexManager中相应index的相应值 但是没想好怎么操作...
}

vector<string> API::split(const string &SQL, const string &pattern)
{
	vector<string> res;
	string attrValue;
	size_t currPos = 0, nextPos;
	while ((nextPos = SQL.find(pattern, currPos)) != string::npos) {
		attrValue = SQL.substr(currPos, nextPos - currPos);
		res.push_back(attrValue);
		currPos = nextPos + pattern.size();
	}
	return res;
}

void API::drawResult(const Table &table, vector<string>select_value) {
	vector<string>::iterator it;
	int cnt = 0;
	cout << endl << "\t\t";
	for (int i = 0; i < table.attr_count; i++) {
		cout << table.attrs[i].attr_name << "\t";
	}
	cout << endl << endl << "\t\t";
	for(it = select_value.begin(); it != select_value.end(); ++it){
		cout << *it << "\t";
		cnt++;
		if(cnt == table.attr_count){
			cout << endl << "\t\t";
			cnt = 0;
		}
	}
	cout << endl;
}
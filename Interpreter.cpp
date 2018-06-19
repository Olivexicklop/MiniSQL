#include <iostream> 
#include <string>
#include <algorithm>
#include <fstream>
#include "Interpreter.h"
 
using namespace std;

API myAPI;
extern bool online;

/////////////////////////////////////////////////////////////////////////////////////////////
//��ȡ�û�����
string read_input()
{
	string SQL;//�û���������
	string temp;
	char str[100];
	bool finish = false;//���δ������
	while (!finish)
	{
		cin >> str;//�Կո�Ϊ�ָ�
		temp = str;
		SQL = SQL + " " + temp;
		if (SQL[SQL.length() - 1] == ';')//��������ֺ�
		{
			SQL[SQL.length() - 1] = ' ';//�ڷֺ�ǰ��һ���ո�
			SQL += ";";
			finish = true;//��־������
		}
	}
	//�������дת��ΪСд
    transform(SQL.begin(), SQL.end(), SQL.begin(), ::tolower);
	//�����û�����
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤create����Ƿ���Ч
string create_clause(string SQL, int start)
{
	string temp;
	int end;
	//��ȡ�ڶ�������
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "syntax error: syntax error for create statement!" << endl;
		SQL = "99";
	}
	//��Ϊtable,������֤
	else if (temp == "table")
		SQL = create_table(SQL, start);
	//��Ϊindex,������֤
	else if (temp == "index")
		SQL = create_index(SQL, start);
	//��Ϊ������Ϣ����ӡ������Ϣ
	else
	{
		cout << "syntax error:" << " " << temp << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//����create�����ڲ���ʽ
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤create table����Ƿ���Ч
string create_table(string SQL, int start)
{
	string temp, sql, T;
	int index, end, length;
	//��ȡ����
	while (SQL[start] == ' ')
		start++;
	index = start;
	if ((end = SQL.find('(', start)) == -1)//����Ҳ���������
	{
		cout << "error: missing ( in the statement!" << endl;
		SQL = "99";
		return SQL;
	}
	temp = SQL.substr(start, end - start);//������ǰ����Ǳ���
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "error: error in create table statement!" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		length = temp.length() - 1;
		while (temp[length] == ' ')//ȥ��������ǰ�Ŀո�
			length--;
		temp = temp.substr(0,length + 1);
	}
	
	//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
	if (temp.find(' ') != -1)//�������пո�
	{
		cout << "error: " << temp << "---is not a valid table name!" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		sql = temp + ",";
		while (SQL[start] == ' ')
			start++;
		//��ȡÿ������
		while ((end = SQL.find(',', start)) != -1)
		{
			temp = SQL.substr(start, end - start);
			start = end + 1;
			//���п����ԣ���ӡ������Ϣ
			if (temp.empty())
			{
				cout << "error: error in create table statement!" << endl;
				SQL = "99";
				return SQL;
			}
			//��������
			else
			{
				sql = get_attribute(temp, sql);
				if (sql == "99")
					return sql;
			}
			while (SQL[start] == ' ')
				start++;
		}
		//����������
		temp = SQL.substr(start, SQL.length() - start - 1);
		length = temp.length() - 1;
		while (temp[length] != ')'&&length >= 0)
			length--;
		//���ޣ���ӡ������Ϣ  (û��������
		if (length<1)
		{
			cout << "error: error in create table statement!" << endl;
			SQL = "99";
			return SQL;
		}
		//�洢����
		else
		{
			temp = temp.substr(0,length);
			end = SQL.find(' ', start);
			T = SQL.substr(start, end - start);
			start = end + 1;
			//��Ϊ��������
			if (T == "primary")
			{
				//�ж��Ƿ��йؼ���key
				temp += ")";
				while (SQL[start] == ' ')
					start++;
				end = SQL.find('(', start);
				T = SQL.substr(start, end - start);
				start = end + 1;
				length = T.length() - 1;
				while (T[length] == ' ')
					length--;
				T = T.substr(0,length + 1);
				//��Ϊ�գ���ӡ������Ϣ
				if (T.empty())
				{
					cout << "syntax error: syntax error in create table statement!" << endl;
					cout << "\t missing key word 'key'!" << endl;
					SQL = "99";
					return SQL;
				}
				//���У�������֤
				else if (T == "key")
				{
					//��ȡ����������
					while (SQL[start] == ' ')
						start++;
					end = SQL.find(')', start);
					T = SQL.substr(start, end - start);
					length = T.length() - 1;
					while (T[length] == ' ')
						length--;
					T = T.substr(0,length + 1);
					//���ޣ���ӡ������Ϣ
					if (T.empty())
					{
						cout << "error : missing primary key attribute name!" << endl;
						SQL = "99";
						return SQL;
					}
					//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
					else if (T.find(' ') != -1)
					{
						cout << "error : " << T << "---is not a valid primary key attribute name!" << endl;
						SQL = "99";
						return SQL;
					}
					//��������
					else
					{
						sql += T + " #,";
						SQL = "01" + sql;
					}
				}
				//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
				else
				{
					cout << "error : " << T << "---is not a valid key word!" << endl;
					SQL = "99";
					return SQL;
				}
			}
			//��Ϊһ������
			else
			{
				sql = get_attribute(temp, sql);
				if (sql == "99")
				{
					SQL = "99";
					return SQL;
				}
				else
					SQL = "01" + sql;
			}
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//�������
//Ҫ��֧��int,float,char
string get_attribute(string temp, string sql)
{
	int start = 0, end, index;
	string T, C;
	temp += " ";
	//���������
	end = temp.find(' ', start);
	T = temp.substr(start, end - start);//�ֽ��һ������
	start = end + 1;
	sql += T + " ";
	//�����������
	while (temp[start] == ' ')//�ҵ���һ�����ʵĿ�ͷ
		start++;
	end = temp.find(' ', start);
	T = temp.substr(start, end - start);//��������
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (T.empty())
	{
		cout << "error : error in create table statement!" << endl;
		sql = "99";
		return sql;
	}
	//��Ϊint 
	else if (T == "int")
		sql += "+";
	//��Ϊfloat
	else if (T == "float")
		sql += "-";
	//��Ϊchar
	else
	{
		index = T.find('(');//������������ʣ�
		C = T.substr(0,index);//��ȡ��������ߵĵ���
		index++;
		//�����󣬴�ӡ������Ϣ
		if (C.empty())
		{
			cout << "error: " << T << "---is not a valid data type definition!" << endl;
			sql = "99";
			return sql;
		}
		//��Ϊchar
		else if (C == "char")
		{
			C = T.substr(index, T.length() - index - 1);//��ȡchar�������Եĳ���
			if (C.empty())
			{
				cout << "error: the length of the data type char has not been specified!" << endl;
				sql = "99";
				return sql;
			}
			else
				sql += C;
		}
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		else
		{
			cout << "error: " << C << "---is not a valid key word!" << endl;
			sql = "99";
			return sql;
		}
	}
	//�Ƿ��и�����Ϣ
	while (start<temp.length() && (temp[start] == ' '))
		start++;
	if (start<temp.length())
	{
		//��Ϊunique���壬������Ϣ
		end = temp.find(' ', start);
		T = temp.substr(start, end - start);
		if (T == "unique")
		{
			sql += " 1,";
		}
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		else
		{
			cout << "error: " << temp << "---is not a valid key word!" << endl;
			sql = "99";
			return sql;
		}
	}
	//���޸�����Ϣ
	else
		sql += " 0,";
	return sql;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤create index����Ƿ���Ч
string create_index(string SQL, int start)
{
	string temp, sql;
	int end, length;
	//��ȡ����������
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "syntax error: syntax error for create index statement!" << endl;
		SQL = "99";
	}
	else
	{
		sql = temp;//?
		//��ȡ���ĸ�����
		while (SQL[start] == ' ')
			start++;
		end = SQL.find(' ', start);
		temp = SQL.substr(start, end - start);
		start = end + 1;
		//���ޣ���ӡ������Ϣ
		if (temp.empty())
		{
			cout << "syntax error: syntax error for create index statement!" << endl;
			SQL = "99";
		}
		//��Ϊon,������֤
		else if (temp == "on"){
			//��ȡ����
			while (SQL[start] == ' ')
				start++;
			end = SQL.find('(', start);
			temp = SQL.substr(start, end - start);
			start = end + 1;
			//���ޣ���ӡ������Ϣ
			if (temp.empty())
			{
				cout << "syntax error: syntax error for create index statement!" << endl;
				cout << "\t missing ( !" << endl;
				SQL = "99";
			}
			else
			{
				//�����Ƿ�Ϊ��Ч����
				length = temp.length() - 1;
				while (temp[length] == ' ')
					length--;
				temp = temp.substr(0,length + 1);
				//��Ч
				if (temp.find(' ') == -1)
				{
					sql += " " + temp;
					//��ȡ������
					while (SQL[start] == ' ')
						start++;
					end = SQL.find(')', start);
					temp = SQL.substr(start, end - start);
					start = end + 1;
					//���ޣ���ӡ������Ϣ
					if (temp.empty())
					{
						cout << "syntax error: syntax error for create index statement!" << endl;
						cout << "\t missing ) !" << endl;
						SQL = "99";
					}
					else
					{
						//�����Ƿ�Ϊ��Ч������
						length = temp.length() - 1;
						while (temp[length] == ' ')
							length--;
						temp = temp.substr(0,length + 1);
						//��Ч
						if (temp.find(' ') == -1)
						{
							sql += " " + temp;
							while (SQL[start] == ' ')
								start++;
							if (SQL[start] != ';' || start != SQL.length() - 1)
							{
								cout << "syntax error: syntax error for ending the statement!" << endl;
								SQL = "99";
							}
							//����create index�����ڲ���ʽ
							else
								SQL = "02" + sql;
						}
						//��Ч,��ӡ������Ϣ
						else
						{
							cout << "error:" << " " << temp << "---is not a valid attribute name!" << endl;
							SQL = "99";
						}
					}
				}
				//��Ч,��ӡ������Ϣ
				else
				{
					cout << "error:" << " " << temp << "---is not a valid table name!" << endl;
					SQL = "99";
				}
			}
		}
		//��Ϊ�Ƿ���Ϣ����ӡ�Ƿ���Ϣ
		else
		{
			cout << "syntax error:" << " " << temp << "---is not a valid key word!" << endl;
			SQL = "99";
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤drop����Ƿ���Ч
string drop_clause(string SQL, int start)
{
	string temp;
	int end;
	//��ȡ�ڶ�������
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "syntax error: syntax error for drop statement!" << endl;
		SQL = "99";
	}
	//��Ϊtable,������֤
	else if (temp == "table")
		SQL = drop_table(SQL, start);
	//��Ϊindex,������֤
	else if (temp == "index")
		SQL = drop_index(SQL, start);
	//��Ϊ������Ϣ����ӡ������Ϣ
	else
	{
		cout << "syntax error:" << " " << temp << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//����drop�����ڲ���ʽ
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤drop table����Ƿ���Ч
string drop_table(string SQL, int start)
{
	string temp;
	int index, end;
	//��ȡ����������
	while (SQL[start] == ' ')
		start++;
	index = start;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "error: table name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL[start] == ' ')
			start++;
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if (SQL[start] != ';' || start != SQL.length() - 1)
		{
			cout << "error:" << SQL.substr(index, SQL.length() - index - 2) << "---is not a valid table name!" << endl;
			SQL = "99";
		}
		//����drop table�����ڲ���ʽ
		else
			SQL = "11" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤drop index����Ƿ���Ч
string drop_index(string SQL, int start)
{
	string temp;
	int index, end;
	//��ȡ����������
	while (SQL[start] == ' ')
		start++;
	index = start;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "error: table name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL[start] == ' ')
			start++;
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if (SQL[start] != ';' || start != SQL.length() - 1)
		{
			cout << "error:" << SQL.substr(index, SQL.length() - index - 2) << "---is not a valid index name!" << endl;
			SQL = "99";
		}
		//����drop index�����ڲ���ʽ
		else
			SQL = "12" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤select����Ƿ���Ч
string select_clause(string SQL, int start)
{
	string temp, temp2, sql, oldSQL;
	int end;
	sql = "";
	oldSQL = SQL;
	//��ȡ�ڶ�������
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "syntax error: syntax error for select statement!" << endl;
		SQL = "99";
	}
	//��Ϊ*��������֤
	else if (temp == "*"){
		//��ȡ����������
		while (SQL[start] == ' ')
			start++;
		end = SQL.find(' ', start);
		temp = SQL.substr(start, end - start);
		start = end + 1;
		//���ޣ���ӡ������Ϣ
		if (temp.empty())
		{
			cout << "syntax error: syntax error for select statement!" << endl;
			SQL = "99";
		}
		//��Ϊfrom��������֤
		else if (temp == "from"){
			//��ȡ���ĸ�����
			while (SQL[start] == ' ')
				start++;
			end = SQL.find(' ', start);
			temp = SQL.substr(start, end - start);//����
			start = end + 1;
			//���ޣ���ӡ������Ϣ
			if (temp.empty())
			{
				cout << "syntax error: syntax error for select statement!" << endl;
				SQL = "99";
			}
			else{
				while (SQL[start] == ' ')
					start++;
				//���ڱ���֮��������
				if (SQL[start] != ';' || start != SQL.length() - 1){
					while (SQL[start] == ' ')
						start++;
					end = SQL.find(' ', start);
					temp2 = SQL.substr(start, end - start);
					start = end + 1;
					//���ޣ���ӡ������Ϣ
					if (temp2.empty())
					{
						cout << "syntax error: syntax error for select statement!" << endl;
						SQL = "99";
					}
					//��Ϊwhere��������֤
					else if (temp2 == "where"){
						SQL = "40" + temp;
						temp = oldSQL.substr(start, oldSQL.length() - start);
						sql = select_where(temp);
						if (sql == "99"){
							cout << "syntax error: syntax error in selection of conditions!" << endl;
							SQL = "99";
						}
						else SQL += "," + sql;
					}
					else{
						cout << "syntax error: missing 'where'!" << endl;
						SQL = "99";
						return SQL;
					}
				}
				else
					SQL = "41" + temp;
			}
		}
		//��Ϊ������Ϣ����ӡ������Ϣ
		else
		{
			cout << "error: you are allowed to use from here only!" << endl;
			SQL = "99";
		}
	}
	//������*Ҳ��Ϊ��
	else
	{
		cout << "syntax error: syntax error for select statement!" << endl;
		SQL = "99";
	}
	//����select�����ڲ���ʽ
	return SQL;
}

////////////////////////////////////////////////////////////////////////////////////////////
//��֤select_where����Ƿ���Ч
string select_where(string temp){
	int start = 0, end, index;
	string T;
	string sql;
	sql = "";
	//��ȡÿһ������
	while ((end = temp.find("and ", start)) != -1){
		index = end - 1;
		if (index < 0){
			sql = "99";
			return sql;
		}
		while (((temp[start] == ' ')||(temp[start] == '\'') || (temp[start] == '\"'))&&start != temp.length() - 1){
			start++;//?
		}
		if (((temp[start] == ' ')||(temp[start] == '\'') || (temp[start] == '\"'))&&start == temp.length() - 1){//û������
			sql = "99";
			return sql;
		}
		while (((temp[index] == ' ')||(temp[index] == '\'') || (temp[index] == '\"'))&&index > 0){
			index--;
		}
		if (index<=start){//û������
			sql = "99";
			return sql;
		}
		T = temp.substr(start, index - start + 1);
		T = select_condition(T);//����������
		if (T == "99"){//��������ʽ����
			sql = "99";
			return sql;
		}
		sql += T + ",";
		start = end + 4;
	}
	//��ȡ���һ������
	end = temp.length() - 2;
	while (temp[end] == ' ') end--;
	while (((temp[start] == ' ')||(temp[start] == '\'') || (temp[start] == '\"'))) start++;
	if (end <= start){
		sql = "99";
		return sql;
	}
	T = temp.substr(start, end - start + 1);
	T = select_condition(T);
	if (T == "99"){//��������ʽ����
		sql = "99";
		return sql;
	}
	sql += T;
	return sql;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//�ֽ�ѡ�������Ƿ���Ч
string select_condition(string T) {
	int start, index, end, m1, m2;
	string temp1, temp2;
	start = 0;
	end = T.length() - 1;
	
	while ((T[end] == ' ') || (T[end] == '\'')) 
		end--;
	if (((index = T.find(">=")) != -1) || ((index = T.find("<=")) != -1) || ((index = T.find(">")) != -1) \
		|| ((index = T.find("<")) != -1) || ((index = T.find("<>")) != -1) || ((index = T.find("=")) != -1))
	{
		m1 = index - 1;
		while (m1 >= start && ((T[m1] == ' ') || (T[m1] == '\''))) 
			m1--;
		if (m1 < start) {
			T = "99";
			return T;
		}
		if(((index = T.find(">")) != -1) || ((index = T.find("<")) != -1) || ((index = T.find("=")) != -1))
			m2 = index + 1;
		else
			m2 = index + 2;
		while (m2 <= end && ((T[m2] == ' ') || (T[m2] == '\''))) 
			m2++;
		if (m2 > end) {
			T = "99";
			return T;
		}
		temp1 = T.substr(start, m1 - start + 1);
		temp2 = T.substr(m2, end - m2 + 1);
		if ((temp1.find(' ') != -1) || (temp2.find(' ') != -1)) {
			T = "99";
			return T;
		}
		else{
			if((index = T.find(">=")) != -1)
				T = temp1 + " >= " + temp2;
			else if((index = T.find("<=")) != -1)
					T = temp1 + " <= " + temp2;
			else if((index = T.find("<")) != -1)
					T = temp1 + " << " + temp2;
			else if((index = T.find(">")) != -1)
					T = temp1 + " >> " + temp2;
			else if((index = T.find("<>")) != -1)
					T = temp1 + " <> " + temp2;
			else if((index = T.find("=")) != -1)
					T = temp1 + " == " + temp2;
		}
	}
	else {
		T = "99";
	}

	return T;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤insert����Ƿ���Ч
string insert_clause(string SQL, int start){
	string temp, sql;
	int end;
	sql = "";
	//��ȡ�ڶ�������
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "syntax error: syntax error for insert statement!" << endl;
		SQL = "99";
		return SQL;
	}
	//��Ϊinto��������֤
	else if (temp == "into"){
		//��ȡ����������
		while (SQL[start] == ' ')
			start++;
		end = SQL.find(' ', start);
		temp = SQL.substr(start, end - start);
		start = end + 1;
		//���ޣ���ӡ������Ϣ
		if (temp.empty())
		{
			cout << "syntax error: syntax error for insert statement!" << endl;
			SQL = "99";
			return SQL;
		}
		else {
			sql += "60" + temp;//����
			//��ȡ���ĸ�����
			while (SQL[start] == ' ')
				start++;
			end = SQL.find('(', start);
			if (end == -1){
				cout << "error:missing ( in insert!" << endl;
				SQL = "99";
				return SQL;
			}
			end -= 1;
			while (SQL[end] == ' ') end--;
			temp = SQL.substr(start, end - start + 1);
			start = end + 1;
			//���ޣ���ӡ������Ϣ
			if (temp.empty())
			{
				cout << "syntax error: syntax error for insert statement!" << endl;
				SQL = "99";
				return SQL;
			}
			else if (temp == "values"){//��Ϊvalues�������ж�
				while (SQL[start] == ' ')
					start++;
				end = SQL.length() - 2;
				temp = SQL.substr(start, end - start + 1);
				sql = get_insert(temp, sql);
				if (sql == "99"){
					SQL = "99";
					return SQL;
				}
				else SQL = sql;
			}
			else{
				cout << "syntax error: syntax error for insert statement!" << endl;
				SQL = "99";
				return SQL;
			}
		}
	}
	else{
		cout << "syntax error: syntax error for insert statement!" << endl;
		SQL = "99";
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤insertֵ�Ƿ���Ч
string get_insert(string temp, string sql){
	int start = 0;
	int end, index;
	string T;
	start += 1;
	//��ȡÿһ��value
	while ((end = temp.find(',', start)) != -1){
		index = end - 1;
		while (((temp[start] == ' ')||(temp[start] == '\'')))
			start++;
		while (((temp[index] == ' ')||(temp[index] == '\'')))
			index--;
		T = temp.substr(start, index - start + 1);
		sql += "," + T;
		start = end + 1;
	}
	//��ȡ���һ��value
	index = temp.length() - 1;
	while (((temp[index] == ' ')))
		index--;
	end = temp.find(')', start);
	if (index != end){//���һ���ַ�����)
		cout << "error:missing ) in insert!" << endl;
		sql = "99";
		return sql;
	}
	else{
		index -= 1;
		while (((temp[index] == ' ')||(temp[index] == '\'')))
			index--;
		while (((temp[start] == ' ')||(temp[start] == '\'')))
			start++;
		T = temp.substr(start, index - start + 1);
		sql += "," + T;
	}
	return sql;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤delete����Ƿ���Ч
string delete_clause(string SQL, int start){
	string temp, temp2, sql, sql2, oldSQL;
	int end;
	sql = sql2 = "";
	oldSQL = SQL;
	//��ȡ�ڶ�������
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "syntax error: syntax error for select statement!" << endl;
		SQL = "99";
	}
	//��Ϊfrom��������֤
	else if (temp == "from"){
		//��ȡ����������
		while (SQL[start] == ' ')
			start++;
		end = SQL.find(' ', start);
		temp = SQL.substr(start, end - start);//����
		start = end + 1;
		//���ޣ���ӡ������Ϣ
		if (temp.empty())
		{
			cout << "syntax error: syntax error for delete statement!" << endl;
			SQL = "99";
		}
		else{
			while (SQL[start] == ' ')
				start++;
			//���ڱ���֮��������
			if (SQL[start] != ';' || start != SQL.length() - 1){
				while (SQL[start] == ' ')
					start++;
				end = SQL.find(' ', start);
				temp2 = SQL.substr(start, end - start);
				start = end + 1;
				//���ޣ���ӡ������Ϣ
				if (temp2.empty())
				{
					cout << "syntax error: syntax error for delete statement!" << endl;
					SQL = "99";
				}
				//��Ϊwhere��������֤
				else if (temp2 == "where"){
					SQL = "50" + temp;
					temp = oldSQL.substr(start, oldSQL.length() - start);
					sql = select_where(temp);//ֱ����select�ķ�����������
					if (sql == "99"){
						SQL = "99";
						cout << "syntax error: syntax error in selection of conditions!" << endl;
					}
					else SQL += "," + sql;				
				}
				else{
					SQL = "99";
					cout << "syntax error: missing 'where'!" << endl;
					return SQL;
				}
			}
			else
				SQL = "51" + temp;
		}
	}
	//��Ϊ������Ϣ����ӡ������Ϣ
	else
	{
		cout << "error:missing from!" << endl;
		SQL = "99";
	}
	//����delete�����ڲ���ʽ
	return SQL;
}

//��֤quit����Ƿ���Ч
string quit_clause(string SQL, int start){
	while (SQL[start] == ' ')
		start++;
	if (SQL[start] == ';' )
		SQL = "90";
	else//�����drop���滹�б�ĵ���
	{
		cout << "error: The grammar for quit is wrong!" << endl;
		SQL = "99";
	}	
	return SQL;
}

//��֤execfile����Ƿ���Ч
string execfile_clause(string SQL, int start) {
	string temp;
	int index, end;
	while (SQL[start] == ' ')//�ҵ���һ�����ǿո��λ��
		start++;
	index = start;
	end = SQL.find(' ', start);//������λ�ú����һ���ո�
	temp = SQL.substr(start, end - start);//��ȡ�õ���
	start = end + 1;//������һ�����ʵ�λ��
					//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "error: file name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL[start] == ' ')
			start++;
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if (SQL[start] != ';' || start != SQL.length() - 1)//������ļ����ֺ��滹�б�ĵ���
		{
			cout << "error:" << SQL.substr(index, SQL.length() - index - 2) << "---is not a valid file name!" << endl;
			SQL = "99";
		}
		else
			SQL = execfile_file(temp);
				
	}
	return SQL;
}

//��ȡ�ļ��е�����
string execfile_file(string temp) {
	temp += ".sql";
	const char* filename = temp.data();//stringתchar*
	int posi;
	bool flag = 0;
	string SQL, s, s2;
	s = "";
	
	ifstream infile(filename);
	if (!infile) {//���ļ���ʧ��
		cout << "Error in reading the file!" << endl;
		SQL = "99";
		return SQL;
	}
	else {//�ļ��򿪳ɹ�
		while (getline(infile, s2)) {
			if (s2.empty()) continue;
			posi = s2.length() - 1;
			while (s2[posi] == ' ') 
				posi--;
			//һ���ѯ����
			if (s2[posi] == ';') {
				s += s2;				
				s = Interpreter(s);
				if (flag == 0) {
					SQL += s;
					flag = 1;
				}
				else SQL += "\n" + s;				
				s = "";
			}
			else{
				s += s2;
			}			
		}
		infile.close();
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��ȡ�û����룬������������Ч�Լ�飬����ȷ�����������ڲ���ʾ��ʽ
string Interpreter(string statement)
{
	string SQL;
	string temp;
	int start = 0, end;
	
	if(statement.empty())	
		//��ȡ�û�����
		SQL = read_input();
	else{
		SQL=statement;
		if (SQL[SQL.length() - 1] == ';')//��������ֺ�
		{
			SQL[SQL.length() - 1] = ' ';//�ڷֺ�ǰ��һ���ո�
			SQL += ";";
		}
	}		
	
	//��ȡ����ĵ�һ������
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//�������룬��ӡ������Ϣ
	if (temp.empty())
	{
		cout << "syntax error: empty statement!" << endl;
		SQL = "99";
	}
	//��Ϊcreate���
	else if (temp == "create") {
		SQL = create_clause(SQL, start);

		if (SQL.substr(0, 2) == "01") {
			// create table
			myAPI.createTable(SQL);
		}
		else if (SQL.substr(0, 2) == "02") {
			// create index
			myAPI.createIndex(SQL);
		}
	}
	//��Ϊdrop���
	else if (temp == "drop") {
		SQL = drop_clause(SQL, start);
		if (SQL.substr(0, 2) == "11") {
			// drop table
			string tblName = SQL.substr(2, SQL.size() - 2);
			myAPI.dropTable(tblName);
		}
		else if (SQL.substr(0, 2) == "12") {
			// drop index
			string indexName = SQL.substr(2, SQL.size() - 2);
			myAPI.dropIndex(indexName);
		}
	}
	//��Ϊselect���
	else if (temp == "select") {
		SQL = select_clause(SQL, start);
		
		if (SQL.substr(0, 2) == "41") {
			// no "where"
			string tblName = SQL.substr(2, SQL.size() - 2);
			myAPI.selectTuple_nowhere(tblName);
		}
		else if (SQL.substr(0, 2) == "40") {
			// "where"
			size_t nextPos = SQL.find(",", 0);
			string tblName = SQL.substr(2, nextPos - 2);
			myAPI.selectTuple(tblName, SQL);
		}
	}
	//��Ϊinsert���
	else if (temp == "insert") {
		SQL = insert_clause(SQL, start) + ",";//ͨ�����ż��ÿ��value
	
		if (SQL.substr(0, 2) == "60") {
			myAPI.insertTuple(SQL);
		}
	}
	//��Ϊdelete���
	else if (temp == "delete") {
		SQL = delete_clause(SQL, start);
		
		if (SQL.substr(0, 2) == "51") {
			// no "where"
			string tblName = SQL.substr(2, SQL.size() - 2);
			myAPI.deleteTuple_nowhere(tblName);
		}
		else if (SQL.substr(0, 2) == "50") {
			// "where"
			size_t nextPos = SQL.find(",", 0);
			string tblName = SQL.substr(2, nextPos - 2);
			myAPI.deleteTuple(tblName, SQL);
		}
	}

	//��Ϊexecfile���
	else if (temp == "execfile") {
		SQL = execfile_clause(SQL, start);
	}
	//��Ϊquit���
	else if (temp == "quit") {
		SQL = quit_clause(SQL, start);
		if (SQL == "90") 
		{
			online = 0;
			//block_record.flush();
		}
	}
	//��ȡ����
	else if (temp == "help") {
		SQL = "80";
		cout << endl;
		cout << endl;
		// cout << "��ӭʹ�ñ�ϵͳ��" << endl;
		// cout << "��ϵͳ��ǰ֧�����¹��ܣ�" << endl;
		// cout << "-�Ա�Ĳ���:" << endl;
		// cout << "    -������        ʾ����䣺create table student (id char(10) , primary key(id));" << endl;
		// cout << "    -ɾ����        ʾ����䣺drop table student;" << endl;
		// cout << "-�Լ�¼�Ĳ���:" << endl;
		// cout << "    -�����¼      ʾ����䣺insert into student values('3150000000');" << endl;
		// cout << "    -ɾ����¼      ʾ����䣺delete from student;" << endl;
		// cout << "			     delete from student where id = '3150000000';" << endl;
		// cout << "    -ѡ���¼      ʾ����䣺select * from student;" << endl;
		// cout << "			     select * from student where id='3150000000';" << endl;
		// cout << "-�������Ĳ���:" << endl;
		// cout << "    -��������      ʾ����䣺create index ind on student(id);" << endl;
		// cout << "    -ɾ������      ʾ����䣺drop index ind;" << endl;
		// cout << "-�Խű��Ĳ���:" << endl;
		// cout << "    -ִ�нű�      ʾ����䣺execfile test;" << endl;
		// cout << "-��ϵͳ�Ĳ���:" << endl;
		// cout << "    -�˳�ϵͳ      ʾ����䣺quit;" << endl;
		// cout << "    -������ʾ      ʾ����䣺help;" << endl;
		// cout << "��л����֧�֣�ף��ʹ����죡" << endl;
		cout << endl;
	}
	//��Ϊ�Ƿ����
	else
	{
		cout << "syntax error:" << " " << temp << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//�������������ڲ���ʽ
	return SQL;
}

/*int main()
{
	string SQL;
	while(online){
		SQL = "";
		SQL=Interpreter(SQL);
		cout<<SQL<<endl;
	}
	
}
*/

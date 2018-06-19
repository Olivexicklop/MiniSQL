#include <iostream> 
#include <string>
#include <algorithm>
#include <fstream>
#include "Interpreter.h"
 
using namespace std;

API myAPI;
extern bool online;

/////////////////////////////////////////////////////////////////////////////////////////////
//读取用户输入
string read_input()
{
	string SQL;//用户输入的语句
	string temp;
	char str[100];
	bool finish = false;//语句未输入完
	while (!finish)
	{
		cin >> str;//以空格为分割
		temp = str;
		SQL = SQL + " " + temp;
		if (SQL[SQL.length() - 1] == ';')//如果碰到分号
		{
			SQL[SQL.length() - 1] = ' ';//在分号前加一个空格
			SQL += ";";
			finish = true;//标志语句结束
		}
	}
	//将输入大写转化为小写
    transform(SQL.begin(), SQL.end(), SQL.begin(), ::tolower);
	//返回用户输入
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证create语句是否有效
string create_clause(string SQL, int start)
{
	string temp;
	int end;
	//获取第二个单词
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "syntax error: syntax error for create statement!" << endl;
		SQL = "99";
	}
	//若为table,继续验证
	else if (temp == "table")
		SQL = create_table(SQL, start);
	//若为index,继续验证
	else if (temp == "index")
		SQL = create_index(SQL, start);
	//若为错误信息，打印出错信息
	else
	{
		cout << "syntax error:" << " " << temp << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//返回create语句的内部形式
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证create table语句是否有效
string create_table(string SQL, int start)
{
	string temp, sql, T;
	int index, end, length;
	//获取表名
	while (SQL[start] == ' ')
		start++;
	index = start;
	if ((end = SQL.find('(', start)) == -1)//如果找不到左括号
	{
		cout << "error: missing ( in the statement!" << endl;
		SQL = "99";
		return SQL;
	}
	temp = SQL.substr(start, end - start);//左括号前面的是表名
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "error: error in create table statement!" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		length = temp.length() - 1;
		while (temp[length] == ' ')//去掉左括号前的空格
			length--;
		temp = temp.substr(0,length + 1);
	}
	
	//若为非法信息，打印出错信息
	if (temp.find(' ') != -1)//表名中有空格
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
		//获取每个属性
		while ((end = SQL.find(',', start)) != -1)
		{
			temp = SQL.substr(start, end - start);
			start = end + 1;
			//若有空属性，打印出错信息
			if (temp.empty())
			{
				cout << "error: error in create table statement!" << endl;
				SQL = "99";
				return SQL;
			}
			//保存属性
			else
			{
				sql = get_attribute(temp, sql);
				if (sql == "99")
					return sql;
			}
			while (SQL[start] == ' ')
				start++;
		}
		//获得最后属性
		temp = SQL.substr(start, SQL.length() - start - 1);
		length = temp.length() - 1;
		while (temp[length] != ')'&&length >= 0)
			length--;
		//若无，打印出错信息  (没有右括号
		if (length<1)
		{
			cout << "error: error in create table statement!" << endl;
			SQL = "99";
			return SQL;
		}
		//存储属性
		else
		{
			temp = temp.substr(0,length);
			end = SQL.find(' ', start);
			T = SQL.substr(start, end - start);
			start = end + 1;
			//若为主键定义
			if (T == "primary")
			{
				//判断是否有关键字key
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
				//若为空，打印出错信息
				if (T.empty())
				{
					cout << "syntax error: syntax error in create table statement!" << endl;
					cout << "\t missing key word 'key'!" << endl;
					SQL = "99";
					return SQL;
				}
				//若有，继续验证
				else if (T == "key")
				{
					//获取主键属性名
					while (SQL[start] == ' ')
						start++;
					end = SQL.find(')', start);
					T = SQL.substr(start, end - start);
					length = T.length() - 1;
					while (T[length] == ' ')
						length--;
					T = T.substr(0,length + 1);
					//若无，打印出错信息
					if (T.empty())
					{
						cout << "error : missing primary key attribute name!" << endl;
						SQL = "99";
						return SQL;
					}
					//若为非法信息，打印出错信息
					else if (T.find(' ') != -1)
					{
						cout << "error : " << T << "---is not a valid primary key attribute name!" << endl;
						SQL = "99";
						return SQL;
					}
					//保存主键
					else
					{
						sql += T + " #,";
						SQL = "01" + sql;
					}
				}
				//若为非法信息，打印出错信息
				else
				{
					cout << "error : " << T << "---is not a valid key word!" << endl;
					SQL = "99";
					return SQL;
				}
			}
			//若为一般属性
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
//获得属性
//要求支持int,float,char
string get_attribute(string temp, string sql)
{
	int start = 0, end, index;
	string T, C;
	temp += " ";
	//获得属性名
	end = temp.find(' ', start);
	T = temp.substr(start, end - start);//分解出一个单词
	start = end + 1;
	sql += T + " ";
	//获得数据类型
	while (temp[start] == ' ')//找到下一个单词的开头
		start++;
	end = temp.find(' ', start);
	T = temp.substr(start, end - start);//属性类型
	start = end + 1;
	//若无，打印出错信息
	if (T.empty())
	{
		cout << "error : error in create table statement!" << endl;
		sql = "99";
		return sql;
	}
	//若为int 
	else if (T == "int")
		sql += "+";
	//若为float
	else if (T == "float")
		sql += "-";
	//若为char
	else
	{
		index = T.find('(');//如果是其他单词？
		C = T.substr(0,index);//获取左括号左边的单词
		index++;
		//若有误，打印出错信息
		if (C.empty())
		{
			cout << "error: " << T << "---is not a valid data type definition!" << endl;
			sql = "99";
			return sql;
		}
		//若为char
		else if (C == "char")
		{
			C = T.substr(index, T.length() - index - 1);//获取char类型属性的长度
			if (C.empty())
			{
				cout << "error: the length of the data type char has not been specified!" << endl;
				sql = "99";
				return sql;
			}
			else
				sql += C;
		}
		//若为非法信息，打印出错信息
		else
		{
			cout << "error: " << C << "---is not a valid key word!" << endl;
			sql = "99";
			return sql;
		}
	}
	//是否有附加信息
	while (start<temp.length() && (temp[start] == ' '))
		start++;
	if (start<temp.length())
	{
		//若为unique定义，保存信息
		end = temp.find(' ', start);
		T = temp.substr(start, end - start);
		if (T == "unique")
		{
			sql += " 1,";
		}
		//若为非法信息，打印出错信息
		else
		{
			cout << "error: " << temp << "---is not a valid key word!" << endl;
			sql = "99";
			return sql;
		}
	}
	//若无附加信息
	else
		sql += " 0,";
	return sql;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证create index语句是否有效
string create_index(string SQL, int start)
{
	string temp, sql;
	int end, length;
	//获取第三个单词
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "syntax error: syntax error for create index statement!" << endl;
		SQL = "99";
	}
	else
	{
		sql = temp;//?
		//获取第四个单词
		while (SQL[start] == ' ')
			start++;
		end = SQL.find(' ', start);
		temp = SQL.substr(start, end - start);
		start = end + 1;
		//若无，打印出错信息
		if (temp.empty())
		{
			cout << "syntax error: syntax error for create index statement!" << endl;
			SQL = "99";
		}
		//若为on,继续验证
		else if (temp == "on"){
			//获取表名
			while (SQL[start] == ' ')
				start++;
			end = SQL.find('(', start);
			temp = SQL.substr(start, end - start);
			start = end + 1;
			//若无，打印出错信息
			if (temp.empty())
			{
				cout << "syntax error: syntax error for create index statement!" << endl;
				cout << "\t missing ( !" << endl;
				SQL = "99";
			}
			else
			{
				//检验是否为有效表名
				length = temp.length() - 1;
				while (temp[length] == ' ')
					length--;
				temp = temp.substr(0,length + 1);
				//有效
				if (temp.find(' ') == -1)
				{
					sql += " " + temp;
					//获取属性名
					while (SQL[start] == ' ')
						start++;
					end = SQL.find(')', start);
					temp = SQL.substr(start, end - start);
					start = end + 1;
					//若无，打印出错信息
					if (temp.empty())
					{
						cout << "syntax error: syntax error for create index statement!" << endl;
						cout << "\t missing ) !" << endl;
						SQL = "99";
					}
					else
					{
						//检验是否为有效属性名
						length = temp.length() - 1;
						while (temp[length] == ' ')
							length--;
						temp = temp.substr(0,length + 1);
						//有效
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
							//返回create index语句的内部形式
							else
								SQL = "02" + sql;
						}
						//无效,打印出错信息
						else
						{
							cout << "error:" << " " << temp << "---is not a valid attribute name!" << endl;
							SQL = "99";
						}
					}
				}
				//无效,打印出错信息
				else
				{
					cout << "error:" << " " << temp << "---is not a valid table name!" << endl;
					SQL = "99";
				}
			}
		}
		//若为非法信息，打印非法信息
		else
		{
			cout << "syntax error:" << " " << temp << "---is not a valid key word!" << endl;
			SQL = "99";
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证drop语句是否有效
string drop_clause(string SQL, int start)
{
	string temp;
	int end;
	//获取第二个单词
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "syntax error: syntax error for drop statement!" << endl;
		SQL = "99";
	}
	//若为table,继续验证
	else if (temp == "table")
		SQL = drop_table(SQL, start);
	//若为index,继续验证
	else if (temp == "index")
		SQL = drop_index(SQL, start);
	//若为错误信息，打印出错信息
	else
	{
		cout << "syntax error:" << " " << temp << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//返回drop语句的内部形式
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证drop table语句是否有效
string drop_table(string SQL, int start)
{
	string temp;
	int index, end;
	//获取第三个单词
	while (SQL[start] == ' ')
		start++;
	index = start;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "error: table name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL[start] == ' ')
			start++;
		//若为非法信息，打印出错信息
		if (SQL[start] != ';' || start != SQL.length() - 1)
		{
			cout << "error:" << SQL.substr(index, SQL.length() - index - 2) << "---is not a valid table name!" << endl;
			SQL = "99";
		}
		//返回drop table语句的内部形式
		else
			SQL = "11" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证drop index语句是否有效
string drop_index(string SQL, int start)
{
	string temp;
	int index, end;
	//获取第三个单词
	while (SQL[start] == ' ')
		start++;
	index = start;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "error: table name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL[start] == ' ')
			start++;
		//若为非法信息，打印出错信息
		if (SQL[start] != ';' || start != SQL.length() - 1)
		{
			cout << "error:" << SQL.substr(index, SQL.length() - index - 2) << "---is not a valid index name!" << endl;
			SQL = "99";
		}
		//返回drop index语句的内部形式
		else
			SQL = "12" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证select语句是否有效
string select_clause(string SQL, int start)
{
	string temp, temp2, sql, oldSQL;
	int end;
	sql = "";
	oldSQL = SQL;
	//获取第二个单词
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "syntax error: syntax error for select statement!" << endl;
		SQL = "99";
	}
	//若为*，继续验证
	else if (temp == "*"){
		//获取第三个单词
		while (SQL[start] == ' ')
			start++;
		end = SQL.find(' ', start);
		temp = SQL.substr(start, end - start);
		start = end + 1;
		//若无，打印出错信息
		if (temp.empty())
		{
			cout << "syntax error: syntax error for select statement!" << endl;
			SQL = "99";
		}
		//若为from，继续验证
		else if (temp == "from"){
			//获取第四个单词
			while (SQL[start] == ' ')
				start++;
			end = SQL.find(' ', start);
			temp = SQL.substr(start, end - start);//表名
			start = end + 1;
			//若无，打印出错信息
			if (temp.empty())
			{
				cout << "syntax error: syntax error for select statement!" << endl;
				SQL = "99";
			}
			else{
				while (SQL[start] == ' ')
					start++;
				//若在表名之后还有条件
				if (SQL[start] != ';' || start != SQL.length() - 1){
					while (SQL[start] == ' ')
						start++;
					end = SQL.find(' ', start);
					temp2 = SQL.substr(start, end - start);
					start = end + 1;
					//若无，打印出错信息
					if (temp2.empty())
					{
						cout << "syntax error: syntax error for select statement!" << endl;
						SQL = "99";
					}
					//若为where，继续验证
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
		//若为错误信息，打印出错信息
		else
		{
			cout << "error: you are allowed to use from here only!" << endl;
			SQL = "99";
		}
	}
	//若不是*也不为空
	else
	{
		cout << "syntax error: syntax error for select statement!" << endl;
		SQL = "99";
	}
	//返回select语句的内部形式
	return SQL;
}

////////////////////////////////////////////////////////////////////////////////////////////
//验证select_where语句是否有效
string select_where(string temp){
	int start = 0, end, index;
	string T;
	string sql;
	sql = "";
	//获取每一个条件
	while ((end = temp.find("and ", start)) != -1){
		index = end - 1;
		if (index < 0){
			sql = "99";
			return sql;
		}
		while (((temp[start] == ' ')||(temp[start] == '\'') || (temp[start] == '\"'))&&start != temp.length() - 1){
			start++;//?
		}
		if (((temp[start] == ' ')||(temp[start] == '\'') || (temp[start] == '\"'))&&start == temp.length() - 1){//没有内容
			sql = "99";
			return sql;
		}
		while (((temp[index] == ' ')||(temp[index] == '\'') || (temp[index] == '\"'))&&index > 0){
			index--;
		}
		if (index<=start){//没有内容
			sql = "99";
			return sql;
		}
		T = temp.substr(start, index - start + 1);
		T = select_condition(T);//分析该条件
		if (T == "99"){//若条件格式不对
			sql = "99";
			return sql;
		}
		sql += T + ",";
		start = end + 4;
	}
	//获取最后一个条件
	end = temp.length() - 2;
	while (temp[end] == ' ') end--;
	while (((temp[start] == ' ')||(temp[start] == '\'') || (temp[start] == '\"'))) start++;
	if (end <= start){
		sql = "99";
		return sql;
	}
	T = temp.substr(start, end - start + 1);
	T = select_condition(T);
	if (T == "99"){//若条件格式不对
		sql = "99";
		return sql;
	}
	sql += T;
	return sql;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//分解选择条件是否有效
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
//验证insert语句是否有效
string insert_clause(string SQL, int start){
	string temp, sql;
	int end;
	sql = "";
	//获取第二个单词
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "syntax error: syntax error for insert statement!" << endl;
		SQL = "99";
		return SQL;
	}
	//若为into，继续验证
	else if (temp == "into"){
		//获取第三个单词
		while (SQL[start] == ' ')
			start++;
		end = SQL.find(' ', start);
		temp = SQL.substr(start, end - start);
		start = end + 1;
		//若无，打印出错信息
		if (temp.empty())
		{
			cout << "syntax error: syntax error for insert statement!" << endl;
			SQL = "99";
			return SQL;
		}
		else {
			sql += "60" + temp;//表名
			//获取第四个单词
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
			//若无，打印出错信息
			if (temp.empty())
			{
				cout << "syntax error: syntax error for insert statement!" << endl;
				SQL = "99";
				return SQL;
			}
			else if (temp == "values"){//若为values，继续判断
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
//验证insert值是否有效
string get_insert(string temp, string sql){
	int start = 0;
	int end, index;
	string T;
	start += 1;
	//获取每一个value
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
	//获取最后一个value
	index = temp.length() - 1;
	while (((temp[index] == ' ')))
		index--;
	end = temp.find(')', start);
	if (index != end){//最后一个字符不是)
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
//验证delete语句是否有效
string delete_clause(string SQL, int start){
	string temp, temp2, sql, sql2, oldSQL;
	int end;
	sql = sql2 = "";
	oldSQL = SQL;
	//获取第二个单词
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "syntax error: syntax error for select statement!" << endl;
		SQL = "99";
	}
	//若为from，继续验证
	else if (temp == "from"){
		//获取第三个单词
		while (SQL[start] == ' ')
			start++;
		end = SQL.find(' ', start);
		temp = SQL.substr(start, end - start);//表名
		start = end + 1;
		//若无，打印出错信息
		if (temp.empty())
		{
			cout << "syntax error: syntax error for delete statement!" << endl;
			SQL = "99";
		}
		else{
			while (SQL[start] == ' ')
				start++;
			//若在表名之后还有条件
			if (SQL[start] != ';' || start != SQL.length() - 1){
				while (SQL[start] == ' ')
					start++;
				end = SQL.find(' ', start);
				temp2 = SQL.substr(start, end - start);
				start = end + 1;
				//若无，打印出错信息
				if (temp2.empty())
				{
					cout << "syntax error: syntax error for delete statement!" << endl;
					SQL = "99";
				}
				//若为where，继续验证
				else if (temp2 == "where"){
					SQL = "50" + temp;
					temp = oldSQL.substr(start, oldSQL.length() - start);
					sql = select_where(temp);//直接用select的分析函数分析
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
	//若为错误信息，打印出错信息
	else
	{
		cout << "error:missing from!" << endl;
		SQL = "99";
	}
	//返回delete语句的内部形式
	return SQL;
}

//验证quit语句是否有效
string quit_clause(string SQL, int start){
	while (SQL[start] == ' ')
		start++;
	if (SQL[start] == ';' )
		SQL = "90";
	else//如果在drop后面还有别的单词
	{
		cout << "error: The grammar for quit is wrong!" << endl;
		SQL = "99";
	}	
	return SQL;
}

//验证execfile语句是否有效
string execfile_clause(string SQL, int start) {
	string temp;
	int index, end;
	while (SQL[start] == ' ')//找到第一个不是空格的位置
		start++;
	index = start;
	end = SQL.find(' ', start);//搜索该位置后面第一个空格
	temp = SQL.substr(start, end - start);//获取该单词
	start = end + 1;//跳到下一个单词的位置
					//若无，打印出错信息
	if (temp.empty())
	{
		cout << "error: file name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL[start] == ' ')
			start++;
		//若为非法信息，打印出错信息
		if (SQL[start] != ';' || start != SQL.length() - 1)//如果在文件名字后面还有别的单词
		{
			cout << "error:" << SQL.substr(index, SQL.length() - index - 2) << "---is not a valid file name!" << endl;
			SQL = "99";
		}
		else
			SQL = execfile_file(temp);
				
	}
	return SQL;
}

//读取文件中的内容
string execfile_file(string temp) {
	temp += ".sql";
	const char* filename = temp.data();//string转char*
	int posi;
	bool flag = 0;
	string SQL, s, s2;
	s = "";
	
	ifstream infile(filename);
	if (!infile) {//若文件打开失败
		cout << "Error in reading the file!" << endl;
		SQL = "99";
		return SQL;
	}
	else {//文件打开成功
		while (getline(infile, s2)) {
			if (s2.empty()) continue;
			posi = s2.length() - 1;
			while (s2[posi] == ' ') 
				posi--;
			//一句查询结束
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
//获取用户输入，并对输入作有效性检查，若正确，返回语句的内部表示形式
string Interpreter(string statement)
{
	string SQL;
	string temp;
	int start = 0, end;
	
	if(statement.empty())	
		//获取用户输入
		SQL = read_input();
	else{
		SQL=statement;
		if (SQL[SQL.length() - 1] == ';')//如果碰到分号
		{
			SQL[SQL.length() - 1] = ' ';//在分号前加一个空格
			SQL += ";";
		}
	}		
	
	//获取输入的第一个单词
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无输入，打印出错信息
	if (temp.empty())
	{
		cout << "syntax error: empty statement!" << endl;
		SQL = "99";
	}
	//若为create语句
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
	//若为drop语句
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
	//若为select语句
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
	//若为insert语句
	else if (temp == "insert") {
		SQL = insert_clause(SQL, start) + ",";//通过逗号间隔每个value
	
		if (SQL.substr(0, 2) == "60") {
			myAPI.insertTuple(SQL);
		}
	}
	//若为delete语句
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

	//若为execfile语句
	else if (temp == "execfile") {
		SQL = execfile_clause(SQL, start);
	}
	//若为quit语句
	else if (temp == "quit") {
		SQL = quit_clause(SQL, start);
		if (SQL == "90") 
		{
			online = 0;
			//block_record.flush();
		}
	}
	//获取帮助
	else if (temp == "help") {
		SQL = "80";
		cout << endl;
		cout << endl;
		// cout << "欢迎使用本系统！" << endl;
		// cout << "本系统当前支持如下功能：" << endl;
		// cout << "-对表的操作:" << endl;
		// cout << "    -创建表        示例语句：create table student (id char(10) , primary key(id));" << endl;
		// cout << "    -删除表        示例语句：drop table student;" << endl;
		// cout << "-对记录的操作:" << endl;
		// cout << "    -插入记录      示例语句：insert into student values('3150000000');" << endl;
		// cout << "    -删除记录      示例语句：delete from student;" << endl;
		// cout << "			     delete from student where id = '3150000000';" << endl;
		// cout << "    -选择记录      示例语句：select * from student;" << endl;
		// cout << "			     select * from student where id='3150000000';" << endl;
		// cout << "-对索引的操作:" << endl;
		// cout << "    -创建索引      示例语句：create index ind on student(id);" << endl;
		// cout << "    -删除索引      示例语句：drop index ind;" << endl;
		// cout << "-对脚本的操作:" << endl;
		// cout << "    -执行脚本      示例语句：execfile test;" << endl;
		// cout << "-对系统的操作:" << endl;
		// cout << "    -退出系统      示例语句：quit;" << endl;
		// cout << "    -调出提示      示例语句：help;" << endl;
		// cout << "感谢您的支持，祝您使用愉快！" << endl;
		cout << endl;
	}
	//若为非法语句
	else
	{
		cout << "syntax error:" << " " << temp << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//返回输入语句的内部形式
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

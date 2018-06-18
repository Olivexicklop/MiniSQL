#ifndef _BTNODE_H__
#define _BTNODE_H_

#include <vector>
#include<string>

using namespace std;

class BTNode {
private:
	string BTreeFile;
	int blockId;

	vector<char*> keys;
	vector<int> idPtrs; //nonleaf-blockId; leaf-recordId

	int keyLen; //key length
	int keyNum; // in a specific node
	bool leaf;  //is leaf-true
	bool dir;  //is dirty-true
	bool deleted; //has been removed-true

public:
	BTNode(const char* fileName, int _blockId, int _keyLen); //load from file
	BTNode(const char* fileName, int _blockId, int _keyLen, bool _leaf, int firstIdPtr); //empty

	~BTNode(); //save in file if needed

	int getKeyNum();
	int isLeaf();
	
	int findPosByKey(const char* key) const;
	const char* getKeyFromPos(int pos) const;
	int getPtrFromPos(int pos) const;
	bool setKeyAtPos(char* key, int pos);
	bool setPtrAtPos(int idPtr, int pos);

	void insertKP(const char*, int idPtr, int pos); //after pos
	void deleKP(int pos);

	BTNode* divide(int newBlockId, char* newKey); //return new node
	const char* borrow(BTNode* sib, bool isFromLeft, const char* parentKey); //return new parent key
	void merge(BTNode* sib, const char* parentKey); //merge right sib


};



 
#endif

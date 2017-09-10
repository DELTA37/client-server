#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>

#define STARTBUF (100)
#define HASHTABLESIZE (5)
#define STARTDATA (100)


using namespace std;

class CDataBase;
class CEntry;
class CMemory;
class CNameNode;
class CRateNode;
class CTable;
class CStudent;


class CDataBase
{
private:
	CTable* Main;
	CTable* Cur;
	vector<CStudent*> mas;
public:
	CDataBase(void);
	~CDataBase(void);
	
	CTable* 	GetMainTable(void);
	CStudent* 	Add(CStudent s);

    std::string ToString(std::vector<std::string> f, std::string sort);
    friend class CServer;
};

class CTable
{
private:
	CEntry* hash[HASHTABLESIZE];
public:
	CTable(void);
	~CTable(void);
    
    CTable* Unique(CTable *);    
	CTable* Select(int* groups, int len);
	CTable* Select(std::vector<int> groups);
	CTable* Select(string prefix);
	CTable* Select(double rating1, double rating2);

	void 	Add(CStudent* s);
	void 	Delete(CStudent* s);
	CEntry* GetGroup(int group);

	void 	print(std::string f);

    void FromFile(const std::string& fname);
    void ToFile(const std::string& fname);
    std::string ToString(const std::vector<std::string>& f, std::string sort);
    std::string ToHTMLString(const std::vector<std::string>& f, std::string sort);
    friend class CServer;
};


class CEntry
{
private:
	CRateNode *RateRoot;
	CNameNode *NameRoot;
	int group;
	CEntry* next;
public:
	CEntry(void);
	CEntry(int group);
	CEntry(CStudent* s);
	~CEntry(void);

	void 	Delete(CStudent* s);
	void 	Add(CStudent* s);

	void 	print(std::string f);
    
    std::string ToString(const std::vector<std::string>& f, std::string sort);
    std::string ToHTMLString(const std::vector<std::string>& f, std::string sort);
	friend class CTable;
    friend class CServer;
};

class CStudent
{
public:
    string 	name;
	double 	rating;
	int 	group;
	string 	info;
public:
	CStudent(void) : name(""), rating(-1), group(-1), info("") {}
	CStudent(const string& , double rating, int group, const string& info);
	CStudent(const string& , double rating, int group);
	CStudent(const CStudent&);

	CStudent& 	operator=(const CStudent&);
	bool 		operator==(const CStudent&);
	void 		print(void);
	void 		scan(void);

	string& 	GetName(void) 			{return name; }
	double& 	GetRating(void) 		{return rating; }
	int& 		GetGroup(void) 			{return group; }
	string& 	GetInfo(void) 			{return info; }

    string      ToString(const std::vector<std::string>& f);
    string      ToHTMLString(const std::vector<std::string>& f);

	friend istream& operator>>(istream& in, CStudent& s);
	friend ostream& operator<<(ostream& out, const CStudent& s);

	friend class CRateNode;
};


class CRateNode
{
private:
	CStudent*	s;
	double 		rating;
	CRateNode*	next;
	CRateNode*	prev;
public:
	CRateNode(void) 	: s(NULL), rating(0), next(NULL), prev(NULL)		{}
	CRateNode(CStudent *_s) : s(_s), rating(_s->rating), next(NULL), prev(NULL)	{}
	CRateNode(const CRateNode& Q);
	~CRateNode(void);

	CRateNode* 	Add(CStudent* _s);
	CRateNode* 	Delete(CStudent* _s);
	CRateNode* 	Select(double rating1, double rating2);

	bool 		check(void);
	void 		print(void);

	friend class CIterRate;
	friend class CTable;
	friend class CEntry;
    friend class CServer;
};

class CNameNode
{
private:
	CStudent*	s;
	string 		name;

	unsigned int 	bleft;
	unsigned int 	bright;

	CNameNode*	parent;
	CNameNode*	left;
	CNameNode*	right;
public:
	CNameNode(void) 	: s(NULL), name(""), bleft(0), bright(0), parent(NULL), left(NULL), right(NULL)	{}
	CNameNode(CStudent *_s) : s(_s), name(s->name), bleft(0), bright(0), parent(NULL), left(NULL), right(NULL) 	{}
	~CNameNode(void);

	static CNameNode* 	Complete(CNameNode* R1, CNameNode* R2);
	void 			Add(CStudent* _s);
	void 			Delete(CStudent* _s);
	CNameNode* 		Select(string prefix);

	void 			print(void);
	int 			check(void);

	friend class CIterName;
	friend class CEntry;
	friend class CTable;
    friend class CServer;
};


class CMemory
{
private:
	CStudent 	mem[STARTBUF];
	int 		freedom[STARTBUF];
	size_t 		frnum;
public:
	CMemory(void);

	void 		Free(CStudent *s);
	CStudent* 	Alloc(void);
};

class CIterName
{
private:
	CNameNode* root;
	CNameNode* cur;
public:
	CIterName(CNameNode* _root);

	void 		Start(void);
	bool 		is_end(void);
	CStudent* 	Next(void);
};

class CIterRate
{
private:
	CRateNode* root;
	CRateNode* cur;
public:
	CIterRate(CRateNode* _root);

	void 		Start(void);
	bool 		is_end(void);
	CStudent* 	Next(void);
};

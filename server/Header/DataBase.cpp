#include "DataBase.hpp"

//----------------CStudent--------------

CStudent::CStudent(const string& name, double rating, int group, const string& info)
{
	this->name 	= name;
	this->rating 	= rating;
	this->group 	= group;
	this->info 	= info;
}

CStudent::CStudent(const string& name, double rating, int group)
{
	this->name 	= name;
	this->rating 	= rating;
	this->group 	= group;
	this->info 	= "";
}

void CStudent::print(void)
{
	cout << "Name: " 	<< this->name 	<< endl
	     << "Group: " 	<< this->group 	<< endl
	     << "Rating: " 	<< this->rating << endl
	     << "Info: " 	<< this->info 	<< endl;
}

bool CStudent::operator==(const CStudent& st)
{
	return ((st.name == this->name)&&(st.group == this->group));
}

void CStudent::scan(void)
{
	cin >> this->name >> this->group >> this->rating >> this->info;
}

CStudent::CStudent(const CStudent& R)
{
	this->name 	= R.name;
	this->info 	= R.info;
	this->group 	= R.group;
	this->rating 	= R.rating;
}


CStudent& CStudent::operator=(const CStudent& R)
{
	this->name 	= R.name;
	this->info 	= R.info;
	this->group 	= R.group;
	this->rating 	= R.rating;

	return *this;
}

istream& operator>>(istream& in, CStudent& s)
{
	in >> s.name >> s.rating >> s.group >> s.info;

	return in;
}

ostream& operator<<(ostream& out, const CStudent& s)
{
	out << "Name: " 	<< s.name 	<< ", "
	    << "Group: " 	<< s.group 	<< ", "
	    << "Rating: " 	<< s.rating 	<< ", "
	    << "Info: " 	<< s.info 	<< endl;

	return out;
}

//------------------CRateNode ------------


CRateNode::~CRateNode(void)
{
	if (this->next != NULL)
	{
		delete this->next;
	}
	this->prev = NULL;
	this->next = NULL;
}

CRateNode* CRateNode::Add(CStudent* _s)
{
	if (this->s == _s)
	{
		return this;
	}
	else if (this->s == NULL)
	{
		this->s 	= _s;
		this->rating 	= _s->rating;

		return this;
	}
	else
	{
		if (this->next == NULL)
		{
			this->next 		= new CRateNode(_s);
			this->next->prev 	= this;

			return this;
		}
		else
		{
			if (_s->rating > this->rating)
			{
				this->next 		= this->next->Add(_s);
				this->next->prev 	= this;

				return this;
			}
			else
			{
				CRateNode* chain 	= new CRateNode(_s);
				chain->next 		= this;
				chain->prev 		= this->prev;

				if (this->prev != NULL)
				{
					this->prev->next = chain;
				}

				this->prev 		= chain;

				if (chain->prev == NULL)
				{
					return chain;
				}
				else
				{
					return this;
				}
			}
		}
	}
}

CRateNode::CRateNode(const CRateNode& Q)
{
	this->prev = NULL;
	this->s = Q.s;
	this->rating = Q.rating;
	if (Q.next != NULL)
	{
		this->next 		= new CRateNode(*(Q.next));
		this->next->prev 	= this;
	}
	else
	{
		this->next 		= NULL;
	}
}

void CRateNode::print(void)
{
	this->s->print();
	if (this->next != NULL)
	{
		this->next->print();
	}
}

CRateNode* CRateNode::Delete(CStudent* _s)
{
	if (this->s == _s)
	{
		if (this->next == NULL)
		{
			this->s 	= NULL;
			this->rating 	= -1;

			return this;
		}
		else
		{
			CRateNode *res 	= this->next;

			if (this->prev != NULL)
			{
				this->prev->next = res;
			}

			res->prev 	= this->prev;
			this->next 	= NULL;
			this->prev 	= NULL;

			delete this;

			return res;
		}
	}
	else
	{
		if (this->next != NULL)
		{
			this->next 		= this->next->Delete(_s);
			this->next->prev 	= this;
		}
		return this;
	}
}

CRateNode* CRateNode::Select(double rating1, double rating2)
{
	CRateNode *where = this;
	while (where->rating < rating1)
	{
		if (where->next != NULL)
		{
			where = where->next;
		}
		else
		{
			return NULL;
		}
	}

	if (where->rating > rating2)
	{
		return NULL;
	}

	CRateNode* res 	= new CRateNode(*where);
	where 		= res;

	while(where->rating <= rating2)
	{
		if (where->next != NULL)
		{
			where = where->next;
		}
		else
		{
			return res;
		}
	}

	where->prev->next = NULL;
	delete where;

	return res;
}

bool CRateNode::check(void)
{
	CRateNode* where = this;
	while(where->next != NULL)
	{
		if (where->next->prev != where)
		{
			return 0;
		}
		where = where->next;
	}
	return 1;
}


// ---------------CNameNode----------------

void CNameNode::Add(CStudent* _s)
{
	if (this->s == _s)
	{
		return;
	}
	else if (this->s == NULL)
	{
		this->s 	= _s;
		this->name 	= _s->name;
		this->bright 	= 0;
		this->bleft 	= 0;
	}
	else if (_s->name < this->name)
	{
		if (bleft <= bright)
		{
			if (this->left == NULL)
			{
				this->left 		= new CNameNode(_s);
				this->left->parent 	= this;
			}
			else
			{
				this->left->Add(_s);
			}
			bleft++;
		}
		else
		{
			CStudent* tmp = this->s;
			this->s = _s;
			this->name = _s->name;
			if (this->right == NULL)
			{
				this->right 		= new CNameNode(tmp);
				this->right->parent 	= this;
			}
			else
			{
				this->right->Add(tmp);
			}
			bright++;
		}
	}
	else
	{
		if (bright <= bleft)
		{
			if (this->right == NULL)
			{
				this->right 		= new CNameNode(_s);
				this->right->parent 	= this;
			}
			else
			{
				this->right->Add(_s);
			}
			bright++;
		}
		else
		{
			CStudent* tmp = this->s;
			this->s = _s;
			this->name = _s->name;
			if (this->left == NULL)
			{
				this->left 		= new CNameNode(tmp);
				this->left->parent 	= this;
			}
			else
			{
				this->left->Add(tmp);
			}
			bleft++;
		}

	}
}



void CNameNode::Delete(CStudent* _s)
{
	if (_s == this->s)
	{
		if ((this->right == NULL)&&(this->left == NULL))
		{
			if (this->parent == NULL)
			{
				this->s 	= NULL;
				this->name 	= "";
				this->bleft 	= 0;
				this->bright 	= 0;
			}
		}
		else if (this->right == NULL)
		{
			bleft 		= 0;
			this->s 	= this->left->s;
			this->name 	= this->left->name;

			delete this->left;
			this->left 	= NULL;
		}
		else if (this->left == NULL)
		{
			bright 		= 0;

			this->s 	= this->right->s;
			this->name 	= this->right->name;

			delete this->right;
			this->right 	= NULL;
		}
		else
		{
			if (bleft >= bright)
			{
				this->s 	= this->left->s;
				this->name 	= this->left->name;

				this->left->Delete(this->s);

				bleft--;
			}
			else
			{
				this->s 	= this->right->s;
				this->name 	= this->right->name;

				this->right->Delete(this->s);

				bright--;
			}
		}
	}
	else if (this->name < _s->name)
	{
		bright--;
		this->right->Delete(_s);
	}
	else
	{
		bleft--;
		this->left->Delete(_s);
	}
}

int CNameNode::check(void)
{
	if (this->left != NULL)
	{
		if (this->left->parent != this)
		{
			return 0;
		}
		if (this->left->check() == 0)
		{
			return 0;
		}
	}
	if (this->right != NULL)
	{
		if (this->right->parent != this)
		{
			return 0;
		}
		if (this->right->check() == 0)
		{
			return 0;
		}
	}
	return 1;
}



CNameNode* CNameNode::Select(string prefix)
{
	CNameNode* res 	= NULL;
	CNameNode* L 	= NULL;
	CNameNode* R 	= NULL;
	
	if ((this->left == NULL)&&(this->right == NULL))
	{
		if ((this->s->name.length() >= prefix.length())&&(this->s->name.substr(0, prefix.length()) == prefix))
		{
			res = new CNameNode(this->s);

			return res;
		}
		else
		{
			return NULL;
		}
	}
	else if (this->left == NULL)
	{
		R = this->right->Select(prefix);

		if ((this->s->name.length() >= prefix.length() )&&(this->s->name.substr(0, prefix.length()) == prefix))
		{
			R->Add(this->s);

			return R;
		}
		else
		{
			return R;
		}
	}
	else if (this->right == NULL)
	{
		L = this->left->Select(prefix);

		if ((this->s->name.length() >= prefix.length())&&(this->s->name.substr(0, prefix.length()) == prefix))
		{
			L->Add(this->s);

			return L;
		}
		else
		{
			return L;
		}
	}
	else
	{
		R 	= this->right->Select(prefix);
		L 	= this->left->Select(prefix);

		res 	= CNameNode::Complete(R, L);
		
		delete R;
		delete L;

		if ((this->s->name.length() >= prefix.length())&&(this->s->name.substr(0, prefix.length()) == prefix))
		{
			res->Add(this->s);

			return res;
		}
		else
		{
			return res;
		}
	}
	
}

CNameNode* CNameNode::Complete(CNameNode *R1, CNameNode *R2)
{
	if ((R1 == NULL)&&(R2 == NULL))
	{
		return NULL;
	}
	else if (R1 == NULL)
	{
		CNameNode* res 	= CNameNode::Complete(R2->left, R2->right);
		
		if (res != NULL)
		{
			res->Add(R2->s);
		}
		else
		{
			res 	= new CNameNode(R2->s);
		}
		
		return res;
	}
	else if (R2 == NULL)
	{
		CNameNode* res 	= CNameNode::Complete(R1->left, R1->right);
		
		if (res != NULL)
		{
			res->Add(R1->s);
		}
		else
		{
			res 	= new CNameNode(R1->s);
		}
		
		return res;
	}
	else
	{
		CNameNode* L 	= CNameNode::Complete(R1->left, R2->left);
		CNameNode* R 	= CNameNode::Complete(R1->right, R2->right);
		CNameNode* res 	= CNameNode::Complete(L, R);

		if (res != NULL)
		{
			res->Add(R1->s);
			res->Add(R2->s);
		}
		else
		{
			res 	= new CNameNode(R1->s);
			res->Add(R2->s);
		}

		delete L;
		delete R;

		return res;
	}
}

CNameNode::~CNameNode(void)
{
	if (this->left != NULL)
	{
		delete this->left;
	}
	if (this->right != NULL)
	{
		delete this->right;
	}
}

void CNameNode::print(void)
{
	if (this->s != NULL)
	{
		this->s->print();
#ifdef DEBUG
		cout 	<< "in left: " 		<< bleft 	<< endl;
		cout 	<< "in right: " 	<< bright 	<< endl;
#endif
	}
	if (this->left != NULL)
	{
#ifdef DEBUG
		cout << "------------left---------------" << endl;
#endif
		this->left->print();
	}
	if (this->right != NULL)
	{
#ifdef DEBUG
		cout << "------------right--------------" << endl;
#endif
		this->right->print();
	}
}


//-------CMemory------------------------

CMemory::CMemory(void)
{
	for (int i = 0; i < STARTBUF; i++)
	{
		freedom[i] = i;
	}
	frnum = STARTBUF;
}

void CMemory::Free(CStudent* s)
{
	int diff = (s - mem) / sizeof(CStudent);

	if ((diff <= STARTBUF)&&(diff >= 0))
	{
		for (int i = 0; i < STARTBUF; i++)
		{
			if (freedom[i] == diff)
			{
				return;
			}
		}
		freedom[frnum] = diff;
		frnum++;
	}
}

CStudent* CMemory::Alloc(void)
{
	if (frnum == 0)
	{
		return NULL;
	}
	else
	{
		return &(mem[freedom[frnum - 1]]);
		frnum--;
	}
}

//---------Iterator for CNameNode--------


CIterName::CIterName(CNameNode* _root)
{
	root = _root;
	cur = _root;
	while (cur->left != NULL)
	{
		cur = cur->left;
	}
}

CStudent* CIterName::Next(void)
{
	CNameNode* res = cur;
    if (cur == NULL)
    {
        return NULL;
    }
	if (cur->right != NULL)
	{
		cur = cur->right;
	}
	else
	{
		if (cur->parent == NULL)
		{
			cur = NULL;
		}
		else
		{
			while(cur->parent->left != cur)
			{
				cur = cur->parent;
				if (cur->parent == NULL)
				{
					cur = NULL;
					break;
				}
			}
			if (cur != NULL)
			{
				cur = cur->parent;
			}
		}
	}

	if (res != NULL)
	{
		return res->s;
	}
	else
	{
		return NULL;
	}
}

bool CIterName::is_end(void)
{
	return (cur == NULL)||(cur->s == NULL);
}

void CIterName::Start(void)
{
	cur = root;
	while (cur->left != NULL)
	{
		cur = cur->left;
	}
}

//---------------CIterRate---------------

CIterRate::CIterRate(CRateNode* _root)
{
	root = _root;
	cur = root;
}

CStudent* CIterRate::Next(void)
{
	CRateNode* res = cur;

	if (cur != NULL)
	{
		cur = cur->next;
	}
	
	if (res != NULL)
	{
		return res->s;
	}
	else
	{
		return NULL;
	}
}

bool CIterRate::is_end(void)
{
	return (cur == NULL)||(cur->s == NULL);
}

void CIterRate::Start(void)
{
	cur = root;
}


//---------------CEntry------------------


CEntry::CEntry(void)
{
	next = NULL;
	RateRoot = new CRateNode();
	NameRoot = new CNameNode();
	group = -1;
}

CEntry::CEntry(int group)
{
	next = NULL;
	RateRoot = new CRateNode();
	NameRoot = new CNameNode();
	this->group = group;
}

CEntry::CEntry(CStudent* s)
{
	next = NULL;
	RateRoot = new CRateNode(s);
	NameRoot = new CNameNode(s);
	this->group = s->group;
}

void CEntry::Add(CStudent* s)
{
	if (group == -1)
	{
		group = s->group;
		RateRoot->Add(s);
		NameRoot->Add(s);
	}
	else if (s->group % HASHTABLESIZE == group % HASHTABLESIZE)
	{
		if (s->group == group)
		{
			RateRoot->Add(s);
			NameRoot->Add(s);
		}
		else
		{
			if (next == NULL)
			{
				next = new CEntry(s);
			}
			else
			{
				next->Add(s);
			}
		}
	}
}

void CEntry::Delete(CStudent* s)
{
	if (s->group % HASHTABLESIZE == group % HASHTABLESIZE)
	{
		if (s->group == group)
		{
			RateRoot->Delete(s);
			NameRoot->Delete(s);
		}
		else
		{
			if (next != NULL)
			{
				next->Delete(s);
			}
		}
	}
}


CEntry::~CEntry(void)
{
	delete NameRoot;
	delete RateRoot;
	if (this->next != NULL)
	{
		delete this->next;
	}
}

void CEntry::print(std::string v)
{
	if (this->group != -1)
	{
		cout << "group: " << this->group << endl;
        if (v == "rating")
        {
		    RateRoot->print();
        }
        else if (v == "name")
        {
            NameRoot->print();
        }
		cout << "-----------------------------" << endl;
		if (this->next != NULL)
		{
			this->next->print(v);
		}
	}
}

//---------------CTable-------------------

CTable::CTable(void)
{
	for (int i = 0; i < HASHTABLESIZE; i++)
	{
		hash[i] = new CEntry();
	}
}

CTable::~CTable(void)
{
	for (int i = 0; i < HASHTABLESIZE; i++)
	{
		if (hash[i] != NULL)
		delete hash[i];
	}
}

void CTable::Add(CStudent* s)
{
	hash[s->group % HASHTABLESIZE]->Add(s);
}

void CTable::Delete(CStudent* s)
{
	hash[s->group % HASHTABLESIZE]->Delete(s);
}

CTable* CTable::Select(std::string prefix)
{
	CTable* res = new CTable();
	
	for (int i = 0; i < HASHTABLESIZE; i++)
	{
		CStudent* cur = NULL;
		CEntry* entr = hash[i];
		while(entr != NULL && entr->group >= 0)
		{
				CNameNode* aft = entr->NameRoot->Select(prefix);
				if (aft != NULL)
				{
						CIterName it1(aft);
						while ((cur = it1.Next()) != NULL)
						{
							res->Add(cur);
						}
						delete aft;
				}
				entr = entr->next;
		}
	}
    return res;
}

CTable* CTable::Select(int* groups, int len)
{
	CTable* res = new CTable();
	
	for (int i = 0; i < len; i++)
	{
		CStudent* cur = NULL;
		CEntry *entr = hash[groups[i] % HASHTABLESIZE];
		while(entr != NULL)
		{
				if (entr->group == groups[i])
				{
						CRateNode* RNode = entr->RateRoot;
						CIterRate it1(RNode);
						while ((cur = it1.Next()) != NULL)
						{
							res->Add(cur);
						}
				}
				entr = entr->next;
		}
	}
    return res;
}

CTable* CTable::Select(double rating1, double rating2)
{
	CTable* res = new CTable();

	for (int i = 0; i < HASHTABLESIZE; i++)
	{
		CStudent* cur = NULL;
		CEntry *entr = hash[i];

		while(entr != NULL && entr->group >= 0)
		{
			CRateNode* aft = entr->RateRoot->Select(rating1, rating2);
			CIterRate it(aft);

			while((cur = it.Next()) != NULL)
			{
				res->Add(cur);
			}
			entr = entr->next;
			delete aft;
		}
	}
    return res;
}

CTable* CTable::Select(std::vector<int> groups)
{
    return this->Select(groups.data(), groups.size());
}

CEntry* CTable::GetGroup(int group)
{
	CEntry* start = hash[group % HASHTABLESIZE];

	while ((start != NULL)&&(start->group != group))
	{
		start = start->next;
	}

	return start;
}

void CTable::print(std::string f)
{
	for (int i = 0; i < HASHTABLESIZE; i++)
	{
		hash[i]->print(f);
	}
}

CTable* CTable::Unique(CTable *t)
{
    CTable* res = new CTable();
    for (int i = 0; i < HASHTABLESIZE; i++)
    {
        CEntry* entr = this->hash[i];
        while(entr != NULL && entr->RateRoot != NULL)
        {
            CRateNode *r = entr->RateRoot;
            while(r != NULL && r->s != NULL)
            {
                res->Add(r->s);
                r = r->next;
            }
        }
    }
    for (int i = 0; i < HASHTABLESIZE; i++)
    {
        CEntry* entr = t->hash[i];
        while(entr != NULL && entr->RateRoot != NULL)
        {
            CRateNode *r = entr->RateRoot;
            while(r != NULL && r->s != NULL)
            {
                res->Add(r->s);
                r = r->next;
            }
        }
    }
    return res;
}

void CTable::FromFile(const std::string& fname)
{
    FILE *fp = fopen(fname.c_str(), "r");
    
    while(feof(fp) == 0)
    {
        char buf[100];
        double rating;
        int group;
        fscanf(fp, "%s\n%lf\n%d\n", buf, &rating, &group);
        CStudent *s = new CStudent(std::string(buf), rating, group);
        this->Add(s);
    }
}

void CTable::ToFile(const std::string& fname)
{
    FILE *fp = fopen(fname.c_str(), "w");

    for (int l = 0; l < HASHTABLESIZE; l++)
    {
        CEntry * entr = this->hash[l];
        while(entr != NULL && entr->RateRoot != NULL)
        {
            CRateNode *r = entr->RateRoot;
            while(r != NULL && r->s != NULL)
            {
                fprintf(fp, "%s\n%lf\n%d\n", r->s->name.c_str(), r->s->rating, r->s->group);
                r = r->next;
            }
            entr = entr->next;
        }
    }

}

//---------------CDataBase-----------------

CDataBase::CDataBase(void)
{
	Main = new CTable();
	Cur = new CTable();
}

CStudent* CDataBase::Add(CStudent s)
{
	CStudent* ns = new CStudent(s);
	mas.push_back(ns);
	Main->Add(ns);
	return ns;
}


CDataBase::~CDataBase(void)
{
	delete Main;
	delete Cur;
	for (int i = 0; i < (int)mas.size(); i++)
	{
		delete mas[i];
	}
}

CTable* CDataBase::GetMainTable(void)
{
	return this->Main;
}

//--------------ToString-------------------



string CStudent::ToString(const std::vector<std::string>& f)
{
    string s;
	for (int i = 0; i < (int)f.size(); i++)
	{
			if (f[i] == "name")
			{
				s += "name: "; 
				s += this->name;
				s += "\n";
			}
			if (f[i] == "rating")
			{
				s += "rating: " + std::to_string(this->rating) + "\n";
			}
			if (f[i] == "group")
			{
				s += "group: " + std::to_string(this->group) + "\n";
			}
			if (f[i] == "info")
			{
				s += "info: " + this->info + "\n";
			}
	}
    return s;
}

string CTable::ToString(const std::vector<std::string>& f, std::string sort)
{
    std::string res;
    for (int i = 0; i < HASHTABLESIZE; i++)
    {
        CEntry *entr = this->hash[i];
        while(entr != NULL && entr->group != -1)
        {
            res += entr->ToString(f, sort);
            res += "\n";
            entr = entr->next;
        }
    }
    return res;
}

string CEntry::ToString(const std::vector<std::string>& f, std::string sort)
{
    std::string res;
    if (sort == "name")
    {
        CIterName it(this->NameRoot); 

        while(it.is_end() == 0)
        {
            CStudent* s = it.Next();
            if (s != NULL)
            res += s->ToString(f) + "\n";
        }
    }
    else if(sort == "rating")
    {
        CIterRate it(this->RateRoot); 

        while(it.is_end() == 0)
        {
            CStudent* s = it.Next();
            if (s != NULL)
            res += s->ToString(f) + "\n";
        }
    }
    else
    {
        CIterRate it(this->RateRoot); 

        while(it.is_end() == 0)
        {
            CStudent* s = it.Next();
            if (s != NULL)
            res += s->ToString(f) + "\n";
        }
    }
    return res;
}



std::string CDataBase::ToString(std::vector<std::string> f, std::string sort)
{
    return this->Cur->ToString(f, sort);
}



//--------------ToHTMLString-------------------



string CStudent::ToHTMLString(const std::vector<std::string>& f)
{
    string s;
    s += "<ul>";
	for (int i = 0; i < (int)f.size(); i++)
	{
			if (f[i] == "name")
			{
				s += "<li>.name: "; 
				s += this->name;
				s += "</li\n";
			}
			if (f[i] == "rating")
			{
				s += "<li>rating: " + std::to_string(this->rating) + "</li>\n";
			}
			if (f[i] == "group")
			{
				s += "<li>group: " + std::to_string(this->group) + "</li>\n";
			}
			if (f[i] == "info")
			{
				s += "<li>info: " + this->info + "</li>\n";
			}
			s += "</ul>";
	}
    return s;
}

string CTable::ToHTMLString(const std::vector<std::string>& f, std::string sort)
{
    std::string res;
    res += "<html>";
    for (int i = 0; i < HASHTABLESIZE; i++)
    {
        CEntry *entr = this->hash[i];
        while(entr != NULL && entr->group != -1)
        {
            res += entr->ToHTMLString(f, sort);
            res += "\n";
            entr = entr->next;
        }
    }
    res += "</html>";
    return res;
}

string CEntry::ToHTMLString(const std::vector<std::string>& f, std::string sort)
{
    std::string res;
    res += "<div>";
    if (sort == "name")
    {
        CIterName it(this->NameRoot); 

        while(it.is_end() == 0)
        {
            CStudent* s = it.Next();
            if (s != NULL)
	    {
            	res += s->ToHTMLString(f) + "\n";
	    }
        }
    }
    else if(sort == "rating")
    {
        CIterRate it(this->RateRoot); 

        while(it.is_end() == 0)
        {
            CStudent* s = it.Next();
            if (s != NULL)
	    {
            	res += s->ToHTMLString(f) + "\n";
	    }
        }
    }
    else
    {
        CIterRate it(this->RateRoot); 

        while(it.is_end() == 0)
        {
            CStudent* s = it.Next();
            if (s != NULL)
            res += s->ToHTMLString(f) + "\n";
        }
    }
    res += "</div>";
    return res;
}


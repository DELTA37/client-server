#include "Server.hpp"

int main(void)
{
    /*
     * 
     *
     */
   /* 
    CTable t;
    CStudent s(std::string("Regina"), 5, 209, "George loves Regina");
    CStudent s1(std::string("George"), 5, 210, "Regina loves George");
    std::set<std::string> f;
    f.insert("name");
    f.insert("rating");
    f.insert("group");
    */
   // t.Add(&s);
    //t.Add(&s1);
   // std::cout << t.ToString(f, "name") << std::endl;
    /*
    t.FromFile("data");
    std::cout << t.ToString(f, "name") << std::endl;
    */
    
    CServer serv;
    /*
    serv.Do(0, "insert name=Regina rating=5 group=209 end");
    serv.Do(0, "insert name=George rating=5 group=210 end");
    serv.Do(0, "print name group end");
    */
    serv.MainLoop();
    return 0;
}

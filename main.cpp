#include<iostream>
#include <string>
#include <sstream>

using namespace std;

struct SStop {

public:
    string m_Name;
    SStop *m_Next;
    int m_Fee;

    SStop(string name, int fee) {
        m_Fee = fee;
        m_Name = name;
        m_Next = NULL;
    }
};

class CStationException {
public:
    string text;

    CStationException(const string &x) : text(x) {}

    friend ostream &operator<<(ostream &os, const CStationException &x) {
        os << x.text;
        return os;
    }

};

class CTrip {
public:
    int c_Fee;
    string cesta;

    friend ostream &operator<<(ostream &os, const CTrip &x) {
        os << x.cesta;
        return os;
    }

    CTrip() : c_Fee(0) {};

    int Fee() {
        return c_Fee;
    }
};

class CTrain {
public:
    CTrain() {
        m_Stations = NULL;
        m_Last = NULL;
    }

    ~CTrain() {
        SStop *helper = m_Stations;
        while (helper != m_Last) {
            delete (helper);
            helper = helper->m_Next;
        }
        delete (helper);
    }

    bool newline(const char &c) {
        if (c != '\n')
            return false;
        else
            return true;
    }

    void operator=(const CTrain &other) { // copy assignment
        SStop *del = m_Stations;
        while (del != m_Last) {
            delete (del);
            del = del->m_Next;
        }
        delete (del);
        m_Stations = NULL;
        SStop *helper = other.m_Stations;
        while (helper != other.m_Last) {
            Push(helper->m_Name, helper->m_Fee);
            helper = helper->m_Next;
        }
        Push(helper->m_Name, helper->m_Fee);
    }

    void Add(istringstream &is) {
        string a_Name;
        int a_Fee;
        char ch;
        while (getline(is, a_Name, ';')) {
            is >> a_Fee;
            while ((ch = is.peek()) == '\n') {
                if (ch == '\n')
                    is.get(ch);
            }
            Push(a_Name, a_Fee);
        }
    }

    void Push1(const string &name, const int &fee) {
        if (!m_Stations) {
            m_Stations = new SStop(name, fee);
            m_Last = m_Stations;
            m_Last->m_Next = m_Stations;
        } else {
            SStop *nova = new SStop(name, fee);
            nova->m_Next = m_Stations;
            m_Last->m_Next = nova;
            m_Last = nova;
        }
    }

    void Push(const string &name, const int &fee) {
        if (!m_Stations) {
            m_Stations = new SStop(name, fee);
            m_Last = m_Stations;
            m_Last->m_Next = m_Stations;
        } else {
            SStop *helper = new SStop(name, fee);
            helper->m_Next = m_Stations;
            m_Last->m_Next = helper;
            m_Last = helper;
        }
    }

    void Print() {
        SStop *helper = m_Stations;

        while (helper != m_Last) {
            cout << helper->m_Name << ", " << helper->m_Fee << endl;
            helper = helper->m_Next;
        }
        cout << helper->m_Name << ", " << helper->m_Fee << endl;
    }

    CTrip Trip(const string &nastup, const string &vystup) {
        string cesta;
        bool Nnalezeno = false;
        bool Vnalezeno = true;
        CTrip vylet;
        bool first = true;
        SStop *helper = m_Stations;
        if (nastup == m_Last->m_Name) {
            // cout << m_Last->m_Name << endl;
            cesta.append(helper->m_Name);
            cesta.push_back('\n');
            first = false;
            vylet.c_Fee += m_Last->m_Fee;
            if (nastup != vystup)
                goto label;
            else {
                vylet.c_Fee += m_Last->m_Fee;
                vylet.cesta = cesta;
                return vylet;
            }
        }
        while (helper != m_Last) {
            if ((helper->m_Name) == nastup) {
                vylet.c_Fee += helper->m_Fee;
                label:
                SStop *check;
                check = helper;
                Nnalezeno = true;
                while ((helper->m_Name) != vystup) {
                    //   cout << helper->m_Name << endl;
                    if (!first)
                        cesta.append("-> ");
                    cesta.append(helper->m_Name);
                    cesta.push_back('\n');
                    first = false;
                    // vylet.c_Fee += helper->m_Fee;
                    helper = helper->m_Next;
                    if (helper == check) {
                        Vnalezeno = false;
                        break;
                    }
                }
                //  cout << helper->m_Name << endl;
                if (!first)
                    cesta.append("-> ");
                first = false;
                cesta.append(helper->m_Name);
                cesta.push_back('\n');
                vylet.c_Fee += helper->m_Fee;
                break;
            }
            helper = helper->m_Next;
        }
        if (!Nnalezeno)
            throw CStationException("Unknown station \'" + nastup + "\'\n");
        if (!Vnalezeno)
            throw CStationException("Unknown station \'" + vystup + "\'\n");

        vylet.cesta = cesta;
        return vylet;
    }

private:
    SStop *m_Stations;
    SStop *m_Last;

};


int main() {
    istringstream is;
    int fee;
    CTrain Thomas;
    is.clear();
    is.str("Knapford;20\nTidmouth;10\nBrendam Docks;10\nSodor Steamwork;5\nGordon\'s Hill;1\n");
    Thomas.Add(is);
    fee = Thomas.Trip("Tidmouth", "Sodor Steamwork").Fee(); // fee = 15
    cout << Thomas.Trip("Tidmouth", "Sodor Steamwork");

    cout << "fi: " << fee << endl;
/*
--8<----8<----8<----8<----8<----8<----8<--
Tidmouth
-> Brendam Docks
-> Sodor Steamwork
--8<----8<----8<----8<----8<----8<----8<--
*/
    cout << "----------" << endl;
    fee = Thomas.Trip("Brendam Docks", "Knapford").Fee(); // fee = 30
    cout << Thomas.Trip("Brendam Docks", "Knapford");
    cout << "fi: " << fee << endl;
/*
--8<----8<----8<----8<----8<----8<----8<--
Brendam Docks
-> Sodor Steamwork
-> Gordon's Hill
-> Knapford
--8<----8<----8<----8<----8<----8<----8<--
*/
    cout << "----------" << endl;
    fee = Thomas.Trip("Tidmouth", "Tidmouth").Fee(); // fee = 20
    cout << Thomas.Trip("Tidmouth", "Tidmouth");
    cout << "fi: " << fee << endl;
/*
--8<----8<----8<----8<----8<----8<----8<--
Tidmouth
--8<----8<----8<----8<----8<----8<----8<--
*/
    cout << "----------" << endl;
    is.clear();
    is.str("Whiff\'s Waste Dump;0\nSodor Ironworks;5\n");
    Thomas.Add(is);
    fee = Thomas.Trip("Gordon's Hill", "Knapford").Fee(); // fee = 21
    cout << Thomas.Trip("Gordon's Hill", "Knapford");
    cout << "fi: " << fee << endl;
/*
--8<----8<----8<----8<----8<----8<----8<--
Gordon's Hill
-> Whiff's Waste Dump
-> Sodor Ironworks
-> Knapford
--8<----8<----8<----8<----8<----8<----8<--
*/
    cout << "----------" << endl;
    try { cout << Thomas.Trip("Emily's Hill", "Knapford"); }
    catch (const CStationException &e) { cout << e; }
/*
--8<----8<----8<----8<----8<----8<----8<--
Unknown station 'Emily's Hill'
--8<----8<----8<----8<----8<----8<----8<--
*/
    cout << "----------" << endl;
    CTrain Edward;
    is.clear();
    is.str("Knapford;20\nTidmouth;10\n\n\nSodor Steamwork;5\nGordon\'s Hill;1\nBrendam Docks;10\n");
    Edward.Add(is);
    fee = Edward.Trip("Tidmouth", "Gordon's Hill").Fee(); // fee = 11
    cout << Edward.Trip("Tidmouth", "Gordon's Hill");
    cout << "fi: " << fee << endl;
/*
--8<----8<----8<----8<----8<----8<----8<--
Tidmouth
-> Sodor Steamwork
-> Gordon's Hill
--8<----8<----8<----8<----8<----8<----8<--
*/cout << "----------" << endl;

    is.clear();
    is.str("Whiff\'s Waste Dump;0\nSuddery;17\nSuddery;17\nSodor Ironworks;5");
    Edward.Add(is);
    fee = Edward.Trip("Gordon's Hill", "Sodor Ironworks").Fee(); // fee = 6
    cout << Edward.Trip("Gordon's Hill", "Sodor Ironworks");
    cout << "fi: " << fee << endl;
/*
--8<----8<----8<----8<----8<----8<----8<--
Gordon's Hill
-> Brendam Docks
-> Whiff's Waste Dump
-> Suddery
-> Suddery
-> Sodor Ironworks
--8<----8<----8<----8<----8<----8<----8<--
*/cout << "----------" << endl;

    fee = Edward.Trip("Whiff's Waste Dump", "Tidmouth").Fee(); // fee = 10
    cout << Edward.Trip("Whiff's Waste Dump", "Tidmouth");
    cout << "fi: " << fee << endl;
/*
--8<----8<----8<----8<----8<----8<----8<--
Whiff's Waste Dump
-> Suddery
-> Suddery
-> Sodor Ironworks
-> Knapford
-> Tidmouth
--8<----8<----8<----8<----8<----8<----8<--
*/cout << "----------" << endl;

    CTrain Gordon;
    is.clear();
    is.str("Knapford;20\nTidmouth;10\n\n\nSodor Steamwork;5\nGordon\'s Hill;1\nBrendam Docks;10\n");
    Gordon.Add(is);
    CTrain Emily;
    is.clear();
    is.str("Sodor Ironworks;12\nTidmouth;10\nGordon\'s Hill;1\n");
    Emily.Add(is);

    Emily = Gordon;

    /*   cout<<"...E..\n";
       Emily.Print();
       cout<<"..G...\n";
       Gordon.Print();
       cout<<".....\n";
   */
    is.clear();
    is.str("Maron;15\nSuddery;17\nMaithwaite;11\nGreat Waterton;16\n");
    Gordon.Add(is);
    is.clear();
    is.str("Whiff\'s Waste Dump;0\nGreat Waterton;16\n");
    Emily.Add(is);
    fee = Gordon.Trip("Tidmouth", "Great Waterton").Fee(); // fee = 26
    cout << Gordon.Trip("Tidmouth", "Great Waterton");
    cout << "fi: " << fee << endl;
/*
--8<----8<----8<----8<----8<----8<----8<--
Tidmouth
-> Sodor Steamwork
-> Gordon's Hill
-> Brendam Docks
-> Maron
-> Suddery
-> Maithwaite
-> Great Waterton
--8<----8<----8<----8<----8<----8<----8<--
*/cout << "----------" << endl;


    fee = Emily.Trip("Tidmouth", "Great Waterton").Fee(); // fee = 26

    cout << Emily.Trip("Tidmouth", "Great Waterton");
    cout << "fi: " << fee << endl;

/*
--8<----8<----8<----8<----8<----8<----8<--
Tidmouth
-> Sodor Steamwork
-> Gordon's Hill
-> Brendam Docks
-> Whiff's Waste Dump
-> Great Waterton
--8<----8<----8<----8<----8<----8<----8<--
*/


}
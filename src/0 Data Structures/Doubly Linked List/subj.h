#ifndef SUBJ_LIST
#define SUBJ_LIST

#include "list.h"
#include <string>
#include <cstring>
#include <limits>
#include <iomanip>

using namespace LIST;

enum ItemType {INone, IAudioCD, IMP3CD, IDataCD, IVideoCD, IDVD, IAudioBase, IVideoBase};

class Base: public Item {
protected:
    string name;
    int storageMb;
    int occupStorageMb;
    bool isAdditionalRecord;
    bool isOverwriting;

public:
    Base();
    virtual ~Base() {};
    static Base *Create(enum ItemType);

    virtual void Print(void) = 0;
    virtual void Input(void) = 0;

    virtual enum ItemType getType() const = 0;
    string getName() const { return name; }
    int getOccupStorageMb() const { return occupStorageMb; }
    int getStorageMb() const { return storageMb; }
    virtual string getCreator() const { return ""; }
    virtual int getDuration() const { return 0; }

    inline bool operator==(const Base& other) const { return this->getType() == other.getType(); }
    inline bool operator>=(int requiredSpace) const { return (this->getStorageMb() - this->getOccupStorageMb()) >= requiredSpace; }
    inline bool operator<(const Base& other) const { return this->getName() < other.getName(); }
    inline bool operator==(ItemType type) const { return this->getType() == type; }
};

class SubjList: public List {
    int compareByName(Base *, Base *);
    int compareByDuration(Base *, Base *);
    int compareByOccupiedStorage(Base *, Base *);
    void FindByType(enum ItemType);
    void FindByPerformer(const string&);
    void FindByDirector(const string&);
    void FindByFreeSpace(int);

public:
    void Print(void);
    void Sort(void);
    void Find(void);
    
    inline Base& operator[](int index) {
        if (index < 0 || index >= Count()) {
            cout << "Error: Index out of range\n";
            return *(Base*)get_tail();
        } else {
            return *(Base*)GetItem(index);
        }
    }
};

ItemType SelectType();

#endif


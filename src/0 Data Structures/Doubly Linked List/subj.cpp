#include "subj.h"

typedef struct DurationTime {
    int hour, minute, second;
} DurationTime;

// AudioBase

class AudioBase: public Base {
protected:
    string performer;
    DurationTime dTime;

public:
    AudioBase();
    ~AudioBase() override;

    void Input(void) override;
    void Print(void) override;

    DurationTime getDTime() const { return dTime; }
    string getPerformer() const { return performer; }
    ItemType getType() const override { return IAudioBase; }
    string getCreator() const override { return performer; }
    int getDuration() const override { return dTime.hour * 3600 + dTime.minute * 60 + dTime.second; }
};

AudioBase::AudioBase(): performer(""), dTime{0, 0, 0} {}
AudioBase::~AudioBase() {}

void AudioBase::Input() {
    Base::Input();
    cout << "Enter performer: ";
    cin >> performer;

    int hour, minute, second;
    while (true) {
        cout << "Enter duration (hh mm ss): ";
        if (cin >> hour >> minute >> second &&
            hour >= 0 &&
            minute >= 0 && minute <= 59 &&
            second >= 0 && second <= 59) {
            dTime.hour = hour;
            dTime.minute = minute;
            dTime.second = second;
            break;
        } else {
            cout << "Error! Wrong duration input!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

void AudioBase::Print() {
    cout << "Name: " << name << "\n";
    cout << "Storage: " << storageMb << " MB\n";
    cout << "Occupied Storage: " << occupStorageMb << " MB\n";
    cout << "Additional Record: " << (isAdditionalRecord ? "Yes" : "No") << "\n";
    cout << "Overwriting: " << (isOverwriting ? "Yes" : "No") << "\n";
    cout << "Performer: " <<   performer << "\n";
    cout << "Duration: "
         << setw(2) << setfill('0') << dTime.hour << ":"
         << setw(2) << setfill('0') << dTime.minute << ":"
         << setw(2) << setfill('0') << dTime.second << "\n";
}

// AudioCD

class AudioCD: public AudioBase {

public:
    AudioCD();
    ~AudioCD() override;

    void Print(void) override;

    ItemType getType() const override { return IAudioCD; }
};

AudioCD::AudioCD() {}
AudioCD::~AudioCD() {}

void AudioCD::Print() {
    cout << "\n__________\n";
    cout << "Audio CD\n";
    AudioBase::Print();
    cout << "____________\n";
}

// MP3CD

class MP3CD: public AudioBase {

public:
    MP3CD();
    ~MP3CD() override;

    void Print(void) override;

    ItemType getType() const override { return IMP3CD; }
};

MP3CD::MP3CD() {}
MP3CD::~MP3CD() {}

void MP3CD::Print() {
    cout << "\n__________\n";
    cout << "MP3 CD\n";
    AudioBase::Print();
    cout << "____________\n";
}

// DataCD

class DataCD: public Base {

public:
    DataCD();
    ~DataCD() override;

    void Print(void) override;
    void Input(void) override;

    ItemType getType() const override { return IDataCD; }
};

DataCD::DataCD() {}
DataCD::~DataCD() {}

void DataCD::Print() {
    cout << "\n__________\n";
    cout << "Data CD\n";
    cout << "Name: " << name << "\n";
    cout << "Storage: " << storageMb << " MB\n";
    cout << "Occupied Storage: " << occupStorageMb << " MB\n";
    cout << "Additional Record: " << (isAdditionalRecord ? "Yes" : "No") << "\n";
    cout << "Overwriting: " << (isOverwriting ? "Yes" : "No") << "\n";
    cout << "____________\n";
}

void DataCD::Input() {
    Base::Input();
}

//VideoBase

class VideoBase: public Base {
protected:
    string director;
    DurationTime dTime;

public:
    VideoBase();
    ~VideoBase() override;

    void Input(void) override;
    void Print(void) override;

    DurationTime getDTime() const { return dTime; }
    string getDirector() const { return director; }
    ItemType getType() const override { return IVideoBase; }
    string getCreator() const override { return director; }
    int getDuration() const override { return dTime.hour * 3600 + dTime.minute * 60 + dTime.second; }
};

VideoBase::VideoBase(): director(""), dTime{0, 0, 0} {}
VideoBase::~VideoBase() {}

void VideoBase::Input() {
    Base::Input();
    cout << "Enter director: ";
    cin >> director;

    int hour, minute, second;
    while (true) {
        cout << "Enter duration (hh mm ss): ";
        if (cin >> hour >> minute >> second &&
            hour >= 0 &&
            minute >= 0 && minute <= 59 &&
            second >= 0 && second <= 59) {
            dTime.hour = hour;
            dTime.minute = minute;
            dTime.second = second;
            break;
        } else {
            cout << "Error! Wrong duration input!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

void VideoBase::Print() {
    cout << "Name: " << name << "\n";
    cout << "Storage: " << storageMb << " MB\n";
    cout << "Occupied Storage: " << occupStorageMb << " MB\n";
    cout << "Additional Record: " << (isAdditionalRecord ? "Yes" : "No") << "\n";
    cout << "Overwriting: " << (isOverwriting ? "Yes" : "No") << "\n";
    cout << "Director: " << director << "\n";
    cout << "Duration: "
         << setw(2) << setfill('0') << dTime.hour << ":"
         << setw(2) << setfill('0') << dTime.minute << ":"
         << setw(2) << setfill('0') << dTime.second << "\n";
}

// VideoCD

class VideoCD: public VideoBase {

public:
    VideoCD();
    ~VideoCD() override;

    void Print(void) override;

    ItemType getType() const override { return IVideoCD; }
};

VideoCD::VideoCD() {}
VideoCD::~VideoCD() {}

void VideoCD::Print() {
    cout << "\n__________\n";
    cout << "Video CD\n";
    VideoBase::Print();
    cout << "____________\n";
}

// DVD

class DVD: public VideoBase {
protected:
    int chapterQty;

public:
    DVD();
    ~DVD() override;

    void Print(void) override;
    void Input(void) override;

    ItemType getType() const override { return IDVD; }
};

DVD::DVD(): chapterQty(0) {}
DVD::~DVD() {}

void DVD::Print() {
    cout << "\n__________\n";
    cout << "DVD\n";
    VideoBase::Print();
    cout << "Chapters: " << chapterQty << "\n";
    cout << "__________\n";
}

void DVD::Input() {
    VideoBase::Input();
    while (true) {
        cout << "Enter number of chapters: ";
        if (cin >> chapterQty && chapterQty >= 0) {
            break;
        } else {
            cout << "Error! Wrong number input!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// Base

Base::Base() : name(""), storageMb(0), occupStorageMb(0), isAdditionalRecord(false), isOverwriting(false) {}

Base *Base::Create(enum ItemType t) {
    Base *p = nullptr;

    switch(t) {
        case IAudioCD:
            p = new AudioCD();
            break;
        case IMP3CD:
            p = new MP3CD();
            break;
        case IDataCD:
            p = new DataCD();
            break;
        case IVideoCD:
            p = new VideoCD();
            break;
        case IDVD:
            p = new DVD();
            break;
    }

    return p;
}

void Base::Input() {
    cout << "Enter name: ";
    cin >> name;

    cout << "Enter storage (MB): ";
    while (!(cin >> storageMb) || storageMb < 0) {
        cout << "Error! Wrong storage input!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Enter occupied storage (MB): ";
    while (!(cin >> occupStorageMb) || occupStorageMb < 0 || occupStorageMb > storageMb) {
        cout << "Error! Wrong occupied storage input!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Additional Record (Yes/No): ";
    string additionalRecord;
    cin >> additionalRecord;
    isAdditionalRecord = (additionalRecord == "Yes" || additionalRecord == "No");

    cout << "Overwriting (Yes/No): ";
    string overwriting;
    cin >> overwriting;
    isOverwriting = (overwriting == "Yes" || overwriting == "No");
}

// SelectType

ItemType SelectType() {
    int temp;

    while (true) {
        cout << "\nSelect type:\n1. Audio CD\n2. MP3 CD\n3. Data CD\n4. Video CD\n5. DVD\n\nYour choice: ";
        if (cin >> temp && temp >= 1 && temp <= 5) {
            break;
        } else {
            cout << "Error! Wrong input!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    return static_cast<ItemType>(temp);
}

// SubjList

void SubjList::Print() {
    Item *p = get_head();
    int i = 0;

    if (!p) {
        cout << "\nList is empty!\n";
        return;
    }

    while (p) {
        cout << "\n" << i << ". ";
        ((Base *)p)->Print();
        i++;
        p = p->get_next();
    }
}

int SubjList::compareByName(Base *item1, Base *item2) {
    if (!item1 || !item2) return 0;

    return item1->getName().compare(item2->getName());
}

int SubjList::compareByDuration(Base *item1, Base *item2) {
    if (!item1 || !item2) return 0;

    return item1->getDuration() - item2->getDuration();
}

int SubjList::compareByOccupiedStorage(Base *item1, Base *item2) {
    if (!item1 || !item2) return 0;

    return item1->getOccupStorageMb() - item2->getOccupStorageMb();
}

void SubjList::Sort() {
    if (!get_head()) return;

    int criteria;
    cout << "\nSelect sorting criteria:\n1. By Name\n2. By Duration\n3. By Occupied Storage\n\nYour choice: ";
    while (!(cin >> criteria) || criteria < 1 || criteria > 3) {
        cout << "Error! Wrong input!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nSelect sorting criteria:\n1. By Name\n2. By Duration\n3. By Occupied Storage\n\nYour choice: ";
    }

    Item *current = get_head()->get_next();
    while (current != NULL) {
        Item *next = current->get_next();
        Item *prev = current->get_prev();

        switch (criteria) {
            case 1:
                while (prev != NULL && *((Base*)current) < *((Base*)prev)) {
                    prev = prev->get_prev();
                }
                break;
            case 2:
                while (prev != NULL && ((Base*)current)->getDuration() < ((Base*)prev)->getDuration()) {
                    prev = prev->get_prev();
                }
                break;
            case 3:
                while (prev != NULL && ((Base*)current)->getOccupStorageMb() < ((Base*)prev)->getOccupStorageMb()) {
                    prev = prev->get_prev();
                }
                break;
            default:
                return;
        }

        if (prev != current->get_prev()) {
            Remove(GetIndex(current));
            if (prev == NULL) {
                Insert(current, 0);
            } else {
                Insert(current, GetIndex(prev) + 1);
            }
        }

        current = next;
    }
}

void SubjList::FindByType(enum ItemType type) {
    Item *p = get_head();
    int found = 0;

    while (p) {
        if (*(Base*)p == type) {
            ((Base*)p)->Print();
            found = 1;
        }
        p = p->get_next();
    }

    if (!found) {
        cout << "No items of type " << type << " found.\n";
    }
}

void SubjList::FindByPerformer(const string &performer) {
    Item *p = get_head();
    int found = 0;

    while (p) {
        if (((Base*)p)->getCreator() == performer) {
            ((Base*)p)->Print();
            found = 1;
        }

        p = p->get_next();
    }

    if (!found) {
        cout << "No items with performer '" << performer << "' found.\n";
    }
}

void SubjList::FindByDirector(const string &director) {
    Item *p = get_head();
    int found = 0;

    while (p) {
        if (((Base*)p)->getCreator() == director) {
            ((Base*)p)->Print();
            found = 1;
        }

        p = p->get_next();
    }

    if (!found) {
        cout << "No items with director '" << director << "' found.\n";
    }
}

void SubjList::FindByFreeSpace(int requiredSpace) {
    Item *p = get_head();
    int found = 0;

    while (p) {
        if (*(Base*)p >= requiredSpace) {
            ((Base*)p)->Print();
            found = 1;
        }

        p = p->get_next();
    }

    if (!found) {
        cout << "No items with at least " << requiredSpace << " MB of free space found.\n";
    }
}

void SubjList::Find() {
    int criteria;

    cout << "\nSelect search criteria:\n1. By Type\n2. By Performer\n3. By Director\n4. By Free Space\n\nYour choice: ";
    while (!(cin >> criteria) || criteria < 1 || criteria > 4) {
        cout << "Error! Wrong input!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nSelect search criteria:\n1. By Type\n2. By Performer\n3. By Director\n4. By Free Space\n\nYour choice: ";
    }

    switch (criteria) {
        case 1: {
            int type;

            cout << "Enter type:\n1. Audio CD\n2. MP3 CD\n3. Data CD\n4. Video CD\n5. DVD\n\nYour choice: ";
            while (!(cin >> type) || type < 1 || type > 5) {
                cout << "Error! Wrong input!\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter type:\n1. Audio CD\n2. MP3 CD\n3. Data CD\n4. Video CD\n5. DVD\n\nYour choice: ";
            }
            FindByType(static_cast<ItemType>(type));
            break;
        }
        case 2: {
            string performer;

            cout << "Enter performer: ";
            cin.ignore();
            getline(cin, performer);
            FindByPerformer(performer);
            break;
        }
        case 3: {
            string director;

            cout << "Enter director: ";
            cin.ignore();
            getline(cin, director);
            FindByDirector(director);
            break;
        }
        case 4: {
            int requiredSpace;

            cout << "Enter required free space (MB): ";
            while (!(cin >> requiredSpace) || requiredSpace < 0) {
                cout << "Error! Wrong input!\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter required free space (MB): ";
            }
            FindByFreeSpace(requiredSpace);
            break;
        }
        default:
            cout << "Invalid criteria.\n";
    }
}


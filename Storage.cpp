#include "Storage.h"
#include <fstream>
#include "Path.h"
#include <iostream>
#include<string>
std::shared_ptr<Storage> Storage::m_instance = NULL;
Storage::Storage() {
    m_dirty = false;
    readFromFile();
}

bool Storage::readFromFile(void) {
    std::fstream file1, file2;
    file1.open(Path::userPath);
    if (!file1)return false;
    std::string str1;
    while (getline(file1, str1)) {
        std::string name, password, email, phone;
        if (str1.size() == 0) {
            continue;
        }
        int l = str1.size();
        std::vector<int> tem;
        for (int i = 0; i < l; i++) {
            if (str1[i] == '"')tem.push_back(i);
        }
        for (int i = tem[0] + 1; i < tem[1]; i++) {
            name.push_back(str1[i]);
        }
        for (int i = tem[2] + 1; i < tem[3]; i++) {
            password.push_back(str1[i]);
        }
        for (int i = tem[4] + 1; i < tem[5]; i++) {
            email.push_back(str1[i]);
        }
        for (int i = tem[6] + 1; i < tem[7]; i++) {
            phone.push_back(str1[i]);
        }
        m_userList.push_back(User(name, password, email, phone));
    }

    file1.close();

    file2.open(Path::meetingPath);
    if (!file2)return false;
    std::string str2;

    while (getline(file2, str2)) {
        std::string sponsor, participators, startDate, endDate, title;
        int l = str2.size();
        std::vector<int> tem2;
        for (int i = 0; i < l; i++) {
            if (str2[i] == '"')tem2.push_back(i);
        }
        for (int i = tem2[0] + 1; i < tem2[1]; i++) {
            sponsor.push_back(str2[i]);
        }
        for (int i = tem2[2] + 1; i < tem2[3]; i++) {
            participators.push_back(str2[i]);
        }
        for (int i = tem2[4] + 1; i < tem2[5]; i++) {
            startDate.push_back(str2[i]);
        }
        for (int i = tem2[6] + 1; i < tem2[7]; i++) {
            endDate.push_back(str2[i]);
        }
        for (int i = tem2[8] + 1; i < tem2[9]; i++) {
            title.push_back(str2[i]);
        }
        std::vector<std::string> pars;
        int l2 = participators.length();
        std::string person;
        for (int i = 0; i < l2; i++) {
            if (participators[i] != '&') {
                person.push_back(participators[i]);
            }
            else {
                pars.push_back(person);
                person = "";
            }
        }
        pars.push_back(person);
        Date start(startDate), end(endDate);
        m_meetingList.push_back(Meeting(sponsor, pars, start, end, title));
    }

    file2.close();
    return true;
}

bool Storage::writeToFile(void) {
    std::ofstream out(Path::userPath);
    std::ofstream out2(Path::meetingPath);
    if (!(out.is_open() && out2.is_open()))return false;
    std::list<User>::iterator iter = m_userList.begin();
    for (; iter != m_userList.end(); iter++) {
        out << '"' << (*iter).getName() << '"' << ',' << '"' << (*iter).getPassword()
            << '"' << ',' << '"' << (*iter).getEmail() << '"' << ',' << '"' << (*iter).getPhone() << '"' << "\n";
    }
    out.close();


    std::list<Meeting>::iterator iter2 = m_meetingList.begin();
    for (; iter2 != m_meetingList.end(); iter2++) {
        out2 << '"' << (*iter2).getSponsor() << '"' << ',' << '"';
        std::vector<std::string> par = (*iter2).getParticipator();
        int l = par.size();
        for (int i = 0; i < l - 1; i++) {
            out2 << par[i] << '&';
        }
        out2 << par[l - 1];

        out2 << '"' << ',' << '"' << (*iter2).getStartDate().dateToString((*iter2).getStartDate()) << '"' << ','
            << '"' << (*iter2).getEndDate().dateToString((*iter2).getEndDate()) << '"' << ','
            << '"' << (*iter2).getTitle() << '"' << "\n";
    }
    out2.close();
    return true;
}


std::shared_ptr<Storage> Storage::getInstance(void) {
    if (m_instance == NULL) {
        m_instance = std::shared_ptr<Storage>(new Storage);
    }
    return m_instance;
}

Storage::~Storage() {
    sync();
}


void Storage::createUser(const User& t_user) {
    m_userList.push_back(t_user);
    m_dirty = true;
}


std::list<User> Storage::queryUser(std::function<bool(const User&)> filter) const {
    std::list<User> result;
    std::list<User> cp(m_userList);
    std::list<User>::iterator iter = cp.begin();
    for (; iter != cp.end(); iter++) {
        if (filter(*iter)) {
            result.push_back(*iter);
        }
    }
    return result;
}


int Storage::updateUser(std::function<bool(const User&)> filter,
    std::function<void(User&)> switcher) {
    int num = 0;
    std::list<User>::iterator iter = m_userList.begin();
    for (; iter != m_userList.end(); iter++) {
        if (filter(*iter)) {
            switcher(*iter);
            num++;
        }
    }
    if (num)m_dirty = true;
    return num;
}



int Storage::deleteUser(std::function<bool(const User&)> filter) {
    int n = 0;
    std::list<User>::iterator iter = m_userList.begin();
    for (; iter != m_userList.end();) {
        if (filter(*iter)) {
            iter = m_userList.erase(iter);
            n++;

        }
        else iter++;
    }
    if (n)m_dirty = true;
    return n;
}


void Storage::createMeeting(const Meeting& t_meeting) {
    m_meetingList.push_back(t_meeting);
    m_dirty = true;
}


std::list<Meeting> Storage::queryMeeting(
    std::function<bool(const Meeting&)> filter) const {
    std::list<Meeting> result;
    std::list<Meeting> cp(m_meetingList);
    std::list<Meeting>::iterator iter = cp.begin();
    for (; iter != cp.end(); iter++) {
        if (filter(*iter)) {
            result.push_back(*iter);
        }
    }
    return result;
}


int Storage::updateMeeting(std::function<bool(const Meeting&)> filter,
    std::function<void(Meeting&)> switcher) {
    int num = 0;
    std::list<Meeting>::iterator iter = m_meetingList.begin();
    for (; iter != m_meetingList.end(); iter++) {
        if (filter(*iter)) {
            switcher(*iter);
            num++;
        }
    }
    if (num)m_dirty = true;
    return num;
}

/**
* delete meetings
* @param a lambda function as the filter
* @return the number of deleted meetings
*/
int Storage::deleteMeeting(std::function<bool(const Meeting&)> filter) {
    int n = 0;
    std::list<Meeting>::iterator iter = m_meetingList.begin();
    for (; iter != m_meetingList.end();) {
        if (filter(*iter)) {
            iter = m_meetingList.erase(iter);
            n++;
        }
        else iter++;
    }
    if (n)m_dirty = true;
    return n;
}


bool Storage::sync(void) {
    m_dirty = false;
    return writeToFile();
}
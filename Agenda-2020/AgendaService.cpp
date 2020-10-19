#include "AgendaService.h"
#include "User.h"
#include "Meeting.h"
#include "Date.h"
#include <iostream>
AgendaService::AgendaService() {
    startAgenda();
}


AgendaService::~AgendaService() {
    quitAgenda();
}


bool AgendaService::userLogIn(const std::string& userName, const std::string& password) {
    auto filter = [userName, password](const User& user) {
        if (userName == user.getName()) {
            if (password == user.getPassword()) {
                return true;
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    };

    std::list<User> tem = m_storage->queryUser(filter);
    if (tem.size() == 1) {
        return true;
    }
    else return false;
}


bool AgendaService::userRegister(const std::string& userName, const std::string& password,
    const std::string& email, const std::string& phone) {
    auto filter = [userName](const User& user) {
        if (userName == user.getName()) {
            return true;
        }
        else return false;
    };
    std::list<User> tem = m_storage->queryUser(filter);
    if (tem.size()) {
        std::cout << "ERROE: The user name has existed!" << std::endl;
        return false;
    }
    else {
        m_storage->createUser(User(userName, password, email, phone));
        std::cout << "Register successfully!" << std::endl;
        return true;
    }
}



bool AgendaService::deleteUser(const std::string& userName, const std::string& password) {
    auto filter1 = [userName, password](const User& x) {
        if (userName == x.getName() && password == x.getPassword()) {
            return true;
        }
        return false;
    };
    int n1 = m_storage->deleteUser(filter1);
    deleteAllMeetings(userName);
    auto parlist = listAllParticipateMeetings(userName);
    for (auto iter = parlist.begin(); iter != parlist.end(); iter++) {
        quitMeeting(userName, iter->getTitle());
    }
    return n1;
}


std::list<User> AgendaService::listAllUsers(void) const {
    auto filter = [](const User& user) {
        return true;
    };

    std::list<User> tem = m_storage->queryUser(filter);
    return tem;
}


bool AgendaService::createMeeting(const std::string& userName, const std::string& title,
    const std::string& startDate, const std::string& endDate,
    const std::vector<std::string>& participator) {
    Date start(startDate);
    Date end(endDate);
    if (!start.isValid(start) || !end.isValid(end) || start >= end) {
        return false;
    }
    if (participator.empty()) {
        return false;
    }

    Meeting m(userName, participator, start, end, title);
    bool have = false;
    std::list<User> tmp = listAllUsers();
    for (auto iter = tmp.begin(); iter != tmp.end(); iter++) {
        if (userName == iter->getName()) {
            have = true;
            break;
        }
    }
    if (have == false)return false;
    auto filter = [title](const Meeting& m) {
        if (m.getTitle() == title) {
            return true;
        }
        else return false;
    };
    std::list<Meeting> tmp1 = m_storage->queryMeeting(filter);
    if (tmp1.empty() == false) {
        return false;
    }

    std::list<Meeting> ms = listAllMeetings(userName);
    for (auto iter = ms.begin(); iter != ms.end(); iter++) {
        if (!(iter->getEndDate() <= start || iter->getStartDate() >= end)) {
            return false;
        }
    }
    std::vector<std::string> par;
    Meeting mt(userName, par, start, end, title);
    m_storage->createMeeting(mt);
    for (auto it = participator.begin(); it != participator.end(); it++) {
        bool suc;
        suc = addMeetingParticipator(userName, title, *it);
        if (suc) {
            par.push_back(*it);

        }
        else {
            m_storage->deleteMeeting(filter);
            return false;
        }
    }
    return true;
}


bool AgendaService::addMeetingParticipator(const std::string& userName,
    const std::string& title,
    const std::string& participator) {
    auto filter = [participator](const User& x) {
        if (x.getName() == participator) {
            return true;
        }
        return false;
    };
    std::list<User> temp = m_storage->queryUser(filter);
    if (temp.size() != 1)return false;

    auto filter1 = [&](const Meeting& x) {
        if (x.getSponsor() == userName && x.getTitle() == title) {
            std::list<Meeting> l = m_storage->queryMeeting([&](const Meeting& x2) {
                if (x2.getSponsor() == participator || x2.isParticipator(participator)) {
                    if (x.getStartDate() >= x2.getEndDate() || x.getEndDate() <= x2.getStartDate()) {
                        return false;
                    }
                    else return true;
                }
                return false;
                });
            if (l.empty()) {
                return true;
            }
            return false;
        }
        return false;
    };
    auto switcher = [participator](Meeting& x) {
        x.addParticipator(participator);
    };
    int n = m_storage->updateMeeting(filter1, switcher);
    return n;
}


bool AgendaService::removeMeetingParticipator(const std::string& userName,
    const std::string& title,
    const std::string& participator) {
    if (userName == participator)return false;
    auto filter = [userName](const User& x) {
        if (x.getName() == userName) {
            return true;
        }
        else return false;
    };
    auto filter2 = [participator](const User& x) {
        if (x.getName() == participator) {
            return true;
        }
        else return false;
    };
    std::list<User> u1 = m_storage->queryUser(filter);
    std::list<User> u2 = m_storage->queryUser(filter2);
    if (u1.empty() || u2.empty())return false;

    auto filter3 = [userName, title](const Meeting& m) {
        if (m.getSponsor() == userName && m.getTitle() == title) {
            return true;
        }
        else return false;
    };
    std::list<Meeting>  m = m_storage->queryMeeting(filter3);
    if (m.size() != 1) {
        return false;
    }


    auto iter = m.begin();
    if (!iter->isParticipator(participator)) {
        return false;
    }
    auto switcher = [participator](Meeting& m) {
        m.removeParticipator(participator);
    };
    int n = m_storage->updateMeeting(filter3, switcher);
    m_storage->deleteMeeting([](const Meeting& m) {
        return m.getParticipator().empty();
        });
    return n;
}


bool AgendaService::quitMeeting(const std::string& userName, const std::string& title) {
    auto filter = [userName, title](const Meeting& x) {
        if (x.isParticipator(userName) && x.getTitle() == title) {
            return true;
        }
        else {
            return false;
        }
    };
    auto switcher = [userName](Meeting& x) {
        x.removeParticipator(userName);
    };
    int n = m_storage->updateMeeting(filter, switcher);
    auto filter1 = [](const Meeting& x) {
        if (x.getParticipator().empty()) {
            return true;
        }
        else return false;
    };
    m_storage->deleteMeeting(filter1);
    if (n > 0) {
        return true;
    }
    else return false;
}


std::list<Meeting> AgendaService::meetingQuery(const std::string& userName,
    const std::string& title) const {
    std::list<User>  user = m_storage->queryUser([userName](const User& u) {
        if (u.getName() == userName) {
            return true;
        }
        else return false;
        });
    if (user.empty()) {
        std::list<Meeting> tem;
        return tem;
    }
    auto filter = [userName, title](const Meeting& x) {
        if (title == x.getTitle()) {
            for (int i = 0; i < x.getParticipator().size(); i++) {
                if (userName == x.getParticipator()[i]) {
                    return true;
                }
            }
            if (userName == x.getSponsor()) {
                return true;
            }
            return false;
        }
        return false;
    };
    std::list<Meeting> tem = m_storage->queryMeeting(filter);
    return tem;
}



std::list<Meeting> AgendaService::meetingQuery(const std::string& userName,
    const std::string& startDate,
    const std::string& endDate) const {
    std::list<Meeting> tem;
    Date start(startDate), end(endDate);
    if (!Date::isValid(start) || !Date::isValid(end) || end < start)
        return tem;

    std::list<User>  user = m_storage->queryUser([userName](const User& u) {
        if (u.getName() == userName) {
            return true;
        }
        else return false;
        });
    if (user.empty()) {
        return tem;
    }


    auto filter = [userName, start, end](const Meeting& x) {
        if (!(x.getStartDate() > end || x.getEndDate() < start)) {
            for (int i = 0; i < x.getParticipator().size(); i++) {
                if (userName == x.getParticipator()[i]) {
                    return true;
                }
            }
            if (userName == x.getSponsor()) {
                return true;
            }
            return false;
        }
        return false;
    };
    tem = m_storage->queryMeeting(filter);
    return tem;
}



std::list<Meeting> AgendaService::listAllMeetings(const std::string& userName) const {
    std::list<Meeting> tem = listAllSponsorMeetings(userName);
    std::list<Meeting> tem2 = listAllParticipateMeetings(userName);
    tem.insert(tem.end(), tem2.begin(), tem2.end());
    return tem;
}


std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string& userName) const {
    auto filter = [userName](const Meeting& x) {
        if (userName == x.getSponsor()) {
            return true;
        }
        return false;
    };
    std::list<Meeting> tem = m_storage->queryMeeting(filter);
    return tem;
}


std::list<Meeting> AgendaService::listAllParticipateMeetings(
    const std::string& userName) const {
    auto filter = [userName](const Meeting& x) {
        if (userName != x.getSponsor()) {
            for (int i = 0; i < x.getParticipator().size(); i++) {
                if (userName == x.getParticipator()[i]) {
                    return true;
                }
            }
        }
        return false;
    };
    std::list<Meeting> tem = m_storage->queryMeeting(filter);
    return tem;
}



bool AgendaService::deleteMeeting(const std::string& userName, const std::string& title) {
    auto filter = [userName, title](const Meeting& x) {
        if (userName == x.getSponsor() && title == x.getTitle()) {
            return true;
        }
        return false;
    };
    int delet = m_storage->deleteMeeting(filter);
    return delet;
}

bool AgendaService::deleteAllMeetings(const std::string& userName) {
    auto filter = [userName](const Meeting& x) {
        if (userName == x.getSponsor()) {
            return true;
        }
        return false;
    };
    int delet = m_storage->deleteMeeting(filter);
    return delet;
}

void AgendaService::startAgenda(void) {
    m_storage = Storage::getInstance();
}

void AgendaService::quitAgenda(void) {
    m_storage->sync();
}

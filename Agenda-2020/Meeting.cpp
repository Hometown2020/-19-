#include "Meeting.h"


Meeting::Meeting(const std::string& t_sponsor,
    const std::vector<std::string>& t_participator,
    const Date& t_startTime, const Date& t_endTime,
    const std::string& t_title) {
    setSponsor(t_sponsor);
    setParticipator(t_participator);
    setStartDate(t_startTime);
    setEndDate(t_endTime);
    setTitle(t_title);
}


Meeting::Meeting(const Meeting& t_meeting) {
    setSponsor(t_meeting.getSponsor());
    setParticipator(t_meeting.getParticipator());
    setStartDate(t_meeting.getStartDate());
    setEndDate(t_meeting.getEndDate());
    setTitle(t_meeting.getTitle());
}


std::string Meeting::getSponsor(void) const {
    return m_sponsor;
}


void Meeting::setSponsor(const std::string& t_sponsor) {
    m_sponsor = t_sponsor;
}


std::vector<std::string> Meeting::getParticipator(void) const {
    return m_participators;
}


void Meeting::setParticipator(const std::vector<std::string>& t_participators) {
    m_participators = t_participators;
}


void Meeting::addParticipator(const std::string& t_participator) {
    m_participators.push_back(t_participator);
}


void Meeting::removeParticipator(const std::string& t_participator) {
    std::vector<std::string>::iterator iter = m_participators.begin();
    for (; iter != m_participators.end(); iter++) {
        if (!(*iter).compare(t_participator)) {
            m_participators.erase(iter);
            return;
        }
    }
}


Date Meeting::getStartDate(void) const {
    return m_startDate;
}


void Meeting::setStartDate(const Date& t_startTime) {
    m_startDate = t_startTime;
}


Date Meeting::getEndDate(void) const {
    return m_endDate;
}


void Meeting::setEndDate(const Date& t_endTime) {
    m_endDate = t_endTime;
}


std::string Meeting::getTitle(void) const {
    return m_title;
}


void Meeting::setTitle(const std::string& t_title) {
    m_title = t_title;
}


bool Meeting::isParticipator(const std::string& t_username) const {
    for (int i = 0; i < m_participators.size(); i++) {
        if (!(m_participators[i].compare(t_username))) {
            return true;
        }
    }
    return false;
}
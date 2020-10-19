#include "AgendaUI.h"
#include "User.h"
#include "Meeting.h"
#include <iostream>
#include <iomanip>
using namespace std;
AgendaUI::AgendaUI()
{
    m_userName = "UNDEFINED";
    startAgenda();
}
void operateMessage(void)
{
    cout << "\n---------------------------------------------------Agenda----------------------------------------------------" << endl;
    cout << "Action :\n"
        "o    - log out Agenda\n"
        "dc   - delete Agenda account\n"
        "lu   - list all Agenda user\n"
        "cm   - create a meeting\n"
        "amp  - add meeting participator\n"
        "rmp  - remove meeting participator\n"
        "rqm  - request to quit meeting\n"
        "la   - list all meeting\n"
        "las  - list all sponsor meetings\n"
        "lap  - list all sponsor meetings\n"
        "qm   - query meeting by title\n"
        "qt   - query meeting by time interval\n"
        "dm   - delete meeting by title\n"
        "da   - delete all meetings"
        << endl;
    cout << "----------------------------------------------------------------------------------------------------------------\n"
        << endl;
}
void operateMessage2(void)
{
    cout << "---------------------------------------------------Agenda-------------------------------------------------------" << endl;
    cout << "Action :\n"
        "l    - log in Agenda by user name and password\n"
        "r    - register an Agenda account\n"
        "q    - quit Agenda"
        << endl;
    cout << "----------------------------------------------------------------------------------------------------------------" << endl;
    cout << "Agenda :~$ ";
}
void AgendaUI::OperationLoop(void)
{
    string opera ;
    while ((opera = getOperation()) != "q")
    {
        executeOperation(opera);
        operateMessage2();
    }
    quitAgenda();
    cout << "Quited successfully!" << endl;
}

/*private*/
/**
     * constructor
     */
void AgendaUI::startAgenda(void)
{
    m_agendaService.startAgenda();
    operateMessage2();
    OperationLoop();
}

/**
     * catch user's operation
     * @return the operation
     */
std::string AgendaUI::getOperation()
{
    string opera;
    cin >> opera;
    return opera;
}

/**
     * execute the operation
     * @return if the operationloop continue
     */
bool AgendaUI::executeOperation(std::string t_operation)
{
    if (t_operation == "l")
    {
        userLogIn();
        if (m_userName != "UNDEFINED")
        {
            operateMessage(); //TODO: ´ýÍêÉÆ
            cout << "Agenda@" << m_userName << " ~# ";
            string input = getOperation();
            while (input != "o")
            {
                if (input == "lu")
                {
                    listAllUsers();
                }
                else if (input == "cm")
                {
                    createMeeting();
                }
                else if (input == "amp")
                {
                    cout << "[add participator] [meeting title] [participator username]" << endl;
                    string title, name;
                    cout << "[add participator] ";
                    cin >> title >> name;
                    if (m_agendaService.addMeetingParticipator(m_userName, title, name))
                    {
                        cout << "[add participator] succeed!" << endl;
                    }
                    else
                    {
                        cout << "[add participator] error!" << endl;
                    }
                }
                else if (input == "rmp")
                {
                    cout << " [remove participator] [meeting title] [participator username]" << endl;
                    cout << "[remove participator] ";
                    string title, name;
                    cin >> title >> name;
                    if (m_agendaService.removeMeetingParticipator(m_userName, title, name))
                    {
                        cout << "[remove participator] succeed!" << endl;
                    }
                    else
                    {
                        cout << "[remove participator] error!" << endl;
                    }
                }
                else if (input == "rqm")
                {
                    cout << " [quit meeting] [meeting title]" << endl;
                    cout << "[quit meeting] ";
                    string title;
                    cin >> title;
                    if (m_agendaService.quitMeeting(m_userName, title))
                    {
                        cout << "[quit meeting] succeed!" << endl;
                    }
                    else
                    {
                        cout << "[quit meeting] error!" << endl;
                    }
                }
                else if (input == "la")
                {
                    listAllMeetings();
                }
                else if (input == "las")
                {
                    listAllSponsorMeetings();
                }
                else if (input == "lap")
                {
                    listAllParticipateMeetings();
                }
                else if (input == "qm")
                {
                    queryMeetingByTitle();
                }
                else if (input == "qt")
                {
                    queryMeetingByTimeInterval();
                }
                else if (input == "dm")
                {
                    deleteMeetingByTitle();
                }
                else if (input == "da")
                {
                    deleteAllMeetings();
                }
                else if (input == "dc")
                {
                    deleteUser();
                    m_userName = "UNDEFINED";
                    break;
                }
                else
                {
                    cout << "Illegal action!" << endl;
                }
                operateMessage();
                cout << "Agenda";
                if (m_userName != "UNDEFINED")cout << "@" << m_userName;
                cout<< " ~# ";
                input = getOperation();
            }
            userLogOut();
            cout << "logout successfully!" << endl;
        }
    }
    else if (t_operation == "r")
    {
        userRegister();
    }
    else
    {
    cout << "Illegal action!" << endl;
        return false;
    }
    return true;
}

void AgendaUI::userLogIn(void)
{
    cout << "[log in] [user name] [password]" << endl;
    cout << "[log in] ";
    string name, password;
    cin >> name >> password;
    if (m_agendaService.userLogIn(name, password))
    {
        m_userName = name;
        m_userPassword = password;
        cout << "[log in] succeed!" << endl;
    }
    else
    {
        cout << "[log in] Password error or user doesn't exist" << endl;
    }
}

void AgendaUI::userRegister(void)
{
    cout << "[register] [username] [password] [email] [phone]" << endl;
    cout << "[register] ";
    string name, password, email, phone;
    cin >> name >> password >> email >> phone;
    if (m_agendaService.userRegister(name, password, email, phone))
    {
        cout << "[register] succeed!" << endl;
    }
    else
        cout << "[register] This username has been registered!" << endl;
}

void AgendaUI::userLogOut(void)
{
    m_agendaService.quitAgenda();
}

void AgendaUI::quitAgenda(void)
{
    m_agendaService.quitAgenda();
}

void AgendaUI::deleteUser(void)
{
    if (m_agendaService.deleteUser(m_userName, m_userPassword))
    {
        cout << "[delete agenda account] succeed!" << endl;
    }
    else
        cout << "[delete agenda account] delete fail!" << endl;
}

void AgendaUI::listAllUsers(void)
{
    std::list<User> users = m_agendaService.listAllUsers();
    int n = users.size();
    if (n == 0)
    {
        std::cout << "[list all users] There is no user!" << std::endl;
    }
    else
    {
        std::cout << "\n[list all users]\n"
            << std::endl;
        std::cout << std::left << setw(20) << "name" << std::left << setw(20) << "email" << left << setw(20) << "phone" << endl;
        auto iter = users.begin();
        for (; iter != users.end(); iter++)
        {
            cout << left << setw(20) << iter->getName() << left << setw(20) << iter->getEmail() << left << setw(20) << iter->getPhone() << endl;
        }
    }
}

void AgendaUI::createMeeting(void)
{
    int num;
    cout << "[create meeting] [the number of participators]" << endl;
    cout << "[create meeting] ";
    cin >> num;
    vector<string> pars;
    string tem;
    for (int i = 0; i < num; i++)
    {
        cout << "[create meeting] [please enter the participator " << i + 1 << " ]" << endl;
        cout << "[create meeting] ";
        cin >> tem;
        pars.push_back(tem);
    }
    cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[create meeting] ";
    string title, starttime, endtime;
    cin >> title >> starttime >> endtime;
    if (m_agendaService.createMeeting(m_userName, title, starttime, endtime, pars)) {
        cout << "[create meeting] succeed!" << endl;
    }
    else cout << "[create meeting] fail!" << endl;
}

void AgendaUI::listAllMeetings(void)
{
    cout << "[list all meetings]\n"
        << endl;
    list<Meeting> meetings = m_agendaService.listAllMeetings(m_userName);
    if (meetings.size()) {
        printMeetings(meetings);
    }
    else {
        cout << "There is no meeting existied!" << endl;
    }
}

void AgendaUI::listAllSponsorMeetings(void)
{
    cout << "[list all sponsor meetings]\n"
        << endl;
    list<Meeting> meetings = m_agendaService.listAllSponsorMeetings(m_userName);
    if (meetings.size()) {
        printMeetings(meetings);
    }
    else {
        cout << "You haven't sponsor any meeting!" << endl;
    }
}

void AgendaUI::listAllParticipateMeetings(void)
{
    cout << "[list all participator meetings]\n"
        << endl;
    list<Meeting> meetings = m_agendaService.listAllParticipateMeetings(m_userName);
    if (meetings.size()) {
        printMeetings(meetings);
    }
    else {
        cout << "You haven't participate any meeting!" << endl;
    }
}

void AgendaUI::queryMeetingByTitle(void)
{
    cout << "[query meeting] [title]:" << endl;
    cout << "[query meeting] ";
    string title;
    cin >> title;
    list<Meeting> meetings = m_agendaService.meetingQuery(m_userName, title);
    if (meetings.size()) {
        printMeetings(meetings);
    }
    else {
        cout << "There is no meeting with the title entered!" << endl;
    }
}

void AgendaUI::queryMeetingByTimeInterval(void)
{
    cout << "[query meeting] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[query meeting] ";
    string startTime, endTime;
    cin >> startTime>> endTime;
    cout << "[query meeting]\n"
        << endl;
    list<Meeting> meetings = m_agendaService.meetingQuery(m_userName, startTime, endTime);
    if (meetings.size()) {
        printMeetings(meetings);
    }
    else {
        cout << "There is no meeting in the time interval!" << endl;
    }
}

void AgendaUI::deleteMeetingByTitle(void)
{
    cout << "[delete meeting] [title]" << endl;
    cout << "[delete meeting] ";
    string title;
    cin >> title;
    if (m_agendaService.deleteMeeting(m_userName, title))
    {
        cout << "[delete meeting] succeed!" << endl;
    }
    else
        cout << "[error] delete meeting fail!" << endl;
}

void AgendaUI::deleteAllMeetings(void)
{
    if (m_agendaService.deleteAllMeetings(m_userName))
        cout << "[delete all meeting] succeed!" << endl;
    else
        cout << "[error] delete meeting fail!" << endl;
}

void AgendaUI::printMeetings(std::list<Meeting> t_meetings)
{
    cout << std::left << setw(20) << "title" << std::left << setw(20) << "sponsor" << std::left << setw(20) << "start time" << std::left << setw(20) << "end time" << std::left << setw(20) << "participators" << endl;

    auto iter = t_meetings.begin();
    for (; iter != t_meetings.end(); iter++)
    {
        cout << std::left << setw(20) << iter->getTitle() << std::left << setw(20) << iter->getSponsor() << std::left << setw(20) << Date::dateToString(iter->getStartDate()) << std::left << setw(20) << Date::dateToString(iter->getEndDate());
        vector<string> par = iter->getParticipator();
        for (auto iter2 = par.begin(); iter2 != par.end(); iter2++)
        {
            if (iter2 != par.begin())
                cout << ",";
            cout << *iter2;
        }
        cout << endl;
    }
}
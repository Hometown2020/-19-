#include "User.h"

User::User(const std::string& t_userName, const std::string& t_userPassword,
    const std::string& t_userEmail, const std::string& t_userPhone) {
    setName(t_userName);
    setPassword(t_userPassword);
    setEmail(t_userEmail);
    setPhone(t_userPhone);
}


User::User(const User& t_user) {
    setName(t_user.m_name);
    setPassword(t_user.m_password);
    setEmail(t_user.m_email);
    setPhone(t_user.m_phone);
}

std::string User::getName() const {
    return m_name;
}
void User::setName(const std::string& t_name) {
    m_name = t_name;
}
std::string User::getPassword() const {
    return m_password;
}
void User::setPassword(const std::string& t_password) {
    m_password = t_password;
}
std::string User::getEmail() const {
    return m_email;
}
void User::setEmail(const std::string& t_email) {
    m_email = t_email;
}
std::string User::getPhone() const {
    return m_phone;
}
void User::setPhone(const std::string& t_phone) {
    m_phone = t_phone;
}
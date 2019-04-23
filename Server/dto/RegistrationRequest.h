//
// Created by kacper on 19.04.2019.
//

#ifndef SERVER_REGISTRATIONREQUEST_H
#define SERVER_REGISTRATIONREQUEST_H


#include <string>
#include <jsoncpp/json/value.h>
#include <ostream>
#include "../entity/enums/AccountRole.h"

class RegistrationRequest {

    std::string email;
    std::string login;
    std::string password;
    std::string name;
    std::string surname;
    AccountRole accountRole;
public:
    const std::string &getEmail() const;

    const std::string &getLogin() const;

    const std::string &getPassword() const;

    const std::string &getName() const;

    const std::string &getSurname() const;

    AccountRole getAccountRole() const;

    friend std::ostream &operator<<(std::ostream &os, const RegistrationRequest &request);

    RegistrationRequest(Json::Value);
};


#endif //SERVER_REGISTRATIONREQUEST_H
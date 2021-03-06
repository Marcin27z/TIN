//
// Created by kacper on 09.05.2019.
//

#ifndef SERVER_ACCOUNTINFOFORCLIENT_H
#define SERVER_ACCOUNTINFOFORCLIENT_H


#include <string>
#include <ostream>
#include <bsoncxx/document/view_or_value.hpp>
#include "../serialization/Serializable.h"
#include "Account.h"

class AccountInfoForClient: Serializable {

private:
    std::string name;
    std::string surname;
    std::string login;

public:
    AccountInfoForClient(const std::string &name, const std::string &surname, const std::string &login);
    AccountInfoForClient(Json::Value);
    explicit AccountInfoForClient(Account);
    AccountInfoForClient() = default;
    Json::Value getJson() override;
    bsoncxx::document::view_or_value getDocumentFormat();
    const bool operator==(const AccountInfoForClient& other) const;
    const bool operator!=(const AccountInfoForClient& other) const;

    friend std::ostream &operator<<(std::ostream &os, const AccountInfoForClient &client);

};


#endif //SERVER_ACCOUNTINFOFORCLIENT_H

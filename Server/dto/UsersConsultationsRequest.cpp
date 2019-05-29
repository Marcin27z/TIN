//
// Created by slawek on 29.05.19.
//

#include "UsersConsultationsRequest.h"
#include "UsersConsultationsResponse.h"

UsersConsultationsRequest::UsersConsultationsRequestHelper UsersConsultationsRequest::helper;

const std::string &UsersConsultationsRequest::getLogin() const{
    return login;
}

void UsersConsultationsRequest::setLogin(const std::string &login){
    this->login = login;
}


UsersConsultationsRequest::UsersConsultationsRequest(Json::Value json){
    this->login = json["login"].asString();
}


std::unique_ptr<Request> UsersConsultationsRequest::create(Json::Value value) {
    std::unique_ptr<Request> request (new UsersConsultationsRequest(value));
    return std::move(request);
}

std::unique_ptr<Serializable> UsersConsultationsRequest::execute() {
    auto daoCon = Dao::getDaoCollection("TIN", "consultation");
    auto daoUser = Dao::getDaoCollection("TIN", "account");
    Account user = daoUser->getAccountByLogin(login);
    AccountInfoForClient userInfo(user.getName(), user.getSurname(), user.getLogin());
    try{
        std::vector<ConsultationInfoForClient> consultations = daoCon->getConsultationsByUser(userInfo);
        std::unique_ptr<Serializable> response(new UsersConsultationsResponse(consultations));
        return std::move(response);
    }
    catch (std::exception e){
        std::cout<<e.what();
        std::vector<ConsultationInfoForClient> vector;
        std::unique_ptr<Serializable> response(new UsersConsultationsResponse(vector));
        return std::move(response);
    }

}
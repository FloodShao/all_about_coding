#pragma once

#include <string>
#include "Account.h"

class AccountManager
{
public:
    virtual Account findAccountForUser(const std::string& userId) = 0;
    virtual void updateAccount(const Account& account) = 0;
};


#pragma once

#include <string>
#include <memory>
#include "Account.h"
#include "AccountManager.h"

class AccountService
{
using AccountManagerPtr = std::shared_ptr<AccountManager>;
private:
    AccountManagerPtr _accountManager;
public:
    AccountService();

    void setAccountManager(AccountManagerPtr accountManager);
    void transfer(
        const std::string& senderId,
        const std::string& beneficiaryId,
        long amount);
};

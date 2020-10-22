#include <stdexcept>
#include "AccountService.h"

AccountService::AccountService()
{
    _accountManager = nullptr;
}

void AccountService::setAccountManager(
    AccountService::AccountManagerPtr accountManager)
{
    _accountManager = accountManager;
}

void AccountService::transfer(
    const std::string& senderId,
    const std::string& beneficiaryId,
    long amount)
{
    if(!_accountManager) {
        throw std::runtime_error(
            "No AccountManager specified in AccountService!");
    }

    Account sender = _accountManager->findAccountForUser(senderId);
    Account beneficiary = _accountManager->findAccountForUser(beneficiaryId);
    
    sender.debit(amount);
    beneficiary.credit(amount);
    _accountManager->updateAccount(sender);
    _accountManager->updateAccount(beneficiary);
}
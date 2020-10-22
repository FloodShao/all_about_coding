#include <map>
#include <string>
#include <stdexcept>
#include <iostream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Account.h"
#include "AccountManager.h"
#include "AccountService.h"

//MockAccountManager
class MockAccountManager : public AccountManager
{
public:
    //MOCK_METHODn, n stands for the number of parameters
    MOCK_METHOD1(findAccountForUser, Account(const std::string&));
    MOCK_METHOD1(updateAccount, void(const Account&));
};

//A facility class acts as an external DB
class AccountHelper
{
private:
    std::map<std::string, Account> _accountData;

public:
    AccountHelper(const std::map<std::string, Account>& accountData);

    void updateAccount(const Account& account);
    Account findAccountForUser(const std::string& userId);
};

//==================================================
AccountHelper::AccountHelper(const std::map<std::string, Account>& accountData)
{
    _accountData = accountData;
}

void AccountHelper::updateAccount(const Account& account)
{
    auto accountIter = _accountData.find(account.getAccountId());
    if(accountIter == _accountData.end()) {
        throw std::runtime_error(
            "No account found!");
    }
    accountIter->second = account;
}

Account AccountHelper::findAccountForUser(const std::string& userId)
{
    auto accountIter = _accountData.find(userId);
    if(accountIter == _accountData.end()) {
        _accountData[userId] = Account();
        std::cout << "Not found userId: " << userId
            << " . Creating Account for it." << std::endl;
        return _accountData[userId];
    }
    return accountIter->second;
}
//==================================================

//Test case to test AccountService
TEST(AccountService, transferTest)
{
    //meta data
    std::map<std::string, Account> accountData;
    accountData["A"] = Account("A", 3000);
    accountData["B"] = Account("B", 2000);
    AccountHelper helper(accountData);

    std::shared_ptr<MockAccountManager> pManager = std::make_shared<MockAccountManager>();

    EXPECT_CALL(*pManager, findAccountForUser(testing::_))
        .WillRepeatedly(
            testing::Invoke(&helper, &AccountHelper::findAccountForUser));

    EXPECT_CALL(*pManager, updateAccount(testing::_))
        .WillRepeatedly(
            testing::Invoke(&helper, &AccountHelper::updateAccount));

    AccountService accountService;

    accountService.setAccountManager(pManager);
    accountService.transfer("A", "B", 1005);

    EXPECT_EQ(1995, helper.findAccountForUser("A").getBalance());
    EXPECT_EQ(3005, helper.findAccountForUser("B").getBalance());
}

#pragma once

#include <string>

class Account
{
public:
    Account();
    Account(const std::string& accountId, long initialBalance);

    void debit(long amount);
    void credit(long amount);
    long getBalance() const;
    std::string getAccountId() const;

private:
    std::string _accountId;
    long _balance;
};
#include "Account.h"

Account::Account()
{
    //do nothing
}

Account::Account(const std::string& accountId, long initialBalance)
{
    _accountId = accountId;
    _balance = initialBalance;
}

void Account::debit(long amount)
{
    _balance -= amount;
}

void Account::credit(long amount)
{
    _balance += amount;
}

std::string Account::getAccountId() const
{
    return _accountId;
}

long Account::getBalance() const
{
    return _balance;
}
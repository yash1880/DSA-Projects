#include <iostream>
#include <string>
#include <vector>
using namespace std;

class BankAccount {
protected:
    string accountNumber;
    string accountHolderName;
    double balance;
public:
    BankAccount(string accNo, string name, double bal) {
        accountNumber = accNo;
        accountHolderName = name;
        balance = bal;
    }
    virtual void deposit(double amount) {
        if(amount>0){
            balance += amount;
            cout<<"Amount deposited successfully."<<endl;
        }else{
            cout<<"Invalid deposit amount."<<endl;
        }
    }
    virtual void withdraw(double amount){
        if(amount<=0){
            cout<<"Invalid withdrawal amount."<<endl;
        }else if(amount>balance){
            cout<<"ATM ERROR: Insufficient Balance!"<<endl;
            cout<<"Your Current Balance: "<<balance<<endl;
            cout<<"Transaction Declined."<<endl;
        }else{
            balance -= amount;
            cout<<"Withdrawal successful."<<endl;
        }
    }
    string getAccountNumber(){return accountNumber;}
    virtual void displayAccountInfo(){
        cout<<"Account Number: "<<accountNumber<<endl;
        cout<<"Account Holder: "<<accountHolderName<<endl;
        cout<<"Balance: "<<balance<<endl;
    }
    virtual void calculateInterest()=0;
    virtual ~BankAccount(){}
};

class SavingsAccount : public BankAccount {
    double interestRate;
public:
    SavingsAccount(string accNo,string name,double bal,double rate)
        : BankAccount(accNo,name,bal){
        interestRate = rate;
    }
    void calculateInterest() override {
        double interest = balance*interestRate/100;
        cout<<"Savings Interest: "<<interest<<endl;
    }
};

class CheckingAccount : public BankAccount {
    double overdraftLimit;
public:
    CheckingAccount(string accNo,string name,double bal,double limit)
        : BankAccount(accNo,name,bal){
        overdraftLimit = limit;
    }
    void withdraw(double amount) override {
        if(amount <= 0){
            cout << "Invalid withdrawal amount." << endl;
        }
        else if(amount > (balance + overdraftLimit)){
            cout << "ATM ERROR: Overdraft Limit Exceeded!" << endl;
            cout << "Your Current Balance: " << balance << endl;
            cout << "Overdraft Limit: " << overdraftLimit << endl;
            cout << "Transaction Declined." << endl;
        }
        else if(amount > balance){ 
            double overdraftUsed = amount - balance;
            balance = 0;
            overdraftLimit -= overdraftUsed;
            cout << "Withdrawal successful using overdraft: " << overdraftUsed << endl;
            cout << "Remaining Overdraft Limit: " << overdraftLimit << endl;
        }
        else{
            balance -= amount;
            cout << "Withdrawal successful." << endl;
        }
    }
    void calculateInterest() override {
        cout << "Checking accounts do not earn interest." << endl;
    }
};

class FixedDepositAccount : public BankAccount {
    int term;
    double interestRate;
public:
    FixedDepositAccount(string accNo,string name,double bal,int t,double rate)
        : BankAccount(accNo,name,bal){
        term = t;
        interestRate = rate;
    }
    void calculateInterest() override{
        double interest = (balance*interestRate*term)/(12*100);
        cout<<"Fixed Deposit Interest: "<<interest<<endl;
    }
    void withdraw(double amount) override{
        cout<<"Cannot withdraw before maturity of term."<<endl;
    }
    void displayAccountInfo() override{
        cout<<"Account Number: "<<accountNumber<<endl;
        cout<<"Account Holder: "<<accountHolderName<<endl;
        cout<<"Balance: "<<balance<<endl;
        cout<<"Term: "<<term<<" months"<<endl;
        cout<<"Interest Rate: "<<interestRate<<"%"<<endl;
    }
};

// function to find account
BankAccount* findAccount(vector<BankAccount*>& accounts, string accNum){
    for(auto acc : accounts){
        if(acc->getAccountNumber() == accNum)
            return acc;
    }
    return nullptr;
}

int main(){
    vector<BankAccount*> accounts;
    int choice;
    double amount;
    string accNum;

    do{
        cout<<"Banking System Menu"<<endl;
        cout<<"1. Create Account"<<endl;
        cout<<"2. Display Account Info"<<endl;
        cout<<"3. Deposit"<<endl;
        cout<<"4. Withdraw"<<endl;
        cout<<"5. Calculate Interest"<<endl;
        cout<<"6. Exit"<<endl;
        cout<<"Enter your choice: ";
        cin>>choice;

        switch(choice){
            case 1:{
                int type;
                cout << "Select Account Type:" << endl;
                cout << "1. Savings" << endl;
                cout << "2. Checking" << endl;
                cout << "3. Fixed Deposit" << endl;
                cout << "Enter your choice: ";

                cin>>type;
                string name;
                double bal, rate, limit;
                int term;
                cout<<"Enter Account Number: "; cin>>accNum;
                cout<<"Enter Account Holder Name: "; cin.ignore(); getline(cin,name);
                cout<<"Enter Initial Balance: "; cin>>bal;

                if(type==1){
                    cout<<"Enter Interest Rate (%): "; cin>>rate;
                    accounts.push_back(new SavingsAccount(accNum,name,bal,rate));
                }else if(type==2){
                    cout<<"Enter Overdraft Limit: "; cin>>limit;
                    accounts.push_back(new CheckingAccount(accNum,name,bal,limit));
                }else if(type==3){
                    cout<<"Enter Term (months): "; cin>>term;
                    cout<<"Enter Interest Rate (%): "; cin>>rate;
                    accounts.push_back(new FixedDepositAccount(accNum,name,bal,term,rate));
                }else{
                    cout<<"Invalid type!"<<endl;
                }
                cout<<"Account created successfully."<<endl;
                break;
            }
            case 2:
                cout<<"Enter Account Number: "; cin>>accNum;
                cout<<"--------------------------"<<endl;
                if(BankAccount* acc = findAccount(accounts,accNum))
                    acc->displayAccountInfo();
                else
                    cout<<"Account not found!"<<endl;
                break;
            case 3:
                cout<<"Enter Account Number: "; cin>>accNum;
                if(BankAccount* acc = findAccount(accounts,accNum)){
                    cout<<"Enter amount to deposit: "; cin>>amount;
                    acc->deposit(amount);
                }else cout<<"Account not found!"<<endl;
                break;
            case 4:
                cout<<"Enter Account Number: "; cin>>accNum;
                if(BankAccount* acc = findAccount(accounts,accNum)){
                    cout<<"Enter amount to withdraw: "; cin>>amount;
                    acc->withdraw(amount);
                }else cout<<"Account not found!"<<endl;
                break;
            case 5:
                cout<<"Enter Account Number: "; cin>>accNum;
                if(BankAccount* acc = findAccount(accounts,accNum))
                    acc->calculateInterest();
                else
                    cout<<"Account not found!"<<endl;
                break;
            case 6:
                cout<<"Thank you for using our banking system."<<endl;
                break;
            default:
                cout<<"Invalid choice. Try again."<<endl;
        }

    }while(choice != 6);

    // clean up memory
    for(auto acc : accounts)
        delete acc;

    return 0;
}

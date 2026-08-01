#include <iostream>
#include "database.hpp"
#include "customer.hpp"

using namespace std;


void runClientCLI(sqlite3* db){
    bool run = true;
    while(run){
        cout << "What will you do today!" << endl;
        cout << "1. Create account" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" << endl;
        string choice,name,email,phone,age,password;
        Customer verify;
        getline(cin>>ws,choice);
        switch (stoi(choice)){
            case 1:{
                cout<<"Enter an account name"<<endl;
                getline(cin>>ws,name);

                cout << "Enter your email"<<endl;
                getline(cin>>ws,email);
                if(!verify.validateEmail(email)){
                    cout<<"Enter a valid email"<<endl;
                    break;
                }else if(checkEmail(db,email)){
                    cout<<"Email already exists, only unique emails are allowed"<<endl;
                    break;
                }

                cout << "Enter your age" << endl;
                getline(cin>>ws,age);
                if(stoi(age)<12 || stoi(age)>150){
                    cout<<"Enter a valid age";
                    break;
                }

                cout<<"Enter your phone number"<<endl;
                getline(cin>>ws,phone);
                if(stod(phone) < 1000000000 && stod(phone) > 9999999999){
                    cout<<"Invalid Phone number, must be 10 digits only"<<endl;
                    break;
                }

                cout<<"Enter your password"<<endl;
                getline(cin>>ws,password);
                

                insertCustomer(db,name,email,stod(phone),stoi(age),password);
                cout << "Created the user" << endl;
                break;
            }


            case 2:
            break;

            case 3:{
                run = false;
                break;
            }

            default:
            cout<< "Please choose a valid option!" << endl;
            break;

        }
    }
}




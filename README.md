## Online Banking Management System
# Welcome to Our project -_-
An creative approach on how do we approach traditional banking services via using the core concepts of Operating system

Teammates:-) 1)Tushar kanda (https://github.com/Tusharkanda) 
            mail :- tusharkanda.221ai042@nitk.edu.in
            2) Priyanshu maniyar(https://github.com/priyanshu-Maniyar)
            mail:- priyanshumaniyar.221ai023@nitk.edu.in
            3) Sachin Choudhary(https://github.com/sachin9536)
            mail-sachinchoudhary.221ai034@nitk.edu.in
            

The project is a simulation of the online banking system. The server handles each client request with help of multi-threading concept. Clients' information along with account details and balance amount is stored in the database files (.txt files). Semaphores are used to synchronize user operations. There are three types of users - Administrators, Normal users, and Joint account users. File locking concept is also used to synchronize file manipulations for joint account user operations.

### Operating system concepts used:
- Socket programming
- Multi threading
- Semaphores
- File manipulations and File locking mechanism

### Functionalities available to admin:
- Adding other users
- Deleting other users
- Searching for a user
- Modifying the details of the user

### Functionalities available to normal and joint users:
- Depositing and withdrawing money
- Balance enquiry option
- Password change option
- Viewing account details


### Instructions to run the program

#### Starting the server
```
make clean
make 

./Server 9800 
```

#### Starting the client program

```
./a.out 127.0.0.1 9800
```

Once the Bank Server starts running, the bank owner (supreme admin) will log in and add other users.\
bank owner's username :point_right: Bankowner \
bank owner's password :point_right: b@nkowner


### Assumptions
- Bank server never shuts down. If it does so, the bank owner has to add the current customers. 
- Customers can't register directly. They need to have consent from administrators.



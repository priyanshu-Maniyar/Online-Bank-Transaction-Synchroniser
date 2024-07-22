#ifndef DECLARATIONS
#define DECLARATIONS

#define MAX_CHAR_LEN 12
#define MAX_USERS 100
#define NORMAL_USER 0
#define JOINT_USER 1
#define ADMIN_USER 2

#include <sqlite3.h>

extern sqlite3 *db; // Database connection

struct Person{
  char username[MAX_CHAR_LEN];
  char password[MAX_CHAR_LEN];
};

struct Data{
  int id;
  struct Person user;
  long int balance;
};

struct JointData{
  int id;
  char keyword[MAX_CHAR_LEN]; // assumed to be unique
  struct Person user1, user2;
  long int balance;
};

// semun
union semun{
  int val;
  struct semid_ds *buf;
  unsigned short int *array;
};

void* server_to_client(void* arg);
int getIdNorm(char* username);
int getIdJoint(char* keyword);
int getIdAdmin(char* username);
int loginPrompt(int client_fd, char* username, char* password, char* keyword, int* type);
void registerPrompt(int client_fd, char* username, char* password);

// admin level operations
int addNormal(char* username, char* password);
int addJoint(char* keyword, char* username1, char* password1, char* usernam2, char* password2);
int addAdmin(char* username, char* password);
int deleteNorm(int id);
int deleteJoint(int id);
int deleteAdmin(int id);
struct Data searchNorm(int id);
struct JointData searchJoint(int id);
struct Data searchAdmin(int id);
int modifyNorm(int id, char* username, char* password);
int modifyJoint(int id, char* keyword, char* username1, char* password1, char* usernam2, char* password2);
int modifyAdmin(int id, char* username, char* password);

// user level operations
long int balanceEnquiry(int id, int type);
struct Data viewDetailsNorm(int id);
struct JointData viewDetailsJoint(int id);

int passwordChangeNorm(int id, char* password);
int passwordChangeJoint(int id, char* password, char* cur_username);
int depositAmtNorm(int id, long int amt);
int depositAmtJoint(int id, long int amt);
int withdrawAmtNorm(int id, long int amt);
int withdrawAmtJoint(int id, long int amt);

#endif

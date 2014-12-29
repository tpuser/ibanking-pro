#include "admin_command_cust.h"
#include "math.h"
#include "time.h"
static int callbackLoginCustomer(void *NotUsed, int argc, char **argv, char **azColName){
	int* id = (int*)NotUsed;
    (void)azColName;        // unused

	if(argc > 0)
		*id = atoi(argv[0]);
	
	return 0;
}

void createNewCustomer(sqlite3 *db) {
	int rc;
	char *zErrMsg = 0, newLogin[30], newPassword[30], sql[200];
	printf("Enter LOGIN and PASSWORD across space>> ");
    scanf("%s %s", newLogin, newPassword);

    sprintf(sql, "INSERT INTO customers(login, passsword) VALUES('%s', '%s');", newLogin, newPassword);
	rc = sqlite3_exec(db, sql,  0, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}								
}

void deleteCustomer(sqlite3 *db) {

	char newLogin[30], sql[200], *zErrMsg = 0;
	int rc, checkLogin = -1;
	printf("Enter LOGIN for DELETE >> ");
    if (scanf("%s", newLogin) != 1)
        return;

	sprintf(sql, "SELECT customerID FROM customers WHERE (login = '%s');", newLogin);
	rc = sqlite3_exec(db, sql,  callbackLoginCustomer, (void*)&checkLogin, &zErrMsg);

	//Delete customer's accounts 
	sprintf(sql, "DELETE FROM account WHERE  (customerID = '%d');", checkLogin);
	rc = sqlite3_exec(db, sql,  0, 0, &zErrMsg);

	//Delete custumer
	sprintf(sql, "DELETE FROM customers WHERE  (login = '%s');", newLogin);
	rc = sqlite3_exec(db, sql,  0, 0, &zErrMsg);
    (void)rc;       // unused;
}

void deleteAccount(sqlite3 *db) {
	int idAccount;
    char *sql, *zErrMsg = 0;
	printf("Enter ACCOUNT ID >> ");
    if (scanf("%d", &idAccount) != 1)
        return;
    sql = calloc(200, sizeof(char));
	sprintf(sql, "DELETE FROM account WHERE  (accountID = '%d');", idAccount);
    sqlite3_exec(db, sql,  0, 0, &zErrMsg);
}

void addAccount(sqlite3 *db) {
    int rc, checkLogin, typeAccount, rnd;
    char *zErrMsg = 0, newLogin[30], *sql;
	float balance;
    sql = calloc(250, sizeof(char));
	printf("Enter LOGIN >> ");
    if (scanf("%s", newLogin) != 1)
        return;
	checkLogin = -1;
	sprintf(sql, "SELECT customerID FROM customers WHERE (login = '%s');", newLogin);
	rc = sqlite3_exec(db, sql,  callbackLoginCustomer, (void*)&checkLogin, &zErrMsg);
	if(checkLogin == -1){
		printf("Not login in database\n");
	}

	else {
		for(;;) {
			printf("\nChoose type account\n0- Checking\n1 - Saving\n2 - OverDraft\n");
            if ((scanf("%d", &typeAccount) == 1) && (typeAccount >= 0 && typeAccount <=2))
                break;
		}

		printf("Enter BALANCE new account >> ");
        if ((scanf("%f", &balance) == 1) && (balance < 0))
			balance = 0;
        srand(time(NULL));
        rnd = (int)rand()%1000000;
		if(typeAccount == 0) 
            sprintf(sql, "INSERT INTO account(accountID,balance, time,totalTransaction, accountTypeID, accountOwner) VALUES('%d','%f', '0', '0', '%d', '%d');", rnd,balance, typeAccount, checkLogin);
		else if(typeAccount == 1) 
            sprintf(sql, "INSERT INTO account(accountID,balance, time,totalTransaction,accountTypeID, accountOwner) VALUES('%d','%f', '0', '0','%d', '%d');", rnd,balance, typeAccount, checkLogin);
		else if(typeAccount == 2)
            sprintf(sql, "INSERT INTO account(accountID,balance, time, totalTransaction,accountTypeID, accountOwner) VALUES('%d','%f', '0', '0',%d', '%d');", rnd,balance, typeAccount, checkLogin);
		

		rc = sqlite3_exec(db, sql,  0, 0, &zErrMsg);
	}
    (void)rc;       // unused;
}

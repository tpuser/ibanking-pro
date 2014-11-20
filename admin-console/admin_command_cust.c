#include "admin_command_cust.h"

static int callbackLoginCustomer(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	int* id = (int*)NotUsed;

	if(argc > 0)
		*id = atoi(argv[0]);
	
	return 0;
}

void createNewCustomer(sqlite3 *db) {
	int rc;
	char *zErrMsg = 0, newLogin[30], newPassword[30], sql[200];
	printf("Enter LOGIN and PASSWORD across space>> ");
	scanf("%s %s", newLogin, newPassword);
	sprintf(sql, "INSERT INTO customers(login, password) VALUES('%s', '%s');", newLogin, newPassword);
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
	scanf("%s", newLogin);

	sprintf(sql, "SELECT customerID FROM customers WHERE (login = '%s');", newLogin);
	rc = sqlite3_exec(db, sql,  callbackLoginCustomer, (void*)&checkLogin, &zErrMsg);

	//Delete customer's accounts 
	sprintf(sql, "DELETE FROM account WHERE  (customerID = '%d');", checkLogin);
	rc = sqlite3_exec(db, sql,  0, 0, &zErrMsg);

	//Delete custumer
	sprintf(sql, "DELETE FROM customers WHERE  (login = '%s');", newLogin);
	rc = sqlite3_exec(db, sql,  0, 0, &zErrMsg);

}

void deleteAccount(sqlite3 *db) {
	int idAccount;
	int rc;

	printf("Enter ACCOUNT ID >> ");
	scanf("%d", &idAccount);
	sprintf(sql, "DELETE FROM account WHERE  (accountID = '%d');", idAccount);
	rc = sqlite3_exec(db, sql,  0, 0, &zErrMsg);
}

void addAccount(sqlite3 *db) {
	int rc, checkLogin, typeAccount;
	char *zErrMsg = 0, newLogin[30], sql[200];
	float balance;

	printf("Enter LOGIN >> ");
	scanf("%s", newLogin);
	checkLogin = -1;
	sprintf(sql, "SELECT customerID FROM customers WHERE (login = '%s');", newLogin);
	rc = sqlite3_exec(db, sql,  callbackLoginCustomer, (void*)&checkLogin, &zErrMsg);
	if(checkLogin == -1){
		printf("Not login in database\n");
	}

	else {
		for(;;) {
			printf("\nChoose type account\n0- Checking\n1 - Saving\n2 - OverDraft\n");
			scanf("%d", &typeAccount);
			if(typeAccount >= 0 && typeAccount <=2)
				break;
		}

		printf("Enter BALANCE new account >> ");
		scanf("%f", &balance);
		if(balance < 0)
			balance = 0;

		if(typeAccount == 0) 
			sprintf(sql, "INSERT INTO account(balance, totalTransaction, accountTypeID, customerID) VALUES('%f', '0', '%d', '%d');", balance, typeAccount, checkLogin);
		else if(typeAccount == 1) 
			sprintf(sql, "INSERT INTO account(balance, accountTypeID, customerID) VALUES('%f', '%d', '%d');", balance, typeAccount, checkLogin);
		else if(typeAccount == 2)
			sprintf(sql, "INSERT INTO account(balance, time, accountTypeID, customerID) VALUES('%f', '0', '%d', '%d');", balance, typeAccount, checkLogin);
		

		rc = sqlite3_exec(db, sql,  0, 0, &zErrMsg);
	}
}
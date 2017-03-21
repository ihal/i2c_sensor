/*


上手く行かなかった
http://stackoverflow.com/questions/26144212/libpqxx-how-to-reconnect-to-a-postgresql-database-after-connection-process-has
*/


#include "sys_postgres.h"
#include "sys_log.h"

using namespace std;
using namespace pqxx;
void DB::Connect() {
   try{
      C= new connection("dbname=db01 user=DBユーザー名 password=DBパスワード "
                   "hostaddr=LinuxサーバーのIPアドレス port=5432");
      if (C->is_open()) {
         cout << "Opened database successfully: " << C->dbname() << endl;
      } else {
         cerr << "Can't open database" << endl;
      }
   }
   catch (const std::exception &e){
      cerr << e.what() << std::endl;
   }
}

void DB::Reconnect() {
  try {
	if(!C->is_open()) {
		C->activate();
	}
  }
  catch(const pqxx::broken_connection &e) {
    return;
  }
  cout << "Reconncted database successfully: "<< C->dbname() << endl;
}
void DB::Insert(class LOG l) {
  if(!C->is_open()) {
	cout << "DB NOT OPEN!!!"<<endl;
	return;//開いてなければ何もしない。
  }
  /* Create SQL statement */
  string sql;
  
  sql ="INSERT INTO log1 VALUES ( "+l.Compose()+");";
  cout <<sql <<endl;
  try{
    /* Create a transactional object. */
    work W(*C);
    /* Execute SQL query */
    W.exec( sql );
    W.commit();
  }
  catch (const std::exception &e){
    cout << e.what() << std::endl;
  }
}
void DB::Disconnect() {
  C->disconnect();
  delete C;
  cout << "Closed database." << endl;
}

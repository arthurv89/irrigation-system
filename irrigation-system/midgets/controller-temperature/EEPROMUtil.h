String getDeviceId() ;

int getCycle();
void setCycle(int cycle);

String getPrefixFromEEPROM() ;


/**
   Only sets it if we don't have a deviceId yet!
*/
void writeEEPROM(int cycle) ;

void initializeEEPROM(boolean resetEEPROM) ;

void writeString(String str, int startpos) ;
String readStr(int startIndex, int len);

void writeInt(int cycle, int startpos);
int readInt(int startpos);

String createDeviceId();




void printLine(String str);
void printString(String str);
void printString(int i);
void printLine();

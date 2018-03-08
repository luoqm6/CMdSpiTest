#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <dlfcn.h>
#include <cstdlib>
#include <unistd.h>
#include "ThostFtdcMdApi.h"
#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcTraderApi.h"
#include "ThostFtdcUserApiStruct.h"
#include "CMdSpi.h"
#include "Ini.h"

using namespace std;


//configuration parameter

void readLoginFile(char* filePath);
void readSubFile(char* filePath);

// Api pointer
CThostFtdcMdApi* pMdUserApi = NULL;

// Market data spi
char gMdFrontAddr[] = "tcp://180.168.146.187:10010";//10031
TThostFtdcBrokerIDType gBrokerID ;		//code of the broker
TThostFtdcInvestorIDType gInvestorID ;
TThostFtdcPasswordType gInvestorPassword ;
char *ppInstrumentID[50];
int instrumentNum = 2;

//
bool isLogin = 0; 
bool isConnect = 0;

//request id
int iRequestID = 0;

int main(int argc,char* argv[])
{

	printf("-------------\n");

	//get parameters
	if(argc < 2)
	{
		printf("Usage: ./Md -opt1 para1 -opt2 para2 ...\n");
		printf("Use \"./Md -h\" to see the options\n");
		exit(0);
	}

	char ch;

	char* filePath;

	while((ch = getopt(argc, argv, "f:h"))!= -1){
		switch(ch){

			case 'f':
				filePath=optarg;
				printf("%s\n", filePath);
				break;

			case 'h':
				printf("Options:\n");
				printf("Usage: ./Md -opt1 para1 -opt2 para2 ...\n");
				// printf("-b: Broker ID\n");
				// printf("-i: Investor ID\n");
				// printf("-p: Investor Password\n");
				printf("-f: login config filepath\n");
				// printf("-n: Sum of instrument\n");
				printf("-h: Help to list the options\n");
				exit(0);
				break;

			default:
				printf("Usage: ./Md -opt1 para1 -opt2 para2 ...\n");
				printf("Use \"./Md -h\" to see the options\n");
				exit(0);
				break;
		}
	}

	//read config.ini file
	CIni ini;

	readLoginFile(filePath);
    
	// CMdSpi
	pMdUserApi = CThostFtdcMdApi::CreateFtdcMdApi();
	CMdSpi* pMdUserSpi = new CMdSpi();
	pMdUserApi->RegisterSpi(pMdUserSpi);
	pMdUserApi->RegisterFront(gMdFrontAddr);
	pMdUserApi->Init();
	isLogin = 0;
	isConnect = 0;

	// waiting for connect and login
	while(!isLogin||!isConnect){}
	string cmdOrder;
	while(isLogin&&isConnect){
		cout<<"Please enter an order(sub or unsub):";
		cin>>cmdOrder;
		if(cmdOrder == "sub")
		{
			cout<<"Enter SubscribeMarketData config file path:\n";
			cin>>filePath;
			cout<<"filePath="<<filePath<<endl;
			readSubFile(filePath);
			pMdUserSpi->SubscribeMarketData(ppInstrumentID,instrumentNum);
		}
		else if(cmdOrder == "unsub")
		{
			cout<<"Enter UnSubscribeMarketData config file path:\n";
			cin>>filePath;
			cout<<"filePath="<<filePath<<endl;
			readSubFile(filePath);
			pMdUserSpi->UnSubscribeMarketData(ppInstrumentID,instrumentNum);
		}
		
	}
	
	
	pMdUserApi->Join();

	//pMdUserApi->Release();
	return 0;
}
void readLoginFile(char* filePath){
	//read config.ini file
	CIni ini;

	ini.openFile(filePath,"r");
	cout<<"filePath"<<filePath<<endl;

	char* brokerId = ini.getStr("Broker","ID");
	sprintf(gBrokerID,"%s",brokerId);
	printf("gBrokerID=%s\n", gBrokerID);
	char* investorId = ini.getStr("Investor","ID");
	sprintf(gInvestorID,"%s",investorId);
	printf("gInvestorID=%s\n", gInvestorID);
	char* password = ini.getStr("Investor","Password");
	sprintf(gInvestorPassword,"%s",password);
	printf("gInvestorPassword=%s\n", gInvestorPassword);
}
void readSubFile(char* filePath){
	//read config.ini file
	CIni ini;

	ini.openFile(filePath,"r");

	instrumentNum = ini.getInt("InstrumentNum", "InstrumentNum");
	printf("instrumentNum=%d\n", instrumentNum);

	char InstrumentCode[6] = {'C','o','d','e','0','\0'};
	string sInstrumentID[instrumentNum];
	for(int i=0;i<instrumentNum;i++)
	{
		InstrumentCode[4] = (char)(i+48);
		cout<<"InstrumentCode="<<InstrumentCode<<endl;
		sInstrumentID[i] = ini.getStr("InstrumentCode", InstrumentCode);
		cout<<"sInstrumentID[i]="<<sInstrumentID[i]<<endl;
	}
	for(int i=0;i<instrumentNum;i++)
	{
		ppInstrumentID[i] = new char [20];
		strcpy(ppInstrumentID[i] ,sInstrumentID[i].data());
	}
	for(int i=0;i<instrumentNum;i++)
	{
		cout << "ppInstrumentID[" << i << "]=" << ppInstrumentID[i] << endl;
	}
}
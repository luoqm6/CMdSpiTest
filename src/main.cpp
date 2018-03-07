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
//read config.ini file and asign to the global varibles
void readConfig(char* filePath);

// Api pointer
CThostFtdcMdApi* pMdUserApi = NULL;

// Market data spi
char gMdFrontAddr[] = "tcp://180.168.146.187:10010";//10031
TThostFtdcBrokerIDType gBrokerID ;		//code of the broker
TThostFtdcInvestorIDType gInvestorID ;
TThostFtdcPasswordType gInvestorPassword ;
char *ppInstrumentID[] = {};
int instrumentNum = 2;

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
				filePath = optarg;
				printf("%s\n", filePath);
				break;

			case 'h':
				printf("Options:\n");
				printf("Usage: ./Md -opt1 para1 -opt2 para2 ...\n");
				// printf("-b: Broker ID\n");
				// printf("-i: Investor ID\n");
				// printf("-p: Investor Password\n");
				printf("-f: config filepath\n");
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

	ini.openFile(filePath,"r");

	char* brokerId = ini.getStr("Broker","ID");
	sprintf(gBrokerID,"%s",brokerId);
	printf("gBrokerID=%s\n", gBrokerID);
	char* investorId = ini.getStr("Investor","ID");
	sprintf(gInvestorID,"%s",investorId);
	printf("gInvestorID=%s\n", gInvestorID);
	char* password = ini.getStr("Investor","Password");
	sprintf(gInvestorPassword,"%s",password);
	printf("gInvestorPassword=%s\n", gInvestorPassword);

	instrumentNum = ini.getInt("InstrumentNum", "InstrumentNum");
	printf("instrumentNum=%d\n", instrumentNum);

	char InstrumentCode[5] = {'C','o','d','e','0'};
	string temp[instrumentNum];
	for(int i=0;i<instrumentNum;i++)
	{
		InstrumentCode[4] = (char)(i+48);
		temp[i] = ini.getStr("InstrumentCode", InstrumentCode);

	}
	for(int i=0;i<instrumentNum;i++)
	{
		ppInstrumentID[i] = (char*) temp[i].data();
	}
	for(int i=0;i<instrumentNum;i++)
	{
		cout << "ppInstrumentID[" << i << "]=" << ppInstrumentID[i] << endl;
	}

    
	// CMdSpi
	pMdUserApi = CThostFtdcMdApi::CreateFtdcMdApi();
	CMdSpi* pMdUserSpi = new CMdSpi();
	pMdUserApi->RegisterSpi(pMdUserSpi);
	pMdUserApi->RegisterFront(gMdFrontAddr);
	pMdUserApi->Init();

	pMdUserApi->Join();

	pMdUserApi->Release();
	return 0;
}

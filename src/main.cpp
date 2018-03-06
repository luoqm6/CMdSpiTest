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
#include "CTraderSpi.h"
#include "Ini.h"

using namespace std;


//configuration parameter

// Api pointer
CThostFtdcMdApi* pMdUserApi = NULL;

// Market data spi
char gMdFrontAddr[] = "tcp://180.168.146.187:10010";
TThostFtdcBrokerIDType gBrokerID = "9999";		//code of the broker
TThostFtdcInvestorIDType gInvestorID = "112573";
TThostFtdcPasswordType gInvestorPassword = "Luoqingming1997";
char *ppInstrumentID[] = {};
int instrumentNum = 2;

//request id
int iRequestID = 0;

int main(int argc,char* argv[])
{

	printf("-------------\n");

	if(argc < 2)
	{
		printf("Usage: ./CTP -options\n");
		printf("Use \"./CTP\" -h to see the options\n");
		exit(0);
	}
	char ch;

	char* filePath;

	while((ch = getopt(argc, argv, "b:i:p:f:n:h"))!= -1){
		switch(ch){

			case 'b':
				sprintf(gBrokerID,"%s",optarg);
				printf("%s\n", gBrokerID);
				break;

			case 'i':
				sprintf(gInvestorID,"%s",optarg);
				printf("%s\n", gInvestorID);
				break;

			case 'p':
				sprintf(gInvestorPassword,"%s",optarg);
				printf("%s\n", gInvestorPassword);
				break;

			case 'f':
				filePath = optarg;
				printf("%s\n", filePath);
				break;

			case 'n':
				instrumentNum = atoi(optarg);
				printf("%d\n", instrumentNum);
				break;

			case 'h':
				printf("Options:\n");
				printf("Usage: ./CTP -opt1 para1 -opt2 para2 ...\n");
				printf("-b: Broker ID\n");
				printf("-i: Investor ID\n");
				printf("-p: Investor Password\n");
				printf("-f: config filepath\n");
				printf("-n: Sum of instrument\n");
				printf("-h: Help to list the options\n");
				break;

			default:
				printf("Usage: ./CTP -opt1 para1 -opt2 para2 ...\n");
				printf("Use \"./CTP -h\" to see the options\n");
				exit(0);
				break;
		}
	}
    
	// CMdSpi
	// pMdUserApi = CThostFtdcMdApi::CreateFtdcMdApi();
	// CMdSpi* pMdUserSpi = new CMdSpi();
	// pMdUserApi->RegisterSpi(pMdUserSpi);
	// pMdUserApi->RegisterFront(gMdFrontAddr);
	// pMdUserApi->Init();

	// pMdUserApi->Join();

	//pMdUserApi->Release();
	return 0;
}

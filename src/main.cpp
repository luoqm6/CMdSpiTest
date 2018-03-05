#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <dlfcn.h>
#include "ThostFtdcMdApi.h"
#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcTraderApi.h"
#include "ThostFtdcUserApiStruct.h"
#include "CMdSpi.h"

using namespace std;


//configuration parameter
CThostFtdcMdApi* pUserApi;
char gMdFrontAddr[] = "tcp://180.168.146.187:10010";
TThostFtdcBrokerIDType gBrokerID = "2030";		//code of the broker
TThostFtdcInvestorIDType gInvesterID = "112573";
TThostFtdcPasswordType gInvesterPassword = "Luoqingming1997";
char *ppInstrumentID[] = {"cu1803","cu1804"};
int instrumentNum = 2;

//request id
int iRequestID = 0;

int main()
{
	// cout<<"Enter investerID: ";
	// scanf("%s",gInvesterID);
	// cout<<"Enter password: ";
	// scanf("%s",gInvesterPassword);
    
    // CMdSpi
	pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();
	CMdSpi* pUserSpi = new CMdSpi();
	pUserApi->RegisterSpi(pUserSpi);
	pUserApi->RegisterFront(gMdFrontAddr);
	pUserApi->Init();
	pUserApi->Join();
    
    

	//pUserApi->Release();
	return 0;
}
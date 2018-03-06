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
#include "CTraderSpi.h"

using namespace std;


//configuration parameter

// Api pointer
CThostFtdcMdApi* pMdUserApi = NULL;

// Market data spi
char gMdFrontAddr[] = "tcp://180.168.146.187:10010";
TThostFtdcBrokerIDType gBrokerID = "9999";		//code of the broker
TThostFtdcInvestorIDType gInvestorID = "112573";
TThostFtdcPasswordType gInvestorPassword = "Luoqingming1997";
char *ppInstrumentID[] = {"cu1805","cu1806"};
int instrumentNum = 2;

//request id
int iRequestID = 0;

int main()
{
	// cout<<"Enter investerID: ";
	// scanf("%s",gInvestorID);
	// cout<<"Enter password: ";
	// scanf("%s",gInvestorPassword);
    
    // CMdSpi

	pMdUserApi = CThostFtdcMdApi::CreateFtdcMdApi();
	CMdSpi* pMdUserSpi = new CMdSpi();
	pMdUserApi->RegisterSpi(pMdUserSpi);
	pMdUserApi->RegisterFront(gMdFrontAddr);
	pMdUserApi->Init();

    pMdUserApi->Join();

	//pMdUserApi->Release();
	return 0;
}

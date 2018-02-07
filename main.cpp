#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <dlfcn.h>
#include "CTP_API/ThostFtdcMdApi.h"
#include "CTP_API/ThostFtdcUserApiDataType.h"
#include "CTP_API/ThostFtdcTraderApi.h"
#include "CTP_API/ThostFtdcUserApiStruct.h"
#include "CMdSpi.h"

using namespace std;


//configuration parameter
char FRONT_ADDR[] = "tcp://asp-sim2-md1.financial-trading-platform.com:26213";
TThostFtdcBrokerIDType BROKER_ID = "2030";		//code of the broker
TThostFtdcInvestorIDType INVESTOR_ID = "00092";
TThostFtdcPasswordType PASSWORD = "888888";
char *ppInstrumentID[] = {"cu1205","cu1206"};
int iInstrumentID = 2;

//request id
int iRequestID = 0;

int main()
{
	// cout<<"Enter investerID: ";
	// scanf("%s",gInvesterID);
	// cout<<"Enter password: ";
	// scanf("%s",gInvesterPassword);

	CThostFtdcMdApi* pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();
	CMdSpi* pUserSpi = new CMdSpi();
	pUserApi->RegisterSpi(pUserSpi);
	pUserApi->RegisterFront(FRONT_ADDR);
	pUserApi->Init();
	pUserApi->Join();


	//pUserApi->Release();
	return 0;
}
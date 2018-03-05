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
CThostFtdcTraderApi* pTradeUserApi = NULL;

// Market data spi
char gMdFrontAddr[] = "tcp://180.168.146.187:10010";
TThostFtdcBrokerIDType gBrokerID = "2030";		//code of the broker
TThostFtdcInvestorIDType gInvestorID = "112573";
TThostFtdcPasswordType gInvestorPassword = "Luoqingming1997";
char *ppInstrumentID[] = {"cu1805","cu1806"};
int instrumentNum = 2;

// Trader spi
char gTradeFrontAddr[] = "tcp://asp-sim2-front1.financial-trading-platform.com:26205";
TThostFtdcInstrumentIDType gTraderInstrumentID = "cu1805";
TThostFtdcDirectionType gTradeDirection = THOST_FTDC_D_Sell;
TThostFtdcPriceType gLimitPrice = 22735;

//request id
int iRequestID = 0;

int main()
{
	// cout<<"Enter investerID: ";
	// scanf("%s",gInvestorID);
	// cout<<"Enter password: ";
	// scanf("%s",gInvestorPassword);
        
    // CMdSpi

	// pMdUserApi = CThostFtdcMdApi::CreateFtdcMdApi();
	// CMdSpi* pMdUserSpi = new CMdSpi();
	// pMdUserApi->RegisterSpi(pMdUserSpi);
	// pMdUserApi->RegisterFront(gMdFrontAddr);
	// pMdUserApi->Init();
	

    // TraderSpi
    pTradeUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
    CTraderSpi *pTradeUserSpi = new CTraderSpi();
    pTradeUserApi->RegisterSpi((CThostFtdcTraderSpi*) pTradeUserSpi);
    pTradeUserApi->SubscribePublicTopic(THOST_TERT_QUICK);
    pTradeUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);
    pTradeUserApi->RegisterFront(gTradeFrontAddr);
    pTradeUserApi->Init();


    // pMdUserApi->Join();
    pTradeUserApi->Join();

	//pMdUserApi->Release();
	//pTradeUserApi->Release();
	return 0;
}

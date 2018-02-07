#include <iostream>
#include <fstream>
//#include <unordered_map>
#include "CMdSpi.h"

extern CThostFtdcMdApi *g_pMdUserApi;         //
extern char gMdFrontAddr[];
extern TThostFtdcBrokerIDType gBrokerID;
extern TThostFtdcInvestorIDType gInvesterID;
extern TThostFtdcPasswordType gInvesterPassword; // 
extern char *g_pInstrumentID[];                  // 
extern int instrumentNum;                        // 
// extern std::unordered_map<std::string, TickToKlineHelper> g_KlineHash; // 

/// ctp_api callback function
// response for connected successfully
void CMdSpi::OnFrontConnected()
{
	std::cout<<"-----connected successfully-----"<<std::endl;
	// begin to login
	CThostFtdcReqUserLoginField loginReq;
}


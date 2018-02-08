#include <iostream>
#include <fstream>
#include <cstring>
//#include <unordered_map>
#include "CMdSpi.h"
#include "CTP_API/ThostFtdcMdApi.h"
#include "CTP_API/ThostFtdcUserApiStruct.h"

extern CThostFtdcMdApi *pUserApi;         //
extern char gMdFrontAddr[];
extern TThostFtdcBrokerIDType gBrokerID;
extern TThostFtdcInvestorIDType gInvesterID;
extern TThostFtdcPasswordType gInvesterPassword; // 
extern char *ppInstrumentID[];                  // 
extern int instrumentNum;                        // 
// extern std::unordered_map<std::string, TickToKlineHelper> g_KlineHash; // 


/// ctp_api callback function
// response for connected successfully
void CMdSpi::OnFrontConnected()
{
	std::cout<<"------Network connected successfully------"<<std::endl;
	// begin to login
	CThostFtdcReqUserLoginField loginReq;
	memset(&loginReq,0,sizeof(loginReq));
	strcpy(loginReq.BrokerID, gBrokerID);
	strcpy(loginReq.UserID, gInvesterID);
	strcpy(loginReq.Password, gInvesterPassword);
	static int requestID = 0;
	int rt = pUserApi -> ReqUserLogin(&loginReq,requestID);
	if(!rt)
		std::cout << ">>>>>>Sent login request successfully" << std::endl;
	else 
		std::cout << "--->>>Failed to send login request" << std::endl;
}

// imform the disconnection
void CMdSpi::OnFrontDisconnected(int nReason)
{
	std::cout << "------Network disconnected------" << std::endl;
	std::cout << "error code: " << nReason << std::endl;
}

// heartbeat warning
void CMdSpi::OnHeartBeatWarning(int nTimeLapse)
{
	std::cout << "------Network heartbeat overtime------" << std::endl;
	std::cout << "The interval from last time: " << nTimeLapse <<std::endl;
}

// login response
void CMdSpi::OnRspUserLogin(
	CThostFtdcRspUserLoginField *pRspUserLogin,
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID,
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo -> ErrorID != 0);
	if (!bResult)
	{
		std::cout << "------Account login successfully------" << std::endl;
		std::cout << "Market day: " << pRspUserLogin->TradingDay << std::endl;
		std::cout << "Login time: " << pRspUserLogin->LoginTime << std::endl;
		std::cout << "Broker: " << pRspUserLogin->BrokerID << std::endl;
		std::cout << "User: " << pRspUserLogin->UserID << std::endl;
		// begin to send subscribe quotation request
		int rt = pUserApi->SubscribeMarketData(ppInstrumentID, instrumentNum);
		if(!rt)
			std::cout << ">>>>>>Sent subscribe market request successfully" << std::endl;
		else 
			std::cout << "--->>>Failed to send subscribe quotation request" << std::endl;
	}
	else
		std::cout << "return error--->>> ErrorID = " << pRspInfo->ErrorID << std::endl << "ErrorMsg = " << pRspInfo->ErrorMsg << std::endl;
}

// logout response
void CMdSpi::OnRspUserLogout(
	CThostFtdcUserLogoutField *pUserLogout,
	CThostFtdcRspInfoField *pRspInfo, 
	int nRequestID, 
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		std::cout << "------Account logout successfully------" << std::endl;
		std::cout << "Broker: " << pUserLogout->BrokerID << std::endl;
		std::cout << "User: " << pUserLogout->UserID << std::endl;
	}
	else
		std::cout << "return error--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
}

void CMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (bResult)
		std::cout << "return error--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
}

void CMdSpi::SubscribeMarketData()
{
	int iResult = pUserApi->SubscribeMarketData(ppInstrumentID, instrumentNum);
	std::cout << "--->>> Send market data subscribe: " << ((iResult == 0) ? "successful" : "failed") << std::endl;
}

void CMdSpi::SubscribeForQuoteRsp()
{
	int iResult = pUserApi->SubscribeForQuoteRsp(ppInstrumentID, instrumentNum);
	std::cout << "--->>> Send quotation subscribe: " << ((iResult == 0) ? "successful" : "failed") << std::endl;
}

void CMdSpi::OnRspSubMarketData(
	CThostFtdcSpecificInstrumentField *pSpecificInstrument, 
	CThostFtdcRspInfoField *pRspInfo, 
	int nRequestID, 
	bool bIsLast)
{
	bool bResult = pRspInfo && (pRspInfo->ErrorID !=0 );
	if(!bResult)
	{
		std::cout << "------Subscribe quotation successfully------" << std::endl;
		std::cout << "InstrumentID: "pSpecificInstrument->InstrumentID << std::endl;
	}
}

void CMdSpi::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cerr << "OnRspSubForQuoteRsp" << std::endl;
}

void CMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << "OnRspUnSubMarketData" << std::endl;
}

void CMdSpi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << "OnRspUnSubForQuoteRsp" << std::endl;
}

void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	// cout the market data
	std::cout << "OnRtnDepthMarketData" << std::endl;
}

void CMdSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{
	std::cout << "OnRtnForQuoteRsp" << std::endl;
}

bool CMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// if ErrorID != 0, it means receiving error response
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		std::cout << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
	return bResult;
}


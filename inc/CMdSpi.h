/////////////////////////////////////////////////////////////////////////////
///@system trading message system
///@company Wizard-Quant
///@file CustomMdSpi.h
///@brief generate a class inherit class CThostFtdcMdSpi 
///	to get the markect
///@history
///20180207 created by luoqingming
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>
#include "ThostFtdcMdApi.h"
using namespace std;

struct OrderStru
{
	char *ppInstrumentID[50];
	int instrumentNum;
};

///inherit CThostFtdcMdSpi and implement the virtual functions
class CMdSpi : public CThostFtdcMdSpi
{
public:
	/// Call it when the client connected with the backstage(before login)
	virtual void OnFrontConnected();

	/// Call it when the client disconnected with the backstage. 
	/// API will auto reconnect
	/// @param nReason
	///			0x1001 network reading failure
	///			0x1002 network writing failure
	///			0x2001 receiving heartbeat failure
	///			0x2002 sending heartbeat failure
	///			0x2003 wrong message package received
	virtual void OnFrontDisconnected(int nReason);

	/// heartbeat overtime warning. Call it when wait to receive package overtime
	/// @param nTimeLapse interval of last time received message
	virtual void OnHeartBeatWarning(int nTimeLapse);

	/// response to the login requst
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	/// response to the logout reqThostmduserust
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	/// response to the error
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	/// response to the subscribed market data
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	/// response to the unsubscribed market data
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	/// response to the subscribed for quote requst
	virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	/// response to the unsubscribed for quote requst
	virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///	depth market inform
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

	/// quote inform
	virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp);
	
//private:
	void ReqUserLogin();
	void SubscribeMarketData(char* ppInstrumentID[],int instrumentNum);
	
	void UnSubscribeMarketData(char* ppInstrumentID[],int instrumentNum);
	// 
	void SubscribeForQuoteRsp(char* ppInstrumentID[],int instrumentNum);
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);

};
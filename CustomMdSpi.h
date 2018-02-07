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
#include "ctp_api/ThostFtdcMdApi.h"



///inherit CThostFtdcMdSpi and implement the virtual functions
class CustomMdSpi : public CThostFtdcMdSpi
{
public:
	///Call it when the client connected with the backstage(before login)
	void OnFrontConnected();

	///Call it when the client disconnected with the backstage. 
	///API will auto reconnect
	///@param nReason
	///			0x1001 network reading failure
	///			0x1002 network writing failure
	///			0x2001 receiving heartbeat failure
	///			0x2002 sending heartbeat failure
	///			0x2003 wrong message package received
	void OnFrontDisconnected(int nReason);

	///heartbeat overtime warning. Call it when wait to receive package overtime
	///@param nTimeLapse interval of last time received message
	void OnHeartBeatWarning(int nTimeLapse);

	/// response of the login requst
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	/// response of the logout requst
	void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	/// response of the error
	void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	/// response of the subscribed market data
	void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	/// response of the unsubscribed market data
	void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	/// response of the subscribed for quote requst
	void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	/// response of the unsubscribed for quote requst
	void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///	depth market inform
	void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

	/// quote inform
	void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp);

};
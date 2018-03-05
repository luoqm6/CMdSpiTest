#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include "CTraderSpi.h"
#include "ThostFtdcTraderApi.h"
using namespace std;

// USER_API parameter
extern CThostFtdcTraderApi* pTradeUserApi;

// configure parameter
extern char gTradeFrontAddr[];               //front address
extern TThostFtdcBrokerIDType gBrokerID;				    //broker id
extern TThostFtdcInvestorIDType gInvestorID;				//investor id
extern TThostFtdcPasswordType gInvestorPassword;			//investor password
extern TThostFtdcInstrumentIDType gTraderInstrumentID;		//instrument id
extern TThostFtdcPriceType gLimitPrice; //price  
extern TThostFtdcDirectionType gTradeDirection;//Trading direction

// request id
extern int iRequestID;

// session parameters 
TThostFtdcFrontIDType frontID; 		//front id
TThostFtdcSessionIDType	sessionID;		//session id
TThostFtdcOrderRefType	orderRef;		//order reference
TThostFtdcOrderRefType	exeOrderRef;	//execute order reference 
TThostFtdcOrderRefType	forquoteRef;	//for quote reference
TThostFtdcOrderRefType	quoteRef;		//quote reference

// 
bool IsFlowControl(int iResult)
{
	return ((iResult == -2) || (iResult == -3));
}

void CTraderSpi::OnFrontConnected()
{
	cout << "--->>> " << "OnFrontConnected" << endl;
	/// user login requst
	ReqUserLogin();
}

void CTraderSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, gBrokerID);
	strcpy(req.UserID, gInvestorID);
	strcpy(req.Password, gInvestorPassword);
	int iResult = pTradeUserApi->ReqUserLogin(&req, ++iRequestID);
	cout << "--->>> Send user login request result: " << iResult << ((iResult == 0) ? ", Succeed" : ", Failed") << endl;
}

void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "--->>> " << "OnRspUserLogin" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		// save session parameter
		frontID = pRspUserLogin->FrontID;
		sessionID = pRspUserLogin->SessionID;
		int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		iNextOrderRef++;
		sprintf(orderRef, "%d", iNextOrderRef);
		sprintf(exeOrderRef, "%d", 1);
		sprintf(forquoteRef, "%d", 1);
		sprintf(quoteRef, "%d", 1);
		///get current trading day
		cout << "--->>> TradingDay = " << pTradeUserApi->GetTradingDay() << endl;
		///Settlement infomation confirm
		ReqSettlementInfoConfirm();
	}
}

void CTraderSpi::ReqSettlementInfoConfirm()
{
	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, gBrokerID);
	strcpy(req.InvestorID, gInvestorID);
	int iResult = pTradeUserApi->ReqSettlementInfoConfirm(&req, ++iRequestID);
	cout << "--->>> ReqSettlementInfoConfirm result: " << iResult << ((iResult == 0) ? ", Succeed" : ", Failed") << endl;
}

void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "--->>> " << "OnRspSettlementInfoConfirm" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///Requst query instrument
		ReqQryInstrument();
	}
}

void CTraderSpi::ReqQryInstrument()
{
	CThostFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.InstrumentID, gTraderInstrumentID);
	while (true)
	{
		int iResult = pTradeUserApi->ReqQryInstrument(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			cout << "--->>> ReqQryInstrument result: "  << iResult << ((iResult == 0) ? ", Succeed" : ", Failed") << endl;
			break;
		}
		else
		{
			cout << "--->>> ReqQryInstrument result: "  << iResult << ", FlowControl" << endl;
			sleep(1000);
		}
	} // while
}

void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "--->>> " << "OnRspQryInstrument" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		/// requst query trading account
		ReqQryTradingAccount();
	}
}

void CTraderSpi::ReqQryTradingAccount()
{
	CThostFtdcQryTradingAccountField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, gBrokerID);
	strcpy(req.InvestorID, gInvestorID);
	while (true)
	{
		int iResult = pTradeUserApi->ReqQryTradingAccount(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			cout << "--->>> ReqQryTradingAccount result: "  << iResult << ((iResult == 0) ? ", Succeed" : ", Failed") << endl;
			break;
		}
		else
		{
			cout << "--->>> ReqQryTradingAccount result: "  << iResult << ", FlowControl" << endl;
			sleep(1000);
		}
	} // while
}

void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "--->>> " << "OnRspQryTradingAccount" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///Requst query investor position
		ReqQryInvestorPosition();
	}
}

void CTraderSpi::ReqQryInvestorPosition()
{
	CThostFtdcQryInvestorPositionField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, gBrokerID);
	strcpy(req.InvestorID, gInvestorID);
	strcpy(req.InstrumentID, gTraderInstrumentID);
	while (true)
	{
		int iResult = pTradeUserApi->ReqQryInvestorPosition(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			cout << "--->>> ReqQryInvestorPosition result: "  << iResult << ((iResult == 0) ? ", Succeed" : ", Failed") << endl;
			break;
		}
		else
		{
			cout << "--->>> ReqQryInvestorPosition result: "  << iResult << ", FlowControl" << endl;
			sleep(1000);
		}
	} // while
}

void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "--->>> " << "OnRspQryInvestorPosition" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///Order insert requst
		ReqOrderInsert();
		///Execute order insert requst
		ReqExecOrderInsert();
		///For quote insert requst
		ReqForQuoteInsert();
		///quote insert requst
		ReqQuoteInsert();
	}
}

void CTraderSpi::ReqOrderInsert()
{
	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	///Broder id
	strcpy(req.BrokerID, gBrokerID);
	///inverstor id
	strcpy(req.InvestorID, gInvestorID);
	///instrument id
	strcpy(req.InstrumentID, gTraderInstrumentID);
	///order reference
	strcpy(req.OrderRef, orderRef);
	///user id
//	TThostFtdcUserIDType	UserID;
	///order price type: limit price
	req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	///Trade direction: 
	req.Direction = gTradeDirection;
	///combine offset flag: open
	req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	///combine hedge flag
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///price
	req.LimitPrice = gLimitPrice;
	///volume: 1
	req.VolumeTotalOriginal = 1;
	///Time condition: 
	req.TimeCondition = THOST_FTDC_TC_GFD;
	///GTD date
//	TThostFtdcDateType	GTDDate;
	///Volume condition
	req.VolumeCondition = THOST_FTDC_VC_AV;
	///Minimum volume: 1
	req.MinVolume = 1;
	///contingent condition
	req.ContingentCondition = THOST_FTDC_CC_Immediately;
	///stop price
//	TThostFtdcPriceType	StopPrice;
	///Force close reason
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///is auto suspend: not
	req.IsAutoSuspend = 0;
	///business unit
//	TThostFtdcBusinessUnitType	BusinessUnit;
	///requst id
//	TThostFtdcRequestIDType	RequestID;
	///User force close flag
	req.UserForceClose = 0;

	int iResult = pTradeUserApi->ReqOrderInsert(&req, ++iRequestID);
	cout << "--->>> request order insert : " << iResult << ((iResult == 0) ? ", Succeed" : ", Failed") << endl;
}

//Ö´ÐÐÐû¸æÂ¼ÈëÇëÇó
void CTraderSpi::ReqExecOrderInsert()
{
	CThostFtdcInputExecOrderField req;
	memset(&req, 0, sizeof(req));
	///¾­¼Í¹«Ë¾´úÂë
	strcpy(req.BrokerID, gBrokerID);
	///Í¶×ÊÕß´úÂë
	strcpy(req.InvestorID, gInvestorID);
	///ºÏÔ¼´úÂë
	strcpy(req.InstrumentID, gTraderInstrumentID);
	///±¨µ¥ÒýÓÃ
	strcpy(req.ExecOrderRef, exeOrderRef);
	///ÓÃ»§´úÂë
	//	TThostFtdcUserIDType	UserID;
	///ÊýÁ¿
	req.Volume=1;
	///ÇëÇó±àºÅ
	//TThostFtdcRequestIDType	RequestID;
	///ÒµÎñµ¥Ôª
	//TThostFtdcBusinessUnitType	BusinessUnit;
	///¿ªÆ½±êÖ¾
	req.OffsetFlag=THOST_FTDC_OF_Close;//Èç¹ûÊÇÉÏÆÚËù£¬ÐèÒªÌîÆ½½ñ»òÆ½×ò
	///Í¶»úÌ×±£±êÖ¾
	req.HedgeFlag=THOST_FTDC_HF_Speculation;
	///Ö´ÐÐÀàÐÍ
	req.ActionType=THOST_FTDC_ACTP_Exec;//Èç¹û·ÅÆúÖ´ÐÐÔòÌîTHOST_FTDC_ACTP_Abandon
	///±£ÁôÍ·´çÉêÇëµÄ³Ö²Ö·½Ïò
	req.PosiDirection=THOST_FTDC_PD_Long;
	///ÆÚÈ¨ÐÐÈ¨ºóÊÇ·ñ±£ÁôÆÚ»õÍ·´çµÄ±ê¼Ç
	req.ReservePositionFlag=THOST_FTDC_EOPF_UnReserve;//ÕâÊÇÖÐ½ðËùµÄÌî·¨£¬´óÉÌËùÖ£ÉÌËùÌîTHOST_FTDC_EOPF_Reserve
	///ÆÚÈ¨ÐÐÈ¨ºóÉú³ÉµÄÍ·´çÊÇ·ñ×Ô¶¯Æ½²Ö
	req.CloseFlag=THOST_FTDC_EOCF_AutoClose;//ÕâÊÇÖÐ½ðËùµÄÌî·¨£¬´óÉÌËùÖ£ÉÌËùÌîTHOST_FTDC_EOCF_NotToClose

	int iResult = pTradeUserApi->ReqExecOrderInsert(&req, ++iRequestID);
	cout << "--->>> Ö´ÐÐÐû¸æÂ¼ÈëÇëÇó: " << iResult << ((iResult == 0) ? ", ³É¹¦" : ", Ê§°Ü") << endl;
}

//Ñ¯¼ÛÂ¼ÈëÇëÇó
void CTraderSpi::ReqForQuoteInsert()
{
	CThostFtdcInputForQuoteField req;
	memset(&req, 0, sizeof(req));
	///¾­¼Í¹«Ë¾´úÂë
	strcpy(req.BrokerID, gBrokerID);
	///Í¶×ÊÕß´úÂë
	strcpy(req.InvestorID, gInvestorID);
	///ºÏÔ¼´úÂë
	strcpy(req.InstrumentID, gTraderInstrumentID);
	///±¨µ¥ÒýÓÃ
	strcpy(req.ForQuoteRef, exeOrderRef);
	///ÓÃ»§´úÂë
	//	TThostFtdcUserIDType	UserID;

	int iResult = pTradeUserApi->ReqForQuoteInsert(&req, ++iRequestID);
	cout << "--->>> Ñ¯¼ÛÂ¼ÈëÇëÇó: " << iResult << ((iResult == 0) ? ", ³É¹¦" : ", Ê§°Ü") << endl;
}
//±¨¼ÛÂ¼ÈëÇëÇó
void CTraderSpi::ReqQuoteInsert()
{
	CThostFtdcInputQuoteField req;
	memset(&req, 0, sizeof(req));
	///¾­¼Í¹«Ë¾´úÂë
	strcpy(req.BrokerID, gBrokerID);
	///Í¶×ÊÕß´úÂë
	strcpy(req.InvestorID, gInvestorID);
	///ºÏÔ¼´úÂë
	strcpy(req.InstrumentID, gTraderInstrumentID);
	///±¨µ¥ÒýÓÃ
	strcpy(req.QuoteRef, quoteRef);
	///Âô¼Û¸ñ
	req.AskPrice=gLimitPrice;
	///Âò¼Û¸ñ
	req.BidPrice=gLimitPrice-1.0;
	///ÂôÊýÁ¿
	req.AskVolume=1;
	///ÂòÊýÁ¿
	req.BidVolume=1;
	///ÇëÇó±àºÅ
	//TThostFtdcRequestIDType	RequestID;
	///ÒµÎñµ¥Ôª
	//TThostFtdcBusinessUnitType	BusinessUnit;
	///Âô¿ªÆ½±êÖ¾
	req.AskOffsetFlag=THOST_FTDC_OF_Open;
	///Âò¿ªÆ½±êÖ¾
	req.BidOffsetFlag=THOST_FTDC_OF_Open;
	///ÂôÍ¶»úÌ×±£±êÖ¾
	req.AskHedgeFlag=THOST_FTDC_HF_Speculation;
	///ÂòÍ¶»úÌ×±£±êÖ¾
	req.BidHedgeFlag=THOST_FTDC_HF_Speculation;
	
	int iResult = pTradeUserApi->ReqQuoteInsert(&req, ++iRequestID);
	cout << "--->>> ±¨¼ÛÂ¼ÈëÇëÇó: " << iResult << ((iResult == 0) ? ", ³É¹¦" : ", Ê§°Ü") << endl;
}

void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "--->>> " << "OnRspOrderInsert" << endl;
	IsErrorRspInfo(pRspInfo);
}

void CTraderSpi::OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//Èç¹ûÖ´ÐÐÐû¸æÕýÈ·£¬Ôò²»»á½øÈë¸Ã»Øµ÷
	cout << "--->>> " << "OnRspExecOrderInsert" << endl;
	IsErrorRspInfo(pRspInfo);
}

void CTraderSpi::OnRspForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//Èç¹ûÑ¯¼ÛÕýÈ·£¬Ôò²»»á½øÈë¸Ã»Øµ÷
	cout << "--->>> " << "OnRspForQuoteInsert" << endl;
	IsErrorRspInfo(pRspInfo);
}

void CTraderSpi::OnRspQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//Èç¹û±¨¼ÛÕýÈ·£¬Ôò²»»á½øÈë¸Ã»Øµ÷
	cout << "--->>> " << "OnRspQuoteInsert" << endl;
	IsErrorRspInfo(pRspInfo);
}

void CTraderSpi::ReqOrderAction(CThostFtdcOrderField *pOrder)
{
	static bool ORDER_ACTION_SENT = false;		//ÊÇ·ñ·¢ËÍÁË±¨µ¥
	if (ORDER_ACTION_SENT)
		return;

	CThostFtdcInputOrderActionField req;
	memset(&req, 0, sizeof(req));
	///¾­¼Í¹«Ë¾´úÂë
	strcpy(req.BrokerID, pOrder->BrokerID);
	///Í¶×ÊÕß´úÂë
	strcpy(req.InvestorID, pOrder->InvestorID);
	///±¨µ¥²Ù×÷ÒýÓÃ
//	TThostFtdcOrderActionRefType	OrderActionRef;
	///±¨µ¥ÒýÓÃ
	strcpy(req.OrderRef, pOrder->OrderRef);
	///ÇëÇó±àºÅ
//	TThostFtdcRequestIDType	RequestID;
	///Ç°ÖÃ±àºÅ
	req.FrontID = frontID;
	///»á»°±àºÅ
	req.SessionID = sessionID;
	///½»Ò×Ëù´úÂë
//	TThostFtdcExchangeIDType	ExchangeID;
	///±¨µ¥±àºÅ
//	TThostFtdcOrderSysIDType	OrderSysID;
	///²Ù×÷±êÖ¾
	req.ActionFlag = THOST_FTDC_AF_Delete;
	///¼Û¸ñ
//	TThostFtdcPriceType	LimitPrice;
	///ÊýÁ¿±ä»¯
//	TThostFtdcVolumeType	VolumeChange;
	///ÓÃ»§´úÂë
//	TThostFtdcUserIDType	UserID;
	///ºÏÔ¼´úÂë
	strcpy(req.InstrumentID, pOrder->InstrumentID);

	int iResult = pTradeUserApi->ReqOrderAction(&req, ++iRequestID);
	cout << "--->>> ±¨µ¥²Ù×÷ÇëÇó: "  << iResult << ((iResult == 0) ? ", ³É¹¦" : ", Ê§°Ü") << endl;
	ORDER_ACTION_SENT = true;
}

void CTraderSpi::ReqExecOrderAction(CThostFtdcExecOrderField *pExecOrder)
{
	static bool EXECORDER_ACTION_SENT = false;		//ÊÇ·ñ·¢ËÍÁË±¨µ¥
	if (EXECORDER_ACTION_SENT)
		return;

	CThostFtdcInputExecOrderActionField req;
	memset(&req, 0, sizeof(req));

	///¾­¼Í¹«Ë¾´úÂë
	strcpy(req.BrokerID,pExecOrder->BrokerID);
	///Í¶×ÊÕß´úÂë
	strcpy(req.InvestorID,pExecOrder->InvestorID);
	///Ö´ÐÐÐû¸æ²Ù×÷ÒýÓÃ
	//TThostFtdcOrderActionRefType	ExecOrderActionRef;
	///Ö´ÐÐÐû¸æÒýÓÃ
	strcpy(req.ExecOrderRef,pExecOrder->ExecOrderRef);
	///ÇëÇó±àºÅ
	//TThostFtdcRequestIDType	RequestID;
	///Ç°ÖÃ±àºÅ
	req.FrontID=frontID;
	///»á»°±àºÅ
	req.SessionID=sessionID;
	///½»Ò×Ëù´úÂë
	//TThostFtdcExchangeIDType	ExchangeID;
	///Ö´ÐÐÐû¸æ²Ù×÷±àºÅ
	//TThostFtdcExecOrderSysIDType	ExecOrderSysID;
	///²Ù×÷±êÖ¾
	req.ActionFlag=THOST_FTDC_AF_Delete;
	///ÓÃ»§´úÂë
	//TThostFtdcUserIDType	UserID;
	///ºÏÔ¼´úÂë
	strcpy(req.InstrumentID,pExecOrder->InstrumentID);

	int iResult = pTradeUserApi->ReqExecOrderAction(&req, ++iRequestID);
	cout << "--->>> Ö´ÐÐÐû¸æ²Ù×÷ÇëÇó: "  << iResult << ((iResult == 0) ? ", ³É¹¦" : ", Ê§°Ü") << endl;
	EXECORDER_ACTION_SENT = true;
}

void CTraderSpi::ReqQuoteAction(CThostFtdcQuoteField *pQuote)
{
	static bool QUOTE_ACTION_SENT = false;		//ÊÇ·ñ·¢ËÍÁË±¨µ¥
	if (QUOTE_ACTION_SENT)
		return;

	CThostFtdcInputQuoteActionField req;
	memset(&req, 0, sizeof(req));
	///¾­¼Í¹«Ë¾´úÂë
	strcpy(req.BrokerID, pQuote->BrokerID);
	///Í¶×ÊÕß´úÂë
	strcpy(req.InvestorID, pQuote->InvestorID);
	///±¨¼Û²Ù×÷ÒýÓÃ
	//TThostFtdcOrderActionRefType	QuoteActionRef;
	///±¨¼ÛÒýÓÃ
	strcpy(req.QuoteRef,pQuote->QuoteRef);
	///ÇëÇó±àºÅ
	//TThostFtdcRequestIDType	RequestID;
	///Ç°ÖÃ±àºÅ
	req.FrontID=frontID;
	///»á»°±àºÅ
	req.SessionID=sessionID;
	///½»Ò×Ëù´úÂë
	//TThostFtdcExchangeIDType	ExchangeID;
	///±¨¼Û²Ù×÷±àºÅ
	//TThostFtdcOrderSysIDType	QuoteSysID;
	///²Ù×÷±êÖ¾
	req.ActionFlag=THOST_FTDC_AF_Delete;
	///ÓÃ»§´úÂë
	//TThostFtdcUserIDType	UserID;
	///ºÏÔ¼´úÂë
	strcpy(req.InstrumentID,pQuote->InstrumentID);

	int iResult = pTradeUserApi->ReqQuoteAction(&req, ++iRequestID);
	cout << "--->>> ±¨¼Û²Ù×÷ÇëÇó: "  << iResult << ((iResult == 0) ? ", ³É¹¦" : ", Ê§°Ü") << endl;
	QUOTE_ACTION_SENT = true;
}

void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "--->>> " << "OnRspOrderAction" << endl;
	IsErrorRspInfo(pRspInfo);
}

void CTraderSpi::OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInpuExectOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//ÕýÈ·µÄ³·µ¥²Ù×÷£¬²»»á½øÈë¸Ã»Øµ÷
	cout << "--->>> " << "OnRspExecOrderAction" << endl;
	IsErrorRspInfo(pRspInfo);
}

void CTraderSpi::OnRspQuoteAction(CThostFtdcInputQuoteActionField *pInpuQuoteAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//ÕýÈ·µÄ³·µ¥²Ù×÷£¬²»»á½øÈë¸Ã»Øµ÷
	cout << "--->>> " << "OnRspQuoteAction" << endl;
	IsErrorRspInfo(pRspInfo);
}

///Return Order
void CTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	cout << "--->>> " << "OnRtnOrder"  << endl;
	if (IsMyOrder(pOrder))
	{
		if (IsTradingOrder(pOrder))
			ReqOrderAction(pOrder);
		else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)
			cout << "--->>> Order canceled" << endl;
	}
}

//Ö´ÐÐÐû¸æÍ¨Öª
void CTraderSpi::OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder)
{
	cout << "--->>> " << "OnRtnExecOrder"  << endl;
	if (IsMyExecOrder(pExecOrder))
	{
		if (IsTradingExecOrder(pExecOrder))
			ReqExecOrderAction(pExecOrder);
		else if (pExecOrder->ExecResult == THOST_FTDC_OER_Canceled)
			cout << "--->>> Ö´ÐÐÐû¸æ³·µ¥³É¹¦" << endl;
	}
}

//Ñ¯¼ÛÍ¨Öª
void CTraderSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{
	//ÉÏÆÚËùÖÐ½ðËùÑ¯¼ÛÍ¨ÖªÍ¨¹ý¸Ã½Ó¿Ú·µ»Ø£»Ö»ÓÐ×öÊÐÉÌ¿Í»§¿ÉÒÔÊÕµ½¸ÃÍ¨Öª
	cout << "--->>> " << "OnRtnForQuoteRsp"  << endl;
}

//±¨¼ÛÍ¨Öª
void CTraderSpi::OnRtnQuote(CThostFtdcQuoteField *pQuote)
{
	cout << "--->>> " << "OnRtnQuote"  << endl;
	if (IsMyQuote(pQuote))
	{
		if (IsTradingQuote(pQuote))
			ReqQuoteAction(pQuote);
		else if (pQuote->QuoteStatus == THOST_FTDC_OST_Canceled)
			cout << "--->>> ±¨¼Û³·µ¥³É¹¦" << endl;
	}
}

///³É½»Í¨Öª
void CTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	cout << "--->>> " << "OnRtnTrade"  << endl;
}

void CTraderSpi:: OnFrontDisconnected(int nReason)
{
	cout << "--->>> " << "OnFrontDisconnected" << endl;
	cout << "--->>> Reason = " << nReason << endl;
}
		
void CTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
	cout << "--->>> " << "OnHeartBeatWarning" << endl;
	cout << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "--->>> " << "OnRspError" << endl;
	IsErrorRspInfo(pRspInfo);
}

bool CTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// Èç¹ûErrorID != 0, ËµÃ÷ÊÕµ½ÁË´íÎóµÄÏìÓ¦
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cout << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}

bool CTraderSpi::IsMyOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->FrontID == frontID) &&
			(pOrder->SessionID == sessionID) &&
			(strcmp(pOrder->OrderRef, orderRef) == 0));
}

bool CTraderSpi::IsMyExecOrder(CThostFtdcExecOrderField *pExecOrder)
{
	return ((pExecOrder->FrontID == frontID) &&
		(pExecOrder->SessionID == sessionID) &&
		(strcmp(pExecOrder->ExecOrderRef, exeOrderRef) == 0));
}

bool CTraderSpi::IsMyQuote(CThostFtdcQuoteField *pQuote)
{
	return ((pQuote->FrontID == frontID) &&
		(pQuote->SessionID == sessionID) &&
		(strcmp(pQuote->QuoteRef, quoteRef) == 0));
}

bool CTraderSpi::IsTradingOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
			(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
			(pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}

bool CTraderSpi::IsTradingExecOrder(CThostFtdcExecOrderField *pExecOrder)
{
	return (pExecOrder->ExecResult != THOST_FTDC_OER_Canceled);
}

bool CTraderSpi::IsTradingQuote(CThostFtdcQuoteField *pQuote)
{
	return (pQuote->QuoteStatus != THOST_FTDC_OST_Canceled);
}

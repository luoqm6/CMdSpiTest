#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include "TraderSpi.h"
#include "ThostFtdcTraderApi.h"
using namespace std;

// USER_API parameter
extern CThostFtdcTraderApi* pUserApi;

// configure parameter
extern char FRONT_ADDR[];               //front address
extern char BROKER_ID[];				//broker id
extern char INVESTOR_ID[];				//investor id
extern char PASSWORD[];					//investor password
extern char INSTRUMENT_ID[];			//instrument id
extern TThostFtdcPriceType LIMIT_PRICE; //price  
extern TThostFtdcDirectionType DIRECTION;//Trading direction

// request id
extern int iRequestID;

// session parameters 
TThostFtdcFrontIDType FRONT_ID; 		//front id
TThostFtdcSessionIDType	SESSION_ID;		//session id
TThostFtdcOrderRefType	ORDER_REF;		//order reference
TThostFtdcOrderRefType	EXECORDER_REF;	//execute order reference 
TThostFtdcOrderRefType	FORQUOTE_REF;	//for quote reference
TThostFtdcOrderRefType	QUOTE_REF;		//quote reference

// 
bool IsFlowControl(int iResult)
{
	return ((iResult == -2) || (iResult == -3));
}

void CTraderSpi::OnFrontConnected()
{
	cout << "--->>> " << "OnFrontConnected" << endl;
	///ÓÃ»§µÇÂ¼ÇëÇó
	ReqUserLogin();
}

void CTraderSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.UserID, INVESTOR_ID);
	strcpy(req.Password, PASSWORD);
	int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
	cout << "--->>> Send user login request: " << iResult << ((iResult == 0) ? ", Succeed" : ", Failed") << endl;
}

void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "--->>> " << "OnRspUserLogin" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		// save session parameter
		FRONT_ID = pRspUserLogin->FrontID;
		SESSION_ID = pRspUserLogin->SessionID;
		int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		iNextOrderRef++;
		sprintf(ORDER_REF, "%d", iNextOrderRef);
		sprintf(EXECORDER_REF, "%d", 1);
		sprintf(FORQUOTE_REF, "%d", 1);
		sprintf(QUOTE_REF, "%d", 1);
		///»ñÈ¡µ±Ç°½»Ò×ÈÕ
		cout << "--->>>  = " << pUserApi->GetTradingDay() << endl;
		///Í¶×ÊÕß½áËã½á¹ûÈ·ÈÏ
		ReqSettlementInfoConfirm();
	}
}

void CTraderSpi::ReqSettlementInfoConfirm()
{
	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	int iResult = pUserApi->ReqSettlementInfoConfirm(&req, ++iRequestID);
	cout << "--->>> Í¶×ÊÕß½áËã½á¹ûÈ·ÈÏ: " << iResult << ((iResult == 0) ? ", ³É¹¦" : ", Ê§°Ü") << endl;
}

void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "--->>> " << "OnRspSettlementInfoConfirm" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///ÇëÇó²éÑ¯ºÏÔ¼
		ReqQryInstrument();
	}
}

void CTraderSpi::ReqQryInstrument()
{
	CThostFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.InstrumentID, INSTRUMENT_ID);
	while (true)
	{
		int iResult = pUserApi->ReqQryInstrument(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			cout << "--->>> ÇëÇó²éÑ¯ºÏÔ¼: "  << iResult << ((iResult == 0) ? ", ³É¹¦" : ", Ê§°Ü") << endl;
			break;
		}
		else
		{
			cout << "--->>> ÇëÇó²éÑ¯ºÏÔ¼: "  << iResult << ", ÊÜµ½Á÷¿Ø" << endl;
			sleep(1000);
		}
	} // while
}

void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "--->>> " << "OnRspQryInstrument" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///ÇëÇó²éÑ¯ºÏÔ¼
		ReqQryTradingAccount();
	}
}

void CTraderSpi::ReqQryTradingAccount()
{
	CThostFtdcQryTradingAccountField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	while (true)
	{
		int iResult = pUserApi->ReqQryTradingAccount(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			cout << "--->>> ÇëÇó²éÑ¯×Ê½ðÕË»§: "  << iResult << ((iResult == 0) ? ", ³É¹¦" : ", Ê§°Ü") << endl;
			break;
		}
		else
		{
			cout << "--->>> ÇëÇó²éÑ¯×Ê½ðÕË»§: "  << iResult << ", ÊÜµ½Á÷¿Ø" << endl;
			sleep(1000);
		}
	} // while
}

void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "--->>> " << "OnRspQryTradingAccount" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///ÇëÇó²éÑ¯Í¶×ÊÕß³Ö²Ö
		ReqQryInvestorPosition();
	}
}

void CTraderSpi::ReqQryInvestorPosition()
{
	CThostFtdcQryInvestorPositionField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	strcpy(req.InstrumentID, INSTRUMENT_ID);
	while (true)
	{
		int iResult = pUserApi->ReqQryInvestorPosition(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			cout << "--->>> ÇëÇó²éÑ¯Í¶×ÊÕß³Ö²Ö: "  << iResult << ((iResult == 0) ? ", ³É¹¦" : ", Ê§°Ü") << endl;
			break;
		}
		else
		{
			cout << "--->>> ÇëÇó²éÑ¯Í¶×ÊÕß³Ö²Ö: "  << iResult << ", ÊÜµ½Á÷¿Ø" << endl;
			sleep(1000);
		}
	} // while
}

void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "--->>> " << "OnRspQryInvestorPosition" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///±¨µ¥Â¼ÈëÇëÇó
		ReqOrderInsert();
		//Ö´ÐÐÐû¸æÂ¼ÈëÇëÇó
		ReqExecOrderInsert();
		//Ñ¯¼ÛÂ¼Èë
		ReqForQuoteInsert();
		//×öÊÐÉÌ±¨¼ÛÂ¼Èë
		ReqQuoteInsert();
	}
}

void CTraderSpi::ReqOrderInsert()
{
	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	///¾­¼Í¹«Ë¾´úÂë
	strcpy(req.BrokerID, BROKER_ID);
	///Í¶×ÊÕß´úÂë
	strcpy(req.InvestorID, INVESTOR_ID);
	///ºÏÔ¼´úÂë
	strcpy(req.InstrumentID, INSTRUMENT_ID);
	///±¨µ¥ÒýÓÃ
	strcpy(req.OrderRef, ORDER_REF);
	///ÓÃ»§´úÂë
//	TThostFtdcUserIDType	UserID;
	///±¨µ¥¼Û¸ñÌõ¼þ: ÏÞ¼Û
	req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	///ÂòÂô·½Ïò: 
	req.Direction = DIRECTION;
	///×éºÏ¿ªÆ½±êÖ¾: ¿ª²Ö
	req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	///×éºÏÍ¶»úÌ×±£±êÖ¾
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///¼Û¸ñ
	req.LimitPrice = LIMIT_PRICE;
	///ÊýÁ¿: 1
	req.VolumeTotalOriginal = 1;
	///ÓÐÐ§ÆÚÀàÐÍ: µ±ÈÕÓÐÐ§
	req.TimeCondition = THOST_FTDC_TC_GFD;
	///GTDÈÕÆÚ
//	TThostFtdcDateType	GTDDate;
	///³É½»Á¿ÀàÐÍ: ÈÎºÎÊýÁ¿
	req.VolumeCondition = THOST_FTDC_VC_AV;
	///×îÐ¡³É½»Á¿: 1
	req.MinVolume = 1;
	///´¥·¢Ìõ¼þ: Á¢¼´
	req.ContingentCondition = THOST_FTDC_CC_Immediately;
	///Ö¹Ëð¼Û
//	TThostFtdcPriceType	StopPrice;
	///Ç¿Æ½Ô­Òò: ·ÇÇ¿Æ½
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///×Ô¶¯¹ÒÆð±êÖ¾: ·ñ
	req.IsAutoSuspend = 0;
	///ÒµÎñµ¥Ôª
//	TThostFtdcBusinessUnitType	BusinessUnit;
	///ÇëÇó±àºÅ
//	TThostFtdcRequestIDType	RequestID;
	///ÓÃ»§Ç¿ÆÀ±êÖ¾: ·ñ
	req.UserForceClose = 0;

	int iResult = pUserApi->ReqOrderInsert(&req, ++iRequestID);
	cout << "--->>> ±¨µ¥Â¼ÈëÇëÇó: " << iResult << ((iResult == 0) ? ", ³É¹¦" : ", Ê§°Ü") << endl;
}

//Ö´ÐÐÐû¸æÂ¼ÈëÇëÇó
void CTraderSpi::ReqExecOrderInsert()
{
	CThostFtdcInputExecOrderField req;
	memset(&req, 0, sizeof(req));
	///¾­¼Í¹«Ë¾´úÂë
	strcpy(req.BrokerID, BROKER_ID);
	///Í¶×ÊÕß´úÂë
	strcpy(req.InvestorID, INVESTOR_ID);
	///ºÏÔ¼´úÂë
	strcpy(req.InstrumentID, INSTRUMENT_ID);
	///±¨µ¥ÒýÓÃ
	strcpy(req.ExecOrderRef, EXECORDER_REF);
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

	int iResult = pUserApi->ReqExecOrderInsert(&req, ++iRequestID);
	cout << "--->>> Ö´ÐÐÐû¸æÂ¼ÈëÇëÇó: " << iResult << ((iResult == 0) ? ", ³É¹¦" : ", Ê§°Ü") << endl;
}

//Ñ¯¼ÛÂ¼ÈëÇëÇó
void CTraderSpi::ReqForQuoteInsert()
{
	CThostFtdcInputForQuoteField req;
	memset(&req, 0, sizeof(req));
	///¾­¼Í¹«Ë¾´úÂë
	strcpy(req.BrokerID, BROKER_ID);
	///Í¶×ÊÕß´úÂë
	strcpy(req.InvestorID, INVESTOR_ID);
	///ºÏÔ¼´úÂë
	strcpy(req.InstrumentID, INSTRUMENT_ID);
	///±¨µ¥ÒýÓÃ
	strcpy(req.ForQuoteRef, EXECORDER_REF);
	///ÓÃ»§´úÂë
	//	TThostFtdcUserIDType	UserID;

	int iResult = pUserApi->ReqForQuoteInsert(&req, ++iRequestID);
	cout << "--->>> Ñ¯¼ÛÂ¼ÈëÇëÇó: " << iResult << ((iResult == 0) ? ", ³É¹¦" : ", Ê§°Ü") << endl;
}
//±¨¼ÛÂ¼ÈëÇëÇó
void CTraderSpi::ReqQuoteInsert()
{
	CThostFtdcInputQuoteField req;
	memset(&req, 0, sizeof(req));
	///¾­¼Í¹«Ë¾´úÂë
	strcpy(req.BrokerID, BROKER_ID);
	///Í¶×ÊÕß´úÂë
	strcpy(req.InvestorID, INVESTOR_ID);
	///ºÏÔ¼´úÂë
	strcpy(req.InstrumentID, INSTRUMENT_ID);
	///±¨µ¥ÒýÓÃ
	strcpy(req.QuoteRef, QUOTE_REF);
	///Âô¼Û¸ñ
	req.AskPrice=LIMIT_PRICE;
	///Âò¼Û¸ñ
	req.BidPrice=LIMIT_PRICE-1.0;
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
	
	int iResult = pUserApi->ReqQuoteInsert(&req, ++iRequestID);
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
	req.FrontID = FRONT_ID;
	///»á»°±àºÅ
	req.SessionID = SESSION_ID;
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

	int iResult = pUserApi->ReqOrderAction(&req, ++iRequestID);
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
	req.FrontID=FRONT_ID;
	///»á»°±àºÅ
	req.SessionID=SESSION_ID;
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

	int iResult = pUserApi->ReqExecOrderAction(&req, ++iRequestID);
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
	req.FrontID=FRONT_ID;
	///»á»°±àºÅ
	req.SessionID=SESSION_ID;
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

	int iResult = pUserApi->ReqQuoteAction(&req, ++iRequestID);
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

///±¨µ¥Í¨Öª
void CTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	cout << "--->>> " << "OnRtnOrder"  << endl;
	if (IsMyOrder(pOrder))
	{
		if (IsTradingOrder(pOrder))
			ReqOrderAction(pOrder);
		else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)
			cout << "--->>> ³·µ¥³É¹¦" << endl;
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
	return ((pOrder->FrontID == FRONT_ID) &&
			(pOrder->SessionID == SESSION_ID) &&
			(strcmp(pOrder->OrderRef, ORDER_REF) == 0));
}

bool CTraderSpi::IsMyExecOrder(CThostFtdcExecOrderField *pExecOrder)
{
	return ((pExecOrder->FrontID == FRONT_ID) &&
		(pExecOrder->SessionID == SESSION_ID) &&
		(strcmp(pExecOrder->ExecOrderRef, EXECORDER_REF) == 0));
}

bool CTraderSpi::IsMyQuote(CThostFtdcQuoteField *pQuote)
{
	return ((pQuote->FrontID == FRONT_ID) &&
		(pQuote->SessionID == SESSION_ID) &&
		(strcmp(pQuote->QuoteRef, QUOTE_REF) == 0));
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

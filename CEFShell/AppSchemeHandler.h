/*
* AppSchemeHandler.h
*
*  Created on: Jan 10, 2015
*      Author: steve
*/

#ifndef APPSCHEMEHANDLER_H_
#define APPSCHEMEHANDLER_H_


#include "include/cef_scheme.h"



namespace CEFShell
{

	class AppSchemeHandlerFactory : public CefSchemeHandlerFactory
	{
	public:


		AppSchemeHandlerFactory()
		{}

		virtual ~AppSchemeHandlerFactory()
		{}


		CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser>		browser,
			CefRefPtr<CefFrame> 		frame,
			const CefString&			schemeName,
			CefRefPtr<CefRequest>		request) override;


	private:

		IMPLEMENT_REFCOUNTING(AppSchemeHandlerFactory);
	};
}

#endif /* APPSCHEMEHANDLER_H_ */

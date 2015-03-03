/*
* AppSchemeHandler.cpp
*
*  Created on: Jan 10, 2015
*      Author: steve
*/


#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <memory>

#include <boost/filesystem.hpp>

#include <include/wrapper/cef_stream_resource_handler.h>

#include "AppSchemeHandler.h"




namespace CEFShell
{

	static const boost::filesystem::path			g_applicationPath = boost::filesystem::initial_path();


	CefRefPtr<CefResourceHandler> 	AppSchemeHandlerFactory::Create( CefRefPtr<CefBrowser>		browser,
																	 CefRefPtr<CefFrame> 		frame,
																	 const CefString&			schemeName,
																	 CefRefPtr<CefRequest>		request)
	{
		//	Get the request URL and build the filename from that

		std::wstring			m_URL = request->GetURL();

//		std::wstring			filename = std::wstring( g_applicationPath.c_str() ) + std::wstring(L"/war/") + m_URL.substr(6);

		std::wstring			filename = std::wstring(L"D:\\Users\\Steve\\WebstormProjects\\PolymerTest\\index.html");

		CefRefPtr<CefStreamReader> stream = CefStreamReader::CreateForFile(filename);

		//	If the stream is NULL, then we could not load the file.  Therefore, abort the load.

		if (stream.get() == NULL)
		{
			browser->StopLoad();
			return(NULL);
		}

		// Constructor for HTTP status code 200 and no custom response headers.
		// There’s also a version of the constructor for custom status code and response headers.

		return new CefStreamResourceHandler("text/html", stream);
	}

}		//	Namespace CEFShell




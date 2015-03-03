// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "ShellApp.h"

#include <string>

#include "ShellAppHandler.h"
#include "AppSchemeHandler.h"

#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/wrapper/cef_helpers.h"



ShellApp::ShellApp()
{}

void ShellApp::OnContextInitialized()
{
	CEF_REQUIRE_UI_THREAD();

	//	Register our custom scheme handler for local apps served from the filesystem

	if( !CefRegisterSchemeHandlerFactory( "app", "", new CEFShell::AppSchemeHandlerFactory()) )
	{
		DLOG(FATAL) << "Unable to register custom scheme handler 'app' ";
	}

	// Information used when creating the native window.
	
	CefWindowInfo				windowInfo;

	#if defined(OS_WIN)
		// On Windows we need to specify certain flags that will be passed to
		// CreateWindowEx().

		windowInfo.SetAsPopup( NULL, "CEFShell" );
	#endif

	// SimpleHandler implements browser-level callbacks.

	CefRefPtr<ShellAppHandler>	handler( new ShellAppHandler() );

	// Specify CEF browser settings here.

	CefBrowserSettings			browserSettings;

	std::string					url;

	// Check if a "--url=" value was provided via the command-line. If so, use
	// that instead of the default URL.

	CefRefPtr<CefCommandLine> command_line = CefCommandLine::GetGlobalCommandLine();
	url = command_line->GetSwitchValue("url");
	
	if( url.empty() )
	{
		url = "app://gwtest/GWTTest.html";
	}

	// Create the first browser window.

	CefBrowserHost::CreateBrowser( windowInfo,
								   handler.get(),
								   url,
								   browserSettings,
								   NULL );
}

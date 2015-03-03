// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "ShellAppHandler.h"

#include <sstream>
#include <string>

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"



namespace
{

	ShellAppHandler* g_instance = NULL;

}  // namespace


ShellAppHandler::ShellAppHandler()
    : is_closing_(false)
{
	DCHECK( !g_instance );
	g_instance = this;
}



ShellAppHandler::~ShellAppHandler()
{
	g_instance = NULL;
}



// static
ShellAppHandler* ShellAppHandler::GetInstance()
{
	return g_instance;
}



void ShellAppHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	// Add to the list of existing browsers.
	browser_list_.push_back(browser);
}



bool ShellAppHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	// Closing the main window requires special handling. See the DoClose()
	// documentation in the CEF header for a detailed destription of this
	// process.

	if( browser_list_.size() == 1 )
	{
		// Set a flag to indicate that the window close should be allowed.
		is_closing_ = true;
	}

	// Allow the close. For windowed browsers this will result in the OS close
	// event being sent.

	return false;
}



void ShellAppHandler::OnBeforeClose( CefRefPtr<CefBrowser>		browser )
{
	CEF_REQUIRE_UI_THREAD();

	// Remove from the list of existing browsers.

	for( BrowserList::iterator bit = browser_list_.begin(); bit != browser_list_.end(); ++bit )
	{
		if( (*bit)->IsSame( browser ) )
		{
			browser_list_.erase( bit );
			break;
		}
	}

	if( browser_list_.empty() )
	{
		// All browser windows have closed. Quit the application message loop.

		CefQuitMessageLoop();
	}
}



void ShellAppHandler::OnLoadError( CefRefPtr<CefBrowser>		browser,
								   CefRefPtr<CefFrame>			frame,
                                   ErrorCode					errorCode,
                                   const CefString&				errorText,
                                   const CefString&				failedUrl )
{
	CEF_REQUIRE_UI_THREAD();

	// Don't display an error for downloaded files.

	if (errorCode == ERR_ABORTED)
	{
		return;
	}

	// Display a load error message.
	
	std::stringstream ss;
	
	ss << "<html><body bgcolor=\"white\">"
		"<h2>Failed to load URL " << std::string(failedUrl) <<
		" with error " << std::string(errorText) << " (" << errorCode <<
		").</h2></body></html>";
	
	frame->LoadString( ss.str(), failedUrl );
}



void ShellAppHandler::CloseAllBrowsers(bool force_close)
{
	if( !CefCurrentlyOn( TID_UI ))
	{
		// Execute on the UI thread.

		CefPostTask( TID_UI, base::Bind( &ShellAppHandler::CloseAllBrowsers, this, force_close ));
		return;
	}

	if (browser_list_.empty())
	{
		return;
	}

	;
	for (BrowserList::const_iterator itr = browser_list_.begin(); itr != browser_list_.end(); ++itr)
	{
		(*itr)->GetHost()->CloseBrowser( force_close );
	}
}

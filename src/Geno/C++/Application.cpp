/*
 * Copyright (c) 2021 Sebastian Kylander https://gaztin.com/
 *
 * This software is provided 'as-is', without any express or implied warranty. In no event will
 * the authors be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose, including commercial
 * applications, and to alter it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not claim that you wrote the
 *    original software. If you use this software in a product, an acknowledgment in the product
 *    documentation would be appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be misrepresented as
 *    being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include "Application.h"

#include "Discord/DiscordRPC.h"
#include "GUI/Modals/IModal.h"
#include "GUI/MainWindow.h"

#include <Common/Async/JobSystem.h>

#include <iostream>

//////////////////////////////////////////////////////////////////////////

Application::~Application( void )
{
	// Save workspace on exit
	CloseWorkspace();

} // ~Application

//////////////////////////////////////////////////////////////////////////

int Application::Run( int NumArgs, char** ppArgs )
{
	HandleCommandLineArgs( NumArgs, ppArgs );

	JobSystem::Instance().StartThreads( std::thread::hardware_concurrency() );
	DiscordRPC::Instance().InitDiscord();
	MainWindow::Instance();

	while( MainWindow::Instance().Update() )
	{
		MainWindow::Instance().Render();

		DiscordRPC::Instance().UpdateDiscord();
	}

	DiscordRPC::Instance().Shutdown();

	return 0;

} // Run

//////////////////////////////////////////////////////////////////////////

void Application::NewWorkspace( std::filesystem::path Location, std::string Name )
{
	CloseWorkspace();

	Workspace& rWorkspace    = m_CurrentWorkspace.emplace( std::move( Location ) );
	rWorkspace.m_Name        = std::move( Name );
	rWorkspace.m_BuildMatrix = BuildMatrix::PlatformDefault();

} // NewWorkspace

//////////////////////////////////////////////////////////////////////////

bool Application::LoadWorkspace( std::filesystem::path Path )
{
	std::error_code Error;

	if( std::filesystem::exists( Path, Error ) )
	{
		if( Path.is_relative() )
			Path = std::filesystem::absolute( Path, Error );

		if( !Error )
		{
			CloseWorkspace();
			NewWorkspace( Path.parent_path(), Path.filename().replace_extension().string() );

			return m_CurrentWorkspace->Deserialize();
		}
	}

	return false;

} // LoadWorkspace

//////////////////////////////////////////////////////////////////////////

void Application::CloseWorkspace( void )
{
	if( m_CurrentWorkspace )
		m_CurrentWorkspace->Serialize();

	m_CurrentWorkspace.reset();

} // CloseWorkspace

//////////////////////////////////////////////////////////////////////////

Workspace* Application::CurrentWorkspace( void )
{
	return m_CurrentWorkspace.has_value() ? &m_CurrentWorkspace.value() : nullptr;

} // CurrentWorkspace

//////////////////////////////////////////////////////////////////////////

void Application::HandleCommandLineArgs( int NumArgs, char** ppArgs )
{
	switch( NumArgs )
	{
		default:
		{
		} [[ fallthrough ]];

		case 2:
		{
			const std::filesystem::path WorkspacePath = ppArgs[ 1 ];
			std::error_code             Error;

			if( !std::filesystem::exists( WorkspacePath, Error ) )
				exit( 1 );

			if( !LoadWorkspace( WorkspacePath ) )
				exit( 1 );

		} [[ fallthrough ]];

		case 1:
		{
			m_ExePath = std::filesystem::absolute( ppArgs[ 0 ] ).lexically_normal();
#if defined( BUILD_XCODE )
			m_AppDir  = m_ExePath.parent_path().parent_path().parent_path();
			m_DataDir = m_AppDir / L"Contents" / L"Resources" / L"data";
#elif defined( _WIN32 ) || defined( __linux__ ) || defined( __APPLE__ ) // BUILD_XCODE
			m_AppDir  = m_ExePath.parent_path();
			m_DataDir = m_AppDir.parent_path().parent_path() / L"data";
#endif // _WIN32 || __linux__ || __APPLE__
			
			std::error_code Error;
			std::filesystem::current_path( m_AppDir, Error );

		} [[ fallthrough ]];

		case 0:
		{
		} break;
	}

} // HandleCommandLineArgs

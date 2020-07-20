/*
 * Copyright (c) 2020 Sebastian Kylander https://gaztin.com/
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

#include "TextEditor.h"

#include "GUI/MainMenuBar.h"
#include "GUI/MainWindow.h"

#include <imgui.h>

TextEditor::TextEditor( void )
	: text_( "#include <iostream>\n\nint main(int argc, char* argv[])\n{\n\tstd::cout << \"Hello, world!\\n\";\n\treturn 0;\n}\n" )
	, show_( true )
{
}

TextEditor::TextEditor( TextEditor&& other )
	: text_( std::move( other.text_ ) )
	, show_( other.show_ )
{
	other.show_ = false;
}

TextEditor& TextEditor::operator=( TextEditor&& other )
{
	text_ = std::move( other.text_ );
	show_ = other.show_;

	other.show_ = false;

	return *this;
}

void TextEditor::Show( void )
{
	const int window_flags = ( 0
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoBringToFrontOnFocus
	);

	MainWindow& main_window = MainWindow::Get();

	ImVec2 pos;
	pos.y += MainMenuBar::Get().Height();

	ImGui::SetNextWindowPos( pos, ImGuiCond_Always );
	ImGui::SetNextWindowSize( ImVec2( main_window.Width() - pos.x, main_window.Height() - pos.y ), ImGuiCond_Always );

	if( ImGui::Begin( "TextEditor", &show_, window_flags ) )
	{
		const int input_text_flags = ImGuiInputTextFlags_AllowTabInput;

		ImGui::InputTextMultiline( "Editor", &text_[ 0 ], text_.size(), ImVec2( 0, 0 ), input_text_flags );
	}
	ImGui::End();
}

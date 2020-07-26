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

#pragma once
#include <codecvt>
#include <filesystem>
#include <string_view>
#include <string>

using path                 = std::filesystem::path;
using path_string          = std::basic_string< std::filesystem::path::value_type >;
using path_view            = std::basic_string_view< std::filesystem::path::value_type >;
using wstring_convert_utf8 = std::wstring_convert< std::codecvt_utf8< wchar_t > >;

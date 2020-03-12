/*
** tcp.cpp - tcp/ip implementation
**
** lecnet network library
** Copyright (c) 2018 Alec T. Musasa (alecmus at live dot com)
**
*******************************************************************************
** This file is part of the liblec library which is released under the Creative
** Commons Attribution Non-Commercial 2.0 license (CC-BY-NC 2.0). See the file
** LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
** for full license details.
*/

#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_	// prevent winsock 1 from being defined
#endif

#include "../tcp.h"
#include "../helper_fxns/helper_fxns.h"

#define _CRT_SECURE_NO_WARNINGS
#define ASIO_STANDALONE

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif

#ifdef _WINSOCKAPI_
#undef _WINSOCKAPI_
#include <boost/asio.hpp>
#define _WINSOCKAPI_
#else
#include <boost/asio.hpp>
#endif

#ifdef _WIN64
#pragma comment(lib, "libeay64.lib")
#pragma comment(lib, "ssleay64.lib")
#else
#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")
#endif // _WIN64

#undef _CRT_SECURE_NO_WARNINGS

void liblec::lecnet::tcp::get_host_ips(std::vector<std::string>& ips) {
	try {
		boost::asio::io_service io_service;
		boost::asio::ip::tcp::resolver resolver(io_service);

		std::string host_name = boost::asio::ip::host_name();
		boost::asio::ip::tcp::resolver::query query(host_name, "");

		boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
		boost::asio::ip::tcp::resolver::iterator end;

		while (iter != end) {
			boost::asio::ip::tcp::endpoint ep = *iter++;

			boost::asio::ip::address address = ep.address();

			if (address.is_v4())
				ips.push_back(address.to_string());
		}
	}
	catch (std::exception&) {
		ips.clear();
	}

	// add localhost if it doesn't exist
	std::string local_host = "127.0.0.1";

	if (std::find(ips.begin(), ips.end(), local_host) == ips.end())
		ips.push_back(local_host);

	// sort alphabetically
	std::sort(ips.begin(), ips.end(), compare_no_case);
}

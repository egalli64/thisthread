/**
 * ASIO Basic Skills examples
 *
 * Author:   Manny egalli64@gmail.com
 * Info:     http://thisthread.blogspot.com/2018/03/boost-asio-basic-skills.html
 * Based on: Boost ASIO Tutorial "Basic Skills"
 *           http://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/tutorial.html
 */
#pragma once

#include <boost/asio/io_context.hpp>

namespace ba = boost::asio;
namespace bs = boost::system;
namespace sc = std::chrono;

extern void timer1(ba::io_context& io);
extern void timer2(ba::io_context& io);
extern void timer2a(ba::io_context& io);
extern void timer3(ba::io_context& io);
extern void timer3a(ba::io_context& io);
extern void timer4(ba::io_context& io);
extern void timer5(ba::io_context& io);

#ifndef _SSH_COMMON_HPP
#define _SSH_COMMON_HPP

#include <boost/shared_ptr.hpp>
#include <stdexcept>

class Session;
class Channel;
class ExecResult;

typedef boost::shared_ptr<Session> sessionptr;
typedef boost::shared_ptr<Channel> channelptr;
typedef boost::shared_ptr<ExecResult> execresultptr;

# endif

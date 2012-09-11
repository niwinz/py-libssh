#ifndef _SSH_COMMON_HPP
#define _SSH_COMMON_HPP

#include <boost/shared_ptr.hpp>
#include <libssh/libssh.h>
#include <stdexcept>
#include <string>

class Session;
class Channel;
class ExecResult;

typedef boost::shared_ptr<Session> sessionptr;
typedef boost::shared_ptr<Channel> channelptr;
typedef boost::shared_ptr<ExecResult> execresultptr;

# endif


#ifndef LEVEL_HPP
#define LEVEL_HPP

namespace MCServer {
namespace Logging {

enum Level {
    FINEST,
    FINE,
    INFO,
    WARNING,
    SEVERE
};

#define USING_LOGGING_LEVEL \
    using MCServer::Logging::FINEST; \
    using MCServer::Logging::FINE; \
    using MCServer::Logging::INFO; \
    using MCServer::Logging::WARNING; \
    using MCServer::Logging::SEVERE;

}
}

#endif

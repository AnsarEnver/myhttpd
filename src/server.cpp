#include <glog/logging.h>
#include <gflags/gflags.h>
#include <boost/uuid/uuid_generators.hpp>

#include "server.hpp"
#include "http/http_session.hpp"

using namespace boost::asio::ip;

namespace myhttpd {

    void server::event_loop() {
        this->_io.run();
    }

    server::server(tinyxml2::XMLElement *config)
    : _work_guard(this->_io.get_executor()) {
        auto acs_cfg = config->FirstChildElement("acceptors");
        auto ac_cfg = acs_cfg->FirstChildElement();
        //The memory of XMLElement objects are managed by the XMLDocument object which created themslevs,
        //Thus no need to delete it in this scope.
        while(ac_cfg) {
            /* Create acceptors */
            this->_acceptors.push_back(std::make_unique<acceptor>(this->_io, ac_cfg));
            ac_cfg = ac_cfg->NextSiblingElement();
        }
    }

    void server::start() {
        for(auto &lis: this->_acceptors) {
            lis->start_async_accept(
                /* New connection accept event handler */
                [&sessions = this->_sessions]
                (std::unique_ptr<connection> conn) {
                    auto ses = std::make_unique<http_session>(std::move(conn));
                    boost::uuids::uuid id = boost::uuids::random_generator()();
                    ses->start([id, &sessions]() {
                        sessions.erase(id);
                    });
                    sessions.insert(std::pair<boost::uuids::uuid, std::unique_ptr<session>>(id, std::move(ses)));
                }
            );
        }
    }
}


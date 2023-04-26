#include <boost/filesystem.hpp>
#include <boost/date_time/posix_time/conversion.hpp>

#include "filesystem_content.hpp"
#include "filesystem_rnode.hpp"
#include "../../utils.hpp"

namespace myhttpd::session::http {

	struct range_string {
		
		std::string begin;
		std::string end;
	};

	static std::list<range_string> extract_ranges(std::string attr_val) {

		std::size_t begin_offset = attr_val.find("bytes=");
		std::size_t end_offset;
		std::list<range_string> result;

		if (begin_offset == std::string::npos) {

			return result;
		} 

		begin_offset += 6;

		while (true) {

			range_string range;
			end_offset = attr_val.find('-', begin_offset);

			if (end_offset == std::string::npos) {

				return result;
			}

			if (begin_offset != end_offset) {

				range.begin = attr_val.substr(begin_offset, end_offset - begin_offset);
			}

			if (end_offset + 1 == attr_val.size()) {

				result.push_back(range);
				return result;
			}

			begin_offset = end_offset + 1;
			end_offset = attr_val.find(',', begin_offset);

			if (end_offset == std::string::npos) {

				range.end = attr_val.substr(begin_offset);
				result.push_back(range);
				return result;
				 
			}

			range.end = attr_val.substr(begin_offset, end_offset - begin_offset);
			result.push_back(range);
			begin_offset = end_offset;
		}
	}

	bool filesystem_rnode::_exists(std::string url) {

		std::string abs_path = this->_path + url;
		return boost::filesystem::exists(abs_path) && boost::filesystem::is_regular_file(abs_path);
	}

	void http::filesystem_rnode::_do_option(std::unique_ptr<request> req, request_handler handler) {

	}

	void http::filesystem_rnode::_do_get(std::unique_ptr<request> req, request_handler handler) {

		auto rsp = std::make_unique<response>(std::move(req));
		auto &req_ref = rsp->get_request();

		if (this->_exists(req_ref.get_url())) {

			rsp->insert_attribute("accept-ranges", "bytes");
			boost::filesystem::path filepath(this->_path + req_ref.get_url());
			auto last_modified_time = 
				boost::posix_time::from_time_t(boost::filesystem::last_write_time(filepath));
			auto last_modified_time_str = utils::ptime_to_http_date(last_modified_time);
			rsp->insert_attribute("last-modified", last_modified_time_str);
			auto if_modified_since = req_ref.find_attribute("if-modified-since");

			if (if_modified_since != req_ref.end_attribute()) {

				if (last_modified_time_str == if_modified_since->second) {

					rsp->set_status(304);
					handler(std::move(rsp));
					return;
				}
			}

			auto content = std::make_shared<filesystem_content>(this->_path + req_ref.get_url());
			rsp->set_content(content);

			if (req_ref.contains_attribute("range")) {

				auto ranges = extract_ranges(req_ref.find_attribute("range")->second);

				if (ranges.size() != 0) {

					auto range = ranges.front();
					auto file_size = content->get_size();

					if (!range.begin.empty()) {

						std::size_t begin = std::stoll(range.begin);

						if (begin >= file_size) {

							rsp->set_status(416);
							rsp->set_content(nullptr);
							handler(std::move(rsp));
							return;

						} else {

							content->set_range_begin(begin);
						}
					}

					if (!range.end.empty()) {

						std::size_t end = std::stoll(range.end);

						if (end >= file_size) {

							rsp->set_status(416);
							rsp->set_content(nullptr);
							handler(std::move(rsp));
							return;

						} else {

							content->set_range_end(end);
						}
					}

					if (!range.begin.empty() && !range.end.empty()) {

						std::size_t begin = std::stoll(range.begin);
						std::size_t end = std::stoll(range.end);

						if (begin >= end) {

							rsp->set_status(416);
							rsp->set_content(nullptr);
							handler(std::move(rsp));
							return;
						}
					}

					rsp->set_status(206);
					rsp->insert_attribute("content-range", 
						range.begin + "-" + range.end + "/" + std::to_string(content->get_size()));

				} else {

					rsp->set_status(200);
					handler(std::move(rsp));
					return;
				}

			} else {

				rsp->set_status(200);
				handler(std::move(rsp));
				return;
			}

		} else {

			rsp->set_status(404);
			handler(std::move(rsp));
			return;
		}
	}

	void http::filesystem_rnode::_do_head(std::unique_ptr<request> req, request_handler handler) {

		this->_do_get(std::move(req), 

			[handler](std::unique_ptr<response> rsp) {
				
				rsp->set_content(nullptr);
				handler(std::move(rsp));
			}
		);
	}

	void http::filesystem_rnode::_do_post(std::unique_ptr<request> req, request_handler handler) {

		this->_do_get(std::move(req), handler);
	}

	void http::filesystem_rnode::_do_put(std::unique_ptr<request> req, request_handler handler) {

	}

	void http::filesystem_rnode::_do_delete(std::unique_ptr<request> req, request_handler handler) {

	}

	void http::filesystem_rnode::_do_trace(std::unique_ptr<request> req, request_handler handler) {

		auto rsp = std::make_unique<response>(std::move(req));
		rsp->set_content(req->get_content());
		handler(std::move(rsp));
	}

	void filesystem_rnode::async_request(std::unique_ptr<request> req, request_handler handler) {

		switch (req->get_method()) {

		case request::method::OPTIONS: this->_do_option(std::move(req), handler); break;
		case request::method::GET: this->_do_get(std::move(req), handler); break;
		case request::method::HEAD: this->_do_head(std::move(req), handler); break;
		case request::method::POST: this->_do_post(std::move(req), handler); break;
		case request::method::PUT: this->_do_put(std::move(req), handler); break;
		case request::method::DELETE: this->_do_delete(std::move(req), handler); break;
		case request::method::TRACE: this->_do_trace(std::move(req), handler); break;

		default:

			break;
		}
	}

	filesystem_rnode::filesystem_rnode(std::string path): 
		_path(path) {

	}
}
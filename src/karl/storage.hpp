#pragma once

#include <map>
#include <pqxx/pqxx>
#include <boost/optional.hpp>

#include <supermarx/id_t.hpp>
#include <supermarx/token.hpp>

#include <supermarx/product.hpp>
#include <supermarx/api/product_summary.hpp>
#include <supermarx/api/product_history.hpp>
#include <supermarx/api/product_log.hpp>
#include <supermarx/api/session.hpp>
#include <supermarx/api/tag.hpp>

#include <karl/karluser.hpp>
#include <karl/session.hpp>
#include <karl/sessionticket.hpp>

namespace supermarx
{
class storage
{
public:
	class not_found_error : std::runtime_error
	{
	public:
		not_found_error();
	};

private:
	pqxx::connection conn;

public:
	storage(std::string const& host, std::string const& user, std::string const& password, const std::string& db);
	~storage();

	id_t add_karluser(karluser const& user);
	karluser get_karluser(id_t karluser_id);
	std::pair<id_t, karluser> get_karluser_by_name(std::string const& name);
	id_t add_sessionticket(sessionticket const& st);
	std::pair<id_t, sessionticket> get_sessionticket(id_t sessionticket_id);
	id_t add_session(session const& s);
	std::pair<id_t, session> get_session_by_token(api::sessiontoken const& token);

	void add_product(product const& p, id_t supermarket_id, datetime retrieved_on, confidence conf, std::vector<std::string> const& problems);
	api::product_summary get_product(std::string const& identifier, id_t supermarket_id);
	std::vector<api::product_summary> get_products_by_name(std::string const& name, id_t supermarket_id);
	std::vector<api::product_log> get_recent_productlog(id_t supermarket_id);
	api::product_history get_product_history(std::string const& identifier, id_t supermarket_id);

	void absorb_productclass(id_t src_productclass_id, id_t dest_productclass_id);

	id_t find_add_tag(api::tag const& t);
	void bind_tag(id_t productclass_id, id_t tag_id);

	id_t add_image_citation(id_t supermarket_id, std::string const& original_uri, std::string const& source_uri, size_t original_width, size_t original_height, datetime const& retrieved_on);
	void update_product_image_citation(std::string const& product_identifier, id_t supermarket_id, id_t image_citation_id);

private:
	void update_database_schema();
	void prepare_statements();
};
}

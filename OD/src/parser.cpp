#include"headers/alias.hpp"
#include"headers/parser.hpp"

Parser make_parser(Graph const& g) {
	Parser p;
	for (auto it = boost::vertices(g).first; it != boost::vertices(g).second; it++) {
		PVertex v = boost::add_vertex(p);
		p[v].index = g[*it].index;
		p[v].pass = 0;
		boost::add_edge(v, v, { 0 }, p);
	}

	for (auto it = boost::edges(g).first; it != boost::edges(g).second; it++) {
		Vertex s = boost::source(*it, g);
		Vertex t = boost::target(*it, g);
		PVertex ps = *std::find_if(boost::vertices(p).first, boost::vertices(p).second, [&](PVertex v) {return p[v].index == g[s].index; });
		PVertex pt = *std::find_if(boost::vertices(p).first, boost::vertices(p).second, [&](PVertex v) {return p[v].index == g[t].index; });
		boost::add_edge(ps, pt, { 0 }, p);
	}
	return p;
}
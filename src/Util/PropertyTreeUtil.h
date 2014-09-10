/*
 * PropertyTreeUtil.h
 *
 *  Created on: 2014/06/12
 *      Author: kryozahiro
 */

#ifndef PROPERTYTREEUTIL_H_
#define PROPERTYTREEUTIL_H_

#include <string>
#include <boost/property_tree/ptree.hpp>

namespace PropertyTreeUtil {

//domain内からpathの内容がcontentであるような子を探す
//pathの選び方は保障されない
template<typename Key, typename Data, typename KeyCompare>
const boost::property_tree::basic_ptree<Key, Data, KeyCompare>& search(
		const boost::property_tree::basic_ptree<Key, Data, KeyCompare>& domain,
		const typename boost::property_tree::basic_ptree<Key, Data, KeyCompare>::path_type& path,
		const typename boost::property_tree::basic_ptree<Key, Data, KeyCompare>::data_type& content,
		bool recursive = true) {

	namespace pt = boost::property_tree;
	using tree_type = pt::basic_ptree<Key, Data, KeyCompare>;

	for (const typename tree_type::value_type& kvp : domain) {
		const tree_type& child = kvp.second;
		try {
			const typename tree_type::data_type& childContent = child.template get<typename tree_type::data_type>(path);
			if (!KeyCompare()(content, childContent) and !KeyCompare()(childContent, content)) {
				return child;
			}
		} catch (pt::ptree_bad_path& e) {
			//何もしない
		}

		//再帰探索
		if (recursive) {
			try {
				return search(child, path, content, recursive);
			} catch (pt::ptree_bad_path& e) {
				//何もしない
			}
		}
	}
	throw pt::ptree_bad_path("ptree not found", path);
}

//<node name="name-of-node" ref="reference-to-name-of-other-node"/>の形式で表現される参照の解決
template<typename Key, typename Data, typename KeyCompare>
const boost::property_tree::basic_ptree<Key, Data, KeyCompare>& solveReference(
		const boost::property_tree::basic_ptree<Key, Data, KeyCompare>& domain,
		const boost::property_tree::basic_ptree<Key, Data, KeyCompare>& tree) {

	namespace pt = boost::property_tree;
	using tree_type = pt::basic_ptree<Key, Data, KeyCompare>;

	const tree_type* now = &tree;
	while (true) {
		typename tree_type::data_type ref;
		try {
			ref = now->template get<typename tree_type::data_type>("<xmlattr>.ref");
		} catch (pt::ptree_bad_path& e) {
			return *now;
		}
		now = &search(domain, "<xmlattr>.name", ref);
	}
}

}

#endif /* PROPERTYTREEUTIL_H_ */

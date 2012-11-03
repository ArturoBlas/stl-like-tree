/**
 * @file tree.hpp
 * @author  Arturo Blas Jiménez <arturoblas@gmail.com>
 * @version 0.1
 *
 * @section LICENSE
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 *  This header file provides a STL-like implementation 
 *  for a tree data structure with specific iterators for
 *  BFS and DFS.
 *
 */

#ifndef TREE_HPP_
#define TREE_HPP_

#include <list>
#include <algorithm>
#include <stack>
#include <queue>

#ifndef TREE_NAMESPACE__
#define TREE_NAMESPACE__ std
#endif

namespace TREE_NAMESPACE__
{
	/**
	 * This defines the default container type
	 * that handles the list of children of a node
	 */
	template<class T, class container_type = std::list<T> >
	struct container_type_def
	{
		typedef container_type type; //< The container type!
	};
	
	template<typename T>
	class node: public container_type_def<node<T> >::type
	{
	public:
		
		typedef T value_type;
		typedef node<value_type> node_type;
		typedef typename container_type_def<node<T> >::type container_type;
		typedef typename container_type::iterator child_iterator;
		typedef typename container_type::reverse_iterator reverse_child_iterator;
		typedef typename container_type::const_iterator const_child_iterator;
		typedef typename container_type::const_reverse_iterator const_reverse_child_iterator;
		typedef typename container_type::size_type size_type;
		
		node(const value_type& data) : data_(data) {}
		
		node(const node& other) : container_type(other), data_(other.data_) 
		{
		}
		
		inline
		const value_type& const_data() const
		{
			return data_;
		}
		
		inline
		value_type& data()
		{
			return data_;
		}
		
		bool operator==(const node_type& other)
		{
			return data_ == other.data_;
		}
		
		bool operator==(const value_type& value)
		{
			return data_ == value;
		}
		
		// This iterator provides deep first search iteration capability
		class dfs_iterator: public std::iterator<std::forward_iterator_tag,	value_type>
		{
		private:
			template<class U> friend class node;
			
			explicit dfs_iterator(node_type& root)
			{
				stack_.push(&root);
			}
			
			
			
		public:
			
			dfs_iterator()
			{
				// This is used to create the dfs_end() iterator
			}
			
			dfs_iterator& operator++()
			{
				if (!stack_.empty())
				{
					node_type* n = stack_.top();
					
					// Remove the item and put its children
					stack_.pop();
					reverse_child_iterator it = n->rbegin();
					while (it != n->rend())
					{
						stack_.push(const_cast<node_type*>(&*it));
						it++;
					}
				}
				
				return *this;
			}
			
			node_type* operator->() const
			{
				if (!stack_.empty())
					return stack_.top();
				else
					return 0;
			}
			
			node_type& operator*() const
			{
				return *stack_.top();
			}
			
			bool operator!=(const dfs_iterator& other) const
			{
				return this->operator ->() != other.operator ->();
			}
			
			bool operator==(const dfs_iterator& other) const
			{
				return !this->operator !=(other);
			}
			
		private:
			std::stack<node_type*> stack_;
		};
		
		dfs_iterator dfs_begin()
		{
			return dfs_iterator(*this);
		}
		
		dfs_iterator dfs_end() const
		{
			return dfs_iterator();
		}
		
		// This iterator provides breath first search iteration capability
		class bfs_iterator: public std::iterator<std::forward_iterator_tag,	value_type>
		{
		private:
			template<class U> friend class node;
			
			explicit bfs_iterator(const node_type& root)
			{
				queue_.push(const_cast<node_type*>(&root));
			}
			
		public:
			
			bfs_iterator()
			{
				// This is used to create the end() iterator
			}
			
			bfs_iterator& operator++()
			{
				if (!queue_.empty())
				{
					node_type* n = queue_.front();
					
					// Remove the item and put its children
					queue_.pop();
					child_iterator it = n->begin();
					while (it != n->end())
					{
						queue_.push(const_cast<node_type*>(&*it));
						it++;
					}
				}
				
				return *this;
			}
			
			node_type* operator->() const
			{
				if (!queue_.empty())
					return queue_.front();
				else
					return 0;
			}
			
			node_type& operator*() const
			{
				return *queue_.front();
			}
			
			bool operator!=(const bfs_iterator& other) const
			{
				return this->operator ->() != other.operator ->();
			}
			
			bool operator==(const bfs_iterator& other) const
			{
				return !this->operator !=(other);
			}
			
		private:
			std::queue<node_type*> queue_;
		};
		
		bfs_iterator bfs_begin() const
		{
			return bfs_iterator(*this);
		}
		
		bfs_iterator bfs_end() const
		{
			return bfs_iterator();
		}
		
		bool contains(const value_type& data) const
		{
			return std::find(this->begin(), this->end(), data) != this->end();
		}
		
		bool contains_recursive(const value_type& data) const
		{
			return std::find(bfs_begin(), bfs_end(), data) != bfs_end();
		}
		
		size_type remove_recursive(const value_type& data)
		{
			size_type many = 0;
			child_iterator it = this->begin();
			while(it!=this->end())
			{
				many += it->remove_recursive(data);
				it++;
			}
			size_type size = this->size();
			this->remove(data);
			many += size - this->size();
			return many;
		}
		
	private:
		value_type data_;
	};
	
	template<class T>
	bool operator<(const node<T>& a, const node<T>& b)
	{
		return a.const_data() < b.const_data();
	}
	
	template<class T>
	bool operator==(const node<T>& a, const node<T>& b)
	{
		return a.const_data() == b.const_data();
	}
	
	template<class T>
	node<T>& operator>>(node<T>& a, node<T>& b)
	{
		a.push_back(b);
		return a.back();
	}
	
	template<class T>
	node<T>& operator>>(node<T>& a, const T& b)
	{
		a.push_back(b);
		return a.back();
	}
	
	template<typename T>
	class tree: public node<T>
	{
	public:
		typedef node<T> base_type;
		typedef node<T> node_type;
		typedef typename base_type::value_type value_type;
		
		tree(const value_type& data) : base_type(data) {}
	};
	
	template<typename T>
	class graph: public node<typename container_type_def<node<T> >::type>
	{
	public:
		
		typedef T value_type;
		typedef node<value_type> node_type;
		typedef typename container_type_def<node_type>::type container_type;
		typedef node<container_type> base_type;
		
		graph() :
		base_type(container_type())
		{
		}
	};
	
} // namespace TREE_NAMESPACE__

#endif /* TREE_HPP_ */

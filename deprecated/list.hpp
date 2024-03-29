new node_type();
					node_type	*head = _tail->next;

					tmp->data = _alloc.allocate(1);
					_alloc.construct(tmp->data,val);
					tmp->next = head;
					tmp->prev = _tail;
					head->prev = tmp;
					_tail->next = tmp;
					_size++;
				}

				void	pop_front(void)
				{
					node_type *save = _tail->next;

					_tail->next = save->next;
					_tail->next->prev = _tail;
					_size--;
					_alloc.destroy(save->data);
					_alloc.deallocate(save->data, sizeof(value_type));
					delete save;
				}

				void	push_back(const value_type& val)
				{
					node_type	*tmp = new node_type();

					tmp->data = _alloc.allocate(1);
					_alloc.construct(tmp->data,val);
					tmp->next = _tail;
					tmp->prev = _tail->prev;
					_tail->prev->next = tmp;
					_tail->prev = tmp;
					_size++;
				}

				void	pop_back(void)
				{
					node_type *save = _tail->prev;

					_tail->prev = save->prev;
					_tail->prev->next = _tail;
					_size--;
					_alloc.destroy(save->data);
					_alloc.deallocate(save->data, sizeof(value_type));
					delete save;
				}

				iterator insert(iterator position, const value_type &val)
				{
					node_type	*tmp = new node_type();

					tmp->data = _alloc.allocate(1);
					_alloc.construct(tmp->data,val);
					tmp->next = position.getPtr();
					tmp->prev = position.getPtr()->prev;
					position.getPtr()->prev = tmp;
					tmp->prev->next = tmp;
					_size++;

					return (iterator(tmp));
				}

				void insert(iterator position, size_type n, const value_type& val)
				{
					size_type	i;

					for (i = 0; i < n; ++i)
						position = insert(position, val);
				}

				template <class InputIterator>
					void insert (iterator position, InputIterator first, typename ft::enable_if<InputIterator::input_iter, InputIterator>::type last)
				{
					position--;
					for (; first != last; ++first)
						position = insert(++position, *first);
				}

				iterator erase(iterator position)
				{
					node_type	*ptr = position.getPtr();
					iterator	ret(ptr->next);

					ptr->next->prev = ptr->prev;
					ptr->prev->next = ptr->next;
					_size--;
					_alloc.destroy(ptr->data);
					_alloc.deallocate(ptr->data, sizeof(value_type));
					delete ptr;

					return ret;
				}

				iterator erase(iterator first, iterator last)
				{
					iterator it = first;
					while (it != last)
						it = erase(it);
					return last;
				}

				void swap(list &x)
				{
					std::swap(_tail, x._tail);
					std::swap(_size, x._size);
				}

				void resize (size_type n, value_type val = value_type())
				{
					iterator	it = begin();

					if (n < _size)
					{
						for (size_type i = 0; i < n; ++i)
							++it;
						erase(it, end());
					}
					if (n > _size)
						insert(end(), n - _size, val);

				}

				void clear(void)
				{
					node_type	*save;
					node_type	*to_del = _tail->next;

					while (_size)
					{
						save = to_del->next;
						_alloc.destroy(to_del->data);
						_alloc.deallocate(to_del->data, sizeof(value_type));
						delete to_del;
						to_del = save;
						_size--;
					}
					_tail->prev = _tail;
					_tail->next = _tail;
				}

//operations
				void splice(iterator position, list &x)
				{
//					node_type	*end_insert = position.getPtr();
//					node_type	*start_insert = end_insert->prev;
//					node_type	*last = x.end().getPtr()->prev;
//					node_type	*first = x.begin().getPtr();
//
//					start_insert->next = first;
//					first->prev = start_insert;
//					end_insert->prev = last;
//					last->next = end_insert;
//					_size += x._size;
//					x._tail->prev = x._tail;
//					x._tail->next = x._tail;
//					x._size = 0;
					splice(position, x, x.begin(), x.end());
				}

				void splice(iterator position, list &x, iterator i)
				{
//					node_type	*end_insert = position.getPtr();
//					node_type	*start_insert = end_insert->prev;
//					node_type	*to_insert = i.getPtr();
//
//					to_insert->prev->next = to_insert->next;
//					to_insert->next->prev = to_insert->prev;
//					start_insert->next = to_insert;
//					to_insert->prev = start_insert;
//					end_insert->prev = to_insert;
//					to_insert->next = end_insert;
//					_size++;
//					x._size--;
					splice(position, x, i, ++i);
				}

				void splice(iterator position, list &x, iterator first, iterator last)
				{
					size_type	size = 1;
					node_type	*end_insert = position.getPtr();
					node_type	*start_insert = end_insert->prev;
					node_type	*first_ptr = first.getPtr();
					node_type	*last_ptr = (--last).getPtr();


					for (;first != last; ++first)
						size++;

					first_ptr->prev->next = last_ptr->next;
					last_ptr->next->prev = first_ptr->prev;
					start_insert->next = first_ptr;
					first_ptr->prev = start_insert;
					end_insert->prev = last_ptr;
					last_ptr->next = end_insert;
					_size += size;
					x._size -= size;
				}

				void remove(const value_type& val)
				{
					iterator	it = begin();

					while (it != end())
					{
						if (*it == val)
							it = erase(it);
						else
							++it;
					}
				}

				template <class Predicate>
					void remove_if(Predicate pred)
				{
					iterator	it = begin();

					while (it != end())
					{
						if (pred(*it))
							it = erase(it);
						else
							++it;
					}
				}

				void unique(void)
				{
					iterator	it = ++begin();
					iterator	cmp = begin();

					while (it != end())
					{
						if (*it == *cmp)
							erase(it++);
						else
							it++;
						cmp = it;
						--cmp;

					}
//					for (; it != end(); ++it)
//					{
//						if (*it == *cmp)
//							erase(cmp++);
//						else
//							cmp++;
//					}
				}

				template <class BinaryPredicate>
					void unique(BinaryPredicate binary_pred)
				{
					iterator	it = ++begin();
					iterator	cmp = begin();

					while (it != end())
					{
						if (binary_pred(*it,*cmp))
							erase(it++);
						else
							it++;
						cmp = it;
						--cmp;

					}

//					for (; it != end(); ++it)
//					{
//						if (binary_pred(*it,*cmp))
//							erase(it);
////						else
//							cmp++;
//					}
				}
				void	merge(list& x)
				{
					iterator	it1 = begin();
					iterator	it2 = x.begin();

					while (it1 != end() && it2 != x.end())
					{
						if (*it1 > *it2)
						{
							splice(it1, x, it2);
							it1 = begin();
							it2 = x.begin();
						}
						++it1;
					}
					if (it2 != x.end())
						splice(it1, x);
				}

				template <class Compare>
					void merge (list& x, Compare comp)
				{
					iterator	it1 = begin();
					iterator	it2 = x.begin();

					while (it1 != end() && it2 != x.end())
					{
						if (comp(*it2, *it1))
						{
							splice(it1, x, it2);
							it1 = begin();
							it2 = x.begin();
						}
						++it1;
					}
					if (it2 != x.end())
						splice(it1, x);
				}


				void	sort(void)
				{
					iterator	it = begin();
					iterator	it2 = ++begin();

					while (it2 != end())
					{
						if (*it > *it2)
						{
							splice(it, *this, it2);
							it = begin();
							it2 = ++begin();
						}
						else
						{
							it++;
							it2++;
						}
					}
				}

				template <class Compare>
					void sort(Compare comp)
				{
					iterator	it = begin();
					iterator	it2 = ++begin();

					while (it2 != end())
					{
						if (comp(*it2, *it))
						{
							splice(it, *this, it2);
							it = begin();
							it2 = ++begin();
						}
						else
						{
							it++;
							it2++;
						}
					}
				}

				void reverse(void)
				{
					iterator	it = begin();

					while (it != end())
					{
						std::swap(it.getPtr()->prev, it.getPtr()->next);
						--it;
					}
					std::swap(_tail->next, _tail->prev);
				}

			private:
				node_type*	_tail;
				size_type	_size;
				allocator_type	_alloc;

				void	init_list(void)
				{
					_tail = new node_type;
					_tail->prev = _tail;
					_tail->next = _tail;
				}

	};

	template <class T, class Alloc>
		bool operator== (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs)
	{
		if (lhs.size() != rhs.size())
			return false;
		return std::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template <class T, class Alloc>
		bool operator!= (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs)
	{
		return !(lhs == rhs);
	}

	template <class T, class Alloc>
		bool operator<  (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs)
	{
		return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class T, class Alloc>
			bool operator<= (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs)
	{
		return !(rhs < lhs);
	}

	template <class T, class Alloc>
		bool operator>  (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs)
	{
		return (rhs < lhs);
	}

	template <class T, class Alloc>
			bool operator>= (const list<T,Alloc>& lhs, const list<T,Alloc>& rhs)
	{
		return !(lhs < rhs);
	}

	template <class T, class Alloc>
		void swap (list<T,Alloc>& x, list<T,Alloc>& y)
	{
		return x.swap(y);
	}
}

#endif

#pragma once

#include "vili/NodeIterator.hpp"

namespace vili
{
	template <class T>
	class NodeValidator : public NodeIterator
	{
	private:
		T m_result;
	public:
		NodeValidator();
		NodeValidator(ComplexAttribute* node);
		void validate(T result);
		T result();
	};

	template <class T>
	NodeValidator<T>::NodeValidator()
	{

	}
	template <class T>
	NodeValidator<T>::NodeValidator(ComplexAttribute* node)
	{
		node->walk([this](NodeIterator& node) { m_cache.push_back(node.get()); });
	}
	template <class T>
	void NodeValidator<T>::validate(T result)
	{
		m_result = result;
	}
	template <class T>
	T NodeValidator<T>::result()
	{
		return m_result;
	}
}

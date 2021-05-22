#include "node_base.hpp"

void NodeBase::AddChild(NodeBase* ptr) {
	children.push_back(ptr);
	ptr->parent = this;
}

NodeBase* NodeBase::GetChild(int index) {
	if (index >= 0) {
		std::list<NodeBase*>::iterator it;
		it = children.begin();
		std::advance(it, index);
		return *it;
	} else {
		std::list<NodeBase*>::reverse_iterator it;
		it = children.rbegin();
		std::advance(it, -index - 1);
		return *it;
	}
}

void NodeBase::RemoveChild(int index) {
	if (index >= 0) {
		std::list<NodeBase*>::iterator it;
		it = children.begin();
		std::advance(it, index);
		(*it)->parent = nullptr;
		children.erase(it);
	} else {
		std::list<NodeBase*>::reverse_iterator it;
		it = children.rbegin();
		std::advance(it, -index - 1);
		(*it)->parent = nullptr;
		children.erase(it.base());
	}
}

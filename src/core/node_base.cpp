#include "node_base.hpp"

void NodeBase::AddChild(NodeBase* ptr) {
	children.push_back(ptr);
}

NodeBase* NodeBase::GetChild(int index) {
	std::list<NodeBase*>::iterator it = children.begin();
	std::advance(it, index);
	return *it;
}

void NodeBase::RemoveChild(int index) {
	std::list<NodeBase*>::iterator it = children.begin();
	std::advance(it, index);
	children.erase(it);
}


#include <iostream>
#include <memory>

using std::unique_ptr;
using std::make_unique;

struct Node
{
	int data;
	unique_ptr<Node> next;

	Node(int _data)
	{
		data = _data;
		next = nullptr;
	}
};

void incrementNodeData(Node& node)
{
	node.data++;
}

int main(void)
{
	std::cout << "=== Starting Program ===" << std::endl;

	unique_ptr<Node> head = make_unique<Node>(7);
	std::cout << "head data = " << head->data << std::endl;
	incrementNodeData(*head);
	head->data += 10;
	std::cout << "head data = " << head->data << std::endl;

	std::cout << "=== Ending Program ===" << std::endl;
}


#include <iostream>
#include <memory>
#include <string>

struct Node
{
	int data;
	Node* next;

	Node(int _data, Node* _next)
	{
		data = _data;
		next = _next;
	}
};

std::string nodeToString(Node* node)
{
	return "Node(" + std::to_string(node->data) + ")";
}

void recPrintStack(Node* node)
{
	if (node)
	{
		std::cout << nodeToString(node) << std::endl;
		recPrintStack(node->next);
	}
	else
	{ std::cout << "NullNode" << std::endl; }
}

int main(void)
{
	std::cout << "=== Starting Program ===" << std::endl;

	Node* head = new Node(1, nullptr);
	Node* cur = head;
	for (int i = 2; i <= 15; i++)
	{
		cur->next = new Node(i, nullptr);
		cur = cur->next;
	}

	recPrintStack(head);

	std::cout << "=== Ending Program ===" << std::endl;
}

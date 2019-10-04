﻿// Huffman Tree.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <algorithm>


class node
{
public:
	node(){}
	node(char c):character(c),is_char(true){}
	node(std::shared_ptr<node> l_node, std::shared_ptr<node> r_node) { left_node = l_node; right_node = r_node; }
	char character = ' ';
	bool is_char = false;
	std::string pos;
	std::shared_ptr<node> left_node;
	std::shared_ptr<node> right_node;
};

class huffman_encoder
{
public:
	std::vector<std::pair<int, std::shared_ptr<node>>> node_queue;
	std::vector<std::pair<int, char>> final_map;
	std::vector<std::shared_ptr<node>> character_map;
	
	void GenerateTree()
	{
		std::cout << "\n GENERATING THE TREE NOW! \n";
		
		std::ifstream file("text.txt");
		std::stringstream buffer;

		buffer << file.rdbuf();
		std::cout << buffer.str() << "\n";
		const std::string text(buffer.str());

		std::vector<char> char_map;
		std::vector<int>  count_map;
		
		auto finished_build = false;
		long current_pos = 0;
		while(!finished_build)
		{
			auto cc = text.at(current_pos);
			current_pos++;

			auto it = std::find(char_map.begin(), char_map.end(), cc);
			if (it != char_map.end()){
				if (current_pos > text.size() - 1)
					finished_build = true;
				continue;
			}

			int count = 0;
			for(auto c : text)
			{
				if (c == cc)
					count++;
			}

			char_map.push_back(cc);
			count_map.push_back(count);

			//std::cout << "char: " << cc << " count: " << count << "\n";

			std::cout << current_pos << "\n";

			if (current_pos > text.size() - 1)
				finished_build = true;
		}

		final_map = std::vector<std::pair<int, char>>();
		for(int i = 0; i < char_map.size(); i++)
		{
			final_map.emplace_back(count_map.at(i), char_map.at(i));
		}

		std::sort(final_map.begin(), final_map.end());
		std::reverse(final_map.begin(), final_map.end());

		for(auto p : final_map)
			std::cout << "Char: " << p.second << " Count: " << p.first << "\n";

		node_queue = std::vector<std::pair<int, std::shared_ptr<node>>>();
		for (auto p : final_map)
			node_queue.emplace_back(p.first, std::make_shared<node>(p.second));

		OrderTree();
	}

	void OrderTree()
	{

		std::cout << "\n ORDERING THE TREE NOW! \n";
		
		bool end_gen = false;
		while (!end_gen)
		{
			if (node_queue.size() == 1)
			{
				end_gen = true;
			}
			else {
				auto l_node = node_queue.at(node_queue.size() - 2);
				auto r_node = node_queue.at(node_queue.size() - 1);

				node_queue.pop_back();
				node_queue.pop_back();

				node merge_node(l_node.second, r_node.second);

				node_queue.emplace_back(l_node.first + r_node.first, std::make_shared<node>(merge_node));

				std::sort(node_queue.begin(), node_queue.end());
				std::reverse(node_queue.begin(), node_queue.end());
			}
		}
		GenerateCharacterMap();
	}

	void GenerateCharacterMap()
	{
		std::cout << "\n GENERATING CHARACTER MAP! \n";
		
		character_map = std::vector<std::shared_ptr<node>>(0);
		std::vector<std::shared_ptr<node>> check_list;
		check_list.push_back(node_queue.at(0).second);
		bool end = false;
		while(!end)
		{
			if(check_list.empty())
			{
				end = true;
				break;
			}
			auto n = check_list.at(0);
			if(!n->is_char){
				n->left_node->pos += "0" + n->pos;
				check_list.push_back(n->left_node);
				n->right_node->pos += "1" + n->pos;
				check_list.push_back(n->right_node);
				//std::cout << "Char: " << n->character << " | Code: " << n->pos << "\n";
			}else
			{
				character_map.emplace_back(n);
			}
			check_list.erase(check_list.begin());
		}
		
		//std::sort(character_map.begin(), character_map.end());
		for (auto h : character_map)
		{
			std::cout << "Char: " << h->character << " Code: " << h->pos << "\n";
		}
		
		//DrawTree();
		ShowSize();
	}

	void ShowSize()
	{
		int size = 0;
		std::ifstream file("text.txt");
		std::stringstream buffer;

		buffer << file.rdbuf();
		//std::cout << buffer.str() << "\n";
		const std::string text(buffer.str());

		for(auto c : text)
		{
			for(auto n : character_map)
			{
				if(c == n->character)
				{
					size += n->pos.size();
					break;
				}
			}
		}
		std::cout << size;
	}

	void DrawTree()
	{
		print2D(node_queue.at(0).second);
	}

	/*void CalculateFileSize()
	{
		std::ifstream file("text.txt");
		std::stringstream buffer;

		buffer << file.rdbuf();
		std::cout << buffer.str() << "\n";
		const std::string text(buffer.str());
	}*/

	void print2DUtil(std::shared_ptr<node> root, int space)
	{
		// Base case  
		if (root == nullptr)
			return;

		// Increase distance between levels  
		space += 1;

		// Process right child first  
		print2DUtil(root->right_node, space);

		// Print current node after space  
		// count  
		std::cout << std::endl;
		for (int i = 1; i < space; i++)
			std::cout << "-";
		std::cout << root->character;

		// Process left child  
		print2DUtil(root->left_node, space);
	}

	// Wrapper over print2DUtil()  
	void print2D(std::shared_ptr<node> root)
	{
		// Pass initial space count as 0  
		print2DUtil(root, 0);
	}

	
};

class huffman_decoder
{
	
};

int main()
{
	huffman_encoder enc;
	enc.GenerateTree();
	return 0;
}
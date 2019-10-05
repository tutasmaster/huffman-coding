// Huffman Tree.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <utility>
#include <vector>
#include <algorithm>


class node
{
public:
	node(){}
	node(char c):character(c),is_char(true){}
	node(std::shared_ptr<node> l_node, std::shared_ptr<node> r_node)
	{
		left_node = std::move(l_node);
		right_node = std::move(r_node);
		left_node->top_node = std::make_shared<node>(*this);
		right_node->top_node = std::make_shared<node>(*this);
	}
	char character = ' ';
	bool is_char = false;
	std::string pos;
	std::shared_ptr<node> top_node;
	std::shared_ptr<node> left_node;
	std::shared_ptr<node> right_node;
};

class huffman_encoder
{
public:
	std::vector<std::pair<int, std::shared_ptr<node>>> node_queue;
	std::vector<std::pair<int, char>> final_map;
	std::vector<std::shared_ptr<node>> character_map;
	size_t new_file_size = 0;
	
	void generate_tree()
	{
		std::cout << "\n GENERATING THE TREE NOW! \n\n";
		
		std::ifstream file("text.txt");
		std::stringstream buffer;

		buffer << file.rdbuf();
		std::cout << "TEXT FOUND IN FILE: " << buffer.str() << "\n";
		const std::string text(buffer.str());

		std::vector<char> char_map;
		std::vector<int>  count_map;
		
		auto finished_build = false;
		long current_pos = 0;
		std::cout << "CHARACTERS FOUND IN FILE: \n";
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
			std::cout << "\tChar: " << p.second << " Count: " << p.first << "\n";

		node_queue = std::vector<std::pair<int, std::shared_ptr<node>>>();
		for (auto p : final_map)
			node_queue.emplace_back(p.first, std::make_shared<node>(p.second));

		order_tree();
	}

	void order_tree()
	{

		std::cout << "\n ORDERING THE TREE NOW! \n\n";
		
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
		generate_character_map();
	}

	void generate_character_map()
	{
		std::cout << "\n GENERATING CHARACTER MAP NOW! \n\n";
		
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
				n->left_node->pos += n->pos + "0";
				check_list.push_back(n->left_node);
				n->right_node->pos += n->pos + "1";
				check_list.push_back(n->right_node);
				//std::cout << "Char: " << n->character << " | Code: " << n->pos << "\n";
			}else
			{
				character_map.emplace_back(n);
			}
			check_list.erase(check_list.begin());
		}

		std::cout << "CHARACTERS BUILT: \n";
		//std::sort(character_map.begin(), character_map.end());
		for (auto h : character_map)
		{
			std::cout << "\tChar: " << h->character << " Code: " << h->pos << "\n";
		}
		
		//DrawTree();
		show_size();
	}

	void show_size()
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
		new_file_size = size;
		std::cout << "\tCURRENT SIZE: " << new_file_size << "BITS\n";
		generate_end_file();
	}

	void generate_end_file()
	{
		std::cout << "\n GENERATING OUT FILE! \n\n";
		unsigned long int curPos = 32; //COMPENSATING FOR THE LENGTH
		char * byte_arr = new char[std::ceil(new_file_size/8) + 5]; //COMPENSATING FOR THE LENGTH

		unsigned long int endPos = new_file_size + 32;
		
		byte_arr[0] = static_cast<int>(endPos >> 24) & 0xFF;
		byte_arr[1] = static_cast<int>(endPos >> 16) & 0xFF;
		byte_arr[2] = static_cast<int>(endPos >> 8) & 0xFF;
		byte_arr[3] = static_cast<int>(endPos) & 0xFF;
		
		std::ifstream file("text.txt");
		std::stringstream buffer;

		buffer << file.rdbuf();
		std::ofstream new_file("out.txt",std::ios::binary);
		//std::cout << buffer.str() << "\n";
		const std::string text(buffer.str());
		for (auto c : text)
		{
			for (auto n : character_map)
			{
				if(n->character == c)
				{
					for(auto v : n->pos)
					{
						write_to_bit(byte_arr, curPos, v == '1');
						if(curPos < new_file_size + 32)
							curPos++;
					}
					break;
				}
			}
		}
		for(int i = curPos; i < (std::ceil(new_file_size / 8) + 5) * 8; i++)
		{
			write_to_bit(byte_arr, i, false);
		}
		new_file.write(byte_arr, std::ceil(new_file_size / 8) + 5);
		new_file.close();
		delete[] byte_arr;

		draw_binary_tree();
	}

	void draw_binary_tree()
	{
		std::cout << "\n DRAWING THE BINARY TREE NOW! \n\n";
		int lowest_position = 0;
		for(auto c : character_map)
		{
			if (c->pos.size() > lowest_position)
				lowest_position = c->pos.size();
		}

		int delay = 0;

		bool end = false;
		auto current_node = node_queue.at(0).second;
		std::vector<std::shared_ptr<node>> queue;
		std::vector<std::string> full_string(256);
		int current_string = 0;
		int current_side = 0;
		draw_binary_node(full_string, current_side, current_string,current_node);

		for(auto s : full_string)
		{
			if (s.empty())
				break;
			std::cout << s << "\n";
		}
	}

	void draw_binary_node(
		std::vector<std::string>& str,
		int current_side,
		int current_string,
		std::shared_ptr<node> n,
		char prefix = ' ',
		bool follow = false,
		bool jump = false
	){

		if(jump)
		{
			int bonus = 0;
			for(auto s : str)
			{
				if(s.empty())
				{
					break;
				}
				bonus++;
			}
			current_string = bonus;
		}
		
		if (n == nullptr)
			return;
		if(follow)
			for (int i = 0; i < current_side * 7; i++)
				str.at(current_string) += " ";
		if (n->is_char){
			str.at(current_string) += prefix;
			str.at(current_string) += n->character;
		}
		else{
			str.at(current_string) += prefix;
			str.at(current_string) += "+";
			str.at(current_string) += "-----";
		}
		
		draw_binary_node(str, current_side + 1,current_string, n->right_node, '1');
		draw_binary_node(str, current_side + 1, current_string + 1, n->left_node, '0', true, true);
	}

	
	
	void write_to_bit(char * &byte_arr, int pos, bool val)
	{
		int sec = std::floor(static_cast<float>(pos) / 8);
		int rest = pos % 8;
		char bit = 128 >> rest;
		char new_bit = (val * 128) >> rest;
		byte_arr[sec] = (byte_arr[sec] & (~bit)) | new_bit;
	}
};

class huffman_decoder
{
public:
	std::shared_ptr<node> top_node;
	huffman_decoder(std::shared_ptr<node> top_node):top_node(std::move(top_node)){}
	void decode_file()
	{
		std::cout << "\n DECODING...\n\nDECODED TEXT: ";
		std::ifstream file("out.txt",std::ios::ate | std::ios::binary);
		std::stringstream buffer;

		file.seekg(0, file.end);
		size_t size = file.tellg();
		file.seekg(0, file.beg);

		char* char_arr = new char[size];
		file.read(char_arr, size);
		unsigned long int endPos = static_cast<unsigned int>(char_arr[0]) << 24;
		endPos |= static_cast<unsigned int>(char_arr[1]) << 16;
		endPos |= static_cast<unsigned int>(char_arr[2]) << 8;
		endPos |= static_cast<unsigned int>(char_arr[3]);
		int pos = 32;
		bool end = false;
		std::shared_ptr<node> current_node = top_node;
		while (!end)
		{
			bool foundChar = false;
			while(!foundChar){
				bool bit = read_from_bit(char_arr, pos);
				if(bit == true)
					current_node = current_node->right_node;
				else
					current_node = current_node->left_node;
				

				if (current_node->is_char) {
					std::cout << current_node->character;
					foundChar = true;
				}
				
				pos++;
			}
			current_node = top_node;
			if (pos >= endPos)
				end = true;
		}
	}

	bool read_from_bit(char* byte_arr, int pos)
	{
		int sec = std::floor(static_cast<float>(pos) / 8);
		int rest = pos % 8;
		char bit = 128 >> rest;
		return (byte_arr[sec] & bit) != 0;
	}
};

int main()
{
	huffman_encoder enc;
	enc.generate_tree();
	huffman_decoder dec(enc.node_queue.at(0).second);
	dec.decode_file();
	return 0;
}

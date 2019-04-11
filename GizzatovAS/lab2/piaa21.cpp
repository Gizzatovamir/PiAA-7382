#include <iostream>
#include <map>
#include <list>
#include <cmath>
#include <string>
#include <algorithm>
class Graph{
	bool find_point(std::list<char>& points_list, char point){
		for(auto i=points_list.begin();i!=points_list.end();i++){
			if(*i==point)
				return true;
		}
		return false;
	}
	std::map<char,std::map<char,double>> list;
public:
	void add_way(char from, char to, double length){
		if(from==to)
			return;
		std::pair<char,double> way(to,length);
		if(list.find(from)==list.end()){
			std::map<char,double> l;
			l.insert(way);
			list.insert(std::pair<char,std::map<char,double>>(from,l));
			//std::cout<<list.begin()->first<<' '<<(list[from]).begin()->first<<std::endl;
			return;
		}
		list[from].insert(way);
	}
	int heuristic(char a,char b){
		return abs(a-b);
	}
	double find_way_cost(char from, char to){
		if(list.find(from)==list.end())
			return -1.0;
		auto i=list[from].find(to);
		//std::cout<<(list[from])[to]<<std::endl;
		if(i==list[from].end())
			return -1.0;
		return i->second;
	}
	bool has_any_way(std::list<char> &points_hist, std::list<char> &deadlocks, char point){
		for(auto i=list[point].begin();i!=list[point].end();i++){
			if(!find_point(points_hist,i->first) && !find_point(deadlocks,i->first))
				return true;
		}
		return false;
	}
	void print_list(std::list<char> a){
		for(int i = 0; i < a.size();i++){
			std::cout <<"элемент очереди - " << a.front() << std::endl;
			a.pop_front();
		}
	}
	void find_min_way(char from, char to){
		std::list<char> points_hist, deadlocks;
		char cur=from,next;
		double min;
		points_hist.push_front(cur);
		while(cur!=to){
			bool flag=true;
			if(!has_any_way(points_hist,deadlocks,cur)){
				//std::cout<<cur<<" тупик\n";
				deadlocks.push_back(cur);
				std::cout << "добавляем вершину в список тупиков - " << cur << std::endl;
				points_hist.pop_back();
				cur=points_hist.back();
				continue;
			}
			for(auto i=list[cur].begin();i!=list[cur].end();i++){
				if(flag && !find_point(points_hist,i->first) && !find_point(deadlocks,i->first)){
					min=i->second;
					next=i->first;
					flag=false;
					continue;
				}
				//std::cout<<cur<<"->"<<i->first<<std::endl;
				if(i->second<min && !find_point(points_hist,i->first) && !find_point(deadlocks,i->first)){
					next=i->first;
					std::cout << "points_hist" << std::endl;
			                //print_list(points_hist);

				}
			}
			cur=next;
			points_hist.push_back(cur);
		}
		std::cout << "deadlocks" << std::endl;
		print_list(deadlocks);
		/*std::cout << "points_hist" << std::endl;
		print_list(points_hist);*/
		//print_list(points_hist);
		for(auto i=points_hist.begin();i!=points_hist.end();i++){
			std::cout<<(*i);
		}
		std::cout<<std::endl;
	}


};

int main(){
	Graph a;
	char start,end,from,to;
	std::cin>>start>>end;
	double length;
	while(std::cin>>from>>to>>length){
		a.add_way(from, to, length);
	}
	a.find_min_way(start,end);
	return 0;
}

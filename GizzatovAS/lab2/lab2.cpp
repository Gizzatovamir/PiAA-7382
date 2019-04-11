#include <iostream>
#include <map>
#include <queue>
#include <cmath>
#include <string>
#include <algorithm>
class Graph{
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
			return 0.0;
		auto i=list[from].find(to);
		//std::cout<<(list[from])[to]<<std::endl;
		if(i==list[from].end())
			return 0.0;
		return i->second;//+heuristic(from,to);
	}
	void print_q(std::priority_queue <std::pair<double, char>,
		std::vector < std::pair<double, char> >,
		std::greater< std::pair<double, char>>> q){
		std::cout<<"Queue:\n";
		while(!q.empty()){
			std::cout<< "вершина в очереди: " << q.top().second<<",эвристка + длина пути "<<q.top().first<<std::endl;
			q.pop();
		}
		std::cout<<std::endl;

	}
	double get_cost(std::string way){
		double sum=0.0;
		for(auto i=0;i<way.length()-1;i++){
			sum+=find_way_cost(way[i],way[i+1]);
		}
		return sum;
	}

	void is_adm(std::string way){
		bool is_admis=true,is_mon=true;
		char to=*way.rbegin();
		for(auto i=0;i<way.length()-1 && is_admis;i++){
			auto cost=get_cost(way.substr(i));
			if(heuristic(way[i],to)>cost){
				is_admis=false;
				break;
			}
			if(heuristic(way[i],to)>find_way_cost(way[i],way[i+1])+heuristic(way[i+1],to))
				is_mon=false;
		}
		if(is_admis){
			std::cout<<"Функция допустима!\n";
			if(is_mon){
				std::cout<<"Функция монотонна!\n";
			}
			else{
				std::cout<<"Функция не монотонна!\n";
			}
		}
		else
			std::cout<<"Функция не допустима и не монотонна!\n";
	}

	void find_min_way(char from, char to){
		std::priority_queue <std::pair<double, char>,
		std::vector < std::pair<double, char> >,
		std::greater< std::pair<double, char>>> q;

		//if(list.find(from)==list.end()) //|| list.find(to)==list.end())
		//	return;
		q.push({0,from});
		//std::cout<<"strart ";
		std::map<char, double> way_cost;
		std::map<char,char> point_hist;
		while(!q.empty()){
			auto cur = q.top();
			q.pop();
			if(cur.second==to)
				break;
			for(auto i=list[cur.second].begin();i!=list[cur.second].end();i++){
				double new_cost=way_cost[cur.second]+find_way_cost(cur.second,i->first);
				if(!way_cost.count(i->first) || new_cost<way_cost[i->first]){
					way_cost[i->first]=new_cost;
					std::cout<<cur.second<<"->"<<i->first<<' '<< new_cost<<std::endl;
					q.push({new_cost+heuristic(i->first,to),i->first});
					point_hist[i->first]=cur.second;
				}
				print_q(q);
			}

		}
		std::string answer;
		answer.push_back(to);
		while (answer.back() != from)
			answer.push_back(point_hist[answer.back()]);
		std::reverse(answer.begin(), answer.end());
		is_adm(answer);
		for (auto e : answer)
			std::cout << e;
		std::cout<<std::endl;
	}

};

int main(){
	Graph a;
	//int i;
	char start,end,from,to;
	std::cin>>start>>end;
	double length;
	while(std::cin>>from){
		std::cin>>to>>length;
		a.add_way(from, to, length);
		//i--;
	}
	a.find_min_way(start,end);
	return 0;
}

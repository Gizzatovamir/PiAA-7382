#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <set>


class Compare{																			//Класс-компоратор для сортировки пар в очереди с приоритетом
public:
	bool operator()(const std::pair<char, char> &a,const std::pair<char, char> &b) const
	{
		return abs(a.first-a.second)>abs(b.first-b.second);
	}
};


typedef std::priority_queue <std::pair<char, char>,
		std::vector < std::pair<char, char>>,
		Compare> queue;									//объявление очереди с приоритетом

std::set<std::pair<char,char>> input;					//объявление котейнера для ребер, поданных на вход

class Graph{
	std::vector<std::vector<std::pair<int,int>>> matr;  //объявление матрица смежности графа
	std::vector<char> v;
	int next_way(char start, char end){					//нахождение потока через поданый путь
		queue q;
		std::set<char> visited;							//список посещенных вершин
		std::map<char,char> way;						//текущий путь
		char cur=start;
		visited.insert(cur);
		find_neighbour(q,visited,start, end);

		while(!q.empty()){									
			cur=q.top().second;

			visited.insert(cur);
			way[cur]=q.top().first;
			q.pop();
			if(cur==end){
				int min=min_stream(way,start, end);			//находим поток
				recount_stream(way, min, start, end);				//пересчитываем проходящий поток
				return min;
			}
			find_neighbour(q,visited,cur, end);
		}
		return 0;
	}

public:
	Graph(){}
	void add_way(char start, char end, int stream){						//добавления ребра в граф
		int fr,t;
		fr=get_index(start);
		if(fr==-1)
			v.push_back(start);
		t=get_index(end);
		if(t==-1)
			v.push_back(end);
		if(matr.size()<v.size()){
			matr.resize(v.size());
			for(int i=0;i<matr.size();i++){
				matr[i].resize(matr.size());
			}
		}
		matr[get_index(start)][get_index(end)]={stream,0};
	}
	int get_index(char el){										//получение индекса вершины
		for(int i=0;i<v.size();i++){
			if(v[i]==el)
				return i;
		}
		return -1;
	}
	void print_matr(){											//печать матрицы
		std::cout<<"   ";
		for(int i=0;i<v.size();i++)
			std::cout<<v[i]<<"   ";
		std::cout<<std::endl;
		for(int i=0;i<matr.size();i++){
			std::cout<<v[i]<<" ";
			for(int j=0;j<matr[i].size();j++)
				std::cout<<matr[i][j].first<<"/"<<matr[i][j].second<<' ';
			std::cout<<std::endl;
		}
	}
	int get_cur_stream(char start, char end){							//максимально возможный поток через данное ребро
		if(get_index(start)==-1 || get_index(end)==-1)
			return -1;
		return matr[get_index(start)][get_index(end)].first-matr[get_index(start)][get_index(end)].second;
	}

	void find_neighbour(queue &q,std::set<char> &visited, char el,char end){			//нахождение инцидентных вершин, которые не были посещены и добавляет их в очередь
		if(get_cur_stream(el,end)>0){
			while(!q.empty()){
				q.pop();
			}
			q.push({el,end});
			return;
		}
		for(int k=0;k<v.size();k++){

			if(get_cur_stream(el,v[k])<=0 || visited.find(v[k])!=visited.end())
				continue;
			q.push({el,v[k]});
			visited.insert(v[k]);
		}
	}
	void recount_stream(std::map<char,char> &way, int stream, char start, char end){				//пересчет потока проходящего через ребра
		char cur=end;
		while(cur!=start){
			matr[get_index(way[cur])][get_index(cur)].second+=stream;
			matr[get_index(cur)][get_index(way[cur])].second-=stream;
			cur=way[cur];
		}
	}

	int min_stream(std::map<char, char> &way,char start, char end){							//поиск минимального потока в пути
		int min=get_cur_stream(way[end],end);
		char cur=way[end];
		std::string str;
		str.push_back(cur);
		str.push_back(end);
		while(cur!=start){
			if(get_cur_stream(way[cur],cur)<min)
				min=get_cur_stream(way[cur],cur);
			cur=way[cur];
			str.insert(str.begin(),cur);

		}

			std::cout<<"Путь: "<<str<<std::endl
			<<"Поток пути: "<<min<<std::endl << std::endl;
		return min;
	}

	void find_stream(char start, char end){ 								//поиск максимального потока в графе
		int max_gr_stream=0,min_way_stream=0;
		min_way_stream=next_way(start, end);
		while(min_way_stream>0){								
			max_gr_stream+=min_way_stream;					//увеличиваем максимальный поток на величину потока пути
			min_way_stream=next_way(start, end);
		}
		std::cout<<max_gr_stream<<std::endl;
		print_streams();
	}
	void print_streams(){										//печать реальных потоков в изначальных ребрах
		for(auto k:input){
			if(matr[get_index(k.first)][get_index(k.second)].second>=0)
				std::cout<<k.first<<' '<<k.second<<' '<<matr[get_index(k.first)][get_index(k.second)].second<<std::endl;
			else
				std::cout<<k.first<<' '<<k.second<<' '<<0<<std::endl;
		}
	}
	void print_q(queue q){								//печать очереди с приоритетом
		std::cout<<"Ребра в очереди:\n";
		while(!q.empty()){
			std::cout<<q.top().first<<"->"<<q.top().second<<std::endl;
			q.pop();
		}
		std::cout<<std::endl;
	}
};

int main(){
	Graph a;
	int n,stream;
	char start,end,first,second;
	std::cin>>n>>start>>end;
	for(int i=0;i<n;i++){
		std::cin>>first>>second>>stream;
		input.insert(std::pair<char,char>(first,second));
		a.add_way(first,second,stream);
	}
	a.find_stream(start,end);
	return 0;
}

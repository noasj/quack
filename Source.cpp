#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

typedef wchar_t delim;

struct range
{
	delim start;
	delim end;
	range()
	{
		start = L'\0';
		end = L'\0';
	}
	range(delim _b, delim _e)
	{
		start = _b;
		end = _e;
	}
};

struct asdf
{
	delim bound;
	int id;
	asdf(wchar_t _bound, int _id)
	{
		bound = _bound;
		id = _id;
	}
	bool operator<(struct asdf& b)
	{
		return this->bound < b.bound;
	};
};

int chew(vector<struct asdf>& out, const vector<range> in)
{
	int cid = 0;
	for (auto it : in)
	{
		out.push_back(asdf(it.start, cid));
		out.push_back(asdf(it.end, cid));
		cid++;
	}
	sort(out.begin(), out.end());
	return cid;
}

struct arg
{
	struct range r;
	void set_start(delim _start)
	{
		r.start = _start;
	}
	void set_end(delim _end)
	{
		r.end = _end;
	}
	vector<int> ids;
};
//typedef vector<int> id_list;

struct id_list : public vector<int>
{
	id_list(int len) : vector<int>(len)
	{
		fill(this->begin(), this->end(), 0);
	}
	id_list(initializer_list<int> len) : vector<int>(len)
	{
		return;
	};
	void reset()
	{
		fill(this->begin(), this->end(), 0);
	}
	void add(vector<int> addend)
	{
		auto it_a = this->begin();
		auto it_b = addend.begin();
		while (it_a != this->end())
		{
			*(it_a) = *(it_a)+*(it_b);
			it_a++; it_b++;
		}
	}
	bool is_zeros()
	{
		return all_of(this->begin(), this->end(), [](int x){return (x % 2) == 0;});
	}
};

void convert(vector<int>& out, id_list ids)
{
	out.clear();
	int i = 0;
	for (auto g : ids)
	{
		if ((g % 2) == 1) out.push_back(i);
		i++;
	}
}

int main(int argc, char** argv)
{
	auto a = range(L'a', L'j');
	auto d = range(L'f', L'f');
	auto b = range(L'm', L'y');
	auto c = range(L'm', L'z');
	id_list test({ 0 , 1 , 2, 3 });
	id_list test2({ 1,2,3,4 });
	test.add(test2);

	vector<struct range> nice = { a,b, c, d };

	vector<struct asdf> food;
	int last_id = chew(food, nice);
	for (auto g : food)
	{
		wcout << g.bound << L" : " << g.id << endl;
	}

	auto it = food.begin();
	id_list active_ids(last_id);
	id_list tmp_fin_ids(last_id);
	bool HAS_END = false;
	struct arg pair;
	vector<struct arg> spider_list;

	pair.set_start(it->bound);
	bool passthrough = true;
	//convert(spider.ids, active_ids);
	//it++;
	bool cheese;
	while (it != food.end() - 1)
	{
		bool coll = false;
		if (passthrough) cheese = false;
		else cheese = true;
		//if (it->bound == (it + 1)->bound) one of the cooler lines
		while (it->bound == (it + 1)->bound && it != food.end() - 1)
		{
			coll = true;
			//both the id starts and ends are part of the partition the ending bits don't get set until the spider is dispatched
//			if (passthrough) passthrough = false;
			if (active_ids[it->id] == 0) // first encounter
			{
				if (cheese) //early dispatch
				{
					cheese = false; //dispatch'd now we only need to worry about the next time we dispatch which happen in either our off list or the 
					pair.set_end(it->bound - 1);
					convert(pair.ids, active_ids); //very important that we save the state and dispatch the spider before manipulating it
					spider_list.push_back(pair);
					//
					pair.set_start(it->bound);
				}
				active_ids[it->id]++; //we don't need to worry about this one, there were no active id's to keep track of so building the state is fine
			}
			else if (active_ids[it->id] == 1) //encountered either before the block or during
			{
				tmp_fin_ids[it->id] = 1;
				HAS_END = true;
			}
			it++;
		}
		if (cheese && active_ids[it->id] == 0) //only run once now that we have delicious cheese we just need to update the current id after we dispatch
		{
			pair.set_end(it->bound - 1);
			convert(pair.ids, active_ids);
			spider_list.push_back(pair);
			//
			//this is crazy broken
			//now it's crazy fixed yo
			pair.set_start(it->bound);
		}
		else if (HAS_END || active_ids[it->id] == 1) //if it fails the second check it can end up here, the last bit of data can be handled independently
		{
			HAS_END = false;
			pair.set_end(it->bound);
			convert(pair.ids, active_ids);
			spider_list.push_back(pair);
			if (coll) active_ids.add(tmp_fin_ids); //all but the last id are accounted for
			tmp_fin_ids.reset();
			pair.set_start(it->bound + 1);
		}
		passthrough = false;
		active_ids[it->id]++; //this must be completed by all routes
		it++;
	}

	pair.set_end(it->bound);
	convert(pair.ids, active_ids);
	spider_list.push_back(pair);

	vector<struct arg> output(spider_list.size());
	auto cap = copy_if(spider_list.begin(), spider_list.end(), output.begin(), [](struct arg x) { return !x.ids.empty(); });
	output.resize(distance(output.begin(),cap));


	for (auto g : output)
	{
		wcout << g.r.start << L"-" << g.r.end << L": ";
		for (auto l : g.ids)
		{
			wcout << l << L",";
		}
		wcout << endl;
	}
	return 0;
}
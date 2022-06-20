#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>
#include <iterator>

using namespace std;



class Rectangle
{
	public:
		int x1, x2, y1, y2, index, color;

	Rectangle()
	{

	}
};

vector <Rectangle> rectangles;
vector <int> idx;

const int MAX = 10001;
int intersectionGraph[MAX][MAX];
int coloring[MAX];
int subSet[MAX];
int n;

set <int> cover;


//Function to check whether a given string is a number or not.
bool is_number(string &str, int n)
{
	int i=0;
	if(str[0]=='-' || str[0]=='+')
		i=1;
    for (;i<n;i++) 
        if (isdigit(str[i]) == 0)
            return false;
	
    return true;
}

//Function to check heights of rectangles
bool compare(const Rectangle &r1, const Rectangle &r2)
{
	return abs(r1.y1-r1.y2) > abs(r2.y1-r2.y2);
}


//Checking whether a subset of rectangles is clique or not
bool isClique(int s)
{
	int i,j;

	for(i=1;i<=s;i++)
		for(j=i+1;j<=s;j++)
			if(intersectionGraph[subSet[i]][subSet[j]]==0)
				return false;

	return true;
}


//Printing first l elements in the subset
void printSubSet(int l)
{
    for (int i = 1; i <=l; i++)
        cout << subSet[i] << " ";
    cout <<endl;
}


//Function to find out max size of the clique
int maxCliqueSize(int i, int s)
{
	int w=0; 
	
    //Adding the next possible rectangle
    for (; i <= n; i++) 
	{
		//Adding a rectangle to the subset
    	subSet[s] = i;
    
        // Checking if the current subset is clique and adding the next rectangles.
        if (isClique(s)) 
		{
            w= max(w, s);
            w= max(w, maxCliqueSize(i+1, s+1));
        }
    }

	return w;
}


//Function to find out the rectangles that satify alpha covering
void alphaCovering(int s, int alpha, set <int> sp)
{ 
	int top,bottom;
	for(int i=1;i<=s;i++)
	{
		top=0;
		bottom=0;

		//Checking which rectangles satisfy the alpha covering. 
		for(int j=1;j<=s;j++)
		{
			//
			if(i!=j)
			{
				//Counting the number of rectangles intersecting on the top
				if(rectangles[idx[subSet[j]]].y1 <= rectangles[idx[subSet[i]]].y2 && rectangles[idx[subSet[i]]].y2 <= rectangles[idx[subSet[j]]].y2 && rectangles[idx[subSet[i]]].x1 <= rectangles[idx[subSet[j]]].x2 && rectangles[idx[subSet[j]]].x1 <= rectangles[idx[subSet[i]]].x2)
					top++;

				//Counting the number of rectangles intersecting on the top
				if(rectangles[idx[subSet[j]]].y1 <= rectangles[idx[subSet[i]]].y1 && rectangles[idx[subSet[i]]].y1 <= rectangles[idx[subSet[j]]].y2 && rectangles[idx[subSet[i]]].x1 <= rectangles[idx[subSet[j]]].x2 && rectangles[idx[subSet[j]]].x1 <= rectangles[idx[subSet[i]]].x2)
					bottom++;
			}
		}
		//Checking if rectangle satisfies alpha covering
		if(top>=alpha && bottom>=alpha)
			cover.insert(subSet[i]);
	}	
}


//Finding all possible cliques of size s in the set sp
void findCliques(int i, int s, int size, int alpha, set <int> sp)
{
	int count=1;
	set<int>::iterator itr;

	//Finding cliques of size s in the set sp
	for(itr = sp.begin(); count<=i-1; itr++)
		count++;
	for(; itr != sp.end(); itr++)
 	{
		if(i > sp.size() - (size - s))
			break;

		else
		{
			subSet[s] = *itr;

			//Checking if current subset is a clique
			if (isClique(s))
			{
				if (s < size)
					findCliques(i+1, s+1, size, alpha, sp);        	
            
				else
					alphaCovering(s, alpha, sp);
			}
		}
		i++;
	 }
}


//Printing rectangles index and coordinates
void printRectangles(vector<Rectangle> rectangles)
{
	for(auto rect : rectangles)
		cout<<"Index: "<<rect.index<<"  ("<< rect.x1<<","<<rect.x2<<")  ("<<rect.y1<<","<<rect.y2<<")"<<endl;
}


//printing the Intersection graph of the rectangles
void printIntersectionGraph()
{
	for(int i=1;i<=n;i++)
	{
		for(int j=1;j<=n;j++)
			cout<<intersectionGraph[i][j]<<" ";

		cout<<endl;
	}
}


// Printing all the k+1 partitons that is made
void printpartitions(vector <set <int> > partitions)
{
	cout<<"\nPartitions:"<<endl;
	int i;
	//Printing partitions(R') from 1 to k
	for(i=0;i<partitions.size()-1;i++)
	{
		cout<<"R'"<<i+1<<" :";
		for(auto it : partitions[i])
			cout<<" "<<it;
		cout<<endl;
	}

	//Printing the final partition
	cout<<"R"<<partitions.size()-1<<" :";
	for(auto it : partitions[i])
			cout<<" "<<it;

	cout<<endl;
}


//Coloring the rectangle in each set
void colorRectangles(vector <set <int> > partitions, int ch)
{
	int i,j,c=ch;
	set<int>::iterator itr, itr1;
	
	for(i=0;i<partitions.size();i++)
	{
		if(!(partitions[i].empty()))
		{
		
			//First available is assigned to first rectangle of each set
			coloring[*(partitions[i].begin())] = c+1;
			rectangles[idx[*(partitions[i].begin())]].color=c+1;
		
			//Depicting that remaining rectangles in the set are uncolored
			itr=partitions[i].begin();
			itr++;
			for (;itr!=partitions[i].end();itr++)
				coloring[*(itr)] = -1; 
		
			//An array that contains all available colors.
			bool available[partitions[i].size()+1];
			for (j=0;j<partitions[i].size();j++)
				available[j] = false;	//False indicates none of it's adjacent are colored with the color j

			//Assigning colors rest of the rectangles in the set one in each iteration
			itr=partitions[i].begin();
			itr++;
		
			for (;itr!=partitions[i].end();itr++)
			{
	
				//All the adjacent rectangle colors will be made unavailable
				for(itr1=partitions[i].begin(); itr1!=partitions[i].end();itr1++)
					if(intersectionGraph[*(itr)][*(itr1)] == 1 && coloring[*(itr1)]!=-1)
						available[coloring[*(itr1)]-c]=true;

        		//Lookin for the first available color
        		for (j=1; j < partitions[i].size(); j++)
           			if (available[j] == false)
						break;
				
        		coloring[*(itr)] = c+j;
				rectangles[idx[*(itr)]].color = c+j;
				ch= max(ch,c+j);

				//Re definining all the colors to available before coloring next rectangle
				for(itr1=partitions[i].begin(); itr1!=partitions[i].end();itr1++)
					if(intersectionGraph[*(itr)][*(itr1)] == 1 && coloring[*(itr1)]!=-1)
						available[coloring[*(itr1)]]=false;
			}
		}
		c=ch;
	}
	
}


//printing colors of each rectangle
void printcolors()
{
	cout<<"\nColoring:"<<endl;
	for(int i=1;i<=n;i++)
		cout<<"Rectangle "<<i<<" : "<<coloring[i]<<endl;
}


//Checking if coloring is correctly done
void checkColoring()
{
	for(int i=1; i<n; i++)
		for(int j=i+1; j<=n;j++)
			if(intersectionGraph[i][j]==1 && coloring[i] == coloring[j])
			{
				cout<<"\nWrong Coloring"<<endl;
				return;
			}

	cout<<"\nColored properly"<<endl;


}


int main()
{

	int i=0, j=0, w, k, count, c;
	string str, file;
	
	vector <set <int> > v, s, t, partitions;
	vector <set <pair <int,int> > > witnessPoints;

	set <int> r, temp0, temp1;
	set <pair <int,int> > temp;

	/*
	//Reading input from command line. Input n Rectangles.
	//For each axis parallel rectangle we read left most x-coordinate (x1), right most x-coordinate (x2), bottom most y-coordinate (y1), top most y-coordinate (y2)
	while(1)
    {
        //Reading input line
        getline(cin,str);

        //Stops taking inputs if we give the empty line
        if(str.empty())
            break;
	
        //Validating the Input (Input is valid only if we give 4 integers in all other cases input is Invalid)
        else
        {
	    	vector<string> temp;
	    	string word;
	    	stringstream ss;

            ss << str;

            while(ss >> word)
                temp.push_back(word);
	    
            if(temp.size()!=4)
            	continue;
	    
            else
            {
				j=0;
                for(auto itr : temp)
				{
                    if(!is_number(itr, itr.size()))
					{
                        j++;
						break;
					}
				}
				
				if(j==0)
				{
					Rectangle newRec;				
                	newRec.x1=stoi(temp[0]), newRec.x2=stoi(temp[1]), newRec.y1=stoi(temp[2]), newRec.y2=stoi(temp[3]);
					newRec.index=++i;
					if(newRec.x1 <= newRec.x2 && newRec.y1 <= newRec.y2)
						rectangles.push_back(newRec);
				}
			}
        }
    }
	n=i; //n indicates number of rectangles
	*/
	

	//Reading Input from a file
	cin>>file; //Read the file name

	//Opening the file for reading
  	ifstream inputFile(file);
	
	//Checking if the file name is correct
	if(!inputFile)
	{
		cout<<"File not found"<<endl;
		inputFile.close();
		return 0;
	}

  	// Reading the file line by line till the end and validating the Input (Input is valid only if we give 4 integers in all other cases input is Invalid)
  	while (getline (inputFile, str)) 
	{
		vector<string> temp;
	    string word;
	    stringstream ss;

        ss << str;

        while(ss >> word)
        	temp.push_back(word);
	    
        if(temp.size()!=4)
        	continue;
	    
        else
        {
			j=0;
            for(auto itr : temp)
			{
                if(!is_number(itr,itr.size()))
                {
					j++;
					break;
				}
			}

			if(j==0)
			{
				Rectangle newRec;				
            	newRec.x1=stoi(temp[0]), newRec.x2=stoi(temp[1]), newRec.y1=stoi(temp[2]), newRec.y2=stoi(temp[3]);
				if(newRec.x1 <= newRec.x2 && newRec.y1 <= newRec.y2)
				{
					newRec.index=++i; //Rectangles are indexed from 1 to n but they are stored in the 
					rectangles.push_back(newRec);
				}
					
			}
        }
  	}
	// Close the file
  	inputFile.close();
	n=i;
	
	//Intializing corresponding Intersection Graph. Stored in the respective indexes of intersection graph from 1 to n ignoring 0.
	for(auto rect1 : rectangles)
		for(auto rect2 : rectangles)
			if(rect1.x1<=rect2.x2 && rect2.x1<=rect1.x2 && rect1.y1<=rect2.y2 && rect2.y1<=rect1.y2)
				intersectionGraph[rect1.index][rect2.index]=intersectionGraph[rect2.index][rect1.index]=1;

	//printIntersectionGraph();
	
	
	//Sorting rectangles based on their heights.
	sort(rectangles.begin(), rectangles.end(), compare);
	
	//printRectangles(rectangles); 

	
	//idx will give the position of particular rectangle in rectangles vector. idx[6] will give the sixth rectangle's index in rectangles vector.
	idx = vector<int>(n+1, 0);
	i=0;
	for(auto rect : rectangles)
		idx[rect.index]=i++;

	//Prints the order of rectangles in rectangles vector based on their heights
	//print(idx,1,n);

	/*
	
	for(i=1;i<=n;i++)
		cout<<idx[i]<<" ";
	cout<<"\n";
	*/
	
	//w stores max size of clique.
	w=maxCliqueSize(1,1);
	k=ceil(log2(w));

	//cout<<"Width of Rectangles:"<<w<<" and k="<<k<<endl;

	
	//Initializing starting set s with 1,2...n indices of rectangles and witness points set of each rectangle.
	s.push_back({});
	for(i=1;i<=n;i++)
		s[0].insert(i);

	//Defining vertical intersection set of each rectangle from 1 to n in 1 to n indices of v
	v = vector <set <int> > (n+1,{});
	for(i=0;i<n;i++)
		for (j=0;j<i;j++)
			if(rectangles[i].x1<=rectangles[j].x2 && rectangles[j].x1<=rectangles[i].x2 && rectangles[i].y1>=rectangles[j].y1 && rectangles[i].y2<=rectangles[j].y2)
				v[rectangles[i].index].insert(rectangles[j].index);
		
	/*
	//Printing V(R) of each rectangle R
	for(i=1;i<=n;i++)
	{
		for(auto r : v[i])
			cout<<r<<" ";
		cout<<endl;
	}
	*/

	
	//Initializing witness points set of each rectangle for the intial s set. Witness point of each rectangle will be stored from 1 to n indices for 1 to n rectangles respectively
	witnessPoints = vector <set <pair <int, int> > > (n+1,{});
	for(i=1;i<=n;i++)
	{
		//Adding top two extreme corners of a rectangle as its witness point
		witnessPoints[i].insert({rectangles[idx[i]].x1,rectangles[idx[i]].y2});
		witnessPoints[i].insert({rectangles[idx[i]].x2,rectangles[idx[i]].y2});
		for(auto it : v[i])
		{
			//Adding the intersections points of each rectangle with its vertical intersecting rectangles
			if(rectangles[idx[i]].x1<=rectangles[idx[it]].x1)
				witnessPoints[i].insert({rectangles[idx[it]].x1,rectangles[idx[i]].y2});
			if(rectangles[idx[i]].x2>=rectangles[idx[it]].x2)
				witnessPoints[i].insert({rectangles[idx[it]].x2,rectangles[idx[i]].y2});
		}
	}

	/*
	//Printing starting witness points of n Rectangles;
	for(i=1;i<=n;i++)
	{
		for(auto it : witnessPoints[i])
			cout<<it.first<<","<<it.second<<"  ";
		cout<<endl;
	}
	*/
	
	
	//Partitioning into set S
	//Iterating the loop for k times because we have to partition for k levels
	for(i=1;i<=k;i++)
	{
		//Each set from the previous level is split into 2 sets, which will give pow(2,i) sets in a particular level
		for(j=1;j<=pow(2,i);)
		{
			int parent=((pow(2,i)-1+j-1)-1)/2; // Since it's a binary tree and it is stored in an array, parent node index is (p-1)/2 of a node s
			for(auto rect : rectangles)
			{
				if(s[parent].find(rect.index)!=s[parent].end())
				{
					//Splitting the set into two sets based on the witness point rule
					for(auto wp : witnessPoints[rect.index])
					{
						count=0;
						for(auto it : temp0)
							if(v[rect.index].find(it)!=v[rect.index].end())
								if(wp.first>=rectangles[idx[it]].x1 && wp.first<=rectangles[idx[it]].x2 && wp.second>=rectangles[idx[it]].y1 && wp.second<=rectangles[idx[it]].y2)
									count++;

						if(count>=pow(2,k-i))
							temp.insert({wp.first, wp.second});
					}
					
					if(temp.empty())
						temp0.insert(rect.index);
					else
					{
						temp1.insert(rect.index);
						witnessPoints[rect.index]=temp;
					}
					temp.clear();		
				}
				
			}
			s.push_back(temp0);
			s.push_back(temp1);
			temp0.clear();
			temp1.clear();

			j+=2;
		}
	}

	/*
	//Printing Set s
	for(i=0;i<pow(2,k+1)-1;i++)
	{
		cout<<"S"<<i<<": ";
		for(auto it : s[i])
			cout<<it<<" ";
		cout<<endl;
	}
	*/
	
	
	//Creating set t
	t.push_back({});
	for(i=1;i<=k;i++)
	{
		for(j=1;j<=pow(2,i);j++)
		{
			int pos=pow(2,i)-1+j-1;
			//Creating a set of all rectangles such that it satisfies the covering relation in the set S
			for(c=pow(2,k-i+2)+1;c<=pow(2,k);c++)
				findCliques(1, 1, c ,pow(2,k-i+2), s[pos]);
			t.push_back(cover);
			cover.clear();
		}
	}

	/*
	//Printing Set t
	for(i=0;i<pow(2,k+1)-1;i++)
	{
		cout<<"T"<<i<<": ";
		for(auto it : t[i])
			cout<<it<<" ";
		cout<<endl;
	}
	*/

	
	//After Dividing the set based on the rules we merge all t sets of each level into one partition. 
	r=s[0];
	for(i=1;i<=k;i++)
	{
		partitions.push_back({});
		
		for(j=1;j<=pow(2,i);j++)
		{
			int pos = pow(2,i)-1+j-1;
			//All the elements that are removed from s set (T')
			set_intersection(t[pos].begin(), t[pos].end(), r.begin(), r.end(), inserter(temp0, temp0.begin()));	
			
			//Union of all removed elements in a level (All T' of a level)
			set_union(partitions[i-1].begin(), partitions[i-1].end(), temp0.begin(), temp0.end(), inserter(partitions[i-1], partitions[i-1].begin()));
			temp0.clear();

			//Union of all removed elements of a particular level
			set_union(t[pos].begin(), t[pos].end(), temp1.begin(), temp1.end(), inserter(temp1, temp1.begin()));	
		}	
	
		//Removing the t elements sets 
		set_difference(r.begin(), r.end(), temp1.begin(), temp1.end(), inserter(temp0, temp0.begin()));
		r=temp0;

		temp0.clear();
		temp1.clear();
	}
	partitions.push_back(r);
	
	printpartitions(partitions);

	colorRectangles(partitions,0);

	printcolors();

	checkColoring();
	
	return 0;
}

#include <iostream>
#include <string.h>
#include <stdarg.h>
#include <vector>

using namespace std;

#define crash(...) internal_crash(__LINE__,__FILE__,__VA_ARGS__)

//crash reporting the expanded error message
void internal_crash(int line,const char *file,const char *templ,...)
{
  fflush(stdout);
  fflush(stderr);
  
  //expand error message
  char mess[1024];
  va_list ap;
  va_start(ap,templ);
  vsprintf(mess,templ,ap);
  va_end(ap);
  
  fprintf(stderr,"\x1b[31m" "ERROR on line %d of file \"%s\", message error: \"%s\".\n\x1b[0m",line,file,mess);
  
  exit(1);
}

/// List of gamma used for source or sink
struct LocBilinear
{
  const vector<int> list;
  
  const char letter;
  
  static int CliffOfChar(const char& g)
  {
    static constexpr char CliffMap[8]="SVPATBG";
    
    for(int i=0;i<7;i++)
      if(CliffMap[i]==g)
	return
	  i;
    
    crash("Cannot convert gamma: %c",g);
    
    return {};
  }
  
  static vector<int> getList(const char& g,
			     const char& letter)
  {
    static const vector<int> numeric[7]={
      {0},
      {4,1,2,3},
      {5},
      {9,6,7,8},
      {10,11,12},
      {13,14,15},
      {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}};
    
    static const vector<int> literal[7]={
      {0},
      {1,2,3},
      {5},
      {6,7,8},
      {10,11,12},
      {13,14,15},
      {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}};
    
    constexpr static bool ignoreLetter[7]=
		{1,0,1,0,0,0,1};
    
    const int c=
      CliffOfChar(g);
    
    if(ignoreLetter[c] or not isdigit(letter))
      return
	literal[c];
    
    const int i=
      letter-'0';
    
    if(i<0 or i>=4)
      crash("Error, letter %c converts to int %d not in range [0:3]",letter,i);
    
    return
      {numeric[c][i]};
  }
  
  LocBilinear(const char& g,
	    const char& letter)
    : list(getList(g,letter)),letter(letter)
  {
  }
};

vector<pair<int,int>> combine(const vector<pair<LocBilinear,LocBilinear>>& in)
{
  vector<pair<int,int>> out;
  
  for(const auto& i : in)
    {
      const auto& a=
	i.first;
      
      const auto& b=
	i.second;
      
      if(a.letter!=b.letter)
	for(const int& ai : a.list)
	  for(const int& bi : b.list)
	    out.push_back({ai,bi});
      else
	{
	  if(a.list.size()!=b.list.size())
	    crash("Error, sizes of a %lu does not agree with size of b %lu",a.list.size(),b.list.size());
	  
	  for(size_t i=0;i<a.list.size();i++)
	    out.push_back({a.list[i],b.list[i]});
	}
    }
  
  return
    out;
}

pair<LocBilinear,LocBilinear> decrypt(const char* combo)
{
  if(strlen(combo)!=4)
    crash("Error, string length %lu different from 4",strlen(combo));
  
  return
    {{combo[0],combo[1]},{combo[2],combo[3]}};
}

void print(const vector<pair<int,int>>& in)
{
  for(const auto i : in)
    cout<<i.first<<" "<<i.second<<endl;
}

int main()
{
  vector<pair<LocBilinear,LocBilinear>> list;
  
  string str;
  while(cin>>str)
    list.push_back(decrypt(str.c_str()));
  
  print(combine(list));
  
  return 0;
}

#include<iostream>
#include<fstream>
#include<string>
#include<stack>
#include<ctime>
#include<climits>
using namespace std;
#define INTMIN -65000

int n,p;
double timeleft; 
int prune;


struct node{
char **arr;
int mycount;
int oppcount;
int movei;
int movej;
int depth;

};    

node generateNode(){
    char **arr = new char*[n];
	//pointer initialization
    for(int i=0;i<n;i++)
	 arr[i]=new char[n];
	 node n1;
     n1.arr = arr;
     n1.mycount = 0;
     n1.oppcount = 0;
     n1.depth = 0 ;
     n1.movei=-1;
     n1.movej=-1;
     	 
 return n1;
} 
   
void printBoard(char **arr){
       for(int i=0;i<n;i++){
		   for(int j=0;j<n;j++){
		      cout<<arr[i][j];
		  }cout<<endl;
          }	
          cout<<endl;
          
}       
void gravity(char ** arr){
for(int j=0;j<n;j++)
for(int i=n-1; i>=0 ;i--)
     {    
         if (arr[i][j]=='*')
         {
          int k = i ;
          while(k>=0 && arr[k][j]=='*')
            k--;
          if(k>=0){
          arr[i][j]=arr[k][j];
          arr[k][j]='*';   
         }    
        }    
     }      
}    

int getNextFruit(char ** arr, int* i ,int* j){
for(int x= 0 ; x<n ;x++)
for(int y = 0 ;y<n;y++)
  {
      if(arr[x][y]!='*')
        {
            *i = x;
            *j = y;
            return 1; 
        }    
  }    
  return 0;
}    
void select(char ** arr, int i,int j, char fruit, int *count){

 if(i>=0&&i<n&&j>=0&&j<n){

 if(arr[i][j]==fruit){
   arr[i][j]='*';
   *count = *count +1;
   select(arr,i+1,j,fruit,count);
   select(arr,i,j+1,fruit,count) ;
   select(arr,i,j-1,fruit,count) ;
   select(arr,i-1,j,fruit,count);
  }

 }
} 

void setArray(char** arr1, char**arr2)
{
    for(int i=0;i<n;i++)
   for(int j=0;j<n;j++)
     arr2[i][j] = arr1[i][j];
}    


void findMaxMove(int** scoreArr, int *ideali, int *idealj){

int max = INTMIN;

for(int i=0 ; i<n ;i++){
 for(int j=0;j<n;j++)
  {
  if(scoreArr[i][j] > max){
  	max = scoreArr[i][j];
  	*ideali = i;
  	*idealj = j;
  }
  }
}
	
}

void getMaxfruits(char** a,int*ideali ,int*idealj){

  int i,j;
   int max=0;
   int count;
 
  while(getNextFruit(a,&i,&j)){
  
   select(a,i,j,a[i][j],&count);
   
   if(max<count){
   max = count;
    *ideali = i;
    *idealj = j;
   }
  }
       
}

void dfs(char** a,int *ideali,int*idealj){
   
   stack<node> S;
   node root = generateNode();
   for(int i=0;i<n;i++)
   for(int j=0;j<n;j++)
   root.arr[i][j] = a[i][j];
   
   S.push(root);
   
    char **reuseArray = new char*[n];
	//pointer initialization
    for(int i=0;i<n;i++)
		reuseArray[i]=new char[n];


   int **scoreArray = new int*[n];
	//pointer initialization
    for(int i=0;i<n;i++)
    scoreArray[i]=new int[n];
	
  for(int i =0; i<n;i++)
   for(int j=0;j<n;j++)
    scoreArray[i][j] = INTMIN;
    
    int diff ;
    int i,j;
    int curri=-1,currj=-1;
    int firstmovei=0;
    int firstmovej=0;
 
   char selectedFruit; 
   
  clock_t timer = clock();
  clock_t timer2;
 
  while(1){
   
  timer2 = clock() - timer;
  
   if( (timeleft - ((double)timer2)/CLOCKS_PER_SEC) < 0.1 ){
   
  getMaxfruits(a, ideali, idealj);
    return ;
  } 
   
   
   //stack empty then return   
   if(S.empty())
     break;
   
   node n = S.top();
   S.pop();
 
   //depth of 3 reached then compare ideal moves 
   if(n.depth == 3 || !getNextFruit(n.arr,&i,&j)){
    
   int maxscore = n.mycount;
   int minscore = n.oppcount; 
   
     //first move 
    if(curri==-1&&currj==-1){
     firstmovei = n.movei;
     firstmovej = n.movej;
    curri=n.movei;
    currj=n.movej;
    diff= maxscore-minscore; 
     prune = diff;
    }    
    
      //minimize current move
    else if(curri==n.movei&&currj==n.movej){
    if((maxscore-minscore) < diff){
     diff=maxscore-minscore;
    
     
     if(curri==firstmovei && currj==firstmovej)
       prune = diff;
     
    }
     
     if(curri==0 && currj ==0 )
        scoreArray[0][0]=diff;
    }
    //explore new move 
    else
    {
     
         curri=n.movei;
         currj= n.movej;
         diff= maxscore - minscore;   
    }    
    
    scoreArray[curri][currj]=diff;
    
   }    
   
   
   //else keep searching
   
   if(n.depth < 3){
   setArray(n.arr ,reuseArray);
   
   int flag=getNextFruit(reuseArray,&i,&j);
 
   
   while(flag){
    node newnode = generateNode();
    setArray(n.arr,newnode.arr);
    
    newnode.depth = n.depth +1;
    
    if(newnode.depth == 1)
    {
        newnode.movei = i;
        newnode.movej = j;
    }    
    
    else 
    {
        newnode.movei = n.movei;
        newnode.movej = n.movej;
    }    
    
    selectedFruit= newnode.arr[i][j];

    int count = 0;
 	
    select(reuseArray,i,j,selectedFruit,&count);
 	
    count = 0;
 	
    select(newnode.arr,i,j,selectedFruit,&count);
 	 
    flag = getNextFruit(reuseArray,&i,&j);
    
    gravity(newnode.arr);
    
    
    if((newnode.depth)%2){
    newnode.mycount = n.mycount + count*count;
    newnode.oppcount = n.oppcount;
    }    
    else{
    newnode.mycount = n.mycount ;
    newnode.oppcount = n.oppcount + count*count;
    }    
  
 	if(newnode.depth==1)
      S.push(newnode);
   
   else if(newnode.mycount-newnode.oppcount > prune)
               S.push(newnode); 

    
   }//endwhile inner
    
   }//endif    	

  }//endwhile main
  
 
 findMaxMove(scoreArray,ideali,idealj);
  
}   


int main()
{

 	string line;
	ifstream infile("input.txt");
    
    cin>>n>>p>>timeleft;
 
     char **a = new char*[n];
	//pointer initialization
    for(int i=0;i<n;i++)
		a[i]=new char[n];
	
   char **toprint = new char*[n];
	//pointer initialization
    for(int i=0;i<n;i++)
		toprint[i]=new char[n];
      
 
 
  if (infile.is_open())
  { 
    int i=0;
    while ( getline (infile,line) )
    {
      for(int j=0;j<n;j++){
      a[i][j] = line[j];
      toprint[i][j]= line[j];
      }         
     i++;
    } 
    infile.close();

  }
 
 int ideali;
 int idealj;
 int count=0;

  dfs(a,&ideali,&idealj);

 
  select(toprint,ideali,idealj,toprint[ideali][idealj],&count);
  gravity(toprint);
 
 
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++){
          cout<<toprint[i][j];
        }
 
 return 0;
}   


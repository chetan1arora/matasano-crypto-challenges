#include <random>
#include <iostream>


using namespace std;

int main(){

mt19937 mt_rand(1);

for(int i=0;i<5;i++){
cout<<mt_rand()<<endl;
}
}

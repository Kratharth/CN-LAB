#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<bits/stdc++.h>
#include <unistd.h>
using namespace std;
#define bucketsize 512

void bucketInput(int size, int rate){
	
	if(size > bucketsize)
		cout<<"Bucket overflow "<<endl;
	else{
		usleep(500);
		while(size > rate){
			cout<<rate<<" Bytes output"<<endl;
			size -= rate;
			usleep(500);
		}
		if(size > 0)
			cout<<"Last "<<size<<" bytes sent"<<endl;
		cout<<"Bucket output successfull"<<endl;
	}
}

int main(){
	int op, pktsize;
	cout<<"Enter output rate :";
	cin>>op;
	for(int i=1; i<=5; i++){
		usleep(rand()%1000);
		pktsize = rand()%1000;
		cout<<"Packet no "<<i<<" Packet size = "<<pktsize<<endl;
		bucketInput(pktsize, op);
	}
}

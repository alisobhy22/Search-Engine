#pragma once
#include<vector>
#include<iostream>
#include<string>
using namespace std;

struct website_node {
	string url;
	int id;
	double score;
	double impressions;
	double click_throughs;
	double ctr;
	vector<string> keywords;
	double page_rank;
};

struct Edge
{
	website_node source;
	website_node destination;
};

class Graph
{
public:
	int size;
	vector<vector<website_node>> adjlist;
	vector<website_node> websites;
public:
	Graph(int s, vector<Edge> const &edges, vector<website_node> w);
	void print();
	void  print_website_contents();
	void get_pageranks();
	void get_scores();
	void quickSort(double arr[], int low, int high);
	int partition(double arr[], int low, int high);
};


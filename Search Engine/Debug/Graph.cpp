#include "Graph.h"

Graph::Graph(int s, vector<Edge> const &edges, vector<website_node> w)
{
    
	size = s;
    adjlist.resize(s);
    for (int i = 0; i < edges.size(); i++)
    {
        adjlist[edges[i].source.id].push_back(edges[i].destination); //adjlist
    }
    websites = w; 
    get_pageranks();
    get_scores();
}

void Graph::print()
{
    for (int i = 0; i < size; i++)
    {
        cout << i << " : ";
        for (int j = 0; j < adjlist[i].size(); j++)
            cout << adjlist[i][j].id << " ";
        cout << endl;
    }
}

void Graph::print_website_contents()
{
    for (int i = 0; i < websites.size(); i++)
    {
        cout << "WEBSITE NUMBER " << i + 1 << ':' << endl << "URL: " << websites[i].url << endl;
        cout << "ID: " << websites[i].id << endl << "Impressions: " << websites[i].impressions << endl;
        cout << "Click through: " << websites[i].click_throughs << endl << "CTR: " << websites[i].ctr << endl;
        cout << "Keywords: ";
        for (int j = 0; j < websites[i].keywords.size(); j++)
            cout << websites[i].keywords[j] << "  ";
        cout << endl;
    }

}

void Graph::get_pageranks()
{
    for (int i = 0; i < size; i++)
         websites[i].page_rank = 1.0 / websites.size(); //calculates intial pr  = 1/n
    double* previous_iteration = new double[size]; 
    for (int it = 0; it < 5; it++) //5 iterations for accurate results (iterations should increase depending on size of n)
    {
        for (int i = 0; i < size; i++)
            previous_iteration[i] = websites[i].page_rank; //stores previous iteration in array to be used for the rest of the iteration
        for (int i = 0; i < size; i++) //loops over all websites
        {
            double sum_of_pageranks = 0; //sum of pr for this website
            for(int x = 0; x < size;x++)
                for(int y = 0; y < adjlist[x].size();y++) //loop over adjlist
                    if (websites[i].id == adjlist[x][y].id) //if the website we are calcualting is found to be pointed at by another node
                    {
                        sum_of_pageranks = sum_of_pageranks + (previous_iteration[x] / adjlist[x].size()); 
                        //adds sum by adding the found node's previous pr / size of that node's path (how many outgoing edges)
                    }
            websites[i].page_rank = sum_of_pageranks; //assigns website page rank to the sum
        }
    }
    for (int i = 0; i < size; i++)
         previous_iteration[i] = websites[i].page_rank; //updates previous iteration to match with final pr
        int i, j;
        quickSort(previous_iteration,0, size - 1); //quick sort from highest to lowest
        for (int i = size; i > 0; i--) //two for loops responsible to assign ranks to website nodes according to its order in the sorted array
        {
            
            for (int j = 0; j < websites.size(); j++)
            {
                if (previous_iteration[i - 1] == websites[j].page_rank)
                    websites[j].page_rank = size - i +1;
            }
        }

}

void Graph::get_scores()
{
    for (int i = 0; i < websites.size(); i++)
    {
        websites[i].score = 0.4 * websites[i].page_rank + ((1 - ((0.1 * websites[i].impressions) / (1 + 0.1 * websites[i].impressions))) * websites[i].page_rank + ((0.1 * websites[i].impressions) / (1 + 0.1 * websites[i].impressions)) * websites[i].ctr) * 0.6;
    }
}

void Graph::quickSort(double arr[], int low, int high)
{
    
        if (low < high)
        {
            /* pi is partitioning index, arr[p] is now
            at right place */
            int pi = partition(arr, low, high);

            // Separately sort elements before  
            // partition and after partition  
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    
}

int Graph::partition(double arr[], int low, int high)
{
   
        double pivot = arr[high]; // pivot  
        int i = (low - 1); // Index of smaller element  

        for (int j = low; j <= high - 1; j++)
        {
            // If current element is smaller than the pivot  
            if (arr[j] > pivot)
            {
                i++; // increment index of smaller element  
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);
        return (i + 1);
   
}

#include <iostream>
#include<vector>
#include<string>
#include<fstream>
#include "Graph.h"
using namespace std;


vector<website_node> get_websites(); //creates a vector of websites_nodes from any of the files (this will be the main vector that will be pushed into the graph and will be my main data type)
void input_impressions(vector<website_node> &websites); //updates vector by adding impressions
void input_click_through(vector<website_node>& websites); //updates vector by adding click_through
void input_keywords(vector<website_node>& websites); //updates vector by adding keywords
void calculate_ctr(vector<website_node>& websites); //calculates ctr for vector of websites
Graph create_graph(vector<website_node>& websites); //creates graph from vector of websites and given edges in file
Graph get_input_graph(); //creates the whole graph by calling all the above function
vector<website_node> search_websites(string search, Graph& web_graph); //takes a string (search from the user), splits the string into keywords and returns websites found that fits specifications of string
void sort_by_rank(vector<website_node> &un_sorted); //sorts the vector of websites found to show the highest rank first (used in previous function)
void increment_impression(website_node &website); //incrementes impression every time it shows up as a result
void increment_click_through(string url, Graph & web_graph); //increments click_through every time someone visits website
void save_impressions_and_clickthroughs(Graph web_graph); //saves the changed data to file


int main()
{
    Graph web_graph = get_input_graph(); //creates graph
    int answer;
    bool exited = false;
    cout << "Welcome!" << endl << "What would you like to do?" << endl << "1. New Search" << endl << "2. Exit" << endl << endl << "Type in your choice:";
    cin >> answer;
    while ((answer != 1) && (answer != 2))
    {
        cout << "invalid, input, please check the possible options and try again!" << endl << endl << "Type in your choice:";
        cin >> answer;
    }
    
    if (answer == 1)
    {
        while (1<2) //infinite loop that can only be exited when break; is called
        {
            system("cls"); //clears console for neatness
            calculate_ctr(web_graph.websites); //calcultes ctr after every search since impressions and click throughs update
            web_graph.get_scores(); //gets the score
            string empty;
            getline(cin, empty); // gets rid of the empty string that gets inputed for some reason (BUGGED as the first search will always be an empty string)
            string search;
            cout << "Search: ";
            getline(cin,search);
            vector<website_node> results =  search_websites(search, web_graph); //returns vector of websites that match with the query of user
            cout << endl << endl << "results:" << endl << endl; 
            for (int i = 0; i < results.size(); i++)
                cout << i+1 << ".  " << results[i].url << endl; //printing of URL
            cout << endl << "Would you like to:" << endl << "1.  Choose a webpage to open" << endl << "2.  New Search" << endl << "3.  Exit" << endl;
            cout << "Type in your choice:";
            cin >> answer;
            while ((answer != 1) && (answer != 2) && (answer != 3))
            {
                cout << "Invalid input! Try again." << endl;
                cout << endl << "Would you like to:" << endl << "1.  Choose a webpage to open" << endl << "2.  New Search" << endl << "3.  Exit" << endl;
                cout << "Type in your choice:";
                cin >> answer;
            }
            if (answer == 3) 
                break; //exit
            if (answer == 1)
            {
                cout << endl << "Choose a webpage to open!" << endl << endl << "Type in your choice:";
                    cin >> answer;
                    while ((answer < 1) || (answer > results.size()))
                    {
                        cout << "Invalid input! Try again." << endl;
                        cout << endl << "Choose a webpage to open!" << endl << endl << "Type in your choice:";
                        cin >> answer;
                    }
                    system("cls"); //clears console for neatness
                    cout << "You are now viewing " << results[answer - 1].url << endl << "Would you like to" << endl << "1.  New Search" << endl << "2.  Exit" << endl;
                    increment_click_through(results[answer-1].url,web_graph); //increase click through since it was visited
                    cout << "Type in your choice:";
                    cin >> answer;
                    while ((answer != 1) && (answer != 2))
                    {
                        cout << "invalid, input, please check the possible options and try again!" << endl << endl << "Type in your choice:";
                        cin >> answer;
                    }
                    if (answer == 2)
                        break; //exit
                    system("cls"); //neatness


            }


        }
    }
   else
   {
        //don't have to save here since no searches were made
        system("pause");
        return 0;
       
   }
    cout << "Thanks for using my engine!!" << endl;
    save_impressions_and_clickthroughs(web_graph); //saving
    return 0;
}

vector<website_node> get_websites()
{
    vector<website_node> result;
    ifstream input;
    input.open("keywords.csv");
    if (input.fail())
    {
        cout << "Error, terminating";
        exit(0);
    }
    int id = 0;
    while (!input.eof())
    {
        
        string line;
        input >> line;
        website_node temp;
        temp.url = "";
        for (int i = 0; line[i] != ','; i++)
            temp.url = temp.url + line[i];
        temp.id = id;
        id++;
        result.push_back(temp);
    }
    input.close();
    return result;
}
void input_impressions(vector<website_node> &websites)
{
    ifstream input;
    input.open("impressions.csv");
    if (input.fail())
    {
        cout << "Error, terminating";
        exit(0);
    }
    double impression;
    string line;
    
    for (int i = 0; i < websites.size(); i++)
    {
        input >> line;
        int j = 0;
        while (line[j] != ',')
            j++;
        j++;
        
        string imp = "";
        for (j; j < line.size(); j++)
            imp = imp + line[j];
        impression = stod(imp);
        websites[i].impressions = impression;

    }
}
void input_click_through(vector<website_node>& websites)
{
    ifstream input;
    input.open("click_through.csv");
    if (input.fail())
    {
        cout << "Error, terminating";
        exit(0);
    }
    double click_through;
    string line;

    for (int i = 0; i < websites.size(); i++)
    {
        input >> line;
        int j = 0;
        while (line[j] != ',')
            j++;
        j++;

        string click = "";
        for (j; j < line.size(); j++)
            click = click + line[j];
        click_through = stod(click);
        websites[i].click_throughs = click_through;

    }
}
void input_keywords(vector<website_node>& websites)
{
    ifstream input;
    input.open("keywords.csv");
    if (input.fail())
    {
        cout << "Error, terminating";
        exit(0);
    }
    string line;

    for (int i = 0; i < websites.size(); i++)
    {
        input >> line;
        int j = 0;
        while (line[j] != ',')
            j++;
        j++;
        for(j;j<line.size();)
        {
            
            string keyword = "";
            while ((line[j] != ',') &&(j<line.size()))
            {
                keyword = keyword + line[j];
                j++;
            }
            j++;
            websites[i].keywords.push_back(keyword);
        }
    }
}
void calculate_ctr(vector<website_node>& websites)
{
    for (int i = 0; i < websites.size(); i++)
    {
        websites[i].ctr = (websites[i].click_throughs / websites[i].impressions) * 100;
    }
}
Graph create_graph(vector<website_node>& websites)
{
    vector<Edge> edges;
    ifstream input;
    input.open("graph.csv");
    if (input.fail())
    {
        cout << "Error, terminating";
        exit(0);
    }
    website_node* source = NULL; 
    website_node* destination = NULL;
    while (!input.eof())
    {
        string line;
        input >> line;
        string source_url = "";
        string destination_url = "";
        int i = 0;
        while (line[i] != ',')
        {
            source_url = source_url + line[i];
            i++;
        }

        i++;
        while (i != line.size())
        {
            destination_url = destination_url + line[i];
            i++;
        }
        for (int i = 0; i < websites.size(); i++) //finds proper website that matches url in text file, points source and destination to it
        {
            
            if (source_url == websites[i].url)
            {
                
                source = &websites[i];
               
            }
            if (destination_url == websites[i].url)
            {
                destination = &websites[i];
                
            }
        }
        Edge temp;
        temp.source = *source;
        temp.destination = *destination;
        edges.push_back(temp);
    }
    Graph web_graph(websites.size(), edges,websites);
    return web_graph;
}
Graph get_input_graph()
{
    vector<website_node> websites = get_websites();
    input_impressions(websites);
    input_click_through(websites);
    input_keywords(websites);
    calculate_ctr(websites);
    Graph web_graph = create_graph(websites);
    return web_graph;
}

vector<website_node> search_websites(string search, Graph& web_graph) 
{
    vector<website_node>results;
    if (search == "") //if empty string (in case enter was pressed by accident while searching so it doesn't crash)
        return results;
    vector<string> type; //type vector which contains commands like AND, OR, and DONE signifying the search is done
    vector<string> keywords; //vector containing keywords
    int i = 0;
    while (i < search.size())
    {
            string keyword = "";
            while ((search[i] != ' ') && (i < search.size())) //adds characters to keyword until it meets space or end of line (end of word)
            {
                keyword = keyword + search[i];
                i++;
               
            }
            keywords.push_back(keyword); //pushes keywords into vector
        
        if (i + 3 < search.size()) //checks if there are still more then 3 characters to go
        {
            if ((search[i + 1] == 'A') && (search[i + 2] == 'N') && (search[i + 3] == 'D')) // checks if AND is present after keyword
            {
                type.push_back("AND");
                i = i + 5;
            }
            else if ((search[i + 1] == 'O') && (search[i + 2] == 'R')) //checks if OR is present
            {

                type.push_back("OR");
                i = i + 4;
            }
            else //assumes to be OR
            {
                type.push_back("OR");
                i++;
            }
        }
    }
    type.push_back("DONE");
    for (int i = 0; i < keywords.size(); i++) //now we compare the keywords with our web graph
    {
        if (type[i] == "AND") //If we have an AND between i and i+1 keyword
        {
            for (int l = 0; l < web_graph.websites.size(); l++)
            {
                
                bool found[2] = { false,false }; //two values of both keywords are found in each website (must both be true in order for website to be pushed)
                for (int j = 0; j < web_graph.websites[l].keywords.size(); j++)
                    if (keywords[i] == web_graph.websites[l].keywords[j]) //if keyword 1 is found
                        found[0] = true;
                for (int j = 0; j < web_graph.websites[l].keywords.size(); j++)
                    if (keywords[i+1] == web_graph.websites[l].keywords[j]) //if keyword 2 is found
                        found[1] = true;
                if ((found[0] == true) && (found[1] == true))
                {
                    results.push_back(web_graph.websites[l]);
                    increment_impression(web_graph.websites[l]); //increments impression since it will be printed
                }
            }
        }
        else if (type[i] == "OR") //If we have an OR between i and i+1 keyword
        {
            for (int l = 0; l < web_graph.websites.size(); l++)
            {
                bool pushed = false; //bool function so websites don't get pushed more than once, used to break out of for loop of keywords since it will be printed either way
                for (int j = 0; j < web_graph.websites[l].keywords.size(); j++)
                {
                    if (keywords[i] == web_graph.websites[l].keywords[j])
                    {
                        results.push_back(web_graph.websites[l]);
                        pushed = true;
                        increment_impression(web_graph.websites[l]);
                    }
                    else if (keywords[i + 1] == web_graph.websites[l].keywords[j])
                    {
                        results.push_back(web_graph.websites[l]);
                        pushed = true;
                        increment_impression(web_graph.websites[l]);
                    }
                    if (pushed == true)
                        break;
                }
                
                   
            }
        }
        else if (type[i] == "DONE") //if there is only one keyword or we have one keyword left
        {
            for (int l = 0; l < web_graph.websites.size(); l++)
            {
                for (int j = 0; j < web_graph.websites[l].keywords.size(); j++)
                    if (keywords[i] == web_graph.websites[l].keywords[j])
                    {
                        results.push_back(web_graph.websites[l]);
                        increment_impression(web_graph.websites[l]);
                    }
            }
        }
        if (type[i] != "DONE") //if there was AND or OR, we already dealt with i+1, so we don't need to loop on it again
            i++;
    }
    if(results.size() > 0) //if the result isn't empty
    sort_by_rank(results); //sort
    return results;
}

void sort_by_rank(vector<website_node> &un_sorted)
{
    int i, j; //bubble sort
    for (i = 0; i < un_sorted.size() - 1; i++)
        for (j = 0; j < un_sorted.size() - i - 1; j++)
            if (un_sorted[j].page_rank < un_sorted[j + 1].page_rank)
                swap(un_sorted[j], un_sorted[j+1]);
}
void increment_impression(website_node& website)
{
    website.impressions++;
}
void increment_click_through(string url, Graph& web_graph)
{
    for (int i = 0; i < web_graph.websites.size(); i++)
        if (url == web_graph.websites[i].url)
            web_graph.websites[i].click_throughs++;
}
void save_impressions_and_clickthroughs(Graph web_graph)
{
    ofstream output1("impressions.csv", ofstream::trunc);
    for (int i = 0; i < web_graph.websites.size(); i++)
        output1 << web_graph.websites[i].url << ',' << web_graph.websites[i].impressions << endl;
    output1.close();

    ofstream output2("click_through.csv", ofstream::trunc);
    for (int i = 0; i < web_graph.websites.size(); i++)
        output2 << web_graph.websites[i].url << ',' << web_graph.websites[i].click_throughs << endl;
    output2.close();
}
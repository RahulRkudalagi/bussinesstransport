#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <climits>
#include <queue>
#include <stack>
#include<time.h>
#include<iomanip>

using namespace std;

#define M_SIZE 10
#define V 10
#define d 256
#define m 999

/** Structure for routes and vehicles list (array implementation)**/
struct CP {
    int distance;
    int seats;
    int fare;
    int start;
    int end;
    string type;
};

/**structure for finding pollution of a particular vehicle(list implementation)**/
struct NODE {
    float p_level;
    string tov;
    NODE* next;
};

/**structure for printing the feedback to the user at the start of the program(simple array implementation)**/
struct Feedback {
    string username;
    float ratings;
    string feedback;
    string timestamp;
};

/**structure for payment after the end of journey(stack implementation)**/
struct PAYMENT {
    string tov[20];
    float cost[20];
    string mode_of_payment[20];
    int top;
};

/**structure for minimum spanning tree(kruskals implementation)**/

struct edge {
    int src;
    int dest;
    int weight;
};

/**structure for entering the feedback by the user(queue implementation)**/
struct myqueue {
    string name[20];
    float rating[20];
    string message[20];
    int front;
    int rear;
};


/**globally declared variables**/
int global_count=0;
CP details[100];
CP result_list[50];
CP final_list[20];
CP choice_list[10];

int visited[10];
int fenwick_tree[30];


/** Function name: create_node
 *  Input Params: none
 *  Return type: NODE*
 *  Description: Creates a node for storing vehicle and its associated pollution type
 */
NODE* create_node() {
    NODE* temp = (NODE*)malloc(sizeof(NODE));
    if (temp == NULL) {
        cout << "INSUFFICIENT MEMORY\n";
        return NULL;
    }
    temp->next = NULL;
    return temp;
}


/** Function name: read_details
 *  Input Params: NODE* t
 *  Return type: NODE*
 *  Description: Reads the details of a vehicle and its pollution level
 */
NODE* read_details(NODE* t) {
    cin >> t->tov >> t->p_level;
    return t;
}

/** Function name: insert_end
 *  Input Params: head, node to be inserted
 *  Return type: NODE*
 *  Description: Inserts the node at the end of the list
 */
NODE* insert_end(NODE* head, NODE* t) {
    NODE* ptr;

    if (head == NULL) {
        head = t;
    }
    else {
        ptr = head;
        while (ptr->next != NULL ) {
            ptr = ptr->next;
        }
        ptr->next = t;
    }
    return head;
}

/** Function name: display_list
 *  Input Params: head
 *  Return type: void
 *  Description: Displays all the vehicles and their associated pollution level
 */
void display_list(NODE* head) {
    if (head == NULL) {
        cout << "Empty list\n";
    }
    else {
        cout << left << setw(15) << "Vehicle" << setw(10) << "Pollution Level" << endl;
        cout << "-------------------------\n";

        NODE* cur = head;
        while (cur != NULL) {
            cout << left << setw(15) << cur->tov << setw(10) << fixed << setprecision(2) << cur->p_level << endl;
            cur = cur->next;
        }
    }
}


/** Function name: bubble_sort_distance
 *  Input Params: number of items in the final list presented to the user
 *  Return type: void
 *  Description: sorts the final_list based on distance to be given by the user
 */
void bubble_sort_distance(int p) {
    int i;
    int j;
    for (i = 0; i < p - 1; i++) {
        for (j = 0; j < p - i - 1; j++) {
            if (final_list[j + 1].distance < final_list[j].distance) {
                CP temp = final_list[j];
                final_list[j] = final_list[j + 1];
                final_list[j + 1] = temp;
            }
        }
    }
}



/** Function name: selection_sort_fare
 *  Input Params: number of items in the final list presented to the user
 *  Return type: void
 *  Description: sorts the final_list based on fare
 */
void selection_sort_fare(int p) {
    int i, j, min;

    for (i = 0; i < p - 1; i++) {
        min = i;
        for (j = i + 1; j < p; j++) {
            if (final_list[j].fare < final_list[min].fare) {
                min = j;
            }
        }
        swap(final_list[min], final_list[i]);
    }
}

/** Function name: insertion_sort_seats
 *  Input Params: number of items in the final list presented to the user
 *  Return type: void
 *  Description: sorts the final_list based on the number of seats available
 */
void insertion_sort_seats(int p) {
    int i, j;
    CP key;

    for (i = 1; i < p; i++) {
        key = final_list[i];
        j = i - 1;

        while (j >= 0 && final_list[j].seats > key.seats) {
            final_list[j + 1] = final_list[j];
            j = j - 1;
        }
        final_list[j + 1] = key;
    }
}

/** Function to display the final_list once it is sorted as required */
void display_sorted_list() {
    cout << "\n\n";
    cout << "DISTANCE SEATS  FARE STARTPOINT END POINT TYPE OF TRANSPORT\n";

    for (int i = 0; i < 8; i++) {
        if (final_list[i].distance != 0) {
            cout << final_list[i].distance << "\t"
                 << final_list[i].seats << "\t"
                 << final_list[i].fare << "\t"
                 << final_list[i].start << "\t"
                 << final_list[i].end << "\t"
                 << final_list[i].type << endl;
        }
    }
}


/** Function name: search_vehicle_pollution
 *  Input Params: head of the list and the vehicle type whose pollution you wish to search
 *  Return type: void
 *  Description: gives the pollution level of the searched vehicle
 */
void search_vehicle_pollution(NODE *head, const string &vehicletype) {
    NODE *cur = head;

    while (cur != nullptr) {
        if (cur->tov == vehicletype) {
            cout << "Vehicle: " << cur->tov << ", Pollution Level: " << fixed << setprecision(2) << cur->p_level << endl;
        }
        cur = cur->next;
    }
}


int rabin_karp(const char text[10], const char opt[10]) {
    int M = strlen(opt);
    int N = strlen(text);
    int Q = 101;
    int i, j;
    int p = 0;
    int t = 0;
    int h = 1;

    for (i = 0; i < M - 1; i++)
        h = (h * d) % Q;

    for (i = 0; i < M; i++) {
        p = (d * p + opt[i]) % Q;
        t = (d * t + text[i]) % Q;
    }

    for (i = 0; i <= N - M; i++) {
        if (p == t) {
            for (j = 0; j < M; j++) {
                if (text[i + j] != opt[j])
                    break;
            }
            if (j == M)
                cout << "Pattern matched. You can search for the vehicle now\n";
        }

        if (i < N - M) {
            t = (d * (t - text[i] * h) + text[i + M]) % Q;
            if (t < 0)
                t = (t + Q);
        }
    }
    return i;
}




void search_vehicle(int p, const char* name)
{
    int found = 0;
    int k = 0;
    vector<int> a;
    int count = 0;

    cout << "The following is/are the options to opt for\n";

    for (int i = 0; i < p; i++)
    {
        if (strstr(final_list[i].type.c_str(), name) != NULL)
        {
            a.push_back(i);  // Add index to vector
            cout << final_list[a[k]].distance << "\t" << final_list[a[k]].seats << "\t"
                 << final_list[a[k]].fare << "\t" << final_list[a[k]].start << "\t"
                 << final_list[a[k]].end << "\t" << final_list[a[k]].type << "\n";
            k++;
            count++;
        }
    }

    if (count == 1)
    {
        cout << "Thus distance to travel in kms in order to reach your desired destination and the vehicle chosen\n";
        cout << "distance: " << final_list[a[0]].distance << "\topteed for: " << final_list[a[0]].type << "\n";
    }
    else if (count > 1)
    {
        cout << "The user always chooses the first option i.e the following\n";
        cout << final_list[a[0]].distance << "\t" << final_list[a[0]].seats << "\t"
             << final_list[a[0]].fare << "\t" << final_list[a[0]].start << "\t"
             << final_list[a[0]].end << "\t" << final_list[a[0]].type << "\n";
        cout << "Thus distance to travel in kms in order to reach your desired destination and the vehicle chosen\n";
        cout << "distance: " << final_list[a[0]].distance << "\topteed for: " << final_list[a[0]].type << "\n";
    }
}




/**Function Name: write_into_file
Input Params: NILL
Return Type: void
Description: writes all the data available with details into appropriate data structure
**/
void write_into_file()
{
    ofstream file("file.txt");

    if (!file.is_open())
    {
        cout << "File open error";
        return;
    }

    int i;
    for (i = 0; i < 97; i++)
    {
        cout << "Enter distance, seats, fare, start, end, and type (separated by spaces): ";
        cin >> details[i].distance >> details[i].seats >> details[i].fare >> details[i].start >> details[i].end;
        cin.ignore(); // Ignore newline before reading the type string
        getline(cin, details[i].type);

        file << details[i].distance << " "
             << details[i].seats << " "
             << details[i].fare << " "
             << details[i].start << " "
             << details[i].end << " "
             << details[i].type << "\n";

        if (cin.fail())
        {
            cout << "Error in input format. Loop terminated.\n";
            break;
        }
    }
    file.close();
}

/**Function Name: load_from_file
Input Params: NILL
Return Type: void
Description: loads all the data available with details into appropriate data structure**/

void read_from_file()
{
    ifstream file("file.txt");

    if (!file.is_open())
    {
        cout << "File Not Found\n";
        return;
    }

    string line;
    cout << "DISTANCE SEATS  FARE STARTPOINT ENDPOINT TYPE OF TRANSPORT\n";
    while (global_count < 100 && getline(file, line))
    {
        istringstream iss(line);

        if (iss >> details[global_count].distance
                >> details[global_count].seats
                >> details[global_count].fare
                >> details[global_count].start
                >> details[global_count].end)
        {
            getline(iss >> ws, details[global_count].type); // Read the remaining type (with leading whitespace skipped)
            cout << details[global_count].distance << "\t"
                 << details[global_count].seats << "\t"
                 << details[global_count].fare << "\t"
                 << details[global_count].start << "\t"
                 << details[global_count].end << "\t"
                 << details[global_count].type << "\n";
            global_count++;
        }
    }

    file.close();
}

/**Function name:search_from_file
Input Params:start point of the journey
Return type:void
Description:prints all pair of vertices with the details having their starting point as s
**/
void search_from_file(int s)
{
    ifstream file("file.txt");

    if (!file.is_open())
    {
        cout << "File Not Found\n";
        return;
    }

    string line;
    cout << "The following shows the\n";
    cout << "DISTANCE SEATS  FARE STARTPOINT ENDPOINT TYPE OF TRANSPORT\n";
    cout << "all starting from " << s << "\n";

    while (global_count < 100 && getline(file, line))
    {
        istringstream iss(line);
        if (iss >> details[global_count].distance
                >> details[global_count].seats
                >> details[global_count].fare
                >> details[global_count].start
                >> details[global_count].end)
        {
            getline(iss >> ws, details[global_count].type); // Extract the type of transport

            if (details[global_count].start == s)
            {
                cout << details[global_count].distance << "\t"
                     << details[global_count].seats << "\t"
                     << details[global_count].fare << "\t"
                     << details[global_count].start << "\t"
                     << details[global_count].end << "\t"
                     << details[global_count].type << "\n";

                result_list[global_count].distance = details[global_count].distance;
                result_list[global_count].seats = details[global_count].seats;
                result_list[global_count].fare = details[global_count].fare;
                result_list[global_count].start = details[global_count].start;
                result_list[global_count].end = details[global_count].end;
                result_list[global_count].type = details[global_count].type;

                global_count++;
            }
        }
    }

    file.close();
}

/**Function name:result
Input Params:start point of the journey,end point of the journey
Return type:int
Description:Returns the shortest distance of those vertices whose start point=s and end point=e
**/
int get_distance_associated(int s, int e)
{
    int i;
    int k = -1;
    for (i = 0; i < 8; i++)
    {
        if (result_list[i].start == s && result_list[i].end == e)
        {
            k = result_list[i].distance;
        }
        else
        {
            continue;
        }
    }
    return k;
}


int all_distances(int s, int e, int k) {
    int i;
    int c = 0;
    int l = 0;
    cout << "\n\n";
    cout << "The following shows the vertices all starting from " << s
         << " and having distance greater than or equal to " << k << endl;
    cout << "DISTANCE SEATS  FARE STARTPOINT END POINT TYPE OF TRANSPORT\n";

    for(i = 0; i < 10; i++) {
        if(result_list[i].start == s && result_list[i].distance >= k) {
            c++;

            cout << result_list[i].distance << "\t"
                 << result_list[i].seats << "\t"
                 << result_list[i].fare << "\t"
                 << result_list[i].start << "\t"
                 << result_list[i].end << "\t"
                 << result_list[i].type << endl;

            final_list[l].distance = result_list[i].distance;
            final_list[l].seats = result_list[i].seats;
            final_list[l].fare = result_list[i].fare;
            final_list[l].start = result_list[i].start;
            final_list[l].end = result_list[i].end;
            final_list[l].type = result_list[i].type;
            l++;
        }
    }

    return c;
}



/**
Function name:floyd_warshall
Input Params:matrix from main
Return type:void
Description:prints all pair shortest path from each vertices
**/
void print_solution(int dist[10][10]);
void floyd_warshall(int dist[10][10])
{
    int i, j, k;


    for (k = 0; k < 10; k++)
        {
        for (i = 0; i < 10; i++)
        {

            for (j = 0; j < 10; j++)
            {

                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }

    print_solution(dist);
}
/**Function name:printsolution
Input Params:matrix from main
Return type:void
Description:prints all pair shortest path from each vertices
**/
void print_solution(int dist[10][10])
{ int i,j;
    printf("\nThe following matrix shows the shortest distances between every pair of vertices(places here)\n");
    printf("*****************************\n");
    for (i = 0; i < 10; i++)
    {
        for (j = 0; j < 10; j++)
        {
            if (i==j)
             printf(" 0");
            else
                printf("%3d", dist[i][j]);
        }
        printf("\n");
    }
    printf("*****************************\n");
}

/**fenwick tree implementation**/

void update(int i, int delta)
{
    i++;
    while (i <= 2)
    {
        fenwick_tree[i] += delta;
        i += i & -i;
    }
}

int query(int i)
{
    int sum = 0;
    i++;
    while (i > 0)
    {
        sum += fenwick_tree[i];
        i -= i & -i;
    }
    return sum;
}

/** Function to simplify the elements of the feedback structure to the user as a user-history record **/
void print_table_header()
{
    cout << left << setw(10) << "Username" << setw(10) << "Ratings" << setw(10) << "Feedback" << setw(15) << "Timestamp" << endl;
    cout << "--------------------------------------------------" << endl;
}

/** Prints the feedback **/
void print_feedback_row(Feedback *feedback)
{
    cout << left << setw(10) << feedback->username << setw(10) << fixed << setprecision(1) << feedback->ratings
         << setw(10) << feedback->feedback << setw(15) << feedback->timestamp << endl;
}


/**Function name:compute_LPS_array
Input Params: pattern
Return type:void
Description:generates the longest suffix and prefix needed for KMP algo
**/
void compute_LPS_array(const char* pattern, int M, int* lps)
{
    int len = 0;
    int i = 1;
    lps[0] = 0;

    while (i < M)
    {
        if (pattern[i] == pattern[len])
        {
            len++;
            lps[i] = len;
            i++;
        }
        else
        {
             if (len != 0)
            {
                len = lps[len - 1];
            }
            else
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}

/**Function name:KMP_search
Input Params: pattern and text(security pin:text)(input string:pattern)
Return type:int
Description:return the sum of indices(if the text and pattern match)
**/

int KMP_search(const char* pattern, const char* text)
{
    int M = strlen(pattern);
    int N = strlen(text);

    if (M == 0 || N == 0)
    {
        return -1;
    }

    int* lps = (int*)malloc(sizeof(int) * M);
    int j = 0;

    compute_LPS_array(pattern, M, lps);

    int i = 0;
        while (i < N)
        {
        if (pattern[j] == text[i])
        {
            j++;
            i++;
        }

        if (j == M)
        {
            free(lps);
            return i - j;
        }
        else if (i < N && pattern[j] != text[i])
        {
            if (j != 0)
            {
                j = lps[j - 1];
            }
            else
            {
                i++;
            }
        }
        }

    free(lps);
    return -1;
}

/**Function name:look_table
Input Params: matrix,look up matrix
Return type:void
Description:searches for time taken to travel betwen any two vertices(places)
**/
void look_up_table(int matrix[10][10], int look_up[10][10])
{
    int i, j;
    for (i = 0; i < 10; i++)
    {
        for (j = 0; j <10; j++)
        {
            look_up[i][j] = matrix[i][j] * 2;
        }
    }
}

/**data look_up**/

int data_look_up(int look_up[10][10], int i, int j)
{
    return look_up[i][j];
}

/**Function name:dfs
Input Params: souce vertex,graph
Return type:void
Description:displays the path the user must visit
**/
void DFS(int vertex,int graph[10][10])
{   int i;
    visited[vertex] = 1;
    printf("%d ", vertex);

    for (i = 0; i < 10; i++)
    {
        if (graph[vertex][i] && !visited[i])
        {
            DFS(i,graph);
        }
    }
}

/**dijktras implementation

/**function to return the min_index**/
int min_distance(int dist[], int arr[])
{
    int min = INT_MAX, min_index,v;

    for (v = 0; v < V; v++)
        if (arr[v] == 0 && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

/**prints the path the user has to take**/
void print_path(int parent[], int j)
{
    if (parent[j] == -1)
        return;

    print_path(parent, parent[j]);

    cout << j;
}

void print(int dist[], int parent[])
{
    int i;
    cout << "Vertex   Distance from Source   Path\n";
    for ( i = 0; i < V; i++)
    {
        cout << left << setw(8) << i << setw(23) << dist[i];
        print_path(parent, i);
        cout << endl;
    }
}

/**Function name:dijktras
Input Params: souce vertex,graph
Return type:void
Description:displays the path the user must visit
**/
void dijkstra(int graph[V][V], int source)
{
    int i,v;
    int dist[V], parent[V];
    int arr[V];
    int count;

    for (i = 0; i < V; i++)
    {
        dist[i] = INT_MAX;
        arr[i] = 0;
        parent[i] = -1;
    }

    dist[source] = 0;

    for (count = 0; count < V - 1; count++)
    {
        int u = min_distance(dist, arr);

        arr[u] = 1;

        for (v = 0; v < V; v++)
            if (!arr[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v])
            {
                dist[v] = dist[u] + graph[u][v];
                parent[v] = u;
            }
    }

    print(dist, parent);
}


/**Function name:bfs
Input Params: souce vertex,graph
Return type:void
Description:displays the path the user must visit
**/
void bfs(int graph[10][10],int source)
{
    int queue[100];

    int front = 0, rear = 0, u, i;

    int visited[10];

    for (i= 0; i < V; i ++)
       visited[i] = 0;

    queue[rear] = source;


    visited[source] = 1;

    printf("The BFS Traversal is \n");
    while (front <= rear)
    {

        u = queue[front];
        printf("%d ", u);
        front = front + 1;

    for(i=0;i<V;i++)
    {
       if(graph[u][i]==1&&visited[i]==0)
       {
        visited[i]=1;

       rear=rear+1;
       queue[rear]=i;

       }
    }
    }
}

//prims implementation
/**fuction to find min_index **/
int min_key(int key[], int mst_set[])
{   int v;
    int min = INT_MAX, min_index;

    for ( v = 0; v < V; v++)
        if (mst_set[v] == 0 && key[v] < min)
            min = key[v], min_index = v;

    return min_index;
}
/**function to print mst**/
void print_mst(int parent[], int graph[V][V])
{   int i;
    int sum=0;
    cout << "Edge \tWeight\n";
    for ( i = 1; i < V; i++)
    {

        sum=sum+graph[i][parent[i]];


    }
    cout << "\nYOU HAVE TO TRAVEL A DISTANCE OF" << sum << " kms IN ORDER TO VISIT ALL THE PLACES AT A MINIMUM DISTANCE\n";


}


/**Function name:prim_mst
Input Params: graph
Return type:void
Description:displays the path the user must visit
**/
void prim_mst(int graph[V][V])
{
    int parent[V],v,count,i;
    int key[V];
    int mst_set[V];

    for ( i = 0; i < V; i++)
    {
      key[i] = INT_MAX;
      mst_set[i] = 0;
    }

    key[0] = 0;
    parent[0] = -1;

    for ( count = 0; count < V - 1; count++)
    {
        int u = min_key(key, mst_set);
        mst_set[u] = 1;

        for ( v = 0; v < V; v++)
            if (graph[u][v] && mst_set[v] == 0 && graph[u][v] < key[v])
                parent[v] = u, key[v] = graph[u][v];
    }

    print_mst(parent, graph);
}

//kruskals implementation//
/**function to create_set**/
void create_set(int a[], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        a[i] = i;
    }
}
/**find function**/
int find(int a[], int u)
{

    return a[u];
}
/**union function**/
void union_sets(int a[], int n, int u, int v)
{int i;
    int temp = a[u];
    for (i = 0; i < n; i++)
    {
        if (a[i] == temp)
        {
            a[i] = a[v];
        }
    }
}

/**Function name:merge and merge sort
Input Params: edges,left,mid,right
Return type:void
Description:sorts the edges for kruskals
**/
void merge(struct edge edges[], int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int i,j,k;

    struct edge L[n1], R[n2];

    for ( i = 0; i < n1; i++)
    {
        L[i] = edges[left + i];
    }
    for ( j = 0; j < n2; j++)
    {
        R[j] = edges[mid + 1 + j];
    }

   i = 0, j = 0, k = left;

    while (i < n1 && j < n2)
    {
        if (L[i].weight <= R[j].weight)
        {
            edges[k] = L[i];
            i++;
        } else {
            edges[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        edges[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        edges[k] = R[j];
        j++;
        k++;
    }
}

void merge_sort(struct edge edges[], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        merge_sort(edges, left, mid);
        merge_sort(edges, mid + 1, right);

        merge(edges, left, mid, right);
    }
}

void sort_edges(struct edge edges[], int edge_count)
{
    merge_sort(edges, 0, edge_count - 1);
}
/**Function name:kruskals
Input Params: souce vertex,graph
Return type:void
Description:displays the shortest non-connected path the user must visit
**/
void kruskals(int graph[V][V])
{
    struct edge edges[V * V];
    int edge_count = 0;
    int i,j;

    for (i = 0; i < V; i++)
    {
        for (j = i + 1; j < V; j++)
    {
            if (graph[i][j] != 0)
            {
                edges[edge_count].src = i;
                edges[edge_count].dest = j;
                edges[edge_count].weight = graph[i][j];
                edge_count++;
            }
        }
    }


   sort_edges(edges, edge_count);
   int sets[V];
   create_set(sets, V);

   int sum = 0;

   for (i = 0; i < edge_count; i++)
    {
        int u = edges[i].src;
        int v = edges[i].dest;

        int setU = find(sets, u);
        int setV = find(sets, v);

        if (setU != setV)
        {


            sum += edges[i].weight;
            union_sets(sets, V, setU, setV);
        }
    }
    cout << "THE PERSON HAS TO TRAVEL A DISTANCE OF" << sum << "kms IN INORDER TO TRAVEL ALL THE PLACES BUT IN A DISCONNECTED WAY.";

}

//queue implementation
/**function to initialize queue**/
void initialize_queue(struct myqueue *q)
{
    q->front = -1;
    q->rear = -1;
}

/**Function name:enqueue
Input Params: queue,name,rating,message
Return type:void
Description:enqueues the feedback details provided by the user as feedback
**/
void enqueue(myqueue &q, const string& name, float rating, const string& message) {
    q.rear++;
    q.name[q.rear] = name;
    q.rating[q.rear] = rating;
    q.message[q.rear] = message;
}
/**displays the queue**/
void display_queue(struct myqueue *q)
{
    int i;

    for (i = q->front; i <= q->rear; i++)
    {
        cout << q->name[i] << "\t" << fixed << setprecision(1) << q->rating[i] << "\t" << q->message[i] << endl;

    }
}

/**function to check if the stack is empty or not**/
int isempty(PAYMENT& stack)
{
    return stack.top == -1;
}

/**function to check if the stack is full or not**/
int is_full(PAYMENT* stack)
{
    return stack->top == M_SIZE - 1;
}


//stack operations are maintained for payment purpose//
/**Function name:push the elements into the stack
Input Params:stack,and the elements to be pushed into the stack
Return type:void
Description:gives the pollution level of searched vehicle
**/

void push(PAYMENT* stack, const string& vehicle, float cost, const string& payment) {

    if (is_full(stack))
    {
        cout << "Stack overflow\n";
        return;
    }

    stack->top++;
    stack->tov[stack->top] = vehicle;
    stack->cost[stack->top] = cost;
    stack->mode_of_payment[stack->top] = payment;
}

/**Function name:pop_cost_of vehicle
Input Params:stack,vehicle name
Return type:float
Description:gives out the cost of the vehicle the user has opted for
**/
float pop_cost_of_vehicle(PAYMENT& stack, const char* vehicle)
{
    if (isempty(stack))
    {
        cout << "Stack underflow.\n";
        return -1;
    }

    int i, j;
    float cost = -1;


    for (i = stack.top; i >= 0; i--)
    {
        if (stack.tov[i] == vehicle)
        {
            cout << "Options available is/are " << stack.mode_of_payment[i] << endl;
            cost = stack.cost[i];


            for (j = i; j < stack.top; j++)
            {
                stack.cost[j] = stack.cost[j + 1];
                stack.tov[j] = stack.tov[j + 1];
                stack.mode_of_payment[j] = stack.mode_of_payment[j + 1];
            }

            stack.top--;
            break;
        }
    }

    return cost;
}
/**Function name:display_stack
Input Params:stack
Return type:float*
Description:copies the cost/km of vehicletype into a vehicle to sort the cost and present it to the user
**/
float* display_stack(PAYMENT stack)
{
    int i;
    cout << left << setw(20) << "Vehicle" << setw(10) << "Cost" << setw(20) << "Mode of Payment" << endl;
    cout << "-----------------------------------------------------\n"<< endl;

    float* cost_array = (float*)malloc((stack.top + 1) * sizeof(float));

    if (cost_array == NULL)
    {

        cout << "Memory allocation failed.\n";
        return NULL;
    }
    for(i=stack.top;i>=0;i--)
    {
        cost_array[i]=stack.cost[i];
        cout << left << setw(20) << stack.tov[i] << setw(10) << fixed << setprecision(1) << stack.cost[i] << setw(20) << stack.mode_of_payment[i] << endl;

    }
    return cost_array;
}
/**function to swap the elements of stack**/
void swap_quick(PAYMENT *stack, int i, int j)
{

    string temp_vehicle = stack->tov[i];
    float temp_cost = stack->cost[i];
    string temp_mode = stack->mode_of_payment[i];


    stack->tov[i] = stack->tov[j];
    stack->cost[i] = stack->cost[j];
    stack->mode_of_payment[i] = stack->mode_of_payment[j];

    stack->tov[j] = temp_vehicle;
    stack->cost[j] = temp_cost;
    stack->mode_of_payment[j] = temp_mode;
}
/**function to partition the stack**/
int partition(PAYMENT* stack, int l, int h)
{
    int j;
    float pivot = stack->cost[h];
    int i = l - 1;

    for (j = l; j <= h - 1; j++)
    {
        if (stack->cost[j] <= pivot)
        {
            i++;
            swap_quick(stack, i, j);
        }
    }

    swap_quick(stack, i + 1, h);
    return i + 1;
}
/**Function name:quick sort
Input Params: stack,input parameters required for quick sort
Return type:void
Description:gives a sorted(decreasing order) stack to the user
**/
void quick_sort(PAYMENT* stack, int l, int h)
{
    if (l < h)
    {
        int pi = partition(stack, l, h);

        quick_sort(stack, l, pi - 1);
        quick_sort(stack, pi + 1, h);
    }
}
/**function to display the stack**/
void display_sorted_stack(PAYMENT stack)
{
    int i;
    cout << left << setw(20) << "Vehicle" << setw(10) << "Cost" << setw(20) << "Mode of Payment" << endl;
    cout << "-----------------------------------------------------\n"<< endl;

    for ( i =stack.top; i>=0; i--)
    {
        cout << left << setw(20) << stack.tov[i] << setw(10) << fixed << setprecision(2) << stack.cost[i] << setw(20) << stack.mode_of_payment[i] << endl;

    }
}

/**Function name:calculate_distances
Input Params: no of elements in final_list,vehicle name
Return type:int
Description:returns distance associated with the vehicle chosen by the user
**/
int calculate_distance(int c, const string& searchType) {
    int found = 0;
    int count = 0;
    int k = -1;

    for (int i = 0; i < c; ++i) {
        if (strstr(final_list[i].type.c_str(), searchType.c_str()) != NULL) {  // Using strstr
            found = 1;
            count++;
            k = i;
        }
    }

    if (count == 1) {
        return final_list[k].distance;
    } else if (count > 1) {
        return final_list[count - 1].distance;
    }

    return -1;
}

int main()
{


        /**all declarations**/
        int matrix[10][10];
        int time_matrix[10][10];
        NODE* head=NULL;
        NODE* t   =NULL;
        int choice;
        int s;
        int e;
        int k;
        int c;
        int n;
        int i;
        int j;
        char tov[15];
        char opt[15];
        PAYMENT stack;
        stack.top = -1;
        float* a;
        int p;
        int source;
        int found;
        /**end of declarations**/

        /**security to the program(KMP implementation)**/
        char security[15]="Rahul@236";
        char pin[15];
        cout << "Enter the security key to use the app\n";
        cout << "Hint:Name@rollno\n";
        cout << "Please enter the password as: Rahul@236\n";
        cin >> pin;
        found = KMP_search(pin, security);
        if (found != -1)
        {
        cout << "Security key matched. Access granted!\n";

        /**structure implementation**/
        struct Feedback list[7] =
        {
        {"A", 4.5, "Best", "2daysago"},
        {"S", 3.0, "Good", "1dayago"},
        {"R", 5.0, "Best", "23hrsago"},
        {"O", 2.0, "Notgood", "15hrsago"},
        {"J", 4.0, "Average", "20daysago"},
        {"T", 3.5, "Average", "12minago"},
        {"VR", 5.0, "Best", "30minago"}
        };
        cout << "\nGiven below are some of the feedback we received recently\n";

        print_table_header();

        for (i = 0; i < 7; i++)
        {
        print_feedback_row(&list[i]);
        }

        /**this graph is taken as input for person who wishes to wanderlust(graph algorithms)**/
        int graph[10][10]= {

                            { 0,  9,  m,  m,  m, 51,  m,  m,  m,  m },
                            { 9,  0, 24,  m,  m,  m, 60,  m,  m,  m },
                            { m, 24,  0, 27,  m,  m,  m,  m,  m,  m },
                            { m,  m, 27,  0, 39,  m,  m,  m,  m,  m },
                            { m,  m,  m, 39,  0,  5,  m,  m, 30, 45 },
                            {51,  m,  m,  m,  5,  0, 27,  m,  m,  m },
                            { m, 60,  m,  m,  m, 27,  0, 45,  m,  m },
                            { m,  m,  m,  m,  m,  m, 45,  0, 24,  m },
                            { m,  m,  m,  m, 30,  m,  m, 24,  0, 42 },
                            { m,  m,  m,  m, 45,  m,  m,  m, 42,  0 }

                            };

       /**Map for an area(represented in the form of a graph)**/
        for (i = 0; i < 10; i++)
        {
        for (j = 0; j < 10; j++)
        {
            if(i==j)
                matrix[i][j]=0;
            matrix[i][j] = rand() % 100 + 1;
        }
        }
        printf("\nGiven below is map for an area which the user is presently in\n");
        printf("*****************************\n");
        for (i = 0; i < 10; ++i)
        {
        for (j = 0; j < 10; ++j)
        {
            if(i==j)
            cout << "0 " ;

            cout << matrix[i][j]<< " ";
        }
        cout << "\n";
        }
        cout << "*****************************\n";
        cout << "\n";

        /**floyd warshall implementation**/
        floyd_warshall(matrix);

        /**user input**/
        /**matrix that tells the time taken to travel betwwen any two places**/
        cout << "Do you wish the time travel matrix between the places you want to visit?press 1 for yes..any other number for no\n";
        cin >> p;

        if(p==1)
        {
             srand(time(NULL));
          for (i = 0; i < 10; i++)
          {
            for (j = 0; j < 10; j++)
             {
              if (i == j)
            {

               time_matrix[i][j] = 0;
            } else
            {
               time_matrix[i][j] = rand() % 100 + 1;
            }
            }
          }
         cout << "The time matrix for the given area is\n";
         cout << "*****************************\n";

         for (i = 0; i < 10; i++)
         {
         for (j = 0; j < 10; j++)
         {
            if(i==j)
            cout << "0 ";
            cout << time_matrix[i][j] << " ";
         }
         cout << "\n";
         }
         cout << "*****************************\n";
       }
         else
         cout << "No i don't need a time matrix\n";

         p=0;

        cout << "\nDo you prefer to travel(tourist)or in a need of immediate emergency\n";
        cout << "PRESS 0 to travel OR 1 FOR IMMEDIATE EMERGENCY\n";
        cin >> p;
        if(p==0)
        {
            cout << "Please prefer the following options available\n";
            while(1)
            {
                cout << "\n******** Menu *************\n";
                cout << "0-IF THE USER WANTS TO SEARCH TIME BETWEEN ANY TWO PLACES\n";/**LOOK UP TABLE**/
                cout << "1-IF THE USER WANTS TO JUST WANDERLUST THE PLACES NOT PARTICULARLY IN NEED OF SHORTEST PATH(wants to find immediate cities)\n";/**DFS**/
                cout << "2-IF THE USER WANTS TO FIND THE SHORTEST PATH FROM A PARTICULAR POINT TO ANY PARTICULAR PLACE WITHOUT ANY DISCONNECTIVITY\n";/**DIJKTRAS**/
                cout << "3-IF THE USER WANTS TO VISIT LEVEL BY LEVEL\n";/**BFS**/
                cout << "4-IF THE USER WANTS TO VISIT IN A CONNECTED WAY WITH SHORTEST PATH BEEN COVERED AND ALL THE PLACES VISITED\n";/**PRIMS**/
                cout << "5-IF THE USER DOES NOT WISH TO TRAVEL IN A CONNECTED WAY BUT WITH SHORTEST PATH BEEN COVERED AND ALL THE PLACES VISITED\n";/**KRUSKALS**/
                cout << "6-LOAD FILE\n";
                cout << "7-READ FILE\n";
                cout << "8-USER INPUT TO GO TO DESIRED LOCATION\n";
                cout << "9-IF THE USER WANTS TO MAKE PAYMENT\n";
                cout << "10-IF THE USER WANTS TO GIVE RATING\n";
                cout << "*****************************\n";
                cout << "Enter your choice\n";
                cin >> choice;

                switch(choice)
                {

                    case 0:{
                            /**Lookup table implementation**/
                            int table[10][10];
                            look_up_table(time_matrix,table);
                            int s,e;
                            printf("Enter any two start and end values(any 2(different)vertices) about which you want to see the time taken\n");
                            scanf("%d %d",&s,&e);
                            printf("Time taken to travel from[%d] to [%d] is : %dminutes\n", s, e, data_look_up(table,s, e));
                            break;
                            }

                    case 1:{

                            /**DFS implementation**/

                            printf("\nEnter the place(vertex)from which you wish to start: ");
                            scanf("%d", &source);

                            for (i = 0; i < 10; i++)
                            {
                                visited[i] = 0;
                            }

                            printf("\nTHE PATH THAT THE USER HAS TO OPT IN ORDER TO REACH THE END DESTINATION%d: ", source);
                            DFS(source,graph);
                            break;
                            }
                    case 2:{
                            /**DIJKTRAS implementation**/
                            printf("Enter the source vertex where which you wish to start the journey\n");
                            scanf("%d",&source);
                            printf("YOU CAN CHOOSE ANY OF THE PATHS GIVEN BESIDES(as path)\n");
                            dijkstra(graph,source);
                            break;
                            }
                    case 3:{
                            /**BFS implementation**/
                            int bfs_graph[10][10] = {
                            {0, 1, 0, 0, 0, 0, 1, 0, 0, 0},
                            {1, 1, 1, 1, 0, 1, 1, 0, 0, 0},
                            {0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                            {0, 1, 1, 0, 1, 0, 0, 0, 0, 0},
                            {0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
                            {0, 1, 0, 0, 1, 0, 1, 0, 1, 0},
                            {1, 1, 0, 0, 0, 1, 0, 1, 0, 0},
                            {0, 0, 0, 0, 0, 0, 1, 0, 1, 0},
                            {0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
                            {0, 0, 0, 0, 1, 0, 0, 0, 1, 0}
                                           };

                            printf("enter the starting place(vertex)from which you wish to start your journey\n");
                            scanf("%d",&source);
                            printf("\nTHE PATH THAT THE USER HAS TO OPT IN ORDER TO REACH THE END DESTINATION%d: ", source);
                            bfs(bfs_graph,source);
                            break;
                            }
                    case 4:{
                            /**PRIMS implementation**/
                            prim_mst(graph);
                            break;

                            }
                    case 5:{
                            /**KRUSKALS implementation**/
                            kruskals(graph);
                            break;
                            }

                    case 6:write_into_file();
                            break;

                    case 7:read_from_file();
                            break;

                    case 8:{
                            j = 0;
                            s = 0;
                            e = 0;
                            cout << "Enter the start(space)end point of the journey(0-9)\n";
                            cout << "CAUTION** START AND END POINT SHOULD NOT BE THE SAME\n";
                            cin >> s >> e;
                            search_from_file(s);
                            k = get_distance_associated(s, e);
                            c = all_distances(s, e, k);
                            if (c == 1)
                            {
                                cout << "There is only one option available to you\n";
                            }
                            else
                            {
                                cout << "Enter the details of vehicle and pollution caused by them\n";
                                cout << "Sample is provided in the code for reference (line 1822-1833)\n";

                            for (i = 0; i < 8; i++)
                            {                          /**sample input**/
                                t = create_node();     /**RAPIDO 15
                                                        OLA 30
                                                        METRO 20
                                                        PUBLICBUS 63.4
                                                        PRIVATEVEHICLE 87.5
                                                        TRAIN 76.54
                                                        UBERAUTO 21
                                                        RICKSHAW 41.7
                                                        **/
                                t = read_details(t);
                                head = insert_end(head, t);
                            }
                            cout << "\n";
                            display_list(head);
                            cout << "\nEnter either 0 or 1 or 2 for sorting on basis of fare, distance, and seats respectively\n";
                            cout << "**CAUTION-SORTING ON BASIS OF SEATS IS GENERALLY NOT PREFERRED**\n";
                            cout << "\nPlease prefer to go with distance-based sorting as that would give you the expected result with accuracy!\n";
                            cin >> j;
                            if (j == 0)
                            {
                                selection_sort_fare(c);
                                cout << "\nThe sorted list(according to fare)\n";
                                display_sorted_list();
                            }
                            if (j == 1)
                            {
                                cout << "\nThe sorted list(according to distance)";
                                bubble_sort_distance(c);
                                display_sorted_list();
                            }
                            if (j == 2)
                            {
                                cout << "\nThe sorted list(according to seats)";
                                insertion_sort_seats(c);
                                display_sorted_list();
                            }
                            string text = "YES";
                            string pattern;

                            cout << "Do you want to search for any vehicles pollution in particular?..if yes..types YES.else you can't\n";
                            cin >> pattern;
                            int r = rabin_karp(text.c_str(), pattern.c_str());
                            for (i = 0; i < 2; i++)
                            {
                                if (r == 1)
                                {

                                    /**searching for a particular vehicle and pollution caused by it**/
                                    cout << "Please enter the preferable type of transport(ENTER IN CAPS)";
                                    cout << " the transport must be choosen from any one of the vehicles listed above in the sorted list";
                                    cin >> tov;
                                    search_vehicle_pollution(head, tov);
                                    break;
                                }
                                else
                                {
                                    cout << "re-enter YES..this is your last chance else you will be redirected to the front page!";
                                    cin >> pattern;
                                    r = rabin_karp(text.c_str(), pattern.c_str());

                                }
                            }
                            cout << "Which vehicle do you wish to opt for(ENTER IN CAPS)\n";
                            cin >> opt;
                            search_vehicle(c, opt);
                            }
                            break;
                            }

                    case 9:{
                                int n=8;
                                string vehicle[20];
                                float cost;
                                string mop[20];

                                printf("Here are the details of vehicle type,cost/km and mode of payment the transport provides\n");
                                push(&stack, "RAPIDO", 10.0, "CASH/PPAY/PAY");
                                push(&stack, "OLA", 25.0, "GPAY");
                                push(&stack, "METRO", 7.0, "PPAY/GPAY/CASH");
                                push(&stack, "PUBLICBUS", 6.0, "CASH");
                                push(&stack, "PRIVATEVEHICLE", 0.0, "0");
                                push(&stack, "TRAIN", 5.0, "CASH/ONLINE");
                                push(&stack, "UBERAUTO", 16.0, "CASH/GPAY/PPAY");
                                push(&stack, "RICKSHAW", 20.0, "CASH/PPAY/GPAY");

                                printf("\n\n");
                                float* cost_array = display_stack(stack);
                                printf("\n");
                                quick_sort(&stack,0,stack.top);
                                printf("The following displays the sorted(descreasing order)for the elements of the stack\n");

                                display_sorted_stack(stack);
                                printf("Enter the vehicle that you had opted for previously in case 10(ENTER IN CAPS)\n");
                                scanf("%s",opt);
                                if(strcmp(opt,"PRIVATE VEHICLE")==0)
                                {
                                    printf("The user uses his/her own vehicle and travels to the required destination\n");
                                }
                                else
                                {
                                    int distance=calculate_distance(c,opt);

                                    float rupees=pop_cost_of_vehicle(stack,opt);

                                    printf("and the cost needed to travel on %s is %.2f",opt,rupees*distance);

                                    printf("\nDo you want to pay the amount in installments or you wish to pay it now?\n");
                                    printf("1-YES 0-NO\n");
                                    i=0;
                                    scanf("%d",&i);
                                    if(i==1)
                                    {
                                        printf("You have only two installments to pay\n");


                                        for (i = 0; i <30; i++)
                                        {
                                            fenwick_tree[i] = 0;
                                        }


                                        int installment_amounts[30];
                                        printf("Enter the installment amounts:\n");
                                        for (i = 0; i < 2; i++)
                                        {
                                            scanf("%d", &installment_amounts[i]);
                                            update(i, installment_amounts[i]);
                                        }


                                        int amount = query(2+1);

                                        printf("Remaining amount to be paid after installment %d: %d\n",2 + 1,(distance*rupees)-amount );



                                    }
                                    else
                                        printf("I'll pay the desired amount now itself.");
                                    printf("\nCongratulation!!%d amount is amount paid..Have a safe journey!!");

                                     break;
                                    }
                                }
                        case 10:{
                                    struct myqueue q;
                                    string name;
                                    float rating;
                                    string message;
                                    initialize_queue(&q);

                                    cout << "Enter feedback details for entry " << i + 1 << "\n";
                                    cout << "Name: ";
                                    cin >> name;

                                    cout << "Rating: ";
                                    cin >> rating;

                                    cout << "Message: ";
                                    cin >>  message;

                                    enqueue(q, name, rating, message);


                                    cout << "THANKYOU! " << name <<"have a safe journey!";
                                    display_queue(&q);

                                }



                    default:{
                                printf("exciting code");
                                exit(0);
                            }

                }
            }
        }
        else if(p==1)
        {
            printf("Please prefer the following options available\n");
            while(1)
            {

                cout << "\n******** Menu *************\n";
                cout << "6-Hospital\n";
                cout << "7-Police station\n";
                cout << "8-Fire Department\n";
                cout << "9-USER INPUT TO GO TO DESIRED LOCATION";
                cout << "10-If the user wants to make payment\n";
                cout << "11-If the user wants to give rating\n";
                cout << "*****************************\n";

                printf("Enter your choice\n");
                scanf("%d", &choice);
                switch(choice)
                {

                    case 6:{
                                int z;
                                floyd_warshall(matrix);
                                cout << "Enter your current location";
                                cin>> z;
                                cout <<"The Hospital is "<< matrix[z][8]<< "km's away";
                                break;
                            }

                    case 7:{
                                floyd_warshall(matrix);
                                int z;
                                cout << "Enter your current location";
                                cin >> z;
                                cout << "The police station is "<< matrix[z][5] << "km's away";
                                break;
                            }
                    case 8:{
                                floyd_warshall(matrix);
                                int z;
                                cout << "Enter your current location";
                                cin >> z;
                                cout << "The Fire Department is " << matrix[z][0] << "km's away";
                                break;
                            }

                    case 9:{
                                j = 0;
                                s = 0;
                                e = 0;
                                cout << "Enter the start(space)end point of the journey(0-9)\n";
                                cout << "CAUTION** START AND END POINT SHOULD NOT BE THE SAME\n";
                                cin >> s >> e;
                                search_from_file(s);
                                k = get_distance_associated(s, e);
                                c = all_distances(s, e, k);
                                if (c == 1)
                                {
                                    cout << "There is only one option available to you\n";
                                }
                                else
                                {
                                    cout << "Enter the details of vehicle and pollution caused by them\n";
                                    cout << "Sample is provided in the code for reference (line 1796-1806\n";

                                for (i = 0; i < 8; i++)
                                {                          /**sample input**/
                                    t = create_node();     /**RAPIDO 15
                                                            OLA 30
                                                            METRO 20
                                                            PUBLICBUS 63.4
                                                            PRIVATEVEHICLE 87.5
                                                            TRAIN 76.54
                                                            UBERAUTO 21
                                                            RICKSHAW 41.7
                                                            **/
                                    t = read_details(t);
                                    head = insert_end(head, t);
                                }
                                cout << "\n";
                                display_list(head);
                                cout << "\nEnter either 0 or 1 or 2 for sorting on basis of fare, distance, and seats respectively\n";
                                cout << "**CAUTION-SORTING ON BASIS OF SEATS IS GENERALLY NOT PREFERRED**\n";
                                cout << "\nPlease prefer to go with distance-based sorting as that would give you the expected result with accuracy!\n";
                                cin >> j;
                                if (j == 0)
                                {
                                    selection_sort_fare(c);
                                    cout << "\nThe sorted list(according to fare)\n";
                                    display_sorted_list();
                                }
                                if (j == 1)
                                {
                                    cout << "\nThe sorted list(according to distance)";
                                    bubble_sort_distance(c);
                                    display_sorted_list();
                                }
                                if (j == 2)
                                {
                                    cout << "\nThe sorted list(according to seats)";
                                    insertion_sort_seats(c);
                                    display_sorted_list();
                                }
                                string text = "YES";
                                string pattern;

                                cout << "Do you want to search for any vehicles pollution in particular?..if yes..types YES.else you can't\n";
                                cin >> pattern;
                                int r = rabin_karp(text.c_str(), pattern.c_str());
                                for (i = 0; i < 2; i++)
                                {
                                    if (r == 1)
                                    {

                                        /**searching for a particular vehicle and pollution caused by it**/
                                        cout << "Please enter the preferable type of transport(ENTER IN CAPS)";
                                        cout << " the transport must be choosen from any one of the vehicles listed above in the sorted list";
                                        cin >> tov;
                                        search_vehicle_pollution(head, tov);
                                        break;
                                    }
                                    else
                                    {
                                        cout << "re-enter YES..this is your last chance else you will be redirected to the front page!";
                                        cin >> pattern;
                                        r = rabin_karp(text.c_str(), pattern.c_str());

                                    }
                                }
                                cout << "Which vehicle do you wish to opt for(ENTER IN CAPS)\n";
                                cin >> opt;
                                search_vehicle(c, opt);
                                }
                            break;
                        }

                    case 10:{
                                int n=8;
                                string vehicle[20];
                                float cost;
                                string mop[20];

                                printf("Here are the details of vehicle type,cost/km and mode of payment the transport provides\n");
                                push(&stack, "RAPIDO", 10.0, "CASH/PPAY/PAY");
                                push(&stack, "OLA", 25.0, "GPAY");
                                push(&stack, "METRO", 7.0, "PPAY/GPAY/CASH");
                                push(&stack, "PUBLICBUS", 6.0, "CASH");
                                push(&stack, "PRIVATEVEHICLE", 0.0, "0");
                                push(&stack, "TRAIN", 5.0, "CASH/ONLINE");
                                push(&stack, "UBERAUTO", 16.0, "CASH/GPAY/PPAY");
                                push(&stack, "RICKSHAW", 20.0, "CASH/PPAY/GPAY");

                                cout << "\n\n";
                                float* cost_array = display_stack(stack);
                                cout << endl;
                                quick_sort(&stack,0,stack.top);
                                cout <<"The following displays the sorted(descreasing order)for the elements of the stack\n";

                                display_sorted_stack(stack);
                                cout << "Enter the vehicle that you had opted for previously in case 10(ENTER IN CAPS)\n";
                                cin >> opt;
                                if(strcmp(opt,"PRIVATE VEHICLE")==0)
                                {
                                    cout << "The user uses his/her own vehicle and travels to the required destination\n";
                                }
                                else
                                {
                                    int distance=calculate_distance(c,opt);

                                    float rupees=pop_cost_of_vehicle(stack,opt);

                                    printf("and the cost needed to travel on %s is %.2f",opt,rupees*distance);

                                    printf("\nDo you want to pay the amount in installments or you wish to pay it now?\n");
                                    printf("1-YES 0-NO\n");
                                    i=0;
                                    scanf("%d",&i);
                                    if(i==1)
                                    {
                                        printf("You have only two installments to pay\n");


                                        for (i = 0; i <30; i++)
                                        {
                                            fenwick_tree[i] = 0;
                                        }


                                        int installment_amounts[30];
                                        printf("Enter the installment amounts:\n");
                                        for (i = 0; i < 2; i++)
                                        {
                                            scanf("%d", &installment_amounts[i]);
                                            update(i, installment_amounts[i]);
                                        }


                                        int amount = query(2+1);

                                        printf("Remaining amount to be paid after installment %d: %d\n",2 + 1,(distance*rupees)-amount );



                                    }
                                    else
                                        printf("I'll pay the desired amount now itself.");
                                    printf("\nCongratulation!!%d amount is amount paid..Have a safe journey!!");

                                     break;
                                    }
                                }
                        case 11:{
                                    struct myqueue q;
                                    string name;
                                    float rating;
                                    string message;
                                    initialize_queue(&q);

                                    cout << "Enter feedback details for entry " << i + 1 << "\n";
                                    cout << "Name: ";
                                    cin >> name;

                                    cout << "Rating: ";
                                    cin >> rating;

                                    cout << "Message: ";
                                    cin >>  message;

                                    enqueue(q, name, rating, message);


                                    cout << "THANKYOU! " << name <<"have a safe journey!";
                                    display_queue(&q);

                                }


                        default:printf("Exciting code");


                }
            }
        }
        }
        else
        {
            printf("Invalid security key. Access denied!\n");
        }

    return 0;

 }

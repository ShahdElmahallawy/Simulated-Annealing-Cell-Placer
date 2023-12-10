#include<bits/stdc++.h>
using namespace std;

int cells_no, nets_no, n, m;
double cooling_rate = 0.95;

string test_case_file = "tests/test0.txt";
vector<vector<int>> grid, nets;
vector<pair<int, int>> cells;

int calculateWireLength(int i) {
    int min_x = INT_MAX, min_y = INT_MAX, max_x = INT_MIN, max_y = INT_MIN;
    for(auto c: nets[i]) {
        min_x = min(min_x, cells[c].first);
        min_y = min(min_y, cells[c].second);
        max_x = max(max_x, cells[c].first);
        max_y = max(max_y, cells[c].second);
    }
    return (max_x - min_x + 1) + (max_y - min_y + 1);
}

void random_placement() {
    vector<int> indices;
    for(int i=0; i<n*m; i++) {
        indices.push_back(i);
    }
    random_shuffle(indices.begin(), indices.end()); 
    for(int i=0; i<cells_no; i++) {
        int row = indices[i] / m;
        int col = indices[i] % m;
        cells[i] = {row, col};
        grid[row][col] = i;
    }
}

void print_grid() {
    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            if(grid[i][j] == -1) cout << "__ ";
            else if(grid[i][j] >= 0 && grid[i][j] <= 9) cout << "0" << grid[i][j] << " ";
            else cout << grid[i][j] << " ";
        }
        cout << "\n";
    }
}

void print_binary() {
    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            if(grid[i][j] == -1) cout << 0;
            else cout << 1;
        }
        cout << "\n";
    }
}

int main() {
    srand(1);
    ifstream input_file;
    input_file.open(test_case_file);
    input_file >> cells_no >> nets_no >> n >> m;

    // Create grid, nets, and cells with proper sizes
    grid.resize(n, vector<int>(m, -1));
    nets.resize(nets_no);
    cells.resize(cells_no);
    for(int i=0; i<nets_no; i++) {
        int comp_no, temp;
        input_file >> comp_no;
        while(comp_no--){
             input_file >> temp;
             nets[i].push_back(temp);
        }
    }

    // Random Placment
    random_placement();
    int current_cost = 0;
    for(int i=0; i<nets_no; i++) current_cost += calculateWireLength(i);

    // Print Initial Placement
    cout << "Initial Placement:\n";
    print_binary();
    cout << "Initial Cost: " << current_cost << endl;
    cout << endl << endl;

    // Simulated Annealing Parameters
    double initial_temp = 500 * current_cost;
    double final_temp = ((5 * 1e-6) * current_cost) / nets_no;
    double current_temp = initial_temp;

    // Execute Simulated Annealing
    while(current_temp > final_temp) {
        // cout << current_temp << endl;
        int t = 10*cells_no;
        while(t--) {
            int a = rand() % cells_no, b = -1, b_loc = rand() % (n*m);
            int a_row = cells[a].first, a_col = cells[a].second;
            int b_row = b_loc / m, b_col = b_loc % m;
            if(grid[b_row][b_col] == -1) {      // Swap with an empty cell
                cells[a] = {b_row, b_col};
            }
            else {
                b = grid[b_row][b_col];
                swap(cells[a], cells[b]);
            }
            grid[b_row][b_col] = a;
            grid[a_row][a_col] = b;
            // cout << "Swapping " << a << " with " << b <<endl;
            int nw_cost = 0;
            for(int i=0; i<nets_no; i++) nw_cost += calculateWireLength(i);
            if(nw_cost < current_cost) {
                // cout << "Success\n";
                current_cost = nw_cost;
            }
            else {
                double prob = (1 - exp((-1*(nw_cost - current_cost))/current_temp));
                if(prob < 0.5) {
                    // cout << "Success2\n";
                    current_cost = nw_cost;
                }
                else {
                    // cout << "Fail\n";
                    if(b == -1) {
                        cells[a] = {a_row, a_col};
                    }
                    else {
                        swap(cells[a], cells[b]);
                    }
                    grid[a_row][a_col] = a;
                    grid[b_row][b_col] = b;
                }
            }
        }
        current_temp = cooling_rate * current_temp;
    }

    // Print Binary
    cout << "Final Placement:\n";
    print_binary();
    cout << endl << endl;

    // Print Grid
    cout << "Final Result is:\n";
    print_grid();

    // Print Total Cost
    cout << "Final wire length = " << current_cost << endl;
}
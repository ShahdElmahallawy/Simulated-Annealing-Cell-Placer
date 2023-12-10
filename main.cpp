#include<bits/stdc++.h>
#include<random>
#include <chrono>
using namespace std::chrono;
using namespace std;

int cells_no, nets_no, n, m;
double cooling_rate = 0.95;

string test_case_file = "tests/t3.txt";
vector<vector<int>> grid, nets;
vector<pair<int, int>> cells;
vector<vector<int>> nets_of_cells;
vector<int> wire_length;

void save_grid_state(const string& filename, const vector<vector<int>>& grid) {
    ofstream outFile(filename, ios::app);
    for (const auto& row : grid) {
        for (const auto& elem : row) {
            if (elem == -1)
                outFile << "__ ";
            else
                outFile << setw(2) << setfill('0') << elem << " ";
        }
        outFile << "\n";
    }
    outFile << "\n";
}

int calculateWireLength(int i, bool update_wire=false) {
    int min_x = INT_MAX, min_y = INT_MAX, max_x = INT_MIN, max_y = INT_MIN;
    for(auto c: nets[i]) {
        min_x = min(min_x, cells[c].first);
        min_y = min(min_y, cells[c].second);
        max_x = max(max_x, cells[c].first);
        max_y = max(max_y, cells[c].second);
    }
    int length = (max_x - min_x) + (max_y - min_y);
    if(update_wire) wire_length[i] = length;
    return length;
}

void random_placement() {
    vector<int> indices;
    for(int i=0; i<n*m; i++) {
        indices.push_back(i);
    }
    unsigned seed = 12345;  // Example fixed seed
    std::default_random_engine engine(seed);
    shuffle(indices.begin(), indices.end(), engine); 
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
    auto start = high_resolution_clock::now();
    ifstream input_file;
    input_file.open(test_case_file);
    input_file >> cells_no >> nets_no >> n >> m;

    ofstream logFile("out\\t3_annealing_log_0.95.txt");
    // Create grid, nets, and cells with proper sizes
    grid.resize(n, vector<int>(m, -1));
    nets.resize(nets_no);
    cells.resize(cells_no);
    nets_of_cells.resize(cells_no);
    wire_length.resize(nets_no);
    for(int i=0; i<nets_no; i++) {
        int comp_no, temp;
        input_file >> comp_no;
        while(comp_no--){
             input_file >> temp;
             nets[i].push_back(temp);
             nets_of_cells[temp].push_back(i);
        }
    }

    // Random Placment
    random_placement();
    int current_cost = 0;
    for(int i=0; i<nets_no; i++) current_cost += calculateWireLength(i, true);

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
            int nw_cost = current_cost;
            vector<int> new_costs;
            for(auto i:nets_of_cells[a]) {
                int new_wire_length = calculateWireLength(i);
                new_costs.push_back(new_wire_length);
                nw_cost = nw_cost - wire_length[i] + new_wire_length;
            }
            if(b != -1) {
                for(auto i:nets_of_cells[b]) {
                    int new_wire_length = calculateWireLength(i);
                    new_costs.push_back(new_wire_length);
                    nw_cost = nw_cost - wire_length[i] + new_wire_length;
                }
            }
            if(nw_cost < current_cost) {
                current_cost = nw_cost;
                int i = 0;
                for(; i<nets_of_cells[a].size(); i++) {
                    wire_length[nets_of_cells[a][i]] = new_costs[i];
                }
                if(b != -1) {
                    for(; i<nets_of_cells[a].size() + nets_of_cells[b].size(); i++) {
                        wire_length[nets_of_cells[b][i-nets_of_cells[a].size()]] = new_costs[i];
                    }
                }

            }
            else {
                double prob = (1 - exp((-1*(nw_cost - current_cost))/current_temp));
                if(prob < 0.5) {
                    current_cost = nw_cost;
                    int i = 0;
                    for(; i<nets_of_cells[a].size(); i++) {
                        wire_length[nets_of_cells[a][i]] = new_costs[i];
                    }
                    if(b != -1) {
                        for(; i<nets_of_cells[a].size() + nets_of_cells[b].size(); i++) {
                            wire_length[nets_of_cells[b][i-nets_of_cells[a].size()]] = new_costs[i];
                        }
                    }
                }
                else {
                    // Return back original cell places and grid
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
        if (logFile.is_open()) {
            logFile << current_temp << " " << current_cost << "\n";
        }
        current_temp = cooling_rate * current_temp;
        save_grid_state("out\\grid_states_d1.txt", grid);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    // Print Binary
    cout << "Final Placement:\n";
    print_binary();
    cout << endl << endl;

    // Print Grid
    cout << "Final Result is:\n";
    print_grid();

    // Print Total Cost
    cout << "Final wire length = " << current_cost << endl;

    cout << "Time taken is: " << duration.count() << " milliseconds\n";
}
#include<bits/stdc++.h>
// #include<random>
#include <chrono>
using namespace std::chrono;
using namespace std;

int cells_no, nets_no, n, m;
double cooling_rate = 0.95;

string test_case_file = "tests/t3.txt";

// For logging, uncomment
// ofstream logFile("log/d0_annealing_log_0.95.txt");
// string gif_file_name = "gif_script_and_log/grid_states_t3_0.95.txt";
// int isLog = 0;

vector<vector<int>> grid, nets;
vector<pair<int, int>> cells;
vector<vector<int>> nets_of_cells;
vector<int> wire_length;

// For logging, uncomment
// void save_grid_state(const string& filename, const vector<vector<int>>& grid) {
//     ofstream outFile(filename, ios::app);
//     for (const auto& row : grid) {
//         for (const auto& elem : row) {
//             if (elem == -1)
//                 outFile << "__ ";
//             else
//                 outFile << setw(2) << setfill('0') << elem << " ";
//         }
//         outFile << "\n";
//     }
//     outFile << "\n";
// }

int calculateWireLength(int i, bool update_wire=true) {
    // This function calculates the wire length for a net
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
    // This function initializes the grid with random placement
    vector<int> indices;
    for(int i=0; i<n*m; i++) {
        indices.push_back(i);
    }
    // unsigned seed = 12345;  // Example fixed seed
    // std::default_random_engine engine(seed);
    random_shuffle(indices.begin(), indices.end()); 
    // shuffle(indices.begin(), indices.end(), engine);
    for(int i=0; i<cells_no; i++) {
        int row = indices[i] / m;
        int col = indices[i] % m;
        cells[i] = {row, col};
        grid[row][col] = i;
    }
}

void print_grid() {
    // Helper function for printing grid
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
    // Helper funcntion for printing in binary
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
    nets_of_cells.resize(cells_no);
    wire_length.resize(nets_no);
    vector<int> old_wire_length(nets_no, 0);
    for(int i=0; i<nets_no; i++) {
        int comp_no, temp;
        input_file >> comp_no;
        while(comp_no--){
             input_file >> temp;
             nets[i].push_back(temp);
             nets_of_cells[temp].push_back(i);
        }
    }
    auto start = high_resolution_clock::now();
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
        int t = 10*cells_no;
        while(t--) {
            // Try swapping two cells where a is always a cell and b can be an empty site
            int a = rand() % cells_no, b = -1, b_loc = rand() % (n*m);
            int a_row = cells[a].first, a_col = cells[a].second;
            int b_row = b_loc / m, b_col = b_loc % m;
            if(grid[b_row][b_col] == -1) {      // Swap with an empty site
                cells[a] = {b_row, b_col};
            }
            else {                              // Swap where both a and b are cells
                b = grid[b_row][b_col];
                swap(cells[a], cells[b]);
            }
            // update grid
            grid[b_row][b_col] = a;             
            grid[a_row][a_col] = b;
            // Calculate new cost             
            int nw_cost = current_cost;
            
            // Only loop over nets that have cell a
            for(auto i:nets_of_cells[a]) {
                old_wire_length[i] = wire_length[i];
                int new_wire_length = calculateWireLength(i);
                nw_cost = nw_cost - old_wire_length[i] + new_wire_length;
            }
            // Do same thing for nets of cell b if it is not empty
            if(b != -1) {
                for(auto i:nets_of_cells[b]) {
                    if(old_wire_length[i]) continue;
                    old_wire_length[i] = wire_length[i];
                    int new_wire_length = calculateWireLength(i);
                    nw_cost = nw_cost - old_wire_length[i] + new_wire_length;
                }
            }
            if(nw_cost < current_cost) { // Accept swap
                current_cost = nw_cost;
                for(auto i:nets_of_cells[a]) if(old_wire_length[i]) {
                        old_wire_length[i] = 0;
                    }
                    if(b != -1) {
                        for(auto i:nets_of_cells[b]) if(old_wire_length[i]) {
                            old_wire_length[i] = 0;
                        }
                    }
            }
            else {
                double prob = (1 - exp((-1*(nw_cost - current_cost))/current_temp));
                if(prob < 0.5) { // Accept swap
                    current_cost = nw_cost;
                    for(auto i:nets_of_cells[a]) if(old_wire_length[i]) {
                        old_wire_length[i] = 0;
                    }
                    if(b != -1) {
                        for(auto i:nets_of_cells[b]) if(old_wire_length[i]) {
                            old_wire_length[i] = 0;
                        }
                    }
                }
                else {
                    // Return back original cell places, grid, and wire lengths
                    if(b == -1) {
                        cells[a] = {a_row, a_col};
                    }
                    else {  
                        swap(cells[a], cells[b]);
                    }
                    for(auto i:nets_of_cells[a]) if(old_wire_length[i]) {
                        wire_length[i] = old_wire_length[i];
                        old_wire_length[i] = 0;
                    }
                    if(b != -1) {
                        for(auto i:nets_of_cells[b]) if(old_wire_length[i]) {
                            wire_length[i] = old_wire_length[i];
                            old_wire_length[i] = 0;
                        }
                    }
                    grid[a_row][a_col] = a;
                    grid[b_row][b_col] = b;
                    
                }
            }
        }
        // For logging, uncomment
        // if (isLog == 2) {
        //     logFile << current_temp << " " << current_cost << "\n";
        // }
        // if(isLog == 1)
        //     save_grid_state(gif_file_name, grid);
        current_temp = cooling_rate * current_temp;
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

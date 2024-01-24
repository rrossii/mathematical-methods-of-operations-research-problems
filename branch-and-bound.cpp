#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>


#define ll long long


using namespace std;


struct Indexes {
   Indexes(int i, int j) {
       row = i;
       col = j;
   }
   int row = 0, col = 0;
};


vector<double> row_const_pryvedennya(vector<vector<double>> &distances) {
   size_t rows = distances.size();
   vector<double> res;


   for (int i = 0; i < rows; i++) {
       double min_el = *min_element(distances[i].begin(), distances[i].end());
       if (min_el == INFINITY) {
           min_el = 0;
       }
       res.emplace_back(min_el);
   }
   return res;
}


vector<double> col_const_pryvedennya(vector<vector<double>> &distances) {
   size_t rows = distances.size(), cols = distances[0].size();
   vector<double> res;


   for (int j = 0; j < cols; j++) {
       double min_el = 10000000;
       for (int i = 0; i < rows; i++) {
           min_el = min(min_el, distances[i][j]);
       }
       if (min_el == 10000000) {
           min_el = 0;
       }
       res.emplace_back(min_el);
   }
   return res;
}


void row_reduction(vector<vector<double>> &distances, vector<double> &row_const_pryved) {
   size_t rows = distances.size(), cols = distances[0].size();


   for (int i = 0; i < rows; i++) {
       for (int j = 0; j < cols; j++) {
           distances[i][j] -= row_const_pryved[i];
       }
   }
}


void col_reduction(vector<vector<double>> &distances, vector<double> &col_const_pryved) {
   size_t rows = distances.size(), cols = distances[0].size();


   for (int j = 0; j < cols; j++) {
       for (int i = 0; i < rows; i++) {
           distances[i][j] -= col_const_pryved[j];
       }
   }
}


double lower_bound(vector<double> &row_const_pryved, vector<double> &col_const_pryved) {
   double sum_row = 0, sum_col = 0;


   for (auto &el : row_const_pryved) {
       sum_row += el;
   }
   for (auto &el : col_const_pryved) {
       sum_col += el;
   }


   return sum_row + sum_col;
}


Indexes zero_estimation(vector<vector<double>> &distances) {
   size_t rows = distances.size(), cols = distances[0].size();
   double max_zero_est = 0;
   Indexes indexes_of_max_zero_est = {0, 0};


   for (int i = 0; i < rows; i++) {
       for (int j = 0; j < cols; j++) {
           if (distances[i][j] == 0) {
               double min_row_el = 10000000, min_col_el = 10000000;


               for (int j_1 = 0; j_1 < cols; j_1++) {
                   if (j_1 != j) {
                       min_row_el = min(min_row_el, distances[i][j_1]);
                   }
               } // мінімальний по рядку окрім поточного нуля (можуть бути інші нулі якщо вони є в рядку)
               for (int i_1 = 0; i_1 < rows; i_1++) {
                   if (i_1 != i) {
                       min_col_el = min(min_col_el, distances[i_1][j]);
                   }
               }// мін по стовпцю окрім поточного нуля (можуть бути інші нулі якщо вони є в стовпці)


               double zero_est = min_row_el + min_col_el;


               if (zero_est > max_zero_est) {
                   indexes_of_max_zero_est.row = i;
                   indexes_of_max_zero_est.col = j;


                   max_zero_est = zero_est;
               }
           }
       }
   }
   return indexes_of_max_zero_est;
}


double sum_distance(vector<vector<double>> &distances, vector<Indexes> &route) {
   double res = 0;
   for (auto &r : route) {
       res += distances[r.row][r.col];
   }


   return res;
}


double exclude_route(vector<vector<double>> distances, double current_lower_bound, Indexes &indexes_of_current_element) { // не посилання на distances (робимо копію), оскільки невідомо чи повернемось до цієї гілки пізніше (потрібно буде мати оригінальну матрицю без заміни елемента на inf)
   int i = indexes_of_current_element.row, j = indexes_of_current_element.col;
   distances[i][j] = INFINITY;


   auto r = row_const_pryvedennya(distances);
   auto c = col_const_pryvedennya(distances);


   return current_lower_bound + lower_bound(r, c);
}


double include_route(vector<vector<double>> &distances, double current_lower_bound, Indexes &indexes_of_current_element) {
   int row = indexes_of_current_element.row, col = indexes_of_current_element.col;


   // робимо маршрут (col, row) безкінечністю
   distances[col][row] = INFINITY;


   // замість видалення
   for (int j = 0; j < distances[0].size(); j++) {
       distances[row][j] = INFINITY;
   }
   for (int i = 0; i < distances.size(); i++) {
       distances[i][col] = INFINITY;
   }


   auto r = row_const_pryvedennya(distances);
   auto c = col_const_pryvedennya(distances);


   return current_lower_bound + lower_bound(r, c);
}


vector<Indexes> branch_and_bound(vector<vector<double>> distances) { // повертаємо шлях від міста до міста
   vector<Indexes> route;


   double current_lower_bound = 0;
   while (route.size() < distances.size()) { // маршрутів стільки, скільки й міст
       auto row_consts = row_const_pryvedennya(distances);
       row_reduction(distances, row_consts);
       auto col_consts = col_const_pryvedennya(distances);
       col_reduction(distances, col_consts);


       current_lower_bound += lower_bound(row_consts, col_consts);


       auto ind_of_max_zero_est = zero_estimation(distances);
       int i = ind_of_max_zero_est.row, j = ind_of_max_zero_est.col;


       double lower_bound_exclude = exclude_route(distances, current_lower_bound, ind_of_max_zero_est);
       double lower_bound_include = include_route(distances, current_lower_bound, ind_of_max_zero_est);


       if (lower_bound_include <= lower_bound_exclude) { // чи < ?
           route.emplace_back(i, j); // включили маршрут
       } else {
           // не включаємо маршрут
           exclude_route(distances, current_lower_bound, ind_of_max_zero_est);
       }
   }
   return route;
}

int main() {
   vector<vector<double>> distances = {
           {INFINITY, 5, 10, 19, 14},
           {10, INFINITY, 3, 11, 20},
           {12, 15, INFINITY, 13, 3},
           {19, 20, 6, INFINITY, 12},
           {19, 9, 5, 12, INFINITY}
   };


   auto res_route = branch_and_bound(distances);
   for (auto &route : res_route) {
       cout << route.row + 1 << " -> " << route.col + 1 << '\t';
   }
   cout << "\nMin sum distance = " << sum_distance(distances, res_route);
   return 0;
}

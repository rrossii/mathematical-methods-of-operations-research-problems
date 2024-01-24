#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

double calc_estimated_value(vector<vector<double>> &A, vector<double> &C, vector<double> &C_basis, int j) { // повертає оцінку[j] за ф-лою: оцінка[j] = F[j] - C[j] = sum(C[i] * a[i][j]) - C[j], де i = 1...m
	int m = A.size(); //A.size() = к-ті обмежень
	double res = 0;

	for (int i = 0; i < m; i++) {
    	res += C_basis[i] * A[i][j];
	}
	res -= C[j];

	return res;
}

bool is_optimal(vector<double> &estimated_values) {
	if (all_of(estimated_values.begin(), estimated_values.end(), [](double el){ return el >= 0; })) {
    	return true;
	}
	else {
    	return false;
	}
}
vector<pair<vector<int>, int>> calc_basis(vector<vector<double>> &A) {
	int m = A.size();
	vector<pair<vector<int>, int>> basis;

	// проходимось по рядках: зверху вниз
	for (int j = 0; j < A[0].size(); j++) {
    	int count_one = 0, count_zero = 0;
    	vector<int> likely_unit_vector(m); // можливо одиничний вектор, проходячи зверху вниз, обираємо одиничні вектори, з яких потім складемо базис

    	for (int i = 0; i < m; i++) {
        	if (A[i][j] == 1) {
            	count_one++;
        	} else if (A[i][j] == 0) {
            	count_zero++;
        	}
        	likely_unit_vector[i] = A[i][j];
    	}
    	if (count_one == 1 && count_zero == m - 1) {
        	basis.emplace_back(likely_unit_vector, j);
        	likely_unit_vector.clear();
    	}
	}
	sort(basis.begin(), basis.end(), greater<>()); // тепер маємо базис, який утворює одиничну матрицю розміру m
	return basis;
}

double simplex_method(vector<vector<double>> &A, vector<double> &plan, vector<double> &C) {
	double res = 0;
	size_t n = A[0].size(), m = A.size();

	vector<double> estimated_values(n, -1);

	while (!is_optimal(estimated_values)) {
    	res = 0;

    	vector<pair<vector<int>, int>> basis = calc_basis(A);

    	vector<double> C_basis; // коефіцієнти базисних змінних у цільовій функції
    	for (auto el : basis) {
        	int index = el.second;
        	C_basis.push_back(C[index]);
    	}

    	int ind = 0;
    	for (int j = 0; j < n; j++) {
        	double val = calc_estimated_value(A, C, C_basis, j);
        	estimated_values[ind] = val;
        	ind++;
    	}

    	double min_estimated_val = *min_element(estimated_values.begin(), estimated_values.end());
    	int col = find(estimated_values.begin(), estimated_values.end(), min_estimated_val) - estimated_values.begin(); // індекс змінної яка має найменшу оцінку а також індекс нової базисної змінної

    	vector<double> theta(m);
    	for (int i = 0; i < m; i++) {
        	theta[i] = plan[i] / A[i][col];
    	}

    	double min_theta_value = *min_element(theta.begin(), theta.end(), [](double x, double y) {
        	return x >= 0 && (y <= 0 || x < y) && x != INFINITY && y != INFINITY;
    	});
    	int row = find(theta.begin(), theta.end(), min_theta_value) - theta.begin();
    	// індекс розв'язувального елемента це [row, col]

    	double pivot = A[row][col]; // розв. елемент
    	for (int j = 0; j < n; j++) {
        	A[row][j] /= pivot;
    	}
    	plan[row] /= pivot;

    	// робимо новий базис, перетворюючи коефіцієнти нової базисної змінної на 0 (крім розв. елемента)
    	for (int i = 0; i < m; i++) {
        	if (i != row) {
            	double coef = A[i][col];
            	for (int j = 0; j < n; j++) {
                	A[i][j] = A[i][j] - coef * A[row][j];
            	}
            	plan[i] = plan[i] - coef * plan[row];
        	}
    	}
    	basis = calc_basis(A);

    	// коеф при нових базисних змінних
    	C_basis.clear();
    	for (auto el : basis) {
        	int index = el.second;
        	C_basis.push_back(C[index]);
    	}

    	// опорний план
    	vector<double> res_plan(n, 0);
    	int index = 0;
    	for (auto el : basis) {
        	res_plan[el.second] = plan[index]; // el.second - це індекс базисної змінної
        	index++;
    	}

    	for (int i = 0; i < n; i++) {
        	cout << "x[" << i << "] = " << res_plan[i] << '\n';
    	}

    	// значення функції
    	for (int i = 0; i < n; i++) {
        	res += C[i] * res_plan[i];
    	}

    	ind = 0;
    	for (int j = 0; j < n; j++) {
        	double val = calc_estimated_value(A, C, C_basis, j);
        	estimated_values[ind] = val;
        	ind++;
    	}
    	cout << "==================\n";
	}
	cout << "Plan is optimal\n";
	return res;
}

int main() {
	vector<double> C = {2, 2, 0, 0, 0, 0, -100000}; // коефіцієнти в цільовій ф-ції
	vector<vector<double>> A = {
        	{-3, 2, 1, 0,  0, 0, 0},
        	{1,  1, 0, -1, 0, 0, 1},
        	{1,  0, 0, 0,  1, 0, 0},
        	{0,  1, 0, 0,  0, 1, 0}
	};

	vector<double> plan = {6, 3, 3, 5};
	double optimal_value = simplex_method(A, plan, C);
	cout << "F max = " << optimal_value;
	return 0;
}

#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>


#define ld long double


using namespace std;


struct Company_CostAndProfit {
   Company_CostAndProfit(int company_number, int cost, ld profit) {
       this->company_number = company_number;
       this->cost = cost;
       this->profit = profit;
   }
   int company_number = 1;
   int cost = 0;
   ld profit = 0;
};


struct MaxProfitAndInvestment { // f'n(x_n) and k_n
   MaxProfitAndInvestment(int company_number, int investment, ld profit) {
       this->company_number = company_number;
       this->investment = investment;
       this->profit = profit;
   }
   int company_number = 1;
   int investment = 0;
   ld profit = 0;
};


void search_max_profit_and_according_investment_by_cols_for_current_company(int company_index, vector<vector<ld>> &profits_in_current_company, vector<MaxProfitAndInvestment> &max_profit_by_cols_in_current_company) {
   size_t n = profits_in_current_company.size();


   for (int x = 0; x < n; x++) {
       ld max_profit = 0;
       int investment_to_achieve_that_profit = 0;
       for (int k = 0; k < n; k++) {
           if (profits_in_current_company[x][k] > max_profit) {
               max_profit = profits_in_current_company[x][k];
               investment_to_achieve_that_profit = k;
           }
       }
       max_profit_by_cols_in_current_company[x].profit = max_profit;
       max_profit_by_cols_in_current_company[x].investment = investment_to_achieve_that_profit;
       max_profit_by_cols_in_current_company[x].company_number = company_index + 1;
   }
}


ld profit_for_company_given_cost_for_it(int company_index, ld cost, vector<vector<Company_CostAndProfit>> &companies) { // якщо існує прибуток на певний вклад(cost), то повертаємо його
   ld profit_for_given_cost = 0;


   for (auto & company_cost_and_profit_info : companies[company_index]) {
       if (company_cost_and_profit_info.cost == cost) {
           profit_for_given_cost = max(profit_for_given_cost, company_cost_and_profit_info.profit); // якщо є декілька однакових значень cost, то обираємо там де найбільший прибуток
       }
   }


   return profit_for_given_cost;
}


vector<vector<MaxProfitAndInvestment>> search_max_profits_by_company_and_investment_to_achieve_profit(vector<vector<Company_CostAndProfit>> &companies, int total_investments) { // f'n(x_n) i k_n
   size_t n = total_investments + 1;
   vector<MaxProfitAndInvestment> max_profit_by_cols_and_according_investment_in_current_company(n, MaxProfitAndInvestment(1, 0, 0));
   vector<vector<MaxProfitAndInvestment>> max_profits_and_investments_for_all_companies;


   for (int i = companies.size() - 1; i >= 0; i--) { // починаємо обрахунок з останньої компанії
       vector<vector<ld>> profits_in_current_company(n, vector<ld>(n, -1)); // де -1 це пусті клітинки


       // якщо вкладаємо 0 в поточну компанію (k = 0), то повторюємо результати максимальний прибутків f'n(x_n) з минулої компанії
       for (int x = 0; x < n; x++) {
           profits_in_current_company[x][0] = max_profit_by_cols_and_according_investment_in_current_company[x].profit;
       }


       for (int k = 1; k < n; k++) { // йдемо по рядках зверху вниз, к - стовпчик, х - рядок
           for (int x = k; x < n; x++) {
               profits_in_current_company[x][k] = profit_for_company_given_cost_for_it(i, k, companies);
               if (x != k && x != 0) {
                   profits_in_current_company[x][k] += max_profit_by_cols_and_according_investment_in_current_company[x-k].profit;
               }
           }
       }
       search_max_profit_and_according_investment_by_cols_for_current_company(i, profits_in_current_company, max_profit_by_cols_and_according_investment_in_current_company);
       max_profits_and_investments_for_all_companies.emplace_back(max_profit_by_cols_and_according_investment_in_current_company);
   }


   return max_profits_and_investments_for_all_companies;
}


vector<int> how_much_to_invest_in_each_company_to_get_max_profit(vector<vector<MaxProfitAndInvestment>> &final_matrix_of_max_profits_and_investments, int total_investments) {
   size_t n = final_matrix_of_max_profits_and_investments.size();
   vector<int> investments;


   int leftover_investment = total_investments;
   for (int i = n - 1; i >= 0; i--) { // починаємо ззаду, але n-1 зараз це індекс 1-ї компанії і піднімаємось до 3ї компанії
       int investment_to_current_company = final_matrix_of_max_profits_and_investments[i][leftover_investment].investment; // витратили 3 з 9 інвестицій -> отже максимум на вклад в наступну компанію дор. 6 і т.д, тому [i][leftover_investment]
       investments.emplace_back(investment_to_current_company);


       leftover_investment -= investment_to_current_company;
   }


   return investments;
}
int main() {
   int total_investments = 9;
   int n = total_investments;
   int companies_amount = 3;
   vector<vector<Company_CostAndProfit>> companies = {
           {
               Company_CostAndProfit(1, 3, 0.8), Company_CostAndProfit(1, 2, 0.4),
               Company_CostAndProfit(1, 3, 1.1), Company_CostAndProfit(1, 3, 0.9),
               Company_CostAndProfit(1, 3, 0.8)
            },
           {
               Company_CostAndProfit(2, 2, 0.5), Company_CostAndProfit(2, 2, 0.4),
               Company_CostAndProfit(2, 1, 0.4), Company_CostAndProfit(2, 2, 0.6),
               Company_CostAndProfit(2, 5, 1.3)
           },
           {
               Company_CostAndProfit(3, 1, 0.3), Company_CostAndProfit(3, 4, 0.8),
               Company_CostAndProfit(3, 1, 0.4), Company_CostAndProfit(3, 4, 1.2),
               Company_CostAndProfit(3, 3, 0.8)
           }
   };


   vector<vector<MaxProfitAndInvestment>> f_k = search_max_profits_by_company_and_investment_to_achieve_profit(companies, total_investments);
   ld total_profit = f_k[companies_amount - 1][n].profit;
   cout << "Max total profit = " << total_profit << '\n'; // дивіденди


   vector<int> investments = how_much_to_invest_in_each_company_to_get_max_profit(f_k, total_investments);


   for (int i = 0; i < investments.size(); i++) {
       cout << "You need to invest " << investments[i] << " currency" << " in " << i + 1 << "th company \n";
   }
   cout << "\nTotal income = " << total_investments + total_profit << '\n';

   return 0;
}

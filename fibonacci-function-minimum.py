#include <iostream>
#include <algorithm>
#include <string>
#include <cmath>


using namespace std;


double f(double x) {
   return exp(x) - ((1./3.)*pow(x, 3)) - 2*x;
}


int find_number_of_fib_number_that_is_greater_than_estimated_number(double est) {
   int f_prev_prev = 1, f_prev = 1, f_res = 1;
   int n = 2;
   while (f_res < est) {
       f_res = f_prev_prev + f_prev;
       f_prev_prev = f_prev;
       f_prev = f_res;


       n++;
   }
   return n;
}


int find_n(double a, double b, double eps) {
   double estimate = (b - a) / eps;
   int n = find_number_of_fib_number_that_is_greater_than_estimated_number(estimate);
   return n;
}


int fib_number(int n) {
   int f_prev_prev = 1, f_prev = 1, f_res = 1;
   n -= 2;
   while (n > 0) {
       f_res = f_prev_prev + f_prev;
       f_prev_prev = f_prev;
       f_prev = f_res;


       n--;
   }
   return f_res;
}


double fibonacci_algorithm(double a, double b, double eps) {
   int n = find_n(a, b, eps) + 1;
   double x1 = a + (double(fib_number(n-2)) / double(fib_number(n))) * (b - a);
   double x2 = a + (double(fib_number(n-1)) / double(fib_number(n))) * (b - a);


   int k = 0;
   while (b - a > eps) {
       double f1 = f(x1), f2 = f(x2);
       cout << k + 1 << " iteration: \n";
       cout << "b = " << b << "\ta = " << a << '\n';
       cout << "x[1] = " << x1 << "\t\tx[2] = " << x2 << '\n';
       cout << "f(x1) = " << f1 << "\t\tf(x2) = " << f2 << "\n\n";


       if (f1 <= f2) {
           if (f1 == f2) {
               break;
           }
           b = x2;
           x2 = x1;
           x1 = a + (double(fib_number(n-k-3)) / double(fib_number(n-k-1))) * (b - a);
       } else {
           a = x1;
           x1 = x2;
           x2 = a + (double(fib_number(n-k-2)) / double(fib_number(n-k-1))) * (b - a);
       }
       k++;
   }
   return (a + b) / 2; //min
}


int main() {
   double a, b, eps = 0.01;
   cout << "Enter segment limits: ";
   cin >> a >> b;
   cout << "Enter eps (0.01 by default): ";
   cin >> eps;


   double x_min = fibonacci_algorithm(a, b, eps);
   cout << "Result: \n";
   cout << "x(min) = " << x_min << '\n';
   cout << "y(min) = " << f(x_min) << '\n';


   return 0;
}

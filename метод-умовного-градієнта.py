from math import sqrt
import numpy as np
from sympy import symbols, sympify, diff, solve
from scipy.optimize import linprog
from grad_descent_method import calc_grad


x1, x2 = symbols('x1 x2')
eps = 0.1


def calc_approximation_point(prev_approx_point, beta, h):
   return prev_approx_point + beta * h




def calc_linear_function(grad, approx_point):
   approx_point_x1 = approx_point[0]
   approx_point_x2 = approx_point[1]


   grad_x1 = grad[0]
   grad_x2 = grad[1]


   return grad_x1 * (x1 - approx_point_x1) + grad_x2 * (x2 - approx_point_x2)


def find_beta(func, approx_point, h):
   beta = symbols("beta")


   approx_point_x1 = approx_point[0]
   approx_point_x2 = approx_point[1]


   h_x1 = h[0]
   h_x2 = h[1]


   func_with_placed_x1_and_x2 = func.subs(
       [(x1, approx_point_x1 + h_x1 * beta), (x2, approx_point_x2 + h_x2 * beta)])
   derivative = diff(func_with_placed_x1_and_x2, beta)


   # прирівняємо похідну до нуля
   beta_value = solve(derivative, beta)[0]  # тому що повертає список можливих розвязків
   return beta_value.evalf()


def calc_h(result_y, approx_point):
   return result_y - approx_point


def main_condition(prev_approx_point, current_approx_point):
   prev_approx_point_x1 = prev_approx_point[0]
   prev_approx_point_x2 = prev_approx_point[1]
   current_approx_point_x1 = current_approx_point[0]
   current_approx_point_x2 = current_approx_point[1]


   # x(k) - x(k-1)
   current_minus_prev_x1 = (current_approx_point_x1 - prev_approx_point_x1)
   current_minus_prev_x2 = (current_approx_point_x2 - prev_approx_point_x2)


   condition = sqrt(current_minus_prev_x1 ** 2 + current_minus_prev_x2 ** 2)


   return condition < eps


def output_interim_results(iteration, approx_point_x1, approx_point_x2, beta, h, f):
   print(f"{iteration} iteration:")
   print(f"x{iteration} = ({approx_point_x1}; {approx_point_x2})")
   print(f"beta = {beta}")
   print(f"h = ({h[0]}, {h[1]})")
   print(f"f = {f}\n")




def conditional_grad_method(func, approx_point, limitation_coefficients, plan_coefficients):
   prev_approx_point = [10000, 10000]
   iteration = 0
   while not main_condition(prev_approx_point, approx_point):
       grad = calc_grad(approx_point, func)
       linear_func = calc_linear_function(grad, approx_point)


       linear_func_coefficients = [linear_func.coeff(x1), linear_func.coeff(x2)]


       # solve linear programming problem (simplex method)
       result_y = linprog(linear_func_coefficients, limitation_coefficients, plan_coefficients)['x']


       if np.array_equal(result_y, approx_point):
           return approx_point


       h = calc_h(result_y, approx_point)
       beta = min(1, find_beta(func, approx_point, h))


       # ==================
       f = func.subs([(x1, approx_point[0]), (x2, approx_point[1])])
       output_interim_results(iteration, approx_point[0], approx_point[1], beta, h, f)
       # ==================


       prev_approx_point = approx_point
       approx_point = calc_approximation_point(approx_point, beta, h)


       iteration += 1


   return approx_point


def valid_initial_approx_point(initial_approx_point, limitations, plan):
   approx_point_x1 = initial_approx_point[0]
   approx_point_x2 = initial_approx_point[1]


   if approx_point_x1 == 0 or approx_point_x2 == 0:
       return False


   i = 0
   for lim in limitations:
       calc = 0
       calc += lim[0] * approx_point_x1
       calc += lim[1] * approx_point_x2


       if calc > plan[i]:
           return False
       i += 1
   return True


if __name__ == '__main__':
   # func = input("Enter function: ")
   func = "x1**2 + x2**2 + 2*x1 + -8*x2"
   f = sympify(func)


   limitation = [
       [1, 2],
       [-1, 1]
   ]
   plan = [12, 8]


   input_ = input("Enter initial approximation points (separated by space): ")  # (4, 4)
   point = input_.split()
   initial_approx_point = (float(point[0]), float(point[1]))


   if valid_initial_approx_point(initial_approx_point, limitation, plan):
       optimal_x1, optimal_x2 = conditional_grad_method(f, initial_approx_point, limitation, plan)
       print(f"f(x) = ", f.subs([(x1, optimal_x1), (x2, optimal_x2)]))
   else:
       print("Initial approximation points must match limitations")

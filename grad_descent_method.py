from sympy import symbols, sympify, diff, solve
from math import sqrt


def convex(func):
    d2f_x1x1 = diff(func, x1, x1)
    d2f_x2x2 = diff(func, x2, x2)

    df_x1x2 = diff(func, x1, x2)
    df_x2x1 = diff(func, x2, x1)

    return d2f_x1x1 * d2f_x2x2 - df_x1x2 * df_x2x1 > 0 and d2f_x1x1 > 0  # може забрати останній енд


def calc_grad(approx_point, func):
    approx_point_x1 = approx_point[0]
    approx_point_x2 = approx_point[1]

    df_dx1 = diff(func, x1)
    df_dx2 = diff(func, x2)

    grad_in_approx_point = (df_dx1.subs([(x1, approx_point_x1), (x2, approx_point_x2)]),
                            df_dx2.subs([(x1, approx_point_x1), (x2, approx_point_x2)]))  # grad in x0, x1 etc
    return grad_in_approx_point


def find_beta(func, approx_point, grad):
    beta = symbols("beta")

    approx_point_x1 = approx_point[0]
    approx_point_x2 = approx_point[1]

    grad_x1 = grad[0]
    grad_x2 = grad[1]

    func_with_placed_x1_and_x2 = func.subs(
        [(x1, approx_point_x1 - grad_x1 * beta), (x2, approx_point_x2 - grad_x2 * beta)])
    derivative = diff(func_with_placed_x1_and_x2, beta)

    # прирівняємо похідну до нуля
    beta_value = solve(derivative, beta)[0]  # тому що повертає список можливих розвязків
    return beta_value.evalf()


def calc_approximation(approx_point, beta, grad):
    approx_point_x1 = approx_point[0]
    approx_point_x2 = approx_point[1]

    # grad in approx_point
    grad_x1 = grad[0]
    grad_x2 = grad[1]

    new_approx_point = (approx_point_x1 - beta * grad_x1, approx_point_x2 - beta * grad_x2)
    return new_approx_point


def main_condition(prev_approx_point, current_approx_point, eps):
    prev_approx_point_x1 = prev_approx_point[0]
    prev_approx_point_x2 = prev_approx_point[1]
    current_approx_point_x1 = current_approx_point[0]
    current_approx_point_x2 = current_approx_point[1]

    # x(k) - x(k-1)
    current_minus_prev_x1 = (current_approx_point_x1 - prev_approx_point_x1)
    current_minus_prev_x2 = (current_approx_point_x2 - prev_approx_point_x2)

    condition = sqrt(current_minus_prev_x1 ** 2 + current_minus_prev_x2 ** 2)

    return condition < eps


def output_interim_results(iteration, approx_point_x1, approx_point_x2, beta, grad, f):
    print(f"{iteration} iteration:")
    print(f"x{iteration} = ({approx_point_x1}; {approx_point_x2})")
    print(f"beta = {beta}")
    print(f"grad = ({grad[0]}, {grad[1]})")
    print(f"f = {f}\n")


def grad_descent_method(func, approx_point, eps):
    if not convex(func):
        print("This method cannot apply to non-convex functions!\n")
        return
    else:
        print("Convex function\n")

        iteration = 1
        approx_point_x1 = approx_point[0]
        approx_point_x2 = approx_point[1]
        while True:
            f = func.subs([(x1, approx_point_x1), (x2, approx_point_x2)])

            grad = calc_grad((approx_point_x1, approx_point_x2), func)

            if grad[0] == 0 and grad[1] == 0:
                output_interim_results(iteration, approx_point_x1, approx_point_x2, beta, grad, f)
                return f

            beta = find_beta(func, (approx_point_x1, approx_point_x2), grad)
            new_approx_point = calc_approximation((approx_point_x1, approx_point_x2), beta, grad)

            approx_point_x1 = new_approx_point[0]
            approx_point_x2 = new_approx_point[1]

            # ==================
            output_interim_results(iteration, approx_point_x1, approx_point_x2, beta, grad, f)
            # ==================

            if main_condition(approx_point, new_approx_point, eps):
                return f

            approx_point = new_approx_point
            iteration += 1


if __name__ == '__main__':
    func = input("Enter function: ")
    # x1**2 -2*x1*x2 + 6*x2**2 + x1 - x2
    x1, x2 = symbols('x1 x2')

    f = sympify(func)

    input_initial_approx_point = input("Enter initial approximation points (separated by space): ")     # (2, 1)
    point = input_initial_approx_point.split()
    initial_approx_point = (float(point[0]), float(point[1]))

    eps = float(input("Enter eps: "))

    grad_descent_method(f, initial_approx_point, eps)

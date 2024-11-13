def unbounded_knapsack(C, weights, profits):
    # Initialize an array to store maximum profit for each capacity
    P = [0] * (C + 1)
    
    # Iterate through all capacities from 1 to C
    for c in range(1, C + 1):
        # Iterate through each item
        for i in range(len(weights)):
            if weights[i] <= c:  # If item i can fit in the current capacity c
                P[c] = max(P[c], P[c - weights[i]] + profits[i])
    
    # Return the maximum profit for the full capacity C
    return P[C]

# Example usage
C = 10
weights = [2, 3, 5]
profits = [15, 20, 50]

print(unbounded_knapsack(C, weights, profits))
